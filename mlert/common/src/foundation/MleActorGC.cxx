/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MleActorGC.cxx
 * @ingroup MleFoundation
 *
 * @author Mark S. Millard
 * @date May 1, 2003
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2000-2018 Wizzer Works
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//  For information concerning this header file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END


#if defined(MLE_DEBUG)
    #define INLINE
#else
    #define INLINE inline
#endif


// Include standard header files.
#include <stddef.h>
#include <stdlib.h>
#ifdef __MWERKS__
#include <SIOUX.h>
#endif

// Include Magic Lantern header files.
#include "mle/mlAssert.h"
#include "mle/mlMalloc.h"
#include "mle/mlFileio.h"

#include "mle/MleActor.h"
#include "mle/MleActorGC.h"
#include "mle/MleOpcode.h"
#include "mle/MleRole.h"
#include "mle/MleSet.h"
#include "mle/MleTables.h"
#include "mle/MleLoad.h"
#include "mle/MleDirector.h"
#include "mle/mlArray.h"

#include "mle/ppinput.h"


/* Dynamically allocated array of actors. */
struct MleGCRegistry *g_actorRegistry;

// This is an array of set pointers that correspond to the actors loaded
//   in a group.  It is kept so that mlLoadGroup can set MleSet::currentSet
//   before calling the actor init() function.  It is allocated for each
//   group load in chunkInitialization() and deallocated in mlLoadGroup().
MleSet **_mleSetArray;

// This is an array of ints that flags whether an actor's role has
//   been attached.  Zero means that it has not.  This array is used
//   to attach roles not specifically given a parent to their set.
static int *g_attachmentArray;

#if defined(MLE_DEBUG)
/* end of registry */
struct MleGCRegistry* g_actorRegistryEnd;
#endif

struct MleGCRegistry* g_actorRegistryPtr;

#define DEBUG_PRINT
#ifdef DEBUG_PRINT
    #include <stdio.h>

    #define stdOutOpcode(a) printf("\n"#a" "); fflush(stdout)
    //#define stdOutIndex(a) printf("index=%ld ", a); fflush(stdout)
    #define stdOutIndex(a) printf("index=%d ", a); fflush(stdout)
    #define stdOutRawByte(a) printf("raw=x%02x ", a); fflush(stdout)
#else
    #define stdOutOpcode(a)
    #define stdOutIndex(a)
    #define stdOutRawByte(a)
#endif


void chunkInitialization(int sizeofRegistry)
{
    g_actorRegistry = new MleGCRegistry[sizeofRegistry];
    g_actorRegistryPtr = &g_actorRegistry[0];

    _mleSetArray = new MleSet*[sizeofRegistry];
    g_attachmentArray = new int[sizeofRegistry];
#if defined(MLE_DEBUG)
    g_actorRegistryEnd = &g_actorRegistry[sizeofRegistry];
#endif
}


static INLINE unsigned char
nextStreamByte(unsigned char *&streamPtrRef,
               unsigned char *streamStart,
               unsigned char *streamEnd)
{
    MLE_ASSERT(streamPtrRef >= streamStart && streamPtrRef < streamEnd);
    return *streamPtrRef++;
}


int readInt(unsigned char *&streamPtrRef,
            unsigned char *streamStart,
            unsigned char *streamEnd)
{
    int value = 0;
    unsigned char * v = (unsigned char *) &value;
    *v++ = nextStreamByte(streamPtrRef, streamStart, streamEnd);
    *v++ = nextStreamByte(streamPtrRef, streamStart, streamEnd);
    *v++ = nextStreamByte(streamPtrRef, streamStart, streamEnd);
    *v++ = nextStreamByte(streamPtrRef, streamStart, streamEnd);

    //fprintf(stdout, "MleActorGC: Read integer value %d\n", value);
    return value;
}

int
readIndex(unsigned char *&streamPtrRef,
          unsigned char *streamStart,
          unsigned char *streamEnd)
{
    int value = 0;
    signed char byte;
    
    while ((byte = nextStreamByte(streamPtrRef, streamStart, streamEnd)) >= 0)
    {
        MLE_ASSERT((value & (signed int) 0x80000000 >> valueBits - 1) == 0);
        value = (value << valueBits) | byte;
    }
    stdOutIndex(value);
    MLE_ASSERT(value >= 0);
    streamPtrRef--; /* Back up one, since while loop always over-advances by one. */
    //fprintf(stdout, "MleActorGC: backing up stream 1 byte.\n");

    //fprintf(stdout, "MleActorGC: Read index value %d\n", value);
    return value;
}


static void registerActor(MleActor* actor, int index)
{
#if defined(MLE_DEBUG)
    MLE_ASSERT(g_actorRegistryPtr < g_actorRegistryEnd);
#endif
    g_actorRegistryPtr->m_actor = actor;
    g_actorRegistryPtr->m_index = index;
    g_actorRegistryPtr++;
}


static INLINE MleActor* createActor(int index)
{
    MleActor* newActor;
    
    MLE_ASSERT(index >= 0 && index < mlRTActorClassLength);
    newActor = (*mlRTActorClass[index].constructor)();
    registerActor(newActor, index); /* use the index of the actor for future reference */
    return newActor;
}


#if defined(MLE_DEBUG)
static INLINE int getCurrentActorSize(void)
{
    return mlRTActorClass[g_actorRegistryPtr[-1].m_index].size;
}
#endif /* MLE_DEBUG */


static INLINE MleRole* createRole(MleActor* currentActor, int index)
{
    MleRole* newRole;
    
    MLE_ASSERT(index >= 0 && index < mlRTRoleClassLength);
    newRole = (*mlRTRoleClass[index].constructor)(currentActor);
    g_actorRegistryPtr[-1].m_role = newRole;
    return newRole;
}


void parseStream(unsigned char *&streamPtrRef,
                 unsigned char *streamStart,
                 unsigned char *streamEnd,
                 MleChunkType chunkType)
{
    //int propertyOffset = -1;
	MleRTPropertyEntry *propertyEntry;
    int propertyLength = 0;
    MleActor* currentActor = NULL;
    MleRole* currentRole = NULL;
    MleSet* currentSet = NULL;
    int index;
    
    int actorIndex = -1;    // index of actor within the group

    // opcode arguments.
    int setIndex = -1;
    int parentActorIndex = -1;
    int childActorIndex = -1;

    do {
        switch (nextStreamByte(streamPtrRef, streamStart, streamEnd))
        {
// The ActorSet functionality has been superceded.  Sets are now
//   specified in role bindings.
//      case beginActorSetOpcode:
//        if (currentActor) {
//            currentActor = NULL;
//            currentRole = NULL;
//        }
//        stdOutOpcode(beginActorSetOpcode);
//        index = readIndex(streamPtrRef, streamStart, streamEnd);
//        currentSet = mlLoadSet(index);
//        MleSet::currentSet = currentSet;
//        break;

          case createActorOpcode:
            // Increment the actor count.
            actorIndex++;

            stdOutOpcode(createActorOpcode);
            index = readIndex(streamPtrRef, streamStart, streamEnd);
            currentActor = createActor(index);

            // Initialize state that the actor may use.
            _mleSetArray[actorIndex] = NULL;
            g_attachmentArray[actorIndex] = 0;
            currentRole = NULL;
            setIndex = -1;
            parentActorIndex = -1;
            break;

          case setSetOpcode:
            stdOutOpcode(setSetOpcode);
            index = readIndex(streamPtrRef, streamStart, streamEnd);

            setIndex = index;
            break;

          case bindRoleOpcode:
            stdOutOpcode(bindRoleOpcode);

            MLE_ASSERT(currentActor != NULL);
            MLE_ASSERT(setIndex >= 0);

            // Read the arguments, role class index and set index.
            index = readIndex(streamPtrRef, streamStart, streamEnd);

            // Set the current set, creating it if necessary
            //   mlLoadSet() just returns the pointer if it has already
            //   been created.  We also remember the set as being used
            //   for this actor.
            MleSet::g_currentSet =
                _mleSetArray[actorIndex] =
            mlLoadSet(setIndex);

            // Now create the role.
            currentRole = createRole(currentActor, index);
            break;

          case parentRoleOpcode:
            stdOutOpcode(parentRoleOpcode);
            index = readIndex(streamPtrRef, streamStart, streamEnd);

            parentActorIndex = index;
            break;

          case childRoleOpcode:
            stdOutOpcode(parentRoleOpcode);
            MLE_ASSERT(currentSet != NULL);

            // Read child actor index.
            childActorIndex = readIndex(streamPtrRef, streamStart, streamEnd);

            // Verify that the role has not already been attached.
            MLE_ASSERT(g_attachmentArray[childActorIndex] != 0);

            // Verify that the sets for these actors match.
            MLE_ASSERT(_mleSetArray[parentActorIndex] == _mleSetArray[childActorIndex]);

            // And do the attachment.
            _mleSetArray[parentActorIndex]->attach(
            g_actorRegistry[parentActorIndex].m_actor->getRole(),
            g_actorRegistry[childActorIndex].m_actor->getRole());

            // Mark the actor/role as having been attached.
            g_attachmentArray[childActorIndex] = 1;
            parentActorIndex = -1;
            childActorIndex = -1;
            break;

          case setPropertyOffsetOpcode:
            stdOutOpcode(setPropertyOffsetOpcode);
            index = readIndex(streamPtrRef, streamStart, streamEnd);
            if (chunkType == MLE_CHUNK_GROUP)
            {
                MLE_ASSERT(index >= 0 && index < mlRTActorPropertyLength);
                //propertyOffset = mlRTActorProperties[index].offset;
                propertyEntry = &mlRTActorProperties[index];
            }
            else if (chunkType == MLE_CHUNK_SET)
            {
                MLE_ASSERT(index >= 0 && index < mlRTSetPropertyLength);
                //propertyOffset = mlRTSetProperties[index].offset;
                propertyEntry = &mlRTSetProperties[index];
            }
            break;

          case setPropertyLengthOpcode:
            stdOutOpcode(setPropertyLengthOpcode);
            propertyLength = readIndex(streamPtrRef, streamStart, streamEnd);
            break;

          case copyPropertyOpcode:
            stdOutOpcode(copyPropertyOpcode);
            {
                int bytesToCopy = propertyLength;
            	unsigned char *propertyValue = (unsigned char *)mlMalloc(bytesToCopy);

#if defined(MLE_DEBUG)
            	// TBD - fix this now that offsetof is no longer being used.
                if (chunkType == MLE_CHUNK_GROUP)
                {
                    MLE_ASSERT(currentActor != NULL && getCurrentActorSize() > 0);
                    MLE_ASSERT(propertyEntry != NULL);
                    MLE_ASSERT(propertyLength > 0 && propertyLength <= getCurrentActorSize());
                }
#endif
                unsigned char *propertyPtr = propertyValue;
                while (--bytesToCopy >= 0)
                {
                    stdOutRawByte(*streamPtrRef);
                    *propertyPtr++ = nextStreamByte(streamPtrRef, streamStart,
                                                    streamEnd);
                }

                if(chunkType == MLE_CHUNK_GROUP)
                    //propertyPtr = &((unsigned char*) currentActor)[propertyOffset];
                	propertyEntry->setProperty(currentActor, propertyEntry->name, propertyValue);
                else if(chunkType == MLE_CHUNK_SET)
                    //propertyPtr = &((unsigned char*) currentSet)[propertyOffset];
                    propertyEntry->setProperty(currentSet, propertyEntry->name, propertyValue);

                // Free temporary storage space.
                mlFree(propertyValue);
            }
            break;

          case copyDynamicPropertyOpcode:
            {
                int bytesToCopy = readInt(streamPtrRef, streamStart, streamEnd);
                unsigned char *propertyValue = (unsigned char *)mlMalloc(bytesToCopy);

#if defined(MLE_DEBUG)
                if (chunkType == MLE_CHUNK_GROUP)
                {
                    MLE_ASSERT((currentActor != NULL) &&  (getCurrentActorSize() > 0));
                    MLE_ASSERT(propertyEntry != NULL);
                }
#endif
                unsigned char *propertyPtr = propertyValue;
                while (--bytesToCopy >= 0)
                {
                    stdOutRawByte(*streamPtrRef);
                    *propertyPtr++ = nextStreamByte(streamPtrRef, streamStart,
                                                    streamEnd);
                }

                if(chunkType == MLE_CHUNK_GROUP)
                    //propertyPtr = &((unsigned char**) currentActor)[propertyOffset];
        	        propertyEntry->setProperty(currentActor, propertyEntry->name, propertyValue);
                else if(chunkType == MLE_CHUNK_SET)
                    //propertyPtr = &((unsigned char**) currentSet)[propertyOffset];
                    propertyEntry->setProperty(currentSet, propertyEntry->name, propertyValue);

                // Do NOT free temporary storage space, since it is not temporary!
                //mlFree(propertyValue);
            }
            break;

          case copyArrayPropertyOpcode:
            {
                //MleArray<char> * propertyPtr = (MleArray<char> *)&((char *) currentActor)[propertyOffset];
                MleArray<char> propertyValue;

#if defined(MLE_DEBUG)
                MLE_ASSERT(currentActor != NULL &&  getCurrentActorSize() > 0);
                MLE_ASSERT(propertyEntry != NULL);
#endif
        
                int numElements = readInt(streamPtrRef, streamStart, streamEnd);
                int bytesToCopy = numElements * propertyLength;
                char * buf = new char[bytesToCopy];
                MleArray<char> charArray(numElements,buf);
                while (--bytesToCopy >= 0)
                {
                    stdOutRawByte(*streamPtrRef);
                    *buf++ = nextStreamByte(streamPtrRef, streamStart, streamEnd);
                }
                propertyValue = charArray;

                if(chunkType == MLE_CHUNK_GROUP)
        	        propertyEntry->setProperty(currentActor, propertyEntry->name, (unsigned char *)&propertyValue);
                else if(chunkType == MLE_CHUNK_SET)
                    propertyEntry->setProperty(currentSet, propertyEntry->name, (unsigned char *)&propertyValue);

                MleArray<char> nulArray;
                charArray = nulArray;
            }
            break;

          case endActorGroupOpcode:
            stdOutOpcode(endActorGroupOpcode);
            streamPtrRef = streamEnd;
            break;

          case copyMediaRefOpcode:
            {
                stdOutOpcode(copyMediaRefOpcode);
                //*(int*) &((char*) currentActor)[propertyOffset] = readIndex(streamPtrRef, streamStart, streamEnd);
                int mrefIndex = readIndex(streamPtrRef, streamStart, streamEnd);
                propertyEntry->setProperty(currentActor, propertyEntry->name, (unsigned char *)&mrefIndex);
            }
            break;

          case createSetOpcode:
            stdOutOpcode(createSetOpcode);
            index = readIndex(streamPtrRef, streamStart, streamEnd);
            currentSet = (*mlRTSet[index].constructor)();
            mlRTSet[index].ptr = currentSet;
            MleSet::g_currentSet = currentSet;
            break;

          case endSetOpcode:
            stdOutOpcode(endSetOpcode);
            break;

          default:
            MLE_ASSERT(0);
        }
    } while (streamPtrRef < streamEnd);

    // Now finish up by doing leftover role attachments.
    if ( chunkType == MLE_CHUNK_GROUP )
    {
        // Increment actor counter (it started at -1).
        actorIndex++;

        // Loop through all the actors.
        for ( int i = 0; i < actorIndex; i++ )
        {
            // See if the role has been attached.
            if ( g_attachmentArray[i] == 0 &&
                g_actorRegistry[i].m_actor->getRole() )
            {
                // If not, put it on the set.
                _mleSetArray[i]->attach(
                    NULL,g_actorRegistry[i].m_actor->getRole());
            }
        }

        // Have to be careful here - can't move this outside the if
        //   clause because parseStream may be called recursively to
        //   load sets.
        delete g_attachmentArray;
        g_attachmentArray = NULL;
    }
}

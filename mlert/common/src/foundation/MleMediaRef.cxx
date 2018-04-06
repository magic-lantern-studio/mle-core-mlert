/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MleMediaRef.cxx
 * @ingroup MleFoundation
 *
 * @author Mark S. Millard
 * @date May 1, 2003
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015 Wizzer Works
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
//  For information concerning this source file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

// Include system header files.
#include <stdio.h>
#include <string.h>

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"
#include "mle/mlAssert.h"

// Include Magic Lantern Runtime Player header files.
#include "mle/MleMediaRefConverter.h"
#include "mle/MleMediaRef.h"

#ifdef MLE_REHEARSAL
#include "mle/DwpMediaRef.h"
#include "mle/mlExpandFilename.h"
#else
#include "mle/chunk.h"
#include "mle/ppinput.h"
#endif /* MLE_REHEARSAL */


MleMediaRef::MleMediaRef(void)
    :m_references(NULL),
     m_numReferences(0)
{
    m_converter = new MleMediaRefConverter();
}


MleMediaRef::~MleMediaRef(void)
{
    // Unload cached media data from DWP/Playprint.
    unload();

    // Do what all destructors must do, which is to delete the
    // reference converter.  
    deleteConverter();
}

void *
MleMediaRef::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleMediaRef::operator delete(void *p)
{
	mlFree(p);
}

#ifdef MLE_REHEARSAL
const char *MleMediaRef::g_rehearsal = "rehearsal";
MleDwpStrKeyDict MleMediaRef::g_registry(32);  // Argument is hash table size.


const char *
MleMediaRef::getTypeName(void) const
{
    return "MleMediaRef";
}


int
MleMediaRef::isa(const char *type) const
{
    return !strcmp(type,"MleMediaRef");
}
#endif /* MLE_REHEARSAL */


// Routines to manipulate the reference converter.  
// It is the user\'s responsibility to delete the old reference 
// converter when setting a new one.
void
MleMediaRef::setConverter(MleMediaRefConverter *converter)
{
    m_converter = converter;
}


MleMediaRefConverter *
MleMediaRef::getConverter(void)
{
    return m_converter;
}


void
MleMediaRef::deleteConverter(void)
{
    // Might like to make this use mleFree, but that will 
    // depend on overriding new and delete.
    if (m_converter)
	{
        delete m_converter;
    }
}

#ifndef MLE_REHEARSAL
static int getMediaRefCB(void *clientData,void *callData)
{
    MleMediaRefChunk *chunkData = (MleMediaRefChunk *)callData;

    ((MleMediaRefChunk *)clientData)->m_data = (char *)mlMalloc(chunkData->m_size);
    memcpy(((MleMediaRefChunk *)clientData)->m_data,chunkData->m_data,
           chunkData->m_size);
    ((MleMediaRefChunk *)clientData)->m_size = chunkData->m_size;
    ((MleMediaRefChunk *)clientData)->m_flags = chunkData->m_flags;
    return(0);
}
#endif /* ! MLE_REHEARSAL */

// Cache media data from the DWP/Playprint.
MlBoolean
MleMediaRef::load(unsigned long numEntries,void *usrData)
{
    // Declare local variables
    MleMediaLoadReference *newReference, *nextReference;
    int status = TRUE;

#ifdef MLE_REHEARSAL
    MleDwpMediaRefTargetMedia *entries = (MleDwpMediaRefTargetMedia *)usrData;

    for (unsigned int i = 0; i < numEntries; i++)
	{
        // Allocate a new reference.
        newReference = (MleMediaLoadReference *)mlMalloc(
            sizeof(MleMediaLoadReference));
        newReference->flags = entries[0].m_flags;
        const char *filename = entries[0].m_filename;
        newReference->buffer = mlFilenameExpand(filename);
        newReference->bufsiz = strlen(filename);

        // Attach next reference.
        nextReference = m_references;
        if (! nextReference) {
            // First entry on the list.
            m_references = newReference;
        } else {
            // Add entry to end of list.
            while (nextReference->next != NULL)
                nextReference = nextReference->next;
            nextReference->next = newReference;
        }
        m_numReferences++;
    }

#else /* !MLE_REHEARSAL */

    MleMediaRefChunk mrefData;
    MleDppInput* pp = (MleDppInput *)usrData;

    for (unsigned int i = 0; i < numEntries; i++)
	{
        // Allocate a new reference.
        newReference = (MleMediaLoadReference *) mlMalloc(
            sizeof(MleMediaLoadReference));
    
        pp->readMedia(getMediaRefCB,&mrefData);
        newReference->flags = mrefData.m_flags;
        newReference->bufsiz = mrefData.m_size;
        newReference->buffer = mrefData.m_data;

        // Attach next reference.
        nextReference = m_references;
        if (! nextReference)
		{
            // First entry on the list.
            m_references = newReference;
        } else
		{
            // Add entry to end of list.
            while (nextReference->next != NULL)
                nextReference = nextReference->next;
            nextReference->next = newReference;
        }
        m_numReferences++;
    }
#endif /* MLE_REHEARSAL */

    return(status);
}


// Un-cache media data from the DPW/Playprint.
void
MleMediaRef::unload(void)
{
    // Declare local variables.
    MleMediaLoadReference *nextReference, *tmp;

    nextReference = m_references;
    for (unsigned int i = 0; i < m_numReferences; i++)
	{
        tmp = nextReference->next;
        mlFree(nextReference->buffer);
        mlFree(nextReference);
        nextReference = tmp;
    }
}


MleMediaLoadReference *MleMediaRef::getNextMediaRef(MleMediaLoadReference* loadReference)
{
    if (loadReference)
        return(loadReference->next);
    else
        return(m_references);
}


unsigned long MleMediaRef::getMediaRefFlags(MleMediaLoadReference* loadReference)
{
    MLE_ASSERT(loadReference)
    return(loadReference->flags);
}


unsigned long MleMediaRef::getMediaRefBufsiz(MleMediaLoadReference* loadReference)
{
    MLE_ASSERT(loadReference)
    return(loadReference->bufsiz);
}


char *MleMediaRef::getMediaRefBuffer(MleMediaLoadReference* loadReference)
{
    MLE_ASSERT(loadReference)
    return(loadReference->buffer);
}

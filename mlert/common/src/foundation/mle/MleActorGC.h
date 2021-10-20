/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MleActorGC.h
 * @ingroup MleFoundation
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2000-2021 Wizzer Works
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

#ifndef __MLE_ACTORGC_H_
#define __MLE_ACTORGC_H_

// Include Magic Lantern header files.
#include "mle/MleActor.h"


struct MleGCRegistry
{
    MleActor* m_actor;
    MleRole*  m_role;
    int m_index;
};


/**
 * This is an enum used in the runtime opcode parser.  This really should
 * be shared with the targeting library's MleChunkTable.h, there is a
 * ChunkEntryType with the same content.  We decided to keep them separate
 * for now because
 * <ol>
 *   <li>MleActorGC.h cannot include MleChunkTable.h</li>
 *   <li>MleChunkTable.h can include MleActorGC.h but it will make
 *       targeting library additional dependencies on the player library.</li>
 *</ol>
 */
typedef enum
{
    MLE_CHUNK_MEDIA,
    MLE_CHUNK_GROUP,
    MLE_CHUNK_SET
} MleChunkType;

extern unsigned char* g_streamEnd;

extern struct MleGCRegistry* g_actorRegistry;

extern struct MleGCRegistry* g_actorRegistryPtr;

#if defined(MLE_DEBUG)
/**
 * The end of registry.
 */
extern struct MleGCRegistry* g_actorRegistryEnd;
#endif /* MLE_DEBUG */

extern void chunkInitialization(int sizeofRegistry);

extern int readIndex(unsigned char *&streamPtrRef, unsigned char *streamStart,
  unsigned char *streamEnd);

extern int readInt(unsigned char *&streamPtrRef, unsigned char *streamStart,
  unsigned char *streamEnd);

extern void parseStream(unsigned char *&streamPtrRef,
  unsigned char *streamStart, unsigned char *streamEnd,
  MleChunkType chunkType);

#endif /* __MLE_ACTORGC_H_ */

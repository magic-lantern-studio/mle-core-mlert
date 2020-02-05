/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file 3danim.h
 * @ingroup MleFoundation
 *
 * @author Mark S. Millard
 * @date May 1, 2003
 *
 * This file defines 3D animation registry structures.
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015-2018 Wizzer Works
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

#ifndef __MLE_3DANIM_H_
#define __MLE_3DANIM_H_


#ifdef psx
#define FILE int
#endif

#if defined(MLE_DEBUG)
#include <stdio.h>
#endif 

#include "mle/mlTypes.h"
#include "math/transfrm.h"


#ifndef  MLE_UNKNOWN_LENGTH
#define  MLE_UNKNOWN_LENGTH 1
#endif //MLE_UNKNOWN_LENGTH

#ifndef  MLE_NAME_LENGTH 
#define  MLE_NAME_LENGTH 32
#endif //MLE_NAME_LENGTH 

#define MLE_3D_ANIM_MAGIC                (char *)"MLEa1"
#define MLE_3D_ANIM_MAGIC_LITTLE_ENDIAN  (char *)"MLEa2"
#define MLE_3D_ANIM_MAGIC_FIXED_POINT    (char *)"MLEa3"
#define MLE_3D_ANIM_MAGIC_LITTLE_FIXED   (char *)"MLEa4"


/*
 * @brief 3D Animation Sequence. 
 */
class Mle3dSequence
{ 
  public:

    char                   m_name[MLE_NAME_LENGTH];

    unsigned long          m_parent;

    unsigned short         m_startFrame;

    unsigned short         m_type;

    MlTransform            m_frame[MLE_UNKNOWN_LENGTH];

#if defined(MLE_DEBUG)
    static void            print(Mle3dSequence* sequence, FILE *out = NULL);

    static void            printTransform(MlTransform t, FILE* out = NULL);
#endif
};


/**
 * @brief 3D Animation
 */
class Mle3dAnimation
{ 
  public:

    char                   m_name[MLE_NAME_LENGTH];

    unsigned long          m_numSequences;

    unsigned long          m_numFrames;

    // An array of Mle3dSequences follows next, indexed by computing sizes.

    static Mle3dSequence*   getSequence(Mle3dAnimation* animation, unsigned int index);

    static Mle3dSequence*   getSequence(Mle3dAnimation* animation, char* nameSearch);

    static Mle3dSequence**  getSequenceList(Mle3dAnimation* animation);

#if defined(MLE_DEBUG)
    static void            print(Mle3dAnimation* animation, FILE* out = NULL);
#endif
};


/**
 * @brief 3D Animation Registry
 */
class Mle3dAnimationRegistry
{ 
  public:

    unsigned long          m_magicNumber;

    char                   m_name[MLE_NAME_LENGTH];

    unsigned long          m_numAnimations;

    unsigned long          m_animationOffset[MLE_UNKNOWN_LENGTH];

    // An array of Mle3dAnimations follows next, indexed by the animationOffsets

    static Mle3dAnimation*  getAnimation(Mle3dAnimationRegistry* registry, unsigned int index);

    static Mle3dAnimation*  getAnimation(Mle3dAnimationRegistry* registry, char* nameSearch);

    static Mle3dAnimationRegistry* read(char* filename);

#if defined(MLE_DEBUG)
    static void            print(Mle3dAnimationRegistry* registry, FILE *out = NULL);
#endif
};

#endif /* __MLE_3DANIM_H_ */

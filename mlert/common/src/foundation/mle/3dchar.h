/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file 3dcar.h
 * @ingroup MleFoundation
 *
 * This file defines 3D character registry structures.
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015-2021 Wizzer Works
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

#ifndef __MLE_3DCHAR_H_
#define __MLE_3DCHAR_H_

#ifdef psx
#define FILE int
#endif

#if defined(MLE_DEBUG)
#include <stdio.h>
#endif 

#include "mle/mlTypes.h"


#ifndef  MLE_UNKNOWN_LENGTH
#define  MLE_UNKNOWN_LENGTH 1
#endif //MLE_UNKNOWN_LENGTH

#ifndef MLE_NAME_LENGTH
#define MLE_NAME_LENGTH 32
#endif

#define MLE_3D_CHAR_MAGIC "FWc1"


class Mle3dTransition
{ 
  public:

    char                   m_name[MLE_NAME_LENGTH];

    char                   m_event[MLE_NAME_LENGTH];

    int                    m_targetIndex;

    char                   m_targetName[MLE_NAME_LENGTH];

    unsigned short         m_fromFrame;

    unsigned short         m_toFrame;

#if defined(MLE_DEBUG)
    static void            print(Mle3dTransition* transition, FILE *out=NULL);
#endif
};


class Mle3dSnippet
{ 
  public:

    char                   m_name[MLE_NAME_LENGTH];

    unsigned short         m_startFrame;

    unsigned short         m_endFrame;

    unsigned long          m_numTransitions;

    Mle3dTransition**      m_transition;

    // An array of Mle3dTransitions follows next, indexed by computing sizes.

    static Mle3dTransition* getTransition(Mle3dSnippet* snippet, unsigned int index);

    static Mle3dTransition* getTransition(Mle3dSnippet* snippet, char* nameSearch);

    static Mle3dTransition** getTransitionList(Mle3dSnippet* snippet);

#if defined(MLE_DEBUG)
    static void            print(Mle3dSnippet* snippet, FILE* out=NULL);
#endif
};


class Mle3dCharacter
{ 
  public:

    char                   m_name[MLE_NAME_LENGTH];

    unsigned long          m_numSnippets;

    Mle3dSnippet**         m_snippet;

    unsigned long          m_snippetOffset[MLE_UNKNOWN_LENGTH];

    // An array of Mle3dSnippets follows next, indexed by computing sizes.

    static Mle3dSnippet*   getSnippet(Mle3dCharacter* character, unsigned int index);

    static Mle3dSnippet*   getSnippet(Mle3dCharacter* character, char* nameSearch);

    static Mle3dSnippet**  getSnippetList(Mle3dCharacter* character);

#if defined(MLE_DEBUG)
    static void            print(Mle3dCharacter* character, FILE* out=NULL);
#endif
};


class Mle3dCharacterRegistry
{ 
  public:

    unsigned long          m_magicNumber;

    char                   m_name[MLE_NAME_LENGTH];

    unsigned long          m_numCharacters;

    Mle3dCharacter**       m_character;

    unsigned long          m_characterOffset[MLE_UNKNOWN_LENGTH];

    // An array of Mle3dCharacters follows next, indexed by the characterOffsets.

    static Mle3dCharacter*  getCharacter(Mle3dCharacterRegistry* registry, unsigned int index);

    static Mle3dCharacter*  getCharacter(Mle3dCharacterRegistry* registry, char* nameSearch);

    static Mle3dCharacterRegistry* read(char* filename);

#if defined(MLE_DEBUG)
    static void            print(Mle3dCharacterRegistry* registry, FILE *out=NULL);
#endif
};


#endif /* __MLE_3DCHAR_H_ */

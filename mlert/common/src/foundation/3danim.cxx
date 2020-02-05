/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file 3danim.cxx
 * @ingroup MleFoundation
 *
 * @author Mark S. Millard
 * @date May 1, 2003
 *
 * This file implements 3D animation registry structures.
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
//  For information concerning this source code, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

// Include system header files.
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <sys/stat.h>

#ifdef WIN32
#include <io.h>
#else /* ! WIN32 */
#include <unistd.h>
#endif 

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"
#include "mle/mlFileio.h"
#include "mle/3danim.h"


Mle3dSequence* Mle3dAnimation::getSequence(Mle3dAnimation* animation, unsigned int index)
{
    if (animation)
        return((Mle3dSequence*)(((char*)animation)
            +MLE_NAME_LENGTH
            +sizeof(animation->m_numSequences)
            +sizeof(animation->m_numFrames)
            +index*(sizeof(Mle3dSequence)
            +(animation->m_numFrames-1)*sizeof(MlTransform))));
    else
        return NULL;

}


Mle3dSequence* Mle3dAnimation::getSequence(Mle3dAnimation* animation, char* nameSearch)
{
    Mle3dSequence* result = NULL;
    Mle3dSequence* temp;

    if (animation && nameSearch && strlen(nameSearch))
        for (unsigned int i = 0; i < animation->m_numSequences; i++)
        { 
            temp = Mle3dAnimation::getSequence(animation,i);
            if (temp && !strcmp(temp->m_name,nameSearch))
            {
				result = temp;
                i = animation->m_numSequences;
            }
        }

    return result;
}


Mle3dSequence** Mle3dAnimation::getSequenceList(Mle3dAnimation* animation)
{
    Mle3dSequence** sequenceList = NULL;
  
    if (animation)
    {
		sequenceList = new Mle3dSequence*[animation->m_numSequences];
        for (unsigned int i = 0; i < animation->m_numSequences; i++)
            sequenceList[i] = Mle3dAnimation::getSequence(animation,i);
    }
    return sequenceList;
}


Mle3dAnimation* Mle3dAnimationRegistry::getAnimation(Mle3dAnimationRegistry* registry, 
    unsigned int index)
{
    if (registry && index<registry->m_numAnimations)
        return((Mle3dAnimation*)(((char*)registry)+registry->m_animationOffset[index]));
    else 
        return NULL;
};


Mle3dAnimation* Mle3dAnimationRegistry::getAnimation(Mle3dAnimationRegistry* registry, 
                                                   char* nameSearch)
{
    Mle3dAnimation* result = NULL;
    Mle3dAnimation* temp;
    
    for (unsigned int i = 0; i < registry->m_numAnimations; i++)
	{
	    temp = Mle3dAnimationRegistry::getAnimation(registry,i);
        if (strcmp(temp->m_name,nameSearch) == 0)
		{
	        result = temp;
	        break;
		}
    }
    
    return result;
}


Mle3dAnimationRegistry* Mle3dAnimationRegistry::read(char* filename)
{
	Mle3dAnimationRegistry* registry = NULL;
  
    if (filename)
	{
        unsigned int filesize = 0;
        int statcheck = mlOpen(filename,O_RDONLY);
        if (statcheck >= 0)
		{
	        struct stat status;
	        fstat(statcheck,&status);
	        filesize = status.st_size;
	        mlClose(statcheck);
	  
			if (filesize)
			{
				FILE* in = mlFOpen(filename,"rb");
				if (in != NULL)
				{
					registry = (Mle3dAnimationRegistry*)mlMalloc(filesize);
					if (mlFRead(registry,filesize,1,in) == 0)
					{
						registry = NULL;
					}
					mlFClose(in);
			  
					if (registry)
					{
						// First-pass check to make sure file has magic number for animation registry.
	#ifdef WIN32
						char* magic = MLE_3D_ANIM_MAGIC_LITTLE_ENDIAN;
	#else
						char* magic = MLE_3D_ANIM_MAGIC;
	#endif
						unsigned long* check = (unsigned long*)magic;
						if (*check != registry->m_magicNumber)
						{
							fprintf(stderr,"%s%s%s","Mle3dAnimationRegistry::Error--file ",filename,
								" is not a Magic Lantern animation registry.\n");
							delete [] registry;
							registry = NULL;
						}
					}
			  
					if (registry)
					{
						// Second-pass check to see if animation information in file is possibly invalid.
						unsigned int check = 0;
						for (unsigned int i = 0; i < registry->m_numAnimations; i++)
							if (registry->m_animationOffset[i] > check)
								check = registry->m_animationOffset[i];
						if (check > filesize)
						{
							delete [] registry;
							registry = NULL;
						}
					}
				}
			}
	  
	  
	        if (! registry)
	            fprintf(stderr,"Mle3dAnimationRegistry::Error--Could not load animation from file %s.\n",filename);
		}
        else
	        fprintf(stderr,"Mle3dAnimationRegistry::Error--Could not find animation registry file %s.\n",filename);
	}
    else
        fprintf(stderr,"Mle3dAnimationRegistry::Error--No filename given for animation registry.\n");
  
    return registry;
}


#if defined(MLE_DEBUG)

void Mle3dSequence::printTransform(MlTransform t, FILE* out)
{
    if (!out) out = stdout;
    fprintf(out,"\t\t%10.4f %10.4f %10.4f\n", t[0][0], t[0][1], t[0][2]);
    fprintf(out,"\t\t%10.4f %10.4f %10.4f\n", t[1][0], t[1][1], t[1][2]);
    fprintf(out,"\t\t%10.4f %10.4f %10.4f\n", t[2][0], t[2][1], t[2][2]);
    fprintf(out,"\t\t%10.4f %10.4f %10.4f\n", t[3][0], t[3][1], t[3][2]);
}


void Mle3dSequence::print(Mle3dSequence* sequence, FILE* out)
{
    if (!out) out = stdout;
    if (sequence)
    { 
	    fprintf(out,"Name=%s\n",sequence->m_name);
	    fprintf(out,"            parent=%d\n",sequence->m_parent);
	    fprintf(out,"            startFrame=%d\n",sequence->m_startFrame);
	    fprintf(out,"            type=%d\n",sequence->m_type);
	    fprintf(out,"            First Transform:\n");
        Mle3dSequence::printTransform(sequence->m_frame[0],out);
        fprintf(out,"==\n");
        Mle3dSequence::printTransform(sequence->m_frame[1],out);
        fprintf(out,"==\n");
	}
}


void Mle3dAnimation::print(Mle3dAnimation* animation, FILE* out)
{ 
    if (!out) out = stdout;
    if (animation)
	{
        fprintf(out,"Animation name=%s\n",animation->m_name);
        fprintf(out,"Number of Sequences: %d\n",animation->m_numSequences);
        fprintf(out,"Number of Frames: %d\n",animation->m_numFrames);
     
        Mle3dSequence* sequence;
        for (unsigned int i=0;i<animation->m_numSequences;i++)
		{ 
            sequence = Mle3dAnimation::getSequence(animation,i);
            fprintf(out,"Sequence %d:\n",i);
            Mle3dSequence::print(sequence,out);
        }
	}
}


void Mle3dAnimationRegistry::print(Mle3dAnimationRegistry* registry, FILE* out)
{
    if (!out) out = stdout;
    if (registry)
    {
		fprintf(out,"Registry name=%s\n",registry->m_name);
        fprintf(out,"Number of Animations=%d\n",registry->m_numAnimations);
        for (unsigned int i = 0; i < registry->m_numAnimations; i++)
            Mle3dAnimation::print(Mle3dAnimationRegistry::getAnimation(registry,i),out);
	}
}

#endif /* MLE_DEBUG */

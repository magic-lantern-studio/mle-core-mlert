/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file 3dchar.cxx
 * @ingroup MleFoundation
 *
 * This file implements 3D character registry structures.
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015-2024 Wizzer Works
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

#include "mle/3dchar.h"


Mle3dTransition* Mle3dSnippet::getTransition(Mle3dSnippet* snippet, unsigned int index)
{
    if (snippet)
        return((Mle3dTransition*)(((char*)snippet)
            + MLE_NAME_LENGTH
            + sizeof(snippet->m_startFrame)
            + sizeof(snippet->m_endFrame)
            + sizeof(snippet->m_numTransitions)
            + index*(sizeof(Mle3dTransition))));
    else
        return NULL;
}


Mle3dTransition* Mle3dSnippet::getTransition(Mle3dSnippet* snippet, char* nameSearch)
{
    Mle3dTransition* result = NULL;
    Mle3dTransition* temp;

    if (snippet && nameSearch && strlen(nameSearch))
       for (unsigned int i = 0; i < snippet->m_numTransitions; i++)
	   { 
           temp = Mle3dSnippet::getTransition(snippet,i);
           if (temp && !strcmp(temp->m_name,nameSearch))
           {
			   result = temp;
               i = snippet->m_numTransitions;
           }
       }

    return result;
}


Mle3dTransition** Mle3dSnippet::getTransitionList(Mle3dSnippet* snippet)
{
    Mle3dTransition** transitionList=NULL;
  
    if (snippet)
    {
		transitionList=new Mle3dTransition*[snippet->m_numTransitions];
        for (unsigned int i = 0; i < snippet->m_numTransitions; i++)
            transitionList[i] = Mle3dSnippet::getTransition(snippet,i);
    }

    return transitionList;
}


Mle3dSnippet* Mle3dCharacter::getSnippet(Mle3dCharacter* character, unsigned int index)
{
    if (character && index<character->m_numSnippets)
        return((Mle3dSnippet*)(((char*)character) + character->m_snippetOffset[index]));
    else 
        return NULL;
};


Mle3dSnippet* Mle3dCharacter::getSnippet(Mle3dCharacter* character, char* nameSearch)
{
    Mle3dSnippet* result = NULL;
    Mle3dSnippet* temp;

    if (character && nameSearch && strlen(nameSearch))
        for (unsigned int i = 0; i < character->m_numSnippets; i++)
        { 
            temp = Mle3dCharacter::getSnippet(character,i);
            if (temp && !strcmp(temp->m_name,nameSearch))
            {
				result = temp;
                i = character->m_numSnippets;
            }
		}

    return result;
}


Mle3dSnippet** Mle3dCharacter::getSnippetList(Mle3dCharacter* character)
{
    Mle3dSnippet** snippetList = NULL;
  
    if (character)
    {
		snippetList = new Mle3dSnippet*[character->m_numSnippets];
        for (unsigned int i = 0; i < character->m_numSnippets; i++)
            snippetList[i] = Mle3dCharacter::getSnippet(character,i);
    }

    return snippetList;
}


Mle3dCharacter* Mle3dCharacterRegistry::getCharacter(Mle3dCharacterRegistry* registry, 
                                                     unsigned int index)
{
    if (registry && index<registry->m_numCharacters)
        return((Mle3dCharacter*)(((char*)registry) + registry->m_characterOffset[index]));
    else 
        return NULL;
};


Mle3dCharacter* Mle3dCharacterRegistry::getCharacter(Mle3dCharacterRegistry* registry, 
                                                     char* nameSearch)
{
    Mle3dCharacter* result = NULL;
    Mle3dCharacter* temp;
    
    for (unsigned int i = 0; i < registry->m_numCharacters;i++)
	{
	    temp = Mle3dCharacterRegistry::getCharacter(registry,i);
            if (strcmp(temp->m_name,nameSearch) == 0)
			{
	            result = temp;
	            break;
			}
    }
    
    return result;
}


#ifdef BINARY_REGISTRY
Mle3dCharacterRegistry* Mle3dCharacterRegistry::read(char* filename)
{
	Mle3dCharacterRegistry* registry = NULL;
  
    if (filename)
	{
        unsigned int filesize = 0;
        int statcheck = open(filename,O_RDONLY);
        if (statcheck >= 0)
		{
	        struct stat status;
	        fstat(statcheck,&status);
	        filesize = status.st_size;
	        close(statcheck);
	  
		    if (filesize)
			{
			    FILE* in = fopen(filename,"rb");
			    if (in != NULL)
				{
			        registry = (Mle3dCharacterRegistry*)malloc(filesize);
			        if (fread(registry,filesize,1,in) == 0)
					{
				        registry = NULL;
					}
			        fclose(in);
			  
			        if (registry)
					{
						// First-pass check to make sure file has magic number for character registry
				        char* magic = MLE_3D_CHAR_MAGIC;
				        unsigned long* check = (unsigned long*)magic;
				        if (*check != registry->magicNumber)
						{
							fprintf(stderr,"%s%s%s","Mle3dCharacterRegistry::Error--file ",filename,
						        " is not a Magic Lantern character registry.\n");
					        delete [] registry;
					        registry = NULL;
						}
					}
			  
			        if (registry)
					{
						// Second-pass check to see if character information in file is possibly invalid
				        unsigned int check = 0;
				        for (unsigned int i = 0; i < registry->numCharacters; i++)
				            if (registry->characterOffset[i] > check)
					            check=registry->characterOffset[i];
				        if (check > filesize)
						{
							delete [] registry;
					        registry = NULL;
						}
					}
				}
			}
	  
	        if (! registry)
	            fprintf(stderr,"Mle3dCharacterRegistry::Error--Could not load character(s) from file %s.\n",filename);
		}
        else
	        fprintf(stderr,"Mle3dCharacterRegistry::Error--Could not find character registry file %s.\n",filename);
	}
    else
        fprintf(stderr,"Mle3dCharacterRegistry::Error--No filename given for character registry.\n");
  
    return registry;
}
#endif /* BINAYR_REGISTRY */


Mle3dCharacterRegistry* Mle3dCharacterRegistry::read(char* filename)
{
	Mle3dCharacterRegistry* registry = NULL;
  
    if (filename) 
    {
		unsigned int filesize = 0;
#ifdef WIN32
        int statcheck = _open(filename,O_RDONLY);
#else
        int statcheck = open(filename,O_RDONLY);
#endif
        if (statcheck >= 0) 
        {
			struct stat status;
	        fstat(statcheck,&status);
	        filesize = status.st_size;
#ifdef WIN32
	        _close(statcheck);
#else
	        close(statcheck);
#endif
	  
	        if (filesize) 
            {
				FILE* in = fopen(filename,"ra");
	            if (in != NULL) 
                {
					char magic[MLE_NAME_LENGTH];
					int nItems;
                    nItems = fscanf(in,"%s",magic);

                    if (magic && (!strcmp(magic,MLE_3D_CHAR_MAGIC)))
                    {
						registry = new Mle3dCharacterRegistry;

                        // Read Character Registry
                        nItems = fscanf(in,"%s",registry->m_name);
#ifdef MLE_DEBUG 
printf("Registry name is %s\n",registry->m_name); 
#endif
                        nItems = fscanf(in,"%ld",&registry->m_numCharacters);
#ifdef MLE_DEBUG 
printf("Number of characters is %d\n",registry->m_numCharacters);
#endif
                        registry->m_character = new Mle3dCharacter*[registry->m_numCharacters];

                        // Read Characters
                        for (unsigned int i = 0; i < registry->m_numCharacters; i++)
                        {
							Mle3dCharacter* character = new Mle3dCharacter;
                            registry->m_character[i] = character;
                            nItems = fscanf(in,"%s",character->m_name);
#ifdef MLE_DEBUG 
printf("Character name is %s\n",character->m_name);
#endif

                            nItems = fscanf(in,"%ld",&character->m_numSnippets);
#ifdef MLE_DEBUG 
printf("Number of snippets is %d\n",character->m_numSnippets);
#endif
                                 
                            character->m_snippet = new Mle3dSnippet*[character->m_numSnippets];

                            // Read Snippets
                            for (unsigned int j = 0; j < character->m_numSnippets; j++)
                            {
								Mle3dSnippet* snippet = new Mle3dSnippet;
                                character->m_snippet[j] = snippet;

                                nItems = fscanf(in,"%s",snippet->m_name);
#ifdef MLE_DEBUG 
printf("Snippet name is %s\n",snippet->m_name);
#endif

                                nItems = fscanf(in,"%hd %hd",&snippet->m_startFrame,&snippet->m_endFrame);
#ifdef MLE_DEBUG 
printf("Snippet frame range is from %d to %d\n",snippet->m_startFrame,snippet->m_endFrame);
#endif

                                nItems = fscanf(in,"%ld",&snippet->m_numTransitions);
#ifdef MLE_DEBUG 
printf("Number of transitions is %d\n",snippet->m_numTransitions);
#endif
                                 
                                snippet->m_transition=new Mle3dTransition*[snippet->m_numTransitions];

                                // Read Transitions
                                for (unsigned int k = 0; k < snippet->m_numTransitions; k++)
                                {
									Mle3dTransition* transition = new Mle3dTransition;
                                    snippet->m_transition[k] = transition;

                                    nItems = fscanf(in,"%s",transition->m_name);
#ifdef MLE_DEBUG 
printf("Transition name is %s\n",transition->m_name);
#endif

                                    nItems = fscanf(in,"%s",transition->m_event);
#ifdef MLE_DEBUG 
printf("Transition event is %s\n",transition->m_event);
#endif

                                    nItems = fscanf(in,"%d",&transition->m_targetIndex);
#ifdef MLE_DEBUG 
printf("Transition target snippet index is %d\n",transition->m_targetIndex);
#endif

                                    nItems = fscanf(in,"%s",transition->m_targetName);
#ifdef MLE_DEBUG 
printf("Transition target snippet name is %s\n",transition->m_targetName);
#endif

                                    nItems = fscanf(in,"%hd %hd",&transition->m_fromFrame,&transition->m_toFrame);
#ifdef MLE_DEBUG 
printf("Transition changes frame from %d to %d\n",transition->m_fromFrame,transition->m_toFrame);
#endif

								}
							}
						}
					}

		            fclose(in);
				}
	       }
	  
	       if (! registry)
	           fprintf(stderr,"Mle3dCharacterRegistry::Error--Could not load character(s) from file %s.\n",filename);
       }
       else
	       fprintf(stderr,"Mle3dCharacterRegistry::Error--Could not find character registry file %s.\n",filename);
   }
   else
       fprintf(stderr,"Mle3dCharacterRegistry::Error--No filename given for character registry.\n");
  
  return registry;
}


#ifdef MLE_DEBUG

void Mle3dTransition::print(Mle3dTransition* t, FILE* out)
{
    if (!out) out = stdout;
	if (t)
	{ 
	    fprintf(out,"Name=%s\n",t->m_name);
	    fprintf(out,"            event=%s\n",t->m_event);
	    fprintf(out,"            fromFrame=%d\n",t->m_fromFrame);
	    fprintf(out,"            toFrame=%d\n",t->m_toFrame);
	}
}


void Mle3dSnippet::print(Mle3dSnippet* snippet, FILE* out)
{
    if (!out) out = stdout;
    if (snippet)
	{ 
	    fprintf(out,"Name=%s\n",snippet->m_name);
	    fprintf(out,"            startFrame=%d\n",snippet->m_startFrame);
	    fprintf(out,"            endFrame=%d\n",snippet->m_endFrame);
	    fprintf(out,"            Transitions:\n");

        Mle3dTransition** transitionList = Mle3dSnippet::getTransitionList(snippet);
        for (unsigned int i = 0; i < snippet->m_numTransitions; i++)
            Mle3dTransition::print(transitionList[i],out);
	}
}


void Mle3dCharacter::print(Mle3dCharacter* character, FILE* out)
{ 
    if (!out) out = stdout;
    if (character)
    {
        fprintf(out,"Character name=%s\n",character->m_name);
        fprintf(out,"Number of Snippets: %d\n",character->m_numSnippets);
     
        Mle3dSnippet* snippet;
        for (unsigned int i = 0; i < character->m_numSnippets; i++)
		{ 
             snippet = Mle3dCharacter::getSnippet(character,i);
             fprintf(out,"Snippet %d:\n",i);
             Mle3dSnippet::print(snippet,out);
		}
	}
}


void Mle3dCharacterRegistry::print(Mle3dCharacterRegistry* registry, FILE* out)
{
    if (!out) out = stdout;
    if (registry)
	{
		fprintf(out,"Registry name=%s\n",registry->m_name);
        fprintf(out,"Number of Characters=%d\n",registry->m_numCharacters);
        for (unsigned int i = 0; i < registry->m_numCharacters; i++)
            Mle3dCharacter::print(Mle3dCharacterRegistry::getCharacter(registry,i),out);
	}
}

#endif /* MLE_DEBUG */

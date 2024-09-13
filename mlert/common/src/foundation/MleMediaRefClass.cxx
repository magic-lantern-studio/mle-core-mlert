/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MleMediaRefClass.cxx
 * @ingroup MleFoundation
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

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"

// Include Runtime Engine header files.
#include "mle/MleMediaRefClass.h"

// Declare the registry for mediaref classes.
// This registry is indexed by mediaref class names and holds a pointer
// to a function that creates an instance of the mediaref class.
//
#if defined(WIN32)
// Make sure that the registry can be shared if the library is
// included as part of a DLL.
#pragma data_seg( ".GLOBALS" )
#endif
MleDwpStrKeyDict MleMediaRefClass::g_registry(16);
#if defined(WIN32)
#pragma data_seg()
#pragma comment(linker, "/section:.GLOBALS,rws")
#endif

// Declare the DSO loader for mediaref classes.
MleDSOLoader MleMediaRefClass::g_loader;

// This function adds a new entry to the mediaref class registry.  The
// arguments the mediaref class name and an instance creation function.
void
MleMediaRefClass::add(const char *name,MleMediaRef *(*create)(void))
{
    g_registry.set(name,(const void *)create);
}

// This function, MleMediaReflass::find() (the implementation syntax is
// a little confusing), returns a pointer to an instance creation
// function for a mediaref class.
MleMediaRef *(*MleMediaRefClass::find(const char *name))(void)
{
    MleMediaRef *(*create)(void);    // Creation function pointer.

    // Try to look up the mediaref class in the registry.
    create = (MleMediaRef *(*)(void))g_registry.find(name);

    // Return the function pointer if we found it.
    if ( create )
        return create;
    
    // Otherwise, try to load the class from a DSO.
    g_loader.loadClass(name);

    // Now try to look up the mediaref class again.
    create = (MleMediaRef *(*)(void))g_registry.find(name);

    // And return it
    return create;
}

void *
MleMediaRefClass::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleMediaRefClass::operator delete(void *p)
{
	mlFree(p);
}

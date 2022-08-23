/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MleActor.cxx
 * @ingroup MleFoundation
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015-2022 Wizzer Works
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

// Include Runtime Engine header files.
#include "mle/MleGroupClass.h"

// Include Digital Workprint header files.
#include "mle/DwpDatatype.h"

// Initialize dictionaries to work off a pool of 256 entries
// (classes + members) and a hash table of 20 for both
// group classes and group members.
//
#if defined(WIN32)
// Make sure that the registry can be shared if the library is
// included as part of a DLL.
#pragma data_seg( ".GLOBALS" )
#endif
MleGroupClassDict MleGroupClass::g_registry(20);
#if defined(WIN32)
#pragma data_seg()
#pragma comment(linker, "/section:.GLOBALS,rws")
#endif

// Declare the DSO loader for group classes.
MleDSOLoader MleGroupClass::g_loader;

MleGroupClass::MleGroupClass(const char *name,
	MleGroup *(*c)(void),const char *superclass)
: MleDwpStrKeyDict(20)
{
	/* Set the editor name. */
	m_editorName = 0;
	m_contentEditorName = 0;

	/* Remember the creation func. */
	create = c;

	/* Look up the superclass info. */
	MleGroupClass *super = (MleGroupClass *)g_registry.find(superclass);
    if (super != NULL) {
        // ToDo: Report message? What are we looking up the superclass for?
    }

	/* Put self into the registry. */
	g_registry.set(name,this);
}

MleGroupClass::MleGroupClass(const char *name,
	MleGroup *(*c)(void),const char *superclass, 
	const char* e, const char* ce)
: MleDwpStrKeyDict(20)
{
	/* Set the editor name. */
#ifdef WIN32
	m_editorName = (e) ? _strdup(e) : _strdup("");
	m_contentEditorName = (ce) ? _strdup(ce) : _strdup("");
#else
	m_editorName = (e) ? strdup(e) : strdup("");
	m_contentEditorName = (ce) ? strdup(ce) : strdup("");
#endif

	/* Remember the creation func. */
	create = c;

	/* Look up the superclass info. */
	MleGroupClass *super = (MleGroupClass *)g_registry.find(superclass);
    if (super != NULL) {
        // ToDo: Report message? What are we looking up the superclass for?
    }

	/* Put self into the registry */
	g_registry.set(name,this);
}

MleGroupClass::~MleGroupClass()
{
	if (m_editorName != NULL) free(m_editorName);
	if (m_contentEditorName != NULL) free(m_contentEditorName);
}

/*
	This function returns a new instance of an group of this class.
*/
MleGroup *
MleGroupClass::createInstance(void) const
{
	return (*create)();
}

/*
	This static function looks up an group class from a name.
*/
MleGroupClass *
MleGroupClass::find(const char *name)
{
	MleGroupClass *ac;	// Group class return.

	// Try to look up the group class in the registry.
	ac = (MleGroupClass *)g_registry.find(name);

	// Return the group class if we found it.
	if ( ac )
		return ac;
	
	// Otherwise, try to load the class from a DSO.
	g_loader.loadClass(name);

	// Now try to look it up again.
	ac = (MleGroupClass *)g_registry.find(name);

	// Return it whether we succeeded or not.
	return ac;
}

void *
MleGroupClass::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleGroupClass::operator delete(void *p)
{
	mlFree(p);
}

// This defines the MleGroupClassDictEntry subclass for a dictionary
// of MleGroupClass pointers.  All it knows how to do is delete the
// MleGroupClass when the dictionary entry is deleted.
class MleGroupClassDictEntry : public MleDwpStrKeyDictEntry
{
  public:

	/**
	 * The destructor.
	 */
	~MleGroupClassDictEntry() { delete (MleGroupClass *)m_value; }

	/**
	 * Override operator new.
	 *
	 * @param tSize The size, in bytes, to allocate.
	 */
	void* operator new(size_t tSize);

	/**
	 * Override operator delete.
	 *
	 * @param p A pointer to the memory to delete.
	 */
    void  operator delete(void *p);

};

MleDwpDictEntry *
MleGroupClassDict::makeEntry(void)
{
	return new MleGroupClassDictEntry;
}

void *
MleGroupClassDict::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleGroupClassDict::operator delete(void *p)
{
	mlFree(p);
}

void *
MleGroupClassDictEntry::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleGroupClassDictEntry::operator delete(void *p)
{
	mlFree(p);
}

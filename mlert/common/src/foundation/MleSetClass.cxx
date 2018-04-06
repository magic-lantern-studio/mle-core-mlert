/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MleSetClass.cxx
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
//  For information concerning this source code, contact Mark S. Millard,
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

// Inlucde Runtime Engine header files.
#include "mle/MleSetClass.h"

// Include Digital Workprint header files.
#include "mle/DwpDatatype.h"

#if defined(WIN32)
// Make sure that the registry can be shared if the library is
// included as part of a DLL.
#pragma data_seg( ".GLOBALS" )
#endif
MleSetClassDict MleSetClass::g_registry(16);
#if defined(WIN32)
#pragma data_seg()
#pragma comment("linker, /section:.GLOBALS,rws")
#endif

// declare the DSO loader for forum classes.
MleDSOLoader MleSetClass::g_loader;

MleSetClass::MleSetClass(const char *name,
	MleSet *(*c)(void),const char *superclass)
: MleDwpStrKeyDict()
{
	/* Set the editor name. */
	m_editorName = 0;
	m_contentEditorName = 0;

	/* Remember the creation func. */
	create = c;

	/* Look up the superclass info. */
	MleSetClass *super = (MleSetClass *)g_registry.find(superclass);

	/* Copy the superclass members into the local space. */
	if ( super )
	{
		/* Use the dictionary iterator. */
		MleSetMemberIter iter(super);
		const MleSetMember *member;
		while ( member = iter.getMember() )
		{
			set(iter.getName(),new MleSetMember(*member));
			iter.next();
		}
	}

	/* Put self into the registry. */
	g_registry.set(name,this);
}

MleSetClass::MleSetClass(const char *name,
	MleSet *(*c)(void),const char *superclass, 
	const char* e, const char* ce)
: MleDwpStrKeyDict()
{
	/* Set the editor name. */
	/* XXX - need to add a destructor to free these. */
	m_editorName = (e) ? strdup(e) : strdup("");
	m_contentEditorName = (ce) ? strdup(ce) : strdup("");

	/* Remember the creation func */
	create = c;

	/* Look up the superclass info */
	MleSetClass *super = (MleSetClass *)g_registry.find(superclass);

	/* Copy the superclass members into the local space. */
	if ( super )
	{
		/* use the dictionary iterator */
		MleSetMemberIter iter(super);
		const MleSetMember *member;
		while ( member = iter.getMember() )
		{
			set(iter.getName(),new MleSetMember(*member));
			iter.next();
		}
	}

	/* Put self into the registry. */
	g_registry.set(name,this);
}

/*
	This function returns a new instance of an forum of this class.
*/
MleSet *
MleSetClass::createInstance(void) const
{
	return (*create)();
}

/*
	This function adds a new property type to the forum class.
*/
void
MleSetClass::addMember(const char *name,const char *type,int offset)
{
	/* Create the new member object. */
	MleSetMember *member = new MleSetMember(type,offset);

	/* Put the new property into the dictionary. */
	set(name,member);
}

/*
	This function looks up a property.
*/
const MleSetMember *
MleSetClass::findMember(const char *name) const
{
	/* Get the pointer from the registry. */
	return (const MleSetMember *)MleDwpStrKeyDict::find(name);
}

// this defines the MleDwpDictEntry subclass for a dictionary of
// MleSetMember pointers.  All it knows how to do is delete
// the MleSetMember when the dictionary entry is deleted.
class MleSetMemberEntry : public MleDwpStrKeyDictEntry
{
	public:

		~MleSetMemberEntry() { delete (MleSetMember *)m_value; }
};

/*
	This function overrides MleDwpStrKeyDict::makeEntry() to
	return an entry of the above type.
*/
MleDwpDictEntry *
MleSetClass::makeEntry(void)
{
	return new MleSetMemberEntry;
}

/*
	This static function looks up an forum class from a name.
*/
MleSetClass *
MleSetClass::find(const char *name)
{
	MleSetClass *ac;	// set class return.

	// Try to look up the forum class in the registry.
	ac = (MleSetClass *)g_registry.find(name);

	// Return the forum class if we found it.
	if ( ac )
		return ac;
	
	// Otherwise, try to load the class from a DSO.
	g_loader.loadClass(name);

	// Now try to look it up again.
	ac = (MleSetClass *)g_registry.find(name);

	// Return it whether we succeeded or not.
	return ac;
}

void *
MleSetClass::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleSetClass::operator delete(void *p)
{
	mlFree(p);
}

// This defines the MleSetClassDictEntry subclass for a dictionary
// of MleSetClass pointers.  All it knows how to do is delete the
// MleSetClass when the dictionary entry is deleted.
class MleSetClassDictEntry : public MleDwpStrKeyDictEntry
{
  public:

	/**
	 * The destructor.
	 */
	~MleSetClassDictEntry() { delete (MleSetClass *)m_value; }

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

void *
MleSetClassDictEntry::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleSetClassDictEntry::operator delete(void *p)
{
	mlFree(p);
}

MleDwpDictEntry *
MleSetClassDict::makeEntry(void)
{
	return new MleSetClassDictEntry;
}

void *
MleSetClassDict::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleSetClassDict::operator delete(void *p)
{
	mlFree(p);
}

MleSetMember::MleSetMember(const char *t,int o)
{
	m_type = MleDwpDatatype::findType(t);

	if ( m_type == NULL )
	{
		fprintf(stderr, "The workprint data type \"%s\" could not be found.\n",t);
		MLE_ASSERT(m_type!=NULL);
	}
	
	m_offset = o;
}

void *
MleSetMember::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleSetMember::operator delete(void *p)
{
	mlFree(p);
}

void *
MleSetMemberIter::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleSetMemberIter::operator delete(void *p)
{
	mlFree(p);
}

/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MleActorClass.cxx
 * @ingroup MleFoundation
 *
 * @author Mark S. Millard
 * @date May 1, 2003
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015-2019 Wizzer Works
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
#include "mle/MleActorClass.h"

// Include Digital Workprint header files.
#include "mle/DwpDatatype.h"

// Initialize dictionaries to work off a pool of 256 entries
// (classes + members) and a hash table of 20 for both
// actor classes and actor members.
//
#if defined(WIN32)
// Make sure that the registry can be shared if the library is
// included as part of a DLL.
#pragma data_seg( ".GLOBALS" )
#endif
MleActorClassDict MleActorClass::g_registry(20);
#if defined(WIN32)
#pragma data_seg()
#pragma comment("linker, /section:.GLOBALS,rws")
#endif


// Declare the DSO loader for actor classes.
MleDSOLoader MleActorClass::g_loader;

MleActorClass::MleActorClass(const char *name,
	MleActor *(*c)(void),const char *superclass)
: MleDwpStrKeyDict(20), m_propDatasetDict(3)
{
    /* Set the class name. */
    m_name = strdup(name);

	/* Set the editor names. */
	m_editorName = 0;
	m_contentEditorName = 0;

	/* Remember the creation func. */
	create = c;

	/* Look up the superclass info. */
	MleActorClass *super = (MleActorClass *)g_registry.find(superclass);

	/* Copy the superclass members into the local space. */
	if ( super )
	{
		/* Use the dictionary iterator. */
		MleActorMemberIter iter(super);
		const MleActorMember *member;
		while ( member = iter.getMember() )
		{
			set(iter.getName(),new MleActorMember(*member));
			iter.next();
		}
	}

	/* Put self into the global Actor class registry ... */
	MleActorClass *tmp = (MleActorClass *)g_registry.find(name);
	if (tmp == NULL)  // ... but only if it hasn't been registered yet.
	    g_registry.set(name,this);
}

MleActorClass::MleActorClass(const char *name,
	MleActor *(*c)(void),const char *superclass, 
	const char* e, const char* ce)
: MleDwpStrKeyDict(20), m_propDatasetDict(3)
{
	/* Set the class and editor names. */
    m_name = strdup(name);
	m_editorName = (e) ? strdup(e) : strdup("");
	m_contentEditorName = (ce) ? strdup(ce) : strdup("");

	/* Remember the creation func. */
	create = c;

	/* Look up the superclass info. */
	MleActorClass *super = (MleActorClass *)g_registry.find(superclass);

	/* Copy the superclass members into the local space. */
	if ( super )
	{
		/* Use the dictionary iterator. */
		MleActorMemberIter iter(super);
		const MleActorMember *member;
		while ( member = iter.getMember() )
		{
			set(iter.getName(),new MleActorMember(*member));
			iter.next();
		}
	}

	/* Put self into the global Actor class registry ... */
	MleActorClass *tmp = (MleActorClass *)g_registry.find(name);
	if (tmp == NULL)  // ... but only if it hasn't been registered yet.
	    g_registry.set(name,this);
}

MleActorClass::~MleActorClass()
{
    /* Remove self from the registry? */
    // g_registry.remove(m_name);
    // Note - this destructor is called when the class is unregistered with the above
    // remove! Therefore, don't unregister from the destructor.

    if (m_name != NULL) free(m_name);                     // allocated using strdup()
    if (m_editorName != NULL) free(m_editorName);         // allocated using strdup()
    if (m_contentEditorName != NULL) free(m_editorName);  // allocated using strdup()

    // Remove property members.
    MleActorMemberIter iter(this);
    const MleActorMember *member;
    while ( member = iter.getMember() )
    {
        // Note that the dictionary remove method below does NOT delete the
        // value of the dictionary entry. Therefore, we return it for the
        // caller to clean up the entry.
        MlePropertyEntry *entry = member->getEntry();
        // XXX - delete entry here?

        // Remove the property type from the dictionary.
        remove(iter.getName());

        // Get the next member.
        iter.next();
    }
}

void *
MleActorClass::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleActorClass::operator delete(void *p)
{
	mlFree(p);
}

/*
	This function returns a new instance of an actor of this class.
*/
MleActor *
MleActorClass::createInstance(void) const
{
	return (*create)();
}

/*
 * This function adds a new property type to the actor class.
*/
void
MleActorClass::addMember(const char *name,const char *type,MlePropertyEntry *entry)
{
	/* Create the new member object. */
	MleActorMember *member = new MleActorMember(type,entry);

	/* Put the new property into the dictionary. */
	set(name,member);
}

/*
 * This function removes an existing property type from the actor class.
 * NULL will be returned if no entry matching the specified name is found.
 */
MlePropertyEntry *
MleActorClass::removeMember(const char *name)
{
    MlePropertyEntry *entry = NULL;

    if (name == NULL) return NULL;

    const MleActorMember *member = findMember(name);
    if (member != NULL)
    {
        // Note that the dictionary remove method below does NOT delete the
        // value of the dictionary entry. Therefore, we return it for the
        // caller to clean up the entry.
        entry = member->getEntry();

        // Remove the property type from the dictionary.
        remove(name);
    }

    return entry;
}

/*
 * This function looks up a property.
*/
const MleActorMember *
MleActorClass::findMember(const char *name) const
{
	/* Get the pointer from the registry. */
	return (const MleActorMember *)MleDwpStrKeyDict::find(name);
}

void
MleActorClass::addMemberDataset(const char *propName,
	const char *propDatasetName)
{
    int i, count;
    MlePtrArray *propNameArray;

    if (! propDatasetName || ! propName)
		return;

    propNameArray = (MlePtrArray *) m_propDatasetDict.find(propDatasetName);
    if (propNameArray)
	{
		count = propNameArray->getSize();
		for(i = 0; i < count; i++)
		{
			if(strcmp((char *) ((*propNameArray)[i]), propName) == 0)
			return;
		}
		propNameArray->add((void *) propName);
    }
    else
	{
		propNameArray = new MlePtrArray(1);
		propNameArray->add((void *) propName);

		m_propDatasetDict.set(propDatasetName, propNameArray);
    }
}

MlePtrArray *
MleActorClass::getPropNames(const char *propDatasetName) const
{
    return ((MlePtrArray *) m_propDatasetDict.find(propDatasetName));
}

// This defines the MleDwpDictEntry subclass for a dictionary of
// MleActorMember pointers.  All it knows how to do is delete
// the MleActorMember when the dictionary entry is deleted.
class MleActorMemberEntry : public MleDwpStrKeyDictEntry
{
  public:

	/**
	 * The destructor.
	 */
	virtual ~MleActorMemberEntry() { delete (MleActorMember *)m_value; }

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
MleActorMemberEntry::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleActorMemberEntry::operator delete(void *p)
{
	mlFree(p);
}

/*
	This function overrides MleDwpStrKeyDict::makeEntry() to
	return an entry of the above type.
*/
MleDwpDictEntry *
MleActorClass::makeEntry(void)
{
	return new MleActorMemberEntry;
}

/*
	This static function looks up an actor class from a name.
*/
MleActorClass *
MleActorClass::find(const char *name)
{
	MleActorClass *ac;	// Actor class return.

	// Try to look up the actor class in the registry.
	ac = (MleActorClass *)g_registry.find(name);

	// Return the actor class if we found it.
	if ( ac )
		return ac;
	
	// Otherwise, try to load the class from a DSO.
	g_loader.loadClass(name);

	// Now try to look it up again.
	ac = (MleActorClass *)g_registry.find(name);

	// Return it whether we succeeded or not.
	return ac;
}

// This defines the MleActorClassDictEntry subclass for a dictionary
// of MleActorClass pointers.  All it knows how to do is delete the
// MleActorClass when the dictionary entry is deleted.
class MleActorClassDictEntry : public MleDwpStrKeyDictEntry
{
  public:

	/**
	 * The destructor.
	 */
	virtual ~MleActorClassDictEntry() { delete (MleActorClass *)m_value; }

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
MleActorClassDict::makeEntry(void)
{
	return new MleActorClassDictEntry;
}

void *
MleActorClassDict::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleActorClassDict::operator delete(void *p)
{
	mlFree(p);
}

void *
MleActorClassDictEntry::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleActorClassDictEntry::operator delete(void *p)
{
	mlFree(p);
}

MleActorMember::MleActorMember(const char *t,MlePropertyEntry *e)
{
	m_type = MleDwpDatatype::findType(t);

	if ( m_type == NULL )
	{
		fprintf(stderr, "The workprint data type \"%s\" could not be found.\n",t);
		MLE_ASSERT(m_type!=NULL);
	}
	
	m_entry = e;
}

void *
MleActorMember::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleActorMember::operator delete(void *p)
{
	mlFree(p);
}

void *
MleActorMemberIter::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleActorMemberIter::operator delete(void *p)
{
	mlFree(p);
}

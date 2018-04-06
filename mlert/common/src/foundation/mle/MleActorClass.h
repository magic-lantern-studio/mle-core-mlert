/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MleActorClass.h
 * @ingroup MleFoundation
 *
 * @author Mark S. Millard
 * @created Sep 20, 2005
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

#ifndef __MLE_ACTORCLASSTABLE_H_
#define __MLE_ACTORCLASSTABLE_H_

#include "mle/DwpStrKeyDict.h"
#include "mle/MleRuntime.h"
#include "mle/MleDsoLoader.h"
#include "mle/MleProperty.h"
#define TEMPLATE
#include "mle/MlePtrArray.h"

class MleActor;
class MleActorClass;
class MleDwpDatatype;

/**
 * This class represents a property registered by an Actor in its
 * initClass() routine.  There will be one of these objects in
 * the MleActorClass dictionary for every property registered with
 * MleRegisterActorMember().
 */
class MLE_RUNTIME_API MleActorMember
{
  public:

    // Property info.

    /**
	 * Get the type of the property, specified
     * as a workprint type object.  The type object knows
     * how to read and write from a workprint, as well as
     * how to put data into an actor.
	 *
	 * @return A pointer to the property type is returned.
	 */
    const MleDwpDatatype *getType(void) const { return m_type; }

	/**
	 * Get the meta-data of this property for
     * the owning Actor's data structure.  This is used to
     * poke values directly into the property.
	 *
	 * @return The property meta-data is returned.
	 */
    MlePropertyEntry *getEntry(void) const { return m_entry; }

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

  protected:

    /**
	 * A constructor that initializes the type and offset.
	 *
     * These items are only created through operation of
     * the MleActorClass, so the constructor is not publically
     * accessible.
	 *
	 * @param type A pointer to the member's data type.
	 * @param entry A pointer to the Actor's meta-data; which can be used
     * to access the property data.
	 */
    MleActorMember(const char *type,MlePropertyEntry *entry);

	/** Property data type. */
    const MleDwpDatatype *m_type;
	/** Property meta-dta. */
    MlePropertyEntry *m_entry;

    friend class MleActorClass;
};

/**
 * This is an object that contains MleActorClass pointers.  It needs
 * to be a distinguished object so it can create dictionary entries
 * that will delete the MleActorClass objects when the dictionary
 * goes away.
 */
class MLE_RUNTIME_API MleActorClassDict : public MleDwpStrKeyDict
{
  public:

    MleActorClassDict(int hashsize) : MleDwpStrKeyDict(hashsize) {}

    virtual MleDwpDictEntry *makeEntry(void);

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

/**
 * This is an object that holds information about an Actor class.  It
 * contains a function pointer that knows how to create an instance
 * of the Actor class, and it is a dictionary of Actor members
 * (properties).  It has a static find() function that can be used to
 * look up an Actor class by name.
 */
class MLE_RUNTIME_API MleActorClass : public MleDwpStrKeyDict
{
  public:

    /**
	 * A constructor that initializes the Actor's class name,
	 * a creation function pointer, and immediate superclass.
	 *
     * @param name The class name.
	 * @param create An instance creation function pointer.
	 * @param superclass The name of the immediate superclass.
	 */
    MleActorClass(const char *name,
		 MleActor *(*create)(void),const char *superclass);

	/**
	 * A constructor that initializes the Actor's class name,
	 * a creation function pointer, and immediate superclass.
	 *
     * @param name The class name.
	 * @param create An instance creation function pointer.
	 * @param superclass The name of the immediate superclass.
	 * @param editor The Actor class editor.
	 * @param contentEditor The Actor editor.
	 */
    MleActorClass(const char *name,
		 MleActor *(*create)(void),const char *superclass, 
		 const char* editor, const char* contentEditor);
    
    /**
	 * Creates a new instance of the class.
	 *
	 * @return A pointer to the new instance is returned.
	 */
    MleActor *createInstance(void) const;
    
	/**
     * Adds a new property given the property name,
     * the property type (given as a string so it can be used
     * to look up a type object), and the offset in bytes of this
     * property in an actor instance.
	 *
	 * @param name The name of the property.
	 * @param type The property type.
	 * @param entry A pointer to a property entry containing meta-data
	 * such as the Actor's setter and getter functions.
	 */
    void addMember(const char *name,const char *type,MlePropertyEntry *entry);

	/**
	 * Finds the member object of a given name.
	 *
	 * @param name The name of the member to find.
	 *
	 * @return A pointer to the Actor member is returned.
	 */
    const MleActorMember *findMember(const char *name) const;

    /**
     * Add the member into a property dataset.  A new property
     * dataset is created if it hasn't been created yet.
	 *
	 * @param The name of the property.
	 * @param The name of the property dataset.
     */
    void addMemberDataset(const char *name, const char *propDatasetName);

    /**
     * Get an array of property names related to propDatasetName.
	 *
	 * @param propDatasetName The name of the property dataset.
	 *
	 * @return A MlePtrArray * is returned where each of its member is a char *.
     */
    MlePtrArray *getPropNames(const char *propDatasetName) const; 

    /**
	 * A static member function that looks up an
     * Actor class object from a string name.
	 *
	 * @param name The name of the Actor class.
	 * 
	 * @param A pointer to the found MleActorClass is returned.
	 */
    static MleActorClass *find(const char *name);
    
    char* getEditorName() { return m_editorName; }

    char* getContentEditorName()  { return m_contentEditorName; }

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

    /** The Actor class loader. */
    static MleDSOLoader g_loader;

  protected:

	/** The instance creation function pointer. */
    MleActor *(*create)(void);
    
    /**
	 * This is a MleDwpDict virtual override that creates entries
     * that can delete the member when the entry is deleted.
	 *
	 * @reutnr A pointer to the new dictionary entry is returned.
	 */
    virtual MleDwpDictEntry *makeEntry(void);
    
	/* A static Actor class registry. */
    static MleActorClassDict g_registry;
    
	/* Contains property dataset names and the corresponding property names. */
    MleDwpStrKeyDict m_propDatasetDict;

    friend class MleActorMemberIter;
    
    /** The name of the Actor class editor. */
    char* m_editorName;
	/** The name of the Actor editor. */
    char* m_contentEditorName;
};

/**
 * This is a simple iterator class to run through an Actor class property
 * list.
 */
class MLE_RUNTIME_API MleActorMemberIter : public MleDwpDictIter
{
  public:

	/**
     * A constructor.
	 *
	 * @param ac The argument is the Actor class whose properties are of interest.
	 */
    MleActorMemberIter(MleActorClass *ac) : MleDwpDictIter(*ac) {};
    
    /**
	 * Get the name for the current property.
	 *
	 * @return The name of the current property is returned.
	 */
    const char *getName(void) const
      { return (const char *)getKey(); }

	/**
	 * Get the member object pointer for the current property.
	 *
	 * @return A pointer to the MleActorMemeber is returned.
	 */
    const MleActorMember *getMember(void) const
      { return (const MleActorMember *)getValue(); }
    
    /**
	 * Change the state of the iterator to the next property.
	 */
    int next(void) { return MleDwpDictIter::next(); }

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

#endif /* __MLE_ACTORCLASSTABLE_H_ */

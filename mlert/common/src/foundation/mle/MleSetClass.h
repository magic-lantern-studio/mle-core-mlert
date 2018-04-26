/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MleSetClass.h
 * @ingroup MleFoundation
 *
 * @author Mark S. Millard
 * @created Sep 20, 2005
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
//  For information concerning this header file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

#ifndef __MLE_SETCLASS_H_
#define __MLE_SETCLASS_H_

#include "mle/DwpStrKeyDict.h"
#include "mle/MleDsoLoader.h"
#include "mle/MleProperty.h"

#include "mle/MleRuntime.h"

class MleSet;
class MleSetClass;
class MleDwpDatatype;

/**
 * This class represents a property registered by a Set in its
 * initClass() routine.  There will be one of these objects in
 * the MleSetClass dictionary for every property registered with
 * MleRegisterSetMember().
 */
class MLE_RUNTIME_API MleSetMember
{
  public:

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
     * the owning Set's data structure.  This is used to
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
     * the MleSetClass, so the constructor is not publically
     * accessible.
	 *
	 * @param type A pointer to the member's data type.
	 * @param entry A pointer to the Set's meta-data; which can be used
     * to access the property data.
	 */
    MleSetMember(const char *type,MlePropertyEntry *entry);
    
	/** Property data type. */
    const MleDwpDatatype *m_type;
	/** Property meta-dta. */
    MlePropertyEntry *m_entry;
    
    friend class MleSetClass;
};


/**
 * This is an object that contains MleSetClass pointers.  It needs
 * to be a distinguished object so it can create dictionary entries
 * that will delete the MleSetClass objects when the dictionary
 * goes away.
 */
class MLE_RUNTIME_API MleSetClassDict : public MleDwpStrKeyDict
{
  public:

    MleSetClassDict(int hashsize) : MleDwpStrKeyDict(hashsize) {}
    
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
 * This is an object that holds information about a Set class.  It
 * contains a function pointer that knows how to create an instance
 * of the Set class, and it is a dictionary of Set members
 * (properties).  It has a static find() function that can be used to
 * look up a Set class by name.
 */
class MLE_RUNTIME_API MleSetClass : public MleDwpStrKeyDict
{
  public:

    /**
	 * A constructor that initializes the Set's class name,
	 * a creation function pointer, and immediate superclass.
	 *
     * @param name The class name.
	 * @param create An instance creation function pointer.
	 * @param superclass The name of the immediate superclass.
	 */
    MleSetClass(const char *name,
		 MleSet *(*create)(void),const char *superclass);

	/**
	 * A constructor that initializes the Set's class name,
	 * a creation function pointer, and immediate superclass.
	 *
     * @param name The class name.
	 * @param create An instance creation function pointer.
	 * @param superclass The name of the immediate superclass.
	 * @param editor The Set class editor.
	 * @param contentEditor The Set editor.
	 */
    MleSetClass(const char *name,
		 MleSet *(*create)(void),const char *superclass,
		 const char* editor, const char* contentEditor);
    
    /**
	 * Creates a new instance of the class.
	 *
	 * @return A pointer to the new instance is returned.
	 */
    MleSet *createInstance(void) const;
    
	/**
     * Adds a new property given the property name,
     * the property type (given as a string so it can be used
     * to look up a type object), and the offset in bytes of this
     * property in an actor instance.
	 *
	 * @param name The name of the property.
	 * @param type The property type.
	 * @param entry A pointer to a property entry containing meta-data
	 * such as the Set's setter and getter functions.
	 */
    void addMember(const char *name,const char *type,MlePropertyEntry *entry);

	/**
	 * Finds the member object of a given name.
	 *
	 * @param name The name of the member to find.
	 *
	 * @return A pointer to the Set member is returned.
	 */
    const MleSetMember *findMember(const char *name) const;
    
    /**
	 * A static member function that looks up a
     * Set class object from a string name.
	 *
	 * @param name The name of the Set class.
	 * 
	 * @param A pointer to the found MleSetClass is returned.
	 */
    static MleSetClass *find(const char *name);
    
    char* getEditorName() { return m_editorName; }

    char* getContentEditorName() { return m_contentEditorName; }
    
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

	/** The instance creation function pointer. */
    MleSet *(*create)(void);
    
    /**
	 * This is a MleDwpDict virtual override that creates entries
     * that can delete the member when the entry is deleted.
	 *
	 * @reutnr A pointer to the new dictionary entry is returned.
	 */
    virtual MleDwpDictEntry *makeEntry(void);
    
	/* A static Set class registry. */
    static MleSetClassDict g_registry;
	/** The Set class loader. */
    static MleDSOLoader g_loader;
    
    friend class MleSetMemberIter;
    
    /** The name of the Set class editor. */
    char* m_editorName;
	/** The name of the Set editor. */
    char* m_contentEditorName;
};

/**
 * This is a simple iterator class to run through an Set class property
 * list.
 */
class MLE_RUNTIME_API MleSetMemberIter : public MleDwpDictIter
{
  public:

	/**
     * A constructor.
	 *
	 * @param ac The argument is the Set class whose properties are of interest.
	 */
    MleSetMemberIter(MleSetClass *ac) : MleDwpDictIter(*ac) {};
    
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
	 * @return A pointer to the MleSetMemeber is returned.
	 */
    const MleSetMember *getMember(void) const
      { return (const MleSetMember *)getValue(); }
    
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

#endif /* __MLE_SETCLASS_H_ */

/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MleStageClass.h
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

#ifndef __MLE_STAGECLASS_H_
#define __MLE_STAGECLASS_H_

// Include Digital Workprint header files.
#include "mle/DwpStrKeyDict.h"

#include "mle/MleRuntime.h"
#include "mle/MleProperty.h"

// Foward Declaration.
class MLE_RUNTIME_API MleDSOLoader;
class MLE_DWP_API MleDwpDatatype;
class MLE_RUNTIME_API MleStage;
class MLE_RUNTIME_API MleStageClass;
class MLE_RUNTIME_API MleStageClassDict;
class MLE_RUNTIME_API MleStageMember;
class MLE_RUNTIME_API MleStageMemberIter;


/**
 * This is an object that holds information about a Stage class.  It
 * contains a function pointer that knows how to create an instance
 * of the Stage class, and it is a dictionary of Stage members
 * (properties).  It has a static find() function that can be used to
 * look up a Stage class by name.
 */
class MLE_RUNTIME_API MleStageClass : public MleDwpStrKeyDict
{
  public:

    /**
	 * A constructor that initializes the Stage's class name,
	 * a creation function pointer, and immediate superclass.
	 *
     * @param name The class name.
	 * @param create An instance creation function pointer.
	 * @param superclass The name of the immediate superclass.
	 */
    MleStageClass(const char *name, MleStage *(*create)(void),
		 const char *superclass);

    /**
	 * Creates a new instance of the class.
	 *
	 * @return A pointer to the new instance is returned.
	 */
    MleStage *createInstance(void) const;

	/**
     * Adds a new property given the property name,
     * the property type (given as a string so it can be used
     * to look up a type object), and the offset in bytes of this
     * property in an actor instance.
	 *
	 * @param name The name of the property.
	 * @param type The property type.
	 * @param entry A pointer to the property entry.
	 */
    void addMember(const char *name, const char *type, MlePropertyEntry *entry);

	/**
	 * Finds the member object of a given name.
	 *
	 * @param name The name of the member to find.
	 *
	 * @return A pointer to the Stage member is returned.
	 */
    const MleStageMember *findMember(const char *name) const;

    /**
	 * A static member function that looks up an
     * Stage class object from a string name.
	 *
	 * @param name The name of the stage class.
	 * 
	 * @param A pointer to the found MleStageClass is returned.
	 */
    static MleStageClass *find(const char *name);

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

    friend class MleStageMemberIter;

    /**
	 * This is a MleDwpDict virtual override that creates entries
     * that can delete the member when the entry is deleted.
	 *
	 * @reutnr A pointer to the new dictionary entry is returned.
	 */
    virtual MleDwpDictEntry *makeEntry(void);

	/** The instance creation function pointer. */
    MleStage *(*create)(void);

	/* A static Actor class registry. */
    static MleStageClassDict g_registry;

	/* A static Actor class registry. */
    static MleDSOLoader g_loader;
};


/**
 * This is an object that contains MleActorClass pointers.  It needs
 * to be a distinguished object so it can create dictionary entries
 * that will delete the MleActorClass objects when the dictionary
 * goes away.
 */
class MLE_RUNTIME_API MleStageClassDict : public MleDwpStrKeyDict
{
  public:

    MleStageClassDict(int hashsize) : MleDwpStrKeyDict(hashsize) {};

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
 * This class represents a property registered by a Stage in its
 * initClass() routine.  There will be one of these objects in
 * the MleStageClass dictionary for every property registered with
 * mleRegisterStageMember().
 */
class MLE_RUNTIME_API MleStageMember
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
	 * Get the meta-data of this property. This is used to
     * poke values directly into the stage using the registered
     * setter member function.
	 *
	 * @return A pointer to the meta-data container is returned.
	 */
    //int getOffset(void) const { return m_offset; }
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

    friend class MleStageClass;

    /**
	 * A constructor that initializes the type and meta-data.
	 *
     * These items are only created through operation of
     * the MleStageClass, so the constructor is not publically
     * accessible.
	 *
	 * @param type A pointer to the member's data type.
	 * @param entry A pointer to the meta-data container.
	 */
    MleStageMember(const char *type, MlePropertyEntry *entry);

	/** Property data type. */
    const MleDwpDatatype *m_type;
	/** Property meta-data. */
    MlePropertyEntry *m_entry;
};


/**
 * This is a simple iterator class to run through a Stage class property
 * list.
 */
class MLE_RUNTIME_API MleStageMemberIter : public MleDwpDictIter
{
  public:

	/**
     * A constructor.
	 *
	 * @param ac The argument is the Stage class whose properties are of interest.
	 */
    MleStageMemberIter(MleStageClass *stageClass)
	: MleDwpDictIter(*stageClass)	{};

    /**
	 * Get the name for the current property.
	 *
	 * @return The name of the current property is returned.
	 */
    const char *getName(void) const
	{ return (const char *) getKey(); }

	/**
	 * Get the member object pointer for the current property.
	 *
	 * @return A pointer to the MleActorMemeber is returned.
	 */
    const MleStageMember *getMember(void) const
	{ return (const MleStageMember *) getValue(); }

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


#endif /* __MLE_STAGECLASS_H_ */

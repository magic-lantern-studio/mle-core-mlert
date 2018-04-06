/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MleGroupClass.h
 * @ingroup MleFoundation
 *
 * @author Mark S. Millard
 * @created Set 20, 2005
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

#ifndef __MLE_GROUPCLASSTABLE_H_
#define __MLE_GROUPCLASSTABLE_H_

#include "mle/DwpStrKeyDict.h"
#include "mle/MleDsoLoader.h"

#include "mle/MleRuntime.h"

class MleGroup;
class MleGroupClass;
class MleDwpDatatype;

/**
 * This is an object that contains MleGroupClass pointers.  It needs
 * to be a distinguished object so it can create dictionary entries
 * that will delete the MleGroupClass objects when the dictionary
 * goes away.
 */
class MLE_RUNTIME_API MleGroupClassDict : public MleDwpStrKeyDict
{
  public:

    MleGroupClassDict(int hashsize) : MleDwpStrKeyDict(hashsize) {}

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
 * This is an object that holds information about an Group class.  It
 * contains a function pointer that knows how to create an instance
 * of the Group class, and it is a dictionary of Group members
 * (properties). It has a static find() function that can be used
 * look up an group class by name.
 */
class MLE_RUNTIME_API MleGroupClass : public MleDwpStrKeyDict
{
  public:

    /**
	 * A constructor that initializes the Group's class name,
	 * a creation function pointer, and immediate superclass.
	 *
     * @param name The class name.
	 * @param create An instance creation function pointer.
	 * @param superclass The name of the immediate superclass.
	 */
    MleGroupClass(const char *name,
		 MleGroup *(*create)(void),const char *superclass);

	/**
	 * A constructor that initializes the Group's class name,
	 * a creation function pointer, and immediate superclass.
	 *
     * @param name The class name.
	 * @param create An instance creation function pointer.
	 * @param superclass The name of the immediate superclass.
	 * @param editor The Group class editor.
	 * @param contentEditor The Group editor.
	 */
    MleGroupClass(const char *name,
		 MleGroup *(*create)(void),const char *superclass, 
		 const char* editor, const char* contentEditor);
    
    /**
	 * Creates a new instance of the class.
	 *
	 * @return A pointer to the new instance is returned.
	 */
    MleGroup *createInstance(void) const;
    
    /**
	 * A static member function that looks up a
     * Group class object from a string name.
	 *
	 * @param name The name of the Group class.
	 * 
	 * @param A pointer to the found MleGroupClass is returned.
	 */
    static MleGroupClass *find(const char *name);
    
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
    
    /** The Group class loader. */
    static MleDSOLoader g_loader;

  protected:

	/** The instance creation function pointer. */
    MleGroup *(*create)(void);
    
	/* A static Group class registry. */
    static MleGroupClassDict g_registry;
    
    /** The name of the Group class editor. */
    char* m_editorName;
	/** The name of the Group editor. */
    char* m_contentEditorName;
};

#endif /* __MLE_GROUPCLASSTABLE_H_ */

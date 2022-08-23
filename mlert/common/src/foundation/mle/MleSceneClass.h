/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MleSceneClass.h
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
//  For information concerning this header file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

#ifndef __MLE_SCENECLASSTABLE_H_
#define __MLE_SCENECLASSTABLE_H_

// Include Digital Workprint header files.
#include "mle/DwpStrKeyDict.h"

// Include Runtime Engine header files.
#include "mle/MleDsoLoader.h"

#include "mle/MleRuntime.h"

class MleScene;
class MleSceneClass;
class MleDwpDatatype;

/**
 * This is an object that contains MleSceneClass pointers.  It needs
 * to be a distinguished object so it can create dictionary entries
 * that will delete the MleSceneClass objects when the dictionary
 * goes away.
 */
class MLE_RUNTIME_API MleSceneClassDict : public MleDwpStrKeyDict
{
  public:

    MleSceneClassDict(int hashsize) : MleDwpStrKeyDict(hashsize) {}

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
 * This is an object that holds information about a Scene class.  It
 * contains a function pointer that knows how to create an instance
 * of the Scene class, and it is a dictionary of Scene members
 * (properties).  It has a static find() function that can be used to
 * look up a Set class by name.
 */
class MLE_RUNTIME_API MleSceneClass : public MleDwpStrKeyDict
{
  public:

    /**
	 * A constructor that initializes the Scene's class name,
	 * a creation function pointer, and immediate superclass.
	 *
     * @param name The class name.
	 * @param create An instance creation function pointer.
	 * @param superclass The name of the immediate superclass.
	 */
    MleSceneClass(const char *name,
		 MleScene *(*create)(void),const char *superclass);

	/**
	 * A constructor that initializes the Scene's class name,
	 * a creation function pointer, and immediate superclass.
	 *
     * @param name The class name.
	 * @param create An instance creation function pointer.
	 * @param superclass The name of the immediate superclass.
	 * @param editor The Scene class editor.
	 * @param contentEditor The Scene editor.
	 */
    MleSceneClass(const char *name,
		 MleScene *(*create)(void),const char *superclass, 
		 const char* editor, const char* contentEditor);
    
    /**
     * Destructor.
     */
    ~MleSceneClass();

    /**
	 * Creates a new instance of the class.
	 *
	 * @return A pointer to the new instance is returned.
	 */
    MleScene *createInstance(void) const;
    
    /**
	 * A static member function that looks up a
     * Scene class object from a string name.
	 *
	 * @param name The name of the Scene class.
	 * 
	 * @param A pointer to the found MleSceneClass is returned.
	 */
    static MleSceneClass *find(const char *name);
    
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

	/** The Scene class loader. */
    static MleDSOLoader g_loader;

  protected:

	 /** The instance creation function pointer. */
    MleScene *(*create)(void);
    
	/* A static Scene class registry. */
    static MleSceneClassDict g_registry;
    
    /** The name of the Scene class editor. */
    char* m_editorName;
	/** The name of the Set editor. */
    char* m_contentEditorName;
};

#endif /* __MLE_SCENECLASSTABLE_H_ */

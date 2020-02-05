/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MleGroup.h
 * @ingroup MleFoundation
 *
 * @author Mark S. Millard
 * @created May 5, 2003
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

#ifndef __MLE_GROUP_H_
#define __MLE_GROUP_H_

// Include Magic Lantern header files.
#include "mle/MleRuntime.h"
#include "mle/MlePtrContainer.h"
#include "mle/MleActor.h"

#ifdef MLE_DIGITAL_WORKPRINT
#include "mle/MleGroupClass.h"
#include "mle/DwpGroup.h"
#endif /* MLE_DIGITAL_WORKPRINT */


/**
 * @brief MleGroup is the base class for run time actor groups.  
 *
 * Base class for run time actor Groups.  The group is the minimal
 * unit of actor loading at run time.  A group may contain one or
 * more actors and every actor must be loaded as part of a group
 * defined in a MleWpGroup structure in the workprint.  A group may
 * be loaded directly at runtime by calling the mleLoadGroup() function.
 *
 * At run time the MleGroup object holds an array of pointers to 
 * all the actors it has loaded.  When the group is deleted, it
 * deletes all actors it refers to.  Similarly, an MleGroup loaded as
 * part of an MleScene is deleted when the containing scene is deleted.
 *
 * REHEARSAL PLAYER
 *
 * The MleGroup class may be subclassed by the title author provided that
 * the workprint names the subclassed type and includes a MleWpHeaderFile
 * object that refers to the subclass header.  At runtime the subclassed
 * object's init() method will be called to invoke group-dependent 
 * operations.   The subclassed group must use the MLE_GROUP_HEADER and
 * MLE_GROUP_SOURCE macros and must invoke the mleRegisterGroupClass()
 * macro in the initClass() method.
 *
 * @see MleScene
 */
class MLE_RUNTIME_API MleGroup : public MlePtrContainer<MleActor*> 
{
  // Declare member funtions.

  public:

    /**
     * @brief Construct a group with an initially sized array of actor
     * pointers.
     *
     * @param size The size of the actor array.
     */
    MleGroup( int size );

    /**
     * @brief Default constructor.
     *
     * Construct a group with a default sized array of actor pointers.
     */
    MleGroup( void );


    /**
     * @brief The destructor.
     */
    virtual ~MleGroup( void );


    /**
     * @brief Initialize the group.
     *
     * The class-specific initialization to be called after the group is
     * loaded and its actors' init() methods are called.
     */
    virtual void init(void);

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

#ifdef MLE_DIGITAL_WORKPRINT

    // Get the type of the group -- rehearsal time only.
    virtual const char *getTypeName(void) const;

    // Predicate returning true if the type of the group is the same as type -- rehearsal time only.
    virtual int isa(const char *type) const;

    // One time class initialization that sets up the constructor for this class.
    static void initClass(void);
 
    //   This function returns the (cached) MleGroupClass object for
    //   this group.  It is not const because of the caching.
    const MleGroupClass *getClass(void);

    // Registry functions.  Registers a newly created instance.
    // This will give the tools a way to search for particular instances
    static MleDwpStrKeyDict* getInstanceRegistry()  { return &g_instanceRegistry; }
    virtual void registerInstance(const char* name);
    virtual void unregisterInstance();

    // Load the group from the relevant item in the DWP.
    // Use different binding types depending on whether the DWP is 
    // a playprint or workprint.  In the workprint, we use the WP struct.
    // Load by group pointer.
   virtual MleGroup* load( MleDwpGroup* wpGroup );	

    // Load by name.
    virtual MleGroup* load(const char* id);

#endif /* MLE_DIGITAL_WORKPRINT */
#ifdef MLE_DIGITAL_PLAYPRINT

    /**
     * @brief Load the group from the Digital Playprint.
     *
     * In the playprint, we use the compiled table of group identifiers.
     * Load by playprint identifier.
     */
    virtual MleGroup* load( const int indexToPpTOC );

#endif /* MLE_DIGITAL_PLAYPRINT */

#ifdef MLE_DIGITAL_WORKPRINT

  protected:

    // The group instance registry.
    static MleDwpStrKeyDict g_instanceRegistry; 
    
    // Group class.
    const MleGroupClass *m_groupClass;

  private:

	// The name of the group.
    char* m_name;

#endif /* MLE_DIGITAL_WORKPRINT */
};


#ifdef MLE_DIGITAL_WORKPRINT

// This macro should be placed in the class declaration of any subclass
// of MleGroup.  It defines some basic functions needed by every subclass.
// The single argument is the unquoted class name.  Be aware that
// this macro switches subsequent declarations to public.
#define MLE_GROUP_HEADER(C) \
  public: \
    virtual const char *getTypeName(void) const; \
    virtual int isa(const char *type) const; \
    static C *cast(MleGroup *group); \
    static void initClass(void)

// This macro should be placed in the implementation of any subclass of
// MleGroup.  It defines some basic functionality needed by every subclass.
// The single argument is the unquoted class name.
#define MLE_GROUP_SOURCE(C,S) \
    const char *C::getTypeName(void) const { return #C; } \
    int C::isa(const char *type) const { \
	if ( !strcmp(type,#C) ) return 1; \
	else return S::isa(type); \
    } \
    C *C::cast(MleGroup *group) { \
	MLE_ASSERT(group->isa(#C)); \
	return (C *)group; \
    } \
    MleGroup *_mlCreate##C(void); MleGroup *_mlCreate##C(void) { return new C; }

// Every subclass of MleGroup must implement a static initClass() member
// function which registers itself with the system.  This registration
// is requires for rehearsal runtime and the mastering process.  The
// initClass function should be implemented inside a #ifdef MLE_REHEARSAL
// block so it is not compiled for the runtime.
//
// Inside the function, the macro mlRegisterGroupClass should be invoked.
// This registers the group class with the system so it can be created.

// This macro takes the unquoted group class and superclass names.
#define mlRegisterGroupClass(GROUP,SUPERCLASS) \
    static int _mlGroupClassInited = 0; \
    if ( _mlGroupClassInited ) return; \
    _mlGroupClassInited = 1; \
    SUPERCLASS::initClass(); \
    new MleGroupClass(#GROUP,_mlCreate##GROUP,#SUPERCLASS)

// This macro takes the unquoted group class and superclass names and the
// unquoted name of the editor
#define mlRegisterGroupClassWithEditors(GROUP,SUPERCLASS,EDITOR,CONTENT_EDITOR) \
    static int _mlGroupClassInited = 0; \
    if ( _mlGroupClassInited ) return; \
    _mlGroupClassInited = 1; \
    SUPERCLASS::initClass(); \
    new MleGroupClass(#GROUP,_mlCreate##GROUP,#SUPERCLASS, #EDITOR, #CONTENT_EDITOR)

#endif /* MLE_DIGITAL_WORKPRINT */
#ifdef MLE_DIGITAL_PLAYPRINT

// Null macros for non-rehearsal - see above for description.

// Need a statement so trailing semicolon will work.
#define MLE_GROUP_HEADER(C) \
	friend class _mleDummy
#define MLE_GROUP_SOURCE(C,S) \
	MleGroup *_mlCreate##C(void) { return new C; }

#endif /* MLE_DIGITAL_PLAYPRINT */

#endif /* __MLE_GROUP_H_ */

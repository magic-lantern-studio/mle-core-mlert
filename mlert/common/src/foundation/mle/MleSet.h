/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MleSet.h
 * @ingroup MleFoundation
 *
 * @author Mark S. Millard
 * @created May 1, 2003
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

#ifndef __MLE_SET_H_
#define __MLE_SET_H_

// Include system header files.
#include "mle/mlTypes.h"

// Include Runtime Engine header files.
#include "mle/MleRuntime.h"
#include "mle/MleObject.h"

// New Set Property definitions (to remove legacy offsetof implementation).
#define MLE_SET_PROPERTY(TYPE,NAME,GETTER,SETTER)         \
  public:                                                 \
    TYPE NAME;                                            \
    virtual TYPE GETTER() {return NAME;}                  \
    virtual void SETTER(TYPE NAME) {this->NAME = NAME;}   \
    int NAME##IsOfType(const char *type) {                \
        if (strcmp(#TYPE, type) == 0) return 1;           \
        else return 0; }

#define MLE_SET_DYNAMIC_PROPERTY(TYPE,NAME,GETTER,SETTER) \
  public:                                                 \
    TYPE NAME;                                            \
    virtual const TYPE GETTER() {return NAME;}            \
    virtual void SETTER(const TYPE value, size_t size) {  \
        if (this->NAME != NULL) mlFree(this->NAME);       \
        this->NAME = (TYPE) mlMalloc(size);               \
        memcpy(this->NAME, value, size); }                \
    int NAME##IsOfType(const char *type) {                \
        if (strcmp(#TYPE, type) == 0) return 1;           \
        else return 0; }

#define MLE_SET_ARRAY_PROPERTY(TYPE,NAME,GETTER,SETTER)    \
  public:                                                  \
    MleArray<TYPE> NAME;                                   \
    virtual const MleArray<TYPE> *GETTER() {return &NAME;} \
    virtual void SETTER(MleArray<TYPE> &value) {           \
        this->NAME.resize(value.size());                   \
        for (int i = 0; i < value.size(); i++)             \
            this->NAME[i] = value[i]; }                    \
    int NAME##IsOfArrayType(const char *type) {            \
        if (strcmp(#TYPE, type) == 0) return 1;            \
        else return 0; }

#define MLE_SET_PROPERTY_SOURCE()                                                        \
  public:                                                                                \
    static void getProperty(MleObject *object, const char *name, unsigned char **value); \
	static void setProperty(MleObject *object, const char *name, unsigned char *value);

class MleRole;
class MleActor;

#ifdef MLE_REHEARSAL

// Include system header files.
#include <string.h>	// Used for isa().

#include "mle/mlAssert.h"
#include "mle/MleSetClass.h"

#include "mle/DwpStrKeyDict.h"

class MleDwpDataUnion;

// MleSet Rehearsal-only macro.
//
// This macro should be placed in the class declaration of any subclass
// of MleSet.  It defines some basic functions needed by every subclass.
// The single argument is the unquoted class name.  Be aware that
// this macro switches subsequent declarations to public.
#define MLE_SET_HEADER(C) \
  public: \
    virtual const char *getTypeName(void) const; \
    virtual int isa(const char *type) const; \
    static C *cast(MleSet *set); \
    static void initClass(void)

// MleSet Rehearsal-only macro.
//
// This macro should be placed in the implementation of any subclass of
// MleSet.  It defines some basic functionality needed by every subclass.
// The single argument is the unquoted class name.
#define MLE_SET_SOURCE(C,S) \
    const char *C::getTypeName(void) const { return #C; } \
    int C::isa(const char *type) const { \
        if ( !strcmp(type,#C) ) return 1; \
        else return S::isa(type); \
    } \
    C *C::cast(MleSet *set) { \
        MLE_ASSERT(set->isa(#C)); \
        return (C *)set; \
    } \
    MleSet *_mlCreate##C(void) { return new C; }

// MleSet Rehearsal-only macro.
//
// This macro should be used instead of MLE_SET_HEADER for abstract classes.
#define MLE_SET_ABSTRACT_HEADER(C) \
  public: \
    virtual const char *getTypeName(void) const = 0; \
    virtual int isa(const char *type) const; \
    static C *cast(MleSet *set); \
    static void initClass(void)

// MleSet Rehearsal-only macro.
//
// This macro should be used instead of MLE_SET_SOURCE for abstract classes.
#define MLE_SET_ABSTRACT_SOURCE(C,S) \
    int C::isa(const char *type) const { \
        if ( !strcmp(type,#C) ) return 1; \
	else return S::isa(type); \
    } \
    C *C::cast(MleSet *set) { \
	MLE_ASSERT(set->isa(#C)); \
	return (C *)set; \
    } \
    MleSet *_mlCreate##C(void) { return NULL; }


// Every subclass of MleSet must implement a static initClass() member
// function which registers itself with the system.  This registration
// is required for rehearsal runtime and the mastering process.  The
// initClass function should be implemented inside a #ifdef MLE_REHEARSAL
// block so it is not compiled for the runtime.
//
// Inside the function, the macro mlRegisterSetClass should be invoked.
// This registers the set class with the system so it can be created.
// Then the macro mlRegisterSetMember should be called for each member
// variable that is potentially to have instance values kept in the
// workprint.

// MleSet Rehearsal-only macro.
//
// This macro takes the unquoted set class and superclass names.
#define mlRegisterSetClass(SET,SUPERCLASS) \
    static int _mlSetClassInited = 0; \
    if ( _mlSetClassInited ) return ; \
    _mlSetClassInited = 1; \
    SUPERCLASS::initClass(); \
    new MleSetClass(#SET,_mlCreate##SET,#SUPERCLASS)

// MleSet Rehearsal-only macro.
//
// This macro takes the unquoted set class and superclass names and the
// unquoted name of the editor
#define mlRegisterSetClassWithEditors(SET,SUPERCLASS,EDITOR,CONTENT_EDITOR) \
    static int _mlSetClassInited = 0; \
    if ( _mlSetClassInited ) return; \
    _mlSetClassInited = 1; \
    SUPERCLASS::initClass(); \
    new MleSetClass(#SET,_mlCreate##SET,#SUPERCLASS, #EDITOR, #CONTENT_EDITOR)

// MleSet Rehearsal-only macro.
//
// This macro takes the unquoted set class name, the unquoted member
// variable name, and the unquoted member variable type.
#if 0
#define mlRegisterSetMember(SET,MEMBER,TYPE) \
    MleSetClass::find(#SET)->addMember( \
		#MEMBER,#TYPE,((char *)&((SET *)0)->MEMBER) - (char *)0)
#else
#define mlRegisterSetMember(SET,MEMBER,TYPE) \
	MlePropertyEntry *entryFor##MEMBER = new MlePropertyEntry(); \
	entryFor##MEMBER->name = #MEMBER; \
	entryFor##MEMBER->getProperty = SET::getProperty; \
	entryFor##MEMBER->setProperty = SET::setProperty; \
    MleSetClass::find(#SET)->addMember(#MEMBER,#TYPE,entryFor##MEMBER)
#endif
#else /* MLE_REHEARSAL */

// Make a dummy statement to avoid errors with the trailing semicolon.
#define MLE_SET_HEADER(C) \
  public: \
    static C *cast(MleSet *forum) { return (C *)forum; } \
    friend class _mleDummy
#define MLE_SET_ABSTRACT_HEADER(C) \
	friend class _mleDummy
#define MLE_SET_SOURCE(C,S) \
    MleSet *_mlCreate##C(void) { return new C; }
#define MLE_SET_ABSTRACT_SOURCE(C,S)


#endif /* MLE_REHEARSAL */

/**
 * @brief Base class for Sets
 */
class MLE_RUNTIME_API MleSet : public MleObject
{
  // Declare member variables.

  public:

    /**
	 * This static holds the current forum, which is valid during a group
     * load.  Actors or roles that are instantiated by a
     * group load may refer to this variable only in their init() function
     * to get a pointer to the forum that contains them.
	 */
    static MleSet *g_currentSet;


  // Declare member functions.

  public:

    /**
     * @brief Default property get accessor.
     *
     * Does nothing since there aren't any default properties to get.
     *
     * @param object A pointer to the Object container instance.
     * @param name A pointer to the name of the property to get.
     * @param value An output parameter for returning the property value.
     */
    static void getProperty(MleObject *object, const char *name, unsigned char **value);

    /**
      * @brief Default property set accessor.
      *
      * Does nothing since there aren't any default properties to set.
      *
      * @param object A pointer to the Object container instance.
      * @param name A pointer to the name of the property to set.
      * @param value An input parameter for setting the property value.
      */
	static void setProperty(MleObject *object, const char *name, unsigned char *value);

	/**
	 * @brief Default constructor.
	 */
    MleSet(void);

	/*
	 * @brief Destructor.
	 */
    virtual ~MleSet(void);
    
	/**
	 * @brief Initialize the set.
	 *
     * This function is a hook to do any initialization
     * Typically, the set may schedule itself here.
     * The base class implementation does nothing.
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

	/**
	 * @brief Attach the roles to the set.
	 *
     * The implementations of this function will generally cast the 
     * MleRole* arguments to the type of roles which the given 
     * forum use and then perform role specific operations to perform
     * the attach.
	 *
	 * @param parent The parent MleRole of the specified child.
	 * @param child A pointer the the chile MleRole to attach to
	 * the specified parent.
	 */
    virtual void attach(MleRole* parent, MleRole* child);

#ifdef MLE_REHEARSAL
  public:

    // Set identity.

    // getTypeName() returns the class name of a set instance.
    virtual const char *getTypeName(void) const;
	// isa() returns nonzero if the instance is of the specified class.
    virtual int isa(const char *type) const;
    static void initClass(void);

    // Get the set class.
    // This function returns the (cached) MleSetClass object for
    // this set.  It is not const because of the caching.
    const MleSetClass *getClass(void);

    // Set a property value.
    // poke() puts the value of a property directly into set memory.
    // The set is used to determine an offset for the data that
    // comes from a data union object.
    int poke(const char *property,MleDwpDataUnion *value);

    // Update the forum after editing.
    // This function synchronizes the set with any property changes
    // that might be made as a result of editing.  The argument is the
    // name of the changed property - if NULL, then the set should
    // assume that all properties may have changed.  This does nothing
    // in the base class.
    virtual void resolveEdit(const char *property = NULL);

    // Getting the set name.
    char* getName() const { return m_name; }

    // Change the set name, which also causes re-registration under
    // new name.
    virtual void setName(char *newName);

    // Registry functions. Registers a newly created instance (currently
    // only instances from loadGroups - not new'ed from author code).
    // This will give the tools a way to search for particular instances
    static MleDwpStrKeyDict* getInstanceRegistry()  { return &g_instanceRegistry; }
    virtual void registerInstance(const char* name);
    virtual void unregisterInstance();

    // Support for picking - x,y in window coords
    virtual MleActor* pick(int x, int y);

    // Getting attributes of the stage.  These methods should be abstract.
    virtual const char** getFunctions();
    virtual const char** getFunctionAttributes(char* functionName);

    // Tools functions that should be abstract
    // actor layering.
    virtual int pushActor(MleActor* actor);
    virtual int pushActorToBottom(MleActor* actor);
    virtual int popActor(MleActor* actor);
    virtual int popActorToTop(MleActor* actor);

    // Actor render modes.
    virtual void setRenderMode(MleActor* actor, char* mode);
    virtual char* getRenderMode(MleActor* actor);

    // Global render mode.
    virtual void setRenderMode(char* mode);
    virtual char* getRenderMode();

  protected:

    // This is the registry for sets at rehearsal.  When sets
    // are created, they are entered into the registry with a
    // name so that cast loads can reference them.
    static MleDwpStrKeyDict g_instanceRegistry;

    // Set class.
    const MleSetClass *m_setClass;

  private:

    // Instance name of a set.
    char *m_name;

#endif /* MLE_REHEARSAL */
};

#endif /* __MLE_SET_H_ */

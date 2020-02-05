/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MleActor.h
 * @ingroup MleFoundation
 *
 * @author Mark S. Millard
 * @date May 1, 2003
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

#ifndef __MLE_ACTOR_H_
#define __MLE_ACTOR_H_

// Include system header files.
#include <stdio.h>
#include <string.h>

// Include Magic Lantern header files.
#include "mle/mlAssert.h"
//#include "math/scalar.h"

#include "mle/MleRuntime.h"
#include "mle/MleObject.h"

#ifdef MLE_DIGITAL_WORKPRINT

// Include additional Magic Lantern header files.
#include "mle/mlErrno.h"
#include "mle/MleActorClass.h"
#include "mle/MlePropDataset.h"
#include "mle/MlePtrArray.h"
#include "mle/MleMonitor.h"

// Include Digital Workprint header files.
#include "mle/DwpStrKeyDict.h"


class MleDwpDataUnion;
class MlTransform;


// MleActor Rehearsal-only constant.
//
// Support for translation for MLE_PROP_DATASET_TRANSFORM dataset.
// Returned by MleActor::hasTransform().
#define MLE_ACTOR_TRANSLATION 0x1

// MleActor Rehearsal-only constant.
//
// Support for rotation for MLE_PROP_DATASET_TRANSFORM dataset.
// Returned by MleActor::hasTransform().
#define MLE_ACTOR_ROTATION 0x2

// MleActor Rehearsal-only constant.
//
// Support for scaling for MLE_PROP_DATASET_TRANSFORM dataset.
// Returned by MleActor::hasTransform().
#define MLE_ACTOR_SCALE 0x4

// MleActor Rehearsal-only constant.
//
// Support for translation, rotation and scaling
// for MLE_PROP_DATASET_TRANSFORM dataset.
// Returned by MleActor::hasTransform().
#define MLE_ACTOR_AFFINE 0x7

#endif /* MLE_DIGITAL_WORKPRINT */

// New Actor Property definitions (to remove legacy offsetof implementation).
#define MLE_ACTOR_PROPERTY(TYPE,NAME,GETTER,SETTER)         \
  public:                                                   \
    TYPE NAME;                                              \
    virtual TYPE GETTER() {return NAME;}                    \
    virtual void SETTER(TYPE value) {this->NAME = value;}   \
    int NAME##IsOfType(const char *type) {                  \
        if (strcmp(#TYPE, type) == 0) return 1;             \
        else return 0; }

#define MLE_ACTOR_DYNAMIC_PROPERTY(TYPE,NAME,GETTER,SETTER) \
  public:                                                   \
    TYPE NAME;                                              \
    virtual const TYPE GETTER() {return NAME;}              \
    virtual void SETTER(const TYPE value, size_t size) {    \
        if (this->NAME != NULL) mlFree(this->NAME);         \
        this->NAME = (TYPE) mlMalloc(size);                 \
        memcpy(this->NAME, value, size); }                  \
    int NAME##IsOfType(const char *type) {                  \
        if (strcmp(#TYPE, type) == 0) return 1;             \
        else return 0; }

#define MLE_ACTOR_ARRAY_PROPERTY(TYPE,NAME,GETTER,SETTER)   \
  public:                                                   \
    MleArray<TYPE> NAME;                                    \
    virtual const MleArray<TYPE> *GETTER() {return &NAME;}  \
    virtual void SETTER(MleArray<TYPE> &value) {            \
        this->NAME.resize(value.size());                    \
        for (int i = 0; i < value.size(); i++)              \
            this->NAME[i] = value[i]; }                     \
    int NAME##IsOfArrayType(const char *type) {             \
        if (strcmp(#TYPE, type) == 0) return 1;             \
        else return 0; }

#define MLE_ACTOR_PROPERTY_SOURCE()                                                      \
  public:                                                                                \
    static void getProperty(MleObject *object, const char *name, unsigned char **value); \
	static void setProperty(MleObject *object, const char *name, unsigned char *value);


// Forward declaration.
class MleRole;

#ifdef MLE_DIGITAL_WORKPRINT

// MleActor DWP-only macro.
// 
// This macro should be placed in the class declaration of any subclass
// of MleActor.  It defines some basic functions needed by every subclass.
// The Magic Lantern authoring tools use these functions as
// an runtime interface to monitor and control actor subclasses
// and actor instances. C must be the unquoted class name.
// If your class is an abstract class (i.e., a non-instantiable
// class), you should instead use MLE_ACTOR_ABSTRACT_HEADER().
//
// Example:
//
//      MLE_ACTOR_HEADER(MyActor);
//
// Note: This macro switches subsequent declarations to 'public.'
#define MLE_ACTOR_HEADER(C) \
  public: \
    virtual const char *getTypeName(void) const; \
    virtual int isa(const char *type) const; \
    static C *cast(MleActor *actor); \
    static void initClass(void)

// MleActor DWP-only macro.
// 
// This macro should be placed in the implementation of any subclass of
// MleActor.  It defines some basic functionality needed by every subclass.
// The single argument is the unquoted class name.
// If your class is an abstract class (i.e., a non-instantiable
// class), you should instead use MLE_ACTOR_ABSTRACT_SOURCE().
//
// Example:
//
//      MLE_ACTOR_SOURCE(MyActor, MleActor);
//
#define MLE_ACTOR_SOURCE(C,S) \
    const char *C::getTypeName(void) const { return #C; } \
    int C::isa(const char *type) const { \
	if ( !strcmp(type,#C) ) return 1; \
	else return S::isa(type); \
    } \
    C *C::cast(MleActor *actor) { \
	MLE_ASSERT(actor->isa(#C)); \
	return (C *)actor; \
    } \
    MleActor *_mlCreate##C(void) { return new C; }

// MleActor DWP-only macro.
//
// This macro should be used instead of MLE_ACTOR_HEADER for abstract classes.
#define MLE_ACTOR_ABSTRACT_HEADER(C) \
  public: \
    virtual const char *getTypeName(void) const = 0; \
    virtual int isa(const char *type) const; \
    static C *cast(MleActor *actor); \
    static void initClass(void)

// MleActor DWP-only macro.
//
// This macro should be used instead of MLE_ACTOR_SOURCE for abstract classes
#define MLE_ACTOR_ABSTRACT_SOURCE(C,S) \
    int C::isa(const char *type) const { \
	if ( !strcmp(type,#C) ) return 1; \
	else return S::isa(type); \
    } \
    C *C::cast(MleActor *actor) { \
	MLE_ASSERT(actor->isa(#C)); \
	return (C *)actor; \
    } \
    MleActor *_mlCreate##C(void) { return NULL; }

// MleActor DWP-only macro.
//
// This macro should be used inside your actor's initClass()
// member function. You are required to register your actor's
// class name with the Magic Lantern authoring tools by using
// this macro. ACTOR should be the unquoted actor class name.
// SUPERCLASS should be the actor class's direct superclass name.
//
// Example:
//
//       mleRegisterActorClass(MyActor, MleActor);
//
#define mleRegisterActorClass(ACTOR,SUPERCLASS) \
    static int _mleActorClassInited = 0; \
    if ( _mleActorClassInited ) return; \
    _mleActorClassInited = 1; \
    SUPERCLASS::initClass(); \
    new MleActorClass(#ACTOR,_mlCreate##ACTOR,#SUPERCLASS)

// MleActor DWP-only macro.
//
// ACTOR must be the unquoted actor class name. SUPERCLASS must be
// the actor class's direct superclass name. EDITOR should be
// the name of the editor. (xxx this needs better documentation)
#define mleRegisterActorClassWithEditors(ACTOR,SUPERCLASS,EDITOR,CONTENT_EDITOR) \
    static int _mleActorClassInited = 0; \
    if ( _mleActorClassInited ) return; \
    _mleActorClassInited = 1; \
    SUPERCLASS::initClass(); \
    new MleActorClass(#ACTOR,_mlCreate##ACTOR,#SUPERCLASS, #EDITOR, #CONTENT_EDITOR)

// MleActor DWP-only macro.
//
// ACTOR must be the unquoted actor class's name MEMBER must be
// the unquoted member variable name. TYPE must be the unquoted member
// variable type.
//
// Example:
//
//   mleRegisterActorMember(MyActor, myMemberVar, int);
//
#if 0
#define mleRegisterActorMember(ACTOR,MEMBER,TYPE) \
    MleActorClass::find(#ACTOR)->addMember( \
	#MEMBER,#TYPE,((char *)&((ACTOR *)0)->MEMBER) - (char *)0)
#else
#define mleRegisterActorMember(ACTOR,MEMBER,TYPE) \
	MlePropertyEntry *entryFor##MEMBER = new MlePropertyEntry(); \
	entryFor##MEMBER->name = #MEMBER; \
	entryFor##MEMBER->getProperty = ACTOR::getProperty; \
	entryFor##MEMBER->setProperty = ACTOR::setProperty; \
    MleActorClass::find(#ACTOR)->addMember(#MEMBER,#TYPE,entryFor##MEMBER)
#endif

// MleActor DWP-only macro.
//
// ACTOR must be the unquoted actor class name. MEMBER must be
// the unquoted member variable name. DATASET must be one of
// the supported dataset names.
//
// Example:
//
//   mleRegisterActorMemberDataset(MyActor, position, MLE_PROP_DATASET_TRANSFORM);
//
#define mleRegisterActorMemberDataset(ACTOR,MEMBER,DATASET) \
    MleActorClass::find(#ACTOR)->addMemberDataset(#MEMBER,DATASET)

#endif /* MLE_DIGITAL_WORKPRINT */
#ifdef MLE_DIGITAL_PLAYPRINT

// Null macros for non-rehearsal - see description for rehearsal player.

// Need a statement so trailing semicolon will work.
#define MLE_ACTOR_HEADER(C) \
    friend class _mleDummy
#define MLE_ACTOR_ABSTRACT_HEADER(C) \
    friend class _mleDummy
#define MLE_ACTOR_SOURCE(C,S) \
    MleActor *_mlCreate##C(void) { return new C; }
#define MLE_ACTOR_ABSTRACT_SOURCE(C,S)
#endif /* MLE_DIGITAL_PLAYPRINT */

/**
 * @brief MleActor is the base class for all Magic Lantern actors.
 *
 * This is the base class for all Magic Lantern actors.
 * MleActor provides the baseline runtime and rehearsal-time
 * functionality of the actors.
 * <p>
 * For runtime, MleActor simply provides the ability
 * to associate an actor instance to a role instance.
 * For rehearsal time, MleActor provides many hooks
 * that allow Magic Lantern authoring tools, like the
 * Scene Editor, to monitor, control and edit the actor
 * instance in its runtime environment.
 * <p>
 * Use init() to initialize the actor after the actor's
 * member variables (i.e., properties) have been
 * loaded by mleLoadGroup() or mleLoadScene(). Use getRole() 
 * to get the actor's role instance,
 * <p>
 * REHEARSAL PLAYER ONLY:
 * <p>
 * This section discusses issues that pertain to the actor's
 * API within the rehearsal environment. The rehearsal environment
 * code is  compiled by definining the MLE_REHEARSAL flag.
 * <p>
 * Unless you are developing your own tools or rehearsal-time
 * capabilities, or extending the rehearsal-time capabilities
 * of Magic Lantern, you will not need to use the rehearsal
 * player functions of MleActor. However, you do have to 
 * manually register certain information about your actor
 * classes using the interface described in the next paragraph.
 * <p>
 * When you define your own subclass or MleActor or extend
 * an existing MleActor subclass, in order to interface your
 * actor class to Magic Lantern, you need to follow the following
 * steps:
 * <ol>
 *   <li>Add a MLE_ACTOR_HEADER() or MLE_ACTOR_ABSTRACT_HEADER()
 *       macro call into your actor class definition in your
 *       actor header file.</li>
 * 
 *   <li>Add a MLE_ACTOR_SOURCE() or MLE_ACTOR_ABSTRACT_SOURCE()
 *       macro call into your actor class implementation file.</li>
 * 
 *   <li>Add an initClass() member function to your
 *       actor class implementation file.</li>
 * </ol>
 * Every subclass of MleActor must implement a static initClass() member
 * function which registers itself with the system.  This registration
 * is required for rehearsal runtime and for mastering purposes.  The
 * initClass function should be implemented inside an #ifdef MLE_REHEARSAL
 * block so it is not compiled for the runtime. initClass() is automatically
 * declared in your class header by the MLE_ACTOR_HEADER() macro.
 * Inside initClass(), the macro mleRegisterActorClass() must be invoked.
 * This registers the actor class with Magic Lantern so the class can be 
 * dynamically created at runtime in the rehearsal player environment.
 * Following that, the macro mleRegisterActorMember() can be optionally
 * used to register each member variable which you wish to publish
 * as a property (i.e., which can be edited from Magic Lantern authoring
 * tools and whose values can be set in workprints). Finally,
 * you can optionally use mleRegisterActorMemberDataset and
 * mleRegisterActorClassWithEditors(), depending on the type of
 * actor that you're implementing.
 * <p>
 * getName() and setName() are used to get and set the actor 
 * instance's name, respectively: all actor instances in the
 * rehearsal player must have a name. 
 * <p>
 * getTypeName() returns the actor's class name from an actor instance.
 * isa() is used to test whether an actor instance's class is
 * a subclass of another actor class. getClass() returns
 * a class that holds information about your actor class
 * needed by the authoring tools.
 * <p>
 * getPropNames(), getPropDataset() and setPropDataset() is a
 * generic interface that allows a set of actor properties
 * to be associated with a specific actor member variable.
 * This set of actor properties is called a "dataset" and
 * is assigned a dataset name. A list of available datasets
 * is provided in this document (xxx link). datasets are motivated
 * by the fact that change in value of certain properties
 * can propagate to other properties. Therefore, by making
 * these causal chains explicit, the Magic Lantern tools can
 * ensure that property values that it changes are properly
 * propagated to all other affected properties.
 * <p>
 * poke() is an internal used to insert property values directly
 * into the memory location of actor instance member
 * variables. This is used by the Digital WorkPrint loader.
 * <p>
 * getInstanceRegistry(), registerInstance() and unregisterInstance()
 * are used to directly access the actor registry. This registry
 * is used to associate actor instances with the actor names
 * (i.e., the names that can be accessed via getName() and
 * setName()).
 * <p>
 * hasTransform(), setTransform() and getTransform() are
 * used by the transform dataset interface.
 * 
 * @see MleRole
 */

class MLE_RUNTIME_API MleActor : public MleObject
{
  public:
    /**
     * MleRole is a friend of MleActor since MleActor's attachRole and
     * removeRole functions are private and protected, respectively.
	 */
    friend class MleRole;
    
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
	 *
     * This is the base actor class constructor. It initializes
     * the actor's role member to NULL.
	 */
    MleActor(void);
    
    /**
	 * @brief Destructor.
	 *
     * This is the base actor class destructor. 
     * It deletes the actor's role, if any.
     *
     * Rehearsal Player Note: the actor instance is
     * unregistered from the actor registry.
	 */
    virtual ~MleActor();
    
    /**
	 * @brief Get the actor's role.
	 *
     * Returns a pointer to the actor's role, if any.
     * Use getRole() to get the actor's role instance,
     * if any. When loading an actor instance using mleLoadGroup()
     * or mleLoadScene(), the role, if any, will be set
     * after the actor's constructor is called and before
     * the actor's init() function is called. Therefore,
     * the result of calling getRole() before init()
     * is called is undefined.
	 */
    MleRole* getRole(void) const { return m_role; }
    
    /**
	 * @brief Initialize the actor.
	 *
     * This function is a hook to do any initialization after property
     * values are inserted into the actor when the actor is directly loaded
     * by mleLoadGroup() or indirectly by mleLoadScene().
     * Typically, the actor may schedule itself or initialize its
     * role here.  The base init() function does nothing.
     * Use init() to initialize the actor after the actor's
     * member variables (i.e., properties) have been
     * loaded by mleLoadGroup() or mleLoadScene(). 
     * init() is called after the actor's data has
     * been loaded into memory, so this is the safest
     * time to perform initialization and synchronization
     * with the rest of the environment.
	 */
    virtual void init(void);
    
#ifdef MLE_DIGITAL_WORKPRINT

	/**
	 * @brief Get the actor's type name.
	 *
     * Rehearsal-only member function.
     *
     * @return Returns the actor's type name (a pointer to a string).
	 */
    virtual const char *getTypeName(void) const;

	/**
	 * @brief Determine whether the actor class is of the specified type.
	 *
     * Rehearsal-only member function.
	 *
	 * @param type The type to check.
     *
     * @return Returns a non-zero value if the actor class
     * inherits from type (a string that is the name 
     * of the actor superclass candidate).
	 */
    virtual int isa(const char *type) const;

    /**
	 * @brief Initialize the actor's class.
	 *
     * Rehearsal-only member function.
     *
     * This member function is used to: (1) register
     * the actor's class with the tools, (2) register the
     * member variables of the actor as Magic Lantern
     * properties which can be edited using the Scene Editor
     * or other Magic Lantern tools, (3) register the
     * actor's constructor with the Magic Lantern tools
     * so that actors can be dynamically created by the
     * tools, and (4) to register attributes of properties.
     * Corresponding to each of the above cases: (1) You use
     * mleRegisterActorClass() to register the actor's class.
     * This is required. (2) You use mleRegisterActorMember() 
     * to register actor member variables which you want 
     * to publish as properties. This is optional: you need
     * only do it for the member variables which you want
     * to publish to the tools. (3) This is done automatically
     * for you by the base initClass() member function.
     * (4) This is required for certain types of properties.
     * In Magic Lantern 1.0, you must use mleRegisterActorMemberDataset()
     * to register properties which affect the actor's 2d or
     * 3d transform. (See the definitions of these functions
     * for further details.)
	 */
    static void initClass(void);
    
    /**
	 * @brief Get the actor class.
	 *
     * Rehearsal-only member function.
     *
     * Returns the actor class "holder." The actor class holder
     * is an instance of MleActorClass which is used by the
     * stage to contain information about your actor class.
     * See the API for MleStage for more information on
     * how this class is used. (Note: this information is cached, 
     * so it does not return a constant.)
	 */
    const MleActorClass *getClass(void);
    
    /**
	 * @brief Sets a property value..
	 *
     * Rehearsal-only member function.
     *
     * poke() inserts the value of a property 
     * directly into actor memory (i.e., at the memory location
     * of the appropriate member variable).
	 *
	 * @param property The name of the property being poked:
	 * it is used to determine an offset into the property's member
	 * variable in the actor instance.
	 * @param value A union object which contains the
     * data to be poked, which can take on various types depending
     * on the type of the property itself.
	 *
	 * @return
	 */
    int poke(const char *property,MleDwpDataUnion *value);
    
    /**
	 * Resolve changes to properties.
	 *
     * Rehearsal-only member function.
     *
     * Updates the actor after editing it through Magic Lantern tools.
     * This function synchronizes the actor with any property changes
     * that might be made as a result of editing (e.g., via the
     * Actor Editor).
	 *
	 * @param property The name of the changed property: 
     * if it is NULL, then the actor should assume that all properties
     * may have changed.  The base function resolveEdit() does nothing.
	 */
    virtual void resolveEdit(const char *property = NULL);
    
    /**
	 * Get the name of the actor.
	 *
     * Rehearsal-only member function.
     *
     * @return Returns an actor's name. This is the name by which the
     * actor is known in the digital Workprint.
	 */
    char* getName() const { return m_name; }

    /**
	 * Set the name of the actor.
	 *
     * Rehearsal-only member function.
     *
     * Changes an actor's name. The actor's old name is unregistered
     * and the new name is registered. setName() allocates
     * its own memory for the actor name, so setName()'s caller
     * is responsible for deallocating newName.
	 *
	 * @param newName The new name of the actor.
	 */
    virtual void setName(char *newName);
    
    /**
	 * Get the list of property names.
	 *
     * Rehearsal-only member function.
	 *
	 * @param propDatasetName  The data set name should be
     * one of the valid names of a property dataset used with 
     * mleRegisterActorMemberDataset() (e.g., MLE_PROP_DATASET_TRANSFORM).
     *
     * @returns Returns an array of property names.
     * In the returned MlePtrArray*, each item is a char* with the
     * name of each property registered with that dataset.
	 */
    virtual MlePtrArray *getPropNames(const char *propDatasetName) const;

    /**
	 * @brief Get the property data set for the specified name.
	 *
     * Rehearsal-only member function.
     *
     * Returns the data structure which is affected by a dataset.
     * The type of this data structure depends on the dataset name:
     * for example, for the dataset MLE_PROP_DATASET_TRANSFORM, the
     * data argument will be set by getPropertDataset to an instance
     * of MlTransform. You should allocate space for the data pointer
     * before passing it in.
	 *
	 * @param propDatasetName The name of the data set.
	 * @param data The returned property data set.
	 *
	 * @return
	 */
    virtual MlErr getPropDataset(const char *propDatasetName, void *data);

    /**
	 * @brief Set the property data set for the specified name.
	 *
     * Rehearsal-only member function.
     *
     * Sets a specific dataset data structure for the actor.
     *
	 * @param propDatasetName is the name of the dataset.
	 * @param data The appropriate data structure to set in the actor.
     * For example, if proDatasetName is MLE_PROP_DATASET_TRANSFORM,
     * then data would be of type MlTransform*.
	 *
	 * @return
	 */
    virtual MlErr setPropDataset(const char *propDatasetName, void *data);

    /**
	 * Get the registry for actor instances.
	 *
     * Rehearsal-only member function.
     *
     * This registry is used by the Magic Lantern authoring tools 
     * (e.g., Scene Editor) to correlate actor names with 
     * actual actor instances in the runtime player.
     * 
	 */
    static MleDwpStrKeyDict* getInstanceRegistry()  { return &g_instanceRegistry; }

    /**
	 * Register this actor with the actor registry.
	 *
	 * registerInstance() will copy name, so you are responsible
     * for deallocating the name string after calling registerInstance().
	 *
	 * @param name The name of the actor.
     */
    virtual void registerInstance(const char* name);

	/**
	 * Unregister this actor with the actor registry.
	 */
    virtual void unregisterInstance();

    /**
	 * @brief Determine whether this actor has a transform.
	 *
     * Rehearsal-only member function.
     *
     * Interface to Magic Lantern authoring tool graphical manipulators.
     * When an authoring tool manipulator on an actor is changed
     * by the tool user (e.g., the actor is dragged or rotated),
     * the manipulator interface is used to transmit the necessary
     * changes (e.g., a translation or rotation) to the actor's
     * transform matrix. This interface supports this capability.
     *
     * Design Note: The transform matrices can be 2d or 3d matrices, depending
     * on whether the actor is a 2d or 3d actor. MleActor is overloaded
     * for both 2d and 3d matrices; that is, instead of representing
     * 2d and 3d actors with a class like Mle2dActor and Mle3dActor, 
     * respectively, Magic Lantern subsumes the functionality of both
     * types in MleActor. This was done in order to keep the
     * inheritance hierarchy as flat as possible.
     *
     * hasTransform() determines what kinds of tool-based placement
     * this actor can support.  The return is a set of logically ORed
     * flags.  By default, the actor class is examined for some well-known
     * properties to implement transformation.  This function is not
     * const because it needs to call getClass(). Possible flags
     * returned by hasTransform() include:
	 * <ul>
     *      <li>MLE_ACTOR_TRANSLATION</li>
     *      <li>MLE_ACTOR_ROTATION</li>
     *      <li>MLE_ACTOR_AFFINE</li>
     *      <li>MLE_ACTOR_SCALE</li>
	 * </ul>
     * This interface is defined under MLE_REHEARSAL here, but it can be
     * exposed for subclasses.
	  */
    virtual int hasTransform(void);

    /**
	 * @brief Set the transform.
	 *
     * Rehearsal-only member function.
     *
     * Sets the transformation state of the actor to a new one,
     * from a tool.  The matrix sent down is subject to interpretation
     * with respect to what transformations this object supports.  By
     * default the actor class is examined for some well-known properties
     * to implement transformation.
     * This should be interpreted as a 2d matrix if the
     * actor is a 2d actor; else as a 3d matrix.
	 */
    virtual int setTransform(MlTransform& transform);

    /**
	 * @brief Get the transform.
	 *
     * Rehearsal-only member function.
     *
     * Returns the current transform state of the actor.
     * This should be interpreted as a 2d matrix if the
     * actor is a 2d actor; else as a 3d matrix.
	 */
    virtual void getTransform(MlTransform& transform);
    
#endif /* MLE_DIGITAL_WORKPRINT */

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
	 * @brief Remove the role from this actor.
	 *
     * Attached role provides notification when it is destroyed
     * This function made virtual to allow sub-class to provide 
     * notification to other classes, or for actor to possibly
     * delete self.  Only the MleRole destructor should call this
	 * method.
	 */
    virtual void removeRole();
    
	/**
	 * @brief Attach the specified role to this actor.
	 *
     * Can only add a role if the actor currently does not have one.  
     * Only the MleRole destructor shoul call this method.
	 *
	 * @param newRole The role to attach to this actor.
	 */
    virtual void attachRole(MleRole* newRole);
    //   { MLE_ASSERT(! m_role); m_role = newRole; }
    
#ifdef MLE_DIGITAL_WORKPRINT
    /** The actor instance registry. */
    static MleDwpStrKeyDict g_instanceRegistry; 
    
    /** The actor class. */
    const MleActorClass *m_actorClass;    
#endif /* MLE_DIGITAL_WORKPRINT */

  private:

	// The actor's role.
    MleRole* m_role;

#ifdef MLE_DIGITAL_WORKPRINT
	// The name of the actor.
    char* m_name;
#endif /* MLE_DIGITAL_WORKPRINT */

};

#endif /* __MLE_ACTOR_H_ */

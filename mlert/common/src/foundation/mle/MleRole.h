/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MleRole.h
 * @ingroup MleFoundation
 *
 * @author Mark S. Millard
 * @created May 1, 2003
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

#ifndef __MLE_ROLE_H_
#define __MLE_ROLE_H_


// Include Magic Lantern header files.
#include "mle/MleRuntime.h"
#include "mle/MleActor.h"
#include "mle/MleSet.h"


/**
 * MleRole is the platform-specific code for an actor.
 *
 * MleRole implements the platform-specific code
 * for an actor. Each instance of MleActor can have
 * zero or one corresponding role instance.
 * At runtime, the only space overhead for MleRole
 * is a pointer to the role's actor instance.
 * <p>
 * getActor() returns the role instance's corresponding
 * actor instance. init() can be used to initialize the
 * role after it has been attached to its actor
 * and to the forum/role hierarchy.
 * <p>
 * MleRole instances are created by mleLoadGroup() and
 * mleLoadScene() when actors are loaded from
 * either the workprint or playprint. mleLoadGroup()
 * (which is called by mleLoadScene() goes through
 * the following sequence to initialize the role/
 * actor pair):
 * <ol>     
 *   <li>When an actor is found inside a group that
 *       is being loaded, the actor is created 
 *       (i.e., memory is allocated and its constructor is called).</li>
 * 
 *   <li>The values of the actor's properties
 *       in the workprint (or playprint) are inserted into the
 *       appropiate actor member variable locations in memory.</li>
 * 
 *   <li> If the workprint (or playprint) has a
 *        role class binding for the actor, then an
 *        instance of that role class is created
 *        (the role's constructor is called).</li>
 * 
 *   <li>The role instance is bound to its actor.</li>
 * 
 *   <li>The role is attached either to another
 *       role or to the current forum, depending
 *       on whether the role class supports
 *       role-to-role attachment. addChild() is
 *       used to add a role to another role
 *       whenever the roles support attachment.</li>
 * 
 *   <li>The role's actor init() member function
 *       is called. This allows the actor to do
 *       final initialization since all of its data
 *       has been loaded and its role has been
 *       attached. The role actor might at this
 *       point call the role's init() function.</li>
 * </ol>
 * <p>
 * MleRole's instance variable forum points to the
 * role instance's forum (i.e., the forum to which
 * it is directly or indirectly attached).
 * <p>
 * REHEARSAL PLAYER
 * <p>
 * getTypeName() and isa() allow you to query the
 * role's class name and position in the role
 * inheritance hierarchy.
 *  <p>
 * setIdentifier() sets the role name's into the
 * rehearsal-time role registry. This name is 
 * thus associate with the role instance for
 * authoring tool lookups. The instance variable pickRegistry
 * contains that registry.
 * 
 * @see MleActor
 * @see Mle2dRole
 * @see Mle3dRole.
 */

class MLE_RUNTIME_API MleRole
{
  // Declare member variables.

  public:

    /**
     * Points to the role's set.
	 */
    MleSet *m_set;

  private:

    MleActor* const m_actor;


  // Declare member functions.

  public:

    /**
	 * @brief A constructor that attaches a role to the specifed actor.
	 *
     * MleRole constructor sets the actor to point to this role  
     * because both sides of the link must be maintained at all times.
	 *
	 * @param theActor A pointer to the MleActor that will be associated
	 * with this role.
	 */
    MleRole(MleActor* theActor) 
      : m_actor(theActor)
    {
        m_actor->attachRole(this);
        m_set = MleSet::g_currentSet;
    }

    /**
	 * @brief Destructor.
	 *
     * Base destructor function calls the role actor's 
     * removeRole() function to notify the actor
     * that it's role should be removed. This protocol
     * allows the role's actor to perform any other
     * necessary actions at the time its role is
     * actually deleted.
	 */
    virtual ~MleRole();

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

#ifdef MLE_REHEARSAL
	/**
	 * Get the name of the role's class.
	 *
     * Rehearsal-only function
     *
     * @return Returns the name of the role's class.
	 */
    virtual const char *getTypeName(void) const;

	/**
	 * Determine if role is a subclass of the specified superclass.
	 *
     * Rehearsal-only function
     *
	 * @param type  The name of the candidate superclass.
	 *
     * @return  Returns <b>TRUE</b> when type is a superclass of
     * the role's class. Otherwise <b>FALSE</b> will be returned.
	 */
    virtual int isa(const char *type) const;

	/**
	 * Set the role's unique identifier.
	 *
     * Rehearsal-only function
     *
     * This virtual function is overloaded by
     * subclasses of MleRole in order to insert
     * the role name into the role registry.
     * The base function does nothing.
	 */
    virtual void setIdentifier(const char* id);

    // This is dictionary maintained at rehearsal time for roles
    // to enable picking on themselves.
    static MleDwpDict g_pickRegistry;
#endif /* MLE_REHEARSAL */

    /**
	 * @brief Get the role's associated actor.
	 *
     * Returns the actor associated with this role.
     * getActor() should not be called inside the role's
     * constructor because the role's actor might not be necessarily
     * bound to the role when the role's constructor is called.
     *
	 * @return A pointer to the MleActor is returned.
	 */
    MleActor* getActor(void) const
    { return m_actor; }

    /**
	 * @brief Initialize the role.
	 *
     * init() is a virtual to initialize the role.  This may
     * involve initial data "pull" from the actor, scheduling, etc.
     * The base class implementation does nothing.
	 */
    virtual void init(void);

    /**
	 * @brief Add a child to this role.
	 *
     * addChild() is a virtual function to attach other roles
     * to this role.  It should be overridden by subclasses 
     * of MleRole for which there's a semantic policy for attachment.
     * The base function does nothing.
	 *
	 * @param child A pointer to the MleRole to add as a child.
	 */
    virtual void addChild(MleRole *child);

	/**
	 * @brief Initialize the class.
	 */
    static void initClass(void) {}


  private:

	  // Hide the default constructor.
	  MleRole() : m_actor(NULL) {};
};

#ifdef MLE_REHEARSAL

#include <string.h>

#include "mle/mlAssert.h"
#include "mle/MleRoleClass.h"

// MleRole Rehearsal-only macro.
// 
// This macro should be placed in the class declaration of a role.
// It defines some basic functionality needed by every subclass. 
// C is the unquoted class name.
//
// Example:
//
//      MLE_ROLE_HEADER(MyRole);
//
// Note: This macro switches subsequent declarations to 'public.'
#define MLE_ROLE_HEADER(C) \
	public: \
		virtual const char *getTypeName(void) const; \
		virtual int isa(const char *type) const; \
		static C *cast(MleRole *role); \
		static void initClass(void) 

// MleRole Rehearsal-only macro.
// 
// This macro should be placed in the implementation of any abstract
// subclass of MleRole, i.e. any subclass that won't (can't) be
// instanced.  C should be the unquoted class name and
// S should be the unquoted superclass name.
//
// Example:
//
//    MLE_ROLE_ABSTRACT_SOURCE(MyRoleClass, Mle2dRole);
//
#define MLE_ROLE_ABSTRACT_SOURCE(C,S) \
    const char *C::getTypeName(void) const { return #C; } \
	int C::isa(const char *type) const { \
		if ( !strcmp(type,#C) ) return 1; \
		else return S::isa(type); \
	} \
	C *C::cast(MleRole *role) { \
		MLE_ASSERT(role->isa(#C)); \
		return (C *)role; \
	} \
	void C::initClass(void) { \
		static int mlRoleClassInited = 0; \
		if ( mlRoleClassInited ) return; \
		mlRoleClassInited = 1; \
		S::initClass(); \
	}
	
// MleRole Rehearsal-only macro.
// 
// This macro should be placed in the implementation of any concrete
// subclass of MleRole, i.e. any subclass that will be instanced.
// The arguments are the unquoted class name and superclass name.
//
// Example:
//
//   MLE_ROLE_SOURCE(MyRoleClass, Mle2dRole);
//
#define MLE_ROLE_SOURCE(C,S) \
    const char *C::getTypeName(void) const { return #C; } \
	MleRole *_mlCreate##C(MleActor *a); MleRole *_mlCreate##C(MleActor *a) { return new C(a); } \
	int C::isa(const char *type) const { \
		if ( !strcmp(type,#C) ) return 1; \
		else return S::isa(type); \
	} \
	C *C::cast(MleRole *role) { \
		MLE_ASSERT(role); \
		MLE_ASSERT(role->isa(#C)); \
		return (C *)role; \
	} \
	void C::initClass(void) { \
		static int mlRoleClassInited = 0; \
		if ( mlRoleClassInited ) return; \
		mlRoleClassInited = 1; \
		S::initClass(); \
		MleRoleClass::add(#C,_mlCreate##C); \
	}

#else /* MLE_REHEARSAL */

/* Define null macros for non-rehearsal players. */

/* Make a dummy statement to avoid errors with the trailing semicolon. */

#define MLE_ROLE_HEADER(C) \
    public: \
        static C *cast(MleRole *role) { return (C *)role; } \
        friend class _mleDummy
#define MLE_ROLE_ABSTRACT_SOURCE(C,S)
#define MLE_ROLE_SOURCE(C,S) \
    MleRole *_mlCreate##C(MleActor *a) { return new C(a); }

#endif /* MLE_REHEARSAL */

#endif /* __MLE_ROLE_H_ */

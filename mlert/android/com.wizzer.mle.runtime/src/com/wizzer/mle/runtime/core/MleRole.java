/**
 * @file MleRole.java
 * Created on April 30, 2003. (msm@wizzerworks.com)
 */

//COPYRIGHT_BEGIN
//
//  Copyright (C) 2000-2007  Wizzer Works
//
//  Wizzer Works makes available all content in this file ("Content").
//  Unless otherwise indicated below, the Content is provided to you
//  under the terms and conditions of the Common Public License Version 1.0
//  ("CPL"). A copy of the CPL is available at
//
//      http://opensource.org/licenses/cpl1.0.php
//
//  For purposes of the CPL, "Program" will mean the Content.
//
//  For information concerning this Makefile, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
//COPYRIGHT_END

//Declare package.
package com.wizzer.mle.runtime.core;

// Import standard Java packages.

// Import Magic Lantern packages.
import com.wizzer.mle.runtime.core.MleActor;
//import com.wizzer.mle.runtime.dpp.MleDppLoader;


/**
 * <code>MleRole</code> is a class that is used to
 * specify the platform-specific interface of an actor.
 * <p>
 * MleRole implements the platform-specific code
 * for an actor. Each instance of MleActor can have
 * zero or one corresponding role instance.
 * At runtime, the only space overhead for MleRole
 * is a reference to the role's actor instance.
 * </p><p>
 * getActor() returns the role instance's corresponding
 * actor instance. init() can be used to initialize the
 * role after it has been attached to its actor
 * and to the set/role hierarchy.
 * </p><p>
 * MleRole instances are created by MleDppLoader.loadGroup() and
 * Mle.loadScene() when actors are loaded from
 * either the workprint or playprint. MleDppLoader.loadGroup()
 * (which is called by MleDppLoader.loadScene()) goes through
 * the following sequence to initialize the role/actor
 * pair:
 * </p><p>
 * <ol>
 *   <li> When an actor is found inside a group that
 *        is being loaded, the actor is created 
 *        (i.e., memory is allocated and its constructor is called).
 *   </li>
 *   <li> The values of the actor's properties
 *        in the workprint (or playprint) are inserted into the
 *        appropiate actor member variable locations in memory.
 *   </li>
 *   <li> If the workprint (or playprint) has a
 *        role class binding for the actor, then an
 *        instance of that role class is created
 *        (the role's constructor is called).
 *   </li>
 *   <li> The role instance is bound to its actor.
 *   </li>
 *   <li> The role is attached either to another
 *        role or to the current set, depending
 *        on whether the role class supports
 *        role-to-role attachment. addChild() is
 *        used to add a role to another role
 *        whenever the roles support attachment.
 *   </li>
 *   <li> The role's actor init() member function
 *        is called. This allows the actor to do
 *        final initialization since all of its data
 *        has been loaded and its role has been
 *        attached. The role actor might at this
 *        point call the role's init() function.
 *   </li>
 * </p><p>
 * MleRole's instance variable <i>m_set</i> points to the
 * role instance's set (i.e., the set to which
 * it is directly or indirectly attached).
 * </p>
 * 
 * @see MleActor
 * @see MleDppLoader#loadGroup
 * @see MleDppLoader#loadScene
 *
 * @author  Mark S. Millard
 * @version 1.0
 */
public class MleRole extends Object implements IMleRole
{
    /** A reference to the actor that is rendered by this role. */
    protected MleActor m_actor;

    /** A reference to the role's set. */
    protected MleSet m_set;

    /**
     * A factory method for creating a role.
     *
     * @return A new MleRole is returned.
     */
    public static MleRole _mlCreateMleRole(MleActor actor)
    {
    	return new MleRole(actor);
    }

    /**
     * The default constructor.
     */
    public MleRole()
    {
		// Place role in current set.
		m_set = MleSet.getCurrentSet();
    };

    /**
     * A constructor that is used to assign the associated actor.
     * <p>
     * MleRole constructor sets the actor to point to this role
     * because both sides of the link must be maintained at all times.
     * </p>
     *
     * @param actor The actor for this role.
     */
    public MleRole(MleActor actor)
    {
		m_actor = actor;
		actor.attachRole(this);
		
		// Place role in current set.
		m_set = MleSet.getCurrentSet();
    }
    
    /**
     * Set the actor for this role.
     * 
     * @param actor The actor to set.
     */
    public void setActor(MleActor actor)
    {
		m_actor = actor;
		actor.attachRole(this);        
    }

    /**
     * Get the actor for this role.
     * <p>
     * This method should not be called inside the role's
     * constructor because the role's actor might not be necessarily
     * bound to the role when the role's constructor is called.
     * </p>
     *
     * @return A reference to the actor for this role is returned.
     */
    public MleActor getActor()
    {
    	return m_actor;
    }

    /**
     * Initialize the role.
     * <p>
     * This method is a hook to initialize the role.  This may
     * involve initial data "pull" from the actor, scheduling, etc.
     * The base class implementation does nothing.
     * </p>
     */
    public void init() {}

	/**
	 * Dispose all resources associated with the role.
	 */
	public void dispose() {}

    /**
     * Add a child to this role.
     *
     * This method is used to attach other roles
     * to this role.  It should be overridden by sub-classes 
     * of MleRole for which there's a semantic policy for attachment.
     * The base class implemetation does nothing.
     */
    protected void addChild(MleRole child) {}

}

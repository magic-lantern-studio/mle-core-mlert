/**
 * @file MleActor.java
 * Created on April 30, 2003. (msm@wizzerworks.com)
 */

// COPYRIGHT_BEGIN
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
// COPYRIGHT_END

// Declare package.
package com.wizzer.mle.runtime.core;

// Import standard Java packages.
import java.util.HashMap;
import java.util.Vector;

//Import Magic Lantern packages.
import com.wizzer.mle.runtime.core.MleRole;
import com.wizzer.mle.runtime.event.IMlePropChangeListener;
import com.wizzer.mle.runtime.event.MlePropChangeEvent;


/**
 * <code>MleActor</code> is a class that encapsulates a title element
 * that is independent of a specific target platform.
 * <p>
 * This is the base class for all Magic Lantern actors.
 * MleActor provides the baseline runtime and rehearsal-time
 * functionality of the actors.
 * </p><p>
 * For runtime, MleActor simply provides the ability
 * to associate an actor instance to a role instance.
 * For rehearsal time, MleActor provides many hooks
 * that allow Magic Lantern authoring tools, like the
 * Scene Editor, to monitor, control and edit the actor
 * instance in its runtime environment.
 * </p><p>
 * Use init() to initialize the actor after the actor's
 * member variables (i.e., properties) have been
 * loaded by MleDppLoader.loadGroup() or MleDppLoader.loadScene().
 * Use getRole() to get the actor's role instance,
 * </p>
 *
 * @see MleRole
 * @see com.wizzer.mle.runtime.dpp.MleDppLoader#mleLoadGroup(int)
 * @see com.wizzer.mle.runtime.dpp.MleDppLoader#mleLoadScene(int)
 *
 * @author  Mark S. Millard
 * @version 1.0
 */
public abstract class MleActor extends Object implements IMleObject, IMleActor
{
    /** A reference to the role for this actor to play. */
    protected MleRole m_role = null;
    /** The collection of "PropChange" event listeners, per property. */
    protected HashMap<String,Vector<IMlePropChangeListener>> m_propChangeListeners;
    
    /**
     * The default constructor.
     */
    public MleActor()
    {
    	// The role should be set to null.
    	m_propChangeListeners = new HashMap<String,Vector<IMlePropChangeListener>>();
    }

    /**
     * Get the actor's associated role.
     * <p>
     * Returns a pointer to the actor's role, if any.
     * Use getRole() to get the actor's role instance,
     * if any. When loading an actor instance using MleDppLoader.loadGroup()
     * or MleLaod.loadScene(), the role, if any, will be set
     * after the actor's constructor is called and before
     * the actor's init() function is called. Therefore,
     * the result of calling getRole() before init()
     * is called is undefined.
     * </p>
     *
     * @return The actor's associcated role is returned.
     */
    public MleRole getRole()
    {
    	return m_role;
    }

    /**
     * Remove the role from the actor.
     * <p>
     * The attached role provides notification when it is destroyed.
     * This method is made protected to allow sub-class to provide 
     * notification to other classes, or for the actor to possibly
     * delete self.
     * </p><p>
     * Note that only the role instance should call this method
     * since it can clean-up the actor/role relationship.
     * </p>
     */
    public void removeRole()
    {
    	m_role = null;
    }
    
    /**
     * Attach a role to the actor.
     * <p>
     * A role can only be added if the actor currently does not have one. 
     * Note that only the role instance should call this method.
     * </p>
     *
     * @param role The role to be attached to this actor.
     */
    public void attachRole(MleRole role)
    {
		if (m_role == null)
	    	m_role = role;
    }

    /**
     * Initialize the actor.
     * <p>
     * This method is a hook to do any initialization after property
     * values are inserted into the actor when the actor is directly loaded
     * by MleDppLoader.loadGroup() or indirectly by MleDppLoader.loadScene().
     * Typically, the actor may schedule itself or initialize its
     * role here.  The base init() function does nothing.
     * Use init() to initialize the actor after the actor's
     * member variables (i.e., properties) have been
     * loaded by MleDppLoader.loadGroup() or MleDppLoader.loadScene(). 
     * init() is called after the actor's data has
     * been loaded into memory, so this is the safest
     * time to perform initialization and synchronization
     * with the rest of the environment.
     * </p>
     * 
     * @throws MleRuntimeException This exception is thrown if the
     * actor can not be successfully initialized.
     */
    public abstract void init() throws MleRuntimeException;

	/**
	 * Dispose all resources associated with the Actor.
     * 
     * @throws MleRuntimeException This exception is thrown if the
     * actor can not be successfully initialized.
	 */
	public abstract void dispose() throws MleRuntimeException;

	@Override
	public void addPropertyChangeListener(String name, IMlePropChangeListener listener)
		throws MleRuntimeException
	{
		if (name == null)
			throw new MleRuntimeException("Property name must not be null.");

		Vector<IMlePropChangeListener> listeners;
		
		if (! m_propChangeListeners.containsKey(name))
		{
			// Add a new container to collect the listeners for the named property.
			listeners = new Vector<IMlePropChangeListener>();
			m_propChangeListeners.put(name, listeners);
		} else
		{
			// Use the existing collection container.
			listeners = m_propChangeListeners.get(name);
		}
		
		// Add the property change listener.
		listeners.add(listener);
	}

	@Override
	public void removePropertyChangeListener(String name, IMlePropChangeListener listener)
		throws MleRuntimeException
	{
		if (name == null)
			throw new MleRuntimeException("Property name must not be null.");

		if (m_propChangeListeners.containsKey(name))
		{
			Vector<IMlePropChangeListener> listeners = m_propChangeListeners.get(name);
			listeners.remove(listener);
		}
	}

	@Override
	public void notifyPropertyChange(String name, Object oldProperty,
			Object newProperty) throws MleRuntimeException
	{
		// Create a new event.
		MlePropChangeEvent event = new MlePropChangeEvent(this, name,
			oldProperty, newProperty);
		
		if (m_propChangeListeners.containsKey(name))
		{
			Vector<IMlePropChangeListener> listeners = m_propChangeListeners.get(name);
			for (int i = 0; i < listeners.size(); i++)
			{
				IMlePropChangeListener listener = listeners.elementAt(i);
				listener.propChangedEvent(event);
			}
		}
	}
}

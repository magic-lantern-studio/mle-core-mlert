/**
 * @file MleSet.java
 * Created on April 30, 2003. (msm@wizzerworks.com.com)
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
//import com.wizzer.mle.runtime.dpp.MleDppLoader;
import com.wizzer.mle.runtime.event.IMlePropChangeListener;
import com.wizzer.mle.runtime.event.MlePropChangeEvent;


/**
 * <code>MleSet</code> is a class that encapsulates a
 * actor/role policy for platform-specific behavior.
 * <p>
 * This is the base class for all Magic Lantern sets.
 * MleSet provides the baseline runtime and rehearsal-time
 * functionality of the sets.
 * </p><p>
 * Use init() to initialize the set after the sets's
 * member variables (i.e., properties) have been
 * loaded by MleDppLoader.loadGroup() or MleDppLoader.loadScene().
 * Use attachRoles() to maintain a hierarchy of roles that
 * are managed by the set.
 * </p>
 *
 * @see MleRole
 * @see com.wizzer.mle.runtime.dpp.MleDppLoader#mleLoadGroup(int)
 * @see com.wizzer.mle.runtime.dpp.MleDppLoader#mleLoadScene(int)
 *
 * @author  Mark S. Millard
 * @version 1.0
 */
public abstract class MleSet extends Object implements IMleObject, IMleSet
{
    /**
     * This static holds the current set, which is valid during a group
     * load.
     */
    protected static MleSet g_currentSet = null;

    /** The collection of "PropChange" event listeners, per property. */
    protected HashMap<String,Vector<IMlePropChangeListener>> m_propChangeListeners;
 
    /**
     * The default constructor.
     */
    public MleSet()
    {
    	// The current set should be set to null.
    	m_propChangeListeners = new HashMap<String,Vector<IMlePropChangeListener>>();
    }
    /**
     * Get the current Set.
     * <p>
     * Actors or roles that are instantiated by a group load should only
     * refer to the current Set in their init() function
     * (to get a reference to the set that contains them).
     * </p>
     * 
     * @return The current Set is returned. <b>null</b> may be returned if no
     * Set has been made current.
     */
    public static MleSet getCurrentSet()
    {
    	return g_currentSet;
    }
    
    /**
     * Set the current Set.
     * 
     * @param set The actor/role behavior policy to set.
     */
    public static void setCurrentSet(MleSet set)
    {
    	g_currentSet = set;
    }

    /**
     * Attach a child role to its parent role.
     * <p>
     * The implementations of this function will generally cast the 
     * MleRole arguments to the type of roles which the given 
     * set use and then perform role-specific operations to perform
     * the attach.
     * </p>
     * 
     * @param parent The role to attach the child role to.
     * @param child The role which is being attached.
     * 
     * @throws MleRuntimeException This exception is thrown if the
     * set can not be successfully attach the roles.
     */
    public abstract void attachRoles(MleRole parent, MleRole child)
        throws MleRuntimeException;

    /**
     * Initialize the set.
     * <p>
     * Typically, the set may register itself with the scheduler.
     * </p>
     * 
     * @throws MleRuntimeException This exception is thrown if the
     * set can not be successfully initialized.
     */
    public abstract void init() throws MleRuntimeException;

	/**
	 * Dispose all resources associated with the Set.
     * 
     * @throws MleRuntimeException This exception is thrown if the
     * set can not be successfully disposed.
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

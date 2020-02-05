/*
 * MleGroup.java
 * Created on Aug 26, 2004
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
import java.util.Vector;

// Import Magic Lantern Digital Playprint packages.


/**
 * Base class for runtime actor groups.  
 * <p>
 * The group is the minimal unit of actor loading at runtime.
 * A group may contain one or more actors and every actor must be loaded as part of a group
 * defined in a MleDwpGroup item in the Digital Workprint.
 * A group may  be loaded directly at runtime by calling the MleDppLoader.mleLoadGroup() method.
 * </p><p>
 * At runtime the MleGroup object holds an array of refernces to 
 * all the actors it has loaded.  When the group is deleted, it
 * deletes all actors it refers to.  Similarly, a MleGroup loaded as
 * part of an MleScene is deleted when the containing scene is deleted.
 * </p>
 *
 * @see MleGroup
 * @see MleScene
 * 
 * @author Mark S. Millard
 * @version 1.0
 */
public class MleGroup extends Object implements IMleGroup
{
    // The collection of Actors belonging to this group.
    private Vector m_actors;

    /**
     * A factory method for creating a Group.
     *
     * @return A new MleGroup is returned.
     */
    public static MleGroup _mlCreateMleGroup()
    {
    	return new MleGroup();
    }
    
    /**
     * The default constructor.
     */
    public MleGroup()
    {
        super();
        m_actors = new Vector();
    }

    /**
     * A constructor that initializes the number of Actors in the group.
     */
    public MleGroup(int size)
    {
        super();
        m_actors = new Vector(size);
    }

    /**
     * Initialize the group.
     * <p>
     * The class-specific initialization to be called after the group is
     * loaded and its actors' init() methods are called.
     * </p>
     * 
     * @throws MleRuntimeException This exception is thrown if the
     * group can not be successfully initialized.
     */
    public void init() throws MleRuntimeException {}

	/**
	 * Dispose all resources associated with the Group.
     * 
     * @throws MleRuntimeException This exception is thrown if the
     * group can not be successfully disposed.
	 */
	public void dispose() throws MleRuntimeException {}
    
    /**
     * Add an Actor to the Group.
     * 
     * @param actor The <code>MleActor</code> to add.
     */
    public void add(MleActor actor)
    {
        m_actors.addElement(actor);
    }
    
    /**
     * Remove the specified Actor from the Group.
     * 
     * @param actor The <code>MleActor</code> to remove.
     */
    public void remove(MleActor actor)
    {
        m_actors.removeElement(actor);
    }
}

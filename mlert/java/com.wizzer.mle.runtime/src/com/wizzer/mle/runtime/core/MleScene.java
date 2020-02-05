/*
 * MleScene.java
 * Created on Sep 1, 2004
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

// Import Magic Lantern Runtime packages.


/**
 * Base class for Magic Lantern Runtime scenes.
 * <p>
 * The scene is the maximal  unit of actor loading at run time. 
 * A scene may contain one or more groups or group references.
 * A scene may be loaded directly at runtime by calling the
 * <code>MleDppLoader.mleLoadScene()</code> method.
 * </p><p>
 * Two distinguished scenes may be active at once in a title.  The 
 * <i>current</i> scene is set by scene loading, unloading, or 
 * changing methods.  The <i>global</i> scene is under user control
 * and is intended to hold a scene that will govern transitions between
 * two current scenes, for instance during a level change in a game.
 * </p><p>
 * At run time the MleScene object holds an array of references to 
 * all the groups it has loaded.  When the scene is deleted, it
 * deletes all groups it refers to.
 * </p>
 *
 * @see MleGroup
 * 
 * @author Mark S. Millard
 * @version 1.0
 */
public class MleScene implements IMleScene
{
    // The collection of Groups belonging to this Scene.
    private Vector m_groups;
    // The global Scene.
    private static MleScene g_globalScene = null;
    // The current Scene.
    private static MleScene g_currentScene = null;

    /**
     * A factory method for creating a Scene.
     *
     * @return A new MleScene is returned.
     */
    public static MleScene _mlCreateMleScene()
    {
    	return new MleScene();
    }
 
    /**
     * The default constructor.
     */
    public MleScene()
    {
        super();
        m_groups = new Vector();
    }
    
    /**
     * Get the global scene.
     * 
     * @return A reference to the global Scene is returned.
     */
    public MleScene getGlobalScene()
    {
        return g_globalScene;
    }
    
	/**
	 * Set the caller as the global scene.
	 */
    public void setGlobalScene()
    {
        g_globalScene = this;
    }
    
    /**
     * Get the current scene.
     * 
     * @return A reference to the current Scene is returned.
     */
    public MleScene getCurrentScene()
    {
        return g_currentScene;
    }
    
	/**
	 * Set the caller as the current scene.
	 */
    public void setCurrentScene()
    {
        g_currentScene = this;
    }
    
    // Clear the current scene.
    private void clearCurrentScene()
    {
        g_currentScene = null;
    }

    /**
     * Initialize the scene.
     * <p>
     * The class-specific initialization to be called after the scene is
     * loaded and its groups' init() methods are called.
     * </p>
     * 
     * @throws MleRuntimeException This exception is thrown if the
     * scene can not be successfully initialized.
     */
    public void init() throws MleRuntimeException {}

	/**
	 * Dispose all resources associated with the Scene.
     * 
     * @throws MleRuntimeException This exception is thrown if the
     * scene can not be successfully initialized.
	 */
	public void dispose() throws MleRuntimeException {}

    /**
     * Delete the global Scene.
     * 
     * @throws MleRuntimeException This exception is always thrown becuase it has not
     * yet been implemented.
     */
    public void deleteGlobalScene()
        throws MleRuntimeException
    {
        throw new MleRuntimeException("MleScene: deleteGlobalScene not implemented.");
    }
    
    /**
     * Delete the current Scene.
     * 
     * @throws MleRuntimeException This exception is thrown if the
     * scene can not be successfully deleted.
     */
    public void deleteCurrentScene()
        throws MleRuntimeException
    {
    	// Clear current Scene.
    	MleScene old = getCurrentScene();
    	clearCurrentScene();

    	// Get rid of old scene and all its contents by disposing it.
    	if (null != old)
    	{
    		old.dispose();
    	}

    }

	/**
	 * Changes the current scene to that passed in. In this default
	 * implementation, it simply disposes the old currently active scene,
	 * replacing it with the new one.
	 * 
	 * @param newScene The new Scene to switch to.
     * 
     * @throws MleRuntimeException This exception is thrown if the
     * old scene can not be successfully disposed of.
	 */
	public MleScene changeCurrentScene(MleScene newScene)
		throws MleRuntimeException
	{
	 	// Swap old Scene for new Scene.
	    
	 	// Do it this way so that we\'ve set the new Scene before deleting
	 	// the old one, in case we\'re the old one.
	 	MleScene old = getCurrentScene();
	 	newScene.setCurrentScene();
	
	 	// Get rid of old scene and all its contents by disposing it.
	 	if (null != old)
	 	{
	 		old.dispose();
	 	}
	 	
	 	return newScene;
	}

    /**
     * Add a Group to the Scene.
     * 
     * @param group The <code>MleGroup</code> to add.
     */
    public void add(MleGroup group)
    {
        m_groups.addElement(group);
    }
    
    /**
     * Remove the specified Group from the Scene.
     * 
     * @param group The <code>MleGroup</code> to remove.
     */
    public void remove(MleGroup group)
    {
        m_groups.removeElement(group);
    }

}

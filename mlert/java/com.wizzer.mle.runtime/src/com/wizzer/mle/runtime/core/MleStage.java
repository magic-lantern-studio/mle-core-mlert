/**
 * @file MleStage.java
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

// Import Magic Lantern packages.


/**
 * <code>MleStage</code> is a class that manages the rendering
 * for a specific target platform.
 * <p>
 * This is the base class for all Magic Lantern stages.
 * </p><p>
 * Use init() to initialize the stage. Use getSize() to get the dimensions
 * of the stage.
 * </p>
 *
 * @author  Mark S. Millard
 * @version 1.0
 */
public class MleStage extends Object implements IMleStage
{
    /**
     * The reference to the stage, restricting each process to a single
     * stage.
     */
    protected static MleStage g_theStage = null;

    /**
     * A factory method for creating a Stage.
     *
     * @return A <code>MleStage</code> is returned.
     */
    public static MleStage _mlCreateMleStage()
    {
    	return getInstance();
    }
    
    /**
     * Get the Singleton instance of the Stage.
     * 
     * @return A <code>MleStage</code> is returned.
     */
    public static MleStage getInstance()
    {
		// Set the global stage reference.
		if (g_theStage == null)
			g_theStage = new MleStage();
		return g_theStage;   	
    }

    /**
     * Hide the default constructor.
     */
    public MleStage() {}

    /**
     * Initialize the stage.
     * <p>
     * Typically, the stage may register itself with the scheduler.
     * The base init() function does nothing.
     * </p>
     * 
     * @throws MleRuntimeException This exception is thrown if the
     * stage can not be successfully initialized.
     */
    public void init() throws MleRuntimeException {}

	/**
	 * Dispose all resources associated with the Stage.
     * 
     * @throws MleRuntimeException This exception is thrown if the
     * stage can not be successfully disposed.
	 */
	public void dispose() throws MleRuntimeException {}

    /**
     * Get the size of the stage.
     * <p>
     * Returns size of stage's window. Magic Lantern 1.0
     * supports one window per stage: this is the
     * default window.
     * </p>
     * 
     * @return <b>null</b> is returned.
     */
    public MleSize getSize()
    {
        return null;
    }

}

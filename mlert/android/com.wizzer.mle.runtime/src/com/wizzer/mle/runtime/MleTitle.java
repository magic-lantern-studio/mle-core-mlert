/*
 * MleTitle.java
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
package com.wizzer.mle.runtime;

// Import the Magic Lantern Runtimer packages.
import com.wizzer.mle.runtime.core.MleGroup;
import com.wizzer.mle.runtime.core.MleRuntimeException;
import com.wizzer.mle.runtime.core.MleScene;
import com.wizzer.mle.runtime.dpp.MleDppInput;
import com.wizzer.mle.runtime.dpp.MleDppLoader;
import com.wizzer.mle.runtime.event.MleEventDispatcher;
import com.wizzer.mle.runtime.scheduler.MlePhase;
import com.wizzer.mle.runtime.scheduler.MleScheduler;


/**
 * This class contains title generic information that is required by parts
 * of the Magic Lantern Runtime engine. There is only one instance of this
 * class for a given application.
 * 
 * @author Mark S. Millard
 * @version 1.0
 */
public class MleTitle
{
    // The application title environment.
    private static MleTitle g_theTitle = null;

	/** Global actor phase handle for use with MleScheduler.addPhase(). */
    public static MlePhase g_theActorPhase;

    /** Global post actor phase handle for use with MleScheduler.addPhase(). */
    public static MlePhase g_thePostActorPhase;

    /** Global pre role phase handle for use with MleScheduler.addPhase(). */
    public static MlePhase g_thePreRolePhase;

    /** Global role phase handle for use with MleScheduler.addPhase(). */
    public static MlePhase g_theRolePhase;

    /** Global set phase handle for use with MleScheduler.addPhase(). */
    public static MlePhase g_theSetPhase;

    /** Global stage phase handle for use with MleScheduler.addPhase(). */
    public static MlePhase g_theStagePhase;

    /** The title's scheduler. */
    public MleScheduler m_theScheduler = null;
    /** The title's event dispatcher. */
    public MleEventDispatcher m_theDispatcher = null;
    /** The title's Digital Playprint. */
    public MleDppInput m_theDpp = null;
    
    /** Debug string for logging otput. */
    static final public String DEBUG_TAG = "Magic Lantern Runtime Engine";

    /**
     * Platform specific data.  Clients for this data include the stage, callbacks
     * for system events, possibly a central audio class, etc. 
     */
    public Object m_platformData = null; 

    /**
     * Global data specific to the title.
     */
    public Object m_titleData = null; 

    
    // Hide the default constructor.
    private MleTitle()
    {
        super();
    }
    
    /**
     * Get the Singleton instance of the title.
     * 
     * @return A reference to the <code>MleTitle</code> is returned.
     */
    public static MleTitle getInstance()
    {
        if (g_theTitle == null)
            g_theTitle = new MleTitle();
        return g_theTitle;
    }

    /**
     * Load the group of Actors specified by the index into the Digital Playprint's
     * Table-of_Content.
     * <p>
     * The mastered table of group identifiers is used to load from the DPP.
     * </p>
     * 
     * @throws MleRuntimeException This exception is thrown if the Group
     * can not be loaded from the Digital Playprint.
     */
    public MleGroup loadGroup(int indexToDppTOC)
        throws MleRuntimeException
    {
        MleGroup group = null;
        try
        {
            group = MleDppLoader.getInstance().mleLoadGroup(indexToDppTOC);
	    } catch (Exception ex)
	    {
	        throw new MleRuntimeException(ex.getMessage());
	    }
        return  group;
    }

    /**
     * Load the Scene at the specified TOC index location.
     * 
     * @param indexToDppTOC The index into the Digital Playprint Table-Of-Content.
     *  
     * @return The loaded Scene is returned.
     * 
     * @throws MleRuntimeException This exception is thrown if the specified
     * Scene can not be loaded from the Digital Playprint.
     */
    public MleScene loadScene(int indexToDppTOC)
        throws MleRuntimeException
	{
        MleScene scene = null;
        try
        {
            scene = MleDppLoader.getInstance().mleLoadScene(indexToDppTOC);
        } catch (Exception ex)
        {
            throw new MleRuntimeException(ex.getMessage());
        }
	    return scene;
	}

}

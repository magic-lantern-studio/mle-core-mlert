/*
 * MleEventCallback.java
 * Created on Aug 16, 2004
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
package com.wizzer.mle.runtime.event;

/**
 * This abstract class may be used to register a callback handler for a specific event.
 * 
 * @author Mark S. Millard
 * @version 1.0
 */
public abstract class MleEventCallback implements IMleEventCallback
{
	/** Flag indicating that the callback is enabled/disabled. */
	protected boolean m_enabled = false;
	
	/**
	 * The default constructor.
	 */
	public MleEventCallback()
	{ super(); }
	
    /**
     * The callback dispatch method. Extenders must implement this method
     * to provide functionality specific to the handler.
     * 
     * @param event The event that is being dispatched by the handler.
     * @param clientdata Client data registered with this handler.
     * 
     * @return If the event is successfully dispatched, then <b>true</b> should be
     * returned. Otherwise, <b>false</b> should be returned.
     * 
 	 * @see com.wizzer.mle.runtime.event.IMleEventCallback#dispatch(com.wizzer.mle.runtime.event.MleEvent, java.lang.Object)
	 */
	public abstract boolean dispatch(MleEvent event,Object clientdata);
	
	/**
	 * Enable the callback handler.
	 * 
	 * @param enable <b>true</b> should be used if the callback is to be enabled. Otherwise,
	 * <b>false</b> should be used to disable the callback.
	 */
	public void enable(boolean enable)
	{
		m_enabled = enable;
	}
	
	/**
	 * Determine whether the callback is enabled.
	 * 
	 * @return <b>true</b> is returned if the callback is enabled. Otherwise,
	 * <b>false</b> will be returned.
	 */
	public boolean isEnabled()
	{
		return m_enabled;
	}

}

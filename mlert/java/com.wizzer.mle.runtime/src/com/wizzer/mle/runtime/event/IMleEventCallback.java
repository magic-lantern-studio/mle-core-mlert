/*
 * IMleEventCallback.java
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

// Import Magic Lantern classes.
import com.wizzer.mle.runtime.core.IMleCallback;

/**
 * This interface is used to declare a callback handler for a <code>MleEvent</code>.
 * 
 * @author Mark S. Millard
 * @version 1.0
 */
public interface IMleEventCallback extends IMleCallback
{
    /** The event is to be dispatched immediately, without delay. */
    public static final short MLE_EVENT_IMMEDIATE  = 0x0001;

    /** The event is to be dispatched at a later date, it is queued. */
    public static final short MLE_EVENT_DELAYED    = 0x0002;

    /**
     * Dispatch the event using the specified event object and client data.
     * <p>
     * Callbacks should implement this method with functionality
     * that executes the work to be done when the event is dispatched.
     * </p>
     *
     * @param event The event object for the event being dispatched.
     * @param clientdata The data associated with this callback by the client
     * that registered the event with the event dispatcher.
     *
     * @return If the event is successfully dispatched, then <b>true</b>
     * is returned. Otherwise, <b>false</b> should be returned.
     */
    public boolean dispatch(MleEvent event, Object clientdata);
	
}

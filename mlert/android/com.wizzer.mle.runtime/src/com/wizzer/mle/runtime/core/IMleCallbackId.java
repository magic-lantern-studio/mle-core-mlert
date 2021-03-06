/*
 * IMleCallbackId.java
 * Created on Dec 14, 2004
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

/**
 * This interface is used to associate an identifier with a callback
 * implementing the <code>IMleCallback</code> interface.
 * 
 * @author Mark S. Millard
 */
public interface IMleCallbackId
{
    /**
     * Determine if the callback associated with this identifier is enabled or not.
     * 
     * @return <b>true</b> is returned if the callback is enabled. Otherwise,
     * <b>false</b> will be returned.
     */
    public boolean isEnabled();
    
    /**
     * Get the callback associated with this identifier.
     * 
     * @return A reference to an <code>IMleCallback</code> is returned.
     */
    public IMleCallback getCallback();

}

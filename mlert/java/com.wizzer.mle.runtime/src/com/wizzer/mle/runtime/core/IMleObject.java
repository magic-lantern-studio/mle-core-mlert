/*
 * IMleObject.java
 * Created on Nov 16, 2004
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
import java.io.ByteArrayInputStream;

import com.wizzer.mle.runtime.event.IMlePropChangeListener;

/**
 * This interface is used to define functionality that is shared across
 * Magic Lantern Runtime classes.
 * <p>
 * The IMleObject interface specifies a common way of setting and
 * retrieving properties.
 * </p>
 * 
 * @author Mark S. Millard
 */
public interface IMleObject
{
    /**
     * Get the value of the property with the specified name.
     * 
     * @param name The name of the property as a <code>String</code>.
     * 
     * @return The property value is returned as an <code>Object</code>.
     * 
     * @throws MleRuntimeException This exception is thrown if the specified
     * property can not be retrieved.
     */
    public Object getProperty(String name)
    	throws MleRuntimeException;
    
    /**
     * Set the value of the property with the specified name.
     * 
     * @param name The name of the property as a <code>String</code>.
     * @param property The property to set.
     * 
     * @throws MleRuntimeException This exception is thrown if the specified
     * property can not be set.
     */
    public void setProperty(String name, IMleProp property)
    	throws MleRuntimeException;
    
    /**
     * Set the value of the property with the specified name.
     * 
     * @param name The name of the property as a <code>String</code>.
     * @param length The length of each property, in bytes.
     * @param nElements The number of elements in the array, each of size <b>length</b>.
     * @param value The value of the property to set.
     * 
     * @throws MleRuntimeException This exception is thrown if the specified
     * property can not be set.
     */
    public void setPropertyArray(String name, int length, int nElements, ByteArrayInputStream value)
        throws MleRuntimeException;
    
    /**
     * Report a bound property change. If <i>oldValue</i> and <i>newValue</i> are not
     * equal and the <code>MlePropChangeEvent</code> listener list isn't empty,
     * then fire a <code>MlePropChangeEvent</code> event to each listener.
     * 
     * @param name The name of the property that was changed.
     * @param oldProperty The old value of the property (as an Object).
     * @param newProperty The new value of the property (as an Object).
     * 
     * @throws MleRuntimeException This exception is thrown if registered property
     * listeners can not be notified.
     */
    public void notifyPropertyChange(String name, Object oldProperty, Object newProperty)
        throws MleRuntimeException;
    
    /**
     * Adds a <code>IMlePropChangeListener</code> for a specific property.
     * The listener will be invoked only when a call on <code>notifyPropertyChange</code>
     * names that specific property. If listener is <b>null</b>, no exception is thrown
     * and no action is performed. 
     *
     * @param name The name of the property to listen on.
     * @param listener The <code>IMlePropChangeListener</code> to be added.
     * 
     * @throws MleRuntimeException This exception is thrown if the property
     * listener can not be added. It is also thrown if the <i>name</i> argument
     * is <b>null</b>.
     */
    public void addPropertyChangeListener(String name, IMlePropChangeListener listener)
        throws MleRuntimeException;
    
    /**
     * Removes a <code>IMlePropChangeListener</code> for a specific property.
     * If listener is <b>null</b>, no exception is thrown and no action is performed.
     * 
     * @param name The name of the property that was listened on.
     * @param listener The <code>IMlePropChangeListener</code> to be removed.
     * 
     * @throws MleRuntimeException This exception is thrown if the property
     * listener can not be removed. It is also thrown if the <i>name</i> argument
     * is <b>null</b>.
     */
    public void removePropertyChangeListener(String name, IMlePropChangeListener listener)
            throws MleRuntimeException;
}

/*
 * IMleProp.java
 * Created on Mar 24, 2005
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

//Import standard Java packages.
import java.io.ByteArrayInputStream;

/**
 * This interface identifies the contract for dealing with Magic Lantern
 * properties in a consistent manner.
 * 
 * @author Mark S. Millard
 */
public interface IMleProp
{
    /** An unknown property type. */
    public static int PROP_TYPE_UNKNOWN = -1;
    /** The Media Reference property. */
    public static int PROP_TYPE_MEDIAREF = 10;
    
    /**
     * Get the property type.
     * <p>
     * Valid types include:
     * <li>
     * </li>
     * </p>
     * 
     * @return The property's type is returned.
     */
    public int getType();
    
    /**
     * Get the length of the property data.
     * 
     * @return The size of the property data is returned.
     */
    public int getLength();
    
    /**
     * Get the property data as an input stream.
     * 
     * @return An input stream is returned.
     */
    public ByteArrayInputStream getStream();
}

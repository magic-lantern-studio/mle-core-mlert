/*
 * MleSize.java
 * Created on Aug 27, 2004
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
 * This class is used as a container for a managed size element.
 * 
 * @author Mark S. Millard
 */
public class MleSize extends Object
{
    // The width.
    private long m_width = 0;
    // The height.
    private long m_height = 0;
    
    // Hide the default constructor.
    private MleSize() {}
    
    /**
     * The default constructor.
     * 
     * @param width The width of the size element.
     * @param height The height of the size element.
     */
    public MleSize(long width, long height)
    {
        super();
        m_width = width;
        m_height = height;
    }
    
    /**
     * Get the width.
     * 
     * @return The width is returned as a <b>long</b>.
     */
    public long getWidth()
    {
        return m_width;
    }
    
    /**
     * Get the height.
     * 
     * @return The height is returned as a <b>long</b>.
     */
    public long getHeight()
    {
        return m_height;
    }

}

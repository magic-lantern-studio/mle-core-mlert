/*
 * MleProp.java
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

// Import standard Java classes.
import java.io.ByteArrayInputStream;

/**
 * This class implements a generic property with an unknown data
 * type (by default).
 * 
 * @author Mark S. Millard
 */
public class MleProp implements IMleProp
{
    /** The property type. */
    protected int m_type = PROP_TYPE_UNKNOWN;
    /** The property length. */
    protected int m_length = 0;
    /** The data input stream. */
    protected ByteArrayInputStream m_stream = null;

    // Hide the default constructor.
    protected MleProp() {}
    
    /**
     * A constructor that initializes the data managed by the property.
     * 
     * @param length The expected length of the data stream.
     * @param data The data input stream for the property.
     */
    public MleProp(int length, ByteArrayInputStream data)
    {
        super();
        m_length = length;
        m_stream = data;
    }
    
    /**
     * Set the property type.
     * 
     * @param type An integer representing the property type.
     */
    public void setType(int type)
    {
        m_type = type;
    }

    /* (non-Javadoc)
     * @see com.wizzer.mle.runtime.core.IMleProp#getType()
     */
    public int getType()
    {
        return m_type;
    }

    /* (non-Javadoc)
     * @see com.wizzer.mle.runtime.core.IMleProp#getLength()
     */
    public int getLength()
    {
        return m_length;
    }

    /* (non-Javadoc)
     * @see com.wizzer.mle.runtime.core.IMleProp#getStream()
     */
    public ByteArrayInputStream getStream()
    {
        return m_stream;
    }

}

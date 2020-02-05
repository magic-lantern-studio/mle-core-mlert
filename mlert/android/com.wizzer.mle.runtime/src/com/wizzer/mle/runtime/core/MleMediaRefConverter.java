/**
 * @file MleMediaRefConverter.java
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
 * <code>MleMediaRefConverter</code> is a class that determines a local
 * representation from a media reference, which may be a file, URL, or some
 * other type of identifier.
 * <p>
 * This is the base class for all Magic Lantern media reference converters.
 * It treats the reference as a local file name.
 * </p>
 *
 * @see MleMediaRef
 *
 * @author  Mark S. Millard
 * @version 1.0
 */
public class MleMediaRefConverter extends Object
{
	/** The local reference. */
    protected Object m_reference = null;
    /** Flag indicating conversion is complete. */
    protected boolean m_converted = false;

    /**
     * The default constructor.
     */
    public MleMediaRefConverter() {}

    /**
     * Set the media reference for this converter.
     * 
     * @param reference The media reference to convert.
     */
    public void setReference(Object reference)
    {
    	m_reference = reference;
    }

    /**
     * Get the media reference for this converter.
     * 
     * @return The media reference is returned.
     */
    public Object getReference()
    {
    	return m_reference;
    }

    /**
     * Treat the media reference as a filename.
	 * 
     * @return The media reference is returned as a <code>String</code>.
     * <b>null</b> will be returned if no reference has ever been set
     * for conversion.
     * 
     * @throws MleRuntimeException This exception will be thrown if an
     * error occurs while attempting to get the filename.
     * 
     * @see setReference
     */
    public String getFilename() throws MleRuntimeException
    {
        if (m_reference != null)
        {
            m_converted = true;
            if (m_reference instanceof byte[])
                return new String((byte[])m_reference);
            else
                return m_reference.toString();
        } else
        {
            m_converted = false;
            return null;
        }
    }

    /**
     * Determine if the conversion is complete.
     * 
     * @return <b>true</b> will be returned if the conversion has completed
     * successfully. Otherwise, <b>false</b> will be returned.
     * 
     * @throws MleRuntimeException This exception is thrown if an error
     * occured while converting the media reference data.
     */
    public boolean conversionComplete()
        throws MleRuntimeException
    {
    	return m_converted;
    }

    /**
     * Dispose of converter resources.
     */
    public void dispose()
    {
        // Does nothing for now.
    }
}

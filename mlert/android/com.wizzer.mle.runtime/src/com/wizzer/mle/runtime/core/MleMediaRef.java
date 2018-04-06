/**
 * @file MleMediaRef.java
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
import com.wizzer.mle.runtime.core.MleMediaRefConverter;


/**
 * <code>MleMediaRef</code> is a class that manages references to
 * media assets.
 * <p>
 * This is the base class for all Magic Lantern media references.
 * </p><p>
 * Use init() to initialize the media reference.
 * </p>
 *
 * @see MleMediaRefBuffer
 * @see MleMediaRefConverter
 *
 * @author  Mark S. Millard
 * @version 1.0
 */
public class MleMediaRef extends Object implements IMleMediaRef
{
    /**
     * A container for managing media.
     */
    public class MleMediaRefBuffer
    {
        /** Flags associated with the buffer. */
        public int m_flags;
        /** The size of the buffer. */
        public int m_bufferSize = 0;
        /** The media reference buffer. */
        public byte[] m_buffer = null;

        // The next buffer in a linked list.
        private MleMediaRefBuffer m_next = null;

        /**
         * The default constructor.
         */
        MleMediaRefBuffer() {}
    }

    // Declare member variables.

    /** The media references. */
    protected MleMediaRefBuffer m_references = null;

    /** The number of media references. */
    protected int m_numReferences = 0;

    /** The reference converter to change a buffer into a local representation. */
    protected MleMediaRefConverter m_converter;

    /**
     * A factory method for creating a MediaRef.
     *
     * @return A new MleMediaRef is returned.
     */
    public static MleMediaRef _mlCreateMleMediaRef()
    {
    	return new MleMediaRef();
    }

    /**
     * The default constructor.
     */
    public MleMediaRef()
    {
		// Create a default converter.
		m_converter = new MleMediaRefConverter();
    }

    /**
     * Initialize the media reference.
     * <p>
     * This method is a hook to do any initialization of the media reference.
     * Typically, the media reference may schedule itself.
     * The base init() function does nothing.
     * </p>
     * 
     * @throws MleRuntimeException This exception is thrown if the
     * media reference can not be successfully initialized.
     */
    public void init() throws MleRuntimeException {}

    
	/**
	 * Dispose all resources associated with the MediaRef.
     * 
     * @throws MleRuntimeException This exception is thrown if the
     * media reference can not be successfully disposed.
	 */
	public void dispose() throws MleRuntimeException {}


    /**
     * Register the media for this reference.
     * <p>
     * This method is used to register the media data that is
     * referenced.
     * </p>
     */
    public boolean registerMedia(long flags,int size,byte[] media)
    {
		MleMediaRefBuffer newRef, nextRef;
		boolean status = true;

        // Allocate a new reference.
        newRef = new MleMediaRefBuffer();
        newRef.m_flags = (int)flags;
        newRef.m_bufferSize = size;
        newRef.m_buffer = new byte[size];
        System.arraycopy(media,0,newRef.m_buffer,0,size);

        // Attach next reference.
        nextRef = m_references;
        if (nextRef == null)
        {
            // First entry on the list.
            m_references = newRef;
        } else
        {
            // Add entry to the list.
            while (nextRef.m_next != null)
                nextRef = nextRef.m_next;
            nextRef.m_next = newRef;
        }
	
		return status;
    }

    /**
     * Clear the registry of media references.
     * <p>
     * This method is used to unload the media data from 
     * the media registry.
     * </p>
     */
    public void clearRegistry()
    {
        MleMediaRefBuffer nextRef, tmp;

        nextRef = m_references;
        for (int i = 0; i < m_numReferences; i++)
        {
            tmp = nextRef.m_next;
            nextRef.m_buffer = null;
            nextRef = null;
            nextRef = tmp;
        }
        m_references = null;
        m_numReferences = 0;
        
        // Invoke garbage collection.
        System.gc();
    }

    // Accessors for the buffer

    /**
     * Get the next media reference relative to the specifed
	 * load reference.
	 * 
	 * @param loadReference The <code>MleMediaRefBuffer</code> which marks
	 * the current media reference. May be <b>null</b>.
	 * 
	 * @return A <code>MleMediaRefBuffer</code> is returned.
	 * If <i>loadReference</i> was specified as <b>null</b>, then the first
	 * <code>MleMediaRefBuffer</code> in the list will be returned.
     */
    public MleMediaRefBuffer getNextMediaRef(MleMediaRefBuffer loadReference)
    {
		if (loadReference != null)
		    return loadReference.m_next;
		else
		    return m_references;
    }

    /**
     * Get the media reference flags for the specified load
	 * reference.
	 * 
     * @param loadReference The <code>MleMediaRefBuffer</code> which marks
	 * the current media reference.
     * 
     * @return The flags will be returned.
     * 
     * @throws MleRuntimeException This exception is thrown if the specified
     * load reference is <b>null</b>.
     */
    public int getMediaRefFlags(MleMediaRefBuffer loadReference)
        throws MleRuntimeException
    {
		if (loadReference == null)
		{
		    throw new MleRuntimeException("Media Reference Buffer is null.");
		}
		
		return loadReference.m_flags;
    }

    /**
     * Get the media reference size for the specified load
	 * reference.
	 * 
     * @param loadReference The <code>MleMediaRefBuffer</code> which marks
	 * the current media reference.
     * 
     * @return The size will be returned.
     * 
     * @throws MleRuntimeException This exception is thrown if the specified
     * load reference is <b>null</b>.
     */
    public int getMediaRefBufferSize(MleMediaRefBuffer loadReference)
        throws MleRuntimeException
    {
		if (loadReference == null)
		{
		    throw new MleRuntimeException("Media Reference Buffer is null.");
		}

		return loadReference.m_bufferSize;
    }

    /**
     * Get the media reference buffer for the specified load
	 * reference.
	 * 
     * @param loadReference The <code>MleMediaRefBuffer</code> which marks
	 * the current media reference.
     * 
     * @return A byte array will be returned.
     * 
     * @throws MleRuntimeException This exception is thrown if the specified
     * load reference is <b>null</b>.
     */
    public byte[] getMediaRefBuffer(MleMediaRefBuffer loadReference)
        throws MleRuntimeException
    {
		if (loadReference == null) 
		{
		    throw new MleRuntimeException("Media Reference Buffer is null.");
		}

		return loadReference.m_buffer;
    }

    // Accessors for the reference converter.

    /**
     * Set the reference converter.
	 *
	 * @param converter The media reference converter to
	 * set for this media reference.
	 */
    public void setMediaRefConverter(MleMediaRefConverter converter)
    {
    	m_converter = converter;
    }

    /**
     * Get the reference converter.
     * 
     * @return A reference to a <code>MleMediaRefConverter</code> is returned.
     * May be <b>null</b>.
     */
    public MleMediaRefConverter getMediaRefConverter()
    {
    	return m_converter;
    }

    /**
     * Remove the reference converter.
     */
    public void deleteMediaRefConverter()
    {
		if (m_converter != null)
		{
		    m_converter.dispose();
	        m_converter = null;
		}
    }

}

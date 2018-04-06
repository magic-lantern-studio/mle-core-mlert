/*
 * MleChunkReader.java
 * Created on Sep 3, 2004
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
package com.wizzer.mle.runtime.dpp;

// Import Magic Lantern Runtime packages.
import com.wizzer.mle.runtime.util.MleUnsigned;

/**
 * This class provides utility for reading a Chunk that has been extracted from
 * a Digital Playprint.
 * 
 * @author Mark S. Millard
 */
public class MleChunkReader
{
    // The byte stream to process.
    protected byte[] m_stream = null;
    // The current offset into the byte stream.
    protected int m_offset = 0;

    // Hide the default constructor.
    private MleChunkReader()
    {}
    
    /**
     * A constructor that initializes the byte stream to process.
     * 
     * @param stream An array of <b>byte</b>s consisting of chunk data that
     * has been extracted from the Digital Playprint.
     */
    public MleChunkReader(byte[] stream)
    {
        super();
        m_stream = stream;
    }
    
    /**
     * Get the next byte in the stream.
     * <p>
     * The offset into the stream is automatically incremented.
     * </p>
     * 
     * @return The next <b>byte</b> in the stream is returned.
     * 
     * @throws MleDppException This exception is thrown if the next byte
     * can not be read from the stream.
     */
    protected byte nextStreamByte()
        throws MleDppException
    {
        if ((m_offset < 0) || (m_offset >= m_stream.length))
            throw new MleDppException("Unable to read next byte from stream.");
    	return m_stream[m_offset++];
    }

    /**
     * Read a 4-byte integer from the stream.
     * 
     * @return A <b>long</b> is returned containing the integer value.
     * 
     * @throws MleDppException This exception is thrown if the integer can
     * not be read from the stream.
     */
    public long readInt()
        throws MleDppException
    {
        long value = 0;
        
        byte[] v = new byte[4];
        v[0]= nextStreamByte();
        v[1]= nextStreamByte();
        v[2]= nextStreamByte();
        v[3]= nextStreamByte();
        value = MleUnsigned.readDWORD(v,0);
        
        return value;
    }

}

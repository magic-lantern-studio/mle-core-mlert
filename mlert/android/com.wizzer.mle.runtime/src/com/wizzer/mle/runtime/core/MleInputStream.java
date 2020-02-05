/*
 * MledDppInputStream.java
 * Created on Jan 16, 2008
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
import java.io.IOException;
import java.io.ByteArrayOutputStream;

// Import Magic Lantern Runtime Engine classes.

/**
 * Process input as a random-access stream.
 * 
 * @author Mark S. Millard
 */
public class MleInputStream extends InputStream implements Cloneable
{
	// The data from the input stream.
	private byte[] m_bytes;
	// The current position of the file pointer.
	private long m_pos = 0;
	// Flag indicating that the stream is closed.
	private boolean m_closed;
	
	// Hide the default constructor.
	private MleInputStream() {}
	
	// Read from the specified stream, returning the contents in a byte array.
	private byte[] readFromStream(java.io.InputStream input) throws IOException
	{
		if (input == null)
			return null;
	
		int bufferSize = 1024;
		byte[] byteBuffer = new byte[bufferSize];
	
		int bytesRead = 0;
		int totalBytesRead = 0;
		int count = 0;
	
		ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream(bufferSize*2);
	
		while ((bytesRead = input.read(byteBuffer)) != -1)
		{
			totalBytesRead += bytesRead;
			count++;
		
			byteArrayOutputStream.write(byteBuffer, 0, bytesRead);
		}
	
		byte [] dataBytes = byteArrayOutputStream.toByteArray();

		return dataBytes;
	}

	/**
	 * A constructor that initializes the stream to access.
	 * 
	 * @param in The Java IO <code>InputStream</code>.
	 * 
	 * @see java.io.InputStream
	 */
	public MleInputStream(java.io.InputStream in) throws MleRuntimeException
	{
		//String buffer = in.toString();
		//m_bytes = buffer.getBytes();
		
		try
		{
			m_bytes = readFromStream(in);
			m_length = m_bytes.length;
		} catch (IOException ex)
		{
			throw new MleRuntimeException(ex.getMessage());
		}
		
		m_closed = false;
	}
	
	/**
	 * A constructor that initializes the data to access.
	 * 
	 * @param data The data to access as a random-access stream.
	 */
	public MleInputStream(byte[] data)
	{
		m_bytes = data;
		m_length = m_bytes.length;
		
		m_closed = false;
	}

    /**
     * Close the input stream.
     * 
     * @throws IOException This exception is thrown if the stream has already
     * been closed.
     */
	@Override
	public void close() throws IOException
	{
		if (m_closed)
			throw new IOException("MleDppInputStream is already closed.");

		m_closed = true;
	}

	/**
	 * Read bytes from the internal representation of the data passed in at the
	 * time of construction.
	 * 
	 * @param b The buffer to copy the bytes into.
	 * @param offset The offset into <b>b</b> in which to begin copying.
	 * @param length The number of bytes to read into <b>b</b>.
	 * 
	 * @throws IOException This exception is thrown if the stream is already closed
	 * or another IOException occurs.
	 */
	@Override
	protected void readInternal(byte[] b, int offset, int length)
      throws IOException
	{
		if (m_closed)
			throw new IOException("MleDppInputStream has been closed.");
		
		System.arraycopy(m_bytes, (int)m_pos, b, offset, length);
		m_pos += length;
	}

	/**
	 * Seek to the internal position in the file.
	 * 
	 * @param pos The position to seek to.
	 * 
	 * @throws IOException This exception is thrown if the stream is already closed
	 * or another IOException occurs.
	 */
	@Override
	protected void seekInternal(long pos) throws IOException
	{
		if (m_closed)
			throw new IOException("MleDppInputStream has been closed.");

		m_pos = pos;
	}

    /**
     * Clone this object.
     * 
     * @return A copy of this object is returned.
     */
    public Object clone()
    {
        MleInputStream clone = (MleInputStream)super.clone();

        clone.m_bytes = new byte[(int)m_length];
        System.arraycopy(m_bytes, 0, clone.m_bytes, 0, (int)m_length);
        clone.m_pos = m_pos;
        clone.m_closed = m_closed;

        return clone;
    }
}

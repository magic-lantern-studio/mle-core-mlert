/*
 * MleRandomAccessFile.java
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
package com.wizzer.mle.runtime.dpp;

// Import standard Java classes.
import java.io.IOException;

// Import Magic Lantern Runtime Engine classes.
import com.wizzer.mle.runtime.core.InputStream;

/**
 * 
 * @author Mark S. Millard
 */
public class MleRandomAccessStream implements IMleRandomAccess
{
    /** Open the file for reading. */
    public static final String MLE_READING_MODE = "r";

    // The random-access stream.
    InputStream m_stream = null;
    
    // Hide the default constructor.
    private MleRandomAccessStream() {}
    
    /**
	 * A constructor that opens a <code>MleInputStream</code>.
	 * 
	 * @param stream The name of the stream to open.
	 * 
	 * @throws MleDppException This exception is thrown if file can not
	 * be opened.
	 */
    public MleRandomAccessStream(InputStream stream) throws MleDppException
    {
    	m_stream = stream;
    }
    
    /**
	 * Answers the current position within this stream.
	 * <p>
	 * All reads and writes take place at the current file pointer position.
	 * </p>
	 * 
	 * @return The current file pointer position.
	 * 
	 * @throws IOException If the stream is already closed or another IOException occurs.
	 */
	public long getFilePointer() throws IOException
	{
		return m_stream.getFilePointer();
	}

	/**
	 * Reads a 32-bit integer value from this stream.
	 * 
	 * @return The next <i>int</i> value from the source stream.
	 * 
	 * @throws IOException If the stream is already closed or another IOException occurs.
	 */
	public int readInt() throws IOException
	{
		return m_stream.readInt();
	}

	/**
	 * Read bytes from this stream and stores them in byte array <i>buffer</i>
	 * starting at offset <i>offset</i>. This method blocks until <i>count</i>
	 * number of bytes have been read.
	 * 
	 * @param buffer The byte array in which to store the read bytes.
	 * @param offset The offset in <i>buffer</i> to store the read bytes.
	 * @param count The maximum number of bytes to store in buffer.
	 * 
	 * @throws IOException If the stream is already closed or another IOException occurs.
	 */
	public void readFully(byte[] buffer, int offset, int count)
			throws IOException
	{
		m_stream.readBytes(buffer, offset, count);
	}

	/**
	 * Seeks to the position <i>pos</i> in this stream.
	 * <p>
	 * All read/write/skip methods sent will be relative to <i>pos</i>.
	 * </p>
	 * 
	 * @param pos The desired file pointer position.
	 * 
	 * @throws IOException If the stream is already closed or another IOException occurs.
	 */
	public void seek(long pos) throws IOException
	{
		m_stream.seek(pos);
	}

	/**
	 * Skips <i>count</i> number of bytes in this stream.
	 * <p>
	 * Subsequent <code>read()</code>'s will not return these bytes
	 * unless <code>reset()</code> is used.
	 * 
	 * @param count The number of bytes to skip.
	 * 
	 * @return The number of bytes actually skipped is returned.
	 * 
	 * @throws IOException If the stream is already closed or another IOException occurs.
	 */
	public int skipBytes(int count) throws IOException
	{
		long pos = getFilePointer();
		seek(pos + count);
		return count;
	}

	/**
	 * Close the stream.
	 * 
	 * @throws IOException If the stream is already closed or another IOException occurs.
	 */
	public void close() throws IOException
	{
		m_stream.close();
	}
}

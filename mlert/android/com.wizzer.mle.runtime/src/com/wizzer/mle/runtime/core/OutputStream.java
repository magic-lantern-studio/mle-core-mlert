/*
 * MleOutputStream.java
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

/**
 * A random-access output stream.
 */
abstract public class OutputStream
{
	/** The buffer size. */
    final static int BUFFER_SIZE = 1024;

    // The working buffer.
    private final byte[] m_buffer = new byte[BUFFER_SIZE];
	// Position in file of buffer.
    private long m_bufferStart = 0;
	// Position in buffer.
    private int m_bufferPosition = 0;

    /**
     * Write a single byte to the stream.
     * 
     * @param b The byte to write.
     * 
     * @throws IOException This exception is thrown if the stream has been closed
	 * or another IOException occurs.
     */
    public final void writeByte(byte b) throws IOException
    {
        if (m_bufferPosition >= BUFFER_SIZE)
            flush();
        m_buffer[m_bufferPosition++] = b;
    }

    /**
     * Write the specified byte array to the stream.
     * 
     * @param b The array of bytes to output.
     * @param length The number of bytes to write.
     * 
     * @throws IOException This exception is thrown if the stream has been closed
	 * or another IOException occurs.
     */
    public final void writeBytes(byte[] b, int length) throws IOException
    {
        for (int i = 0; i < length; i++)
            writeByte(b[i]);
    }

    /**
     * Write an integer to the stream.
     * 
     * @param i The integer to write.
     * 
     * @throws IOException This exception is thrown if the stream has been closed
	 * or another IOException occurs.
     */
    public final void writeInt(int i) throws IOException
    {
        writeByte((byte)(i >> 24));
        writeByte((byte)(i >> 16));
        writeByte((byte)(i >>  8));
        writeByte((byte) i);
    }

    /**
     * Writes an int in a variable-length format.
     * <p>
     * Writes between one and five bytes. Smaller values take fewer bytes. Negative numbers are not supported.
     * </p>
     * 
     * @param i The integer to write.
     * 
     * @throws IOException This exception is thrown if the stream has been closed
	 * or another IOException occurs.
     */
    public final void writeVInt(int i) throws IOException
    {
        while ((i & ~0x7F) != 0)
        {
            writeByte((byte)((i & 0x7f) | 0x80));
            i >>>= 7;
        }
        writeByte((byte)i);
    }

    /**
     * Write a long value to the stream.
     * 
     * @param i The long value to write.
     * 
     * @throws IOException This exception is thrown if the stream has been closed
	 * or another IOException occurs.
     */
    public final void writeLong(long i) throws IOException
    {
        writeInt((int) (i >> 32));
        writeInt((int) i);
    }

    /**
     * Writes a long in a variable-length format.
     * <p>
     * Writes between one and five bytes. Smaller values take fewer bytes. Negative numbers are not supported.
     * </p>
     * 
     * @param i The long value to write.
     * 
     * @throws IOExceptionThis exception is thrown if the stream has been closed
	 * or another IOException occurs.
     */
    public final void writeVLong(long i) throws IOException
    {
        while ((i & ~0x7F) != 0)
        {
            writeByte((byte)((i & 0x7f) | 0x80));
            i >>>= 7;
        }
        writeByte((byte)i);
    }

    /**
     * Write a string of characers to the stream.
     * 
     * @param s The <code>String</code> to write.
     * 
     * @throws IOException This exception is thrown if the stream has been closed
	 * or another IOException occurs.
     */
    public final void writeString(String s) throws IOException
    {
        int length = s.length();
        writeVInt(length);
        writeChars(s, 0, length);
    }

    public final void writeChars(String s, int start, int length)
      throws IOException
    {
        final int end = start + length;
        for (int i = start; i < end; i++)
        {
            final int code = (int)s.charAt(i);
            if (code >= 0x01 && code <= 0x7F)
	            writeByte((byte)code);
            else if (((code >= 0x80) && (code <= 0x7FF)) || code == 0)
            {
	             writeByte((byte)(0xC0 | (code >> 6)));
	             writeByte((byte)(0x80 | (code & 0x3F)));
            } else
            {
	             writeByte((byte)(0xE0 | (code >>> 12)));
	             writeByte((byte)(0x80 | ((code >> 6) & 0x3F)));
	             writeByte((byte)(0x80 | (code & 0x3F)));
            }
        }
    }

    /**
     * Flush the stream.
     * 
     * @throws IOException This exception is thrown if the stream has been closed
	 * or another IOException occurs.
     */
    protected final void flush() throws IOException
    {
        flushBuffer(m_buffer, m_bufferPosition);
        m_bufferStart += m_bufferPosition;
        m_bufferPosition = 0;
    }

    /**
     * Flush the specified buffer.
     * 
     * @param b The buffer to flush.
     * @param len The size of the buffer.
     * 
     * @throws IOException This exception is thrown if the stream has been closed
	 * or another IOException occurs.
     */
    abstract protected void flushBuffer(byte[] b, int len) throws IOException;

    /**
     * Close the output stream.
     * 
     * @throws IOException This exception is thrown if the stream has already
     * been closed.
     */
    public void close() throws IOException
    {
       flush();
    }

    /**
     * Get the current file pointer postion.
     * 
     * @return The current file pointer position is returned as a <b>long</b> value.
     * 
     * @throws IOException This exception is thrown if the stream has been closed
	 * or another IOException occurs.
     */
    public final long getFilePointer() throws IOException
    {
        return m_bufferStart + m_bufferPosition;
    }

    /**
     * Seek to the requested position in the file.
     * 
     * @param pos The position to seek to.
     * 
     * @throws IOException This exception is thrown if the stream has been closed
	 * or another IOException occurs.
     */
    public void seek(long pos) throws IOException
    {
        flush();
        m_bufferStart = pos;
    }

    /**
     * Get the length of the stream.
     * 
     * @return A <b>long</b> value is returned representing the size of the stream
     * (in bytes).
     * 
     * @throws IOException This exception is thrown if the stream has been closed
	 * or another IOException occurs.
     */
    abstract public long length() throws IOException;
}

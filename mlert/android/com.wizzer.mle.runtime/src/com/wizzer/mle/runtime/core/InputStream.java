/*
 * MleInputStream.java
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
 * A random-access input stream.
 * 
 * @author Mark S. Millard
 */
abstract public class InputStream implements Cloneable
{
	/** The size of the working buffer. */
    final static int BUFFER_SIZE = OutputStream.BUFFER_SIZE;

    // The working buffer.
    private byte[] m_buffer;
    // Utility for prcessing chars and Strings.
    private char[] m_chars;

	// Position in file of underlying stream.
    private long m_bufferStart = 0;
	// End of valid bytes in the working buffer.
    private int m_bufferLength = 0;
	// The next byte to read from the working buffer.
    private int m_bufferPosition = 0;

    /**
     *  The length of the stream. This must be set by subclasses.
     */
    protected long m_length;

    /**
     * Read the next byte in the stream.
     * 
     * @return A <b>byte</b> is returned.
     * 
     * @throws IOException This exception is thrown if the stream has been closed
	 * or another IOException occurs.
     */
    public final byte readByte() throws IOException
    {
        if (m_bufferPosition >= m_bufferLength)
            refill();
        return m_buffer[m_bufferPosition++];
    }

    /**
     * Read bytes from the stream.
     * 
	 * @param b The buffer to copy the bytes into.
	 * @param offset The offset into <b>b</b> in which to begin copying.
	 * @param len The number of bytes to read into <b>b</b>.
	 * 
     * @throws IOException This exception is thrown if the stream has been closed
	 * or another IOException occurs.
     */
    public final void readBytes(byte[] b, int offset, int len)
      throws IOException
    {
        if (len < BUFFER_SIZE)
        {   // Read byte-by-byte.
            for (int i = 0; i < len; i++)
	            b[i + offset] = (byte)readByte();
        } else
        {   // Read all-at-once.
            long start = getFilePointer();
            seekInternal(start);
            readInternal(b, offset, len);

            m_bufferStart = start + len;	// Adjust stream variables.
            m_bufferPosition = 0;
            m_bufferLength = 0;			// Trigger refill() on read.
        }
    }

    /**
     * Read the next integer in the stream.
     * 
     * @return The next 4 bytes in the stream will be returned as an <b>int</b>.
     * 
     * @throws IOException This exception is thrown if the stream has been closed
	 * or another IOException occurs.
     */
    public final int readInt() throws IOException
    {
        return ((readByte() & 0xFF) << 24) | ((readByte() & 0xFF) << 16) |
               ((readByte() & 0xFF) <<  8) |  (readByte() & 0xFF);
    }

    /**
     * Reads an int stored in variable-length format.
     * <p>
     * Reads between one and five bytes. Smaller values take fewer bytes. Negative numbers are not supported.
     * </p>
     * 
     * @return The integer value is returned.
     * 
     * @throws IOException This exception is thrown if the stream has been closed
	 * or another IOException occurs.
     */
    public final int readVInt() throws IOException
    {
        byte b = readByte();
        int i = b & 0x7F;
        for (int shift = 7; (b & 0x80) != 0; shift += 7)
        {
            b = readByte();
            i |= (b & 0x7F) << shift;
        }
        return i;
    }

    /**
     * Read the next long value in the stream.
     * 
     * @return The next 8 bytes in the stream will be returned as a <b>long</b>.
     * 
     * @throws IOException This exception is thrown if the stream has been closed
	 * or another IOException occurs.
     */
    public final long readLong() throws IOException
    {
        return (((long)readInt()) << 32) | (readInt() & 0xFFFFFFFFL);
    }

    /**
     * Reads a long stored in variable-length format.
     * <p>
     * Reads between one and nine bytes. Smaller values take fewer bytes.
     * Negative numbers are not supported.
     * </p>
     * 
     * @return The long value is returned.
     * 
     * @throws IOException This exception is thrown if the stream has been closed
	 * or another IOException occurs.
     */
    public final long readVLong() throws IOException
    {
        byte b = readByte();
        long i = b & 0x7F;
        for (int shift = 7; (b & 0x80) != 0; shift += 7)
        {
            b = readByte();
            i |= (b & 0x7FL) << shift;
        }
        return i;
    }

    /**
     * Read a string of characters from the stream.
     * 
     * @return A <code>String</code> is returned.
     * 
     * @throws IOException This exception is thrown if the stream has been closed
	 * or another IOException occurs.
     */
    public final String readString() throws IOException
    {
        int length = readVInt();
        if (m_chars == null || length > m_chars.length)
            m_chars = new char[length];
        readChars(m_chars, 0, length);
        return new String(m_chars, 0, length);
    }

    /**
     * Read characters into the supplied <i>buffer</i>.
     * 
     * @param buffer The buffer to read the characters into.
     * @param start An offset into the buffer.
     * @param length The number of characters to read.
     * 
     * @throws IOException This exception is thrown if the stream has been closed
	 * or another IOException occurs.
     */
    public final void readChars(char[] buffer, int start, int length)
      throws IOException
    {
        final int end = start + length;
        for (int i = start; i < end; i++)
        {
            byte b = readByte();
            if ((b & 0x80) == 0)
	            buffer[i] = (char)(b & 0x7F);
            else if ((b & 0xE0) != 0xE0)
            {
	            buffer[i] = (char)(((b & 0x1F) << 6)
		            | (readByte() & 0x3F));
            } else 
	            buffer[i] = (char)(((b & 0x0F) << 12)
		            | ((readByte() & 0x3F) << 6)
	                |  (readByte() & 0x3F));
        }
    }

    /**
     * Refill the working buffer from the internal representation.
     * 
     * @throws IOException This exception is thrown if the stream has been closed
	 * or another IOException occurs.
     */
    protected final void refill() throws IOException
    {
        long start = m_bufferStart + m_bufferPosition;
        long end = start + BUFFER_SIZE;
        if (end > m_length)	// Don't read past EOF.
            end = m_length;
        m_bufferLength = (int)(end - start);
        if (m_bufferLength == 0)
            throw new IOException("read past EOF");

        if (m_buffer == null)
            m_buffer = new byte[BUFFER_SIZE];	// Allocate buffer lazily.
        readInternal(m_buffer, 0, m_bufferLength);

        m_bufferStart = start;
        m_bufferPosition = 0;
    }

	/**
	 * Read bytes from the internal representation of the data.
	 * 
	 * @param b The buffer to copy the bytes into.
	 * @param offset The offset into <b>b</b> in which to begin copying.
	 * @param length The number of bytes to read into <b>b</b>.
	 * 
	 * @throws IOException This exception is thrown if the stream has been closed
	 * or another IOException occurs.
	 */
    abstract protected void readInternal(byte[] b, int offset, int length)
      throws IOException;

    /**
     * Close the input stream.
     * <p>
     * Subclasses must implement this method.
     * </p>
     * 
     * @throws IOException This exception is thrown if the stream has already
     * been closed.
     */
    abstract public void close() throws IOException;

    /**
     * Get the current file pointer postion.
     * 
     * @return The current file pointer position is returned as a <b>long</b> value.
     */
    public final long getFilePointer()
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
    public final void seek(long pos) throws IOException
    {
        if (pos >= m_bufferStart && pos < (m_bufferStart + m_bufferLength))
            m_bufferPosition = (int)(pos - m_bufferStart);	// Seek within working buffer.
        else
        {
             m_bufferStart = pos;
             m_bufferPosition = 0;
             m_bufferLength = 0;		// Trigger refill() on read().
             seekInternal(pos);
        }
    }

	/**
	 * Seek to the internal position in the file.
	 * 
	 * @param pos The position to seek to.
	 * 
	 * @throws IOException This exception is thrown if the stream has been closed
	 * or another IOException occurs.
	 */
    abstract protected void seekInternal(long pos) throws IOException;

    /**
     * Get the length of the stream.
     * 
     * @return A <b>long</b> value is returned representing the size of the stream
     * (in bytes).
     */
    public final long length()
    {
        return m_length;
    }

    /**
     * Clone this object.
     * 
     * @return A copy of this object is returned.
     */
    public Object clone()
    {
        InputStream clone = null;
        try
        {
            clone = (InputStream)super.clone();
        } catch (CloneNotSupportedException e) {}

        if (m_buffer != null)
        {
            clone.m_buffer = new byte[BUFFER_SIZE];
            System.arraycopy(m_buffer, 0, clone.m_buffer, 0, m_bufferLength);
        }

        clone.m_chars = null;

        return clone;
    }

}

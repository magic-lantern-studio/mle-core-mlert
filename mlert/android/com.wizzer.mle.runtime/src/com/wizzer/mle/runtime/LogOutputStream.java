/*
 * LogOutputStream.java
 * Created on Jan 2, 2008
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
package com.wizzer.mle.runtime;

// Import standard Java classes.
import java.io.IOException;
import java.io.OutputStream;

// Import Android classes.
import android.util.Log;

/**
 * An OutputStream used for redirecting System.out to the Android Logging facility.
 */
public class LogOutputStream extends OutputStream
{
	/** The category tag for System.out messages. */
	public static final String OUTPUT_TAG = "System.out";
	/** The category tag for System.err messages. */
	public static final String ERROR_TAG = "System.err";
	
	// The buffer for managing output.
	private StringBuffer m_buffer;
	// The output tag.
	private String m_tag;
	
	// Hide the default constructor.
	private LogOutputStream() {}
	
	/**
	 * A constructor that establishes the string buffer for managing the
	 * System.out and System.err messages.
	 * 
	 * @param tag The tag to use for Android logging.
	 */
	public LogOutputStream(String tag)
	{
		m_buffer = new StringBuffer();
		if ((tag == null) || (tag.equals("")))
			m_tag = OUTPUT_TAG;
		else
			m_tag = tag;
	}

	/**
	 * Writes the specified byte to this output stream. The general contract for write is that one byte
	 * is written to the output stream. The byte to be written is the eight low-order bits of the argument b.
	 * The 24 high-order bits of b are ignored.
	 * 
	 * @param b The byte to write.
	 * 
	 * @throws IOException This exception is thrown if an I/O error occurs. In particular,
	 * an IOException may be thrown if the output stream has been closed.
	 */
	public void write(int b) throws IOException
	{
		byte v[] = new byte[1];
		v[0] = (byte)(b & 0x000000FF);
		m_buffer.append(new String(v));
	}
	
	/**
	 * Flushes this output stream and forces any buffered output bytes to be written out.
	 * The general contract of flush is that calling it is an indication that,
	 * if any bytes previously written have been buffered by the implementation of the output stream,
	 * such bytes should immediately be written to their intended destination.
	 * 
	 * @throws IOException This exception is thrown if an I/O error occurs.
	 */
	public void flush() throws IOException
	{
		super.flush();
		if (m_tag.equals(ERROR_TAG))
			Log.e(m_tag, m_buffer.toString());
		else
			Log.v(m_tag, m_buffer.toString());
		m_buffer.setLength(0);
	}
}

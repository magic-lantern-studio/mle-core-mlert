/*
 * HeapsortTest.java
 * Created on Dec 9, 2004
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
package com.wizzer.mle.runtime.unittest;

// Import standard Java classes.
import java.io.IOException;

// Import JUnit classes.
import junit.framework.TestCase;

//Import Android classes.
import android.util.Log;

// Import Magic Lantern classes.
import com.wizzer.mle.runtime.MleTitle;
import com.wizzer.mle.runtime.core.MleInputStream;

/**
 * This class is a unit test for com.wizzer.mle.runtime.dpp.MleDppInputStream.
 * 
 * @author Mark S. Millard
 */
public class InputStreamTest extends TestCase
{
	/**
	 * Set up the test case.
	 * 
	 * @see TestCase#setUp()
	 */
    protected void setUp() throws Exception
    {
        super.setUp();
    }

	/**
	 * Tear down the test case.
	 * 
	 * @see TestCase#tearDown()
	 */
    protected void tearDown() throws Exception
    {
        super.tearDown();
    }

    /**
     * Constructor for DppInputStreamTest.
     * 
     * @param name The name of the unit test.
     */
    public InputStreamTest(String name)
    {
        super(name);
    }
    
    // Write a variable-length integer into the specified buffer.
    // The number of valid bytes in the buffer is returned. This return
    // value will be between 1 and 5 bytes.
    private final int writeVInt(int i, byte[] buffer)
    {
    	int len = 0;
        while ((i & ~0x7F) != 0)
        {
            buffer[len] = (byte)((i & 0x7f) | 0x80);
            i >>>= 7;
            len++;
        }
        buffer[len] = (byte)i;
        len++;
        
        return len;
    }
    
	/**
	 *  Test input stream.
	 */
	public void testInputStream()
	{
	    Log.i(MleTitle.DEBUG_TAG,"*** Test DPP Input Stream ***");
	    
	    byte[] numeric = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	                       0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
	                       0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                           0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f };
	    byte[] alpha = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
	                     'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
	    
	    // Test reading a byte.
	    MleInputStream in = new MleInputStream(numeric);
	    for (int i = 0; i < numeric.length; i++)
	    {
	    	try
	    	{
	    		byte n = in.readByte();
	    		TestCase.assertEquals(numeric[i], n);
	    	} catch (IOException ex)
	    	{
	    		TestCase.fail(ex.getMessage());
	    	}
	    }
	    
	    // Test seeking to a position.
	    try {
	    	int pos = 11;
	    	in.seek(pos);
	    	long curPos = in.getFilePointer();
	    	TestCase.assertEquals(pos, curPos);
	    	
	    	byte n = in.readByte();
	    	TestCase.assertEquals(numeric[pos], n);
	    } catch (IOException ex)
    	{
    		TestCase.fail(ex.getMessage());
    	}
	    
	    // Test reading a String.
	    String msg = "All work and no play makes Mark a dull boy.";
	    byte[] len = new byte[5];
	    int vlen = writeVInt(msg.length(),len);

	    byte[] data = new byte[msg.length() + vlen];
	    for (int i = 0; i < vlen; i++)
	    	data[i] = len[i];
	    System.arraycopy(msg.getBytes(), 0, data, vlen, msg.length());
	    
	    in = new MleInputStream(data);
	    try
	    {
	    	String result = in.readString();
	    	TestCase.assertEquals(msg, result);
	    } catch (IOException ex)
    	{
    		TestCase.fail(ex.getMessage());
    	}
	}

}

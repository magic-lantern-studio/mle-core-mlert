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

// Import JUnit classes.
import junit.framework.TestCase;

// Import Magic Lantern classes.
import com.wizzer.mle.runtime.util.MleHeapsortArray;
import com.wizzer.mle.runtime.util.MlePQElement;

/**
 * This class is a unit test for com.wizzer.mle.runtime.util.MleHeapsortArray.
 * 
 * @author Mark S. Millard
 */
public class HeapsortTest extends TestCase
{
	/**
	 * Executes the test runner.
	 * 
	 * @param args Command line arguements.
	 */
    public static void main(String[] args)
    {
        junit.textui.TestRunner.run(HeapsortTest.class);
    }

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
     * Constructor for HeapsortTest.
     * 
     * @param name The name of the unit test.
     */
    public HeapsortTest(String name)
    {
        super(name);
    }
    
	// Test algorithm with a fixed input.
	public void testHeapsort()
	{
	    System.out.println("Test 1: Heapsort Algorithm");
	    
        int [] tk = {3, 2, 1, 5 , 4 , 6, 8, 7 , 8};
	    MleHeapsortArray aha = new MleHeapsortArray(tk.length);
        for (int k = 0; k < tk.length; k++)
        {
            aha.addElement(new MlePQElement(tk[k],null));
        }
        
        System.out.println("Unsorted list: " + aha.toString());
        aha.heapsort();
        
        System.out.println("Sorted list: " + aha.toString());
        TestCase.assertEquals("(1 2 3 4 5 6 7 8 8 )",aha.toString());
	}

}

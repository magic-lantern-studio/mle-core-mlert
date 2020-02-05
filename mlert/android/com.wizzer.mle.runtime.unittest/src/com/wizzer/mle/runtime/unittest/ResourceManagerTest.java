/*
 * ResourceManagerTest.java
 * Created on Jan 8, 2008
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
import android.util.Log;
import junit.framework.TestCase;

// Import Magic Lantern Runtime classes.
import com.wizzer.mle.runtime.MleTitle;
import com.wizzer.mle.runtime.ResourceManager;
import com.wizzer.mle.runtime.core.MleRuntimeException;

// Import unit test classes.
import com.wizzer.mle.runtime.unittest.R;

/**
 * This class is a unit test for com.wizzer.mle.runtime.core.RuntimeManager.
 * 
 * @author Mark S. Millard
 */
public class ResourceManagerTest extends TestCase
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
     * Constructor for ResourceManagerTest.
     * 
     * @param name The name of the unit test.
     */
    public ResourceManagerTest(String name)
    {
        super(name);
    }
    
	// Test algorithm with a fixed input.
	public void testGetResourceId()
	{
	    Log.i(MleTitle.DEBUG_TAG,"*** Test Resource Manager ***");
	    try
	    {
	    	int id = ResourceManager.getResourceId(R.drawable.class, "icon");
	    	TestCase.assertEquals(0x7f020000, id);
	    	id = ResourceManager.getResourceId(R.layout.class, "main");
	    	TestCase.assertEquals(0x7f030000, id);
	    	id = ResourceManager.getResourceId(R.string.class, "app_name");
	    	TestCase.assertEquals(0x7f040001, id);

	    } catch (MleRuntimeException ex)
	    {
	    	TestCase.fail(ex.getLocalizedMessage());
	    }
	    
	    try
	    {
	    	int id = ResourceManager.getResourceId(R.drawable.class, "bogus");
	    	TestCase.fail();  // Should not reach here.
	    } catch (MleRuntimeException ex)
	 	{
	 	   	// Expect to be here.
	 	}
	}
}

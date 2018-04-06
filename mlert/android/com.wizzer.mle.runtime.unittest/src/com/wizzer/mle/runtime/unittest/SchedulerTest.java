/**
 * SchedulerTest.java
 * Created on August 13, 2004
 * Modified on December 27, 2007
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

// Declare packages.
package com.wizzer.mle.runtime.unittest;

// Import JUnit packages.
import junit.framework.TestCase;

// Import Android classes.
import android.util.Log;

//Import Magic Lantern Runtime packges.
import com.wizzer.mle.runtime.MleTitle;
import com.wizzer.mle.runtime.scheduler.MlePhase;
import com.wizzer.mle.runtime.scheduler.MleScheduler;
import com.wizzer.mle.runtime.scheduler.MleTask;


/**
 * This class is a unit test for com.wizzer.mle.runtime.scheduler.MleScheduler.
 */
public class SchedulerTest extends TestCase
{
	// Initial number of phases.
    private static final int m_numPhases = 4;
    // Initial number of tasks.
    private static final int m_numTasks = 5;

    protected static class ThreadTest1 extends Thread
	{
        public void run()
        {
            for (int i = 0; i < 10; i++)
            {
            	Log.i(MleTitle.DEBUG_TAG,"Thread Test 1: " + i);
            }
        }
    }
    
    protected static class ThreadTest2 extends Thread
	{
        public void run()
        {
            for (int i = 0; i < 10; i++)
            {
            	Log.i(MleTitle.DEBUG_TAG,"Thread Test 2: " + i);
            }
        }
    }
    
    protected static class ThreadTest3 extends Thread
	{
        public void run()
        {
            for (int i = 0; i < 10; i++)
            {
            	Log.i(MleTitle.DEBUG_TAG,"Thread Test 3: " + i);
            }
        }
    }
    
    protected static class ThreadTest4 extends Thread
	{
        public void run()
        {
            for (int i = 0; i < 10; i++)
            {
            	Log.i(MleTitle.DEBUG_TAG,"Thread Test 4: " + i);
            }
        }
    }
    
    protected static class ThreadTest5 extends Thread
	{
        public void run()
        {
            for (int i = 0; i < 10; i++)
            {
            	Log.i(MleTitle.DEBUG_TAG,"Thread Test 5: " + i);
            }
        }
    }

    /**
	 * Constructor for DwpWriterTest.
	 * 
	 * @param name The name of the unit test.
	 */
	public SchedulerTest(String name)
	{
		super(name);
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
	 * Test the Scheduler by registering three phases with two tasks to run in each phase.
	 * The scheulder is then run to completion and some of the tasks are removed.
	 */
	public void testScheduler()
	{
        MleScheduler scheduler = new MleScheduler(SchedulerTest.m_numPhases);
        
        /* Register three phases. */
        for (int i = 0; i < 3; i++)
        {
            String str1 = new String("Test Phase ");
            Integer phaseId = new Integer(i+1);
            String name = str1.concat(phaseId.toString());
            MlePhase phase = new MlePhase(name,SchedulerTest.m_numTasks);
            MleTask task;

            /** Create and register task 1 */
            Thread one = new SchedulerTest.ThreadTest1();
            task = new MleTask(one,"Test Task 1");
            phase.addTask(task);
            
            /** Create and register task 2 */
            Thread two = new SchedulerTest.ThreadTest2();
            task = new MleTask(two,"Test Task 2");
            phase.addTask(task);
            
            /** Register phase */
            scheduler.addPhase(phase);
        }
        
        /* Print number of registered phases. */
        Log.i(MleTitle.DEBUG_TAG,"Number of scheduled phases: " + scheduler.getNumberOfPhases());
        
        Log.i(MleTitle.DEBUG_TAG,"*** Test 1 ***");

        TestCase.assertEquals(3,scheduler.getNumberOfPhases());
        for (int i = 0; i < 3; i++)
        {
            MlePhase phase = scheduler.getPhase(i);
            TestCase.assertEquals("Test Phase " + (i+1),phase.getName());
            MleTask task1 = phase.getTask(0);
            TestCase.assertEquals("Test Task 1",task1.getName());
            MleTask task2 = phase.getTask(1);
            TestCase.assertEquals("Test Task 2",task2.getName());
        }
        
        /* Print tasks. */
        scheduler.dump();
        
        /* Invoke tasks, that is execute them. */
        scheduler.run();
        
        /* Delete task 2 from each phase. */
        for (int i = 0; i < 3; i++)
        {
            String str1 = new String("Test Phase ");
            Integer phaseId = new Integer(i+1);
            String name = str1.concat(phaseId.toString());
            MlePhase phase = scheduler.getPhase(name);
            
            if (phase != null)
                phase.deleteTask("Test Task 2");
        }
        
        Log.i(MleTitle.DEBUG_TAG,"*** Test 2 ***");

        TestCase.assertEquals(3,scheduler.getNumberOfPhases());
        for (int i = 0; i < 3; i++)
        {
            MlePhase phase = scheduler.getPhase(i);
            TestCase.assertEquals("Test Phase " + (i+1),phase.getName());
            MleTask task1 = phase.getTask(0);
            TestCase.assertEquals("Test Task 1",task1.getName());
            MleTask task2 = phase.getTask(1);
            TestCase.assertNull(task2);
        }

        /* Print tasks. */
        scheduler.dump();
        
        /* Invoke tasks, that is execute them. */
        scheduler.run();

	}
}

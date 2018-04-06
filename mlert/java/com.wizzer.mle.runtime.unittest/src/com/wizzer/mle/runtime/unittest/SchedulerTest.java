/**
 * SchedulerTest.java
 * Created on August 13, 2004
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

// Import JUnit packatges.
import junit.framework.TestCase;

// Import Magic Lantern Runtime packges.
import com.wizzer.mle.runtime.scheduler.MlePhase;
import com.wizzer.mle.runtime.scheduler.MleScheduler;
import com.wizzer.mle.runtime.scheduler.MleTask;


/**
 * This class is a unit test for com.wizzer.mle.runtime.scheduler.MleScheduler.
 */
public class SchedulerTest extends TestCase
{
    private static final int m_numPhases = 4;
    private static final int m_numTasks = 5;

    protected static class ThreadTest1 extends Thread
	{
        public void run()
        {
            for (int i = 0; i < 10; i++)
            {
                System.out.print("Thread Test 1: ");
                System.out.println(i);
            }
        }
    }
    
    protected static class ThreadTest2 extends Thread
	{
        public void run()
        {
            for (int i = 0; i < 10; i++)
            {
                System.out.print("Thread Test 2: ");
                System.out.println(i);
            }
        }
    }
    
    protected static class ThreadTest3 extends Thread
	{
        public void run()
        {
            for (int i = 0; i < 10; i++)
            {
                System.out.print("Thread Test 3: ");
                System.out.println(i);
            }
        }
    }
    
    protected static class ThreadTest4 extends Thread
	{
        public void run()
        {
            for (int i = 0; i < 10; i++)
            {
                System.out.print("Thread Test 4: ");
                System.out.println(i);
            }
        }
    }
    
    protected static class ThreadTest5 extends Thread
	{
        public void run()
        {
            for (int i = 0; i < 10; i++)
            {
                System.out.print("Thread Test 5: ");
                System.out.println(i);
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
	 * Executes the test runner.
	 * 
	 * @param args Command line arguements.
	 */
	public static void main(String[] args)
	{
		junit.textui.TestRunner.run(SchedulerTest.class);
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
            task = new MleTask(one,"Test 1");
            phase.addTask(task);
            
            /** Create and register task 2 */
            Thread two = new SchedulerTest.ThreadTest2();
            task = new MleTask(two,"Test 2");
            phase.addTask(task);
            
            /** Register phase */
            scheduler.addPhase(phase);
        }
        
        /* Print number of registered phases. */
        System.out.print("Number of scheduled phases: ");
        System.out.println(scheduler.getNumberOfPhases());
        
        System.out.println("*** Test 1 ***");

        TestCase.assertEquals(3,scheduler.getNumberOfPhases());
        for (int i = 0; i < 3; i++)
        {
            MlePhase phase = scheduler.getPhase(i);
            TestCase.assertEquals("Test Phase " + (i+1),phase.getName());
            MleTask task1 = phase.getTask(0);
            TestCase.assertEquals("Test 1",task1.getName());
            MleTask task2 = phase.getTask(1);
            TestCase.assertEquals("Test 2",task2.getName());
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
                phase.deleteTask("Test 2");
        }
        
        System.out.println("*** Test 2 ***");

        TestCase.assertEquals(3,scheduler.getNumberOfPhases());
        for (int i = 0; i < 3; i++)
        {
            MlePhase phase = scheduler.getPhase(i);
            TestCase.assertEquals("Test Phase " + (i+1),phase.getName());
            MleTask task1 = phase.getTask(0);
            TestCase.assertEquals("Test 1",task1.getName());
            MleTask task2 = phase.getTask(1);
            TestCase.assertNull(task2);
        }

        /* Print tasks. */
        scheduler.dump();
        
        /* Invoke tasks, that is execute them. */
        scheduler.run();

	}
}

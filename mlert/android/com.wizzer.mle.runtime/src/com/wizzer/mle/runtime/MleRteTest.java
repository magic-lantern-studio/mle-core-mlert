/**
 * MleRTE.java
 * Created on December 27, 2007
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
import java.io.BufferedOutputStream;
import java.io.PrintStream;

// Import Android classes.
import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;
import android.util.Log;

// Import Magic Lantern classes.
import com.wizzer.mle.runtime.LogOutputStream;
import com.wizzer.mle.runtime.scheduler.*;

/**
 * This class is a unit test for the Magic Lantern Runtime Engine.
 * 
 * @author Mark Millard
 */
public class MleRteTest extends Activity
{
	/** The View for outputting status of test. */
	public TextView m_tv = null;
	
	// Initial number of phases.
    private static final int m_numPhases = 4;
    // Initial number of tasks (per phase).
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
    
    public MleRteTest()
    {
    	System.out.close();
    	PrintStream out = new PrintStream(new BufferedOutputStream(new LogOutputStream(LogOutputStream.OUTPUT_TAG)));
    	System.setOut(out);
    }

	/**
	 * Test the Scheduler by registering three phases with two tasks to run in each phase.
	 * The scheulder is then run to completion and some of the tasks are removed.
	 */
	public void testScheduler()
	{
        MleScheduler scheduler = new MleScheduler(MleRteTest.m_numPhases);
        
        /* Register three phases. */
        for (int i = 0; i < 3; i++)
        {
            String str1 = new String("Test Phase ");
            Integer phaseId = new Integer(i+1);
            String name = str1.concat(phaseId.toString());
            MlePhase phase = new MlePhase(name,MleRteTest.m_numTasks);
            MleTask task;

            /** Create and register task 1 */
            Thread one = new MleRteTest.ThreadTest1();
            task = new MleTask(one,"Test Task 1");
            phase.addTask(task);
            
            /** Create and register task 2 */
            Thread two = new MleRteTest.ThreadTest2();
            task = new MleTask(two,"Test Task 2");
            phase.addTask(task);
            
            /** Register phase */
            scheduler.addPhase(phase);
        }
        
        /* Print number of registered phases. */
        Log.i(MleTitle.DEBUG_TAG,"Number of scheduled phases: " + scheduler.getNumberOfPhases());
        
        Log.i(MleTitle.DEBUG_TAG,"*** Test 1 ***");

        //TestCase.assertEquals(3,scheduler.getNumberOfPhases());
        for (int i = 0; i < 3; i++)
        {
            MlePhase phase = scheduler.getPhase(i);
            //TestCase.assertEquals("Test Phase " + (i+1),phase.getName());
            m_tv.append(phase.getName() + "\n");
            MleTask task1 = phase.getTask(0);
            //TestCase.assertEquals("Test Task 1",task1.getName());
            m_tv.append(task1.getName() + "\n");
            MleTask task2 = phase.getTask(1);
            //TestCase.assertEquals("Test Task 2",task2.getName());
            m_tv.append(task2.getName() + "\n");
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

        //TestCase.assertEquals(3,scheduler.getNumberOfPhases());
        for (int i = 0; i < 3; i++)
        {
            MlePhase phase = scheduler.getPhase(i);
            //TestCase.assertEquals("Test Phase " + (i+1),phase.getName());
            m_tv.append(phase.getName() + "\n");
            MleTask task1 = phase.getTask(0);
            //TestCase.assertEquals("Test Task 1",task1.getName());
            m_tv.append(task1.getName() + "\n");
            MleTask task2 = phase.getTask(1);
            //TestCase.assertNull(task2);
        }

        /* Print tasks. */
        scheduler.dump();
        
        /* Invoke tasks, that is execute them. */
        scheduler.run();
	}

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle icicle)
    {
        super.onCreate(icicle);
        
        // Create a View for displaying test status.
        m_tv = new TextView(this);
        m_tv.append("Hello, Magic Lantern Android\n");
        //setContentView(R.layout.main);
        setContentView(m_tv);
    }
    
    /** Called when the activity will start interacting with the user. */
    @Override
    public void onResume()
    {
    	super.onResume();
    	
    	// Clear the display.
    	m_tv.setText("");
    	System.out.println("***** Running Scheduler Test *****");
    	System.out.flush();
    	
        // Test the Magic Lantern Scheduler.
        testScheduler();
    }
}
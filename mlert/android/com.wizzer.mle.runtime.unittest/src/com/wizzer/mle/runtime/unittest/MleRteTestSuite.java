// Declare package.
package com.wizzer.mle.runtime.unittest;

//Import JUnit packages.
import junit.framework.TestCase;
import junit.framework.TestResult;

// Import Android classes.
import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

/**
 * This class implements a Test Suite for the Magic Lantern Runtime Engine.
 * 
 * @author Mark S. Millard
 */
public class MleRteTestSuite extends Activity
{
	// The View for outputting status of test.
	public TextView m_tv = null;

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle icicle)
    {
        super.onCreate(icicle);
        
        // Create a View for displaying test status.
        m_tv = new TextView(this);
        setContentView(m_tv);
    }
    
    /** Called when the activity will start interacting with the user. */
    @Override
    public void onResume()
    {
    	super.onResume();
    	
    	// Clear the display.
    	m_tv.setText("Running Magic Lantern Test Suite\n");
    	
    	// Test the Scheduler.
    	m_tv.append("*** Scheduler Test ***\n");
    	TestCase schedulerTest = new SchedulerTest("Scheduler Test")
    	{
    		public void runTest()
    		{
    			testScheduler();
    		}
    	};
    	TestResult result = schedulerTest.run();
    	if (result.wasSuccessful())
    		m_tv.append("\tSUCCESSFUL\n");
    	else
    		m_tv.append("\tUNSUCCESSFUL\n");
    	
    	// Test the Dispatch Manager
    	m_tv.append("*** Event Dispatcher Test ***\n");
    	TestCase dispatcherTest = new EventDispatcherTest("Event Dispatcher Test")
    	{
    		public void runTest()
    		{
    			testImmediateMode();
    			testDelayedMode();
    			testDisableEventCB();
    			testDisableEvent();
    			testFlush();
    			testPrioritizedCB();
    		}
    	};
    	result = dispatcherTest.run();
    	if (result.wasSuccessful())
    		m_tv.append("\tSUCCESSFUL\n");
    	else
    		m_tv.append("\tUNSUCCESSFUL\n");
    	
    	// Test the Heapsort array.
    	m_tv.append("*** Heapsort Test ***\n");
    	TestCase heapsortTest = new HeapsortTest("Heapsort Test")
    	{
    		public void runTest()
    		{
    			testHeapsort();
    		}
    	};
    	result = heapsortTest.run();
    	if (result.wasSuccessful())
    		m_tv.append("\tSUCCESSFUL\n");
    	else
    		m_tv.append("\tUNSUCCESSFUL\n");
    	
    	// Test the Priority Queue.
    	m_tv.append("*** Priority Queue Test ***\n");
    	TestCase pqTest = new PQTest("Priority Queue Test")
    	{
    		public void runTest()
    		{
    			testInsertionRemoval();
    			testQueueGrowth();
    			testClear();
    			testDestroyItem();
    			testRemove();
    			testJoin();
    			testChangeItem();
    			testDestroy();
    		}
    	};
    	result = pqTest.run();
    	if (result.wasSuccessful())
    		m_tv.append("\tSUCCESSFUL\n");
    	else
    		m_tv.append("\tUNSUCCESSFUL\n");
    	
    	// Test the Resource Manager.
    	m_tv.append("*** Resource Manager Test ***\n");
    	TestCase resourceMgrTest = new ResourceManagerTest("Resource Manager Test")
    	{
    		public void runTest()
    		{
    			testGetResourceId();
    		}
    	};
    	result = resourceMgrTest.run();
    	if (result.wasSuccessful())
    		m_tv.append("\tSUCCESSFUL\n");
    	else
    		m_tv.append("\tUNSUCCESSFUL\n");

       	// Test the Input Stream.
    	m_tv.append("*** Input Stream Test ***\n");
    	TestCase InputStreamTest = new InputStreamTest("Input Stream Test")
    	{
    		public void runTest()
    		{
    			testInputStream();
    		}
    	};
    	result = InputStreamTest.run();
    	if (result.wasSuccessful())
    		m_tv.append("\tSUCCESSFUL\n");
    	else
    		m_tv.append("\tUNSUCCESSFUL\n");
    }
}
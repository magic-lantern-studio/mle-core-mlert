/*
 * EventDispatcherTest.java
 * Created on Aug 17, 2004
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

// Import JUnit test.
import junit.framework.TestCase;

// Import Magic Lantern Runtime packages.
import com.wizzer.mle.runtime.core.IMleCallbackId;
import com.wizzer.mle.runtime.core.MleRuntimeException;
import com.wizzer.mle.runtime.event.IMleEventCallback;
import com.wizzer.mle.runtime.event.MleEvent;
import com.wizzer.mle.runtime.event.MleEventCallback;
import com.wizzer.mle.runtime.event.MleEventDispatcher;


/**
 * This class is a unit test for com.wizzer.mle.runtime.event.MleEventDispatcher.
 * 
 * @author Mark S. Millard
 */
public class EventDispatcherTest extends TestCase
{
    // The current state.
    private int m_state = MleEvent.MLE_EVENT_INVALID_ID;
    // The states in the state machine.
    private int[] m_states;
    // The callback ids.
    private IMleCallbackId[] m_ids;
    
    // The event dispatch manager.
    MleEventDispatcher m_machine;

    protected class State extends MleEventCallback
    {
        // The state name.
        private String m_name = "UNKNOWN";
        
        /**
         * The default constructor.
         */
        public State()
        {
            super();
        }
        
    	/**
    	 * A constructor that initializes the state.
    	 * 
    	 * @param name The state name.
    	 */
    	public State(String name)
    	{
    	    super();
    	    m_name = name;
    	}

        /**
         * Execute this state that is
         * 
         * @param event The event that is being dispatched by the handler.
         * @param clientdata Client data registered with this handler.
         * 
         * @return If the event is successfully dispatched, then <b>true</b> will be
         * returned. Otherwise, <b>false</b> will be returned.
         * 
     	 * @see com.wizzer.mle.runtime.event.IMleEventCallback#dispatch(com.wizzer.mle.runtime.event.MleEvent, java.lang.Object)
    	 */
    	public boolean dispatch(MleEvent event,Object clientdata)
    	{
    	    boolean retValue = true;
    	    
    	    // Do some work.
 		    System.out.println("Executing Callback: " + m_name + "\n" +
		        "\tCaller Data: " + event.getCallData() + "\n" +
		        "\tClient Data: " + clientdata);
		    m_state = event.getId();
    		
    		return retValue;
    	}
}

	/**
	 * Executes the test runner.
	 * 
	 * @param args Command line arguements.
	 */
	public static void main(String[] args)
	{
		junit.textui.TestRunner.run(EventDispatcherTest.class);
	}

	/**
	 * Set up the test case.
	 * 
	 * @see TestCase#setUp()
	 */
	protected void setUp() throws Exception
	{
		super.setUp();
		
		// Create the event dispatcher.
	    m_machine = new MleEventDispatcher();
	    
	    // Initialize the composite event identifiers.
	    m_states = new int[5];
	    // State 0.
	    m_states[0] = MleEvent.makeId((short)0x0000,(short)0x0001);
	    // State 1.
	    m_states[1] = MleEvent.makeId((short)0x0000,(short)0x0002);
	    // State 2.
	    m_states[2] = MleEvent.makeId((short)0x0000,(short)0x0003);
	    // State 3.
	    m_states[3] = MleEvent.makeId((short)0x0000,(short)0x0004);
	    // State 4.
	    m_states[4] = MleEvent.makeId((short)0x0000,(short)0x0005);
	    
	    m_ids = new IMleCallbackId[5];

	}

	/**
	 * Tear down the test case.
	 * 
	 * @see TestCase#tearDown()
	 */
	protected void tearDown() throws Exception
	{
		super.tearDown();
		m_machine = null;
		m_states = null;
		m_ids = null;
	}

	/**
	 * Constructor for EventDispatcherTest.
	 * 
	 * @param name The name of the unit test.
	 */
	public EventDispatcherTest(String name)
	{
		super(name);
	}
	
	// Execute MainLoop.
	private void mainloop()
	{
	    String msg;
	    for (int i = 0; i < m_states.length; i++)
	    {
	        msg = new String("Processing State = " + m_states[i]);
	        m_machine.processEvent(m_states[i],msg,IMleEventCallback.MLE_EVENT_IMMEDIATE);
	    }
	}
	
	// Initialize the state machine.
	private void initStateMachine(State cb)
	{
	    // Add the handlers to the event dispatcher.
	    try
	    {
		    m_ids[0] = m_machine.installEventCB(m_states[0],cb,new String("Next State = " + m_states[1]));
		    m_ids[1] = m_machine.installEventCB(m_states[1],cb,new String("Next State = " + m_states[2]));
		    m_ids[2] = m_machine.installEventCB(m_states[2],cb,new String("Next State = " + m_states[3]));
		    m_ids[3] = m_machine.installEventCB(m_states[3],cb,new String("Next State = " + m_states[4]));
		    m_ids[4] = m_machine.installEventCB(m_states[4],cb,new String("Next State = " + m_states[0]));
	    } catch (MleRuntimeException ex)
	    {
	        TestCase.fail("Magic Lantern Runtime Exception occurred.");
	    }
	    
	    // Enable the handlers.
	    m_machine.enableEventCB(m_states[0],m_ids[0]);
	    m_machine.enableEventCB(m_states[1],m_ids[1]);
	    m_machine.enableEventCB(m_states[2],m_ids[2]);
	    m_machine.enableEventCB(m_states[3],m_ids[3]);
	    m_machine.enableEventCB(m_states[4],m_ids[4]);

	}
	
	/**
	 * Test the MleEventDispatcher using a simple state machine.
	 * Process the events in MLE_IMMEDIATE_MODE;
	 */
	public void testImmediateMode()
	{
	    // Create the state machine callback handlers.
	    State stateCB = new State();
	    
	    initStateMachine(stateCB);
	    
	    // Process the state machine.
	    System.out.println("*** Test installEventCB() ***");
	    this.mainloop();
	}

	/**
	 * Test the MleEventDispatcher using a simple state machine.
	 * Process the events in MLE_DELAYED_MODE;
	 */
	public void testDelayedMode()
	{
	    // Create the state machine callback handlers.
	    State stateCB = new State();
	    
	    initStateMachine(stateCB);
	    
	    // Process the state machine.
	    for (int i = 0; i < m_states.length; i++)
	    {
	        String msg = new String("Processing State = " + m_states[i]);
	        m_machine.processEvent(m_states[i],msg,IMleEventCallback.MLE_EVENT_DELAYED,m_states[i]);
	    }

	    System.out.println("*** Test dispatchEvents() ***");
	    m_machine.dispatchEvents();
	}
	
	public void testDisableEventCB()
	{
	    // Create the state machine callback handlers.
	    State stateCB = new State();
	    
	    initStateMachine(stateCB);
	    
	    // Disable some handlers.
	    m_machine.disableEventCB(m_states[0],m_ids[0]);
	    m_machine.disableEventCB(m_states[2],m_ids[2]);
	    m_machine.disableEventCB(m_states[4],m_ids[4]);

	    // Process the state machine.
	    System.out.println("*** Test disableEventCB() ***");
	    this.mainloop();
	}

	public void testDisableEvent()
	{
	    // Create the state machine callback handlers.
	    State stateCB = new State();
	    
	    initStateMachine(stateCB);
	    
	    // Disable some events.
	    m_machine.disableEvent(m_states[0]);
	    m_machine.disableEvent(m_states[1]);
	    m_machine.disableEvent(m_states[2]);
	    m_machine.disableEvent(m_states[4]);

	    // Process the state machine.
	    System.out.println("*** Test disableEvent() ***");
	    this.mainloop();
	}
	
	public void testFlush()
	{
	    // Create the state machine callback handlers.
	    State stateCB = new State();
	    
	    initStateMachine(stateCB);
	    
	    // Process the state machine.
	    for (int i = 0; i < m_states.length; i++)
	    {
	        String msg = new String("Processing State = " + m_states[i]);
	        m_machine.processEvent(m_states[i],msg,IMleEventCallback.MLE_EVENT_DELAYED,m_states[i]);
	    }
	    
	    m_machine.flush();

	    System.out.println("*** Test flush() ***");
	    m_machine.dispatchEvents();
	}

	public void testPrioritizedCB()
	{
	    // Create the state machine callback handlers.
	    State cb1 = new State("One");
	    State cb2 = new State("Two");
	    State cb3 = new State("Three");
	    State cb4 = new State("Four");
	    State cb5 = new State("Five");
	    
	    // Add the handlers to the event dispatcher.
	    try
	    {
		    m_ids[0] = m_machine.installEventCB(m_states[0],cb1,new String("Next State = " + 1));
		    m_ids[1] = m_machine.installEventCB(m_states[0],cb2,new String("Next State = " + 2));
		    m_ids[2] = m_machine.installEventCB(m_states[0],cb3,new String("Next State = " + 3));
		    m_ids[3] = m_machine.installEventCB(m_states[0],cb4,new String("Next State = " + 4));
		    m_ids[4] = m_machine.installEventCB(m_states[0],cb5,new String("Next State = " + 0));
	    } catch (MleRuntimeException ex)
	    {
	        TestCase.fail("Magic Lantern Runtime Exception occurred.");
	    }
	    
	    // Enable the handlers.
	    m_machine.changeCBPriority(m_states[0],m_ids[0],5);
	    m_machine.changeCBPriority(m_states[0],m_ids[1],4);
	    m_machine.changeCBPriority(m_states[0],m_ids[2],3);
	    m_machine.changeCBPriority(m_states[0],m_ids[3],2);
	    m_machine.changeCBPriority(m_states[0],m_ids[4],1);

	    // Process the state machine.
	    String msg = new String("Processing Event 0 ");
	    m_machine.processEvent(m_states[0],msg,IMleEventCallback.MLE_EVENT_DELAYED);

	    System.out.println("*** Test Prioritized Callbacks ***");
	    m_machine.dispatchEvents();
	}
}

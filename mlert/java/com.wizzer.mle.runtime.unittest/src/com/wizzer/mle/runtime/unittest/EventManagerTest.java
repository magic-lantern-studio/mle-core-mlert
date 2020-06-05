// COPYRIGHT_BEGIN
// COPYRIGHT_END

// Declare package.
package com.wizzer.mle.runtime.unittest;

// Import JUnit classes.
import com.wizzer.mle.runtime.core.MleRuntimeException;
import com.wizzer.mle.runtime.event.MleEventManager;

import junit.framework.TestCase;

/**
 *  This class is a unit test for com.wizzer.mle.runtime.event.MleEventManager.
 * 
 * @author Mark S. Millard
 */
public class EventManagerTest extends TestCase
{
	/**
	 * Set up the test case.
	 * 
	 * @see TestCase#setUp()
	 */
	protected void setUp() throws Exception
	{
		super.setUp();
		MleEventManager.getInstance().clear();
	}

	/**
	 * Tear down the test case.
	 * 
	 * @see TestCase#tearDown()
	 */
	protected void tearDown() throws Exception
	{
		super.tearDown();
		MleEventManager.getInstance().clear();
	}

	/**
	 * Test retrieving the singleton instance of event manager.
	 */
	public void testGetInstance()
	{
		MleEventManager theManager = MleEventManager.getInstance();
		assertNotNull(theManager);
	}

	/**
	 * Test adding events to the event manager.
	 */
	public void testAddEvent()
	{
		try
		{
			int event = MleEventManager.createEvent((short)0);
			assertEquals(event, 0);
			MleEventManager.getInstance().addEvent(event, null);
			// Try to add it again, should not be able to.
			MleEventManager.getInstance().addEvent(event, null);
			assertEquals(MleEventManager.getInstance().size(), 1);
		} catch (MleRuntimeException ex)
		{
			fail("MleRuntimeException thrown: " + ex.getMessage());
		}
	}

	/**
	 * Test removing events from the event manager.
	 */
	public void testRemoveEvent()
	{
		try
		{
			int event = MleEventManager.createEvent((short)0);
			assertEquals(event, 0);
			MleEventManager.getInstance().addEvent(event, null);
			event = MleEventManager.createEvent((short)0);
			assertEquals(event, 1);
			MleEventManager.getInstance().addEvent(event, null);
			MleEventManager.getInstance().removeEvent(event);
			assertEquals(MleEventManager.getInstance().size(), 1);
		} catch (MleRuntimeException ex)
		{
			fail("MleRuntimeException thrown: " + ex.getMessage());
		}
	}

	/**
	 * Test creating new events.
	 */
	public void testCreateEvent()
	{
		try
		{
			int event = MleEventManager.createEvent((short)0);
			assertEquals(event, 0);
			MleEventManager.getInstance().addEvent(event, null);
			event = MleEventManager.createEvent((short)0);
			assertEquals(event, 1);
			MleEventManager.getInstance().addEvent(event, null);
			event = MleEventManager.createEvent((short)0);
			assertEquals(event, 2);
			MleEventManager.getInstance().addEvent(event, null);
			event = MleEventManager.createEvent((short)0);
			assertEquals(event, 3);
			MleEventManager.getInstance().addEvent(event, null);
			event = MleEventManager.createEvent((short)0);
			assertEquals(event, 4);
			MleEventManager.getInstance().addEvent(event, null);
			event = MleEventManager.createEvent((short)1);
			assertEquals(event, 65536);
			MleEventManager.getInstance().addEvent(event, null);
			event = MleEventManager.createEvent((short)1);
			assertEquals(event, 65537);
			MleEventManager.getInstance().addEvent(event, null);
			event = MleEventManager.createEvent((short)1);
			assertEquals(event, 65538);
			MleEventManager.getInstance().addEvent(event, null);
			event = MleEventManager.createEvent((short)1);
			assertEquals(event, 65539);
			MleEventManager.getInstance().addEvent(event, null);
			event = MleEventManager.createEvent((short)1);
			assertEquals(event, 65540);
			MleEventManager.getInstance().addEvent(event, null);
			event = MleEventManager.createEvent((short)2);
			assertEquals(event, 131072);
			event = MleEventManager.createEvent((short)0);
			assertEquals(event, 5);
			
			assertEquals(MleEventManager.getInstance().size(), 10);
		} catch (MleRuntimeException ex)
		{
			fail("MleRuntimeException thrown: " + ex.getMessage());
		}
	}

	public void testHasEvent()
	{
		try
		{
			int event = MleEventManager.createEvent((short)0);
			assertEquals(event, 0);
			MleEventManager.getInstance().addEvent(event, null);
			event = MleEventManager.createEvent((short)0);
			assertEquals(event, 1);
			MleEventManager.getInstance().addEvent(event, null);
	        assertTrue(MleEventManager.getInstance().hasEvent(0));
	        assertTrue(MleEventManager.getInstance().hasEvent(1));
			event = MleEventManager.createEvent((short)1);
			assertEquals(event, 65536);
			MleEventManager.getInstance().addEvent(event, null);
			assertTrue(MleEventManager.getInstance().hasEvent(65536));
		} catch (MleRuntimeException ex)
		{
			fail("MleRuntimeException thrown: " + ex.getMessage());
		}
	}
	
	public void testHasEventWithName()
	{
		try
		{
			int event = MleEventManager.createEvent((short)0);
			assertEquals(event, 0);
			MleEventManager.getInstance().addEvent(event, null);
			event = MleEventManager.createEvent((short)0);
			assertEquals(event, 1);
			MleEventManager.getInstance().addEvent(event, "One");
	        assertTrue(MleEventManager.getInstance().hasEvent(0));
	        assertTrue(MleEventManager.getInstance().hasEvent(1));
	        assertTrue(MleEventManager.getInstance().hasEvent("One"));
			event = MleEventManager.createEvent((short)1);
			assertEquals(event, 65536);
			MleEventManager.getInstance().addEvent(event, "Big");
			assertTrue(MleEventManager.getInstance().hasEvent(65536));
			assertTrue(MleEventManager.getInstance().hasEvent("Big"));
			assertFalse(MleEventManager.getInstance().hasEvent("Two"));
		} catch (MleRuntimeException ex)
		{
			fail("MleRuntimeException thrown: " + ex.getMessage());
		}
	}
}

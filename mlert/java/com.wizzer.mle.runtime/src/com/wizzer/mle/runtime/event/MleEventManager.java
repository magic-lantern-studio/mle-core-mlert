// COPYRIGHT_BEGIN
// COPYRIGHT_END

// Declare package.
package com.wizzer.mle.runtime.event;

// Import standard Java classes.
import java.util.Iterator;
import java.util.TreeSet;

import com.wizzer.mle.runtime.core.MleRuntimeException;

/**
 * This class is used to help manage events. A registry is kept for bookkeeping,
 * in order to generate unique event identifiers on-the-fly.
 * 
 * @author Mark S. Millard
 */
public class MleEventManager
{
    /** The event group reserved by the manager. */
    public static final short MLE_SYSTEM_GROUP = 0;
    
    /** The paint event. */
    public static final int MLE_PAINT = MleEvent.makeId(MLE_SYSTEM_GROUP,(short)0);
    /** The size/resize event. */
    public static final int MLE_SIZE = MleEvent.makeId(MLE_SYSTEM_GROUP,(short)1);
    /** The resize paint event. */
    public static final int MLE_RESIZEPAINT = MleEvent.makeId(MLE_SYSTEM_GROUP,(short)2);
    /** The exit event. */
    public static final int MLE_QUIT = MleEvent.makeId(MLE_SYSTEM_GROUP,(short)3);

    /** The first composite event in the range of reserved events. */
    public static final int MLE_FIRST_EVENT = MLE_PAINT;
    /** The last composite event in the range of reserved events. */
    public static final int MLE_LAST_EVENT = MLE_QUIT;

    //  Constants for event dispatching priorities. Note that this ordering is
    //  important. The Stage resize callback must be executed prior to the
    //  Set resize callback because the Set uses the new off-screen buffer
    //  which is reallocated by the Stage resize callback. Also, the Stage
    //  resize paint callback is executed after all other resize events have
    //  been processed. The resize paint callback redraws the world in the new
    //  off-screen buffer.
 	
    /** The priority for dispatching the MLE_RESIZE event for a Stage. */
    public static final int MLE_RESIZE_STAGE_PRIORITY = 10;
    /** The priority for dispatching the MLE_RESIZE event for a Set. */
    public static final int MLE_RESIZE_SET_PRIORITY = 9;
    /** The priority for dispatching the MLE_RESIZEPAINT event for a Stage. */
    public static final int MLE_RESIZEPAINT_STAGE_PRIORITY = -1;

	// The singleton instance of the event manager.
    private static MleEventManager g_theEventManager = null;
    
    // Flag indicating whether it is Ok to exit.
    private static boolean g_okToExit = false;
    
    /**
     * Utility class used to manage named events.
     * 
     * @author Mark S. Millard
     */
    public class EventSetItem implements Comparable<EventSetItem>
    {
    	/** The event identifier. */
    	public Integer m_id;
    	/** The named event. May be <b>null</b>. */
    	public String m_name;

		@Override
		public int compareTo(EventSetItem item)
		{
			if (m_id < item.m_id) return -1;
			else if (m_id > item.m_id) return 1;
			else return 0;
		}
		
		@Override
    	public boolean equals(Object obj)
    	{
    		boolean retValue = false;
    		if (obj instanceof EventSetItem)
    		{
    			EventSetItem item = (EventSetItem) obj;
    			if (item.m_id.equals(m_id))
    				retValue = true;
    		}
    		return retValue;
		}
    }

    // Helper class managing registered events.
    private class EventSet extends TreeSet<EventSetItem>
    {
		// The unique serialization identifier.
		private static final long serialVersionUID = 6007959591361849913L;

        /**
         * Find the registered <code>EventSetItem</code> for the specified event
         * identifier.
         * 
         * @param id The event identifier.
         * 
         * @return The registered <code>EventSetItem</code> will be returned.
         */
		public synchronized EventSetItem find(int id)
		{
			EventSetItem found = null;
			
			Iterator<EventSetItem> iter = m_eventRegistry.iterator();
			while (iter.hasNext())
			{
				EventSetItem next = iter.next();
				if (next.m_id.intValue() == id)
				{
					found = next;
					break;
				}
			}

			return found;
		}
		
		/**
         * Find the registered <code>EventSetItem</code> for the specified
         * name.
         * 
         * @param name The event name.
         * 
         * @return The registered <code>EventSetItem</code> will be returned.
         */
		public synchronized EventSetItem find(String name)
		{
			EventSetItem found = null;
			
			Iterator<EventSetItem> iter = m_eventRegistry.iterator();
			while (iter.hasNext())
			{
				EventSetItem next = iter.next();
				if ((next.m_name != null) && (next.m_name.equals(name)))
				{
					found = next;
					break;
				}
			}

			return found;
		}
		
		/**
		 * Get the last event for the specified event group.
		 * 
		 * @param group The group identifier for the event category.
		 * 
		 * @return The registered <code>EventSetItem</code> will be returned for
		 * the last event in the specified group. <b>null</b> may be returned
		 * if there is no registered event.
		 */
		public synchronized EventSetItem last(short group)
		{
			EventSetItem found = null;
			short lastEid = 0;
			
			Iterator<EventSetItem> iter = m_eventRegistry.iterator();
			while (iter.hasNext())
			{
				EventSetItem next = iter.next();
				short gid = MleEvent.getGroupId(next.m_id.intValue());
				if (gid == group)
				{
					short eid = MleEvent.getEventId(next.m_id.intValue());
					if (eid >= lastEid) 
					{
						lastEid = eid;
						found = next;
					}
				}
			}
			
			return found;
		}
		
		/**
		 * Get the first event for the specified event group.
		 * 
		 * @param group The group identifier for the event category.
		 * 
		 * @return The registered <code>EventSetItem</code> will be returned for
		 * the first event in the specified group. <b>null</b> may be returned
		 * if there is no registered event.
		 */
		public synchronized EventSetItem first(short group)
		{
			EventSetItem found = null;
			
			Iterator<EventSetItem> iter = m_eventRegistry.iterator();
			while (iter.hasNext())
			{
				EventSetItem next = iter.next();
				short gid = MleEvent.getGroupId(next.m_id.intValue());
				if (gid == group)
				{
					found = next;
					break;
				}
			}
			
			return found;
		}
    };
    
    // The event registry.
    private EventSet m_eventRegistry = new EventSet();
    
    // Hide the default constructor.
    private MleEventManager()
    {
    	// Add system default events.
    	try {
			addEvent(MLE_PAINT, null);
	    	addEvent(MLE_SIZE, null);
	    	addEvent(MLE_RESIZEPAINT, null);
	    	addEvent(MLE_QUIT, null);
		} catch (MleRuntimeException e) {
			// Should never occur.
		}

    }
    
    /**
     * Get the Singleton instance of the event manager.
     * 
     * @return The Singleton is returned.
     */
    public static MleEventManager getInstance()
    {
    	if (g_theEventManager == null)
    		g_theEventManager = new MleEventManager();
    	return g_theEventManager;
    }
    
    /**
     * Determine if the event manager already contains the specified event.
     * 
     * @param id The event identifier to test.
     * 
     * @return <b>true</b> will be returned if the event manager already
     * has the specified event registered. Otherwise, <b>false</b> will be
     * returned.
     */
    public boolean hasEvent(int id)
    {
    	EventSetItem item = new EventSetItem();
    	item.m_id = new Integer(id);
    	item.m_name = null;
    	if (m_eventRegistry.contains(item))
    		return true;
    	else
    		return false;
    }
    
    /**
     * Determine if the event manager already contains the named event.
     * 
     * @param name The event name to test.
     * 
     * @return <b>true</b> will be returned if the event manager already
     * has the specified event registered. Otherwise, <b>false</b> will be
     * returned.
     */
    public boolean hasEvent(String name)
    {
    	if (m_eventRegistry.find(name) != null)
    		return true;
    	else
    		return false;
    }
    
    /**
     * Register an event with the event manager.
     * 
     * @param id The event identifier. Nothing will be done if the event has
     * previously been registered.
     * @param name The named event; must be unique or <b>null</b>.
     * 
     * @throws MleRuntimeException This exception is thrown if an event
     * already exists for the specified <i>name</i>.
     */
    public void addEvent(int id, String name) throws MleRuntimeException
    {
    	EventSetItem item = new EventSetItem();
    	item.m_id = new Integer(id);
    	item.m_name = name;

    	if (m_eventRegistry.contains(item))
    		return;
    	if ((name != null) && (m_eventRegistry.find(name) != null))
    		throw new MleRuntimeException("Named event already exists.");

    	m_eventRegistry.add(item);
    }
    
    /**
     * Get the event identifier for the named event.
     * 
     * @param name The named event.
     * 
     * @return If the named event is registered, then the value of the
     * event identifier will be returned. If the named event does not
     * exist, then <code>MleEvent.MLE_EVENT_INVALID_ID</code> will be
     * returned.
     */
    public int getEventId(String name)
    {
    	EventSetItem item = m_eventRegistry.find(name);
    	if (item != null)
    		return item.m_id.intValue();
    	else
    		return MleEvent.MLE_EVENT_INVALID_ID;
    }
    
    /**
     * Get the name for the specified event.
     * 
     * @param id The event identifier.
     * 
     * @return If the event is registered, then the name of the
     * event will be returned. This value may be <b>null</b> if
     * the event was not registered with a name.
     * If the event does not exist, then <b>null</b> will be
     * returned.
     */
    public String getEventName(int id)
    {
    	EventSetItem item = m_eventRegistry.find(id);
    	if (item != null)
    		return item.m_name;
    	else
    		return null;
    }
    
    /**
     * Unregister an event from the event manager.
     * 
     * @param id The event identifier. Nothing happens if the registry does not
     * contain the specified event.
     */
    public void removeEvent(int id)
    {
    	EventSetItem item = new EventSetItem();
    	item.m_id = new Integer(id);

    	if (m_eventRegistry.contains(item))
    	{
    		EventSetItem event = m_eventRegistry.find(id);
    		m_eventRegistry.remove(event);
    	}
    }
    
    /**
     * Clear the event manager registry of all events.
     */
    public void clear()
    {
    	m_eventRegistry.clear();
    }
    
    /**
     * Get the number of events registered with the event manager.
     * 
     * @return The number of registered events is returned.
     */
    public int size()
    {
    	return m_eventRegistry.size();
    }
    
    /**
     * Determine whether it is ok to exit.
     * 
     * @return <b>true</b> will be returned if it is Ok to exit.
     * Otherwise, <b>false</b> will be returned.
     */
    public static boolean okToExit()
    {
        return g_okToExit;
    }
    
    /**
     * Set the exit state.
     * 
     * @param status This parameter should be <b>true</b> if it is
     * Ok to exit the application/title. If it is not Ok to exit,
     * then the status should be set to <b>false</b>.
     */
    public static void setExitStatus(boolean status)
    {
        g_okToExit = status;
    }
    
    /**
     * Create an event identifier for the specified group.
     * 
     * @param group The group identifier to create an event for.
     * 
     * @return A new event identifier will be returned. Note that the returned event
     * has not yet been added to the event manager (see addEvent(int)).
     * 
     * @see MleEvent
     */
    public static int createEvent(short group)
    {
    	int id;

    	EventSetItem lastEvent = MleEventManager.getInstance().m_eventRegistry.last(group);
    	if (lastEvent == null)
    	{
    		// First event in the group.
    		id = MleEvent.makeId(group, (short)0);
    	} else
    	{
    		// Create the event identifier following the last one registered.
    		short eid = MleEvent.getEventId(lastEvent.m_id.intValue());
    		eid++;
    		id = MleEvent.makeId(group, eid);
    	}
    	
    	return id;
    }
}

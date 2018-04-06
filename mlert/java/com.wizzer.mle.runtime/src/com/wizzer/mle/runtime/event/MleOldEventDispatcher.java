/**
 * MleEventDispatcher.java
 * Created on October 12, 2000 (msm@wizzerworks.com)
 * Modified on April 28, 2003 (msm@wizzerworks.com)
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
package com.wizzer.mle.runtime.event;

// Import standard Java classes.
import java.util.Hashtable;

// Import Magic Lantern classes.
import com.wizzer.mle.runtime.util.MlePQ;
import com.wizzer.mle.runtime.util.MlePQElement;


/**
 * <code>MleEventDispatcher</code> is used to synchronize the dispatching of
 * Magic Lantern runtime events.
 *
 * @author  Mark S. Millard
 * @version 1.0
 */
public class MleOldEventDispatcher extends Object
{
    /*
     * A helper class to encapsulate a queue element.
     */
    private class EventQueueElement extends Object
    {
    	/** The event. */
        public MleEvent m_event = null;
        /** The event caller data. */
        public Object m_calldata = null;
        
        // Hide the default EventQueueElement constructor.
        private EventQueueElement()
        {
        	// Should never be called!
        }
        
		/**
		 * Construct an queue element to hold a MleEvent and its
		 * associated call data.
		 *
		 * @param event A Magic Lantern event.
		 * @param calldata The data to associate with the event.
		 */
        public EventQueueElement(MleEvent event, Object calldata)
        {
            m_event = event;
            m_calldata = calldata;
        }
        
		/**
		 * Get the Magic Lantern event associated with this queue element.
		 *
		 * @return A Magic Lantern event is returned.
		 */
        public MleEvent getEvent()
        {
            return m_event;
        }
        
		/**
		 * Get the caller data associated with this queue element.
		 *
		 * @return A EventObject is returned.
		 */
        public Object getCallData()
        {
            return m_calldata;
        }

    } // end of EventQueueElement

    /*
     * A helper class to associate a callback handler with its clientdata.
     */
    private class EventCallbackObject extends Object
    {
        /** The callback handler. */
        public IMleEventCallback m_cb = null;
        /** The associated client data. */
        public Object m_clientdata = null;
        /** The dispatch priority. */
        public int m_priority = MLE_DEFAULT_PRIORITY;
        /** Flag indicating whether handler is enabled. */
        public boolean m_isEnabled = false;
        
        // Hide the default constructor.
        private EventCallbackObject() {}
        
        /**
         * A constructor used to initialize the callback handler and associated
         * client data.
         * 
         * @param cb The callback handler.
         * @param clientdata The client data.
         * @param int priority The dispatch priority.
         */
        public EventCallbackObject(IMleEventCallback cb, Object clientdata, int priority)
        {
            m_cb = cb;
            m_clientdata = clientdata;
            m_priority = priority;
        }
    }
    
    // The collection of event callbacks by group.
    private Hashtable m_eventCallbacks = null;
    // The event queue.
    private MlePQ m_eventQueue = null;
    
    /** The default priority. */
    public static int MLE_DEFAULT_PRIORITY = 0;

    /**
     * Creates new MleEventDispatcher.
     */
    public MleOldEventDispatcher()
    {
        m_eventCallbacks = new Hashtable();
        m_eventQueue = new MlePQ();
    }

    /**
     * Creates a new MleEventDispatcher with the specified number of
     * event groups.
     *
     * @param numGroups The initial number of groups expected to
     * be managed by the event dispatcher.
     */
    public MleOldEventDispatcher(int numGroups)
    {
        m_eventCallbacks = new Hashtable(numGroups);
        m_eventQueue = new MlePQ();
    }
    
    /**
     * Add an event callback handler.
     * 
 	 * @param id The composite event id.
     * @param cb The <code>IMleEventCallback</code> handler to add.
     * @param clientdata Client data associated with the callback handler.
     * @param priority The event priority.
     * 
     * @return If the handler was successfully added, then <b>true</b> will be
     * returned. Otherwise, <b>false</b> will be returned.
     */
    public boolean addEventCallback(int id, IMleEventCallback cb, Object clientdata,int priority)
    {
        boolean retValue = false;
        Hashtable cbs;

        short groupId = MleEvent.getGroupId(id);
        short eventId = MleEvent.getEventId(id);
        
        Short groupKey = new Short(groupId);
        if (! m_eventCallbacks.containsKey(groupKey))
        {
        	// Create a new collection of event callbacks for the group.
        	cbs = new Hashtable();
        	m_eventCallbacks.put(groupKey,cbs);
        } else
        {
        	// Get the collection of event callbacks for the specified group.
        	cbs = (Hashtable)m_eventCallbacks.get(groupKey);
        }
        
        // Check to see if the event has already been added or not.
        Short eventKey = new Short(eventId);
        if (! cbs.containsKey(eventKey))
        {
        	// Add the event callback.
            EventCallbackObject eco = new EventCallbackObject(cb,clientdata,priority);
        	cbs.put(eventKey,eco);
        	retValue = true;
        }
        
        return retValue;
    }

    /**
     * Add an event callback handler. The default priority is used.
     * 
 	 * @param id The composite event id.
     * @param cb The <code>IMleEventCallback</code> handler to add.
     * @param clientdata Client data associated with the callback handler.
     * 
     * @return If the handler was successfully added, then <b>true</b> will be
     * returned. Otherwise, <b>false</b> will be returned.
     */
    public boolean addEventCallback(int id, IMleEventCallback cb, Object clientdata)
    {
        return addEventCallback(id,cb,clientdata,MLE_DEFAULT_PRIORITY);
    }

    /**
     * Remove an event callback handler from the collection of callbacks that can be processed.
     * 
     * @param id The composite event identifier for the callback handler to be removed.
     * 
     * @return If the handler was successfully removed, then <b>true</b> will be
     * returned. Otherwise, <b>false</b> will be returned.
     */
    public boolean deleteEventCallback(int id)
    {
        boolean retValue = false;
        
        short groupId = MleEvent.getGroupId(id);
        short eventId = MleEvent.getEventId(id);

        Short groupKey = new Short(groupId);
        if (m_eventCallbacks.containsKey(groupKey))
        {
        	// Get the collection of callbacks for the specified group.
        	Hashtable cbs = (Hashtable)m_eventCallbacks.get(groupKey);

            // Check to see if the event callback exists in this group.
        	Short eventKey = new Short(eventId);
            if (cbs.containsKey(eventKey))
            {
            	// Remove the event callback.
            	cbs.remove(eventKey);
            	retValue = true;
            }
        }
        
        return retValue;
    }
    
    /**
     * Get the callback handler and its associated client data
     * for the specified composite event id.
     * 
     * @param id The composite event id.
     * 
     * @return An <code>EventCallbackObject</code> is returned for the specified id.
     * <b>null</b> may be returned if one is not found.
     */
    private EventCallbackObject getEventCallbackObject(int id)
    {
        EventCallbackObject eco = null;

        short groupId = MleEvent.getGroupId(id);
        short eventId = MleEvent.getEventId(id);

        Short groupKey = new Short(groupId);
        if (m_eventCallbacks.containsKey(groupKey))
        {
        	// Get the collection of callbacks for the specified group.
        	Hashtable cbs = (Hashtable)m_eventCallbacks.get(groupKey);

            // Check to see if the event callback exists in this group.
        	Short eventKey = new Short(eventId);
            if (cbs.containsKey(eventKey))
            {
            	// Return the event callback object.
            	eco = (EventCallbackObject)cbs.get(eventKey);
            }
        }
        
        return eco;
    }
    
    /**
     * Dispatch all events that have been placed in the delayed queue.
     */
    public void dispatchEvents()
    {
        /* Retreive the size of the queue. */
        //int size = m_eventQueue.size();
        int size = m_eventQueue.getNumElements();
        
        /* Dispatch all events currently on the queue */
        for (int i = 0; i < size; i++)
        {
            EventQueueElement element;
            if ((element = this.popEvent()) != null)
            {
            	MleEvent event = element.getEvent();
            	
            	EventCallbackObject eco = getEventCallbackObject(element.getEvent().getId());
                if ((eco != null) && (eco.m_isEnabled))
                {
                    IMleEventCallback cb = eco.m_cb;
                    cb.dispatch(event, eco.m_clientdata);
                }
            }
        }
    }

    /**
     * Push the event onto the queue for delayed dispatching.
     *
     * @param event The event to be pushed onto the queue.
     * @param callfata The call data to be associated with the event.
     *
     * @return If the event is successfully pushed onto the queue,
     * then <b>true</b> will be returned. Otherwise <b>false</b> will be
     * returned.
     */
    private boolean pushEvent(MleEvent event, Object calldata)
    {
        EventQueueElement queueElement =
            new EventQueueElement(event, calldata);
        m_eventQueue.insert(new MlePQElement(0,queueElement));
        return true;
    }
    
    /**
     * Pop the event from the queue.
     *
     * @return If the event is successfully popped from the queue,
     * then an <b>EventQueuElement</b> will be returned.
     * Otherwise <b>null</b> will be returned.
     */
    private EventQueueElement popEvent()
    {
    	EventQueueElement element = null;
        
    	// Check if the queue is empty.
        if (! m_eventQueue.isEmpty())
        {
             /* There is at least one element in the queue. */
            element = (EventQueueElement) (m_eventQueue.remove()).m_data;
        }
        
        return element;
    }

    /**
     * Process the event specified by the event id.
     * <p>
     * If the event type associated with the id is MLE_EVENT_IMMDIATE,
     * then the event will dispatched immediately. If the event type
     * is MLE_EVENT_DELAYED, then the event will be placed on a FIFO
     * queue to be processed at a later date (when dispatchEvents() is
     * called).
     * </p>
     *
     * @param id The composite event identifier.
     * @param calldata An Object containing the data to
     * be processed along with the event.
     *
     * @return If the event is successfully processed, then
     * <b>true</b> will be returned. Otherwise, <b>false</b> will be
     * returned.
     */
    public boolean processEvent(int id, Object calldata,short type)
    {
        boolean status = false;
        MleEvent event = new MleEvent(this, id, type, calldata);
        
        if (event != null)
        {
            if (type == IMleEventCallback.MLE_EVENT_IMMEDIATE)
            {
                /* Dispatch event immediately. */
            	EventCallbackObject eco = getEventCallbackObject(id);
            	if ((eco != null) && (eco.m_isEnabled))
            	{
            	    IMleEventCallback cb = eco.m_cb;
            		status = cb.dispatch(event, eco.m_clientdata);
            	}
            } else if (type == IMleEventCallback.MLE_EVENT_DELAYED)
            {
                /* Push event onto delayed queue. */
                status = this.pushEvent(event, calldata);
            }
        }
        
        return status;
    }
    
    /**
     * Enable the callback handler for the specified event.
     * 
     * @param id The composite event identifier.
     * 
     * @return If the event callback is successfully enabled, then
     * <b>true</b> will be returned. Otherwise, <b>false</b> will be
     * returned.
     */
    public boolean enableEventCallback(int id)
	{
    	boolean retValue = false;
    	
    	EventCallbackObject eco = getEventCallbackObject(id);
    	if (eco != null)
    	{
    		eco.m_isEnabled = true;
    		retValue = true;
    	}
    	
    	return retValue;
	}

    /**
     * Disable the callback handler for the specified event.
     * 
     * @param id The composite event identifier.
     * 
     * @return If the event is successfully disabled, then
     * <b>true</b> will be returned. Otherwise, <b>false</b> will be
     * returned.
     */
    public boolean disableEventCallback(int id)
	{
    	boolean retValue = false;
    	
    	EventCallbackObject eco = getEventCallbackObject(id);
    	if (eco != null)
    	{
    		eco.m_isEnabled = false;
    		retValue = true;
    	}
    	
    	return retValue;
	}
    
    /**
     * Change the dispatch priority for the specified event.
     * 
     * @param id The composite event identifier.
     * @param key The new priority.
     * 
     * @return If the event priority is successfully changed, then
     * <b>true</b> will be returned. Otherwise, <b>false</b> will be
     * returned.
     */
    public boolean changeEventCBPriority(int id,int key)
    {
        boolean result = false;
        
        EventCallbackObject cb = getEventCallbackObject(id);
        if (cb != null)
        {
            cb.m_priority = key;
            result = true;
        }
        
        return result;
    }

}

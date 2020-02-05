/*
 * MleEventDispatcher.java
 * Created on Dec 13, 2004
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
import java.util.Vector;

// Import Magic Lantern classes.
import com.wizzer.mle.runtime.core.IMleCallbackId;
import com.wizzer.mle.runtime.core.IMleCallback;
import com.wizzer.mle.runtime.core.MleRuntimeException;
import com.wizzer.mle.runtime.util.MlePQ;
import com.wizzer.mle.runtime.util.MlePQElement;

/**
 * <code>MleEventDispatcher</code> is used to synchronize the dispatching of
 * Magic Lantern runtime events.
 * 
 * @author Mark S. Millard
 */
public class MleEventDispatcher
{
    // Event callback node definition.
    private static class EventCBNode implements IMleCallbackId
    {
        /** The callback handler. */
        public IMleEventCallback m_callback = null;
        /** The associated client data. */
        public Object m_clientData = null;
        /** Flag indicating whether event is enabled. */
        public boolean m_isEnabled = false;

        /**
         * Determine if the callback associated with this identifier is enabled or not.
         * 
         * @return <b>true</b> is returned if the callback is enabled. Otherwise,
         * <b>false</b> will be returned.
         */
        public boolean isEnabled()
        {
            return m_isEnabled;
        }
        
        /**
         * Get the callback associated with this identifier.
         * 
         * @return A reference to an <code>IMleCallback</code> is returned.
         */
        public IMleCallback getCallback()
        {
            return m_callback;
        }
    } // end of EventCBNode
    
    // Group event node definition.
    private static class EventGroupNode
    {
        // The first event node in a group.
        EventNode m_head = null;
        // The last event node in a group.
        EventNode m_tail = null;

        // Link in the specified node.
        boolean linkEventNode(EventNode node)
        	throws MleRuntimeException
	    {
	        if (node == null)
	            throw new MleRuntimeException("MleEventDispatcher: Node is null.");
	
	        if (m_head == null)
	        {
	            m_head = node;
	            m_tail = node;
	        } else
	        {
	            m_tail.m_next = node;
	            m_tail = node;
	            node.m_next = null;
	        }
	
	        return true;
	    }
        
        // Unlink the specified node.
        boolean unlinkEventNode(EventNode node)
        	throws MleRuntimeException
        {
            EventNode prevNode;

            if (node == null)
                throw new MleRuntimeException("MleEventDispatcher: Node is null.");

            // Find previous node.
            prevNode = findPrevEventNode(m_head,node);
            if (prevNode != null)
                return false;

            // Unlink node from linked-list.
            if (m_head == node)
            {
                // Node is first one in list.
                m_head = node.m_next;
                if (m_tail == node)
                    // Node is also last one in list.
                    m_tail = null;
            } else
            {
                prevNode.m_next = node.m_next;
                if (m_tail == node)
                    // Node is last one in list.
                    m_tail = prevNode;
            }

            return false;
        }

        // Find the event node in a linked list of nodes.
        public EventNode findEventNode(int event)
        {
            EventNode node;
            boolean found = false;

            node = m_head;
            while (node != null)
            {
                if (node.m_event == event)
                {
                    found = true;
                    break;
                }
                node = node.m_next;
            }

            if (! found)
                node = null;
            return(node);
        }

        // Find the previous node to the one specified.
        private EventNode findPrevEventNode(
                EventNode firstNode,EventNode node)
        {
            EventNode prevNode,nextNode;
            boolean found = false;

            prevNode = nextNode = firstNode;
            while(nextNode != null)
            {
                if (nextNode == node)
                {
                    found = true;
                    break;
                } else
                {
                    prevNode = nextNode;
                    nextNode = nextNode.m_next;
                }
            }

            if (! found)
                return null;
            else
                return prevNode;
        }
	} // end of EventGroupNode

    // Event node definition.
    private static class EventNode
    {
        // The event composite value.
        int m_event = MleEvent.MLE_EVENT_INVALID_ID;
        // A priority queue of callbacks.
        MlePQ m_callbacks = null;
        // Flag indicating whether event is enabled.
        public boolean m_isEnabled = false;
        // Next event node in a linked list.
        EventNode m_next;
        // The owning group.
        EventGroupNode m_group = null;
    } // end of EventNode

    // A helper class to encapsulate a queue element.
    private class EventQueueElement
    {
    	/** The event. */
        public MleEvent m_event = null;
        
        // Hide the default EventQueueElement constructor.
        private EventQueueElement()
        {
        	// Should never be called!
        }
        
		/**
		 * Construct an queue element to hold a <code>MleEvent</code>.
		 *
		 * @param event A Magic Lantern event.
		 */
        public EventQueueElement(MleEvent event)
        {
            m_event = event;
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

    } // end of EventQueueElement
    
    // The event callback nodes organized by groups.
    private Hashtable m_eventGroups;
    // The queued events.
    private MlePQ m_eventQueue;
    // Registry of event listeners.
    private Vector m_eventListeners = new Vector();
 
    /**
     * The default constructor.
     * <p>
     * The default number of groups is set to <b>10</b>.
     * </p>
     */
    public MleEventDispatcher()
    {
        super();
        m_eventGroups = new Hashtable(10);
        m_eventQueue = new MlePQ(MlePQ.MLE_INC_QSIZE);
    }
    
    /**
     * A constructor that initializes the capacity of the number
     * of event groups.
     * 
     * @param capacity The number of event groups.
     */
    public MleEventDispatcher(int capacity)
    {
        super();
        m_eventGroups = new Hashtable(capacity);
        m_eventQueue = new MlePQ(MlePQ.MLE_INC_QSIZE);
    }

    /**
     * Install a callback for the specified event.
     * 
     * @param event The composite event identifier.
     * @param callback The callback to install.
     * @param clientData Client data associated with the dispatch
     * of the callback.
     * 
     * @return A callback identifier is returned. This may be used to
     * uniquely identify a specific callback for a particular event.
     * 
     * @throws MleRuntimeException This exception is thrown if the
     * callback can not be installed successfully.
     */
    public IMleCallbackId installEventCB(
         int event,
         IMleEventCallback callback,
         Object clientData)
         throws MleRuntimeException
    {
        EventNode node;

        // Check if event node already exists.
        if ((node = findEventNode(event)) == null)
        {
            // It doesn't, so create a new one.
            node = new EventNode();
            if (node != null)
            {
                // Initialize the node.
                node.m_event = event;
                node.m_callbacks = new MlePQ(MlePQ.MLE_INC_QSIZE);
                node.m_isEnabled = true;
                 
                addEventNode(node);
            } else {
                throw new MleRuntimeException("MleEventDispatcher: Unable to install event callback.");
            }
        }

        // Install callback.
        EventCBNode cbNode = new EventCBNode();
        if (cbNode != null)
        {
            cbNode.m_callback = callback;
            cbNode.m_clientData = clientData;
            cbNode.m_isEnabled = true;

            // Add callback node to priority queue.
            MlePQElement item = new MlePQElement(0,cbNode);
            node.m_callbacks.insert(item);
        } else
        {
            throw new MleRuntimeException("MleEventDispatcher: Unable to install event callback.");
        }

        return(cbNode);
    }

    /**
     * Uninstall all callbacks associated with the specified event.
     *
     * @param event The composite event identifier.
     * 
     * @return <b>true</b> is returned if the event is successfully uninstalled.
     * Otherwise, <b>false</b> will be returned indicating that the specified
     * event is invalid or could not be uninstalled.
     * 
     * @throws MleRuntimeException This exception is thrown if the callbacks
     * can not be removed from the priority queue.
     */
    public boolean uninstallEvent(int event)
        throws MleRuntimeException
    {
        EventNode node;

        // Check if event already exists.
        if ((node = findEventNode(event)) == null)
        {
            return false;
        } else
        {
            // Destroy callback nodes.
            if (node.m_callbacks != null)
            {
                int numCallbacks = node.m_callbacks.getNumElements();
                for (int i = 0; i < numCallbacks; i++)
                {
                    MlePQElement item = node.m_callbacks.remove();
                    item.m_data = null;
                }
                node.m_callbacks = null;
            }

            // Free node.
            removeEventNode(node);
        }

        return true;
    }

    /**
     * Enable the specified event for dispatching.
     *
     * @param event The composite event identifier.
     * 
     * @return <b>true</b> is returned if the event is succussfull enabled.
     * Otherwise, <b>false</b> will be returned.
     */
    public boolean enableEvent(int event)
    {
        EventNode node;

        // Check if event already exists.
        if ((node = findEventNode(event)) == null)
            return false;
        else
            node.m_isEnabled = true;

        return true;
    }

    /**
     * Disable the specified event for dispatching.
     *
     * @param event The composite event identifier.
     * 
     * @return <b>true</b> is returned if the event is succussfull disabled.
     * Otherwise, <b>false</b> will be returned.
     */
    public boolean disableEvent(int event)
    {
        EventNode node;

        // Check if event already exists.
        if ((node = findEventNode(event)) == null)
            return false;
        else
            node.m_isEnabled = false;

        return true;
    }

    /**
     * Uninstall the specified callback associated with the given
     * <b>event</b>.
     * 
     * @param event The composite event identifier.
     * @param id The identifier for the callback to uninstall.
     * 
     * @return <b>true</b> is returned if the callback is successfully uninstalled.
     * Otherwise, <b>false</b> will be returned.
     */
    public boolean uninstallEventCB(int event,IMleCallbackId id)
    {
        EventNode node;

        // Find event node.
        if ((node = findEventNode(event)) == null)
        {
            return false;
        } else
        {
            int index;

            // Find callback node.
            if ((index = findEventCBNode(node,(EventCBNode)id)) == -1)
            {
                return false ;
            } else
            {
                // Destroy priority queue item.
                node.m_callbacks.destroyItem(index);
            }
        }

        return true;
    }

    /**
     * Enable the specified callback associated with the given
     * <b>event</b>.
     * 
     * @param event The composite event identifier.
     * @param id The identifier for the callback to enable.
     * 
     * @return <b>true</b> is returned if the callback is successfully enabled.
     * Otherwise, <b>false</b> will be returned.
     */
    public boolean enableEventCB(int event,IMleCallbackId id)
    {
        EventNode node;
        EventCBNode cbNode;

        // Find event node.
        if ((node = findEventNode(event)) == null)
        {
            return false;
        } else
        {
            // Find callback node.
            if ((cbNode = (EventCBNode)id) == null)
            {
                return false;
            } else {
                cbNode.m_isEnabled = true;
            }
        }

        return true;
    }

    /**
     * Disable the specified callback associated with the given
     * <b>event</b>.
     * 
     * @param event The composite event identifier.
     * @param id The identifier for the callback to disable.
     * 
     * @return <b>true</b> is returned if the callback is successfully enabled.
     * Otherwise, <b>false</b> will be returned.
     */
    public boolean disableEventCB(int event,IMleCallbackId id)
    {
        EventNode node;
        EventCBNode cbNode;

        // Find event node.
        if ((node = findEventNode(event)) == null)
        {
            return false;
        } else {
            // Find callback node.
            if ((cbNode = (EventCBNode)id) == null)
            {
                return false;
            } else {
                cbNode.m_isEnabled = false;
            }
        }

        return true;
    }

    /**
     * Change the priority of the callback for the specified event.
     * 
     * @param event The composite event identifier.
     * @param id The callback identifier.
     * @param key The new priority.
     * 
     * @return If the callback priority is successfully changed, then
     * <b>true</b> will be returned. Otherwise, <b>false</b> will be
     * returned.
     */
    public boolean changeCBPriority(int event,IMleCallbackId id,int key)
    {
        EventNode node;
        boolean result;

        // Find event node.
        if ((node = findEventNode(event)) == null)
        {
            result = false;
        } else
        {
            int index; 

            // Find callback node.
            if ((index = findEventCBNode(node,(EventCBNode)id)) == -1)
            {
                result = false;
            } else
            {
                result = node.m_callbacks.changeItem(index,key);
            }
        }

        return result;
    }

    /**
     * Change the dispatch priority for the specified event.
     * 
     * @param event The composite event identifier.
     * @param key The new priority.
     * 
     * @return If the event priority is successfully changed, then
     * <b>true</b> will be returned. Otherwise, <b>false</b> will be
     * returned.
     */
    public boolean changeEventPriority(int event,int key)
    {
        EventNode node;
        boolean result = false;
        
        for (int i = 0; i < m_eventQueue.getNumElements(); i++)
        {
            MlePQElement element = (MlePQElement)m_eventQueue.getElementAt(i);
            if (event == ((MleEvent)(element.m_data)).m_id)
            {
	            result = m_eventQueue.changeItem(event,key);
	            break;
            }
        }

        return result;
    }
    
    /**
     * Dispatch all events that have been placed in the delayed queue.
     */
    public void dispatchEvents()
    {
        // Retrieve the size of the queue.
        int size = m_eventQueue.getNumElements();
        
        // Dispatch all events currently on the queue.
        for (int i = 0; i < size; i++)
        {
            EventQueueElement element;
            if ((element = this.popEvent()) != null)
            {
            	MleEvent event = element.getEvent();
            	
            	// Find the event node is our registry.
            	EventNode node = findEventNode(element.getEvent().getId());
                if ((node != null) && (node.m_isEnabled))
                {
                    // Execute each callback that has been installed for this event
                    // a priori.
                    
                    // Copy the queue into one we can process.
                    MlePQ processQ = new MlePQ(node.m_callbacks.copyQueue());
                    while (! processQ.isEmpty())
                    {
                        MlePQElement item = processQ.remove();
                        EventCBNode cbNode = (EventCBNode)item.m_data;
                        if ((cbNode != null) && (cbNode.m_isEnabled))
                        {
                            // Invoke callback.
                            cbNode.m_callback.dispatch(event,cbNode.m_clientData);
                        }
                    }
                    
                    
                    // Notify listeners.
                    for (int j = 0; j < m_eventListeners.size(); j++)
                        ((IMleEventListener)m_eventListeners.elementAt(j)).eventDispatched(event);
                }
            }
        }
    }

    /**
     * Process the event specified by the event id.
     * <p>
     * If the event type associated with the id is MLE_EVENT_IMMDIATE,
     * then the event will dispatched immediately. If the event type
     * is MLE_EVENT_DELAYED, then the event will be placed on a priority
     * queue to be processed at a later date (when dispatchEvents() is
     * called).
     * </p><p>
     * The event will be dispatched with the specified priority if the event
     * type is MLE_EVENT_DELAYED.
     * </p>
     *
     * @param id The composite event identifier.
     * @param calldata An Object containing the data to
     * be processed along with the event.
     * @param type The type of dispatching to use.
     * @param priority The event dispatch priority.
     *
     * @return If the event is successfully processed, then
     * <b>true</b> will be returned. Otherwise, <b>false</b> will be
     * returned.
     */
    public boolean processEvent(int id, Object calldata, short type, int priority)
    {
        boolean status = false;
        MleEvent event = new MleEvent(this, id, type, calldata);
        
        if (event != null)
        {
            if (type == IMleEventCallback.MLE_EVENT_IMMEDIATE)
            {
                // Dispatch event immediately.
            	EventNode node = findEventNode(id);
            	if ((node != null) && (node.m_isEnabled))
            	{
                    // Execute each callback that has been installed for this event
                    // a priori.

            	    // Copy the queue into one we can process.
                    MlePQ processQ = new MlePQ(node.m_callbacks.copyQueue());
                    while (! processQ.isEmpty())
            	    {
                        MlePQElement item = processQ.remove();
                        EventCBNode cbNode = (EventCBNode)item.m_data;
            	        if ((cbNode != null) && (cbNode.isEnabled()))
            	        {
            	            IMleEventCallback cb = cbNode.m_callback;
            	            status = cb.dispatch(event, cbNode.m_clientData);
            	        }
            	    }
                    
                    // Notify listeners.
                    for (int i = 0; i < m_eventListeners.size(); i++)
                        ((IMleEventListener)m_eventListeners.elementAt(i)).eventProcessed(event);
            	}
            } else if (type == IMleEventCallback.MLE_EVENT_DELAYED)
            {
                /* Push event onto delayed queue. */
                status = this.pushEvent(event, calldata, priority);
            }
        }
        
        return status;
    }

    /**
     * Process the event specified by the event id.
     * <p>
     * If the event type associated with the id is MLE_EVENT_IMMEDIATE,
     * then the event will dispatched immediately. If the event type
     * is MLE_EVENT_DELAYED, then the event will be placed on a priority
     * queue to be processed at a later date (when dispatchEvents() is
     * called).
     * </p><p>
     * The event will be dispatched with the default priority if the event
     * type is MLE_EVENT_DELAYED.
     * </p>
     *
     * @param id The composite event identifier.
     * @param calldata An Object containing the data to
     * be processed along with the event.
     * @param type The type of dispatching to use.
     *
     * @return If the event is successfully processed, then
     * <b>true</b> will be returned. Otherwise, <b>false</b> will be
     * returned.
     */
    public boolean processEvent(int id, Object calldata, short type)
    {
        return processEvent(id,calldata,type,0);
    }
    
    // Add a node. Note that this method does not check for duplicate entries.
    private void addEventNode(EventNode node)
        throws MleRuntimeException
    {
        short groupId = MleEvent.getGroupId(node.m_event);

        // Get the associated group.
        EventGroupNode group = (EventGroupNode)m_eventGroups.get(new Short(groupId));
        if (group == null)
        {
            // No group by this id, create a new one.
        	group = new EventGroupNode();
        	m_eventGroups.put(new Short(groupId),group);
         	
        	node.m_group = group;
        }
         
        group.linkEventNode(node);
    }
     
    // Remove the specified node.
    private void removeEventNode(EventNode node)
        throws MleRuntimeException
    {
        short groupId = MleEvent.getGroupId(node.m_event);

        // Get the associated group.
        EventGroupNode group = (EventGroupNode)m_eventGroups.get(new Short(groupId));
        if (group != null)
        {
            group.unlinkEventNode(node);
        }         
    }
     
    // Find the event node based on the composite event id.
    private EventNode findEventNode(int id)
    {
        EventNode node = null;
        short groupId = MleEvent.getGroupId(id);
         
        EventGroupNode group = (EventGroupNode)m_eventGroups.get(new Short(groupId));
        if (group != null)
            node = group.findEventNode(id);
        
        return node;
    }

    // Find the event callback node.
    private int findEventCBNode(EventNode node,EventCBNode id)
    {
        int result = -1;

        for (int i = 0; i < node.m_callbacks.getNumElements(); i++)
        {
            MlePQElement item = (MlePQElement)node.m_callbacks.peek(i);
            if (item.m_data == id)
            {
                result = i;
                break;
            }
        }

         return result;
    }
    
    /*
     * Push the event onto the queue for delayed dispatching.
     *
     * @param event The event to be pushed onto the queue.
     * @param calldata The call data to be associated with the event.
     * @param key The event priority.
     *
     * @return If the event is successfully pushed onto the queue,
     * then <b>true</b> will be returned. Otherwise <b>false</b> will be
     * returned.
     */
    private boolean pushEvent(MleEvent event, Object calldata, int priority)
    {
        EventQueueElement queueElement = new EventQueueElement(event);
        m_eventQueue.insert(new MlePQElement(priority,queueElement));
        return true;
    }
    
    /*
     * Pop the event from the queue.
     *
     * @return If the event is successfully popped from the queue,
     * then an <code>EventQueueElement</code> will be returned.
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
     * Flush the delayed dispatch queue. All pending events will be
     * ignored.
     */
    public void flush()
    {
        m_eventQueue.clear();
    }
    
    /**
     * Add an event listener.
     * 
     * @param listener The event listener to notify when events
     * are processed and/or dispatched.
     *
     */
    public void addListener(IMleEventListener listener)
    {
        if (listener == null) return;
        m_eventListeners.addElement(listener);
    }
    
    /**
     * Remove an event listener.
     * 
     * @param listener The event listener to remove.
     */
     public void removeListener(IMleEventListener listener)
    {
        if (listener == null) return;
        m_eventListeners.removeElement(listener);
    }

}

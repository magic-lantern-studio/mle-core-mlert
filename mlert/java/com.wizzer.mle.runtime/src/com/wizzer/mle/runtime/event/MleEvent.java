/**
 * MleEvent.java
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

// Import standard Java packages.
import java.util.EventObject;


/**
 * <code>MleEvent</code> encapsulates a Magic Lantern event.
 *
 * @author  Mark S. Millard
 * @version 1.0
 */
public class MleEvent extends EventObject
{
    /** The id is invalid. */
    public static final int MLE_EVENT_INVALID_ID = 0xffffffff;
    
    /** The event id. */
    protected int m_id = MLE_EVENT_INVALID_ID;
    
    /** The event dispatch type. */
    protected short m_type = IMleEventCallback.MLE_EVENT_DELAYED;
    
    /** Call data assoicated with the event. */
    protected Object m_calldata = null;


    /**
     * Make a composite identifier based on the specified group
     * and event ids.
     *
     * @param group The group that the event belongs to.
     * @param id The unique identifier for the event.
     *
     * @return The composite event identifier is returned.
     */
    public static int makeId(short group, short id)
    {
        int newId = ((group << 16) | id);
        return newId;
    }
    
    /**
     * Get the group id for the specified composite identifier.
     * 
     * @param cid The composite event identifier.
     * 
     * @return The group that the event belongs to is returned.
     */
    public static short getGroupId(int cid)
    {
    	short groupId = (short)(cid >> 16);
    	return groupId;
    }
    
    /**
     * Get the event id for the specified composite identifier.
     * 
     * @param cid The composite event identifier.
     * 
     * @return The event id is returned.
     */
    public static short getEventId(int cid)
    {
    	short eventId = (short)(cid & 0x00FF);
    	return eventId;
    }

    // Hide the required, inherited constructor.
    private MleEvent(Object source)
    {
        super(source);
    }

    /**
     * Create a MleEvent for the specified object with the specified
     * identifier. By default the event will be dispatched in delayed
     * mode.
     *
     * @param source The object on which the Event initially occurred.
     * @param id The event id.
     */
    public MleEvent(Object source, int id)
    {
        super(source);
        m_id = id;
    }

    /**
     * Create a MleEvent for the specified object with the specified
     * identifier. Set the event dispatching type upon construction.
     *
     * @param source The object on which the Event initially occurred.
     * @param id The event id.
     * @param type The event dispatching type. Valid types include:
     * <ul>
     *   <li>IMleEventCallback.MLE_EVENT_IMMEDIATE</li>
     *   <li>IMleEventCallback.MLE_EVENT_DELAYED</li>
     * </ul>
     */
    public MleEvent(Object source, int id, short type)
    {
        super(source);
        m_id = id;
        m_type = type;
    }

    /**
     * Create a MleEvent for the specified object with the specified
     * identifier. By default the event will be dispatched in delayed
     * mode.
     *
     * @param source The object on which the Event initially occurred.
     * @param id The event id.
     * @param calldata The caller data associated with this event.
     */
    public MleEvent(Object source, int id, Object calldata)
    {
        super(source);
        m_id = id;
        m_calldata = calldata;
    }
    
    /**
     * Create a MleEvent for the specified object with the specified
     * identifier. Set the event dispatching type upon construction.
     *
     * @param source The object on which the Event initially occurred.
     * @param id The event id.
     * @param type The event dispatching type. Valid types include:
     * <ul>
     *   <li>IMleEventCallback.MLE_EVENT_IMMEDIATE</li>
     *   <li>IMleEventCallback.MLE_EVENT_DELAYED</li>
     * </ul>
     * @param calldata The caller data associated with this event.
     */
    public MleEvent(Object source, int id, short type, Object calldata)
    {
        super(source);
        m_id = id;
        m_type = type;
        m_calldata = calldata;
    }
    
    /**
     * Get the event id.
     *
     * @return An integer value representing the event id is returned.
     */
    public int getId()
    {
        return m_id;
    }
    
    /**
     * Get the event dispatching type.
     *
     * @return A short is returned identifying the type of event dispatching
     * used for this event. Valid types include:
     * <ul>
     *   <li>IMleEventCallback.MLE_EVENT_IMMEDIATE</li>
     *   <li>IMleEventCallback.MLE_EVENT_DELAYED</li>
     * </ul>
     */
    public short getType()
    {
        return m_type;
    }

    /**
     * Set the event dispatching type.
     *
     * @param type A short identifying the type of event dispatching
     * to be used for this event. Valid types include:
     * <ul>
     *   <li>IMleEventCallback.MLE_EVENT_IMMEDIATE</li>
     *   <li>IMleEventCallback.MLE_EVENT_DELAYED</li>
     * </ul>
     */
    public void setType(short type)
    {
        m_type = type;
    }

    /**
     * Get the caller data associated with this event.
     *
     * @return An Object is returned encapsulating the client data.
     */
    public Object getCallData()
    {
        return m_calldata;
    }

    /**
     * Set the caller data for this event.
     *
     * @param calldata An Object containing the caller data.
     */
    public void setCallData(Object calldata)
    {
        m_calldata = calldata;
    }

    /**
     * Create a String representation of this MleEvent Object.
     *
     * @return A <code>String</code> representation is returned.
     */
    public String toString()
    {
		String type;
	
		if (m_type == IMleEventCallback.MLE_EVENT_IMMEDIATE)
		    type = new String("MLE_EVENT_IMMEDIATE");
		else if (m_type == IMleEventCallback.MLE_EVENT_DELAYED)
		    type = new String("MLE_EVENT_DELAYED");
		else
		    type = new String("UNKNOWN");
	
		String str = new String("MleEvent: id=" +
					m_id + " : type=" + type);
		return str;
    }

}

/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundatoin Library API */

/**
 * @file MleEvent.h
 * @ingroup MleFoundation
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2017-2022 Wizzer Works
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//  For information concerning this header file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

#ifndef __MLE_EVENT_H_
#define __MLE_EVENT_H_

// Include system header files.
#include <string>

// Include Magic Lantern header files.
#include "mle/mlTypes.h"
#include "mle/MleRuntime.h"


/**
 * Event value definition.
 */
//typedef signed long MleEvent;
class MLE_RUNTIME_API MleEvent {

  public:

    /** The id is invalid. */
	static const int MLE_EVENT_INVALID_ID = 0xffffffff;

	/** The event is to be dispatched immediately, without delay. */
	static const short MLE_EVENT_IMMEDIATE = 0x0001;

	/** The event is to be dispatched at a later date, it is queued. */
	static const short MLE_EVENT_DELAYED   = 0x0002;

	/**
	 * Make a composite identifier based on the specified group
	 * and event ids.
	 *
	 * @param group The group that the event belongs to.
	 * @param id The unique identifier for the event.
	 *
	 * @return The composite event identifier is returned.
	 */
	static int makeId(short group, short id);

	/**
	 * Get the group id for the specified composite identifier.
	 *
	 * @param cid The composite event identifier.
	 *
	 * @return The group that the event belongs to is returned.
	 */
	static short getGroupId(int cid);

	/**
	 * Get the event id for the specified composite identifier.
	 *
	 * @param cid The composite event identifier.
	 *
	 * @return The event id is returned.
	 */
	static short getEventId(int cid);

	/**
	 * Create a MleEvent with the specified identifier.
	 * By default the event will be dispatched in delayed
	 * mode.
	 *
	 * @param id The event id.
	 */
	MleEvent(int id);

	/**
	 * Create a MleEvent with the specified identifier.
	 * Set the event dispatching type upon construction.
	 *
	 * @param id The event id.
	 * @param type The event dispatching type. Valid types include:
	 * <ul>
	 *   <li>MleEvent::MLE_EVENT_IMMEDIATE</li>
	 *   <li>MleEvemt::MLE_EVENT_DELAYED</li>
	 * </ul>
	 */
	MleEvent(int id, short type);

	/**
     * Create a MleEvent eith the specified identifier.
	 * By default the event will be dispatched in delayed
	 * mode.
	 *
	 * @param id The event id.
	 * @param calldata The caller data associated with this event.
	 */
	MleEvent(int id, void *calldata);

    /**
     * Create a MleEvent with the specified identifier.
     * Set the event dispatching type upon construction.
     *
     * @param id The event id.
     * @param type The event dispatching type. Valid types include:
     * <ul>
     *   <li>MleEvent::MLE_EVENT_IMMEDIATE</li>
     *   <li>MleEvent::MLE_EVENT_DELAYED</li>
     * </ul>
     * @param calldata The caller data associated with this event.
     */
    MleEvent(int id, short type, void *calldata);

	/**
	 * The destructor.
	 */
	virtual ~MleEvent()
	{ /* Do nothing. */ }

	/**
     * Get the event id.
     *
     * @return An integer value representing the event id is returned.
     */
    int getId()
    {
        return m_id;
    }

	/**
	 * Get the event dispatching type.
	 *
	 * @return A short is returned identifying the type of event dispatching
	 * used for this event. Valid types include:
	 * <ul>
	 *   <li>MleEvent::MLE_EVENT_IMMEDIATE</li>
	 *   <li>MleEvent::MLE_EVENT_DELAYED</li>
	 * </ul>
	 */
	short getType()
	{
	    return m_type;
	}

	/**
     * Set the event dispatching type.
     *
     * @param type A short identifying the type of event dispatching
     * to be used for this event. Valid types include:
     * <ul>
     *   <li>MleEvent::MLE_EVENT_IMMEDIATE</li>
     *   <li>MleEvent::MLE_EVENT_DELAYED</li>
     * </ul>
     */
    void setType(short type)
    {
        m_type = type;
    }

    /**
     * Get the caller data associated with this event.
     *
     * @return An pointer to a void is returned encapsulating the client data.
     */
    void *getCallData()
    {
        return m_calldata;
    }

	/**
	 * Set the caller data for this event.
	 *
	 * @param calldata An Object containing the caller data.
	 */
	void setCallData(void *calldata)
	{
	    m_calldata = calldata;
	}

	/**
	 * Override the == operator.
	 *
	 * @param rhs The right-hand side of the operator comparison.
	 *
	 * @return <b>true</b> will be returned if the two events are equal.
	 * Otherwise, <b>false</b> will be returned.
	 */
	bool operator==(const MleEvent &rhs);

	/**
	 * Create a string representation of this MleEvent Object.
	 *
	 * @return A <code>std::string</code> representation is returned.
	 */
	std::string toString();

  protected:

	/** The event id. */
	int m_id;

	/** The event dispatch type. */
	short m_type;

	/** Call data associated with the event. */
	void *m_calldata;

  private:

	// Hide default constructor.
	MleEvent();

};

#endif /* __MLE_EVENT_H_ */

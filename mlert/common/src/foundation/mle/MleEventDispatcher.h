/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundatoin Library API */

/**
 * @file MleEventDispatcher.h
 * @ingroup MleFoundation
 *
 * @author Mark S. Millard
 * @created May 1, 2003
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015 Wizzer Works
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

#ifndef __MLE_EVENTDISPATCHER_H_
#define __MLE_EVENTDISPATCHER_H_

// include system header files
#ifdef WIN32
#include <windows.h>
#endif /* WIN32 */

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"
#include "mle/mlTypes.h"

#include "mle/MleRuntime.h"
#include "mle/MlePq.h"


/**
 * Event value definition.
 */
typedef signed long MleEvent;

/**
 * Event callback definition.
 */
typedef int (*MleCallback)(
    MleEvent event, void *callData, void *clientData);

/**
 * Event callback identifier.
 */
typedef void *MleCallbackId;

/**
 * Event callback node definition.
 */
typedef struct MLE_RUNTIME_API _MleEventCBNode
{
    unsigned long m_flags;            /**< Flags for ... */
    MleCallbackId m_id;               /**< An event callback identifier. */
    MleCallback m_callback;           /**< The event callback. */
    void *m_clientData;               /**< The event callback client data. */
} MleEventCBNode;

/**
 * Event node definition.
 */
typedef struct MLE_RUNTIME_API _MleEventNode
{
    unsigned long m_flags;            /**< Flags for mode, ... */
    MleEvent m_event;                 /**< Event value. */
    MlePQ *m_callbacks;               /**< Priority queue of callbacks. */
    struct _MleEventNode *m_next;     /**< Next event node. */
} MleEventNode;

/**
 * Event entry definition.
 */
typedef struct MLE_RUNTIME_API _MleEventEntry
{
    MleEvent m_event;                 /**< The Magic Lantern event. */
    MleCallback m_callback;           /**< The event callback function. */
    void *m_clientData;               /**< The event callback client data. */
    unsigned long m_flags;            /**< Flags for ... */
} MleEventEntry;


/**
 * Event dispatcher used to manage Magic Lantern events.
 */
class MLE_RUNTIME_API MleEventDispatcher
{
  // Declare member variables.

  private:

    unsigned long m_flags;            // Flags for event mgr state, ...
    MleEventNode *m_head;             // Head of registered events.
    MleEventNode *m_tail;             // Tail of registered events.
    
  // Declare member functions.

  public:

    /**
	 * The default constructor.
     */
    MleEventDispatcher(void);

    /**
	 * The destructor.
     */
    virtual ~MleEventDispatcher(void);
    
    /**
	 * Install the event callback for the specified event.
	 *
	 * @param event The Magic Lantern event.
	 * @param callback The event callback function.
	 * @param cleintData The event callback client data.
     */
    MleCallbackId installEventCB(
        MleEvent event,
        MleCallback callback,
        void *clientData);

    /**
	 * Install a table of event callbacks.
	 *
	 * @param eventTable A pointer to an array of event entries.
	 * @param numEvents The number of event entries in the table.
	 *
	 * @return <b>TRUE</b> will be returned if the events are successfully
	 * installed. Otherwise, <b>FALSE</b> will be returned.
     */
    MlBoolean installEventCB(MleEventEntry *eventTable,int numEvents);

    /**
	 * Uninstall all the event callbacks for the specified event.
	 *
	 * @param event The Magic Lantern event to uninstall.
	 *
	 * @return <b>TRUE</b> will be returned if the event is successfully
	 * uninstalled. Otherwise, <b>FALSE</b> will be returned.
     */
    MlBoolean uninstallEvent(MleEvent event);

    /**
	 * Enable all callbacks for the specified event.
	 *
	 * @param event The Magic Lantern event to enable.
	 *
	 * @return <b>TRUE</b> will be returned if the event is successfully
	 * enabled. Otherwise, <b>FALSE</b> will be returned.
     */
    MlBoolean enableEvent(MleEvent event);

    /**
	 * Disable all callbacks for the specified event.
	 *
	 * @param event The Magic Lantern event to disable.
	 *
	 * @return <b>TRUE</b> will be returned if the event is successfully
	 * disabled. Otherwise, <b>FALSE</b> will be returned.
     */
    MlBoolean disableEvent(MleEvent event);

    /**
	 * Uninstall the event callback for the specified event.
	 *
	 * @param event The Magic Lantern event to uninstall.
	 * @param id The callback identifier to uninstall.
	 *
	 * @return <b>TRUE</b> will be returned if the event callback is successfully
	 * uninstalled. Otherwise, <b>FALSE</b> will be returned.
     */
    MlBoolean uninstallEventCB(MleEvent event,MleCallbackId id);

    /**
	 * Enable the event callback for the specified event.
	 *
	 * @param event The Magic Lantern event to enable.
	 * @param id The callback identifier to enable.
	 *
	 * @return <b>TRUE</b> will be returned if the event callback is successfully
	 * enabled. Otherwise, <b>FALSE</b> will be returned.
     */
    MlBoolean enableEventCB(MleEvent event,MleCallbackId id);

    /**
	 * Disable the event callback for the specified event.
	 *
	 * @param event The Magic Lantern event to disable.
	 * @param id The callback identifier to disable.
	 *
	 * @return <b>TRUE</b> will be returned if the event callback is successfully
	 * disabled. Otherwise, <b>FALSE</b> will be returned.
     */
    MlBoolean disableEventCB(MleEvent event,MleCallbackId id);

    /**
	 * Change the event callback priority.
	 *
	 * @param event The Magic Lantern event.
	 * @param id The callback identifier to modify.
	 * @param key The new priority.
	 *
     * @return <b>TRUE</b> will be returned if the event callback priority is successfully
	 * modified. Otherwise, <b>FALSE</b> will be returned.
     */
    MlBoolean changeEventCBPriority(MleEvent event,MleCallbackId id,int key);

    /**
	 * Dispatch the specified event.
	 *
	 * @param event The Magic Lantern event to dispatch.
	 * @param callData The event data.
	 *
	 * @return The return value of the dispatched callback is returned.
     */
    int dispatchEvent(MleEvent event,void *callData);

	/**
	 * Override operator new.
	 *
	 * @param tSize The size, in bytes, to allocate.
	 */
	void* operator new(size_t tSize);

	/**
	 * Override operator delete.
	 *
	 * @param p A pointer to the memory to delete.
	 */
    void  operator delete(void *p);

  private:

	// Link the event node into the runtime structures.
    MlBoolean _linkEventNode(MleEventNode *node);
	// Unlink the event node from the runtime structures.
    MlBoolean _unlinkEventNode(MleEventNode *node);
	// Find the event node in the runtime structures.
    MleEventNode *_findEventNode(MleEvent event);

	// Find the index location of the event callback in the runtime structures.
    unsigned int _findEventCBNode(MleEventNode *node,MleCallbackId id);
};

#ifdef WIN32
// Define Win32 event callData.
typedef struct _Win32CallData {
    HWND hwnd;                      // window instance handle
    UINT uMsg;                      // message identifier
    WPARAM wParam;                  // message parameter
    LPARAM lParam;                  // message parameter
} Win32CallData;
#endif /* WIN32 */


// Define event dispatcher flags.
#define MLE_EVMGR_ALLOCMASK  0x00000001  /**< Dispatch Manager allocation mask. */
#define MLE_EVMGR_SYSALLOC   0x00000000  /**< Allocated by system. */
#define MLE_EVMGR_USRALLOC   0x00000001  /**< Allocated by user. */

#define MLE_EVMGR_MODEMASK   0x00000001  /**< Mask for dispatching modes. */
#define MLE_EVMGR_IMMEDIATE  0x00000000  /**< Dispatch events in immediate mode */
#define MLE_EVMGR_DELAYED    0x00000001  /**< Dispatch events in delayed mode */

#define MLE_EVMGR_ENABLEMASK 0x00000002  /**< Mask for enabling event callbacks. */
#define MLE_EVMGR_DISABLED   0x00000000  /**< Event callback is disabled. */
#define MLE_EVMGR_ENABLED    0x00000002  /**< Event callback is enabled. */


#ifdef WIN32
LRESULT CALLBACK MleWndProc(HWND hwnd,UINT uMessage,WPARAM wparam,LPARAM lparam);
#endif /* WIN32 */


#endif /* __MLE_EVENTDISPATCHER_H_ */

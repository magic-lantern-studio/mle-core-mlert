/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MleEventDispatcher.cxx
 * @ingroup MleFoundation
 *
 * @author Mark S. Millard
 * @date May 1, 2003
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
//  For information concerning this source file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

// Include system header files.
#include <stdlib.h>
#include <string.h>

#ifdef __sgi
#include <string.h>
#endif /* __sgi */
#ifdef WIN32
#include <memory.h>
#endif /* WIN32 */

// Include Magic Lantern header files.
#include "mle/mlAssert.h"
#include "mle/mlErrno.h"
#include "mle/MleEventDispatcher.h"


#define MLE_EVMGR_PQSIZE 10

#define MLE_ENABLE_EVENT(node) \
    (node->m_flags = (node->m_flags & ~MLE_EVMGR_ENABLEMASK) | MLE_EVMGR_ENABLED)
#define MLE_DISABLE_EVENT(node) \
    (node->m_flags = (node->m_flags & ~MLE_EVMGR_ENABLEMASK) | MLE_EVMGR_DISABLED)
#define MLE_EVENT_ENABLED(node) \
    ((node->m_flags & MLE_EVMGR_ENABLEMASK) == MLE_EVMGR_ENABLED)
#define MLE_EVENT_DISABLED(node) \
    ((node->m_flags & MLE_EVMGR_ENABLEMASK) == MLE_EVMGR_DISABLED)


MleEventDispatcher::MleEventDispatcher()
  :m_flags(0),
   m_head(NULL),
   m_tail(NULL)
{
    // Do nothing extra.
}


MleEventDispatcher::~MleEventDispatcher()
{
    // Declare local variables.
    MleEventNode *nextNode,*node;
    MlePQItem item;
    unsigned int numCallbacks;

    // Destroy event nodes.
    nextNode = m_head;
    while (nextNode) {
        node = nextNode;

        // Destroy callback nodes.
        if (node->m_callbacks) {
            numCallbacks = node->m_callbacks->getNumItems();
            for (unsigned int i = 0; i < numCallbacks; i++) {
                node->m_callbacks->remove(item);
                if (item.m_data)
                    mlFree(item.m_data);
            }
            delete node->m_callbacks;
        }
        
        nextNode = nextNode->m_next;
        mlFree(node);
    }
}


MleCallbackId MleEventDispatcher::installEventCB(
    MleEvent event,
    MleCallback callback,
    void *clientData)
{
    // Declare local variables.
    MleEventNode *node;
    MleEventCBNode *cbNode;
    MlePQItem item;

    // Check if event node already exists.
    if ((node = _findEventNode(event)) == NULL) {
        // it doesn't, so create a new one
        node = (MleEventNode *) mlMalloc(sizeof(MleEventNode));
        if (node != NULL) {
            node->m_event = event;
            node->m_flags = MLE_EVMGR_SYSALLOC |
                            MLE_EVMGR_IMMEDIATE |
                            MLE_EVMGR_ENABLED;
            node->m_callbacks = new MlePQ(MLE_EVMGR_PQSIZE);
			node->m_next = NULL;
            _linkEventNode(node);
        } else {
            // XXX -- set MLERRno here
            return(NULL);
        }
    }

    // Install callback.
    cbNode = (MleEventCBNode *) mlMalloc(sizeof(MleEventCBNode));
    if (cbNode != NULL) {
        cbNode->m_id = (MleCallbackId)cbNode;
        cbNode->m_callback = callback;
        cbNode->m_clientData = clientData;
        cbNode->m_flags = MLE_EVMGR_SYSALLOC | MLE_EVMGR_ENABLED;

        // Add callback node to priority queue.
        item.m_key = 0;
        item.m_data = (void *)cbNode;
        node->m_callbacks->insert(item);
    } else {
        // XXX -- set MLERRno here
        return NULL;
    }

    return(cbNode);
}


MlBoolean MleEventDispatcher::installEventCB(MleEventEntry *eventTable,int numEvents)
{
    // declare local variables
    MleEventNode *node;
    MleEventCBNode *cbNode;
    MlePQItem item;

    MLE_ASSERT(eventTable);

    for (int i = 0; i < numEvents; i++) {
        // Check if event node already exists.
        if ((node = _findEventNode(eventTable[i].m_event)) == NULL) {
            // It doesn't, so create a new one.
            node = (MleEventNode *) mlMalloc(sizeof(MleEventNode));
            if (node != NULL) {
                node->m_event = eventTable[i].m_event;
                node->m_flags = eventTable[i].m_flags;
                node->m_callbacks = new MlePQ(MLE_EVMGR_PQSIZE);
                node->m_next = NULL;
                _linkEventNode(node);
            } else {
                // XXX -- set MLERRno here
                return ML_FALSE;
            }
        }

        // Install callback.
        cbNode = (MleEventCBNode *) mlMalloc(sizeof(MleEventCBNode));
        if (cbNode != NULL) {
            cbNode->m_id = (MleCallbackId)cbNode;
            cbNode->m_callback = eventTable[i].m_callback;
            cbNode->m_clientData = eventTable[i].m_clientData;
            cbNode->m_flags = MLE_EVMGR_SYSALLOC | MLE_EVMGR_ENABLED;

            // add callback node to priority queue
            item.m_key = 0;
            item.m_data = (void *)cbNode;
            node->m_callbacks->insert(item);
        } else {
            // XXX -- set MLERRno here
            return ML_FALSE;
        }

    }

    return ML_TRUE;
}


MlBoolean MleEventDispatcher::uninstallEvent(MleEvent event)
{
    // Declare local variables.
    MleEventNode *node;
    MlePQItem item;
    unsigned int numCallbacks;

    // Check if event already exists.
    if ((node = _findEventNode(event)) == NULL) {
        return(FALSE);
    } else {
        // Destroy callback nodes.
        if (node->m_callbacks) {
            numCallbacks = node->m_callbacks->getNumItems();
            for (unsigned int i = 0; i < numCallbacks; i++) {
                node->m_callbacks->remove(item);
                if (item.m_data)
                    mlFree(item.m_data);
            }
            delete node->m_callbacks;
        }

        // Free node.
        _unlinkEventNode(node);
        mlFree(node);
    }

    return(TRUE);
}


MlBoolean MleEventDispatcher::enableEvent(MleEvent event)
{
    // Declare local variables.
    MleEventNode *node;

    // Check if event already exists.
    if ((node = _findEventNode(event)) == NULL)
        return(FALSE);
    else
        MLE_ENABLE_EVENT(node);

    return(TRUE);
}


MlBoolean MleEventDispatcher::disableEvent(MleEvent event)
{
    // Declare local variables.
    MleEventNode *node;

    // Check if event already exists.
    if ((node = _findEventNode(event)) == NULL)
        return(FALSE);
    else
        MLE_DISABLE_EVENT(node);

    return(TRUE);
}


MlBoolean MleEventDispatcher::uninstallEventCB(MleEvent event,MleCallbackId id)
{
    // Declare local variables.
    MleEventNode *node;
    unsigned int index;

    // Find event node.
    if ((node = _findEventNode(event)) == NULL) {
        return(FALSE);
    } else {
        // Find callback node.
        if ((index = _findEventCBNode(node,id)) == 0) {
            return(FALSE);
        } else {
            // Destroy priority queue item.
            node->m_callbacks->destroyItem(index);

            // Free node.
            mlFree((MleEventCBNode *)id);
        }
    }

    return(TRUE);
}


MlBoolean MleEventDispatcher::enableEventCB(MleEvent event,MleCallbackId id)
{
    // Declare local variables.
    MleEventNode *node;
    MleEventCBNode *cbNode;

    // Find event node.
    if ((node = _findEventNode(event)) == NULL) {
        return(FALSE);
    } else {
        // Find callback node.
        if ((cbNode = (MleEventCBNode *)id) == NULL) {
            return(FALSE);
        } else {
            MLE_ENABLE_EVENT(cbNode);
        }
    }

    return(TRUE);
}


MlBoolean MleEventDispatcher::disableEventCB(MleEvent event,MleCallbackId id)
{
    // Declare local variables.
    MleEventNode *node;
    MleEventCBNode *cbNode;

    // Find event node.
    if ((node = _findEventNode(event)) == NULL) {
        return(FALSE);
    } else {
        // find callback node
        if ((cbNode = (MleEventCBNode *)id) == NULL) {
            return(FALSE);
        } else {
            MLE_DISABLE_EVENT(cbNode);
        }
    }

    return(TRUE);
}


MlBoolean MleEventDispatcher::changeEventCBPriority(
    MleEvent event,MleCallbackId id,int key)
{
    // Declare local variables.
    MleEventNode *node;
    MlBoolean retValue;
    unsigned int index;

    // Find event node.
    if ((node = _findEventNode(event)) == NULL) {
        retValue = FALSE;
    } else {
        // Find callback node.
        if ((index = _findEventCBNode(node,id)) == 0) {
            retValue = FALSE;
        } else {
            retValue = node->m_callbacks->changeItem(index,key);
        }
    }

    return(retValue);
}


int MleEventDispatcher::dispatchEvent(MleEvent event,void *callData)
{
    // Declare local variables.
    MleEventNode *node;
    MleEventCBNode *cbNode;
    MlePQ processQ;
    MlePQItem item;
    unsigned int numCallbacks;
    int retValue = MLE_E_EVMGR_FAILEDDISPATCH;

    // Check if event already exists.
    if ((node = _findEventNode(event)) != NULL) {
        if (MLE_EVENT_ENABLED(node)) {

            // Copy the queue into one we can process.
            processQ = *(node->m_callbacks);
            numCallbacks = processQ.getNumItems();
            for (unsigned int i = 0; i < numCallbacks; i++) {

                // Remove the items a priori.
                processQ.remove(item);
                cbNode = (MleEventCBNode *)item.m_data;
                if (MLE_EVENT_ENABLED(cbNode)) {

                    // Envoke callback.
                    retValue = (cbNode->m_callback)(
                        event,callData,cbNode->m_clientData);
                    if (retValue != 0)
                        retValue = MLE_E_EVMGR_FAILEDCALLBACK;
                } else {
                    retValue = MLE_E_EVMGR_DISABLEDDISPATCH;
                }
            }
        } else {
            retValue = MLE_E_EVMGR_DISABLEDDISPATCH;
        }
    }
    
    return(retValue);
}


static MleEventNode *_findPrevEventNode(
    MleEventNode *firstNode,MleEventNode *node)
{
    // Declare local variables.
    MleEventNode *prevNode,*nextNode;
    MlBoolean found = FALSE;

    prevNode = nextNode = firstNode;
    while(nextNode) {
        if (nextNode == node) {
            found = TRUE;
            break;
        } else {
            prevNode = nextNode;
            nextNode = nextNode->m_next;
        }
    }

    if (! found)
        return NULL;
    else
        return prevNode;
}


MlBoolean MleEventDispatcher::_linkEventNode(MleEventNode *node)
{
    MLE_VALIDATE_PTR(node);

    if (m_head == NULL) {
        m_head = node;
        m_tail = node;
    } else {
        m_tail->m_next = node;
        m_tail = node;
        node->m_next = NULL;
    }

    return TRUE;
}


MlBoolean MleEventDispatcher::_unlinkEventNode(MleEventNode *node)
{
    // Declare local variables.
    MleEventNode *prevNode;

    MLE_VALIDATE_PTR(node);

    // Find previous node.
    prevNode = _findPrevEventNode(m_head,node);
    if (! prevNode)
        return FALSE;

    // Unlink node from linked-list.
    if (m_head == node) {
        // Node is first one in list.
        m_head = node->m_next;
        if (m_tail == node)
            // Node is also last one in list.
            m_tail = NULL;
    } else {
        prevNode->m_next = node->m_next;
        if (m_tail == node)
            // Node is last one in list.
            m_tail = prevNode;
    }

    return FALSE;
}


MleEventNode *MleEventDispatcher::_findEventNode(MleEvent event)
{
    // Declare local variables.
    MleEventNode *node;
    MlBoolean found = FALSE;

    node = m_head;
    while(node) {
        if (node->m_event == event) {
            found = TRUE;
            break;
        }
        node = node->m_next;
    }

    if (! found)
        node = NULL;
    return(node);
}


unsigned int MleEventDispatcher::_findEventCBNode(
    MleEventNode *node,MleCallbackId id)
{
    // Declare local variables.
    MlePQItem item;
    unsigned int retValue = 0;

    for (unsigned int i = 1; i <= node->m_callbacks->getNumItems(); i++) {
        node->m_callbacks->peek(i,item);
        if (item.m_data == id) {
            retValue = i;
            break;
        }
    }

    return(retValue);
}


void *
MleEventDispatcher::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}


void
MleEventDispatcher::operator delete(void *p)
{
	mlFree(p);
}


#ifdef UNITTEST

//
// Unit test for Event Dispatch Manager.
//
#include <stdio.h>
#include "mle/malloc.h"


enum {
    EVENT_ONE = 1,
    EVENT_TWO = 2,
    EVENT_THREE = 3,
    EVENT_FOUR = 4,
    EVENT_FIVE = 5,
    EVENT_SIX = 6,
    EVENT_SEVEN = 7,
    EVENT_EIGHT = 8,
    EVENT_NINE = 9,
    EVENT_TEN = 10,
    NUM_EVENTS = 10
};


MleEventDispatcher *evMgr;


int eventHndlr(MleEvent event,void *callData,void *clientData)
{
    // declare local variables
    char *clientStr = (char *)clientData;
    char *callStr = (char *)callData;

    fprintf(stdout,"Received event %d: processing data %s, %s.\n",event,
        clientStr,callStr);

    return(0);
}


int mleRTMainLoop()
{
    /* Declare local variable. */
    int i;
    static char msg[] = "Hello World!";

    for (i = 0; i <= NUM_EVENTS; i++) {
        evMgr->dispatchEvent(i,(void *)msg);
    }

    return(0);
}


main(int argc,char **argv)
{
    // declare local variables
    MleCallbackId cbId[14];

    // init event manager
    evMgr = new MleEventDispatcher();
    MLE_ASSERT(evMgr);

    // install event callbacks
    cbId[0] = evMgr->installEventCB(EVENT_ONE,eventHndlr,"one");
    cbId[1] = evMgr->installEventCB(EVENT_TWO,eventHndlr,"two");
    cbId[2] = evMgr->installEventCB(EVENT_THREE,eventHndlr,"three");
    cbId[3] = evMgr->installEventCB(EVENT_FOUR,eventHndlr,"four");
    cbId[4] = evMgr->installEventCB(EVENT_FIVE,eventHndlr,"five");
    cbId[5] = evMgr->installEventCB(EVENT_SIX,eventHndlr,"six");
    cbId[6] = evMgr->installEventCB(EVENT_SEVEN,eventHndlr,"seven");
    cbId[7] = evMgr->installEventCB(EVENT_EIGHT,eventHndlr,"eight");
    cbId[8] = evMgr->installEventCB(EVENT_NINE,eventHndlr,"nine");
    cbId[9] = evMgr->installEventCB(EVENT_TEN,eventHndlr,"ten");

    cbId[10] = evMgr->installEventCB(EVENT_ONE,eventHndlr,"one A");
    cbId[11] = evMgr->installEventCB(EVENT_ONE,eventHndlr,"one B");
    cbId[12] = evMgr->installEventCB(EVENT_THREE,eventHndlr,"three A");
    cbId[13] = evMgr->installEventCB(EVENT_THREE,eventHndlr,"three B");

    // disable some events
    evMgr->disableEvent(EVENT_SIX);
    evMgr->disableEvent(EVENT_SEVEN);
    evMgr->disableEvent(EVENT_EIGHT);
    evMgr->disableEvent(EVENT_NINE);

    // execution cycle
    fprintf(stdout,"Executing Test 1.\n");
    mleRTMainLoop();

    // uninstall some events
    evMgr->uninstallEvent(EVENT_SIX);
    evMgr->uninstallEvent(EVENT_SEVEN);
    evMgr->uninstallEvent(EVENT_EIGHT);
    evMgr->uninstallEvent(EVENT_NINE);

    // execution cycle
    fprintf(stdout,"Executing Test 2.\n");
    mleRTMainLoop();

    // uninstall/disable some specific event callbacks
    evMgr->uninstallEventCB(EVENT_THREE,cbId[12]);
    evMgr->uninstallEventCB(EVENT_THREE,cbId[13]);
    evMgr->disableEventCB(EVENT_ONE,cbId[10]);
    evMgr->disableEventCB(EVENT_ONE,cbId[11]);

    // execution cycle
    fprintf(stdout,"Executing Test 3.\n");
    mleRTMainLoop();

    // reset the priority of an event callback
    evMgr->changeEventCBPriority(EVENT_ONE,cbId[0],2);
    evMgr->changeEventCBPriority(EVENT_ONE,cbId[10],1);
    evMgr->changeEventCBPriority(EVENT_ONE,cbId[11],0);
    evMgr->enableEventCB(EVENT_ONE,cbId[10]);
    evMgr->enableEventCB(EVENT_ONE,cbId[11]);

    // execution cycle
    fprintf(stdout,"Executing Test 4.\n");
    mleRTMainLoop();

    return(0);
}

#endif /* TEST */

// COPYRTIGH_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2022 Wizzer Works
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

// Include system header files.
#include <iostream>

// Include Google Test header files.
#include "gtest/gtest.h"

// Include Magic Lantern header files.
#include "mle/MleEventDispatcher.h"

using namespace std;

// Define some events.
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

// Define an event handler.
int eventHndlr(MleEvent event,void *callData,void *clientData)
{
    // Declare local variables.
    char *clientStr = (char *)clientData;
    char *callStr = (char *)callData;

    fprintf(stdout,"Received event %d: processing data %s, %s.\n", event.getId(),
        clientStr, callStr);

    return 1;
}

// Define an event loop that generates events.
int mleRTMainLoop(MleEventDispatcher *evMgr)
{
    /* Declare local variable. */
    int i;
    static char msg[] = "Hello World!";

    for (i = 0; i <= NUM_EVENTS; i++) {
        int status = evMgr->dispatchEvent(i,(void *)msg);
        EXPECT_TRUE(status);
    }

    return(0);
}

TEST(MleEventDispatcherTest, DefaultConstructor) {
    // This test is named "DefaultConstructor", and belongs to the "MleEventdispatcherTest"
    // test case.

	MleEventDispatcher *evMgr = new MleEventDispatcher();
    EXPECT_TRUE(evMgr != NULL);
}

TEST(MleEventDispatcherTest, Destructor) {
    // This test is named "Destructor", and belongs to the "MleEventDispatcherTest"
    // test case.

	MleEventDispatcher *evMgr = new MleEventDispatcher();
    EXPECT_TRUE(evMgr != NULL);

    delete evMgr;
}

TEST(MleEventDispatcherTest, DispatchEmpty) {
    // This test is named "DispatchEmpty", and belongs to the "MleEventDispatcherTest"
    // test case.

	MleEventDispatcher *evMgr = new MleEventDispatcher();
    EXPECT_TRUE(evMgr != NULL);

    // Note that no event handlers have been registered.
    mleRTMainLoop(evMgr);

    delete evMgr;
}

TEST(MleEventDispatcherTest, DispatchEvents) {
    // This test is named "DispatchEvents", and belongs to the "MleEventDispatcherTest"
    // test case.

	MleEventDispatcher *evMgr = new MleEventDispatcher();
    EXPECT_TRUE(evMgr != NULL);

    // Install event callbacks.
    MleCallbackId cbId[14];
    cbId[0] = evMgr->installEventCB(EVENT_ONE, eventHndlr, (void *)"one");
    cbId[1] = evMgr->installEventCB(EVENT_TWO, eventHndlr, (void *)"two");
    cbId[2] = evMgr->installEventCB(EVENT_THREE, eventHndlr, (void *)"three");
    cbId[3] = evMgr->installEventCB(EVENT_FOUR, eventHndlr, (void *)"four");
    cbId[4] = evMgr->installEventCB(EVENT_FIVE, eventHndlr, (void *)"five");
    cbId[5] = evMgr->installEventCB(EVENT_SIX, eventHndlr, (void *)"six");
    cbId[6] = evMgr->installEventCB(EVENT_SEVEN, eventHndlr, (void *)"seven");
    cbId[7] = evMgr->installEventCB(EVENT_EIGHT, eventHndlr, (void *)"eight");
    cbId[8] = evMgr->installEventCB(EVENT_NINE, eventHndlr, (void *)"nine");
    cbId[9] = evMgr->installEventCB(EVENT_TEN, eventHndlr, (void *)"ten");

    cbId[10] = evMgr->installEventCB(EVENT_ONE, eventHndlr, (void *)"one A");
    cbId[11] = evMgr->installEventCB(EVENT_ONE, eventHndlr, (void *)"one B");
    cbId[12] = evMgr->installEventCB(EVENT_THREE, eventHndlr, (void *)"three A");
    cbId[13] = evMgr->installEventCB(EVENT_THREE, eventHndlr, (void *)"three B");

    // Dispatch events.
    mleRTMainLoop(evMgr);

    delete evMgr;
}

TEST(MleEventDispatcherTest, DisableEvents) {
    // This test is named "DisableEvents", and belongs to the "MleEventDispatcherTest"
    // test case.

	MleEventDispatcher *evMgr = new MleEventDispatcher();
    EXPECT_TRUE(evMgr != NULL);

    // Install event callbacks.
    MleCallbackId cbId[14];
    cbId[0] = evMgr->installEventCB(EVENT_ONE, eventHndlr, (void *)"one");
    cbId[1] = evMgr->installEventCB(EVENT_TWO, eventHndlr, (void *)"two");
    cbId[2] = evMgr->installEventCB(EVENT_THREE, eventHndlr, (void *)"three");
    cbId[3] = evMgr->installEventCB(EVENT_FOUR, eventHndlr, (void *)"four");
    cbId[4] = evMgr->installEventCB(EVENT_FIVE, eventHndlr, (void *)"five");
    cbId[5] = evMgr->installEventCB(EVENT_SIX, eventHndlr, (void *)"six");
    cbId[6] = evMgr->installEventCB(EVENT_SEVEN, eventHndlr, (void *)"seven");
    cbId[7] = evMgr->installEventCB(EVENT_EIGHT, eventHndlr, (void *)"eight");
    cbId[8] = evMgr->installEventCB(EVENT_NINE, eventHndlr, (void *)"nine");
    cbId[9] = evMgr->installEventCB(EVENT_TEN, eventHndlr, (void *)"ten");

    cbId[10] = evMgr->installEventCB(EVENT_ONE, eventHndlr, (void *)"one A");
    cbId[11] = evMgr->installEventCB(EVENT_ONE, eventHndlr, (void *)"one B");
    cbId[12] = evMgr->installEventCB(EVENT_THREE, eventHndlr, (void *)"three A");
    cbId[13] = evMgr->installEventCB(EVENT_THREE, eventHndlr, (void *)"three B");

    // Disable some events.
    evMgr->disableEvent(EVENT_SIX);
    evMgr->disableEvent(EVENT_SEVEN);
    evMgr->disableEvent(EVENT_EIGHT);
    evMgr->disableEvent(EVENT_NINE);

    // Dispatch events.
    mleRTMainLoop(evMgr);

    delete evMgr;
}

TEST(MleEventDispatcherTest, UninstallEvents) {
    // This test is named "UninstallEvents", and belongs to the "MleEventDispatcherTest"
    // test case.

	MleEventDispatcher *evMgr = new MleEventDispatcher();
    EXPECT_TRUE(evMgr != NULL);

    // Install event callbacks.
    MleCallbackId cbId[14];
    cbId[0] = evMgr->installEventCB(EVENT_ONE, eventHndlr, (void *)"one");
    cbId[1] = evMgr->installEventCB(EVENT_TWO, eventHndlr, (void *)"two");
    cbId[2] = evMgr->installEventCB(EVENT_THREE, eventHndlr, (void *)"three");
    cbId[3] = evMgr->installEventCB(EVENT_FOUR, eventHndlr, (void *)"four");
    cbId[4] = evMgr->installEventCB(EVENT_FIVE, eventHndlr, (void *)"five");
    cbId[5] = evMgr->installEventCB(EVENT_SIX, eventHndlr, (void *)"six");
    cbId[6] = evMgr->installEventCB(EVENT_SEVEN, eventHndlr, (void *)"seven");
    cbId[7] = evMgr->installEventCB(EVENT_EIGHT, eventHndlr, (void *)"eight");
    cbId[8] = evMgr->installEventCB(EVENT_NINE, eventHndlr, (void *)"nine");
    cbId[9] = evMgr->installEventCB(EVENT_TEN, eventHndlr, (void *)"ten");

    cbId[10] = evMgr->installEventCB(EVENT_ONE, eventHndlr, (void *)"one A");
    cbId[11] = evMgr->installEventCB(EVENT_ONE, eventHndlr, (void *)"one B");
    cbId[12] = evMgr->installEventCB(EVENT_THREE, eventHndlr, (void *)"three A");
    cbId[13] = evMgr->installEventCB(EVENT_THREE, eventHndlr, (void *)"three B");

    // Uninstall all callback handlers for the specified event.
    evMgr->uninstallEvent(EVENT_SIX);
    evMgr->uninstallEvent(EVENT_SEVEN);
    evMgr->uninstallEvent(EVENT_EIGHT);
    evMgr->uninstallEvent(EVENT_NINE);

    // Dispatch events.
    mleRTMainLoop(evMgr);

    delete evMgr;
}

TEST(MleEventDispatcherTest, UninstallEventCBs) {
    // This test is named "UninstallEventCBs", and belongs to the "MleEventDispatcherTest"
    // test case.

	MleEventDispatcher *evMgr = new MleEventDispatcher();
    EXPECT_TRUE(evMgr != NULL);

    // Install event callbacks.
    MleCallbackId cbId[14];
    cbId[0] = evMgr->installEventCB(EVENT_ONE, eventHndlr, (void *)"one");
    cbId[1] = evMgr->installEventCB(EVENT_TWO, eventHndlr, (void *)"two");
    cbId[2] = evMgr->installEventCB(EVENT_THREE, eventHndlr, (void *)"three");
    cbId[3] = evMgr->installEventCB(EVENT_FOUR, eventHndlr, (void *)"four");
    cbId[4] = evMgr->installEventCB(EVENT_FIVE, eventHndlr, (void *)"five");
    cbId[5] = evMgr->installEventCB(EVENT_SIX, eventHndlr, (void *)"six");
    cbId[6] = evMgr->installEventCB(EVENT_SEVEN, eventHndlr, (void *)"seven");
    cbId[7] = evMgr->installEventCB(EVENT_EIGHT, eventHndlr, (void *)"eight");
    cbId[8] = evMgr->installEventCB(EVENT_NINE, eventHndlr, (void *)"nine");
    cbId[9] = evMgr->installEventCB(EVENT_TEN, eventHndlr, (void *)"ten");

    cbId[10] = evMgr->installEventCB(EVENT_ONE, eventHndlr, (void *)"one A");
    cbId[11] = evMgr->installEventCB(EVENT_ONE, eventHndlr, (void *)"one B");
    cbId[12] = evMgr->installEventCB(EVENT_THREE, eventHndlr, (void *)"three A");
    cbId[13] = evMgr->installEventCB(EVENT_THREE, eventHndlr, (void *)"three B");

    // uninstall/disable some specific event callbacks
    evMgr->uninstallEventCB(EVENT_THREE, cbId[12]);
    evMgr->uninstallEventCB(EVENT_THREE, cbId[13]);
    evMgr->disableEventCB(EVENT_ONE, cbId[10]);
    evMgr->disableEventCB(EVENT_ONE, cbId[11]);

    // Dispatch events.
    mleRTMainLoop(evMgr);

    delete evMgr;
}

TEST(MleEventDispatcherTest, ModifyPriority) {
    // This test is named "ModifyPriority", and belongs to the "MleEventDispatcherTest"
    // test case.

	MleEventDispatcher *evMgr = new MleEventDispatcher();
    EXPECT_TRUE(evMgr != NULL);

    // Install event callbacks.
    MleCallbackId cbId[14];
    cbId[0] = evMgr->installEventCB(EVENT_ONE, eventHndlr, (void *)"one");
    cbId[1] = evMgr->installEventCB(EVENT_TWO, eventHndlr, (void *)"two");
    cbId[2] = evMgr->installEventCB(EVENT_THREE, eventHndlr, (void *)"three");
    cbId[3] = evMgr->installEventCB(EVENT_FOUR, eventHndlr, (void *)"four");
    cbId[4] = evMgr->installEventCB(EVENT_FIVE, eventHndlr, (void *)"five");
    cbId[5] = evMgr->installEventCB(EVENT_SIX, eventHndlr, (void *)"six");
    cbId[6] = evMgr->installEventCB(EVENT_SEVEN, eventHndlr, (void *)"seven");
    cbId[7] = evMgr->installEventCB(EVENT_EIGHT, eventHndlr, (void *)"eight");
    cbId[8] = evMgr->installEventCB(EVENT_NINE, eventHndlr, (void *)"nine");
    cbId[9] = evMgr->installEventCB(EVENT_TEN, eventHndlr, (void *)"ten");

    cbId[10] = evMgr->installEventCB(EVENT_ONE, eventHndlr, (void *)"one A");
    cbId[11] = evMgr->installEventCB(EVENT_ONE, eventHndlr, (void *)"one B");
    cbId[12] = evMgr->installEventCB(EVENT_THREE, eventHndlr, (void *)"three A");
    cbId[13] = evMgr->installEventCB(EVENT_THREE, eventHndlr, (void *)"three B");

    // Reset the priority of an event callback.
    evMgr->changeEventCBPriority(EVENT_ONE, cbId[0], 2);
    evMgr->changeEventCBPriority(EVENT_ONE, cbId[10], 1);
    evMgr->changeEventCBPriority(EVENT_ONE, cbId[11], 0);

    // Dispatch events.
    mleRTMainLoop(evMgr);

    delete evMgr;
}

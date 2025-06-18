/** @defgroup MleInput Magic Lantern Runtime Engine Input Library API */

/**
 * @file MleMouseEvent.h
 * @ingroup MleInput
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015-2025 Wizzer Works
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

#ifndef __MLE_MOUSEEVENT_H_
#define __MLE_MOUSEEVENT_H_

// Include Magic Lantern header files.
#include "mle/mlTypes.h"

/* X11 includes */
#if defined(__linux__)
#ifdef MLE_XT
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#endif // MLE_XT
#endif /* __linux__ */

/* Win32 includes */
#ifdef _WINDOWS
#include <windows.h>
#endif /* _WINDOWS */


// Valid keys supported for the platform.
// Only MLE_KEY_CONTROL, MLE_KEY_ALT, MLE_KEY_SHIFT,
// and all other modifier keys are of interest
// for mouse event handling.
//
//  MleKeyMap.h is of the following form:
//    typedef enum MleKey {

//     // Modifiers
//     MLE_KEY_CONTROL = 1000,
//     MLE_KEY_ALT = 1001,

//     // Characters
//    #ifdef _WINDOWS
//      MLE_KEY_A = VK_A,
//      MLE_KEY_B = VK_B,
//      ...
//    #endif // _WINDOWS
//    #ifdef __sgi
//      MLE_KEY_A = XK_A,
//      MLE_KEY_B = XK_A,
//      ...
//    #endif // __sgi
//    };
#include "mle/MleKeyMap.h"


// Mouse button states used by getButtonStatus().
enum MleMouseButtonState
{
  MLE_MOUSE_NOP = 0,
  MLE_BUTTON_DOWN,
  MLE_BUTTON_UP,
};


// Key states
//   The key state is used to test an individual
//   key's state. This is used to find out the
//   state of modifier keys during mouse management.
enum MleKeyState
{
  MLE_KEY_NOP,
  MLE_KEY_DOWN,
  MLE_KEY_UP,
};

// MleMouseEvent - Platform-independent mouse events
//                 supported by MleMouseEvent.
//  xxx this enum will be inside the class definition
//      and will be parametrized by platform event type.
enum MleMouseEventType
{
  MLE_EVENT_MOUSE_ALL,    // used to request ALL mouse events

  MLE_EVENT_MOUSE_MOVED,  // mouse moved

  MLE_EVENT_BUTTON1_DOWN, // button 1 went down
  MLE_EVENT_BUTTON2_DOWN, // button 2 went down
  MLE_EVENT_BUTTON3_DOWN, // button 3 went down

  MLE_EVENT_BUTTON1_UP,   // button 1 went up
  MLE_EVENT_BUTTON2_UP,   // button 2 went up
  MLE_EVENT_BUTTON3_UP,   // button 3 went up
};

// MleMouseEventCB - This is the form of the mouse event callback
//                  functions that can be registered with
//                  MleMouseEvent. 
//                  The member functions getPosition(), getDelta(),
//                  and getButtonStatus() can be used inside the
//                  callbackFn to get platform-independent results.
//
//                  PARAMETERS
//
//                  mouseEvent - The event type passed in eventData.
//                  eventData - A platform-specific event data structure.
//                              This can be parsed in a platform-independent
//                              manner by calling the getButtonStatus(), 
//                              getPosition() or getDelta() member functions.
//                  clientData - The arbitrary client data passed
//                               through the registerCB member function.
typedef int (*MleMouseEventCB)
     (MleMouseEventType mouseEvent,  // one of MleMouseEvent
      void *eventData,               // platform-specific event data
      void *clientData);             // arbitrary client data.


/* MleMouseEvent Class
 *
 *   This class allows the programmer to handle mouse status
 *   using event handlers (i.e., callbacks). 
 *
 */

class MleMouseEvent
{

/************ MleMouseEvent member functions  ***********************/
public:
  // constructor - Pass it the scheduler phase in which mouse device
  //               events should be dispatched.
  //               If phase < 0, then the dispatch() function will
  //               not be scheduled at all. The mouse cursor is visible
  //               and uses the platform's default cursor pixmap.
  MleMouseEvent(int  phase);

  // destructor - Releases mouse resources and ensures that the default
  //              platform mouse cursor pointer is restored.
  ~MleMouseEvent(void);

  // getNumberOfButtons - Returns the number of buttons on the mouse.
  //                      If it returns 0, then there's no mouse physically
  //                      attached to the system.
  int getNumberOfButtons(void);

  // registerCB - Registers a callback to handle a mouse event.
  //              The member functions getPosition(), getDelta,
  //              and getButtonStatus can be used inside the
  //              callbackFn to get platform-independent results.
  //              See typedef for MleMouseEventCB for more information.
  MlBoolean registerCB(int mouseEvent,                 // one of the fwMouseEvents
               (MleMouseEventCB *) callbackFn,  // see MleMouseEventCB typedef above
               (void *) clientData);           // an arbitrary data structure to
                                                       // pass to the callbackFn

  // getButtonStatus - Returns the state of the mouse buttons.
  //                   This supports a maximum of three buttons.
  //                   This is a convenience function which allows one to
  //                   retrieve mouse button information when a non-button
  //                   event was received and the platform supports this
  //                   functionality without any overhead. (Note that there's
  //                   a more direct way of doing this by, e.g., checking
  //                   whether the event was a button event like MLE_EVENT_BUTTON1_DOWN.)
  //                   eventData is a platform-specific event data structure
  //                   passed to the MleMouseEventCB.
  //                   buttonN = MLE_MOUSE_NOP means that no information was received for this button.
  //                   buttonN = MLE_BUTTON_DOWN  means that the button just went down.
  //                   buttonN = MLE_BUTTON_UP means that the button just went up.
  //                   Returns FALSE if the event was not a button event.
  MlBoolean getButtonStatus(void *eventData, 
                MleMouseButtonState *button1,
                MleMouseButtonState *button2,
                MleMouseButtonState *button3);

  // keyDown - Returns TRUE when the given modifier key is now down.
  //           Mlekey includes modifier keys supported by the
  //           current platform. If a non-modifier key is passed, then
  //           keyDown's return value is meaningless.
  MlBoolean keyDown(void *eventData, MleKey key);

  // getPosition - Returns the current mouse position in
  //               absolute x,y coordinates. eventData is the
  //               platform-specific event data structure
  //               passed to the MleMouseEventCB.
  //               Returns FALSE if this event does not report
  //               a changed mouse position.
  MlBoolean getPosition(void *eventData, int *x, int *y);

  // getDelta - Returns the relative displacement of the
  //            mouse since the last time getPosition
  //            or getDelta was called. eventData is the
  //            platform-specific event data structure
  //            passed to the MleMouseEventCB.
  //            Returns FALSE if this event does not report
  //            a changed mouse position.
  MlBoolean getDelta(void *eventData, int *deltaX, int *deltaY);

  // setSensitivity - Sets the mouse sensitivity. This is the
  //                  ratio of actual to received pointer motion.
  //                  Returns the actually set sensitivity. The actual
  //                  sensitivity might be less than requestedSensitivity
  //                  when the platform does not support it.
  int setSensitivity(int requestedSensitivity);

  // setCursor - Sets the mouse cursor to the supplied one.
  //             Cursors are stored as MleCursorMediaRef objects.
  //             The initial cursor is the default cursor used
  //             by the platform.
  //             Since a title can use multiple cursors, it's
  //             the title's responsibility to load and register
  //             the necessary cursors.
  MlBoolean setCursor(MediaRef *cursor);

  // setVisibility - Sets the visibility of the mouse cursor.
  //                 If show = TRUE, then the cursor will be shown;
  //                 else it will be hidden. It's the title's
  //                 responsibility to remember whether the
  //                 mouse is hidden or not. Initially, the
  //                 mouse is visible.
  void setVisibility(MlBoolean show);

private:
  // dispatch - Dispatches mouse events from the platform event queue.
  //            This function will be called during the
  //            scheduling phase specified as a constructor argument.
  //            Programmers not using the FireWalker scheduler to
  //            schedule dispatching will want to directly use dispatch()
  //            at appropriate points in their code.
  static void dispatch(void *self);


/************ MleMouseEvent member variables  ***********************/
public:
private:  
#if defined (__linux__)
#ifdef MLE_XT
  Display* display;                     // caches pointer to X display
#endif // MLE_XT
#endif /* __linux__ */
};


#endif /*__MLE_MOUSEEVENT_H_*/

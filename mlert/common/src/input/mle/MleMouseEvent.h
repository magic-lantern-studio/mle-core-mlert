/** @defgroup MleInput Magic Lantern Runtime Engine Input Library API */

/**
 * @file MleMouseEvent.h
 * @ingroup MleInput
 *
 * @author Mark S. Millard
 * @date May 1, 2003
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

#ifndef __MLE_MOUSEEVENT_H_
#define __MLE_MOUSEEVENT_H_

// Include Magic Lantern header files.
#include "mle/mlTypes.h"

/* X11 includes */
#if defined(__sgi) || defined(__linux__)
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#endif /* __sgi */

/* Win32 includes */
#ifdef WIN32
#include <windows.h>
#endif /* WIN32 */


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
//    #ifdef WIN32
//      MLE_KEY_A = VK_A,
//      MLE_KEY_B = VK_B,
//      ...
//    #endif // WIN32
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
enum MleMouseEvent
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
     (MleMouseEvent mouseEvent,  // one of MleMouseEvent
      void *eventData,          // platform-specific event data
      void *clientData)         // arbitrary client data.


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
#if defined(__sgi) || defined (__linux__)
  Display* display;                     // caches pointer to X display
#endif // __sgi
};


#endif /*__MLE_MOUSEEVENT_H_*/

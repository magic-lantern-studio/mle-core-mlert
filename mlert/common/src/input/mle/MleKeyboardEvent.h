/** @defgroup MleInput Magic Lantern Runtime Engine Input Library API */

/**
 * @file MleKeyboardEvent.h
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

#ifndef __MLE_KEYBOARDEVENT_H_
#define __MLE_KEYBOARDEVENT_H_

// Include Magic Lantern header files.
#include "mle/mlTypes.h"

// Include Magic Lantern Runtime Engine header files.
#include "mle/MleEventDispatcher.h"
#include "mle/MleKeyMap.h"

#if defined(MLE_REHEARSAL) || defined(__sgi) || defined(__linux__)
// We'll use the Xt library to deal with X events.
#include <X11/Intrinsic.h>
#endif /* MLE_REHEARSAL or __sgi */

#if defined(WIN32)
#include <windows.h>
#endif /* WIN32 */

/**
 * @brief Macro constant
 *
 * Maximum number of keys which can be simultaneous down.
 * Default is 20.
 */

#define MLE_MAX_NUMBER_OF_SIMULTANEOUS_KEYS_DOWN 20

/**
 * @brief Global enum
 *
 * MleKeyboardCallbackName is used to specify whether
 * a keyboard event callback should handle all events,
 * the key down event or the key up event.
 */

typedef enum MleKeyboardCallbackName
{
    MLE_EVENT_KEY_ALL = 1,
    MLE_EVENT_KEY_DOWN,
    MLE_EVENT_KEY_UP,
};


// This is the form of the keyboard event callback
// functions that can be registered with FwKeyboardEvent.
//
// PARAMETERS
//   keyboardEvent - The event type passed in eventData. This can be
//                   either MLE_EVENT_KEY_DOWN or MLE_EVENT_KEY_UP.
//   eventData - A platform-specific event data structure which
//               can be queried in a platform-independent way using
//               member functions of FwKeyboardEvent.
//   clientData - The arbitrary client data passed through the registerCB 
//                member function.
typedef void (*MleKeyboardEventCallback) (MleKeyboardCallbackName keyboardEvent, void *eventData, void *clientData);


/* Internal data structure used to record a registered callback. */
typedef struct
{
    MleKeyboardEventCallback m_callbackFn;
    void *m_clientData;
} MleKeyboardCallbackRecord;


/***********************************************************************
CLASS
     MleKeyboardEvent

     This is a Magic Lantern platform-independent keyboard device manager 
     manager class.

     MleKeyboardEvent allows you to register one or more event handlers
     that are called when a key is either pressed, released or both.
     Use registerCB to register a keyboard event handler.  Use unregisterCB
     to unregister the handler, or unregisterAllCB to unregister
     all handlers.

     The event handlers are passed a platform-specific event data structure.
     Some member functions provide platform-independent access to this
     data structure. However, knowledge about the event data structure
     can be exploited by the user to access keyboard capabilities not
     supported by MleKeyboardEvent.

     Keyboard input is queried using Magic Lantern (i.e., platform-independent)
     key symbols obtainable via MleKeyMap.h (which is already included
     in the MleKeyboardPolled header file).

     Platform-Specific Notes
     -----------------------

     SGI systems handles keydown and keyup events in a symmetric function.
     For Windows (i.e., the Win32 API) keyboard events, the keydown
     events are as expected; however, keyup events are valid only
     for the keys other than:

     EXAMPLE
        Any character key
	    Backspace
	    Enter (i.e., Return)
        Escape
	    Tab
     END

     That is, there is no corresponding keyup event for a keydown event for 'A'.
     Therefore, for best cross-platform compatibility, it's recommended
     that you use only rely on keydown events.

     Superclass:   none
     Subclasses:   none

SEE ALSO

     MleKeyboardPolled, MleMousePolled, MleMouseEvent, MleJoystick.
***********************************************************************/


class MLE_RUNTIME_API MleKeyboardEvent
{
  public:

    // Creates an event-based Magic Lantern keyboard device manager.
    // Each title can have at most one flavor of a keyboard input
    // manager (e.g., MleKeyboardPolled, MleKeyboardEvent). If a
    // title uses keyboard input, the title designer
    // must ensure that a keyboard manager is created at
    // the appropriate place in the title. This can be done
    // either in the title's main() or when an actor or some
    // other object that uses keyboard input is created.
    MleKeyboardEvent(void);
 
    // Releases resources used to monitor keyboard device.
    // All callbacks are unregistered. Other types of
    // keyboard input managers can be used after this
    // object is deleted.
    ~MleKeyboardEvent(void);

    // Returns TRUE if a keyboard input manager is currently installed.
    // Only one keyboard manager can be active at any one time.
    MlBoolean keyboardManagerInstalled(void);

    // Registers a callback to handle a keyboard event.
    // Returns TRUE if the callback was successfully registered.
    // The callback can be registered either to handle both
    // key press and release events or you can register separate
    // callbacks to handle each kind of event.
    //
    // keyboardEvent is an enum of type MleKeyboardCallbackName. The
    // following keyboard event callbacks are supported:
    //
    //    MLE_EVENT_KEY_DOWN -- for keydown events
    //    MLe_EVENT_KEY_UP   -- for keyup events
    //    MLE_EVENT_KEY_ALL  -- for both keydown and keyup events
    //
    // After registering a MLE_EVENT_KEY_ALL callback, an attempt
    // to register either of the other callbacks will fail. Also,
    // registering a MLE_EVENT_KEY_ALL callback will automatically
    // unregister all other callbacks.
    //
    // A callback must of be of type MleKeyboardEventCallback. The
    // formal paramters of MleKeyboardEventCallback are:
    //
    //   keyboardEvent - The event type passed in eventData. This can be
    //                   either MLe_EVENT_KEY_DOWN or MLE_EVENT_KEY_UP.
    //   eventData - A platform-specific event data structure which
    //               can be queried in a platform-independent way using
    //               member functions of FwKeyboardEvent.
    //   clientData - The arbitrary client data passed through the registerCB.
    //
    // 
    // clientData is a pointer to an arbitrary data structure that should
    // be passed back to the event handler.
    //
    MlBoolean registerCB(MleKeyboardCallbackName keyboardEvent,
	  MleKeyboardEventCallback callbackFn, void* clientData);

    // Unregisters an existing callback. Returns TRUE if the callback
    // was actually removed (i.e., when it was not registered
    // in the first place).
    MlBoolean unregisterCB(MleKeyboardCallbackName keyboardEvent);

    // Unregisters all existing callbacks.
    MlBoolean unregisterAllCB(void);

    // Returns the Magic Lantern key symbol representing the key that just went up or 
    // down. This should be used inside of the MleKeyboardEventCallback function 
    // to access the key information in a platform-independent manner.
    // For the SGI platform, the platformEvent is of type XKeyEvent, whereas
    // for the Win32 platform it is Win32CallData.
    unsigned int getKeySym(void *platformEvent);

    // Translates the event into a printable character, if possible.
    //
    //    buffer - A buffer where the character string will be returned
    //    bufferSize - the size of the buffer in char-length.
    //    ascii - TRUE means that the key will be translated into ASCII representation
    //            FALSE means that the key will be translated into UNICODE representation
    //
    // translateToCharacter() returns the number of characters deposited
    // into buffer. If 0, then the event pointed to an unprintable character.
    // translateToCharacter() can return more than one character for ASCII
    // translations when diacritical and similar characters are used in
    // conjunction with the main character.
    //
    // For the SGI platform, the platformEvent is of type XKeyEvent, whereas
    // for the Win32 platform it is Win32CallData.
    unsigned int translateToCharacter(void *platformEvent,
	    void *buffer, int bufferSize, MlBoolean ascii);

  private:

  // Dispatches events to registered event handlers.
  void dispatch(void *platformEvent, MleKeyboardCallbackName eventType);

#if defined(__sgi) || defined(__linux__)

  // Handles Xt keyboard events.
  static void MleKeyboardEvent::EventHandler(Widget widget,
					    XPointer keyboardManager,
					    XEvent *event);

#endif /* __sgi */

#if defined(WIN32)
  // Handles Win32 WM_KEYDOWN events.
  static void WM_KEYDOWN_EventHandler(FwEvent event,
				      void *callData,
				      void *clientData);

  // Handles Win32 WM_KEYUP events.
  static void WM_KEYUP_EventHandler(FwEvent event,
				    void *callData,
				    void *clientData);

  // Handles Win32 WM_CHAR events.
  static void WM_CHAR_EventHandler(FwEvent event,
				   void *callData,
				   void *clientData);

#endif /* WIN32 */

  protected:

  public:

    // This global can be used to access the current event-driven
    // keyboard manager object, if any. This is cleared
    // by this class's destructor when the object is deleted.
    static MleKeyboardEvent *g_keyboardManager;

  private:

    MleKeyboardCallbackRecord m_keyDownCallback;
    MleKeyboardCallbackRecord m_keyUpCallback;
    MleKeyboardCallbackRecord m_keyCallback;

  protected:

#if defined(WIN32)

  // Was the last event a keydown event? This is used for a workaround
  // for a bug in the Win32 API function MapVirtualKey.
  MlBoolean m_lastEventKeyDown;

#endif // WIN32
};


#endif /*__MLE_KEYBOARDEVENT_H_*/

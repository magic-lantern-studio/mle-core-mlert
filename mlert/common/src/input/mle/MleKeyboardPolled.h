/** @defgroup MleInput Magic Lantern Runtime Engine Input Library API */

/**
 * @file MleKeyboardPolled.h
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

#ifndef __MLE_KEYBOARDPOLLED_H_
#define __MLE_KEYBOARDPOLLED_H_

// Include Magic Lantern header files.
#include "mle/mlTypes.h"

// Include Magic Lantern Runtime Engine header files.
#include "mle/MleEventDispatcher.h"
#include "mle/MleKeyMap.h"

#if defined(__sgi) || defined(__linux__)
// We'll use the Xt library to deal with X events.
#include <X11/Intrinsic.h>
#endif /* __sgi */

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
 * @brief This is a Magic Lantern platform-independent keyboard device manager 
 * class. 
 *
 * This is a Magic Lantern platform-independent keyboard device manager 
 * class. MleKeyboardPolled allows programmers to query the keyboard 
 * using a polling technique.
 * 
 * Keyboard input is queried using Magic Lantern (i.e., platform-independent)
 * key symbols obtainable via MleKeyMap.h (which is already included
 * in the MleKeyboardPolled header file).
 * 
 * @see MleKeyboardEvent, MleMousePolled, MleMouseEvent, MleJoystick and MleKeyMap.h.
 */

class MLE_RUNTIME_API MleKeyboardPolled
{
  public:

    /**
	 * @brief Creates a keyboard manager with which to poll
     * the state of the keyboard.
	 *
	 * Only one keyboard manager
     * instance is allowed in a title at any one time. The
     * current keyboard manager object can be accessed through
     * this class's static member variable g_keyboardManager. It
     * is possible to dynamically change the type of keyboard manager
     * by deleting any existing keyboard manager and then creating a new one.
     * This object should be deleted when the title no longer needs it.
	 */
    MleKeyboardPolled(void);
 
    /**
	 * @brief Releases all resources used to monitor keyboard device.
	 *
     * This also clears the static g_keyboardManager and signals
     * the system that there's no current keyboard manager (or any type).
	 */
    ~MleKeyboardPolled(void);

    /**
	 * @brief Determine if keyboard is active.
	 *
	 * @return Returns TRUE when the keyboard is "active". A keyboard
     * is said to be active when the title is the target
     * of keyboard user events.
	 */
    MlBoolean keyboardIsActive(void);

    /**
	 * @brief Determine if a key is down.
	 *
	 * @return Returns TRUE if the given key is down.
     * The key must be a Magic Lantern key symbol obtainable
     * from MleKeyMap.h. The key is guaranteed to be down 
     * since the last time the platform's operating system 
     * sent a key event to the title.
	 */
    MlBoolean keyDown(unsigned int keysym);

    /**
	 * @brief Waits for and returns a keyboard input string.
	 *
	 * @param buffer Must be a character buffer that will be filled
     * with the string.
	 * @param bufferSize The size of the buffer.
 	 * @param eoc Must be a terminating character;
     * that is, a character which signals the end of the string.
     * @param timeout Must be the amount of time in milliseconds
     * after which no keyboard input will be accepted. If the timeout
     * is exceeded, the getInputString() returns FALSE; else TRUE.
	 */
    MlBoolean getInputString(char *buffer, int bufferSize, int eoc, int timeout);

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

    // Registers a key that has just gone down.
    void keyWentDown(unsigned int keysym);

    // Signals a key that has just gone up.
    void keyWentUp(unsigned int keysym);

#if defined(__sgi) || defined(__linux__)

    // Handles X FocusIn, FocusOut, EnterNotify and LeaveNotify events
    static void MleFocusChange(Widget /* widget */,
		XPointer /* clientData */, XEvent *event);

    // Handlers X KeyPress and KeyReleased events
    static void MleKeyboardPolledEventHandler(Widget w,
		XPointer keyboardManager, XEvent *event);

#endif /* __sgi */

#if defined(WIN32)

    // Handles Win32 WM_KEYDOWN events.
    static void WM_KEYDOWN_EventHandler(MleEvent event,
		void *callData, void *clientData);

    // Handles Win32 WM_KEYUP events.
    static void WM_KEYUP_EventHandler(MleEvent event,
		void *callData, void *clientData);

    // Handles Win32 WM_CHAR events.
    static void WM_CHAR_EventHandler(MleEvent event,
		void *callData, void *clientData);

#endif // WIN32

  public:

    /**
	 * This global can be used to access the current polled 
     * keyboard manager object, if any. This is cleared
     * by this class's destructor when the object is deleted.
	 */
    static MleKeyboardPolled *g_keyboardManager;

  private:

    // Records which keys symbols are currently down. 
    unsigned int m_keysDownTable[MLE_MAX_NUMBER_OF_SIMULTANEOUS_KEYS_DOWN];

};


#endif /*__MLE_KEYBOARDPOLLED_H_*/

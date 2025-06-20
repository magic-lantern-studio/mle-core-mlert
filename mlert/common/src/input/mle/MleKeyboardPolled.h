/** @defgroup MleInput Magic Lantern Runtime Engine Input Library API */

/**
 * @file MleKeyboardPolled.h
 * @ingroup MleInput
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2000-2025 Wizzer Works
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

#ifndef __MLE_KEYBOARDPOLLED_H_
#define __MLE_KEYBOARDPOLLED_H_

// Include Magic Lantern header files.
#include "mle/mlTypes.h"

// Include Magic Lantern Runtime Engine header files.
#include "mle/MleEventDispatcher.h"
#include "mle/MleKeyMap.h"

#if defined(__linux__)
#ifdef MLE_XT
// We'll use the Xt library to deal with X events.
#include <X11/Intrinsic.h>
#endif /* MLE_XT */
#endif /* __linux__ */

#if defined(_WINDOWS)
#include <windows.h>
#endif /* _WINDOWS */


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

#if defined(__linux__) || defined(__APPLE__)
#ifdef MLE_XT
    // Handles X11 FocusIn, FocusOut, EnterNotify and LeaveNotify events
    static void MleFocusChange(Widget /* widget */,
        XPointer /* clientData */, XEvent *event);

    // Handles X11 KeyPress and KeyReleased events
    static void MleKeyboardPolledEventHandler(Widget w,
        XPointer keyboardManager, XEvent *event);
#endif /* MLE_XT */
#ifdef MLE_QT
    // Handles QKeyEvent events.
    static void QT_KEYPRESS_EventHandler(MleEvent event,
        void *callData, void *clientData);

    // Handles QKeyEvent events.
    static void QT_KEYRELEASE_EventHandler(MleEvent event,
        void *callData, void *clientData);
#endif /* MLE_QT */
#endif /* __linux__ */

#if defined(_WINDOWS)
    // Handles Win32 WM_KEYDOWN events.
    static void WM_KEYDOWN_EventHandler(MleEvent event,
        void *callData, void *clientData);

    // Handles Win32 WM_KEYUP events.
    static void WM_KEYUP_EventHandler(MleEvent event,
        void *callData, void *clientData);

    // Handles Win32 WM_CHAR events.
    static void WM_CHAR_EventHandler(MleEvent event,
        void *callData, void *clientData);
#endif // _WINDOWS

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

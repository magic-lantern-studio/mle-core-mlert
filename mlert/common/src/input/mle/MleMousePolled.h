/** @defgroup MleInput Magic Lantern Runtime Engine Input Library API */

/**
 * @file MleMousePolled.h
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

#ifndef __MLE_MOUSEPOLLED_H_
#define __MLE_MOUSEPOLLED_H_

// Include Magic Lantern header files.
#include "mle/mlTypes.h"
#include "mle/MleKeyMap.h"

// Include Linux header files
#if defined(__linux__)
#ifdef MLE_XT
#include <X11/Intrinsic.h>
#endif // MLE_XT
#endif // __linux__

// Include Windows header files.
#if defined(_WINDOWS)
#include <windows.h>
#include "mle/MleEventDispatcher.h"
#endif // _WINDOWS


/*********************************************************************
CLASS
     MleMousePolled Class

     This class allows the programmer to handle mouse status
     using member functions that poll mouse state. This manager
     supports mice with one, two or three buttons.

KEYWORDS

OVERVIEW
     This class allows the programmer to handle mouse status
     using member functions that poll mouse state. This manager
     supports mice with one, two or three buttons.

     Mouse buttons are queried using FireWalker (i.e., platform-independent)
     key symbols obtainable via keymap.h (which is already included in the
     MleMousePolled header file).

     Superclasses: none
     Subclasses: none

SEE ALSO
     MleKeyboardEvent, MleKeyboardPolled, MleMouseEvent, MleJoystick 
     and keymap.h.
*********************************************************************/

#define MLE_MAX_NUMBER_OF_MOUSE_BUTTONS_SUPPORTED 3

class MleMousePolled
{
  public:

    // Constructor for creating an instance of this mouse manager.
    // This object should be deleted when the title no longer needs it.
    // The current instance of the polled mouse manager, if any,
    // can be accessed through this class's mouseManager static variable.
    MleMousePolled(void);

    // Destructor releases mouse driver resources and ensures
    // that the default platform mouse cursor pointer is restored.
    ~MleMousePolled(void);

    // Returns the number of buttons on the mouse.
    // If it returns 0, then there's no mouse physically
    // attached to the system.
    int getNumberOfButtons(void);

    // Returns TRUE when given mouse button is down and FALSE when
    // the button is up.  If keysym is 0, then TRUE will be
    // returned if any button is down. The mouse buttons are
    // recognized by the following FireWalker keysyms:
    //
    //    MLE_KEY_Pointer_Button1  -- left button
    //    MLE_KEY_Pointer_Button2  -- middle button
    //    MLE_KEY_Pointer_Button3  -- right button
    //
    // Note: Magic Lantern keysyms are defined in MleKeyMap.h.
    //
    MlBoolean buttonDown(unsigned int keysym);

    // Returns TRUE when given modifier key was down when
    // the mouse data was last updated.
    // keysym must be a modifier key supported by the
    // target platform. The common modifiers supported
    // by Linux and Windows are:
    //
    //    MLE_KEY_Control -- control key modifier
    //    MLE_KEY_Shift   -- shift key modifier
    //
    // If a key that's not supported as a modifier is passed in,
    // then the return value will be meaningless.
    MlBoolean modifierDown(unsigned int keysym);

    // Returns the current mouse position in
    // absolute x,y coordinates. These coordinates
    // are relative to the window of the title in
    // which the mouse pointer is in.
    void getPosition(int *x, int *y);

    // Returns TRUE if visual cursor has been set.
    // Sets the mouse cursor to the supplied one.
    // Cursors are stored in a file in a format
    // appropriate for each platform.
    // The initial cursor is the default cursor used
    // by the platform. Since a title can use multiple
    // cursors, it's the title's responsibility to load
    // and register the necessary cursors.
    MlBoolean setCursor(char* filename);

    // Sets the visibility of the mouse cursor.
    // If show = TRUE, then the cursor will be shown;
    // else it will be hidden. It's the title's
    // responsibility to remember whether the
    // mouse is hidden or not. Initially, the
    // mouse is visible.
    void setVisibility(MlBoolean show);

  private:

    // Auxiliary function for MleMousePolledEventHandler
    void updateModifiers(unsigned int modifiers);

#if defined(__linux__)
#ifdef MLE_XT
    // Handles X mouse events
    static void MleMousePolledEventHandler(Widget w,
        XPointer mouseManager, XEvent *event);
#endif // MLE_XT
#endif // __linux__

#if defined(_WINDOWS)
    // Handles Win32 mouse messages
    static void  MleMousePolledEventHandler(MleEvent event,
        void *eventData, void *mouseManager);
#endif // _WINDOWS

  public:

      // This global can be used to access the current polled
      // mouse manager instance.
      static MleMousePolled *mouseManager;

  private:

    // Latest mouse x position
    int x_pos;

    // Latest mouse y position
    int y_pos;

    // State of three supported mouse buttons
    MlBoolean buttonIsDown[MLE_MAX_NUMBER_OF_MOUSE_BUTTONS_SUPPORTED];

    // Control key modifier state (TRUE = down)
    MlBoolean controlKeyDown;

    // Shift key modifier state (TRUE = down)
    MlBoolean shiftKeyDown;
};


#endif /*__MLE_MOUSEPOLLED_H_*/

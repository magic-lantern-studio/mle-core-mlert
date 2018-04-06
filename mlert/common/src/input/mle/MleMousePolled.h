/** @defgroup MleInput Magic Lantern Runtime Engine Input Library API */

/**
 * @file MleMousePolled.h
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

#ifndef __MLE_MOUSEPOLLED_H_
#define __MLE_MOUSEPOLLED_H_

/************************Begin common includes***********************/
#include "mle/mlTypes.h"
#include "mle/MleKeyMap.h"
/************************End common includes*************************/


/************************Begin SGI includes***********************/
#if defined(MLE_REHEARSAL) || defined(__sgi) || defined(__linux__)
#include <X11/Intrinsic.h>
#endif // MLE_REHEARSAL or __sgi
/*************************End SGI includes*************************/


/*************************Begin Win32 includes*********************/
#if defined(WIN32)
#include <windows.h>
#include "mle/MleEventDispatcher.h"
#endif // WIN32
/*************************End Win32 includes***********************/



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

/************ MleMousePolled member functions  ***********************/
public:
  //////////
  // Constructor for creating an instance of this mouse manager.
  // This object should be deleted when the title no longer needs it.
  // The current instance of the polled mouse manager, if any,
  // can be accessed through this class's mouseManager static variable.
  MleMousePolled(void);

  //////////
  // Destructor releases mouse driver resources and ensures 
  // that the default platform mouse cursor pointer is restored.
  ~MleMousePolled(void);

  //////////
  // Returns the number of buttons on the mouse.
  // If it returns 0, then there's no mouse physically
  // attached to the system.
  int getNumberOfButtons(void);

  //////////
  // Returns TRUE when given mouse button is down and FALSE when
  // the button is up.  If keysym is 0, then TRUE will be
  // returned if any button is down. The mouse buttons are
  // recognized by the following FireWalker keysyms:
  //
  //    MLE_KEY_Pointer_Button1  -- left button
  //    MLE_KEY_Pointer_Button2  -- middle button
  //    MLE_KEY_Pointer_Button3  -- right button
  //
  // Note: FireWalker keysyms are defined in keymap.h.
  //
  MlBoolean buttonDown(unsigned int keysym);

  //////////
  // Returns TRUE when given modifier key was down when
  // the mouse data was last updated. 
  // keysym must be a modifier key supported by the
  // target platform. The common modifiers supported
  // by SGI and Windows are:
  //
  //    MLE_KEY_Control -- control key modifier
  //    MLE_KEY_Shift   -- shift key modifier
  //
  // If a key that's not supported as a modifier is passed in,
  // then the return value will be meaningless.
  MlBoolean modifierDown(unsigned int keysym);

  //////////
  // Returns the current mouse position in
  // absolute x,y coordinates. These coordinates
  // are relative to the window of the title in
  // which the mouse pointer is in.
  void getPosition(int *x, int *y);

  //////////
  // Returns TRUE if visual cursor has been set.
  // Sets the mouse cursor to the supplied one.
  // Cursors are stored in a file in a format
  // appropriate for each platform.
  // The initial cursor is the default cursor used
  // by the platform. Since a title can use multiple 
  // cursors, it's the title's responsibility to load
  // and register the necessary cursors.
  MlBoolean setCursor(char* filename);

  //////////
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

#if defined(MLE_REHEARSAL) || defined(__sgi) || defined(__linux__)
  // Handles X mouse events
  static void MleMousePolledEventHandler(Widget w,
					XPointer mouseManager,
					XEvent *event);
#endif // MLE_REHEARSAL or __sgi

#if defined(WIN32)
  // Handles Win32 mouse messages
  static void  MleMousePolledEventHandler(MleEvent event,
					 void *eventData,
					 void *mouseManager);
#endif // WIN32

protected:

/************ MleMousePolled member variables  ***********************/
public:
  //////////
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

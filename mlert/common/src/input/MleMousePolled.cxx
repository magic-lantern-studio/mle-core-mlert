/** @defgroup MleInput Magic Lantern Runtime Engine Input Library API */

/**
 * @file MleMousePolled.cxx
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

/************************Begin common includes***********************/
#include "mle/mlPlatformData.h"
#include "mle/mlAssert.h"

// Need to get theTitle global
#include "mle/MleDirector.h"
#include "mle/MleMousePolled.h"
/************************End common includes*************************/


/***************************Begin SGI includes***********************/
#if defined(MLE_REHEARSAL) || defined( __sgi)
// Need to get the X display
#include "mle/sgiplatform.h"
#endif // MLE_REHEARSAL or __sgi
/***************************End SGI includes*************************/


/***************************Begin Win32 includes*********************/
#if defined(_WINDOWS)
//#include "mle/comstg.h"
#endif // _WINDOWS
/***************************End Win32 includes***********************/



//////////
// Holds global mouse manager (polled).
MleMousePolled *MleMousePolled::mouseManager = NULL;


MleMousePolled::MleMousePolled(void)
{

  // Get the platform data
#if defined(MLE_REHEARSAL) || defined(__sgi)
  MleSGIPlatformData *platformData = (MleSGIPlatformData *)g_theTitle->m_platformData;
#endif // MLE_REHEARSAL or __sgi
#if defined(_WINDOWS)
  MleWin32PlatformData *platformData = (MleWin32PlatformData *)g_theTitle->m_platformData;
#endif // _WINDOWS
  MLE_ASSERT(platformData);

  // Install and register only one mouse manager per title at any one time.
  //MLE_ASSERT(platformData->m_mouseManager == MLE_INPUT_DEVICE_MANAGER_NOT_INSTANTIATED);
  MLE_ASSERT(! platformData->hasMouseManager());
  MleMousePolled::mouseManager = this;
  //platformData->m_mouseManager = MLE_INPUT_DEVICE_MANAGER_INSTANTIATED;
  platformData->setMouseManager(MLE_INPUT_DEVICE_MANAGER_INSTANTIATED);

  // Install platform-specific mouse event handlers
#if defined(MLE_REHEARSAL) || defined(__sgi)
  // Ensure that we have the widget for the window's rendering area.
  MLE_ASSERT(platformData->widget);

  // Add mouse event handler (highest possible priority).
  XtInsertEventHandler(platformData->widget,
               (ButtonPressMask |
            ButtonReleaseMask |
            PointerMotionMask),
               False,
               (XtEventHandler) MleMousePolled::MleMousePolledEventHandler,
               this,
               XtListHead);
#endif // MLE_REHEARSAL or __sgi
#if defined(_WINDOWS)
  g_theTitle->m_theEventMgr->installEventCB((MleEvent) WM_LBUTTONDOWN,
                     (MleCallback) MleMousePolled::MleMousePolledEventHandler,
                     this);
  g_theTitle->m_theEventMgr->installEventCB((MleEvent) WM_LBUTTONUP,
                     (MleCallback) MleMousePolled::MleMousePolledEventHandler,
                     this);
  g_theTitle->m_theEventMgr->installEventCB((MleEvent) WM_MBUTTONDOWN,
                     (MleCallback) MleMousePolled::MleMousePolledEventHandler,
                     this);
  g_theTitle->m_theEventMgr->installEventCB((MleEvent) WM_MBUTTONUP,
                     (MleCallback) MleMousePolled::MleMousePolledEventHandler,
                     this);
  g_theTitle->m_theEventMgr->installEventCB((MleEvent) WM_RBUTTONDOWN,
                     (MleCallback) MleMousePolled::MleMousePolledEventHandler,
                     this);
  g_theTitle->m_theEventMgr->installEventCB((MleEvent) WM_RBUTTONUP,
                     (MleCallback) MleMousePolled::MleMousePolledEventHandler,
                     this);
  g_theTitle->m_theEventMgr->installEventCB((MleEvent) WM_MOUSEMOVE,
                     (MleCallback) MleMousePolled::MleMousePolledEventHandler,
                     this);
#endif // _WINDOWS

  // Initialize mouse state
  controlKeyDown = shiftKeyDown = FALSE;
  x_pos = y_pos = 0;
  for (int i = 0 ; i < MLE_MAX_NUMBER_OF_MOUSE_BUTTONS_SUPPORTED ; i++)
    buttonIsDown[i] = FALSE;
}

MleMousePolled::~MleMousePolled(void)
{
  // Get the platform data
#if defined(MLE_REHEARSAL) || defined(__sgi)
  MleSGIPlatformData *platformData = (MleSGIPlatformData *)g_theTitle->m_platformData;
#endif // MLE_REHEARSAL or __sgi
#if defined(_WINDOWS)
  MleWin32PlatformData *platformData = (MleWin32PlatformData *)g_theTitle->m_platformData;
#endif // _WINDOWS
  MLE_ASSERT(platformData);

#if defined(MLE_REHEARSAL) || defined(__sgi)
  // Remove mouse event handler
    XtRemoveEventHandler(platformData->widget,
             (ButtonPressMask |
              ButtonReleaseMask |
              PointerMotionMask),
             False,
             (XtEventHandler) MleMousePolled::MleMousePolledEventHandler,
             this);
#endif // MLE_REHEARSAL or __sgi
#if defined(_WINDOWS)
  // Remove Win32 message handler
  g_theTitle->m_theEventMgr->uninstallEvent((MleEvent) WM_LBUTTONDOWN);
  g_theTitle->m_theEventMgr->uninstallEvent((MleEvent) WM_LBUTTONUP);
  g_theTitle->m_theEventMgr->uninstallEvent((MleEvent) WM_MBUTTONDOWN);
  g_theTitle->m_theEventMgr->uninstallEvent((MleEvent) WM_MBUTTONUP);
  g_theTitle->m_theEventMgr->uninstallEvent((MleEvent) WM_RBUTTONDOWN);
  g_theTitle->m_theEventMgr->uninstallEvent((MleEvent) WM_RBUTTONUP);
  g_theTitle->m_theEventMgr->uninstallEvent((MleEvent) WM_MOUSEMOVE);
#endif // _WINDOWS

  // Signal released mouse manager resources
  //platformData->m_mouseManager = MLE_INPUT_DEVICE_MANAGER_NOT_INSTANTIATED;
  platformData->setMouseManager(MLE_INPUT_DEVICE_MANAGER_NOT_INSTANTIATED);
  MleMousePolled::mouseManager = NULL;
}

int
MleMousePolled::getNumberOfButtons(void)
{
// xxx use appropriate query functions for each OS
#if defined(MLE_REHEARSAL) || defined(__sgi) 
  return 3;
#endif // MLE_REHEARSAL or __sgi

#if defined(_WINDOWS)
  return 2;
#endif // _WINDOWS
}

void
MleMousePolled::getPosition(int *x, int *y)
{
  *x = x_pos;
  *y = y_pos;
}

MlBoolean
MleMousePolled::setCursor(char* /* filename */)
{
  return FALSE;
}

void
MleMousePolled::setVisibility(MlBoolean /* show */)
{
}

MlBoolean
MleMousePolled::buttonDown(unsigned int keysym)
{
  if (keysym == MLE_KEY_Pointer_Button1)
    return buttonIsDown[0]; // Left button down
  if (keysym == MLE_KEY_Pointer_Button2)
    return buttonIsDown[1]; // Middle button down
  if (keysym == MLE_KEY_Pointer_Button3)
    return buttonIsDown[2]; // Right button down
  return FALSE;             // No button is down
}

MlBoolean
MleMousePolled::modifierDown(unsigned int keysym)
{
  // Shift modifier test
  if (keysym == MLE_KEY_Shift)
     return shiftKeyDown;

  // Control modifier test
  if (keysym == MLE_KEY_Control)
     return controlKeyDown;

  // Any other modifier
  return FALSE;
}



// Begin Event handlers ****************************************
void
MleMousePolled::updateModifiers(unsigned int modifiers)
{
#if defined(MLE_REHEARSAL) || defined(__sgi)
  controlKeyDown = (MlBoolean) (modifiers & ControlMask);
  shiftKeyDown = (MlBoolean) (modifiers & ShiftMask);
#endif // MLE_REHEARSAL or __sgi

#if defined(_WINDOWS)
  controlKeyDown = (MlBoolean) (modifiers & MK_CONTROL);
  shiftKeyDown = (MlBoolean) (modifiers & MK_SHIFT);
#endif // _WINDOWS
}

#if defined(MLE_REHEARSAL) || defined(__sgi) 
void
MleMousePolled::MleMousePolledEventHandler(Widget /* w */,
                     XPointer mouseManager,
                     XEvent *event)
{
  MleMousePolled *mgr = (MleMousePolled *) mouseManager;
  XButtonReleasedEvent *buttonEvent = (XButtonReleasedEvent *) event;

  switch (event->type) {
  case MotionNotify:
    XMotionEvent *motionEvent = (XMotionEvent *) event;
    mgr->x_pos = motionEvent->x;
    mgr->y_pos = motionEvent->y;
    mgr->updateModifiers(motionEvent->state);
    break;
  case ButtonPress:
    if (buttonEvent->button == Button1)
      mgr->buttonIsDown[0] = TRUE;
    else {
      if (buttonEvent->button == Button2)
    mgr->buttonIsDown[1] = TRUE;
      else
    mgr->buttonIsDown[2] = TRUE;
    }
    mgr->updateModifiers(buttonEvent->state);
    break;
  case ButtonRelease:
    if (buttonEvent->button == Button1)
      mgr->buttonIsDown[0] = FALSE;
    else {
      if (buttonEvent->button == Button2)
    mgr->buttonIsDown[1] = FALSE;
      else
    mgr->buttonIsDown[2] = FALSE;
    }
    mgr->updateModifiers(buttonEvent->state);
    break;
  }
}
#endif // MLE_REHEARSAL or __sgi


#if defined(_WINDOWS)
void
MleMousePolled::MleMousePolledEventHandler(MleEvent event,
                     void* eventData,
                     void* manager)
{
  Win32CallData *data = (Win32CallData *) eventData;
  MleMousePolled *mgr = (MleMousePolled *) manager;

  // Update state of modifier keys
  mgr->updateModifiers(data->wParam);

  switch(event) {
  case WM_MOUSEMOVE:
    mgr->x_pos = LOWORD(data->lParam);
    mgr->y_pos = HIWORD(data->lParam);
    break;
  case WM_LBUTTONDOWN:
    mgr->buttonIsDown[0] = TRUE;
    mgr->buttonIsDown[1] = MK_MBUTTON & data->wParam;
    mgr->buttonIsDown[2] = MK_RBUTTON & data->wParam;
    break;
  case WM_LBUTTONUP:
    mgr->buttonIsDown[0] = FALSE;
    mgr->buttonIsDown[1] = MK_MBUTTON & data->wParam;
    mgr->buttonIsDown[2] = MK_RBUTTON & data->wParam;
    break;
  case WM_MBUTTONDOWN:
    mgr->buttonIsDown[0] = MK_LBUTTON & data->wParam;
    mgr->buttonIsDown[1] = TRUE;
    mgr->buttonIsDown[2] = MK_RBUTTON & data->wParam;
    break;
  case WM_MBUTTONUP:
    mgr->buttonIsDown[0] = MK_LBUTTON & data->wParam;
    mgr->buttonIsDown[1] = FALSE;
    mgr->buttonIsDown[2] = MK_RBUTTON & data->wParam;
    break;
  case WM_RBUTTONDOWN:
    mgr->buttonIsDown[0] = MK_LBUTTON & data->wParam;
    mgr->buttonIsDown[1] = MK_MBUTTON & data->wParam;
    mgr->buttonIsDown[2] = TRUE;
    break;
  case WM_RBUTTONUP:
    mgr->buttonIsDown[0] = MK_LBUTTON & data->wParam;
    mgr->buttonIsDown[1] = MK_MBUTTON & data->wParam;
    mgr->buttonIsDown[2] = FALSE;
    break;
  }
}
#endif // _WINDOWS
// End Event Handlers ******************************************






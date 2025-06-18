/** @defgroup MleInput Magic Lantern Runtime Engine Input Library API */

/**
 * @file MleKeyboardEvent.cxx
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

#ifndef __MLE_KEYBOARDEVENT_H_
#define __MLE_KEYBOARDEVENT_H_


/************************Begin common includes***********************/
#include "fw/titleenv.h"
#include "fw/kbrdevnt.h"
/************************End common includes*************************/


/***************************Begin SGI includes***********************/
#if defined(MLE_REHEARSAL)
// Need SGI platform data structure access
#include "fw/sgiplatform.h"
#endif // MLE_REHEARSAL
/***************************End SGI includes*************************/


/***************************Begin Win32 includes*********************/
#if defined(_WINDOWS)
#include <windows.h>
// Need Win32 platform data structure access
#include "fw/comstg.h"
#endif // _WINDOWS
/***************************End Win32 includes***********************/



//////////
// Returns n bits of x starting at bit number p.
#define _getbits(x,p,n) ((x >> (p+1-n)) & ~(~0 << n))

//////////
// Holds global keyboard manager (event).
FwKeyboardEvent *FwKeyboardEvent::keyboardManager = NULL;


FwKeyboardEvent::FwKeyboardEvent(void)
{
  // Get the platform data
#if defined(MLE_REHEARSAL)
  FwSGIPlatformData *platformData = (FwSGIPlatformData *)theTitle->platformData;
#endif // MLE_REHEARSAL
#if defined(_WINDOWS)
  FwWin32Data *platformData = (FwWin32Data *)theTitle->platformData;
#endif // _WINDOWS
  FW_ASSERT(platformData);

  // Install and register only one keyboard manager (of any type) per title at any one time.
  FW_ASSERT(platformData->keyboardManager == FW_INPUT_DEVICE_MANAGER_NOT_INSTANTIATED);
  FwKeyboardEvent::keyboardManager = this;
  platformData->keyboardManager = FW_INPUT_DEVICE_MANAGER_INSTANTIATED;

#if defined(MLE_REHEARSAL)
  // Get the widget for the window's rendering area.
  FW_ASSERT(platformData->widget);

#if 0
  // Add focus event handler.
  if (platformData->focusEventHandlerRefCount == 0) {
    XtAddEventHandler(platformData->widget,
		      (FocusChangeMask |
		       EnterWindowMask |
		       LeaveWindowMask),
		      False,
		      (XtEventHandler) FwFocusChange,
		      NULL);

    platformData->inFocus = FALSE; // Assume not now in focus
  }
  platformData->focusEventHandlerRefCount += 1;
#endif // 0

  // Add keyboard event handler (highest possible priority).
  XtInsertEventHandler(platformData->widget,
		       (KeyPressMask | KeyReleaseMask),
		       False,
		       (XtEventHandler) FwKeyboardEvent::EventHandler,
		       this,
		       XtListHead);
#endif // MLE_REHEARSAL
  
#if defined(_WINDOWS)
  theTitle->eventMgr->installEventCB(
	 (FwEvent) WM_KEYDOWN,
         (FwCallback) FwKeyboardEvent::WM_KEYDOWN_EventHandler,
         this);
  theTitle->eventMgr->installEventCB(
	 (FwEvent) WM_KEYUP,
         (FwCallback) FwKeyboardEvent::WM_KEYUP_EventHandler,
         this);
  theTitle->eventMgr->installEventCB(
	 (FwEvent) WM_CHAR,
         (FwCallback) FwKeyboardEvent::WM_CHAR_EventHandler,
         this);
#endif // _WINDOWS

  // Initialize callback registry
  keyDownCallback.callbackFn = NULL;
  keyDownCallback.clientData = NULL;
  keyUpCallback.callbackFn = NULL;
  keyUpCallback.clientData = NULL;
  keyCallback.callbackFn = NULL;
  keyCallback.clientData = NULL;

#if defined(_WINDOWS)
  lastEventKeyDown = FALSE;
#endif // _WINDOWS
}

FwKeyboardEvent::~FwKeyboardEvent(void)
{
  // Remove all callbacks
  unregisterAllCB();

#if defined(MLE_REHEARSAL)
  // Get platform-independent data
  FwSGIPlatformData *platformData = (FwSGIPlatformData *)theTitle->platformData;

#if 0
  // Decrease use count of focus handlers
  platformData->focusEventHandlerRefCount -= 1;

  // Remove focus event handlers if I am only user
  if (platformData->focusEventHandlerRefCount == 0)
    XtRemoveEventHandler(platformData->widget,
			 (FocusChangeMask |
			  EnterWindowMask |
			  LeaveWindowMask),
			 False,
			 (XtEventHandler) FwFocusChange,
			 NULL);
#endif // 0

  // Remove keyboard-specific event handlers
    XtRemoveEventHandler(platformData->widget,
			 (KeyPressMask | KeyReleaseMask),
			 False,
			 (XtEventHandler) FwKeyboardEvent::EventHandler,
			 this);
#endif // MLE_REHEARSAL

#if defined(_WINDOWS)
  FwWin32Data *platformData = (FwWin32Data *)theTitle->platformData;

  theTitle->eventMgr->uninstallEvent((FwEvent) WM_KEYDOWN);
  theTitle->eventMgr->uninstallEvent((FwEvent) WM_KEYUP);
  theTitle->eventMgr->uninstallEvent((FwEvent) WM_CHAR);
#endif // _WINDOWS

  // Release keyboard manager control
  FwKeyboardEvent::keyboardManager = NULL;
  platformData->keyboardManager = FW_INPUT_DEVICE_MANAGER_NOT_INSTANTIATED;
}

FwBoolean
FwKeyboardEvent::registerCB(FwKeyboardCallbackName keyboardEvent,
			    FwKeyboardEventCallback callbackFn,
			    void* clientData)
{
  FwBoolean result = FALSE;

  switch(keyboardEvent) {
  case FW_EVENT_KEY_ALL:
    keyCallback.callbackFn = callbackFn;
    keyCallback.clientData = clientData;
    result = TRUE;
    break;
  case FW_EVENT_KEY_DOWN:
    if (keyCallback.callbackFn != NULL)
      break;
    keyDownCallback.callbackFn = callbackFn;
    keyDownCallback.clientData = clientData;
    result = TRUE;
    break;
  case FW_EVENT_KEY_UP:
    if (keyCallback.callbackFn != NULL)
      break;
    keyUpCallback.callbackFn = callbackFn;
    keyUpCallback.clientData = clientData;
    result = TRUE;
    break;
  }
  return result;
}

FwBoolean
FwKeyboardEvent::unregisterCB(FwKeyboardCallbackName keyboardEvent)
{
  switch (keyboardEvent) {
  case FW_EVENT_KEY_ALL:
    keyCallback.callbackFn = (FwKeyboardEventCallback) NULL;
    keyCallback.clientData = NULL;
    keyDownCallback.callbackFn = (FwKeyboardEventCallback) NULL;
    keyDownCallback.clientData = NULL;
    keyUpCallback.callbackFn = (FwKeyboardEventCallback) NULL;
    keyUpCallback.clientData = NULL;
    break;
  case FW_EVENT_KEY_DOWN:
    keyDownCallback.callbackFn = (FwKeyboardEventCallback) NULL;
    keyDownCallback.clientData = NULL;
    break;
  case FW_EVENT_KEY_UP:
    keyUpCallback.callbackFn = (FwKeyboardEventCallback) NULL;
    keyUpCallback.clientData = NULL;
    break;
  }
  return TRUE;
}

FwBoolean
FwKeyboardEvent::unregisterAllCB(void)
{
  unregisterCB(FW_EVENT_KEY_ALL);
  unregisterCB(FW_EVENT_KEY_DOWN);
  unregisterCB(FW_EVENT_KEY_UP);

  return TRUE;
}

FwBoolean
FwKeyboardEvent::keyboardManagerInstalled(void)
{
  // Get the platform data
#if defined(MLE_REHEARSAL)
  FwSGIPlatformData *platformData = (FwSGIPlatformData *)theTitle->platformData;
#endif // MLE_REHEARSAL
#if defined(_WINDOWS)
  FwWin32Data *platformData = (FwWin32Data *)theTitle->platformData;
#endif // _WINDOWS
  FW_ASSERT(platformData);

  return platformData->keyboardManager;
}

unsigned int
FwKeyboardEvent::getKeySym(void *platformEvent)
{
#if defined(MLE_REHEARSAL)
  KeySym keysym;
  XKeyEvent* keyEvent  = (XKeyEvent *) platformEvent;
  XComposeStatus compose;
  char buffer[20];
  int buffersize = 20;
  int count = 0;
  
  count = XLookupString(keyEvent, buffer, buffersize,
			&keysym, &compose);
#endif // MLE_REHEARSAL

#if defined(_WINDOWS)
#if 0
  unsigned int keysym;
  Win32CallData *data = (Win32CallData *) platformEvent;
  unsigned int scanCode =  _getbits(data->lParam, 23, 8);
  keysym = MapVirtualKey(scanCode, 1); */
#endif // 0

  Win32CallData *data = (Win32CallData *) platformEvent;
  unsigned int keysym = data->wParam;

#endif // _WINDOWS

  return keysym;
}

unsigned int
FwKeyboardEvent::translateToCharacter(void *platformEvent,
				      void *buffer,
				      int bufferSize,
				      FwBoolean ascii)
{
  unsigned int count = 0;

#if defined(MLE_REHEARSAL)
  XKeyEvent* keyEvent  = (XKeyEvent *) platformEvent;
  KeySym keysym;
  XComposeStatus compose;
  if (ascii)
    count = XLookupString(keyEvent, (char *)buffer, bufferSize,
			  &keysym, &compose);
  else {
    // xxx unicode not handled
  }
#endif // MLE_REHEARSAL

#if defined(_WINDOWS)
  unsigned int keysym = 0;
  unsigned char lpbKeyState[256];  // key state array
  Win32CallData *data = (Win32CallData *) platformEvent;
  unsigned int scanCode =  _getbits(data->lParam, 23, 8);
  keysym = MapVirtualKey(scanCode, 1);

  GetKeyboardState(&lpbKeyState[0]);

  if (ascii)
    count = ToAscii(keysym, scanCode, &lpbKeyState[0], (unsigned short *)buffer, 0);
  else
    count = ToUnicode(keysym, scanCode, &lpbKeyState[0], (unsigned short *)buffer, bufferSize, 0);
#endif // _WINDOWS

  return count;
}



// Begin Event handlers ****************************************

#if 0
static void 
FwFocusChange(Widget /* widget */, XPointer /* clientData */, XEvent *event)
{
  FwSGIPlatformData *platformData = (FwSGIPlatformData *)theTitle->platformData;

  switch(event->type) {
  case FocusIn:
    platformData->inFocus =        TRUE;
    platformData->keyboardActive = TRUE;
    break;
  case FocusOut:
    platformData->inFocus =        FALSE;
    platformData->keyboardActive = FALSE;
    break;
  case EnterNotify:
    if (event->xcrossing.focus)
      platformData->keyboardActive = TRUE;
    else
      platformData->keyboardActive = FALSE;
    break;
  case LeaveNotify:
    if (platformData->inFocus)
      platformData->keyboardActive = FALSE;
    else
      platformData->keyboardActive = TRUE;
    break;
  }
}
#endif // 0

void
FwKeyboardEvent::dispatch(void *platformEvent,  FwKeyboardCallbackName eventType)
{
  FwKeyboardEvent *mgr = FwKeyboardEvent::keyboardManager;

  if (mgr->keyCallback.callbackFn) {
    keyCallback.callbackFn(eventType,
			   (void *) platformEvent,
			   keyCallback.clientData);
  }
  else {
    if ((eventType == FW_EVENT_KEY_DOWN) &
	(mgr->keyDownCallback.callbackFn != NULL)) {
      keyDownCallback.callbackFn(eventType,
				 (void *) platformEvent,
				 keyDownCallback.clientData);
    }
    else {
    if ((eventType == FW_EVENT_KEY_UP) &
	(mgr->keyUpCallback.callbackFn != NULL))
      keyUpCallback.callbackFn(eventType,
			       (void *) platformEvent,
			       keyUpCallback.clientData);
  }
  }
}


#if defined(MLE_REHEARSAL)
void
FwKeyboardEvent::EventHandler(Widget /* widget */,
			      XPointer m,
			      XEvent *event)
{
  XKeyEvent *keyEvent = (XKeyEvent *) event;
  FwKeyboardEvent *mgr = (FwKeyboardEvent *) m;

  switch (event->type) {
  case KeyPress:
    mgr->dispatch(keyEvent, FW_EVENT_KEY_DOWN);
    break;
  case KeyRelease:
    mgr->dispatch(keyEvent, FW_EVENT_KEY_UP);
    break;
  }
}
#endif // MLE_REHEARSAL


#if defined(_WINDOWS)

void
FwKeyboardEvent::WM_KEYDOWN_EventHandler(FwEvent event,
					 void* eventData,
					 void* manager)
{
  FwKeyboardEvent *mgr = (FwKeyboardEvent *) manager;
  
  mgr->lastEventKeyDown = TRUE;

  Win32CallData *evt = (Win32CallData *) eventData;
  int keysym = evt->wParam;
  if (!(((keysym >= FW_KEY_0) && (keysym <= FW_KEY_z))
	|| (keysym == FW_KEY_BackSpace)
	|| (keysym == FW_KEY_Return)
	|| (keysym == FW_KEY_Escape)
	|| (keysym == FW_KEY_Tab)))
    mgr->dispatch((void *) eventData, FW_EVENT_KEY_DOWN);
}

void
FwKeyboardEvent::WM_KEYUP_EventHandler(FwEvent event,
				       void* eventData,
				       void* manager)
{
  FwKeyboardEvent *mgr = (FwKeyboardEvent *) manager;
  
  mgr->lastEventKeyDown = FALSE;

  mgr->dispatch((void *) eventData, FW_EVENT_KEY_UP);
}

void
FwKeyboardEvent::WM_CHAR_EventHandler(FwEvent event,
				      void* eventData,
				      void* manager)
{
  FwKeyboardEvent *mgr = (FwKeyboardEvent *) manager;
  unsigned int state = 0;

  if (mgr->lastEventKeyDown == TRUE)
    state = FW_EVENT_KEY_DOWN;
  else
    state = FW_EVENT_KEY_UP;

  mgr->dispatch((void *) eventData, (enum FwKeyboardCallbackName) state);
}


#endif // _WINDOWS


// End Event Handlers ******************************************

#endif /* __MLE_KEYBOARDEVENT_H */

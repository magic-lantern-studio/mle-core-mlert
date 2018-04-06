

/************************Begin common includes***********************/
#include "fw/titleenv.h"
#include "fw/kbrdevnt.h"
/************************End common includes*************************/


/***************************Begin SGI includes***********************/
#if defined(FW_REHEARSAL) || defined(__sgi)
// Need SGI platform data structure access
#include "fw/sgiplatform.h"
#endif // FW_REHEARSAL or __sgi
/***************************End SGI includes*************************/


/***************************Begin Win32 includes*********************/
#if defined(WIN32)
#include <windows.h>
// Need Win32 platform data structure access
#include "fw/comstg.h"
#endif // WIN32
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
#if defined(FW_REHEARSAL) || defined(__sgi)
  FwSGIPlatformData *platformData = (FwSGIPlatformData *)theTitle->platformData;
#endif // FW_REHEARSAL or __sgi
#if defined(WIN32)
  FwWin32Data *platformData = (FwWin32Data *)theTitle->platformData;
#endif // WIN32
  FW_ASSERT(platformData);

  // Install and register only one keyboard manager (of any type) per title at any one time.
  FW_ASSERT(platformData->keyboardManager == FW_INPUT_DEVICE_MANAGER_NOT_INSTANTIATED);
  FwKeyboardEvent::keyboardManager = this;
  platformData->keyboardManager = FW_INPUT_DEVICE_MANAGER_INSTANTIATED;

#if defined(FW_REHEARSAL) || defined(__sgi)
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
#endif // FW_REHEARSAL or __sgi
  
#if defined(WIN32)
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
#endif // WIN32

  // Initialize callback registry
  keyDownCallback.callbackFn = NULL;
  keyDownCallback.clientData = NULL;
  keyUpCallback.callbackFn = NULL;
  keyUpCallback.clientData = NULL;
  keyCallback.callbackFn = NULL;
  keyCallback.clientData = NULL;

#if defined(WIN32)
  lastEventKeyDown = FALSE;
#endif // WIN32
}

FwKeyboardEvent::~FwKeyboardEvent(void)
{
  // Remove all callbacks
  unregisterAllCB();

#if defined(FW_REHEARSAL) || defined(__sgi)  
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
#endif // FW_REHEARSAL or __sgi

#if defined(WIN32)
  FwWin32Data *platformData = (FwWin32Data *)theTitle->platformData;

  theTitle->eventMgr->uninstallEvent((FwEvent) WM_KEYDOWN);
  theTitle->eventMgr->uninstallEvent((FwEvent) WM_KEYUP);
  theTitle->eventMgr->uninstallEvent((FwEvent) WM_CHAR);
#endif // WIN32

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
#if defined(FW_REHEARSAL) || defined(__sgi)
  FwSGIPlatformData *platformData = (FwSGIPlatformData *)theTitle->platformData;
#endif // FW_REHEARSAL or __sgi
#if defined(WIN32)
  FwWin32Data *platformData = (FwWin32Data *)theTitle->platformData;
#endif // WIN32
  FW_ASSERT(platformData);

  return platformData->keyboardManager;
}

unsigned int
FwKeyboardEvent::getKeySym(void *platformEvent)
{
#if defined(FW_REHEARSAL) || defined(__sgi)
  KeySym keysym;
  XKeyEvent* keyEvent  = (XKeyEvent *) platformEvent;
  XComposeStatus compose;
  char buffer[20];
  int buffersize = 20;
  int count = 0;
  
  count = XLookupString(keyEvent, buffer, buffersize,
			&keysym, &compose);
#endif // FW_REHEARSAL or __sgi

#if defined(WIN32)
#if 0
  unsigned int keysym;
  Win32CallData *data = (Win32CallData *) platformEvent;
  unsigned int scanCode =  _getbits(data->lParam, 23, 8);
  keysym = MapVirtualKey(scanCode, 1); */
#endif // 0

  Win32CallData *data = (Win32CallData *) platformEvent;
  unsigned int keysym = data->wParam;

#endif // WIN32

  return keysym;
}

unsigned int
FwKeyboardEvent::translateToCharacter(void *platformEvent,
				      void *buffer,
				      int bufferSize,
				      FwBoolean ascii)
{
  unsigned int count = 0;

#if defined(FW_REHEARSAL) || defined(__sgi)
  XKeyEvent* keyEvent  = (XKeyEvent *) platformEvent;
  KeySym keysym;
  XComposeStatus compose;
  if (ascii)
    count = XLookupString(keyEvent, (char *)buffer, bufferSize,
			  &keysym, &compose);
  else {
    // xxx unicode not handled
  }
#endif // FW_REHEARSAL or __sgi

#if defined(WIN32)
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
#endif // WIN32

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


#if defined(FW_REHEARSAL) || defined(__sgi)
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
#endif // FW_REHEARSAL or __sgi


#if defined(WIN32)

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


#endif // WIN32


// End Event Handlers ******************************************

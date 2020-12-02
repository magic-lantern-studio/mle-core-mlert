/** @defgroup MleInput Magic Lantern Runtime Engine Input Library API */

/**
 * @file MleKeyboardPolled.cxx
 * @ingroup MleInput
 *
 * @author Mark S. Millard
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2000-2020 Wizzer Works
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

#if defined(MLE_QT)
#include <QtGlobal>
#include <Qt>
#include <QWidget>
#include <QEvent>
#include <QKeyEvent>
#endif /* MLE_QT */

// Include Magic Lantern header files.
#include "mle/mlPlatformData.h"
#include "mle/mlAssert.h"

// Include Magic Lantern Runtime Engine header files.
#include "mle/MleDirector.h"
#include "mle/MleKeyboardPolled.h"
#include "mle/MlePlatformData.h"

#if defined(WIN32)
// Include Magic Lantern event manager.
#include "mle/MleEventDispatcher.h"
#endif /* WIN32 */

//#define MLE_KEY_DEBUG 1

#if defined(WIN32) && defined(MLE_KEY_DEBUG)
#include <windows.h>
#include <stdio.h>
#include <conio.h>

static char pbuf[1000];

static int
printf ( const char * format, ... )
{
    va_list ap;
    va_start ( ap, format );
    _vsnprintf ( pbuf, sizeof(pbuf)-1, format, ap );
    va_end ( ap );
    OutputDebugString ( pbuf );
    return 0;
}
#endif /* WIN32 and MLE_KEY_DEBUG */

#if defined(WIN32)
// Make sure that the registry can be shared if the library is
// included as part of a DLL.
#pragma data_seg( ".GLOBALS" )
#endif /* WIN32 */
// Holds global keyboard manager (polled).
MleKeyboardPolled *MleKeyboardPolled::g_keyboardManager = NULL;
#if defined(WIN32)
#pragma data_seg()
#pragma comment("linker, /section:.GLOBALS,rws")
#endif /* WIN32 */

MleKeyboardPolled::MleKeyboardPolled(void)
{
    // Initialize keys-down symbol table.
    for (int i = 0 ; i < MLE_MAX_NUMBER_OF_SIMULTANEOUS_KEYS_DOWN; i++)
        m_keysDownTable[i] = 0;
  
    // Get the platform data.
#if defined(WIN32)
#ifdef MLE_REHEARSAL
	MleIvPlatformData *platformData = (MleIvPlatformData *)g_theTitle->m_platformData;
#else /* !MLE_REHEARSAL */
    MleWin32PlatformData *platformData = (MleWin32PlatformData *)g_theTitle->m_platformData;
#endif /* !MLE_REHEARSAL */
#endif /* WIN32 */
#if defined(__linux__)
#ifdef MLE_REHEARSAL
	MleIvPlatformData *platformData = (MleIvPlatformData *)g_theTitle->m_platformData;
#else
#if defined(MLE_XT)
	MleXtPlatformData *platformData = (MleXtPlatformData *)g_theTitle->m_platformData;
#else
	/* Default Unknown. */
	MlePlatformData *platformData = (MlePlatformData *)g_theTitle->m_platformData;
#endif /* MLE_XT */
#endif /* MLE_REHEARSAL */
#endif /* __linux__ */
    MLE_ASSERT(platformData);

    // Install and register only one keyboard manager (of any type) per title at any one time.
    //MLE_ASSERT(platformData->hasKeyboardManager == MLE_INPUT_DEVICE_MANAGER_NOT_INSTANTIATED);
	MLE_ASSERT(! platformData->hasKeyboardManager());
    MleKeyboardPolled::g_keyboardManager = this;
    platformData->setKeyboardManager(MLE_INPUT_DEVICE_MANAGER_INSTANTIATED);

#if defined(__linux__)
#if defined(MLE_XT)

    // Get the widget for the window's rendering area.
    MLE_ASSERT(platformData->m_widget);

    // Add focus event handler.
    if (platformData->m_focusEventHandlerRefCount == 0)
	{
		XtAddEventHandler(platformData->m_widget,
			(FocusChangeMask | EnterWindowMask | LeaveWindowMask),
			False,
			(XtEventHandler) MleFocusChange,
			NULL);

		platformData->m_inFocus = FALSE; // Assume not now in focus.
	}
    platformData->m_focusEventHandlerRefCount += 1;

    // Add keyboard event handler (highest possible priority).
    XtInsertEventHandler(platformData->m_widget,
		(KeyPressMask | KeyReleaseMask),
		False,
		(XtEventHandler) MleKeyboardPolled::MleKeyboardPolledEventHandler,
		this,
		XtListHead);
  
    platformData->m_keyboardActive = FALSE;

#endif /* MLE_XT */
#ifdef MLE_QT
    // Add keyboard event handler callbacks.
    g_theTitle->m_theEventMgr->installEventCB(
        (MleEvent) Qt::Key_Down,
        (MleCallback) MleKeyboardPolled::QT_KEYPRESS_EventHandler,
        this);
    g_theTitle->m_theEventMgr->installEventCB(
        (MleEvent) Qt::Key_Up,
        (MleCallback) MleKeyboardPolled::QT_KEYRELEASE_EventHandler,
        this);
#endif /* MLE_QT */
#endif /* __linux__ */

#if defined(WIN32)

// Todo: Not sure how to handle these event callbacks in rehearsal
// mode, ala Inventor Stage.
//#ifndef MLE_REHEARSAL
    // Add keyboard message handler callbacks.
    g_theTitle->m_theEventMgr->installEventCB(
	    (MleEvent) WM_KEYDOWN,
        (MleCallback) MleKeyboardPolled::WM_KEYDOWN_EventHandler,
        this);
    g_theTitle->m_theEventMgr->installEventCB(
	    (MleEvent) WM_KEYUP,
        (MleCallback) MleKeyboardPolled::WM_KEYUP_EventHandler,
        this);
    g_theTitle->m_theEventMgr->installEventCB(
	    (MleEvent) WM_CHAR,
        (MleCallback) MleKeyboardPolled::WM_CHAR_EventHandler,
        this);
//#endif /* !MLE_REHEARSAL */

#endif /* WIN32 */

}

MleKeyboardPolled::~MleKeyboardPolled(void)
{
    // Get platform-independent data.
#if defined(WIN32)
#ifdef MLE_REHEARSAL
	MleIvPlatformData *platformData = (MleIvPlatformData *)g_theTitle->m_platformData;
#else /* !MLE_REHEARSAL */
    MleWin32PlatformData *platformData = (MleWin32PlatformData *)g_theTitle->m_platformData;
#endif /* !MLE_REHEARSAL */
#endif /* WIN32 */
#if defined(__linux__)
#ifdef MLE_REHEARSAL
	MleIvPlatformData *platformData = (MleIvPlatformData *)g_theTitle->m_platformData;
#else
#if defined(MLE_XT)
	MleXtPlatformData *platformData = (MleXtPlatformData *)g_theTitle->m_platformData;
#else
	MlePlatformData *platformData = (MlePlatformData *)g_theTitle->m_platformData;
#endif /* MLE_XT */
#endif /* MLE_REHEARSAL */
#endif /* __linux__ */

#if defined(__linux__)
#if defined(MLE_XT)
    // Remove Xt event handlers

    // Decrease reference count of focus handlers.
    platformData->m_focusEventHandlerRefCount -= 1;

    // Remove focus event handlers if I am only user.
    if (platformData->m_focusEventHandlerRefCount == 0)
        XtRemoveEventHandler(platformData->m_widget,
		    (FocusChangeMask | EnterWindowMask | LeaveWindowMask),
			False,
			(XtEventHandler) MleFocusChange,
			NULL);

    // Remove keyboard-specific event handlers.
    XtRemoveEventHandler(platformData->m_widget,
	    (KeyPressMask | KeyReleaseMask),
		False,
		(XtEventHandler) MleKeyboardPolled::MleKeyboardPolledEventHandler,
		this);

#endif /* MLE_XT */
#ifdef MLE_QT
    // Remove Qt evvent handlers.
    g_theTitle->m_theEventMgr->uninstallEvent((MleEvent) Qt::Key_Down);
    g_theTitle->m_theEventMgr->uninstallEvent((MleEvent) Qt::Key_Up);
#endif /* MLE_QT */
#endif /* __linux__ */

#if defined(WIN32)

// Todo: Not sure how to handle these event callbacks in rehearsal
// mode, ala Inventor Stage.
//#ifndef MLE_REHEARSAL
    // Remove Win32 message handlers.
    g_theTitle->m_theEventMgr->uninstallEvent((MleEvent) WM_KEYDOWN);
    g_theTitle->m_theEventMgr->uninstallEvent((MleEvent) WM_KEYUP);
    g_theTitle->m_theEventMgr->uninstallEvent((MleEvent) WM_CHAR);
//#endif /* !MLE_REHEARSAL */

#endif /* WIN32 */

    // Release keyboard manager resource.
    platformData->setKeyboardManager(MLE_INPUT_DEVICE_MANAGER_NOT_INSTANTIATED);
    MleKeyboardPolled::g_keyboardManager = NULL;
}

MlBoolean
MleKeyboardPolled::keyboardIsActive(void)
{
    MlBoolean result = FALSE;

#if defined(__linux__)
#ifdef MLE_REHEARSAL
    MleIvPlatformData *platformData = (MleIvPlatformData *)g_theTitle->m_platformData;
    result = platformData->m_keyboardActive;
#else
    MlePlatformData *platformData = (MlePlatformData *)g_theTitle->m_platformData;
#if defined(MLE_XT)
    MleXtPlatformData *platformData = (MleXtPlatformData *)g_theTitle->m_platformData;
#endif /* MLE_XT */
#endif /* MLE_REHEARSAL */
#endif /* __linux__ */

#if defined(WIN32)
    HWND win = GetFocus();

    if (win)
        result = TRUE;
#endif /* WIN32 */

    return result;
}

MlBoolean
MleKeyboardPolled::keyDown(unsigned int keysym)
{
    MlBoolean result = FALSE;

    for (int i = 0 ; i < MLE_MAX_NUMBER_OF_SIMULTANEOUS_KEYS_DOWN ; i++)
    {
        if (m_keysDownTable[i] == keysym)
		{
            result = TRUE;
            break;
		}
    }

    return (result);
}

MlBoolean
MleKeyboardPolled::getInputString(
	char* /* buffer */,
	int /* bufferSize */,
	int /* eoc */,
	int /* timeout */)
{
    MLE_ASSERT(0);
    // Todo: implement this.

    return FALSE;
}

void
MleKeyboardPolled::keyWentDown(unsigned int keysym)
{
    MlBoolean alreadyThere = FALSE;
    int i;

#if defined(MLE_KEY_DEBUG)
    printf("MleKeyboardPolled: KEY DOWN: 0x%x\n", keysym);
#endif /* MLE_KEY_DEBUG */

    for (i = 0 ; i < MLE_MAX_NUMBER_OF_SIMULTANEOUS_KEYS_DOWN ; i++)
	{
        if (m_keysDownTable[i] == keysym)
		{
#if defined(MLE_KEY_DEBUG)
            printf("MleKeyboardPolled: KEY ALREADY THERE: 0x%x\n", keysym);
#endif /* MLE_KEY_DEBUG */
           alreadyThere = TRUE;
		}
	}

    if (alreadyThere == FALSE)
	{
		for (i = 0 ; i < MLE_MAX_NUMBER_OF_SIMULTANEOUS_KEYS_DOWN ; i++)
		{
			if (m_keysDownTable[i] == 0)
			{
#if defined(MLE_KEY_DEBUG)
                printf("MleKeyboardPolled: KEY STORED: 0x%x\n", keysym);
#endif /* MLE_KEY_DEBUG */
				m_keysDownTable[i] = keysym;
				break;
			}
		}
	}

	if ((alreadyThere == FALSE) &
        (i == MLE_MAX_NUMBER_OF_SIMULTANEOUS_KEYS_DOWN))
	{
#if defined(MLE_KEY_DEBUG)
        printf("MleKeyboardPolled: KEY STACK OVERFLOW: 0x%x\n", keysym);
#endif /* MLE_KEY_DEBUG */
		m_keysDownTable[0] = keysym;
		for (int i = 1 ; i < MLE_MAX_NUMBER_OF_SIMULTANEOUS_KEYS_DOWN ; i++)
			m_keysDownTable[i] = 0;
	}
}

void
MleKeyboardPolled::keyWentUp(unsigned int keysym)
{
#if defined(MLE_KEY_DEBUG)
    printf("MleKeyboardPolled: KEY UP: 0x%x\n", keysym);
#endif /* MLE_KEY_DEBUG */
  
    for (int i= 0; i < MLE_MAX_NUMBER_OF_SIMULTANEOUS_KEYS_DOWN ; i++)
	{
        if (m_keysDownTable[i] == keysym)
		{
#if defined(MLE_KEY_DEBUG)
            printf("MleKeyboardPolled: KEY REMOVED: 0x%x\n", keysym);
#endif /* MLE_KEY_DEBUG */
			m_keysDownTable[i] = 0;
			break;
		}
	}
}

void *
MleKeyboardPolled::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleKeyboardPolled::operator delete(void *p)
{
	mlFree(p);
}

#if defined(__linux__)
#if defined(MLE_XT)
void 
MleKeyboardPolled::MleFocusChange(
	Widget   /* widget */,
	XPointer /* clientData */,
	XEvent *event)
{
#if defined(__linux__)
#ifdef MLE_REHEARSAL
	MleIvPlatformData *platformData = (MleIvPlatformData *)g_theTitle->m_platformData;
#else
	MleXtPlatformData *platformData = (MleXtPlatformData *)g_theTitle->m_platformData;
#endif /* MLE_REHEARSAL */
#endif /* __linux__ */

    switch(event->type)
	{
		case FocusIn:
			platformData->m_inFocus = TRUE;
			platformData->m_keyboardActive = TRUE;
			break;
		case FocusOut:
			platformData->m_inFocus = FALSE;
			platformData->m_keyboardActive = FALSE;
			break;
		case EnterNotify:
			if (event->xcrossing.focus)
				platformData->m_keyboardActive = TRUE;
			else
				platformData->m_keyboardActive = FALSE;
			break;
		case LeaveNotify:
			if (platformData->m_inFocus)
				platformData->m_keyboardActive = FALSE;
			else
				platformData->m_keyboardActive = TRUE;
			break;
	}
}

void
MleKeyboardPolled::MleKeyboardPolledEventHandler(
	Widget /* widget */,
	XPointer m,
	XEvent *event)
{
    XKeyEvent *keyEvent = (XKeyEvent *) event;
    MleKeyboardPolled *mgr = (MleKeyboardPolled *) m;
    KeySym keysym;
    XComposeStatus compose;
    char buffer[20];
    int buffersize = 20;
    int count = 0;
  
    switch (event->type)
	{
		case KeyPress:
			count = XLookupString(keyEvent, buffer, buffersize,
			    &keysym, &compose);
			buffer[count] = 0;
			mgr->keyWentDown(keysym);
			break;
		case KeyRelease:
			count = XLookupString(keyEvent, buffer, buffersize,
			    &keysym, &compose);
			buffer[count] = 0;
			mgr->keyWentUp(keysym);
			break;
	}
}
#endif /* MLE_XT */
#ifdef MLE_QT
void
MleKeyboardPolled::QT_KEYPRESS_EventHandler(MleEvent event,
        void *eventData, void *clientData)
{
    QEvent *data = (QEvent *) eventData;
    MleKeyboardPolled *mgr = (MleKeyboardPolled *) clientData;

    if (data->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = (QKeyEvent *)data;
        unsigned int keysym = keyEvent->nativeVirtualKey();

#if defined(MLE_KEY_DEBUG)
        const char *text = keyEvent->text().toStdString().c_str();
        printf("MleKeyboardPolled: KeyPress Event: %s: key = 0x%x, vkey=0x%x\n",
               text, keyEvent->key(), keyEvent->nativeVirtualKey());
#endif /* MLE_KEY_DEBUG */

        // Filter which keys to capture .
        //if (! (((keysym >= MLE_KEY_0) && (keysym <= MLE_KEY_z))
        if ((((keysym >= MLE_KEY_0) && (keysym <= MLE_KEY_z))
            || (keysym == MLE_KEY_BackSpace)
            || (keysym == MLE_KEY_Return)
            || (keysym == MLE_KEY_Escape)
            || (keysym == MLE_KEY_Tab)))
        {
            mgr->keyWentDown(keysym);
        }
    }
}

void
MleKeyboardPolled::QT_KEYRELEASE_EventHandler(MleEvent event,
        void *eventData, void *clientData)
{
    QEvent *data = (QEvent *) eventData;
    MleKeyboardPolled *mgr = (MleKeyboardPolled *) clientData;

    if (data->type() == QEvent::KeyRelease) {
        QKeyEvent *keyEvent = (QKeyEvent *)data;
        unsigned int keysym = keyEvent->nativeVirtualKey();

        mgr->keyWentUp(keysym);
    }
}
#endif /* MLE_QT */
#endif /* __linux__ */

#if defined(WIN32)
void
MleKeyboardPolled::WM_KEYDOWN_EventHandler(
	MleEvent /* event */,
	void* eventData,
	void* clientData)
{
    Win32CallData *data = (Win32CallData *) eventData;
    MleKeyboardPolled *mgr = (MleKeyboardPolled *) clientData;
    unsigned int keysym = data->wParam;

    if (! (((keysym >= MLE_KEY_0) && (keysym <= MLE_KEY_z))
	    || (keysym == MLE_KEY_BackSpace)
	    || (keysym == MLE_KEY_Return)
	    || (keysym == MLE_KEY_Escape)
	    || (keysym == MLE_KEY_Tab)))
	{
        mgr->keyWentDown(keysym);
	}
}

void
MleKeyboardPolled::WM_KEYUP_EventHandler(
	MleEvent /* event */,
	void* eventData,
	void* clientData)
{
#define WINDOWS_CAPS_LOCK_KEYSYM 0x14
#define _SHIFT_KEY_UP() ((GetKeyState(VK_SHIFT) & 0x1000) == 0x0000)

    Win32CallData *data = (Win32CallData *) eventData;
    MleKeyboardPolled *mgr = (MleKeyboardPolled *) clientData;
    unsigned int keysym = data->wParam;
    SHORT capsLockUp = !(GetKeyState(WINDOWS_CAPS_LOCK_KEYSYM) == 1);

    if (((keysym >= MLE_KEY_A) && (keysym <= MLE_KEY_Z)) &&
        (_SHIFT_KEY_UP() && capsLockUp))
	{
        keysym += 32; // shift key is up -> lower-case character
	}
    mgr->keyWentUp(keysym);
}

void
MleKeyboardPolled::WM_CHAR_EventHandler(
	MleEvent /* event */,
	void* eventData,
	void* clientData)
{
    Win32CallData *data = (Win32CallData *) eventData;
    MleKeyboardPolled *mgr = (MleKeyboardPolled *) clientData;

    mgr->keyWentDown(data->wParam);
}
#endif /* WIN32 */

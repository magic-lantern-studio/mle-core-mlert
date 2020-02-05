/** @defgroup MleRteRehearsal Magic Lantern Runtime Engine Rehearsal Library API */

/**
 * @file mlPlatformData.h
 * @ingroup MleRteRehearsal
 *
 * @author Mark S. Millard
 * @date May 1, 2003
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2017-2020 Wizzer Works
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

#ifndef __MLE_REHEARSAL_PLATFORMDATA_H_
#define __MLE_REHEARSAL_PLATFORMDATA_H_

// Include system header files.
#if defined(WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#if defined(__linux__)
#if defined(Q_OS_LINIX)
#include <QWidget>
#else
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#endif
#endif

// Include Magic Lantern types.
#include "mle/mlTypes.h"

// Include Magic Lantern Runtime Engine header files.
#include "mle/MlePlatformData.h"


/**
 * Inventor-specific platform data.
 *
 * This data structure is the Inventor-specific platform data
 * used in the m_platformData slot of the MleDirector.
 */
class MleIvPlatformData : public MlePlatformData
{
  public:

#if defined(__linux__)
#if defined(Q_OS_LINUX)
    /* Qt platform */
    QWidget *m_widget;                /* Qt widget for player window's render area. */
#else
    /* Xt platform */
    Widget m_widget;                  /* Xt widget for player window's render area. */
    XtAppContext m_appContext;        /* Application context for player window. */
#endif
#endif
#if defined(WIN32)
	HWND m_widget;                    /* Windows widget for player window's render area. */
#endif
    int m_focusEventHandlerRefCount;  /* Input event mgmt: Count of managers relying on focus events. */
    MlBoolean m_inFocus;              /* Input event mgmt: Is widget in focus? */
    MlBoolean m_keyboardActive;       /* Input event mgmt: Is keyboard active? */

    void setKeyboardManager(MlBoolean flag)
    { m_keyboardManager = flag; }

    MlBoolean hasKeyboardManager()
    { return m_keyboardManager; }

    void setMouseManager(MlBoolean flag)
    { m_mouseManager = flag; }

    MlBoolean hasMouseManager()
    { return m_mouseManager; }

    void setJoystickManager(MlBoolean flag)
    { m_joystickManager = flag; }

    MlBoolean hasJoystickManager()
    { return m_joystickManager; }

  private:

    MlBoolean m_keyboardManager;
    MlBoolean m_mouseManager;
    MlBoolean m_joystickManager;

};


/**
 * Definitions for use on keyboardManager, mouseManager,
 * and joystickManager Booleans
 */
#define MLE_INPUT_DEVICE_MANAGER_NOT_INSTANTIATED   FALSE
#define MLE_INPUT_DEVICE_MANAGER_INSTANTIATED       TRUE

#endif /* __MLE_REHEARSAL_PLATFORMDATA_H_ */

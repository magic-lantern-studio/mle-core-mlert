/** @defgroup MleRteWin32 Magic Lantern Runtime Engine Win32 Library API */

/**
 * @file mlPlatformData.h
 * @ingroup MleRteWin32
 *
 * @author Mark S. Millard
 * @date May 1, 2003
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2017 Wizzer Works
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

#ifndef __MLE_WIN32_PLATFORMDATA_H_
#define __MLE_WIN32_PLATFORMDATA_H_

// Include system header files.
#include <windows.h>
#include <windowsx.h>

// Include Magic Lantern types.
#include "mle/mlTypes.h"

// Include Magic Lantern Runtime Engine header files.
#include "mle/MlePlatformData.h"

/**
 * This structure is what goes into the m_platformData slot
 * of the MleDirector structure for the Win32 API.
 */
class MleWin32PlatformData : public MlePlatformData
{
  public:

    HINSTANCE m_appInstance;       /* Passed in from WinMain   */
    char      m_appName[9];        /* Derived from appInstance */
    char      m_appTitle[41];      /* Derived from appInstance */
    char      m_playprint[256];    /* Derived from appInstance */
    DWORD     m_winStyle;          /* Initialized in main      */
    int       m_defaultWinWidth;   /* Initialized in main      */
    int       m_defaultWinHeight;  /* Initialized in main      */

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

//
// Constants for MleWin32Data initialization
//
#define IDS_APPNAME         1
#define IDS_DESCRIPTION     2
#define IDS_PLAYPRINT       3
#define DEFAULT_VIEW_WIDTH  320
#define DEFAULT_VIEW_HEIGHT 240

// Definitions for use on keyboardManager, mouseManager,
// and joystickManager booleans.
#define MLE_INPUT_DEVICE_MANAGER_NOT_INSTANTIATED FALSE
#define MLE_INPUT_DEVICE_MANAGER_INSTANTIATED     TRUE


#endif /* __MLE_WIN32_PLATFORMDATA_H_ */

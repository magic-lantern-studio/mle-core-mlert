/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MlePlatformData.h
 * @ingroup MleFoundation
 *
 * @author Mark S. Millard
 * @created June 7, 2017
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2017-2018 Wizzer Works
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

#ifndef __MLE_PLATFORMDATA_H_
#define __MLE_PLATFORMDATA_H_

// Include Magic Lantern types.
#include "mle/mlTypes.h"

// Include Magic Lantern Runtime Engine header files.
#include "mle/MleRuntime.h"

class MLE_RUNTIME_API MlePlatformData
{
  public:

    /**
     * Default constructor.
     */
    MlePlatformData() {};

    /**
     * Destructor.
     */
    virtual ~MlePlatformData() {};

    /**
     * @brief Test for registered keyboard manager.
     *
     * @return ML_TRUE will be returned if the platform supports
     * a keyboard. Otherwise, ML_FALSE will be returned.
     */
    virtual MlBoolean hasKeyboardManager() { return ML_FALSE; };

    /**
     * @brief Set flag indicating that a keyboad manager is present.
     *
     * @param flag Use ML_TRUE to indicate that the platform
     * supports a keyboard. Use ML_FALSE to indicate that the platform
     * does not support a keyboard.
     */
    virtual void setKeyboardManager(MlBoolean flag) {};

    /**
     * @brief Test for registered mouse manager.
     *
     * @return ML_TRUE will be returned if the platform supports
     * a mouse. Otherwise, ML_FALSE will be returned.
     */
    virtual MlBoolean hasMouseManager() { return ML_FALSE; };

    /**
     * @brief Set flag indicating that a mouse manager is present.
     *
     * @param flag Use ML_TRUE to indicate that the platform
     * supports a mouse. Use ML_FALSE to indicate that the platform
     * does not support a mouse.
     */
    virtual void setMouseManager(MlBoolean flag) {};

    /**
     * @brief Test for registered joystick manager.
     *
     * @return ML_TRUE will be returned if the platform supports
     * a joystick. Otherwise, ML_FALSE will be returned.
     */
    virtual MlBoolean hasJoystickManager() { return ML_FALSE; };

    /**
     * @brief Set flag indicating that a joystick manager is present.
     *
     * @param flag Use ML_TRUE to indicate that the platform
     * supports a joystick. Use ML_FALSE to indicate that the platform
     * does not support a joystick.
     */
    virtual void setJoystickManager(MlBoolean flag) {};
};

#endif /** __MLE_PLATFORMDATA_H_ */

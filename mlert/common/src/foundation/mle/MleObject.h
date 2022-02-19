/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MleObject.h
 * @ingroup MleFoundation
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015-2022 Wizzer Works
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

#ifndef __MLE_OBJECT_H_
#define __MLE_OBJECT_H_

#ifdef MLE_REHEARSAL
#include "mle/MleMonitor.h"
#endif

// Include Magic Lanter header files.
#include "mle/MleRuntime.h"

/**
 * The class is a base class for Magic Lantern classes that provide common
 * functional behavior (i.e. properties).
 *
 * @see MleActor
 * @see MleSet
 */
class MLE_RUNTIME_API MleObject
#ifdef MLE_REHEARSAL
  : public MleMonitor // This enables notification on deletion.
#endif
{
  public:

    /**
     * @brief Default property get accessor.
     *
     * Does nothing since there aren't any default properties to get.
     *
     * @param object A pointer to the Object instance.
     * @param name A pointer to the name of the property to get.
     * @param value An output parameter for returning the property value.
     */
    static void getProperty(MleObject * /*object*/, const char * /*name*/, unsigned char **value)
    { /* There are no global properties to retrieve. */ value = 0; }

    /**
      * @brief Default property set accessor.
      *
      * Does nothing since there aren't any default properties to set.
      *
      * @param object A pointer to the Object instance.
      * @param name A pointer to the name of the property to set.
      * @param value An input parameter for setting the property value.
      */
    static void setProperty(MleObject * /*object*/, const char * /*name*/, unsigned char * /*value*/)
	{ /* There are no global properties to set. */ }

    /**
     * @brief Default constructor.
     *
     * This is the base object class constructor.
     */
    MleObject(void);
    
    /**
     * @brief Destructor.
     *
     * This is the base object class destructor.
     */
    virtual ~MleObject();
};

#endif /* __MLE_OBJECT_H_ */

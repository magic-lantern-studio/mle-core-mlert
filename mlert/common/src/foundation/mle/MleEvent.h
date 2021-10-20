/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundatoin Library API */

/**
 * @file MleEvent.h
 * @ingroup MleFoundation
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2017-2021 Wizzer Works
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

#ifndef __MLE_EVENT_H_
#define __MLE_EVENT_H_

// Include Magic Lantern header files.
#include "mle/mlTypes.h"

/**
 * Event value definition.
 */
typedef signed long MleEvent;

/**
 * Make a composite identifier based on the specified group
 * and event ids.
 *
 * @param group The group that the event belongs to.
 * @param id The unique identifier for the event.
 *
 * @return The composite event identifier is returned.
 */
int mleMakeId(short group, short id);

/**
 * Get the group id for the specified composite identifier.
 * 
 * @param cid The composite event identifier.
 * 
 * @return The group that the event belongs to is returned.
 */
short mleGetGroupId(int cid);

/**
 * Get the event id for the specified composite identifier.
 * 
 * @param cid The composite event identifier.
 * 
 * @return The event id is returned.
 */
short mleGetEventId(int cid);

#endif /* __MLE_EVENT_H_ */

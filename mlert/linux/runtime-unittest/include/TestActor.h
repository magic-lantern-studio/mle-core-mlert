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

#ifndef __TESTACTOR_H_
#define __TESTACTOR_H_

// Include system header files.
#include <stddef.h>
#include <string.h>

// Include Magic Lantern header files.
#include "mle/mlArray.h"
#include "mle/mlMalloc.h"
#include "mle/MleObject.h"
#include "mle/MleActor.h"

class TestActor : public MleActor
{
    MLE_ACTOR_HEADER(TestActor);

  public:

    static void getProperty(MleObject *object, const char *name, unsigned char **value); // value is an output parameter.
    static void setProperty(MleObject *object, const char *name, unsigned char *value); // value is an input parameter.

    MLE_ACTOR_PROPERTY(int, id, getIdProperty, setIdProperty)
    MLE_ACTOR_DYNAMIC_PROPERTY(char *, text, getTextProperty, setTextProperty)
    MLE_ACTOR_ARRAY_PROPERTY(int, age, getAgeProperty, setAgeProperty)

  public:

    TestActor();
    virtual ~TestActor();
};

#endif /* __TESTACTOR_H_ */

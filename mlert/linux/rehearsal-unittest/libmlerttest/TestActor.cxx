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

// Include system header files.
#include <iostream>

// Include Magic Lantern header files.

// Include Magic Lantern Runtime unit test header files.
#include "TestActor.h"

using namespace std;


MLE_ACTOR_SOURCE(TestActor,MleActor)


TestActor::TestActor()
  : id(0), text(NULL)
{
	// Initialize position Property.
	position[0] = 100.0;
	position[1] = 50.0;
	position[2] = 250.0;

	// Initialize orientation Property.
	orientation = MlRotation::identity();

	// Initialize farClipping Property.
	farClipping = 1.0;

	// Initialize nearClipping Property.
	nearClipping = -1.0;

	// Initialize viewHeight Property.
	viewHeight = 75.0;

	// Initialize cameraType Property.
	cameraType = 0;

	// Initialize aspectRatio Property.
	aspectRatio = 0.75;
}


TestActor::~TestActor()
{}


void TestActor::initClass()
{
	// Register the actor class.
	mleRegisterActorClass(TestActor,MleActor);

	// Register properties.
	mleRegisterActorMember(TestActor,position,MlVector3);
	mleRegisterActorMember(TestActor,orientation,MlRotation);
	mleRegisterActorMember(TestActor,nearClipping,MlScalar);
	mleRegisterActorMember(TestActor,farClipping,MlScalar);
	mleRegisterActorMember(TestActor,aspectRatio,MlScalar);
	mleRegisterActorMember(TestActor,cameraType,int);
	mleRegisterActorMember(TestActor,viewHeight,MlScalar);

	// Mark all the properties that belongs to the "transform" property
	// data set.
	mleRegisterActorMemberDataset(TestActor,position,MLE_PROP_DATASET_TRANSFORM);
	mleRegisterActorMemberDataset(TestActor,orientation,MLE_PROP_DATASET_TRANSFORM);
}


void
TestActor:: getProperty(MleObject *object, const char *name, unsigned char **value)
{
    //cout << "Getting TestActor property " << name << "." << endl;
    if (strcmp("id",name) == 0)
    {
        int id = ((TestActor *)object)->getIdProperty();
        *((int *)value) = id;
    } else if (strcmp("text",name) == 0)
    {
        const char *text = ((TestActor *)object)->getTextProperty();
        *value = (unsigned char *)text;
    } else if (strcmp("age",name) == 0)
    {
        const MleArray<int> *age;
        age = ((TestActor *)object)->getAgeProperty();
        *((MleArray<int> *)value) = *age;
    } else if (strcmp("position",name) == 0)
    {
    	MlVector3 position = ((TestActor *)object)->getPositionProperty();
    	*((MlVector3 *)value) = position;
    } else if (strcmp("orientation",name) == 0)
    {
    	MlRotation orientation = ((TestActor *)object)->getOrientationProperty();
    	*((MlRotation *)value) = orientation;
    } else if (strcmp("nearClipping",name) == 0)
    {
    	MlScalar nearClipping = ((TestActor *)object)->getNearClippingProperty();
    	*((MlScalar *)value) = nearClipping;
    } else if (strcmp("farClipping",name) == 0)
    {
    	MlScalar farClipping = ((TestActor *)object)->getFarClippingProperty();
    	*((MlScalar *)value) = farClipping;
    } else if (strcmp("aspectRatio",name) == 0)
    {
    	MlScalar aspectRatio = ((TestActor *)object)->getAspectRatioProperty();
    	*((MlScalar *)value) = aspectRatio;
    } else if (strcmp("cameraType",name) == 0)
    {
    	int cameraType = ((TestActor *)object)->getCameraTypeProperty();
    	*((int *)value) = cameraType;
    } else if (strcmp("viewHeight",name) == 0)
    {
    	MlScalar viewHeight = ((TestActor *)object)->getViewHeightProperty();
    	*((MlScalar *)value) = viewHeight;
    } else
    {
        // TBD: log warning.
        cout << "***** ERROR: Unknown TestActor property: " << name << endl;
    }
}


void
TestActor::setProperty(MleObject *object, const char *name, unsigned char *value)
{
    //cout << "Setting TestActor property " << name << "." << endl;
    if (strcmp("id",name) == 0)
    {
        ((TestActor *)object)->setIdProperty(*((int *)value));
    } else if (strcmp("text",name) == 0)
    {
        size_t size = strlen((const char *)value) + 1;
        ((TestActor *)object)->setTextProperty((const char *)value,size);
    } else if (strcmp("age",name) == 0)
    {
        ((TestActor *)object)->setAgeProperty(*((MleArray<int> *)value));
    } else if (strcmp("position",name) == 0)
    {
        ((TestActor *)object)->setPositionProperty(*((MlVector3 *)value));
    } else if (strcmp("orientation",name) == 0)
    {
        ((TestActor *)object)->setOrientationProperty(*((MlRotation *)value));
    } else if (strcmp("nearClipping",name) == 0)
    {
        ((TestActor *)object)->setNearClippingProperty(*((MlScalar *)value));
    } else if (strcmp("farClipping",name) == 0)
    {
    	((TestActor *)object)->setFarClippingProperty(*((MlScalar *)value));
    } else if (strcmp("aspectRatio",name) == 0)
    {
    	((TestActor *)object)->setAspectRatioProperty(*((MlScalar *)value));
    } else if (strcmp("cameraType",name) == 0)
    {
        ((TestActor *)object)->setCameraTypeProperty(*((int *)value));
    } else if (strcmp("viewHeight",name) == 0)
    {
    	((TestActor *)object)->setViewHeightProperty(*((MlScalar *)value));
    } else
    {
        // TBD: log warning.
        cout << "***** ERROR: Unknown TestActor property: " << name << endl;
    }
}

// COPYRTIGH_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2016 - 2018 Wizzer Works
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

// Include Google Test header files.
#include "gtest/gtest.h"

// Include Magic Lantern header files.
#include "mle/MleActor.h"
#include "mle/MleRole.h"
#include "mle/MleTables.h"

// Include Magic Lantern Runtime unit test header files.
#include "TestActor.h"

extern MleActor *_mlCreateTestActor(void);

using namespace std;


TEST(MleActorTest, DefaultConstructor) {
    // This test is named "DefaultConstructor", and belongs to the "MleActorTest"
    // test case.

	MleActor *actor = new MleActor();

    EXPECT_TRUE(actor != NULL);
}


TEST(MleActorTest, Create) {
    // This test is named "DefaultConstructor", and belongs to the "Create"
    // test case.

	MleActor *actor = _mlCreateTestActor();

    EXPECT_TRUE(actor != NULL);
}


TEST(MleActorTest, Destructor) {
    // This test is named "Destructor", and belongs to the "MleActorTest"
    // test case.

	MleActor *actor = new MleActor();

    EXPECT_TRUE(actor != NULL);

    delete actor;
}


TEST(MleActorTest, AttachRole) {
    // This test is named "AttachRole", and belongs to the "MleActorTest"
    // test case.

	MleActor *actor = new MleActor();
	MleRole *role = new MleRole(actor);

    EXPECT_TRUE(actor != NULL);
    EXPECT_TRUE(role != NULL);

    // actor->attachRole(role) is called in MleRole constructor above.
    MleRole *tRole = actor->getRole();

    EXPECT_EQ(role, tRole);
}


TEST(MleActorTest, TestActorProperties) {
    // This test is named "TestActorProperties", and belongs to the "MleActorTest"
    // test case.

	TestActor *tActor = new TestActor();
    ASSERT_TRUE(tActor != NULL);

    // Testing for property types.
    EXPECT_TRUE(tActor->idIsOfType("int"));
    EXPECT_TRUE(tActor->textIsOfType("char *"));
    EXPECT_TRUE(tActor->ageIsOfArrayType("int"));
    EXPECT_FALSE(tActor->idIsOfType("bool"));

    // Testing id property.
    tActor->setIdProperty(5);
    //cout << "id Property value: " << tActor->getIdProperty() << endl;
    EXPECT_EQ(5,tActor->id);
    EXPECT_EQ(5,tActor->getIdProperty());

    // Testing text dynamic property.
    tActor->setTextProperty("Hello World!", 13);
    //cout << "text Property value: " << tActor->getTextProperty() << endl;
    EXPECT_STREQ("Hello World!",tActor->text);
    EXPECT_STREQ("Hello World!",tActor->getTextProperty());

    // Testing age array property.
    MleArray<int> birthdays(4);
    birthdays[0] = 15;
    birthdays[1] = 19;
    birthdays[2] = 51;
    birthdays[3] = 57;
    //cout << "Input ages before: " << birthdays << endl;
    tActor->setAgeProperty(birthdays);
    //cout << "age Property value: " << tActor->age << endl;
    //cout << "age Property array: " << *tActor->getAgeProperty() << endl;
    EXPECT_EQ(15,tActor->age[0]);
    EXPECT_EQ(19,tActor->age[1]);
    EXPECT_EQ(51,tActor->age[2]);
    EXPECT_EQ(57,tActor->age[3]);
    MleArray<int> *ages = (MleArray<int> *)tActor->getAgeProperty();
    int bday = (*ages)[0];
    EXPECT_EQ(15,bday);
    bday = (*ages)[1];
    EXPECT_EQ(19,bday);
    bday = (*ages)[2];
    EXPECT_EQ(51,bday);
    bday = (*ages)[3];
    EXPECT_EQ(57,bday);

    EXPECT_EQ(4,ages->size());

    // Testing position property.
    EXPECT_EQ(100.0,tActor->position[0]);
    EXPECT_EQ(50.0,tActor->position[1]);
    EXPECT_EQ(250.0,tActor->position[2]);

    // Test global setter.
    MlVector3 newPosition(0.0, 0.0, 1.0);
    TestActor::setProperty(tActor, "position", (unsigned char *)&newPosition);
    // Test direct access to member.
    EXPECT_EQ(0.0,tActor->position[0]);
    EXPECT_EQ(0.0,tActor->position[1]);
    EXPECT_EQ(1.0,tActor->position[2]);
    // Test instance getter.
    MlVector3 positionResult = tActor->getPositionProperty();
    EXPECT_EQ(0.0,positionResult[0]);
    EXPECT_EQ(0.0,positionResult[1]);
    EXPECT_EQ(1.0,positionResult[2]);
    // Test instance setter.
    newPosition[0] = 1.0;
    tActor->setPositionProperty(newPosition);
    EXPECT_EQ(1.0,tActor->position[0]);
    EXPECT_EQ(0.0,tActor->position[1]);
    EXPECT_EQ(1.0,tActor->position[2]);
    // Test global getter.
    TestActor::getProperty(tActor, "position", (unsigned char **)&positionResult);
    EXPECT_EQ(1.0,positionResult[0]);
    EXPECT_EQ(0.0,positionResult[1]);
    EXPECT_EQ(1.0,positionResult[2]);

    // Testing orientation property.
    EXPECT_EQ(ML_SCALAR_ZERO,tActor->orientation[0]);
    EXPECT_EQ(ML_SCALAR_ZERO,tActor->orientation[1]);
    EXPECT_EQ(ML_SCALAR_ZERO,tActor->orientation[2]);
    EXPECT_EQ(ML_SCALAR_ONE,tActor->orientation[3]);

    // Testing farClipping property.
    EXPECT_EQ(1.0,tActor->farClipping);

    // Testing nearClipping property.
    EXPECT_EQ(-1.0,tActor->nearClipping);

    // Testing aspectRatio property.
    EXPECT_EQ(0.75,tActor->aspectRatio);

    // Testing cameraHeight property.
    EXPECT_EQ(0,tActor->cameraType);

    // Testing viewHeight property.
    EXPECT_EQ(75.0,tActor->viewHeight);
}

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
#include "mle/DwpInt.h"
#include "mle/DwpString.h"
#include "mle/DwpIntArray.h"

// Include Magic Lantern Runtime unit test header files.
#include "TestActorClass.h"
#include "TestActor.h"

extern MleActor *_mlCreateTestActor(void);

using namespace std;


TEST(MleActorClassTest, Constructor) {
    // This test is named "Constructor", and belongs to the "MleActorClassTest"
    // test case.

    TestActorClass *actorClass = new TestActorClass("TestActor", _mlCreateTestActor, "MleActor");
    EXPECT_TRUE(actorClass != NULL);
    delete actorClass;
}

TEST(MleActorClassTest, AddMember) {
    // This test is named "AddMember", and belongs to the "MleActorClassTest"
    // test case.

    TestActorClass *actorClass = new TestActorClass("TestActor", _mlCreateTestActor, "MleActor");
    ASSERT_TRUE(actorClass != NULL);

    // Test simple property type.
    MlePropertyEntry *idEntry = new MlePropertyEntry();
    idEntry->name = "id";
    idEntry->getProperty = TestActor::getProperty;
    idEntry->setProperty = TestActor::setProperty;

    actorClass->addMember("id", "int", idEntry);
    const MleActorMember *idMember = actorClass->findMember("id");
    ASSERT_TRUE(idMember != NULL);
    const MleDwpDatatype *idDataType = idMember->getType();
    ASSERT_TRUE(idDataType != NULL);
    const char *idDataTypeName = ((MleDwpInt *)idDataType)->getName();
    EXPECT_STREQ("int", idDataTypeName);
    MlePropertyEntry *idMemberEntry = idMember->getEntry();
    EXPECT_STREQ("id", idMemberEntry->name);
    MlePropertyGetter idGetter = idMemberEntry->getProperty;
    ASSERT_TRUE(idGetter != NULL);
    MlePropertySetter idSetter = idMemberEntry->setProperty;
    ASSERT_TRUE(idSetter != NULL);

    TestActor *tActor = (TestActor *) actorClass->createInstance();
    ASSERT_TRUE(tActor != NULL);
    int value = 100;
    idSetter(tActor, "id", (unsigned char *)&value);
    EXPECT_EQ(100,tActor->id);

    // Test dynamic property type.
    MlePropertyEntry *textEntry = new MlePropertyEntry();
    textEntry->name = "text";
    textEntry->getProperty = TestActor::getProperty;
    textEntry->setProperty = TestActor::setProperty;

    actorClass->addMember("text", "string", textEntry);
    const MleActorMember *textMember = actorClass->findMember("text");
    ASSERT_TRUE(textMember != NULL);
    const MleDwpDatatype *textDataType = textMember->getType();
    ASSERT_TRUE(textDataType != NULL);
    const char *textDataTypeName = ((MleDwpString *)textDataType)->getName();
    EXPECT_STREQ("string", textDataTypeName);
    MlePropertyEntry *textMemberEntry = textMember->getEntry();
    EXPECT_STREQ("text", textMemberEntry->name);
    MlePropertyGetter textGetter = textMemberEntry->getProperty;
    ASSERT_TRUE(textGetter != NULL);
    MlePropertySetter textSetter = textMemberEntry->setProperty;
    ASSERT_TRUE(textSetter != NULL);

    const char *str = "Hello Colorado!";
    textSetter(tActor, "text", (unsigned char *)str);
    EXPECT_STREQ(str,tActor->text);

    // Test MlArray property type.
    MlePropertyEntry *ageEntry = new MlePropertyEntry();
    ageEntry->name = "age";
    ageEntry->getProperty = TestActor::getProperty;
    ageEntry->setProperty = TestActor::setProperty;

    actorClass->addMember("age", "IntArray", ageEntry);
    const MleActorMember *ageMember = actorClass->findMember("age");
    ASSERT_TRUE(ageMember != NULL);
    const MleDwpDatatype *ageDataType = ageMember->getType();
    ASSERT_TRUE(ageDataType != NULL);
    const char *ageDataTypeName = ((MleDwpIntArray *)ageDataType)->getName();
    EXPECT_STREQ("IntArray", ageDataTypeName);
    MlePropertyEntry *ageMemberEntry = ageMember->getEntry();
    EXPECT_STREQ("age", ageMemberEntry->name);
    MlePropertyGetter ageGetter = ageMemberEntry->getProperty;
    ASSERT_TRUE(ageGetter != NULL);
    MlePropertySetter ageSetter = ageMemberEntry->setProperty;
    ASSERT_TRUE(ageSetter != NULL);

    MleArray<int> birthdays(4);
    birthdays[0] = 15;
    birthdays[1] = 19;
    birthdays[2] = 51;
    birthdays[3] = 57;
    ageSetter(tActor, "age", (unsigned char *)&birthdays);
    EXPECT_EQ(15,tActor->age[0]);
    EXPECT_EQ(19,tActor->age[1]);
    EXPECT_EQ(51,tActor->age[2]);
    EXPECT_EQ(57,tActor->age[3]);

    delete actorClass;
}

TEST(MleActorClassTest, RegisterActorClass) {
    // This test is named "RegisterActorClass", and belongs to the "MleActorClassTest"
    // test case.

	TestActor *tActor = (TestActor *)_mlCreateTestActor();
	ASSERT_TRUE(tActor != NULL);

	// initClass() will register the TestActor's class and members.
	tActor->initClass();
	const MleActorClass* tActorClass = tActor->getClass();

	// Test registered position member access.
	const MleActorMember *positionMember = tActorClass->findMember("position");
	ASSERT_TRUE(positionMember != NULL);
	MlVector3 position;
	MlePropertyEntry *memberEntry = positionMember->getEntry();
	EXPECT_STREQ("position", memberEntry->name);
	memberEntry->getProperty(tActor, memberEntry->name, (unsigned char **)&position);
    EXPECT_EQ(100.0,position[0]);
    EXPECT_EQ(50.0,position[1]);
    EXPECT_EQ(250.0,position[2]);

	// Test registered orientation member access.
	const MleActorMember *orientationMember = tActorClass->findMember("orientation");
	ASSERT_TRUE(orientationMember != NULL);
	MlRotation orientation;
	memberEntry = orientationMember->getEntry();
	EXPECT_STREQ("orientation", memberEntry->name);
	memberEntry->getProperty(tActor, memberEntry->name, (unsigned char **)&orientation);
	EXPECT_EQ(ML_SCALAR_ZERO,orientation[0]);
	EXPECT_EQ(ML_SCALAR_ZERO,orientation[1]);
	EXPECT_EQ(ML_SCALAR_ZERO,orientation[2]);
	EXPECT_EQ(ML_SCALAR_ONE,orientation[3]);

	// Test registered nearClipping member access.
	const MleActorMember *nearClippingMember = tActorClass->findMember("nearClipping");
	ASSERT_TRUE(nearClippingMember != NULL);
	MlScalar nearClipping;
	memberEntry = nearClippingMember->getEntry();
	EXPECT_STREQ("nearClipping", memberEntry->name);
	memberEntry->getProperty(tActor, memberEntry->name, (unsigned char **)&nearClipping);
	EXPECT_EQ(-1.0,nearClipping);

	// Test registered farClipping member access.
	const MleActorMember *farClippingMember = tActorClass->findMember("farClipping");
	ASSERT_TRUE(farClippingMember != NULL);
	MlScalar farClipping;
	memberEntry = farClippingMember->getEntry();
	EXPECT_STREQ("farClipping", memberEntry->name);
	memberEntry->getProperty(tActor, memberEntry->name, (unsigned char **)&farClipping);
	EXPECT_EQ(1.0,farClipping);

	// Test registered aspectRatio member access.
	const MleActorMember *aspectRatioMember = tActorClass->findMember("aspectRatio");
	ASSERT_TRUE(aspectRatioMember != NULL);
	MlScalar aspectRatio;
	memberEntry = aspectRatioMember->getEntry();
	EXPECT_STREQ("aspectRatio", memberEntry->name);
	memberEntry->getProperty(tActor, memberEntry->name, (unsigned char **)&aspectRatio);
	EXPECT_EQ(0.75,aspectRatio);

	// Test registered cameraType member access.
	const MleActorMember *cameraTypeMember = tActorClass->findMember("cameraType");
	ASSERT_TRUE(cameraTypeMember != NULL);
	int cameraType;
	memberEntry = cameraTypeMember->getEntry();
	EXPECT_STREQ("cameraType", memberEntry->name);
	memberEntry->getProperty(tActor, memberEntry->name, (unsigned char **)&cameraType);
	EXPECT_EQ(0,cameraType);

	// Test registered viewHeight member access.
	const MleActorMember *viewHeightMember = tActorClass->findMember("viewHeight");
	ASSERT_TRUE(viewHeightMember != NULL);
	MlScalar viewHeight;
	memberEntry = viewHeightMember->getEntry();
	EXPECT_STREQ("viewHeight", memberEntry->name);
	memberEntry->getProperty(tActor, memberEntry->name, (unsigned char **)&viewHeight);
	EXPECT_EQ(75,viewHeight);

	// Test for non-registered property
	const MleActorMember *idMember = tActorClass->findMember("id");
	ASSERT_TRUE(idMember == NULL); // id Property is not registered by TestActor initClass()
}

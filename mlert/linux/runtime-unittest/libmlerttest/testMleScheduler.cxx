// COPYRTIGH_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2022 Wizzer Works
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
#include "mle/MleScheduler.h"

using namespace std;

void testFn(void* parm)
{
	cout << (char *)parm;
}

static MleSchedulerItem* deleteArg = NULL;
void deleteFn(void* parm)
{
    cout << "Test String D MleScheduler" << endl;
    if (deleteArg != NULL)
    {
	    ((MleScheduler*)parm)->remove(deleteArg);
    }
}

void* deleteTagArg = NULL;
void deleteTagFn(void* parm)
{
    cout << "Test String T MleScheduler" << endl;
    if (deleteTagArg != NULL)
    {
	    ((MleScheduler*)parm)->remove(deleteTagArg);
    }
}

TEST(MleSchedulerTest, DefaultConstructor) {
    // This test is named "DefaultConstructor", and belongs to the "MleSchedulerTest"
    // test case.

	MleScheduler *scheduler = new MleScheduler();
    EXPECT_TRUE(scheduler != NULL);
}

TEST(MleSchedulerTest, Destructor) {
    // This test is named "Destructor", and belongs to the "MleSchedulerTest"
    // test case.

	MleScheduler *scheduler = new MleScheduler();
    EXPECT_TRUE(scheduler != NULL);

    delete scheduler;
}

TEST(MleSchedulerTest, ParameterizedConstructor) {
    // This test is named "ParameterizedConstructor", and belongs to the "MleSchedulerTest"
    // test case.

    MleScheduler* scheduler = new MleScheduler(6, 16);
    EXPECT_TRUE(scheduler != NULL);

    delete scheduler;
}

TEST(MleSchedulerTest, InsertPhase) {
    // This test is named "InsertPhase", and belongs to the "MleSchedulerTest"
    // test case.

	MleScheduler* scheduler = new MleScheduler(6, 16);
    EXPECT_TRUE(scheduler != NULL);
    MleSchedulerPhase *p0 = scheduler->insertPhase();
    EXPECT_TRUE(p0 != NULL);
    // ToDo: Check contents of p0. There currently is no way to do this since the
    // structure is not exposed in the header file.
    MleSchedulerPhase *p1 = scheduler->insertPhase();
    EXPECT_TRUE(p1 != NULL);

    delete scheduler;
}

TEST(MleSchedulerTest, InsertPhaseLimit) {
    // This test is named "InsertPhaseLimit", and belongs to the "MleSchedulerTest"
    // test case.

	int nPhases = 6;
    MleScheduler* scheduler = new MleScheduler(nPhases, 16);
    for (int i = 0; i < nPhases; i++) {
        MleSchedulerPhase *p0 = scheduler->insertPhase();
        EXPECT_TRUE(p0 != NULL);
    }
    MleSchedulerPhase *p1 = scheduler->insertPhase();
    EXPECT_TRUE(p1 != NULL);
    // ToDo: Check contents of p1. There currently is no way to do this since the
    // structure is not exposed in the header file.

    delete scheduler;
}

TEST(MleSchedulerTest, InsertFunc) {
    // This test is named "InsertFunc", and belongs to the "MleSchedulerTest"
    // test case.

    const char* data = "Test function data";
    const char* tag = "Test function tag";

    MleScheduler* scheduler = new MleScheduler(6, 16);
    EXPECT_TRUE(scheduler != NULL);
    MleSchedulerPhase *p0 = scheduler->insertPhase();
    EXPECT_TRUE(p0 != NULL);
    MleSchedulerItem *item = scheduler->insertFunc(p0, testFn, (void *)data, (void *)tag);
    EXPECT_TRUE(item != NULL);
    // ToDo: Check contents of item. There currently is no way to do this since the
    // structure is not exposed in the header file.

    delete scheduler;
}

TEST(MleSchedulerTest, Go1) {
    // This test is named "InsertFunc", and belongs to the "Go1"
    // test case.

	const char* theString1 = "Test String 1 MleScheduler.\n";
	const char* theString2 = "Test String 2 MleScheduler.\n";
	const char* theString3 = "Test String 3 MleScheduler.\n";
	MleSchedulerItem* blocks[3];

    MleScheduler* scheduler = new MleScheduler(6, 16);
    EXPECT_TRUE(scheduler != NULL);
    MleSchedulerPhase *p0 = scheduler->insertPhase();
    EXPECT_TRUE(p0 != NULL);
    MleSchedulerPhase *p1 = scheduler->insertPhase();
    EXPECT_TRUE(p1 != NULL);

    cout << "Should see strings printed in order 13 3 123" << endl;
    blocks[0] = scheduler->insertFunc(p1, testFn, (void *)theString1, (void *)theString1, 2);
    blocks[1] = scheduler->insertFunc(p1, testFn, (void *)theString2, (void *)theString2, 1, 3);
    blocks[2] = scheduler->insertFunc(p1, testFn, (void *)theString3, (void *)theString3);
    scheduler->go(p1);
    scheduler->go(p0);
    scheduler->go(p1);
    scheduler->go(p0);
    scheduler->go(p1);
    scheduler->go(p0);

    delete scheduler;
}

TEST(MleSchedulerTest, Go2) {
    // This test is named "InsertFunc", and belongs to the "Go2"
    // test case.

	const char* theString1 = "Test String 1 MleScheduler.\n";
	const char* theString2 = "Test String 2 MleScheduler.\n";
	const char* theString3 = "Test String 3 MleScheduler.\n";
	MleSchedulerItem* blocks[4];

    MleScheduler* scheduler = new MleScheduler(6, 16);
    EXPECT_TRUE(scheduler != NULL);
    MleSchedulerPhase *p0 = scheduler->insertPhase();
    EXPECT_TRUE(p0 != NULL);
    MleSchedulerPhase *p1 = scheduler->insertPhase();
    EXPECT_TRUE(p1 != NULL);

    cout << "Should see strings printed in order 3D 3D D D." << endl;
    blocks[0] = scheduler->insertFunc(p1, testFn, (void *)theString1, (void *)theString1, 2);
    blocks[1] = scheduler->insertFunc(p1, testFn, (void *)theString2, (void *)theString2, 1, 3);
    blocks[2] = scheduler->insertFunc(p1, testFn, (void *)theString3, (void *)theString3);
    scheduler->remove(blocks[0]);
    scheduler->remove(blocks[1]);
    blocks[3] = scheduler->insertFunc(p1, deleteFn, scheduler, scheduler);
    scheduler->go(p1);
    scheduler->go(p0);
    deleteArg = blocks[2];
    scheduler->go(p1);
    scheduler->go(p0);
    deleteArg = NULL;
    scheduler->go(p1);
    scheduler->go(p0);
    deleteArg = blocks[3];
    scheduler->go(p1);
    scheduler->go(p1);
    scheduler->go(p1);
    scheduler->go(p1);
    scheduler->go(p1);

    delete scheduler;
}

TEST(MleSchedulerTest, Go3) {
    // This test is named "InsertFunc", and belongs to the "Go3"
    // test case.

    const char* theString4 = "Test String 4 MleScheduler.\n";
    const char* theString5 = "Test String 5 MleScheduler.\n";

    MleScheduler* scheduler = new MleScheduler(6, 16);
    EXPECT_TRUE(scheduler != NULL);
    MleSchedulerPhase *p0 = scheduler->insertPhase();
    EXPECT_TRUE(p0 != NULL);
    MleSchedulerPhase *p1 = scheduler->insertPhase();
    EXPECT_TRUE(p1 != NULL);

    cout << "Should see strings printed in order 544T5 5T5 5T5 5T." << endl;
    scheduler->insertFunc(p1, testFn, (void *)theString5, (void *)theString5);
    scheduler->insertFunc(p1, testFn, (void *)theString4, (void *)theString4);
    scheduler->insertFunc(p1, testFn, (void *)theString4, (void *)theString4);
    scheduler->insertFunc(p1, deleteTagFn, scheduler, (void *)theString5);
    scheduler->insertFunc(p1, testFn, (void *)theString5, (void *)theString5);
    scheduler->go(p1);
    scheduler->go(p0);
    scheduler->remove((void *)theString4);
    scheduler->go(p1);
    scheduler->go(p0);
    scheduler->go(p1);
    scheduler->go(p0);
    deleteTagArg = (void *)theString5;
    scheduler->go(p1);
    scheduler->go(p0);
    deleteTagArg = NULL;
    scheduler->go(p1);
    scheduler->go(p1);
    scheduler->go(p1);
    scheduler->go(p1);
    scheduler->go(p1);

    delete scheduler;
}

//#include <iostream>
#include <mle/mlAssert.h>
#include <mle/MlePtrArray.h>

// Scheduled Object class.
class SchedObj
{
  public:

    SchedObj(char *s=(char *)"") :item(NULL) { str = strdup(s); };
    ~SchedObj() { if (str) { delete str; } };

    static void update(SchedObj *data);
    char*  getStr() { return str; };

    MleSchedulerItem*  getItem() { return item; };
    void setItem(MleSchedulerItem *i) { item = i; };

    //friend ostream& operator <<(ostream&, SchedObj *obj);

    static int outLen;

    // The array of objects we\'re scheduling.
    static MlePtrArray *objArray;
    static int          objArrayLen;

  protected:

    char *str;
    MleSchedulerItem *item;
};

#define MARGIN_COL 72

// The length written so far this line.
int SchedObj::outLen = 0;

// The array of objects we're scheduling.
MlePtrArray* SchedObj::objArray = NULL;
int SchedObj::objArrayLen = 0;

// The only phase.
MleSchedulerPhase* theMleSchedulerPhase = NULL;

void
SchedObj::update(SchedObj *data)
{
	delete new char[8];
	fprintf(ML_DEBUG_OUTPUT_FILE, "%s", data->getStr());
	//cout << data;
	if ((SchedObj::outLen += strlen(data->getStr())) > MARGIN_COL) {
		//cout << "\n" << flush;
		fprintf(ML_DEBUG_OUTPUT_FILE, "\n");
		fflush(ML_DEBUG_OUTPUT_FILE);
		SchedObj::outLen = 0;
	}
}

//ostream&
//operator << (ostream& os, SchedObj *obj)
//{
//	os << obj->getStr();
//	return os;
//}

void
scheduleObj(MleScheduler *sched, unsigned int i)
{
	// Make a tagged object to schedule.
	char id[16];
	sprintf(id, "%.3d", i);
	SchedObj *obj = new SchedObj( id );

	// Put into the scheduler.
	MLE_ASSERT( NULL != theMleSchedulerPhase);
	MleSchedulerItem *item = sched->insertFunc
		(theMleSchedulerPhase, (MleSchedulerFunc)SchedObj::update, obj, id);
	EXPECT_TRUE(item != NULL);
	obj->setItem(item);

	// Log to stdout.
	fprintf(ML_DEBUG_OUTPUT_FILE, "Add  [%d] = %s\n", i, obj->getStr());
	fflush(ML_DEBUG_OUTPUT_FILE);
	//cout << "Add  [" << i << "] = " << obj << "\n" << flush;

	// Save for future deletion.
	(*obj->objArray)[obj->objArrayLen++] = (void*) obj;
}

SchedObj*
unscheduleObj(MleScheduler *sched, unsigned int i)
{
	MLE_ASSERT(i < SchedObj::objArrayLen);
	MLE_ASSERT(SchedObj::objArrayLen >= 1);

	// Find in the array and replace with NULL.
	SchedObj *obj = (SchedObj *) (*SchedObj::objArray)[i];

	if (NULL == obj) {
		return NULL;
	}

	// Log to stdout.
	fprintf(ML_DEBUG_OUTPUT_FILE, "Del  [%d] = %s\n", i, obj->getStr());
	fflush(ML_DEBUG_OUTPUT_FILE);
	//cout << "Del  [" << i << "]   " << obj << "\n" << flush;

	(*SchedObj::objArray)[i] = NULL;

	// Now unschedule the update function and delete the object.
	MleSchedulerItem *item = obj->getItem();
	sched->remove(item);

	return obj;
}

TEST(MleSchedulerTest, ScheduleObject) {
    // This test is named "InsertFunc", and belongs to the "ScheduleObject"
    // test case.

	int inserts = 7;
	int updates = 13;

	// Idea is to insert a set of funcs, then to make a number
	// of updates (insert/delete) to the scheduler.
	// To avoid leaking memory, we must keep track of the objects
	// whose update() function we're scheduling, so that we can
	// delete the object when we unschedule it.

	SchedObj::objArray = new MlePtrArray(inserts + updates);
	EXPECT_TRUE(SchedObj::objArray != NULL);
	MleScheduler* scheduler = new MleScheduler(6, inserts + updates);
    EXPECT_TRUE(scheduler != NULL);
	theMleSchedulerPhase = scheduler->insertPhase();

	for (unsigned int i = 0; i < inserts; i++) {
		scheduleObj(scheduler, i);
	}

	// Init seed for random number generation,
	// any old integral number will do in place of 1.
	srand(1);

	// We know that rand() returns numbers in [0..2^15-1)
#define DRAND(j) (double)((j) / 32768.0)

	for (unsigned int i = 0; i < updates; i++) {

		int j = rand();
		double d = DRAND(j);
		if (d >= 0.5) {
			// Insert a new one.
			scheduleObj(scheduler, i);
		}
		else {
			// Delete an old one.

			// We know that DRAND < 1/2, so multiply
			// by 2 to get in the range [0..1) and then
			// scale to size of objArray.

			unsigned int origIndex;
			unsigned int index;
			origIndex = index = SchedObj::objArrayLen * 2 * d;

			SchedObj *obj;
			while (NULL == (obj = unscheduleObj(scheduler, index))) {
				index = ++index % SchedObj::objArrayLen;
				// Prevent infinite loop.
				if (index == origIndex) {
					printf("Ran out of items to unschedule. Increase number of in/dels.\n");
					fflush(stdout);
					goto finalize;
				}
			}
			delete obj;
		}

	}

finalize:

    // Final cleanup.
    unsigned int n = SchedObj::objArrayLen;
    for (unsigned int i = 0; i < n; i++) {
	    SchedObj *obj = unscheduleObj(scheduler, i);
	    if (NULL != obj) {
		    delete obj;
	    }
    }

    delete SchedObj::objArray;
    SchedObj::objArray = NULL;
    delete scheduler;
}

/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MlePq.h
 * @ingroup MleFoundation
 *
 * This file implements the Priority Queue object class for
 * the Magic Lantern Runtime Engine.
 *
 * @author Mark S. Millard
 * @date May 1, 2003
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015 Wizzer Works
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
//  For information concerning this source file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END


// Include system header files.
#ifdef WIN32
#include <stdlib.h>
#include <memory.h>
#else
#include <string.h>
#ifdef __linux__
#else
#ifndef __APPLE__
#include <stream.h>
#endif /* ! __APPLE__ */
#endif
#endif /* WIN32 */

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"

// Include Runtime Engine header files.
#include "mle/MlePq.h"


MlePQ::MlePQ(void)
    :m_fpqQueue(NULL),
     m_fpqSize(0),
     m_fpqNumItems(0)
{
    // do nothing extra
}


MlePQ::MlePQ(unsigned int size)
{
    m_fpqQueue = new MlePQItem[size + 1];
    m_fpqSize = size;
    m_fpqNumItems = 0;
}

MlePQ::~MlePQ(void)
{
    if (m_fpqQueue) delete [] m_fpqQueue;
}


void MlePQ::insert(MlePQItem &item)
{
    // Grow m_fpqQueue if needed
    if (m_fpqNumItems == m_fpqSize)
        grow();

    // Insert item
    m_fpqNumItems++;
    m_fpqQueue[m_fpqNumItems] = item;
    upHeap(m_fpqNumItems);
}


MlBoolean MlePQ::remove(MlePQItem &item)
{
    // declare local variables
    MlBoolean retValue = TRUE;

    if (m_fpqNumItems == 0) {
        // check to see if there are any items in the queue
        item.m_key = MLE_MIN_QPRIORITY;
        item.m_data = NULL;
        retValue = FALSE;
    } else if (m_fpqNumItems == 1) {
        // check to see if there is only one item in the queue
        item = m_fpqQueue[m_fpqNumItems--];
    } else
    {
        item = m_fpqQueue[1];
        m_fpqQueue[1] = m_fpqQueue[m_fpqNumItems--];
        downHeap(1);
    }

    return(retValue);
}


MlBoolean MlePQ::remove(int priority,MlePQItem **items,unsigned int *numItems)
{
    // declare local variables
    unsigned int k,numFound = 0;
    MlePQItem *foundQ;
    MlBoolean retValue = TRUE;

    // allocate enough space for potential "hit" list
    foundQ = new MlePQItem[m_fpqNumItems];

    // find matching items
    while((k = findItem(priority)) != 0) {
        foundQ[numFound++] = m_fpqQueue[k];
        destroyItem(k);
    }

    if (! numFound) {
        *items = NULL;
        *numItems = 0;
        delete [] foundQ;
        retValue = FALSE;
    } else {
        *items = foundQ;
        *numItems = numFound;
    }

    return(retValue);
}


MlBoolean MlePQ::replace(MlePQItem &item)
{
    m_fpqQueue[0] = item;
    downHeap(0);
    if ((item.m_key == m_fpqQueue[0].m_key) && (item.m_data == m_fpqQueue[0].m_data))
        return(FALSE);
    else
        return(TRUE);
}


void MlePQ::destroy(void)
{
    destroyItem(1);
}


void MlePQ::destroy(int priority)
{
    // declare local variables
    unsigned int k;

    // delete all items of specified weight
    while((k = findItem(priority)) != 0) {
        destroyItem(k);
    }
}


MlBoolean MlePQ::inQueue(int priority)
{
    // declare local variables
    MlBoolean retValue = TRUE;

    if (findItem(priority) == 0)
        retValue = FALSE;

    return(retValue);
}


MlBoolean MlePQ::inQueue(MlePQItem &item)
{
    // declare local variables
    MlBoolean retValue = TRUE;

    if (findItem(item) == 0)
        retValue = FALSE;

    return(retValue);
}


unsigned int MlePQ::copyQueue(MlePQItem **queue)
{
    if (m_fpqNumItems > 0) {
        *queue = new MlePQItem[m_fpqNumItems];
        memcpy(*queue,&m_fpqQueue[1],m_fpqNumItems * sizeof(MlePQItem));
    } else *queue = NULL;

    return(m_fpqNumItems);
}


#if defined(MLE_DEBUG)
void MlePQ::print(void)
{
    // print queue to standard out
    for (unsigned int i = 1; i <= m_fpqNumItems; i++)
	{
		fprintf(ML_DEBUG_OUTPUT_FILE, "Queue Item: %d\n", i);
		fprintf(ML_DEBUG_OUTPUT_FILE, "\tPriority: %d\n", m_fpqQueue[i].m_key);
		fprintf(ML_DEBUG_OUTPUT_FILE, "\tData: %d\n", (int)m_fpqQueue[i].m_data);
		fflush(ML_DEBUG_OUTPUT_FILE);
    }
}


void MlePQ::printSorted(void)
{
    // declare local variables
    MlePQItem *printQ;

    // allocate print queue
    printQ = new MlePQItem[m_fpqNumItems];
    memcpy(printQ,&m_fpqQueue[1],m_fpqNumItems * sizeof(MlePQItem));

    // sort queue according to priority
    sort(printQ,m_fpqNumItems);

    // print queue to standard out
    for (unsigned int i = 0; i < m_fpqNumItems; i++)
	{
		fprintf(ML_DEBUG_OUTPUT_FILE, "Priority: %d\n", printQ[i].m_key);
		fprintf(ML_DEBUG_OUTPUT_FILE, "Data: %d\n", (int)printQ[i].m_data);
		fflush(ML_DEBUG_OUTPUT_FILE);
    }

    // deallocate print queue
    delete [] printQ;
}
#endif /* MLE_DEBUG */


void MlePQ::clear(void)
{
    m_fpqNumItems = 0;
}


MlBoolean MlePQ::grow(void)
{
    return(_grow(MLE_INC_QSIZE));
}


MlBoolean MlePQ::grow(unsigned int size)
{
    return(_grow(size));
}


//
// This method is used to fix the heap condition violation after a new
// item is put at location m_fpqQueue[k].
//
void MlePQ::upHeap(unsigned int k)
{
    // declare local variables
    MlePQItem curItem;

    curItem = m_fpqQueue[k];

    // Put a sentinel key (miximum priority) in m_fpqQueue[0] to stop the
    // the loop for the case that priority is greater than all the keys
    // in the heap.
    m_fpqQueue[0].m_key = MLE_MAX_QPRIORITY;

    while(m_fpqQueue[k/2].m_key <= curItem.m_key)
    {
        m_fpqQueue[k] = m_fpqQueue[k/2];
        k = k/2;
    }
    m_fpqQueue[k] = curItem;
}


//
// This method is used to fix the heap condition violation after a new
// item is put at location m_fpqQueue[k].
//
void MlePQ::downHeap(unsigned int k)
{
    // declare local variables
    MlePQItem curItem;
    unsigned int j;

    curItem = m_fpqQueue[k];

    unsigned int loopCnd = m_fpqNumItems/2;
    while(k <= loopCnd) {
        j = k + k;
        if ((j < m_fpqNumItems) && 
            (m_fpqQueue[j].m_key < m_fpqQueue[j + 1].m_key)) j++;
        if (curItem.m_key >= m_fpqQueue[j].m_key) break;
        m_fpqQueue[k] = m_fpqQueue[j];
        k = j;
    }
    m_fpqQueue[k] = curItem;
}


MlBoolean MlePQ::_grow(unsigned int size)
{
    // declare local variables
    unsigned int memSize;
    MlePQItem *newQ;


    // add 1 to requested size; m_fpqQueue[0] is reserved for a
    // sentinel value (used by the "heap" algorithms)
    memSize = m_fpqSize + size + 1;

    // allocate a new queue
    newQ = new MlePQItem[memSize];
    if (newQ == NULL) {
        // set error status here - XXX
        return(FALSE);
    } else {
        if (m_fpqQueue) {
            memcpy(newQ,m_fpqQueue,(m_fpqSize + 1) * sizeof(MlePQItem));
            delete [] m_fpqQueue;
        }
        m_fpqQueue = newQ;
    }

    // bump size of queue
    m_fpqSize += size;

    return(TRUE);
}


MlBoolean MlePQ::changeItem(unsigned int k,int priority)
{
    // declare local variables
    MlePQItem item;
    MlBoolean retValue = TRUE;

    // check to see if there are any items in the queue
    if ((k == 0) || (k > m_fpqNumItems) || (m_fpqNumItems == 0)) {
        retValue = FALSE;
    } else {
        item.m_key = priority;
        item.m_data = m_fpqQueue[k].m_data;
        destroyItem(k);
        insert(item);
    }

    return(retValue);
}


void MlePQ::destroyItem(unsigned int k)
{
    // declare local variables
    int prevKey;

    // check to see if there are any items in the queue
    if ((k == 0) || (k > m_fpqNumItems) || (m_fpqNumItems == 0)) return;
    else if (m_fpqNumItems == 1) {
        m_fpqNumItems--;
        return;
    }

    prevKey = m_fpqQueue[k].m_key;
    m_fpqQueue[k] = m_fpqQueue[m_fpqNumItems--];
    if (m_fpqQueue[k].m_key < prevKey) downHeap(k);
    else if (m_fpqQueue[k].m_key > prevKey) upHeap(k);
}


MlBoolean MlePQ::peek(unsigned int k,MlePQItem &item)
{
    // declare local variables
    MlBoolean retValue = TRUE;

    if ((k > 0) && (k <= m_fpqNumItems)) {
        item.m_key = m_fpqQueue[k].m_key;
        item.m_data = m_fpqQueue[k].m_data;
    } else {
        item.m_key = MLE_MIN_QPRIORITY;
        item.m_data = NULL;
        retValue = FALSE;
    }

    return retValue;
}


unsigned int MlePQ::findItem(int priority)
{
    // declare local variables
    MlBoolean notFound = TRUE;
    unsigned int i = 1;

    // find first item with specified priority
    while((notFound) && (i <= m_fpqNumItems)) {
        if (m_fpqQueue[i].m_key == priority) notFound = FALSE;
        else i++;
    }

    if (notFound) return(0);
    else return(i);
}


unsigned int MlePQ::findItem(MlePQItem &item)
{
    // declare local variables
    MlBoolean notFound = TRUE;
    unsigned int i = 1;

    // find first item with specified priority
    while((notFound) && (i <= m_fpqNumItems)) {
        if ((m_fpqQueue[i].m_key == item.m_key) &&(m_fpqQueue[i].m_data == item.m_data))
            notFound = FALSE;
        else i++;
    }

    if (notFound) return(0);
    else return(i);
}


unsigned int MlePQ::findItem(MlePQCallback func,void *clientData)
{
    // declare local variables
    MlBoolean notFound = TRUE;
    unsigned int i = 1;

    // find first item with specified priority
    while((notFound) && (i <= m_fpqNumItems)) {
        if (func(m_fpqQueue[i],clientData))
            notFound = FALSE;
        else i++;
    }

    if (notFound) return(0);
    else return(i);
}


void MlePQ::sort(MlePQItem *queue, int numItems)
{
    // declare local variables
    int i;
    MlePQ heap(numItems);

    for (i = 0; i < numItems; i++)
        heap.insert(queue[i]);
    for (i = --numItems; i >= 0; i--)
        heap.remove(queue[i]);
}


MlePQ *MlePQ::join(MlePQ &pq1,MlePQ &pq2)
{
    // declare local variables
    unsigned int i,numItems;
    MlePQ *heap;
    MlePQItem *tmpQ1,*tmpQ2;

    // construct a new queue
    numItems = pq1.getNumItems() + pq2.getNumItems();
    if (numItems > 0) {
        heap = new MlePQ(numItems);

        if (pq1.copyQueue(&tmpQ1) > 0)
            for (i = 0; i < pq1.getNumItems(); i++)
                heap->insert(tmpQ1[i]);
        if (pq2.copyQueue(&tmpQ2) > 0)
            for (i = 0; i < pq2.getNumItems(); i++)
                heap->insert(tmpQ2[i]);

        if (tmpQ1) delete [] tmpQ1;
        if (tmpQ2) delete [] tmpQ2;
    } else
        heap = NULL;

    return(heap);
}


void MlePQ::operator =(MlePQ &queue)
{
    // declare local variables
    MlePQItem item;

    // clear the queue
    if (m_fpqNumItems > 0)
        clear();

    //
    // Note that index 0 is used as a sentinel value for heap
    // management. Therefore, begin copying the queue with index 1.
    //
    for (unsigned int i = 1; i <= queue.getNumItems(); i++) {
        if (queue.peek(i,item))
            insert(item);
    }
}


void *
MlePQ::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}


void
MlePQ::operator delete(void *p)
{
	mlFree(p);
}


#ifdef UNITTEST

// Include Magic Lantern header files.
#include "mle/MleDebug.h"


int main(int argc,char *argv[])
{
    // Declare some local variables.
    int i,size,numItems,runTest = 0;
    int origSize = MLE_INC_QSIZE;
    MlePQ testQ;
    MlePQItem testItem;
    MlBoolean runAll = TRUE;

    // Parse arguments.

    //
    // TEST 1 - test basic insertion and removal.
    //
    if (runAll || (runTest == 1)) {
		fprintf(ML_DEBUG_OUTPUT_FILE, "\n#### TEST 1 #####\n");
 
        size = origSize;

        for (i = 0; i < size; i++) {
            testItem.key = i;
            testItem.data = (void *)i;
            testQ.insert(testItem);
        }

        MLE_DEBUG_CAT("info",{
			fprintf(ML_DEBUG_OUTPUT_FILE, "\n***** Contents of Queue *****\n");
            testQ.print();
			fprintf(ML_DEBUG_OUTPUT_FILE, "\n\n");
			fflush(ML_DEBUG_OUTPUT_FILE);
        });

        numItems = testQ.getNumItems();
        for (i = 0; i < numItems; i++) {
            testQ.remove(testItem);
			fprintf(ML_DEBUG_OUTPUT_FILE, "Item: %d\n", i);
			fprintf(ML_DEBUG_OUTPUT_FILE, "\tPriority: %d\n", testItem.key);
			fprintf(ML_DEBUG_OUTPUT_FILE, "\tData: %d\n", (int)testItem.data);
			fflush(ML_DEBUG_OUTPUT_FILE);
        }
    }

    //
    // TEST 2 - test queue growth and priority ordering.
    //
    if (runAll || (runTest == 2)) {
		fprintf(ML_DEBUG_OUTPUT_FILE, "\n#### TEST 2 #####\n");

        size = 2 * origSize;

        for (i = 0; i < size; i++) {
            testItem.key = i / 5;
            testItem.data = (void *)i;
            testQ.insert(testItem);
        }

        MLE_DEBUG_CAT("info",{
			fprintf(ML_DEBUG_OUTPUT_FILE, "\n***** Contents of Queue *****\n");
            testQ.print();
			fprintf(ML_DEBUG_OUTPUT_FILE, "\n\n");
			fflush(ML_DEBUG_OUTPUT_FILE);
        });

        numItems = testQ.getNumItems();
        for (i = 0; i < numItems; i++) {
            testQ.remove(testItem);
			fprintf(ML_DEBUG_OUTPUT_FILE, "Item: %d\n", i);
			fprintf(ML_DEBUG_OUTPUT_FILE, "\tPriority: %d\n", testItem.key);
			fprintf(ML_DEBUG_OUTPUT_FILE, "\tData: %d\n", (int)testItem.data);
			fflush(ML_DEBUG_OUTPUT_FILE);
        }
    }

    //
    // TEST 3 - test queue deletion.
    //
    if (runAll || (runTest == 3)) {
		fprintf(ML_DEBUG_OUTPUT_FILE, "\n#### TEST 3 #####\n");

        size = 2 * origSize;

        for (i = 0; i < size; i++) {
            testItem.key = i / 5;
            testItem.data = (void *)i;
            testQ.insert(testItem);
        }

        for (i = 0; i < size; i++)
            testQ.destroy();

        numItems = testQ.getNumItems();
        for (i = 0; i < numItems; i++) {
            testQ.remove(testItem);
			fprintf(ML_DEBUG_OUTPUT_FILE, "Item: %d\n", i);
			fprintf(ML_DEBUG_OUTPUT_FILE, "\tPriority: %d\n", testItem.key);
			fprintf(ML_DEBUG_OUTPUT_FILE, "\tData: %d\n", (int)testItem.data);
			fflush(ML_DEBUG_OUTPUT_FILE);
        }
    }

    //
    // TEST 4 - test queue deletion by priority.
    //
    if (runAll || (runTest == 4)) {
		fprintf(ML_DEBUG_OUTPUT_FILE, "\n#### TEST 4 #####\n");

        size = 2 * origSize;

        for (i = 0; i < size; i++) {
            testItem.key = i / 5;
            testItem.data = (void *)i;
            testQ.insert(testItem);
        }

        MLE_DEBUG_CAT("info",{
			fprintf(ML_DEBUG_OUTPUT_FILE, "\n***** Contents of Queue *****\n");
            testQ.print();
			fprintf(ML_DEBUG_OUTPUT_FILE, "\n\n");
			fflush(ML_DEBUG_OUTPUT_FILE);
        });

        // Delete even priority items.
        for (i = 0; i < size; i += 10)
            testQ.destroy(i/5);

        MLE_DEBUG_CAT("info",{
			fprintf(ML_DEBUG_OUTPUT_FILE, "\n***** Contents of Queue *****\n");
            testQ.print();
			fflush(ML_DEBUG_OUTPUT_FILE);
        });

        numItems = testQ.getNumItems();
        for (i = 0; i < numItems; i++) {
            testQ.remove(testItem);
 			fprintf(ML_DEBUG_OUTPUT_FILE, "Item: %d\n", i);
			fprintf(ML_DEBUG_OUTPUT_FILE, "\tPriority: %d\n", testItem.key);
			fprintf(ML_DEBUG_OUTPUT_FILE, "\tData: %d\n", (int)testItem.data);
			fflush(ML_DEBUG_OUTPUT_FILE);
        }
    }

    //
    // TEST 5 - test finding priority in queue and prioritized removal.
    //
    if (runAll || (runTest == 5)) {
		fprintf(ML_DEBUG_OUTPUT_FILE, "\n#### TEST 5 #####\n");

        size = 2 * origSize;

        for (i = 0; i < size; i++) {
            testItem.key = i / 5;
            testItem.data = (void *)i;
            testQ.insert(testItem);
        }

        MLE_DEBUG_CAT("info",{
			fprintf(ML_DEBUG_OUTPUT_FILE, "\n***** Contents of Queue *****\n");
            testQ.print();
			fprintf(ML_DEBUG_OUTPUT_FILE, "\n\n");
			fflush(ML_DEBUG_OUTPUT_FILE);
        });

        if (testQ.inQueue(size / 10)) {
            MlePQItem *foundItems;
            unsigned int numFoundItems;

            testQ.remove(size / 10,&foundItems,&numFoundItems);
            for (i = 0; i < numFoundItems; i++) {
 				fprintf(ML_DEBUG_OUTPUT_FILE, "Item: %d\n", i);
				fprintf(ML_DEBUG_OUTPUT_FILE, "\tPriority: %d\n", foundItems[i].key);
				fprintf(ML_DEBUG_OUTPUT_FILE, "\tData: %d\n", (int)foundItems[i].data);
				fflush(ML_DEBUG_OUTPUT_FILE);
            }

            if (foundItems) delete [] foundItems;
        }

        testQ.clear();
    }

    //
    // TEST 6 - test item replacement.
    //
    if (runAll || (runTest == 6)) {
		fprintf(ML_DEBUG_OUTPUT_FILE, "\n#### TEST 6 #####\n");

        size = 2 * origSize;

        for (i = 0; i < size; i++) {
            testItem.key = i / 5;
            testItem.data = (void *)i;
            testQ.insert(testItem);
        }

        MLE_DEBUG_CAT("info",{
			fprintf(ML_DEBUG_OUTPUT_FILE, "\n***** Contents of Queue *****\n");
            testQ.print();
			fprintf(ML_DEBUG_OUTPUT_FILE, "\n\n");
			fflush(ML_DEBUG_OUTPUT_FILE);
        });

        testQ.remove(testItem);
		fprintf(ML_DEBUG_OUTPUT_FILE, "Old Highest Priority Item: \n");
		fprintf(ML_DEBUG_OUTPUT_FILE, "\tPriority: %d\n", testItem.key);
		fprintf(ML_DEBUG_OUTPUT_FILE, "\tData: %d\n", (int)testItem.data);
		fflush(ML_DEBUG_OUTPUT_FILE);
        testQ.insert(testItem);

        testItem.key = size / 10;
        testQ.replace(testItem);
        if (testQ.inQueue(testItem)) {
            testQ.remove(testItem);
			fprintf(ML_DEBUG_OUTPUT_FILE, "New Highest Priority Item: \n");
			fprintf(ML_DEBUG_OUTPUT_FILE, "\tPriority: %d\n", testItem.key);
			fprintf(ML_DEBUG_OUTPUT_FILE, "\tData: %d\n", (int)testItem.data);
			fflush(ML_DEBUG_OUTPUT_FILE);
        }

        testQ.clear();
    }

    //
    // TEST 7 - test "join" operation on two queues.
    //
    if (runAll || (runTest == 7)) {
		fprintf(ML_DEBUG_OUTPUT_FILE, "\n#### TEST 7 #####\n");

        MlePQ secondQ(100),*jointQ;

        size = 2 * origSize;

        for (i = 0; i < size; i++) {
            testItem.key = i / 5;
            testItem.data = (void *)i;
            testQ.insert(testItem);
        }

        for (i = 0; i < 100; i++) {
            testItem.key = i;
            testItem.data = (void *)i;
            secondQ.insert(testItem);
        }

        MLE_DEBUG_CAT("info",{
			fprintf(ML_DEBUG_OUTPUT_FILE, "\n***** Contents of First Queue *****\n");
            testQ.print();
			fprintf(ML_DEBUG_OUTPUT_FILE, "\n\n");
			fflush(ML_DEBUG_OUTPUT_FILE);
			fprintf(ML_DEBUG_OUTPUT_FILE, "\n***** Contents of Second Queue *****\n");
            secondQ.print();
			fprintf(ML_DEBUG_OUTPUT_FILE, "\n\n");
			fflush(ML_DEBUG_OUTPUT_FILE);
        });

        jointQ = MlePQ::join(testQ,secondQ);

        numItems = jointQ->getNumItems();
        for (i = 0; i < numItems; i++) {
            jointQ->remove(testItem);
			fprintf(ML_DEBUG_OUTPUT_FILE, "Queue Item: %d\n", i);
			fprintf(ML_DEBUG_OUTPUT_FILE, "\tPriority: %d\n", testItem.key);
			fprintf(ML_DEBUG_OUTPUT_FILE, "\tData: %d\n", (int)testItem.data);
			fflush(ML_DEBUG_OUTPUT_FILE);
        }

        testQ.clear();
        secondQ.clear();
    }

    //
    // TEST 8 - test assignment operation overloading
    //          and priority changing.
    //
    if (runAll || (runTest == 8)) {
		fprintf(ML_DEBUG_OUTPUT_FILE, "\n#### TEST 8 #####\n");

        MlePQ secondQ;
        int index;

        size = origSize;

        for (i = 0; i < size; i++) {
            testItem.key = i;
            testItem.data = (void *)i;
            testQ.insert(testItem);
        }

        secondQ = testQ;

        // Change the ten highest priority items to priority 0.
        for (i = size-1; i >= size-10; i--) {
             index = secondQ.findItem(i);
             secondQ.changeItem(index,0);
        }

        MLE_DEBUG_CAT("info",{
			fprintf(ML_DEBUG_OUTPUT_FILE, "\n***** Contents of Queue *****\n");
            secondQ.print();
			fprintf(ML_DEBUG_OUTPUT_FILE, "\n\n");
			fflush(ML_DEBUG_OUTPUT_FILE);
        });

        numItems = secondQ.getNumItems();
        for (i = 0; i < numItems; i++) {
            secondQ.remove(testItem);
			fprintf(ML_DEBUG_OUTPUT_FILE, "Item: %d\n", i);
			fprintf(ML_DEBUG_OUTPUT_FILE, "\tPriority: %d\n", testItem.key);
			fprintf(ML_DEBUG_OUTPUT_FILE, "\tData: %d\n", (int)testItem.data);
			fflush(ML_DEBUG_OUTPUT_FILE);

        }

        testQ.clear();
        secondQ.clear();
    }

    return 0;
}

#endif /* UNITTEST */

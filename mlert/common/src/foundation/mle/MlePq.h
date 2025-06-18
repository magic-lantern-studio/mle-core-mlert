/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MlePq.h
 * @ingroup MleFoundation
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015-2025 Wizzer Works
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

#ifndef __MLE_PQ_H_
#define __MLE_PQ_H_


// Include system header files.
//#include <stdlib.h>
#ifdef _WINDOWS
#include <climits>
#endif /* _WINDOWS */

// Include Magic Lantern header files.
#include "mle/mlTypes.h"

#include "mle/MleRuntime.h"


typedef struct MLE_RUNTIME_API mlePQItem
{
    int m_key;              // priority key
    void *m_data;           // associated data
} MlePQItem;

typedef MlBoolean (*MlePQCallback)(MlePQItem &item,void *clientData);


/**
 * MlePQ is a priority queue.
 */
class MLE_RUNTIME_API MlePQ
{
  // Declare member variables.

  private:

    MlePQItem *m_fpqQueue;       // array of items in queue
    unsigned int m_fpqSize;      // size of queue
    unsigned int m_fpqNumItems;  // number of items in queue

  // Declare member functions.

  public:

    /**
     *
     * Default constructor.
     */
    MlePQ(void);

    /**
     * Constructor that takes an initial size.
     *
     * @param size The initial size to set the queue to.
     */
    MlePQ(unsigned int size);

    /**
     * The destructor.
     */
    virtual ~MlePQ(void);

    /**
     * @brief Insert a new item.
     *
     * This method will grow the queue if necessary.
     * Once extended, the queue will never shrink back to its original size
     * (it remains extended).
     *
     * @param item The item to insert into the queue.
     */
    virtual void insert(MlePQItem &item);

    /**
     * @brief Remove the highest priority item.
     *
     * If the action is successful,
     * "item" will contain the data referenced by the removed entry.
     *
     * @param item The item to remove from the queue.
     *
     * @return If the item is successfully removed, then TRUE will be
     * returned. Otherwise, FALSE will be removed.
     */
    virtual MlBoolean remove(MlePQItem &item);

    /**
     * @brief Remove all items for specified priority.
     *
     * If the action is successful,
     * "item" will point to an array containing all data matching the
     * requested priority.
     *
     * @param priority The prioority of the items which are to be removed.
     * @param items A pointer to an array of items matching the
     * requeted priority.
     * @param numItems A pointer to the number of items in the returned
     * arary.
     *
     * @return If the items are successfully removed, then TRUE will be
     * returned. Otherwise, FALSE will be removed.
     */
    virtual MlBoolean remove(int priority, MlePQItem **items,
                              unsigned int *numItems);

    /**
     * @brief Replace the specified item.
     *
     * Replace the highest priority item with a new one (unless the new
     * item is of higher priority).
     *
     * @param item The new item to replace.
     *
     * @return If the item is successfully replaced, then TRUE will be
     * returned. Otherwise, FALSE will be removed.
     */
    virtual MlBoolean replace(MlePQItem &item);

    /**
     * @brief Delete the highest priority item.
     *
     * @return If the item is successfully deleted, then TRUE will be
     * returned. Otherwise, FALSE will be removed.
     */
    virtual void destroy(void);

    /**
     * @brief Delete all items with matching "priority".
     *
     * @param priority The matching priority to use to delete
     * an item from the queue.
     *
     * @return If the item is successfully deleted, then TRUE will be
     * returned. Otherwise, FALSE will be removed.
     */
    virtual void destroy(int priority);

    /**
     * @brief Create and return a copy of the queue.
     */
    unsigned int copyQueue(MlePQItem **queue);

    /**
     * @brief Determine if an item with the specified "priority" is in queue.
     */
    virtual MlBoolean inQueue(int priority);

    /**
     * @brief Determine if a specified "item" is in queue.
     */
    virtual MlBoolean inQueue(MlePQItem &item);

#if defined(MLE_DEBUG)
    /**
     * @brief Print the queue.
     */
    void print(void);

    /**
     * @brief Print the queue - with sorted priorities.
     */
    void printSorted(void);
#endif /* MLE_DEBUG */

    /**
     * @brief Clear the queue.
     */
    virtual void clear(void);

    /**
     * @brief Find an item with specified priority.
     */
    virtual unsigned int findItem(int priority);

    virtual unsigned int findItem(MlePQItem &item);

    virtual unsigned int findItem(MlePQCallback func,void *clientData);

    /**
     * @brief Delete an arbitrary item.
     */
    virtual void destroyItem(unsigned int k);

    /**
     * @brief  Change the priority of an item.
     */
    virtual MlBoolean changeItem(unsigned int k,int priority);

    /**
     * @brief Peek into queue for specified item.
     */
    virtual MlBoolean peek(unsigned int k,MlePQItem &item);

    /**
     * @brief Get the number of items in the queue.
     */
    inline unsigned int getNumItems(void);

    /**
     * @brief Join two priority queues into one larger one.
     */
    static MlePQ *join(MlePQ &pq1,MlePQ &pq2);

    /**
     * @brief Sort the specified queue.
     *
     * The resulting queue will be sorted from
     * lowest to highest priority (queue[0] = lowest priority item and
     * queue[numItems] = highest priority item).
     */
    static void sort(MlePQItem *queue,int numItems);

    /**
     * Overload assignment operator.
     */
    void operator =(MlePQ &queue);

    /**
     * Override operator new.
     *
     * @param tSize The size, in bytes, to allocate.
     */
    void* operator new(size_t tSize);

    /**
     * Override operator delete.
     *
     * @param p A pointer to the memory to delete.
     */
    void  operator delete(void *p);

  protected:

    /**
     * @brief Extend the queue.
     */
    MlBoolean grow(void);
    MlBoolean grow(unsigned int size);

    /**
     * @brief Traverse through the heap from bottom to top,
     * reordering priorities.
     */
    void upHeap(unsigned int k);

    /*
     * @brief Traverse through the heap from top to bottom,
     * reordering priorities.
     */
    void downHeap(unsigned int k);

  private:

    MlBoolean _grow(unsigned int size);  // Grow queue by specified size.

};


// Define some useful macros.

#ifdef _WINDOWS
//#define MAXINT 32767
#if !defined(MAXINT)
    #define MAXINT INT_MAX
#endif
#endif /* _WINDOWS */
#if defined(__linux__) || defined(__APPLE__)
#define MAXINT INT_MAX
#endif /* __linux__ */
#define MLE_MAX_QPRIORITY MAXINT         // maximum priority
#define MLE_MIN_QPRIORITY ~MAXINT        // minimum priority
#define MLE_INC_QSIZE     64             // defualt size queue grows by


// Define inline methods.

inline unsigned int MlePQ::getNumItems(void)
{
    return(m_fpqNumItems);
}


#endif /* __MLE_PQ_H_ */

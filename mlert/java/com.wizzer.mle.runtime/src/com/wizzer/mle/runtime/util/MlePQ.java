/*
 * MlePQ.java
 * Created on Dec 9, 2004
 */

// COPYRIGHT_BEGIN
//
//  Copyright (C) 2000-2007  Wizzer Works
//
//  Wizzer Works makes available all content in this file ("Content").
//  Unless otherwise indicated below, the Content is provided to you
//  under the terms and conditions of the Common Public License Version 1.0
//  ("CPL"). A copy of the CPL is available at
//
//      http://opensource.org/licenses/cpl1.0.php
//
//  For purposes of the CPL, "Program" will mean the Content.
//
//  For information concerning this Makefile, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

// Declare package.
package com.wizzer.mle.runtime.util;

// Import standard Java packages.

/**
 * This class implements a priority queue.
 * 
 * @author Mark S. Millard
 */
public class MlePQ extends MlePriorityQueue
{
    /** The maximum priority. */
    public static final int MLE_MAX_QPRIORITY = 32767;
    /** The minimum priority. */
    public static final int MLE_MIN_QPRIORITY = ~32767;

    /**
     * The default constructor.
     */
    public MlePQ()
    {
        super(MLE_INC_QSIZE);
     }

    /**
     * Constructor that takes an initial size.
     *
     * @param size The initial size to set the queue to.
     */
    public MlePQ( int size)
    {
        super(size);
    }
    
    /**
     * Construct a priority queue from the specified elements.
     * 
     * @param elements The items to place in the queue.
     */
    public MlePQ(MlePQElement[] elements)
    {
        super(elements.length);
        for (int i = 0; i < elements.length; i++)
        {
            insert(elements[i]);
        }
    }
    
    /**
     * Insert a new item.
     * <p>
     * This method will grow the queue if necessary.
     * Once extended, the queue will never shrink back to its original size
     * (it remains extended).
     * </p>
     *
     * @param item The item to insert into the queue.
     */
    public void insert(MlePQElement item)
    {
        addElement(item);
    }
    
    /**
     * Remove the highest priority item.
     *
     * @return If an item is successfully removed, then it will be
     * returned. Otherwise, <b>null</b> will be returned.
     */
    public MlePQElement remove()
    {
        return (MlePQElement)getMaxElement();
     }

    /**
     * Remove all items with the specified priority.
     * 
     * @param priority The priority to match.
     * 
     * @return If any items are successfully removed, then they will be
     * returned in an array. Otherwise, <b>null</b> will be returned.
     */
    public MlePQElement[] remove(int priority)
    {
        int k, numFound = 0;
        MlePQElement[] foundQ;

        // Allocate enough space for potential "hit" list.
        foundQ = new MlePQElement[getNumElements()];

        // Find matching items.
        while((k = findItem(priority)) != -1)
        {
            foundQ[numFound++] = (MlePQElement)getElementAt(k);
            destroyItem(k);
        }

        if (numFound > 0)
        {
            MlePQElement[] result = new MlePQElement[numFound];
            System.arraycopy(foundQ,0,result,0,numFound);
            return result;
        } else
            return null;
    }
    
    /**
     * Check whether the queue is empty.
     * 
     * @return <b>true</b> is returned if the queue is empty. Otherwise,
     * <b>false</b> will be returned.
     */
    public boolean isEmpty()
    {
        return heapEmpty();
    }
   
    /**
     * Delete the specified item.
     * 
     * @param k The item to destroy.
     */
    public void destroyItem(int k)
    {
        int prevKey;

        // Check to see if there are any items in the queue.
        if ((k < 0) || (k > getNumElements()) || heapEmpty())
        {
            return;
        } else if (getNumElements() == 1)
        {
            decrementHeapsize();
            decrementNumElements();
            return;
        }

        prevKey = ((MlePQElement)getElementAt(k)).m_key;
        swap(k,getNumElements() - 1);
        decrementHeapsize();
        //decrementNumElements();
        if (((MlePQElement)getElementAt(k)).m_key < prevKey) flowDown(k);
        else if (((MlePQElement)getElementAt(k)).m_key > prevKey) flowUp(k);
        decrementNumElements();
    }

    public void destroy()
    {
        destroyItem(0);
    }


    public void destroy(int priority)
    {
        int k;

        // Delete all items of specified weight.
        while((k = findItem(priority)) != -1)
        {
            destroyItem(k);
        }
    }

    /**
     * Change the priority of an item.
     * 
     * @param k The item to change.
     * @param priority The new priority.
     */
    public boolean changeItem(int k, int priority)
    {
        boolean result = true;
        MlePQElement item = new MlePQElement(priority,null);

        // Check to see if there are any items in the queue.
        if ((k < 0) || (k > getNumElements()) || heapEmpty())
        {
            result = false;
        } else
        {
            item.m_key = priority;
            item.m_data = ((MlePQElement)getElementAt(k)).m_data;
            destroyItem(k);
            insert(item);
        }

        return result;
    }

    /**
     * Find an item with specified priority.
     * 
     * @param priority The priority to look for.
     * 
     * @return The index to the found item is returned.
     */
    public int findItem(int priority)
    {
        boolean notFound = true;
        int i = 0;

        // Find first item with specified priority.
        while((notFound) && (i < getNumElements()))
        {
            if (((MlePQElement)getElementAt(i)).m_key == priority) notFound = false;
            else i++;
        }

        if (notFound) return(-1);
        else return(i);
    }

    /**
     * Find an item matching the specified element.
     * 
     * @param item The element used to match.
     * 
     * @return The index to the found item is returned.
     */
    public int findItem(MlePQElement item)
    {
        boolean notFound = true;
        int i = 0;

        // Find first item with specified priority.
        while((notFound) && (i < getNumElements()))
        {
            if ((((MlePQElement)getElementAt(i)).m_key == item.m_key) &&
                (((MlePQElement)getElementAt(i)).m_data == item.m_data))
                notFound = false;
            else i++;
        }

        if (notFound) return(-1);
        else return(i);
    }
    
    /**
     * Copy the elements in the queue.
     * 
     * @return An array of the copied elements is returned.
     */
    public MlePQElement[] copyQueue()
    {
        MlePQElement[] queue;
        
        if (getNumElements() > 0)
        {
            queue = new MlePQElement[getNumElements()];
            for (int i = 0; i < getNumElements(); i++)
                queue[i] = (MlePQElement)getElementAt(i);
        } else queue = null;

        return(queue);
    }
    
    /**
     * Determine if an element exits in the queue
     * with the specified priority.
     * 
     * @param priority The priority to check for.
     * 
     * @return <b>true</b> is returned if an element exists in
     * the queue with the specified priority. Otherwise <b>false</b>
     * will be returned.
     */
    public boolean inQueue(int priority)
    {
        boolean result = true;

        if (findItem(priority) == -1)
            result = false;

        return result;
    }

    /**
     * Determine if an element exits in the queue
     * matching the specified element.
     * 
     * @param item The element to check for.
     * 
     * @return <b>true</b> is returned if an element exists in
     * the queue matching the specified item. Otherwise <b>false</b>
     * will be returned.
     */
    public boolean inQueue(MlePQElement item)
    {
        boolean result = true;

        if (findItem(item) == -1)
            result = false;

        return result;
    }


    /**
     * Join two priority queues into one larger one.
     * 
     * @param pq1 The first queue.
     * @param pq2 The second queue.
     * 
     * @return A new priority queue is returned containing elements
     * from queues <b>pq1</b> and <b>pq2</b>.
     */
    public static MlePQ join(MlePQ pq1,MlePQ pq2)
    {
        int i,numItems;
        MlePQ heap;
        MlePQElement[] tmpQ1, tmpQ2;

        // Construct a new queue.
        numItems = pq1.getNumElements() + pq2.getNumElements();
        if (numItems > 0)
        {
            heap = new MlePQ(numItems);
            if ((tmpQ1 = pq1.copyQueue()) != null)
                for (i = 0; i < pq1.getNumElements(); i++)
                    heap.insert(tmpQ1[i]);
            if ((tmpQ2 = pq2.copyQueue()) != null)
                for (i = 0; i < pq2.getNumElements(); i++)
                    heap.insert(tmpQ2[i]);
        } else
            heap = null;

        return(heap);
    }

}

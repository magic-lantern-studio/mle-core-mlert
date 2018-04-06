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

/**
 *  Implements a Priority Queue using a heap tree.
 */
public class MlePriorityQueue extends MleHeapArray
{  
    /**
     * A constructor that initializes with an empty array of length <b>len</b>.
     * 
     * @param len The initial capacity of the array.
     */
	public MlePriorityQueue(int len)
	{
	    super(len);
	}

	/**
	 * Add an element to the queue.
	 * 
	 * @param n The element to add.
	 */
    public void addElement(IMleElement n)
    {
        super.addElement(n);
        flowUp(m_lastHeap); 
    }

	/**
	 * Get and remove maximum element.
	 * 
	 * @return If queue is empty, <b>null</b> is returned.
	 */
	public IMleElement getMaxElement()
	{
	    if (heapEmpty()) 
	        return null;
	  
	    IMleElement maxNode = getElementAt(moveMax());
	    decrementNumElements();
	  
	    return maxNode;
	}
    
    /**
     * Peek into queue for specified item.
     * 
     * @param k The index of the element to check.
     * 
     * @return A reference to the item in the queue is returned
     * without actually removing it from the queue.
     */
    public IMleElement peek(int k)
    {
        IMleElement result = null;
        
        if ((k >= 0) && (k < getNumElements()))
        {
            return getElementAt(k);
        }
        
        return result;
    }
    
    /**
     * Move up a key until it satisfies the heap order.
     * 
     * @param k The index of the element to move up.
     */
    protected void flowUp(int k)
    {
	    // Swap the key at k with its parents along the path to the root
        // until it finds the place where the heap order is fulfilled.
        while (parent(k) != m_nullIndex && isGreaterThan(k, parent(k)))
        {
            swap(parent(k),k);
            k = parent(k);
        }
    }

    /**
     * Move down a key until it satisfies the heap order.
     * 
     * @param k The index of the element to move down.
     */
    protected void flowDown(int k)
    {
	    heapify(k);
    }

}

/*
 * MleHeapSortArray.java
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
 *  Sorts an array of element keys with the heapsort algorithm.
 */
public class MleHeapsortArray extends MleHeapArray
{  
    /**
     * A constructor that initializes with an empty array of length <b>len</b>.
     * 
     * @param len The capacity of the array.
     */
 	public MleHeapsortArray(int len)
	{
	    super(len);
	}

	/**
	 * Sort the input keys in increasing order.
     * This is the entry function for the heapsort algorithm.
     */
    public void heapsort()
    {
        buildHeap();
        sort();
    }

	/**
	 * Move all elements in decreasing key size order into the result list.
	 * <p>
	 * Precondition: the tree must be a heap.
	 * </p>
	 */
    protected void sort()
    {
        while (! heapEmpty())
        {
            moveMax();
        }
   }
    
}

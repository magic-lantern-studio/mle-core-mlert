/*
 * MleElementArray.java
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
 * Base class of <code>MleHeapArray</code>.
 * <p>
 * Contains an array of elements that can be compared and swapped.
 * It provides the type of array used by classical sorting algorithms like heapsort.
 * Functionality includes:
 * <ul>
 * <li>specialized access methods to elements</li>
 * <li>compare elements</li>
 * <li>swap elements</li>
 * <li>add an element</li>
 * <li>String of element strings</li>
 * </ul>
 */
public abstract class MleElementArray
{
    /** Defualt size queue grows by. */
    public static int MLE_INC_QSIZE = 64;

    // Array holds heap tree followed by the result list.
	private IMleElement[] m_array;
	// Index of last node in array.
	private int m_lastElement = -1;
   
    // Hide the default constructor.
    private MleElementArray() {}

    /**
     * A constructor that initializes with an empty array of length <b>len</b>.
     * 
     * @param len The capacity of the array.
     */
    public MleElementArray(int len)
    {
        m_array = new IMleElement[len];
    }
   
    /**
     * Get the array of elements as a string.
     * 
     * @return A <code>String</code> is returned.
     */
    public String toString()
    {
        StringBuffer s = new StringBuffer();
        for (int i = 0; i < m_array.length; i++)
        {
            s.append(m_array[i] + " ");
        }
        return "(" + s + ")";
    }
   
    /**
     * Add an element after the last element in the array.
     * 
     * @param n The element to add.
     */
    public void addElement(IMleElement n)
    {
        if (m_lastElement == (m_array.length - 1))
            grow(MLE_INC_QSIZE);
	    m_array[++m_lastElement] = n;
    }
   
    /**
     * Get the element at the specified index.
     * 
     * @param k The index of the element to get.
     * 
     * @return A reference to an <code>IMleElement</code> is returned.
     */
    public IMleElement getElementAt(int k)
    {
	    return m_array[k];
    }
    
    /**
     * Get the number of items in the queue.
     * 
     * @return The number of elements in the queue is returned.
     */
    public int getNumElements()
    {
        return m_lastElement + 1;
    }
   
    /** 
     * Register that the last element has been removed.
     */
    protected void decrementNumElements()
    {
        m_array[m_lastElement] = null;
	    m_lastElement--;
    }

    /**
     * Determines if element at index a greater than element at index b.
     * 
     * @param a The first element index.
     * @param b The second element index.
     * 
     * @return <b>true</b> is returned if the first element at index <b>a</b>
     * is greater than the second element at index <b>b</b>.
     */
    protected boolean isGreaterThan(int a, int b)
    {
        return m_array[a].isGreaterThan(m_array[b]);
    }
   
    /**
     * Swap the elements at the indices a and b in the array.
     * 
     * @param a The first element index.
     * @param b The second element index.
     */
    protected void swap(int a, int b)
    {
        IMleElement t = m_array[a];
        m_array[a] = m_array[b];
        m_array[b] = t;
    }

    /**
     * Increase the size of the array.
     * 
     * @param size The size to increase the array by.
     * 
     * @return <b>true</b> is returned if the array grew successfully.
     * Otherwise <b>false</b> is returned.
     */
    protected boolean grow(int size)
    {
        boolean result = true;
        
        // Allocate space for the new array size.
        int newSize = m_array.length + size;
        IMleElement[] newArray = new IMleElement[newSize];
        
        if (newArray != null)
        {
            if (m_array != null)
            {
                System.arraycopy(m_array,0,newArray,0,m_array.length);
                for (int i = 0; i < m_lastElement; i++)
                    m_array[i] = null;
                m_array = null;
            }
            m_array = newArray;
        }
        
        return result;
    }

}

/*
 * MlePQElement.java
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
 * An element as stored in the heap tree or the result list.
 * The sorting key is implemented as an <b>int</b>.
 */
public class MlePQElement implements IMleElement
{
    /** The sorting key. */
    public int m_key;
    /** The associated data. */
    public Object m_data = null;
  
    // Hide the default constructor.
    private MlePQElement() {}
    
    /**
     * A constructor that initializes the element.
     * 
     * @param key The sorting key.
     * @param data The element's associated data.
     */
    public MlePQElement(int key, Object data)
    {
        m_key = key;
        m_data = data;
    }
  
    /**
     * Determine if the specified element <b>b</b> is greater
     * than <b>this</b> element.
     * 
     * @param b The element to test.
     * 
     * @return <b>true</b> is returned if <b>this</b> element is greater
     * than element <b>b</b>. Otherwise, <b>false</b> will be returned.
     */
    public boolean isGreaterThan(IMleElement b)
    {
        return m_key > ((MlePQElement)b).m_key;
    }
  
    /**
     * Get the string representation of the element.
     * 
     * @return A <code>String</code> is returned.
     */
    public String toString()
    {
        return String.valueOf(m_key);
    }
}

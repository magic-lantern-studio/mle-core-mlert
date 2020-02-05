/*
 * PQTest.java
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
package com.wizzer.mle.runtime.unittest;

// Import JUnit classes.
import junit.framework.TestCase;

//Import Android classes.
import android.util.Log;

// Import Magic Lantern classes.
import com.wizzer.mle.runtime.MleTitle;
import com.wizzer.mle.runtime.util.IMleElement;
import com.wizzer.mle.runtime.util.MleElementArray;
import com.wizzer.mle.runtime.util.MlePQ;
import com.wizzer.mle.runtime.util.MlePQElement;

/**
 *  This class is a unit test for com.wizzer.mle.runtime.util.MlePQ.
 * 
 * @author Mark S. Millard
 */
public class PQTest extends TestCase
{
	/**
	 * Set up the test case.
	 * 
	 * @see TestCase#setUp()
	 */
    protected void setUp() throws Exception
    {
        super.setUp();
    }

	/**
	 * Tear down the test case.
	 * 
	 * @see TestCase#tearDown()
	 */
    protected void tearDown() throws Exception
    {
        super.tearDown();
    }

	/**
	 * Constructor for PQTest.
	 * 
	 * @param name The name of the unit test.
	 */
	public PQTest(String name)
	{
		super(name);
	}

	/**
	 *  Test element insertion/removal.
	 */
	public void testInsertionRemoval()
	{
	    MlePQ pq = new MlePQ(10);

	    Log.i(MleTitle.DEBUG_TAG,"Test 1: Simple Test");
	    //System.out.println("Expected / Received");
	  
	    pq.addElement(new MlePQElement(3,null));
	    IMleElement n3 = pq.getMaxElement();
	    //System.out.println("3 /" + n3);
	    TestCase.assertEquals(new Integer(3).toString(),n3.toString());
	  
	    pq.addElement(new MlePQElement(5,null));
	    pq.addElement(new MlePQElement(2,null));
	    MlePQElement n5 = (MlePQElement)pq.getMaxElement();
	    //System.out.println("5 / " + n5);
	    TestCase.assertEquals(new Integer(5).toString(),n5.toString());
	    MlePQElement n2 = (MlePQElement)pq.getMaxElement();
	    //System.out.println("2 / " + n2);
	    TestCase.assertEquals(new Integer(2).toString(),n2.toString());
	  
	    pq.addElement(new MlePQElement(4,null));
	    pq.addElement(new MlePQElement(7,null));
	    //System.out.println("7 / " + pq.getMaxElement());
	    //System.out.println("4 / " + pq.getMaxElement());
	    MlePQElement n7 = (MlePQElement)pq.getMaxElement();
	    MlePQElement n4 = (MlePQElement)pq.getMaxElement();
	    TestCase.assertEquals(new Integer(7).toString(),n7.toString());
	    TestCase.assertEquals(new Integer(4).toString(),n4.toString());
	}
	
	/**
	 *  Test queue growth.
	 */
	public void testQueueGrowth()
	{
	    MlePQ pq = new MlePQ(MleElementArray.MLE_INC_QSIZE);

	    Log.i(MleTitle.DEBUG_TAG,"Test 2: Queue Growth");
	    //System.out.println("Expected / Received");
	    
	    for (int i = 0; i < MleElementArray.MLE_INC_QSIZE; i++)
	    {
	        pq.addElement(new MlePQElement(i,null));
	    }
	    pq.addElement(new MlePQElement(MleElementArray.MLE_INC_QSIZE,null));
	    
	    for (int i =  MleElementArray.MLE_INC_QSIZE; i >= 0;  i--)
	    {
	        IMleElement element = pq.getMaxElement();
	        //System.out.println(i + " / " + element.toString());
	        TestCase.assertEquals(new Integer(i).toString(),element.toString());
	    }

	}
	
	/**
	 * Test the clear() method.
	 */
	public void testClear()
	{
	    MlePQ pq = new MlePQ(MleElementArray.MLE_INC_QSIZE);

	    Log.i(MleTitle.DEBUG_TAG,"Test 3: Clear Queue");
	    //System.out.println("Expected / Received");
	    
	    for (int i = 0; i < MleElementArray.MLE_INC_QSIZE; i++)
	    {
	        pq.addElement(new MlePQElement(i,null));
	    }
	    
	    pq.clear();
	    
	    TestCase.assertEquals(0,pq.getNumElements());
	}

	/**
	 * Test the destroyItem() method.
	 */
	public void testDestroyItem()
	{
	    MlePQ pq = new MlePQ(MleElementArray.MLE_INC_QSIZE);

	    Log.i(MleTitle.DEBUG_TAG,"Test 4: Destroy Item");
	    //System.out.println("Expected / Received");
	    
	    for (int i = 0; i < MleElementArray.MLE_INC_QSIZE; i++)
	    {
	        pq.addElement(new MlePQElement(i,null));
	    }
	    
	    pq.destroyItem(pq.findItem(10));
	    pq.destroyItem(pq.findItem(20));
	    pq.destroyItem(pq.findItem(30));
	    pq.destroyItem(pq.findItem(40));
	    TestCase.assertEquals(60,pq.getNumElements());
	    
	    int k = pq.findItem(10);
	    TestCase.assertEquals(-1,k);
	    k = pq.findItem(20);
	    TestCase.assertEquals(-1,k);
	    k = pq.findItem(30);
	    TestCase.assertEquals(-1,k);
	    k = pq.findItem(40);
	    TestCase.assertEquals(-1,k);
	}
	
	/**
	 * Test the remove() methods.
	 */
	public void testRemove()
	{
	    MlePQ pq = new MlePQ(10);
	    
	    Log.i(MleTitle.DEBUG_TAG,"Test 5: Remove Item.");
	    
	    for (int i = 0; i < 10; i++)
	        pq.insert(new MlePQElement(i,null));
	    pq.insert(new MlePQElement(5,null));
	    pq.insert(new MlePQElement(5,null));
	    pq.insert(new MlePQElement(5,null));
	    pq.insert(new MlePQElement(5,null));
	    
	    IMleElement element = pq.remove();
	    TestCase.assertEquals(new Integer(9).toString(),element.toString());
	    
	    IMleElement[] elements = pq.remove(5);
	    for (int i = 0; i < 5; i++)
	        TestCase.assertEquals(new Integer(5).toString(),elements[i].toString());
	}
	
	/**
	 * Test the join() method.
	 */
	public void testJoin()
	{
	    MlePQ pq1 = new MlePQ();
	    MlePQ pq2 = new MlePQ();
	    MlePQ result;

	    Log.i(MleTitle.DEBUG_TAG,"Test 5: Join Queues.");
	    
	    for (int i = 0; i < 10; i++)
	    {
	        pq1.insert(new MlePQElement(i,null));
	        pq2.insert(new MlePQElement(i+25,null));
	    }
	    
	    result = MlePQ.join(pq1,pq2);
	    
	    IMleElement element = result.remove();
	    TestCase.assertEquals(new Integer(34).toString(),element.toString());
	}
	
	/**
	 * Test the changeItem() method.
	 */
	public void testChangeItem()
	{
	    MlePQ pq = new MlePQ(10);
	    
	    Log.i(MleTitle.DEBUG_TAG,"Test 6: Change Item.");
	    
	    for (int i = 0; i < 10; i++)
	        pq.insert(new MlePQElement(i,null));

	    pq.changeItem(pq.findItem(5),56);
	    
	    IMleElement element = pq.remove();
	    TestCase.assertEquals(new Integer(56).toString(),element.toString());
	}

	
	/**
	 * Test the destroy() methods.
	 */
	public void testDestroy()
	{
	    MlePQ pq = new MlePQ(10);
	    
	    Log.i(MleTitle.DEBUG_TAG,"Test 7: Destroy Item.");
	    
	    for (int i = 0; i < 10; i++)
	        pq.insert(new MlePQElement(i,null));

	    // Destroy item with priority 5.
	    pq.destroy(5);
	    
	    int k = pq.findItem(5);
	    TestCase.assertEquals(-1,k);
	    
	    // Destroy the top 3 priority items.
	    pq.destroy();
	    pq.destroy();
	    pq.destroy();
	    
	    IMleElement element = pq.remove();
	    TestCase.assertEquals(new Integer(6).toString(),element.toString());
	}

}

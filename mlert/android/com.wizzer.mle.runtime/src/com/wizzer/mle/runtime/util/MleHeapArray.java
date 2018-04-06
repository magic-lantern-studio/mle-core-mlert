/*
 * MleHeapArray.java
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
 * This class contains a heap tree and result list.
 * It provides the following functionality:
 * <ul>
 * <li>specialized access methods to elements</li>
 * <li>test for emptiness of heaptree</li>
 * <li>swap</li>
 * </ul>
 */
public abstract class MleHeapArray extends MleElementArray
{
    // Heap tree root stored at index 0.
    final static int m_rootIndex = 0;
    final static int m_nullIndex = m_rootIndex - 1;
    // Points to last element in heap. Initialize for empty heap.
    int m_lastHeap = m_nullIndex;
   
    /**
     * A constructor that initializes with an empty array of length <b>len</b>.
     * 
     * @param len The initial capacity of the array.
     */
	public MleHeapArray(int len)
	{
	    super(len);
	}
	
	/**
	 * Add an element to the array.
	 * 
	 * @param n The element to add.
	 */
	public void addElement(IMleElement n)
    {
        super.addElement(n);
        incrementHeapsize(); 
    }

   // For debug: return heapTree formatted as a tree.
   protected StringBuffer heapTreeString()
   {
      int ns = m_rootIndex;   // Index of start element in current level.
      int ne = leftIndex(ns); // Index of start element in next level.
      int spa = 7; // Number of spaces before first element.
      int spb = 1; // Number of spaces between elements.
      
      StringBuffer s = new StringBuffer();
      for (int level = 0; level <= 3; level ++)
      {
         s.append(space(spa)); // Space before first element.
         for (int ni = ns; ni < ne; ni++)  // ni: indices of elements in current level.
         {
             // Get Nodes string representation; space if no node any more.
             String node = at(ni) != m_nullIndex ? getElementAt(ni).toString() : " ";
             s.append(node + space(spb)); // append node and space between nodes.
         } 
         s.append("\n");
         ns = leftIndex(ns) ;
         ne = leftIndex(ne) ;
         spb = spa;
         spa = (spa+1) / 2 -1;
      }
      
      return s;
   }
   
   // For debug: return a string of len spaces.
   private String space(int len)
   {
       return "                                                                                   ".substring(1,len+1);
   }
   
   // For debug: left index of child element.
   private int leftIndex(int k)
   {
       // Because root is at 0. Root at 1 gives 2*k.
       return 2 * k + 1;
   }

    /**
     * Build the heap order for the whole tree.
     */
   protected void buildHeap()
    { 
	    // Start at last non-leaf node. Process all nodes in reverse storage
        // order until we reach the root by this order, we ensure that heapify's
        // precondition is fulfilled.
	    for (int k = lastInnerElement(); k != m_nullIndex; k = predesessor(k))
	    {
	        heapify(k);
        }
    }

	/**
	 * Heapify the subtree located at index <b>k</b>.
	 * <p>
	 * Precondition: both of k's child trees are heap ordered.
	 * </p>
	 * 
	 * @param k The index of the subtree.
	 */
    protected void heapify(int k)
    { 
		// Move the key down the tree till we're done.
		while( k != m_nullIndex)
		{
		    k = heapifyLocally(k);
		};     
    }
    
	/**
	 * Heap order element k with respect to both its children.
	 * 
	 * @param k The index of the subtree.
	 * 
	 * @return If keys had to be swapped, return the element where k.m_key now is.
	 * Otherwise, <m>null</m> will be returned.
	 */
    protected int heapifyLocally(int k)
    {
       int mc = maxChild(k);
       
       if (mc == m_nullIndex)
       {
           // k is leaf, we're done.
           return m_nullIndex;
       }
       
       if (isGreaterThan(mc,k))
       {
           // If max child has bigger key then swap.
           swap(k,mc);
           return mc;
       } else
       {
           return m_nullIndex;
       }
    }

	/** 
	 * Move the maximum node from the heap into the result list.
	 * <p>
	 * Precondition: the tree must be a heap.
	 * </p>
	 * 
	 * @return Return index where node was placed in result list
	 */
    protected int moveMax()
    {
        swap(m_rootIndex,m_lastHeap); // Move maximum element to result list.
        decrementHeapsize();          // Heap is now one element smaller.
        heapify(m_rootIndex);         // Restore the heap.
        return m_lastHeap + 1;
    }
   
    /**
     * Determine if heap is empty.
     * 
     * @return <b>true</b> is returned if the heap is empty. Otherwise,
     * <b>false</b> is returned.
     */
    protected boolean heapEmpty()
    {
        return m_lastHeap == m_nullIndex;
    }
   
   /**
    * A node has been moved out of the heap. Register this by decrementing the heapsize counter.
    */
   protected void decrementHeapsize()
   {
	   m_lastHeap--;
   }

   /**
    * Increment the heap size.
    */
   protected void incrementHeapsize()
   {
	   m_lastHeap++;
   }

   /**
    * Get the last inner element in the tree.
    * 
    * @return Return last inner element (has no children) in heap,
    * nullIndex is returned if there is no such element. 
    */
   protected int lastInnerElement()
   {
       return heapEmpty()? m_nullIndex : parent(m_lastHeap);
   }
   
   /**
    * Utiltiy for determining validity of index.
    * 
    * @param index The index to test.
    * 
    * @return If index is in heap, return it unchanged, otherwise return nullIndex.
    */
   protected int at(int index)
   {
       return index >= m_rootIndex && index <= m_lastHeap ? index : m_nullIndex;
   }
   
   /**
    * Get the predessor of the specified element.
    * 
    * @param k The index of the element to get the predessor of.
    * 
    * @return Return the predessor of element k in storage order,
    * nullIndex is returned if k is the root.
    */ 
   protected int predesessor(int k)
   {
       return at(k - 1);
   }
   
   /**
    * Get the child on the left of the heap tree.
    * 
    * @param k The index of the parent element.
    * 
    * @return Return left child of element k, nullIndex if there is no such child.
    */
   protected int leftChild(int k)
   {
       // Because root is at 0. Root at 1 gives 2*k.
       return at(2 * k + 1);
   }
   
   /**
    * Get the child on the right of the heap tree.
    * 
    * @param k The index of the parent element.
    * 
    * @return Return right child of element k, nullIndex if there is no such child.
    */
   protected int rightChild(int k)
   {
       // Because root is at 0. Root at 1 gives 2*k +1.
       return at(2 * k + 2);
   }
   
   /** 
    * Get the parent element of the specified element as index <b>k</b>.
    * 
    * @param k The index of the element to get the parent of.
    * 
    * @return Return parent element of element k. Return nullIndex if k is the root.
    */
   protected int parent(int k)
   {
       // Because root is at 0. Root at 1 gives k/2.
       // parent(root) is now root. This makes ?: necessary
       return k == m_rootIndex ? m_nullIndex : at((k-1) / 2);
   }
   
    /**
     * Get the child with the highest priority.
     * <p>
     * Precondition: heap must be complete.
     * </p>
     * 
     * @param k The element to obtain the child from.
     * 
     * @return Return the bigger child of element k. Return nullIndex if k is a leaf.
     */
    protected int maxChild(int k)
    {
        int rc = rightChild(k);
        int lc = leftChild(k);
     
        if (rc == m_nullIndex)
        {
            return lc;
        } else
        {
            // Because heap is complete there must be a left child.
            return isGreaterThan(lc, rc)? lc : rc;
        }
    }
   
    /**
     * Clear the queue.
     */
    public void clear()
    {
        if (heapEmpty()) return;
        
        IMleElement node = getElementAt(moveMax());
	    do
	    {
	        decrementNumElements();
	        if (heapEmpty()) break;
	    } while ((node = getElementAt(moveMax())) != null);
    }
}
   

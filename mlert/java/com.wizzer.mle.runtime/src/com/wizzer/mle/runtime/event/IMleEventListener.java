/**
 * IMleEventListener.java
 * Created on October 12, 2000 (msm@wizzerworks.com)
 * Modified on April 28, 2003 (msm@wizzerworks.com)
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


// Delcare package.
package com.wizzer.mle.runtime.event;

// Import standard Java packages.
import java.util.*;


/**
 * <code>IMleEventListener</code> is used to encapsulate a Magic Lantern
 * listener for the <code>MleEvent</code> events.
 *
 * @author  Mark S. Millard
 * @version 1.0
 */
public interface IMleEventListener extends EventListener
{
   /**
    * Invoked when the <code>MleEvent</code> is processed.
    * 
    * @param event The event that has been processed by the <code>MleEventDispatcher</code>.
    */
	public void eventProcessed(MleEvent event);

   /**
    * Invoked when the <code>MleEvent</code> is dispatched.
    * 
    * @param event The event that has been dispatched by the <code>MleEventDispatcher</code>.
    */
	public void eventDispatched(MleEvent event);
}

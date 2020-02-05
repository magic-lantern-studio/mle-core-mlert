/*
 * MleRuntimeException.java
 * Created on Aug 30, 2004
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
package com.wizzer.mle.runtime.core;

/**
 * This class is an <code>Exception</code> used to manage errors that occur
 * while executing a Magic Lantern application.
 * 
 * @author Mark S. Millard
 */
public class MleRuntimeException extends Exception
{
	/**
	 * Constructs a new exception with <b>null</b> as its detail message.
	 * <p>
	 * The cause is not initialized, and may subsequently be initialized by a call to
	 * <code>Throwable.initCause(java.lang.Throwable)</code>.
	 * </p>
	 */
	public MleRuntimeException()
	{
		super();
	}
	
	/**
	 * Constructs a new exception with the specified detail message.
	 * <p>
	 * The cause is not initialized, and may subsequently be initialized by a call to
	 * <code>Throwable.initCause(java.lang.Throwable)</code>.
	 * </p>
	 * 
	 * @param message The detail message. The detail message is saved for later retrieval
	 * by the <code>Throwable.getMessage()</code> method.

	 */
	public MleRuntimeException(String message)
	{
		super(message);
	}

}

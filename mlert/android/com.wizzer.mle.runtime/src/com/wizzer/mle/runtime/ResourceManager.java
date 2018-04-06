/*
 * ResourceManager.java
 * Created on Jan 9, 2008
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
package com.wizzer.mle.runtime;

// Import standard Java classes.
import java.lang.reflect.Field;

// Import Android classes.
import android.util.Log;

// Import Magic Lantern classes.
import com.wizzer.mle.runtime.core.MleRuntimeException;

/**
 * This class provides utility for managing Android platform resources.
 * 
 * @author Mark Millard
 */
public class ResourceManager
{
	/** The output tag for the Android Log utility. */
	static final public String OUTPUT_TAG = "Magic Lantern Resource Manager";
	
	/**
	 * Get the resource identifier from the specified Android class, <b>R</b>.
	 * The following categories (subclasses of R) are valid:
	 * <ul>
	 * <li>R.drawable</li>
	 * <li>R.layout</li>
	 * <li>R.raw</li>
	 * <li>R.string</li>
	 * <ul>
	 * <p>
	 * 
	 * @param resource The Android <b>R</b> class.
	 * @param name The name of the identifier (e.g. "icon")
	 * 
	 * @return The identifier for the specified <i>name</i> is returned. This
	 * will be the integer value extracted from the Android class <code>R</code>.
	 * 
	 * @throws MleRuntimeException This exception is thrown if no identifier
	 * is found matching the specified <i>name</i>.
	 */
	static public int getResourceId(Class resource, String name)
		throws MleRuntimeException
	{
		int id = 0;

     	// The id for the resource is extracted from the Android R class using reflection.
    	// The name of the id MUST match the argument name (e.g. if the expected
    	// id is R.drawable.icon, then the value of name should be icon).
		try
		{
			int start = resource.getName().lastIndexOf('$') + 1;
			String className = resource.getName().substring(start);
    		if (className.equals("drawable"))
    		{
    			// Class is R.drawable
            	Field field = resource.getField(name);
            	id = field.getInt(null);
    		} else if (className.equals("layout"))
    		{
            	// Class is R.layout
            	Field field = resource.getField(name);
            	id = field.getInt(null);
    		} else if (className.equals("raw"))
    		{
            	// Class is R.raw.
            	Field field = resource.getField(name);
            	id = field.getInt(null);
    		} else if (className.equals("string"))
    		{
            	// Class is R.string
            	Field field = resource.getField(name);
            	id = field.getInt(null);
    		} else
    		{
    			String msg = "Invalid resource class " + className + ".";
    			Log.e(OUTPUT_TAG, msg);
    			throw new MleRuntimeException(msg);
    		}
    	} catch (NoSuchFieldException ex)
        {
    		String msg = "Resource " + name + " does not exist for class " + resource.getName() + ".";
    		Log.e(OUTPUT_TAG, msg);
    		throw new MleRuntimeException(msg);
        } catch (IllegalAccessException ex)
        {
        	String msg = "Illegal access to field " + name + " for class " + resource.getName() + ".";
        	Log.e(OUTPUT_TAG, msg);
        	throw new MleRuntimeException(msg);
        }
        
        return id;
	}
}

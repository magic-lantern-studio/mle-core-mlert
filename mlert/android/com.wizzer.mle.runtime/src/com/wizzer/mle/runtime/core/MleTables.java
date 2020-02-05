/*
 * MleTables.java
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

// Import standard Java packages.
import java.util.Vector;
import java.util.Observable;


/**
 * This class is used to define and register the tables generated for a
 * Magic Lantern application.
 * 
 * @author Mark S. Millard
 */
public class MleTables extends Observable
{
    // The Singleton instance of the tables.
    private static MleTables g_theTables = null;
    
    /** The collection of Actor properties. */
    public static Vector g_mleRTActorProperties = null;
    /** The collection of Set properties. */
    public static Vector g_mleRTSetProperties = null;
    /** The collection of Actor classes. */
    public static Vector g_mleRTActorClass = null;
    /** The collection of Role classes. */
    public static Vector g_mleRTRoleClass = null;
    /** The collection of Set classes. */
    public static Vector g_mleRTSetClass = null;
    /** The collection of Sets. */
    public static Vector g_mleRTSet = null;
    /** The collection of Group classes. */
    public static Vector g_mleRTGroupClass = null;
    /** The collection of Scene classes. */
    public static Vector g_mleRTSceneClass = null;
    /** The collection of MediaRefs. */
    public static Vector g_mleRTMediaRef = null;
	/**
	 * This table gives the playprint chunk number for the corresponding
	 * set in the mleRTSet table.
	 */
	public static int g_mleRTSetChunk[] = null;
    /**
     * This table gives the playprint chunk number for the corresponding
     * group in the mleRTGroup table.
     */
	public static int g_mleRTGroupChunk[]  = null;
	/**
	 * This table gives the playprint chunk number for the corresponding
	 * scene in the mleRTScene table.
	 */
	public static int g_mleRTSceneChunk[] = null;
	/**
	 * The boot Scene.
	 */
	public static int g_mleBootScene = -1;

    // Hide the default constructor.
    private MleTables()
    {
        super();
        g_mleRTActorProperties = new Vector();
        g_mleRTSetProperties = new Vector();
        g_mleRTActorClass = new Vector();
        g_mleRTRoleClass = new Vector();
        g_mleRTSetClass = new Vector();
        g_mleRTSet = new Vector();
        g_mleRTGroupClass = new Vector();
        g_mleRTSceneClass = new Vector();
        g_mleRTMediaRef = new Vector();
    }
    
    /**
     * Get the Singleton instance of the table manager.
     * 
     * @return A reference to the <code>MleTables</code> is returned.
     */
    public static MleTables getInstance()
    {
        if (g_theTables == null)
            g_theTables = new MleTables();
        return g_theTables;
    }

    /**
	 * This class is a runtime Actor/Set Property Table Entry.
	 */
	public static class MleRTPropertyEntry
	{
	    public MleRTPropertyEntry(String classname, String fieldname)
	    {
	        m_classname = classname;
	        m_fieldname = fieldname;
	    }
	    
        /**
         * Get the property field.
         * 
         * @return The name of the field is returned.
         */
	    public String getProperty()
	    {
	        return m_fieldname;
	    }
	    
	    // The name of the Class.
	    public String m_classname;
	    // The name of the Field.
	    public String m_fieldname;
	}

	/**
	 * This class is a runtime Actor Type Table Entry.
	 */
	public static class MleRTActorClassEntry
	{
	    public MleRTActorClassEntry(String classname, int offset)
	    {
	        m_classname = classname;
	        m_offset = offset;
	    }
	    
	    public Object createActor()
	        throws MleRuntimeException
	    {
	        Object actor = null;
	        try
	        {
	            Class actorClass = Class.forName(m_classname);
	            actor = actorClass.newInstance();
	        } catch (ClassNotFoundException ex)
	        {
	            throw new MleRuntimeException(ex.getMessage());
	        } catch (InstantiationException ex)
	        {
	            throw new MleRuntimeException(ex.getMessage());
	        } catch (IllegalAccessException ex)
	        {
	            throw new MleRuntimeException(ex.getMessage());
	        }
	        return actor;
	    }
	     
        /** The class name for invoking the default constructor. */
	    public String m_classname;
        /** The property table offset. */
	    public int m_offset;
	}

	/**
	 * This class is a runtime Role Type Table Entry
	 */
	public static class MleRTRoleClassEntry
	{
	    public MleRTRoleClassEntry(String classname)
	    {
	        m_classname = classname;
	    }
	    
	    public Object createRole(MleActor actor)
        	throws MleRuntimeException
        {
	        Object role = null;
	        try
	        {
	            Class roleClass = Class.forName(m_classname);
	            role = roleClass.newInstance();
	            ((MleRole)role).setActor(actor);
	        } catch (ClassNotFoundException ex)
	        {
	            throw new MleRuntimeException(ex.getMessage());
	        } catch (InstantiationException ex)
	        {
	            System.out.println(ex.getMessage());
	            throw new MleRuntimeException(ex.getMessage());
	        } catch (IllegalAccessException ex)
	        {
	            throw new MleRuntimeException(ex.getMessage());
	        }
	        return role;
        }
	    
	    /** The class name for invoking the default constructor. */
	    public String m_classname;
	}

	/**
	 * This class is a runtime Set Type Table Entry.
	 */
	public static class MleRTSetClassEntry
	{
	    public MleRTSetClassEntry(String classname, int offset)
	    {
	        m_classname = classname;
	        m_offset = offset;
	    }
	    
	    public Object createSet()
	        throws MleRuntimeException
	    {
	        Object set = null;
	        try
	        {
	            Class setClass = Class.forName(m_classname);
	            set = setClass.newInstance();
	        } catch (ClassNotFoundException ex)
	        {
	            throw new MleRuntimeException(ex.getMessage());
	        } catch (InstantiationException ex)
	        {
	            throw new MleRuntimeException(ex.getMessage());
	        } catch (IllegalAccessException ex)
	        {
	            throw new MleRuntimeException(ex.getMessage());
	        }
	        return set;
	    }
	    
	    /** The class name for invoking the default constructor. */
	    public String m_classname;
	    /** The property table offset. */
	    public int m_offset;
	}

	/**
	 * This class is a runtime Set Table Entry.
	 */
	public static class MleRTSetEntry
	{
	    public MleRTSetEntry(String classname, MleSet set)
	    {
	        m_classname = classname;
	        m_theSet = set;
	    }
	    
	    /** The class name for invoking the default constructor. */
	    public String m_classname;
	    /** A reference to the instance. */
	    public MleSet m_theSet;
	}

	/**
	 * This class is a runtime Group Type Table Entry.
	 */
	public static class MleRTGroupClassEntry
	{
	    public MleRTGroupClassEntry(String classname)
	    {
	         m_classname = classname;
	    }

	    public Object createGroup()
        	throws MleRuntimeException
        {
	        Object group = null;
	        try
	        {
	            Class groupClass = Class.forName(m_classname);
	            group = groupClass.newInstance();
	        } catch (ClassNotFoundException ex)
	        {
	            throw new MleRuntimeException(ex.getMessage());
	        } catch (InstantiationException ex)
	        {
	            throw new MleRuntimeException(ex.getMessage());
	        } catch (IllegalAccessException ex)
	        {
	            throw new MleRuntimeException(ex.getMessage());
	        }
	        return group;
        }

	    /** The class name for invoking the default constructor. */
	    public String m_classname;
	};

	/**
	 * This class is a runtime Scene Type Table Entry.
	 */
	public static class MleRTSceneClassEntry
	{
	    public MleRTSceneClassEntry(String classname)
	    {
	        m_classname = classname;
	    }

	    public Object createScene()
        	throws MleRuntimeException
        {
	        Object scene = null;
	        try
	        {
	            Class sceneClass = Class.forName(m_classname);
	            scene = sceneClass.newInstance();
	        } catch (ClassNotFoundException ex)
	        {
	            throw new MleRuntimeException(ex.getMessage());
	        } catch (InstantiationException ex)
	        {
	            throw new MleRuntimeException(ex.getMessage());
	        } catch (IllegalAccessException ex)
	        {
	            throw new MleRuntimeException(ex.getMessage());
	        }
	        return scene;
        }
	     
	    /** The class name for invoking the default constructor. */
	    public String m_classname;
	}

	/**
	 * This class is a runtime Media Reference Table Entry.
	 */
	public static class MleRTMediaRefEntry
	{
	    public MleRTMediaRefEntry(String classname)
	    {
	        m_classname = classname;
	    }
	    
	    public Object createMediaRef()
    		throws MleRuntimeException
        {
	        Object mref = null;
	        try
	        {
	            Class mrefClass = Class.forName(m_classname);
	            mref = mrefClass.newInstance();
	        } catch (ClassNotFoundException ex)
	        {
	            throw new MleRuntimeException(ex.getMessage());
	        } catch (InstantiationException ex)
	        {
	            throw new MleRuntimeException(ex.getMessage());
	        } catch (IllegalAccessException ex)
	        {
	            throw new MleRuntimeException(ex.getMessage());
	        }
	        return mref;
	    }
	    
	    /** The class name for invoking the default constructor. */
	    public String m_classname;
	}
	
	public boolean addActorProperty(MleRTPropertyEntry property)
	    throws MleRuntimeException
	{
	    boolean retValue = true;
	    
	    if (! (property instanceof MleRTPropertyEntry))
	        throw new MleRuntimeException("Not an Actor property.");
	    g_mleRTActorProperties.addElement(property);
	    
	    // Notfiy observers of change.
	    this.setChanged();
	    this.notifyObservers(property);
	    
	    return retValue;
	}
	
	public boolean removeActorProperty(MleRTPropertyEntry property)
        throws MleRuntimeException
	{
	    boolean retValue = false;
	    
	    if (! (property instanceof MleRTPropertyEntry))
	        throw new MleRuntimeException("Not an Actor property.");
	    retValue = g_mleRTActorProperties.removeElement(property);
	    
	    // Notfiy observers of change.
	    this.setChanged();
	    this.notifyObservers(property);
	    
	    return retValue;
	}
	
	public boolean addSetProperty(MleRTPropertyEntry property)
        throws MleRuntimeException
	{
	    boolean retValue = true;
	    
	    if (! (property instanceof MleRTPropertyEntry))
	        throw new MleRuntimeException("Not a Set property.");
	    g_mleRTSetProperties.addElement(property);
	    
	    // Notfiy observers of change.
	    this.setChanged();
	    this.notifyObservers(property);
	    
	    return retValue;
	}
	
	public boolean removeSetProperty(MleRTPropertyEntry property)
        throws MleRuntimeException
	{
	    boolean retValue = false;
	    
	    if (! (property instanceof MleRTPropertyEntry))
	        throw new MleRuntimeException("Not a Set property.");
	    retValue = g_mleRTSetProperties.removeElement(property);
	    
	    // Notfiy observers of change.
	    this.setChanged();
	    this.notifyObservers(property);
	    
	    return retValue;
	}
	
	public boolean addActorClass(MleRTActorClassEntry clazz)
        throws MleRuntimeException
	{
	    boolean retValue = true;
	    
	    if (! (clazz instanceof MleRTActorClassEntry))
	        throw new MleRuntimeException("Not a Actor class.");
	    g_mleRTActorClass.addElement(clazz);
	    
	    // Notfiy observers of change.
	    this.setChanged();
	    this.notifyObservers(clazz);
	    
	    return retValue;
	}
	
	public boolean removeActorClass(MleRTActorClassEntry clazz)
        throws MleRuntimeException
	{
	    boolean retValue = false;
	    
	    if (! (clazz instanceof MleRTActorClassEntry))
	        throw new MleRuntimeException("Not a Actor class.");
	    retValue = g_mleRTActorClass.removeElement(clazz);
	    
	    // Notfiy observers of change.
	    this.setChanged();
	    this.notifyObservers(clazz);
	    
	    return retValue;
	}
	
	public boolean addRoleClass(MleRTRoleClassEntry clazz)
        throws MleRuntimeException
	{
	    boolean retValue = true;
	    
	    if (! (clazz instanceof MleRTRoleClassEntry))
	        throw new MleRuntimeException("Not a Role class.");
	    g_mleRTRoleClass.addElement(clazz);
	    
	    // Notfiy observers of change.
	    this.setChanged();
	    this.notifyObservers(clazz);
	    
	    return retValue;
	}
	
	public boolean removeRoleClass(MleRTRoleClassEntry clazz)
        throws MleRuntimeException
	{
	    boolean retValue = false;
	    
	    if (! (clazz instanceof MleRTRoleClassEntry))
	        throw new MleRuntimeException("Not a Role class.");
	    retValue = g_mleRTRoleClass.removeElement(clazz);
	    
	    // Notfiy observers of change.
	    this.setChanged();
	    this.notifyObservers(clazz);
	    
	    return retValue;
	}
	
	public boolean addSetClass(MleRTSetClassEntry clazz)
        throws MleRuntimeException
	{
	    boolean retValue = true;
	    
	    if (! (clazz instanceof MleRTSetClassEntry))
	        throw new MleRuntimeException("Not a Set class.");
	    g_mleRTSetClass.addElement(clazz);
	    
	    // Notfiy observers of change.
	    this.setChanged();
	    this.notifyObservers(clazz);
	    
	    return retValue;
	}
	
	public boolean removeSetClass(MleRTSetClassEntry clazz)
        throws MleRuntimeException
	{
	    boolean retValue = false;
	    
	    if (! (clazz instanceof MleRTSetClassEntry))
	        throw new MleRuntimeException("Not a Set class.");
	    retValue = g_mleRTSetClass.removeElement(clazz);
	    
	    // Notfiy observers of change.
	    this.setChanged();
	    this.notifyObservers(clazz);
	    
	    return retValue;
	}
	
	public boolean addGroupClass(MleRTGroupClassEntry clazz)
        throws MleRuntimeException
	{
	    boolean retValue = true;
	    
	    if (! (clazz instanceof MleRTGroupClassEntry))
	        throw new MleRuntimeException("Not a Group class.");
	    g_mleRTGroupClass.addElement(clazz);
	    
	    // Notfiy observers of change.
	    this.setChanged();
	    this.notifyObservers(clazz);
	    
	    return retValue;
	}
	
	public boolean removeGroupClass(MleRTGroupClassEntry clazz)
        throws MleRuntimeException
	{
	    boolean retValue = false;
	    
	    if (! (clazz instanceof MleRTGroupClassEntry))
	        throw new MleRuntimeException("Not a Group class.");
	    retValue = g_mleRTGroupClass.removeElement(clazz);
	    
	    // Notfiy observers of change.
	    this.setChanged();
	    this.notifyObservers(clazz);
	    
	    return retValue;
	}
	
	public boolean addSceneClass(MleRTSceneClassEntry clazz)
        throws MleRuntimeException
	{
	    boolean retValue = true;
	    
	    if (! (clazz instanceof MleRTSceneClassEntry))
	        throw new MleRuntimeException("Not a Scene class.");
	    g_mleRTSceneClass.addElement(clazz);
	    
	    // Notfiy observers of change.
	    this.setChanged();
	    this.notifyObservers(clazz);
	    
	    return retValue;
	}
	
	public boolean removeRoleClass(MleRTSceneClassEntry clazz)
        throws MleRuntimeException
	{
	    boolean retValue = false;
	    
	    if (! (clazz instanceof MleRTSceneClassEntry))
	        throw new MleRuntimeException("Not a Scene class.");
	    retValue = g_mleRTSceneClass.removeElement(clazz);
	    
	    // Notfiy observers of change.
	    this.setChanged();
	    this.notifyObservers(clazz);
	    
	    return retValue;
	}
	
	public boolean addSet(MleRTSetEntry set)
        throws MleRuntimeException
	{
	    boolean retValue = true;
	    
	    if (! (set instanceof MleRTSetEntry))
	        throw new MleRuntimeException("Not a Set.");
	    g_mleRTSet.addElement(set);
	    
	    // Notfiy observers of change.
	    this.setChanged();
	    this.notifyObservers(set);
	    
	    return retValue;
	}
	
	public boolean removeSet(MleRTSetEntry set)
        throws MleRuntimeException
	{
	    boolean retValue = false;
	    
	    if (! (set instanceof MleRTSetEntry))
	        throw new MleRuntimeException("Not a Set.");
	    retValue = g_mleRTSet.removeElement(set);
	    
	    // Notfiy observers of change.
	    this.setChanged();
	    this.notifyObservers(set);
	    
	    return retValue;
	}
	
	public boolean addMediaRef(MleRTMediaRefEntry mediaref)
        throws MleRuntimeException
	{
	    boolean retValue = true;
	    
	    if (! (mediaref instanceof MleRTMediaRefEntry))
	        throw new MleRuntimeException("Not a MediaRef.");
	    g_mleRTMediaRef.addElement(mediaref);
	    
	    // Notfiy observers of change.
	    this.setChanged();
	    this.notifyObservers(mediaref);
	    
	    return retValue;
	}
	
	public boolean removeMediaRef(MleRTMediaRefEntry mediaref)
        throws MleRuntimeException
	{
	    boolean retValue = false;
	    
	    if (! (mediaref instanceof MleRTMediaRefEntry))
	        throw new MleRuntimeException("Not a MediaRef.");
	    retValue = g_mleRTMediaRef.removeElement(mediaref);
	    
	    // Notfiy observers of change.
	    this.setChanged();
	    this.notifyObservers(mediaref);
	    
	    return retValue;
	}
	
	/**
	 * Register the specified Magic Lantern Object.
	 * <p>
	 * Note that this class does not actually manage the registration
	 * of the MLE Object. Instead it passes the Object on to any
	 * Observers so that they can determine wether to register the
	 * Object or not.
	 * </p>
	 * 
	 * @param obj The MLE Object to register.
	 */
	public void registerObject(IMleObject obj)
	{
	    if (this.countObservers() > 0)
	    {
	        // Notfiy observers of change.
	        this.setChanged();
	        this.notifyObservers(obj);
	    }
	}

	/**
	 * Unregister the specified Magic Lantern Object.
	 * <p>
	 * Note that this class does not actually manage the registration
	 * of the MLE Object. Instead it passes the Object on to any
	 * Observers so that they can determine wether to unregister the
	 * Object or not.
	 * </p>
	 * 
	 * @param obj The MLE Object to unregister.
	 */
	public void unregisterObject(IMleObject obj)
	{
	    if (this.countObservers() > 0)
	    {
		    // Notfiy observers of change.
		    this.setChanged();
		    this.notifyObservers(obj);
	    }
	}

}

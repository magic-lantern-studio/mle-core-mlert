/*
 * MleDppModel.java
 * Created on Aug 25, 2004
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
package com.wizzer.mle.runtime.dpp;

/**
 * This class supplies utility for managing Magic Lantern Digital Playprint items.
 * 
 * @author Mark S. Millard
 */
public class MleDppModel
{
    /**
     * The default constructor.
     */
    public MleDppModel()
    {
        super();
    }

    /**
     * This class is used for reading/writing a chunk file header.
     */
	public static class MleDppChunkFileHeader
	{
	    public long m_id;       // Chunk file identifier.
	    public long m_version;  // Version of chunk file.
	    public long m_time;     // Creation time stamp.
	    public long m_crc;      // Total CRC.
	    
	    /**
	     * The default constructor.
	     */
	    public MleDppChunkFileHeader()
	    {
	        m_id = 0;
	        m_version = 0;
	        m_time = 0;
	        m_crc = 0;
	    }
	}

	 /**
	  * This class is used for reading/writing a Cast/Group chunk.
	  */
	public static class MleDppGroupChunk
	{
	    public long m_flags;
	    public long m_size;
	    public byte[] m_data = null;

	    /**
	     * The default constructor.
	     */
	    public MleDppGroupChunk()
	     {
	         m_flags = 0;
	         m_size = 0;
	     }
	}

	/**
	 * This class is used for reading/writing a Scene chunk.
	 */
	public static class MleDppSceneChunk
	{
	    public long m_flags;
	    public long m_size;
	    public byte[] m_data = null;
	    
	    /**
	     * The default constructor.
	     */
	    public MleDppSceneChunk()
	     {
	         m_flags = 0;
	         m_size = 0;
	     }
	}

	/**
	 * This class is used for reading/writing a Set chunk.
	 */
	 public static class MleDppSetChunk
	 {
	     public long m_flags;
	     public long m_size;
	     public byte[] m_data = null;
	     
	     /**
	      * The default constructor.
	      */
	     public MleDppSetChunk()
	     {
	         m_flags = 0;
	         m_size = 0;
	     }
	 }

	 /**
	  * This class is used for reading/writing a MediaRef 'info' chunk.
	  */
	 public static class MleDppMediaRefInfoChunk
	 {
	     public long m_flags;
	     public long m_type;     // Index into runtime class constructor table.
	     public long m_numMrefs; // Number of MediaRef chunks in MRFL LIST.
	     
	     /**
	      * The default constructor.
	      */
	     public MleDppMediaRefInfoChunk()
	     {
	         m_flags = 0;
	         m_type = 0;
	         m_numMrefs = 0;
	     }
	 }

	 /**
	  * This class is used for reading/writing a MediaRef 'mref' chunk.
	  */
	 public static class  MleDppMediaRefChunk
	 {
	     public long m_flags;
	     public long m_type;
	     public long m_size;
	     public byte[] m_data = null;
	     
	     /**
	      * The default constructor.
	      */
	     public MleDppMediaRefChunk()
	     {
	         m_flags = 0;
	         m_type = 0;
	         m_size = 0;
	     }
	 }
}

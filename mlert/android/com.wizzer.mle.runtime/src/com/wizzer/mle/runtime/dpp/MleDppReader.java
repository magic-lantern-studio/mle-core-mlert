/*
 * MleDppReader.java
 * Created on Aug 19, 2004
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

// Import standard Java packages.
import java.util.Vector;
import java.io.IOException;

// Import Magic Lantern Runtime packages.
import com.wizzer.mle.runtime.core.InputStream;
import com.wizzer.mle.runtime.util.MleUnsigned;


/**
 * This class is a Magic Lantern Digital Playprint reader.
 * 
 * @author Mark S. Millard
 */
public class MleDppReader
{
    /** Open the DPP for reading. */
    public static final String MLE_DPP_READING_MODE = "r";
    
    /** The Digital Playprint file. */
    protected IMleRandomAccess m_file;
    /** Swap on read. */
    protected boolean m_swapRead = false;
    /** Use the Table-of-Contents. */
    protected boolean m_useTOC = false;
    /** The Table-of-Contents. */
    protected MleDppTOC m_TOC = null;
    /** The number of reserved entries in the TOC. */
    protected int m_numTOCReserved = 0;
    /** The number of used entries in the TOC. */
    protected int m_numTOCUsed = 0;
    /** A FORM INFO chunk. */
    protected MleDppUtil.MleDppChunkInfo formInfo = new MleDppUtil.MleDppChunkInfo();
    
    /**
     * This class is a Magic Lantern Digital Playprint Table-of-Contents.
     * 
     * @author Mark S. Millard
     */
    protected class MleDppTOC
    {
        /**
         * This class is an entry in the Magic Lantern Digital Playprint's
         * Table-of-Contents.
         * 
         * @author Mark S. Millard
         */
        public class MleDppTOCEntry
        {
            /** The label for the TOC entry. */
            public long m_label;
            /** The offset into the Digital Playprint for the TOC entry. */
            public long m_offset;
            
            /**
             * The default constructor.
             */
            public MleDppTOCEntry() {}
        }

        /** The list of Table-of-Contents entries. */
        public Vector m_entries = new Vector();
        
        /**
         * The default constructor.
         */
        public MleDppTOC() {}
    }
    
    // Hide the default constructor.
    private MleDppReader() {}

    /**
     * Construct a Magic Lantern Digital Playprint for the specified file.
     * The file will be opened in read-only mode.
     * 
     * @param filename The name of the Digital Playprint.
     * 
     * @throws MleDppException This exception is thrown if the Digital Playprint
     * can not be opened.
     */
    public MleDppReader(String filename)
        throws MleDppException
	{
	    // Initialize Digial Playprint book keeping structures.
	
	    // Open the file.
        // XXX - RandomAccessFile is not available in J2ME CLDC 1.0 configuration.
	    //m_file = new RandomAccessFile(filename,MLE_DPP_READING_MODE);
    	m_file = new MleRandomAccessFile(filename);
	}
    
    /**
     * Construct a Magic Lantern Digital Playprint for the specified file.
     * The file will be opened in read-only mode.
     * 
     * @param stream The stream containing the Digital Playprint.
     * 
     * @throws MleDppException This exception is thrown if the Digital Playprint
     * can not be opened.
     */
    public MleDppReader(InputStream stream)
    	throws MleDppException
    {
    	m_file = new MleRandomAccessStream(stream);
    }
    
    /**
     * Set the byte-ordering flag.
     * 
     * @param swap If <b>true</b>, then the bytes will be swapped on reading the data
     * from the Digital Playprint. Otherwise the bytes will remaing unchanged.
     */
    public void setSwap(boolean swap)
    {
        // Set byte ordering flag.
        setSwapRead(swap);
    }
    
    /**
     * Get the byte-ordering flag.
     * 
     * @return <b>true</b> will be returned if the bytes will be swapped on reading the data
     * from the Digital Playprint. Otherwise <b>false</b> will be returned.
     */
    public boolean getSwapRead()
    {
        return m_swapRead;
    }

    /**
     * Set the byte-ordering flag for reading.
     * 
     * @param swap If <b>true</b>, then the bytes will be swapped on reading the data
     * from the Digital Playprint. Otherwise the bytes will remaing unchanged.
     */
    public void setSwapRead(boolean swap)
    {
        m_swapRead = swap;
        MleDppUtil.setSwapRead(swap);
    }

    /**
     * Begin reading the Digital Playprint.
     * 
     * @param type The type of Digital Playprint to read. Valid types include:
     * <li>
     * <ul>MleDppUtil.MLE_PPBE_TAG</ul>
     * <ul>MleDppUtil.MLE_PPLE_TAG</ul>
     * </li>
     * 
     * @return <b>true</b> is returned if the Digital Playprint is valid and ready
     * to be read. Otherwise, <b>false</b> will be returned.
     * 
     * @throws MleDppException This exception is thrown if the DPP can not be read.
     */
    protected boolean begin(int type)
        throws MleDppException
    {
    	boolean retValue = true;
 
        // Make sure that the DPP is still open.
        if (m_file == null)
            throw new MleDppException("DPP has been closed.");

        // Initialize the Playprint file for reading.
        formInfo.m_form = MleDppUtil.dppMakeTag('M','L','R','T');
        MleDppUtil.MleDppChunkInfo info = MleDppUtil.dppDescend(m_file,formInfo,null,type);
        if (info == null)
        	retValue = false;
 
        return retValue;
    }
    
    /**
     * Wrap-up reading the Digital Playprint.
     * 
     * @return <b>true</b> is returned if the Digital Playprint is valid and no longer
     * to be read. Otherwise, <b>false</b> will be returned.
     * 
     * @throws MleDppException This exception is thrown if the DPP can not be read.
     *
     */
    protected boolean end() throws MleDppException
    {
    	boolean status = false;

        // Make sure that the DPP is still open.
        if (m_file == null)
            throw new MleDppException("DPP has been closed.");

        // Wrap-up Playprint book keeping.
        status = MleDppUtil.dppAscend(m_file,formInfo,0);
        
        // Close the file.
        try
        {
            m_file.close();
            m_file = null;
        } catch (IOException ex)
        {
            throw new MleDppException("Unable to close DPP.");
        }

        return status;
    }
    
    /**
     * Read the header from the Digital Playprint.
     * 
     * @return A reference to the <code>MleDppUtil.MleDppHeader</code> is returned.
     * 
     * @throws MleDppException This exception is thrown if the header can not be read.
     */
    protected MleDppUtil.MleDppHeader readHeader()
        throws MleDppException
    {
        MleDppUtil.MleDppChunkInfo hdrInfo = new MleDppUtil.MleDppChunkInfo();
        MleDppUtil.MleDppHeader chunkData = new MleDppUtil.MleDppHeader();
        
        // Make sure that the DPP is still open.
        if (m_file == null)
            throw new MleDppException("DPP has been closed.");

        hdrInfo = MleDppUtil.dppDescend(m_file,hdrInfo,null,0);
        if (hdrInfo.m_chunk.m_tag == MleDppUtil.dppMakeTag('h','d','r',' '))
        {
            // Read the chunk file header.
            byte[] data = new byte[(int)hdrInfo.m_chunk.m_size];
            try
            {
                m_file.readFully(data,0,data.length);
            } catch (IOException ex)
            {
                throw new MleDppException("Unable to read DPP header.");
            }
            chunkData.m_version = MleUnsigned.readDWORD(data,0);
            chunkData.m_date = MleUnsigned.readDWORD(data,4);

            if (m_swapRead)
            {
                chunkData.m_version = MleDppUtil.dppSwapLong((int)chunkData.m_version);
                chunkData.m_date = MleDppUtil.dppSwapLong((int)chunkData.m_date);
            }
        }

        MleDppUtil.dppAscend(m_file,hdrInfo,0);

        return chunkData;
    }

    /**
     * Begin processing the Table-of-Contents.
     * 
     * @param reserved The number of reserved entries in the TOC.
     * 
     * @return <b>true</b> is returned the the TOC can be processed. Otherwise,
     * <b>false</b> will be returned.
     * 
     * @throws MleDppException This exception is thrown if the specified number
     * of reserved entries is less than 0.
     */
    protected boolean beginTOC(int reserved)
        throws MleDppException
    {
        if (reserved <= 0)
            throw new MleDppException("Number of reserved TOC entries must be greater than 0.");

        m_TOC = new MleDppTOC();
        m_numTOCReserved = reserved;
        m_useTOC = true;
        
        return true;
    }
    
    /**
     * Add an entry to the Table-of-Contents.
     * 
     * @param label The label for the TOC entry.
     * @param offset An offset into the Digital Playprint indicating the labeled cunk.
     * 
     * @return <b>true</b> is returned if the entry is successfully added to the TOC.
     * Otherwise, <b>false</b> will be returned.
     * 
     * @throws MleDppException This exception is thrown if the TOC is unavailable, or
     * if the TOC is full.
     */
    protected boolean addTOCEntry(long label, long offset)
        throws MleDppException
    {
        boolean retValue = true;
        MleDppTOC.MleDppTOCEntry newEntry;

        // Check to see if we are using the TOC.
        if (! m_useTOC)
        {
            throw new MleDppException("DPP TOC is unavailable.");
        }

        // Check to see if the TOC is full.
        if (m_numTOCUsed == m_numTOCReserved)
        {
            throw new MleDppException("Reserved DPP TOC entries are full.");
        }

        newEntry = m_TOC.new MleDppTOCEntry();
        if (newEntry != null)
        {
            // Add new entry to table of contents.
            newEntry.m_label = label;
            newEntry.m_offset = offset;
            
            m_TOC.m_entries.addElement(newEntry);
            m_numTOCUsed++;
        } else
            throw new MleDppException("Unable to allocate a new DPP TOC entry.");

        return(retValue);
    }
    
    /**
     * Wrap-up processing the Table-of-Contents.
     * 
     * @return <b>true</b> is always returned.
     */
    protected boolean endTOC()
    {
        return true;
    }
    
    /**
     * Get the Digital Playprint file.
     * 
     * @return A reference to the <code>IMleRandomAccess</code> is returned.
     */
    public IMleRandomAccess getFile()
    {
        return m_file;
    }
}

/*
 * MleDppInput.java
 * Created on Aug 20, 2004
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

// Impot standard Java packages.
import java.io.IOException;

// Import Magic Lantern Runtime packages.
import com.wizzer.mle.runtime.core.InputStream;
import com.wizzer.mle.runtime.event.MleEvent;
import com.wizzer.mle.runtime.event.MleEventCallback;
import com.wizzer.mle.runtime.util.MleUnsigned;


/**
 * This class is used to read input from a Magic Lantern Digital Playprint file.
 * 
 * @author Mark S. Millard
 */
public class MleDppInput extends MleDppReader
{
    /** The DPP Input Read Event. */
    public static final int MLE_DPP_EVENT_READ = MleEvent.makeId((short)0xFF,(short)0x01);
    
    // IFF book keeping structure.
    private MleDppUtil.MleDppChunkInfo m_mrefListInfo = new MleDppUtil.MleDppChunkInfo();
   
    /**
     * A constructor that initializes the file to read as input.
     * 
     * @param filename The Digital Playprint.
     * 
     * @throws MleDppException This exception is thrown if the DPP can not
     * be initialized.
     */
    public MleDppInput(String filename) throws MleDppException
    {
        super(filename);
    }
    
    /**
     * A constructor that initializes the stream to read as input.
     * 
     * @param stream The Digital Playprint.
     * 
     * @throws MleDppException This exception is thrown if the DPP can not
     * be initialized.
     */
    public MleDppInput(InputStream stream) throws MleDppException
    {
    	super(stream);
    }
   
    /**
     * Read IFF 'hdr' chunk.
     * 
     * @param cb An event callback.
     * @param clientData Data associated with the callback.
     * 
     * @throws MleDppException This exception is thrown if the 'hdr' chunk
     * can not be successfully read from the Digital Playprint.
     */
    public boolean readHeader(MleEventCallback cb, Object clientData)
        throws MleDppException
    {
        boolean retValue = false;
        MleDppUtil.MleDppHeader hdrData;

        // Read Playprint header.
        hdrData = super.readHeader();

        // Process callback.
        if (cb != null)
        {
            // Create a new DPP event.
            MleEvent event = new MleEvent(this,MLE_DPP_EVENT_READ,hdrData);
            // And dispatch it.
            retValue = cb.dispatch(event,clientData);
        } else
            retValue = true;
        
        return retValue;
    }

    /**
     * Read IFF 'grp' chunk.
     * 
     * @param cb An event callback.
     * @param clientData Data associated with the callback.
     * 
     * @throws MleDppException This exception is thrown if the 'grp' chunk
     * can not be successfully read from the Digital Playprint.
     */
    public boolean readGroup(MleEventCallback cb, Object clientData)
        throws MleDppException
    {
        boolean retValue = false;

        MleDppUtil.MleDppChunkInfo groupInfo = new MleDppUtil.MleDppChunkInfo();

        groupInfo = MleDppUtil.dppDescend(m_file,groupInfo,null,0);
        if (groupInfo.m_chunk.m_tag == MleDppUtil.dppMakeTag('g','r','p',' '))
        {
            // Process callback.
            if (cb != null)
            {
                byte[] calldata = new byte[(int)groupInfo.m_chunk.m_size];
                try
                {
                    m_file.readFully(calldata,0,(int)groupInfo.m_chunk.m_size);
                } catch (IOException ex)
                {
                    throw new MleDppException("Unable to read DPP set chunk.");
                }
                
                // Create a new DPP event.
                MleEvent event = new MleEvent(this,MLE_DPP_EVENT_READ,calldata);
                // And dispatch it.
                retValue = cb.dispatch(event,clientData);
                
                calldata = null;
            } else
                retValue = true;
        }
        
        MleDppUtil.dppAscend(m_file,groupInfo,0);
        
        return retValue;
    }

    /**
     * Begin IFF 'MRFL' LIST chunk.
     * 
     * @param cb An event callback.
     * @param clientData Data associated with the callback.
     * 
     * @throws MleDppException This exception is thrown if the 'MRFL' chunk
     * can not be successfully read from the Digital Playprint.
    */
    public boolean beginMref(MleEventCallback cb, Object clientData)
        throws MleDppException
    {
        boolean retValue = false;
        MleDppModel.MleDppMediaRefInfoChunk mrefInfoData;

        // Begin IFF 'MRFL' LIST chunk.
        beginMrefList();

        // Read IFF 'info' chunk.
        mrefInfoData = readMrefInfo();

        // Process callback.
        if (cb != null)
        {
            // Create a new DPP event.
            MleEvent event = new MleEvent(this,MLE_DPP_EVENT_READ,mrefInfoData);
            // And dispatch it.
            retValue = cb.dispatch(event,clientData);
        } else
            retValue = true;
        
        return retValue;
    }

    /**
     * End IFF 'MRFL' LIST chunk.
     * 
     * @param cb An event callback.
     * @param clientData Data associated with the callback.
     * 
     * @throws MleDppException This exception is thrown if the 'MRFL' chunk
     * can not be successfully read from the Digital Playprint.
     */
    public boolean endMref(MleEventCallback cb, Object clientData)
        throws MleDppException
    {
        boolean retValue = false;

        // End IFF 'MRFL' LIST chunk.
        endMrefList();

        // Process callback.
        if (cb != null)
        {
            // Create a new DPP event.
            MleEvent event = new MleEvent(this,MLE_DPP_EVENT_READ,null);
            // And dispatch it.
            retValue = cb.dispatch(event,clientData);
        } else
            retValue = true;
        
        return retValue;
    }
    
    /**
     * Read IFF 'mref' chunk.
     * 
     * @param cb An event callback.
     * @param clientData Data associated with the callback.
     * 
     * @throws MleDppException This exception is thrown if the 'mref' chunk
     * can not be successfully read from the Digital Playprint.
     */
    public boolean readMedia(MleEventCallback cb, Object clientData)
        throws MleDppException
    {
        boolean retValue = true;
        MleDppModel.MleDppMediaRefChunk mrefData = new MleDppModel.MleDppMediaRefChunk();

        // Read IFF 'mref' chunk.
        mrefData = readMref();

        // Process callback.
        if (cb != null)
        {
            // Create a new DPP event.
            MleEvent event = new MleEvent(this,MLE_DPP_EVENT_READ,mrefData);
            // And dispatch it.
            retValue = cb.dispatch(event,clientData);
        } else
            retValue = true;
        
        // Clean up.
        mrefData.m_data = null;
        
        return retValue;
    }

    /**
     * Read IFF 'set' chunk.
     * 
     * @param cb An event callback.
     * @param clientData Data associated with the callback.
     * 
     * @throws MleDppException This exception is thrown if the 'set' chunk
     * can not be successfully read from the Digital Playprint.
     */
    public boolean readSet(MleEventCallback cb, Object clientData)
        throws MleDppException
    {
        boolean retValue = false;

        MleDppUtil.MleDppChunkInfo setInfo = new MleDppUtil.MleDppChunkInfo();

        setInfo = MleDppUtil.dppDescend(m_file,setInfo,null,0);
        if (setInfo.m_chunk.m_tag == MleDppUtil.dppMakeTag('s','e','t',' '))
        {
            // Process callback.
            if (cb != null)
            {
                byte[] calldata = new byte[(int)setInfo.m_chunk.m_size];
                try
                {
                    m_file.readFully(calldata,0,(int)setInfo.m_chunk.m_size);
                } catch (IOException ex)
                {
                    throw new MleDppException("Unable to read DPP set chunk.");
                }
                
                // Create a new DPP event.
                MleEvent event = new MleEvent(this,MLE_DPP_EVENT_READ,calldata);
                // And dispatch it.
                retValue = cb.dispatch(event,clientData);
                
                calldata = null;
            } else
                retValue = true;
        }
        
        MleDppUtil.dppAscend(m_file,setInfo,0);
        
        return retValue;
    }

    /**
     * Read IFF 'scn' chunk.
     * 
     * @param cb An event callback.
     * @param clientData Data associated with the callback.
     * 
     * @throws MleDppException This exception is thrown if the 'scn' chunk
     * can not be successfully read from the Digital Playprint.
     */
    public boolean readScene(MleEventCallback cb, Object clientData)
        throws MleDppException
    {
        boolean retValue = false;
        MleDppUtil.MleDppChunkInfo sceneInfo = new MleDppUtil.MleDppChunkInfo();

        MleDppUtil.dppDescend(m_file,sceneInfo,null,0);
        if (sceneInfo.m_chunk.m_tag == MleDppUtil.dppMakeTag('s','c','n',' '))
        {
            // Process callback.
            if (cb != null)
            {
                byte[] calldata = new byte[(int)sceneInfo.m_chunk.m_size];
                try
                {
                    m_file.readFully(calldata,0,(int)sceneInfo.m_chunk.m_size);
                } catch (IOException ex)
                {
                    throw new MleDppException("Unable to read DPP scn chunk.");
                }
                
                // Create a new DPP event.
                MleEvent event = new MleEvent(this,MLE_DPP_EVENT_READ,calldata);
                // And dispatch it.
                retValue = cb.dispatch(event,clientData);
                
                calldata = null;
            } else
                retValue = true;
        }

       MleDppUtil.dppAscend(m_file,sceneInfo,0);
       
       return retValue;
    }
    
    /**
     * Read IFF 'toc' chunk.
     * 
     * @param cb An event callback.
     * @param clientData Data associated with the callback.
     * 
     * @throws MleDppException This exception is thrown if the 'toc' chunk
     * can not be successfully read from the Digital Playprint.
     */
    public boolean readTOC(MleEventCallback cb, Object clientData)
        throws MleDppException
    {
        boolean retValue = false;
        MleDppUtil.MleDppChunkInfo tocInfo = new MleDppUtil.MleDppChunkInfo();
        MleDppTOC.MleDppTOCEntry entry;
        int numEntries;

        tocInfo = MleDppUtil.dppDescend(m_file,tocInfo,null,0);
        if (tocInfo.m_chunk.m_tag == MleDppUtil.dppMakeTag('t','o','c',' '))
        {
            // Read number of TOC entries and initialize TOC.
            try
            {
                numEntries = m_file.readInt();
            } catch (IOException ex)
            {
                throw new MleDppException("Unable to read DPP TOC entries.");
            }
            beginTOC(numEntries);

            // Read each TOC entry.
            for (int i = 0; i < numEntries; i++)
            {
                try
                {
	                int label, offset;
	                label = m_file.readInt();
	                offset = m_file.readInt();
	                addTOCEntry(label,offset);
                } catch (IOException ex)
                {
                    throw new MleDppException("Unable to add DPP TOC entry.");
                }
            }

            // Finish TOC reading.
            endTOC();

            // Process callback.
            if (cb != null)
            {
                // Create a new DPP event.
                MleEvent event = new MleEvent(this,MLE_DPP_EVENT_READ,null);
                // And dispatch it.
                retValue = cb.dispatch(event,clientData);
            } else
                retValue = true;
        }

        MleDppUtil.dppAscend(m_file,tocInfo,0);
        
        return retValue;
        
    }

    /**
     * Get chunk offset from table-of-content.
     * 
     * @param index The index into the TOC.
     * 
     * @return The offset into the DPP is returned. <b>-1</b> is returned if
     * not Table-of-Content exists.
     * 
     * @throws MleDppException This exception is thrown if the specified index
     * is out of range
     */
    public long getTOCOffset(long index)
        throws MleDppException
    {
        MleDppTOC.MleDppTOCEntry entry;

        if (index >= m_numTOCUsed)
            throw new MleDppException("DPP TOC index out of range.");

        if (m_TOC != null)
            entry = (MleDppTOC.MleDppTOCEntry)m_TOC.m_entries.elementAt((int)index);
        else
            return(-1);

        return(entry.m_offset);
        
    }

    /**
     * Go to location within Playprint.
     * 
     * @param tocOffset The offset into the DPP.
     */
    public void goTo(long tocOffset)
        throws MleDppException
    {
        try
        {
            MleDppUtil.seek(m_file,tocOffset,0);
        } catch (IOException ex)
        {
            throw new MleDppException("Unable to seek to location " + tocOffset + " in DPP.");
        }
    }

    // Begin reading the 'MLFL' LIST chunk.
    private boolean beginMrefList()
        throws MleDppException
    {
        boolean retValue = true;
        
        m_mrefListInfo.m_form = MleDppUtil.dppMakeTag('M','R','F','L');
        m_mrefListInfo = MleDppUtil.dppDescend(m_file,m_mrefListInfo,null,0);
         return retValue;
    }

    // Wrap-up reading the 'MRFL' LIST chunk.
    private boolean endMrefList()
        throws MleDppException
    {
        boolean retValue = true;
        
        MleDppUtil.dppAscend(m_file,m_mrefListInfo,0);
       
        return retValue;
    }
    
    // Read the MediaRef 'info' chunk.
    private MleDppModel.MleDppMediaRefInfoChunk readMrefInfo()
        throws MleDppException
    {
        MleDppModel.MleDppMediaRefInfoChunk chunkData = new MleDppModel.MleDppMediaRefInfoChunk();
        MleDppUtil.MleDppChunkInfo mediaInfo = new MleDppUtil.MleDppChunkInfo();

        mediaInfo = MleDppUtil.dppDescend(m_file,mediaInfo,null,0);
        if (mediaInfo.m_chunk.m_tag == MleDppUtil.dppMakeTag('i','n','f','o'))
        {
            // Read the 'info' data.
            byte[] data = new byte[12];
            try
            {
                m_file.readFully(data,0,data.length);
            } catch (IOException ex)
            {
                throw new MleDppException("Unable to read DPP MediaRef info chunk.");
            }
            chunkData.m_flags = MleUnsigned.readDWORD(data,0);
            chunkData.m_type = MleUnsigned.readDWORD(data,4);
            chunkData.m_numMrefs = MleUnsigned.readDWORD(data,8);

            if (getSwapRead())
            {
                chunkData.m_flags = MleDppUtil.dppSwapLong((int)chunkData.m_flags);
                chunkData.m_type = MleDppUtil.dppSwapLong((int)chunkData.m_type);
                chunkData.m_numMrefs = MleDppUtil.dppSwapLong((int)chunkData.m_numMrefs);
            }
        }

        MleDppUtil.dppAscend(m_file,mediaInfo,0);

        return chunkData;
    }

    // Read the MediaRef 'mref' chunk.
    private MleDppModel.MleDppMediaRefChunk readMref()
        throws MleDppException
    {
        MleDppModel.MleDppMediaRefChunk chunkData = new MleDppModel.MleDppMediaRefChunk();
        MleDppUtil.MleDppChunkInfo mediaInfo = new MleDppUtil.MleDppChunkInfo();

        mediaInfo = MleDppUtil.dppDescend(m_file,mediaInfo,null,0);
        if (mediaInfo.m_chunk.m_tag == MleDppUtil.dppMakeTag('m','r','e','f'))
        {
            // Read the 'mref' data.
            byte[] data = new byte[12];
            try
            {
                m_file.readFully(data,0,data.length);
            } catch (IOException ex)
            {
                throw new MleDppException("Unable to read DPP MediaRef mref chunk.");
            }
            chunkData.m_flags = MleUnsigned.readDWORD(data,0);
            chunkData.m_type = MleUnsigned.readDWORD(data,4);
            chunkData.m_size = MleUnsigned.readDWORD(data,8);

            if (getSwapRead())
            {
                chunkData.m_flags = MleDppUtil.dppSwapLong((int)chunkData.m_flags);
                chunkData.m_type = MleDppUtil.dppSwapLong((int)chunkData.m_type);
                chunkData.m_size = MleDppUtil.dppSwapLong((int)chunkData.m_size);
            }

            if (chunkData.m_flags == 0)
                // External MediaRef; data is a filename. Unlike the C++ version
                // of the loader, we do not have to add a '\0' terminator.
                chunkData.m_data = new byte[(int)chunkData.m_size];
            else
                chunkData.m_data = new byte[(int)chunkData.m_size];
            try
            {
                m_file.readFully(chunkData.m_data,0,chunkData.m_data.length);
            } catch (IOException ex)
            {
                throw new MleDppException("Unable to read DPP MediaRef mref chunk.");
            }

            /*
            if (chunkData.m_flags == 0)
            {
                // Terminate filename.
                chunkData.m_data[(int)chunkData.m_size] = '\0';
                chunkData.m_size += 1;
            }
            */
        }

        MleDppUtil.dppAscend(m_file,mediaInfo,0);

        return chunkData;
    }
   
}

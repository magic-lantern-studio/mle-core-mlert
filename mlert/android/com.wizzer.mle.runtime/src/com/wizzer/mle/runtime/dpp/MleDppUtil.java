/*
 * MleDppUtil.java
 * Created on Aug 18, 2004
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
import java.io.IOException;

// Import Magic Lantern Runtime Engine packages.
import com.wizzer.mle.runtime.util.MleUnsigned;


/**
 * This class supplies utility for managing a Magic Lantern Digital Workprint.
 * 
 * @author Mark S. Millard
 */
public class MleDppUtil
{
    /** A Big Endian Digital Playprint. */
    public static final int MLE_BIG_ENDIAN_FORMAT = 1;
    /** A Little Endian Digital Playprint. */
    public static final int MLE_LITTLE_ENDIAN_FORMAT = 2;
    
    /** Tag for Big Endian Digital Playprint. */
    public static final int MLE_PPBE_TAG = dppMakeTag('P','P','B','E');
    /** Tag for Little Endian Digital Playprint. */
    public static final int MLE_PPLE_TAG = dppMakeTag('P','P','L','E');
    /** Tag for a LIST chunk. */
    public static final int MLE_LIST_TAG = dppMakeTag('L','I','S','T');
    
    /** Find the chunk. */
    public static final int MLE_DPP_FINDCHUNK = 0x0010;
    /** Find the PPBE chunk. */
    public static final int MLE_DPP_FINDPPBE  = 0x0020;
    /** Find the LIST chunk. */
    public static final int MLE_DPP_FINDLIST  = 0x0040;
    /** Find the PPLE chunk. */
    public static final int MLE_DPP_FINDPPLE  = 0x0080;

    // The Digital Playprint byte format.
    private static int g_byteFormat = MLE_LITTLE_ENDIAN_FORMAT;
    
    // Flag indicating whether byte swapping is required on read.
    private static boolean g_swapRead = false;
    
    // Seek from current file position.
    static final int SEEK_CUR = 0x00;
    // Seek from the beginning of the file.
    static final int SEEK_SET = 0x01;
    // Seek from the end of the file.
    static final int SEEK_END = 0x02;

    /**
     * This class is a helper class for managing a Digital Playprint generic chunk.
     * 
     * @author Mark S. Millard
     */
    public static class MleDppChunk
    {
        /** The chunk ID. */
        public long m_tag;   // A MleDppTag as an unsigned int (4 bytes).
        /** The size of the data. */
        public long m_size;  // An unsigned int (4 bytes).
        
	    /*
	     *  Note: the actual data of the chunk follows the "size" field.
	     *        This information is here for reference only.
	     *  byte  data[size];
	     */
        
        /**
         * The default constructor.
         */
        public MleDppChunk()
        {
            m_tag = 0;
            m_size = 0;
        }
    }
    
    /**
     * This class is a helper class for managing a Digital Playprint Info chunk.
     * 
     * @author Mark S. Millard
     */
    public static class MleDppChunkInfo
    {
        /** The chunk. */
        public MleDppChunk m_chunk;
        /** The type of Playprint form. */
        public long m_form;    // A MleDppTag as an unsigned int (4 bytes).
        /** The chunk offset. */
        public long m_offset;  // An unsigned int (4 bytes).
        /** Additional flags information. */
        public long m_flags;   // An unsigned int (4 bytes).
        
        /**
         * The default constructor.
         */
        public MleDppChunkInfo()
        {
            m_chunk = new MleDppChunk();
            m_form = 0;
            m_offset = 0;
            m_flags = 0;
        }
    }

    /**
     * This class is a helper class for managing the Digital Playprint header.
     * 
     * @author Mark S. Millard
     */
    public static class MleDppHeader
    {
        /** The version number of file format. */
        public long m_version; // An unsigned int (4 bytes).
        /** The date the file was created. */
        public long m_date;    // A signed int (4 bytes)
        
        /**
         * The default constructor.
         */
        public MleDppHeader()
        {
            m_version = 0;
            m_date = 0;
        }
    }

    /**
     * Make a MleDppTag.
     * 
     * @param ch0 The first character in the tag.
     * @param ch1 The second character in the tag.
     * @param ch2 The third character in the tag.
     * @param ch3 The fourth character in the tag.
     * 
     * @return A four-byte entity (int) is returned containing a MleDppTag.
     */
    public static int dppMakeTag(char ch0,char ch1,char ch2,char ch3) 
    {
        int tag;
        
        // In "C": tag = ( (signed long)(ch0)<<24L | (signed long)(ch1)<<16L | (ch2)<<8 | (ch3) )
        tag = (int)((byte)ch0 << 24 | (byte)ch1 << 16 | (byte)ch2 << 8 | (byte)ch3);

        return tag;
    }
    
    /**
     * Ascend into the specified chunk.
     * 
     * @param file The Digital Playprint file.
     * @param info The Digital Playprint INFO chunk.
     * @param flags Flags specifying how to process the Digital Playprint. Not Used.
     * 
     * @return If the Digital Playprint is successfully processed, then <b>true</b>
     * will be returned. Otherwise, <b>false</b> will be returned.
     * 
     * @throws MleDppException This exception is thrown if the input arguments are invalid,
     * or if an error occurs while processing the Digital Playprint.
     */
    public static boolean dppAscend(IMleRandomAccess file, MleDppChunkInfo info, int flags)
        throws MleDppException
    {
        boolean retValue = true;
        long seekTo;

        if (file == null)
            throw new MleDppException("File not specified.");
        if (info == null)
            throw new MleDppException("Digital Playprint chunk not specified.");

        seekTo = info.m_offset + info.m_chunk.m_size;
        if (isOdd(info.m_chunk.m_size))
            seekTo += 1;

        try
        {
            seek(file,seekTo,SEEK_SET);
        } catch (IOException ex)
        {
            throw new MleDppException("Unable to seek to file position " + seekTo + ".");
        }

        try
        {
            seek(file,0,SEEK_CUR);
        } catch (IOException ex)
        {
            throw new MleDppException("Unable to seek to file position " + 0 + ".");
        }
            
        return retValue;
    }
    
    /**
     * Determine whether the specified value is odd or even.
     * 
     * @param value The value to test.
     * 
     * @return <b>true</b> will be returned if the specified value is an odd value.
     * <b>false</b> will be returned if the specified value is an even value.
     */
    static boolean isOdd(long value)
    {
        boolean retValue = false;
        if ((value & 0x01) == 0x01)
            retValue = true;
        return retValue;
    }
    
    /**
     * Seek within the specified random access file.
     * 
     * @param file The random access file.
     * @param offset The byte offset to seek to as specified by the <b>whence</b> argument.
     * @param whence The mode in which to seek; from the current position or from the
     * beginning of the file.
     * 
     * @throws IOException This exception is thrown if an error occurs while seeking
     * or if the specified <b>whence</b> mode is not supported.
     */
    static boolean seek(IMleRandomAccess file,long offset, int whence)
        throws IOException
    {
        if (whence == SEEK_SET)
            file.seek(offset);
        else if (whence == SEEK_CUR)
            file.skipBytes((int)offset);
        else
            throw new IOException("Seek mode " + whence + " not supported.");
        
        return true;
    }
    
    /**
     * Get the current file position.
     * 
     * @param file The random access file.
     * 
     * @return The current file position is returned.
     * 
     * @throws IOException This exception is thrown if an error occurs while
     * attempting to acces the file.
     */
    static long tell(IMleRandomAccess file)
        throws IOException
    {
        return file.getFilePointer();
    }
    
    /**
     * Determine whether to swap on read.
     * 
     * @return <b>true</b> is returned if the byte order of the Digital Playprint
     * data should be swapped while it is read in. If no swapping should occur,
     * then <b>false</b> is returned.
     */
    public static boolean isSwapRead()
    {
        return g_swapRead;
    }
    
    /**
     * Set whether to swap the byte order of the Digital Playprint data
     * as it is read.
     * 
     * @param swap <b>true</b> should be used if the byte order of the Digital Playprint
     * data should be swapped while it is read in. If no swapping should occur,
     * then <b>false</b> should be set.
     */
    public static void setSwapRead(boolean swap)
    {
        g_swapRead = swap;
    }

    /**
     * Swap the byte order of the specified 4-byte entity.
     * 
     * @param x The 4-byte entity to swap.
     * 
     * @return The swapped entity is returned.
     */
    static int dppSwapLong(int x)
    {
        return ((x << 24) | ((x & 0xff00) << 8) | ((x >> 8) & 0xff00) | (x >> 24));
    }

    /**
     * Swap the byte order of the specified 2-byte entity.
     * 
     * @param x The 2-byte entity to swap.
     * 
     * @return The swapped entity is returned.
     */
    static short dppSwapShort(short x)
    {
       return (short)((x << 8) | (x >> 8));
    }

    /**
     * Read a chunk from the specified file.
     * 
     * @param file The Digital Playprint file.
     * 
     * @return The chunk is returned in the <code>MleDppChunkInfo</code>.
     * 
     * @throws MleDppException This exception is thrown if the Playprint can not
     * be processed.
     */
    protected static MleDppChunkInfo readChunk(IMleRandomAccess file)
        throws MleDppException
    {
        MleDppChunkInfo info = new MleDppChunkInfo();
        MleDppChunk chunk = new MleDppChunk();
        long offset = 0;
 
        if (file == null)
            throw new MleDppException("File not specified.");

        // Get current file position.
        try
        {
            offset = tell(file);
        } catch (IOException ex)
        {
            throw new MleDppException("Unable to read Digital Playprint chunk.");
        }

        // Read next chunk.
        try
        {
            // Read chunk tag.
            byte[] tag = new byte[4];
            file.readFully(tag,0,4);
            chunk.m_tag = MleUnsigned.readDWORD(tag,0);
            
            // Read chunk size.
            byte[] size = new byte[4];
            file.readFully(size,0,4);
            chunk.m_size = MleUnsigned.readDWORD(size,0);
            
            // Bump offset into DPP.
            offset += 8;

            // Swap byte order if necessary.
            if (isSwapRead())
            {
                chunk.m_tag = dppSwapLong((int)chunk.m_tag);
                chunk.m_size = dppSwapLong((int)chunk.m_size);
            }
            
            info.m_chunk.m_tag = chunk.m_tag;
            info.m_chunk.m_size = chunk.m_size;
            info.m_flags = 0;

            if ((chunk.m_tag == MLE_PPBE_TAG) ||
                (chunk.m_tag == MLE_PPLE_TAG) ||
                (chunk.m_tag == MLE_LIST_TAG))
            {
                // Read form type.
                try
                {
                    byte[] form = new byte[4];
                    file.readFully(form,0,form.length);
                    info.m_form = MleUnsigned.readDWORD(form,0);
                    
                    offset += 4;

                    // Swap the byte order if necessary.
                    if (isSwapRead())
                    {
                        info.m_form = dppSwapLong((int)info.m_form);
                    }
                    
                    info.m_offset = offset;
                } catch (Exception ex)
                {
                    throw new MleDppException("Unable to read Digital Playprint chunk.");
    	        }
    	    } else
    	    {
    	        info.m_form = dppMakeTag(' ',' ',' ',' ');
    	        info.m_offset = offset;
            }
        } catch (Exception ex)
        {
           throw new MleDppException("Unable to read Digital Playprint chunk.");
        }

        return(info);
    }
    
    /**
     * Descend into the specified chunk.
     * 
     * @param file The Digital Playprint file.
     * @param info The Digital Playprint INFO chunk.
     * @param pinfo The parent chunk of the <i>info</i> chunk. May be <b>null</b>.
     * @param flags Flags specifying how to process the Digital Playprint.
     * The following flags are valid:
     * <li>
     * <ul>MLE_DPP_FINDCHUNK - Look for chunk specified by <i>info</i>, relative to <i>pinfo</i></ul>
     * <ul>MLE_DPP_FINDLIST - Look for LIST chunk, relative to <i>pinfo</i></ul>
     * <ul>MLE_DPP_FINDPPBE - Look for PPBE chunk</ul>
     * <ul>MLE_DPP_FINDPPLE - Look for PPLE chunk</ul>
     * </li>
     * 
     * @return If the Digital Playprint is successfully processed, then the <code>MleDppChunkInfo</code>
     * will be returned containing the header information for the chunk. The file pointer for the
     * <code>IMleRandomAccess</code> will be pointing to the beginning of the data section in the found
     * chunk.
     * 
     * @throws MleDppException This exception is thrown if the input arguments are invalid,
     * or if an error occurs while processing the Digital Playprint.
     */
    public static MleDppChunkInfo dppDescend(IMleRandomAccess file,MleDppChunkInfo info,MleDppChunkInfo pinfo,int flags)
        throws MleDppException
    {
        MleDppChunkInfo retValue = null;
        MleDppChunkInfo tmpInfo;
        long seekTo;
        long curSize = 0;
        boolean found = false;

        if (file == null)
            throw new MleDppException("File not specified.");
        if (info == null)
            throw new MleDppException("Digital Playprint chunk not specified.");

        switch (flags)
        {
            case MLE_DPP_FINDCHUNK:
                if (pinfo == null)
                    throw new MleDppException("Digital Playprint parent chunk not specified.");

                if (pinfo != null)
                {
                    // XXX - should probably check for proper form type here.

                    // Seek relative to parent chunk.
                    try
                    {
                        seek(file,pinfo.m_offset,SEEK_SET);
                        while ((! found) && (curSize < pinfo.m_chunk.m_size))
                        {
                            if ((tmpInfo = readChunk(file)) == null)
                            {
                                break;
                            } else if (tmpInfo.m_chunk.m_tag != info.m_chunk.m_tag)
                            {
                                seekTo = tmpInfo.m_chunk.m_size;
                                if (isOdd(seekTo))
                                     seekTo += 1;

                                if (seek(file,seekTo,SEEK_CUR) == true)
                                    curSize += seekTo;
                                else
    			                    break;
                            } else
                            {
                                info.m_chunk.m_size = tmpInfo.m_chunk.m_size;
                                info.m_form = tmpInfo.m_form;
                                info.m_offset = tmpInfo.m_offset;
                                info.m_flags = tmpInfo.m_flags;
                                found = true;
    			            }
    		            } // While chunk not found.
    		        } catch (Exception ex)
    		        {
    		            throw new MleDppException("Unable to process Digital Playprint.");
    	            }
                }
                break;

            case MLE_DPP_FINDPPBE:
                if (pinfo != null)
                    throw new MleDppException("Digital Playprint parent chunk is specified.");

                // Seek to beginning of file.
                try
                {
                    seek(file,0,SEEK_SET);
                    while (! found)
                    {
                        if ((tmpInfo = readChunk(file)) == null)
                        {
                            break;
                        } else if ((tmpInfo.m_chunk.m_tag != MLE_PPBE_TAG) ||
                                   (tmpInfo.m_form != info.m_form))
                        {
                            break;
                        } else
                        {
                            info.m_chunk.m_tag = tmpInfo.m_chunk.m_tag;
                            info.m_chunk.m_size = tmpInfo.m_chunk.m_size;
                            info.m_offset = tmpInfo.m_offset;
                            info.m_flags = tmpInfo.m_flags;
                            found = true;
    		            }
                    } // While chunk not found.
                } catch (Exception ex)
                {
                    throw new MleDppException("Unable to process Digital Playprint.");
                }
                break;

            case MLE_DPP_FINDPPLE:
                if (pinfo != null)
                    throw new MleDppException("Digital Playprint parent chunk is specified.");

                // Seek to beginning of file.
                try
                {
                    seek(file,0,SEEK_SET);
                    while (! found)
                    {
                        if ((tmpInfo = readChunk(file)) == null)
                        {
                            break;
                        } else if ((tmpInfo.m_chunk.m_tag != MLE_PPLE_TAG) ||
                                   (tmpInfo.m_form != info.m_form))
                        {
                            break;
                        } else {
                            info.m_chunk.m_tag = tmpInfo.m_chunk.m_tag;
                            info.m_chunk.m_size = tmpInfo.m_chunk.m_size;
                            info.m_offset = tmpInfo.m_offset;
                            info.m_flags = tmpInfo.m_flags;
                            found = true;
                        }
                    } // While chunk not found.
                } catch (Exception ex)
                {
                    throw new MleDppException("Unable to process Digital Playprint.");
                }
                break;

            case MLE_DPP_FINDLIST:
                if (pinfo == null)
                    throw new MleDppException("Digital Playprint parent chunk not specified.");

                if (pinfo != null)
                {
                    // XXX - should probably check for proper form type here.

                    // Seek relative to parent chunk.
                    try
                    {
                        seek(file,pinfo.m_offset,SEEK_SET);
                        while ((! found) && (curSize < pinfo.m_chunk.m_size))
                        {
                            if ((tmpInfo = readChunk(file)) == null)
                            {
                                break;
                            } else if ((tmpInfo.m_chunk.m_tag != MLE_LIST_TAG) ||
                                       (tmpInfo.m_form != info.m_form))
                            {
                                seekTo = tmpInfo.m_chunk.m_size;
                                if (isOdd(seekTo))
                                     seekTo += 1;

                                if (seek(file,seekTo,SEEK_CUR) == true)
                                    curSize += seekTo;
                                else
                                    break;
                            } else
                            {
                                info.m_chunk.m_tag = tmpInfo.m_chunk.m_tag;
                                info.m_chunk.m_size = tmpInfo.m_chunk.m_size;
                                info.m_offset = tmpInfo.m_offset;
                                info.m_flags = tmpInfo.m_flags;
                                found = true;
                            }
                        } // While chunk not found.
                    } catch (Exception ex)
                    {
                        throw new MleDppException("Unable to process Digital Playprint.");
                    }
                }

                break;

            default:
                // Begin reading from current file position.

                // Read next chunk.
                if ((info = readChunk(file)) != null)
                    found = true;
        }

        if (found)
        {
            retValue = info;
        }
        
        try
        {
            seek(file, 0, SEEK_CUR);
        } catch (IOException ex)
        {
            throw new MleDppException("Unable to process Digital Playprint.");
        }
        
        return(retValue);
    }

	/**
	 * Check the format of the Digital Playprint.
	 * 
	 * @param file The file to process.
	 * 
	 * @return If the file is in Big Endian format, <b>1</b> will be returned.
	 * If the file is in Little Endian format, then <b>2</b> will be returned.
	 * 
	 * @throws MleDppException This exception is thrown if the file can not be
	 * processed, or if the file is of unknown format.
	 */
    public static int dppCheckFormat(IMleRandomAccess file)
	     throws MleDppException
	{
	    int chunkFileTag;
	    long offset;
	
	    // Read first 4 bytes in file.
	    try
	    {
		    offset = tell(file);
		    seek(file,0,SEEK_SET);
		    byte[] tag = new byte[4];
		    file.readFully(tag,0,4);
		    chunkFileTag = (int)MleUnsigned.readDWORD(tag,0);
		    seek(file,offset,SEEK_SET);
	    } catch (IOException ex)
	    {
	        throw new MleDppException("Unable to process Digital Playprint.");
	    }
	
		if (g_byteFormat == MLE_BIG_ENDIAN_FORMAT)
		{
		    if (chunkFileTag == MLE_PPBE_TAG)
		    {
		        // Chunk file is in Big Endian format.
		        return(MLE_BIG_ENDIAN_FORMAT);
		    } else if (chunkFileTag == dppMakeTag('E','L','P','P'))
		    {
		        // Chunk file is in Little Endian format.
		        return(MLE_LITTLE_ENDIAN_FORMAT);
		    } else
		    {
		        // Unknown format.
		        throw new MleDppException("Unknown Chunk File format: %" + chunkFileTag + "\n");
		    }
		} else if (g_byteFormat == MLE_LITTLE_ENDIAN_FORMAT)
		{
		    if (chunkFileTag == MLE_PPLE_TAG)
		    {
		        // Chunk file is in Little Endian format.
		        return(MLE_LITTLE_ENDIAN_FORMAT);
		    } else if (chunkFileTag == dppMakeTag('E','B','P','P'))
		    {
		        // Chunk file is in Bit Endian format.
		        return(MLE_BIG_ENDIAN_FORMAT);
		    } else {
		        // Unknown format.
		        throw new MleDppException("Unknown Chunk File format: %" + chunkFileTag + "\n");
		    }
		} else
		{
		    // Unknown format.
		    throw new MleDppException("Unknown Chunk File format: %" + chunkFileTag + "\n");
		}
	}
    
    /**
     * Set the byte order format for the Digital Playprint.
     * 
     * @param format The byte order format. Valid values include:
     * <li>
     * <ul>MLE_BIG_ENDIAN_FORMAT</ul>
     * <ul>MLE_LITTLE_ENDIAN_FORMAT</ul>
     * </li>
     * 
     * @throws MleDppException This exception is thrown if the specified format is unknown.
     */
    public static void setByteFormat(int format)
        throws MleDppException
    {
        if ((format != MLE_BIG_ENDIAN_FORMAT) ||
            (format != MLE_LITTLE_ENDIAN_FORMAT))
            throw new MleDppException("Unknown byte format for Digital Playprint.");
        else
            g_byteFormat = format;
    }

}

/*
 * MleActorGC.java
 * Created on Aug 26, 2004
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
import java.io.ByteArrayInputStream;

//Import Magic Lantern Digital Playprint packages.
import com.wizzer.mle.runtime.core.MleActor;
import com.wizzer.mle.runtime.core.MleRole;
import com.wizzer.mle.runtime.core.MleSet;
import com.wizzer.mle.runtime.core.IMleProp;
import com.wizzer.mle.runtime.core.MleProp;
import com.wizzer.mle.runtime.core.MleTables;
import com.wizzer.mle.runtime.core.MleRuntimeException;
import com.wizzer.mle.runtime.util.MleUnsigned;


/**
 * This class is used to manage the group context (GC) of a collection of
 * Actors.
 * 
 * @author Mark S. Millard
 */
public class MleActorGC extends MleChunkReader
{
    // A debug flag.
    private static final boolean g_debug = true;

    /** The Media Chunk type. */
    public static final int MLE_CHUNK_MEDIA = 0;
    /** The Group Chunk type. */
    public static final int MLE_CHUNK_GROUP = 1;
    /** The Set Chunk type. */
    public static final int MLE_CHUNK_SET   = 2;
    
    // The Digital Playprint OpCodes.
    
    /** The beginActorSet opcode. */
    public static final int beginActorSetOpcode       = 0x80;
    /** The createActor opcode. */
    public static final int createActorOpcode         = 0x81;
    /** The setSet opcode. */
    public static final int setSetOpcode              = 0x82;
    /** The bindRole opcode. */
    public static final int bindRoleOpcode            = 0x83;
    /** The parentRole opcode. */
    public static final int parentRoleOpcode          = 0x84;
    /** The childRole opcode. */
    public static final int childRoleOpcode           = 0x85;
    /** The setPropertyOffset opcode. */
    public static final int setPropertyOffsetOpcode   = 0x86;
    /** The setPropertyLength opcode. */
    public static final int setPropertyLengthOpcode   = 0x87;
    /** The copyProperty opcode. */
    public static final int copyPropertyOpcode        = 0x88;
    /** The copyMediaRef opcode. */
    public static final int copyMediaRefOpcode        = 0x89;
    /** The endActorGroup opcode. */
    public static final int endActorGroupOpcode       = 0x8a;
    /** The copyDynamicProperty opcode. */
    public static final int copyDynamicPropertyOpcode = 0x8b;
    /** The copyArrayProperty opcode. */
    public static final int copyArrayPropertyOpcode   = 0x8c;
    /** The createSet opcode. */
    public static final int createSetOpcode           = 0x8d;
    /** The endSet opcode. */
    public static final int endSetOpcode              = 0x8e;

    // The number of bits in a value;
    private static final int g_valueBits = 7;
    // A mask for extracting the appropriate bits.
    private static final int g_valueMask = ((1 << (g_valueBits)) - 1);
    
    /**
     * An Actor regisgtry entry.
     * 
     * @author Mark S. Millard
     */
    public static class MleActorRegistry
    {
        /** The Actor. */
        public MleActor m_actor;
        /** The Actor's Role. */
        public MleRole m_role;
        /** The index into the registry. */
        public int m_index;
    }

    // The Actor registry.
    private MleActorRegistry[] m_actorRegistry = null;
    // The current index into the Actor registry.
    private int m_actorIndex = 0;
    
    // The collection of Sets in the DPP.
    private MleSet[] m_sets = null;
    // The collection of Role attachments (by index).
    private int[] m_attachmentArray;
    
    // The byte stream to process.
    //private byte[] m_stream = null;
    // The current offset into the byte stream.
    //private int m_offset = 0;
    
    /**
     * A constructor that initializes the byte stream to process.
     * 
     * @param stream An array of <b>byte</b>s consisting of Digital Playprint operations.
     */
    public MleActorGC(byte[] stream)
    {
        super(stream);
        //m_stream = stream;
    }
    
    /**
     * Get the collection of Sets that are associated with the Actor
     * Group Context.
     * 
     * @return The collection is returned as an array of <code>MleSet</code>.
     */
    public MleSet[] getSets()
    {
        return m_sets;
    }
    
    /**
     * Get the Actor registry.
     * 
     * @return The Actor registry is returned as an array of
     * <code>MleActorRegistry</code> entries.
     */
    public MleActorRegistry[] getActorRegistry()
    {
        return m_actorRegistry;
    }

    /**
     * Initialize the state for processing the Actor Group context.
     * 
     * @param size The size of the bookkeeping tables.
     */
    public void chunkInitialization(int size)
    {
        // Initialize the Actor registry.
    	m_actorRegistry = new MleActorRegistry[size];
    	m_actorIndex = 0;

    	// Initialize the collection of Sets.
    	m_sets = new MleSet[size];
    	for (int i = 0; i < size; i++)
    	    m_sets[i] = null;
    	
    	// Initialize bookkeeping.
    	m_attachmentArray = new int[size];
    	for (int i = 0; i < size; i++)
    	    m_attachmentArray[i] =0;
    }
    
    /**
     * Read an index from the byte stream.
     * 
     * @return An index is returned as a <b>long</b>.
     * 
     * @throws MleDppException This exception is thrown if the index
     * can not be read from the byte stream. This exception will also be
     * thrown if the index is invalid (less than 0).
     */
    public long readIndex()
        throws MleDppException
    {
    	long value = 0;
    	byte nextByte;
        
    	while ((nextByte = nextStreamByte()) >= 0)
    	{
    		if ((value & (long) 0x80000000 >> g_valueBits - 1) != 0)
    		    throw new MleDppException("MleActorGC: Invalid index in stream.");
    		value = (value << g_valueBits) | nextByte;
    	}
    	if (g_debug) dumpIndex(value);
    	
    	if (value < 0)
    	    throw new MleDppException("MleActorGC: Invalid index in stream.");
    	
    	m_offset--; // Back up one, since while loop always over-advances by one.
    	
    	return value;
    }
    
    // Register the Actor for bookkeeping purposes.
    private void registerActor(MleActor actor, int index)
    {
        m_actorRegistry[m_actorIndex] = new MleActorRegistry();
    	m_actorRegistry[m_actorIndex].m_actor = actor;
    	m_actorRegistry[m_actorIndex].m_index = index;
    	m_actorIndex++;
    	
    	// Allow the actor to be accessed outside the context of this class.
    	MleTables.getInstance().registerObject(actor);
    }

    // Helper Class to create a Actor.
    private MleActor createActor(int index)
        throws MleDppException
    {
    	MleActor newActor = null;
    	
    	if ((index < 0) || (index >= MleTables.g_mleRTActorClass.size()))
    	    throw new MleDppException("MleActorGC: Table index out of range.");
    	
    	try
    	{
	    	MleTables.MleRTActorClassEntry entry = 
	    	    (MleTables.MleRTActorClassEntry) MleTables.g_mleRTActorClass.elementAt(index);
	    	newActor = (MleActor) entry.createActor();
    	} catch (Exception ex)
    	{
    	    throw new MleDppException("MleActorGC: Unable to instantiate new Actor.");
    	}
    	
    	// Register the new Actor.
    	registerActor(newActor, index); // Use the index of the actor for future reference.
    	
    	return newActor;
    }
    
    // Helper class to create a Role.
    private MleRole createRole(MleActor currentActor, int index)
        throws MleDppException
    {
        MleRole newRole;
        
    	if ((index < 0) || (index >= MleTables.g_mleRTRoleClass.size()))
    	    throw new MleDppException("MleActorGC: Table index out of range.");
        
    	try
    	{
	    	MleTables.MleRTRoleClassEntry entry = 
	    	    (MleTables.MleRTRoleClassEntry) MleTables.g_mleRTRoleClass.elementAt(index);
	    	newRole = (MleRole) entry.createRole(currentActor);
    	} catch (Exception ex)
    	{
    	    throw new MleDppException("MleActorGC: Unable to instantiate new Actor.");
    	}
        
    	// Add the role to the actor registry. It is one less than where the
    	// current index is pointing to.
        m_actorRegistry[m_actorIndex-1].m_role = newRole;
        
        return newRole;
    }
    
    // Helper class to create a Set.
    private MleSet createSet(int index)
    	throws MleDppException
    {
        MleSet newSet = null;
	
        if ((index < 0) || (index >= MleTables.g_mleRTSetClass.size()))
            throw new MleDppException("MleActorGC: Table index out of range.");
	
        try
        {
            MleTables.MleRTSetClassEntry entry = 
                (MleTables.MleRTSetClassEntry) MleTables.g_mleRTSetClass.elementAt(index);
            newSet = (MleSet) entry.createSet();
        } catch (Exception ex)
        {
            throw new MleDppException("MleActorGC: Unable to instantiate new Set.");
        }
	
        return newSet;
    }
    
    /**
     * Parse the byte stream.
     * 
     * @param chunkType The type of chunk that is being parsed from the byte
     * stream.
     * 
     * @throws MleDppException This exception is thrown if the byte
     * stream can not be parsed and processed successfully.
     * @throws MleRuntimeException This exception is thrown if the Set can not
     * be successfully initialized.
     */
    public void parseStream(int chunkType)
        throws MleDppException, MleRuntimeException
    {
        //long propertyOffset = -1;
        String propertyName = null;
        long propertyLength = 0;
        MleActor currentActor = null;
        MleRole currentRole = null;
        MleSet currentSet = null;
        int index;
       
        int actorIndex = -1; // The index of the actor within the group.

        // Opcode arguments.
        int setIndex = -1;         // The index of the Set.
        int parentActorIndex = -1; // The index of the parent Actor.
        int childActorIndex = -1;  // The index of a child Actor.

        do // Process the byte stream.
        {
            int cmd;
            long bytesToCopy;
            byte[] buf;
            int bufIndex;
            
            byte[] input = new byte[2];
            input[0] = 0;
            input[1] = nextStreamByte();
		   	switch (cmd = MleUnsigned.readWORD(input,0))
		   	{
				// The ActorSet functionality has been superceded.  Sets are now
				//   specified in role bindings.
				// case beginActorSetOpcode:
				//     if (currentActor) {
				//   		currentActor = null;
				//   		currentRole = null;
				//   	}
				//   	if (g_debug) dumpOpcode(beginActorSetOpcode);
				//   	index = readIndex();
				//   	currentSet = MleTable.loadSet(index);
				//   	MleSet.setCurrentSet(currentSet);
				//   	break;
		   	    
		   	  case createActorOpcode:
		   	    if (g_debug) {
		   	    	dumpOpcode((byte)createActorOpcode);
		   	    	System.out.print("createActor ");
		   	    }
		   	    
		   	    // Increment the actor count.
		   	    actorIndex++;

		   	    index = (int)readIndex();
		   	    currentActor = createActor(index);
		
		   	    // Initialize state that the actor may use.
		   	    m_sets[actorIndex] = null;
		   	    m_attachmentArray[actorIndex] = 0;
		   	    currentRole = null;
		   	    setIndex = -1;
		   	    parentActorIndex = -1;
		   	    
		   	    break;
		   	    
		   	  case setSetOpcode:
		   	    if (g_debug) {
		   	    	dumpOpcode((byte)setSetOpcode);
		   	    	System.out.print("setSet ");
		   	    }
		   	    
		   	    index = (int)readIndex();
		
		   	    setIndex = index;
		   	    
		   	    break;
		   	    
		   	  case bindRoleOpcode:
		   	    if (g_debug) {
		   	    	dumpOpcode((byte)bindRoleOpcode);
		   	    	System.out.print("bindRole ");
		   	    }
		
		   	    if (currentActor == null)
		   	        throw new MleDppException("MleActorGC: No Actor is currently active.");
		   	    if (setIndex < 0)
		   	        throw new MleDppException("MleActorGC: Invalid Set index.");
		
		   	    // Read the arguments, role class index and set index.
		   	    index = (int)readIndex();
		
		   	    // Set the current Set, creating it if necessary.
		   	    //   MleDppLoader.mleLoadSet() just returns the reference if it has already
		   	    //   been created.  We also remember the set as being used
		   	    //   for this actor.
		   	    MleSet set = MleDppLoader.getInstance().mleLoadSet(setIndex);
		   	    m_sets[actorIndex] = set;
		   	    MleSet.setCurrentSet(m_sets[actorIndex]);		   		
		
		   	    // Now create the role.
		   	    currentRole = createRole(currentActor, index);
		   	    
		   	    break;
		   	    
		   	  case parentRoleOpcode:
		   	    if (g_debug) {
		   	    	dumpOpcode((byte)parentRoleOpcode);
		   	    	System.out.print("parentRole ");
		   	    }
		   	    
		   	    index = (int)readIndex();
		
		   	    parentActorIndex = index;
		   	    
		   	    break;
		   	    
		   	  case childRoleOpcode:
		   	    if (g_debug) {
		   	    	dumpOpcode((byte)childRoleOpcode);
		   	    	System.out.print("childRole ");
		   	    }
		   	    
		   	    if (currentSet == null)
		   	        throw new MleDppException("MleActorGC: No Set is currently active.");
		
		   	    // Read child actor index.
		   	    childActorIndex = (int)readIndex();
		
		   	    // Verify that the role has not already been attached.
		   	    if (m_attachmentArray[childActorIndex] == 0)
		   	        throw new MleDppException("MleActorGC: Role is already attached to an Actor.");
		
		   	    // Verify that the Sets for these Actors match.
		   	    if (m_sets[parentActorIndex] != m_sets[childActorIndex])
		   	        throw new MleDppException("MleActorGC: Actors must share a common Set to hava a parent/child relationship.");
		
		   	    // Do the attachment.
		   	    m_sets[parentActorIndex].attachRoles(
		   		    m_actorRegistry[parentActorIndex].m_actor.getRole(),
		   		    m_actorRegistry[childActorIndex].m_actor.getRole());
		
		   	    // Mark the actor/role as having been attached.
		   	    m_attachmentArray[childActorIndex] = 1;
		   	    parentActorIndex = -1;
		   	    childActorIndex = -1;
		   	    
		   	    break;
		   	    
		   	  case setPropertyOffsetOpcode:		   	      
		   	    if (g_debug) {
		   	    	dumpOpcode((byte)setPropertyOffsetOpcode);
		   	    	System.out.print("setPropertyOffset ");
		   	    }
		   	    
		   	    index = (int)readIndex();
		   	    
		   	    if (chunkType == MLE_CHUNK_GROUP)
		   	    {
		   	        if ((index < 0) || (index >= MleTables.g_mleRTActorProperties.size()))
		   	            throw new MleDppException("MleActorGC: nvalid Actor Property index.");
		   	        propertyName =
		   	            ((MleTables.MleRTPropertyEntry)(MleTables.g_mleRTActorProperties.elementAt(index))).getProperty();
		   	    }
		   	    else if (chunkType == MLE_CHUNK_SET)
		   	    {
		   	        if ((index < 0) || (index >= MleTables.g_mleRTSetProperties.size()))
		   	            throw new MleDppException("MleActorGC: Invalid Set Property index.");
		   	        propertyName =
		   	            ((MleTables.MleRTPropertyEntry)(MleTables.g_mleRTSetProperties.elementAt(index))).getProperty();
		   	    }
		   	    break;
		   	    
		   	  case setPropertyLengthOpcode:
		   	    if (g_debug) {
		   	    	dumpOpcode((byte)setPropertyLengthOpcode);
		   	    	System.out.print("setPropertyLength ");
		   	    }
		   	    
		   	    propertyLength = readIndex();
		   	    
		   	    break;
		   	    
		   	  case copyPropertyOpcode:
		   	    if (g_debug) {
		   	    	dumpOpcode((byte)copyPropertyOpcode);
		   	    	System.out.print("copyProperty ");
		   	    }

		   		bytesToCopy = propertyLength;
		   		buf = new byte[(int)bytesToCopy];
		   		bufIndex = 0;
		   		while (--bytesToCopy >= 0)
		   		{
		   		    byte b = nextStreamByte();
		   		    if (g_debug) dumpRawByte(b);
		   		    buf[bufIndex++] = b;
		   		}
		   		
		   		try
		   		{
		   		    MleProp prop = new MleProp((int)propertyLength,new ByteArrayInputStream(buf));
			   		if (chunkType == MLE_CHUNK_GROUP)
			   		{
			   		    currentActor.setProperty(propertyName,prop);
			   		} else if (chunkType == MLE_CHUNK_SET)
			   		{
			   		    currentSet.setProperty(propertyName,prop);
			   		}
		   		} catch (MleRuntimeException ex)
		   		{
		   		    throw new MleDppException("MleActorGC: Unable to set property " +
		   		        propertyName + ".");
		   		}

		   	    break;
		   	    
		   	  case copyDynamicPropertyOpcode:
		   	    if (g_debug) {
		   	    	dumpOpcode((byte)copyDynamicPropertyOpcode);
		   	    	System.out.print("copyDynamicProperty ");
		   	    }

		   	    bytesToCopy = readInt();
		   	    long length = bytesToCopy;
		   		buf = new byte[(int)bytesToCopy];
		   		bufIndex = 0;
		   		while (--bytesToCopy >= 0)
		   		{
		   		    byte b = nextStreamByte();
		   		    if (g_debug) dumpRawByte(b);
		   		    buf[bufIndex++] = b;
		   		}

		   		try
		   		{
		   		    MleProp prop = new MleProp((int)length,new ByteArrayInputStream(buf));
			   		if (chunkType == MLE_CHUNK_GROUP)
			   		{
			   		    currentActor.setProperty(propertyName,prop);
			   		} else if (chunkType == MLE_CHUNK_SET)
			   		{
			   		    currentSet.setProperty(propertyName,prop);
			   		}
		   		} catch (MleRuntimeException ex)
		   		{
		   		    throw new MleDppException("MleActorGC: Unable to set property " +
		   		        propertyName + ".");
		   		}

		   	    break;
		   	    
		   	  case copyArrayPropertyOpcode:
		   	    if (g_debug) {
		   	    	dumpOpcode((byte)copyArrayPropertyOpcode);
		   	    	System.out.print("copyArrayProperty ");
		   	    }

		   	    int numElements = (int)readInt();
		   	    bytesToCopy = numElements * propertyLength;
		   	    buf = new byte[(int)bytesToCopy];
		   		bufIndex = 0;
		   		while (--bytesToCopy >= 0)
		   		{
		   		    byte b = nextStreamByte();
		   		    if (g_debug) dumpRawByte(b);
		   		    buf[bufIndex++] = b;
		   		}
		   		
		   		try
		   		{
			   		if (chunkType == MLE_CHUNK_GROUP)
			   		{
			   		    currentActor.setPropertyArray(propertyName,(int)propertyLength,
			   		        numElements,new ByteArrayInputStream(buf));
			   		} else  if (chunkType == MLE_CHUNK_SET)
			   		{
			   		    currentSet.setPropertyArray(propertyName,(int)propertyLength,
				   		    numElements,new ByteArrayInputStream(buf));
			   		}
		   		} catch (MleRuntimeException ex)
		   		{
		   		    throw new MleDppException("MleActorGC: Unable to set property " +
		   		        propertyName + ".");
		   		}

		   	    break;
		   	    
		   	  case endActorGroupOpcode:
		   	    if (g_debug) {
		   	    	dumpOpcode((byte)endActorGroupOpcode);
		   	    	System.out.print("endActorGroup ");
		   	    }
		   	    break;
		   	    
		   	  case copyMediaRefOpcode:
		   	    if (g_debug) {
		   	    	dumpOpcode((byte)copyMediaRefOpcode);
		   	    	System.out.print("copyMediaRef ");
		   	    }
		   	    
		   	    Integer mediaRef = new Integer((int)readIndex());
		   	    buf = mediaRef.toString().getBytes();
		   	    
		   	    try
		   	    {
			   	    MleProp prop = new MleProp(buf.length,new ByteArrayInputStream(buf));
			   	    prop.setType(IMleProp.PROP_TYPE_MEDIAREF);
		   	        currentActor.setProperty(propertyName,prop);
		   		} catch (MleRuntimeException ex)
		   		{
		   		    throw new MleDppException("MleActorGC: Unable to set property " +
		   		        propertyName + ".");
		   		}
		   	    
		   	    break;
		   	    
		   	  case createSetOpcode:
		   	    if (g_debug) {
		   	    	dumpOpcode((byte)createSetOpcode);
		   	    	System.out.print("createSet ");
		   	    }
		   	    
		   	    index = (int)readIndex();
		   	    
		   	    currentSet = createSet(index);
		   	    ((MleTables.MleRTSetEntry)(MleTables.g_mleRTSet.elementAt(index))).m_theSet = currentSet;
		   	    MleSet.setCurrentSet(currentSet);
		   	    
		   	    break;
		   	    
		   	  case endSetOpcode:
		   	    if (g_debug) {
		   	    	dumpOpcode((byte)endSetOpcode);
		   	    	System.out.print("endSet ");
		   	    }
		   	    break;
		   	    
		   	  default:
		   	    throw new MleDppException("MleActorGC: Invalid OpCode in Actor Group Context.");
		   	}
       } while (m_offset < m_stream.length);

       // Now finish up by doing leftover role attachments.
       if ( chunkType == MLE_CHUNK_GROUP )
       {
		   	// Increment actor counter (it started at -1).
		   	//m_actorIndex++;
		
		   	// Loop through all the actors.
		   	for ( int i = 0; i < m_actorIndex; i++ )
		   	{
		   	    // See if the role has been attached.
		   	    if ( (m_attachmentArray[i] == 0) &&
		   	         (m_actorRegistry[i].m_actor.getRole() != null) )
		   	    {
			   		// If not, put it on the Set ("Last Call!").
			   		m_sets[i].attachRoles(
			   		    null,m_actorRegistry[i].m_actor.getRole());
		   	    }
		   	}
		
		   	// Have to be careful here - can't move this outside the if
		   	//   clause because parseStream may be called recursively to
		   	//   load Sets.
		   	m_attachmentArray = null;
       }
    }
    
    // Dump the opcode to system out.
	private void dumpOpcode(byte a)
	{
		StringBuffer s =
			new StringBuffer(Integer.toHexString((a >= 0) ? a : 256 + a));
		if (s.length() == 1) s.insert(0,'0');
	    System.out.print("\n" + s + " ");
	}
	
	// Dump the index to system out.
	private void dumpIndex(long a)
	{
	    System.out.print("index=" + a + " ");
	}
	
	// Dump the raw byte to system out.
	private void dumpRawByte(byte a)
	{
		StringBuffer s =
			new StringBuffer(Integer.toHexString((a >= 0) ? a : 256 + a));
		if (s.length() == 1) s.insert(0,'0');
	    System.out.print("raw=0x" + s + " ");
	}

}

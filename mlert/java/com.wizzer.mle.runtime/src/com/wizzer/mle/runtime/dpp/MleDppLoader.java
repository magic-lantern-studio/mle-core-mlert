/*
 * MleDppLoader.java
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

// Import standard Java packages.
import java.io.RandomAccessFile;
import java.io.IOException;

// Import Magic Lantern Digital Playprint packages.
import com.wizzer.mle.runtime.MleTitle;
import com.wizzer.mle.runtime.core.MleGroup;
import com.wizzer.mle.runtime.core.MleMediaRef;
import com.wizzer.mle.runtime.core.MleScene;
import com.wizzer.mle.runtime.core.MleSet;
import com.wizzer.mle.runtime.core.MleTables;
import com.wizzer.mle.runtime.core.MleRuntimeException;
import com.wizzer.mle.runtime.event.MleEvent;
import com.wizzer.mle.runtime.event.MleEventCallback;


/**
 * This class is used to load items from the Digital Playprint.
 * 
 * @author Mark S. Millard
 */
public class MleDppLoader
{
    // The Singleton instance of the loader.
    private static MleDppLoader g_theLoader = null;
    
	//  There are three possible numberings of a scene, group, or actor
	//  that we must be concerned about.  They are
	// 	1. chunk number of the item within the global list of all chunks,
	// 		eg, first group might be chunk 8.
	// 	2. item number within the list of items of that type,
	// 		eg, Nth group would have group index [N-1] (0-based)
	// 	3. item number within the parent item containing it,
	// 		eg, if Nth group is group 3 in 2nd scene, then
	// 		its SceneGroup index within that scene is 3.
	//
	//  These three types of numbering are good for the activities
	//  1. item LOADing from workprint/playprint
	// 		applicable to scenes and groups, but not actors.
	// 	2. item class TYPEing within all items of a generic type (eg, groups)
	// 		applicable to scenes, groups and actors.
	// 	3. item FINDing within a parent container
	// 		applicable to groups and actors.
	//
	//  Thus the 7 methods for mleXXXRefTo{Load,Type,Find}().

    /**
     * Convert Scene reference to class type.
     * 
     * @param ID The Scene reference to convert.
     * 
     * @return The class type is returned.
     */
	public static final String mleSceneRefToType(String ID)
	{
	    return new String("MleDPPSceneID_" + ID);
	}
	 
	/**
	 * Convert Scene reference to load type.
	 * 
	 * @param ID The Scene reference to convert.
	 * 
	 * @return The load type is returned
	 */
	public static final String mleSceneRefToLoad(String ID)
	{
	    return new String("MleDPPScene_" + ID);
	}
	 
	/**
	 * Convert Group reference to find type.
	 *  
	 * @param SID The Scene reference to convert.
	 * @param GID The Group reference to convert.
	 * 
	 * @return The find type is returned.
	 */
	public static final String mleGroupRefToFind(String SID,String GID)
	{
	    return new String("MleDPPSceneGroupID_" + SID + "_" + GID);
	}
	 
	/**
	 * Convert Group reference to class type.
	 * 
	 * @param ID The Group reference to convert.
	 * 
	 * @return The class type is returned.
	 */
	public static final String mleGroupRefToType(String ID)
	{
	    return new String("MleDPPGroupID_" + ID);
	}
	 
	/**
	 * Convert Group reference to load type.
	 * 
	 * @param ID The Group reference to convert.
	 * 
	 * @return The load type is returned.
	 */
	public static final String mleGroupRefToLoad(String ID)
	{
	    return new String("MleDPPGroup_" + ID);
	}
	 
	/**
	 * Convert Actor reference to load type.
	 * 
	 * @param ID The Actor reference to convert.
	 * 
	 * @return The load type is returned.
	 */
	public static final String mleActorRefToType(String ID)
	{
	    return new String("MleDPPActor_" + ID);
	}
	 
	/**
	 * Convert Actor reference to find type.
	 *  
	 * @param GID The Group reference to convert.
	 * @param AID The Actor reference to convert.
	 * 
	 * @return The find type is returned.
	 */
	public static final String mleActorRefToFind(String GID,String AID)
	{
	    return new String("MleDPPGroupActorID_" + GID + "_" + AID);
	}
	 
	/**
	 * Convert Media reference to load type.
	 * 
	 * @param ID The Media reference to convert.
	 * 
	 * @return The load type is returned.
	 */
	public static final String mleMediaRefTo(String ID)
	{
	    return new String("MleDPPMedia_" + ID);
	}
	 
	/**
	 * Convert Set reference to load type.
	 * 
	 * @param ID The Set reference to convert.
	 * 
	 * @return The load type is returned.
	 */
	public static final String mleSetRefTo(String ID)
	{
	    return new String("MleDPPSet_" + ID);
	}
 
 	// Hide default constructor.
    private MleDppLoader()
    {
        super();
    }

    /**
     * Get the Singleton instance of the DPP loader.
     * 
     * @return A reference to the <code>MleDppLoader</code> is returned.
     */
    public static MleDppLoader getInstance()
    {
        if (g_theLoader == null)
            g_theLoader = new MleDppLoader();
        return g_theLoader;
    }
    
    /**
     * Load a Group of Actors from the Digital Playprint.
     * 
     * @param indexToDppTOC The identifier for the Group to load. This is an offset into the
     * Digital Playprint Table-Of-Content.
     * 
     * @return The loaded <code>MleGroup</code> is returned. If the Group was not successfully
     * loaded, then <b>null</b> will be returned.
     * 
     * @throws MleDppException This exception may be thrown if the Group can not
     * be successfully loaded.
     * @throws MleRuntimeException This exception is thrown if the Group can not
     * be succssfully initialized. It will also be thrown if any of the Group's
     * Actors can not be initialized.
     */
    public MleGroup mleLoadGroup(int indexToDppTOC)
        throws MleDppException, MleRuntimeException
    {
        MleGroup group = null;

        // Get the Digital Playprint file.
        RandomAccessFile file = MleTitle.getInstance().m_theDpp.getFile();
        
        // XXX -- Fix when DPP API is available.
        // Get the group offset from the DPP TOC. Then read the length for the
        // associcated group.
        int length;
        try
        {
	        int offset = (int)MleTitle.getInstance().m_theDpp.getTOCOffset(indexToDppTOC) + 4;
	        MleDppUtil.seek(file,offset,MleDppUtil.SEEK_SET);
	        length = file.readInt();
        } catch (MleDppException ex)
        {
            throw new MleDppException("Unable to read Digital Playprint.");
        } catch (IOException ex)
        {
            throw new MleDppException("Unable to read Digital Playprint.");
        }
        
        // Read the chunk.
        byte[] actorGroupChunk = new byte[length];
        try
        {
	        file.readFully(actorGroupChunk,0,length);
        } catch (IOException ex)
        {
            throw new MleDppException("Unable to read Digital Playprint.");
        }
        
        // Create a Actor Group Context for the group chunk.
        MleActorGC gc = new MleActorGC(actorGroupChunk);

        // XXX - Should also get the group type from the DPP (before 1st actor)
        // to create here with a call to mlCreateGroup(<type>).
        try
    	{
            int groupIndex = (int) gc.readInt();
	    	MleTables.MleRTGroupClassEntry entry = 
	    	    (MleTables.MleRTGroupClassEntry) MleTables.g_mleRTGroupClass.elementAt(groupIndex);
	    	group = (MleGroup) entry.createGroup();
    	} catch (Exception ex)
    	{
    	    throw new MleDppException("Unable to instantiate new Group.");
    	}
        if ( group == null )
        {
      	    return null;
        }

        // Read the number of actors in the group.
        int numActors = (int) gc.readIndex();
        // Initialize the chunk.
        gc.chunkInitialization(numActors);
        // Parse the chunk.
        gc.parseStream(MleActorGC.MLE_CHUNK_GROUP);

        // Get the collection of sets. These were built as the stream was parsed.
        MleSet[] theSets = gc.getSets();

        // Initialize the actors.
        MleActorGC.MleActorRegistry[] actorRegistry = gc.getActorRegistry();
        for (int i = 0; i < actorRegistry.length; i++)
        {
            // Set the current set.
            MleSet.setCurrentSet(theSets[i]);

            // XXX - Why not dispense with actor registry and use the group?
            // Well, the registry holds at least the actor\'s table index
            // and role as well, so isn\'t 100% redundant.
            group.add(actorRegistry[i].m_actor);

            // Initialize the actor.
            actorRegistry[i].m_actor.init();
        }

        // Now finish the initialization sequence.
        group.init();

        // Initialize the MleSet.setCurrentSet() back to null since this
        // variable is only valid during actor's init() function.
        MleSet.setCurrentSet(null);
        
        // Return the loaded group.
        return group;
    }

    /**
     * Load a Scene from the Digital Playprint.
     * 
     * @param indexToDppTOC The identifier for the Scene to load. This is an offset into the
     * Digital Playprint Table-Of-Content.
     * 
     * @return The loaded <code>MleScene</code> is returned. If the Scene was not successfully
     * loaded, then <b>null</b> will be returned.
     * 
     * @throws MleDppException This exception may be thrown if the Scene can not
     * be successfully loaded.
     * @throws MleRuntimeException This exception is thrown if the Scene can not
     * be successfully initialized.
     */
    public MleScene mleLoadScene(int indexToDppTOC)
        throws MleDppException, MleRuntimeException
    {
        MleScene scene = null;
        
        // Get the Digital Playprint file.
        RandomAccessFile file;
        MleDppInput dpp = MleTitle.getInstance().m_theDpp;
        if (dpp != null)
            file = dpp.getFile();
        else
        	throw new MleDppException("Unable to read Digital Playprint. DPP is null.");

        // XXX -- Fix when DPP API is available.
        // Get the Set offset from the DPP TOC. Then read the length for the associated
        // Set.
        int length;
        try
        {
	        int offset = (int)MleTitle.getInstance().m_theDpp.getTOCOffset(indexToDppTOC) + 4;
	        MleDppUtil.seek(file,offset,MleDppUtil.SEEK_SET);
	        length = file.readInt();
        } catch (IOException ex)
        {
            throw new MleDppException("Unable to read Digital Playprint.");
        }

        // Read the chunk.
        byte[] sceneChunk = new byte[length];
        try
        {
	        file.readFully(sceneChunk,0,length);
        } catch (IOException ex)
        {
            throw new MleDppException("Unable to read Digital Playprint.");
        }
        
        // Create a new Chunk Reader.
        MleChunkReader chkReader = new MleChunkReader(sceneChunk);

        // XXX - Should also get the scene type from the sceneClass[indexToDppTOC]
        // in order to create the right scene type object, not just MleScene, here.
        try
    	{
            int sceneIndex = (int) chkReader.readInt();
	    	MleTables.MleRTSceneClassEntry entry = 
	    	    (MleTables.MleRTSceneClassEntry) MleTables.g_mleRTSceneClass.elementAt(sceneIndex);
	    	scene = (MleScene) entry.createScene();
    	} catch (Exception ex)
    	{
    	    throw new MleDppException("Unable to instantiate new Scene.");
    	}
        if ( scene == null )
        {
    	    return null;
        }

      // mleLoadScene() always puts it\'s result into currentScene
      // and does this before the groups are loaded and before its
      // init() call, in case the groups or the init()
      // call must use it.
      scene.changeCurrentScene(scene);

      // For each group in the scene, read its index (into global
      // list of groups) and then load that group.
      // Format of the chunk is
      //    [ numGroups, [ g0index, g1index, ... gNumIndex ] ]
      //
      try
      {
	      int numGroups = (int) chkReader.readInt();
	      for (int i = 0; i < numGroups; i++)
	      {
	    	  // readInt() manages the offset, so just call repeatedly.
	    	  int groupIndex = (int) chkReader.readInt();
	
	    	  scene.add(mleLoadGroup(groupIndex));
	      }
      } catch (Exception ex)
      {
          throw new MleDppException("Unable to process new Scene.");
      }

      // Now finish the initialization sequence.
      scene.init();

      // Returne the loaded scene.
      return scene;
    }

    /**
     * Bootstrap the title by loading the first Scene.
     * 
     * @return A <code>MleScene</code> is returned.
     * 
     * @throws MleDppException This exception is thrown if the boot
     * scene can not be loaded from the mastered title tables.
     * @throws MleRuntimeException This exception is thrown if the Scene can not
     * be succssfully initialized.
     */
    public MleScene mleLoadBootScene()
        throws MleDppException, MleRuntimeException
    {
        // The first scene is the boot scene, by convention.
        return mleLoadScene(MleTables.g_mleBootScene);
    }
    
    // Helper method for creating a Set.
    private void createSet(int id)
    	throws MleDppException, MleRuntimeException
    {
        long curFileOffset;
 
        // Get the Digital Playprint file.
        RandomAccessFile file = MleTitle.getInstance().m_theDpp.getFile();

        // Save the current position of the playprint.
        try
        {
            curFileOffset = MleDppUtil.tell(file);
        } catch (IOException ex)
        {
            throw new MleDppException("Unable to read Digital Playprint.");
        }

        // Seek to the beginning of the appropriate Set chunk.
        // Note that the 4 additional bytes are for the size of the tag at the beginning of
        // the chunk.
        // XXX Mark will do the clean up to make use of the tag and size of
        //     MleDppChunk structure correctly.
        //
        int length;
        try
        {
            int chunkTOCIndex = MleTables.g_mleRTSetChunk[id];
            int setChunkOffset = (int)MleTitle.getInstance().m_theDpp.getTOCOffset(chunkTOCIndex) + 4;
            MleDppUtil.seek(file,setChunkOffset,MleDppUtil.SEEK_SET);
            length = file.readInt();
        } catch (MleDppException ex)
        {
            throw new MleDppException("Unable to read Digital Playprint.");
        } catch (IOException ex)
        {
            throw new MleDppException("Unable to read Digital Playprint.");
        }

        // Allocate memory to store the Set chunk, read in the size and content
        // of the Set chunk.
        byte[] setChunk = new byte[length];
        try
        {
	        file.readFully(setChunk,0,length);
        } catch (IOException ex)
        {
            throw new MleDppException("Unable to read Digital Playprint.");
        }

        // Interpret the Set chunk, create the Set, initialize its property
        // values if any.
        
        // Create a Actor Group Context for the set chunk.
        MleActorGC gc = new MleActorGC(setChunk);
        gc.parseStream(MleActorGC.MLE_CHUNK_SET);

        // Restore the current position of the playprint.
        try
        {
            MleDppUtil.seek(file, curFileOffset, MleDppUtil.SEEK_SET);
        } catch (IOException ex)
        {
            throw new MleDppException("Unable to read Digital Playprint.");
        }
    }

    /**
     * Load a Set from the Digital Playprint.
     * 
     * @param indexToDppTOC The identifier for the Set to load. This is an offset into the
     * Digital Playprint Table-Of-Content.
     * 
     * @return The loaded <code>MleSet</code> is returned. If the Set was not successfully
     * loaded, then <b>null</b> will be returned.
     * 
     * @throws MleDppException This exception may be thrown if the Set can not
     * be successfully loaded.
     * @throws MleRuntimeException This exception is thrown if the Set can not
     * be succssfully initialized.
     */
    public MleSet mleLoadSet(int indexToDppTOC)
         throws MleDppException, MleRuntimeException
    {
        if ((indexToDppTOC < 0) || (indexToDppTOC >= MleTables.g_mleRTSet.size()))
            throw new MleDppException("Invalid Set identifier.");
        
        if (((MleTables.MleRTSetEntry)(MleTables.g_mleRTSet.elementAt(indexToDppTOC))).m_theSet == null)
    	{
    		// Find the Set chunk in the playprint, create the Set,
    		// and initialize the Set property values.
    		createSet(indexToDppTOC);

    		// Run the initialization function.
    		((MleTables.MleRTSetEntry)(MleTables.g_mleRTSet.elementAt(indexToDppTOC))).m_theSet.init();
        }

        return ((MleTables.MleRTSetEntry)(MleTables.g_mleRTSet.elementAt(indexToDppTOC))).m_theSet;
    }
    
    // An event callback used for reading a MediaRef from the Digital
    // Playprint.
    private class MediaRefCB extends MleEventCallback
    {
        /*
         * The callback dispatch method.
         * 
         * @param event The event that is being dispatched by the handler.
         * @param clientData Client data registered with this handler.
         * 
         * @return If the event is successfully dispatched, then <b>true</b> will be
         * returned. Otherwise, <b>false</b> will be returned.
         * 
     	 * @see com.wizzer.mle.runtime.event.MleEventCallback#dispatch(com.wizzer.mle.runtime.event.MleEvent, java.lang.Object)
    	 */
    	public boolean dispatch(MleEvent event,Object clientData)
    	{
    	    boolean status = true;
    	    
    	    if (MleDppModel.MleDppMediaRefInfoChunk.class.isInstance(clientData))
    	    {
	    	    MleDppModel.MleDppMediaRefInfoChunk data =
	    	        (MleDppModel.MleDppMediaRefInfoChunk) event.getCallData();
	    	    MleDppModel.MleDppMediaRefInfoChunk chunk = 
	    	        (MleDppModel.MleDppMediaRefInfoChunk) clientData;
	    	    chunk.m_flags = data.m_flags;
	    	    chunk.m_type = data.m_type;
	    	    chunk.m_numMrefs = data.m_numMrefs;
    	    } else if (MleDppModel.MleDppMediaRefChunk.class.isInstance(clientData))
    	    {
	    	    MleDppModel.MleDppMediaRefChunk data =
	    	        (MleDppModel.MleDppMediaRefChunk) event.getCallData();
	    	    MleDppModel.MleDppMediaRefChunk chunk = 
	    	        (MleDppModel.MleDppMediaRefChunk) clientData;
	    	    chunk.m_flags = data.m_flags;
	    	    chunk.m_type = data.m_type;
	    	    chunk.m_size = data.m_size;
	    	    chunk.m_data = new byte[data.m_data.length];
	    	    //for (int i = 0; i < data.m_data.length; i++)
	    	    //    chunk.m_data[i] = data.m_data[i];
	    	    System.arraycopy(data.m_data,0,chunk.m_data,0,data.m_data.length);
    	    } else
    	    {
    	        status = false;
    	    }
    	    
    	    return status;
    	}

    }
    
    // Helper utility for creating a MediaRef.
    private MleMediaRef createMediaRef(int id)
        throws MleDppException
    {
        MleMediaRef mref = null;
        try
        {
	    	MleTables.MleRTMediaRefEntry entry = 
	    	    (MleTables.MleRTMediaRefEntry) MleTables.g_mleRTMediaRef.elementAt(id);
	    	mref = (MleMediaRef) entry.createMediaRef();
        } catch (Exception ex)
        {
            throw new MleDppException("Unable to create a new MediaRef.");
        }
    	return mref;
    }

    /**
     * Load a MediaRef from the Digital Playprint.
     * 
     * @param indexToDppTOC The identifier for the MediaRef to load.
     * This is an offset into the Digital Playprint Table-Of-Content.
     * 
     * @return The loaded <code>MleMref</code> is returned. If the MediaRef
     * was not successfully loaded, then <b>null</b> will be returned.
     * 
     * @throws MleDppException This exception may be thrown if the Scene can not
     * be successfully loaded.
     */
    public MleMediaRef mleLoadMediaRef(int indexToDppTOC)
        throws MleDppException
    {
        MleMediaRef mref = null;
        
        // Get the Digital Playprint file.
        RandomAccessFile file = MleTitle.getInstance().m_theDpp.getFile();
        
        // XXX -- Fix when DPP API is available.
        // Get the mediaref offset from the DPP TOC. Then seek to that position.
        try
        {
	        int offset = (int)MleTitle.getInstance().m_theDpp.getTOCOffset(indexToDppTOC);
	        MleDppUtil.seek(file,offset,MleDppUtil.SEEK_SET);
        } catch (MleDppException ex)
        {
            throw new MleDppException("Unable to read Digital Playprint.");
        } catch (IOException ex)
        {
            throw new MleDppException("Unable to read Digital Playprint.");
        }

        // Read MediaRef chunk from Playprint.
        MleDppModel.MleDppMediaRefInfoChunk mrefInfoData =
            new MleDppModel.MleDppMediaRefInfoChunk();
        MleTitle.getInstance().m_theDpp.beginMref(new MediaRefCB(),mrefInfoData);

        // Create a new MediaRef of specified type.
        mref = createMediaRef((int)mrefInfoData.m_type);
        if (mref != null)
        {
            MleDppModel.MleDppMediaRefChunk chunks[] =
                new MleDppModel.MleDppMediaRefChunk[(int)mrefInfoData.m_numMrefs];
            for (int i = 0; i < mrefInfoData.m_numMrefs; i++)
            {
                // Read the meda from the Playprint.
                chunks[i] = new MleDppModel.MleDppMediaRefChunk();
                MleTitle.getInstance().m_theDpp.readMedia(new MediaRefCB(),chunks[i]);

                // Register the media with the MediaRef.
                mref.registerMedia(chunks[i].m_flags,chunks[i].m_data.length,chunks[i].m_data);
            }
        }

        // Clean up Playprint reading.
        MleTitle.getInstance().m_theDpp.endMref(null,null);

        return mref;
    }

    /**
     * Load the Digital Playprint.
     * 
     * @param filename The Digital Playprint file.
     * @param type The type of Digital Playprint to load.
     * 
     * @return If the Digital Playprint is successfully loaded, then a reference
     * to a <code>MleDppInput</code> is returned. If the DPP fails to load, then <b>null</b>
     * will be returned.
     */
    public MleDppInput mleLoadPlayprint(String filename,long type)
    {
        MleDppInput pp;
        
        try
        {
	        pp = new MleDppInput(filename);
	        pp.begin((int)type);
	        pp.readHeader(null,null);
	        pp.readTOC(null,null);
        } catch (MleDppException ex)
        {
            // An exception occurred while attempting to read the DPP.
            pp = null;
        }

        return pp;
    }

    /**
     * Unload the specified Digital Playprint.
     * 
     * @param pp The Digital Playprint to unload.
     */
    public void mleUnloadPlayprint(MleDppInput pp)
    {
        try
        {
            pp.end();
        } catch (MleDppException ex)
        {
            // Do nothing for now.
        }
        pp = null;
    }
}

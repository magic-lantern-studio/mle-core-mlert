/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MleLoad.cxx
 * @ingroup MleFoundation
 *
 * @author Mark S. Millard
 * @date May 1, 2003
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2003-2018 Wizzer Works
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//  For information concerning this source file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

// Include system header files.
#include <stdio.h>
#include <memory.h>
#ifdef WIN32
// include system header files
#include <string.h>
#endif 

// Include Magic Lantern header files.
#include "mle/mlFileio.h"
#include "mle/mlAssert.h"
#include "mle/mlErrno.h"
#include "mle/mlMalloc.h"

#include "mle/MleLoad.h"
#include "mle/MleDirector.h"
#include "mle/MleActor.h"
#include "mle/MleActorGC.h"
#include "mle/MleRole.h"
#include "mle/MleSet.h"
#include "mle/MleGroup.h"
#include "mle/MleMediaRef.h"
#include "mle/MleScene.h"

#ifdef MLE_DIGITAL_WORKPRINT

// Include the Magic Lantern Digital Workprint header files.
#include "mle/DwpActor.h"
#include "mle/DwpActorTemplate.h"
#include "mle/DwpRoleAttachment.h"
#include "mle/DwpRoleBinding.h"
#include "mle/DwpGroup.h"
#include "mle/DwpGroupRef.h"
#include "mle/DwpMediaRef.h"
#include "mle/DwpFinder.h"
#include "mle/DwpProperty.h"
#include "mle/DwpScene.h"
#include "mle/DwpStrKeyDict.h"

// Include the Magic Lantern Runtime Engine header files.
#include "mle/mlExpandFilename.h"
#include "mle/MleDso.h"
#include "mle/MleActorClass.h"
#include "mle/MleRoleClass.h"
#include "mle/MleGroupClass.h"
#include "mle/MleMediaRefClass.h"
#include "mle/MleSceneClass.h"


// Callback ptr used by _mlLoadGroup to call a client on DSO errors.
static void (*mlLoadErrorCB)(MleDwpActor *wpActor) = NULL;

/*
 * This static function is a convenience function to create an
 * actor from an MleDwpActor, the actor workprint item.  It does
 * the work of referring to the actor registry, creating the
 * actor, finding property values and poking them in, and attaching
 * a role if specified.
 */
static MleActor *_mlCreateActor(MleDwpActor *wpa)
{
    int i;	// Loop variable,

    // We need to find two things, the template and the actor class.
    //   First, find the root
    MleDwpItem *root = wpa;
    while ( root->getParent() )
	    root = root->getParent();

    // Declare the actor class name.
    //   Here we initialize to the type specified in the actor.  This can
    //   be a actor template name, not a class name.  If we want to restrict
    //   it to be only a template name, we shouldn't do the assignment.  As
    //   it is, if a template is not found it is assumed to be a class name.
    const char *acname = wpa->getActorClass();

    // Now search for the template.
#if defined(WIN32)
	// Note: Use string type method instead of typeId mechanism. The typeId
    // method doesn't work for Windows because the global address space for
	// the DWP.dll is different than the one used by the mle runtime.
	MleDwpFinder tmplFinder("MleDwpActorTemplate",wpa->getActorClass());
#else
    MleDwpFinder tmplFinder(MleDwpActorTemplate::typeId,wpa->getActorClass());
#endif
    MleDwpActorTemplate *tmpl = (MleDwpActorTemplate *)tmplFinder.find(root);

    // If we found one, we use that to define the actor class name.
    if ( tmpl )
        acname = tmpl->getActorClass();

    // Look up actor class.
    const MleActorClass *ac = MleActorClass::find(acname);

    if ( ac == NULL )
    {
        /* Try loading the DSO */
        if ( mlLoadDSO(wpa->getActorClass()) )
        {
            printf("MleLoad: ERROR loading DSO for %s.\n",acname);
            fflush(stdout);
            return NULL;
        }

        // Try to look up the class again.
        ac = MleActorClass::find(acname);
    }

    if ( ac == NULL )
        return NULL;

    // Create the actor.
    MleActor *actor = ac->createInstance();

    // Register with instance registry.
    if (actor) actor->registerInstance(wpa->getName());
    
    // Get ready to find properties and set them.
#if defined(WIN32)
	// Note: Use string type method instead of typeId mechanism. The typeId
    // method doesn't work for Windows because the global address space for
	// the DWP.dll is different than the one used by the mle runtime.
	MleDwpFinder propFinder("MleDwpProperty",NULL,1);
#else
    MleDwpFinder propFinder(MleDwpProperty::typeId,NULL,1);
#endif
    int nProps;
    MleDwpProperty **prop;

    // If there is a template, set those properties first.
    if ( tmpl )
    {
		// Execute the finder.
		propFinder.find(tmpl);
		nProps = propFinder.getNumItems();
		prop = (MleDwpProperty **)propFinder.getItems();

		for ( i = 0; i < nProps; i++ )
			actor->poke(prop[i]->getName(),&prop[i]->m_data);
    }

    // Set the properties in the actor.
    propFinder.find(wpa);
    nProps = propFinder.getNumItems();
    prop = (MleDwpProperty **)propFinder.getItems();

    for ( i = 0; i < nProps; i++ )
		actor->poke(prop[i]->getName(),&prop[i]->m_data);

    // Find the role binding if any.
    MleDwpFinder delFinder;
#if defined(WIN32)
	// Note: Use string type method instead of typeId mechanism. The typeId
    // method doesn't work for Windows because the global address space for
	// the DWP.dll is different than the one used by the mle runtime.
	delFinder.setType("MleDwpRoleBinding");
#else
    delFinder.setType(MleDwpRoleBinding::typeId);
#endif
    MleDwpRoleBinding *binding;

    // Search the actor first, then the template.
    binding = (MleDwpRoleBinding *)delFinder.find(wpa);
    if ( binding == NULL && tmpl )
	    binding = (MleDwpRoleBinding *)delFinder.find(tmpl);
	
    if ( binding )
    {
		// Determine the Set.

        // Get the Set here.
		MleDwpStrKeyDict *setInstances = MleSet::getInstanceRegistry();
        MleSet::g_currentSet = (MleSet *) setInstances->find(
            binding->getSet());

        // If no set, try to create it
        if ( MleSet::g_currentSet == NULL )
        {
            MleSet::g_currentSet = mlLoadSet(binding->getSet());

	    // if there is no forum at this point, that is an error
	}

	// Test for a real binding.
	//   It is allowed for a binding to have a set specified but
	//   not a role.  This is so that actors may be created
	//   with a MleSet::g_currentSet set but no role.
	if ( binding->getName() && strcmp(binding->getName(),"NULL") )
	{
        MleRole *(*createRole)(MleActor*) =
            MleRoleClass::find(binding->getName());
        
        if ( createRole == NULL )
        {
            // Try loading the DSO.
            if ( mlLoadDSO(binding->getName()) )
                printf("***** ERROR loading DSO for %s.\n",
	        binding->getName());

            // Try to look up the function again.
            createRole =
                MleRoleClass::find(binding->getName());
        }

        if ( createRole )
            (*createRole)(actor);
	}
    }

    return actor;
}

/*
 * This static function is a convenience function to create an
 * group from an MleDwpGroup, the group workprint item.  It does
 * the work of referring to the group registry and creating the
 * group.
 */
static MleGroup *_mlCreateGroup(MleDwpGroup *wpg)
{
    // Declare the group class name.
    //   Here we initialize to the type specified in the group.  
    const char *gpname = wpg->getGroupClass();

    // Look up group class.
    const MleGroupClass *gc = MleGroupClass::find(gpname);

    if ( gc == NULL )
    {
        // Try loading the DSO.
        if ( mlLoadDSO(wpg->getGroupClass()) )
        {
            printf("***** ERROR loading DSO for %s.\n",gpname);
            return NULL;
        }

        // Try to look up the class again.
        gc = MleGroupClass::find(gpname);
    }

    if ( gc == NULL )
        return NULL;

    // Create the group.
    MleGroup *group = gc->createInstance();

    // Register with instance registry.
    if (group) group->registerInstance(wpg->getName());
    
    return group;
}

/*
 * This function is the rehearsal player scene loading facility.
 * It takes a workprint identifier for the scene id and returns
 * a pointer to the loaded group.
 */
MleGroup *_mlLoadGroup(MleDwpGroup* wpGroup)
{

    int i,j; // Loop variables.

    int numActors = 0;
    MleSet **sets = NULL;	// Array of actors' sets.

    if ( wpGroup == NULL )
    {
        printf("mlLoadGroup: Cant find group in workprint.\n");
        return NULL;
    }
    
    MleGroup *group = _mlCreateGroup(wpGroup);

    // Create a registry for actors.
    MleDwpStrKeyDict actorDict(32);

    // Now look for actors.
    MleDwpFinder actorFinder;
#if defined(WIN32)
	// Note: Use string type method instead of typeId mechanism. The typeId
    // method doesn't work for Windows because the global address space for
	// the DWP.dll is different than the one used by the mle runtime.
	actorFinder.setType("MleDwpActor");
#else
    actorFinder.setType(MleDwpActor::typeId);
#endif
    actorFinder.setFindAll(TRUE);
    actorFinder.find(wpGroup);
    for ( j = 0; j < actorFinder.getNumItems(); j++ )
    {
        // Get an actor.
        MleDwpActor *wpa = (MleDwpActor *)actorFinder.getItems()[j];

        // And call the convenience function to create it.
        MleActor *actor = _mlCreateActor((MleDwpActor *)wpa);

	    // Remember 1-based index.
	    //   This is a hack because the dictionary returns 0 if
	    //   a key is not present.  Since 0 is a real value, we
	    //   have to have a different encoding for it.  A fix
	    //   would be to use a dictionary that returns ints that
	    //   has a separate mechanism for notifying that a key
	    //   is not present.
	    // Register with the group.
        group->m_dictionary.set(wpa->getName(), (void*) (1 + numActors));

        // Check to see if actor got created.
        //   Note we do this *after* the return value is
        //   set because the caller may expect the first
        //   actor to be a particular type.
        if ( actor == NULL )
        {
			//If we have a client callback, then
			// inform them of the DSO error.
			if (mlLoadErrorCB)
				(*mlLoadErrorCB)(wpa);

            printf("_mlLoadGroup: ***** ERROR creating actor %s.\n",
                wpa->getName());
            continue;
        }

        // Put the roles in the registry if they exist
	    //   and assign them to the set.
        MLE_ASSERT(wpa->getName());
        if ( actor->getRole() )
		{
	        // Remember 1-based index.
	        //   This is a hack because the dictionary returns 0 if
	        //   a key is not present.  Since 0 is a real value, we
	        //   have to have a different encoding for it.  A fix
	        //   would be to use a dictionary that returns ints that
	        //   has a separate mechanism for notifying that a key
	        //   is not present.
            actorDict.set(wpa->getName(), (void *)(numActors + 1));
	        actor->getRole()->m_set = MleSet::g_currentSet;
		}

        // Add to list.
	    // we keep an array of actors and their forums
        group->add(actor);
        sets = (MleSet **)mlRealloc(sets,
            (numActors + 1)*sizeof(MleActor *));
        sets[numActors] = MleSet::g_currentSet;
	    numActors++;
    }

    // Create an array that flags whether actor's roles have been attached.
    int *attached = (int *)mlMalloc(numActors*sizeof(int));
    for ( j = 0; j < numActors; j++ )
		attached[j] = 0;

    // Now look for delegate attachments.
    MleDwpFinder atFinder;
#if defined(WIN32)
	// Note: Use string type method instead of typeId mechanism. The typeId
    // method doesn't work for Windows because the global address space for
	// the DWP.dll is different than the one used by the mle runtime.
	atFinder.setType("MleDwpRoleAttachment");
#else
    atFinder.setType(MleDwpRoleAttachment::typeId);
#endif
    atFinder.setFindAll(TRUE);
    atFinder.find(wpGroup);
    for ( j = 0; j < atFinder.getNumItems(); j++ )
    {
        // Get an attachment.
        MleDwpRoleAttachment *da =
            (MleDwpRoleAttachment *) atFinder.getItems()[j];
            
        MLE_ASSERT(da->getParent());
        MLE_ASSERT(da->getChild());

        // Look up the parent and child dels from the dict.
	    //   convert from 1-based indexed to 0-based index
	    //   We use 1-based indices in the dictionary because the
	    //   dictionary returns 0 as an error condition for key not found.
	    long parentIndex = (long)actorDict.find(da->getParent());
	    long childIndex = (long)actorDict.find(da->getChild());

        MleRole *parent = NULL;
        MleRole *child = NULL;
	    if ( parentIndex )
	         parent = (*group)[parentIndex - 1]->getRole();
	    if ( childIndex )
	        child = (*group)[childIndex - 1]->getRole();

        // Make sure both exist.
        if ( parent == NULL || child == NULL )
            printf("_mlLoadGroup: can't find roles for parent %s (0x%x) or child %s (0x%x).\n",da->getParent(),parent,da->getChild(),child);
	    else if ( sets[parentIndex] != sets[childIndex] )
	        printf("_mlLoadGroup: set mismatch between parent %s and child %s.\n",da->getParent(),parent,da->getChild());
	    else
	        sets[parentIndex]->attach(parent,child);
            
        // Now remove the child from the registry.
        //   We do this so no child can be given more
        //   than one parent.  Also, what is left in the
        //   registry will all be parented directly to
        //   the set.
        actorDict.remove(da->getChild());

	    // Mark the actor as having been attached.
	    //   This is an additional means to ensure that no child will
	    //   be given more than one parent.  It also allows determining
	    //   easily which children have no parent by array traversal.
	    if ( childIndex )
		    attached[childIndex - 1] = 1;
    }

    /* Now anything left should be parented directly to the forum. */
    for ( j = 0; j < numActors; j++ )
    {
	    // Get the role.
        MleRole *role = (*group)[j]->getRole();
	
	    // Make sure we want to do this.
	    //   The set array entry will be non-NULL only if this role
    	//   is not already a child of some other role.
	    if ( !attached[j] && sets[j] && role )
            sets[j]->attach(NULL, role);
    }

    // Call all the init() functions on the actors created.
    // Call resolve edit function as well.
    for ( i = 0; i < numActors; i++ )
    {
	    // Set the set this actor was placed in.
	    MleSet::g_currentSet = sets[i];

        (*group)[i]->init();
        (*group)[i]->resolveEdit();
    }

    // Call the init() function on the group containing the actors.
    group->init();

    // Initialize the MleSet::g_currentSet back to NULL since this
    // variable is only valid during actor's init() function.
    MleSet::g_currentSet = NULL;

    mlFree(sets);
    mlFree(attached);

    return group;
}

/*
 * This function is the rehearsal player scene loading facility.
 * It takes a workprint identifier for the scene id and returns
 * a pointer to the loaded group.
 *
 * The rehearsal _mlLoadGroup() requires extern functions.
 *
 * MleDwpGroup *_mlGetWorkprintGroup(const char *groupname);
 * void _mlReleaseWorkprintGroup(MleDwpItem *item);
 *
 * The first should return a pointer to a workprint actor group,
 * which is a tree of group, actor sets, actors, role bindings,
 * etc.  The second is a call to release the group, in case this
 * is a temporary allocation.
 */
MleGroup *mlLoadGroup(const char *id)
{
    // Declare required externs to find and release workprint group.
    extern void _mlReleaseWorkprintGroup(MleDwpItem *item);
    extern MleDwpGroup *_mlGetWorkprintGroup(const char *groupname);


    MleDwpGroup *wpGroup = (MleDwpGroup*) _mlGetWorkprintGroup(id);
    if ( wpGroup == NULL )
    {
        printf("mlLoadGroup: Cant find group %s in workprint.\n" ,id );
        return NULL;
    }
    MleGroup* group = _mlLoadGroup( wpGroup );

    // Release the workprint group.
    _mlReleaseWorkprintGroup(wpGroup);

    return(group);
}

MleGroup *mlLoadGroup(MleDwpGroup* wpGroup)
{
    return(_mlLoadGroup(wpGroup));
}


// Implement the loading for scenes.

/*
 * This static function is a convenience function to create an
 * scene from an MleDwpScene, the scene workprint item.  It does
 * the work of referring to the scene registry and creating the
 * scene.
*/
static MleScene *_mlCreateScene(MleDwpScene *wps)
{
    // Declare the scene class name.
    //   Here we initialize to the type specified in the scene.  
    const char *scnname = wps->getSceneClass();

    // Look up scene class,
    const MleSceneClass *scn = MleSceneClass::find(scnname);

    if ( scn == NULL )
    {
        // Try loading the DSO.
        if ( mlLoadDSO(wps->getSceneClass()) )
        {
            printf("***** ERROR loading DSO for %s.\n",scnname);
            return NULL;
        }

        // Try to look up the class again.
        scn = MleSceneClass::find(scnname);
    }

    if ( scn == NULL )
        return NULL;

    // Create the scene.
    MleScene *scene = scn->createInstance();

    // Register with instance registry.
    if (scene) scene->registerInstance(wps->getName());
    
    return scene;
}

/*
 * This function is the rehearsal player scene loading facility.
 * It takes a workprint identifier for the scene id and returns
 * a pointer to the loaded scene.
 */
MleScene *_mlLoadScene(MleDwpScene* wpScene)
{
    // Declare required externs to find and release workprint group.
    extern void _mlReleaseWorkprintGroup(MleDwpItem *item);
    extern MleDwpGroup *_mlGetWorkprintGroup(const char *groupname);


    int j; // Loop variables.

    int numGroups = 0;

    if ( wpScene == NULL )
    {
        printf("mlLoadScene: Cant find scene in workprint.\n");
        return NULL;
    }
    
    MleScene *scene = _mlCreateScene(wpScene);
    if (NULL == scene) {
	    return NULL;
    }

    // mlLoadScene() always puts it\'s result into g_currentScene
    // and does this before the groups are loaded and before its
    // init() call, in case the groups or the init()
    // call must use it.
    (void) scene->changeCurrentScene( scene );

    // First, find the root.
    MleDwpItem *root = wpScene;
    while ( root->getParent() )
	root = root->getParent();

    // Now look for groups.
    MleDwpFinder groupFinder;
    groupFinder.setCallback(&MleDwpGroup::groupOrGroupRefFinderCB, NULL);
    groupFinder.setFindAll(TRUE);
    groupFinder.find(wpScene);
    for ( j = 0; j < groupFinder.getNumItems(); j++ )
    {
	    // Get an group.
	    MleDwpGroup *wpg;
	    MleDwpItem *item = (MleDwpItem *)groupFinder.getItems()[j];
#if defined(WIN32)
		if (item->isa("MleDwpGroup"))
#else
	    if (MleDwpGroup::typeId == item->getTypeId())
#endif
		{
		    wpg = (MleDwpGroup *)item;
	    }
#if defined(WIN32)
		else if (item->isa("MleDwpGroupRef"))
#else
	    else if (MleDwpGroupRef::typeId == item->getTypeId())
#endif
		{
		    // Find Group in workprint from GroupRef in this scene.
		    wpg = _mlGetWorkprintGroup(item->getName());
		    if (wpg == NULL)
		    {
			    printf("mlLoadGroup: GroupRef named %s not found\n", item->getName());
			    continue;
		    }
	    } 
	    else {
		    MLE_ASSERT(MleDwpGroup::typeId == item->getTypeId() ||
			      MleDwpGroupRef::typeId == item->getTypeId());
	    }

	    // Remember 1-based index.
	    //   This is a hack because the dictionary returns 0 if
	    //   a key is not present.  Since 0 is a real value, we
	    //   have to have a different encoding for it.  A fix
	    //   would be to use a dictionary that returns ints that
	    //   has a separate mechanism for notifying that a key
	    //   is not present.
	    // Register with the scene.
	    scene->m_dictionary.set(wpg->getName(), (void*) (1+numGroups));

	    // And call the convenience function to create it.
	    MleGroup *group = _mlLoadGroup((MleDwpGroup *)wpg);

	    // Check to see if group got created.
	    if ( group == NULL )
	    {
		    printf("_mlLoadScene: ***** ERROR creating group %s.\n",
			   wpg->getName());
		    continue;
	    }

#if defined(WIN32)
		if (item->isa("MleDwpGroupRef"))
#else
	    if (MleDwpGroupRef::typeId == item->getTypeId())
#endif
		{
		    // Find now release the wire-loaded Group.
		    _mlReleaseWorkprintGroup(wpg);
	    }

	    // The scene holds a reference to each group.
	    scene->add(group);
	    numGroups++;
    }

    // Call the init() function on the scene containing the groups.
    scene->init();

    return scene;
}

MleScene *mlLoadScene(const char *id)
{
    // Declare required externs to find and release workprint scene.
    extern void _mlReleaseWorkprintScene(MleDwpItem *item);
    extern MleDwpScene *_mlGetWorkprintScene(const char *scenename);

    MleDwpScene *wpScene = (MleDwpScene*) _mlGetWorkprintScene(id);
    if ( wpScene == NULL )
    {
        printf("mlLoadScene: Cant find scene %s in workprint.\n" ,id );
        return NULL;
    }
    MleScene* scene = _mlLoadScene( wpScene );

    // Release the workprint scene.
    _mlReleaseWorkprintScene(wpScene);

    return(scene);
}

MleScene *mlLoadScene(MleDwpScene* wpScene)
{
    return(_mlLoadScene(wpScene));
}

// Setup for client callback on DSO load error.
void mlSetLoadErrorCallback(void (*loadErrorCB)(MleDwpActor *wpActor))
{
    mlLoadErrorCB = loadErrorCB;
}

/*
 * Load media from the Digital Workprint.
 */
MleMediaRef *
mlLoadMediaRef(const char *id, void* /*userData*/)
{
    // Declare external functions.
    extern MleDwpMediaRef *_mlGetWorkprintMediaRef(const char *groupname);
    extern void _mlReleaseWorkprintMediaRef(MleDwpMediaRef *mediaRef);

    // Declare local variables.
    MleDwpMediaRef *wpMediaRef;
    MleMediaRef *mediaRef;

    // Find DWP MediaRef in workprint group.
    wpMediaRef = _mlGetWorkprintMediaRef(id);
    if (wpMediaRef == NULL)
    {
        printf("mlLoadMediaRef: media named %s not found\n", id);
        return 0;
    }

    // Fet MediaRef class constructor.
    MleMediaRef *(*createMediaRef)() =
        MleMediaRefClass::find(wpMediaRef->getMediaRefClass());
    if ( createMediaRef == NULL )
        return NULL;

    // Instantiate the MediaRef class.
    mediaRef = (*createMediaRef)();
    if ( mediaRef == NULL) {
        return NULL;
    } else {
        MleDwpMediaRefTargetMedia *entries;
        int numEntries;

        // Retrieve target entries from the DWP.
        numEntries = wpMediaRef->queryTargetEntries(&entries);

        // Cache the entries for processing by the MediaRef class.
        if (! mediaRef->load(numEntries,(void *)entries))
		{
			wpMediaRef->releaseTargetEntries(&entries);
            return NULL;
		}

		wpMediaRef->releaseTargetEntries(&entries);
    }

    // XXX - register with instance registry here (for tools?)

    // Release the workprint MediaRef.
    _mlReleaseWorkprintMediaRef(wpMediaRef);

    return mediaRef;
}


MleScene *
mlLoadBootScene(MleDwpItem *item)
{
	// First, find the root.
	MleDwpItem *root = item;
	MleDwpItem *parent;
	while ( NULL != (parent = root->getParent()) ) {
		root = parent;
	}

	// Look for scenes.
	MleDwpFinder sceneFinder;
#if defined(WIN32)
	// Note: Use string type method instead of typeId mechanism. The typeId
    // method doesn't work for Windows because the global address space for
	// the DWP.dll is different than the one used by the mle runtime.
	sceneFinder.setType("MleDwpScene");
#else
	sceneFinder.setType(MleDwpScene::typeId);
#endif
	sceneFinder.setFindAll(TRUE);
	sceneFinder.find(root); 

	// get the first scene.
	MleDwpScene *wps = (MleDwpScene *)sceneFinder.getItem();

	// The first scene is the boot scene, by convention.
	return mlLoadScene(wps);
}

#endif /* Digital Workprint loading. */

#ifdef MLE_DIGITAL_PLAYPRINT

// Include Magic Lantern Runtime Player header files.
#include "mle/ppinput.h"
#include "mle/chunk.h"  // XXX -- should not have this dependency
#include "mle/MleTables.h"


char *
mlGenRegistryKeyFromMedia(int id)
{
    int *key = (int *) mlMalloc(4);

    // ((id<<8) & 0xfff0)
#ifdef WIN32 
    *key = ((((id)&0x7f)<<1)|(((id)&0x3f80)<<2)| \
            (((id)&0x1fc000)<<3)|0x00010101);
#else
    *key = ((((id)&0x7f)<<25)|(((id)&0x3f80)<<10)| \
            (((id)&0x1fc000)>>5)|0x01010100);
#endif /* WIN32 */

    return (char *) key;
}


static int _getMediaRefInfo(void *clientData,void *callData)
{
    memcpy(clientData,callData,sizeof(MleMediaRefInfoChunk));
    return(0);
}


// Load media from the Digital Playprint.
MleMediaRef *
mlLoadMediaRef(const int id, void* /*userData*/)
{
    // declare local variables
    MleMediaRef *mediaRef;
    MleMediaRefInfoChunk mrefInfoData;

#if defined(MLE_DEBUG)
    if (g_theTitle->m_dpp == NULL)
    {
        printf("mlLoadMediaRef: Digital Playprint not initialized!\n");
        return NULL;
    }
#endif /* MLE_DEBUG */

    // go to position within Playprint.
    g_theTitle->m_dpp->goTo(g_theTitle->m_dpp->getTOCOffset((unsigned int) id));

    // read MediaRef type from Playprint.
    g_theTitle->m_dpp->beginMref(_getMediaRefInfo,(void *)&mrefInfoData);

    // create a new MediaRef of specified type.
    mediaRef = (*mlRTMediaRef[mrefInfoData.m_type].constructor)();
    if ( mediaRef != NULL )
	{
        // load MediaRef data
        mediaRef->load(mrefInfoData.m_numMrefs,(void *)g_theTitle->m_dpp);
    }

    // clean up Playprint reading.
    g_theTitle->m_dpp->endMref(NULL,NULL);

    return mediaRef;
}


MleGroup *mlLoadGroup(const int indexToPpTOC)
{
	extern MleSet **_mleSetArray;	// declared in MleActorGC.cxx
	unsigned int length;

	FILE *file = g_theTitle->m_dpp->getFp();
	// XXX -- fix when PP API is available

	mlFSeek(file,g_theTitle->m_dpp->getTOCOffset(indexToPpTOC)+4,SEEK_SET);
	mlFRead(&length,sizeof(length),1,file);
  
	unsigned char *actorGroupChunk = new unsigned char[length];
	unsigned char *chunkPtr = actorGroupChunk;
	unsigned char *&chunkRef = chunkPtr;
  
	mlFRead(actorGroupChunk, length, 1, file);

	// XXX Should also get the group type from the DPP (before 1st actor)
	// to create here with a call to mleCreateGroup(<type>).
	int groupIndex;
	groupIndex = readInt(chunkRef, actorGroupChunk, actorGroupChunk + length);
	MleGroup *group = (*mlRTGroupClass[groupIndex].constructor)();
	if ( group == NULL )
	{
		  return NULL;
	}

	int numActors = readIndex(chunkRef, actorGroupChunk,
		actorGroupChunk + length);
	chunkInitialization(numActors);
	parseStream(chunkRef, actorGroupChunk, actorGroupChunk + length,
	    MLE_CHUNK_GROUP);
	delete actorGroupChunk;
	for (int i = 0; i < g_actorRegistryPtr - g_actorRegistry; i++)
	{
		// Set the current Set.
		MleSet::g_currentSet = _mleSetArray[i];

		// XXX Why not dispense with actor registry and use the group?
		// Well, the registry holds at least the actor\'s table index
		// and delegate as well, so isn\'t 100% redundant.
		group->add(g_actorRegistry[i].m_actor);

		g_actorRegistry[i].m_actor->init();
	}

	// Now finish the init sequence.
	group->init();

	//
	// Initialize the MleSet::g_currentSet back to NULL since this
	// variable is only valid during actor's init() function.
	//
	MleSet::g_currentSet = NULL;

	// Clean up before exit.
	delete g_actorRegistry;
	delete _mleSetArray;

	return group;
}


MleScene *mlLoadScene(const int indexToPpTOC)
{
	extern MleSet **_mleSetArray;	// Declared in MleGroupGC.cxx.
	unsigned int length;

	FILE *file = g_theTitle->m_dpp->getFp();

	// XXX -- fix when DPP API is available
	mlFSeek(file,g_theTitle->m_dpp->getTOCOffset(indexToPpTOC)+4,SEEK_SET);
	mlFRead(&length,sizeof(length),1,file);
  
	unsigned char *sceneChunk = new unsigned char[length];
	unsigned char *chunkPtr = sceneChunk;
	unsigned char *&chunkRef = chunkPtr;
	mlFRead(sceneChunk, length, 1, file);

	// XXX Should also get the scene type from the sceneClass[indexToPpTOC]
	// in order to create the right scene type object, not just MleScene, here.
	int sceneIndex = readInt(chunkRef, sceneChunk, sceneChunk + length);
	MleScene *scene = (*mlRTSceneClass[sceneIndex].constructor)();
	if ( scene == NULL )
	{
		return NULL;
	}

	// mlLoadScene() always puts it\'s result into currentScene
	// and does this before the groups are loaded and before its
	// init() call, in case the groups or the init()
	// call must use it.
	scene->changeCurrentScene( scene );

	// For each group in the scene, read its index (into global
	// list of groups) and then load that group.
	// Format of the chunk is
	// [ numGroups, [ g0index, g1index, ... gNumIndex ] ]
	//
	int numGroups = readInt(chunkRef, sceneChunk, sceneChunk + length);
	for (int i = 0; i < numGroups; i++)
	{
		// readInt() increments the chunk ptr, so just call repeatedly
		// XXX Kinda funny that the ReadInt() we use resides in actorgc.cxx
		int groupIndex = readInt(chunkRef, sceneChunk, sceneChunk + length);

		scene->add(mlLoadGroup( groupIndex ));
	}

	// Now finish the init sequence.
	scene->init();

	// Clean up before exit.
	delete sceneChunk;

	return scene;
}


// To be picked up from the title\'s mlChunk.cxx file.
extern int MleDppBootScene;


MleScene *
mlLoadBootScene(void)
{
    // The first scene is the boot scene, by convention.
    return mlLoadScene(MleDppBootScene);
}


MleDppInput *
mlLoadPlayprint(const char *filename,unsigned long type)
{
    MleDppInput *dpp = new MleDppInput(filename);
    dpp->begin(type);
    dpp->readHeader(NULL,NULL);
    dpp->readTOC(NULL,NULL);

    return(dpp);
}


void
mlUnloadPlayprint(MleDppInput *dpp)
{
    dpp->end();
    delete dpp;
}

#endif /* Digital Playprint loading. */

/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MleLoad.h
 * @ingroup MleFoundation
 *
 * There are three possible numberings of a scene, group, or actor
 * that we must be concerned about.  They are
 * <ol>
 *   <li>chunk number of the item within the global list of all chunks,
 *       e.g., first group might be chunk 8.</li>
 *   <li>item number within the list of items of that type,
 *       e.g., Nth group would have group index [N-1] (0-based)</li>
 *   <li>item number within the parent item containing it,
 *       e.g., if Nth group is group 3 in 2nd scene, then
 *       its SceneGroup index within that scene is 3.<.li>
 * </ol>
 *
 * These three types of numbering are good for the activities
 * <ol>
 *   <li>item LOADing from workprint/playprint
 *     applicable to scenes and groups, but not actors.</li>
 *   <li>item class TYPEing within all items of a generic type (eg, groups)
 *       applicable to scenes, groups and actors.</li>
 *   <li>item FINDing within a parent container
 *       applicable to groups and actors.</li>
 * </ol>
 *
 * Thus the 7 macros for mleXXXRefTo{Load,Type,Find}().
 *
 * @author Mark S. Millard
 * @created May 5, 2003
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015 Wizzer Works
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
//  For information concerning this header file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

#ifndef __MLE_LOAD_H_
#define __MLE_LOAD_H_


// Include Magic Lantern header files.
#include "mle/mlMalloc.h"

#include "mle/MleRuntime.h"

class MleActor;
class MleSet;
class MleGroup;
class MleScene;
class MleMediaRef;
#ifdef MLE_REHEARSAL
class MleDwpItem;
class MleDwpActor;
class MleDwpGroup;
class MleDwpScene;
#else
class MleDppInput;
#endif /* MLE_REHEARSAL */

#ifdef MLE_REHEARSAL

#define mlSceneRefToType(ID) #ID
#define mlSceneRefToLoad(ID) #ID
#define mlDeclareSceneRefToType(ID)
#define mlDeclareSceneRefToLoad(ID)

#define mlGroupRefToFind(SID,GID) #GID
#define mlGroupRefToType(ID) #ID
#define mlGroupRefToLoad(ID) #ID
#define mlDeclareGroupRefToFind(SID,GID)
#define mlDeclareGroupRefToType(ID)
#define mlDeclareGroupRefToLoad(ID)

#define mlActorRefToType(ID) #ID
#define mlActorRefToFind(GID,AID) #AID
#define mlDeclareActorRefToType(ID)
#define mlDeclareActorRefToFind(GID,AID)

#define mlSetRefTo(ID) #ID
#define mlDeclareSetRef(ID)

#define mlMediaRefTo(ID) #ID
#define mlDeclareMediaRef(ID)


#define mlGenRegistryKeyFromMedia(id) id

#define mlFreeMediaRegistryKey(key) 0

extern MLE_RUNTIME_API MleScene *mlLoadScene(const char *id);

extern MLE_RUNTIME_API MleScene *mlLoadScene(MleDwpScene* scene);

extern MLE_RUNTIME_API MleScene *mlLoadBootScene(MleDwpItem* item);

extern MLE_RUNTIME_API MleGroup *mlLoadGroup(const char *id);

extern MLE_RUNTIME_API MleGroup *mlLoadGroup(MleDwpGroup* group);

extern MLE_RUNTIME_API MleMediaRef *mlLoadMediaRef(const char *id, void *userData);

extern MLE_RUNTIME_API MleSet *mlLoadSet(const char *id);

#else /* Playprint/Runtime */

/**
 * @brief mlSceneRefTo Macro.
 *
 * Converts a scene reference id into a global scene
 * name (rehearsal player) or index (target player).
 */
#define mlSceneRefToType(ID) MleDPPSceneID_##ID
#define mlSceneRefToLoad(ID) MleDPPScene_##ID
#define mlDeclareSceneRefToType(ID) extern int MleDPPSceneID_##ID
#define mlDeclareSceneRefToLoad(ID) extern int MleDPPScene_##ID


/**
 * @brief mlGroupRefTo Macro.
 *
 * Converts a group reference id into a global scene
 * name (rehearsal player) or index (target player).
 *
 * @param SID The Scene identifier.
 * @param GID The Group identifier.
 */
#define mlGroupRefToFind(SID,GID) MleDPPSceneGroupID_##SID_##GID
#define mlGroupRefToType(ID) MleDPPGroupID_##ID
#define mlGroupRefToLoad(ID) MleDPPGroup_##ID
#define mlDeclareGroupRefToFind(SID,GID) extern int MleDPPSceneGroupID_##SID_##GID
#define mlDeclareGroupRefToType(ID) extern int MleDPPGroupID_##ID
#define mlDeclareGroupRefToLoad(ID) extern int MleDPPGroup_##ID


/**
 * @brief mlActorRefTo Macro.
 * 
 * Converts an actor id into a pointer to the actor. The definition is
 * used as a symbol for a unique reference that is generated during
 * the targeting process.
 *
 * @param ID The actor identifier.
 * @param GID The group identifier.
 */
#define mlActorRefToType(ID) MleDPPActor_##ID
#define mlActorRefToFind(GID,AID) MleDPPGroupActorID_##GID_##AID
#define mlDeclareActorRefToType(ID) extern int MleDPPActor_##ID
#define mlDeclareActorRefToFind(GID,AID) extern int MleDPPGroupActorID_##GID_##AID


/**
 * @brief mlSetRefTo Macro.
 * 
 * Converts a set id into a pointer to the set. The definition is
 * used as a symbol for a unique reference that is generated during
 * the targeting process.
 *
 * @param The set identifier.
 */
#define mlSetRefTo(ID) MleDPPSet_##ID
#define mlDeclareSetRef(ID) extern int MleDPPSet_##ID


/**
 * @brief mlMediaRefTo Macro.
 *
 * Converts a media reference id into an  object suitable to serve as key for
 * the  media registry.  The definition is
 * used as a symbol for a unique reference that is generated during
 * the targeting process.
 *
 * @param ID The media reference identifier.
 */
#define mlMediaRefTo(ID) MleDPPMedia_##ID
#define mlDeclareMediaRef(ID) extern int MleDPPMedia_##ID


/**
 * @brief Generate a registry key for the specified identifier.
 *
 * @param id A unique identifier specifying the media reference to
 * generate a key for..
 *
 * @return A character string is returned repersenting the generated
 * key.
 */
extern char *_mlGenRegistryKeyFromMedia(int id);

#define mlGenRegistryKeyFromMedia(id) _mlGenRegistryKeyFromMedia(id)

#define mlFreeMediaRegistryKey(key) mlFree(key)


/**
 * @brief Load a scene.
 *
 * @param A unique identifier specifying the scene to load.
 *
 * @return A pointer to the loaded scene is returned.
 */
extern MLE_RUNTIME_API MleScene *mlLoadScene(const int id);


/**
 * @brief Load the boot scene.
 *
 * @param A unique identifier specifying the scene to load
 * upon title startup.
 *
 * @return A pointer to the loaded boot scene is returned.
 */
extern MLE_RUNTIME_API MleScene *mlLoadBootScene(void);


/**
 * @brief Load a group of actors.
 *
 * @param A unique identifier specifying the group to load.
 *
 * @return A pointer to the loaded group is returned.
 */
extern MLE_RUNTIME_API MleGroup *mlLoadGroup(const int id);


/**
 * @brief Load a media reference.
 *
 * @param id A unique identifier specifying the media reference to load.
 * @param userData User data to be passed to the loader.
 *
 * @return A pointer to the loaded media reference is returned.
 */
extern MLE_RUNTIME_API MleMediaRef *mlLoadMediaRef(const int id, void *userData);


/**
 * @brief Load a set of actors.
 *
 * @param A unique identifier specifying the set to load.
 *
 * @return A pointer to the loaded set is returned.
*/
extern MLE_RUNTIME_API MleSet *mlLoadSet(const int id);


/**
 * @brief Load a Digital Playprint from the specified file.
 *
 * XXX - This should really be a byte stream.
 *
 * @param filename The name of the Digital Playprint to load.
 * @param type The type of Digital Playprint to load.
 */
extern MLE_RUNTIME_API MleDppInput *mlLoadPlayprint(const char *filename, unsigned long type);


/**
 * @brief Unload a Digital Playprint.
 *
 * @param dpp The Digital Playprint to unload.
 */
extern MLE_RUNTIME_API void mlUnloadPlayprint(MleDppInput *dpp);

#endif /* MLE_REHEARSAL */

#ifdef MLE_REHEARSAL

// g_mlRehearsalWorkprint is a rehearsal player global that points to the
//   workprint hierarchy.
extern MLE_RUNTIME_API MleDwpItem *g_mlRehearsalWorkprint;

// mlLoadWorkprint() is a rehearsal player call that loads the indicated
//   workprint from the file system and roots the hierarchy at
//   g_mlRehearsalWorkprint.  Zero is returned in the event of an error.
extern MLE_RUNTIME_API MleDwpItem *mlLoadWorkprint(const char *filename);

// mlSetLoadErrorCallback() sets up a callback that _mlLoadGroup 
// calls when actor DSO's cannot be found.
void MLE_RUNTIME_API mlSetLoadErrorCallback(void (*loadErrorCB)(MleDwpActor *wpActor));

#endif /* MLE_REHEARSAL */

#endif /* __MLE_LOAD_H_ */

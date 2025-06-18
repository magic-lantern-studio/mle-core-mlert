/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MleScene.cxx
 * @ingroup MleFoundation
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015-2025 Wizzer Works
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
#ifdef _WINDOWS
#include <string.h>
#endif /* _WINDOWS */
#include <stdio.h>

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"

// Include Runtime Engine header files.
#include "mle/MleScene.h"
#include "mle/MleLoad.h"
#include "mle/MleRole.h"
#include "mle/MleSet.h"

#ifdef MLE_DIGITAL_WORKPRINT
/* Include all the workprint stuff group loading needs to deal with. */
#include "mle/DwpGroup.h"
#include "mle/DwpFinder.h"
#endif /* MLE_DIGITAL_WORKPRINT */

//#include "mle/MleDppInput.h"
//#include "mle/MleTables.h"
//#include "mle/MleFileio.h"
//#include "mle/MleMalloc.h"

// Base class implementation for Scenes.
//
// This implementation does not anticipate supporting subclasses of
// scenes in the Digital Workprint or the tools.


// Static member variables
MleScene* MleScene::g_currentScene = 0;
MleScene* MleScene::g_globalScene = 0;


// Constructors and destructors

// Default constructor makes sure that the groups array has some storage.

MleScene::MleScene(void)
  : MlePtrContainer<MleGroup*>(8)
{
#ifdef MLE_DIGITAL_WORKPRINT
    m_name = NULL;
    m_sceneClass = NULL;
#endif /* MLE_DIGITAL_WORKPRINT */
}


MleScene::~MleScene(void)
{
    // Delete all of our groups
    for (int i=0; i<getSize(); i++) {
        delete (*this)[i];
    }

#ifdef MLE_DIGITAL_WORKPRINT
    this->unregisterInstance();
    if (m_name) mlFree(m_name);
#endif /* MLE_DIGITAL_WORKPRINT */

    // Make sure we\'re no longer listed as the globally active scene.
    if (this == getCurrentScene()) {
        clearCurrentScene();
    }
}
  

void
MleScene::init(void)
{
    // Do nothing yet
}


void *
MleScene::operator new(size_t tSize)
{
    void *p = mlMalloc(tSize);
    return p;
}


void
MleScene::operator delete(void *p)
{
    mlFree(p);
}


MleScene *
_mlCreateMleScene(void)
{
    return new MleScene;
}

#ifdef MLE_DIGITAL_WORKPRINT
const char *
MleScene::getTypeName(void) const
{
    return "MleScene";
}

int
MleScene::isa(const char *type) const
{
    return !strcmp(type,"MleScene");
}

void
MleScene::initClass(void)
{
    new MleSceneClass("MleScene",_mlCreateMleScene,"");
}

#if defined(_WINDOWS)
// Make sure that the registry can be shared if the library is
// included as part of a DLL.
#pragma data_seg( ".GLOBALS" )
#endif
MleDwpStrKeyDict MleScene::g_instanceRegistry;
#if defined(_WINDOWS)
#pragma data_seg()
#pragma comment(linker, "/section:.GLOBALS,rws")
#endif

// Registering the instance with rehearsal player.
void
MleScene::registerInstance(const char* name)
{
#ifdef _WINDOWS
   this->m_name = _strdup(name);
#else
   this->m_name = strdup(name);
#endif
   g_instanceRegistry.set(this->m_name, this);
}

// Unregister ourselves from registry.
void
MleScene::unregisterInstance()
{
    // Eliminate from instance registry.
    // XXX - must iterate through dict.  We could have registered this twice,
    // or we could put a removeValue in the dict class

    for (MleDwpDictIter iter(g_instanceRegistry); iter.getValue(); iter.next())
    {
        if ((MleScene*) iter.getValue() == this)
        {
            g_instanceRegistry.remove(iter.getKey());
            break;
        }
    }
}

// This function returns the scene class of an scene.
// The scene class object is defined in mle/MleSceneClass.h.  It contains
// information about the class, like a creation function.
const MleSceneClass *
MleScene::getClass(void)
{
    // If there is a cached value, return it.
    if ( m_sceneClass )
        return m_sceneClass;
    
    // Look it up in the registry.
    m_sceneClass = MleSceneClass::find(getTypeName());

    return m_sceneClass;
}

#endif /* MLE_DIGITAL_WORKPRINT */

#ifdef MLE_DIGITAL_WORKPRINT

// Load the scene from the relevant item in the DWP.
// Use different binding types depending on whether the DWP is 
// a playprint or workprint.  In the workprint, we use the WP struct.
// Note that this static function calls the mlLoadScene() function
// in load.cxx, which is able to load scenes of various types, eg it 
// is a scene "class factory."

extern MleScene* mlLoadScene(MleDwpScene* wpScene);
extern MleScene* mlLoadScene(const char* id);

MleScene *
MleScene::load(MleDwpScene* wpScene)
{
    if ( wpScene == NULL ) {
        printf("mlLoadScene: Cant find scene in workprint.\n");
        return NULL;
    }

    return mlLoadScene( wpScene );
}

MleScene *
MleScene::load(const char* id)
{
    return mlLoadScene( id );
}

#endif /* MLE_DIGITAL_WORKPRINT */
#ifdef MLE_DIGITAL_PLAYPRINT

// In the playprint, we use the compiled table of scene identifiers.

extern MleScene* mlLoadScene(const int indexToPpTOC);


MleScene *
MleScene::load(const int indexToPpTOC)
{
    return mlLoadScene( indexToPpTOC );
}

#endif /* MLE_DIGITAL_PLAYPRINT */


// Delete the global scene.
void
MleScene::deleteGlobalScene( void )
{
    printf ( "MleScene::g_deleteGlobalScene not implemented!?!?!\n" );
}


// Delete the current scene.
void
MleScene::deleteCurrentScene( void )
{
    // Swap old for new
    MleScene *old = getCurrentScene();
    clearCurrentScene();

    // Get rid of old scene and all its contents by deletion
    if (NULL != old) {
        delete old;
    }
}


// Changes the current scene to that passed in.  In this default
// virtual implementation, simply deletes the old currently active scene,
// replacing it with the new one.
MleScene*
MleScene::changeCurrentScene( MleScene *newScene )
{
    // Swap old for new.
    //
    // Do it this way so that we\'ve set the new before deleting
    // the old, in case we\'re the old one.
    MleScene *old = getCurrentScene();
    newScene->setCurrentScene();

    // Get rid of old scene and all its contents by deletion
    if (NULL != old) {
        delete old;
    }
    return newScene;
}

#ifdef MLE_DIGITAL_WORKPRINT
// Changes the current scene to the one loaded from the identifier that
// is passed in.  
MleScene *
MleScene::changeCurrentScene( MleDwpScene *wp )
{
    // Build a new scene from the workprint scene.
    // We count on the load() function to make it current
    // if necessary. 
    // load() calls changeCurrentScene(MleScene*), which is 
    // where you would override the scene changing behavior.
    return MleScene::load(wp);
}

// Changes the current scene to the one loaded from the identifier that
// is passed in.  
MleScene *
MleScene::changeCurrentScene( const char *id )
{
    // Build a new scene from the workprint scene.
    // We count on the load() function to make it current
    // if necessary. 
    // load() calls changeCurrentScene(MleScene*), which is 
    // where you would override the scene changing behavior
    return MleScene::load(id);
}

#endif /* MLE_DIGITAL_WORKPRINT */
#ifdef MLE_DIGITAL_PLAYPRINT

MleScene *
MleScene::changeCurrentScene( const int indexToPpTOC )
{
    // Build a new scene from the workprint scene
    // We count on the load() function to make it current
    // if necessary. 
    //
    // load() calls changeCurrentScene(MleScene*), which is 
    // where you would override the scene changing behavior
    return MleScene::load(indexToPpTOC);
}

#endif /* MLE_DIGITAL_PLAYPRINT */

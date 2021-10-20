/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MleScene.h
 * @ingroup MleFoundation
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015-2021 Wizzer Works
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

#ifndef __MLE_SCENE_H_
#define __MLE_SCENE_H_

// Include Magic Lantern header files.
#include "mle/MleRuntime.h"
#include "mle/MlePtrContainer.h"
#include "mle/MleGroup.h"

#ifdef MLE_DIGITAL_WORKPRINT
#include "mle/MleSceneClass.h"
#include "mle/DwpScene.h"
#endif /* MLE_DIGITAL_WORKPRINT */

#ifdef MLE_DIGITAL_WORKPRINT
// This macro should be placed in the class declaration of any subclass
// of MleScene.  It defines some basic functions needed by every subclass.
// The single argument is the unquoted class name.  Be aware that
// this macro switches subsequent declarations to public.
#define MLE_SCENE_HEADER(C) \
  public: \
    virtual const char *getTypeName(void) const; \
    virtual int isa(const char *type) const; \
    static C *cast(MleScene *scene); \
    static void initClass(void)

// This macro should be placed in the implementation of any subclass of
// MleScene.  It defines some basic functionality needed by every subclass.
// The single argument is the unquoted class name.
#define MLE_SCENE_SOURCE(C,S) \
    const char *C::getTypeName(void) const { return #C; } \
    int C::isa(const char *type) const { \
	if ( !strcmp(type,#C) ) return 1; \
	else return S::isa(type); \
    } \
    C *C::cast(MleScene *scene) { \
	MLE_ASSERT(scene->isa(#C)); \
	return (C *)scene; \
    } \
    MleScene *_mlCreate##C(void); MleScene *_mlCreate##C(void) { return new C; }

// Every subclass of MleScene must implement a static initClass() member
// function which registers itself with the system.  This registration
// is required for rehearsal runtime and the mastering process.  The
// initClass function should be implemented inside a #ifdef MLE_REHEARSAL
// block so it is not compiled for the runtime.
//
// Inside the function, the macro mlRegisterSceneClass should be invoked.
// This registers the scene class with the system so it can be created.

// This macro takes the unquoted scene class and superclass names.
#define mlRegisterSceneClass(SCENE,SUPERCLASS) \
    static int _mlSceneClassInited = 0; \
    if ( _mlSceneClassInited ) return; \
    _mlSceneClassInited = 1; \
    SUPERCLASS::initClass(); \
    new MleSceneClass(#SCENE,_mlCreate##SCENE,#SUPERCLASS)

// This macro takes the unquoted scene class and superclass names and the
// unquoted name of the editor
#define mlRegisterSceneClassWithEditors(SCENE,SUPERCLASS,EDITOR,CONTENT_EDITOR) \
    static int _mlSceneClassInited = 0; \
    if ( _mlSceneClassInited ) return; \
    _mlSceneClassInited = 1; \
    SUPERCLASS::initClass(); \
    new MleSceneClass(#SCENE,_mlCreate##SCENE,#SUPERCLASS, #EDITOR, #CONTENT_EDITOR)

#endif /* MLE_DIGITAL_WORKPRINT */
#ifdef MLE_DIGITAL_PLAYPRINT

// Null macros for non-rehearsal - see above for description.

// Need a statement so trailing semicolon will work.
#define MLE_SCENE_HEADER(C) \
	friend class _mleDummy
#define MLE_SCENE_SOURCE(C,S) \
    MleScene *_mlCreate##C(void) { return new C; }

#endif /* MLE_DIGITAL_PLAYPRINT */

/**
 * MleScene is the base class for runtime scenes.  
 *
 * Base class for runtime scenes.  The scene is the maximal
 * unit of actor loading at run time.  A scene may contain one or
 * more groups or group references.  A scene may
 * be loaded directly at runtime by calling the mleLoadScene() function.
 *
 * Two distinguished scenes may be active at once in a title.  The 
 * <b>current</b> scene is set by scene loading, unloading, or 
 * changing methods.  The <b>global</b> scene is under user control
 * and is intended to hold a scene that will govern transitions between
 * two current scenes, for instance during a level change in a title.
 *
 * At run time the MleScene object holds an array of pointers to 
 * all the groups it has loaded.  When the scene is deleted, it
 * deletes all groups it refers to.  
 *
 * REHEARSAL PLAYER
 *
 * The MleScene class may be subclassed by the title author provided that
 * the workprint names the subclassed type and includes a MleWpHeaderFile
 * object that refers to the subclass header.  At runtime the subclassed
 * object's init() method will be called to invoke scene-dependent 
 * operations.   The subclassed scene must use the MLE_SCENE_HEADER and
 * MLE_SCENE_SOURCE macros and must invoke the mleRegisterSceneClass()
 * macro in the initClass() method.
 *
 * @see MleGroup
 */
class MLE_RUNTIME_API MleScene : public MlePtrContainer<MleGroup*> 
{
  // Declare member variables.

  protected:

    /**
     * The scene that is currently running in the title.
     */
    static MleScene* g_currentScene;

    /**
     * The global scene that exists under user control in the title.
     */
    static MleScene* g_globalScene;
    
  // Declare member functions.

  public:

    /**
     * @brief Construct a group with a default sized array of actor pointers.
     */
    MleScene( void );

    /**
     * @brief The destructor.
     */
    virtual ~MleScene( void );
  
    /**
     * @brief Initialize the scene.
     *
     * The class-specific initialization to be called after the scene is
     * loaded and its groups' init() methods are called.
     * The loaded scene is stored as the g_currentScene member before the
     * init() function is called.
     */
    virtual void init(void);

	/**
	 * Override operator new.
	 *
	 * @param tSize The size, in bytes, to allocate.
	 */
	void* operator new(size_t tSize);

	/**
	 * Override operator delete.
	 *
	 * @param p A pointer to the memory to delete.
	 */
    void  operator delete(void *p);

#ifdef MLE_DIGITAL_WORKPRINT

    // Get the type of the scene -- rehearsal time only.
    virtual const char *getTypeName(void) const;

    // Predicate returning true if the type of the scene is the same as type -- rehearsal time only.
    virtual int isa(const char *type) const;

    // One time class initialization that sets up the constructor for this class.
    static void initClass(void);
  
    // This function returns the (cached) MleSceneClass object for
    // this scene.  It is not const because of the caching.
    const MleSceneClass *getClass(void);
  
    // Registry functions.  Registers a newly created instance.
    // This will give the tools a way to search for particular instances
    static MleDwpStrKeyDict* getInstanceRegistry()  { return &g_instanceRegistry; }
    virtual void registerInstance(const char* name);
    virtual void unregisterInstance();
  
    // Load the scene from the relevant item in the DWP.
    // Use different binding types depending on whether the DWP is 
    // a playprint or workprint.  In the workprint, we use the WP struct.
    // Load by scene pointer.
    MleScene* load( MleDwpScene* wpScene );	

    // Load by name.
    MleScene* load(const char* id);

    // Change to a scene described in the workprint.  Deletes the
    // previously current scene.  Intended to be overridden by a 
    // programmer-defined subclass of MleScene for specialized scene 
    // changes.
    virtual MleScene* changeCurrentScene( MleDwpScene *wpScene );

    // Change to a scene described in the workprint.  Deletes the
    // previously current scene.  Intended to be overridden by a 
    // programmer-defined subclass of MleScene for specialized scene 
    // changes.
    virtual MleScene* changeCurrentScene( const char *id );

#endif /* MLE_DIGITAL_WORKPRINT */
#ifdef MLE_DIGITAL_PLAYPRINT

    /**
     * @brief Load the specified scene.
     *
     * In the playprint, we use the compiled table of scene identifiers.
     * Load by playprint identifier.
     *
     * @param indexToPpTOC An index into the Digital Playprint's
     * table-of-contents.
     *
     * @return A pointer to the loaded scene is returned.
     */
    MleScene* load( const int indexToPpTOC );

    /**
     * @brief Change to a scene described by a playprint index.
     *
     * @param indexToPpTOC An index into the Digital Playprint's
     * table-of-contents.
     *
     * @return A pointer to the loaded scene is returned.
     */
    virtual MleScene* changeCurrentScene( const int indexToPpTOC );

#endif /* MLE_DIGITAL_PLAYPRINT */

    /**
     * @brief Set the caller as the current scene.
     */
    void setCurrentScene( void )
    { g_currentScene = this; };

    /**
     * @brief Clear the current scene.
     */
    static void clearCurrentScene( void )
    { g_currentScene = NULL; };

    /**
     * @brief Return a pointer to the current scene.
     *
     * @return A pointer to the current scene is returned.
     */
    static MleScene* getCurrentScene( void )
    { return g_currentScene; };

    /**
     * @brief Delete the current scene and all of its children groups.
     */
    static void deleteCurrentScene( void );

    // GROUP: Managers for the globally active scene.

    /**
     * @brief Set the caller as the global scene.
     */
    void  setGlobalScene( void )
    { g_globalScene = this; };

    /**
     * @brief Clear the global scene.
     */
    static void clearGlobalScene( void )
    { g_globalScene = NULL; };

    /**
     * @brief Return a pointer to the global scene.
     *
     * @return A pointer to the global scene is returned.
     */
    static MleScene* getGlobalScene( void )
    { return g_globalScene; };

    /**
     * @brief Delete the global scene and all of its children groups.
     */
    static void deleteGlobalScene( void );

    /**
     * @brief Switch to the specified scene.
     *
     * This scene changing method is intended to be called by the tools to 
     * invoke the scene change.  If the title also calls it, then tools can 
     * display greater fidelity to the title.
     *
     * @param newScene The new scene to switch to.
     *
     * @return A pointer to the current scene is returned.
     */
    virtual MleScene* changeCurrentScene( MleScene *newScene );

#ifdef MLE_DIGITAL_WORKPRINT
  protected:

    // The scene instance registry.
    static MleDwpStrKeyDict g_instanceRegistry; 
    
    // Scene class.
    const MleSceneClass *m_sceneClass;

  private:

    // Instance name of a scene.
    char *m_name;

#endif /* MLE_DIGITAL_WORKPRINT */
};

#endif /* __MLE_SCENE_H_ */

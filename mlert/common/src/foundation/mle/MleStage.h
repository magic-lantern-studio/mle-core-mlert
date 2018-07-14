/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MleStage.h
 * @ingroup MleFoundation
 *
 * @author Mark S. Millard
 * @created May 1, 2003
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015-2018 Wizzer Works
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


#ifndef __MLE_STAGE_H_
#define __MLE_STAGE_H_

// Include system header files.
#if defined(WIN32)
#include <winsock2.h>
#ifdef Q_OS_WIN
// Qt platform on Windows.
#endif /* Q_OS_WIN */
#endif /* WIN32 */
#if defined(__linux__)
#ifdef Q_OS_UNIX
// Qt platform on Unix
#else
// X11/Xt platform on Unix.
#include <X11/Intrinsic.h>
#endif
#endif /* __linux__ */

#ifdef MLE_DIGITAL_WORKPRINT
// Include Magic Lantern Runtime Engine header files.
#include "mle/MleStageClass.h"

class MleSet;
class MleDwpDataUnion;
#endif /* MLE_DIGITAL_WORKPRINT */


// Include Magic Lantern header files.
#include "mle/mlTypes.h"

// Include Magic Lantern Runtime Engine header files.
#include "mle/MleRuntime.h"
#include "mle/MleProperty.h"
#include "mle/MleObject.h"

// Forward declarations.
class MleActor;
class MleRole;


/**
 * @brief The base class for all stages.
 */
class MLE_RUNTIME_API MleStage : public MleObject
{
  // Declare member variables.

  public:
    
    /**
     * This static member variable holds the stage pointer.  This
     * currently restricts each process to a single stage.
     */
    static MleStage *g_theStage;

  // Declare member functions.

  public:

    /**
     * @brief Base class destructor does nothing; here for subclassing.
	 */
    MleStage();

    /**
     * Destructor.
     */
    virtual ~MleStage();

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

    /**
     * @brief Get the size of the stage.
     *
     * @param width A pointer to the width of the stage.
     * @param height A pointer to the height of the stage.
     *
     * @return The size of stage's window is returned. Magic Lantern 1.0
     * supports one window per stage: this is the
     * default window.
     */
    virtual void getSize(int *width,int *height);

#ifdef MLE_DIGITAL_WORKPRINT
    // This section defines rehearsal interfaces.
    // It typically includes entry points for tools.

    // Initialization.
    virtual void init(void);
    
    // Identity.
    virtual const char *getTypeName(void) const;

    virtual int isa(const char *type) const;

    // Class initialization for base class.
    static void initClass(void);

    // Get the stage class.
    const MleStageClass *getClass(void);

    // Set a property value.  This function puts the value of a property
    // directly into stage memory.  The stage is used to determine an offset
    // for the data that comes from a data union object.
    int poke(const char *property, MleDwpDataUnion *value);

    // Update the stage after editing.  This function synchronizes the stage
    // with any property changes that might be made as a result of editing.
    // The argument is the name of the changed property - if NULL, then the
    // stage should assume that all properties may have changed.  This does
    // nothing in the base class.
    virtual void resolveEdit(const char *property = NULL);

    // Getting the stage name.
    char* getName() const { return m_name; }

    // Change the stage name, which also causes re-registration under
    // new name.
    virtual void setName(char *newName);

#if defined(__linux__)
    // Event handling: each stage does its own event handling; this
    // is so that particular stages, such as ones that use inventor,
    // can control the select blocking in the main loop.
    virtual int doSelect(int nfds, 
			 fd_set *readfds,
			 fd_set *writefds,
			 fd_set *exceptfds,
			 struct timeval *userTimeOut);
#endif /* __linux__ */
    
    // Editing mode.

    // setEditing() enables or disables editing mode.  Editing
    // mode can provide a different interpretation of user gestures
    // (such as picking) than occurs in title play.
    virtual void setEditing(int mode);

    // getEditing() returns current editing state.
    virtual int getEditing(void) const;

    // edit() does any updates that are required during editing,
    // for example, scanning for editing events, screen refresh, etc.
    virtual void edit(void);
    
    // Configuration.

    // setSize() requests a width and height for the stage display.
    // A nonzero value is returned if the request could not be honored
    // (for example, if the stage does not support changing its size).
    virtual int setSize(int width,int height);

#if defined(__linux__)
#ifdef Q_OS_UNIX
    // getWindow() returns a Qt window for the stage, if possible.
    // This window may be reparented for tools.
    //virtual QtStageWindow* getWindow(void);
#else
    // getWindow() returns an X window for the stage, if possible.
    // This window may be reparented for tools.
    virtual Window getWindow(void);

    virtual Display* getDisplay(void);
#endif
#endif /* __linux__ */
#if defined(WIN32)
#ifdef Q_OS_WIN
#else
    virtual HWND getWindow(void);
#endif
#endif /* WIN32 */

    virtual void setOffscreen(int flag);
    
    // Rendering.
    // render() forces a redraw
    virtual void render(void);
    
    // Manipulation.
    virtual void activateManipulator(MleActor *actor,int invokeCallback=0);
    virtual void deactivateManipulator(MleActor *actor,int invokeCallback=0);
    
    // Snapping.
    virtual void moveToTarget();

    // Nudging selected obj - dir is 0=x,1=y,2=z
    virtual void nudge(int dir, int numPixels);

    // Callbacks.

    // set*Callback() allows an application's function to be called
    // when certain events occur.  set*Callback() replaces any function
    // that was previously installed.  Passing NULL removes any existing
    // callback.

    // setPickCallback() installs a function that is called when an
    // actor/role is selected.
    void setPickCallback(void (*pickCB)(MleActor *actor,
    void *clientData), void *clientData = NULL);

    // setPickCallback() installs a function that is called when an
    // actor/role is unselected.
    void setUnpickCallback(void (*unpickCB)(MleActor *actor,
    void *clientData), void *clientData = NULL);

    // setOpenCallback() installs a function that is called when an
    // actor/role is opened, e.g. with a double-click.
    void setOpenCallback(void (*openCB)(MleActor *actor,
    void *clientData), void *clientData = NULL);

    // setLaunchCallback() installs a function that is called when
    // an actor/role is launched, e.g. with an alt + double-click.
    void setLaunchCallback(void (*launchCB)(MleActor *actor,
    void *clientData), void *clientData = NULL);

    void setStartManipCallback(void (*manipCB)(MleActor *actor,
    void *clientData), void *clientData = NULL);

    // setManipCallback() installs a function that is called when an
    // actor/role is manipulated.
    void setManipCallback(void (*manipCB)(MleActor *actor,
    void *clientData), void *clientData = NULL);

    void setFinishManipCallback(void (*manipCB)(MleActor *actor,
    void *clientData), void *clientData = NULL);

#if defined(__linux__)
#ifdef Q_OS_UNIX
    //void setRightMouseCallback(void (*rightMouseCB)(QEvent* e,
    //                           void* clientData), void* clientData = NULL);
#else
    void setRightMouseCallback(void (*rightMouseCB)(XEvent* e,
                               void* clientData), void* clientData = NULL);
#endif
#endif /* __linux__ */
#if defined(WIN32)
    void setRightMouseCallback(void (*rightMouseCB)(MSG* e,
                               void* clientData), void* clientData = NULL);
#endif /* WIN32 */

    // Getting the file descriptor
    // XXX - this should be an abstract method, but
    // this class doesn't like being abstract.
    virtual int getFD();

    // Getting attributes of the stage.  These methods should also be abstract.
    virtual const char** getFunctions();

    virtual const char** getFunctionAttributes(char* functionName);

    // More methods for tool APIs - all should be abstract
    // Control over IV viewers
    virtual int setViewer(char* viewerName);

    virtual char* getViewer();

    // Control over edit modes.
    virtual int setEditMode(char* editMode);

    virtual char* getEditMode();
    
    // Sets rendering mode for entire stage.
    virtual void setRenderMode(char *renderMode);

    virtual const char *getRenderMode() const;

    // Turns on/off snapping.
    virtual int hasSnappingTarget();

    virtual void setSnapping(int value);

    virtual int getSnapping();

    // Set layering
    virtual int pushSet(MleSet* set);

    virtual int pushSetToBottom(MleSet* set);

    virtual int popSet(MleSet* set);

    virtual int popSetToTop(MleSet* set);

    // Background colors.
    virtual void setBgndColor(float* color);

    virtual void getBgndColor(float* color); 

    // Horizon grid.
    virtual void setHorizonGrid(int onOff);

    virtual int getHorizonGrid(); 

    virtual void openPrefsDialog();

    virtual void viewAll();

    virtual void showDecoration(int onOff);

#if defined(__linux__)
#ifdef Q_OS_UNIX
    // Reparent window - tools call this to reparent the player window
    // to a window passed in from the tools
    //virtual void reparentWindow(QtStageWindow* parentWindow);
#else
    // Reparent window - tools call this to reparent the player window
    // to a window passed in from the tools
    virtual void reparentWindow(Window parentWindow);
#endif
#endif /* __linux__ */
#if defined(WIN32)
#ifdef Q_OS_WIN
#else
    virtual void reparentWindow(HWND parentWindow);
#endif
#endif /* WIN32 */
    
    // Recalculates the clipping planes if auto clipping planes enabled.
    // Should be called anytime an actor property changes that could
    // effect actor transform.
    virtual void recalcAutoClipPlanes();

#endif /* MLE_DIGITAL_WORKPRINT */

#ifdef MLE_REHEARSAL
    // Define state for rehearsal interfaces.

  protected:

    // Callbacks.
    void (*pickCB)(MleActor *actor,void *clientData);
    void (*unpickCB)(MleActor *actor,void *clientData);
    void (*openCB)(MleActor *actor,void *clientData);
    void (*launchCB)(MleActor *actor,void *clientData);
    void (*startManipCB)(MleActor *actor,void *clientData);
    void (*manipCB)(MleActor *actor,void *clientData);
    void (*finishManipCB)(MleActor *actor,void *clientData);
#if defined(__linux__)
#ifdef Q_OS_UNIX
    void (*rightMouseCB)(QEvent* e,void *clientData);
#else
    void (*rightMouseCB)(XEvent* e,void *clientData);
#endif
#endif /* __linux__ */
#if defined(WIN32)
#ifdef Q_OS_WIN
#else
	void (*rightMouseCB)(MSG* e,void *clientData);
#endif
#endif /* WIN32 */
    void *m_pickClientData;
    void *m_unpickClientData;
    void *m_openClientData;
    void *m_launchClientData;
    void *m_startManipClientData;
    void *m_manipClientData;
    void *m_finishManipClientData;
    void *m_cameraClientData;
    void *m_rightMouseClientData;

    // Editing mode.
    int m_editMode;

#if defined(__linux__)
#ifdef Q_OS_UNIX
    //static int checkForDoubleClick(QButtonEvent* event);
#else
    static int checkForDoubleClick(XButtonEvent* event);
#endif
#endif /* _linux__ */
#if defined(WIN32)
#ifdef Q_OS_WIN
#else
    static int checkForDoubleClick(MSG* event);
#endif
#endif /* WIN32 */

#endif /* MLE_REHEARSAL */

#ifdef MLE_DIGITAL_WORKPRINT
    // Stage class.
    const MleStageClass *m_stageClass;

  private:

    // Instance name of a stage.
    char *m_name;

#endif /* MLE_DIGITAL_WORKPRINT */
};


// New Stage Property definitions (to remove legacy offsetof implementation).
#define MLE_STAGE_PROPERTY(TYPE,NAME,GETTER,SETTER)         \
  public:                                                   \
    TYPE NAME;                                              \
    virtual TYPE GETTER() {return NAME;}                    \
    virtual void SETTER(TYPE value) {this->NAME = value;}   \
    int NAME##IsOfType(const char *type) {                  \
        if (strcmp(#TYPE, type) == 0) return 1;             \
        else return 0; }

#define MLE_STAGE_DYNAMIC_PROPERTY(TYPE,NAME,GETTER,SETTER) \
  public:                                                   \
    TYPE NAME;                                              \
    virtual const TYPE GETTER() {return NAME;}              \
    virtual void SETTER(const TYPE value, size_t size) {    \
        if (this->NAME != NULL) mlFree(this->NAME);         \
        this->NAME = (TYPE) mlMalloc(size);                 \
        memcpy(this->NAME, value, size); }                  \
    int NAME##IsOfType(const char *type) {                  \
        if (strcmp(#TYPE, type) == 0) return 1;             \
        else return 0; }

#define MLE_STAGE_ARRAY_PROPERTY(TYPE,NAME,GETTER,SETTER)   \
  public:                                                   \
    MleArray<TYPE> NAME;                                    \
    virtual const MleArray<TYPE> *GETTER() {return &NAME;}  \
    virtual void SETTER(MleArray<TYPE> &value) {            \
        this->NAME.resize(value.size());                    \
        for (int i = 0; i < value.size(); i++)              \
            this->NAME[i] = value[i]; }                     \
    int NAME##IsOfArrayType(const char *type) {             \
        if (strcmp(#TYPE, type) == 0) return 1;             \
        else return 0; }

#define MLE_STAGE_PROPERTY_SOURCE()                                                      \
  public:                                                                                \
    static void getProperty(MleObject *object, const char *name, unsigned char **value); \
    static void setProperty(MleObject *object, const char *name, unsigned char *value);


#ifdef MLE_DIGITAL_WORKPRINT

#include <string.h>

#include "mle/mlAssert.h"
#include "mle/DwpStrKeyDict.h"

// This macro should be placed in the class declaration of any subclass
// of MleStage.  It defines some basic functions needed by every subclass.
// The single argument is the unquoted class name.  Be aware that
// this macro switches subsequent declarations to public.
#define MLE_STAGE_HEADER(C) \
  public: \
    virtual const char *getTypeName(void) const; \
    virtual int isa(const char *type) const; \
    static C *cast(MleStage *stage); \
    static void initClass(void)

// This macro should be placed in the implementation of any subclass of
// MleStage.  It defines some basic functionality needed by every subclass.
// The single argument is the unquoted class name.
#define MLE_STAGE_SOURCE(C,S) \
    const char *C::getTypeName(void) const { return #C; } \
    int C::isa(const char *type) const { \
	if ( !strcmp(type,#C) ) return 1; \
	else return S::isa(type); \
    } \
    C *C::cast(MleStage *stage) { \
        MLE_ASSERT(stage->isa(#C)); \
        return (C *)stage; \
    } \
    MleStage *_mlCreate##C(void) { return new C; }

// This macro should be used instead of MLE_STAGE_HEADER for abstract classes.
#define MLE_STAGE_ABSTRACT_HEADER(C) \
  public: \
    virtual const char *getTypeName(void) const = 0; \
    virtual int isa(const char *type) const; \
    static C *cast(MleStage *stage); \
    static void initClass(void)

// This macro should be used instead of MLE_STAGE_SOURCE for abstract classes.
#define MLE_STAGE_ABSTRACT_SOURCE(C,S) \
    int C::isa(const char *type) const { \
        if ( !strcmp(type,#C) ) return 1; \
        else return S::isa(type); \
    } \
    C *C::cast(MleStage *stage) { \
        MLE_ASSERT(stage->isa(#C)); \
        return (C *)stage; \
    } \
    MleStage *_mlCreate##C(void) { return NULL; }

// Every subclass of MleStage must implement a static initClass() member
// function which registers itself with the system.  This registration
// is required for rehearsal runtime and the mastering process.  The
// initClass function should be implemented inside a #ifdef MLE_REHEARSAL
// block so it is not compiled for the runtime.
//
// Inside the function, the macro mlRegisterStageClass should be invoked.
// This registers the stage class with the system so it can be created.
// Then the macro mlRegisterStageMember should be called for each member
// variable that is potentially to have instance values kept in the
// workprint.

// This macro takes the unquoted stage class and superclass names.
#define mleRegisterStageClass(STAGE,SUPERCLASS) \
    static int _mlStageClassInited = 0; \
    if ( _mlStageClassInited ) return ; \
    _mlStageClassInited = 1; \
    SUPERCLASS::initClass(); \
    new MleStageClass(#STAGE,_mlCreate##STAGE,#SUPERCLASS)

// This macro takes the unquoted stage class name, the unquoted member
// variable name, and the unquoted member variable type.
#if 0
#define mlRegisterStageMember(STAGE,MEMBER,TYPE) \
    MleStageClass::find(#STAGE)->addMember( \
		#MEMBER,#TYPE,((char *)&((STAGE *)0)->MEMBER) - (char *)0)
#else
#define mleRegisterStageMember(STAGE,MEMBER,TYPE) \
    MlePropertyEntry *entryFor##MEMBER = new MlePropertyEntry(); \
    entryFor##MEMBER->name = #MEMBER; \
    entryFor##MEMBER->getProperty = STAGE::getProperty; \
    entryFor##MEMBER->setProperty = STAGE::setProperty; \
    MleStageClass::find(#STAGE)->addMember(#MEMBER,#TYPE,entryFor##MEMBER)
#endif

#endif /* MLE_DIGITAL_WORKPRINT */
#ifdef MLE_DIGITAL_PLAYPRINT

// Null macros for non-rehearsal - see above for description.

// Need a statement so trailing semicolon will work.
#define MLE_STAGE_HEADER(C) \
  public: \
    static C *cast(MleStage *stage) { return (C *) stage; } \
    friend class _mleDummy
#define MLE_STAGE_ABSTRACT_HEADER(C) \
    friend class _mleDummy
#define MLE_STAGE_SOURCE(C,S) \
    MleStage *_mlCreate##C(void) { return new C; }
#define MLE_STAGE_ABSTRACT_SOURCE(C,S)

#endif /* MLE_DIGITAL_PLAYPRINT */
	
#endif /* __MLE_STAGE_H_ */

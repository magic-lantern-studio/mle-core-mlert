/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MleStage.cxx
 * @ingroup MleFoundation
 *
 * @author Mark S. Millard
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015-2020 Wizzer Works
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
//  For information concerning this source code, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

// Include standard header files.
#include <stdio.h>
//#include <sys/time.h>
#ifndef WIN32
#include <unistd.h>
#endif /* WIN32 */

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"
#include "mle/mlAssert.h"

// Include Runtime Engine header files.
#include "mle/MleStage.h"

#ifdef MLE_DIGITAL_WORKPRINT
#include "mle/DwpDataUnion.h"
#include "mle/DwpDatatype.h"
#endif /* MLE_DIGITAL_WORKPRINT */

// Implement the static stage pointer.
MleStage *MleStage::g_theStage = NULL;


MleStage::MleStage(void)
{
    // Set the current stage.
    MLE_ASSERT(g_theStage == NULL);  // Only one stage allowed.
    g_theStage = this;

#if defined(MLE_REHEARSAL)
	// Initialize variables.
    pickCB = NULL;
    openCB = NULL;
    launchCB = NULL;
    manipCB = NULL;
#if !defined(__linux__)
    rightMouseCB = NULL;
#endif /* Add this back in once we define it for Linux. */

    m_pickClientData = NULL;
    m_openClientData = NULL;
    m_manipClientData = NULL;
    m_rightMouseClientData = NULL;

    m_editMode = 0; // Initialize edit mode to OFF.
#endif /* MLE_REHEARSAL */
}


MleStage::~MleStage()
{
}


void *
MleStage::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}


void
MleStage::operator delete(void *p)
{
	mlFree(p);
}


void 
MleStage::getSize(int * /* width */,int * /* height */)
{
}

// Define null init()
//   This is conditionally compiled under rehearsal
#ifdef MLE_DIGITAL_WORKPRINT
// This function is implemented by subclasses to build the stage.  This
// is not done in the constructor so as to keep the constructor interface
// pure.  There is optional configuration that may be done on a stage -
// offscreen, size, etc. - that is possible only during initial construction.
// This allows a rehearsal stage to be allocated, then configured, then
// actually built.  The base class implementation does nothing.
void
MleStage::init(void)
{
}
#endif /* MLE_DIGITAL_WORKPRINT */

MleStage *
_mlCreateMleStage(void)
{
    return new MleStage;
}

#ifdef MLE_DIGITAL_WORKPRINT
// This virtual function returns the type name of the stage.
const char *
MleStage::getTypeName(void) const
{
    return "MleStage";
}

// This function is the terminal case for determining if an object
// instance is an instance of a specified type.
int
MleStage::isa(const char *type) const
{
    return !strcmp(type,"MleStage");
}

void
MleStage::initClass(void)
{
    new MleStageClass("MleStage", _mlCreateMleStage, "");
}

// This function returns the stage class of a stage.
// The stage class object is defined in mle/MleStageClass.h.  It contains
// information about the class, like a creation function, and it is
// a dictionary of the stage properties registered with
// mlRegisterStageMember.
const MleStageClass *
MleStage::getClass(void)
{
    // If there is a cached value, return it.
    if ( m_stageClass )
		return m_stageClass;

    // Look it up in the registry.
    m_stageClass = MleStageClass::find(getTypeName());

    return m_stageClass;
}

int
MleStage::poke(const char *property,MleDwpDataUnion *value)
{
    MLE_ASSERT(property);
    MLE_ASSERT(value);

    // Make sure the stage class is present.
    if ( getClass() == NULL )
	    return 0;

    // Get the member corresponding to the property name.
    const MleStageMember *member = m_stageClass->findMember(property);
    if ( member == NULL )
    {
	    printf("property %s not present on %s.\n",
		    property,getTypeName());
	    return 1;
    }

    // Transfer the value.
    MLE_ASSERT(value->m_datatype);

    if ( value->m_datatype != member->getType() )
    {
		printf("type mismatch in setting property %s on %s (%s and %s).\n",
			property,getTypeName(),
			value->m_datatype->getName(),
			member->getType()->getName());
		return 1;
    }

    //value->m_datatype->get(value,(char *)this + member->getOffset());
    MlePropertyEntry *entry = member->getEntry();
    MlePropertySetter setter = entry->setProperty;
    setter(this, entry->name, (unsigned char *)value);

    return 0;
}

void
MleStage::resolveEdit(const char *)
{
}

// Set a stage's name.
void
MleStage::setName(char *newName)
{
    if (m_name)
		mlFree(m_name);

    m_name = strdup(newName);
}

#if defined(__linux__)
// X event handling: each stage does its own event handling; this
// is so that particular stages, such as ones that use inventor,
// can control the select blocking in the main loop.
int MleStage::doSelect(int nfds, fd_set *readfds, fd_set *writefds,
	fd_set *exceptfds, struct timeval *userTimeOut)
{
    // Default is to just call select.
    return select(nfds, readfds, writefds, exceptfds, userTimeOut);
}
#endif /* __linux__ */

#endif /* MLE_DIGITAL_WORKPRINT*/

// Define editing functions
// This is all conditionally compiled under rehearsal.
#if defined(MLE_REHEARSAL)
// This function sets the editing mode.  Nonzero turns editing on,
// zero turns it off.  Editing mode can provide a different interpretation
// user gestures (such as picking) than occurs in title play.
void
MleStage::setEditing(int mode)
{
    m_editMode = mode;
}

// This function returns the current editing mode.
int
MleStage::getEditing(void) const
{
    return m_editMode;
}

// This function should be called regularly when the stage is in edit mode.
// It is implemented by subclasses, and gives the stage an opportunity
// to do edit event handling, screen refresh, etc.
void
MleStage::edit(void)
{
    // Should be implemented by subclasses.
}

#endif /* MLE_REHEARSAL editing functions */

// Define configuration functions
#if defined(MLE_REHEARSAL)
int MleStage::setSize(int,int)
{
    return 1;
}

#if defined(__linux__)
#ifdef Q_OS_UNIX
// Qt on Linux platfrom.
QWidget *
MleStage::getWindow(void)
{
    return NULL;
}
#else
// X11/Xt on Linux platrform.
Window
MleStage::getWindow(void)
{
    return 0;
}

Display*
MleStage::getDisplay(void)
{
    return NULL;
}
#endif /* ! Q_OS_UNIX */
#endif /* __linux__ */
#if defined(WIN32)
HWND
MleStage::getWindow(void)
{
    return NULL;
}
#endif /* WIN32 */

void
MleStage::setOffscreen(int)
{
}

// Getting the file descriptor.
// Todo: this should be an abstract method, but
// this class doesn't like being abstract (BrStage balks).
int
MleStage::getFD()
{
    // Subclasses must override this.
    MLE_ASSERT(FALSE);
    return(-1);
}

// Getting a list of functions that this stage will support - overridden
// in subclasses.
const char**
MleStage::getFunctions()
{
    return(NULL);
}

const char**
MleStage::getFunctionAttributes(char* /*functionName*/)
{
    return(NULL);
}

// Control over IV viewers.
int 
MleStage::setViewer(const char* )
{
    return(-1);
}

const char*
MleStage::getViewer()
{
    return(NULL);
}

// Control over edit modes.
int 
MleStage::setEditMode(char* )
{
    return(-1);
}

char* 
MleStage::getEditMode()
{
    return(NULL);
}

// Sets rendering mode for entire stage.
void MleStage::setRenderMode(char* /*renderMode*/)
{
}

const char * MleStage::getRenderMode() const
{
    return NULL;
}

// Turns on/off snapping.
int
MleStage::hasSnappingTarget()
{
    return(0);
}

void 
MleStage::setSnapping(int)
{
}

int 
MleStage::getSnapping()
{
    return(-1);
}

// Set layering.
int 
MleStage::pushSet(MleSet*)
{
    return(-1);
}

int 
MleStage::pushSetToBottom(MleSet*)
{
    return(-1);
}

int 
MleStage::popSet(MleSet*)
{
    return(-1);
}

int 
MleStage::popSetToTop(MleSet*)
{
    return(-1);
}


// Background colors.
void 
MleStage::setBgndColor(float* /*color*/)
{
}

void 
MleStage::getBgndColor(float* /*color*/)
{
}

// Horizon grid.
void 
MleStage::setHorizonGrid(int /*onOff*/)
{
}

int 
MleStage::getHorizonGrid()
{
    return 0;
}

void MleStage::openPrefsDialog()
{
}

void MleStage::viewAll()
{
}

void MleStage::showDecoration(int /*onOff*/)
{
}

#if defined(__linux__)
#ifdef Q_OS_UNIX
// Qt on Linux platform.
void
MleStage::reparentWindow(QWindow */*parentWindow*/)
{
    // Todo: Implement.
}
#else
// X11/Xt on Linux platform.
void
MleStage::reparentWindow(Window parentWindow)
{
    Window w = getWindow();
    Display* display = getDisplay();
    MLE_ASSERT(display);
    MLE_ASSERT(w);
    XReparentWindow(display, w, parentWindow, 0, 0);
    XFlush(display);
    //XMapRaised(getDisplay(), getWindow());
    //printf("Reparenting window\n");

    //printf("STAGE: Disp %d  child: %d  parent: %d\n", getDisplay(), getWindow(), parentWindow);

    // Unselect the button press
    /*
    XWindowAttributes wa;
    XGetWindowAttributes(display, w, &wa);
    XSelectInput(display, w, wa.your_event_mask & ~ButtonPress);
    */
}
#endif /* ! Q_OS_UNIX */
#endif  /* __linux__ */
#if defined(WIN32)
void
MleStage::reparentWindow(HWND parentWindow)
{
	MLE_ASSERT(0);
}
#endif /* WIN32 */

#endif /* MLE_REHEARSAL configuration functions */

// rendering function
// This forces a redraw of the screen (after editing operations, for
// example.  This interface is for rehearsal only.
#if defined(MLE_REHEARSAL)
void
MleStage::render(void)
{
}
#endif /* MLE_REHEARSAL rendering */

// manipulation
#if defined(MLE_REHEARSAL)
void
MleStage::activateManipulator(MleActor *,int)
{
}

void
MleStage::deactivateManipulator(MleActor *,int)
{
}

void
MleStage::moveToTarget()
{
}

void
MleStage::nudge(int /*dir*/, int /*numPixels*/)
{
}

#endif /* MLE_REHEARSAL manipuation */

#if defined(MLE_REHEARSAL)
// Recalculates the clipping planes if auto clipping planes enabled.
// Should be called anytime an actor property changes that could
// effect actor transform.
void MleStage::recalcAutoClipPlanes()
{
    MLE_ASSERT(0);
}

#endif /* MLE_REHEARSAL clip planes */

// Define callback registration functions.
// This is all conditionally compiled under rehearsal.
#if defined(MLE_REHEARSAL)
// This functions sets the pick callback.
// The installed callback function will be called when an actor/role
// is selected from the stage.
void
MleStage::setPickCallback(void (*callback)(MleActor *actor,void *clientData),
    void *clientData)
{
    pickCB = callback;
    m_pickClientData = clientData;
}

// This functions sets the unpick callback.
// The installed callback function will be called when an actor/role
// is deselected from the stage.
void
MleStage::setUnpickCallback(void (*callback)(MleActor *actor,void *clientData),
    void *clientData)
{
    unpickCB = callback;
    m_unpickClientData = clientData;
}

// This function sets the open callback.
// The installed callback function will be called when an actor/role
// is opened from the stage, e.g. with a double-click.
void
MleStage::setOpenCallback(void (*callback)(MleActor *actor,void *clientData),
    void *clientData)
{
    openCB = callback;
    m_openClientData = clientData;
}

// This function sets the launch callback.
// The installed callback function will be called when an actor/role
// is launched from the stage, e.g. with an alt double-click.
void
MleStage::setLaunchCallback(void (*callback)(MleActor *actor,void *clientData),
    void *clientData)
{
    launchCB = callback;
    m_launchClientData = clientData;
}

// This function sets the start manip callback.
// The installed callback function will be called when an actor/role
// is modified from the stage, e.g. through a manipulator.
void
MleStage::setStartManipCallback(void (*cb)(MleActor *actor, void *client),void *client)
{
    startManipCB = cb;
    m_startManipClientData = client;
}

// This function sets the manip callback.
// The installed callback function will be called when an actor/role
// is modified from the stage, e.g. through a manipulator.
void
MleStage::setManipCallback(void (*cb)(MleActor *actor, void *client),void *client)
{
    manipCB = cb;
    m_manipClientData = client;
}

// This function sets the finish manip callback.
// The installed callback function will be called when an actor/role
// is modified from the stage, e.g. through a manipulator.
void
MleStage::setFinishManipCallback(void (*cb)(MleActor *actor, void *client),void *client)
{
    finishManipCB = cb;
    m_finishManipClientData = client;
}

#if defined(__linux__)
#ifdef Q_OS_UNIX
// Qt on Linux platform.
void
MleStage::setRightMouseCallback(void (*cb)(QEvent *e, void *client),void *client)
{
    rightMouseCB = cb;
    m_rightMouseClientData = client;
}
#else
// This function sets the right mouse CB
// This is to inform the tools that a right mouse event has happend.
void
MleStage::setRightMouseCallback(void (*cb)(XEvent *e, void *client),void *client)
{
    rightMouseCB = cb;
    m_rightMouseClientData = client;
}
#endif /* ! Q_OS_UNIX */
#endif /* __linux__ */
#if defined(WIN32)
void
MleStage::setRightMouseCallback(void (*cb)(MSG *e, void *client),void *client)
{
    rightMouseCB = cb;
    m_rightMouseClientData = client;
}
#endif /* WIN32 */

#endif /* MLE_REHEARSAL callback management */

// Utility for stage subclasses.
#if defined(MLE_REHEARSAL)

#define MAX_X_DISTANCE 7
#define MAX_Y_DISTANCE 7
#define DOUBLE_CLICK_SECS 0
#define DOUBLE_CLICK_USECS 500000L
#define DOUBLE_CLICK_MSECS 500L

#if defined(__linux__)
#ifdef Q_OS_UNIX
// Qt on Linux platform.
#else
// X11/Xt on Linux platfrom.
// This function checks for a double click
// (static func).
int 
MleStage::checkForDoubleClick(XButtonEvent* event)
{
    static Time lastTime = 0L;
    static int lastX = 0, lastY = 0;

    // XXX - note, we do not care about wrap - this only happens once
    // every 49.7 days and if you get caught in it, double click
    // again and stop complaining!

    // Get differences.
    Time timeDiff = event->time - lastTime;
    int posDiffX = event->x - lastX, posDiffY = event->y - lastY;

    // Make sure time and distance is within the correct amount.
    int ret = 0;
    if ((posDiffX >= -MAX_X_DISTANCE ) && (posDiffX <= MAX_X_DISTANCE))
	{
        if ((posDiffY >= -MAX_Y_DISTANCE) && (posDiffY <= MAX_Y_DISTANCE)) 
		{
			if (timeDiff <= DOUBLE_CLICK_MSECS)
			{
				ret = 1;
			}
        }
    }

    // Reset last pos and time.
    lastTime = event->time;
    lastX = event->x;
    lastY = event->y;

    // Return the value.
    return(ret);
}
#endif /* ! Q_OS_UNIX */
#endif /* __linux__ */
#if defined(WIN32)
int 
MleStage::checkForDoubleClick(MSG* event)
{
    static DWORD lastTime = 0L;
    static int lastX = 0, lastY = 0;

    // XXX - note, we do not care about wrap - this only happens once
    // every 49.7 days and if you get caught in it, double click
    // again and stop complaining!

    // Get differences.
    DWORD timeDiff = event->time - lastTime;
    int posDiffX = event->pt.x - lastX, posDiffY = event->pt.y - lastY;

    // Make sure time and distance is within the correct amount.
    int ret = 0;
    if ((posDiffX >= -MAX_X_DISTANCE ) && (posDiffX <= MAX_X_DISTANCE))
	{
        if ((posDiffY >= -MAX_Y_DISTANCE) && (posDiffY <= MAX_Y_DISTANCE)) 
		{
			if (timeDiff <= DOUBLE_CLICK_MSECS)
			{
				ret = 1;
			}
        }
    }

    // Reset last pos and time.
    lastTime = event->time;
    lastX = event->pt.x;
    lastY = event->pt.y;

    // Return the value.
    return(ret);
}
#endif /* WIN32 */

#endif /* MLE_REHEARSAL utility for subclasses */

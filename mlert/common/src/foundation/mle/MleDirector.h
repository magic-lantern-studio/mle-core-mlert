/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MleDirector.h
 * @ingroup MleFoundation
 *
 * This contains the title globals that are needed by the default
 * Magic Lantern runtime (the title engine).  It does not contain title
 * specific globals; they are contained in the titleData structure.
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
//  For information concerning this header file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

#ifndef __MLE_DIRECTOR_H_
#define __MLE_DIRECTOR_H_

// Include Magic Lantern header files.
#include "mle/mlTypes.h"

#include "mle/MleRuntime.h"


// Forward class declarations

#if defined(_WINDOWS) || defined(__linux__) || defined(__APPLE__)
class MleEventDispatcher;
#endif
class MleScheduler;
class MleDppInput;


/**
 * The MleDirector structure contains title generic
 * information that is needed by parts of the Magic Lantern runtime engine.
 */
typedef struct MLE_RUNTIME_API MleDirector
{
    /**
     * The execution cycle scheduler.
     */
    MleScheduler *m_theScheduler;

#if defined(_WINDOWS) || defined(__linux__) || defined(__APPLE__)
    /**
     * The event dispatch manager.
     */
    MleEventDispatcher *m_theEventMgr;
#endif

    /**
     * @brief Flag for exiting a title gracefully.
     *
     * Set to FALSE to quit title.
     */
    MlBoolean m_quit;

    /**
     * @brief The Digital Playprint reader.
     */
    MleDppInput *m_dpp;

    /**  
     * @brief Machine specific data.
     *
     * Clients for this data include the stage, callbacks
     * for system events, possibly a central audio class, etc. 
     */
    void *m_platformData; 
  
    /**
     * @brief Title specific global state is stored relative to this pointer.
     */
    void *m_titleData;

} MleDirector;


/**
 * The main title elements as managed by the MleDirector.
 */
extern MLE_RUNTIME_API MleDirector *g_theTitle;

// Forward declaration.
struct MleSchedulerPhase;

/**
 * Global actor phase handle for use with MleScheduler::insertFunc().
 */
extern MLE_RUNTIME_API MleSchedulerPhase *g_theActorPhase;

/**
 * Global post actor phase handle for use with MleScheduler::insertFunc().
 */
extern MLE_RUNTIME_API MleSchedulerPhase *g_thePostActorPhase;

/**
 * Global pre role phase handle for use with MleScheduler::insertFunc().
 */
extern MLE_RUNTIME_API MleSchedulerPhase *g_thePreRolePhase;

/**
 * Global role phase handle for use with MleScheduler::insertFunc().
 */
extern MLE_RUNTIME_API MleSchedulerPhase *g_theRolePhase;

/**
 * Global set phase handle for use with MleScheduler::insertFunc().
 */
extern MLE_RUNTIME_API MleSchedulerPhase *g_theSetPhase;

/**
 * Global stage phase handle for use with MleScheduler::insertFunc().
 */
extern MLE_RUNTIME_API MleSchedulerPhase *g_theStagePhase;


// XXX - Hacks for prototype so that existing demos will still work.
#define PHASE_ACTOR g_theActorPhase
#define PHASE_POST_ACTOR g_thePostActorPhase
#define PHASE_PRE_ROLE g_thePreRolePhase
#define PHASE_ROLE g_theRolePhase
#define PHASE_SET g_theSetPhase
#define PHASE_STAGE g_theStagePhase

#define PHASE_COLLISION PHASE_POST_ACTOR
#define PHASE_PUSH PHASE_PRE_ROLE


#endif /* __MLE_DIRECTOR_H_ */

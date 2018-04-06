/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MleScheduler.h
 * @ingroup MleFoundation
 *
 * @author Mark S. Millard
 * @created May 1, 2003
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

#ifndef __MLE_SCHEDULER_H_
#define __MLE_SCHEDULER_H_

// Include Magic Lantern header files.
#include "mle/mlTypes.h"

// Include Runtime Engine header files.
#include "mle/MleRuntime.h"

// Routine control block
struct MleSchedulerItem;

// Phase control block
struct MleSchedulerPhase;

//
// Define default scheduled phases that all of our general actors, 
// delegates, forums, and stages can use.
//

/**
 * @brief The MleScheduler class implements the Magic Lantern runtime engine scheduler.
 *
 * The MleScheduler class implements the Magic Lantern runtime engine scheduler.
 * A scheduler object executes a sequence of <b>phases</b>, calling a 
 * sequence of scheduled callbacks in each phase.  Each complete pass of
 * the scheduler typically corresponds to a single frame of the title's
 * computation and display.  Each callback registered with the scheduler
 * typically corresponds to the update function of an actor in the title.
 */
class MLE_RUNTIME_API MleScheduler
{

    friend class MleSchedulerIterator;

  // Declare member variables.

  private:
	
    MleSchedulerPhase** m_phaseArray;  // the phases
    MleSchedulerItem* m_freeItem;      // pool of free items
    MleSchedulerItem* m_iterator;      // the iterator
    MleSchedulerItem* m_deleteItem;    // pool of deleted items
    MleSchedulerItem* m_memLink;       // reference to memory
    unsigned int m_maxPhases;          // maximum number of phases
    unsigned int m_inUsePhases;        // number of items in use
    int m_initSize;                    // initial itemMemory size
  

  // Declare member functions.

  public:

    /**
     * @brif A constructor that specifies how many phases this instance of scheduler has,
     * as well as how many routines it might use.
	 *
	 * @param phases The number of phases the scheduler should have.
	 * The default size is set to (6).
	 * @param items The number of items each phase should have.
	 * The default is two hundred (200).
	 */
    MleScheduler(unsigned int phases=6, unsigned int items=200);

    /**
	 * @brief The destructor.
	 */
    virtual ~MleScheduler();
    
    /**
	 * @brief Initialize the scheduler.
	 *
     * Set up the known phases actor/role/set/stage for
     * standard MLE titles.
     * This is provided as a convenience function in the scheduler because
     * the four global phase symbols MUST be assigned in order for any
     * MLE actors, roles, sets, and/or stages to work.  These parts
     * use the global phase symbols to schedule their functions and rely
     * on their having legal phase values.  If you are writing your own
     * code and not using any of the four types of MLE parts, you can
     * omit use of this method.
	 */
    void init(void);
    
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

    // Additional phase allocation routines to insert/delete.

    /**
     * @brief Insert an existing phase into the scheduler.
	 *
     * Insert the new phase before the beforePhase, or append to the end
     * of the phases list if beforePhase is NULL.
     * If phase is NULL, allocate a new phase for insertion.
	 *
	 * @param phase The phase to insert into the scheduler.
	 * @param beforePhase The place to insert the phase to.
	 *
	 * @return Return a pointer to the inserted phase, 
     * or NULL if we failed to allocate more memory.
	 */
    MleSchedulerPhase* insertPhase(MleSchedulerPhase* phase=NULL, 
			      MleSchedulerPhase* beforePhase=NULL);

    /**
     * Remove a phase from the scheduler without deleting it.
	 *
	 * @param The phase to delete.
	 */
    void removePhase(MleSchedulerPhase* phase);


#if defined(MLE_DEBUG)

    /**
	 * @brief Insert scheduled function.
	 *
     * Add a function to be scheduled, specifying the phase to run the 
     * function in, the function, a void* to pass the function each time 
     * step.  Optionally specify a tag for use with the remove member, 
     * the interval between calls (ie. 2 means call every second time), 
     * and the first interval(ie. 3 means only call the third time this 
     * phase is scheduled.)
	 *
	 * @param phase The phase to run the function in.
	 * @param func The function to insert.
	 * @param data A pointer to data that will be used upon callback
	 * to the inserted function.
	 * @param tag An identifier to be used for classification.
	 * @param interval The interval between function invokation.
	 * @param firstInterval The first time the function should be called.
	 * @param name A name.
	 */
    MleSchedulerItem* insertFunc(MleSchedulerPhase* phase, 
			    void (*func)(void*),
			    void* data,
			    void* tag,
			    unsigned int interval =1,
			    unsigned int firstInterval = 1,
			    char *name = NULL);

	/**
	 * @brief Dump the contents of the scheduler to stdout.
	 */
    void dump();

#else /* ! MLE_DEBUG */

    /**
	 * @brief Insert scheduled function.
	 *
     * Add a function to be scheduled, specifying the phase to run the 
     * function in, the function, a void* to pass the function each time 
     * step.  Optionally specify a tag for use with the remove member, 
     * the interval between calls (ie. 2 means call every second time), 
     * and the first interval(ie. 3 means only call the third time this 
     * phase is scheduled.)
	 *
	 * @param phase The phase to run the function in.
	 * @param func The function to insert.
	 * @param data A pointer to data that will be used upon callback
	 * to the inserted function.
	 * @param tag An identifier to be used for classification.
	 * @param interval The interval between function invokation.
	 * @param firstInterval The first time the function should be called.
     */
    MleSchedulerItem* insertFunc(MleSchedulerPhase* phase, 
			    void (*func)(void*),
			    void* data,
			    void* tag,
			    unsigned int interval =1,
			    unsigned int firstInterval = 1);
#endif
	
    /**
	 * Remove a scheduled item based on a specific
     * MleSchedulerItem* (when the function was scheduled).
	 *
	 * @param item A pointer to the MleSchedulerItem that was
	 * registered upon insertion.
     */
    void remove(MleSchedulerItem* item);

    /**
	 * @brief Remove a scheduled item based on a pre-defined tag.
	 *
	 * @param tag Remove all functions scheduled with that tag.
	 */
    void remove(void* tag);
    
    /**
	 * @brief Execute the specified phase.
	 *
     * Check all functions associated with this phase, running them
     * dependent on their interval.
	 *
	 * @param phase A pointer to the phase to execute.
	 */
    void go(MleSchedulerPhase* phase);

    /**
	 * @brief Execute all registered phases.
	 *
     * Iterate through all phases in their insertion order, 
     * checking all functions associated with each phase, running them
     * dependent on their interval.
	 */
    void goAll(void);
    
  private:
  
	// Allocate memory for an item.
    void makeItemMemory(void);

#ifdef MLE_REHEARSAL
    static void notify(void *key,MleScheduler *sched);
#endif /* MLE_SCHEDULER */
};

/**
 * @brief The MleSchedulerIterator class allows you to sequentially access all MLE
 * scheduler phases.
 *
 * The MleSchedulerIterator class allows you to sequentially access all Magic
 * Lantern scheduler phases.  It is used to access the first phase of the 
 * scheduler, then to successively step to subsequent phases until
 * all phases have been traversed.  
 *
 * The iterator traverses the phases in their sorted order based 
 * on their places of insertion.  For instance, if you inserted
 * phases A, B, C and D as follows, the iteration order would be
 * ADBC.
 *
 * <code>
 * theTitle->scheduler->insertPhase( A );
 * theTitle->scheduler->insertPhase( B );
 * theTitle->scheduler->insertPhase( C );
 * theTitle->scheduler->insertPhase( D, B ); // D before B
 * </code>
 *
 * You may operatr on multiple iterators at once, even if all share
 * a given scheduler.  However, inserting or deleting a phase while
 * operating an iterator may yield unexpected behavior.
 *
 * For instance, this example code is used within the Magic Lantern
 * runtime engine scheduler to iterate over all scheduled phases in 
 * the title.
 *
 * <code>
 * // The standard loop over all scheduled phases looks like this.
 *
 * MleSchedulerIterator iter( theTitle->scheduler );
 *
 * for ( MleSchedulerPhase *phase = iter.firstPhase();
 *     NULL != phase;
 *     phase = iter.nextPhase() ) 
 * {
 *     theTitle->scheduler->go( phase );
 * }
 * </code>
 */
class MLE_RUNTIME_API MleSchedulerIterator
{

  public:
  
    /**
	 * @brief Constructor referencing the sceduler.
	 *
     * @param scheduler Create an iterator that is not useable until a scheduler is set.
	 */
    MleSchedulerIterator(MleScheduler *scheduler);
  
    /**
	 * @brief The destructor.
	 */
    virtual ~MleSchedulerIterator();

    // Iterators on the phases in the scheduler.

    /**
	 * @brief Get the first phase.
	 *
     * @return Return a pointer to the first phase in the scheduler.
	 */
    MleSchedulerPhase* firstPhase(void);

    /**
	 * @brief Get the next phase.
	 *
     * @return Return a pointer to the next phase in the scheduler, assuming
     * that firstPhase() has been called and that we have not run out
     * of phases.  Returns NULL if no more phases.
	  */
    MleSchedulerPhase* nextPhase(void);

  protected:
    MleScheduler *scheduler;
    unsigned int currentPhaseIndex;
};
  

/**
 * @brief Convenience typedef for casting functions to pass to insertFunc().
 *
 * This allows a programmer to do, for example:
 *
 * <code>
 * void myfunction(MyObject *);
 *
 * theScheduler->scheduler->insertFunc(PHASE_ACTOR,(MleSchedulerFunc)myfunction, ...
 * </code>
 *
 * instead of the more cryptic cast of
 *
 * <code>
 *   (void (*)(void *))myfunction
 * </code>
 */
typedef void (*MleSchedulerFunc)(void *);


#endif /* __MLE_SCHEDULER_H_ */



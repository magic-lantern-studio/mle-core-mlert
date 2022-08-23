/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MleScheduler.h
 * @ingroup MleFoundation
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015-2022 Wizzer Works
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

// Include system header files.
#if defined(MLE_DEBUG)
#ifdef WIN32
#include <string.h>
#else
#include <strings.h>
#endif /* WIN32 */
#endif /* MLE_DEBUG */

// Include Magic Lantern header files.
#include "mle/mlMacros.h"
#include "mle/mlMalloc.h"
#include "mle/mlAssert.h"

// Include Runtime Engine header files.
#include "mle/MleDirector.h"
#include "mle/MleScheduler.h"

#ifdef MLE_REHEARSAL
#include <stdio.h>

#include "mle/MleMonitor.h"
#endif

 
/**
 * MleSchedulerItem holds all info on scheduled routines.
 *
 * XXX - This should really be a class so that it would be easier
 *       to subclass to add in a pause flag and override the go() method.
 */
struct MleSchedulerItem {
    MleSchedulerItem* m_next;
    MleSchedulerItem** m_prev;  
    void (*m_func)(void*);
    void* m_data;
    void* m_tag;
    unsigned int m_count;
    unsigned int m_interval;
#if defined(MLE_DEBUG)
    char *m_name;
#endif
};

/**
 * MleSchedulerPhase holds the information for a single phase of routines.
 */
struct MleSchedulerPhase {
    MleSchedulerItem* m_first;
};



/////////////////////////////////////////////////////////////////////////////
//
// MleScheduler data member definition

MleScheduler::MleScheduler(unsigned int phases, unsigned int items)
{
    unsigned int index;

    // Create phase array 
    MLE_ASSERT(phases > 0);
    m_phaseArray = new MleSchedulerPhase* [phases];
    m_maxPhases = phases;
	
    // Start all the phases empty
    m_inUsePhases = 0;
    for(index = 0; index<phases; index++)
    {
	    m_phaseArray[index] = NULL;
    }
	
    // set initSize to specified size.
    m_initSize = items;
    m_memLink = NULL;
    makeItemMemory();
    
    // NULL iterator indicates not within a call to go()
    m_iterator = NULL;
    m_deleteItem = NULL;
}
  
MleScheduler::~MleScheduler()
{
    // Delete the contents of the phase array first, then the
    // array itself
	
    // Iterate through the phases
    MleSchedulerIterator iter(this);
    for (MleSchedulerPhase* phase = iter.firstPhase();
	     phase != NULL;
	     phase = iter.nextPhase())
    {
	    removePhase(phase);
	    delete phase;
    }

    delete[] m_phaseArray;

    while (m_memLink)
    {
	  MleSchedulerItem *next = m_memLink->m_next;
	  delete m_memLink;
	  m_memLink = next;
    }
}

// 
// Set up the four standard MLE phases.
//
// This is provided as a convenience function in the scheduler because
// the four global phase symbols MUST be assigned in order for any
// MLE actors, roles, sets, and/or stages to work.  These parts
// use the global phase symbols to schedule their functions and rely
// on their having legal phase values.  If you are writing your own
// code and not using any of the four types of MLE parts, you can
// omit use of this method.
// 
// We could also insert them in reverse order so as to never
// traverse the whole list.  Though this would be only marginally 
// faster, it\'s worth pointing out since it illustrates how to 
// insert before another phase.
// {
//       mleTheStagePhase    = insertPhase(NULL);
//       mleTheSetPhase    = insertPhase(NULL, mleTheStagePhase);
//       mleTheRolePhase = insertPhase(NULL, mleTheSetPhase);
//       mleTheActorPhase    = insertPhase(NULL, mleTheRolePhase);
// }
void
MleScheduler::init(void)
{
	g_theActorPhase       = insertPhase();
	g_thePostActorPhase   = insertPhase();
	g_thePreRolePhase     = insertPhase();
	g_theRolePhase        = insertPhase();
	g_theSetPhase         = insertPhase();
	g_theStagePhase       = insertPhase();
}

void *
MleScheduler::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleScheduler::operator delete(void *p)
{
	mlFree(p);
}

//
// Insert an existing phase into the scheduler.  Return a pointer
// to the inserted phase, or NULL if we failed to allocate more memory.
//
// Implementation note: we don\'t think that many phases will be inserted
// or deleted, so we are content to use brute force insertion into an
// array.  If you will insist on numerous insertions/deletions, then
// this will not be as efficient as using a doubly linked list.
MleSchedulerPhase*
MleScheduler::insertPhase(MleSchedulerPhase* phase, MleSchedulerPhase* beforePhase)
{

	// As a convenience, allocate a phase if the user does 
	// not provide one.
	if (NULL == phase) {
		phase = new MleSchedulerPhase;
		if (NULL == phase) {
			return NULL;
		}
		phase->m_first = NULL;
	}

	// Find index of the location at which to insert this phase.
	unsigned int insertPoint;
	if (NULL == beforePhase) {
		insertPoint = m_inUsePhases;
	}
	else {
		for (insertPoint=0; insertPoint<m_inUsePhases; insertPoint++) {
			if (m_phaseArray[insertPoint] == beforePhase) {
				break;
			}
		}
		// If we fell through the loop, then we must insert
		// at [inUsePhases] which is one past the end.
	}

	// Reallocate if necessary
	if (m_inUsePhases >= m_maxPhases) {
		m_maxPhases = MAX(m_inUsePhases, 2*m_maxPhases);
		m_phaseArray = (MleSchedulerPhase**) mlRealloc(m_phaseArray, m_maxPhases*sizeof(MleSchedulerPhase*));
		if (NULL == m_phaseArray) {
			return NULL;
		}
	}
		
	// move the phases at and above the insertion point
	for (unsigned int i=m_inUsePhases; i>insertPoint; i--) {
		m_phaseArray[i] = m_phaseArray[i-1];
	}
	m_inUsePhases++;

	m_phaseArray[insertPoint] = phase;
	return m_phaseArray[insertPoint];
}

//
// Remove a phase from the scheduler.  Note that we never reallocate
// the scheduler\'s phase array to be smaller.
//
void
MleScheduler::removePhase(MleSchedulerPhase* phase)
{
	unsigned int removePoint;

	// Find index of the location at which to remove this phase.
	for (removePoint=0; removePoint<m_inUsePhases; removePoint++) {
		if (m_phaseArray[removePoint] == phase) {
			break;
		}
	}

	// If we fell through the loop, then there is nothing to remove.
	if (removePoint == m_inUsePhases) {
		return;
	}

	// move the phases at and above the remove point
	for (unsigned int i=removePoint; i<m_inUsePhases-1; i++) {
		m_phaseArray[i] = m_phaseArray[i+1];
	}
	m_inUsePhases--;
}

void
MleScheduler::makeItemMemory(void)
{
  m_freeItem = new MleSchedulerItem [m_initSize+1];
  
  // link to the old memory block
  m_freeItem[0].m_next = m_memLink;
  m_memLink = m_freeItem;

  // freeItem starts from the 1st item in the block
  m_freeItem = &m_memLink[1];
  for (int index=0; index<m_initSize-1; index++)
    {
      m_freeItem[index].m_next = &(m_freeItem[index+1]);
    }
  m_freeItem[m_initSize-1].m_next = NULL;
}

// Execute functions for a single phase
void
MleScheduler::go(MleSchedulerPhase *phase)
{
    // Assert that we actually have a phase to execute.
    MLE_ASSERT(NULL != phase);
    
    // Assert that go() is not being called recursively
    MLE_ASSERT(m_iterator == NULL);
	
#ifdef MLE_DEBUG
// This conditional code checks that the 
// phase being run is actually owned by the scheduler.  So the phase
// that you pass to go() must be owned by this scheduler.  
// If it\'s not, then you could be mixing free lists and REALLY get
// in trouble on deleting a phase.  So at least we prevent it during
// the debug compilation.	

    {
	    MlBoolean phaseFound = FALSE;

	    MleSchedulerIterator iter(this);
	    for ( MleSchedulerPhase *p = iter.firstPhase();
		  NULL != p;
		  p = iter.nextPhase() ) 
	    {
		    if (p == phase) {
			    phaseFound = TRUE;
			    break;
		    }
	    }
	    MLE_ASSERT(TRUE == phaseFound); // MleScheduler::go(phase) must use a phase held by the scheduler.
    }
#endif /* MLE_DEBUG */

    // Loop over whole phase
    m_iterator = phase->m_first;
    while(m_iterator != NULL)
    {
	    // Check if current item needs to be called
	    if (--m_iterator->m_count == 0)
		{
	        m_iterator->m_func(m_iterator->m_data);
	        m_iterator->m_count = m_iterator->m_interval;
		}
		
	    // Move on (before possible delete)
    	m_iterator = m_iterator->m_next;
		    
	    // Check if that item needs to be deleted.
	    if (m_deleteItem != NULL)
		{
	        // Splice out of table
	        *(m_deleteItem->m_prev) = m_deleteItem->m_next;
	        if (m_deleteItem->m_next != NULL) 
			{
		        m_deleteItem->m_next->m_prev = m_deleteItem->m_prev;
			}
	        m_deleteItem->m_next = m_freeItem;
	        m_freeItem = m_deleteItem;

	       m_deleteItem = NULL;
		}
    }
}


// Execute functions for all phases in iterator order
void
MleScheduler::goAll(void)
{
    MleSchedulerPhase* phase;
    MleSchedulerIterator iter(this);
    for (phase = iter.firstPhase(); 
	 phase != NULL; 
	 phase = iter.nextPhase())
    {
	    go(phase);
    }
}

// Insert function into phase table.  
MleSchedulerItem* MleScheduler::insertFunc(MleSchedulerPhase *phase, 
				 void (*func)(void*),
				 void* data,
				 void* tag, 
				 unsigned int interval,
				 unsigned int firstInterval
#if defined(MLE_DEBUG)
				 , char *name
#endif
				 )
{
    MLE_ASSERT(NULL != phase);
	
    
    // if run out of allocated memory for items
    if (m_freeItem == NULL)
    {
	    makeItemMemory();
    }

    // Get a new MleSchedulerItem
    MLE_ASSERT(m_freeItem != NULL); 
    MleSchedulerItem* ctrlBlk = m_freeItem;
    m_freeItem = m_freeItem->m_next;
    
    
    // Setup control block for this insert
    ctrlBlk -> m_next = NULL;
    ctrlBlk -> m_func = func;
    ctrlBlk -> m_data = data;
    ctrlBlk -> m_tag = tag;
    ctrlBlk -> m_interval = interval;
    ctrlBlk -> m_count = firstInterval;
#if defined(MLE_DEBUG)
    if ( name != NULL ) {
	    ctrlBlk->m_name = _strdup(name);
    } else {
	    ctrlBlk->m_name = NULL;
    }
#endif
	
    // Get to end of phase list to perform FIFO inserts
    // NOTE - if this gets to be a time sink could store the insertPt for each list.
    MleSchedulerItem** insertPoint = &phase->m_first;
    while(*insertPoint != NULL)
    {
	    insertPoint = &((*insertPoint)->m_next);
    }
    *insertPoint = ctrlBlk;
    ctrlBlk->m_prev = insertPoint;
	
#ifdef MLE_REHEARSAL
    // Register with the deletion monitor.
    MleMonitor::g_deleteNotifier.addCallback(tag,(MleNotifierFunc)notify,this);
#endif /* MLE_REHEARSAL */

    return ctrlBlk;
}


// Internal function to perform queue remove
void MleScheduler::remove(MleSchedulerItem* ctrlBlk)
{
    // Determine if are deleting out from under the go() member function
    if (ctrlBlk != m_iterator)
    {
	// Splice out of table
	*(ctrlBlk->m_prev) = ctrlBlk->m_next;
	if (ctrlBlk->m_next != NULL) 
	{
	    ctrlBlk->m_next->m_prev = ctrlBlk->m_prev;
	}
	ctrlBlk->m_next = m_freeItem;
	m_freeItem = ctrlBlk;
#if defined(MLE_DEBUG)
	if ( ctrlBlk->m_name != NULL ) {
	    mlFree(ctrlBlk->m_name);
	}
#endif
    }
    else
    {
	// Tell the go() function to delete it later.
	m_deleteItem = ctrlBlk;
    }
}


// Remove all items matching tag.  For the momment do no optimization for 
// finding tagged items.
void MleScheduler::remove(void* tag)
{
    // Iterate through the phases
    MleSchedulerIterator iter(this);
    for ( MleSchedulerPhase *phase = iter.firstPhase();
	  NULL != phase;
	  phase = iter.nextPhase() ) 
    {
	// Iterate through the routines in this phase
	MleSchedulerItem* item = phase->m_first;
	while(item != NULL)
	{
	    // Delete matching tags
	    if (item->m_tag == tag)
	    {
		    MleSchedulerItem* deadItem=item;
		    item = item->m_next;
	    	remove(deadItem);
	    }
	    else
	    {
		    item = item->m_next;
	    }
	}
    }
}

#if defined(MLE_DEBUG)
//
// This function can be called from cvd to dump the data structures
//

void MleScheduler::dump()
{
    printf("SCHEDULER STRUCTURES\n");
    // Iterate through the phases
    MleSchedulerIterator iter(this);
    for ( MleSchedulerPhase *phase = iter.firstPhase();
	  NULL != phase;
	  phase = iter.nextPhase() ) 
    {
	printf("    PHASE 0x%p\n", phase);
	printf("        IDX FUNCPTR  USERDATA TAG      CNT IVL NAME\n");
	int j = 0;
	for ( MleSchedulerItem *s=phase->m_first ; s!=NULL ; s=s->m_next ) {
	    printf("        %3d 0x%p 0x%p 0x%p %3d %3d %s\n",
		   j,
		   s->m_func,
		   s->m_data,
		   s->m_tag,
		   s->m_count,
		   s->m_interval,
		   s->m_name? s->m_name : "NULL");
	    j++;
	}
    }
}
#endif

#ifdef MLE_REHEARSAL
//
// This function is called from the monitor whenever a monitored object
// (like actors) is called.
//
void
MleScheduler::notify(void *key,MleScheduler *sched)
{
	// Remove any entries in the scheduler associated with the tag
    // sched->remove(key);

	// Really all we need to do is call the remove function.
	// But duplicating the code here gives us a chance to print
	// a warning when something is deleted.  This is a rehearsal
	// service, so failures to remove scheduled functions that
	// are caught here will not be intercepted after mastering.

    // Iterate through the phases
    MleSchedulerIterator iter(sched);
    for ( MleSchedulerPhase *phase = iter.firstPhase();
	  NULL != phase;
	  phase = iter.nextPhase() ) 
    {
		// Iterate through the routines in this phase.
		MleSchedulerItem* item = phase->m_first;
		while (item != NULL)
		{
			// Delete matching tags.
			if (item->m_tag == key)
			{
				MleSchedulerItem* deadItem = item;
				item = item->m_next;

				if ( deadItem != sched->m_iterator )
				printf("MleScheduler warning: a deleted object did not unschedule a function.\n");
				sched->remove(deadItem);
			}
			else
			{
				item = item->m_next;
			}
		}
    }

}
#endif /* MLE_REHEARSAL */

/////////////////////////////////////////////////////////////////////////////
MleSchedulerIterator::MleSchedulerIterator(MleScheduler *sched)
{
	scheduler = sched;

	// set the current phase index beyond the range of existing 
	// phases so the user must call firstPhase() to get started.
	currentPhaseIndex = scheduler->m_inUsePhases;
}

MleSchedulerIterator::~MleSchedulerIterator(void)
{
	scheduler = NULL;
}
//
// Get a pointer to the first phase in the scheduler.
//
MleSchedulerPhase*
MleSchedulerIterator::firstPhase(void)
{
	MLE_ASSERT(NULL != scheduler); // Must have a scheduler to iterate on

	currentPhaseIndex = 0;
	if (currentPhaseIndex < scheduler->m_inUsePhases) {
		return scheduler->m_phaseArray[currentPhaseIndex];
	}
	else {
		return NULL;
	}
}

//
// Get a pointer to the next phase in the scheduler.
//
MleSchedulerPhase*
MleSchedulerIterator::nextPhase(void)
{
	if (++currentPhaseIndex < scheduler->m_inUsePhases) {
		return scheduler->m_phaseArray[currentPhaseIndex];
	}
	else {
		return NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
//
// Testing 

#ifdef UNITTEST_1


#if defined(MLE_DEBUG)

#include <stdio.h>

void testFn(void* parm);
void testFn(void* parm)
{
    printf("%s", parm);
}


static MleSchedulerItem* deleteArg = NULL;
void deleteFn(void* parm);
void deleteFn(void* parm)
{
    printf("Test String D MleScheduler.\n");
    if (deleteArg != NULL)
    {
	((MleScheduler*)parm)->remove(deleteArg);
    }
}

void* deleteTagArg = NULL;
void deleteTagFn(void* parm);
void deleteTagFn(void* parm)
{
    printf("Test String T MleScheduler.\n");
    if (deleteTagArg != NULL)
    {
	((MleScheduler*)parm)->remove(deleteTagArg);
    }
}


main()
{
    char* theString1 = "Test String 1 MleScheduler.\n";
    char* theString2 = "Test String 2 MleScheduler.\n";
    char* theString3 = "Test String 3 MleScheduler.\n";
    char* theString4 = "Test String 4 MleScheduler.\n";
    char* theString5 = "Test String 5 MleScheduler.\n";
    char* theStringNL = "\n";
    MleSchedulerItem* blocks[9];
    
    printf("Initialization.\n");
    MleScheduler* theMleScheduler = new MleScheduler(6, 16);
    MleSchedulerPhase *p0 = theMleScheduler->insertPhase();
    MleSchedulerPhase *p1 = theMleScheduler->insertPhase();
    theMleScheduler->insertFunc(p0, testFn, theStringNL, theStringNL);
    printf("Initialization complete.\n\n");

    printf("First test.\n");
    printf("Should have strings printed in order 13 3 123\n");
    blocks[0] = theMleScheduler->insertFunc(p1, testFn, theString1, theString1, 2);
    blocks[1] = theMleScheduler->insertFunc(p1, testFn, theString2, theString2, 1, 3);
    blocks[2] = theMleScheduler->insertFunc(p1, testFn, theString3, theString3);
    theMleScheduler->go(p1);
    theMleScheduler->go(p0);
    theMleScheduler->go(p1);
    theMleScheduler->go(p0);
    theMleScheduler->go(p1);
    theMleScheduler->go(p0);
    printf("First test complete.\n\n");

    printf("Second test.\n");
    printf("Should have strings printed in order 3D 3D D D. \n");
    theMleScheduler->remove(blocks[0]);
    theMleScheduler->remove(blocks[1]);
    blocks[3] = theMleScheduler->insertFunc(p1, deleteFn, theMleScheduler, theMleScheduler);
    theMleScheduler->go(p1);
    theMleScheduler->go(p0);
    deleteArg = blocks[2];
    theMleScheduler->go(p1);
    theMleScheduler->go(p0);
    deleteArg = NULL;
    theMleScheduler->go(p1);
    theMleScheduler->go(p0);
    deleteArg = blocks[3];
    theMleScheduler->go(p1);
    theMleScheduler->go(p1);
    theMleScheduler->go(p1);
    theMleScheduler->go(p1);
    theMleScheduler->go(p1);
    printf("Second test complete.\n\n");
    
    printf("Third test.\n");
    printf("Should have strings printed in order 544T5 5T5 5T5 5T. \n");
    theMleScheduler->insertFunc(p1, testFn, theString5, theString5);
    theMleScheduler->insertFunc(p1, testFn, theString4, theString4);
    theMleScheduler->insertFunc(p1, testFn, theString4, theString4);
    theMleScheduler->insertFunc(p1, deleteTagFn, theMleScheduler, theString5);
    theMleScheduler->insertFunc(p1, testFn, theString5, theString5);
    theMleScheduler->go(p1);
    theMleScheduler->go(p0);
    theMleScheduler->remove(theString4);
    theMleScheduler->go(p1);
    theMleScheduler->go(p0);
    theMleScheduler->go(p1);
    theMleScheduler->go(p0);
    deleteTagArg = theString5;
    theMleScheduler->go(p1);
    theMleScheduler->go(p0);
    deleteTagArg = NULL;
    theMleScheduler->go(p1);
    theMleScheduler->go(p1);
    theMleScheduler->go(p1);
    theMleScheduler->go(p1);
    theMleScheduler->go(p1);
    printf("Third test complete.\n\n");
   

    return 0;
}	
#else /* MLE_DEBUG */

main ()
{
    return 0;
}


#endif /* MLE_DEBUG */

#endif /* UNITTEST_1 */

/////////////////////////////////////////////////////////////////////////////
#ifdef UNITTEST_2

#include <mle/ptrarray.h>
//#include <iostream>

using namespace std;

class SchedObj 
{
  public:

    SchedObj(char *s="") { str = strdup(s); };
    ~SchedObj() { if (str) { delete str; } };

    static void update(SchedObj *data);
    char*  getStr() { return str; };

    MleSchedulerItem*  getItem() { return item; };
    void setItem(MleSchedulerItem *i) { item = i; };

    //friend ostream& operator <<(ostream&, SchedObj *obj);

    static int outLen;

    // The array of objects we\'re scheduling.
    static MlePtrArray *objArray;
    static int          objArrayLen;

  protected:

    char *str;
    MleSchedulerItem *item;
};

#define MARGIN_COL 72

//
// Globals
//
// The only phase
MleSchedulerPhase* theMleSchedulerPhase = NULL;

// The length written so far this line.
int SchedObj::outLen = 0;

// The array of objects we\'re scheduling.
MlePtrArray* SchedObj::objArray = NULL;
int	    SchedObj::objArrayLen = 0;


void
SchedObj::update( SchedObj *data )
{
	delete new char[8];
	fprintf(ML_DEBUG_OUTPUT_FILE, "%s", data->getStr());
	//cout << data;
	if ((SchedObj::outLen += strlen(data->getStr())) > MARGIN_COL) {
		//cout << "\n" << flush;
		fprintf(ML_DEBUG_OUTPUT_FILE, "\n");
		fflush(ML_DEBUG_OUTPUT_FILE);
		SchedObj::outLen = 0;
	}
}

//ostream& 
//operator << (ostream& os, SchedObj *obj)
//{
//	os << obj->getStr();
//	return os;
//}


void
scheduleObj( MleScheduler *sched, unsigned int i )
{
	// Make a tagged object to schedule
	char id[16];
	sprintf(id, "%.3d", i);
	SchedObj *obj = new SchedObj( id );
	
	// Put into the scheduler
	MLE_ASSERT( NULL != theMleSchedulerPhase);
	MleSchedulerItem *item = sched->insertFunc
		(theMleSchedulerPhase, (MleSchedulerFunc)SchedObj::update, obj, id);
	obj->setItem( item );

	// log to stdout
	fprintf(ML_DEBUG_OUTPUT_FILE, "Add  [%d] = %s\n", i; obj->getStr());
	fflush(ML_DEBUG_OUTPUT_FILE);
	//cout << "Add  [" << i << "] = " << obj << "\n" << flush;

	// Save for future deletion
	(*obj->objArray)[obj->objArrayLen++] = (void*) obj;
}

SchedObj*
unscheduleObj( MleScheduler *sched, unsigned int i )
{
	MLE_ASSERT(i < SchedObj::objArrayLen);
	MLE_ASSERT(SchedObj::objArrayLen >= 1);

	// Find in the array and replace with NULL
	SchedObj *obj = (SchedObj *) (*SchedObj::objArray)[i];

	if (NULL == obj) {
		return NULL;
	}

	// log to stdout
	fprintf(ML_DEBUG_OUTPUT_FILE, "Del  [%d] = %s\n", i; obj->getStr());
	fflush(ML_DEBUG_OUTPUT_FILE);
	//cout << "Del  [" << i << "]   " << obj << "\n" << flush;

	(*SchedObj::objArray)[i] = NULL;

	// Now unschedule the update function and delete the object
	MleSchedulerItem *item = obj->getItem();
	sched->remove(item);

	return obj;
}

int
main(int argc, char *argv[]) 
{
	if (argc != 3) {
		printf("Usage: %s <insert> <insert/remove>\n\
	where <insert>         = number of initial schedule insertions\n\
	      <insert/removes> = number of scheduler operations\n", argv[0]);
		exit(1);
	}
	
	printf("Exercising scheduler insertion/deletion.\n");
	fflush(stdout);

	int inserts = atoi(argv[1]);
	int updates = atoi(argv[2]);

	if (inserts < 0) {
		return 0;
	}

	if (updates < 0) {
		return 0;
	}

	// Idea is to insert a set of funcs, then to make a number
	// of updates (insert/delete) to the scheduler.  
	// To avoid leaking memory, we must keep track of the objects
	// whose update() function we\'re scheduling, so that we can
	// delete the object when we unschedule it.

	SchedObj::objArray = new MlePtrArray(inserts + updates);
	MleScheduler* theMleScheduler = new MleScheduler(6, inserts + updates);
	theMleSchedulerPhase = theMleScheduler->insertPhase();

	unsigned int i;
	for (i=0; i<inserts; i++) {
		scheduleObj( theMleScheduler, i );
	}

	// init seed for random number generation
	// any old integral number will do in place of 1.
	srand( 1 );

	// We know that rand() returns numbers in [0..2^15-1)
#define DRAND(j) (double)((j) / 32768.0)

	for (i=0; i<updates; i++) {

		int j = rand();
		double d = DRAND(j);
		if (d >= 0.5) {
			// insert a new one
			scheduleObj( theMleScheduler, i );
		}
		else {
			// delete an old one
			// We know that DRAND < 1/2, so multiply
			// by 2 to get in the range [0..1) and then
			// scale to size of objArray.

			unsigned int origIndex;
			unsigned int index;
			origIndex = index = SchedObj::objArrayLen * 2 * d;

			SchedObj *obj;
			while (NULL == (obj = unscheduleObj( theMleScheduler, index ))) {
				index = ++index % SchedObj::objArrayLen;
				// prevent infinite loop
				if (index == origIndex) {
					printf("Ran out of items to unschedule.  Increase number of in/dels.\n");
					fflush(stdout);
					return 1;
				}
			}
			delete obj;
		}

	}

	// Final cleanup
	unsigned int n = SchedObj::objArrayLen;
	for (i=0; i<n; i++) {
		SchedObj *obj = unscheduleObj( theMleScheduler, i );
		if (NULL != obj) {
			delete obj;
		}
	}
	
	delete SchedObj::objArray;
	SchedObj::objArray = NULL;
	delete theMleScheduler;

	// 
	printf("Test passed.\n");
	fflush(stdout);
	return 0;
}
	
#endif /* UNITTEST_2 */

/////////////////////////////////////////////////////////////////////////////
#ifdef UNITTEST_3
#include <stdio.h>

// Try to break the scheduler by removing a scheduled function from
// within a scheduled function -- is the removed function ever called?
// If so, the data ptr is probably invalid and we should get a core dump. 

// Global pointer to the only phase we create
MleSchedulerPhase* THE_MLE_PHASE = NULL;

static char 		*theString = NULL;
static MleSchedulerItem 	*theItemA = NULL;
static MleSchedulerItem 	*theItemB = NULL;
static MleSchedulerItem 	*theItemC = NULL;
static MleScheduler 		*theMleScheduler = NULL;

void
funcPrint(void *data)
{
	MLE_ASSERT( NULL != data );

	char *str = (char*) data;
	printf("%s\n", str);
	fflush(stdout);
}

void
funcDelete(void *)
{
	static MleSchedulerItem *item = NULL;

	if (NULL != theString) {
		printf("%s\n", theString);
		delete theString;
		theString = NULL;
		printf("Removing A\n");
		theMleScheduler->remove(theItemA);
		printf("Removing C\n");
		theMleScheduler->remove(theItemC);
		printf("Inserting B2\n");
		item = theMleScheduler->insertFunc
			(THE_MLE_PHASE, (MleSchedulerFunc)funcDelete, theString, theString );

	}
	else {
		// remove both copies of myself the second time around
		printf("Removing B and B2\n");
		theMleScheduler->remove(theString);
	}
}

int
main(int argc, char *argv[]) 
{
	if (argc != 1) {
		printf("Usage: %s\n");
		fflush(stdout);
		exit(1);
	}
	
	printf("Attempting to force scheduler to execute removed func.\n");
	fflush(stdout);

	// set up the global string
	theString = strdup("This string to be deleted.");

	// set up the scheduler
	int nPhases = 1;
	int nItems = 128;
	theMleScheduler = new MleScheduler(nPhases, nItems);
	THE_MLE_PHASE = theMleScheduler->insertPhase();

	// schedule the three items
	theItemA = theMleScheduler->insertFunc
		(THE_MLE_PHASE, (MleSchedulerFunc)funcPrint, "Item A", "Item A" );
	theItemB = theMleScheduler->insertFunc
		(THE_MLE_PHASE, (MleSchedulerFunc)funcDelete, theString, theString );
	theItemC = theMleScheduler->insertFunc
		(THE_MLE_PHASE, (MleSchedulerFunc)funcPrint, "Item C", "Item C" );

	// Should fail on third callback.
	printf("Executing 1 with {A, B, C}.\n");
	fflush(stdout);
	theMleScheduler->go(THE_MLE_PHASE);
	theMleScheduler->go(THE_MLE_PHASE);
	delete theMleScheduler;

	theMleScheduler = new MleScheduler(nPhases, nItems);
	THE_MLE_PHASE = theMleScheduler->insertPhase();

	printf("Executing 2 with {B, B}.\n");
	fflush(stdout);
	theItemB = theMleScheduler->insertFunc
	       (THE_MLE_PHASE, (MleSchedulerFunc)funcDelete, theString, theString );
	theMleScheduler->go(THE_MLE_PHASE);
	delete theMleScheduler;

	// Should succeed with no functions executed if we get this far.
	theMleScheduler = new MleScheduler(nPhases, nItems);
	THE_MLE_PHASE = theMleScheduler->insertPhase();
	printf("Executing 3 with {}.\n");
	fflush(stdout);
	theMleScheduler->go(THE_MLE_PHASE);
	delete theMleScheduler;

	// Cleanup
	theMleScheduler->removePhase(THE_MLE_PHASE);
	delete THE_MLE_PHASE;

	if (theString) delete theString;
	delete theItemA;
	delete theItemB;
	delete theItemC;
	delete theMleScheduler;

	printf("Test passed.\n");
	fflush(stdout);
	return 0;
}
	
#endif /* UNITTEST_3 */

/////////////////////////////////////////////////////////////////////////////
//
// Testing 

#ifdef UNITTEST_4


#if defined(MLE_DEBUG)

#include <stdio.h>

void testFn(void* parm);
void testFn(void* parm)
{
    printf("%s", parm);
}


static MleSchedulerItem* deleteArg = NULL;
void deleteFn(void* parm);
void deleteFn(void* parm)
{
    printf("Test String D MleScheduler.\n");
    if (deleteArg != NULL)
    {
	((MleScheduler*)parm)->remove(deleteArg);
    }
}

void* deleteTagArg = NULL;
void deleteTagFn(void* parm);
void deleteTagFn(void* parm)
{
    printf("Test String T MleScheduler.\n");
    if (deleteTagArg != NULL)
    {
	((MleScheduler*)parm)->remove(deleteTagArg);
    }
}


main()
{
    char* theString1 = "Test String 1 MleScheduler.\n";
    char* theString2 = "Test String 2 MleScheduler.\n";
    char* theString3 = "Test String 3 MleScheduler.\n";
    char* theString4 = "Test String 4 MleScheduler.\n";
    char* theString5 = "Test String 5 MleScheduler.\n";
    char* theStringNL = "\n";
    MleSchedulerItem* blocks[9];
    
    printf("Initialization.\n");
    MleScheduler* theMleScheduler = new MleScheduler(2, 16);

    // Do the first 4 phases, which overflows our requested 2.
    theMleScheduler->mleInit();

    // Now do 2 more.
    MleSchedulerPhase *p0 = theMleScheduler->insertPhase();
    MleSchedulerPhase *p1 = theMleScheduler->insertPhase(NULL, p0);
    theMleScheduler->insertFunc(p0, testFn, theStringNL, theStringNL);
    printf("Initialization complete.\n\n");

    printf("First test.\n");
    printf("Should have strings printed in order 13 3 123\n");
    blocks[0] = theMleScheduler->insertFunc(p1, testFn, theString1, theString1, 2);
    blocks[1] = theMleScheduler->insertFunc(p1, testFn, theString2, theString2, 1, 3);
    blocks[2] = theMleScheduler->insertFunc(p1, testFn, theString3, theString3);
    theMleScheduler->go(p1);
    theMleScheduler->go(p0);
    theMleScheduler->go(p1);
    theMleScheduler->go(p0);
    theMleScheduler->go(p1);
    theMleScheduler->go(p0);
    printf("First test complete.\n\n");

    printf("Second test.\n");
    printf("Should have strings printed in order 3D 3D D D. \n");
    theMleScheduler->remove(blocks[0]);
    theMleScheduler->remove(blocks[1]);
    blocks[3] = theMleScheduler->insertFunc(p1, deleteFn, theMleScheduler, theMleScheduler);
    theMleScheduler->go(p1);
    theMleScheduler->go(p0);
    deleteArg = blocks[2];
    theMleScheduler->go(p1);
    theMleScheduler->go(p0);
    deleteArg = NULL;
    theMleScheduler->go(p1);
    theMleScheduler->go(p0);
    deleteArg = blocks[3];
    theMleScheduler->go(p1);
    theMleScheduler->go(p1);
    theMleScheduler->go(p1);
    theMleScheduler->go(p1);
    theMleScheduler->go(p1);
    printf("Second test complete.\n\n");
    
    printf("Third test.\n");
    printf("Should have strings printed in order 544T5 5T5 5T5 5T. \n");
    theMleScheduler->insertFunc(mleTheActorPhase, testFn, theString5, theString5);
    theMleScheduler->insertFunc(mleTheDelegatePhase, testFn, theString4, theString4);
    theMleScheduler->insertFunc(mleTheForumPhase, testFn, theString4, theString4);
    theMleScheduler->insertFunc(mleTheStagePhase, deleteTagFn, theMleScheduler, theString5);
    theMleScheduler->insertFunc(p1, testFn, theString5, theString5);

    
    theMleScheduler->goAll();

    theMleScheduler->remove(theString4);

    for (int run=0; run<2; run++) {
	    theMleScheduler->goAll();
    }

    deleteTagArg = theString5;

    theMleScheduler->goAll();

    deleteTagArg = NULL;

    theMleScheduler->go(p1);
    theMleScheduler->go(p1);
    theMleScheduler->go(p1);
    theMleScheduler->go(p1);
    theMleScheduler->go(p1);
    printf("Third test complete.\n\n");
   
    printf("Fourth test -- phases before and after removal.\n");

    MleSchedulerIterator iter(theMleScheduler);
    MleSchedulerPhase *p;
    for (p = iter.firstPhase();
	 NULL != p;
	 p = iter.nextPhase()) 
    {
	    printf(" Phase 0x%x\n", p);
    }

    printf(" Removing delegate and stage phases:\n\t0x%x\n\t0x%x\n\n",
	   PHASE_DELEGATE, 
	   PHASE_STAGE );

    theMleScheduler->removePhase(PHASE_DELEGATE);
    theMleScheduler->removePhase(PHASE_STAGE);

    for (p = iter.firstPhase();
	 NULL != p;
	 p = iter.nextPhase()) 
    {
	    printf(" Phase 0x%x\n", p);
    }

    theMleScheduler->removePhase(p0);
    theMleScheduler->removePhase(PHASE_FORUM);
    theMleScheduler->removePhase(p1);
    theMleScheduler->removePhase(PHASE_ACTOR);

    printf("Fourth test complete.\n\n");

    delete theMleScheduler;

    return 0;
}	
#else /* MLE_DEBUG */

main ()
{
    return 0;
}


#endif /* MLE_DEBUG */

#endif /* UNITTEST_4 */


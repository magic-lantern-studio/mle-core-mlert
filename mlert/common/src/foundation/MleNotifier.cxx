/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MleNotifier.cxx
 * @ingroup MleFoundation
 *
 * @author Mark S. Millard
 * @date May 1, 2003
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
#include <string.h>

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"
#include "mle/mlAssert.h"

// Include Runtime Engine header files.
#include "mle/MleNotifier.h"

MleNotifierPool::MleNotifierPool(int p)
{
	// Initialize variables.
	m_poolsize = p;
	m_link = NULL;

	// Allocate the initial entries.
	makePool();
}

MleNotifierPool::~MleNotifierPool()
{
	// Delete all the pools.
	//   Each pool block is actually one larger than the poolsize,
	//   and the first entry is used to link the blocks together.
	//   This code follows those links to delete the blocks when
	//   the object is destroyed.
	while ( m_link )
	{
		MleNotifierEntry *next = m_link->m_next;
		delete m_link;
		m_link = next;
	}
}

void
MleNotifierPool::makePool(void)
{
	// Allocate an array of list items.
	//   A pool block is actually one larger than the pool size.
	//   The extra entry is used to link blocks together.
	m_unused = new MleNotifierEntry[m_poolsize + 1];

	// Link to the old pool.
	m_unused[0].m_next = m_link;
	m_link = m_unused;

	// Initialize the free list.
	m_unused = &m_link[1];
	for ( int i = 0; i < m_poolsize - 1; i++ )
		m_unused[i].m_next = &m_unused[i + 1];
	m_unused[m_poolsize - 1].m_next = NULL;
}

// This function is used by a dictionary to get a new entry (instead
//   of newing one off the heap).  It simply pulls the first element
//   off the free list.
MleNotifierEntry *
MleNotifierPool::get(void)
{
	MleNotifierEntry *rval;	// Temporary for return value.

	// Make a new pool if the current one is used up.
	if ( m_unused == NULL )
	{
#if defined(MLE_DEBUG)
//		printf("MleNotifierPool::get: warning - expanding pool.\n");
#endif
		makePool();
	}

	// Pull an entry off the free list.
	rval = m_unused;
	m_unused = m_unused->m_next;
	return rval;
}

// This function is used by a dictionary to dispose of an entry
//   (instead of deleting it from the heap).  It puts the entry
//   back onto the free list.
void
MleNotifierPool::put(MleNotifierEntry *entry)
{
	// Put the entry on the free list.
	entry->m_next = m_unused;
	m_unused = entry;
}

void *
MleNotifierPool::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleNotifierPool::operator delete(void *p)
{
	mlFree(p);
}

// This notifier constructor makes a new pool.
MleNotifier::MleNotifier(int hashsize,int poolsize)
{
	m_pool = new MleNotifierPool(poolsize);

	initialize(hashsize);
}

// This notifier constructor uses an existing pool.
MleNotifier::MleNotifier(int hashsize,MleNotifierPool *p)
{
	m_pool = p;

	initialize(hashsize);
}

MleNotifier::~MleNotifier()
{
	// Loop over all the hash table lists.
	for ( int i = 0; i < m_nbuckets; i++ )
	{
		// Prepend all the hash table lists to the pool free list.
		//   Use intimate knowledge of the hash table instead of
		//   doing this one at a time.
		if ( m_bucket[i] )
		{
			// go to the last entry
			MleNotifierEntry *entry = m_bucket[i];
			while ( entry->m_next )
				entry = entry->m_next;
			
			entry->m_next = m_pool->m_unused;
			m_pool->m_unused = m_bucket[i];
		}
	}
}

// This is an initialization function that constructors can share.
void
MleNotifier::initialize(int hashsize)
{
	m_nbuckets = hashsize;
	m_bucket = new MleNotifierEntry*[m_nbuckets];

	// Zero out the bucket.
	memset(m_bucket,0,m_nbuckets*sizeof(MleNotifierEntry *));
}

// This function adds a notification request.  Calling this function
//   multiple times with identical arguments will result in duplicate
//   entries.
void
MleNotifier::addCallback(const void *key,MleNotifierFunc func,const void *client)
{
	// Create a new entry.
	MleNotifierEntry *entry = m_pool->get();
	/* XXX 6.2 Beta compilers caught this, should fix need to cast. */
	entry->m_key = (void *)key; 
	entry->m_func = func;
	/* XXX 6.2 Beta compilers caught this, should fix need to cast. */
	entry->m_client = (void *)client;

	// Prepend it to the right list.
	MleNotifierEntry *&list = m_bucket[hash(key)];
	entry->m_next = list;
	list = entry;
}

// This function removes a specific notification request.  If multiple
//   duplicate requests exist, only the most recently added will be removed.
void
MleNotifier::removeCallback(const void *key,
	MleNotifierFunc func,const void *client)
{
	// Get the right list.
	MleNotifierEntry **entry = &m_bucket[hash(key)];

	while ( *entry )
	{
		// Look for the right entry.
		if ( (*entry)->m_key == key &&
		     (*entry)->m_func == func &&
		     (*entry)->m_client == client )
		{
			// Save the pointer.
			MleNotifierEntry *tmp = *entry;

			// Make the previous pointer skip over this entry.
			*entry = (*entry)->m_next;

			// Return the entry to the free list.
			m_pool->put(tmp);

			return;
		}

		entry = &(*entry)->m_next;
	}

	// shouldn't ever get here.
	MLE_ASSERT(0);
}

// This function removes all notification requests associated with a key.
void
MleNotifier::removeCallbacks(const void *key)
{
	// Get the right list.
	MleNotifierEntry **entry = &m_bucket[hash(key)];

	while ( *entry )
	{
		// Look for the right entry.
		if ( (*entry)->m_key == key )
		{
			// Save the pointer
			MleNotifierEntry *tmp = *entry;

			// Make the previous pointer skip over this entry.
			*entry = (*entry)->m_next;

			// Return the entry to the free list.
			m_pool->put(tmp);
		}
		else
			entry = &(*entry)->m_next;
	}
}

// This static function does recursive traversal of a bucket.
//   Recursion is used instead of iteration to allow post-order traversal.
static void traverse(const void *key,MleNotifierEntry *entry)
{
	if ( entry->m_next )
		traverse(key,entry->m_next);
	
	if ( entry->m_key == key )
		(*entry->m_func)(key,entry->m_client);
}

// This function notifies all interested parties who have registered with
//   the passed key.
void
MleNotifier::notify(const void *key) const
{
	// Pick the right bucket.
	MleNotifierEntry *entry = m_bucket[hash(key)];

	// This list can easily be traversed through iteration.  At a
	//   slight cost, we traverse through recursion instead to execute
	//   the list items from tail to head.  Because list items are
	//   inserted at the head, this results in execution in the same
	//   order as insertion.
	if ( entry )
		traverse(key,entry);
}

// This is the default hash function, to map pointer values into
//   bucket indices.
unsigned int
MleNotifier::hash(const void *key) const
{
	unsigned int rval;

	// This hash function is a little ad hoc, but seems to work
	//   reasonably well.  It is the classic multiply/modulo thing.
	rval = ((0xa74b2193*(unsigned long)key) >> 16) % m_nbuckets;

	return rval;
}

void *
MleNotifier::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MleNotifier::operator delete(void *p)
{
	mlFree(p);
}

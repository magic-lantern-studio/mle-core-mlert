/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MlePtrDict.cxx
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

// Include Runtime Engine header files.
#include "mle/MlePtrDict.h"

/*
 * This is the constructor for a pool.  The argument is the initial
 * pool size, which is also the blocksize for pool extensions.
 */
MlePtrDictPool::MlePtrDictPool(int p)
{
	/* Initialize variables. */
	m_poolsize = p;
	m_link = NULL;

	/* Allocate the initial entries. */
	makePool();
}

MlePtrDictPool::~MlePtrDictPool()
{
	/* Delete all the pools. */
	/*   Each pool block is actually one larger than the poolsize, */
	/*   and the first entry is used to link the blocks together. */
	/*   This code follows those links to delete the blocks when */
	/*   the object is destroyed. */
	while ( m_link )
	{
		MlePtrDictEntry *next = m_link->m_next;
		delete m_link;
		m_link = next;
	}
}

void
MlePtrDictPool::makePool(void)
{
	/* Allocate an array of list items. */
	/*   A pool block is actually one larger than the pool size. */
	/*   The extra entry is used to link blocks together */
	m_unused = new MlePtrDictEntry[m_poolsize + 1];

	/* Link to the old pool. */
	m_unused[0].m_next = m_link;
	m_link = m_unused;

	/* Initialize the free list. */
	m_unused = &m_link[1];
	for ( int i = 0; i < m_poolsize - 1; i++ )
		m_unused[i].m_next = &m_unused[i + 1];
	m_unused[m_poolsize - 1].m_next = NULL;
}

/*
 * This function is used by a dictionary to get a new entry (instead
 * of newing one off the heap).  It simply pulls the first element
 * off the free list.
 */
MlePtrDictEntry *
MlePtrDictPool::get(void)
{
	MlePtrDictEntry *rval;	// Temporary for return value.

	/* Make a new pool if the current one is used up. */
	if ( m_unused == NULL )
	{
		printf("warning: expanding MlePtrDictPool.\n");
		makePool();
	}

	/* Pull an entry off the free list. */
	rval = m_unused;
	m_unused = m_unused->m_next;
	return rval;
}

/*
 * This function is used by a dictionary to dispose of an entry
 * (instead of deleting it from the heap).  It puts the entry
 * back onto the free list.
 */
void
MlePtrDictPool::put(MlePtrDictEntry *entry)
{
	/* put the entry on the free list. */
	entry->m_next = m_unused;
	m_unused = entry;
}

void *
MlePtrDictPool::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MlePtrDictPool::operator delete(void *p)
{
	mlFree(p);
}

/*
 * This dictionary constructor makes a new pool.
 */
MlePtrDict::MlePtrDict(int hashsize,int poolsize)
{
	m_pool = new MlePtrDictPool(poolsize);

	m_nbuckets = hashsize;
	m_bucket = new MlePtrDictEntry*[m_nbuckets];

	/* Zero out the bucket. */
	memset(m_bucket, 0, m_nbuckets * sizeof(MlePtrDictEntry*));
}

/*
 * This dictionary constructor uses an existing pool.
 */
MlePtrDict::MlePtrDict(int hashsize,MlePtrDictPool *p)
{
	m_pool = p;

	m_nbuckets = hashsize;
	m_bucket = new MlePtrDictEntry*[m_nbuckets];

	/* Zero out the bucket. */
	memset(m_bucket, 0, m_nbuckets * sizeof(MlePtrDictEntry*));
}

MlePtrDict::~MlePtrDict()
{
	/* Loop over all the hash table lists. */
	for ( int i = 0; i < m_nbuckets; i++ )
	{
		/* Prepend all the hash table lists to the pool free list. */
		/*   Use intimate knowledge of the hash table instead of */
		/*   doing this one at a time. */
		if ( m_bucket[i] )
		{
			/* Go to the last entry. */
			MlePtrDictEntry *entry = m_bucket[i];
			while ( entry->m_next )
				entry = entry->m_next;
			
			entry->m_next = m_pool->m_unused;
			m_pool->m_unused = m_bucket[i];
		}
	}
}

/*
 * This function finds the pointer registered with the key argument.
 */
void *
MlePtrDict::find(const void *key) const
{
	/* Find the right hash table list. */
	MlePtrDictEntry *entry = m_bucket[hash(key)];

	/* Search the list for the key */
	while ( entry )
	{
		if ( match(entry->m_key,key) )
			return entry->m_ptr;
		entry = entry->m_next;
	}

	return NULL;
}

/*
 * This function associates a key and a value.  The association will
 * overwrite any previously made association for that key.
 */
void
MlePtrDict::set(const void *key,const void *value)
{
	/* Find the appropriate bucket. */
	MlePtrDictEntry *&list = m_bucket[hash(key)];

	/* Find the right entry. */
	MlePtrDictEntry *entry = list;
	while ( entry )
	{
		if ( match(entry->m_key,key) )
			break;
		
		entry = entry->m_next;
	}

	/* If none, create a new one. */
	if ( entry == NULL )
	{
		entry = m_pool->get();

		/* Prepend to appropriate list. */
		entry->m_next = list;
		list = entry;

		entry->m_key = (void *)key;
	}

	/* Set the pointer. */
	entry->m_ptr = (void *)value;
}

/*
 * This function removes an association for the key argument.
 */
void
MlePtrDict::remove(const void *key)
{
	MlePtrDictEntry **entry = &m_bucket[hash(key)];

	while ( *entry )
	{
		/* Look for the right entry. */
		if ( match((*entry)->m_key,key) )
		{
			/* Save the pointer in a temp variable. */
			MlePtrDictEntry *tmp = *entry;

			/* Make the previous pointer skip over this entry. */
			*entry = (*entry)->m_next;

			/* Return the entry to the free list. */
			m_pool->put(tmp);

			return;
		}

		entry = &(*entry)->m_next;
	}
}

/*
 * This function returns the pool this dictionary uses.
 */
MlePtrDictPool *
MlePtrDict::getPool(void) const
{
	return m_pool;
}

/*
 * This is the default hash function.
 */
unsigned int
MlePtrDict::hash(const void *key) const
{
	unsigned int rval;

	/* This hash function is a little ad hoc, but seems to work */
	/*   reasonably well.  It is the classic multiply/modulo thing */
	rval = ((0xa74b2193*(unsigned long)key) >> 16) % m_nbuckets;

	return rval;
}

/*
 * This is the default key matching function.
 */
int
MlePtrDict::match(const void *key0,const void *key1) const
{
	/* This is simply pointer equality. */
	return (key0 == key1);
}

void *
MlePtrDict::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}

void
MlePtrDict::operator delete(void *p)
{
	mlFree(p);
}


#ifdef UNITTEST

#include <string.h>

main()
{
	MlePtrDict *dict = NULL;

	while ( 1 )
	{
		char cmd[32];

		scanf("%s",cmd);
		if ( !strcmp(cmd,"m") )
		{
			int hsize,psize;
			scanf("%d%d",&hsize,&psize);
			dict = new MlePtrDict(hsize,psize);
			printf("dict created.\n");
		}
		else if ( !strcmp(cmd,"a") )
		{
			void *key;
			void *value;
			scanf("%d%d",&key,&value);
			dict->set(key,value);
			printf("added entry %d %d.\n",key,value);
		}
		else if ( !strcmp(cmd,"r") )
		{
			void *key;
			scanf("%d",&key);
			dict->remove(key);
			printf("removed %d.\n",key);
		}
		else if ( !strcmp(cmd,"p") )
		{
			void *key;
			scanf("%d",&key);
			void *value = dict->find(key);
			if ( value )
				printf("key %d = %d.\n",key,value);
			else
				printf("key %d not found.\n",key);
		}
	}
}

#endif /* UNITTEST */

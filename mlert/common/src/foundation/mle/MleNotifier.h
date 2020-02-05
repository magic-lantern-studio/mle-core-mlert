/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MleNotifier.h
 * @ingroup MleFoundation
 *
 * @author Mark S. Millard
 * @created Sep 21, 2005
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

#ifndef __MLE_NOTIFIER_H_
#define __MLE_NOTIFIER_H_

// Include Runtime Engine header files.
#include "mle/MleRuntime.h"
#include "mle/MlePtrDict.h"

// This is a typedef for the notifier callback function pointer.
typedef void (*MleNotifierFunc)(const void *key,const void *client);

// This is the struct managed by a notifier.
//   Its use is internal for the most part, but is made public for
//   any exceptions.
struct MLE_RUNTIME_API MleNotifierEntry
{
    void *m_key;
    MleNotifierFunc m_func;
    void *m_client;
    
    MleNotifierEntry *m_next;
};

class MLE_RUNTIME_API MleNotifierPool
{
  public:

    /**
	 * The constructor.
	 *
     * @param poolsize The poolsize argument to the constructor determines the
     * initial number of entries and the number added on every
     * extension.
	 */
    MleNotifierPool(int poolsize);

	/**
	 * The destructor.
	 */
    ~MleNotifierPool();

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

  private:

    friend class MleNotifier;
    
    // Pool expansion.
    void makePool(void);
    
    // Allocation and deallocation.
    MleNotifierEntry *get(void);
    void put(MleNotifierEntry *);
    
    int m_poolsize;				// Allocation block size.
    MleNotifierEntry *m_unused;	// Pointer to free list.
    MleNotifierEntry *m_link;	// Pointer to first block.
};

class MLE_RUNTIME_API MleNotifier
{
  public:

    MleNotifier(int hashsize,int poolsize);

    MleNotifier(int hashsize,MleNotifierPool *pool);

    ~MleNotifier();
    
    void initialize(int hashsize);
    
    // Callback management.
    void addCallback(const void *key,
		MleNotifierFunc func,const void *client);

    void removeCallback(const void *key,	
		MleNotifierFunc func,const void *client);

    void removeCallbacks(const void *key);
    
    // callback invocation.
    void notify(const void *key) const;
    
    MleNotifierPool *getPool(void) const { return m_pool; }

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

  protected:

	/** The number of hash table entries. */
    int m_nbuckets;
	/** A pointer to the hash table. */
    MleNotifierEntry **m_bucket;
    
    /**
	 * Hash function.
	 *
	 * @param key The key for the hash.
	 */
    unsigned int hash(const void *key) const;

	/** A pointer to the allocation pool. */
	MleNotifierPool *m_pool;
};

#endif /* __MLE_NOTIFIER_H_ */

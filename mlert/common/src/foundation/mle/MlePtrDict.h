/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MlePtrDict.h
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

#ifndef __MLE_PTRDICT_H_
#define __MLE_PTRDICT_H_

#include "mle/MleRuntime.h"

// This is the struct managed by a dictionary
//   Its use is internal for the most part, but is made public for
//   any exceptions.
struct MLE_RUNTIME_API MlePtrDictEntry
{
    void *m_key;
    void *m_ptr;
    MlePtrDictEntry *m_next;
};

/**
 * This is the manager for allocation of MlePtrDictEntry objects
 * from a pool. 
 *
 * This is the manager for allocation of MlePtrDictEntry objects
 * from a pool.  The initial poolsize passed in with the constructor
 * determines the chunk size of the pool.  A pool can be shared
 * among multiple dictionaries.
 */
class MLE_RUNTIME_API MlePtrDictPool
{
  public:

	/**
	 * The constructor.
	 *
     * @param poolsize The poolsize argument to the constructor determines the
     * initial number of entries and the number added on every
     * extension.
	 */
    MlePtrDictPool(int poolsize);

	/**
	 * the destructor.
	 */
    ~MlePtrDictPool();
    
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

    friend class MlePtrDict;
    
    /*
	 * This method is called when the pool needs to be expanded,
     * which is at least once (when the pool is constructed).
     * The pool is increased by poolsize entries
	 */
    void makePool(void);
    
    // Allocation and deallocation.
    //   get() takes an entry off the free list and returns a pointer.
    MlePtrDictEntry *get(void);

	//   put() takes an entry and adds it to the free list.
    void put(MlePtrDictEntry *);
    
    int m_poolsize;				// Allocation block size.
    MlePtrDictEntry *m_unused;	// Pointer to free list.
	MlePtrDictEntry *m_link;	// Pointer to first block.
};

/**
 * This is a dictionary class that maps pointers to pointers.
 *
 * This is a dictionary class that maps pointers to pointers.
 * In this base class, the key pointer is used numerically as the
 * input to a hashing function, and the match test is pointer
 * equality.  Subclasses can override these protected member
 * functions to do other mappings, such as required for string keys.
 */
class MLE_RUNTIME_API MlePtrDict
{
  public:

	//   Both constructors require a hashsize argument, which
    //   is the size of the hash table the dictionary will use.
    //   The constructors differ in how they set their allocation
    //   pool.  The pool can be created in the constructor by
    //   passing in the initial size, or an existing pool can
    //   be used.
    MlePtrDict(int hashsize,int poolsize);

    MlePtrDict(int hashsize,MlePtrDictPool *pool);

    ~MlePtrDict();
    
    // Dictionary operations.
    //   find() takes the key and returns the corresponding
    //   value pointer.  NULL is returned if the key is not
    //   found (note that this is ambiguous if NULL is a legal
    //   value).
    //   set() associates a value with a key.  This adds a
    //   new dictionary entry if there is no existing one for
    //   the specified key, and simply changes the value if
    //   the entries does already exist.
    //   remove() removes the dictionary entry associated with
    //   the specified key.
    void *find(const void *key) const;

    void set(const void *key,const void *value);

    void remove(const void *key);
    
    // Pool access.
    //   getPool() returns a pointer to the pool this dictionary
    //   uses for allocation.  This may be used to create
    //   another dictionary that will share the same allocation
    //   pool
    MlePtrDictPool *getPool(void) const;
    
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
    MlePtrDictEntry **m_bucket;
    
    // Virtual key functions.
    //   hash() takes the key and maps it into the range [1..hashsize).
    //   for distributing entries in the hash table.  In the
    //   implementation in this class, the hashing function takes
    //   the numerical value of the pointer and executes a multiply
    //   and modulo operation.
    //   match() determines the equality of two keys, returning
    //   nonzero if the keys match.  In the implementation in this
    //   class this is simply pointer equality.
    virtual unsigned int hash(const void *key) const;
	virtual int match(const void *key0, const void *key1) const;

	/** A pointer to the allocation pool. */
	MlePtrDictPool *m_pool;
};

#endif /* __MLE_PTRDICT_H_ */

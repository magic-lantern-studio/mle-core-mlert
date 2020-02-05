/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MlePtrArray.h
 * @ingroup MleFoundation
 *
 * @author Mark S. Millard
 * @created May 5, 2003
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

#ifndef __MLE_PTRARRAY_H_
#define __MLE_PTRARRAY_H_

#ifdef __linux__
#include <stdlib.h>
#endif

#include "mle/MleRuntime.h"

/**
 * MlePtrArray is a simple pointer array class.
 *
 * This is a simple pointer array class.
 * The initial motivation for this functionality was to provide a
 * slightly more memory efficient alternative to the simple pointer
 * list, so it is not very concerned with keeping indices constant.
 * It can be used (with care) for other applications.  This class
 * works by reallocation, so be aware of the consequences.
 */
class MLE_RUNTIME_API MlePtrArray
{
  // Declare member variables.

  protected:

    int m_n;		    // number of elements in the array
    int m_allocSize;	// number of elements allocated ( must be >= n )
	void **m_array;		// pointer to elements

  // Declare member functions.

  public:

    /**
     * @brief A constructor.
     *
     * @param initSize The initial allocation size of the array is passed to
     * the constructor.
     */
    MlePtrArray(int initSize);

    /**
     * @brief The destructor.
     */
    ~MlePtrArray();
    
    /**
     * @brief Get the size of the array.
     *
     * @return Returns the number of elements in the array.
     */
    int getSize(void) const;
    
    /**
     * @brief The array operator allows use of the natural syntax to
     * access array elements.
     */
    void *& operator[](int index) { return m_array[index]; }

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

    // Size management methods.

    /**
     * Adds a new value to the array.
     * 
     * It happens to append, though that is a product of implementation.
     *
     * @param ptr The element to add to the array.
     */
    void add(void *ptr);

    /**
     * @brief Removes the element with the pointer value
     * argument.
     *
     * @param ptr The element to remove from the array.
     */
    void remove(void *ptr);
};


#ifdef TEMPLATE
/**
 * @brief MleTPtrArray is a template that can be used to create type-safe
 * subclasses to the pointer array.
 *
 * This is a template that can be used to create type-safe subclasses
 * to the pointer array.  A pointer (not a type) is the template
 * argument.  This template shouldn't generate any additional machine
 * instructions.
 */
//template<class T> class MLE_RUNTIME_API MleTPtrArray : public MlePtrArray
template<class T> class MleTPtrArray : public MlePtrArray
{

  public:

    //////////
    MleTPtrArray<T>(int initSize) : MlePtrArray(initSize) {}
    
    //////////
    T& operator[](int index) { return *(T *)&m_array[index]; }

    //////////
    void add(T ptr) { MlePtrArray::add(ptr); }

    //////////
    void remove(T ptr) { MlePtrArray::remove(ptr); }
};
#endif /* TEMPLATE */

#endif /* __MLE_PTRARRAY_H_ */


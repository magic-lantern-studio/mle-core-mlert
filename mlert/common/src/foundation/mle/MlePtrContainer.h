/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MlePtrContainer.h
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
//  For information concerning this header file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

#ifndef __MLE_PTRCONTAINER_H_
#define __MLE_PTRCONTAINER_H_

#ifndef NULL
#define NULL 0
#endif /* NULL */

#include "mle/MleRuntime.h"

// Define the TEMPLATE macro so that MlePtrArray exposes its template 
// parent class.

#define TEMPLATE
#include "mle/MlePtrArray.h"

#ifdef MLE_DIGITAL_WORKPRINT
#if defined(__linux__) || defined(__APPLE__)
#include <string.h>
#endif
#include <mle/DwpStrKeyDict.h>
#endif /* MLE_DIGITAL_WORKPRINT */

/**
 * @brief MlePtrContainer is a template container class that serves as the
 * base class for scenes and groups.
 *
 * This container holds an array of pointers to objects of generic type.
 * The intended use of this template is to create a subclass container with
 * a specified item type.  This template provides inline wrappers for the
 * void* pointer manipulations of the MlePtrArray, so that you can localize
 * the type-specific API in one place, yet still reuse MlePtrArray.
 *
 * Note that this container holds item pointers of type ITEM, such as 
 * MleActor* and MleGroup*.
 */
//template <class ITEM> class MLE_RUNTIME_API MlePtrContainer : public MleTPtrArray<ITEM>
template <class ITEM> class MlePtrContainer : public MleTPtrArray<ITEM>
{
  // Declare member functions.

  public:

    /**
     * @brief Constructor of default array size eight (8).
     */
    MlePtrContainer( void ) : MleTPtrArray<ITEM>(8)
    {};


    /**
     * @brief Constructor of user-specified array size.
     *
     * @param initSize The initial size of the array.
     */
    MlePtrContainer(int initSize) : MleTPtrArray<ITEM>(initSize)
    {};


    /**
     * @brief The destructor.
     */
    virtual ~MlePtrContainer( void )
    {};
  

    // ------------------------------------------------------------------
    // Override the void*-based methods from MlePtrArray with
    // methods that use our template type.
    //
    //  ITEM& operator[](int index) { return (ITEM)array[index]; }
    //  void add(ITEM ptr) { ((MlePtrArray*)this)->add((void*)ptr); }
    //  void remove(ITEM ptr) { ((MlePtrArray*)this)->remove((void*)ptr); }
    // ------------------------------------------------------------------


    /**
	 * @brief Clear the specified element from the array.
	 *
     * This function removes an element from the array without compressing
     * out the empty location.  See remove().  We must provide clear() so
     * that the dictionary lookup in find() continues to work after a pointer
     * is dropped from the initial list.
	 *
	 * @param index The element to clear.
     */
    void clear( const int index )
    { MlePtrArray::m_array[index] = NULL; }


    /**
     * @brief Clear by pointer value.
	 *
	 * Search through the array and clear the pointer if found.
	 *
	 * @param ptr The pointer to clear.
     */
    void clear(ITEM ptr)
    {
        for ( int i = 0; i < MlePtrArray::m_n; i++ ) {
            if ( MlePtrArray::m_array[i] == ptr ) {
                clear(i);
                return;
            }
        }
    }


    /**
	 * @brief Initialize the array.
	 *
     * This function is a hook to do any initialization
     * Typically, the scene or group may schedule itself here.
     */
    virtual void init(void) = 0;


    /**
	 * @brief Find the element in the container.
	 *
     * @param pointer Given the pointer, find its index in the container.
	 *
	 * @return The index of the element is returned.
     */
    int find( const ITEM ptr )
    { 
        for ( int i = 0; i < MlePtrArray::getSize(); i++ ) {
            if ( MlePtrArray::m_array[i] == ptr ) {
                return i;
            }
        }
        return -1;
    }

#ifdef MLE_DIGITAL_WORKPRINT

	/**
     * @brief Get the container's name.
	 *
     * The tools need this.  The user code can instead use the
     * macro mle\<container-type\>RefTo(), eg mlSceneRefTo(), which
     * is valid at both rehearsal as a character string and at run time
     * as an integer.
	 *
	 * @return The name of the container is returned.
     */
    char* getName( void ) { return m_name; }

	/**
	 * @brief Set the container's name.
	 *
	 * @param nm The name of the container to set.
	 */
    void setName( char * nm )
	{ 
	    if (m_name) {
		    delete m_name; 
		}
#ifdef WIN32
	    m_name = (nm) ? _strdup(nm) : NULL;
#else
	    m_name = (nm) ? strdup(nm) : NULL;
#endif
	}

    // How to locate a group loaded in a scene from the DWP,
    // where the group's name is known at compile time and so can be
    // converted to string at rehearsal or int at runtime.
    // Note that these lookups are only valid until the group is removed,
    // but are never valid for groups that the programmer adds to the scene.
    ITEM find( const char *id )
	{ 
	    long index = -1 + (long) m_dictionary.find(id);
	    return (index >= 0) ? (*this)[index] : NULL;
	}

    // How to find groups in a scene at rehearsal time.
    // The dictionary stores pairs of (string, 1+index) of the ITEM within
    // its container. Index 0 is used to represent missing ITEM in 
    // the dict() return value.
    MleDwpStrKeyDict m_dictionary;

    // Sub-class must define these methods in order to have workprint
    // loading.  XXX
    //	  virtual MleScene*	load( MleDwpScene* wpScene );	
    //	  virtual MleScene*	load(const char* id);

#else /* MLE_DIGITAL_WORKPRINT */

    /**
	 * @brief Find the element in the container.
	 *
     * At runtime the MlePPGroup_* macro converts to int for us
     * so we can lookup the element easily.
     */
    ITEM find( const int id )
    { return (*this)[id]; }

    // Sub-class must define this method in order to support playprint
    // loading.  XXX
	//	   virtual MleScene* load( const int indexToPpTOC );

#endif /* MLE_REHEARSAL */

  protected:

#ifdef MLE_DIGITAL_WORKPRINT

    // Instance name of a scene.
    char* m_name;

#endif /* MLE_DIGITAL_WORKPRINT */

};

#undef TEMPLATE

#endif /* __MLE_PTRCONTAINER_H_ */

/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MlePtrArray.cxx
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

// Include system header files
#include <stdio.h>

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"
#include "mle/MlePtrArray.h"


// The array allocates the initial number of elements passed in
// as an argument to the constructor.
MlePtrArray::MlePtrArray(int initSize)
{
    m_n = 0;
    m_allocSize = initSize;
    m_array = (void **)mlMalloc(m_allocSize*sizeof(void *));
}


MlePtrArray::~MlePtrArray()
{
    /* deallocate memory */
    mlFree(m_array);
}


// This function returns the current number of elements in the array.
int
MlePtrArray::getSize(void) const
{
    return m_n;
}


// This function adds a new element to the array.
void
MlePtrArray::add(void *ptr)
{
    if ( m_n == m_allocSize )
    {
        m_allocSize *= 2;
        m_array = (void **)mlRealloc(m_array,m_allocSize*sizeof(void *));
    }

    m_array[m_n++] = ptr;
}


// This function removes an element from the array.
void
MlePtrArray::remove(void *ptr)
{
    for ( int i = 0; i < m_n; i++ )
    {
        if ( m_array[i] == ptr )
        {
            for ( int j = i + 1; j < m_n; j++ )
                m_array[j - 1] = m_array[j];
            
            m_n--;
            return;
        }
    }
}


void *
MlePtrArray::operator new(size_t tSize)
{
	void *p = mlMalloc(tSize);
	return p;
}


void
MlePtrArray::operator delete(void *p)
{
	mlFree(p);
}

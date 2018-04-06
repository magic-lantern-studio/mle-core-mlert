/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MleMediaRefConverter.h
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

#ifndef __MLE_MEDIAREFCONVERTER_H_
#define __MLE_MEDIAREFCONVERTER_H_


// Include Magic Lantern header files.
#include "mle/mlTypes.h"

// Include Magic Lantern Runtime Engine header files.
#include "mle/MleRuntime.h"


/**
 * @brief MleMediaRefConverter is the base class for media reference converter.
 *
 * Base class for media reference converter.  Gives a local file name
 * from the media reference, which may be a file, URL, or some other
 * type of identifier.  
 * This base class treats the reference as a local file name.
 *
 * @see MleMediaRef
 */
class MLE_RUNTIME_API MleMediaRefConverter
{
  // Declare member functions.

  protected:
	  /**
	   * The media reference represented as a local file.
	   */
      char *m_reference;


  // Declare member variables.

  public:
    
	/**
	 * Default constructor.
	 */
    MleMediaRefConverter()
	{ m_reference = NULL; };

	/**
	 * The destructor.
	 */
	virtual ~MleMediaRefConverter()
	{
		// Not responsible for freeing media reference.
	}


    /**
	 * @brief Set the media reference for this converter.
	 */
    virtual void setReference(char *reference)
	{ m_reference = reference; };


    /**
	 * @brief Get the media reference for this converter.
     */
    char* getReference(void) const
	{ return m_reference; };


    /**
	 * @brief Get the filename of the media reference associated for this
	 * converter.
	 */
    virtual char* getFilename(void) const
	{ return m_reference; };


    /**
	 * @brief Get the file of the media reference associated for this
	 * converter.
	 */
    virtual MlBoolean getFile(void)
	{ return TRUE; };

};


#endif /* __MLE_MEDIAREFCONVERTER_H_ */

/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MleMediaRef.h
 * @ingroup MleFoundation
 *
 * @author Mark S. Millard
 * @created May 1, 2003
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015-2018 Wizzer Works
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

#ifndef __MLE_MEDIAREF_H_
#define __MLE_MEDIAREF_H_

#ifdef MLE_DIGITAL_WORKPRINT
// Include system header files.
#include <string.h>	// Used for isa().

// Include DWP header files.
#include "mle/DwpStrKeyDict.h"

#include "mle/MleRuntime.h"

// Include Rehearsal Player/Tools header files.
#include "mle/MleMediaRefClass.h"
#endif /* MLE_DIGITAL_WORKPRINT */

// Header file for reference conversion from file (override with
// subclass to get URL conversion).
#include "mle/MleMediaRefConverter.h"

#ifdef MLE_DIGITAL_WORKPRINT

// This macro should be placed in the class declaration of any subclass
// of MleMediaRef.  It defines some basic functions needed by every subclass.
// The single argument is the unquoted class name.  Be aware that
// this macro switches subsequent declarations to public.
#define MLE_MEDIAREF_HEADER(C) \
  public: \
    virtual const char *getTypeName(void) const; \
    virtual int isa(const char *type) const; \
    static void initClass(void)

// This macro should be placed in the implementation of any subclass of
// MleMediaRef.  It defines some basic functionality needed by every subclass.
// The single argument is the unquoted class name.
#define MLE_MEDIAREF_SOURCE(C,S) \
    const char *C::getTypeName(void) const { return #C; } \
    MleMediaRef *_mlCreate##C(void); MleMediaRef *_mlCreate##C(void) { return new C; } \
    int C::isa(const char *type) const { \
        if ( !strcmp(type,#C) ) return 1; \
        else return S::isa(type); \
    } \
    void C::initClass(void) { \
	static int _mlMediaRefClassInited = 0; \
        if ( _mlMediaRefClassInited ) return ; \
	_mlMediaRefClassInited = 1; \
	S::initClass(); \
	MleMediaRefClass::add(#C,_mlCreate##C); \
    }

#endif /* MLE_DIGITAL_WORKPRINT */
#ifdef MLE_DIGITAL_PLAYPRINT

// Make a dummy statement to avoid errors with the trailing semicolon.
#define MLE_MEDIAREF_HEADER(C) \
  public: \
      friend class _mleDummy

#define MLE_MEDIAREF_SOURCE(C,S) \
    MleMediaRef *_mlCreate##C(void); MleMediaRef *_mlCreate##C(void) { return new C; }

#endif /* MLE_DIGITAL_PLAYPRINT */


typedef struct MLE_RUNTIME_API _MleMediaLoadReference
{
    unsigned long flags;
    unsigned long bufsiz;
    char *buffer;
    struct _MleMediaLoadReference *next;
} MleMediaLoadReference;


/**
 * @brief MleMediaRef is the base class for Media References.
 *
 * @see MleReferenceConverter
 */
class MLE_RUNTIME_API MleMediaRef
{

  // Declare member variables.

  protected:

    // Cached media data from DWP/Playprint.
    MleMediaLoadReference *m_references;
    unsigned long m_numReferences;

    // The reference converter to change buffer into a local file name
    // Make this a pointer so that a subclass can override it. 
    MleMediaRefConverter *m_converter;

  // Declare member functions.

  public:

    /**
     * @brief Default constructor.
	 */
    MleMediaRef(void);

    /**
     * @brief Destructor.
	 */
    virtual ~MleMediaRef(void);
    
    /**
	 * @brief Initialize the media reference.
	 *
     * This function is a hook to do any initialization.
     * Typically, the media ref may initialize itself here.
     * The base class implementation does nothing.
	 */
    virtual void init(void) {};
    
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

    /**
	 * @brief Load the media.
	 *
     * This function is used to load the media data from either the
     * Digital Workprint or the runtime Digital Playprint.
     *
	 * @param numEntries The number of number entries to load.
	 * @param usrData A pointer to user data to use upon loading.
	 */
    virtual MlBoolean load(unsigned long numEntries,void *usrData);

    /**
	 * @brief Unload the media.
	 *
     * This function is used to unload the media data from either the
     * Digital Workprint or the runtime Digital Playprint.
	 */
    virtual void unload(void);

    /**
     * @brief Get the next media reference relative to the specifed
	 * load reference.
	 */
    MleMediaLoadReference* getNextMediaRef(MleMediaLoadReference* loadReference);

    /**
	 * @brief Get the media reference flags for the specified load
	 * reference.
	 */
    unsigned long getMediaRefFlags(MleMediaLoadReference* loadReference);

    /**
	 * @brief Get the media reference buffer size for the specified load
	 * reference.
	 */
    unsigned long getMediaRefBufsiz(MleMediaLoadReference* loadReference);

    /**
	 * @brief Get the media reference buffer for the specified load
	 * reference.
	 */
    char *getMediaRefBuffer(MleMediaLoadReference* loadReference);

    /**
     * @brief Set the reference converter.
	 *
	 * @param converter A pointer to the media reference converter to
	 * set for this media reference.
	 */
    void setConverter(MleMediaRefConverter* converter);

    /**
	 * @brief Get the reference converter.
	 */
    MleMediaRefConverter* getConverter(void);

    /**
	 * @brief Delete the reference converter associated with this
	 * media reference.
	 */
    void deleteConverter(void);

#ifdef MLE_DIGITAL_WORKPRINT
  public:
    // Media reference identity.
    // getTypeName() returns the class name of a media ref instance.
    virtual const char *getTypeName(void) const;
	// isa() returns nonzero if the instance is of the specified class.
    virtual int isa(const char *type) const;
    
    // This is the registry for media refs at rehearsal.  When they
    // are created, they are entered into the registry with a
    // name so that group loads can reference them.
    static MleDwpStrKeyDict g_registry;
    
    static void initClass(void) {}
    
    // This constant is reserved for accessing the rehearsal target
    // information from the Digital Workprint.
    static const char *g_rehearsal;
#endif /* MLE_DIGITAL_WORKPRINT */
};

#ifdef MLE_DIGITAL_WORKPRINT

typedef char* MlMediaRef;

// Macro constant
//
// This constant is used to designate a NULL value
// of type MlMediaRef.

#define MLE_NO_MEDIA NULL

#endif /* MLE_DIGITAL_WORKPRINT */
#ifdef MLE_DIGITAL_PLAYPRINT

/* Mastering (these definitions will be used for all target machines) */

typedef signed long MlMediaRef;
#define MLE_NO_MEDIA -1

#endif /* MLE_DIGITAL_WORKPRINT */


#endif /* __MLE_MEDIAREF_H_ */

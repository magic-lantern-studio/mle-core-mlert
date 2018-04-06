/** @defgroup MleRteWin32 Magic Lantern Runtime Engine Win32 Library API */

/**
 * @file palette.h
 * @ingroup MleRteWin32
 *
 * Class definition of the Magic Lantern Palette object, MlePalette.
 *
 * @author Mark S. Millard
 * @date May 1, 2003
 */

// COPYRIGHT_BEGIN
//
//  Copyright (C) 2000-2007  Wizzer Works
//
//  Wizzer Works makes available all content in this file ("Content").
//  Unless otherwise indicated below, the Content is provided to you
//  under the terms and conditions of the Common Public License Version 1.0
//  ("CPL"). A copy of the CPL is available at
//
//      http://opensource.org/licenses/cpl1.0.php
//
//  For purposes of the CPL, "Program" will mean the Content.
//
//  For information concerning this Makefile, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

#ifndef __MLE_PALETTE_H_
#define __MLE_PALETTE_H_

// Include system header files.
#include <windows.h>

// Include Magic Lantern header files.
#include "mle/mlTypes.h"


// Designate some flags that might be useful for palettes.  Mostly
// useful for debugging.

// MLE_PALETTE_REGISTERED bit will be turned on only when the palette 
// has been added to the palette manager, and will be turned off 
// when it is deleted from the palette manager.  
const int MLE_PALETTE_REGISTERED = 0x0001;

// MLE_PALETTE_NO_CHANGE indicates that the palette should not be modified.  
const int MLE_PALETTE_NO_CHANGE  = 0x0002;

// MLE_PALETTE_MODIFIED should be set whenever the palette has been modified.
const int MLE_PALETTE_MODIFIED   = 0x0004;

const int MLE_PALETTE_FLAGS = MLE_PALETTE_REGISTERED|MLE_PALETTE_NO_CHANGE|MLE_PALETTE_MODIFIED;


// Define a type for color palette indices.
typedef unsigned char MlPaletteIndex;


/**
 * @brief This is an abstract base class for a Magic Lantern Windows
 * color palette.
 *
 * This is the abstract base class for all Magic Lantern Windows
 * color palette classes.  It contains no data other than a flags field,
 * since palette data is platform-specific.
 *
 * see MleWinPalette, MleWin32Palette, MleGdkPalette
 */
class MlePalette
{
  public:

    /**
	 * The default constructor.
	 */
    MlePalette(void) { m_flags = 0; }
    
    /**
	 * The destructor.
	 */
    virtual ~MlePalette(void) {}
    
    /**
	 * Get the palette flags.
	 *
	 * The palette flags are returned.
	 */
    virtual unsigned int getFlags(void)
	{ return m_flags; }

	/**
	 * Set the palette flags.
	 *
	 * @param flags The flags to set.
	 *
	 * @returns <b>true</b> is returned if the flags are successfully set.
	 * Otherwise <b>false</b> will be returned.
	 */
    virtual MlBoolean setFlags(unsigned int flags);
    
  protected:

	/**
	 * The flags field may be used for platform-specific or non-specific
	 * purposes. It is currently unused.
	 */
    unsigned int m_flags;
    
    // Note there is no palette data here because that is stage-specific
    // and must be defined in the subclasses.
};


#endif /* __MLE_PALETTE_H_ */

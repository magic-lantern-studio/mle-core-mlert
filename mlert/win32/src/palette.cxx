/** @defgroup MleRteWin32 Magic Lantern Runtime Engine Win32 Library API */

/**
 * @file palette.cxx
 * @ingroup MleRteWin32
 *
 * Class implementation of MlePalette class.
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


// Include Magic Lantern header files.
#include "mle/mlAssert.h"

// Include Win32 target header files.
#include "mle/palette.h"

// MlePalette class - implementation of color palette base class.

MlBoolean MlePalette::setFlags(unsigned int flags)
{
	// If debugging, validate new flags.
	MLE_ASSERT((flags & ~MLE_PALETTE_FLAGS) != 0);

	m_flags = flags;
	return TRUE;
}


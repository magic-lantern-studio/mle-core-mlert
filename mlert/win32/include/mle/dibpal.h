/** @defgroup MleRteWin32 Magic Lantern Runtime Engine Win32 Library API */

/**
 * @file dibpal.h
 * @ingroup MleRteWin32
 *
 * This file contains the definition of the  MleDIBPalette class,
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

#ifndef __MLE_DIBPAL_H_
#define __MLE_DIBPAL_H_

// Include Win32 target header files
#include "mle/dib.h"


class MleDIBPalette
{
    // Declare member variables.

  private:

    HPALETTE m_palette;
    int m_numColors;
    
    // Declare member functions.

  public:

    MleDIBPalette(void);

    virtual ~MleDIBPalette(void);
    
    // Method to create color palette from a DIB.
    HPALETTE create(MleDIB *dib);
    
    // Method to create color palette from an array of palette entries.
    HPALETTE create(PALETTEENTRY *pe,DWORD n);
    
    // Method to get the number of colors in this palette.
    int getNumColorEntries(void)
    { return(m_numColors); }
    
    // Method to get the handle to the palette.
    HPALETTE getPalette(void)
    { return(m_palette); }

    // Method to set system palette colors.
    BOOL setSystemColors(void);
};

#endif /* __MLE_DIBPAL_H_ */

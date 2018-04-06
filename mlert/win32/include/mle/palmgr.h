/** @defgroup MleRteWin32 Magic Lantern Runtime Engine Win32 Library API */

/**
 * @file palmgr.h
 * @ingroup MleRteWin32
 *
 * Class definition of the Magic Lantern Palette Manager object,
 * MlePaletteMgr.
 *
 * The MlePaletteMgr contains an array of registered MlePalettes, 
 * which may be used to set the Windows application palette for the
 * title.
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

#ifndef __MLE_PALMGR_H_
#define __MLE_PALMGR_H_

// Include system header files.
#include <windows.h>

// Include Magic Lantern header files.
#include "mle/mlTypes.h"

// Include Win32 target hader files.
#include "mle/palette.h"


// A type definition for palette identifiers.
typedef int MlPaletteId;

#define MLE_PALETTE_ID_INVALID -1


// Palette manager class
class MlePaletteMgr
{
  public:

    MlePaletteMgr(int numPalettes);

    ~MlePaletteMgr();
    
    // Add or delete a palette from the registry.
    MlPaletteId add(MlePalette *newPalette);
    MlePalette *remove(MlPaletteId id);
    
    // Get or set a registered palette's data.  Will fail
    // if the specified palette has not been registered.
    MlePalette *get(MlPaletteId id);
    MlBoolean set(MlPaletteId id, MlePalette *newPalette);
    
    // Select a registered palette for display.
    //MlBoolean setCurrent(MlPaletteId id);
    MlPaletteId getCurrent(void) { return m_curPalette; }
    
    // Get number of palettes allocated.  Might be used to
    // delete all loaded palettes during a scene change.
    unsigned int getNumPalettes(void) { return m_numPalettes; }
    
  private:

    MlPaletteId m_curPalette;     // Currently selected palette.
    unsigned int m_numPalettes;   // Number allocated, not registered.
    MlePalette **m_paletteArray;  // Pointer to array of palette pointers.
};

// XXX - this should move into the stage.
extern MlePaletteMgr *g_paletteManager;

#endif /*__MLE_PALMGR_H_*/

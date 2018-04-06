/** @defgroup MleRteWin32 Magic Lantern Runtime Engine Win32 Library API */

/**
 * @file palmgr.cxx
 * @ingroup MleRteWin32
 *
 * Class implementation of MlePaletteMgr class.
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
#include "mle/mlMalloc.h"
#include "mle/mlAssert.h"

// Include Win32 target header files.
#include "mle/palmgr.h"


MlePaletteMgr *g_paletteManager = NULL;


MlePaletteMgr::MlePaletteMgr(int numPalettes)
{
    m_paletteArray = (MlePalette **)mlMalloc(numPalettes*sizeof(MlePalette *));
	MLE_ASSERT(m_paletteArray != NULL);

    m_curPalette = -1;	// No palette has been set yet.
    m_numPalettes = numPalettes;
}


MlePaletteMgr::~MlePaletteMgr()
{
	// XXX - Do I want to test whether there's anything in it?
	mlFree(m_paletteArray);
}


// Add a palette from the registry.
MlPaletteId MlePaletteMgr::add(MlePalette *newPalette)
{
	unsigned int i;
	unsigned int flags;

	// Find first empty slot.
	for (i = 0; i < m_numPalettes; i++)
	{
		if (!m_paletteArray[i])
			break;
	}

	// Test if array is full.
	if (i == m_numPalettes)
		return(MLE_PALETTE_ID_INVALID);

	// Add new palette to array.
    m_paletteArray[i] = newPalette;
	flags = newPalette->getFlags();
	flags |= MLE_PALETTE_REGISTERED;
	newPalette->setFlags(flags);
	return(i);
}


// Remove a palette from the registry.
MlePalette *MlePaletteMgr::remove(MlPaletteId id)
{
	MlePalette *entry;
    unsigned int flags;

	// Grab the data from this entry for return.
	entry = m_paletteArray[id];
    flags = entry->getFlags();
	flags &= ~MLE_PALETTE_REGISTERED;
    entry->setFlags(flags);

	// Clear out the entry.
    m_paletteArray[id] = NULL;
	return entry;
}


// Get or set a registered palette's data.  Will fail
// if the specified palette has not been registered.
MlePalette *MlePaletteMgr::get(MlPaletteId id)
{
	// Can't set if no palette was previously registered.
	if (!m_paletteArray[id])
		return NULL;

    return m_paletteArray[id];
}


MlBoolean MlePaletteMgr::set(MlPaletteId id, MlePalette *newPalette)
{
    // Can't set if no palette was previously registered.
    if (!m_paletteArray[id])
        return FALSE;

    m_paletteArray[id] = newPalette;
    return TRUE;
}


#if 0
// Select a registered palette for display.
MlBoolean MlePaletteMgr::setCurrent(MlPaletteId id)
{
	HDC screenDc;

	// Make sure there is a palette with this id.
    if (!m_paletteArray[id])
        return NULL;

	// XXX - this may be obsolete.
	MLE_ASSERT(!m_paletteArray[id]->getHandle());

	// Set the application palette.
	screenDc = GetDC(NULL);
	SelectPalette(screenDc, m_paletteArray[id]->getHandle(), FALSE);
	RealizePalette(screenDc);
	ReleaseDC(NULL, screenDc);

	return TRUE;
}
#endif /* 0 */

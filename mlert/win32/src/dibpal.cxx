/** @defgroup MleRteWin32 Magic Lantern Runtime Engine Win32 Library API */

/**
 * @file dibpal.cxx
 * @ingroup MleRteWin32
 *
 * Class implementation of MleDIBPalette class.
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

// Include system header files.
#if defined(MLE_DEBUG)
#include <stdlib.h>
#endif /* MLE_DEBUG */

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"
#include "mle/mlAssert.h"

// Include Win32 target header files.
#include "mle/dibpal.h"


MleDIBPalette::MleDIBPalette()
{
    // Do nothing.
}


MleDIBPalette::~MleDIBPalette()
{
    // Do nothing.
}


// Create a palette from the color table in a DIB.
HPALETTE MleDIBPalette::create(MleDIB *dib)
{
    DWORD colors = dib->getNumColorEntries();

    // Check the DIB has a color table.
    if (! colors)
	{
#if defined(MLE_DEBUG)
        fprintf(stderr,"MleDIBPalette: No color table.");
#endif /* MLE_DEBUG */
        return(FALSE);
    }

    // Get a pointer to the RGB quads in the DIB's color table.
    RGBQUAD *colorTable = dib->getColorTable();

    // Allocate a logical palette and fill it with the color table info.
    LOGPALETTE *logpal = (LOGPALETTE*) mlMalloc(sizeof(LOGPALETTE) 
        + colors * sizeof(PALETTEENTRY));
    if (! logpal)
	{
#if defined(MLE_DEBUG)
        fprintf(stderr,"MleDIBPalette: Out of memory for logical palette.");
#endif /* MLE_DEBUG */
        return FALSE;
    }

    logpal->palVersion = 0x300;              // Windows 3.0
    logpal->palNumEntries = (WORD) colors;   // Table size
    for (DWORD dw = 0; dw < colors; dw++)
	{
        logpal->palPalEntry[dw].peRed = colorTable[dw].rgbRed;
        logpal->palPalEntry[dw].peGreen = colorTable[dw].rgbGreen;
        logpal->palPalEntry[dw].peBlue = colorTable[dw].rgbBlue;
        logpal->palPalEntry[dw].peFlags = 0;
    }

    // Create color palette.
    m_palette = CreatePalette(logpal);
    m_numColors = colors;

    // Cleanup and return.
    mlFree(logpal);
    return(m_palette);
}


// Create a palette from an array of palette entries.
HPALETTE MleDIBPalette::create(PALETTEENTRY *pe,DWORD colors)
{
    // Check the DIB has a color table.
    if (! colors)
	{
#if defined(MLE_DEBUG)
        fprintf(stderr,"MleDIBPalette: No color table.");
#endif /* MLE_DEBUG */
        return(NULL);
    }

    // Allocate a logical palette and fill it with the color table info.
    LOGPALETTE *logpal = (LOGPALETTE*) mlMalloc(sizeof(LOGPALETTE) 
        + colors * sizeof(PALETTEENTRY));
    if (! logpal)
	{
#if defined(MLE_DEBUG)
        fprintf(stderr,"MleDIBPalette: Out of memory for logical palette.");
#endif /* MLE_DEBUG */
        return(NULL);
    }

    logpal->palVersion = 0x300;              // Windows 3.0
    logpal->palNumEntries = (WORD) colors;   // Table size
    for (DWORD dw = 0; dw < colors; dw++)
	{
        logpal->palPalEntry[dw].peRed = pe[dw].peRed;
        logpal->palPalEntry[dw].peGreen = pe[dw].peGreen;
        logpal->palPalEntry[dw].peBlue = pe[dw].peBlue;
        logpal->palPalEntry[dw].peFlags = 0;
    }

    // Create color palette.
    m_palette = CreatePalette(logpal);
    m_numColors = colors;

    // Cleanup and return.
    mlFree(logpal);
    return(m_palette);
}


BOOL MleDIBPalette::setSystemColors()
{
    // Declare local variables.
    BOOL result = FALSE;
    int i,sysColors,palEntries;
    HPALETTE palOld;

    // Get a screen DC to work with.
    HWND hwndActive = GetActiveWindow();
    HDC hdcScreen = GetDC(hwndActive);
    MLE_ASSERT(hdcScreen);

    // Make sure we are on a palettized device.
    if (! (GetDeviceCaps(hdcScreen,RASTERCAPS) & RC_PALETTE))
	{
#if defined(MLE_DEBUG)
        fprintf(stderr,"MleDIBPalette:: Not a palettized device.");
#endif /* MLE_DEBUG */
        goto abort;
    }

    // Get the number of system colors and the number of palette
    // entries. Note that on a palletized device the number of
    // colors is the number of guaranteed colors, i.e., the number
    // of reserved system colors.
    sysColors = GetDeviceCaps(hdcScreen,NUMCOLORS);
    palEntries = GetDeviceCaps(hdcScreen,SIZEPALETTE);

    // If there are more than 256 colors we are wasting our time.
    if (sysColors > 256) goto abort;

    // Now we force the palette manager to reset its tables so that
    // the next palette to be realized will get its colors in the order they
    // are in the logical palette. This is done by changing the number of
    // reserved colors.
    SetSystemPaletteUse(hdcScreen,SYSPAL_NOSTATIC);
    SetSystemPaletteUse(hdcScreen,SYSPAL_STATIC);

    // Select our palette into the screen DC and realize it so that
    // its colors will be entered into the free slots in the physical palette.
    palOld = SelectPalette(hdcScreen,
                           (HPALETTE) m_palette,
                           FALSE);
    RealizePalette(hdcScreen);

    // Now replace the old palette (but don't realize it).
    SelectPalette(hdcScreen,palOld,FALSE);

    // The physical palette now has our colors set in place and its own
    // reserved colors at either end. We can grab the lot now.
    PALETTEENTRY pe[256];
    GetSystemPaletteEntries(hdcScreen, 
                            0,
                            palEntries,
                            pe);

    // Set the PC_NOCOLLAPSE flag for each of our colors so that the GDI
    // won't merge them. Be careful not to set PC_NOCOLLAPSE for the 
    // system color entries so that we won't get multiple copies of these
    // colors in the palette when we realize it.
    for (i = 0; i < sysColors/2; i++)
	{
        pe[i].peFlags = 0;
    }
    for (; i < palEntries-sysColors/2; i++)
	{
        pe[i].peFlags = PC_NOCOLLAPSE;
    }
    for (; i < palEntries; i++)
	{
        pe[i].peFlags = 0;
    }

    // Resize the palette in case it was smaller.
    ResizePalette(m_palette,palEntries);

    // Update the palette entries with what is now in the physical palette.
    SetPaletteEntries(m_palette,0,palEntries,pe);
    result = TRUE;

abort:
    ReleaseDC(hwndActive,hdcScreen);
    return(result);
}

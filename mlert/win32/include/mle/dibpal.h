/** @defgroup MleRteWin32 Magic Lantern Runtime Engine Win32 Library API */

/**
 * @file dibpal.h
 * @ingroup MleRteWin32
 *
 * This file contains the definition of the  MleDIBPalette class,
 */

 // COPYRIGHT_BEGIN
 //
 // The MIT License (MIT)
 //
 // Copyright (c) 2000-2025 Wizzer Works
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

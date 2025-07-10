/** @defgroup MleRteWin32 Magic Lantern Runtime Engine Win32 Library API */

/**
 * @file dib.h
 * @ingroup MleRteWin32
 *
 * This file contains the definition of the MleDIB class.
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


#ifndef __MLE_DIB_H_
#define __MLE_DIB_H_

// dib.h : header file for MleDIB class

// Include system header files.
#include <windows.h>
#include <stdio.h>


class MleDIB
{
    // Declare member variables.

  protected:

    BITMAPINFO *m_bmInfo; // Pointer to BITMAPINFO struct.
    BYTE *m_bmData;       // Pointer to the image data (bits).
    BOOL m_ownData;       // TRUE if DIB owns image data memory.
    
    // Declare member functions.

  public:

    MleDIB(void);

    virtual ~MleDIB(void);
    
    // Method to get pointer to bitmap info.
    BITMAPINFO *getBitmapInfo(void)
    { return m_bmInfo; }
    
    // Method to get pointer to the image data.
    void *getImageData(void)
    { return m_bmData; }
    
    // Method to get pointer to color table.
    RGBQUAD *getColorTable(void)
	{ return (LPRGBQUAD)(((BYTE*)(m_bmInfo)) 
	      + sizeof(BITMAPINFOHEADER)); }
    
    // Method to get number of color table entries.
    int getNumColorEntries(void);
    
    // Method to create a new DIB.
    BOOL create(int width,int height);
    
    // Method to create DIB from existing memory buffer.
    BOOL create(BITMAPINFO *info,BYTE *buffer);
    
    // method to load DIB file.
    BOOL load(FILE *fp);
    
    // Method to return address of pixel in memory.
    void *getPixel(int x,int y);
    
    // Method to get image width.
    virtual int getWidth()
    { return dibWidth(); }
    
    // Method to get image height.
    virtual int getHeight()
    { return dibHeight(); }
    
    // Method to map DIB color table to specified palette.
    virtual BOOL mapColorsToPalette(HPALETTE pal);
    
    // Method to get extent of DIB.
    virtual void getRect(RECT *rect);
    
    // Method to copy bits into destination DIB
    virtual void copyBits(MleDIB *dest, 
                          int xd,int yd,
                          int w,int h,
                          int xs,int ys,
                          COLORREF transparency = 0xFFFFFFFF);
    
  private:

    // Method to get width of DIB
    int dibWidth(void)
    { return m_bmInfo->bmiHeader.biWidth; }
    
    // Method to get height of DIB
    int dibHeight(void) 
    { return m_bmInfo->bmiHeader.biHeight; }
    
    // Method to get width of DIB (DWORD aligned)
    int storageWidth(void)
    { return (m_bmInfo->bmiHeader.biWidth + 3) & ~3; }
};


#endif /* __MLE_DIB_H_ */

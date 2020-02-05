/** @defgroup MleRteWin32 Magic Lantern Runtime Engine Win32 Library API */

/**
 * @file dib.h
 * @ingroup MleRteWin32
 *
 * This file contains the definition of the MleDIB class.
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

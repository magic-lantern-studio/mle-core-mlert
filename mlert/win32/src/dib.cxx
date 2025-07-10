/** @defgroup MleRteWin32 Magic Lantern Runtime Engine Win32 Library API */

/**
 * @file dib.cxx
 * @ingroup MleRteWin32
 *
 * Class implementation of MleDIB class.
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


// Include system header files.
#if defined(MLE_DEBUG)
#include <stdlib.h>
#endif /* MLE_DEBUG */

// include Magic Lantern header files.
#include "mle/mlMalloc.h"
#include "mle/mlFileio.h"
#include "mle/mlAssert.h"

// Include Win32 Player header files.
#include "mle/dib.h"


// Create a small DIB here so member variables "m_bmInfo" and "m_bmData"
// are always valid.
MleDIB::MleDIB()
    :m_bmInfo(NULL),
     m_bmData(NULL),
     m_ownData(TRUE)
{
    create(16,16);
}


MleDIB::~MleDIB()
{
    // Free the memory.
    if (m_bmInfo != NULL) mlFree(m_bmInfo);
    if (m_ownData && (m_bmData != NULL)) mlFree(m_bmData);
}


static BOOL _isWinDIB(BITMAPINFOHEADER *header)
{
    MLE_ASSERT(header);

    if (((BITMAPCOREHEADER *)header)->bcSize == sizeof(BITMAPCOREHEADER))
	{
        return FALSE;
    }
    return(TRUE);
}


static int _numDIBColorEntries(BITMAPINFO *info) 
{
    // Declare local variables.
    BITMAPINFOHEADER *infoHeader;
    BITMAPCOREHEADER *coreHeader;
    int colors,bitCount;

    MLE_ASSERT(info);

    infoHeader = &(info->bmiHeader);
    coreHeader = (BITMAPCOREHEADER *) infoHeader;

    // Start off by assuming the color table size from
    // the bit-per-pixel field.
    if (_isWinDIB(infoHeader))
	{
        bitCount = infoHeader->biBitCount;
    } else
	{
        bitCount = coreHeader->bcBitCount;
    }

    switch (bitCount)
	{
        case 1:
            colors = 2;
            break;
        case 4:
            colors = 16;
            break;
        case 8:
            colors = 256;
            break;
        default:
            colors = 0;
            break;
    }

    // If this is a Windows DIB, then the color table length
    // is determined by the biClrUsed field if the value in
    // the field is nonzero.
    if (_isWinDIB(infoHeader) && (infoHeader->biClrUsed != 0))
	{
        colors = infoHeader->biClrUsed;
    }

    // Make sure the value is reasonable since some products
    // will write out more then 256 colors for an 8 bpp DIB!!!
    int maxColors = 0;
    switch (bitCount)
	{
        case 1:
            maxColors = 2;
            break;
        case 4:
            maxColors = 16;
            break;
        case 8:
            maxColors = 256;
            break;
        default:
            maxColors = 0;
            break;
    }
    if (maxColors)
	{
        if (colors > maxColors)
		{
#if defined(MLE_DEBUG)
            OutputDebugString("MleDIB: Invalid color count.");
#endif /* MLE_DEBUG */
            colors = maxColors;
        }
    }

    return(colors);
}


// Create a new empty 8bpp DIB with a 256 entry color table.
BOOL MleDIB::create(int width,int height)
{
    // Delete any existing stuff.
    if (m_bmInfo != NULL) mlFree(m_bmInfo);
    if (m_ownData && (m_bmData != NULL)) mlFree(m_bmData);

    // Allocate memory for the header.
    m_bmInfo = (BITMAPINFO*) mlMalloc(sizeof(BITMAPINFOHEADER)
        + 256 * sizeof(RGBQUAD));
    if (! m_bmInfo)
	{
#if defined(MLE_DEBUG)
        OutputDebugString("MleDIB: Out of memory for DIB header.");
        OutputDebugString(_strerror("Error Value"));
#endif /* MLE_DEBUG */
        return FALSE;
    }

    // Allocate memory for the bits (DWORD aligned).
    int bmDataSize = ((width + 3) & ~3) * height;
    m_bmData = (BYTE *) mlMalloc(bmDataSize);
    if (! m_bmData)
	{
#if defined(MLE_DEBUG)
        OutputDebugString("MleDIB: Out of memory for DIB image data.");
        OutputDebugString(_strerror("Error Value"));
#endif /* MLE_DEBUG */
        mlFree(m_bmInfo);
        m_bmInfo = NULL;
        return(FALSE);
    }
    m_ownData = TRUE;

    // Fill in the header info.
    BITMAPINFOHEADER *pInfo = (BITMAPINFOHEADER *) m_bmInfo;
    pInfo->biSize = sizeof(BITMAPINFOHEADER);
    pInfo->biWidth = width;
    pInfo->biHeight = height;
    pInfo->biPlanes = 1;
    pInfo->biBitCount = 8;
    pInfo->biCompression = BI_RGB;
    pInfo->biSizeImage = 0;
    pInfo->biXPelsPerMeter = 0;
    pInfo->biYPelsPerMeter = 0;
    pInfo->biClrUsed = 0;
    pInfo->biClrImportant = 0;

    // Create an arbitrary color table (gray scale).
    RGBQUAD *rgb = getColorTable();
    for (int i = 0; i < 256; i++)
	{
        rgb->rgbBlue = rgb->rgbGreen = rgb->rgbRed = (BYTE) i;
        rgb->rgbReserved = 0;
        rgb++;
    }

    // Set all the bits to a known state (black).
    memset(m_bmData,0,bmDataSize);

    return(TRUE);
}


// Create a MleDIB structure from existing header and bits. The DIB
// won't delete the bits and makes a copy of the header.
BOOL MleDIB::create(BITMAPINFO *info,BYTE *data)
{
    MLE_ASSERT(info);
    MLE_ASSERT(data);

    if (m_bmInfo != NULL) mlFree(m_bmInfo);
    m_bmInfo = (BITMAPINFO *) mlMalloc(sizeof(BITMAPINFOHEADER)
                                     + 256 * sizeof(RGBQUAD));
    MLE_ASSERT(m_bmInfo);

    // XXX - this will probably fail for < 256 color headers.
    memcpy(m_bmInfo,info,sizeof(BITMAPINFOHEADER)+
             _numDIBColorEntries(info) * sizeof(RGBQUAD));

    if (m_ownData && (m_bmData != NULL)) mlFree(m_bmData);
    m_bmData = data;
    m_ownData = FALSE; // We can't delete the bits.
    return(TRUE);
}


// Get the number of color table entries.
int MleDIB::getNumColorEntries()
{
    return _numDIBColorEntries(m_bmInfo);
}


// XXX- this assumes all MleDIB objects have 256 color table entries.
BOOL MleDIB::mapColorsToPalette(HPALETTE pal)
{
    if (! pal)
	{
#if defined(MLE_DEBUG)
        OutputDebugString("MleDIB: No palette to map to.");
#endif /* MLE_DEBUG */
        return FALSE;
    }

    MLE_ASSERT(m_bmInfo);
    MLE_ASSERT(m_bmInfo->bmiHeader.biBitCount == 8);
    MLE_ASSERT(m_bmData);
    LPRGBQUAD colorTable = getColorTable();
    MLE_ASSERT(colorTable);

    // Build an index translation table to map this DIBs colors
    // to those of the reference DIB.
    BYTE map[256];
#if defined(MLE_DEBUG)
    // For debugging only.
    int changed = 0;
#endif /* MLE_DEBUG */

	int i;
    for (i = 0; i < 256; i++)
	{
        map[i] = (BYTE) GetNearestPaletteIndex(pal,
            RGB(colorTable->rgbRed,
                colorTable->rgbGreen,
                colorTable->rgbBlue));
        colorTable++;

#if defined(MLE_DEBUG)
        // For debugging only.
        if (map[i] != i) changed++;
#endif /* MLE_DEBUG */
    }

    // Map the DIB bits to the palette.
    BYTE *bits = (BYTE *) getImageData();
    int size = storageWidth() * dibHeight();
    while (size--)
	{
        *bits = map[*bits];
        bits++;
    }

    // Reset the DIB color table so that its RGB values
    // match those in the palette.
    PALETTEENTRY pe[256];
    GetPaletteEntries(pal,0,256,pe);
    colorTable = getColorTable();
    for (i = 0; i < 256; i++)
	{
        colorTable->rgbRed = pe[i].peRed;    
        colorTable->rgbGreen = pe[i].peGreen;    
        colorTable->rgbBlue = pe[i].peBlue;
        colorTable++;    
    }

    // Mark all the colors in use.
    m_bmInfo->bmiHeader.biClrUsed = 256;
    return TRUE;
}


// Get a pointer to a pixel.
// NOTE: DIB scan lines are DWORD aligned. The scan line 
// storage width may be wider than the scan line image width
// so calculate the storage width by rounding the image width 
// to the next highest DWORD value.
void *MleDIB::getPixel(int x,int y)
{
    int width;

    // XXX - this version deals only with 8 bpp DIBs
    MLE_ASSERT(m_bmInfo->bmiHeader.biBitCount == 8);

    // Make sure it's in range and if it isn't, return zero.
    if ((x >= dibWidth()) || (y >= dibHeight()))
	{
#if defined(MLE_DEBUG)
        OutputDebugString("MleDIB: Attempt to get out of range pixel address.");
#endif /* MLE_DEBUG */
        return(NULL);
    }

    // Calculate the scan line storage width.
    width = storageWidth();
    return(m_bmData + (dibHeight()-y-1) * width + x);
}


// Get the bounding rectangle.
void MleDIB::getRect(RECT *rect)
{
    rect->top = 0;
    rect->left = 0;
    rect->bottom = dibHeight();
    rect->right = dibWidth();
}


// Copy a rectangle of the DIB to another DIB.
// XXX - this only support 8bpp DIBs here.
void MleDIB::copyBits(
    MleDIB *destDIB, 
    int xd,int yd,
    int w,int h,
    int xs,int ys,
    COLORREF transparency)
{
    MLE_ASSERT(m_bmInfo->bmiHeader.biBitCount == 8);
    MLE_ASSERT(destDIB);

    // Test for simple cases.
    if (w == 0 || h == 0)
        return;

    // Get pointers to the start points in the source and destination
    // DIBs. Note that the start points will be the bottom-left
    // corner of the DIBs because the scan lines are reversed in memory.
    BYTE *src = (BYTE *) getPixel(xs, ys + h - 1);
    MLE_ASSERT(src);
    BYTE *dest = (BYTE *) destDIB->getPixel(xd, yd + h - 1);
    MLE_ASSERT(dest);

    // Get the scan line widths of each DIB.
    int scanS = storageWidth();
    int scanD = destDIB->storageWidth();

    if (transparency == 0xFFFFFFFF)
	{
        // Copy the lines.
        while (h--)
		{
            memcpy(dest,src,w);
            src += scanS;
            dest += scanD;
        }
    } else
	{
        // Copy lines with transparency.
        // Note: We accept only a PALETTEINDEX description
        // for the color definition.
        MLE_ASSERT((transparency & 0xFF000000) == 0x01000000);
        BYTE transColor = LOBYTE(LOWORD(transparency));
        int sinc = scanS - w; // Source increment value.
        int dinc = scanD - w; // Destination increment value.
        int count;
        BYTE pixel;

        while (h--)
		{
            count = w; // Number of pixels to scan.
            while (count--)
			{
                pixel = *src++;
                // Copy pixel only if it isn't transparent.
                if (pixel != transColor)
				{
                    *dest++ = pixel;
                } else {
                    dest++;
                }
            }
            // Move on to the next line.
            src += sinc;
            dest += dinc;
        }
    }
}          


BOOL MleDIB::load(FILE *fp)
{
    // Declare local variables.
    BOOL isPM = FALSE;
    BITMAPINFO *BmpInfo = NULL;
    BYTE *data = NULL;

    // Get the current file position.
    DWORD fileStart = mlFTell(fp);

    // Read the file header to get the file size and to
    // find where the bits start in the file.
    BITMAPFILEHEADER BmpFileHdr;
    int numItemsRead;
    numItemsRead = mlFRead(&BmpFileHdr,sizeof(BmpFileHdr),1,fp);
    if (! numItemsRead)
	{
#if defined(MLE_DEBUG)
        OutputDebugString("MleDIB: Failed to read file header.");
        OutputDebugString(_strerror("Error Value"));
#endif /* MLE_DEBUG */
        goto $abort;
    }

    // Check that we have the magic 'BM' at the start.
    if (BmpFileHdr.bfType != 0x4D42)
	{
#if defined(MLE_DEBUG)
        OutputDebugString("MleDIB: Not a bitmap file.");
#endif /* MLE_DEBUG */
        goto $abort;
    }

    // Make a wild guess that the file is in Windows DIB
    // format and read the BITMAPINFOHEADER. If the file turns
    // out to be a PM DIB file we'll convert it later.
    BITMAPINFOHEADER BmpInfoHdr;
    numItemsRead = mlFRead(&BmpInfoHdr,sizeof(BmpInfoHdr),1,fp); 
    if (! numItemsRead)
	{
#if defined(MLE_DEBUG)
        OutputDebugString("MleDIB: Failed to read BITMAPINFOHEADER.");
        OutputDebugString(_strerror("Error Value"));
#endif /* MLE_DEBUG */
        goto $abort;
    }

    // Check that we got a real Windows DIB file.
    if (BmpInfoHdr.biSize != sizeof(BITMAPINFOHEADER))
	{
        if (BmpInfoHdr.biSize != sizeof(BITMAPCOREHEADER))
		{
#if defined(MLE_DEBUG)
            OutputDebugString("MleDIB: File is not Windows or PM DIB format.");
#endif /* MLE_DEBUG */
            goto $abort;
        }

        // Set a flag to convert PM file to Win format later.
        isPM = TRUE;

        // Back up the file pointer and read the BITMAPCOREHEADER
        // and create the BITMAPINFOHEADER from it.
        mlFSeek(fp,fileStart + sizeof(BITMAPFILEHEADER),SEEK_SET);
        BITMAPCOREHEADER BmpCoreHdr;
        numItemsRead = mlFRead(&BmpCoreHdr,sizeof(BmpCoreHdr),1,fp); 
        if (! numItemsRead)
		{
#if defined(MLE_DEBUG)
            OutputDebugString("MleDIB: Failed to read BITMAPCOREHEADER.");
            OutputDebugString(_strerror("Error Value"));
#endif /* MLE_DEBUG */
            goto $abort;
        }

        BmpInfoHdr.biSize = sizeof(BITMAPINFOHEADER);
        BmpInfoHdr.biWidth = (int) BmpCoreHdr.bcWidth;
        BmpInfoHdr.biHeight = (int) BmpCoreHdr.bcHeight;
        BmpInfoHdr.biPlanes = BmpCoreHdr.bcPlanes;
        BmpInfoHdr.biBitCount = BmpCoreHdr.bcBitCount;
        BmpInfoHdr.biCompression = BI_RGB;
        BmpInfoHdr.biSizeImage = 0;
        BmpInfoHdr.biXPelsPerMeter = 0;
        BmpInfoHdr.biYPelsPerMeter = 0;
        BmpInfoHdr.biClrUsed = 0;
        BmpInfoHdr.biClrImportant = 0;
    }

    // Work out how much memory we need for the BITMAPINFO
    // structure, color table and then for the bits.  
    // Allocate the memory blocks.
    // Copy the BmpInfoHdr we have so far,
    // and then read in the color table from the file.
    int colors;
    int colorTableSize;
    colors = _numDIBColorEntries((LPBITMAPINFO) &BmpInfoHdr);
    colorTableSize = colors * sizeof(RGBQUAD);
    int bmDataSize;
    int bmInfoSize;

    // Always allocate enough room for 256 entries.
    bmInfoSize = sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD);
    bmDataSize = BmpFileHdr.bfSize -  BmpFileHdr.bfOffBits;

    // Allocate the memory for the header.
    BmpInfo = (LPBITMAPINFO) mlMalloc(bmInfoSize);
    if (! BmpInfo)
	{
#if defined(MLE_DEBUG)
        OutputDebugString("MleDIB: Out of memory for DIB header.");
        OutputDebugString(_strerror("Error Value"));
#endif /* MLE_DEBUG */
        goto $abort;
    }

    // Copy the header we already have.
    memcpy(BmpInfo,&BmpInfoHdr,sizeof(BITMAPINFOHEADER));

    // Now read the color table from the file.
    if (isPM == FALSE)
	{
        // Read the color table from the file.
        numItemsRead = mlFRead(((LPBYTE) BmpInfo) + sizeof(BITMAPINFOHEADER),
                              colorTableSize,1,fp);
        if (! numItemsRead)
		{
#if defined(MLE_DEBUG)
            OutputDebugString("MleDIB: Failed to read color table.");
            OutputDebugString(_strerror("Error Value"));
#endif /* MLE_DEBUG */
            goto $abort;
        }
    } else
	{
        // Read each PM color table entry in turn and convert it
        // to Win DIB format as we go.
        LPRGBQUAD rgbQuad;
        rgbQuad = (LPRGBQUAD) ((LPBYTE) BmpInfo + sizeof(BITMAPINFOHEADER));
        int i;
        RGBTRIPLE rgbTriple;
        for (i=0; i < colors; i++)
		{
            numItemsRead = mlFRead(&rgbTriple,sizeof(RGBTRIPLE),1,fp);
            if (! numItemsRead)
			{
#if defined(MLE_DEBUG)
                OutputDebugString("FwDIB: Failed to read RGBTRIPLE.");
                OutputDebugString(_strerror("Error Value"));
#endif /* MLE_DEBUG */
                goto $abort;
            }
            rgbQuad->rgbBlue = rgbTriple.rgbtBlue;
            rgbQuad->rgbGreen = rgbTriple.rgbtGreen;
            rgbQuad->rgbRed = rgbTriple.rgbtRed;
            rgbQuad->rgbReserved = 0;
            rgbQuad++;
        }
    }

    // Allocate the memory for the bits
    // and read the bits from the file.
    data = (BYTE *) mlMalloc(bmDataSize);
    if (! data)
	{
#if defined(MLE_DEBUG)
        OutputDebugString("MleDIB: Out of memory for DIB image data.");
        OutputDebugString(_strerror("Error Value"));
#endif MLE_DEBUG
        goto $abort;
    }

    // Seek to the bits in the file.
    if (mlFSeek(fp,fileStart + BmpFileHdr.bfOffBits,SEEK_SET))
	{
#if defined(MLE_DEBUG)
        OutputDebugString("MleDIB: Failed to seek to data offset.");
        OutputDebugString(_strerror("Error Value"));
#endif /* MLE_DEBUG */
        goto $abort;
    }

    // Read the bits.
    numItemsRead = mlFRead(data,1,bmDataSize,fp);
    if (! numItemsRead)
	{
#if defined(MLE_DEBUG)
        OutputDebugString("MleDIB:Failed to read bits.\n");
        OutputDebugString(_strerror("Error Value"));
        if (feof(fp))
            OutputDebugString("EOF Reached.\n");
#endif /* MLE_DEBUG */
        goto $abort;
    }

    // Everything went OK.
    if (m_bmInfo != NULL) mlFree(m_bmInfo);
    m_bmInfo = BmpInfo;
    if (m_ownData && (m_bmData != NULL))mlFree(m_bmData);
    m_bmData = data;
    m_ownData = TRUE;
    return(TRUE);
                
$abort: // Something went wrong.
    if (BmpInfo) mlFree(BmpInfo);
    if (data) mlFree(data);
    return(FALSE);
}

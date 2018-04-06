/** @defgroup MleRteWin32 Magic Lantern Runtime Engine Win32 Library API */

/**
 * @file dbgio.h
 * @ingroup MleRteWin32
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
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

HANDLE Std_Input;
HANDLE Std_Output;

#define MAXLINEBUF 256

static char lineBuf[MAXLINEBUF];

// Handler for console events.  A good place to restore the system palette
// in case the console window, rather than the graphic window, gets closed.
// By the time we get to our console handler, the graphic window is on its way out, 
// and doesn't seem capable of receiving a message to clean up after itself. 

BOOL CtrlHndlr(DWORD CtrlType)
{
	switch(CtrlType)
	{
		case CTRL_C_EVENT:
		case CTRL_BREAK_EVENT:
		case CTRL_CLOSE_EVENT:
		case CTRL_LOGOFF_EVENT:
		case CTRL_SHUTDOWN_EVENT:
			/* XXX - Put cleanup code here. */
			return FALSE;

		default:
			return FALSE;
	}
}

int InitDebugConsole(int winWidth, int winHeight, int bufWidth, int bufHeight)
{
	COORD bufDims;
	CONSOLE_SCREEN_BUFFER_INFO bufInfo;
	SMALL_RECT winRect;
	int owinWidth, owinHeight;

	if (winWidth == 0) winWidth = 80;
	if (winHeight == 0) winHeight = 24;
	if (bufWidth == 0) bufWidth = winWidth;
	if (bufHeight == 0) bufHeight = winHeight;
	
	if (bufWidth < winWidth) bufWidth = winWidth;
	if (bufHeight < winHeight) bufHeight = winHeight;

	bufDims.X = (SHORT)bufWidth;
	bufDims.Y = (SHORT)bufHeight;

	/* Establish Console Window */
	if (!AllocConsole()) return FALSE;
	SetConsoleTitle("Debug Console");
	Std_Input = GetStdHandle(STD_INPUT_HANDLE);		/* Enables dstdio input	*/
	Std_Output = GetStdHandle(STD_OUTPUT_HANDLE);	/* Enables dstdio output */
	SetConsoleTextAttribute(Std_Output, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE |
	 									 FOREGROUND_INTENSITY); /* Force high Palette index	*/
	SetProcessShutdownParameters(0x3ff, SHUTDOWN_NORETRY); /* High priority, direct shutdown */
	SetConsoleCtrlHandler((PHANDLER_ROUTINE) CtrlHndlr, TRUE); /* Cleanup before shutdown */
	SetConsoleMode(Std_Input, ENABLE_PROCESSED_INPUT);  /* CR -> NL, Rubouts */
	/* SetConsoleMode(Std_Output, ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT); (default) */

	/* Need to avoid setting a Window height to less than Buffer height, or
	   Window width to less than Buffer width.  So we set Window dimensions to
	   minimum of old and new, set the new Buffer size, then set the new
	   Window size. */
	GetConsoleScreenBufferInfo(Std_Output, &bufInfo);
	owinWidth = bufInfo.srWindow.Right - bufInfo.srWindow.Left + 1;
	owinHeight = bufInfo.srWindow.Bottom - bufInfo.srWindow.Top + 1;
	winRect.Top = (SHORT)0;
	winRect.Left = (SHORT)0;
	if (owinWidth < winWidth) 
		winRect.Right = (SHORT)(owinWidth - 1);
	else
		winRect.Right = (SHORT)(winWidth - 1);
	if (owinHeight < winHeight)
		winRect.Bottom = (SHORT)(owinHeight - 1);
	else
		winRect.Bottom = (SHORT)(winHeight - 1);  
	SetConsoleWindowInfo(Std_Output, TRUE, &winRect);
	SetConsoleScreenBufferSize(Std_Output, bufDims);
	winRect.Right = (SHORT)winWidth;
	winRect.Bottom = (SHORT)winHeight;
	SetConsoleWindowInfo(Std_Output, TRUE, &winRect);
	 
	return TRUE;
}


int dputchar(unsigned char chr)
{
	DWORD writeCount;

	if (WriteConsole(Std_Output, &chr, 1, &writeCount, NULL) == TRUE)
		return (int) chr;
	else
		return (int) EOF;
}


int dprintf(const char *format, ...)
{
	va_list ap;
	DWORD writeCount;
	int retval;
	
	lineBuf[0] = '\0';
	va_start(ap, format);
	retval = _vsnprintf(lineBuf, MAXLINEBUF-1, format, ap);
	va_end(ap);
	if (retval == -1)
		lineBuf[MAXLINEBUF-1] = '\0';

    if ((WriteConsole(Std_Output, lineBuf, strlen(lineBuf), &writeCount, NULL) == TRUE) &&
       (retval == (int)writeCount))
		return retval;
	else
		return EOF;
}


int dflushinput(void)
{
	return (int) FlushConsoleInputBuffer(Std_Input);
}


int dgetchar(VOID)
{
	char chr[2] = {'\0','\0'};
	DWORD readCount;
	DWORD mode;
	int retval;

	/* Put us into non-buffered mode if we are in line
	   buffered mode. */
	GetConsoleMode(Std_Input, &mode);
	if ((mode & ENABLE_LINE_INPUT) == ENABLE_LINE_INPUT)
	{
		SetConsoleMode(Std_Input, (mode ^ ENABLE_LINE_INPUT));
	}

	retval = ReadConsole(Std_Input, chr, 1, &readCount, NULL);

	/* Restore the mode if we needed to change it. */
	if((mode & ENABLE_LINE_INPUT) == ENABLE_LINE_INPUT)
	{
		SetConsoleMode(Std_Input, mode);
	}
	if (retval == TRUE)
		return (int)chr[0];
	else
		return EOF;
}

#define DSCANF_ARG_MAX 10

/* Limitations:  Single line of input, newline terminated.
                 Maximum of 10 address arguments. */	
int dscanf(const char *format, ...)
{
	va_list ap;
	DWORD readCount;
	int argCount = 0;
	int i, retval;
	DWORD mode, tmode;
	const char *cptr;
	void *argPtr[DSCANF_ARG_MAX];

	cptr = format;
	while (argCount < DSCANF_ARG_MAX)
	{
		/* Look for next % field */
		if((cptr = strchr(cptr, '%')) == NULL) break;
		/* Check for literal % (%%) */
		if(*(cptr + 1) == '%') {
			cptr += 2;
			continue;
		}
		/* Check for string match pattern. */
		if(*(cptr + 1) == '[') {
			/* Find closing bracket */
			if((cptr = strchr((cptr + 2), ']')) == NULL) break;
			cptr++;
			continue;
		}
		cptr++;
		argCount++;
	}

	/* Put us into line-buffered, echoing mode if we are in 
	   non-buffered mode. */
	tmode = ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT;
	GetConsoleMode(Std_Input, &mode);
	if ((mode & tmode) != tmode)
	{
		SetConsoleMode(Std_Input, (mode | tmode));
	}

	/* Get a line of console input. */
    if (ReadConsole(Std_Input, lineBuf, (MAXLINEBUF/2)-1, &readCount, NULL) != TRUE) return FALSE;

	/* Make sure string is null character terminatated. */
	lineBuf[readCount] = '\0';

	/* Restore the mode if we needed to change it. */
	if ((mode & tmode) != tmode)
	{
		SetConsoleMode(Std_Input, mode);
	}

	va_start(ap, format);
	for(i=0; i<argCount; i++)
	{
		argPtr[i] = va_arg(ap, void *);
	}
	va_end(ap);

	switch(argCount)
	{
		case 0:
			retval = sscanf(lineBuf, format);
			break;
		case 1:
			retval = sscanf(lineBuf, format, argPtr[0]);
			break;
		case 2:
			retval = sscanf(lineBuf, format, argPtr[0], argPtr[1]);
			break;
		case 3:
			retval = sscanf(lineBuf, format, argPtr[0], argPtr[1], argPtr[2]);
			break;
 		case 4:
			retval = sscanf(lineBuf, format, argPtr[0], argPtr[1], argPtr[2], argPtr[3]);
			break;
 		case 5:
			retval = sscanf(lineBuf, format, argPtr[0], argPtr[1], argPtr[2], argPtr[3], 
							argPtr[4]);
			break;
 		case 6:
			retval = sscanf(lineBuf, format, argPtr[0], argPtr[1], argPtr[2], argPtr[3], 
							argPtr[4], argPtr[5]);
			break;
 		case 7:
			retval = sscanf(lineBuf, format, argPtr[0], argPtr[1], argPtr[2], argPtr[3], 
							argPtr[4], argPtr[5], argPtr[6]);
			break;
 		case 8:
			retval = sscanf(lineBuf, format, argPtr[0], argPtr[1], argPtr[2], argPtr[3], 
							argPtr[4], argPtr[5], argPtr[6], argPtr[7]);
			break;
 		case 9:
			retval = sscanf(lineBuf, format, argPtr[0], argPtr[1], argPtr[2], argPtr[3], 
							argPtr[4], argPtr[5], argPtr[6], argPtr[7], argPtr[8]);
			break;
 		case 10:
			retval = sscanf(lineBuf, format, argPtr[0], argPtr[1], argPtr[2], argPtr[3], 
							argPtr[4], argPtr[5], argPtr[6], argPtr[7], argPtr[8], argPtr[9]);
			break;
	};

	return retval;
}

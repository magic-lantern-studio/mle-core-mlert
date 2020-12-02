/** @defgroup MleInput Magic Lantern Runtime Engine Input Library API */

/**
 * @file MleKeyMap.h
 * @ingroup MleInput
 *
 * @author Mark S. Millard
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015-2020 Wizzer Works
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

#ifndef __MLE_KEYMAP_H_
#define __MLE_KEYMAP_H_

#if defined(WIN32)
/******************* Win32 Key Map ********************/

// Mouse buttons.
#define MLE_KEY_Pointer_Button1 0x01    /* left button */
#define MLE_KEY_Pointer_Button2 0x02    /* middle button */
#define MLE_KEY_Pointer_Button3 0x04    /* right button */

// Miscellaneous.
#define MLE_KEY_Cancel         0x03
#define MLE_KEY_BackSpace      0x08
#define MLE_KEY_Tab            0x09
#define MLE_KEY_Clear          0x0C
#define MLE_KEY_Return         0x0D
#define MLE_KEY_Shift          0x10
#define MLE_KEY_Control        0x11
#define MLE_KEY_Menu           0x12
#define MLE_KEY_Pause          0x13
#define MLE_KEY_Escape         0x1B
#define MLE_KEY_Space          0x20
#define MLE_KEY_Prior          0x21
#define MLE_KEY_Next           0x22
#define MLE_KEY_End            0x23
#define MLE_KEY_Home           0x24
#define MLE_KEY_Left           0x25
#define MLE_KEY_Up             0x26
#define MLE_KEY_Right          0x27
#define MLE_KEY_Down           0x28
#define MLE_KEY_Select         0x29
#define MLE_KEY_Print          0x2A
#define MLE_KEY_Execute        0x2B
#define MLE_KEY_Insert         0x2D
#define MLE_KEY_Delete         0x2E
#define MLE_KEY_Help           0x2F

// Standard alphanumeric character set.
#define MLE_KEY_0              0x30
#define MLE_KEY_1              0x31
#define MLE_KEY_2              0x32
#define MLE_KEY_3              0x33
#define MLE_KEY_4              0x34
#define MLE_KEY_5              0x35
#define MLE_KEY_6              0x36
#define MLE_KEY_7              0x37
#define MLE_KEY_8              0x38
#define MLE_KEY_9              0x39
#define MLE_KEY_A              0x41
#define MLE_KEY_B              0x42
#define MLE_KEY_C              0x43
#define MLE_KEY_D              0x44
#define MLE_KEY_E              0x45
#define MLE_KEY_F              0x46
#define MLE_KEY_G              0x47
#define MLE_KEY_H              0x48
#define MLE_KEY_I              0x49
#define MLE_KEY_J              0x4a
#define MLE_KEY_K              0x4b
#define MLE_KEY_L              0x4c
#define MLE_KEY_M              0x4d
#define MLE_KEY_N              0x4e
#define MLE_KEY_O              0x4f
#define MLE_KEY_P              0x50
#define MLE_KEY_Q              0x51
#define MLE_KEY_R              0x52
#define MLE_KEY_S              0x53
#define MLE_KEY_T              0x54
#define MLE_KEY_U              0x55
#define MLE_KEY_V              0x56
#define MLE_KEY_W              0x57
#define MLE_KEY_X              0x58
#define MLE_KEY_Y              0x59
#define MLE_KEY_Z              0x5a
#define MLE_KEY_a              0x61
#define MLE_KEY_b              0x62
#define MLE_KEY_c              0x63
#define MLE_KEY_d              0x64
#define MLE_KEY_e              0x65
#define MLE_KEY_f              0x66
#define MLE_KEY_g              0x67
#define MLE_KEY_h              0x68
#define MLE_KEY_i              0x69
#define MLE_KEY_j              0x6a
#define MLE_KEY_k              0x6b
#define MLE_KEY_l              0x6c
#define MLE_KEY_m              0x6d
#define MLE_KEY_n              0x6e
#define MLE_KEY_o              0x6f
#define MLE_KEY_p              0x70
#define MLE_KEY_q              0x71
#define MLE_KEY_r              0x72
#define MLE_KEY_s              0x73
#define MLE_KEY_t              0x74
#define MLE_KEY_u              0x75
#define MLE_KEY_v              0x76
#define MLE_KEY_w              0x77
#define MLE_KEY_x              0x78
#define MLE_KEY_y              0x79
#define MLE_KEY_z              0x7a

// Keypad.
#define MLE_KEY_KP_0           0x60
#define MLE_KEY_KP_1           0x61
#define MLE_KEY_KP_2           0x62
#define MLE_KEY_KP_3           0x63
#define MLE_KEY_KP_4           0x64
#define MLE_KEY_KP_5           0x65
#define MLE_KEY_KP_6           0x66
#define MLE_KEY_KP_7           0x67
#define MLE_KEY_KP_8           0x68
#define MLE_KEY_KP_9           0x69
#define MLE_KEY_Multiply       0x6A
#define MLE_KEY_Add            0x6B
#define MLE_KEY_Separator      0x6C
#define MLE_KEY_Subtract       0x6D
#define MLE_KEY_Decimal        0x6E
#define MLE_KEY_Divide         0x6F

// Function keys (up to 24 defined) 
#define MLE_KEY_F1				0x70
#define MLE_KEY_F2				0x71
#define MLE_KEY_F3				0x72
#define MLE_KEY_F4				0x73
#define MLE_KEY_F5				0x74
#define MLE_KEY_F6				0x75
#define MLE_KEY_F7				0x76
#define MLE_KEY_F8				0x77
#define MLE_KEY_F9				0x78
#define MLE_KEY_F10				0x79
#define MLE_KEY_F11				0x7A
#define MLE_KEY_F12				0x7B
#define MLE_KEY_F13				0x7C
#define MLE_KEY_F14				0x7D
#define MLE_KEY_F15				0x7E
#define MLE_KEY_F16				0x7F
#define MLE_KEY_F17				0x80
#define MLE_KEY_F18				0x81
#define MLE_KEY_F19				0x82
#define MLE_KEY_F20				0x83
#define MLE_KEY_F21				0x84
#define MLE_KEY_F22				0x85
#define MLE_KEY_F23				0x86
#define MLE_KEY_F24				0x87

// Locks.
#define MLE_KEY_Num_Lock       0x90
#define MLE_KEY_Scroll_Lock    0x91

/*
 * Used only as parameters to a future MleKeyboardSynch class which would
 * use direct, synchronous input from the keyboard. MleKeyboardPolled
 * and MleKeyboardEvent are not sensitive to these key symbols.
 */
#define MLE_KEY_Shift_L        0xA0
#define MLE_KEY_Shift_R        0xA1
#define MLE_KEY_Control_L      0xA2
#define MLE_KEY_Control_R      0xA3

#endif /* WIN32 */

#if defined(__linux__)
#if defined(MLE_XT)
/******************* Xt Key Map ********************/

// Mouse buttons.
#define	MLE_KEY_Pointer_Button1			0xFEE9 /* left button */
#define	MLE_KEY_Pointer_Button2			0xFEEA /* middle button */
#define	MLE_KEY_Pointer_Button3			0xFEEB /* right button */

// Miscellaneous.
#define MLE_KEY_Cancel		0xFF69	/* cancel, stop, abort, exit */
#define MLE_KEY_BackSpace	0xFF08	/* back space, back char */
#define MLE_KEY_Tab			0xFF09
#define MLE_KEY_Clear		0xFF0B
#define MLE_KEY_Return		0xFF0D	/* return, enter */
#define MLE_KEY_Shift       0x0010
#define MLE_KEY_Control     0x0011
#define MLE_KEY_Menu		0xFF67
#define MLE_KEY_Pause		0xFF13	/* pause, hold */
#define MLE_KEY_Escape		0xFF1B
#define MLE_KEY_Space       0x020
#define MLE_KEY_Prior		0xFF55	/* prior, previous */
#define MLE_KEY_Next		0xFF56	/* next */
#define MLE_KEY_End			0xFF57	/* EOL */
#define MLE_KEY_Home		0xFF50
#define MLE_KEY_Left		0xFF51	/* move left, left arrow */
#define MLE_KEY_Up			0xFF52	/* move up, up arrow */
#define MLE_KEY_Right		0xFF53	/* move right, right arrow */
#define MLE_KEY_Down		0xFF54	/* move down, down arrow */
#define MLE_KEY_Select		0xFF60	/* select, mark */
#define MLE_KEY_Print		0xFF61
#define MLE_KEY_Execute		0xFF62	/* execute, run, do */
#define MLE_KEY_Insert		0xFF63	/* insert, insert here */
#define MLE_KEY_Delete		0xFFFF	/* delete, rubout */
#define MLE_KEY_Help		0xFF6A	/* help */

// Standard alphanumeric character set.
#define MLE_KEY_0           0x030
#define MLE_KEY_1           0x031
#define MLE_KEY_2           0x032
#define MLE_KEY_3           0x033
#define MLE_KEY_4           0x034
#define MLE_KEY_5           0x035
#define MLE_KEY_6           0x036
#define MLE_KEY_7           0x037
#define MLE_KEY_8           0x038
#define MLE_KEY_9           0x039
#define MLE_KEY_A           0x041
#define MLE_KEY_B           0x042
#define MLE_KEY_C           0x043
#define MLE_KEY_D           0x044
#define MLE_KEY_E           0x045
#define MLE_KEY_F           0x046
#define MLE_KEY_G           0x047
#define MLE_KEY_H           0x048
#define MLE_KEY_I           0x049
#define MLE_KEY_J           0x04a
#define MLE_KEY_K           0x04b
#define MLE_KEY_L           0x04c
#define MLE_KEY_M           0x04d
#define MLE_KEY_N           0x04e
#define MLE_KEY_O           0x04f
#define MLE_KEY_P           0x050
#define MLE_KEY_Q           0x051
#define MLE_KEY_R           0x052
#define MLE_KEY_S           0x053
#define MLE_KEY_T           0x054
#define MLE_KEY_U           0x055
#define MLE_KEY_V           0x056
#define MLE_KEY_W           0x057
#define MLE_KEY_X           0x058
#define MLE_KEY_Y           0x059
#define MLE_KEY_Z           0x05a
#define MLE_KEY_a           0x061
#define MLE_KEY_b           0x062
#define MLE_KEY_c           0x063
#define MLE_KEY_d           0x064
#define MLE_KEY_e           0x065
#define MLE_KEY_f           0x066
#define MLE_KEY_g           0x067
#define MLE_KEY_h           0x068
#define MLE_KEY_i           0x069
#define MLE_KEY_j           0x06a
#define MLE_KEY_k           0x06b
#define MLE_KEY_l           0x06c
#define MLE_KEY_m           0x06d
#define MLE_KEY_n           0x06e
#define MLE_KEY_o           0x06f
#define MLE_KEY_p           0x070
#define MLE_KEY_q           0x071
#define MLE_KEY_r           0x072
#define MLE_KEY_s           0x073
#define MLE_KEY_t           0x074
#define MLE_KEY_u           0x075
#define MLE_KEY_v           0x076
#define MLE_KEY_w           0x077
#define MLE_KEY_x           0x078
#define MLE_KEY_y           0x079
#define MLE_KEY_z           0x07a

// Keypad.
#define MLE_KEY_KP_0		0xFFB0
#define MLE_KEY_KP_1		0xFFB1
#define MLE_KEY_KP_2		0xFFB2
#define MLE_KEY_KP_3		0xFFB3
#define MLE_KEY_KP_4		0xFFB4
#define MLE_KEY_KP_5		0xFFB5
#define MLE_KEY_KP_6		0xFFB6
#define MLE_KEY_KP_7		0xFFB7
#define MLE_KEY_KP_8		0xFFB8
#define MLE_KEY_KP_9		0xFFB9
#define MLE_KEY_Multiply    0x0d7
#define MLE_KEY_Add			0xFFAB
#define MLE_KEY_Separator	0xFFAC	/* separator, often comma */
#define MLE_KEY_Subtract	0xFFAD
#define MLE_KEY_Decimal   	0xFFAE
#define MLE_KEY_Divide	    0xFFAF

// Function keys (up to 24 defined).
#define MLE_KEY_F1			0xFFBE
#define MLE_KEY_F2			0xFFBF
#define MLE_KEY_F3			0xFFC0
#define MLE_KEY_F4			0xFFC1
#define MLE_KEY_F5			0xFFC2
#define MLE_KEY_F6			0xFFC3
#define MLE_KEY_F7			0xFFC4
#define MLE_KEY_F8			0xFFC5
#define MLE_KEY_F9			0xFFC6
#define MLE_KEY_F10			0xFFC7
#define MLE_KEY_F11			0xFFC8
#define MLE_KEY_F12         0xFFC9
#define MLE_KEY_F13			0xFFCA
#define MLE_KEY_F14			0xFFCB
#define MLE_KEY_F15			0xFFCC
#define MLE_KEY_F16			0xFFCD
#define MLE_KEY_F17			0xFFCE
#define MLE_KEY_F18			0xFFCF
#define MLE_KEY_F19			0xFFD0
#define MLE_KEY_F20			0xFFD1
#define MLE_KEY_F21			0xFFD2
#define MLE_KEY_F22			0xFFD3
#define MLE_KEY_F23			0xFFD4
#define MLE_KEY_F24			0xFFD5

// Locks.
#define MLE_KEY_Num_Lock	0xFF7F
#define MLE_KEY_Scroll_Lock	0xFF14

/*
 * Used only as parameters to a future MleKeyboardSynch class which would
 * use direct, synchronous input from the keyboard. MleKeyboardPolled
 * and MleKeyboardEvent are not sensitive to these key symbols.
 */
#define MLE_KEY_Shift_L		0xFFE1	/* left shift */
#define MLE_KEY_Shift_R		0xFFE2	/* right shift */
#define MLE_KEY_Control_L	0xFFE3	/* left control */
#define MLE_KEY_Control_R	0xFFE4	/* right control */

#endif /* MLE_XT */
#if defined(MLE_QT)
/******************* Qt Key Map ********************/

#define MLE_KEY_Pointer_Button1 0x00000001  /* left button */
#define MLE_KEY_Pointer_Button2 0x00000004  /* middle button */
#define MLE_KEY_Pointer_Button3 0x00000002  /* right button */

// Miscellaneous.
#define MLE_KEY_Cancel		0xFF69	// 0x01000063
#define MLE_KEY_BackSpace	0xFF08	// 0x01000003
#define MLE_KEY_Tab			0xFF09  // 0x01000001
#define MLE_KEY_Clear		0xFF0B  // 0x0100000b
#define MLE_KEY_Return		0xFF0D	// 0x01000004
#define MLE_KEY_Shift       0x0010  // 0x01000020
#define MLE_KEY_Control     0x0011  // 0x01000021
#define MLE_KEY_Menu		0xFF67  // 0x01000055
#define MLE_KEY_Pause		0xFF13	// 0x01000008
#define MLE_KEY_Escape		0xFF1B  // 0x01000000
#define MLE_KEY_Space       0x20
#define MLE_KEY_Prior		0xFF55	// 0x01000017
#define MLE_KEY_Next		0xFF56	// 0x01000016
#define MLE_KEY_End			0xFF57	// 0x01000011
#define MLE_KEY_Home		0xFF50  // 0x01000010
#define MLE_KEY_Left		0xFF51	// 0x01000012
#define MLE_KEY_Up			0xFF52	// 0x01000013
#define MLE_KEY_Right		0xFF53	// 0x01000014
#define MLE_KEY_Down		0xFF54	// 0x01000015
#define MLE_KEY_Select		0xFF60	// 0x01010000
#define MLE_KEY_Print		0xFF61  // 0x01000009
#define MLE_KEY_Execute		0xFF62	// 0x01020003
#define MLE_KEY_Insert		0xFF63	// 0x01000006
#define MLE_KEY_Delete		0xFFFF	// 0x01000007
#define MLE_KEY_Help		0xFF6A	// 0x01000058

// Standard alphanumeric character set.
#define MLE_KEY_0           0x30
#define MLE_KEY_1           0x31
#define MLE_KEY_2           0x32
#define MLE_KEY_3           0x33
#define MLE_KEY_4           0x34
#define MLE_KEY_5           0x35
#define MLE_KEY_6           0x36
#define MLE_KEY_7           0x37
#define MLE_KEY_8           0x38
#define MLE_KEY_9           0x39
#define MLE_KEY_A           0x41
#define MLE_KEY_B           0x42
#define MLE_KEY_C           0x43
#define MLE_KEY_D           0x44
#define MLE_KEY_E           0x45
#define MLE_KEY_F           0x46
#define MLE_KEY_G           0x47
#define MLE_KEY_H           0x48
#define MLE_KEY_I           0x49
#define MLE_KEY_J           0x4a
#define MLE_KEY_K           0x4b
#define MLE_KEY_L           0x4c
#define MLE_KEY_M           0x4d
#define MLE_KEY_N           0x4e
#define MLE_KEY_O           0x4f
#define MLE_KEY_P           0x50
#define MLE_KEY_Q           0x51
#define MLE_KEY_R           0x52
#define MLE_KEY_S           0x53
#define MLE_KEY_T           0x54
#define MLE_KEY_U           0x55
#define MLE_KEY_V           0x56
#define MLE_KEY_W           0x57
#define MLE_KEY_X           0x58
#define MLE_KEY_Y           0x59
#define MLE_KEY_Z           0x5a
#define MLE_KEY_a           0x61  /* No direct mapping to Qt Key namespace. */
#define MLE_KEY_b           0x62  /* No direct mapping to Qt Key namespace. */
#define MLE_KEY_c           0x63  /* No direct mapping to Qt Key namespace. */
#define MLE_KEY_d           0x64  /* No direct mapping to Qt Key namespace. */
#define MLE_KEY_e           0x65  /* No direct mapping to Qt Key namespace. */
#define MLE_KEY_f           0x66  /* No direct mapping to Qt Key namespace. */
#define MLE_KEY_g           0x67  /* No direct mapping to Qt Key namespace. */
#define MLE_KEY_h           0x68  /* No direct mapping to Qt Key namespace. */
#define MLE_KEY_i           0x69  /* No direct mapping to Qt Key namespace. */
#define MLE_KEY_j           0x6a  /* No direct mapping to Qt Key namespace. */
#define MLE_KEY_k           0x6b  /* No direct mapping to Qt Key namespace. */
#define MLE_KEY_l           0x6c  /* No direct mapping to Qt Key namespace. */
#define MLE_KEY_m           0x6d  /* No direct mapping to Qt Key namespace. */
#define MLE_KEY_n           0x6e  /* No direct mapping to Qt Key namespace. */
#define MLE_KEY_o           0x6f  /* No direct mapping to Qt Key namespace. */
#define MLE_KEY_p           0x70  /* No direct mapping to Qt Key namespace. */
#define MLE_KEY_q           0x71  /* No direct mapping to Qt Key namespace. */
#define MLE_KEY_r           0x72  /* No direct mapping to Qt Key namespace. */
#define MLE_KEY_s           0x73  /* No direct mapping to Qt Key namespace. */
#define MLE_KEY_t           0x74  /* No direct mapping to Qt Key namespace. */
#define MLE_KEY_u           0x75  /* No direct mapping to Qt Key namespace. */
#define MLE_KEY_v           0x76  /* No direct mapping to Qt Key namespace. */
#define MLE_KEY_w           0x77  /* No direct mapping to Qt Key namespace. */
#define MLE_KEY_x           0x78  /* No direct mapping to Qt Key namespace. */
#define MLE_KEY_y           0x79  /* No direct mapping to Qt Key namespace. */
#define MLE_KEY_z           0x7a  /* No direct mapping to Qt Key namespace. */

// Keypad.
#define MLE_KEY_KP_0		0x30
#define MLE_KEY_KP_1		0x31
#define MLE_KEY_KP_2		0x32
#define MLE_KEY_KP_3		0x33
#define MLE_KEY_KP_4		0x34
#define MLE_KEY_KP_5		0x35
#define MLE_KEY_KP_6		0x36
#define MLE_KEY_KP_7		0x37
#define MLE_KEY_KP_8		0x38
#define MLE_KEY_KP_9		0x39
#define MLE_KEY_Multiply    0x2a  /* asterix */
#define MLE_KEY_Add			0x2b  /* plus */
#define MLE_KEY_Separator	0x2c  /* separator, often comma */
#define MLE_KEY_Subtract	0x2d  /* minus */
#define MLE_KEY_Decimal   	0x2e  /* period */
#define MLE_KEY_Divide	    0x2f  /* slash */

// Function keys (up to 24 defined).
#define MLE_KEY_F1			0xFFBE  // 0x01000030
#define MLE_KEY_F2			0xFFBF  // 0x01000031
#define MLE_KEY_F3			0xFFC0  // 0x01000032
#define MLE_KEY_F4			0xFFC1  // 0x01000033
#define MLE_KEY_F5			0xFFC2  // 0x01000034
#define MLE_KEY_F6			0xFFC3  // 0x01000035
#define MLE_KEY_F7			0xFFC4  // 0x01000036
#define MLE_KEY_F8			0xFFC5  // 0x01000037
#define MLE_KEY_F9			0xFFC6  // 0x01000038
#define MLE_KEY_F10			0xFFC7  // 0x01000039
#define MLE_KEY_F11			0xFFC8  // 0x0100003a
#define MLE_KEY_F12         0xFFC9  // 0x0100003b
#define MLE_KEY_F13			0xFFCA  // 0x0100003c
#define MLE_KEY_F14			0xFFCB  // 0x0100003d
#define MLE_KEY_F15			0xFFCC  // 0x0100003e
#define MLE_KEY_F16			0xFFCD  // 0x01000034
#define MLE_KEY_F17			0xFFCE  // 0x01000040
#define MLE_KEY_F18			0xFFCF  // 0x01000041
#define MLE_KEY_F19			0xFFD0  // 0x01000042
#define MLE_KEY_F20			0xFFD1  // 0x01000043
#define MLE_KEY_F21			0xFFD2  // 0x01000044
#define MLE_KEY_F22			0xFFD3  // 0x01000045
#define MLE_KEY_F23			0xFFD4  // 0x01000046
#define MLE_KEY_F24			0xFFD5  // 0x01000047

// Locks.
#define MLE_KEY_Num_Lock	0xFF7F  // 0x01000025
#define MLE_KEY_Scroll_Lock	0xFF14  // 0x01000026

/*
 * Used only as parameters to a future MleKeyboardSynch class which would
 * use direct, synchronous input from the keyboard. MleKeyboardPolled
 * and MleKeyboardEvent are not sensitive to these key symbols.
 */
#define MLE_KEY_Shift_L		0x01000020	/* left shift */
#define MLE_KEY_Shift_R		0x01000020	/* right shift */
#define MLE_KEY_Control_L	0x01000021	/* left control */
#define MLE_KEY_Control_R	0x01000021	/* right control */

#endif /* MLE_QT */
#endif /* __linux__ */

#endif /* __MLE_KEYMAP_H_ */

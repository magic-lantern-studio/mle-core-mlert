# Microsoft Developer Studio Project File - Name="mlert_win32" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=mlert_win32 - Win32 CDECL FloatDebug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "mlert_win32.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "mlert_win32.mak" CFG="mlert_win32 - Win32 CDECL FloatDebug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "mlert_win32 - Win32 FloatDebug" (based on "Win32 (x86) Static Library")
!MESSAGE "mlert_win32 - Win32 FloatRelease" (based on "Win32 (x86) Static Library")
!MESSAGE "mlert_win32 - Win32 CDECL FloatRelease" (based on "Win32 (x86) Static Library")
!MESSAGE "mlert_win32 - Win32 CDECL FloatDebug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "mlert_win32 - Win32 FloatDebug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "mlert_win32___Win32_FloatDebug"
# PROP BASE Intermediate_Dir "mlert_win32___Win32_FloatDebug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "FloatDebug"
# PROP Intermediate_Dir "FloatDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Gz /Zp4 /W3 /Gm /GX /ZI /Od /I "../../include" /I "../../../common/include" /I "../../../common/src/foundation" /I "../../../common/src/input" /I "../../../../util/common/include" /I "../../../win32/include" /I "$(MLE_HOME)/DigitalPlayprint/runtime/common/include" /D "MLE_DEBUG" /D "MLE_NOT_DLL" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /Gz /Zp4 /W3 /Gm /GX /ZI /Od /I "../../include" /I "../../../common/include" /I "../../../common/src/foundation" /I "../../../common/src/input" /I "../../../../util/common/include" /I "../../../../util/win32/include" /I "../../../../math/common/include" /I "$(MLE_HOME)/DigitalPlayprint/runtime/common/include" /D "MLE_DEBUG" /D "MLE_NOT_DLL" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"FloatDebug/mlertd.pch" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"Debug/mlertd.bsc"
# ADD BSC32 /nologo /o"FloatDebug/mlertd.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Debug\mlertd.lib"
# ADD LIB32 /nologo /out:"FloatDebug\mlertd.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Install Library
PostBuild_Cmds=mkdir $(MLE_HOME)\lib\runtime\stdcall	copy FloatDebug\mlertd.lib $(MLE_HOME)\lib\runtime\stdcall\mlertd.lib	mkdir $(MLE_HOME)\include\mle	xcopy /y $(MLE_HOME)\Core\mlert\common\src\foundation\mle\* $(MLE_HOME)\include\mle	xcopy /y $(MLE_HOME)\Core\mlert\common\src\input\mle\* $(MLE_HOME)\include\mle	xcopy /y $(MLE_HOME)\Core\mlert\win32\include\mle\* $(MLE_HOME)\include\mle
# End Special Build Tool

!ELSEIF  "$(CFG)" == "mlert_win32 - Win32 FloatRelease"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "mlert_win32___Win32_FloatRelease"
# PROP BASE Intermediate_Dir "mlert_win32___Win32_FloatRelease"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "FloatRelease"
# PROP Intermediate_Dir "FloatRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Gz /Zp4 /W3 /GX /O2 /I "../../include" /I "../../../common/include" /I "../../../common/src/foundation" /I "../../../common/src/input" /I "../../../../util/common/include" /I "../../../win32/include" /I "$(MLE_HOME)/DigitalPlayprint/runtime/common/include" /D "MLE_NOT_DLL" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /Gz /Zp4 /W3 /GX /O2 /I "../../include" /I "../../../common/include" /I "../../../common/src/foundation" /I "../../../common/src/input" /I "../../../../util/common/include" /I "../../../win32/include" /I "../../../../math/common/include" /I "$(MLE_HOME)/DigitalPlayprint/runtime/common/include" /D "MLE_NOT_DLL" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"FloatRelease/mlert.pch" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"Release/mlert.bsc"
# ADD BSC32 /nologo /o"FloatRelease/mlert.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Release\mlert.lib"
# ADD LIB32 /nologo /out:"FloatRelease\mlert.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Install Library
PostBuild_Cmds=mkdir $(MLE_HOME)\lib\runtime\stdcall	copy FloatRelease\mlert.lib $(MLE_HOME)\lib\runtime\stdcall\mlert.lib	mkdir $(MLE_HOME)\include\mle	xcopy /y $(MLE_HOME)\Core\mlert\common\src\foundation\mle\* $(MLE_HOME)\include\mle	xcopy /y $(MLE_HOME)\Core\mlert\common\src\input\mle\* $(MLE_HOME)\include\mle	xcopy /y $(MLE_HOME)\Core\mlert\win32\include\mle\* $(MLE_HOME)\include\mle
# End Special Build Tool

!ELSEIF  "$(CFG)" == "mlert_win32 - Win32 CDECL FloatRelease"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "mlert_win32___Win32_CDECL_FloatRelease"
# PROP BASE Intermediate_Dir "mlert_win32___Win32_CDECL_FloatRelease"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "CdeclFloatRelease"
# PROP Intermediate_Dir "CdeclFloatRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Gz /Zp4 /W3 /GX /O2 /I "../../include" /I "../../../common/include" /I "../../../common/src/foundation" /I "../../../common/src/input" /I "../../../../util/common/include" /I "../../../win32/include" /I "../../../../math/common/include" /I "$(MLE_HOME)/DigitalPlayprint/runtime/common/include" /D "MLE_NOT_DLL" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"FloatRelease/mlert.pch" /YX /FD /c
# ADD CPP /nologo /Zp8 /MT /W3 /GX /O2 /I "../../include" /I "../../../common/include" /I "../../../common/src/foundation" /I "../../../common/src/input" /I "../../../../util/common/include" /I "../../../win32/include" /I "../../../../math/common/include" /I "$(MLE_HOME)/DigitalPlayprint/runtime/common/include" /D "MLE_NOT_DLL" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"FloatRelease/mlert.pch" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"FloatRelease/mlert.bsc"
# ADD BSC32 /nologo /o"CdeclFloatRelease/mlert.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"FloatRelease\mlert.lib"
# ADD LIB32 /nologo /out:"CdeclFloatRelease\mlert.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Install Library
PostBuild_Cmds=mkdir $(MLE_HOME)\lib\runtime\cdecl	copy CdeclFloatRelease\mlert.lib $(MLE_HOME)\lib\runtime\cdecl\mlert.lib	mkdir $(MLE_HOME)\include\mle	xcopy /y $(MLE_HOME)\Core\mlert\common\src\foundation\mle\* $(MLE_HOME)\include\mle	xcopy /y $(MLE_HOME)\Core\mlert\common\src\input\mle\* $(MLE_HOME)\include\mle	xcopy /y $(MLE_HOME)\Core\mlert\win32\include\mle\* $(MLE_HOME)\include\mle
# End Special Build Tool

!ELSEIF  "$(CFG)" == "mlert_win32 - Win32 CDECL FloatDebug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "mlert_win32___Win32_CDECL_FloatDebug"
# PROP BASE Intermediate_Dir "mlert_win32___Win32_CDECL_FloatDebug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "CdeclFloatDebug"
# PROP Intermediate_Dir "CdeclFloatDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Gz /Zp4 /W3 /Gm /GX /ZI /Od /I "../../include" /I "../../../common/include" /I "../../../common/src/foundation" /I "../../../common/src/input" /I "../../../../util/common/include" /I "../../../../util/win32/include" /I "../../../../math/common/include" /I "$(MLE_HOME)/DigitalPlayprint/runtime/common/include" /D "MLE_DEBUG" /D "MLE_NOT_DLL" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"FloatDebug/mlertd.pch" /YX /FD /GZ /c
# ADD CPP /nologo /Zp8 /MTd /W3 /Gm /GX /ZI /Od /I "../../include" /I "../../../common/include" /I "../../../common/src/foundation" /I "../../../common/src/input" /I "../../../../util/common/include" /I "../../../../util/win32/include" /I "../../../../math/common/include" /I "$(MLE_HOME)/DigitalPlayprint/runtime/common/include" /D "MLE_DEBUG" /D "MLE_NOT_DLL" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"FloatDebug/mlertd.pch" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o"FloatDebug/mlertd.bsc"
# ADD BSC32 /nologo /o"CdeclFloatDebug/mlertd.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"FloatDebug\mlertd.lib"
# ADD LIB32 /nologo /out:"CdeclFloatDebug\mlertd.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Install Library
PostBuild_Cmds=mkdir $(MLE_HOME)\lib\runtime\cdecl	copy CdeclFloatDebug\mlertd.lib $(MLE_HOME)\lib\runtime\cdecl\mlertd.lib	mkdir $(MLE_HOME)\include\mle	xcopy /y $(MLE_HOME)\Core\mlert\common\src\foundation\mle\* $(MLE_HOME)\include\mle	xcopy /y $(MLE_HOME)\Core\mlert\common\src\input\mle\* $(MLE_HOME)\include\mle	xcopy /y $(MLE_HOME)\Core\mlert\win32\include\mle\* $(MLE_HOME)\include\mle
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "mlert_win32 - Win32 FloatDebug"
# Name "mlert_win32 - Win32 FloatRelease"
# Name "mlert_win32 - Win32 CDECL FloatRelease"
# Name "mlert_win32 - Win32 CDECL FloatDebug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\common\src\foundation\3danim.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\3dchar.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\dbgio.c
# End Source File
# Begin Source File

SOURCE=..\..\src\dib.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\dibpal.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\loadSet.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\common\src\mlDebug.c
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\MleActor.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\MleActorGC.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\MleDirector.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\MleEventDispatcher.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\MleGroup.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\input\MleKeyboardPolled.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\MleLoad.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\MleMediaRef.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\MleMonitor.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\input\MleMousePolled.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\MleNotifier.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\MlePq.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\MlePtrArray.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\MlePtrDict.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\MleRole.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\common\src\mlErrno.c
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\MleScene.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\MleScheduler.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\MleSet.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\MleStage.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\common\src\mlLogFile.c
# End Source File
# Begin Source File

SOURCE=..\..\src\palette.cxx
# End Source File
# Begin Source File

SOURCE=..\..\src\palmgr.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\3danim.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\3dchar.h
# End Source File
# Begin Source File

SOURCE=..\..\include\mle\dib.h
# End Source File
# Begin Source File

SOURCE=..\..\include\mle\dibpal.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\common\include\mle\mlArray.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\common\include\mle\mlAssert.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\common\include\mle\mlDebug.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MleActor.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MleActorGC.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MleDirector.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MleEventDispatcher.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MleGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\input\mle\MleKeyboardPolled.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\input\mle\MleKeyMap.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MleLoad.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MleMediaRef.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MleMediaRefConverter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MleMonitor.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\input\mle\MleMouseEvent.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\input\mle\MleMousePolled.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MleNotifier.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MleOpcode.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MlePq.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MlePropDataset.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MlePtrArray.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MlePtrContainer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MlePtrDict.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MleRole.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\common\include\mle\mlErrno.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MleScene.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MleScheduler.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MleSet.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MleStage.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MleStageFuncs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MleTables.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\common\include\mle\mlFileio.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\common\include\mle\mlLogFile.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\common\include\mle\mlMacros.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\common\include\mle\mlMalloc.h
# End Source File
# Begin Source File

SOURCE=..\include\mle\mlPlatformData.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\common\include\mle\mlToken.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\common\include\mle\mlTypes.h
# End Source File
# Begin Source File

SOURCE=..\..\include\mle\palette.h
# End Source File
# Begin Source File

SOURCE=..\..\include\mle\palmgr.h
# End Source File
# End Group
# End Target
# End Project

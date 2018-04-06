# Microsoft Developer Studio Project File - Name="mlert_rehearsal" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=mlert_rehearsal - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "mlert_rehearsal.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "mlert_rehearsal.mak" CFG="mlert_rehearsal - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "mlert_rehearsal - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "mlert_rehearsal - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "mlert_rehearsal - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MLERT_REHEARSAL_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../../include" /I "../../../common/include" /I "../../../common/src/foundation" /I "../../../common/src/input" /I "../../../../util/common/include" /I "../../../../util/win32/include" /I "$(MLE_ROOT)/Core/math/common/include" /I "$(MLE_ROOT)/DigitalWorkprint/lib/common/include" /I "$(MLE_ROOT)/DigitalWorkprint/lib/win32/include" /I "$(MLE_ROOT)/ATK/common/include" /I "$(MLE_ROOT)/ATK/win32/include" /D "MLE_REHEARSAL" /D "MLE_INTERNAL" /D "MLE_DLL" /D "MLE_MAKE_DLL" /D ML_MATH_DEBUG=0 /D ML_FIXED_POINT=0 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MLERT_REHEARSAL_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 DWP.lib mlmath.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /map /machine:I386 /out:"Release/mlert.dll" /libpath:"$(MLE_HOME)/lib/rehearsal"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Install DLL
PostBuild_Cmds=mkdir $(MLE_HOME)\bin\rehearsal	copy Release\mlert.dll $(MLE_HOME)\bin\rehearsal\mlert.dll	mkdir $(MLE_HOME)\lib\rehearsal	copy Release\mlert.lib $(MLE_HOME)\lib\rehearsal\mlert.lib	mkdir $(MLE_HOME)\include\mle	xcopy /y $(MLE_HOME)\Core\mlert\common\src\foundation\mle\* $(MLE_HOME)\include\mle	xcopy /y $(MLE_HOME)\Core\mlert\common\src\input\mle\* $(MLE_HOME)\include\mle	xcopy /y $(MLE_HOME)\Core\mlert\win32\include\mle\* $(MLE_HOME)\include\mle
# End Special Build Tool

!ELSEIF  "$(CFG)" == "mlert_rehearsal - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MLERT_REHEARSAL_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../include" /I "../../../common/include" /I "../../../common/src/foundation" /I "../../../common/src/input" /I "../../../../util/common/include" /I "../../../../util/win32/include" /I "$(MLE_ROOT)/Core/math/common/include" /I "$(MLE_ROOT)/DigitalWorkprint/lib/common/include" /I "$(MLE_ROOT)/DigitalWorkprint/lib/win32/include" /I "$(MLE_ROOT)/ATK/common/include" /I "$(MLE_ROOT)/ATK/win32/include" /D "MLE_REHEARSAL" /D "MLE_DEBUG" /D "MLE_INTERNAL" /D "MLE_DLL" /D "MLE_MAKE_DLL" /D ML_MATH_DEBUG=1 /D ML_FIXED_POINT=0 /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MLERT_REHEARSAL_EXPORTS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 DWPd.lib mlmathd.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /map /debug /machine:I386 /out:"Debug/mlertd.dll" /pdbtype:sept /libpath:"$(MLE_HOME)/lib/rehearsal"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Install DLL
PostBuild_Cmds=mkdir $(MLE_HOME)\bin\rehearsal	copy Debug\mlertd.dll $(MLE_HOME)\bin\rehearsal\mlertd.dll	mkdir $(MLE_HOME)\lib\rehearsal	copy Debug\mlertd.lib $(MLE_HOME)\lib\rehearsal\mlertd.lib	mkdir $(MLE_HOME)\include\mle	xcopy /y $(MLE_HOME)\Core\mlert\common\src\foundation\mle\* $(MLE_HOME)\include\mle	xcopy /y $(MLE_HOME)\Core\mlert\common\src\input\mle\* $(MLE_HOME)\include\mle	xcopy /y $(MLE_HOME)\Core\mlert\win32\include\mle\* $(MLE_HOME)\include\mle
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "mlert_rehearsal - Win32 Release"
# Name "mlert_rehearsal - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\common\src\foundation\3danim.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\3dchar.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\loaddwp.cxx
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

SOURCE=..\..\..\common\src\foundation\MleActorClass.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\MleDirector.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\MleDso.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\MleEventDispatcher.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\MleGroup.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\MleGroupClass.cxx
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

SOURCE=..\..\..\common\src\foundation\MleMediaRefClass.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\MleMonitor.cxx
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

SOURCE=..\..\..\common\src\foundation\MleRoleClass.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\common\src\mlErrno.c
# End Source File
# Begin Source File

SOURCE=..\..\src\mlert.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\MleScene.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\MleSceneClass.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\MleScheduler.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\MleSet.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\MleSetClass.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\MleStage.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\MleStageClass.cxx
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\common\src\mlExpandFilename.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\util\common\src\mlLogFile.c
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

SOURCE=..\..\..\..\util\common\include\mle\mlArray.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MleActor.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MleActorClass.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MleDirector.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MleDso.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MleEventDispatcher.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MleGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MleGroupClass.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\input\mle\MleKeyboardEvent.h
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

SOURCE=..\..\..\common\src\foundation\mle\MleMediaRefClass.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MleMediaRefConverter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MleMonitor.h
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

SOURCE=..\..\..\common\src\foundation\mle\MleRoleClass.h
# End Source File
# Begin Source File

SOURCE=..\..\src\mlert.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MleRuntime.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MleScene.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MleSceneClass.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MleScheduler.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MleSet.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MleSetClass.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MleStage.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MleStageClass.h
# End Source File
# Begin Source File

SOURCE=..\..\..\common\src\foundation\mle\MleStageFuncs.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project

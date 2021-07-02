# Microsoft Developer Studio Project File - Name="MEASYS" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=MEASYS - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "MEASYS.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "MEASYS.mak" CFG="MEASYS - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "MEASYS - Win32 Release" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "MEASYS - Win32 Debug" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MEASYS - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXEXT" /D "_WINDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"../../DLL/TnsMeaSys.dll" /implib:"../../LIB/TnsMeaSys.lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "MEASYS - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXEXT" /D "_WINDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../../DLL/dTnsMeaSys.dll" /implib:"../../LIB/dTnsMeaSys.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none /incremental:no /nodefaultlib

!ENDIF 

# Begin Target

# Name "MEASYS - Win32 Release"
# Name "MEASYS - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CompEASEHeadDesktop.cpp
# End Source File
# Begin Source File

SOURCE=.\CompEASEHeadSock.cpp
# End Source File
# Begin Source File

SOURCE=.\CompEASEMeasure.cpp
# End Source File
# Begin Source File

SOURCE=.\CompEASEPollingStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\CompEASETcpAsyncPort.cpp
# End Source File
# Begin Source File

SOURCE=.\CtaCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\CtaSio.cpp
# End Source File
# Begin Source File

SOURCE=.\MEASYS.cpp
# End Source File
# Begin Source File

SOURCE=.\MEASYS.rc
# End Source File
# Begin Source File

SOURCE=.\RadiusOfCurvature.cpp
# End Source File
# Begin Source File

SOURCE=.\RSHeadMCPT610.cpp
# End Source File
# Begin Source File

SOURCE=.\RsMeasure.cpp
# End Source File
# Begin Source File

SOURCE=.\ScanRange.cpp
# End Source File
# Begin Source File

SOURCE=.\SeAnalysis.cpp
# End Source File
# Begin Source File

SOURCE=.\SeHeadDesktop.cpp
# End Source File
# Begin Source File

SOURCE=.\SeHeadSock.cpp
# End Source File
# Begin Source File

SOURCE=.\SeMeasure.cpp
# End Source File
# Begin Source File

SOURCE=.\SrAnalysis.CPP
# End Source File
# Begin Source File

SOURCE=.\SrHeadDesktop.cpp
# End Source File
# Begin Source File

SOURCE=.\SrHeadParallel.cpp
# End Source File
# Begin Source File

SOURCE=.\SrHeadSerial.cpp
# End Source File
# Begin Source File

SOURCE=.\SrMeasure.cpp
# End Source File
# Begin Source File

SOURCE=.\SrRefFile.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StressAnalysis.cpp
# End Source File
# Begin Source File

SOURCE=.\StressHeadLKG5000.cpp
# End Source File
# Begin Source File

SOURCE=.\StressMeasure.cpp
# End Source File
# Begin Source File

SOURCE=.\TcpAsyncPort.cpp
# End Source File
# Begin Source File

SOURCE=.\Xmp.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CompEASEHead.h
# End Source File
# Begin Source File

SOURCE=.\CompEASEMeasure.h
# End Source File
# Begin Source File

SOURCE=.\CompEASEPollingStatus.h
# End Source File
# Begin Source File

SOURCE=.\CompEASETcpAsyncPort.h
# End Source File
# Begin Source File

SOURCE=.\CtaCtrl.h
# End Source File
# Begin Source File

SOURCE=.\CtaSio.h
# End Source File
# Begin Source File

SOURCE=.\CtaSioDef.h
# End Source File
# Begin Source File

SOURCE=.\LogFile.h
# End Source File
# Begin Source File

SOURCE=.\Mojiretsu.h
# End Source File
# Begin Source File

SOURCE=.\RadiusOfCurvature.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RsHead.h
# End Source File
# Begin Source File

SOURCE=.\RsMeasure.h
# End Source File
# Begin Source File

SOURCE=.\ScanRange.h
# End Source File
# Begin Source File

SOURCE=.\SeAnalysis.h
# End Source File
# Begin Source File

SOURCE=.\SeHead.h
# End Source File
# Begin Source File

SOURCE=.\SeMeasure.h
# End Source File
# Begin Source File

SOURCE=.\SrAnalysis.H
# End Source File
# Begin Source File

SOURCE=.\SrHead.H
# End Source File
# Begin Source File

SOURCE=.\SrMeasure.h
# End Source File
# Begin Source File

SOURCE=.\SrRefFile.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StressAnalysis.h
# End Source File
# Begin Source File

SOURCE=.\StressHead.h
# End Source File
# Begin Source File

SOURCE=.\StressMeasure.h
# End Source File
# Begin Source File

SOURCE=.\System.h
# End Source File
# Begin Source File

SOURCE=.\TcpAsyncPort.h
# End Source File
# Begin Source File

SOURCE=.\Xmp.h
# End Source File
# Begin Source File

SOURCE=.\XmpHelper.HXX
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\MEASYS.rc2
# End Source File
# End Group
# End Target
# End Project

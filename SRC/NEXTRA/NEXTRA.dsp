# Microsoft Developer Studio Project File - Name="NEXTRA" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=NEXTRA - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "NEXTRA.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "NEXTRA.mak" CFG="NEXTRA - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NEXTRA - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "NEXTRA - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "NEXTRA - Win32 Release"

# PROP BASE Use_MFC 5
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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"../../BIN/NEXTRA.EXE"

!ELSEIF  "$(CFG)" == "NEXTRA - Win32 Debug"

# PROP BASE Use_MFC 5
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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"../../BIN/NEXTRA.EXE" /pdbtype:sept
# SUBTRACT LINK32 /incremental:no /nodefaultlib

!ENDIF 

# Begin Target

# Name "NEXTRA - Win32 Release"
# Name "NEXTRA - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ALARM.CPP
# End Source File
# Begin Source File

SOURCE=.\ALHISDLG.CPP
# End Source File
# Begin Source File

SOURCE=.\COLORBTN.CPP
# End Source File
# Begin Source File

SOURCE=.\COLORSTC.CPP
# End Source File
# Begin Source File

SOURCE=.\COMMON.CPP
# End Source File
# Begin Source File

SOURCE=.\INFORMDLG.CPP
# End Source File
# Begin Source File

SOURCE=.\JOGOPDLG.CPP
# End Source File
# Begin Source File

SOURCE=.\MANOPDLG.CPP
# End Source File
# Begin Source File

SOURCE=.\NEXTRA.CPP
# End Source File
# Begin Source File

SOURCE=.\NEXTRA.RC
# End Source File
# Begin Source File

SOURCE=.\NEXTRADLG.CPP
# End Source File
# Begin Source File

SOURCE=.\STDAFX.CPP
# ADD CPP /Yc"STDAFX.H"
# End Source File
# Begin Source File

SOURCE=.\SYSCFGDLG.CPP
# End Source File
# Begin Source File

SOURCE=.\SYSTRAY.CPP
# End Source File
# Begin Source File

SOURCE=.\TEACHDLG.CPP
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ALARM.H
# End Source File
# Begin Source File

SOURCE=.\ALHISDLG.H
# End Source File
# Begin Source File

SOURCE=.\COLORBTN.H
# End Source File
# Begin Source File

SOURCE=.\COLORSTC.H
# End Source File
# Begin Source File

SOURCE=.\COMMON.HPP
# End Source File
# Begin Source File

SOURCE=.\INFORMDLG.H
# End Source File
# Begin Source File

SOURCE=.\JOGOPDLG.H
# End Source File
# Begin Source File

SOURCE=.\MANOPDLG.H
# End Source File
# Begin Source File

SOURCE=.\NEXTRA.H
# End Source File
# Begin Source File

SOURCE=.\NEXTRADLG.H
# End Source File
# Begin Source File

SOURCE=.\STDAFX.H
# End Source File
# Begin Source File

SOURCE=.\SYSCFGDLG.H
# End Source File
# Begin Source File

SOURCE=.\SYSTRAY.H
# End Source File
# Begin Source File

SOURCE=.\TEACHDLG.H
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\NEXTRA.ICO
# End Source File
# End Target
# End Project

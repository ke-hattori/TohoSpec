# Microsoft Developer Studio Project File - Name="NANOSPECRCJPN" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=NANOSPECRCJPN - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "NANOSPECRCJPN.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "NANOSPECRCJPN.mak" CFG="NANOSPECRCJPN - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "NANOSPECRCJPN - Win32 Release" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "NANOSPECRCJPN - Win32 Debug" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "NANOSPECRCJPN - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "NANOSPECRCJPN_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "NANOSPECRCJPN_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../../../dll/tnsNanospecRCJPN.dll" /noentry
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
TargetDir=\NanoSpec\dll
TargetPath=\NanoSpec\dll\tnsNanospecRCJPN.dll
SOURCE="$(InputPath)"
PostBuild_Cmds=copy "$(TargetPath)" "$(TargetDir)\tnsTohospecRCJPN.dll"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "NANOSPECRCJPN - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "NANOSPECRCJPN_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "NANOSPECRCJPN_EXPORTS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"../../../dll/dtnsNanospecRCJPN.dll" /pdbtype:sept /noentry
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
TargetDir=\NanoSpec\dll
TargetPath=\NanoSpec\dll\dtnsNanospecRCJPN.dll
SOURCE="$(InputPath)"
PostBuild_Cmds=copy "$(TargetPath)" "$(TargetDir)\dtnsTohospecRCJPN.dll"
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "NANOSPECRCJPN - Win32 Release"
# Name "NANOSPECRCJPN - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\NANOSPECRCJPN.RC
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Arrow_Left.ico
# End Source File
# Begin Source File

SOURCE=.\res\b001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b001.ico
# End Source File
# Begin Source File

SOURCE=.\res\b002.ico
# End Source File
# Begin Source File

SOURCE=.\res\b003..ico
# End Source File
# Begin Source File

SOURCE=.\res\b009.ico
# End Source File
# Begin Source File

SOURCE=.\res\b010.ico
# End Source File
# Begin Source File

SOURCE=.\res\b012.ico
# End Source File
# Begin Source File

SOURCE=.\res\b013.ico
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CoordinateFigure.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Cross_1.cur
# End Source File
# Begin Source File

SOURCE=.\res\data_bar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\GarbageCan.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_nano.ico
# End Source File
# Begin Source File

SOURCE=.\res\jog_direction.bmp
# End Source File
# Begin Source File

SOURCE=.\res\recipe_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SampleMap.bmp
# End Source File
# Begin Source File

SOURCE=.\res\StageFigure.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toho.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\warning.ico
# End Source File
# End Group
# End Target
# End Project

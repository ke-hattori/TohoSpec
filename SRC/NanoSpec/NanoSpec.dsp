# Microsoft Developer Studio Project File - Name="NanoSpec" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=NanoSpec - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "NanoSpec.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "NanoSpec.mak" CFG="NanoSpec - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "NanoSpec - Win32 Release" ("Win32 (x86) Application" 用)
!MESSAGE "NanoSpec - Win32 Debug" ("Win32 (x86) Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "NanoSpec - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\..\INC" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /X
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"../../BIN/NanoSpec.exe"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
TargetDir=\NanoSpec\BIN
TargetPath=\NanoSpec\BIN\NanoSpec.exe
SOURCE="$(InputPath)"
PostBuild_Cmds=copy "$(TargetPath)" "$(TargetDir)\TohoSpec.exe"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "NanoSpec - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\INC" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 dtnsnexif.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../../BIN_DEB\NanoSpec.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
TargetDir=\NanoSpec\BIN_DEB
TargetPath=\NanoSpec\BIN_DEB\NanoSpec.exe
SOURCE="$(InputPath)"
PostBuild_Cmds=copy "$(TargetPath)" "$(TargetDir)\TohoSpec.exe"
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "NanoSpec - Win32 Release"
# Name "NanoSpec - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AccessLevelSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AdditionalMeasurementDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Alarm.cpp
# End Source File
# Begin Source File

SOURCE=.\AlarmHistoryListView.cpp
# End Source File
# Begin Source File

SOURCE=.\AlarmHistoryOperationView.cpp
# End Source File
# Begin Source File

SOURCE=.\AlarmLogDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoFocusSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BackupDataSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BCMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\BitmapStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\BkDialogST.cpp
# End Source File
# Begin Source File

SOURCE=.\BtnST.cpp
# End Source File
# Begin Source File

SOURCE=.\CeXDib.cpp
# End Source File
# Begin Source File

SOURCE=.\CheckData.cpp
# End Source File
# Begin Source File

SOURCE=.\ChiefFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ChiefThread.cpp
# End Source File
# Begin Source File

SOURCE=.\ChiefView.cpp
# End Source File
# Begin Source File

SOURCE=.\ChifRcvMailThread.cpp
# End Source File
# Begin Source File

SOURCE=.\ChifTransiAF.cpp
# End Source File
# Begin Source File

SOURCE=.\ChifTransiDeskew.cpp
# End Source File
# Begin Source File

SOURCE=.\ChifTransiMaster.cpp
# End Source File
# Begin Source File

SOURCE=.\ChifTransiSeq.cpp
# End Source File
# Begin Source File

SOURCE=.\ChifTransiSr1Point.cpp
# End Source File
# Begin Source File

SOURCE=.\ChifTransiSrRefer.cpp
# End Source File
# Begin Source File

SOURCE=.\ChifTransiStress.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\CompEASEConfigurationSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CompEASEIPAddressSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ComSetupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ComSetupGridCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfirmDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CTACofigurationSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CTARecipeDefaultValueSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DataListOperationView.cpp
# End Source File
# Begin Source File

SOURCE=.\DataListView.cpp
# End Source File
# Begin Source File

SOURCE=.\DataReviewOperationView.cpp
# End Source File
# Begin Source File

SOURCE=.\DataReviewView.cpp
# End Source File
# Begin Source File

SOURCE=.\DataSearchDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DebugDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DeleteDataPeriodSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DeskewDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DeskewSeq.cpp
# End Source File
# Begin Source File

SOURCE=.\DeskewSeqWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\EdgeAlignerSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ExecutionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FileRetentionPeriod.cpp
# End Source File
# Begin Source File

SOURCE=.\FocusConfigurationSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GeneralPositionSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCell.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCellBase.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCellCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCellNumeric.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\GridDropTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\GropeObject.cpp
# End Source File
# Begin Source File

SOURCE=.\HandlerLoadPositionSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HostSetupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\InPlaceEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\InterlockDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\JogDirSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\JogSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LensComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\LogOffDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LogOnDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrmMailRecvThread.cpp
# End Source File
# Begin Source File

SOURCE=.\MainMenuView.cpp
# End Source File
# Begin Source File

SOURCE=.\MaintenanceDateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MaintenanceNoticeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MaintenanceOperationView.cpp
# End Source File
# Begin Source File

SOURCE=.\MaintenancePositionSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MaintenanceView.cpp
# End Source File
# Begin Source File

SOURCE=.\ManualDeskewDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ManualLoadPositionSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ManualMeasurementFormView.cpp
# End Source File
# Begin Source File

SOURCE=.\ManualMeasurementProcessView.cpp
# End Source File
# Begin Source File

SOURCE=.\ManualSitePtDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ManualStageDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ManuReMeasReCalcStatDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MeasurementGraphDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MeasurementGraphDlg2.cpp
# End Source File
# Begin Source File

SOURCE=.\MeasurementListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MeasurementOperationView.cpp
# End Source File
# Begin Source File

SOURCE=.\MeasurementProcessView.cpp
# End Source File
# Begin Source File

SOURCE=.\MeasurementTabView.cpp
# End Source File
# Begin Source File

SOURCE=.\MessageDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MISC.CPP
# End Source File
# Begin Source File

SOURCE=.\MotDataSetupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MotionComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\MotionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MsCofigurationSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MsRoboCylinderSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MultiLangHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\MySplitterWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\NanoLineSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\NanoListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\NanoListUI.cpp
# End Source File
# Begin Source File

SOURCE=.\NanoMeasureGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\NanoPointGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\NanoRecipeListUI.cpp
# End Source File
# Begin Source File

SOURCE=.\NanoRecipeUI.cpp
# End Source File
# Begin Source File

SOURCE=.\NanoSpec.cpp
# End Source File
# Begin Source File

SOURCE=.\NanoSpec.rc
# End Source File
# Begin Source File

SOURCE=.\NanoSpecDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\NanoSpecIni.cpp
# End Source File
# Begin Source File

SOURCE=.\NanoSpecView.cpp
# End Source File
# Begin Source File

SOURCE=.\NanoUI.cpp
# End Source File
# Begin Source File

SOURCE=.\OriginalPositionSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OverlayDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\PatRecDeskewDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PointConfirmationDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PointSetupDisposition2PointDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PointSetupDispositionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PointSetupLinesDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PointSetupMeasurement2PointDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PointSetupRectangleDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PointSetupTeaching2PointDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PointSetupTeachingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PointSetupTeachingMSDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RecipeDistanceStageProgramView.cpp
# End Source File
# Begin Source File

SOURCE=.\RecipeGantryStageProgramView.cpp
# End Source File
# Begin Source File

SOURCE=.\RecipeMainListView.cpp
# End Source File
# Begin Source File

SOURCE=.\RecipeMainSrView.cpp
# End Source File
# Begin Source File

SOURCE=.\RecipeMainStressView.cpp
# End Source File
# Begin Source File

SOURCE=.\RecipeMeasurementCompEASEThicknessProgramView.cpp
# End Source File
# Begin Source File

SOURCE=.\RecipeMeasurementCTAProgramView.cpp
# End Source File
# Begin Source File

SOURCE=.\RecipeMeasurementMSProgramView.cpp
# End Source File
# Begin Source File

SOURCE=.\RecipeMeasurementProgramListView.cpp
# End Source File
# Begin Source File

SOURCE=.\RecipeMeasurementResistanceProgramView.cpp
# End Source File
# Begin Source File

SOURCE=.\RecipeMeasurementSeThicknessProgramView.cpp
# End Source File
# Begin Source File

SOURCE=.\RecipeMeasurementSrCieReflectanceProgramView.cpp
# End Source File
# Begin Source File

SOURCE=.\RecipeMeasurementSrCieTransmittanceProgramView.cpp
# End Source File
# Begin Source File

SOURCE=.\RecipeMeasurementSrDistanceProgramView.cpp
# End Source File
# Begin Source File

SOURCE=.\RecipeMeasurementSrOdProgramView.cpp
# End Source File
# Begin Source File

SOURCE=.\RecipeMeasurementSrReflectanceProgramView.cpp
# End Source File
# Begin Source File

SOURCE=.\RecipeMeasurementSrThicknessProgramView.cpp
# End Source File
# Begin Source File

SOURCE=.\RecipeMeasurementSrTransmittanceProgramView.cpp
# End Source File
# Begin Source File

SOURCE=.\RecipeMeasurementStressProgramView.cpp
# End Source File
# Begin Source File

SOURCE=.\RecipeMultiListView.cpp
# End Source File
# Begin Source File

SOURCE=.\RecipeMultiView.cpp
# End Source File
# Begin Source File

SOURCE=.\RecipeOperationView.cpp
# End Source File
# Begin Source File

SOURCE=.\RecipePointDeskewProgramListView.cpp
# End Source File
# Begin Source File

SOURCE=.\RecipePointDeskewProgramView.cpp
# End Source File
# Begin Source File

SOURCE=.\RecipeRecalibrationProgramListView.cpp
# End Source File
# Begin Source File

SOURCE=.\RecipeRecalibrationProgramView.cpp
# End Source File
# Begin Source File

SOURCE=.\RecipeStageProgramListView.cpp
# End Source File
# Begin Source File

SOURCE=.\RecipeStageProgramMeasurementPointGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\RecipeStageProgramView.cpp
# End Source File
# Begin Source File

SOURCE=.\RecipeStressStageProgramView.cpp
# End Source File
# Begin Source File

SOURCE=.\RecipeThicknessCopyForm.cpp
# End Source File
# Begin Source File

SOURCE=.\RecipeThicknessSettingView.cpp
# End Source File
# Begin Source File

SOURCE=.\ReferenceMenuDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RSCofigurationSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SampleIdConDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SampleIdDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SampleListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SampleSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SaveAsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SaveModifiedDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SaveNameOverlapDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ScanDataGraphDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SeCofigurationSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SeFigureFormatSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectHeadTypeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectMeasurementTypeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectSampleTypeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectStageProgramTypeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectStressReferenceTypeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SeRecipeDefaultValueSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SeXmpSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ShadeButtonST.cpp
# End Source File
# Begin Source File

SOURCE=.\SignalTowerSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SitePatternEditDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SitePatternSetDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SitePtSeq.cpp
# End Source File
# Begin Source File

SOURCE=.\SitePtSeqWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\SpecifyCoordinate2PointDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SpecifyCoordinateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SpeedSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SrAutoFocusRetryTimesDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SrCcdArraySettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SrCofigurationSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SrFigureFormatSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SrFilterSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SrLensCenterOffsetSetingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SrRecipeDefaultValueSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SrReferencePositionSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SrTransmittanceSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SrTurretSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SrUserSettingPatternRecSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SrVideoCalibrationSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SrViewRangeSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SrXmpSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StageConfigSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StatusBarEx.cpp
# End Source File
# Begin Source File

SOURCE=.\StatusView.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StressCofigurationSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StressElasticModulusListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StressLineSectionSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StressReferenceListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StressSampleElasticModulusSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StressSelectSampleTypeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SubstrateThicknessSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SubstrateThicknessSettingListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemConfigurationSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemOperationCommonSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemOperationCompEASESettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemOperationCTASettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemOperationMsSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemOperationRSSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemOperationSeSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemOperationSrSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemOperationStressSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemOperationTabView.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemView.cpp
# End Source File
# Begin Source File

SOURCE=.\TestMeasurementDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TestMeasurementGantryDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TestMeasurementOperationView.cpp
# End Source File
# Begin Source File

SOURCE=.\TestScan.cpp
# End Source File
# Begin Source File

SOURCE=.\TitleTip.cpp
# End Source File
# Begin Source File

SOURCE=.\UserAccountListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UserAccountSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UserSettingOperationCommonSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UserSettingOperationCompEASESettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UserSettingOperationCTASettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UserSettingOperationMSSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UserSettingOperationRSSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UserSettingOperationSeSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UserSettingOperationSrSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UserSettingOperationStressSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UserSettingOperationTabView.cpp
# End Source File
# Begin Source File

SOURCE=.\UserSettingView.cpp
# End Source File
# Begin Source File

SOURCE=.\VacuumChangeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\VideoCalibDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewScreenDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewWindowView.cpp
# End Source File
# Begin Source File

SOURCE=.\WinXPButtonST.cpp
# End Source File
# Begin Source File

SOURCE=.\ZAxisOffsetSettingDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AccessLevelSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\AdditionalMeasurementDlg.h
# End Source File
# Begin Source File

SOURCE=.\Alarm.h
# End Source File
# Begin Source File

SOURCE=.\AlarmHistoryListView.h
# End Source File
# Begin Source File

SOURCE=.\AlarmHistoryOperationView.h
# End Source File
# Begin Source File

SOURCE=.\AlarmLogDlg.h
# End Source File
# Begin Source File

SOURCE=.\AutoFocusSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\BackupDataSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\BCMenu.h
# End Source File
# Begin Source File

SOURCE=.\BitmapStatic.h
# End Source File
# Begin Source File

SOURCE=.\BkDialogST.h
# End Source File
# Begin Source File

SOURCE=.\BtnST.h
# End Source File
# Begin Source File

SOURCE=.\CellRange.h
# End Source File
# Begin Source File

SOURCE=.\CeXDib.h
# End Source File
# Begin Source File

SOURCE=.\CheckData.h
# End Source File
# Begin Source File

SOURCE=.\ChiefExports.h
# End Source File
# Begin Source File

SOURCE=.\ChiefFrm.h
# End Source File
# Begin Source File

SOURCE=.\ChiefPFuncs.h
# End Source File
# Begin Source File

SOURCE=.\ChiefThread.h
# End Source File
# Begin Source File

SOURCE=.\ChiefView.h
# End Source File
# Begin Source File

SOURCE=.\ChifRcvMailThread.h
# End Source File
# Begin Source File

SOURCE=.\ChifTransiAF.h
# End Source File
# Begin Source File

SOURCE=.\ChifTransiDeskew.h
# End Source File
# Begin Source File

SOURCE=.\ChifTransiMaster.h
# End Source File
# Begin Source File

SOURCE=.\ChifTransiSeq.h
# End Source File
# Begin Source File

SOURCE=.\ChifTransiSr1Point.h
# End Source File
# Begin Source File

SOURCE=.\ChifTransiSrRefer.h
# End Source File
# Begin Source File

SOURCE=.\ChifTransiStress.h
# End Source File
# Begin Source File

SOURCE=.\ColorStatic.h
# End Source File
# Begin Source File

SOURCE=.\CompEASEConfigurationSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\CompEASEIPAddressSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\ComSetupDlg.h
# End Source File
# Begin Source File

SOURCE=.\ComSetupGridCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ConfirmDlg.h
# End Source File
# Begin Source File

SOURCE=.\CTACofigurationSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\CTARecipeDefaultValueSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\DataListOperationView.h
# End Source File
# Begin Source File

SOURCE=.\DataListView.h
# End Source File
# Begin Source File

SOURCE=.\DataReviewOperationView.h
# End Source File
# Begin Source File

SOURCE=.\DataReviewView.h
# End Source File
# Begin Source File

SOURCE=.\DataSearchDlg.h
# End Source File
# Begin Source File

SOURCE=.\DebugDlg.h
# End Source File
# Begin Source File

SOURCE=.\DeleteDataPeriodSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\DeskewDlg.h
# End Source File
# Begin Source File

SOURCE=.\DeskewSeq.h
# End Source File
# Begin Source File

SOURCE=.\DeskewSeqWnd.h
# End Source File
# Begin Source File

SOURCE=.\EdgeAlignerSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\ExecutionDlg.h
# End Source File
# Begin Source File

SOURCE=.\FileRetentionPeriod.h
# End Source File
# Begin Source File

SOURCE=.\FocusConfigurationSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\GeneralPositionSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\GridCell.h
# End Source File
# Begin Source File

SOURCE=.\GridCellBase.h
# End Source File
# Begin Source File

SOURCE=.\GridCellCheck.h
# End Source File
# Begin Source File

SOURCE=.\GridCellNumeric.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl.h
# End Source File
# Begin Source File

SOURCE=.\GridDropTarget.h
# End Source File
# Begin Source File

SOURCE=.\GropeObject.h
# End Source File
# Begin Source File

SOURCE=.\HandlerLoadPositionSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\HostSetupDlg.h
# End Source File
# Begin Source File

SOURCE=.\HyperLink.h
# End Source File
# Begin Source File

SOURCE=.\InPlaceEdit.h
# End Source File
# Begin Source File

SOURCE=.\InterlockDlg.h
# End Source File
# Begin Source File

SOURCE=.\JogDirSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\JogSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\LensComboBox.h
# End Source File
# Begin Source File

SOURCE=.\LogOffDlg.h
# End Source File
# Begin Source File

SOURCE=.\LogOnDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MainFrmMailRecvThread.h
# End Source File
# Begin Source File

SOURCE=.\MainMenuView.h
# End Source File
# Begin Source File

SOURCE=.\MaintenanceDateDlg.h
# End Source File
# Begin Source File

SOURCE=.\MaintenanceNoticeDlg.h
# End Source File
# Begin Source File

SOURCE=.\MaintenanceOperationView.h
# End Source File
# Begin Source File

SOURCE=.\MaintenancePositionSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\MaintenanceView.h
# End Source File
# Begin Source File

SOURCE=.\ManualDeskewDlg.h
# End Source File
# Begin Source File

SOURCE=.\ManualLoadPositionSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\ManualMeasurementFormView.h
# End Source File
# Begin Source File

SOURCE=.\ManualMeasurementOperationView.h
# End Source File
# Begin Source File

SOURCE=.\ManualMeasurementProcessView.h
# End Source File
# Begin Source File

SOURCE=.\ManualSitePtDlg.h
# End Source File
# Begin Source File

SOURCE=.\ManualStageDlg.h
# End Source File
# Begin Source File

SOURCE=.\ManuReMeasReCalcStatDlg.h
# End Source File
# Begin Source File

SOURCE=.\MeasurementCancelView.h
# End Source File
# Begin Source File

SOURCE=.\MeasurementGraphDlg.h
# End Source File
# Begin Source File

SOURCE=.\MeasurementGraphDlg2.h
# End Source File
# Begin Source File

SOURCE=.\MeasurementListDlg.h
# End Source File
# Begin Source File

SOURCE=.\MeasurementOperationView.h
# End Source File
# Begin Source File

SOURCE=.\MeasurementProcessView.h
# End Source File
# Begin Source File

SOURCE=.\MeasurementTabView.h
# End Source File
# Begin Source File

SOURCE=.\MemDC.h
# End Source File
# Begin Source File

SOURCE=.\MessageDlg.h
# End Source File
# Begin Source File

SOURCE=.\MISC.H
# End Source File
# Begin Source File

SOURCE=.\MotDataSetupDlg.h
# End Source File
# Begin Source File

SOURCE=.\MotionComboBox.h
# End Source File
# Begin Source File

SOURCE=.\MotionDlg.h
# End Source File
# Begin Source File

SOURCE=.\MsCofigurationSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\MsRoboCylinderSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\MultiLangHelper.h
# End Source File
# Begin Source File

SOURCE=.\MySplitterWnd.h
# End Source File
# Begin Source File

SOURCE=.\NanoGraphLib.h
# End Source File
# Begin Source File

SOURCE=.\NanoLineSelect.h
# End Source File
# Begin Source File

SOURCE=.\NanoListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\NanoListUI.h
# End Source File
# Begin Source File

SOURCE=.\NanoMeasureGraph.h
# End Source File
# Begin Source File

SOURCE=.\NanoPointGraph.h
# End Source File
# Begin Source File

SOURCE=.\NanoRecipeListUI.h
# End Source File
# Begin Source File

SOURCE=.\NanoRecipeUI.h
# End Source File
# Begin Source File

SOURCE=.\NanoSpec.h
# End Source File
# Begin Source File

SOURCE=.\NanoSpecDoc.h
# End Source File
# Begin Source File

SOURCE=.\NanoSpecIni.h
# End Source File
# Begin Source File

SOURCE=.\NanoSpecView.h
# End Source File
# Begin Source File

SOURCE=.\NanoUI.h
# End Source File
# Begin Source File

SOURCE=.\OriginalPositionSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\OverlayDialog.h
# End Source File
# Begin Source File

SOURCE=.\PatRecDeskewDlg.h
# End Source File
# Begin Source File

SOURCE=.\PointConfirmationDlg.h
# End Source File
# Begin Source File

SOURCE=.\PointSetupDisposition2PointDlg.h
# End Source File
# Begin Source File

SOURCE=.\PointSetupDispositionDlg.h
# End Source File
# Begin Source File

SOURCE=.\PointSetupLinesDlg.h
# End Source File
# Begin Source File

SOURCE=.\PointSetupMeasurement2PointDlg.h
# End Source File
# Begin Source File

SOURCE=.\PointSetupRectangleDlg.h
# End Source File
# Begin Source File

SOURCE=.\PointSetupTeaching2PointDlg.h
# End Source File
# Begin Source File

SOURCE=.\PointSetupTeachingDlg.h
# End Source File
# Begin Source File

SOURCE=.\PointSetupTeachingMSDlg.h
# End Source File
# Begin Source File

SOURCE=.\RecipeDistanceStageProgramView.h
# End Source File
# Begin Source File

SOURCE=.\RecipeGantryStageProgramView.h
# End Source File
# Begin Source File

SOURCE=.\RecipeMainListView.h
# End Source File
# Begin Source File

SOURCE=.\RecipeMainSrView.h
# End Source File
# Begin Source File

SOURCE=.\RecipeMainStressView.h
# End Source File
# Begin Source File

SOURCE=.\RecipeMeasurementCompEASEThicknessProgramView.h
# End Source File
# Begin Source File

SOURCE=.\RecipeMeasurementCTAProgramView.h
# End Source File
# Begin Source File

SOURCE=.\RecipeMeasurementMSProgramView.h
# End Source File
# Begin Source File

SOURCE=.\RecipeMeasurementProgramListView.h
# End Source File
# Begin Source File

SOURCE=.\RecipeMeasurementResistanceProgramView.h
# End Source File
# Begin Source File

SOURCE=.\RecipeMeasurementSeThicknessProgramView.h
# End Source File
# Begin Source File

SOURCE=.\RecipeMeasurementSrCieReflectanceProgramView.h
# End Source File
# Begin Source File

SOURCE=.\RecipeMeasurementSrCieTransmittanceProgramView.h
# End Source File
# Begin Source File

SOURCE=.\RecipeMeasurementSrDistanceProgramView.h
# End Source File
# Begin Source File

SOURCE=.\RecipeMeasurementSrOdProgramView.h
# End Source File
# Begin Source File

SOURCE=.\RecipeMeasurementSrReflectanceProgramView.h
# End Source File
# Begin Source File

SOURCE=.\RecipeMeasurementSrThicknessProgramView.h
# End Source File
# Begin Source File

SOURCE=.\RecipeMeasurementSrTransmittanceProgramView.h
# End Source File
# Begin Source File

SOURCE=.\RecipeMeasurementStressProgramView.h
# End Source File
# Begin Source File

SOURCE=.\RecipeMultiListView.h
# End Source File
# Begin Source File

SOURCE=.\RecipeMultiView.h
# End Source File
# Begin Source File

SOURCE=.\RecipeOperationView.h
# End Source File
# Begin Source File

SOURCE=.\RecipePointDeskewProgramListView.h
# End Source File
# Begin Source File

SOURCE=.\RecipePointDeskewProgramView.h
# End Source File
# Begin Source File

SOURCE=.\RecipeRecalibrationProgramListView.h
# End Source File
# Begin Source File

SOURCE=.\RecipeRecalibrationProgramView.h
# End Source File
# Begin Source File

SOURCE=.\RecipeStageProgramListView.h
# End Source File
# Begin Source File

SOURCE=.\RecipeStageProgramMeasurementPointGrid.h
# End Source File
# Begin Source File

SOURCE=.\RecipeStageProgramView.h
# End Source File
# Begin Source File

SOURCE=.\RecipeStressStageProgramView.h
# End Source File
# Begin Source File

SOURCE=.\RecipeThicknessCopyForm.h
# End Source File
# Begin Source File

SOURCE=.\RecipeThicknessSettingView.h
# End Source File
# Begin Source File

SOURCE=.\ReferenceMenuDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RSCofigurationSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\SampleIdConDlg.h
# End Source File
# Begin Source File

SOURCE=.\SampleIdDlg.h
# End Source File
# Begin Source File

SOURCE=.\SampleListDlg.h
# End Source File
# Begin Source File

SOURCE=.\SampleSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\SaveAsDlg.h
# End Source File
# Begin Source File

SOURCE=.\SaveModifiedDlg.h
# End Source File
# Begin Source File

SOURCE=.\savenameoverlapdlg.h
# End Source File
# Begin Source File

SOURCE=.\ScanDataGraphDlg.h
# End Source File
# Begin Source File

SOURCE=.\SeCofigurationSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\SeFigureFormatSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\SelectHeadTypeDlg.h
# End Source File
# Begin Source File

SOURCE=.\SelectMeasurementTypeDlg.h
# End Source File
# Begin Source File

SOURCE=.\SelectSampleTypeDlg.h
# End Source File
# Begin Source File

SOURCE=.\SelectStageProgramTypeDlg.h
# End Source File
# Begin Source File

SOURCE=.\SelectStressReferenceTypeDlg.h
# End Source File
# Begin Source File

SOURCE=.\SeRecipeDefaultValueSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\SeXmpSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\ShadeButtonST.h
# End Source File
# Begin Source File

SOURCE=.\SignalTowerSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\SitePatternEditDlg.h
# End Source File
# Begin Source File

SOURCE=.\SitePatternSetDlg.h
# End Source File
# Begin Source File

SOURCE=.\SitePtSeq.h
# End Source File
# Begin Source File

SOURCE=.\SitePtSeqWnd.h
# End Source File
# Begin Source File

SOURCE=.\SpecifyCoordinate2PointDlg.h
# End Source File
# Begin Source File

SOURCE=.\SpecifyCoordinateDlg.h
# End Source File
# Begin Source File

SOURCE=.\SpeedSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\SrAutoFocusRetryTimesDlg.h
# End Source File
# Begin Source File

SOURCE=.\SrCcdArraySettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\SrCofigurationSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\SrFigureFormatSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\SrFilterSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\SrLensCenterOffsetSetingDlg.h
# End Source File
# Begin Source File

SOURCE=.\SrRecipeDefaultValueSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\SrReferencePositionSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\SrTransmittanceSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\SrTurretSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\SrUserSettingPatternRecSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\SrVideoCalibrationSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\SrViewRangeSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\SrXmpSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\StageConfigSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\StatusBarEx.h
# End Source File
# Begin Source File

SOURCE=.\StatusView.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StressCofigurationSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\StressElasticModulusListDlg.h
# End Source File
# Begin Source File

SOURCE=.\StressLineSectionSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\StressReferenceListDlg.h
# End Source File
# Begin Source File

SOURCE=.\StressSampleElasticModulusSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\StressSelectSampleTypeDlg.h
# End Source File
# Begin Source File

SOURCE=.\SubstrateThicknessSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\SubstrateThicknessSettingListDlg.h
# End Source File
# Begin Source File

SOURCE=.\System.h
# End Source File
# Begin Source File

SOURCE=.\SystemConfigurationSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\SystemOperationCommonSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\SystemOperationCompEASESettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\SystemOperationCTASettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\SystemOperationMsSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\SystemOperationRSSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\SystemOperationSeSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\SystemOperationSrSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\SystemOperationStressSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\SystemOperationTabView.h
# End Source File
# Begin Source File

SOURCE=.\SystemView.h
# End Source File
# Begin Source File

SOURCE=.\TestMeasurementDlg.h
# End Source File
# Begin Source File

SOURCE=.\TestMeasurementGantryDlg.h
# End Source File
# Begin Source File

SOURCE=.\TestMeasurementOperationView.h
# End Source File
# Begin Source File

SOURCE=.\TestScan.h
# End Source File
# Begin Source File

SOURCE=.\TitleTip.h
# End Source File
# Begin Source File

SOURCE=.\UserAccountListDlg.h
# End Source File
# Begin Source File

SOURCE=.\UserAccountSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\UserSettingOperationCommonSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\UserSettingOperationCompEASESettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\UserSettingOperationCTASettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\UserSettingOperationMSSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\UserSettingOperationRSSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\UserSettingOperationSeSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\UserSettingOperationSrSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\UserSettingOperationStressSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\UserSettingOperationTabView.h
# End Source File
# Begin Source File

SOURCE=.\UserSettingView.h
# End Source File
# Begin Source File

SOURCE=.\VacuumChangeDlg.h
# End Source File
# Begin Source File

SOURCE=.\VideoCalibDlg.h
# End Source File
# Begin Source File

SOURCE=.\ViewScreenDlg.h
# End Source File
# Begin Source File

SOURCE=.\ViewWindowView.h
# End Source File
# Begin Source File

SOURCE=.\WinXPButtonST.h
# End Source File
# Begin Source File

SOURCE=.\ZAxisOffsetSettingDlg.h
# End Source File
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

SOURCE=.\res\b001_Hilight.bmp
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

SOURCE=.\res\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\buttonA1M01.ico
# End Source File
# Begin Source File

SOURCE=.\res\buttonA1M03.ico
# End Source File
# Begin Source File

SOURCE=.\res\buttonA1M05.ico
# End Source File
# Begin Source File

SOURCE=.\res\buttonA1M07.ico
# End Source File
# Begin Source File

SOURCE=.\res\coordina.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CoordinateFigure.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Cross_1.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
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

SOURCE=.\res\NanoSpec.ico
# End Source File
# Begin Source File

SOURCE=.\res\NanoSpec.rc2
# End Source File
# Begin Source File

SOURCE=.\res\NanoSpecDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\PointStage.bmp
# End Source File
# Begin Source File

SOURCE=.\res\recipe_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\right_ar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sample_f.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SampleCoordinate.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SampleMap.bmp
# End Source File
# Begin Source File

SOURCE=.\res\stage_ma.bmp
# End Source File
# Begin Source File

SOURCE=.\res\StageFigure.bmp
# End Source File
# Begin Source File

SOURCE=.\res\StageMap.bmp
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
# Begin Source File

SOURCE=.\res\warning_.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section NanoSpec : {80162274-C6EB-4DAC-9763-50EC93875C00}
# 	1:50:IDD_RECIPE_MEASUREMENT_SR_REFLECTANCE_PROGRAM_FORM:130
# 	2:16:Resource Include:resource.h
# 	2:50:IDD_RECIPE_MEASUREMENT_SR_REFLECTANCE_PROGRAM_FORM:IDD_RECIPE_MEASUREMENT_SR_REFLECTANCE_PROGRAM_FORM
# 	2:10:ENUM: enum:enum
# 	2:45:RecipeMeasurementSrReflectanceProgramView.cpp:RecipeMeasurementSrReflectanceProgramView.cpp
# 	2:43:RecipeMeasurementSrReflectanceProgramView.h:RecipeMeasurementSrReflectanceProgramView.h
# 	2:19:Application Include:NanoSpec.h
# 	2:49:CLASS: CRecipeMeasurementSrReflectanceProgramView:CRecipeMeasurementSrReflectanceProgramView
# End Section
# Section NanoSpec : {0130E72B-FBDF-4093-9A29-DB42B2E20ECB}
# 	1:44:IDD_EDIT_MEASUREMENT_REFLECTANCE_PROGRAM_DLG:121
# 	2:16:Resource Include:resource.h
# 	2:10:ENUM: enum:enum
# 	2:46:CLASS: CEditMeasurementSrReflectanceProgramDlg:CEditMeasurementSrReflectanceProgramDlg
# 	2:42:EditMeasurementSrReflectanceProgramDlg.cpp:EditMeasurementSrReflectanceProgramDlg.cpp
# 	2:44:IDD_EDIT_MEASUREMENT_REFLECTANCE_PROGRAM_DLG:IDD_EDIT_MEASUREMENT_REFLECTANCE_PROGRAM_DLG
# 	2:40:EditMeasurementSrReflectanceProgramDlg.h:EditMeasurementSrReflectanceProgramDlg.h
# 	2:19:Application Include:NanoSpec.h
# End Section
# Section NanoSpec : {83733CE1-59A3-44F7-9EA2-F3F7D776F301}
# 	1:46:IDD_RECIPE_MEASUREMENT_RESISTANCE_PROGRAM_FORM:125
# 	2:16:Resource Include:resource.h
# 	2:46:IDD_RECIPE_MEASUREMENT_RESISTANCE_PROGRAM_FORM:IDD_RECIPE_MEASUREMENT_RESISTANCE_PROGRAM_FORM
# 	2:10:ENUM: enum:enum
# 	2:46:CLASS: CRecipeMeasurementResistanceProgramView:CRecipeMeasurementResistanceProgramView
# 	2:19:Application Include:NanoSpec.h
# 	2:40:RecipeMeasurementResistanceProgramView.h:RecipeMeasurementResistanceProgramView.h
# 	2:42:RecipeMeasurementResistanceProgramView.cpp:RecipeMeasurementResistanceProgramView.cpp
# End Section
# Section NanoSpec : {A85E9025-E76D-4115-A20D-0C98DBAA068B}
# 	1:36:IDD_EDIT_MEASUREMENT_OD_PROGRAM_DLG1:122
# 	2:16:Resource Include:resource.h
# 	2:35:IDD_EDIT_MEASUREMENT_OD_PROGRAM_DLG:IDD_EDIT_MEASUREMENT_OD_PROGRAM_DLG1
# 	2:37:CLASS: CEditMeasurementSrODProgramDlg:CEditMeasurementSrODProgramDlg
# 	2:31:EditMeasurementSrODProgramDlg.h:EditMeasurementSrODProgramDlg.h
# 	2:33:EditMeasurementSrODProgramDlg.cpp:EditMeasurementSrODProgramDlg.cpp
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:NanoSpec.h
# End Section
# Section NanoSpec : {68D66463-08C2-4498-BD39-4DF1BEDE6519}
# 	1:51:IDD_RECIPE_MEASUREMENT_SR_CHROMATICITY_PROGRAM_FORM:138
# 	2:16:Resource Include:resource.h
# 	2:51:IDD_RECIPE_MEASUREMENT_SR_CHROMATICITY_PROGRAM_FORM:IDD_RECIPE_MEASUREMENT_SR_CHROMATICITY_PROGRAM_FORM
# 	2:10:ENUM: enum:enum
# 	2:44:RecipeMeasurementSrChromaticityProgramView.h:RecipeMeasurementSrChromaticityProgramView.h
# 	2:46:RecipeMeasurementSrChromaticityProgramView.cpp:RecipeMeasurementSrChromaticityProgramView.cpp
# 	2:19:Application Include:NanoSpec.h
# 	2:50:CLASS: CRecipeMeasurementSrChromaticityProgramView:CRecipeMeasurementSrChromaticityProgramView
# End Section
# Section NanoSpec : {2647DC2F-ABAB-4E74-8E5F-F8D138AFB534}
# 	1:15:IDD_SAVE_AS_DLG:118
# 	2:16:Resource Include:resource.h
# 	2:15:IDD_SAVE_AS_DLG:IDD_SAVE_AS_DLG
# 	2:21:CLASS: CSaveAsProgram:CSaveAsProgram
# 	2:17:SaveAsProgram.cpp:SaveAsProgram.cpp
# 	2:10:ENUM: enum:enum
# 	2:15:SaveAsProgram.h:SaveAsProgram.h
# 	2:19:Application Include:NanoSpec.h
# End Section
# Section NanoSpec : {FA52F43D-96BF-4AD0-B5C4-94999346CD04}
# 	1:46:IDD_EDIT_MEASUREMENT_CHROMATICITY_PROGRAM_DLG1:123
# 	2:16:Resource Include:resource.h
# 	2:41:EditMeasurementSrChromaticityProgramDlg.h:EditMeasurementSrChromaticityProgramDlg.h
# 	2:45:IDD_EDIT_MEASUREMENT_CHROMATICITY_PROGRAM_DLG:IDD_EDIT_MEASUREMENT_CHROMATICITY_PROGRAM_DLG1
# 	2:10:ENUM: enum:enum
# 	2:47:CLASS: CEditMeasurementSrChromaticityProgramDlg:CEditMeasurementSrChromaticityProgramDlg
# 	2:19:Application Include:NanoSpec.h
# 	2:43:EditMeasurementSrChromaticityProgramDlg.cpp:EditMeasurementSrChromaticityProgramDlg.cpp
# End Section
# Section NanoSpec : {6DA0F0F8-2CCD-4EF7-B169-91846C1CBEA7}
# 	1:26:IDD_MULTI_RECIPE_LIST_FORM:105
# 	2:16:Resource Include:resource.h
# 	2:27:CLASS: CMultiRecipeListView:CMultiRecipeListView
# 	2:21:MultiRecipeListView.h:MultiRecipeListView.h
# 	2:10:ENUM: enum:enum
# 	2:23:MultiRecipeListView.cpp:MultiRecipeListView.cpp
# 	2:26:IDD_MULTI_RECIPE_LIST_FORM:IDD_MULTI_RECIPE_LIST_FORM
# 	2:19:Application Include:NanoSpec.h
# End Section
# Section NanoSpec : {030A242D-D046-4E2E-9A38-C1189594D295}
# 	1:23:IDD_RECIPE_MAIN_SR_FORM:187
# 	2:16:Resource Include:resource.h
# 	2:24:CLASS: CRecipeMainSrView:CRecipeMainSrView
# 	2:20:RecipeMainSrView.cpp:RecipeMainSrView.cpp
# 	2:10:ENUM: enum:enum
# 	2:23:IDD_RECIPE_MAIN_SR_FORM:IDD_RECIPE_MAIN_SR_FORM
# 	2:19:Application Include:NanoSpec.h
# 	2:18:RecipeMainSrView.h:RecipeMainSrView.h
# End Section
# Section NanoSpec : {FD18113D-467B-4816-8F69-08933E7D828C}
# 	1:46:IDD_RECIPE_MEASUREMENT_RESISTANCE_PROGRAM_FORM:138
# 	2:16:Resource Include:resource.h
# 	2:46:IDD_RECIPE_MEASUREMENT_RESISTANCE_PROGRAM_FORM:IDD_RECIPE_MEASUREMENT_RESISTANCE_PROGRAM_FORM
# 	2:10:ENUM: enum:enum
# 	2:46:CLASS: CRecipeMeasurementResistanceProgramView:CRecipeMeasurementResistanceProgramView
# 	2:19:Application Include:NanoSpec.h
# 	2:40:RecipeMeasurementResistanceProgramView.h:RecipeMeasurementResistanceProgramView.h
# 	2:42:RecipeMeasurementResistanceProgramView.cpp:RecipeMeasurementResistanceProgramView.cpp
# End Section
# Section NanoSpec : {AC2BD71C-C07C-4D60-B75B-F6E2CB88C633}
# 	1:40:IDD_SELECT_MEASUREMENT_PROGRAM_TYPE_DLG1:119
# 	2:16:Resource Include:resource.h
# 	2:33:SelectMeasurementProgramTypeDlg.h:SelectMeasurementProgramTypeDlg1.h
# 	2:10:ENUM: enum:enum
# 	2:39:IDD_SELECT_MEASUREMENT_PROGRAM_TYPE_DLG:IDD_SELECT_MEASUREMENT_PROGRAM_TYPE_DLG1
# 	2:39:CLASS: CSelectMeasurementProgramTypeDlg:CSelectMeasurementProgramTypeDlg
# 	2:19:Application Include:NanoSpec.h
# 	2:35:SelectMeasurementProgramTypeDlg.cpp:SelectMeasurementProgramTypeDlg1.cpp
# End Section
# Section NanoSpec : {A5023D10-640E-4636-B3E0-9746DE5F1170}
# 	1:30:IDD_MATERIAL_PROGRAM_LIST_FORM:106
# 	2:16:Resource Include:resource.h
# 	2:30:IDD_MATERIAL_PROGRAM_LIST_FORM:IDD_MATERIAL_PROGRAM_LIST_FORM
# 	2:25:MaterialProgramListView.h:MaterialProgramListView.h
# 	2:10:ENUM: enum:enum
# 	2:31:CLASS: CMaterialProgramListView:CMaterialProgramListView
# 	2:27:MaterialProgramListView.cpp:MaterialProgramListView.cpp
# 	2:19:Application Include:NanoSpec.h
# End Section
# Section NanoSpec : {53E08768-2CA3-4AB1-9ABE-5865F997EAAF}
# 	1:35:IDD_RECALIBRATION_PROGRAM_LIST_FORM:108
# 	2:16:Resource Include:resource.h
# 	2:36:CLASS: CRecalibrationProgramListView:CRecalibrationProgramListView
# 	2:32:RecalibrationProgramListView.cpp:RecalibrationProgramListView.cpp
# 	2:10:ENUM: enum:enum
# 	2:30:RecalibrationProgramListView.h:RecalibrationProgramListView.h
# 	2:19:Application Include:NanoSpec.h
# 	2:35:IDD_RECALIBRATION_PROGRAM_LIST_FORM:IDD_RECALIBRATION_PROGRAM_LIST_FORM
# End Section
# Section NanoSpec : {3E6FF41F-BA68-4F33-BA3F-8A1272688F37}
# 	1:34:IDD_EDIT_RECALIBRATION_PROGRAM_DLG:114
# 	2:16:Resource Include:resource.h
# 	2:34:IDD_EDIT_RECALIBRATION_PROGRAM_DLG:IDD_EDIT_RECALIBRATION_PROGRAM_DLG
# 	2:29:EditRecalibrationProgramDlg.h:EditRecalibrationProgramDlg.h
# 	2:10:ENUM: enum:enum
# 	2:35:CLASS: CEditRecalibrationProgramDlg:CEditRecalibrationProgramDlg
# 	2:31:EditRecalibrationProgramDlg.cpp:EditRecalibrationProgramDlg.cpp
# 	2:19:Application Include:NanoSpec.h
# End Section
# Section NanoSpec : {1419B92E-9E10-4A6F-927A-426C41DEE5C5}
# 	1:25:IDD_EDIT_MULTI_RECIPE_DLG:113
# 	2:16:Resource Include:resource.h
# 	2:26:CLASS: CEditMultiRecipeDlg:CEditMultiRecipeDlg
# 	2:20:EditMultiRecipeDlg.h:EditMultiRecipeDlg.h
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:NanoSpec.h
# 	2:25:IDD_EDIT_MULTI_RECIPE_DLG:IDD_EDIT_MULTI_RECIPE_DLG
# 	2:22:EditMultiRecipeDlg.cpp:EditMultiRecipeDlg.cpp
# End Section
# Section NanoSpec : {D6942658-1607-418B-9709-6182D59FC114}
# 	1:27:IDD_STAGE_PROGRAM_LIST_FORM:109
# 	2:16:Resource Include:resource.h
# 	2:27:IDD_STAGE_PROGRAM_LIST_FORM:IDD_STAGE_PROGRAM_LIST_FORM
# 	2:22:StageProgramListView.h:StageProgramListView.h
# 	2:24:StageProgramListView.cpp:StageProgramListView.cpp
# 	2:10:ENUM: enum:enum
# 	2:28:CLASS: CStageProgramListView:CStageProgramListView
# 	2:19:Application Include:NanoSpec.h
# End Section
# Section NanoSpec : {5B3545D5-9BC2-420B-99BD-51B7A40CBAF2}
# 	1:44:IDD_EDIT_MEASUREMENT_RESISTANCE_PROGRAM_DLG1:124
# 	2:16:Resource Include:resource.h
# 	2:37:EditMeasurementResistanceProgramDlg.h:EditMeasurementResistanceProgramDlg1.h
# 	2:43:IDD_EDIT_MEASUREMENT_RESISTANCE_PROGRAM_DLG:IDD_EDIT_MEASUREMENT_RESISTANCE_PROGRAM_DLG1
# 	2:39:EditMeasurementResistanceProgramDlg.cpp:EditMeasurementResistanceProgramDlg1.cpp
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:NanoSpec.h
# 	2:43:CLASS: CEditMeasurementResistanceProgramDlg:CEditMeasurementResistanceProgramDlg
# End Section
# Section NanoSpec : {3B4268A5-4572-466F-B9B0-EACA616E3980}
# 	1:51:IDD_RECIPE_MEASUREMENT_SR_CHROMATICITY_PROGRAM_FORM:126
# 	2:16:Resource Include:resource.h
# 	2:51:IDD_RECIPE_MEASUREMENT_SR_CHROMATICITY_PROGRAM_FORM:IDD_RECIPE_MEASUREMENT_SR_CHROMATICITY_PROGRAM_FORM
# 	2:10:ENUM: enum:enum
# 	2:44:RecipeMeasurementSrChromaticityProgramView.h:RecipeMeasurementSrChromaticityProgramView.h
# 	2:46:RecipeMeasurementSrChromaticityProgramView.cpp:RecipeMeasurementSrChromaticityProgramView.cpp
# 	2:19:Application Include:NanoSpec.h
# 	2:50:CLASS: CRecipeMeasurementSrChromaticityProgramView:CRecipeMeasurementSrChromaticityProgramView
# End Section
# Section NanoSpec : {55E10944-0115-4339-9F91-ACE2786574A2}
# 	1:26:IDD_EDIT_STAGE_PROGRAM_DLG:115
# 	2:16:Resource Include:resource.h
# 	2:10:ENUM: enum:enum
# 	2:23:EditStageProgramDlg.cpp:EditStageProgramDlg.cpp
# 	2:21:EditStageProgramDlg.h:EditStageProgramDlg.h
# 	2:27:CLASS: CEditStageProgramDlg:CEditStageProgramDlg
# 	2:19:Application Include:NanoSpec.h
# 	2:26:IDD_EDIT_STAGE_PROGRAM_DLG:IDD_EDIT_STAGE_PROGRAM_DLG
# End Section
# Section NanoSpec : {5AF45AB2-D799-41B5-BECB-0C2AC9B301A6}
# 	1:30:IDD_STAGE_SIZE_TABLE_LIST_FORM:112
# 	2:16:Resource Include:resource.h
# 	2:30:CLASS: CStageSizeTableListView:CStageSizeTableListView
# 	2:30:IDD_STAGE_SIZE_TABLE_LIST_FORM:IDD_STAGE_SIZE_TABLE_LIST_FORM
# 	2:10:ENUM: enum:enum
# 	2:24:StageSizeTableListView.h:StageSizeTableListView.h
# 	2:26:StageSizeTableListView.cpp:StageSizeTableListView.cpp
# 	2:19:Application Include:NanoSpec.h
# End Section
# Section NanoSpec : {2D46C1F5-1256-49FE-A37A-E1EA8844D799}
# 	1:28:IDD_LENS_PARAMETER_LIST_FORM:110
# 	2:16:Resource Include:resource.h
# 	2:28:IDD_LENS_PARAMETER_LIST_FORM:IDD_LENS_PARAMETER_LIST_FORM
# 	2:25:LensParameterListView.cpp:LensParameterListView.cpp
# 	2:10:ENUM: enum:enum
# 	2:29:CLASS: CLensParameterListView:CLensParameterListView
# 	2:23:LensParameterListView.h:LensParameterListView.h
# 	2:19:Application Include:NanoSpec.h
# End Section
# Section NanoSpec : {4AA17175-C16D-4555-A018-5AC7CBD4F364}
# 	1:48:IDD_RECIPE_MEASUREMENT_SR_THICKNESS_PROGRAM_FORM:131
# 	2:16:Resource Include:resource.h
# 	2:47:CLASS: CRecipeMeasurementSrThicknessProgramView:CRecipeMeasurementSrThicknessProgramView
# 	2:10:ENUM: enum:enum
# 	2:41:RecipeMeasurementSrThicknessProgramView.h:RecipeMeasurementSrThicknessProgramView.h
# 	2:48:IDD_RECIPE_MEASUREMENT_SR_THICKNESS_PROGRAM_FORM:IDD_RECIPE_MEASUREMENT_SR_THICKNESS_PROGRAM_FORM
# 	2:43:RecipeMeasurementSrThicknessProgramView.cpp:RecipeMeasurementSrThicknessProgramView.cpp
# 	2:19:Application Include:NanoSpec.h
# End Section
# Section NanoSpec : {4E4F18CA-3779-415F-860C-7E807384934C}
# 	1:25:IDD_RECIPE_MAIN_LIST_FORM:170
# 	2:16:Resource Include:resource.h
# 	2:25:IDD_RECIPE_MAIN_LIST_FORM:IDD_RECIPE_MAIN_LIST_FORM
# 	2:22:RecipeMainListView.cpp:RecipeMainListView.cpp
# 	2:20:RecipeMainListView.h:RecipeMainListView.h
# 	2:10:ENUM: enum:enum
# 	2:26:CLASS: CRecipeMainListView:CRecipeMainListView
# 	2:19:Application Include:NanoSpec.h
# End Section
# Section NanoSpec : {6B95D826-FD52-4DD0-963E-09AE7D86124B}
# 	1:50:IDD_RECIPE_MEASUREMENT_SR_REFLECTANCE_PROGRAM_FORM:165
# 	2:16:Resource Include:resource.h
# 	2:50:IDD_RECIPE_MEASUREMENT_SR_REFLECTANCE_PROGRAM_FORM:IDD_RECIPE_MEASUREMENT_SR_REFLECTANCE_PROGRAM_FORM
# 	2:10:ENUM: enum:enum
# 	2:45:RecipeMeasurementSrReflectanceProgramView.cpp:RecipeMeasurementSrReflectanceProgramView.cpp
# 	2:43:RecipeMeasurementSrReflectanceProgramView.h:RecipeMeasurementSrReflectanceProgramView.h
# 	2:19:Application Include:NanoSpec.h
# 	2:49:CLASS: CRecipeMeasurementSrReflectanceProgramView:CRecipeMeasurementSrReflectanceProgramView
# End Section
# Section NanoSpec : {C289F0EE-7FBF-4C40-A329-C2E6C5A601E9}
# 	1:41:IDD_RECIPE_MEASUREMENT_SR_OD_PROGRAM_FORM:127
# 	2:16:Resource Include:resource.h
# 	2:41:IDD_RECIPE_MEASUREMENT_SR_OD_PROGRAM_FORM:IDD_RECIPE_MEASUREMENT_SR_OD_PROGRAM_FORM
# 	2:36:RecipeMeasurementSrOdProgramView.cpp:RecipeMeasurementSrOdProgramView.cpp
# 	2:40:CLASS: CRecipeMeasurementSrOdProgramView:CRecipeMeasurementSrOdProgramView
# 	2:34:RecipeMeasurementSrOdProgramView.h:RecipeMeasurementSrOdProgramView.h
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:NanoSpec.h
# End Section
# Section NanoSpec : {771F16BA-567F-4F55-ACB8-920375710C70}
# 	1:33:IDD_MEASUREMENT_PROGRAM_LIST_FORM:107
# 	2:16:Resource Include:resource.h
# 	2:28:MeasurementProgramListView.h:MeasurementProgramListView.h
# 	2:34:CLASS: CMeasurementProgramListView:CMeasurementProgramListView
# 	2:10:ENUM: enum:enum
# 	2:30:MeasurementProgramListView.cpp:MeasurementProgramListView.cpp
# 	2:19:Application Include:NanoSpec.h
# 	2:33:IDD_MEASUREMENT_PROGRAM_LIST_FORM:IDD_MEASUREMENT_PROGRAM_LIST_FORM
# End Section
# Section NanoSpec : {04183695-7314-4D96-94D5-519D6B6C1CEC}
# 	1:42:IDD_EDIT_MEASUREMENT_THICKNESS_PROGRAM_DLG:120
# 	2:16:Resource Include:resource.h
# 	2:44:CLASS: CEditMeasurementSrThicknessProgramDlg:CEditMeasurementSrThicknessProgramDlg
# 	2:40:EditMeasurementSrThicknessProgramDlg.cpp:EditMeasurementSrThicknessProgramDlg.cpp
# 	2:38:EditMeasurementSrThicknessProgramDlg.h:EditMeasurementSrThicknessProgramDlg.h
# 	2:10:ENUM: enum:enum
# 	2:42:IDD_EDIT_MEASUREMENT_THICKNESS_PROGRAM_DLG:IDD_EDIT_MEASUREMENT_THICKNESS_PROGRAM_DLG
# 	2:19:Application Include:NanoSpec.h
# End Section
# Section NanoSpec : {2CFC6787-D232-48EA-9376-7500F0B064BA}
# 	1:48:IDD_RECIPE_MEASUREMENT_SR_THICKNESS_PROGRAM_FORM:170
# 	2:16:Resource Include:resource.h
# 	2:47:CLASS: CRecipeMeasurementSrThicknessProgramView:CRecipeMeasurementSrThicknessProgramView
# 	2:10:ENUM: enum:enum
# 	2:41:RecipeMeasurementSrThicknessProgramView.h:RecipeMeasurementSrThicknessProgramView.h
# 	2:48:IDD_RECIPE_MEASUREMENT_SR_THICKNESS_PROGRAM_FORM:IDD_RECIPE_MEASUREMENT_SR_THICKNESS_PROGRAM_FORM
# 	2:43:RecipeMeasurementSrThicknessProgramView.cpp:RecipeMeasurementSrThicknessProgramView.cpp
# 	2:19:Application Include:NanoSpec.h
# End Section
# Section NanoSpec : {5EEF61F2-C12C-4E7E-AB95-AAA0F991DCD8}
# 	1:43:IDD_SR_USER_SETTING_PATTERN_REC_SETTING_DLG:117
# 	2:16:Resource Include:resource.h
# 	2:43:IDD_SR_USER_SETTING_PATTERN_REC_SETTING_DLG:IDD_SR_USER_SETTING_PATTERN_REC_SETTING_DLG
# 	2:37:SrUserSettingPatternRecSettingDlg.cpp:SrUserSettingPatternRecSettingDlg.cpp
# 	2:35:SrUserSettingPatternRecSettingDlg.h:SrUserSettingPatternRecSettingDlg.h
# 	2:41:CLASS: CSrUserSettingPatternRecSettingDlg:CSrUserSettingPatternRecSettingDlg
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:NanoSpec.h
# End Section
# Section NanoSpec : {5EEC789E-FA49-450D-BD9B-3F46C2E24BD3}
# 	1:41:IDD_RECIPE_MEASUREMENT_SR_OD_PROGRAM_FORM:164
# 	2:16:Resource Include:resource.h
# 	2:41:IDD_RECIPE_MEASUREMENT_SR_OD_PROGRAM_FORM:IDD_RECIPE_MEASUREMENT_SR_OD_PROGRAM_FORM
# 	2:36:RecipeMeasurementSrOdProgramView.cpp:RecipeMeasurementSrOdProgramView.cpp
# 	2:40:CLASS: CRecipeMeasurementSrOdProgramView:CRecipeMeasurementSrOdProgramView
# 	2:34:RecipeMeasurementSrOdProgramView.h:RecipeMeasurementSrOdProgramView.h
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:NanoSpec.h
# End Section
# Section NanoSpec : {1DC88AEE-A0E1-4A5E-8F54-4AFBBBE3F974}
# 	1:18:IDD_USER_LIST_FORM:111
# 	2:16:Resource Include:resource.h
# 	2:20:CLASS: CUserListView:CUserListView
# 	2:14:UserListView.h:UserListView.h
# 	2:10:ENUM: enum:enum
# 	2:18:IDD_USER_LIST_FORM:IDD_USER_LIST_FORM
# 	2:19:Application Include:NanoSpec.h
# 	2:16:UserListView.cpp:UserListView.cpp
# End Section
# Section NanoSpec : {0F63C2AA-7137-4355-A469-6FDD127CB84A}
# 	1:37:IDD_SR_REFERENCE_POSITION_SETTING_DLG:116
# 	2:16:Resource Include:resource.h
# 	2:37:IDD_SR_REFERENCE_POSITION_SETTING_DLG:IDD_SR_REFERENCE_POSITION_SETTING_DLG
# 	2:31:SrReferencePositionSettingDlg.h:SrReferencePositionSettingDlg.h
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:NanoSpec.h
# 	2:37:CLASS: CSrReferencePositionSettingDlg:CSrReferencePositionSettingDlg
# 	2:33:SrReferencePositionSettingDlg.cpp:SrReferencePositionSettingDlg.cpp
# End Section

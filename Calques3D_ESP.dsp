# Microsoft Developer Studio Project File - Name="Calques3D_ESP" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Calques3D_ESP - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Calques3D_ESP.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Calques3D_ESP.mak" CFG="Calques3D_ESP - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Calques3D_ESP - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Calques3D_ESP - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "Calques3D_ESP"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Calques3D_ESP - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_ESP"
# PROP Intermediate_Dir "Release_ESP"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"Release_ESP/Calques3D.bsc"
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 version.lib /nologo /subsystem:windows /machine:I386 /out:"Release_ESP/Calques3D.exe" /libpath:"OGLTools/"

!ELSEIF  "$(CFG)" == "Calques3D_ESP - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_ESP"
# PROP Intermediate_Dir "Debug_ESP"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"Debug_ESP/Calques3D.bsc"
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 version.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Debug_ESP/Calques3D.exe" /pdbtype:sept /libpath:"OGLTools/"

!ENDIF 

# Begin Target

# Name "Calques3D_ESP - Win32 Release"
# Name "Calques3D_ESP - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\OGLTools\3dmath.cpp
# End Source File
# Begin Source File

SOURCE=.\AboutInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Tasks\AmbiguityList.cpp
# End Source File
# Begin Source File

SOURCE=.\Collection\ArrayEx.cpp
# End Source File
# Begin Source File

SOURCE=.\OGLTools\BallController.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPShapeBar.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPShapeButton.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPShapeMenuButton.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPShapePopup.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGPSliderButton.cpp
# End Source File
# Begin Source File

SOURCE=.\BCGXMLSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\c3dLangES.rc
# End Source File
# Begin Source File

SOURCE=.\Calques3D_ESP.cpp
# End Source File
# Begin Source File

SOURCE=.\hlp\Calques3D_ESP.hpj
# End Source File
# Begin Source File

SOURCE=.\Calques3DDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\Tasks\CenterTask.cpp
# End Source File
# Begin Source File

SOURCE=.\objects\Cercle3D.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\objects\ComboShape.cpp
# End Source File
# Begin Source File

SOURCE=.\objects\CompositeObj3D.cpp
# End Source File
# Begin Source File

SOURCE=.\objects\Cone3D.cpp
# End Source File
# Begin Source File

SOURCE=.\CreditsCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Tasks\CubeTask.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomizeCalques3D.cpp
# End Source File
# Begin Source File

SOURCE=.\objects\Cylinder3D.cpp
# End Source File
# Begin Source File

SOURCE=.\Tasks\DelTask.cpp
# End Source File
# Begin Source File

SOURCE=.\objects\Droite3D.cpp
# End Source File
# Begin Source File

SOURCE=.\Tasks\DrParTask.cpp
# End Source File
# Begin Source File

SOURCE=.\Tasks\DrTask.cpp
# End Source File
# Begin Source File

SOURCE=.\objects\EditLabel.cpp
# End Source File
# Begin Source File

SOURCE=.\Tasks\ExtractTask.cpp
# End Source File
# Begin Source File

SOURCE=.\FormatToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Fraction.cpp
# End Source File
# Begin Source File

SOURCE=.\objects\Geom.cpp
# End Source File
# Begin Source File

SOURCE=.\OGLTools\GLEnabledView.cpp
# End Source File
# Begin Source File

SOURCE=.\OGLTools\glut.cpp
# End Source File
# Begin Source File

SOURCE=.\Tasks\Greffe.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridCell.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridCellBase.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridDropTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\HelpDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HelpPrSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\IconCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\InPlaceEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\Tasks\Intersect.cpp
# End Source File
# Begin Source File

SOURCE=.\MathParser\Lexer.cpp
# End Source File
# Begin Source File

SOURCE=.\objects\Locus3D.cpp
# End Source File
# Begin Source File

SOURCE=.\objects\Macro3D.cpp
# End Source File
# Begin Source File

SOURCE=.\MacroInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MathParser\MathParser.cpp
# End Source File
# Begin Source File

SOURCE=.\Tasks\MilieuTask.cpp
# End Source File
# Begin Source File

SOURCE=.\Tasks\MoveTask.cpp
# End Source File
# Begin Source File

SOURCE=.\thirdparty\MRTracker.cpp
# End Source File
# Begin Source File

SOURCE=.\objects\Object3D.cpp
# End Source File
# Begin Source File

SOURCE=.\objects\ObjectPropPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjectVisual.cpp
# End Source File
# Begin Source File

SOURCE=.\Tasks\Plan.cpp
# End Source File
# Begin Source File

SOURCE=.\objects\Plan3D.cpp
# End Source File
# Begin Source File

SOURCE=.\objects\Point3D.cpp
# End Source File
# Begin Source File

SOURCE=.\objects\Pointint3D.cpp
# End Source File
# Begin Source File

SOURCE=.\Tasks\Pointsur.cpp
# End Source File
# Begin Source File

SOURCE=.\objects\Pointsur3D.cpp
# End Source File
# Begin Source File

SOURCE=.\prefs\POVTool.cpp
# End Source File
# Begin Source File

SOURCE=.\POVUserDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\prefs\prefdiscovery.cpp
# End Source File
# Begin Source File

SOURCE=.\prefs\preferencedialog.cpp
# End Source File
# Begin Source File

SOURCE=.\prefs\PrefHistory.cpp
# End Source File
# Begin Source File

SOURCE=.\prefs\PrefMacros.cpp
# End Source File
# Begin Source File

SOURCE=.\prefs\prefmathpad.cpp
# End Source File
# Begin Source File

SOURCE=.\prefs\prefobjcolor.cpp
# End Source File
# Begin Source File

SOURCE=.\prefs\prefobjshape.cpp
# End Source File
# Begin Source File

SOURCE=.\prefs\Prefs.cpp
# End Source File
# Begin Source File

SOURCE=.\prefs\prefsession.cpp
# End Source File
# Begin Source File

SOURCE=.\prefs\prefuniverse.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectRCVersion.cpp
# End Source File
# Begin Source File

SOURCE=.\Tasks\ProjTask.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Tasks\PtTask.cpp
# End Source File
# Begin Source File

SOURCE=.\Tasks\RedefTask.cpp
# End Source File
# Begin Source File

SOURCE=.\objects\Shape.cpp
# End Source File
# Begin Source File

SOURCE=.\objects\Sphere3D.cpp
# End Source File
# Begin Source File

SOURCE=.\SplashScreen.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\MathParser\StrMap.cpp
# End Source File
# Begin Source File

SOURCE=.\Tasks\SuppressDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MathParser\SymTable.cpp
# End Source File
# Begin Source File

SOURCE=.\Tasks\SymTask.cpp
# End Source File
# Begin Source File

SOURCE=.\Tasks\Task.cpp
# End Source File
# Begin Source File

SOURCE=.\OGLTools\TestGLView.cpp
# End Source File
# Begin Source File

SOURCE=.\objects\Text3D.cpp
# End Source File
# Begin Source File

SOURCE=.\Tasks\TextTask.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\TitleTip.cpp
# End Source File
# Begin Source File

SOURCE=.\prefs\UndoBar.cpp
# End Source File
# Begin Source File

SOURCE=.\objects\Vector3D.cpp
# End Source File
# Begin Source File

SOURCE=.\objects\Vector4.cpp
# End Source File
# Begin Source File

SOURCE=.\Tasks\verifydlg.cpp
# End Source File
# Begin Source File

SOURCE=.\View3DRender.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewAnalytic.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewCalque.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewDepend.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewHisto.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewUniv.cpp
# End Source File
# Begin Source File

SOURCE=.\objects\VolumeObject3D.cpp
# End Source File
# Begin Source File

SOURCE=.\WarningDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ZoomDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\OGLTools\3dmath.h
# End Source File
# Begin Source File

SOURCE=.\AboutInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\Tasks\AmbiguityList.h
# End Source File
# Begin Source File

SOURCE=.\Collection\ArrayEx.h
# End Source File
# Begin Source File

SOURCE=.\OGLTools\BallController.h
# End Source File
# Begin Source File

SOURCE=.\BCGPShapeBar.h
# End Source File
# Begin Source File

SOURCE=.\BCGPShapeButton.h
# End Source File
# Begin Source File

SOURCE=.\BCGPShapePopup.h
# End Source File
# Begin Source File

SOURCE=.\BCGPSliderButton.h
# End Source File
# Begin Source File

SOURCE=.\BCGXMLSettings.h
# End Source File
# Begin Source File

SOURCE=.\Calques3D.h
# End Source File
# Begin Source File

SOURCE=.\Calques3DDoc.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\CellRange.h
# End Source File
# Begin Source File

SOURCE=.\objects\Cercle3D.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\objects\ComboShape.h
# End Source File
# Begin Source File

SOURCE=.\objects\CompositeObj3D.h
# End Source File
# Begin Source File

SOURCE=.\CreditsCtrl.h
# End Source File
# Begin Source File

SOURCE=.\CustomizeCalques3D.h
# End Source File
# Begin Source File

SOURCE=.\objects\Cylinder3D.h
# End Source File
# Begin Source File

SOURCE=.\objects\Droite3D.h
# End Source File
# Begin Source File

SOURCE=.\Fraction.h
# End Source File
# Begin Source File

SOURCE=.\objects\Geom.h
# End Source File
# Begin Source File

SOURCE=.\OGLTools\GLEnabledView.h
# End Source File
# Begin Source File

SOURCE=.\OGLTools\glut.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridCell.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridCellBase.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridCtrl.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridDropTarget.h
# End Source File
# Begin Source File

SOURCE=.\HelpDlg.h
# End Source File
# Begin Source File

SOURCE=.\HelpPrSheet.h
# End Source File
# Begin Source File

SOURCE=.\IconCtrl.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\InPlaceEdit.h
# End Source File
# Begin Source File

SOURCE=.\MathParser\Lexer.h
# End Source File
# Begin Source File

SOURCE=.\objects\Locus3D.h
# End Source File
# Begin Source File

SOURCE=.\objects\Macro3D.h
# End Source File
# Begin Source File

SOURCE=.\MacroInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MathParser\MathParser.h
# End Source File
# Begin Source File

SOURCE=.\MemDC.h
# End Source File
# Begin Source File

SOURCE=.\thirdparty\MRTracker.h
# End Source File
# Begin Source File

SOURCE=.\objects\Object3D.h
# End Source File
# Begin Source File

SOURCE=.\objects\ObjectPropPage.h
# End Source File
# Begin Source File

SOURCE=.\ObjectVisual.h
# End Source File
# Begin Source File

SOURCE=.\OGLTools\OGLT.h
# End Source File
# Begin Source File

SOURCE=.\objects\Plan3D.h
# End Source File
# Begin Source File

SOURCE=.\objects\Point3D.h
# End Source File
# Begin Source File

SOURCE=.\prefs\POVTool.h
# End Source File
# Begin Source File

SOURCE=.\POVUserDialog.h
# End Source File
# Begin Source File

SOURCE=.\prefs\prefdiscovery.h
# End Source File
# Begin Source File

SOURCE=.\prefs\preferencedialog.h
# End Source File
# Begin Source File

SOURCE=.\prefs\PrefMacros.h
# End Source File
# Begin Source File

SOURCE=.\prefs\prefmathpad.h
# End Source File
# Begin Source File

SOURCE=.\prefs\PrefObjShape.h
# End Source File
# Begin Source File

SOURCE=.\prefs\Prefs.h
# End Source File
# Begin Source File

SOURCE=.\prefs\PrefSession.h
# End Source File
# Begin Source File

SOURCE=.\prefs\PrefUniverse.h
# End Source File
# Begin Source File

SOURCE=.\PropertyBar.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\objects\Shape.h
# End Source File
# Begin Source File

SOURCE=.\objects\Sphere3D.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\MathParser\StrMap.h
# End Source File
# Begin Source File

SOURCE=.\Tasks\SuppressDlg.h
# End Source File
# Begin Source File

SOURCE=.\MathParser\SymTable.h
# End Source File
# Begin Source File

SOURCE=.\Tasks\Task.h
# End Source File
# Begin Source File

SOURCE=.\OGLTools\TestGLView.h
# End Source File
# Begin Source File

SOURCE=.\objects\Text3D.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\TitleTip.h
# End Source File
# Begin Source File

SOURCE=.\prefs\UndoBar.h
# End Source File
# Begin Source File

SOURCE=.\objects\Vector3D.h
# End Source File
# Begin Source File

SOURCE=.\objects\Vector4.h
# End Source File
# Begin Source File

SOURCE=.\Tasks\VerifyDlg.h
# End Source File
# Begin Source File

SOURCE=.\ViewAnalytic.h
# End Source File
# Begin Source File

SOURCE=.\ViewCalque.h
# End Source File
# Begin Source File

SOURCE=.\ViewDepend.h
# End Source File
# Begin Source File

SOURCE=.\ViewGraph.h
# End Source File
# Begin Source File

SOURCE=.\ViewHisto.h
# End Source File
# Begin Source File

SOURCE=.\ViewUniv.h
# End Source File
# Begin Source File

SOURCE=.\ZoomDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\arrowcop.cur
# End Source File
# Begin Source File

SOURCE=.\res\Calques3D.ico
# End Source File
# Begin Source File

SOURCE=.\res\dep_bar.ico
# End Source File
# Begin Source File

SOURCE=.\res\drag.cur
# End Source File
# Begin Source File

SOURCE=.\res\Drop_app.cur
# End Source File
# Begin Source File

SOURCE=.\res\drop_era.cur
# End Source File
# Begin Source File

SOURCE=.\res\drop_mar.cur
# End Source File
# Begin Source File

SOURCE=.\res\drop_sel.cur
# End Source File
# Begin Source File

SOURCE=.\res\idr_alge.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_calc.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_dep.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_graph.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_histo.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_macro.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_render.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_univ.ico
# End Source File
# Begin Source File

SOURCE=.\res\Move.cur
# End Source File
# Begin Source File

SOURCE=.\res\Movehor.cur
# End Source File
# Begin Source File

SOURCE=.\res\Movever.cur
# End Source File
# Begin Source File

SOURCE=.\res\Point.cur
# End Source File
# Begin Source File

SOURCE=.\res\prop_bar.ico
# End Source File
# End Group
# Begin Group "Help Files"

# PROP Default_Filter "cnt;rtf"
# End Group
# End Target
# End Project
# Section Calques3D : {72ADFD78-2C39-11D0-9903-00A0C91BC942}
# 	1:10:IDB_SPLASH:114
# 	2:21:SplashScreenInsertKey:4.0
# End Section

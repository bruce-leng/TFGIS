# Microsoft Developer Studio Project File - Name="TilSys" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=TilSys - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TilSys.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TilSys.mak" CFG="TilSys - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TilSys - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "TilSys - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TilSys - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../libR"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../GuiExt" /I "../TilCore" /I "../header" /I "../Canvas/CxImage" /I "../Canvas" /I "../MathPro" /I "../MeshLib" /I "../GuiLib" /I "C:/Program Files/HTML Help Workshop/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 GuiExt.lib GuiLib114.lib TilCore.lib Canvas.lib MeshLib.lib opengl32.lib htmlhelp.lib /nologo /subsystem:windows /machine:I386 /out:"../binR/TilSys.exe" /libpath:"../libR" /libpath:"C:/Program Files/HTML Help Workshop/lib"

!ELSEIF  "$(CFG)" == "TilSys - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../bin"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../GuiExt" /I "../TilCore" /I "../header" /I "../Canvas/CxImage" /I "../Canvas" /I "../MathPro" /I "../MeshLib" /I "../GuiLib" /I "C:/Program Files/HTML Help Workshop/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 GuiExt.lib GuiLibd114.lib TilCore.lib Canvas.lib MeshLib.lib opengl32.lib glaux.lib glu32.lib htmlhelp.lib /nologo /stack:0x989680 /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"../lib" /libpath:"C:/Program Files/HTML Help Workshop/lib"

!ENDIF 

# Begin Target

# Name "TilSys - Win32 Release"
# Name "TilSys - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ADOConn.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFaceCaption.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFaceInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFaceInfoEx.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFaceInfoSaveXml.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGraphObjLnk.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGraphTunSide.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgImageConcat.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgImageGeometryCorrect.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgImageHisto.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMeshGeoPredict.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMeshGeoVirThrill.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMeshSectionNormal.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOtherOptions.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPointStre.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRulerSet.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgThreshold.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTunnelNames.cpp
# End Source File
# Begin Source File

SOURCE=.\DragTree2.cpp
# End Source File
# Begin Source File

SOURCE=.\FaceInfoList.cpp
# End Source File
# Begin Source File

SOURCE=.\FaceInfoPrint.cpp
# End Source File
# Begin Source File

SOURCE=.\FaceInfoPrintEx.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiDataDraw.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiOutPut.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiServerExplorer.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiSolExplorer.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuImage.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuView.cpp
# End Source File
# Begin Source File

SOURCE=.\MeshTunnel.cpp
# End Source File
# Begin Source File

SOURCE=.\MeshView.cpp
# End Source File
# Begin Source File

SOURCE=.\OpenGlFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StyleBar.cpp
# End Source File
# Begin Source File

SOURCE=.\SysView.cpp
# End Source File
# Begin Source File

SOURCE=.\TilSys.cpp
# End Source File
# Begin Source File

SOURCE=.\TilSys.rc
# End Source File
# Begin Source File

SOURCE=.\TilSysDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\TilSysView.cpp
# End Source File
# Begin Source File

SOURCE=.\Tunnel.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ADOConn.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\DlgFaceCaption.h
# End Source File
# Begin Source File

SOURCE=.\DlgFaceInfo.h
# End Source File
# Begin Source File

SOURCE=.\DlgFaceInfoEx.h
# End Source File
# Begin Source File

SOURCE=.\DlgFaceInfoSaveXml.h
# End Source File
# Begin Source File

SOURCE=.\DlgGraphObjLnk.h
# End Source File
# Begin Source File

SOURCE=.\DlgGraphTunSide.h
# End Source File
# Begin Source File

SOURCE=.\DlgImageConcat.h
# End Source File
# Begin Source File

SOURCE=.\DlgImageGeometryCorrect.h
# End Source File
# Begin Source File

SOURCE=.\DlgImageHisto.h
# End Source File
# Begin Source File

SOURCE=.\DlgMeshGeoPredict.h
# End Source File
# Begin Source File

SOURCE=.\DlgMeshGeoVirThrill.h
# End Source File
# Begin Source File

SOURCE=.\DlgMeshSectionNormal.h
# End Source File
# Begin Source File

SOURCE=.\DlgOtherOptions.h
# End Source File
# Begin Source File

SOURCE=.\DlgPointStre.h
# End Source File
# Begin Source File

SOURCE=.\DlgRulerSet.h
# End Source File
# Begin Source File

SOURCE=.\DlgThreshold.h
# End Source File
# Begin Source File

SOURCE=.\DlgTunnelNames.h
# End Source File
# Begin Source File

SOURCE=.\DragTree2.h
# End Source File
# Begin Source File

SOURCE=.\FaceInfoList.h
# End Source File
# Begin Source File

SOURCE=.\FaceInfoPrint.h
# End Source File
# Begin Source File

SOURCE=.\FaceInfoPrintEx.h
# End Source File
# Begin Source File

SOURCE=.\GuiDataDraw.h
# End Source File
# Begin Source File

SOURCE=.\GuiOutPut.h
# End Source File
# Begin Source File

SOURCE=.\GuiServerExplorer.h
# End Source File
# Begin Source File

SOURCE=.\GuiSolExplorer.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MeshTunnel.h
# End Source File
# Begin Source File

SOURCE=.\MeshView.h
# End Source File
# Begin Source File

SOURCE=.\OpenGlFrm.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StyleBar.h
# End Source File
# Begin Source File

SOURCE=.\SysView.h
# End Source File
# Begin Source File

SOURCE=.\TilSys.h
# End Source File
# Begin Source File

SOURCE=.\TilSysDoc.h
# End Source File
# Begin Source File

SOURCE=.\TilSysView.h
# End Source File
# Begin Source File

SOURCE=.\Tunnel.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\b1.ico
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=".\job_images\CRD法.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\cursor_m.cur
# End Source File
# Begin Source File

SOURCE=.\res\dbprojec.bmp
# End Source File
# Begin Source File

SOURCE=.\res\gray.ico
# End Source File
# Begin Source File

SOURCE=.\res\id_chang.cur
# End Source File
# Begin Source File

SOURCE=.\res\id_draw.cur
# End Source File
# Begin Source File

SOURCE=.\res\no1.ico
# End Source File
# Begin Source File

SOURCE=.\res\opengl.bmp
# End Source File
# Begin Source File

SOURCE=.\res\r1.ico
# End Source File
# Begin Source File

SOURCE=.\res\style_ba.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TilSys.ico
# End Source File
# Begin Source File

SOURCE=.\res\TilSys.rc2
# End Source File
# Begin Source File

SOURCE=.\res\TilSysDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=".\job_images\二台阶法.bmp"
# End Source File
# Begin Source File

SOURCE=".\job_images\三台阶法.bmp"
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

# Microsoft Developer Studio Project File - Name="Canvas" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Canvas - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Canvas.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Canvas.mak" CFG="Canvas - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Canvas - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Canvas - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Canvas - Win32 Release"

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
F90=df.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "CxImage" /I "..\TilCore" /I "..\MathPro" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 CxImage.lib TilCore.lib MathPro.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../binR/Canvas.dll" /libpath:"../libR"

!ELSEIF  "$(CFG)" == "Canvas - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../lib"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "CxImage" /I "..\TilCore" /I "..\MathPro" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 lib\CxImage.lib TilCore.lib MathPro.lib /nologo /stack:0x989680 /subsystem:windows /dll /debug /machine:I386 /out:"../bin/Canvas.dll" /pdbtype:sept /libpath:"..\lib"

!ENDIF 

# Begin Target

# Name "Canvas - Win32 Release"
# Name "Canvas - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Canvas.cpp
# End Source File
# Begin Source File

SOURCE=.\Canvas.def
# End Source File
# Begin Source File

SOURCE=.\Canvas.rc
# End Source File
# Begin Source File

SOURCE=.\Graph\DlgSpecialChar.cpp
# End Source File
# Begin Source File

SOURCE=.\Graph\DLineDlg1.cpp
# End Source File
# Begin Source File

SOURCE=.\Graph\DLineDlg2.cpp
# End Source File
# Begin Source File

SOURCE=.\Graph\DLineProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\EdgeFind.cpp
# End Source File
# Begin Source File

SOURCE=.\Graph\Ellipse.cpp
# End Source File
# Begin Source File

SOURCE=.\Graph\EllipseArc.cpp
# End Source File
# Begin Source File

SOURCE=.\FaceCaption.cpp
# End Source File
# Begin Source File

SOURCE=.\Graph.cpp
# End Source File
# Begin Source File

SOURCE=.\Image.cpp
# End Source File
# Begin Source File

SOURCE=.\Graph\IntelScissor.cpp
# End Source File
# Begin Source File

SOURCE=.\Graph\Line.cpp
# End Source File
# Begin Source File

SOURCE=.\MultiSeg.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjEdgeList.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjRunLenCode.cpp
# End Source File
# Begin Source File

SOURCE=.\Graph\Polygon.cpp
# End Source File
# Begin Source File

SOURCE=.\Progress.cpp
# End Source File
# Begin Source File

SOURCE=.\Graph\Round.cpp
# End Source File
# Begin Source File

SOURCE=.\Graph\RoundArc.cpp
# End Source File
# Begin Source File

SOURCE=.\SegApi.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Graph\Text.cpp
# End Source File
# Begin Source File

SOURCE=.\Graph\TextEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\TextProgressCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\TilCanvas.cpp
# End Source File
# Begin Source File

SOURCE=.\TilCanvasMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\TilGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\TilImage.cpp
# End Source File
# Begin Source File

SOURCE=.\Graph\TilTunArc.cpp
# End Source File
# Begin Source File

SOURCE=.\Graph\Unit.cpp
# End Source File
# Begin Source File

SOURCE=.\Veins.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Graph\DlgSpecialChar.h
# End Source File
# Begin Source File

SOURCE=.\Graph\DLineDlg1.h
# End Source File
# Begin Source File

SOURCE=.\Graph\DLineDlg2.h
# End Source File
# Begin Source File

SOURCE=.\Graph\DLineProperty.h
# End Source File
# Begin Source File

SOURCE=.\EdgeFind.h
# End Source File
# Begin Source File

SOURCE=.\Graph\Ellipse.h
# End Source File
# Begin Source File

SOURCE=.\Graph\EllipseArc.h
# End Source File
# Begin Source File

SOURCE=.\FaceCaption.h
# End Source File
# Begin Source File

SOURCE=.\Graph.h
# End Source File
# Begin Source File

SOURCE=.\Graph\GraphDefine.h
# End Source File
# Begin Source File

SOURCE=.\Image.h
# End Source File
# Begin Source File

SOURCE=.\Graph\IntelScissor.h
# End Source File
# Begin Source File

SOURCE=.\Graph\Line.h
# End Source File
# Begin Source File

SOURCE=.\MultiSeg.h
# End Source File
# Begin Source File

SOURCE=.\ObjEdgeList.h
# End Source File
# Begin Source File

SOURCE=.\ObjRunLenCode.h
# End Source File
# Begin Source File

SOURCE=.\Graph\Polygon.h
# End Source File
# Begin Source File

SOURCE=.\Progress.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Graph\Round.h
# End Source File
# Begin Source File

SOURCE=.\Graph\RoundArc.h
# End Source File
# Begin Source File

SOURCE=.\SegApi.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Graph\Text.h
# End Source File
# Begin Source File

SOURCE=.\Graph\TextEdit.h
# End Source File
# Begin Source File

SOURCE=.\TextProgressCtrl.h
# End Source File
# Begin Source File

SOURCE=.\TilCanvas.h
# End Source File
# Begin Source File

SOURCE=.\TilCanvasMgr.h
# End Source File
# Begin Source File

SOURCE=.\TilGraph.h
# End Source File
# Begin Source File

SOURCE=.\TilImage.h
# End Source File
# Begin Source File

SOURCE=.\Graph\TilTunArc.h
# End Source File
# Begin Source File

SOURCE=.\Graph\Unit.h
# End Source File
# Begin Source File

SOURCE=.\Veins.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap10.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap6.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap7.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap8.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap9.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00007.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00008.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00009.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00010.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Canvas.rc2
# End Source File
# Begin Source File

SOURCE=.\res\Start.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

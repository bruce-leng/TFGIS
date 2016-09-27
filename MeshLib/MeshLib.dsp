# Microsoft Developer Studio Project File - Name="MeshLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=MeshLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MeshLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MeshLib.mak" CFG="MeshLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MeshLib - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "MeshLib - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MeshLib - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../Canvas/CxImage" /I "../TilCore" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 opengl32.lib glu32.lib CxImage.lib TilCore.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../binR/MeshLib.dll" /libpath:"../libR"

!ELSEIF  "$(CFG)" == "MeshLib - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../Canvas/CxImage" /I "../TilCore" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 opengl32.lib glu32.lib ../Canvas/lib/CxImage.lib TilCore.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\bin/MeshLib.dll" /pdbtype:sept /libpath:"../lib"

!ENDIF 

# Begin Target

# Name "MeshLib - Win32 Release"
# Name "MeshLib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Array3d.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientCapture.cpp
# End Source File
# Begin Source File

SOURCE=.\Color.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorRamp.cpp
# End Source File
# Begin Source File

SOURCE=.\Edge3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Face3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Material.cpp
# End Source File
# Begin Source File

SOURCE=.\Mesh3d.cpp
# End Source File
# Begin Source File

SOURCE=.\MeshLib.cpp
# End Source File
# Begin Source File

SOURCE=.\MeshLib.def
# End Source File
# Begin Source File

SOURCE=.\MeshLib.rc
# End Source File
# Begin Source File

SOURCE=.\Object3d.cpp
# End Source File
# Begin Source File

SOURCE=.\ParserVrml.cpp
# End Source File
# Begin Source File

SOURCE=.\PsRender.cpp
# End Source File
# Begin Source File

SOURCE=.\SceneGraph3d.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Texture.cpp
# End Source File
# Begin Source File

SOURCE=.\Transform.cpp
# End Source File
# Begin Source File

SOURCE=.\TRIANGLE.CPP
# End Source File
# Begin Source File

SOURCE=.\Utils3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Vector3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Vertex3d.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Array3d.h
# End Source File
# Begin Source File

SOURCE=.\Base3d.h
# End Source File
# Begin Source File

SOURCE=.\ClientCapture.h
# End Source File
# Begin Source File

SOURCE=.\Color.h
# End Source File
# Begin Source File

SOURCE=.\ColorRamp.h
# End Source File
# Begin Source File

SOURCE=.\Edge3d.h
# End Source File
# Begin Source File

SOURCE=.\Face3d.h
# End Source File
# Begin Source File

SOURCE=.\Material.h
# End Source File
# Begin Source File

SOURCE=.\Mesh3d.h
# End Source File
# Begin Source File

SOURCE=.\Object3d.h
# End Source File
# Begin Source File

SOURCE=.\ParserVrml.h
# End Source File
# Begin Source File

SOURCE=.\PsRender.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SceneGraph3d.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Texture.h
# End Source File
# Begin Source File

SOURCE=.\Transform.h
# End Source File
# Begin Source File

SOURCE=.\TRIANGLE.H
# End Source File
# Begin Source File

SOURCE=.\Utils3d.h
# End Source File
# Begin Source File

SOURCE=.\Vector3d.h
# End Source File
# Begin Source File

SOURCE=.\Vertex3d.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\MeshLib.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

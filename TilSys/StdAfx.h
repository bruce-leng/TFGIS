// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__1EE013DD_467D_4724_8DB0_0BBB3C57728D__INCLUDED_)
#define AFX_STDAFX_H__1EE013DD_467D_4724_8DB0_0BBB3C57728D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#pragma warning (disable : 4244)
#pragma warning (disable : 4786)
#pragma warning (disable : 4305)
#pragma warning (disable : 4005)

#define WM_UPDATE_GRAPH_TOOL		WM_USER +0x1004
#define WM_UPDATE_OPENGL_TOOL		WM_USER +0x1005

#define WM_CHANGE_ITEM	WM_USER+0x3001
#define WM_SELE_CHANGED WM_USER+0x3002
#define WM_SELE_DELETED	WM_USER+0x3003
#define WM_LOG_INFO		WM_USER+0x3004
#define WM_USER_NEWIMAGE WM_USER+0X3005

#import <msxml4.dll>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__1EE013DD_467D_4724_8DB0_0BBB3C57728D__INCLUDED_)

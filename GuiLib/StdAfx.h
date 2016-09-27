/****************************************************************************
 *																			*		 
 *								GuiToolKit  								*	
 *							 (MFC extension)								*			 
 * Created by Francisco Campos G. www.beyondata.com fcampos@beyondata.com	*
 *--------------------------------------------------------------------------*		   
 *																			*
 * This program is free software;so you are free to use it any of your		*
 * applications (Freeware, Shareware, Commercial),but leave this header		*
 * intact.																	*
 *																			*
 * These files are provided "as is" without warranty of any kind.			*
 *																			*
 *			       GuiToolKit is forever FREE CODE !!!!!					*
 *																			*
 *--------------------------------------------------------------------------*
 *																			*
 * Bug Fixes and improvements : (Add your name)								*
 * -Francisco Campos														*				
 * -igor1960																*	
 ****************************************************************************/

#if !defined(AFX_STDAFX_H__42A0B087_D792_11D4_B90C_000000000000__INCLUDED_)
#define AFX_STDAFX_H__42A0B087_D792_11D4_B90C_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

// #ifndef _AFX_NO_DAO_SUPPORT
// #include <afxdao.h>			// MFC DAO database classes
// #endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxrich.h>

//
// There is a BUG in GeMenuItemInfo => doesn't want to work with WINVER = 5000, => 48 bytes
// we are redifining it here:
// Garbaged Menu Strings and how to Fix it...   igor1960  19:53 31 Jul '02  
 



#ifndef __MENUITEMINFO_LOCAL_DEFINED
#define __MENUITEMINFO_LOCAL_DEFINED
typedef struct tagMENUITEMINFO_LOCAL
{
	UINT cbSize;
	UINT fMask;
	UINT fType; // used if MIIM_TYPE (4.0) or MIIM_FTYPE (>4.0)
	UINT fState; // used if MIIM_STATE
	UINT wID; // used if MIIM_ID
	HMENU hSubMenu; // used if MIIM_SUBMENU
	HBITMAP hbmpChecked; // used if MIIM_CHECKMARKS
	HBITMAP hbmpUnchecked; // used if MIIM_CHECKMARKS
	DWORD dwItemData; // used if MIIM_DATA
	LPSTR dwTypeData; // used if MIIM_TYPE (4.0) or MIIM_STRING (>4.0)
	UINT cch; // used if MIIM_TYPE (4.0) or MIIM_STRING (>4.0)
} MENUITEMINFO_LOCAL;
#endif

#define _AfxGetDlgCtrlID(hWnd)          ((UINT)(WORD)::GetDlgCtrlID(hWnd))





//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__42A0B087_D792_11D4_B90C_000000000000__INCLUDED_)

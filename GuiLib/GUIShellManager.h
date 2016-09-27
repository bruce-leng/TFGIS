//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a part of the GUIControlBar Library
// Copyright (C) 1998-2000 GUISoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
// GUIShellManager.h: interface for the CGUIShellManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUISHELLMANAGER_H__E19B2D53_BD01_4B0B_94B9_EB32C246CFA5__INCLUDED_)
#define AFX_GUISHELLMANAGER_H__E19B2D53_BD01_4B0B_94B9_EB32C246CFA5__INCLUDED_

#ifndef _SHLOBJ_H_
#include <shlobj.h>
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GuiLib.h"


typedef struct _GUICBITEMINFO
{
	LPSHELLFOLDER  pParentFolder;
	LPITEMIDLIST   pidlFQ;
	LPITEMIDLIST   pidlRel;

	_GUICBITEMINFO()
	{
		pParentFolder = NULL;
		pidlFQ = NULL;
		pidlRel = NULL;
	}
}
GUICBITEMINFO, FAR *LPGUICBITEMINFO;

class GUILIBDLLEXPORT CGUIShellManager : public CObject
{
	friend class CGUIShellList;

// Construction
public:
	CGUIShellManager();
	virtual ~CGUIShellManager();

// Operations
public:
	BOOL BrowseForFolder (	CString& strOutFolder,
							CWnd* pWndParent = NULL,
							LPCTSTR lplszInitialFolder = NULL,
							LPCTSTR lpszTitle = NULL, 
							UINT ulFlags = BIF_RETURNONLYFSDIRS, // Same as flags in BROWSEINFO structure
							LPINT piFolderImage = NULL);

	LPITEMIDLIST CreateItem (UINT cbSize);
	void FreeItem (LPITEMIDLIST pidl);

	UINT GetItemCount (LPCITEMIDLIST pidl);
	UINT GetItemSize (LPCITEMIDLIST pidl);

	LPITEMIDLIST ConcatenateItem (LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2);
	LPITEMIDLIST CopyItem (LPCITEMIDLIST pidlSource);

	LPITEMIDLIST GetNextItem(LPCITEMIDLIST pidl);
	int GetParentItem (LPCITEMIDLIST lpidl, LPITEMIDLIST& lpidlParent);

	HRESULT ItemFromPath (LPCTSTR lpszPath, LPITEMIDLIST& pidl);

protected:
	static int CALLBACK BrowseCallbackProc (HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);

// Attributes
protected:
	LPMALLOC	m_pGUIMalloc;
	LPCTSTR		m_lpszInitialPath;	// Valid in BrowseForFolder only
};

extern CGUIShellManager* g_pShellManager;

#endif // !defined(AFX_GUISHELLMANAGER_H__E19B2D53_BD01_4B0B_94B9_EB32C246CFA5__INCLUDED_)

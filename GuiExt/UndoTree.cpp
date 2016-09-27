// UndoTree.cpp : implementation file
//

#include "stdafx.h"
#include "UndoTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUndoTree

CUndoTree::CUndoTree()
{
}

CUndoTree::~CUndoTree()
{
}


BEGIN_MESSAGE_MAP(CUndoTree, CListBox)
	//{{AFX_MSG_MAP(CUndoTree)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUndoTree message handlers

void CUndoTree::Test()
{
	ShowWindow(SW_HIDE);

	CString str;
	for (int i=0;i < 100;i++)
	{
		str.Format(_T("Test %d"), i);
		AddString( str);
	}
}
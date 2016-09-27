// LogEdit.cpp : implementation file
//

#include "stdafx.h"
#include "LogEdit.h"

#include <PROCESS.H>
#include "resource.h"
//#include "FolderFind.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogEdit

//##ModelId=3FFE4BD201C7
CLogEdit::CLogEdit()
{
}

//##ModelId=3FFE4BD201D4
CLogEdit::~CLogEdit()
{
}


BEGIN_MESSAGE_MAP(CLogEdit, CEdit)
	//{{AFX_MSG_MAP(CLogEdit)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogEdit message handlers

//##ModelId=3FFE4BD201E5
CLogInfo::CLogInfo()
{
	::InitializeCriticalSection( &m_csLock );
}

//##ModelId=3FFE4BD201F4
CLogInfo::~CLogInfo()
{
	::DeleteCriticalSection( &m_csLock );
}

BEGIN_MESSAGE_MAP(CLogInfo, CListBox)
	//{{AFX_MSG_MAP(CLogInfo)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogInfo message handlers

//##ModelId=3FFE4BD201E6
int CLogInfo::AddString(LPCTSTR lpszItem)
{
	::EnterCriticalSection( &m_csLock );
	int nSuc = CListBox::AddString(lpszItem);
//	int nSuc = InsertString(0, lpszItem);
	::LeaveCriticalSection( &m_csLock );
	return nSuc;
}


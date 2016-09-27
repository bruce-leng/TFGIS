#if !defined(AFX_LOGEDIT_H__9A2D882C_2139_421F_A9F7_3170A41D8A54__INCLUDED_)
#define AFX_LOGEDIT_H__9A2D882C_2139_421F_A9F7_3170A41D8A54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLogEdit window
#include "GUIExtStd.h"
//##ModelId=3FFE4BD201C5
class TILEXTCLASS CLogEdit : public CEdit
{
// Construction
public:
	//##ModelId=3FFE4BD201C7
	CLogEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	//##ModelId=3FFE4BD201D4
	virtual ~CLogEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLogEdit)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


//##ModelId=3FFE4BD201D6
class TILEXTCLASS CLogInfo : public CListBox
{
// Construction
public:
	//##ModelId=3FFE4BD201E5
	CLogInfo();

// Attributes
public:
	//##ModelId=3FFE4BD201E6
	int AddString(LPCTSTR lpszItem);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogInfo)
	//}}AFX_VIRTUAL

// Implementation
public:
	//##ModelId=3FFE4BD201F4
	virtual ~CLogInfo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLogInfo)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	//##ModelId=3FFE4BD201F6
	CRITICAL_SECTION	m_csLock;
};

/////////////////////////////////////////////////////////////////////////////


#endif // !defined(AFX_LOGEDIT_H__9A2D882C_2139_421F_A9F7_3170A41D8A54__INCLUDED_)

#if !defined(AFX_UNDOTREE_H__A4DC096E_4C7A_446B_8E64_74A787EC8584__INCLUDED_)

/************************************
  REVISION LOG ENTRY
  维护人员: Leng
  维护时间 2006-10-3 17:44:56
  说明: 用于维护工具栏中的"重复/撤消"操作
 ************************************/

#define AFX_UNDOTREE_H__A4DC096E_4C7A_446B_8E64_74A787EC8584__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UndoTree.h : header file
//

#include "GuiExtStd.h"
/////////////////////////////////////////////////////////////////////////////
// CUndoTree window

class TILEXTCLASS CUndoTree : public CListBox
{
// Construction
public:
	CUndoTree();

// Attributes
public:

// Operations
public:
	void Undo();
	void Redo();

	void Test();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUndoTree)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CUndoTree();

	// Generated message map functions
protected:
	//{{AFX_MSG(CUndoTree)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UNDOTREE_H__A4DC096E_4C7A_446B_8E64_74A787EC8584__INCLUDED_)

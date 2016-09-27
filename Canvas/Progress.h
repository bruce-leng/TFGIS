#if !defined(AFX_PROCESS_H__38A2A357_3D20_4F17_B9B7_0EE929E424AE__INCLUDED_)
#define AFX_PROCESS_H__38A2A357_3D20_4F17_B9B7_0EE929E424AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Process.h : header file
//
#include "TextProgressCtrl.h"
#include "TilCanvas.h"
/////////////////////////////////////////////////////////////////////////////
// CProgress dialog

class TILEXTCLASS CProgress : public CDialog
{
// Construction
public:
	CProgress(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProgress)
	enum { IDD = IDD_DIALOG_PROGRESS };
	CTextProgressCtrl	m_progress;
	//}}AFX_DATA

	int m_nTimer;

public:
	// 启动Canny算子边界查找对话框,对pCanvas进行边界查找
	void StartEdgeFind(CTilCanvas* pCanvas);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgress)
	public:
	virtual BOOL Create();
	virtual BOOL OnInitDialog();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProgress)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROCESS_H__38A2A357_3D20_4F17_B9B7_0EE929E424AE__INCLUDED_)

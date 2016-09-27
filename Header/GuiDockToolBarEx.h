#pragma once


#include "GuiLib.h"


class GUILIBDLLEXPORT CGuiDockToolBarEx : public CControlBar
{
// Construction
public:
	CGuiDockToolBarEx();

// Attributes
public:
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGuiDockToolBar)
	public:
	virtual BOOL Create(CWnd* pParentWnd,DWORD dwAling);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	virtual ~CGuiDockToolBarEx();

	// Generated message map functions
protected:
	int  m_Size;
	COLORREF m_clrface;
protected:
	//{{AFX_MSG(CGuiDockToolBarEx)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnSysColorChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
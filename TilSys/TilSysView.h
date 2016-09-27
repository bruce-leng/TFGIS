// TilSysView.h : interface of the CTilSysView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TILSYSVIEW_H__81A627F9_2DE2_434A_80DF_AD96683EB026__INCLUDED_)
#define AFX_TILSYSVIEW_H__81A627F9_2DE2_434A_80DF_AD96683EB026__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SysView.h"
class CTilSysView : public CSysView
{
protected: // create from serialization only
	CTilSysView();
	DECLARE_DYNCREATE(CTilSysView)

// Attributes
public:
	HCURSOR m_hCursor;
	int m_nLine;
	CPoint m_ptRuler[2];	// 用于画设置比例尺的线段
	double m_fRuler;		// 比例尺

	CTilSysDoc* GetDocument();
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTilSysView)
	protected:
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTilSysView();
protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTilSysView)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnViewRulerSet();
	afx_msg void OnUpdateViewRulerSet(CCmdUI* pCmdUI);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TilSysView.cpp
inline CTilSysDoc* CTilSysView::GetDocument()
   { return (CTilSysDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TILSYSVIEW_H__81A627F9_2DE2_434A_80DF_AD96683EB026__INCLUDED_)

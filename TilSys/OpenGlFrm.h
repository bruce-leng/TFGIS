#if !defined(AFX_OPENGLFRM_H__21EA6A1D_7C06_4F44_9EB9_8A2C7088A0BE__INCLUDED_)
#define AFX_OPENGLFRM_H__21EA6A1D_7C06_4F44_9EB9_8A2C7088A0BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OpenGlFrm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COpenGlFrm frame

class COpenGlFrm : public CMDIChildWnd
{
	DECLARE_DYNCREATE(COpenGlFrm)
protected:
	COpenGlFrm();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COpenGlFrm)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~COpenGlFrm();

	// Generated message map functions
	//{{AFX_MSG(COpenGlFrm)
	afx_msg void OnClose();
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPENGLFRM_H__21EA6A1D_7C06_4F44_9EB9_8A2C7088A0BE__INCLUDED_)

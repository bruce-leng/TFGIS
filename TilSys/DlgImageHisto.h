#if !defined(AFX_DlgImageHisto_H__4EA4FEE4_AE24_11D6_BB83_E5D7B698D37E__INCLUDED_)
#define AFX_DlgImageHisto_H__4EA4FEE4_AE24_11D6_BB83_E5D7B698D37E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgImageHisto.h : header file
//

#include "xTargetButton.h"

/////////////////////////////////////////////////////////////////////////////
// DlgImageHisto dialog

class DlgImageHisto : public CDialogBar
{
// Construction
public:
	DlgImageHisto();
	virtual ~DlgImageHisto();

	BOOL Create(CWnd* pParentWnd, LPCTSTR lpszTemplateName, UINT nStyle, UINT nID );
	BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID );

	void InitDlg();

	//{{AFX_VIRTUAL(DlgImageHisto)
	protected:
	//}}AFX_VIRTUAL
	
// Implementation
protected:
	CxTargetButton	m_r;
	CxTargetButton	m_g;
	CxTargetButton	m_b;
	CxTargetButton	m_gray;
	int	m_ren;
	int	m_gen;
	int	m_ben;
	int	m_grayen;
	int	m_logen;
	bool m_initok;
	// Generated message map functions
	//{{AFX_MSG(DlgImageHisto)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGHISTO_H__4EA4FEE4_AE24_11D6_BB83_E5D7B698D37E__INCLUDED_)

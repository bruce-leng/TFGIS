#if !defined(AFX_DLGRULERSET_H__D3269BEA_0B93_4720_8A15_610C01BF7C43__INCLUDED_)
#define AFX_DLGRULERSET_H__D3269BEA_0B93_4720_8A15_610C01BF7C43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRulerSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DlgRulerSet dialog

class DlgRulerSet : public CDialog
{
// Construction
public:
	DlgRulerSet(CWnd* pParent = NULL);   // standard constructor

	float m_fCurRuler;		// 当前比例尺
	float m_fRealRuler;		// 实际比例尺
// Dialog Data
	//{{AFX_DATA(DlgRulerSet)
	enum { IDD = IDD_IMAGE_RULER_SET };
	float	m_fPixelWidth;	// 图像距离
	float	m_fRealWidth;	// 实际距离
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgRulerSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgRulerSet)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGRULERSET_H__D3269BEA_0B93_4720_8A15_610C01BF7C43__INCLUDED_)

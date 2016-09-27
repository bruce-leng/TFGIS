#if !defined(AFX_DLGOTHEROPTIONS_H__84A6ADCA_096D_4BD0_829D_0A6ED03C675D__INCLUDED_)
#define AFX_DLGOTHEROPTIONS_H__84A6ADCA_096D_4BD0_829D_0A6ED03C675D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgOtherOptions.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgOtherOptions dialog

class CDlgOtherOptions : public CDialog
{
// Construction
public:
	CDlgOtherOptions(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgOtherOptions)
	enum { IDD = IDD_DLG_OTHER_OPTIONS };
	int		m_nPrintTarget;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgOtherOptions)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgOtherOptions)
	afx_msg void OnOk();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOTHEROPTIONS_H__84A6ADCA_096D_4BD0_829D_0A6ED03C675D__INCLUDED_)

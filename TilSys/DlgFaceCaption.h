#if !defined(AFX_FACECAPTION_H__F478305C_EE6B_4CBF_971F_18B356468A89__INCLUDED_)

/************************************
  REVISION LOG ENTRY
  维护人员: ...
  维护时间 2011-12-21 11:37:15
  说明: 掌子面属性设置对话框
 ************************************/

#define AFX_FACECAPTION_H__F478305C_EE6B_4CBF_971F_18B356468A89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FaceCaption.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgFaceCaption dialog

class CDlgFaceCaption : public CDialog
{
// Construction
public:
	CDlgFaceCaption(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgFaceCaption)
	enum { IDD = IDD_FACE_CAPTION_SET };
	CString	m_strFaceDK;
	CString	m_strFaceMile;
	//}}AFX_DATA

	CString m_strSrcFileName;
	double m_dbFaceMile;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFaceCaption)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFaceCaption)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSetfocusFaceMile();
	afx_msg void OnKillfocusFaceMile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FACECAPTION_H__F478305C_EE6B_4CBF_971F_18B356468A89__INCLUDED_)

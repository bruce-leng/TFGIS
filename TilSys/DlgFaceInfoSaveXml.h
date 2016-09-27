#if !defined(AFX_DLGFACEINFOSAVEXML_H__F84D2DF4_3F18_4E4B_A081_C1847A4746DE__INCLUDED_)
#define AFX_DLGFACEINFOSAVEXML_H__F84D2DF4_3F18_4E4B_A081_C1847A4746DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFaceInfoSaveXml.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgFaceInfoSaveXml dialog

class CDlgFaceInfoSaveXml : public CDialog
{
// Construction
public:
	CDlgFaceInfoSaveXml(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgFaceInfoSaveXml)
	enum { IDD = IDD_FACE_INFO_SAVE_XML };
	CString	m_strDstXmlPathFile;
	CString	m_strSrcXmlPathFile;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFaceInfoSaveXml)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFaceInfoSaveXml)
	afx_msg void OnBtnSrcXmlPathFile();
	afx_msg void OnBtnDstXmlPathFile();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFACEINFOSAVEXML_H__F84D2DF4_3F18_4E4B_A081_C1847A4746DE__INCLUDED_)

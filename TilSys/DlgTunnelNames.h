#if !defined(AFX_DLGTUNNELNAMES_H__0B306BD8_B601_44C1_9BB3_BEFC902E79C5__INCLUDED_)
#define AFX_DLGTUNNELNAMES_H__0B306BD8_B601_44C1_9BB3_BEFC902E79C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTunnelNames.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// DlgTunnelNames dialog

class DlgTunnelNames : public CDialog
{
// Construction
public:
	DlgTunnelNames(CWnd* pParent = NULL);   // standard constructor
	~DlgTunnelNames();

	void SetPrjCaption(PRJCAPTION& prjCaption);
	PRJCAPTION GetPrjCaption();
private:
	PRJCAPTION m_prjCaption;

// Dialog Data
	//{{AFX_DATA(DlgTunnelNames)
	enum { IDD = IDD_TUNNEL_NAMES };
	CString	m_strLineName;
	CString	m_strPartIdx;
	CString	m_strTunName;
	CString	m_strPartName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgTunnelNames)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:


	// Generated message map functions
	//{{AFX_MSG(DlgTunnelNames)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTUNNELNAMES_H__0B306BD8_B601_44C1_9BB3_BEFC902E79C5__INCLUDED_)

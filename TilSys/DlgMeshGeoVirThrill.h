#if !defined(AFX_DLGMESHGEOVIRTHRILL_H__6A835BF0_DB92_4D19_AA4F_E7473D358F29__INCLUDED_)

/************************************
  REVISION LOG ENTRY
  维护人员: ...
  维护时间 2009-12-21 11:35:16
  说明: 设置虚拟钻孔对话框
 ************************************/

#define AFX_DLGMESHGEOVIRTHRILL_H__6A835BF0_DB92_4D19_AA4F_E7473D358F29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMeshGeoVirThrill.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMeshGeoVirThrill dialog

class CDlgMeshGeoVirThrill : public CDialog
{
// Construction
public:
	CDlgMeshGeoVirThrill(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgMeshGeoVirThrill)
	enum { IDD = IDD_MESH_VIR_GEO_THRILL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMeshGeoVirThrill)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMeshGeoVirThrill)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMESHGEOVIRTHRILL_H__6A835BF0_DB92_4D19_AA4F_E7473D358F29__INCLUDED_)

#if !defined(AFX_DLGMESHGEOPREDICT_H__0B6DF88C_B8F9_4927_A37F_AF7E2C77650F__INCLUDED_)

/************************************
  REVISION LOG ENTRY
  维护人员: ...
  维护时间 2009-12-21 11:35:58
  说明: 地质预测
 ************************************/

#define AFX_DLGMESHGEOPREDICT_H__0B6DF88C_B8F9_4927_A37F_AF7E2C77650F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMeshGeoPredict.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMeshGeoPredict dialog

class CDlgMeshGeoPredict : public CDialog
{
// Construction
public:
	CDlgMeshGeoPredict(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgMeshGeoPredict)
	enum { IDD = IDD_MESH_GEO_PREDICT };
	float	m_fPredictDK;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMeshGeoPredict)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMeshGeoPredict)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMESHGEOPREDICT_H__0B6DF88C_B8F9_4927_A37F_AF7E2C77650F__INCLUDED_)

#if !defined(AFX_DLGMESHSECTIONNORMAL_H__D8BEF292_F6E0_4037_BAA4_E11FED7EBEC3__INCLUDED_)
#define AFX_DLGMESHSECTIONNORMAL_H__D8BEF292_F6E0_4037_BAA4_E11FED7EBEC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMeshSectionNormal.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMeshSectionNormal dialog

class CDlgMeshSectionNormal : public CDialog
{
// Construction
public:
	CDlgMeshSectionNormal(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgMeshSectionNormal)
	enum { IDD = IDD_MESH_SECTION_NORMAL };
	int		m_nCrossSectionDK;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMeshSectionNormal)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMeshSectionNormal)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMESHSECTIONNORMAL_H__D8BEF292_F6E0_4037_BAA4_E11FED7EBEC3__INCLUDED_)

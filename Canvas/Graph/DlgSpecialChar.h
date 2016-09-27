#if !defined(AFX_SPECIALDLG_H__93EE3AD2_968D_4326_9E73_1D983C499449__INCLUDED_)
#define AFX_SPECIALDLG_H__93EE3AD2_968D_4326_9E73_1D983C499449__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpecialDlg.h : header file
//

#include "..\resource.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgSpecialChar dialog

class CDlgSpecialChar : public CDialog
{
// Construction
public:
	void DrawSelect();
	void InitList();
	bool FindList(int i, int j);
	void InitData();
	CDlgSpecialChar(CWnd* pParent = NULL);   // standard constructor

  CStringList  m_strList1;  //标点符号
	CStringList  m_strList2;  //特殊符号
	CStringList  m_strList3;  //数学符号
	CStringList  m_strList4;  //单位符号
	CStringList  m_strList5 ;  //数字序号
	CStringList  m_strList6;  //拼音

	int num;                 //以上六个选择的是哪一个
	int SelectY;             //鼠标左键选择的字符的Y
	int SelectX;             //鼠标左键选择的字符的X
	CString m_strSelText;    //当前要显示的字符(鼠标移动到的字符)
	CString m_select;        //鼠标左键选择的字符
// Dialog Data
	//{{AFX_DATA(CDlgSpecialChar)
	enum { IDD = IDD_DLG_SPECIALCHAR};
	CTabCtrl	m_tab;
	//}}AFX_DATA



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSpecialChar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	CEdit* m_pEdit;
	// Generated message map functions
	//{{AFX_MSG(CDlgSpecialChar)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnInsert();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPECIALDLG_H__93EE3AD2_968D_4326_9E73_1D983C499449__INCLUDED_)

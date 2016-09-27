#if !defined(AFX_CANVASSECTION_H__CD869A20_EE87_4D5C_B995_8491FB7A64CB__INCLUDED_)
#define AFX_CANVASSECTION_H__CD869A20_EE87_4D5C_B995_8491FB7A64CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CanvasSection.h : header file
// ������������ͼ

#include "GUIExtStd.h"


class CTilGraph;
/////////////////////////////////////////////////////////////////////////////
// CCanvasSection window

class TILEXTCLASS CCanvasSection : public CStatic
{
// Construction
public:
	CCanvasSection();

// Attributes
public:

	// ָ������ͼΪ�����滹��������(0:����ʾ,1:������,2:������)
	int	 m_nDrawID;
	long m_lDK;
	long m_lStartDK, m_lEndDK;
	CTilGraph* m_pGraphSection;
	CTilGraph* m_pProfile;
// Operations
public:
	void Redraw(int nDrawID);

	// ���ú����
	void SetNormalSection(long lDK, CTilGraph& graphMgr);
	void DrawNormalSection(CDC* pDC);

	// �����ݶ���
	void SetProfile(long lStartDK, long lEndDK, CTilGraph& graphMgr);
	void DrawProfile(CDC* pDC);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCanvasSection)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCanvasSection();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCanvasSection)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSectionNormal();
	afx_msg void OnUpdateSectionNormal(CCmdUI* pCmdUI);
	afx_msg void OnSectionProfile();
	afx_msg void OnUpdateSectionProfile(CCmdUI* pCmdUI);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	// ��ָ�������ڻ���ͼ��
	void draw_graph(CDC* pDC, CRect& rect, CTilGraph& graphMgr);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CANVASSECTION_H__CD869A20_EE87_4D5C_B995_8491FB7A64CB__INCLUDED_)

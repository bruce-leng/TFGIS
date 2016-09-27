#pragma once
#include "GUIExtStd.h"
#include "guicontrolbar.h"
#include "GuiTabWnd.h"
#include "GuiContainer.h"
#include "GuiMiniTool.h"
#include "DragTree2.h"
#include "TilCanvasMgr.h"

class TILEXTCLASS CGuiSolExplorer :	public CGuiControlBar
{
protected:
	CGuiTabWnd	m_TabSolExplorer;
	CDragTree2  m_TreeSolExplorer;
	CTreeCtrl   m_TreeClassView;
	CImageList	m_imgList;
	CGuiContainer m_ctSolution;
	CGuiMiniTool  m_miSolution;

	CGuiContainer m_ctClassView;
	CGuiMiniTool  m_miClassView;

public:
	void Onproperties();
	void OnNewFolder();
public:
	CGuiSolExplorer(void);
	virtual ~CGuiSolExplorer(void);
	void FillerSolExplorer();
	void FillTreeClassView();
	void InsertFileToSolExplorer(const CString& fileName);
 	void InsertProjToSolExplorer(const CString& projName);
	void InsertProjToSolExplorer(CTilCanvasMgr* pProject);
	//void InsertProjToSolExplorer();
	int CreatContExplorer();
	int CreatContClassView();
public:
	//{{AFX_MSG(CComboListCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNcRButtonDown(UINT nHitTest, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

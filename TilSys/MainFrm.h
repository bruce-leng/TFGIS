// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__D4F9EECE_F8C0_4D86_8765_8C5723F61379__INCLUDED_)
#define AFX_MAINFRM_H__D4F9EECE_F8C0_4D86_8765_8C5723F61379__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GuiMDIFrame.h"
#include "GuiSolExplorer.h"
#include "GuiDataDraw.h"
#include "GuiOutPut.h"
#include "GuiServerExplorer.h"
#include "UndoTree.h"
#include "StyleBar.h"
#include "Graph\DLineProperty.h"
#include "DlgImageHisto.h"

#define CURVE_WINDOW 0
#define SERVER_WINDOW 0
class CMainFrame : public CGuiMDIFrame
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
protected:
	CGuiSolExplorer  m_WorkSpaceBar;	// 项目文件操作台子窗口
#if CURVE_WINDOW
	CGuiDataDraw		 m_Curve;		// 画曲线子窗口
#endif
	CGuiOutPut			 m_OutPut;		// 输出信息子窗口
	CGuiServerExplorer	 m_ServerBar;	// 服务器子窗口

	CUndoTree		 m_UndoTree;		// 工具栏上"重复/撤消"操作管理
	CGuiToolBarWnd   m_wndToolOpenGL;	// OpenGL工具条
	CGuiToolBarWnd   m_wndToolGraph;	// Palette工具条

public:
	CStyleBar	m_wndTextBar;			// 文字工具条
	CDLineProperty* m_pDLineProperty;	// 图元属性页
	CGuiComboBoxExt	 m_ComboCurLayer;	// 设置当前处于编辑状态的图形层

//	DlgImageHisto m_HistoBar;
// Operations
public:

	CGuiSolExplorer&	GetWorkSpace()	{ return m_WorkSpaceBar; };
#if CURVE_WINDOW
	CGuiDataDraw&		GetDataCurve()	{ return m_Curve; };
#endif
	CGuiOutPut&			GetOutPut()		{ return m_OutPut; };
	CGuiServerExplorer& GetServerExplorer() { return m_ServerBar; };

	inline void SetFontBox() { m_wndTextBar.IniFont(); };	
	inline void SetCurtab(int nCur) { m_MdiTabbed.SetCurtab(nCur); };	
	inline int  GetCurtab() { return m_MdiTabbed.GetCurtab();	};
	void InsertProjToSolExplorer(CTilCanvasMgr* pProject)
		 { m_WorkSpaceBar.InsertProjToSolExplorer(pProject); };
	void InsertFileToSolExplorer(const CString& fileName)
		{	m_WorkSpaceBar.InsertFileToSolExplorer (fileName); };
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	void SetMousePosTextColor(CPoint Logpoint, RGBQUAD color);
	void ShowFontBar(BOOL isShow=true);
	void ShowHistoBar(BOOL isShow=true);

protected:  // control bar embedded members

// Generated message map functions
protected:
	afx_msg LRESULT OnShowFontBar(WPARAM, LPARAM);
	afx_msg LRESULT OnUpdateGraphTool(WPARAM, LPARAM);
	afx_msg LRESULT OnUpdateOpenGLTool(WPARAM, LPARAM);
	afx_msg LRESULT OnSetCurGraphLayer(WPARAM, LPARAM);
	afx_msg LRESULT OnLogInfo(WPARAM, LPARAM);
//	afx_msg LRESULT OnRecogTexture(WPARAM, LPARAM);
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWorkspace();
	afx_msg void OnCurve();
	afx_msg void OnServer();
	afx_msg void OnOutput();
	afx_msg void OnClassview();
	afx_msg void OnUndo();
	afx_msg void OnRedo();
	afx_msg void OnViewToolbar();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnGraphPara();
	afx_msg void OnUpdateGraphPara(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewToolbar(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewGraphLayerCur(CCmdUI* pCmdUI);
	afx_msg void OnViewHistogram();
	afx_msg void OnUpdateViewHistogram(CCmdUI* pCmdUI);
	afx_msg void OnUpdateWorkspace(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOutput(CCmdUI* pCmdUI);
	afx_msg void OnOtherOptions();
	afx_msg void OnUpdateOtherOptions(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__D4F9EECE_F8C0_4D86_8765_8C5723F61379__INCLUDED_)

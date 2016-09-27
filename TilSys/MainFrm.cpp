// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "TilSys.h"
#include "MainFrm.h"
#include "Splash.h"
#include "TilSysDoc.h"
#include "GraphDefine.h"
#include "DlgOtherOptions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_MESSAGE(WM_SHOW_FONT_BAR, OnShowFontBar)
	ON_MESSAGE(WM_UPDATE_GRAPH_TOOL, OnUpdateGraphTool)
	ON_MESSAGE(WM_UPDATE_OPENGL_TOOL, OnUpdateOpenGLTool)
	ON_MESSAGE(WM_SET_CUR_GRAPH_LAYER, OnSetCurGraphLayer)
	ON_MESSAGE(WM_LOG_INFO, OnLogInfo)
//	ON_MESSAGE(WM_RECOG_TEXTURE, OnRecogTexture)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_WORKSPACE, OnWorkspace)
	ON_COMMAND(ID_CURVE, OnCurve)
	ON_COMMAND(ID_SERVER, OnServer)
	ON_COMMAND(ID_OUTPUT, OnOutput)
	ON_COMMAND(ID_CLASSVIEW, OnClassview)
	ON_COMMAND(ID_UNDO, OnUndo)
	ON_COMMAND(ID_REDO, OnRedo)
	ON_COMMAND(ID_VIEW_TOOLBAR, OnViewToolbar)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_COMMAND(ID_GRAPH_PARA, OnGraphPara)
	ON_UPDATE_COMMAND_UI(ID_GRAPH_PARA, OnUpdateGraphPara)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR, OnUpdateViewToolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_GRAPH_LAYER_CUR, OnUpdateViewGraphLayerCur)
	ON_COMMAND(ID_VIEW_HISTOGRAM, OnViewHistogram)
	ON_UPDATE_COMMAND_UI(ID_VIEW_HISTOGRAM, OnUpdateViewHistogram)
	ON_UPDATE_COMMAND_UI(ID_WORKSPACE, OnUpdateWorkspace)
	ON_UPDATE_COMMAND_UI(ID_OUTPUT, OnUpdateOutput)
	ON_COMMAND(ID_OTHER_OPTIONS, OnOtherOptions)
	ON_UPDATE_COMMAND_UI(ID_OTHER_OPTIONS, OnUpdateOtherOptions)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_COORD,		// 逻辑坐标
	ID_INDICATOR_RGBA,		// RGBA颜色灰度值
//	ID_INDICATOR_CAPS,
//	ID_INDICATOR_NUM,
//	ID_INDICATOR_SCRL,
};

static UINT Toolbars[] = {
	IDR_MAINFRAME,
	IDR_TOOLBARS/*,
	IDR_NEWITEM*/
	
};
/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame() : m_pDLineProperty(NULL)
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
	if (m_pDLineProperty)
		delete m_pDLineProperty;
}

void CMainFrame::OnUndo() {}
void CMainFrame::OnRedo() {}
void CMainFrame::OnClassview() {}

void CMainFrame::OnViewToolbar() { ShowHideBar(&m_wndToolBar); }
void CMainFrame::OnWorkspace() { ShowHideBar(&m_WorkSpaceBar); }
void CMainFrame::OnCurve() 
{ 
#if CURVE_WINDOW
	ShowHideBar(&m_Curve);
#endif
}
void CMainFrame::OnServer() 
{	
#if SERVER_WINDOW 
	ShowHideBar(&m_ServerBar); 
#endif
}
void CMainFrame::OnOutput() {	ShowHideBar(&m_OutPut); }

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CSplashWnd::ShowSplashScreen(this);

	if (CGuiMDIFrame::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	InitMenu(IDR_MAINFRAME);
	InitToolBar(IDR_MAINFRAME);
	InitStatusBar(indicators,sizeof(indicators)/sizeof(UINT));

	// TODO: Delete these three lines if you don't want the toolbar to
	if (!m_wndToolOpenGL.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(0,0,0,0),IDR_TOOL_OPENGL) ||
		!m_wndToolOpenGL.LoadToolBar(IDR_TOOL_OPENGL))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	if (!m_wndToolGraph.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(0,0,0,0),IDR_TOOL_GRAPH) ||
		!m_wndToolGraph.LoadToolBar(IDR_TOOL_GRAPH))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	if (!m_wndTextBar.Create(this, WS_CHILD | CBRS_ALIGN_TOP |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, IDR_STYLE_BAR))
	{
		TRACE0("Failed to create text stylebar\n");
		return -1;
	}
	//  be dockable
	if (!m_WorkSpaceBar.Create(_T("工作台"),WS_CHILD | WS_VISIBLE, this, 0x999))
	{
		TRACE0("Failed to create m_WorkSpaceBar\n");
		return -1;      
	}
#if CURVE_WINDOW
	if (!m_Curve.Create(_T("曲线图"),WS_CHILD | WS_VISIBLE, this, 0x998))
	{
		TRACE0("Failed to create m_WorkSpaceBar\n");
		return -1;     
	}
#endif
	if (!m_OutPut.Create(_T("状态信息"),WS_CHILD | WS_VISIBLE, this, 0x997))
	{
		TRACE0("Failed to create m_WorkSpaceBar\n");
		return -1;      
	}
#if SERVER_WINDOW
	if (!m_ServerBar.Create(_T("服务器"),WS_CHILD | WS_VISIBLE, this, 0x996))
	{
		TRACE0("Failed to create m_WorkSpaceBar\n");
		return -1;      
	}
#endif

	m_wndToolBar.SetWindowText("工具条");
	m_wndToolOpenGL.SetWindowText("OpenGL工具条");
	m_wndToolGraph.SetWindowText("Graph工具条");
	m_wndTextBar.SetWindowText("文字工具条");
//	m_pDLineProperty->SetWindowText("图元属性页");

	const DWORD dwStyle =  LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL ;
	m_UndoTree.Create (dwStyle, CRect(0,0,100,100), this, 1);
	m_UndoTree.Test(); // 测试"重复"工具条按钮

	// 设置工具条上的下拉菜单
	//m_wndToolBar.SetButtonDropDown(ID_UNDO, 7, &m_UndoTree, _T("重复"));
	m_wndToolBar.SetButtonDropDown(ID_CLASSVIEW, 7, IDR_MENUTOOLBARS);

	// 设置当前处理的图形层
	m_wndToolBar.CreateCombo(&m_ComboCurLayer, ID_VIEW_GRAPH_LAYER_CUR, 90);
	m_ComboCurLayer.InsertString(GRAPH_LAYER_TUNARC, "隧道拱形层");
	m_ComboCurLayer.InsertString(GRAPH_LAYER_ROCKS, "岩层目标层");
	m_ComboCurLayer.InsertString(GRAPH_LAYER_JOINT, "岩层节理层");
	m_ComboCurLayer.InsertString(GRAPH_LAYER_OTHER, "文字标注层");
	m_ComboCurLayer.SetCurSel(GRAPH_LAYER_JOINT);
	m_ComboCurLayer.m_nCurIdx = GRAPH_LAYER_JOINT;

	m_WorkSpaceBar.SetBarStyle(m_WorkSpaceBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
#if CURVE_WINDOW
	m_Curve.SetBarStyle(m_Curve.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
#endif
	m_OutPut.SetBarStyle(m_OutPut.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
#if SERVER_WINDOW
	m_ServerBar.SetBarStyle(m_ServerBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
#endif

	EnableDocking(CBRS_ALIGN_ANY);

	m_wndToolOpenGL.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolGraph.EnableDocking(CBRS_ALIGN_ANY);
	m_wndTextBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_LEFT);
	m_WorkSpaceBar.EnableDocking(CBRS_ALIGN_ANY);
#if CURVE_WINDOW
	m_Curve.EnableDocking(CBRS_ALIGN_ANY);
#endif
	m_OutPut.EnableDocking(CBRS_ALIGN_BOTTOM);
#if SERVER_WINDOW
	m_ServerBar.EnableDocking(CBRS_ALIGN_ANY);
#endif

	m_wndToolBar.SetMenuContext(IDR_MENUTOOLBARS);
	m_WorkSpaceBar.SetMenuContext(IDR_MENUTOOLBARS);
#if CURVE_WINDOW
	m_Curve.SetMenuContext(IDR_MENUTOOLBARS);
#endif
#if SERVER_WINDOW
	m_ServerBar.SetMenuContext(IDR_MENUTOOLBARS);
#endif

	//直方图栏
//	if (!m_HistoBar.Create(this, IDD_IMAGE_HISTO, CBRS_NOALIGN, IDD_IMAGE_HISTO))
//	{
//		TRACE0( "Failed   to   create   DlgBar\n "); 
//        return   -1;             //   Fail   to   create. 
//	}
//	m_HistoBar.ShowWindow(SW_HIDE);
//	m_HistoBar.EnableDocking(FALSE);
//	RECT r;
//	GetWindowRect(&r);
//	FloatControlBar(&m_HistoBar,CPoint(r.right-300,r.top+80),CBRS_ALIGN_TOP);

	// 工具条及子窗口停靠位置
	DockControlBar(&m_wndToolBar,m_dockTop);
	DockControlBar(&m_wndToolOpenGL,m_dockTop);
	DockControlBar(&m_wndToolGraph,m_dockTop);
	DockControlBar(&m_wndTextBar, m_dockTop);
	DockControlBar(&m_WorkSpaceBar, AFX_IDW_DOCKBAR_LEFT);
#if CURVE_WINDOW
	DockControlBar(&m_Curve, AFX_IDW_DOCKBAR_RIGHT);
#endif
	DockControlBar(&m_OutPut, AFX_IDW_DOCKBAR_BOTTOM);
#if SERVER_WINDOW
	DockControlBar(&m_ServerBar, AFX_IDW_DOCKBAR_RIGHT);
#endif

	FloatControlBar(&m_wndTextBar, CPoint(300,10));

	sProfile = _T("TilSys");
	LoadBars();
	InitMDITabbed();
	m_MdiTabbed.SetTabsMenu(IDR_MENUTABS);
	m_MdiTabbed.SetTabMenu(0,0);

	// 显示工具条上的组合下拉菜单各项的左边的小图
	#define _countof(array) (sizeof(array)/sizeof(array[0]))
	m_wndCool.LoadToolbars(Toolbars, _countof(Toolbars));
	RecalcLayout();

	m_pDLineProperty = new CDLineProperty("图元属性页");
	if (!m_pDLineProperty->Create(this, WS_SYSMENU | WS_POPUP | WS_CAPTION | DS_MODALFRAME, WS_EX_TOOLWINDOW))
	{
		TRACE0("Failed to create lineproperty toolbar\n");
		return -1;
	}
	SetTimer(1, 1000, NULL);
	
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.cy = ::GetSystemMetrics(SM_CYSCREEN); 
	cs.cx = ::GetSystemMetrics(SM_CXSCREEN); 
	cs.y = ::GetSystemMetrics(SM_CYSCREEN); 
	cs.x = -::GetSystemMetrics(SM_CYSCREEN);
	if( !CGuiMDIFrame::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CGuiMDIFrame::PreTranslateMessage(pMsg);
}

LRESULT CMainFrame::OnShowFontBar(WPARAM wParam, LPARAM lParam)
{
	LOGFONT* pLf=(LOGFONT*)lParam;
	if (pLf != NULL)
	{
		// 如果已经设置了字段，则在字体属性工具条中显示属性
		CString strIdx;
		strIdx.Format("%d",pLf->lfHeight/5);
		m_wndTextBar.m_wndFontSizes.SelectString(-1, strIdx);
		m_wndTextBar.m_wndFontNames.SelectString(-1, pLf->lfFaceName);
		m_wndTextBar.isBoldUp = (pLf->lfWeight==FW_BOLD)?0:1;
		m_wndTextBar.isItalicUp = (pLf->lfItalic==1)?0:1;
		m_wndTextBar.isUnderlineUp = (pLf->lfUnderline==1)?0:1;
	}
	ShowFontBar(wParam);
	return 0;
}

// 设置当前图形层,此处只能选择将层转换成目标层或节理层
LRESULT CMainFrame::OnSetCurGraphLayer(WPARAM wParam, LPARAM lParam)
{
	int nGraphLayerIdx = int(wParam);

	// 设置转换后的图元的颜色
	COLORREF rgb;
	if (nGraphLayerIdx == GRAPH_LAYER_JOINT)
		rgb = RGB(255, 0, 0);	// 红色
	else if (nGraphLayerIdx == GRAPH_LAYER_ROCKS)
		rgb = RGB(0, 0, 255);	// 蓝色

	// 将当前画布的被选择的图元加入到当前图形层中
	int i,j;
	CTilGraph* pGraphMgr = GetCanvasMgr()->GetCurCanvas()->GetGraph();
	CGraph* pGraph1 = pGraphMgr->GetGraphByIdx(nGraphLayerIdx);
	BOOL bCurGraphLayerChanged = FALSE;	// 标记是否有选中的图形改变了图层归属
	for (i=0; i<pGraphMgr->Size(); i++)
	{
		if (i == nGraphLayerIdx)
			continue;

		CGraph* pGraph2 = pGraphMgr->GetGraphByIdx(i);
		for (j=pGraph2->Size()-1; j>=0; j--)
		{
			CUnit* pUnit = pGraph2->GetUnitByIdx(j);
			if (pUnit->IsSelected())
			{
				pGraph2->DeleteUnit(j);

				pUnit->SetPreLink(-1);
				pUnit->SetNextLink(-1);
				pUnit->m_PenColor = rgb;
				pGraph1->PushBackUnit(pUnit);

				bCurGraphLayerChanged = TRUE;
			}
		}
	}

	// 如果选中的图形改变了图层归属,则设置脏标记
	if (bCurGraphLayerChanged)
	{
		pGraphMgr->SetModifiedFlag(TRUE);

		// 颜色更改后刷新主窗口
		CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
		CMDIChildWnd *pChild = (CMDIChildWnd *) pFrame->GetActiveFrame();
		CView *pView = (CView *) pChild->GetActiveView();
		pView->Invalidate(FALSE);
	}

	for (i=0; i<GetCanvasMgr()->Size(); i++)
	{
		// 设置当前图形层
		CTilGraph* pGraphMgr = GetCanvasMgr()->GetCanvasByIdx(i)->GetGraph();
		pGraphMgr->SetCurIdx(nGraphLayerIdx);
	}


	return 0;
}

LRESULT CMainFrame::OnUpdateGraphTool(WPARAM wParam, LPARAM lParam)
{
	// TRUE if the child is being activated and FALSE if it is being deactivated
	if (wParam)
	{
		ShowControlBar(&m_wndToolGraph, TRUE, FALSE);
		m_MdiTabbed.SetCurtab(0);
#if CURVE_WINDOW
		m_Curve.SetCurTab(0);
#endif
	}
	else
	{
		ShowControlBar(&m_wndToolGraph, FALSE, FALSE);
	}

	return 0;
}

LRESULT CMainFrame::OnUpdateOpenGLTool(WPARAM wParam, LPARAM lParam)
{
	// TRUE if the child is being activated and FALSE if it is being deactivated
	if (wParam)
	{
		ShowControlBar(&m_wndToolOpenGL, TRUE, FALSE);
		m_MdiTabbed.SetCurtab(1);
#if CURVE_WINDOW
		m_Curve.SetCurTab(1);
#endif
	}
	else
	{
		ShowControlBar(&m_wndToolOpenGL, FALSE, FALSE);
	}

	return 0;
}

LRESULT CMainFrame::OnLogInfo(WPARAM wParam, LPARAM lParam)
{
	int nIdx = int(wParam);
	CTilCanvas* pCanvas = GetCanvasMgr()->GetCanvasByIdx(nIdx);
	LogInfo(pCanvas->GetImage()->GetPathName());
	
	return 0;
}

void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CTilCanvasMgr* pMgr = GetCanvasMgr();

	int nRet1, nRet2;

	// 保存所有图形、图像文件
	for (int i = 0; i < pMgr->Size(); i ++)
	{
		CTilCanvas* pCanvas = pMgr->GetCanvasByIdx(i);
		if (pCanvas->GetImage()->IsModified()
			|| pCanvas->GetGraph()->IsModified())
		{
			nRet1 = AfxMessageBox("是否保存所有文件?", MB_YESNOCANCEL);
			if (nRet1 == IDYES)
			{
				((CTilSysApp*)AfxGetApp())->OnFileSaveAll();
				break;
			}
			else if (nRet1 == IDNO)
				break;
			else if (nRet1 == IDCANCEL)
				return;
		}
	}

	// 保存项目文件
	if (pMgr->IsModified())
	{
		nRet2 = AfxMessageBox("是否保存项目?", MB_YESNOCANCEL);
		if (nRet2 == IDYES)
			((CTilSysApp*)AfxGetApp())->OnProjectSave();
		else if (nRet2 == IDCANCEL)
			return;
	}

	CMDIFrameWnd::OnClose();
}

void CMainFrame::SetMousePosTextColor(CPoint Logpoint, RGBQUAD color)
{
	CString strText;
	strText.Format("坐标(cm):(%d,%d)",Logpoint.x,Logpoint.y);
	int nIndex=m_wndStatusBar.CommandToIndex(ID_INDICATOR_COORD);
	m_wndStatusBar.SetPaneText(nIndex,strText);
	
	CWindowDC dc(&m_wndStatusBar);
	CSize sizeText=dc.GetTextExtent(strText);
	m_wndStatusBar.SetPaneInfo(nIndex,ID_INDICATOR_COORD,SBPS_NORMAL,sizeText.cx);

// 	strText.Format("指针处颜色(R:G:B):(%d,%d,%d)",
// 		color.rgbRed, color.rgbGreen, color.rgbBlue);
	strText = "成都天佑智隧科技有限公司@2015年                                          ";
	nIndex = m_wndStatusBar.CommandToIndex(ID_INDICATOR_RGBA);
	m_wndStatusBar.SetPaneText(nIndex,strText);
}

void CMainFrame::ShowFontBar(BOOL isShow)
{
	if ((!isShow&&m_wndTextBar.IsWindowVisible())
		|| (isShow&&!m_wndTextBar.IsWindowVisible()))
		OnBarCheck(IDR_STYLE_BAR);
}

void CMainFrame::ShowHistoBar(BOOL isShow)
{
//	if (isShow)
//		m_HistoBar.ShowWindow(SW_SHOW);
//	else
//		m_HistoBar.ShowWindow(SW_HIDE);
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	static BOOL bShow=false;
	if(!bShow)
	{  
		int cscy = ::GetSystemMetrics(SM_CYSCREEN) / 4*3; 
		int cscx = ::GetSystemMetrics(SM_CXSCREEN) / 4*3; 
		int csy = cscy/6; 
		int csx = cscx/6;
		CRect rect;

//		this->MoveWindow(csx,csy,cscx,cscy);
//		this->ShowWindow(SW_SHOWMAXIMIZED);
		this->m_pDLineProperty->GetWindowRect(&rect);
		this->m_pDLineProperty->MoveWindow(cscx*4/3-rect.Width(),0,rect.Width(),rect.Height());
		this->m_pDLineProperty->ShowWindow(SW_HIDE);
	}
	bShow=true;

	CGuiMDIFrame::OnTimer(nIDEvent);
}

// 弹出图形属性参数对话框
void CMainFrame::OnGraphPara() 
{
	m_pDLineProperty->ShowWindow(!m_pDLineProperty->IsWindowVisible());
}

void CMainFrame::OnViewHistogram()
{
//	int nShowMode = m_HistoBar.IsWindowVisible();
//	m_HistoBar.ShowWindow(!nShowMode);
}

void CMainFrame::OnUpdateViewHistogram (CCmdUI* pCmdUI)
{
//	pCmdUI->SetCheck(m_HistoBar.IsWindowVisible());
}

void CMainFrame::OnUpdateGraphPara(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_pDLineProperty->IsWindowVisible());
}

void CMainFrame::OnUpdateViewToolbar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_wndToolBar.IsWindowVisible());
}

void CMainFrame::OnUpdateViewGraphLayerCur(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	m_ComboCurLayer.EnableWindow(GetCanvasMgr()->GetCurCanvas() != NULL);
}

/*
LRESULT CMainFrame::OnRecogTexture(WPARAM wParam,LPARAM lParam)
{
	PRECOGTEXTURE pRecog = (PRECOGTEXTURE)wParam;
	int nNum = (int)lParam;
	CTilSysDoc* pDoc = (CTilSysDoc*)GetActiveDocument();

	// 将识别结果放入
	for (int i=0; i<nNum; i++)
	{
		for (int j=0; j<pDoc->m_dFaceCaption.size(); j++)
		{
			if (pDoc->m_dFaceCaption[j]->m_strDK == pRecog[i].strDK)
			{
				pDoc->m_dFaceCaption[j]->m_strLith = pRecog[i].strRockLith;
				break;
			}
		}

		// 如果没有这个里程,则直接加在后面
		if (j == pDoc->m_dFaceCaption.size())
		{
			CFaceCaption* pFaceCap = new CFaceCaption;
			pFaceCap->m_strDK = pRecog[i].strDK;
			pFaceCap->m_strLith = pRecog[i].strRockLith;
			pDoc->m_dFaceCaption.push_back(pFaceCap);
		}
	}

	FaceCaptionInfo(pDoc->m_dFaceCaption);

	return 0;
}
*/


void CMainFrame::OnUpdateWorkspace(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_WorkSpaceBar.IsVisible());
}

void CMainFrame::OnUpdateOutput(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_OutPut.IsVisible());
}

void CMainFrame::OnOtherOptions() 
{
	// TODO: Add your command handler code here
	CDlgOtherOptions dlgOptions;
	if (dlgOptions.DoModal() == IDOK)
	{
		CTilSysDoc* pDoc = (CTilSysDoc*)((CTilSysApp*)AfxGetApp())->GetDocument();
		pDoc->m_FaceInfoPrint.SetPrintTarget(dlgOptions.m_nPrintTarget);
	}
}

void CMainFrame::OnUpdateOtherOptions(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(FALSE);
}

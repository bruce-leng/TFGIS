// TilSysView.cpp : implementation of the CSysView class
//

#include "stdafx.h"
#include "TilSys.h"
#include <float.h>

#include "TilSysDoc.h"
#include "TilSysView.h"
#include "ChildFrm.h"
#include "MainFrm.h"

#include "GuiOutPut.h"
#include "TilCanvas.h"
#include "Image.h"
//#include "Graph/Ellipse.h"
//#include "Graph/EllipseArc.h"
#include "Graph/Line.h"
#include "Graph/Polygon.h"
#include "Graph//IntelScissor.h"
//#include "Graph/Rectangle.h"
//#include "Graph/Round.h"
//#include "Graph/RoundArc.h"
#include "Graph/Text.h"
#include "Graph/TilTunArc.h"
#include "DlgGraphObjLnk.h"
#include "DlgImageGeometryCorrect.h"
#include "DlgImageConcat.h"
#include "DlgGraphTunSide.h"
//#include "DlgFaceInfo.h"
#include "DlgFaceInfoEx.h" // 中铁5局的掌子面地质信息对话框
#include "TilComFunction.h"
#include "SpaceRelation.h"
#include "GuiDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSysView

IMPLEMENT_DYNCREATE(CSysView, CScrollView)

BEGIN_MESSAGE_MAP(CSysView, CScrollView)
	//{{AFX_MSG_MAP(CSysView)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_GRAPH_ARROW, OnGraphArrow)
	ON_COMMAND(ID_GRAPH_LINE, OnGraphLine)
	ON_COMMAND(ID_GRAPH_POLYGON, OnGraphPolygon)
	ON_COMMAND(ID_GRAPH_MAGIC, OnGraphMagic)
	ON_COMMAND(ID_GRAPH_MOVE, OnGraphMove)
	ON_COMMAND(ID_GRAPH_TEXT, OnGraphText)
	ON_COMMAND(ID_IMPORT_TUNARC, OnImportTunArc)
	ON_COMMAND(ID_CHAR_ITALIC, OnCharItalic)
	ON_COMMAND(ID_CHAR_UNDERLINE, OnCharUnderline)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_COMMAND(ID_GRAPH_OBJLINK, OnGraphObjlink)
	ON_UPDATE_COMMAND_UI(ID_GRAPH_OBJLINK, OnUpdateGraphObjlink)
	ON_COMMAND(ID_IMAGE_DISTORT_CORRECT, OnImageDistortCorrect)
	ON_COMMAND(ID_IMAGE_GEOMETRY_CORRECT, OnImageGeometryCorrect)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_DISTORT_CORRECT, OnUpdateImageDistortCorrect)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_GEOMETRY_CORRECT, OnUpdateImageGeometryCorrect)
	ON_COMMAND(ID_IMAGE_CONCAT, OnImageConcat)
	ON_UPDATE_COMMAND_UI(ID_GRAPH_ARROW, OnUpdateGraphArrow)
	ON_UPDATE_COMMAND_UI(ID_GRAPH_LINE, OnUpdateGraphLine)
	ON_UPDATE_COMMAND_UI(ID_GRAPH_POLYGON, OnUpdateGraphPolygon)
	ON_UPDATE_COMMAND_UI(ID_GRAPH_MAGIC, OnUpdateGraphMagic)
	ON_UPDATE_COMMAND_UI(ID_GRAPH_MOVE, OnUpdateGraphMove)
	ON_UPDATE_COMMAND_UI(ID_GRAPH_TEXT, OnUpdateGraphText)
	ON_UPDATE_COMMAND_UI(ID_IMPORT_TUNARC, OnUpdateImportTunArc)
	ON_COMMAND(ID_GRAPH_TUNSIDE, OnGraphTunside)
	ON_UPDATE_COMMAND_UI(ID_GRAPH_TUNSIDE, OnUpdateGraphTunside)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_FACE_CAPTION_CUR, OnFaceCaptionCur)
	ON_COMMAND(ID_FACE_CAPTION_ALL, OnFaceCaptionAll)
	ON_COMMAND(ID_GRAPH_CLEAR, OnGraphClear)
	ON_UPDATE_COMMAND_UI(ID_GRAPH_CLEAR, OnUpdateGraphClear)
	ON_COMMAND(ID_GRAPH_EDGE, OnGraphEdge)
	ON_UPDATE_COMMAND_UI(ID_GRAPH_EDGE, OnUpdateGraphEdge)
	ON_COMMAND(ID_GRAPH_EXPORT, OnGraphExport)
	ON_UPDATE_COMMAND_UI(ID_GRAPH_EXPORT, OnUpdateGraphExport)
	ON_COMMAND(ID_GRAPH_EXPORT_ALL, OnGraphExportAll)
	ON_UPDATE_COMMAND_UI(ID_GRAPH_EXPORT_ALL, OnUpdateGraphExportAll)
	ON_UPDATE_COMMAND_UI(ID_CHAR_ITALIC, OnUpdateCharItalic)
	ON_UPDATE_COMMAND_UI(ID_CHAR_UNDERLINE, OnUpdateCharUnderline)
	ON_COMMAND(ID_CHAR_BOLD, OnCharBold)
	ON_UPDATE_COMMAND_UI(ID_CHAR_BOLD, OnUpdateCharBold)
	ON_COMMAND(ID_FACE_INFO, OnFaceInfo)
	ON_UPDATE_COMMAND_UI(ID_FACE_INFO, OnUpdateFaceInfo)
	ON_UPDATE_COMMAND_UI(ID_FACE_CAPTION_CUR, OnUpdateFaceCaptionCur)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_EXPORT_ALL, OnExportAll)
	ON_UPDATE_COMMAND_UI(ID_EXPORT_ALL, OnUpdateExportAll)
	ON_COMMAND(ID_EXPORT_CUR_GRAPH_IMAGE, OnExportCurGraphImage)
	ON_UPDATE_COMMAND_UI(ID_EXPORT_CUR_GRAPH_IMAGE, OnUpdateExportCurGraphImage)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)

	ON_MESSAGE(WM_CHANGE_ITEM, OnChangeItem)
	ON_MESSAGE(WM_SELE_CHANGED, OnSelChanged)
	ON_MESSAGE(WM_SELE_DELETED, OnSelDeleted)
	ON_MESSAGE(WM_RECOG_TEXTURE, OnRecogTexture)
	ON_MESSAGE(WM_ADD_TO_FACE, OnAddToFace)
	ON_MESSAGE(WM_USER_NEWIMAGE, OnNewImage)
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysView construction/destruction

CSysView::CSysView()
{
	m_GraphCaption.m_IsLastMove=false;
	m_GraphCaption.m_Rtimes=0;
	m_GraphCaption.m_Index=0;
	m_GraphCaption.m_pCurrentUnit=NULL;
	m_GraphCaption.m_HaveAxis=true;
	m_GraphCaption.m_pBitmap=NULL;
	m_GraphCaption.m_IsLocked=false;
	CMainFrame*pMainFrame=STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
	m_GraphCaption.m_pDLineProperty=pMainFrame->m_pDLineProperty;
	m_GraphCaption.m_CenterPoint = POINT2DF(0,0);
	m_GraphCaption.m_IsMouseCap=true;
	m_GraphCaption.m_MinPoint.x=m_GraphCaption.m_MinPoint.y=FLT_MAX;
	m_GraphCaption.m_MaxPoint.x=m_GraphCaption.m_MaxPoint.y=FLT_MIN;
	m_bIsTunArcMoved = FALSE;
	m_isSameSelLayer = FALSE;
}

CSysView::~CSysView()
{
}

BOOL CSysView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CSysView drawing

void CSysView::OnDraw(CDC* pDC)
{
	CTilSysDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	CTilCanvas* pCanvas = GetCanvasMgr()->GetCurCanvas();
	if (pCanvas && !pDC->IsPrinting())
	{
		pCanvas->Draw(pDC);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSysView printing

void CSysView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	pDC->SetMapMode(MM_TWIPS);

	CScrollView::OnPrepareDC(pDC, pInfo);
}

BOOL CSysView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	pInfo->SetMaxPage(GetCanvasMgr()->Size());
	pInfo->SetMinPage(1);

	return DoPreparePrinting(pInfo);
}

void CSysView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	pInfo->m_nCurPage = GetCanvasMgr()->GetCurCanvasIdx()+1;
	CScrollView::OnBeginPrinting(pDC, pInfo);

	CTilSysDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	// 读取ifo文件和对应的画布指针
	CString strPath, strName, strIFO;
	CTilCanvasMgr* pMgr = GetCanvasMgr();
	for (int i=0; i<pMgr->Size(); i++)
	{
		CTilCanvas* pCanvas = pMgr->GetCanvasByIdx(i);
		strPath = get_file_path(pCanvas->GetImage()->GetPathName());
		strName = get_file_name(pCanvas->GetImage()->GetPathName());
		strIFO = strPath + '\\' + strName + ".ifo";
		pDoc->m_FaceInfoPrint.m_FaceInfoRecords.ReadFaceInfoAndCanvas(strIFO, pCanvas);
	}

//	mPageHeight=pDC->GetDeviceCaps(VERTSIZE)/25.4*1440;   //dot num
//	mPageWidth=pDC->GetDeviceCaps(HORZSIZE)/25.4*1440;
}

#include <afxpriv.h> 
void CSysView::OnFilePrintPreview() 
{
	CPrintPreviewState* pState = new CPrintPreviewState;

	if (!DoPrintPreview(AFX_IDD_PREVIEW_TOOLBAR, this,
							RUNTIME_CLASS(CPreviewView), pState))
	{
		// In derived classes, reverse special window handling here for
		// Preview failure case
		TRACE0("Error: DoPrintPreview failed.\n");
		AfxMessageBox(AFX_IDP_COMMAND_FAILURE);
		delete pState;      // preview failed to initialize, delete State now
	}

// 	// 使工具栏隐藏
// 	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
// 	pFrame->GetControlBar()
// 
	// 更新非客户区
	((CMainFrame*)AfxGetMainWnd())->RecalcLayout(FALSE);
}

void CSysView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	if (pInfo->m_nCurPage <= 0 || pInfo->m_nCurPage > GetCanvasMgr()->Size())
		return;

	CTilSysDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDC->SetMapMode(MM_LOMETRIC);

	// 打印
	pDoc->m_FaceInfoPrint.PrintHeaderFooter(pDC);
	pDoc->m_FaceInfoPrint.PrintFaceInfoHead(pDC);
	pDoc->m_FaceInfoPrint.PrintFaceInfoGrid(pDC);
	pDoc->m_FaceInfoPrint.PrintFaceInfoRecord(pDC, pInfo->m_nCurPage-1);

	CScrollView::OnPrint(pDC, pInfo);
}

void CSysView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add cleanup after printing
	CScrollView::OnEndPrinting(pDC, pInfo);

	// 完成打印预览或打印后，清理打印内容
	CTilSysDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	// 将画布的缩放率恢复到打印前的状态(因为是用的主窗口的画布指针)
	pDoc->m_FaceInfoPrint.m_FaceInfoRecords.RestoreOriCanvasSetting();
	pDoc->m_FaceInfoPrint.m_FaceInfoRecords.Clear();
}

/////////////////////////////////////////////////////////////////////////////
// CSysView diagnostics

#ifdef _DEBUG
void CSysView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CSysView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CTilSysDoc* CSysView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTilSysDoc)));
	return (CTilSysDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSysView message handlers

void CSysView::OnInitialUpdate() 
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	sizeTotal.cx = 100;
	sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);

	// 显示标尺（不完善，暂不显示）
	CChildFrame* pParent = ((CChildFrame*)GetParentFrame());
	pParent->ShowRulers(TRUE);
}

void CSysView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	CMainFrame* pMain = (CMainFrame*) AfxGetMainWnd();
//	if (pMain->m_HistoBar.IsWindowVisible()){
//		CTilSysDoc* pDoc = GetDocument();
//		CTilCanvas* pCanvas = GetCanvasMgr()->GetCurCanvas();
//		if (pCanvas!=NULL)
//		{
//			CImage* pImage = pCanvas->GetImage()->GetCurImage();
////			pDoc->m_hmax=pImage->Histogram(pDoc->m_hr,pDoc->m_hg,pDoc->m_hb,pDoc->m_hgray);
////			pMain->m_HistoBar.Invalidate();
//		}
//	}

	UpdateRulersInfo(RW_POSITION, GetDeviceScrollPosition());
	Invalidate();
}

// 更新标尺
void CSysView::UpdateRulersInfo(int nMessage, CPoint ScrollPos, CPoint Pos)
{
	CChildFrame* pParent = ((CChildFrame*)GetParentFrame());
	if (!pParent)
		return;

	CTilCanvasMgr* pMgr = GetCanvasMgr();
	if (!pMgr->GetCurCanvas())
		return;
	CTilCanvas* pCanvas = pMgr->GetCurCanvas();
	long double dbZoomX, dbZoomY;
	pCanvas->GetZoomFactor(dbZoomX, dbZoomY);

	stRULER_INFO pRulerInfo;
	pRulerInfo.uMessage    = nMessage;
	pRulerInfo.ScrollPos   = ScrollPos;
	pRulerInfo.Pos         = Pos;
	pRulerInfo.DocSize     = CSize(pCanvas->GetWidth(), pCanvas->GetHeight()); 

	// 标尺在原图缩放的基础上再缩放0.5倍,因为图像比例尺按像素:厘米＝2:1设置
	if (nMessage == RW_HSCROLL)
		pRulerInfo.fZoomFactor = dbZoomX;
	else
		pRulerInfo.fZoomFactor = dbZoomY;

	pParent->UpdateRulersInfo(pRulerInfo);
}

// 用鼠标拖动改变文件相互位置
LRESULT CSysView::OnChangeItem(WPARAM wParam,LPARAM lParam)
{
	int nOldPos = (int)wParam;
	int nNewPos = (int)lParam;

	GetCanvasMgr()->DragCanvasTo(nOldPos, nNewPos);
	SendMessage(WM_SIZE);

	UpdateRulersInfo(RW_VSCROLL, GetScrollPosition());
	Invalidate();

	return 0;
}

#include <process.h>
// 当文件选择改变时
// WPARAM wParam:当前切片序号
LRESULT CSysView::OnSelChanged(WPARAM wParam,LPARAM lParam)
{
	CTilSysDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	CTilCanvasMgr* pMgr = GetCanvasMgr();
	pMgr->SetCurCanvasIdx((int)wParam);
	
	SendMessage(WM_SIZE);
	
	//画出水平和竖直标尺
	UpdateRulersInfo(RW_VSCROLL, GetScrollPosition());
	Invalidate();

	pDoc->m_hWnd = GetSafeHwnd();
	pDoc->m_lMenuCommand = WM_SELE_CHANGED;
	pDoc->m_hThread = (HANDLE)_beginthread(RunImageThread, 0, pDoc);

#if CURVE_WINDOW
	CImage* pImage = pMgr->GetCurCanvas()->GetImage()->GetCurImage();
	long lRed[256],lGreen[256],lBlue[256],lGrey[256];
	pDoc->m_hmax = pImage->Histogram(lRed,lGreen,lBlue,lGrey);
	
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCanvasHistogram& histo = pFrame->GetDataCurve().GetHistogram();
	histo.SetData(lRed,lGreen,lBlue,lGrey);
	histo.Invalidate();
#endif
	
	return 0;
}

// 删除选中的图像切片
// WPARAM wParam:当前切片序号
LRESULT CSysView::OnSelDeleted(WPARAM wParam,LPARAM lParam)
{
	int nIdx = (int)wParam;
	GetCanvasMgr()->RemoveAt(nIdx);
	Invalidate();

	return 0;
}

void CSysView::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);

	CTilSysDoc* pDoc = GetDocument();

	// TODO: Add your message handler code here
	CTilCanvas* pCanvas = GetCanvasMgr()->GetCurCanvas();
	if (!pCanvas)	return;

	long double dbZoomX, dbZoomY;
	pCanvas->GetZoomFactor(dbZoomX, dbZoomY);

	int nZoomWidth = int(pCanvas->GetWidth()*dbZoomX);
	int nZoomHeigh = int(pCanvas->GetHeight()*dbZoomY);
	SetScrollSizes(MM_TEXT, CSize(nZoomWidth, nZoomHeigh));

	// 显示图像属性
	CTilImage* pImageMgr = pCanvas->GetImage();
	ImageInfo(pImageMgr->GetWidth(), pImageMgr->GetHeight(),
						pImageMgr->GetDepth()*8, pImageMgr->GetPathName());
}

void CSysView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CTilCanvas* pCanvas = GetCanvasMgr()->GetCurCanvas();
	if (!pCanvas)
		return;

	CTilGraph* pGraphMgr = pCanvas->GetGraph();
	CTilSysDoc* pDoc = GetDocument();  
//	CPoint Devpoint=point;
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.SetWindowOrg(GetDstPoint(m_GraphCaption.m_CenterPoint));
	dc.DPtoLP(&point);
	dc.SetROP2(R2_NOTXORPEN);
//	ComputeMinandMaxPoint(point);
	m_GraphCaption.m_ChangeingPoint=GetSrcPoint(point);
	SetMousePosTextColor(point);

	long double dbZoomX, dbZoomY;
	GetCanvasMgr()->GetZoomFactor(dbZoomX, dbZoomY);

	if(m_GraphCaption.m_CurrentDrawStatus==Draw_Status)
	{
		if(m_GraphCaption.m_CurrentDrawTool!=Graph_Polygon
			&& m_GraphCaption.m_CurrentDrawTool!=Graph_Magic
			&&m_GraphCaption.m_CurrentDrawTool!=Graph_PolyBezier)
		{
			if(m_GraphCaption.m_CurrentDrawTool==Graph_EllipseArc)
			{
				if(m_GraphCaption.m_LBDownTimes==3)
				{
					m_GraphCaption.m_pCurrentUnit->DrawActive(&dc,point);
					m_GraphCaption.m_pCurrentUnit->DrawStatic(&dc);
					m_GraphCaption.m_LBDownTimes=-1;
					//if(m_DLineProperty->m_Mode->m_bMode)
					//this->m_pCurrentUnit->DrawStatic(&dc);
				}
			}
			if(m_GraphCaption.m_CurrentDrawTool==Graph_Ellipse
				||m_GraphCaption.m_CurrentDrawTool==Graph_RoundArc)
			{
				if(m_GraphCaption.m_LBDownTimes==2)
				{
					ComputeMinandMaxPoint(point);
					m_GraphCaption.m_pCurrentUnit->DrawActive(&dc,point);
					m_GraphCaption.m_pCurrentUnit->DrawStatic(&dc);
					Invalidate(FALSE);
					//InvalidateMovingRect(m_GraphCaption.m_pCurrentUnit);
					m_GraphCaption.m_LBDownTimes=-1;
					//if(m_DLineProperty->m_Mode->m_bMode)
					//this->m_pCurrentUnit->DrawStatic(&dc);
				}
			}
			if(m_GraphCaption.m_CurrentDrawTool!=Graph_Text
				&&m_GraphCaption.m_CurrentDrawTool!=Graph_Ellipse
				&&m_GraphCaption.m_CurrentDrawTool!=Graph_RoundArc
				&&m_GraphCaption.m_CurrentDrawTool!=Graph_EllipseArc)
			{ 
				if(m_GraphCaption.m_LBDownTimes==1)
				{   
					//ComputeMinandMaxPoint(point);
					//InvalidateMovingRect(m_GraphCaption.m_pCurrentUnit);
					m_GraphCaption.m_LBDownTimes=-1;
					//if(m_DLineProperty->m_Mode->m_bMode)
					//m_GraphCaption.m_pCurrentUnit->DrawStatic(&dc);
				}
			}//*****if(this->m_CurrentDrawTool!=Graph_Ellipse)
		}//*****if(this->m_CurrentDrawTool!=Graph_Polygon)
	}//*******if(this->m_CurrentDrawStatus==Draw_Status)
  if(m_GraphCaption.m_CurrentDrawStatus==Select_Status)
	{
		m_GraphCaption.m_LBDownTimes=0;
		m_GraphCaption.m_CurrentDrawTool=0;
	}

	if(m_GraphCaption.m_CurrentDrawStatus==Change_Status
		||m_GraphCaption.m_CurrentDrawStatus==Drag_Status)
	{
		if((m_GraphCaption.m_CurrentDrawTool==Graph_Round
			||m_GraphCaption.m_CurrentDrawTool==Graph_Ellipse)
			&&m_GraphCaption.m_CurrentDrawStatus==Change_Status)
		{
			m_GraphCaption.m_pCurrentUnit->ShowMovingLine(&dc,
				GetDstPoint(m_GraphCaption.m_pCurrentUnit->m_CenterPoint),
				GetDstPoint(m_GraphCaption.m_pCurrentUnit->m_FoundPoint));
			m_GraphCaption.m_pCurrentUnit->ShowMovingLine(&dc,
				GetDstPoint(m_GraphCaption.m_pCurrentUnit->m_CenterPoint),
				point);
		}

		m_GraphCaption.m_LBDownTimes=-1;
		m_GraphCaption.m_bHaveFindFirst=false;
		m_GraphCaption.m_CurrentDrawStatus=Select_Status;
		m_GraphCaption.m_pCurrentUnit->ShowSelectPoint(&dc);
		m_GraphCaption.m_pCurrentUnit->ShowMovingLine(&dc,
			GetDstPoint(m_GraphCaption.m_pCurrentUnit->m_FoundPoint),
			point);
		m_GraphCaption.m_pCurrentUnit->DrawOldReferencePoint(&dc,
			GetDstPoint(m_GraphCaption.m_pCurrentUnit->m_FoundPoint));
		ComputeMinandMaxPoint(point);
		//InvalidateMovingRect(m_GraphCaption.m_pCurrentUnit);
		pGraphMgr->SetModifiedFlag(TRUE);
		Invalidate(FALSE);
		if(m_GraphCaption.m_CurrentDrawTool==Graph_RoundArc)
		{
			m_GraphCaption.m_pCurrentUnit->DrawActive(&dc,point);
			m_GraphCaption.m_pCurrentUnit->DrawStatic(&dc);
		}
	}
	m_GraphCaption.m_LBDownTimes++;
	m_GraphCaption.m_bHaveFindSecond=false;
	m_GraphCaption.m_bHaveFindFirst=false;
	m_GraphCaption.m_LastDownPoint=GetSrcPoint(point);
	m_GraphCaption.L_iPointCount=-1;
	if(m_GraphCaption.m_LBDownTimes==1
		&&m_GraphCaption.m_CurrentDrawTool!=Graph_Text
		&&m_GraphCaption.m_CurrentDrawStatus!=Select_Status)
	{ 
		m_GraphCaption.m_FirstPoint=GetSrcPoint(point);
		m_GraphCaption.m_SecondPoint=GetSrcPoint(point);
		m_GraphCaption.m_MinPoint=GetSrcPoint(point);
		m_GraphCaption.m_MaxPoint=GetSrcPoint(point);
	}
	ComputeMinandMaxPoint(point);
	if(m_GraphCaption.m_pDLineProperty->m_DLineDlg2->m_Draw_Status==Select_Color_Status)
	{
		m_GraphCaption.m_pDLineProperty->m_DLineDlg2->OnChange(dc.GetPixel(point));
		m_GraphCaption.m_CurrentDrawStatus=Select_Color_Status;
		m_GraphCaption.m_CurrentDrawTool=0;
	}
	//******************************************************************************
	//******************************************************************************
	int nSelNum=0, nSelIdx;
	dc.SetROP2(R2_NOTXORPEN);
	if((m_GraphCaption.m_LBDownTimes==1)
		&&(m_GraphCaption.m_CurrentDrawStatus==Select_Status))
	{  
		m_bIsTunArcMoved = FALSE;

		for(int i=0;i<pGraphMgr->Size();i++)
		{
			CGraph* pGraph=pGraphMgr->GetGraphByIdx(i);
			if (pGraph->GetShowMode())
			{
				for (int j=0; !m_GraphCaption.m_bHaveFindSecond
							&&!m_GraphCaption.m_bHaveFindFirst&&j<pGraph->Size(); j++)
				{
					CUnit* pUnit = pGraph->GetUnitByIdx(j);

					pUnit->OnLButtonDown(&dc,&m_GraphCaption,point);
					//m_GraphCaption.m_pCurrentUnit->GetBoundingRect();

					// 使工具条中的图层下拉菜单显示被选择直线所在图层
					if(pUnit->IsSelected())
					{
						if (nSelNum == 0)
						{
							m_isSameSelLayer = TRUE;
							nSelNum = 1;
							nSelIdx = i;
						}
						else
						{
							if (nSelIdx != i)
								m_isSameSelLayer = FALSE;
						}
					}
					
					// 判断是否当前断面拱形在被移动
					if (i==GRAPH_LAYER_TUNARC && m_GraphCaption.m_CurrentDrawStatus==Drag_Status)
						m_bIsTunArcMoved = TRUE;
				}
			}
		}
	} //******if((m_CurrentDrawStatus==Select_Status))

	// 使工具条中的图层下拉菜单显示被选择直线所在图层
	if (m_isSameSelLayer)
	{
		m_isSameSelLayer = FALSE;
		((CMainFrame*)AfxGetMainWnd())->m_ComboCurLayer.SetCurSel(nSelIdx);
		((CMainFrame*)AfxGetMainWnd())->m_ComboCurLayer.m_nCurIdx = nSelIdx;
	}

 //***************************************************************************************
 //***************************************************************************************
	if(m_GraphCaption.m_CurrentDrawStatus==Draw_Status)
	{
		if(m_GraphCaption.m_CurrentDrawTool==Graph_Text)
		{
			if(m_GraphCaption.m_LBDownTimes==1)
			{   
				m_GraphCaption.m_pCurrentUnit=new CText;
				m_GraphCaption.m_pCurrentUnit->SetZoomFactor(dbZoomX, dbZoomY);
				CGraph* pGraph = pGraphMgr->GetGraphByIdx(GRAPH_LAYER_OTHER);
				pGraph->PushBackUnit(m_GraphCaption.m_pCurrentUnit);
				pGraph->SetModifiedFlag(TRUE);
			}
			m_GraphCaption.m_pCurrentUnit->OnLButtonDown(&dc,&m_GraphCaption,point);
		}
		if(m_GraphCaption.m_CurrentDrawTool==Graph_Polygon)
		{
			if(m_GraphCaption.m_LBDownTimes==1)
			{
				m_GraphCaption.m_pCurrentUnit=new CPolygon;
				m_GraphCaption.m_pCurrentUnit->SetPolyClose(m_GraphCaption.m_bClose);
				m_GraphCaption.m_pCurrentUnit->SetZoomFactor(dbZoomX, dbZoomY);
				CGraph* pGraph = pGraphMgr->GetCurGraph();
				ASSERT(pGraph != NULL);
				pGraph->PushBackUnit(m_GraphCaption.m_pCurrentUnit);
				pGraph->SetModifiedFlag(TRUE);
				m_GraphCaption.m_pCurrentUnit->m_pPointList->Add(GetSrcPoint(point));
			}
			if(m_GraphCaption.m_LBDownTimes!=0&&m_GraphCaption.m_LBDownTimes!=1)
			{
	       m_GraphCaption.m_pCurrentUnit->m_pPointList->Add(GetSrcPoint(point));
	     }
		}//******if(m_CurrentDrawTool==Graph_Polygon)
		if (m_GraphCaption.m_CurrentDrawTool==Graph_Magic)
		{
			if(m_GraphCaption.m_LBDownTimes==1)
			{
				m_GraphCaption.m_pCurrentUnit=new CIntelScissor;
				((CIntelScissor*)m_GraphCaption.m_pCurrentUnit)->SetImage(GetCanvasMgr()->GetCurCanvas()->GetImage()->GetCurImage());
				m_GraphCaption.m_pCurrentUnit->SetPolyClose(m_GraphCaption.m_bClose);
				m_GraphCaption.m_pCurrentUnit->SetZoomFactor(dbZoomX, dbZoomY);
				CGraph* pGraph = pGraphMgr->GetCurGraph();
				ASSERT(pGraph != NULL);
				pGraph->PushBackUnit(m_GraphCaption.m_pCurrentUnit);
				pGraph->SetModifiedFlag(TRUE);
//				m_GraphCaption.m_pCurrentUnit->m_pPointList->Add(GetSrcPoint(point));
			}
			((CIntelScissor*)m_GraphCaption.m_pCurrentUnit)->Add(&dc,point);
		}	

    if(m_GraphCaption.m_CurrentDrawTool==Graph_Line)
		{
			if(m_GraphCaption.m_LBDownTimes==1)
			{
				m_GraphCaption.m_pCurrentUnit=new CLine;
				m_GraphCaption.m_pCurrentUnit->SetZoomFactor(dbZoomX, dbZoomY);
				CGraph* pGraph = pGraphMgr->GetCurGraph();
				pGraph->PushBackUnit(m_GraphCaption.m_pCurrentUnit);
				pGraph->SetModifiedFlag(TRUE);
			}
		} 

	}//******if(m_GraphCaption.m_CurrentDrawStatus==Draw_Status)

  if((m_GraphCaption.m_CurrentDrawStatus==Draw_Status
		&&m_GraphCaption.m_LBDownTimes==1)
		/*||(m_GraphCaption.m_bHaveFindSecond&&!m_GraphCaption.m_IsLocked)*/)
	{
		m_GraphCaption.m_pDLineProperty->m_DLineDlg1->UpdateData();
		m_GraphCaption.m_pCurrentUnit->m_FirstPoint=m_GraphCaption.m_FirstPoint;
		m_GraphCaption.m_pCurrentUnit->m_SecondPoint=m_GraphCaption.m_SecondPoint;
		m_GraphCaption.m_pCurrentUnit->m_PenStyle=m_GraphCaption.m_pDLineProperty->m_DLineDlg1->m_PenStyle;
		m_GraphCaption.m_pCurrentUnit->m_PenWidth=m_GraphCaption.m_pDLineProperty->m_DLineDlg1->m_Edit_LineWidth;
		m_GraphCaption.m_pCurrentUnit->m_PenColor=m_GraphCaption.m_pDLineProperty->m_DLineDlg2->m_PenColor;
		m_GraphCaption.m_pCurrentUnit->m_BrushColor=m_GraphCaption.m_pDLineProperty->m_DLineDlg2->m_BrushColor;
		m_GraphCaption.m_pCurrentUnit->m_BrushHatch=m_GraphCaption.m_pDLineProperty->m_DLineDlg1->m_BrushHatch;
		m_GraphCaption.m_pCurrentUnit->m_BrushStyle=m_GraphCaption.m_pDLineProperty->m_DLineDlg1->m_BrushStyle;
		m_GraphCaption.m_pCurrentUnit->m_BackgroundColor=m_GraphCaption.m_pDLineProperty->m_DLineDlg2->m_BackgroundColor;

		//if(m_GraphCaption.m_pDLineProperty->m_Mode->m_bMode)
		// m_GraphCaption.m_pCurrentUnit->m_DrawingMode=m_GraphCaption.m_pLineProperty->m_Mode->m_DrawingMode;
		//else
		m_GraphCaption.m_pCurrentUnit->m_DrawingMode=dc.GetROP2();
		m_GraphCaption.m_pCurrentUnit->m_BkMode=m_GraphCaption.m_pDLineProperty->m_DLineDlg1->m_BKMode;//m_DLineProperty->m_Mode->m_BKMode;
			
	}//**********if(this->m_LBDownTimes==1&&m_bHaveFindFirst)

	if(m_GraphCaption.m_bHaveFindSecond&&m_GraphCaption.m_IsLocked)
	{
		m_GraphCaption.m_pDLineProperty->m_DLineDlg1->UpdateData();
		m_GraphCaption.m_pCurrentUnit->m_FirstPoint=m_GraphCaption.m_FirstPoint;
		m_GraphCaption.m_pCurrentUnit->m_SecondPoint=m_GraphCaption.m_SecondPoint;
		m_GraphCaption.m_pDLineProperty->m_DLineDlg1->m_PenStyle=m_GraphCaption.m_pCurrentUnit->m_PenStyle;
		m_GraphCaption.m_pDLineProperty->m_DLineDlg1->m_Edit_LineWidth=m_GraphCaption.m_pCurrentUnit->m_PenWidth;
		m_GraphCaption.m_pDLineProperty->m_DLineDlg2->m_PenColor=m_GraphCaption.m_pCurrentUnit->m_PenColor;
		m_GraphCaption.m_pDLineProperty->m_DLineDlg2->m_BrushColor=m_GraphCaption.m_pCurrentUnit->m_BrushColor;
		m_GraphCaption.m_pDLineProperty->m_DLineDlg1->m_BrushHatch=m_GraphCaption.m_pCurrentUnit->m_BrushHatch;
		m_GraphCaption.m_pDLineProperty->m_DLineDlg1->m_BrushStyle=m_GraphCaption.m_pCurrentUnit->m_BrushStyle;
		m_GraphCaption.m_pDLineProperty->m_DLineDlg2->m_BackgroundColor=m_GraphCaption.m_pCurrentUnit->m_BackgroundColor;
		m_GraphCaption.m_pCurrentUnit->m_DrawingMode=dc.GetROP2();
		m_GraphCaption.m_pDLineProperty->m_DLineDlg1->m_BKMode=m_GraphCaption.m_pCurrentUnit->m_BkMode;
		// if(m_GraphCaption.m_pDLineProperty->m_Mode->m_bMode)
		//m_GraphCaption.m_pDLineProperty->m_Mode->m_DrawingMode=m_GraphCaption.m_pCurrentUnit->m_DrawingMode;
		
		m_GraphCaption.m_pDLineProperty->GetActivePage()->UpdateData(false);
		m_GraphCaption.m_pDLineProperty->GetActivePage()->Invalidate();
	 }
  
	if(m_GraphCaption.m_bHaveFindSecond)
	{
		m_GraphCaption.m_pCurrentUnit->DrawStatic(&dc);
	}
	
	if(m_GraphCaption.m_CurrentDrawStatus==Drag_Status)
	{    
		m_GraphCaption.m_FirstPoint=m_GraphCaption.m_pCurrentUnit->m_FoundPoint;
		m_GraphCaption.m_SecondPoint=m_GraphCaption.m_pCurrentUnit->m_FoundPoint;
	}
	if(m_GraphCaption.m_CurrentDrawStatus==Change_Status)//&&m_CurrentDrawTool==Graph_Ellipse)
	{
		m_GraphCaption.m_FirstPoint=m_GraphCaption.m_pCurrentUnit->m_FoundPoint;
		m_GraphCaption.m_SecondPoint=m_GraphCaption.m_pCurrentUnit->m_FoundPoint;
		
		if(m_GraphCaption.m_CurrentDrawTool==Graph_Text)
		{
			m_GraphCaption.m_pCurrentUnit->OnLButtonDown(&dc,&m_GraphCaption,point);
		}
	}

	CScrollView::OnLButtonDown(nFlags, point);
}

void CSysView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CTilCanvas* pCanvas = GetCanvasMgr()->GetCurCanvas();
	if (!pCanvas)
		return;

//	CPoint Devpoint=point;
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.SetWindowOrg(GetDstPoint(m_GraphCaption.m_CenterPoint));
	dc.DPtoLP(&point);
	dc.SetROP2(R2_NOTXORPEN);
	ComputeMinandMaxPoint(point);
	m_GraphCaption.m_ChangeingPoint=GetSrcPoint(point);

	// 设置状态栏的显示坐标
	SetMousePosTextColor(point);
	UpdateRulersInfo(RW_POSITION, GetScrollPosition(), point);

	if(m_GraphCaption.m_pDLineProperty->m_DLineDlg2->m_Draw_Status==Select_Color_Status)
	{
		m_GraphCaption.m_pDLineProperty->m_DLineDlg2->OnChange(dc.GetPixel(point));
		m_GraphCaption.m_CurrentDrawStatus=Select_Color_Status;
		m_GraphCaption.m_CurrentDrawTool=0;
	}

	if((m_GraphCaption.m_LBDownTimes!=0)
		&&(m_GraphCaption.m_CurrentDrawStatus==Draw_Status
		||m_GraphCaption.m_CurrentDrawStatus==Change_Status))	
	{
		if(m_GraphCaption.m_CurrentDrawStatus==Change_Status)
		{
			m_GraphCaption.m_pCurrentUnit->ShowMovingLine(&dc,
				GetDstPoint(m_GraphCaption.m_pCurrentUnit->m_FoundPoint),
				GetDstPoint(m_GraphCaption.m_SecondPoint));
		}

		m_GraphCaption.m_pCurrentUnit->OnMouseMove(&dc,&m_GraphCaption,point);

		if(m_GraphCaption.m_CurrentDrawStatus==Change_Status)
		{
			m_GraphCaption.m_pCurrentUnit->ShowMovingLine(&dc,
			GetDstPoint(m_GraphCaption.m_pCurrentUnit->m_FoundPoint),
			point);
		}
	}//*********if(m_IsDrawing&&(m_CurrentDrawStatus==Draw_Status||m_CurrentDrawStatus==Exchange_Status))  
	if(m_GraphCaption.m_CurrentDrawStatus==Drag_Status)
	{     
		m_GraphCaption.m_pCurrentUnit->ShowMovingLine(&dc,
			GetDstPoint(m_GraphCaption.m_pCurrentUnit->m_FoundPoint),
			GetDstPoint(m_GraphCaption.m_SecondPoint));
		dc.SetROP2(R2_NOTXORPEN);

		CPoint pt=GetDstPoint(m_GraphCaption.m_SecondPoint);

		m_GraphCaption.m_pCurrentUnit->m_DrawingMode=dc.GetROP2();
		if(m_GraphCaption.m_CurrentDrawTool!=Graph_EllipseArc)
			m_GraphCaption.m_pCurrentUnit->DrawActive(&dc,point);
		
		m_GraphCaption.m_FirstPoint=m_GraphCaption.m_SecondPoint;
		m_GraphCaption.m_SecondPoint=GetSrcPoint(point);

		m_GraphCaption.m_pCurrentUnit->OnMouseMove(&dc,&m_GraphCaption,point);
		m_GraphCaption.m_pCurrentUnit->m_DrawingMode=dc.GetROP2();
    
		if(m_GraphCaption.m_CurrentDrawTool!=Graph_EllipseArc)
			m_GraphCaption.m_pCurrentUnit->DrawActive(&dc,point);
		
		m_GraphCaption.m_pCurrentUnit->ShowMovingLine(&dc,
			GetDstPoint(m_GraphCaption.m_pCurrentUnit->m_FoundPoint),
			point);
	}

	if (m_bIsTunArcMoved)
	{
		// 判断是否是隧道断面图形被拖动，若是，则重新设置断面中心点坐标
		CTilGraph* pGraphMgr = GetCanvasMgr()->GetCurCanvas()->GetGraph();
		POINT2DF ptImgCenter = pGraphMgr->GetTunArc()->GetImgCenter();
		ptImgCenter += m_GraphCaption.m_SecondPoint-m_GraphCaption.m_FirstPoint;
		pGraphMgr->GetTunArc()->SetImgCenter(ptImgCenter);
	}

	CScrollView::OnMouseMove(nFlags, point);
}

void CSysView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CTilCanvas* pCanvas = GetCanvasMgr()->GetCurCanvas();
	if (!pCanvas)
		return;

//	CPoint Devpoint=point;
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.SetWindowOrg(GetDstPoint(m_GraphCaption.m_CenterPoint));
	dc.DPtoLP(&point);
	dc.SetROP2(R2_NOTXORPEN);
	ComputeMinandMaxPoint(point);
	m_GraphCaption.m_ChangeingPoint=GetSrcPoint(point);
	SetMousePosTextColor(point);
	
	if(m_GraphCaption.m_CurrentDrawStatus==Draw_Status
		&&(m_GraphCaption.m_CurrentDrawTool==Graph_Polygon)
		||(m_GraphCaption.m_CurrentDrawTool==Graph_Magic))
	{
		m_GraphCaption.m_pCurrentUnit->DrawEnd(&dc,point);
		//m_GraphCaption.m_pCurrentUnit->m_PenColor = RGB(0, 0, 255);
		m_GraphCaption.m_LBDownTimes=0;
		Invalidate(FALSE);
	}

	CScrollView::OnLButtonDblClk(nFlags, point);
}


void CSysView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CTilCanvas* pCanvas = GetCanvasMgr()->GetCurCanvas();
	if (!pCanvas)
		return;
	
//	CPoint Devpoint=point;
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.SetWindowOrg(GetDstPoint(m_GraphCaption.m_CenterPoint));
	dc.DPtoLP(&point);
	dc.SetROP2(R2_NOTXORPEN);
	ComputeMinandMaxPoint(point);
	m_GraphCaption.m_ChangeingPoint=GetSrcPoint(point);
	SetMousePosTextColor(point);

	if(m_GraphCaption.m_pDLineProperty->m_DLineDlg2->m_Draw_Status==Select_Color_Status)
  {
		m_GraphCaption.m_pDLineProperty->m_DLineDlg2->m_Color=dc.GetPixel(point);
		m_GraphCaption.m_pDLineProperty->m_DLineDlg2->OnChange(dc.GetPixel(point));
		m_GraphCaption.m_CurrentDrawStatus=0;
		m_GraphCaption.m_pDLineProperty->m_DLineDlg2->m_Draw_Status=0;
		m_GraphCaption.m_CurrentDrawTool=0;
	}

	if (m_bIsTunArcMoved && m_GraphCaption.m_CurrentDrawStatus!=Drag_Status)
		m_bIsTunArcMoved = FALSE;

	CScrollView::OnLButtonUp(nFlags, point);
}

void CSysView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	switch (nChar)
	{
		case VK_DELETE:
			DeleteSelectedGraph();	// 删除被选择的图元
			GetCanvasMgr()->GetCurCanvas()->SetModifiedFlag(TRUE);
			Invalidate(FALSE);
			break;
		case VK_ESCAPE:
			OnGraphArrow();	// 设置没有被选中状态
			OnGraphMove();
			Invalidate(FALSE);
			break;
		case VK_HOME:
			OnVScroll(SB_TOP, 0, NULL);
			OnHScroll(SB_LEFT, 0, NULL);
			break;
		case VK_END:
			OnVScroll(SB_BOTTOM, 0, NULL);
			OnHScroll(SB_RIGHT, 0, NULL);
			break;
		case VK_UP:
			OnVScroll(SB_LINEUP, 0, NULL);
			break;
		case VK_DOWN:
			OnVScroll(SB_LINEDOWN, 0, NULL);
			break;
		case VK_PRIOR:
			OnVScroll(SB_PAGEUP, 0, NULL);
			break;
		case VK_NEXT:
			OnVScroll(SB_PAGEDOWN, 0, NULL);
			break;
		case VK_LEFT:
			OnHScroll(SB_LINELEFT, 0, NULL);
			break;
		case VK_RIGHT:
			OnHScroll(SB_LINERIGHT, 0, NULL);
			break;
	}

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CSysView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CScrollView::OnKeyUp(nChar, nRepCnt, nFlags);
}

// 取消图形操作
void CSysView::OnGraphArrow() 
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.SetWindowOrg(GetDstPoint(m_GraphCaption.m_CenterPoint));
	// TODO: Add your command handler code here
	
	if(m_GraphCaption.m_CurrentDrawStatus==Drag_Status
		||m_GraphCaption.m_CurrentDrawStatus==Change_Status
		||m_GraphCaption.m_CurrentDrawStatus==Select_Status)
    ResetSelectStatus();
	
	
	CMainFrame* pMainFrame=(CMainFrame*)AfxGetMainWnd();
	if(pMainFrame->m_wndTextBar.IsWindowVisible())
	{ 
		pMainFrame->ShowFontBar(false);
		if(m_GraphCaption.m_LBDownTimes==1)
		{
			m_GraphCaption.m_LBDownTimes=2;
			m_GraphCaption.m_pCurrentUnit->OnLButtonDown(&dc,&m_GraphCaption,0);
		}
	}
	m_GraphCaption.m_CurrentDrawTool=0;
	m_GraphCaption.m_CurrentDrawStatus=0;
	m_GraphCaption.m_LBDownTimes=0;	
}

void CSysView::OnGraphLine() 
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.SetWindowOrg(GetDstPoint(m_GraphCaption.m_CenterPoint));
	// TODO: Add your command handler code here
	if(m_GraphCaption.m_CurrentDrawStatus==Drag_Status
		||m_GraphCaption.m_CurrentDrawStatus==Change_Status
		||m_GraphCaption.m_CurrentDrawStatus==Select_Status)
    ResetSelectStatus();
	
	CMainFrame* pMainFrame=(CMainFrame*)AfxGetMainWnd();
	if(pMainFrame->m_wndTextBar.IsWindowVisible())
	{ 
		pMainFrame->ShowFontBar(false);
		if(m_GraphCaption.m_LBDownTimes==1)
		{
			m_GraphCaption.m_LBDownTimes=2;
			m_GraphCaption.m_pCurrentUnit->OnLButtonDown(&dc,&m_GraphCaption,0);
		}
	}
	m_GraphCaption.m_CurrentDrawTool=Graph_Polygon;
	m_GraphCaption.m_bClose = FALSE;	// 设为非封闭曲线
	m_GraphCaption.m_CurrentDrawStatus=Draw_Status;
	m_GraphCaption.m_LBDownTimes=0;
}

void CSysView::OnGraphPolygon() 
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.SetWindowOrg(GetDstPoint(m_GraphCaption.m_CenterPoint));
	// TODO: Add your command handler code here
	if(m_GraphCaption.m_CurrentDrawStatus==Drag_Status
		||m_GraphCaption.m_CurrentDrawStatus==Change_Status
		||m_GraphCaption.m_CurrentDrawStatus==Select_Status)
    ResetSelectStatus();

	m_GraphCaption.m_IsPolygonFirst=true;
	CMainFrame* pMainFrame=(CMainFrame*)AfxGetMainWnd();
	if(pMainFrame->m_wndTextBar.IsWindowVisible())
	{ 
		pMainFrame->ShowFontBar(false);
		if(m_GraphCaption.m_LBDownTimes==1)
		{
			m_GraphCaption.m_LBDownTimes=2;
			m_GraphCaption.m_pCurrentUnit->OnLButtonDown(&dc,&m_GraphCaption,0);
		}
	}
	m_GraphCaption.m_CurrentDrawTool=Graph_Polygon;
	m_GraphCaption.m_bClose = TRUE;		// 设为封闭曲线
	m_GraphCaption.m_CurrentDrawStatus=Draw_Status;
	m_GraphCaption.m_LBDownTimes=0;
}

void CSysView::OnGraphMagic() 
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.SetWindowOrg(GetDstPoint(m_GraphCaption.m_CenterPoint));
	// TODO: Add your command handler code here
	if(m_GraphCaption.m_CurrentDrawStatus==Drag_Status
		||m_GraphCaption.m_CurrentDrawStatus==Change_Status
		||m_GraphCaption.m_CurrentDrawStatus==Select_Status)
		ResetSelectStatus();
	
	m_GraphCaption.m_IsPolygonFirst=true;
	CMainFrame* pMainFrame=(CMainFrame*)AfxGetMainWnd();
	if(pMainFrame->m_wndTextBar.IsWindowVisible())
	{ 
		pMainFrame->ShowFontBar(false);
		if(m_GraphCaption.m_LBDownTimes==1)
		{
			m_GraphCaption.m_LBDownTimes=2;
			m_GraphCaption.m_pCurrentUnit->OnLButtonDown(&dc,&m_GraphCaption,0);
		}
	}
	m_GraphCaption.m_CurrentDrawTool=Graph_Magic;
	m_GraphCaption.m_bClose = FALSE;		// 设为非封闭曲线
	m_GraphCaption.m_CurrentDrawStatus=Draw_Status;
	m_GraphCaption.m_LBDownTimes=0;
}

void CSysView::OnGraphMove() 
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.SetWindowOrg(GetDstPoint(m_GraphCaption.m_CenterPoint));
	// TODO: Add your command handler code here
	if(m_GraphCaption.m_CurrentDrawStatus==Drag_Status
		||m_GraphCaption.m_CurrentDrawStatus==Change_Status
		||m_GraphCaption.m_CurrentDrawStatus==Select_Status)
    ResetSelectStatus();

	CMainFrame* pMainFrame=(CMainFrame*)AfxGetMainWnd();
	if(pMainFrame->m_wndTextBar.IsWindowVisible())
	{ 
		pMainFrame->ShowFontBar(false);
		if(m_GraphCaption.m_LBDownTimes==1)
		{
			m_GraphCaption.m_LBDownTimes=2;
			m_GraphCaption.m_pCurrentUnit->OnLButtonDown(&dc,&m_GraphCaption,0);
		}
	}
	m_GraphCaption.m_CurrentDrawTool=0;
	m_GraphCaption.m_CurrentDrawStatus=Select_Status;
	m_GraphCaption.m_LBDownTimes=0;
}

void CSysView::OnGraphText() 
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.SetWindowOrg(GetDstPoint(m_GraphCaption.m_CenterPoint));
	// TODO: Add your command handler code here
	if(m_GraphCaption.m_CurrentDrawStatus==Drag_Status
		||m_GraphCaption.m_CurrentDrawStatus==Change_Status
		||m_GraphCaption.m_CurrentDrawStatus==Select_Status)
		ResetSelectStatus();
	
	CMainFrame* pMainFrame=(CMainFrame*)AfxGetMainWnd();
	if(pMainFrame->m_wndTextBar.IsWindowVisible())
	{ 
		pMainFrame->ShowFontBar(true);
		if(m_GraphCaption.m_LBDownTimes==1)
		{
			m_GraphCaption.m_LBDownTimes=2;
			m_GraphCaption.m_pCurrentUnit->OnLButtonDown(&dc,&m_GraphCaption,CPoint(0,0));
		}
	}
	else if(!pMainFrame->m_wndTextBar.IsWindowVisible())
	{
		pMainFrame->ShowFontBar(true);
		m_GraphCaption.m_LBDownTimes=0;
	}
	//m_pTextEdit->ShowWindow(true);
	m_GraphCaption.m_CurrentDrawTool=Graph_Text;
	m_GraphCaption.m_CurrentDrawStatus=Draw_Status;
	m_GraphCaption.m_LBDownTimes=0;
}

void CSysView::OnImportTunArc()
{
	CTilGraph* pGraph = GetCanvasMgr()->GetCurCanvas()->GetGraph();

	long double dbZoomFactor;
	pGraph->GetZoomFactor(dbZoomFactor, dbZoomFactor);

	static char BASED_CODE szFilter[] = "隧道断面 (*.dxf)|*.dxf|*.*|*.*||";
	CFileDialog dlg(TRUE, "*.*", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_FILEMUSTEXIST, szFilter, NULL);
	if (dlg.DoModal() == IDOK)
	{
		CString strPathName = dlg.GetPathName();
		pGraph->ImportTunArc(strPathName);
		pGraph->SetZoomFactor(dbZoomFactor, dbZoomFactor);
		pGraph->GetGraphByIdx(0)->SetShowMode(TRUE);
		pGraph->SetModifiedFlag(TRUE);

		Invalidate(FALSE);
	}
}

void CSysView::OnGraphObjlink() 
{
	// 如果当前掌子面存在，设置对话框中显示的当前图形层并弹出对话框
	DlgGraphObjLnk dlg;
	dlg.SetCurGraphLayer(GRAPH_LAYER_ROCKS);
	if (dlg.DoModal() == IDOK)
	{
	}
}

void CSysView::OnUpdateGraphObjlink(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((GetCanvasMgr()->Size() >= 2) 
				&& (GetCanvasMgr()->GetCurCanvas() != NULL));
}

// 设置状态栏信息
void CSysView::SetMousePosTextColor(CPoint Logpoint)
{
	POINT2DF point = GetSrcPoint(Logpoint);
	Logpoint = CPoint(point.x,point.y);

	CTilCanvas* pCanvas = GetCanvasMgr()->GetCurCanvas();
	if (!pCanvas)
		return;

	CImage* pImage = pCanvas->GetImage()->GetCurImage();
//	如果点不在图像内部，则不显示状态栏信息
//	if (!pImage->IsInside(Logpoint.x, Logpoint.y))
//		return;

	RGBQUAD color = pImage->GetPixelColor(Logpoint.x, pImage->GetHeight()-Logpoint.y-1);

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->SetMousePosTextColor(Logpoint, color);
}

void CSysView::ResetSelectStatus()
{
	CTilSysDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	CClientDC dc(this);
	CClientDC*pDC=&dc;
	OnPrepareDC(pDC);
	dc.SetWindowOrg(GetDstPoint(m_GraphCaption.m_CenterPoint));

	CTilCanvas* pCanvas = GetCanvasMgr()->GetCurCanvas();
	if(pCanvas!=NULL)
	{
		CTilGraph* pGraphMgr = pCanvas->GetGraph();
		for (int i = 0; i < pGraphMgr->Size(); i ++)
		{
			CGraph* pGraph = pGraphMgr->GetGraphByIdx(i);
			for (int j=0; j<pGraph->Size(); j++)
			{
				CUnit* pUnit = pGraph->GetUnitByIdx(j);
				// 看是否处于选中状态
				if (pUnit->m_HaveFindFirst)
				{
					pUnit->ShowSelectPoint(pDC);
					pUnit->m_HaveFindFirst = false;
				}
			}
		}
	}
}

void CSysView::DeleteSelectedGraph()
{
	CTilCanvas* pCanvas = GetCanvasMgr()->GetCurCanvas();
	if (!pCanvas)
		return;
	int nCurIdx = GetCanvasMgr()->GetCurCanvasIdx();
	CTilGraph* pGraphMgr = pCanvas->GetGraph();
	
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.SetWindowOrg(GetDstPoint(m_GraphCaption.m_CenterPoint));
	CDC*pDC=&dc;
	int oldrop=pDC->SetROP2(R2_NOTXORPEN);   
	for(int i=0;i<pGraphMgr->Size();i++)
	{
		CGraph*pGraph=(CGraph*)pGraphMgr->GetGraphByIdx(i);
		for (int j=0; j<pGraph->Size(); j++)
		{
			CUnit* pUnit=(CUnit*)pGraph->GetUnitByIdx(j);
			if(pUnit->m_HaveFindFirst)
			{
				pUnit->ShowSelectPoint(pDC);

				if (pUnit->IsKindOf(RUNTIME_CLASS(CText)))
				{
					CPoint point=GetDstPoint(((CText*)pUnit)->m_PositionPoint);
					pDC->LPtoDP(&point);
					InvalidateRect(CRect(point,CPoint(point.x+((CText*)pUnit)->m_Width,
													point.y+((CText*)pUnit)->m_Hight)));
				}
				else
					pUnit->DrawStatic(pDC);

				// 在删除第j个目标前，若前一掌子面某目标后向链接为j，则设置为-1
				// 并且前一掌子面上目标的后向链接若大于j的，均要减去1
				if (nCurIdx>0)
				{
					CGraph* pPrev = GetCanvasMgr()->GetPrevCanvas()->GetGraph()->GetGraphByIdx(i);
					for (int k=0; k<pPrev->Size(); k++)
					{
						CUnit* pObj = pPrev->GetUnitByIdx(k);
						int nNextLink = pObj->GetNextLink();
						if (nNextLink > j)
							pObj->SetNextLink(nNextLink-1);
						else if (nNextLink == j)
							pObj->SetNextLink(-1);
					}
				}

				// 在删除第j个目标前，若后一掌子面某目标前向链接为j，则设置为-1
				// 并且后一掌子面上目标的前向链接若大于j的，均要减去1
				if (nCurIdx<GetCanvasMgr()->Size()-1)
				{
					CGraph* pNext = GetCanvasMgr()->GetNextCanvas()->GetGraph()->GetGraphByIdx(i);
					for (int k=0; k<pNext->Size(); k++)
					{
						CUnit* pObj = pNext->GetUnitByIdx(k);
						int nPreLink = pObj->GetPreLink();
						if (nPreLink > j)
							pObj->SetPreLink(nPreLink-1);
						else if (nPreLink == j)
							pObj->SetPreLink(-1);
					}
				}

				// 删除第j个目标后，所有后面的目标向前移一个位置，故j--;
				delete pGraph->GetUnitByIdx(j);
				pGraph->DeleteUnit(j);
				j--;
			}
		}
	}
  pDC->SetROP2(oldrop);
}

void CSysView::OnCharBold() 
{
	CMainFrame*pMainFrame=(CMainFrame*)::AfxGetMainWnd();
	pMainFrame->m_wndTextBar.OnCharBold();
}

void CSysView::OnCharItalic() 
{
	CMainFrame*pMainFrame=(CMainFrame*)::AfxGetMainWnd();
	pMainFrame->m_wndTextBar.OnCharItalic();
}

void CSysView::OnCharUnderline() 
{
	CMainFrame*pMainFrame=(CMainFrame*)::AfxGetMainWnd();
	pMainFrame->m_wndTextBar.OnCharUnderline();
}

void CSysView::ComputeMinandMaxPoint(CPoint point)
{
	POINT2DF ptD1,ptD2;
	POINT2DF pt=GetSrcPoint(point);
	ptD1.x=m_GraphCaption.m_MinPoint.x=min(m_GraphCaption.m_MinPoint.x,pt.x);
	ptD1.y=m_GraphCaption.m_MinPoint.y=min(m_GraphCaption.m_MinPoint.y,pt.y);
	ptD2.x=m_GraphCaption.m_MaxPoint.x=max(m_GraphCaption.m_MaxPoint.x,pt.x);
	ptD2.y=m_GraphCaption.m_MaxPoint.y=max(m_GraphCaption.m_MaxPoint.y,pt.y);
	POINT pt1,pt2;
	pt1 = GetDstPoint(ptD1);
	pt2 = GetDstPoint(ptD2);
	m_GraphCaption.m_InvalidRect.SetRect(pt1,pt2);
}

void CSysView::InvalidateMovingRect(CUnit *pMovingUnit)
{
	pMovingUnit->GetBoundingRect(); //该函数取得的是原始外接矩形
	POINT ptSrcMin,ptSrcMax;	// 平移前的坐标
	POINT ptDstMin, ptDstMax;	// 平移后的坐标
	ptSrcMin = GetDstPoint(m_GraphCaption.m_MinPoint);
	ptSrcMax = GetDstPoint(m_GraphCaption.m_MaxPoint);
	ptDstMin = GetDstPoint(pMovingUnit->m_MinPoint);
	ptDstMax = GetDstPoint(pMovingUnit->m_MaxPoint);
	m_GraphCaption.m_InvalidRect.left=min(ptSrcMin.x,ptDstMin.x);
	m_GraphCaption.m_InvalidRect.top=min(ptSrcMin.y,ptDstMin.y);
	m_GraphCaption.m_InvalidRect.right=max(ptSrcMax.x,ptDstMax.x);
	m_GraphCaption.m_InvalidRect.bottom=max(ptSrcMax.y,ptDstMax.y);
	m_GraphCaption.m_InvalidRect.InflateRect(pMovingUnit->m_PenWidth,pMovingUnit->m_PenWidth);
	//
	CClientDC dc(this);
	dc.Rectangle(m_GraphCaption.m_InvalidRect);
	this->InvalidateRect(m_GraphCaption.m_InvalidRect);
}

BOOL CSysView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if(m_GraphCaption.m_CurrentDrawStatus==Draw_Status
		||m_GraphCaption.m_CurrentDrawStatus==Change_Status
		||m_GraphCaption.m_CurrentDrawStatus==Select_Color_Status)
	 SetCursor(AfxGetApp()->LoadCursor(ID_Draw));

	if(m_GraphCaption.m_CurrentDrawStatus==Drag_Status
		||m_GraphCaption.m_CurrentDrawStatus==Select_Status)
     SetCursor(AfxGetApp()->LoadCursor(ID_Change));

	return true;
	//return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}

// 取得缩放前的点
POINT2DF CSysView::GetSrcPoint(const CPoint& point)
{
	POINT2DF pt;
	long double dbZoomX, dbZoomY;

	GetCanvasMgr()->GetZoomFactor(dbZoomX, dbZoomY);
	pt = POINT2DF(point.x/dbZoomX, point.y/dbZoomY);

	return pt;
}

// 取得缩放后的点
CPoint CSysView::GetDstPoint(const POINT2DF& point)
{
	CPoint pt;
	long double dbZoomX, dbZoomY;

	GetCanvasMgr()->GetZoomFactor(dbZoomX, dbZoomY);
	pt = CPoint(long(point.x*dbZoomX+0.5), long(point.y*dbZoomY+0.5));

	return pt;
}

// 图像系统畸变校正
void CSysView::OnImageDistortCorrect() 
{
	
}

// 图像几何校正
void CSysView::OnImageGeometryCorrect() 
{
	DlgImageGeometryCorrect dlg;
	if (dlg.DoModal() == IDOK)
	{
	}
	else
	{
	}
}

void CSysView::OnImageConcat() 
{
	// TODO: Add your command handler code here
	CWizard wizard(_T("图像拼接"),this,1);
	Step1 step1;
	Step2 step2;
	Step3 step3;
	wizard.AddPage(&step1);
	wizard.AddPage(&step2);
	wizard.AddPage(&step3);
	wizard.SetWizardMode();
	

	wizard.SetActivePage(&step1);
	wizard.DoModal();
}

void CSysView::OnUpdateImageDistortCorrect(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CSysView::OnUpdateImageGeometryCorrect(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetCanvasMgr()->GetCurCanvas() != NULL);
}

void CSysView::OnUpdateGraphArrow(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetCanvasMgr()->GetCurCanvas() != NULL);
	pCmdUI->SetCheck(m_GraphCaption.m_CurrentDrawTool==0
				  && m_GraphCaption.m_CurrentDrawStatus==0);
}

void CSysView::OnUpdateGraphLine(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetCanvasMgr()->GetCurCanvas() != NULL);
	pCmdUI->SetCheck(m_GraphCaption.m_CurrentDrawTool==Graph_Polygon
				&& m_GraphCaption.m_CurrentDrawStatus==Draw_Status
				&& m_GraphCaption.m_bClose           ==FALSE);
}

void CSysView::OnUpdateGraphPolygon(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetCanvasMgr()->GetCurCanvas() != NULL);
	pCmdUI->SetCheck(m_GraphCaption.m_CurrentDrawTool==Graph_Polygon
				&& m_GraphCaption.m_CurrentDrawStatus==Draw_Status
				&& m_GraphCaption.m_bClose           ==TRUE);
}

void CSysView::OnUpdateGraphMagic(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetCanvasMgr()->GetCurCanvas() != NULL);
	pCmdUI->SetCheck(m_GraphCaption.m_CurrentDrawTool==Graph_Magic
				&& m_GraphCaption.m_CurrentDrawStatus==Draw_Status);
}

void CSysView::OnUpdateGraphMove(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetCanvasMgr()->GetCurCanvas() != NULL);
	pCmdUI->SetCheck(m_GraphCaption.m_CurrentDrawTool==0
				&& m_GraphCaption.m_CurrentDrawStatus==Select_Status);
}

void CSysView::OnUpdateGraphText(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetCanvasMgr()->GetCurCanvas() != NULL);
	pCmdUI->SetCheck(m_GraphCaption.m_CurrentDrawTool==Graph_Text
				&& m_GraphCaption.m_CurrentDrawStatus==Draw_Status);
}

void CSysView::OnUpdateImportTunArc(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetCanvasMgr()->GetCurCanvas() != NULL);
}

void CSysView::OnGraphTunside() 
{
	// TODO: Add your command handler code here
	DlgGraphTunSide dlg;
	dlg.DoModal();
}

void CSysView::OnUpdateGraphTunside(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CTilCanvas* pCanvas = GetCanvasMgr()->GetCurCanvas();
	pCmdUI->Enable(pCanvas!=NULL && pCanvas->GetGraph()->GetGraphByIdx(GRAPH_LAYER_TUNARC)->Size()!=0);
}

void CSysView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// CG: This block was added by the Pop-up Menu component
	if (point.x == -1 && point.y == -1)
	{
		//keystroke invocation
		CRect rect;
		GetClientRect(rect);
		ClientToScreen(rect);

		point = rect.TopLeft();
		point.Offset(5, 5);
	}

	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_POPUP_SYS_VIEW));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;

	while (pWndPopupOwner->GetStyle() & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent();

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
		pWndPopupOwner);
}

// 生成当前掌子面特征
void CSysView::OnFaceCaptionCur() 
{
	BeginWaitCursor();

	CTilSysDoc* pDoc = GetDocument();
	CTilCanvasMgr* pMgr = GetCanvasMgr();
	CTilGraph* pGraph = pMgr->GetCurCanvas()->GetGraph();
	CString strDK = get_file_name(pGraph->GetPathName());

	// 取得当前掌子面节理、裂隙属性
	CFaceCaption* pCaption = new CFaceCaption();
	pCaption->CopyFrom(pMgr->GetCurCanvas());

	pCaption->m_strDK = strDK;
	pCaption->JointLengthofUnitArea();
	pCaption->AveRockSize(50);
	pCaption->m_fDiscontinueFreq = 1/pCaption->m_fAveRockSize;
	pCaption->RQD(50, 10);

	float fRed, fGreen, fBlue;
	pCaption->DividLinesofUnitArea(10);
	pCaption->EdgeLinesofUnitArea();
	pCaption->RockCrackDegree();
	pCaption->GreyAveValue(fRed, fGreen, fBlue);
	pCaption->GreyMean2Deviation(fRed, fGreen, fBlue);
	pCaption->RockWeathingDegree(fRed, fGreen, fBlue);
	pCaption->Description();

	int nSize = pDoc->m_dFaceCaption.size();
	if (nSize == 0)
	{
		pDoc->m_dFaceCaption.push_back(pCaption);
		FaceCaptionInfo(pDoc->m_dFaceCaption);
		EndWaitCursor();
		return;
	}

	// 如果当前图像特征存在，则改写当前图像特征
	for (int i=0; i<nSize; i++)
	{
		if (pDoc->m_dFaceCaption[i]->m_strDK == strDK)
		{
			pDoc->m_dFaceCaption[i]->m_fAveRockSize = pCaption->m_fAveRockSize;
			pDoc->m_dFaceCaption[i]->m_fAveJointLen = pCaption->m_fAveJointLen;
			pDoc->m_dFaceCaption[i]->m_fDiscontinueFreq = pCaption->m_fDiscontinueFreq;
			pDoc->m_dFaceCaption[i]->m_fRQD = pCaption->m_fRQD;
			FaceCaptionInfo(pDoc->m_dFaceCaption);
			EndWaitCursor();
			return;
		}
	}

	// 如果当前图像特征不存在且在图像之间，则将特征保存
	long lDK, lDK1, lDK2;
	lDK = get_std_mileage(strDK);
	lDK1 = get_std_mileage(pDoc->m_dFaceCaption[0]->m_strDK);
	for (i=1; i<nSize; i++)
	{
		lDK2 = get_std_mileage(pDoc->m_dFaceCaption[i]->m_strDK);
		if ((lDK-lDK1)*(lDK-lDK2)<0)
		{
			pDoc->m_dFaceCaption.insert(pDoc->m_dFaceCaption.begin()+i, pCaption);
			FaceCaptionInfo(pDoc->m_dFaceCaption);
			EndWaitCursor();
			return;
		}
		lDK1 = lDK2;
	}

	// 如果当前图像里程在所有里程之前或之后
	lDK2 = lDK1;
	lDK1 = get_std_mileage(pDoc->m_dFaceCaption[0]->m_strDK);
	if (lDK < lDK1)
	{
		pDoc->m_dFaceCaption.push_front(pCaption);
		FaceCaptionInfo(pDoc->m_dFaceCaption);
		EndWaitCursor();
		return;
	}
	else if (lDK > lDK2)
	{
		pDoc->m_dFaceCaption.push_back(pCaption);
		FaceCaptionInfo(pDoc->m_dFaceCaption);
		EndWaitCursor();
		return;
	}

	// 如果当前图像特征不存在且不在图像之间，则将特征保存
	delete pCaption;
}

void CSysView::OnUpdateFaceCaptionCur(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CTilCanvas* pCanvas = GetCanvasMgr()->GetCurCanvas();
	pCmdUI->Enable(pCanvas != NULL);// && pCanvas->GetGraph()->GetGraphByIdx(GRAPH_LAYER_TUNARC)->Size() > 0);
}

// 生成所有掌子面特征
void CSysView::OnFaceCaptionAll() 
{
	BeginWaitCursor();

	CTilSysDoc* pDoc = GetDocument();
	CTilCanvasMgr* pMgr = GetCanvasMgr();

	int i;
	for (i=0; i<pMgr->Size(); i++)
	{
		CTilCanvas* pCanvas = pMgr->GetCanvasByIdx(i);
		CTilGraph* pGraph = pCanvas->GetGraph();
		CString strDK = get_file_name(pGraph->GetPathName());

		// 取得当前掌子面节理、裂隙属性
		CFaceCaption* pCaption = new CFaceCaption();
		pCaption->CopyFrom(pCanvas);

		// 根据掌子面上的节理、裂隙等信息取得掌子面特征参数
		pCaption->m_strDK = strDK;
		pCaption->JointLengthofUnitArea();
		pCaption->AveRockSize(50);
		pCaption->m_fDiscontinueFreq = 1/pCaption->m_fAveRockSize;
		pCaption->RQD(50, 100);

		float fRed, fGreen, fBlue;
		pCaption->DividLinesofUnitArea(10);
		pCaption->EdgeLinesofUnitArea();
		pCaption->RockCrackDegree();
		pCaption->GreyAveValue(fRed, fGreen, fBlue);
		pCaption->GreyMean2Deviation(fRed, fGreen, fBlue);
		pCaption->RockWeathingDegree(fRed, fGreen, fBlue);

		int nSize = pDoc->m_dFaceCaption.size();
		if (nSize == 0)
		{
			pDoc->m_dFaceCaption.push_back(pCaption);
			FaceCaptionInfo(pDoc->m_dFaceCaption);
			EndWaitCursor();
			return;
		}

		// 如果当前图像特征存在，则改写当前图像特征
		for (i=0; i<nSize; i++)
		{
			if (pDoc->m_dFaceCaption[i]->m_strDK == strDK)
			{
				pDoc->m_dFaceCaption[i]->m_fAveRockSize = pCaption->m_fAveRockSize;
				pDoc->m_dFaceCaption[i]->m_fAveJointLen = pCaption->m_fAveJointLen;
				pDoc->m_dFaceCaption[i]->m_fDiscontinueFreq = pCaption->m_fDiscontinueFreq;
				pDoc->m_dFaceCaption[i]->m_fRQD = pCaption->m_fRQD;
				FaceCaptionInfo(pDoc->m_dFaceCaption);
				delete pCanvas;

				EndWaitCursor();
				break;
			}
		}
		if (i<nSize)
			continue;

		// 如果当前图像特征不存在且在图像之间，则将特征保存
		long lDK, lDK1, lDK2;
		lDK = get_std_mileage(strDK);
		lDK1 = get_std_mileage(pDoc->m_dFaceCaption[0]->m_strDK);
		for (i=1; i<nSize; i++)
		{
			lDK2 = get_std_mileage(pDoc->m_dFaceCaption[i]->m_strDK);
			if ((lDK-lDK1)*(lDK-lDK2)<0)
			{
				pDoc->m_dFaceCaption.insert(pDoc->m_dFaceCaption.begin()+i, pCaption);
				FaceCaptionInfo(pDoc->m_dFaceCaption);
				break;
			}
			lDK1 = lDK2;
		}
		if(i<nSize)
			continue;

		// 如果当前图像里程在所有里程之前或之后
		lDK2 = lDK1;
		lDK1 = get_std_mileage(pDoc->m_dFaceCaption[0]->m_strDK);
		if (lDK < lDK1)
		{
			pDoc->m_dFaceCaption.push_front(pCaption);
			FaceCaptionInfo(pDoc->m_dFaceCaption);
		}
		else if (lDK > lDK2)
		{
			pDoc->m_dFaceCaption.push_back(pCaption);
			FaceCaptionInfo(pDoc->m_dFaceCaption);
		}
	}

	EndWaitCursor();
}

void CSysView::OnGraphClear() 
{
	// TODO: Add your command handler code here
	CTilCanvasMgr* pMgr = GetCanvasMgr();
	CTilGraph* pGraphMgr = pMgr->GetCurCanvas()->GetGraph();
	pGraphMgr->ClearTunOutside();
	pGraphMgr->SetModifiedFlag(TRUE);
	Invalidate(TRUE);
}

void CSysView::OnUpdateGraphClear(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CTilCanvasMgr* pMgr = GetCanvasMgr();
	if (pMgr->GetCurCanvas() != NULL)
	{
		CGraph* pGraph = pMgr->GetCurCanvas()->GetGraph()->GetGraphByIdx(GRAPH_LAYER_TUNARC);
		if (pGraph->Size() == 0)
			pCmdUI->Enable(FALSE);
		else
			pCmdUI->Enable(TRUE);
	}
	else
		pCmdUI->Enable(FALSE);
}

LRESULT CSysView::OnRecogTexture(WPARAM wParam,LPARAM lParam)
{
	PRECOGTEXTURE pRecog = (PRECOGTEXTURE)wParam;
	int nNum = (int)lParam;
	CTilSysDoc* pDoc = GetDocument();

	// 将识别结果放入
	for (int i=0; i<nNum; i++)
	{
		for (int j=0; j<pDoc->m_dFaceCaption.size(); j++)
		{
			CString str1 = pDoc->m_dFaceCaption[j]->m_strDK;
			CString str2 = pRecog[i].strDK;
			str1.MakeLower();
			str2.MakeLower();
			if (str1 == str2)
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

// 岩层边界提取
#include "EdgeFind.h"
#include "Progress.h"
void CSysView::OnGraphEdge() 
{
	// TODO: Add your command handler code here
	CTilSysDoc* pDoc = GetDocument();
	pDoc->BeginWaitCursor();

	CTilCanvas* pCanvas = GetCanvasMgr()->GetCurCanvas();
	CProgress progress;
	progress.StartEdgeFind(pCanvas);
	progress.DoModal();

// 	CEdgeFind* pEdgeFind = new CEdgeFind(pCanvas);
// 	pEdgeFind->FindEdge(40, 50, 30, 20);
// 	delete pEdgeFind;
// 	
// 	pCanvas->GetGraph()->SetModifiedFlag(TRUE);
	
	pDoc->EndWaitCursor();
	Invalidate();
}

void CSysView::OnUpdateGraphEdge(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetCanvasMgr()->GetCurCanvas()!=NULL);
}

void CSysView::OnGraphExport() 
{
	ResetSelectStatus();

	CDC* pDC = GetDC();
	static char BASED_CODE szFilter[] = "supported files|*.jpeg;*.jpg;*.bmp|Jpeg Files (*.jpeg,*.jpg)|*.jpeg;*.jpg|Bmp Files (*.bmp)|*.bmp|All Files (*.*)|*.*||";
	CFileDialog dlg(false, "*.*", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_FILEMUSTEXIST, szFilter, NULL);
	if (dlg.DoModal() == IDOK)
	{
		CString strPathName = dlg.GetPathName();
		CTilCanvas* pCanvas = GetCanvasMgr()->GetCurCanvas();
		pCanvas->SaveGraphToImage(pDC,  strPathName);
	}
	ReleaseDC(pDC);
}

void CSysView::OnUpdateGraphExport(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetCanvasMgr()->GetCurCanvas()!=NULL);
}

void CSysView::OnGraphExportAll() 
{
	ResetSelectStatus();

	BROWSEINFO bi; 
    ZeroMemory(&bi,sizeof(BROWSEINFO)); 
	bi.ulFlags = BIF_STATUSTEXT | 0x40/*BIF_USENEWUI*/ | BIF_RETURNONLYFSDIRS; 
    LPITEMIDLIST pidl = SHBrowseForFolder(&bi); 
    TCHAR * path = new TCHAR[MAX_PATH]; 
    if(pidl != NULL) 
    { 
        SHGetPathFromIDList(pidl,path); 

		// 取得所有保存的文件名并保存
		CDC* pDC = GetDC();
		CTilCanvasMgr* pMgr = GetCanvasMgr();
		for (int i=0; i<pMgr->Size(); i++)
		{
			CTilCanvas* pCanvas = pMgr->GetCanvasByIdx(i);
			CString strSrcPathName = pCanvas->GetGraph()->GetPathName();
			CString strSrcFile = get_file_name(strSrcPathName);
			strSrcFile += "_gph.jpg";
			CString strDstPathName = path;
			strDstPathName += "\\"+strSrcFile;
			pCanvas->SaveGraphToImage(pDC, strDstPathName);
		}
    } 
	delete [] path;
}

void CSysView::OnUpdateGraphExportAll(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetCanvasMgr()->Size() > 0);
}

LRESULT CSysView::OnAddToFace(WPARAM wParam,LPARAM lParam)
{
	CClientDC dc(this);
	OnPrepareDC(&dc);

	CString strFileName=*(CString*)wParam;
	CString strDir = *(CString*)lParam;
	strDir += strFileName;

	// 将掌子面走向信息直接画到掌子面上
	CTilCanvasMgr* pMgr = GetCanvasMgr();
	if (pMgr->GetCurCanvas()==NULL)
	{
		AfxMessageBox("请选择图像!");
		return 0;
	}

	// 取得掌子面的外接矩形
	CTilGraph* pGraphMgr = pMgr->GetCurCanvas()->GetGraph();
	CUnit* pTunArc = pGraphMgr->GetGraphByIdx(GRAPH_LAYER_TUNARC)->GetUnitByIdx(0);
	CRect rect = pTunArc->GetPolyRect();
	rect.InflateRect(TunArc_Extend, TunArc_Extend);

	// 鉴别文件名
	CString strName = strFileName.Left(strFileName.GetLength()-4);
	if (strName == "掌子面走向")
	{
		// 设置“________\”掌子面走向
		POINT2DF pt1,pt2,pt3;	// 依次表示上面的点、右边点、左边点
		pt1.x = rect.right-250;
		pt1.y = rect.top+150;
		pt2.x = pt1.x+150;
		pt2.y = pt1.y;
		pt3.x = pt2.x-35;
		pt3.y = pt2.y-35;
		CUnit* pPoly = (CUnit*)new CPolygon;
		pPoly->m_PenColor = RGB(0,255,0);
		pPoly->m_PenWidth = 2;
		pPoly->m_pPointList->Add(pt1);
		pPoly->m_pPointList->Add(pt2);
		pPoly->m_pPointList->Add(pt3);
		pPoly->SetPolyClose(FALSE);
		CGraph* pOther = pGraphMgr->GetGraphByIdx(GRAPH_LAYER_OTHER);
		pOther->PushBackUnit(pPoly);

//		SetScrollPos(SB_HORZ, pt1.x-50);
//		SetScrollPos(SB_VERT, pt1.y+20);

		POINT pt;
		pt.x=pt1.x+10;
		pt.y=pt1.y-40;
		dc.LPtoDP(&pt);

		// 输入掌子面走向数字
		OnGraphText();
		OnLButtonDown(1, pt);

		Invalidate(FALSE);
	}
	else if (strName == "产状形式1")	// 产状形状:右上->左下
	{
		CRect rect;
		GetClientRect(&rect);
		dc.DPtoLP(&rect);

		POINT2DF pt1,pt2,pt3;
		pt1.x = rect.CenterPoint().x+50;
		pt1.y = rect.CenterPoint().y;
		pt2.x = pt1.x-150;
		pt2.y = pt1.y;
		pt3.x = pt2.x-35;
		pt3.y = pt2.y-35;

		CUnit* pPoly = (CUnit*)new CPolygon;
		pPoly->m_PenColor = RGB(0,255,0);
		pPoly->m_PenWidth = 2;
		pPoly->m_pPointList->Add(pt1);
		pPoly->m_pPointList->Add(pt2);
		pPoly->m_pPointList->Add(pt3);
		pPoly->SetPolyClose(FALSE);

		CGraph* pOther = pGraphMgr->GetGraphByIdx(GRAPH_LAYER_OTHER);
		pOther->PushBackUnit(pPoly);

		Invalidate(FALSE);
	}
	else if (strName == "产状形式2")	// 产状形状:左上->右下
	{
		CRect rect;
		GetClientRect(&rect);
		dc.DPtoLP(&rect);

		POINT2DF pt1,pt2,pt3;
		pt1.x = rect.CenterPoint().x-50;
		pt1.y = rect.CenterPoint().y;
		pt2.x = pt1.x+150;
		pt2.y = pt1.y;
		pt3.x = pt2.x+35;
		pt3.y = pt2.y-35;

		CUnit* pPoly = (CUnit*)new CPolygon;
		pPoly->m_PenColor = RGB(0,255,0);
		pPoly->m_PenWidth = 2;
		pPoly->m_pPointList->Add(pt1);
		pPoly->m_pPointList->Add(pt2);
		pPoly->m_pPointList->Add(pt3);
		pPoly->SetPolyClose(FALSE);

		CGraph* pOther = pGraphMgr->GetGraphByIdx(GRAPH_LAYER_OTHER);
		pOther->PushBackUnit(pPoly);

		Invalidate(FALSE);
	}
	else if (strName == "产状形式3")		// 产状形状:左上->右下
	{
		CRect rect;
		GetClientRect(&rect);
		dc.DPtoLP(&rect);

		POINT2DF pt1,pt2,pt3;
		pt1.x = rect.CenterPoint().x+50;
		pt1.y = rect.CenterPoint().y;
		pt2.x = pt1.x-150;
		pt2.y = pt1.y;
		pt3.x = pt2.x-35;
		pt3.y = pt2.y+35;

		CUnit* pPoly = (CUnit*)new CPolygon;
		pPoly->m_PenColor = RGB(0,255,0);
		pPoly->m_PenWidth = 2;
		pPoly->m_pPointList->Add(pt1);
		pPoly->m_pPointList->Add(pt2);
		pPoly->m_pPointList->Add(pt3);
		pPoly->SetPolyClose(FALSE);

		CGraph* pOther = pGraphMgr->GetGraphByIdx(GRAPH_LAYER_OTHER);
		pOther->PushBackUnit(pPoly);

		Invalidate(FALSE);
	}
	else if (strName = "产状形式4")		// 产状形状:右上->左下
	{
		CRect rect;
		GetClientRect(&rect);
		dc.DPtoLP(&rect);

		POINT2DF pt1,pt2,pt3;
		pt1.x = rect.CenterPoint().x-50;
		pt1.y = rect.CenterPoint().y;
		pt2.x = pt1.x+150;
		pt2.y = pt1.y;
		pt3.x = pt2.x+35;
		pt3.y = pt2.y+35;

		CUnit* pPoly = (CUnit*)new CPolygon;
		pPoly->m_PenColor = RGB(0,255,0);
		pPoly->m_PenWidth = 2;
		pPoly->m_pPointList->Add(pt1);
		pPoly->m_pPointList->Add(pt2);
		pPoly->m_pPointList->Add(pt3);
		pPoly->SetPolyClose(FALSE);

		CGraph* pOther = pGraphMgr->GetGraphByIdx(GRAPH_LAYER_OTHER);
		pOther->PushBackUnit(pPoly);

		Invalidate(FALSE);
	}

	return 0;
}

LRESULT CSysView::OnNewImage(WPARAM wParam, LPARAM lParam)
{
	CTilSysDoc* pDoc = GetDocument();
	CTilCanvas* pCanvas = GetCanvasMgr()->GetCurCanvas();
	if (pCanvas != NULL)
	{
		pDoc->UpdateAllViews(0, WM_USER_NEWIMAGE);
	}
	
	return 0;
}

void CSysView::OnUpdateCharBold(CCmdUI* pCmdUI) 
{
	CMainFrame*pMainFrame=(CMainFrame*)::AfxGetMainWnd();
	pCmdUI->SetCheck(!pMainFrame->m_wndTextBar.isBoldUp);
}

void CSysView::OnUpdateCharItalic(CCmdUI* pCmdUI) 
{
	CMainFrame*pMainFrame=(CMainFrame*)::AfxGetMainWnd();
	pCmdUI->SetCheck(!pMainFrame->m_wndTextBar.isItalicUp);
}

void CSysView::OnUpdateCharUnderline(CCmdUI* pCmdUI) 
{
	CMainFrame*pMainFrame=(CMainFrame*)::AfxGetMainWnd();
	pCmdUI->SetCheck(!pMainFrame->m_wndTextBar.isUnderlineUp);
}

// 弹出掌子面地质信息对话框
void CSysView::OnFaceInfo() 
{
	CTilCanvasMgr* pMgr = GetCanvasMgr();

	// 当所有文件命名正确了，则弹出对话框
	//CDlgFaceInfo dlg(this);
	CDlgFaceInfoEx dlg(this);
	// 读取所有的掌子面素描信息
	CString strPath, strName, strIFO;
	for (int i=0; i<pMgr->Size(); i++)
	{
		CTilCanvas* pCanvas = pMgr->GetCanvasByIdx(i);
		strPath = get_file_path(pCanvas->GetImage()->GetPathName());
		strName = get_file_name(pCanvas->GetImage()->GetPathName());
		strIFO = strPath + '\\' + strName + ".ifo";
		dlg.ReadFaceInfoAndCanvas(strIFO, pCanvas);
	}

	dlg.DoModal();
	dlg.m_faceInfoLst.RestoreOriCanvasSetting();
}

void CSysView::OnUpdateFaceInfo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CTilCanvasMgr* pMgr = GetCanvasMgr();
	pCmdUI->Enable(pMgr->Size() > 0);
}


void CSysView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// 滑动条水平滑动时，标尺作相应的滑动
	UpdateRulersInfo(RW_HSCROLL, GetScrollPosition(), nPos);
	
	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CSysView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// 滑动条垂直滑动时，标尺作相应的滑动
	UpdateRulersInfo(RW_VSCROLL, GetScrollPosition(), nPos);
	
	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CSysView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if (zDelta > 0)
		OnVScroll(SB_LINEUP, 0, NULL);
	else
		OnVScroll(SB_LINEDOWN, 0, NULL);

	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}

void CSysView::OnExportAll() 
{
	ResetSelectStatus();
	
 	BROWSEINFO bi; 
    ZeroMemory(&bi,sizeof(BROWSEINFO)); 
	bi.ulFlags = BIF_STATUSTEXT | 0x40/*BIF_USENEWUI*/ | BIF_RETURNONLYFSDIRS; 
    LPITEMIDLIST pidl = SHBrowseForFolder(&bi); 
    TCHAR * path = new TCHAR[MAX_PATH]; 
    if(pidl != NULL) 
    { 
        SHGetPathFromIDList(pidl,path); 
		
		// 取得所有保存的文件名并保存
		CDC* pDC = GetDC();
		CTilCanvasMgr* pMgr = GetCanvasMgr();
		for (int i=0; i<pMgr->Size(); i++)
		{
			CTilCanvas* pCanvas = pMgr->GetCanvasByIdx(i);
			CString strSrcPathName = pCanvas->GetGraph()->GetPathName();
			CString strSrcFile = get_file_name(strSrcPathName);
			strSrcFile += "_gphimg.jpg";
			CString strDstPathName = path;
			strDstPathName += "\\"+strSrcFile;
			pCanvas->SaveGraphImage2Image(pDC,strDstPathName);
			//pCanvas->SaveGraphToImage(pDC, strDstPathName);
		}
    } 
	delete [] path;
}

void CSysView::OnUpdateExportAll(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetCanvasMgr()->Size() > 0);
}

void CSysView::OnExportCurGraphImage() 
{
	ResetSelectStatus();
	
	CDC* pDC = GetDC();
	static char BASED_CODE szFilter[] = "supported files|*.jpeg;*.jpg;*.bmp|Jpeg Files (*.jpeg,*.jpg)|*.jpeg;*.jpg|Bmp Files (*.bmp)|*.bmp|All Files (*.*)|*.*||";
	CFileDialog dlg(false, "*.*", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_FILEMUSTEXIST, szFilter, NULL);
	if (dlg.DoModal() == IDOK)
	{
		CString strPathName = dlg.GetPathName();
		CTilCanvas* pCanvas = GetCanvasMgr()->GetCurCanvas();
		pCanvas->SaveGraphImage2Image(pDC,  strPathName);
	}
	ReleaseDC(pDC);
}

void CSysView::OnUpdateExportCurGraphImage(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetCanvasMgr()->Size() > 0);
}

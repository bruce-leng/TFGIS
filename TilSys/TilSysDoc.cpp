// TilSysDoc.cpp : implementation of the CTilSysDoc class
//

#include "stdafx.h"
#include "TilSys.h"
#include "TilSysDoc.h"
#include "MainFrm.h"
#include <process.h>

#include "TilCanvas.h"
#include "TilComFunction.h"
#include "Image.h"

#include "DlgThreshold.h"

#include "MathComput.h"
#include "triangle.h"
#include "MeshTunnel.h"
#include "Tunnel.h"
#include "EdgeFind.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


HANDLE g_hCreateTextureThread = NULL;
// 文件操作线程
void RunImageThread(void* pParam)
{
	CTilSysDoc* pDoc = (CTilSysDoc*)pParam;
	pDoc->BeginWaitCursor();

	CTilCanvasMgr* pMgr = GetCanvasMgr();
	CTilCanvas* pCanvas = pMgr->GetCurCanvas();
	CTilImage* pImageMgr = pCanvas->GetImage();
	CImage* pImage = pImageMgr->GetCurImage();

	switch (pDoc->m_lMenuCommand)
	{
		case ID_VIEW_ZOOMIN:	// 放大
			{
				long double dbZoomX, dbZoomY;
				pMgr->GetZoomFactor(dbZoomX, dbZoomY);
				pMgr->SetZoomFactor(dbZoomX*1.25, dbZoomY*1.25);
				
				// 设置滑动条范围
				SendMessage(pDoc->m_hWnd, WM_SIZE, 0, 0);
				break;
			}
		case ID_VIEW_ZOOMOUT:	// 缩小
			{
				long double dbZoomX, dbZoomY;
				pMgr->GetZoomFactor(dbZoomX, dbZoomY);
				pMgr->SetZoomFactor(dbZoomX*0.8, dbZoomY*0.8);
				
				// 设置滑动条范围
				SendMessage(pDoc->m_hWnd, WM_SIZE, 0, 0);
				break;
			}
		case ID_VIEW_ZOOMNORMAL:	// 原始尺寸
			GetCanvasMgr()->SetZoomFactor(1.0, 1.0);
			// 设置滑动条范围
			SendMessage(pDoc->m_hWnd, WM_SIZE, 0, 0);
			break;
		case ID_IMAGE_MIRROR:
			pImage->Mirror();
			break;
		case ID_IMAGE_FLIP:
			pImage->Flip();
			break;
		case ID_IMAGE_NEGATIVE:
			pImage->Negative();
			break;
		case ID_IMAGE_ROTATEL:
			pImage->RotateLeft();
			break;
		case ID_IMAGE_ROTATER:
			pImage->RotateRight();
			break;
		case ID_IMAGE_COLORIZE:
			pImage->HistogramEqualize();
			break;
		case ID_IMAGE_NORMALIZE:
			pImage->HistogramNormalize();
			break;
		case ID_IMAGE_LIGHTEN:
			pImage->Light(20);
			break;
		case ID_IMAGE_DARKEN:
			pImage->Light(-20);
			break;
		case ID_IMAGE_CONTRAST:
			pImage->Light(0, 25);
			break;
		case ID_IMAGE_LESSCONTRAST:
			pImage->Light(0, -25);
			break;
		case ID_IMAGE_MEDIAN:
			pImage->Median();
			break;
		case ID_IMAGE_ERODE:
			pImage->Erode();
			break;
		case ID_IMAGE_DILATE:
			pImage->Dilate();
			break;
		case ID_IMAGE_BLUR:
			{
				long kernel[]={1,1,1,1,8,1,1,1,1};
				pImage->Filter(kernel,3,16,0);
				break;
			}
		case ID_IMAGE_SOFTEN:
			{
				long kernel[]={1,1,1,1,1,1,1,1,1};
				pImage->Filter(kernel,3,9,0);
				break;
			}
		case ID_IMAGE_GAUSSIAN3X3:
			{
				long kernel[]={1,2,1,2,4,2,1,2,1};
				pImage->Filter(kernel,3,16,0);
				break;
			}
		case ID_IMAGE_GAUSSIAN5X5:
			{
				long kernel[]={0,1,2,1,0,1,3,4,3,1,2,4,8,4,2,1,3,4,3,1,0,1,2,1,0};
				pImage->Filter(kernel,5,52,0);
				break;
			}
		case ID_IMAGE_SHARPEN:
			{
				long kernel[]={-1,-1,-1,-1,9,-1,-1,-1,-1};
				pImage->Filter(kernel,3,1,0);
				break;
			}
		case ID_IMAGE_USM:
			pImage->UnsharpMask();
			break;
	}
	// 除了缩放以外的处理都需要对设置脏标记
	if (pDoc->m_lMenuCommand != ID_VIEW_ZOOMIN
	 && pDoc->m_lMenuCommand != ID_VIEW_ZOOMOUT
	 && pDoc->m_lMenuCommand != ID_VIEW_ZOOMNORMAL)
		pImageMgr->SetModifiedFlag(TRUE);

	// 发送消息,显示处理后的图像
	SendMessage(pDoc->m_hWnd, WM_USER_NEWIMAGE,-1,0);

	pDoc->m_hThread = 0;
	_endthread();

	pDoc->EndWaitCursor();
}
/////////////////////////////////////////////////////////////////////////////
// CTilSysDoc

IMPLEMENT_DYNCREATE(CTilSysDoc, CDocument)

BEGIN_MESSAGE_MAP(CTilSysDoc, CDocument)
	//{{AFX_MSG_MAP(CTilSysDoc)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_IMAGE_DILATE, OnImageDilate)
	ON_COMMAND(ID_IMAGE_THRESHOLD, OnImageThreshold)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_DILATE, OnUpdateImageDilate)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_THRESHOLD, OnUpdateImageThreshold)
	ON_COMMAND(ID_VIEW_IMAGE, OnViewImage)
	ON_COMMAND(ID_VIEW_GRAPH, OnViewGraph)
	ON_COMMAND(ID_VIEW_ZOOMIN, OnViewZoomin)
	ON_COMMAND(ID_VIEW_ZOOMOUT, OnViewZoomout)
	ON_COMMAND(ID_IMAGE_MIRROR, OnImageMirror)
	ON_COMMAND(ID_IMAGE_FLIP, OnImageFlip)
	ON_COMMAND(ID_IMAGE_NEGATIVE, OnImageNegative)
	ON_COMMAND(ID_IMAGE_ROTATEL, OnImageRotatel)
	ON_COMMAND(ID_IMAGE_ROTATER, OnImageRotater)
	ON_COMMAND(ID_IMAGE_ROTATE, OnImageRotate)
	ON_COMMAND(ID_IMAGE_CROP, OnImageCrop)
	ON_UPDATE_COMMAND_UI(ID_VIEW_IMAGE, OnUpdateViewImage)
	ON_UPDATE_COMMAND_UI(ID_VIEW_GRAPH, OnUpdateViewGraph)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMIN, OnUpdateViewZoomin)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMOUT, OnUpdateViewZoomout)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_MIRROR, OnUpdateImageMirror)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_FLIP, OnUpdateImageFlip)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_NEGATIVE, OnUpdateImageNegative)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_ROTATEL, OnUpdateImageRotatel)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_ROTATER, OnUpdateImageRotater)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_ROTATE, OnUpdateImageRotate)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_CROP, OnUpdateImageCrop)
	ON_COMMAND(ID_VIEW_ZOOM, OnViewZoom)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM, OnUpdateViewZoom)
	ON_COMMAND(ID_IMAGE_COLORIZE, OnImageColorize)
	ON_COMMAND(ID_IMAGE_LIGHTEN, OnImageLighten)
	ON_COMMAND(ID_IMAGE_DARKEN, OnImageDarken)
	ON_COMMAND(ID_IMAGE_CONTRAST, OnImageContrast)
	ON_COMMAND(ID_IMAGE_LESSCONTRAST, OnImageLesscontrast)
	ON_COMMAND(ID_IMAGE_MEDIAN, OnImageMedian)
	ON_COMMAND(ID_IMAGE_ERODE, OnImageErode)
	ON_COMMAND(ID_IMAGE_BLUR, OnImageBlur)
	ON_COMMAND(ID_IMAGE_SOFTEN, OnImageSoften)
	ON_COMMAND(ID_IMAGE_GAUSSIAN3X3, OnImageGaussian3x3)
	ON_COMMAND(ID_IMAGE_GAUSSIAN5X5, OnImageGaussian5x5)
	ON_COMMAND(ID_IMAGE_SHARPEN, OnImageSharpen)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_COLORIZE, OnUpdateImageColorize)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_LIGHTEN, OnUpdateImageLighten)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_DARKEN, OnUpdateImageDarken)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_CONTRAST, OnUpdateImageContrast)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_LESSCONTRAST, OnUpdateImageLesscontrast)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_MEDIAN, OnUpdateImageMedian)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_ERODE, OnUpdateImageErode)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_BLUR, OnUpdateImageBlur)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_SOFTEN, OnUpdateImageSoften)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_GAUSSIAN3X3, OnUpdateImageGaussian3x3)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_GAUSSIAN5X5, OnUpdateImageGaussian5x5)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_SHARPEN, OnUpdateImageSharpen)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMNORMAL, OnUpdateViewZoomnormal)
	ON_COMMAND(ID_VIEW_ZOOMNORMAL, OnViewZoomnormal)
	ON_COMMAND(ID_OPENGL_UPDATE_DATA, OnOpenglUpdateData)
	ON_UPDATE_COMMAND_UI(ID_OPENGL_UPDATE_DATA, OnUpdateOpenglUpdateData)
	ON_COMMAND(ID_GL_VERTEX, OnGlVertex)
	ON_COMMAND(ID_GL_LINE, OnGlLine)
	ON_COMMAND(ID_GL_FACE, OnGlFace)
	ON_COMMAND(ID_GL_SMOOTH, OnGlSmooth)
	ON_COMMAND(ID_GL_LIGHT, OnGlLight)
	ON_UPDATE_COMMAND_UI(ID_GL_VERTEX, OnUpdateGlVertex)
	ON_UPDATE_COMMAND_UI(ID_GL_LINE, OnUpdateGlLine)
	ON_UPDATE_COMMAND_UI(ID_GL_FACE, OnUpdateGlFace)
	ON_UPDATE_COMMAND_UI(ID_GL_SMOOTH, OnUpdateGlSmooth)
	ON_UPDATE_COMMAND_UI(ID_GL_LIGHT, OnUpdateGlLight)
	ON_COMMAND(ID_MESH_LOOP, OnMeshLoop)
	ON_UPDATE_COMMAND_UI(ID_MESH_LOOP, OnUpdateMeshLoop)
	ON_COMMAND(ID_GL_TUNARC, OnGlTunarc)
	ON_UPDATE_COMMAND_UI(ID_GL_TUNARC, OnUpdateGlTunarc)
	ON_COMMAND(ID_GL_ADD_WIREFRAME, OnGlAddWireframe)
	ON_UPDATE_COMMAND_UI(ID_GL_ADD_WIREFRAME, OnUpdateGlAddWireframe)
	ON_COMMAND(ID_FILE_RESUME, OnFileResume)
	ON_UPDATE_COMMAND_UI(ID_FILE_RESUME, OnUpdateFileResume)
	ON_COMMAND(ID_IMAGE_STRETCH, OnImageStretch)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_STRETCH, OnUpdateImageStretch)
	ON_COMMAND(ID_IMAGE_NORMALIZE, OnImageNormalize)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_NORMALIZE, OnUpdateImageNormalize)
	ON_COMMAND(ID_DRILL_RANGE, OnDrillRange)
	ON_UPDATE_COMMAND_UI(ID_DRILL_RANGE, OnUpdateDrillRange)
	ON_COMMAND(ID_GEO_PREDICT, OnGeoPredict)
	ON_UPDATE_COMMAND_UI(ID_GEO_PREDICT, OnUpdateGeoPredict)
	ON_COMMAND(ID_TUN_SIDE, OnTunSide)
	ON_UPDATE_COMMAND_UI(ID_TUN_SIDE, OnUpdateTunSide)
	ON_COMMAND(ID_TUN_STRUCT, OnTunStruct)
	ON_UPDATE_COMMAND_UI(ID_TUN_STRUCT, OnUpdateTunStruct)
	ON_COMMAND(ID_TUN_DRILL, OnTunDrill)
	ON_UPDATE_COMMAND_UI(ID_TUN_DRILL, OnUpdateTunDrill)
	ON_COMMAND(ID_TUN_FACE, OnTunFace)
	ON_UPDATE_COMMAND_UI(ID_TUN_FACE, OnUpdateTunFace)
	ON_COMMAND(ID_SECTION_NORMAL, OnSectionNormal)
	ON_UPDATE_COMMAND_UI(ID_SECTION_NORMAL, OnUpdateSectionNormal)
	ON_COMMAND(ID_SECTION_PROFILE, OnSectionProfile)
	ON_UPDATE_COMMAND_UI(ID_SECTION_PROFILE, OnUpdateSectionProfile)
	ON_COMMAND(ID_IMAGE_USM, OnImageUsm)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_USM, OnUpdateImageUsm)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_VIEW_GRAPH_ROCKS, ID_VIEW_GRAPH_TUNARC, OnViewGraphLayer)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_GRAPH_ROCKS, ID_VIEW_GRAPH_TUNARC, OnUpdateViewGraphLayer)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTilSysDoc construction/destruction

CTilSysDoc::CTilSysDoc()
{
	// TODO: add one-time construction code here
	m_glShowInfo.bTunArc = TRUE;
	m_glShowInfo.AddWireframe = 0;
	m_glShowInfo.Smooth = 1;
	m_glShowInfo.PolygonOffset = -1.0f;
	m_glShowInfo.Light = TRUE;
	m_glShowInfo.Mode = GL_FILL;

	m_bTunSide = TRUE;
	m_bTunStruct = TRUE;
	m_bTunFace = TRUE;
	m_hmax = 0;

	m_hThread = NULL;
	m_lMenuCommand = 0;
	m_hWnd = NULL;
}

CTilSysDoc::~CTilSysDoc()
{
	//INVALID_HANDLE_VALUE
	if(g_hCreateTextureThread)
	{
		::CloseHandle(g_hCreateTextureThread);
		g_hCreateTextureThread = NULL;
	}
	if (m_hThread)
	{
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
	}

	for (int i=0; i<m_dFaceCaption.size(); i++)
		delete m_dFaceCaption[i];
	m_dFaceCaption.clear();
}

/////////////////////////////////////////////////////////////////////////////
// CTilSysDoc serialization

void CTilSysDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTilSysDoc diagnostics

#ifdef _DEBUG
void CTilSysDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTilSysDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTilSysDoc commands


DWORD WINAPI CreateTextureThreadProc( LPVOID lpParameter  )
{
	CTilSysDoc* pDoc = (CTilSysDoc*)lpParameter;
	
#ifdef CREATE_TEXTURE_BYTHREAD
	LogInfo("创建纹理图像!");
	pDoc->CreateTexture();
	LogInfo("纹理创建完成!");
	pDoc->m_glShowInfo.bCreateTextureIsFinished = TRUE;
#endif

	return 0;
}

// 打开多个文件并添加到当前项目中
void CTilSysDoc::OnFileOpen() 
{
	CFileDialog dlgFile(TRUE);
	CString title="打开掌子面图像";
	CString strInitialDir = AfxGetApp()->GetProfileString("Dir", "Browser");
	CString strFilter = ((CTilSysApp*)AfxGetApp())->GetFileTypes(TRUE);

	char buf[100000];
	memset(buf,NULL,100000);

	dlgFile.m_ofn.Flags |= OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT;
	dlgFile.m_ofn.nFilterIndex = 0;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrInitialDir = strInitialDir;
	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.nMaxFile = 100000;
	dlgFile.m_ofn.lpstrFile = buf;

	BOOL bRet = FALSE;;
	if (dlgFile.DoModal() == IDOK)
		bRet = TRUE;

	CTilCanvasMgr* pMgr = GetCanvasMgr();
	if (bRet)
	{
		POSITION pos = dlgFile.GetStartPosition(); 
		while (pos != NULL) 
		{
			// 如果原来的项目中有相同的文件，则不添加该文件
			CString str = dlgFile.GetNextPathName(pos);
			for (int i = 0; i < pMgr->Size(); i ++)
			{
				CString strTmp = pMgr->GetCanvasByIdx(i)->GetImage()->GetPathName();
				CString str1 = str.Left(str.GetLength()-4);	// 去掉扩展名
				CString strTmp1 = strTmp.Left(strTmp.GetLength()-4);
				if (str1 == strTmp1)
					break;
			}
			if (i < pMgr->Size())
			{
				LogInfo("掌子面图像 "+str+" 已经存在!");
				continue;
			}
			else
			{
				CTilImage* pImageMgr = NULL;
				CTilGraph* pGraphMgr = NULL;
				CString strPathImage, strPathGraph;

				// 创建图像文件对象
				strPathImage = get_path_image(str);
				::CopyFile(str, strPathImage, TRUE);
//				pImageMgr = new CTilImage(strPathImage);

				// 创建图形文件对象
				strPathGraph = get_path_graph(str);
				HANDLE hHandle = ::CreateFile(strPathGraph, 0, 0, NULL, OPEN_ALWAYS,
														FILE_ATTRIBUTE_NORMAL, NULL);
				::CloseHandle(hHandle);
//				pGraphMgr = new CTilGraph(strPathGraph);
				if (pMgr->PushBackCanvas(new CTilCanvas(strPathImage, strPathGraph)))
					LogInfo("装载掌子面图像 "+str+" 成功!");
			}
		}
	}

	AfxGetApp()->WriteProfileString("Dir", "Browser", dlgFile.GetPathName());

	UpdateDrawTree();
}

// 将文件还原到上次保存状态
void CTilSysDoc::OnFileResume() 
{
	// TODO: Add your command handler code here
	BeginWaitCursor();

	CTilCanvas* pCanvas = GetCanvasMgr()->GetCurCanvas();
	long double dbRatioX;
	pCanvas->GetZoomFactor(dbRatioX, dbRatioX);

	CString strImage = pCanvas->GetImage()->GetPathName();
	CString strGraph = pCanvas->GetGraph()->GetPathName();

	pCanvas->Open(strImage, strGraph);
	pCanvas->SetZoomFactor(dbRatioX, dbRatioX);
	pCanvas->SetModifiedFlag(FALSE);

	UpdateAllViews(NULL);
	EndWaitCursor();
}

void CTilSysDoc::OnUpdateFileResume(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CTilCanvas* pCanvas = GetCanvasMgr()->GetCurCanvas();
	if (pCanvas == NULL)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(pCanvas->IsModified());
}

// 保存当前切片的图形和图像文件
void CTilSysDoc::OnFileSave() 
{
	CTilCanvas* pCanvas = GetCanvasMgr()->GetCurCanvas();
	if (!pCanvas)
		return;

	if (pCanvas->IsModified())
		pCanvas->Save();

	pCanvas->SetModifiedFlag(FALSE);
/*
	CFileDialog dlgFile(FALSE);
	CString title;
	CString strInitialDir = AfxGetApp()->GetProfileString("Dir", "Browser");
	CString strFilter = ((CTilSysApp*)AfxGetApp())->GetFileTypes(FALSE);

	dlgFile.m_ofn.Flags |= OFN_OVERWRITEPROMPT;
	dlgFile.m_ofn.nFilterIndex = 0;
	dlgFile.m_ofn.lpstrInitialDir = strInitialDir;
	dlgFile.m_ofn.lpstrFilter = strFilter;

	// 保存图像文件
	title="保存切片图像文件";
	dlgFile.m_ofn.lpstrTitle = title;
	if (pCanvas->GetImage()->GetPathName().IsEmpty())
	{
		if (dlgFile.DoModal() == IDOK)
		{
			CString strPathName = dlgFile.GetPathName();
			pCanvas->GetImage()->SaveAs(strPathName);
			AfxGetApp()->WriteProfileString("Dir", "Browser", strPathName);
		}
	}
	else
		pCanvas->GetImage()->Save();

	// 保存图形文件
	strInitialDir = AfxGetApp()->GetProfileString("Dir", "Browser");
	dlgFile.m_ofn.lpstrInitialDir = strInitialDir;
	title = "保存切片图形文件";
	dlgFile.m_ofn.lpstrTitle = title;
	if (pCanvas->GetGraph()->GetPathName().IsEmpty())
	{
		if (dlgFile.DoModal() == IDOK)
		{
			CString strPathName = dlgFile.GetPathName();
			pCanvas->GetGraph()->SaveAs(strPathName);
			AfxGetApp()->WriteProfileString("Dir", "Browser", strPathName);
		}
	}
	else
		pCanvas->GetGraph()->Save();
*/
}

void CTilSysDoc::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CTilCanvas* pCanvas = GetCanvasMgr()->GetCurCanvas();
	if (pCanvas != NULL)
	{
		pCmdUI->Enable(pCanvas->IsModified());
	}
	else
		pCmdUI->Enable(FALSE);
}

BOOL CTilSysDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	GetCanvasMgr()->RemoveAll();
	UpdateDrawTree();

	SetTitle(_T("未命名"));
//	// (SDI documents will reuse this document)
//	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
//	pFrame->InsertProjToSolExplorer(GetTitle());

	return TRUE;
}

// 打开"*.stu"项目文件
BOOL CTilSysDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	CString strFileInfo;
	strFileInfo.Format("正在读取文件 %s", lpszPathName);
	LogInfo(strFileInfo);

	m_strFileName = lpszPathName;
	// 打开项目文件并装载切片列表
	LogInfo("装载文件列表...");
	GetCanvasMgr()->Open(lpszPathName);
	LogInfo("文件列表装载完成...");

	// 暂时取消纹理创建(2014.12.09)
	g_hCreateTextureThread = ::CreateThread(NULL, 0, CreateTextureThreadProc, this, 0, NULL);

	// 设置当前默认图形层为节理层
	((CMainFrame*)AfxGetMainWnd())->m_ComboCurLayer.SetCurSel(GRAPH_LAYER_JOINT);

	UpdateDrawTree();

	// 删除以前已提取的所有掌子面特征信息
	FaceCaptionInfoClear(m_dFaceCaption);	

#ifndef CREATE_TEXTURE_BYTHREAD
	CreateTexture();
	LogInfo("纹理创建完成!");

// 	int nNum = m_dFaceCaption.size();
// 	for (int i=0; i<nNum; i++)
// 	{
// 		delete m_dFaceCaption[i];
// 	}
// 	m_dFaceCaption.clear();
// 	FaceCaptionInfo(m_dFaceCaption);

#endif

	SetModifiedFlag(FALSE);     // start off with unmodified

	return TRUE;
}

void CTilSysDoc::UpdateDrawTree()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->InsertProjToSolExplorer(GetCanvasMgr());
}

void CTilSysDoc::OnViewGraphLayer(UINT nID)
{
	CTilGraph* pGraphMgr = GetCanvasMgr()->GetCurCanvas()->GetGraph();
	CGraph* pGraph = NULL;
	int nLayerIdx;
	switch (nID)
	{
	case ID_VIEW_GRAPH_ROCKS:
		nLayerIdx = GRAPH_LAYER_ROCKS;
		break;
	case ID_VIEW_GRAPH_JOINT:
		nLayerIdx = GRAPH_LAYER_JOINT;
		break;
	case ID_VIEW_GRAPH_TUNARC:
		nLayerIdx = GRAPH_LAYER_TUNARC;
	}

	pGraph = pGraphMgr->GetGraphByIdx(nLayerIdx);

	// 对每一图像切片的当前图形层都设置显示模式
	BOOL bShow = pGraph->GetShowMode();
	for (int i=0; i<GetCanvasMgr()->Size(); i++)
	{
		CTilCanvas* pCanvas = GetCanvasMgr()->GetCanvasByIdx(i);
		CGraph* pGraph = pCanvas->GetGraph()->GetGraphByIdx(nLayerIdx);
		pGraph->SetShowMode(!bShow);
	}

	UpdateAllViews(NULL);
}

void CTilSysDoc::OnUpdateViewGraphLayer(CCmdUI* pCmdUI)
{
	CTilCanvas* pCanvas = GetCanvasMgr()->GetCurCanvas();
	if (!pCanvas)
		pCmdUI->Enable(FALSE);	// 若没有画布，则禁用
	else
	{
		if (!pCanvas->GetGraph()->GetShowMode())
		{// 如果总的图形层未显示，则各图形层均不显示
			pCmdUI->Enable(FALSE);
		}
		else
		{
			pCmdUI->Enable(TRUE);
			
			// 设置是否选中了当前图形层
			CTilGraph* pGraphMgr = pCanvas->GetGraph();
			BOOL bShow; 
			switch (pCmdUI->m_nID)
			{
			case ID_VIEW_GRAPH_ROCKS:
				bShow = pGraphMgr->GetGraphByIdx(GRAPH_LAYER_ROCKS)->GetShowMode();
				break;
			case ID_VIEW_GRAPH_JOINT:
				bShow = pGraphMgr->GetGraphByIdx(GRAPH_LAYER_JOINT)->GetShowMode();
				break;
			case ID_VIEW_GRAPH_TUNARC:
				bShow = pGraphMgr->GetGraphByIdx(GRAPH_LAYER_TUNARC)->GetShowMode();
			}
			pCmdUI->SetCheck(bShow);
		}
	}

}

// 渲染场景
void CTilSysDoc::RenderScene(BOOL bSelectMode)
{
	// Main drawing
	//单面颜色设置
// 	if (m_glShowInfo.bShowDoubleFace)
// 		::glPolygonMode(GL_FRONT,m_glShowInfo.Mode);
// 	else
// 		::glPolygonMode(GL_FRONT_AND_BACK ,m_glShowInfo.Mode);

	::glPolygonMode(GL_FRONT,GL_FILL);
	if(m_glShowInfo.Light)
		::glEnable(GL_LIGHTING);
	else
		::glDisable(GL_LIGHTING);

	//	if (bSelectMode)
	//		glRenderMode(GL_SELECT);
	//	else
	//		glRenderMode(GL_RENDER);
	
	if (m_glShowInfo.bCreateTextureIsFinished)
	{
		m_SceneGraph.RebuildTexture();	
	}

	m_SceneGraph.glDraw();
	
	// Add wireframe (no light, and line mode)
	if(m_glShowInfo.AddWireframe)
	{
		// Set state
		::glDisable(GL_LIGHTING);
		::glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		::glEnable(GL_POLYGON_OFFSET_LINE);
		::glPolygonOffset(m_glShowInfo.PolygonOffset,-1.0f);

		// Draw again...
		m_SceneGraph.glDraw(TYPE_MESH3D);
		
		// Restore light and mode
		::glDisable(GL_POLYGON_OFFSET_LINE);
		::glEnable(GL_LIGHTING);
	}
}

// 创建曲面对象
void CTilSysDoc::CreateMeshByObject()
{
	// 释放掉以前的所有顶点
	m_SceneGraph.FreeVertext();

	// 创建隧道拱形
	if (m_glShowInfo.bTunArc)
		CreateArcFace();

	POINT3DF ptCenter;
	POINT2DF ptImgCenter;
	int nCanvasNum = GetCanvasMgr()->Size();
	POINT_3F pt1[40], pt2[40];
	int nNum1, nNum2, i, k, m;
//	float z1, z2, x[2], y[2];
	int nLink;
//	POINT_3F pt1mean,pt2mean;
	int m1, k1;

	long lK1, lK2, lK3, lK4;
	CRect rtTunArc;
	if (nCanvasNum > 0)
	{
		CTilCanvas* pCanvas = GetCanvasMgr()->GetCanvasByIdx(0);
		CGraph* pGraph = pCanvas->GetGraph()->GetGraphByIdx(GRAPH_LAYER_TUNARC);
		rtTunArc = pGraph->GetUnitByIdx(0)->GetPolyRect();
		lK1 = get_std_mileage(pCanvas->GetGraph()->GetPathName());
	}

	float fDist, fDist1;
	fDist = 0.0;
	for (int nIdx=2; i<3; i++)
	for (m=0; m<nCanvasNum; m++)
	{
		// 取得岩层目标层中岩层的数目
		CTilCanvas* pCanvas1 = GetCanvasMgr()->GetCanvasByIdx(m);
		CGraph* pGraph1 = pCanvas1->GetGraph()->GetGraphByIdx(nIdx);
		int nRockNum = pGraph1->Size();

		lK2 = get_std_mileage(pCanvas1->GetGraph()->GetPathName());
		fDist += 0.2*abs(lK2-lK1)/3.0;
		lK1 = lK2;

		for (k=0; k<nRockNum; k++)
		{
			CUnit* pUnit1 = pGraph1->GetUnitByIdx(k);

			// 若前向链接为-1，则该目标已经在前面被处理过了，故这里不能为-1
			if (pUnit1->GetPreLink()!=-1 || pUnit1->GetNextLink()==-1)
				continue;

			// 创建网格对象并将其添加到场景
			CMesh3d* pMesh = new CMesh3d;
			m_SceneGraph.Add(pMesh);

			// 设置网格中顶点个数和三角片个数
			pMesh->SetMeshID(m, k);
			pMesh->SetNbVertex(0);
			pMesh->SetNbFace(0);

			m1 = m;		//切片号
			k1 = k;		//切片中的对象号

			fDist1 = fDist;
			lK3 = lK2;

			// 为前后两个对应的目标创建边接
			nLink = pUnit1->GetNextLink();
			while (nLink != -1)
			{
				nNum1 = pUnit1->GetPointNum();

				// 得到下一画布被连接的目标的顶点数
				CTilCanvas* pCanvas2 = GetCanvasMgr()->GetCanvasByIdx(m1+1);
				CGraph* pGraph2 = pCanvas2->GetGraph()->GetGraphByIdx(nIdx);
				CUnit* pUnit2 = pGraph2->GetUnitByIdx(nLink);
				nNum2 = pUnit2->GetPointNum();

				lK4 = get_std_mileage(pCanvas2->GetGraph()->GetPathName());
// 				nWidth = pCanvas1->GetWidth();
// 				nHeight = pCanvas1->GetHeight();

//				//修改锚点设置
//				pMesh->SetLockOffset(x[0] / nWidth, y[0] / nHeight);

				// 获得中心点
//				pCanvas1->GetCenterImgCoord(ptCenter.x, ptCenter.y);
//				ptCenter.x /= rtTunArc.Width();
//				ptCenter.y /= rtTunArc.Width();
//				ptCenter.z = fDist1;

				ptImgCenter = pCanvas1->GetGraph()->GetTunArc()->GetImgCenter();
				ptCenter.x = ptImgCenter.x/rtTunArc.Width();
				ptCenter.y = ptImgCenter.y/rtTunArc.Width();
				ptCenter.z = fDist1;

				TRACE("\n");
				for (i=0; i<nNum1; i++)
				{
					POINT2DF pt = pUnit1->GetPoint(i);
					pt1[i].x = pt.x/rtTunArc.Width();
					pt1[i].y = pt.y/rtTunArc.Width();
					pt1[i].z = fDist1;
//					TRACE("pt[%d](%4.3f,%4.3f,%4.3f)\n", m1, pt.x, pt.y, pt1[i].z);
//					TRACE("pt[%d](%4.3f,%4.3f,%4.3f)\n", m1, pt1[i].x, pt1[i].y, pt1[i].z);
					pt1[i].x = ptCenter.x-pt1[i].x;
					pt1[i].y -= ptCenter.y;
				}

// 				nWidth = pCanvas2->GetWidth();
// 				nHeight = pCanvas2->GetHeight();

				// 获得中心点
				fDist1 += 0.2*abs(lK4-lK3)/3;
//				pCanvas2->GetCenterImgCoord(ptCenter.x, ptCenter.y);
//				ptCenter.x /= rtTunArc.Width();
//				ptCenter.y /= rtTunArc.Width();
//				ptCenter.z = fDist1;
//
				ptImgCenter = pCanvas1->GetGraph()->GetTunArc()->GetImgCenter();
				ptCenter.x = ptImgCenter.x/rtTunArc.Width();
				ptCenter.y = ptImgCenter.y/rtTunArc.Width();
				ptCenter.z = fDist1;

				for (i=0; i<nNum2; i++)
				{
					POINT2DF pt = pUnit2->GetPoint(i);
					pt2[i].x = pt.x/rtTunArc.Width();
					pt2[i].y = pt.y/rtTunArc.Width();
					pt2[i].z = fDist1;
//					TRACE("pt[%d](%4.3f,%4.3f,%4.3f)\n", m1+1, pt.x, pt.y, pt2[i].z);
//					TRACE("pt[%d](%4.3f,%4.3f, %4.3f)\n", m1+1, pt2[i].x, pt2[i].y, pt2[i].z);
					pt2[i].x = ptCenter.x-pt2[i].x;
					pt2[i].y -= ptCenter.y;
				}

				if (!pUnit1->IsPolyClose())
					ConstructCrack(m_SceneGraph, pMesh, STRUCT_FACE, pt1, nNum1, pt2, nNum2, m1+1, m1+1);
				else
				{
					ConstructSidePlaneA(m_SceneGraph, pMesh, pt1, nNum1, pt2, nNum2);
					ConstructFace(m_SceneGraph, TOP_FACE, pt1, nNum1, m1, m1);
					ConstructFace(m_SceneGraph, BOTTOM_FACE, pt2, nNum2, m1+1, m1+1);
				}

				m1 = m1+1;
				k1 = nLink;
				nLink = pUnit2->GetNextLink();
				pUnit1 = pUnit2;
				pCanvas1 = pCanvas2;
				lK3 = lK4;
			}
/*			
		////////////////卫洪春
			for(i = 0; i < nNum1; ++i)
			{
				pt1[i].x = pt1[i].x + x[0]/ nWidth;
				pt1[i].y = pt1[i].y + y[0] / nHeight;
				pt1[i].z = z1;
				}

			for(i = 0; i < nNum2; ++i)
			{
				pt2[i].x = pt2[i].x + x[1] / nWidth ;
				pt2[i].y = pt2[i].y + y[1] / nHeight;
				pt2[i].z = z2;
			}

			double **pp1=new double* [nNum1];
			for(i=0;i<nNum1;i++)
				pp1[i]=new double [3];

			double **pp2 = new double* [nNum2];  //与第二个平面p1相交的点
			for (i = 0; i < nNum2; i ++)
				pp2[i] = new double [3];
	
			for(i = 0; i < nNum1; i++)
			{
				pp1[i][0]=pt1[i].x;
				pp1[i][1]=pt1[i].y;
				pp1[i][2]=pt1[i].z;
			}
	
			for(i =0;i<nNum2;i++)
			{
				pp2[i][0]=pt2[i].x;
				pp2[i][1]=pt2[i].y;
				pp2[i][2]=pt2[i].z;
			}

			double m_section[4]={0,0,1,-0.3};	//截平面方程系数矩阵

			double **interPoint=new double* [NN];
			for(i = 0; i < NN; i ++)
				interPoint[i]=new double [3];

			DisplaySection(pp1,nNum1,pp2,nNum2,m_section,interPoint);

			for(i = 0; i < nNum2; ++i)
			{
				pt2[i].x = pt2[i].x - x[1] / nWidth;
				pt2[i].y = pt2[i].y - y[1] / nHeight;
				pt2[i].z = z2;
			}

			PPOINT_3F pFore=new POINT_3F [NN];
			for(i=0;i<NN;i++)
			{
				pFore[i].x=interPoint[i][0]-x[1] / nWidth ;
				pFore[i].y=interPoint[i][1]-y[1] / nHeight;
				pFore[i].z=interPoint[i][2];
			}

//			CMesh3d* pMesh1 = new CMesh3d;//创建网格对象
//
//			m_SceneGraph.Add(pMesh1);//将网格对象添加到场景
//
//			pMesh1->SetMeshID(m, k+1);
//			pMesh1->SetNbVertex(0);//设置网格中顶点个数
//			pMesh1->SetNbFace(0);//设置网格中三角片个数	
	
			ForecastPlaneC(m_SceneGraph, pMesh, pt2, nNum2,  pFore, NN,m1,m_DataFile);

			delete [] pp1;
			delete [] pp2;
			delete [] pFore;
			delete [] interPoint;
//			delete [] pMesh1;
		//////////////卫洪春
*/
		}	
	}
}

// 创建隧道拱形
void CTilSysDoc::CreateArcFace()
{
	CTilCanvasMgr* pMgr = GetCanvasMgr();
	if (pMgr->Size() <= 0)
		return;

	int i,j;

	// 创建网络并将网格对象添加到场景
	CMesh3d* pMesh = new CMesh3d;
	pMesh->SetMeshType(ARC_FACE);
	m_SceneGraph.Add(pMesh);

//	RECT rcPre, rcPost;

	// 设置网格中顶点个数和三角片个数
	pMesh->SetNbVertex(0);
	pMesh->SetNbFace(0);

// 	// 取得网格中顶点数据指针及三角面片数据指针
// 	CArray3d<CVertex3d>* pVerArr = pMesh->GetArrayVertex();
// 	CArray3d<CFace3d> *pArrayFace = pMesh->GetArrayFace();

	// 取得隧道中心点的图像坐标作为三维坐标原点
	POINT_3F ptCenter;
	double Dx, Dy;
// 	pMgr->GetCanvasByIdx(0)->GetCenterImgCoord(Dx, Dy);
// 	ptCenter.x = Dx;
// 	ptCenter.y = Dy;
// 	ptCenter.z = 0;

	float x, y, z;
	POINT_3F pt1[40], pt2[40];
	int nNum1, nNum2;

	int nFaceIdx = 0;
	int nVerIdx = 0;
	int nWidth, nHeight;

	x = y = z = 0.0;
	nWidth = pMgr->GetCanvasByIdx(0)->GetWidth();
	nHeight = pMgr->GetCanvasByIdx(0)->GetHeight();
	pMesh->SetLockOffset(x/nWidth, y/nHeight);

	int nCanvasNum = pMgr->Size();
	CGraph* pTunArc = pMgr->GetCanvasByIdx(0)->GetGraph()->GetGraphByIdx(GRAPH_LAYER_TUNARC);
	CRect rtTunArc = pTunArc->GetUnitByIdx(0)->GetPolyRect();
	int nLength = rtTunArc.Width();
	nNum1 = pTunArc->GetUnitByIdx(0)->GetPointNum(); 

	pMgr->GetCanvasByIdx(0)->GetCenterImgCoord(Dx, Dy);
	ptCenter.x = Dx/nLength;
	ptCenter.y = Dy/nLength;
	ptCenter.z = 0.0;
	for (j=0; j<nNum1; j++)
	{
		// 取得第0个目标即隧道断面轮廓层目标上的点
		POINT2DF pt = pTunArc->GetUnitByIdx(0)->GetPoint(j);

		pt1[j].x = pt.x/nLength;
		pt1[j].y = pt.y/nLength;
		pt1[j].z = 0.0;

		// 设置以中心点为中心的各点坐标
		pt1[j] -= ptCenter;
	}

	// 计算两拱形在三维视图中的位置
	float fDist = 0.0;
	long lK1, lK2;
	lK1 = get_std_mileage(pMgr->GetCanvasByIdx(0)->GetGraph()->GetPathName());

	for (i=1; i<nCanvasNum; i++)
	{
		pTunArc = pMgr->GetCanvasByIdx(i)->GetGraph()->GetGraphByIdx(GRAPH_LAYER_TUNARC);
		nNum2 = pTunArc->GetUnitByIdx(0)->GetPointNum(); // 这里默认拱形一定存在，如果不存在，则会出错

		lK2 = get_std_mileage(pMgr->GetCanvasByIdx(i)->GetGraph()->GetPathName());
		fDist += 0.2*abs(lK2-lK1)/3.0;
		lK1 = lK2;

		// 取得每一掌子面的中心位置
		pMgr->GetCanvasByIdx(i)->GetCenterImgCoord(Dx, Dy);
		ptCenter.x = Dx/nLength;
		ptCenter.y = Dy/nLength;
		ptCenter.z = fDist;

		//调整坐标,原点位于锚点的位置
		for (j=0; j<nNum2; j++)
		{
			POINT2DF pt = pTunArc->GetUnitByIdx(0)->GetPoint(j);
			pt2[j].x = pt.x/nLength;
			pt2[j].y = pt.y/nLength;
			pt2[j].z = fDist;

			pt2[j].x -= ptCenter.x;
			pt2[j].y -= ptCenter.y;
		}

		ConstructSidePlaneA(m_SceneGraph, pMesh, pt1, nNum1, pt2, nNum2);

		for (j=0; j<nNum2; j++)
		{
			pt1[j] = pt2[j];
		}
		nNum1 = nNum2;
	}

	pMesh->Rebuild();
}

void CTilSysDoc::ConstructSidePlaneA(CSceneGraph3d &pSceneGraph, CMesh3d* pMesh, 
									 PPOINT_3F pObj1, int nNum1, PPOINT_3F pObj2, int nNum2) 
									//int nTextureIdx, int nCurSliceIdx)
{
	float MinLen, fMinTemp; 
	int MinNum;
	int i, j;

	PROPERTY pro1, pro2, proMean, prodiff;
	CalProperty(pObj1, nNum1, pro1, 1, 1);
	CalProperty(pObj2, nNum2, pro2, 1, 1);

	proMean.fArea = (pro1.fArea+pro2.fArea)/2;
	proMean.ptMean.x = (pro1.ptMean.x+pro2.ptMean.x)/2;
	proMean.ptMean.y = (pro1.ptMean.y+pro2.ptMean.y)/2;

	prodiff.fArea = pro1.fArea-pro2.fArea;
	prodiff.ptMean.x = pro1.ptMean.x-pro2.ptMean.x;
	prodiff.ptMean.y = pro1.ptMean.y-pro2.ptMean.y;

//	prodiff.ptMean.x = prodiff.ptMean.y =0;

	if(pro2.fArea < 0.000001)
		return;

	if (fabs(pro2.right-pro2.left)<0.000001 && fabs(pro2.bottom-pro2.top)<0.000001) 
		return;

	float fscale = pro1.fArea/pro2.fArea;
	float fscalex = fabs(pro1.right-pro1.left)/fabs(pro2.right-pro2.left);
	float fscaley = fabs(pro1.bottom-pro1.top)/fabs(pro2.bottom-pro2.top);

	MinLen = -1;
	MinNum = 0;

	for(i = 0;i<nNum2;i++)  //求出离第一个点距离最短的点并标出来
	{
//		fMinTemp = fabs(pObj1[0].x-(pObj2[i].x + prodiff.ptMean.x)* fscalex)
//				+ fabs(pObj1[0].y-(pObj2[i].y + prodiff.ptMean.y)* fscaley);
		fMinTemp = fabs(pObj1[0].x-pro1.ptMean.x-(pObj2[i].x-pro2.ptMean.x)*fscalex)
				+fabs(pObj1[0].y - pro1.ptMean.x - (pObj2[i].y - pro2.ptMean.y)* fscaley);

		if(MinLen>fMinTemp)
		{
			MinLen=fMinTemp;
			MinNum=i;
		}
	}

	int nVerNum = pMesh->NbVertex();
	pMesh->SetMeshType(SIDE_FACE);

	// 提取网格中顶点和三角片指针
	CArray3d<CVertex3d> *pArrayVertex = pMesh->GetArrayVertex();
	CArray3d<CFace3d> *pArrayFace = pMesh->GetArrayFace();

	//　添加顶点
	for (i=0; i<nNum1; ++i)
	{
		pArrayVertex->Add(new CVertex3d(pObj1[i].x, pObj1[i].y, pObj1[i].z));
	}

	for (i=0; i<nNum2; ++i)
	{
		pArrayVertex->Add(new CVertex3d(pObj2[(i+MinNum)%nNum2].x,
										pObj2[(i+MinNum)%nNum2].y,
										pObj2[(i+MinNum)%nNum2].z));
	}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//  侧向边的绘制
	CVertex3d** pOne = new CVertex3d*[nNum1];
	CVertex3d** pTwo = new CVertex3d*[nNum2];
	CVertex3d** pV;

	pV = pArrayVertex->GetData();
	for(i = 0;i<nNum1;i++)
		pOne[i] = pV[nVerNum+i];     //即把前nNum1个附给第一个切片
	for(j = 0;j<nNum2;j++)   //把后nNum1到nNum1+nNum2附给第二个切片
        pTwo[j] = pV[nVerNum+j+nNum1]; 

	int nIdx1 = 0,nIdx2 = 0;
	CVertex3d* pStart1 = pOne[0];
	CVertex3d* pStart2 = pTwo[0];
	CVertex3d* p1 = pStart1;
	CVertex3d* p2 = pStart2;
	CVertex3d* p3 = pOne[++nIdx1];
	CVertex3d* p4 = pTwo[++nIdx2];
	float fCX, fCY, fCZ; //中点坐标
	float fLen1, fLen2;
    i = pMesh->NbFace();   //这儿i必须重置为0
	int nNextWeight = 0;
	float fWeight = 0.5;
	int nLocalFace = 0;
	while(1)
    {
		fCZ = (p1->z()+p2->z())/2.0;
		fCX = ((p1->x()-pro1.ptMean.x)+(p2->x()-pro2.ptMean.x)*fscalex)/2.0;
		fCY = ((p1->y()-pro1.ptMean.y)+(p2->y()-pro2.ptMean.y)*fscalex)/2.0;

		fLen1 = fabs(fCX-(p3->x()-pro1.ptMean.x))+fabs(fCY-(p3->y()-pro1.ptMean.y));
		fLen2 = fabs(fCX-(p4->x()-pro2.ptMean.x)*fscalex)+fabs(fCY-(p4->y()-pro2.ptMean.y)*fscaley);

		if (nNextWeight == 2)
			fLen1 *= fWeight;
		else if (nNextWeight == 1)
			fLen2 *= fWeight;

		CFace3d* pFace = new CFace3d;
		if(fLen1 < fLen2)
		{
			nNextWeight = 1;
			pFace->Set(p1, p3, p2);
			p1 = p3;
			p3 = pOne[(++nIdx1)%nNum1];
		}
		else
		{
			nNextWeight = 2;
			pFace->Set(p1, p4, p2);
			p2 = p4;
			p4 = pTwo[(++nIdx2)%nNum2];
		}

		pArrayFace->SetAtGrow(i++,pFace);
		++nLocalFace;

		if(DWORD(p1)==DWORD(pStart1) && DWORD(p2)==DWORD(pStart2))
			break;

		if(nLocalFace >= 3*(nNum1+nNum2))
			break;
	}

	delete [] pOne;
	delete [] pTwo;
}

void CTilSysDoc::ConstructFace(CSceneGraph3d& pSceneGraph, MESH_TYPE type,
							   PPOINT_3F pObj, int nNum, 
							   int nTextureIdx, int nCanvasIdx)
{
	//三角化表面，形成一个三维平面，这样可以进行纹理贴图
	CMesh3d* pMesh = new CMesh3d;//创建网格对象
	pMesh->SetMeshType(type);
	pSceneGraph.Add(pMesh);//将网格对象添加到场景

	pMesh->SetNbVertex(0);//设置网格中顶点个数
	pMesh->SetNbFace(0);//设置网格中三角片个数

	//设置锚点偏移,调整纹理坐标
// 	int nWidth = GetCanvasMgr()->GetCanvasByIdx(nCanvasIdx)->GetWidth();
// 	int nHeight = GetCanvasMgr()->GetCanvasByIdx(nCanvasIdx)->GetHeight();
//	POINT_3F pt = GetCanvasMgr()->GetCanvasByIdx(nCanvasIdx)->GetFaceCenter();
	CTilGraph* pGraphMgr = GetCanvasMgr()->GetCanvasByIdx(nCanvasIdx)->GetGraph();
	CUnit* pTunArc = pGraphMgr->GetGraphByIdx(GRAPH_LAYER_TUNARC)->GetUnitByIdx(0);
	CRect tunArcRect = pTunArc->GetPolyRect();
	double x, y, z;
	GetCanvasMgr()->GetCanvasByIdx(nCanvasIdx)->GetCenterImgCoord(x, y);
	z = 0.0;
	pMesh->SetLockOffset(x/tunArcRect.Width(), y/tunArcRect.Width());

	// 提取网格中顶点及三角片指针
	CArray3d<CVertex3d> *pArrayVertex = pMesh->GetArrayVertex();
	CArray3d<CFace3d> *pArrayFace = pMesh->GetArrayFace();

	// 设置顶点
	for (int i=0; i<nNum; ++i)
	{
		pArrayVertex->Add(new CVertex3d(pObj[i].x, pObj[i].y, pObj[i].z));
	}
	pMesh->SetTextureIndex(nTextureIdx);

	CTriFace faceTop(pArrayFace, pArrayVertex, 0);
	Vector3F* pVC = new Vector3F [nNum];

	// 通过面积正负判断点是顺时针还是逆时针排序（未知是否正确，有待验证）
	float fArea = 0.0;
	for (i=0; i<nNum-1; i++)
	{
		fArea += pObj[i].x*pObj[i+1].y-pObj[i].y*pObj[i+1].x;
	}
	fArea += pObj[i].x*pObj[0].y-pObj[i].y*pObj[0].x;

	if (fArea > 0)
	{
		for(i = 0; i < nNum; ++i)
		{
			pVC[i][0] = pObj[i].x;
			pVC[i][1] = pObj[i].y;
			pVC[i][2] = pObj[i].z;
		}
	}
	else
	{
		for(i = 0; i < nNum; ++i)
		{
			pVC[i][0] = pObj[nNum-1-i].x;
			pVC[i][1] = pObj[nNum-1-i].y;
			pVC[i][2] = pObj[nNum-1-i].z;
		}
	}

	Vector3F vcNorm(0,0,1);
	faceTop.Triangulate(pVC, vcNorm, nNum);
	pMesh->SubdivisionLoop(3);

	delete [] pVC;
}

// 创建岩体裂隙
void CTilSysDoc::ConstructCrack(CSceneGraph3d& pSceneGraph, CMesh3d* pMesh, MESH_TYPE type,
					PPOINT_3F pObj1, int nNum1, PPOINT_3F pObj2, int nNum2, 
					int nTextureIdx, int nCanvasIdx)
{
	float MinLen; 
	int MinNum;
	int i, j;

	PROPERTY pro1, pro2, /*proMean,*/ prodiff;
	CalProperty(pObj1, nNum1, pro1, 1, 1);
	CalProperty(pObj2, nNum2, pro2, 1, 1);

	if(pro2.fArea < 0.000001)
		return;

	if (fabs(pro2.right-pro2.left)<0.000001 && fabs(pro2.bottom-pro2.top)<0.000001) 
		return;

	float fscale = pro1.fArea/pro2.fArea;
	float fscalex = fabs(pro1.right-pro1.left)/fabs(pro2.right-pro2.left);
	float fscaley = fabs(pro1.bottom-pro1.top)/fabs(pro2.bottom-pro2.top);

	MinLen = -1;
	MinNum = 0;

	int nVerNum = pMesh->NbVertex();
	pMesh->SetMeshType(TOP_FACE);
	pMesh->SetTextureIndex(24);

	// 提取网格中顶点和三角片指针
	CArray3d<CVertex3d> *pArrayVertex = pMesh->GetArrayVertex();
	CArray3d<CFace3d> *pArrayFace = pMesh->GetArrayFace();

	//　添加顶点
	for (i=0; i<nNum1; ++i)
	{
		pArrayVertex->Add(new CVertex3d(pObj1[i].x, pObj1[i].y, pObj1[i].z));
	}

	for (i=0; i<nNum2; ++i)
	{
		pArrayVertex->Add(new CVertex3d(pObj2[i].x, pObj2[i].y, pObj2[i].z));
	}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//  侧向边的绘制
	CVertex3d** pOne = new CVertex3d*[nNum1];
	CVertex3d** pTwo = new CVertex3d*[nNum2];
	CVertex3d** pV;

	pV = pArrayVertex->GetData();
	for(i = 0;i<nNum1;i++)
		pOne[i] = pV[nVerNum+i];     //即把前nNum1个附给第一个切片
	for(j = 0;j<nNum2;j++)   //把后nNum1到nNum1+nNum2附给第二个切片
        pTwo[j] = pV[nVerNum+j+nNum1]; 

	int nIdx1 = 0,nIdx2 = 0;
	CVertex3d* pStart1 = pOne[nIdx1];
	CVertex3d* pStart2 = pTwo[nIdx2];
	CVertex3d* p1 = pStart1;
	CVertex3d* p2 = pStart2;
	CVertex3d* p3 = pOne[++nIdx1];
	CVertex3d* p4 = pTwo[++nIdx2];
	float fCX, fCY, fCZ; //中点坐标
	float fLen1, fLen2;
    i = pMesh->NbFace();   //这儿i必须重置为0
	int nNextWeight = 0;
	float fWeight = 0.5;
	int nLocalFace = 0;
	while(1)
    {
		fCZ = (p1->z()+p2->z())/2.0;
		fCX = ((p1->x()-pro1.ptMean.x)+(p2->x()-pro2.ptMean.x)*fscalex)/2.0;
		fCY = ((p1->y()-pro1.ptMean.y)+(p2->y()-pro2.ptMean.y)*fscaley)/2.0;

		fLen1 = fabs(fCX-(p3->x()-pro1.ptMean.x))+fabs(fCY-(p3->y()-pro1.ptMean.y));
		fLen2 = fabs(fCX-(p4->x()-pro2.ptMean.x)*fscalex)+fabs(fCY-(p4->y()-pro2.ptMean.y)*fscaley);

		if (nNextWeight == 2)
			fLen1 *= fWeight;
		else if (nNextWeight == 1)
			fLen2 *= fWeight;

		if (fLen1 < fLen2)
		{
			nNextWeight = 1;

			CFace3d* pFace = new CFace3d;
			pFace->Set(p1, p3, p2);
			pArrayFace->SetAtGrow(i++,pFace);
			++nLocalFace;

			p1 = p3;

			// 如果p3还没到最后一点,继续,否则直接把后面的点组成三角面片
			if (nIdx1++ < nNum1-1)
				p3 = pOne[nIdx1];
			else
			{
				for (;nIdx2++<nNum2; p2=p4,p4=pTwo[nIdx2])
				{
					CFace3d* pFace1 = new CFace3d;
					pFace1->Set(p1, p4, p2);
					pArrayFace->SetAtGrow(i++,pFace1);
					++nLocalFace;
				}
				break;
			}
		}
		else
		{
			nNextWeight = 2;

			CFace3d* pFace = new CFace3d;
			pFace->Set(p1, p4, p2);
			pArrayFace->SetAtGrow(i++,pFace);
			++nLocalFace;

			p2 = p4;

			// 如果p4还没到最后一点,继续,否则直接把后面的点组成三角面片
			if (nIdx2++<nNum2-1)
				p4 = pTwo[nIdx2];
			else
			{
				for (;nIdx1++<nNum1; p1=p3,p3=pOne[nIdx1])
				{
					CFace3d* pFace1 = new CFace3d;
					pFace1->Set(p1, p3, p2);
					pArrayFace->SetAtGrow(i++,pFace1);
					++nLocalFace;
				}
				break;
			}
		}
	}

	delete [] pOne;
	delete [] pTwo;

//	pMesh->SetTextureIndex(nTextureIdx);

// 	CTriFace faceTop(pArrayFace, pArrayVertex, 0);
// 	Vector3F* pVC = new Vector3F [nNum];
// 	
// 	// 通过面积正负判断点是顺时针还是逆时针排序（未知是否正确，有待验证）
// 	float fArea = 0.0;
// 	for (i=0; i<nNum-1; i++)
// 	{
// 		fArea += pObj[i].x*pObj[i+1].y-pObj[i].y*pObj[i+1].x;
// 	}
// 	fArea += pObj[i].x*pObj[0].y-pObj[i].y*pObj[0].x;
// 	
// 	if (fArea > 0)
// 	{
// 		for(i = 0; i < nNum; ++i)
// 		{
// 			pVC[i][0] = pObj[i].x;
// 			pVC[i][1] = pObj[i].y;
// 			pVC[i][2] = pObj[i].z;
// 		}
// 	}
// 	else
// 	{
// 		for(i = 0; i < nNum; ++i)
// 		{
// 			pVC[i][0] = pObj[nNum-1-i].x;
// 			pVC[i][1] = pObj[nNum-1-i].y;
// 			pVC[i][2] = pObj[nNum-1-i].z;
// 		}
// 	}
// 	
// 	Vector3F vcNorm(0,0,1);
// 	faceTop.Triangulate(pVC, vcNorm, nNum);
// 	delete [] pVC;
}

void CTilSysDoc::CreateTexture()
{
	CString str;
	for(int i = 0; i < GetCanvasMgr()->Size(); ++i)
	{
		CTilCanvas* pCanvas = GetCanvasMgr()->GetCanvasByIdx(i);
		CTilImage* pImage = pCanvas->GetImage();

		CTexture* pTE = new CTexture;
		str = pImage->GetPathName();
		pTE->ReadFile(LPTSTR(LPCTSTR(pImage->GetPathName())));
		m_SceneGraph.SetTextureIdxAt(i, pTE);
	}

	// 导入隧道边墙及拱顶的纹理图像
//	str = get_file_path(str)+"\\MeshArc.jpg";
//	CTexture* pTE = new CTexture;
//	pTE->ReadFile(LPTSTR(LPCTSTR(str)));
//	m_SceneGraph.SetTextureIdxAt(i++,pTE);
}

void CTilSysDoc::OnOpenglUpdateData() 
{
	// TODO: Add your command handler code here
	BeginWaitCursor();

	m_SceneGraph.FreeVertext();

	m_bTunSide = m_bTunFace = m_bTunStruct = TRUE;
	m_bSectionNormal = m_bSectionProfile = FALSE;
	m_Tunnel.ImportData(GetCanvasMgr());
	m_Tunnel.CreateTunFace(m_SceneGraph);
	m_Tunnel.CreateTunStruct(m_SceneGraph);
	m_Tunnel.CreateTunArc(m_SceneGraph);

//	CMeshTunnel meshTunnel;
//	meshTunnel.CreateStructures(m_SceneGraph);
//	meshTunnel.VirtualGeoDrill(m_SceneGraph,50);
////	meshTunnel.VirtualGeoDrill(m_SceneGraph,POINT2DF(750,450),50);
//	if (m_glShowInfo.bTunArc)
//		meshTunnel.CreateTunArc(m_SceneGraph);
//
//	// 计算横剖面
//	CTilGraph graphSection, graphProfile;
//	long lStartDK, lEndDK;
//	lStartDK = lEndDK = 0;
//	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
//	CGuiDataDraw& curve = pFrame->GetDataCurve();
//	CCanvasSection& section = curve.GetSection();
//	// 横剖面
//	meshTunnel.NormalSection(1518, graphSection);
//	section.SetNormalSection(1518, graphSection);
//
//	// 纵剖面
//	meshTunnel.Profile(lStartDK,lEndDK,graphProfile);
//	section.SetProfile(lStartDK,lEndDK,graphProfile);
//	section.Redraw();

// 	int NbObject = m_SceneGraph.NbObject();
// 	for(int i=0;i<NbObject;i++)
// 	{
// 		CObject3d *pObject3d = m_SceneGraph[i];
// 		if(pObject3d->GetType() == TYPE_MESH3D)
// 		{
// 			CMesh3d *pMesh  = (CMesh3d *)pObject3d;
// 			pMesh->SubdivisionLoop();
// 		}
// 	}

	UpdateAllViews(NULL);
	EndWaitCursor();
}

void CTilSysDoc::OnUpdateOpenglUpdateData(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetCanvasMgr()->Size() > 0);
}

void CTilSysDoc::OnGlVertex() 
{
	// TODO: Add your command handler code here
	m_glShowInfo.Mode = GL_POINT;
	UpdateAllViews(NULL);
}

void CTilSysDoc::OnGlLine() 
{
	// TODO: Add your command handler code here
	m_glShowInfo.Mode = GL_LINE;
	UpdateAllViews(NULL);
}

void CTilSysDoc::OnGlFace() 
{
	// TODO: Add your command handler code here
	m_glShowInfo.Mode = GL_FILL;
	UpdateAllViews(NULL);
}

void CTilSysDoc::OnGlSmooth() 
{
	// TODO: Add your command handler code here
	m_glShowInfo.Smooth = !m_glShowInfo.Smooth;
	if(m_glShowInfo.Smooth)
	{
		m_SceneGraph.SetNormalBinding(NORMAL_PER_VERTEX);
		::glShadeModel(GL_SMOOTH);
		m_SceneGraph.BuildAdjacency();
	}
	else
	{
		m_SceneGraph.SetNormalBinding(NORMAL_PER_FACE);
		::glShadeModel(GL_FLAT);
		m_SceneGraph.BuildAdjacency();
	}
	UpdateAllViews(NULL);
}

void CTilSysDoc::OnGlLight() 
{
	// TODO: Add your command handler code here
	m_glShowInfo.Light = !m_glShowInfo.Light;
	UpdateAllViews(NULL);
}

void CTilSysDoc::OnUpdateGlVertex(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_glShowInfo.Mode == GL_POINT);
}

void CTilSysDoc::OnUpdateGlLine(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_glShowInfo.Mode == GL_LINE);
}

void CTilSysDoc::OnUpdateGlFace(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_glShowInfo.Mode == GL_FILL);
}

void CTilSysDoc::OnUpdateGlSmooth(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_glShowInfo.Smooth);
}

void CTilSysDoc::OnUpdateGlLight(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_glShowInfo.Light);
}

void CTilSysDoc::OnMeshLoop() 
{
	// TODO: Add your command handler code here
	BeginWaitCursor();
	int NbObject = m_SceneGraph.NbObject();
	for(int i=0;i<NbObject;i++)
	{
		CObject3d *pObject3d = m_SceneGraph[i];
		if(pObject3d->GetType() == TYPE_MESH3D)
		{
			CMesh3d *pMesh  = (CMesh3d *)pObject3d;
			pMesh->SubdivisionLoop();
		}
	}
	UpdateAllViews(NULL);
	EndWaitCursor();
}

void CTilSysDoc::OnUpdateMeshLoop(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_SceneGraph.NbObject() > 0);
}

void CTilSysDoc::OnGlTunarc() 
{
	// TODO: Add your command handler code here
	m_glShowInfo.bTunArc = !m_glShowInfo.bTunArc;
}

void CTilSysDoc::OnUpdateGlTunarc(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_glShowInfo.bTunArc);
}

void CTilSysDoc::OnGlAddWireframe() 
{
	// TODO: Add your command handler code here
	m_glShowInfo.AddWireframe = !m_glShowInfo.AddWireframe;
	UpdateAllViews(NULL);
}

void CTilSysDoc::OnUpdateGlAddWireframe(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_glShowInfo.AddWireframe);
}

// 显示虚拟钻孔范围
void CTilSysDoc::OnDrillRange() 
{
	// TODO: Add your command handler code here
	/*
	CDlgMeshGeoVirThrill thrill;
	if (thrill.DoModal()==IDOK)
	{

	}
	*/
}

void CTilSysDoc::OnUpdateDrillRange(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

// 地质预测
#include "DlgMeshGeoPredict.h"
void CTilSysDoc::OnGeoPredict() 
{
	// TODO: Add your command handler code here
	CDlgMeshGeoPredict dlg;
	if (dlg.DoModal()==IDOK)
	{
	}
}

void CTilSysDoc::OnUpdateGeoPredict(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CTilSysDoc::OnTunSide() 
{
	m_bTunSide = !m_bTunSide;

	// TODO: Add your command handler code here
	m_SceneGraph.FreeVertext();

	m_Tunnel.ImportData(GetCanvasMgr());
	if (m_bTunFace)
		m_Tunnel.CreateTunFace(m_SceneGraph);
	if (m_bTunStruct)
		m_Tunnel.CreateTunStruct(m_SceneGraph);
	if (m_bTunSide)
		m_Tunnel.CreateTunArc(m_SceneGraph, 3);

	UpdateAllViews(NULL);
}

void CTilSysDoc::OnUpdateTunSide(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetCanvasMgr()->Size() > 0);
	pCmdUI->SetCheck(m_bTunSide);
}

void CTilSysDoc::OnTunStruct() 
{
	m_bTunStruct = !m_bTunStruct;

	// TODO: Add your command handler code here
	m_SceneGraph.FreeVertext();

	if (m_bTunFace)
		m_Tunnel.CreateTunFace(m_SceneGraph);
	if (m_bTunStruct)
		m_Tunnel.CreateTunStruct(m_SceneGraph);
	if (m_bTunSide)
		m_Tunnel.CreateTunArc(m_SceneGraph);

	UpdateAllViews(NULL);
}

void CTilSysDoc::OnUpdateTunStruct(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetCanvasMgr()->Size() > 0);
	pCmdUI->SetCheck(m_bTunStruct);
}

void CTilSysDoc::OnTunDrill() 
{
	// TODO: Add your command handler code here
	
}

void CTilSysDoc::OnUpdateTunDrill(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CTilSysDoc::OnTunFace() 
{
	m_bTunFace = !m_bTunFace;

	// TODO: Add your command handler code here
	m_SceneGraph.FreeVertext();

	m_Tunnel.ImportData(GetCanvasMgr());
	if (m_bTunFace)
		m_Tunnel.CreateTunFace(m_SceneGraph);
	if (m_bTunStruct)
		m_Tunnel.CreateTunStruct(m_SceneGraph);
	if (m_bTunSide)
		m_Tunnel.CreateTunArc(m_SceneGraph);

	UpdateAllViews(NULL);
}

void CTilSysDoc::OnUpdateTunFace(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetCanvasMgr()->Size() > 0);
	pCmdUI->SetCheck(m_bTunFace);
}

#include "DlgMeshSectionNormal.h"
// 显示纵剖面
void CTilSysDoc::OnSectionNormal() 
{
	CDlgMeshSectionNormal dlg;
	if (dlg.DoModal() != IDOK)
		return;

	m_bSectionProfile = FALSE;
	m_bSectionNormal = TRUE;

	// TODO: Add your command handler code here
	CMeshTunnel meshTunnel;
	meshTunnel.CreateStructures(m_SceneGraph);
//	meshTunnel.VirtualGeoDrill(m_SceneGraph,50);
//	meshTunnel.VirtualGeoDrill(m_SceneGraph,POINT2DF(750,450),50);
	if (m_glShowInfo.bTunArc)
		meshTunnel.CreateTunArc(m_SceneGraph);

	// 计算横剖面
	CTilGraph graphSection, graphProfile;
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
#if CURVE_WINDOW
	CGuiDataDraw& curve = pFrame->GetDataCurve();
	CCanvasSection& section = curve.GetSection();
	// 横剖面
	meshTunnel.NormalSection(dlg.m_nCrossSectionDK, graphSection);
	section.SetNormalSection(dlg.m_nCrossSectionDK, graphSection);

	section.Redraw(1);
#endif
}

void CTilSysDoc::OnUpdateSectionNormal(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetCanvasMgr()->Size() > 0);
	pCmdUI->SetCheck(m_bSectionNormal);
}

// 显示纵剖面
void CTilSysDoc::OnSectionProfile() 
{
	m_bSectionProfile = TRUE;
	m_bSectionNormal = FALSE;

	// TODO: Add your command handler code here
	CMeshTunnel meshTunnel;
	meshTunnel.CreateStructures(m_SceneGraph);
	meshTunnel.VirtualGeoDrill(m_SceneGraph,50);
//	meshTunnel.VirtualGeoDrill(m_SceneGraph,POINT2DF(750,450),50);
	if (m_glShowInfo.bTunArc)
		meshTunnel.CreateTunArc(m_SceneGraph);

	// 计算横剖面
	CTilGraph graphSection, graphProfile;
	long lStartDK, lEndDK;
	lStartDK = lEndDK = 0;
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
#if CURVE_WINDOW
	CGuiDataDraw& curve = pFrame->GetDataCurve();
	CCanvasSection& section = curve.GetSection();

	// 纵剖面
	meshTunnel.Profile(lStartDK,lEndDK,graphProfile);
	section.SetProfile(lStartDK,lEndDK,graphProfile);
	section.Redraw(2);
#endif
}

void CTilSysDoc::OnUpdateSectionProfile(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetCanvasMgr()->Size() > 0);
	pCmdUI->SetCheck(m_bSectionProfile);	
}


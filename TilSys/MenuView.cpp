#include "StdAfx.h"
#include "TilSys.h"
#include "TilSysDoc.h"
#include "TilCanvas.h"
#include <process.h>

void CTilSysDoc::OnViewImage() 
{
	// TODO: Add your command handler code here
	for (int i = 0; i < GetCanvasMgr()->Size(); i ++)
	{
		CTilImage* pImageMgr = GetCanvasMgr()->GetCanvasByIdx(i)->GetImage();
		pImageMgr->SetShowMode(!pImageMgr->GetShowMode());
	}
	UpdateAllViews(NULL);
}

void CTilSysDoc::OnViewGraph() 
{
	// TODO: Add your command handler code here
	for (int i = 0; i < GetCanvasMgr()->Size(); i ++)
	{
		CTilGraph* pGraphMgr = GetCanvasMgr()->GetCanvasByIdx(i)->GetGraph();
		pGraphMgr->SetShowMode(!pGraphMgr->GetShowMode());
	}
	UpdateAllViews(NULL);
}

void CTilSysDoc::OnViewZoomin() 
{
	m_lMenuCommand = ID_VIEW_ZOOMIN;
	m_hThread = (HANDLE)_beginthread(RunImageThread, 0, this);
}

void CTilSysDoc::OnViewZoomout() 
{
	m_lMenuCommand = ID_VIEW_ZOOMOUT;
	m_hThread = (HANDLE)_beginthread(RunImageThread, 0, this);
}

void CTilSysDoc::OnViewZoom() 
{
	// TODO: Add your command handler code here
	
}

void CTilSysDoc::OnViewZoomnormal() 
{
	m_lMenuCommand = ID_VIEW_ZOOMNORMAL;
	m_hThread = (HANDLE)_beginthread(RunImageThread, 0, this);
}

void CTilSysDoc::OnImageMirror() 
{
	m_lMenuCommand = ID_IMAGE_MIRROR;
	m_hThread = (HANDLE)_beginthread(RunImageThread, 0, this);
}

void CTilSysDoc::OnImageFlip() 
{
	m_lMenuCommand = ID_IMAGE_FLIP;
	m_hThread = (HANDLE)_beginthread(RunImageThread, 0, this);
}

void CTilSysDoc::OnImageNegative() 
{
	m_lMenuCommand = ID_IMAGE_NEGATIVE;
	m_hThread = (HANDLE)_beginthread(RunImageThread, 0, this);
}

void CTilSysDoc::OnImageRotatel() 
{
	m_lMenuCommand = ID_IMAGE_ROTATEL;
	m_hThread = (HANDLE)_beginthread(RunImageThread, 0, this);
}

void CTilSysDoc::OnImageRotater() 
{
	m_lMenuCommand = ID_IMAGE_ROTATER;
	m_hThread = (HANDLE)_beginthread(RunImageThread, 0, this);
}

void CTilSysDoc::OnImageRotate() 
{
//	CTilCanvas* pCanvas = GetCanvasMgr->GetCurCanvas();
//	pCanvas->GetImage()->Flip();
//	pCanvas->GetImage()->SetModifiedFlag();
//	UpdateAllViews(NULL);
}


#include "Graph/Text.h"
void CTilSysDoc::OnImageCrop() 
{
	// 对图像进行裁剪
	CTilCanvas* pCanvas = GetCanvasMgr()->GetCurCanvas();
	CUnit* pTunUnit = pCanvas->GetGraph()->GetGraphByIdx(GRAPH_LAYER_TUNARC)->GetUnitByIdx(0);
	CRect rtTunArc = pTunUnit->GetPolyRect();
	CRect rtExpand = rtTunArc;

	// 将矩形扩边10像素
	rtTunArc.left = (rtTunArc.left-TunArc_Extend<0)?0:rtTunArc.left-TunArc_Extend;
	rtTunArc.right = rtTunArc.right+TunArc_Extend;
	rtTunArc.top = (rtTunArc.top-TunArc_Extend<0)?0:rtTunArc.top-TunArc_Extend;
	rtTunArc.bottom = rtTunArc.bottom+TunArc_Extend;

// 	rtTunArc.bottom = 750;
	CImage* pImage = pCanvas->GetImage()->GetCurImage();
	pImage->Crop(rtTunArc);
	pCanvas->GetImage()->SetModifiedFlag();

	int x = rtTunArc.left;
	int y = rtTunArc.top;

	// 扩边
	rtExpand = pTunUnit->GetPolyRect();
	rtExpand.OffsetRect(-x, -y);
	int left,top,right,bottom;
	left = TunArc_Extend-rtExpand.left;
	top = TunArc_Extend-rtExpand.top;
	right = bottom = 0;

// 	if (rtExpand.right<rtExpand.bottom)
// 		rtExpand.right = rtExpand.bottom;
// 	else
// 		rtExpand.bottom = rtExpand.right;
	if (rtExpand.right+TunArc_Extend>pImage->GetWidth())
		right = rtExpand.right+10-pImage->GetWidth();
	if (rtExpand.bottom+TunArc_Extend>pImage->GetHeight())
		bottom = rtExpand.bottom+TunArc_Extend-pImage->GetHeight();

	RGBQUAD rgb;
	rgb.rgbBlue = rgb.rgbGreen = rgb.rgbRed = 255;
	pImage->Expand(left, top, right, bottom, rgb);

	// 重新调整图形坐标
	x -= left;
	y -= top;
	CTilGraph* pGraphMgr = pCanvas->GetGraph();
	for (int i=0; i<pGraphMgr->Size(); i++)
	{
		CGraph* pGraph = pGraphMgr->GetGraphByIdx(i);
		for (int j=0; j<pGraph->Size(); j++)
		{
			CUnit* pUnit = pGraph->GetUnitByIdx(j);
			if (!pUnit->IsKindOf(RUNTIME_CLASS(CText)))
			{
				CArray<POINT2DF,POINT2DF>* pPointList = pUnit->m_pPointList;
				for (int k=0; k<pPointList->GetSize(); k++)
				{
					POINT2DF pt = pPointList->GetAt(k);
					pt.x -= x;
					pt.y -= y;
					pPointList->SetAt(k, pt);
				}
			}
			else
			{
				((CText*)pUnit)->m_PositionPoint.x -= x;
				((CText*)pUnit)->m_PositionPoint.y -= y;
			}
		}
	}
	pCanvas->GetGraph()->SetModifiedFlag();

	// 重新调整画布中心点坐标
//	POINT2DF ptCenter = pCanvas->GetCenterImgCoord();
//	ptCenter.x -= x;
//	ptCenter.y -= y;
//	pCanvas->SetCenterImgCoord(ptCenter);
//	pCanvas->SetModifiedFlag();

	//
	CTilTunArc* pTunArc = pCanvas->GetGraph()->GetTunArc();
	POINT2DF ptCenter = pTunArc->GetImgCenter();
	ptCenter.x -= x;
	ptCenter.y -= y;
	pTunArc->SetImgCenter(ptCenter);

	UpdateAllViews(NULL);
}

void CTilSysDoc::OnUpdateViewImage(CCmdUI* pCmdUI) 
{
	CTilCanvas* pCanvas = GetCanvasMgr()->GetCurCanvas();
	if (!pCanvas || m_hThread)
		pCmdUI->Enable(FALSE);
	else
	{
		CTilImage* pImageMgr = pCanvas->GetImage();
		pCmdUI->SetCheck(pImageMgr->GetShowMode());
	}
}

void CTilSysDoc::OnUpdateViewGraph(CCmdUI* pCmdUI) 
{
	CTilCanvas* pCanvas = GetCanvasMgr()->GetCurCanvas();
	if (!pCanvas || m_hThread)
		pCmdUI->Enable(FALSE);
	else
	{
		CTilGraph* pGraphMgr = pCanvas->GetGraph();
		pCmdUI->SetCheck(pGraphMgr->GetShowMode());
	}
}

void CTilSysDoc::OnUpdateViewZoomin(CCmdUI* pCmdUI) 
{
	long double dbZoomX, dbZoomY;
	CTilCanvasMgr* pMgr = GetCanvasMgr();
	pMgr->GetZoomFactor(dbZoomX, dbZoomY);
	pCmdUI->Enable((pMgr->GetCurCanvas() != NULL) && (m_hThread == NULL)
					&& (dbZoomX <= Max_Zoom_Factor)
					&& (dbZoomY <= Max_Zoom_Factor));
}

void CTilSysDoc::OnUpdateViewZoomout(CCmdUI* pCmdUI) 
{
	long double dbZoomX, dbZoomY;
	CTilCanvasMgr* pMgr = GetCanvasMgr();
	pMgr->GetZoomFactor(dbZoomX, dbZoomY);
	pCmdUI->Enable((pMgr->GetCurCanvas() != NULL) && (m_hThread == NULL)
					&& (dbZoomX >= Min_Zoom_Factor)
					&& (dbZoomY >= Min_Zoom_Factor));
}

void CTilSysDoc::OnUpdateViewZoom(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetCanvasMgr()->GetCurCanvas() != NULL);
}

void CTilSysDoc::OnUpdateViewZoomnormal(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	long double dbZoomX, dbZoomY;
	CTilCanvasMgr* pMgr = GetCanvasMgr();
	pMgr->GetZoomFactor(dbZoomX, dbZoomY);
	pCmdUI->Enable((pMgr->GetCurCanvas() != NULL) && (m_hThread == NULL)
				&& (dbZoomX != 1.0) && (dbZoomY != 1.0));
}

void CTilSysDoc::OnUpdateImageMirror(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetCanvasMgr()->GetCurCanvas() != NULL && m_hThread == NULL);
}

void CTilSysDoc::OnUpdateImageFlip(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetCanvasMgr()->GetCurCanvas() != NULL && m_hThread == NULL);
}

void CTilSysDoc::OnUpdateImageNegative(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetCanvasMgr()->GetCurCanvas() != NULL && m_hThread == NULL);
}

void CTilSysDoc::OnUpdateImageRotatel(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetCanvasMgr()->GetCurCanvas() != NULL && m_hThread == NULL);
}

void CTilSysDoc::OnUpdateImageRotater(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetCanvasMgr()->GetCurCanvas() != NULL && m_hThread == NULL);
}

void CTilSysDoc::OnUpdateImageRotate(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetCanvasMgr()->GetCurCanvas() != NULL && m_hThread == NULL);
}

void CTilSysDoc::OnUpdateImageCrop(CCmdUI* pCmdUI) 
{
	CTilCanvasMgr* pMgr = GetCanvasMgr();
	CTilCanvas* pCanvas = pMgr->GetCurCanvas();
	if (pCanvas != NULL  && m_hThread == NULL)
	{
		CGraph* pTunGraph = pCanvas->GetGraph()->GetGraphByIdx(GRAPH_LAYER_TUNARC);
		int nSize = pTunGraph->Size();
		pCmdUI->Enable(nSize > 0);
	}
	else
		pCmdUI->Enable(FALSE);
}

void CTilSysDoc::OnUpdateImageNormalize(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetCanvasMgr()->GetCurCanvas() != NULL && m_hThread == NULL);
}


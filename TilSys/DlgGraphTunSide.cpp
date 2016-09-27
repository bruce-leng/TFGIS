// DlgGraphTunSide.cpp : implementation file
//

#include "stdafx.h"
#include "tilsys.h"
#include "DlgGraphTunSide.h"
#include "Graph\Polygon.h"
#include "Graph\Line.h"
#include "MathComput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgGraphTunSide dialog

DlgGraphTunSide::DlgGraphTunSide(CWnd* pParent /*=NULL*/)
	: CDialog(DlgGraphTunSide::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgGraphTunSide)
	m_fRealWidth = 0.0;
	m_fShowWidth = 0.0;
	//}}AFX_DATA_INIT
	m_fExpand = 0.0;
	m_pCanvas = NULL;
}

DlgGraphTunSide::~DlgGraphTunSide()
{
	if (m_pCanvas)	delete m_pCanvas;
}

void DlgGraphTunSide::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgGraphTunSide)
	DDX_Control(pDX, IDC_SCR_MOVE_V2, m_ctlMoveV2);
	DDX_Control(pDX, IDC_SCR_MOVE_V, m_ctlMoveV);
	DDX_Control(pDX, IDC_SCR_MOVE_H, m_ctlMoveH);
	DDX_Control(pDX, IDC_TUN_SIDE, m_stcTunSide);
	DDX_Control(pDX, IDC_SPIN_SHOW_WIDTH, m_ctlShowWidth);
	DDX_Text(pDX, IDC_EDT_REAL_WIDTH, m_fRealWidth);
	DDV_MinMaxFloat(pDX, m_fRealWidth, 0.f, 10.f);
	DDX_Text(pDX, IDC_EDT_SHOW_WIDTH, m_fShowWidth);
	DDV_MinMaxFloat(pDX, m_fShowWidth, 0.f, 1000.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgGraphTunSide, CDialog)
	//{{AFX_MSG_MAP(DlgGraphTunSide)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDC_GEO_EXPAND, OnGeoExpand)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgGraphTunSide message handlers

BOOL DlgGraphTunSide::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CTilCanvasMgr* pMgr = GetCanvasMgr();
	CTilCanvas* pCanvas = pMgr->GetCurCanvas();
	m_pCanvas = new CTilCanvas(*pCanvas);

	// 重新设置隧道断面层的显示
	CTilGraph* pGraph = m_pCanvas->GetGraph();
	CTilTunArc* pTunArc = pGraph->GetTunArc();
	CUnit* pUnit = pGraph->GetGraphByIdx(GRAPH_LAYER_TUNARC)->GetUnitByIdx(0);
	pTunArc->Transferto(pUnit, 125, 500, 0.125);

	// 将图像裁剪成比隧道断面图形宽10像素
	CRect rect = pUnit->GetPolyRect();
	rect.InflateRect(10,10,10,10);
	CImage* pImage = m_pCanvas->GetImage()->GetCurImage();
	pImage->Crop(rect);

	// 获得图像缩放比率
	int nWidth, nHeight;
	nWidth = m_pCanvas->GetWidth();
	nHeight = m_pCanvas->GetHeight();

	m_stcTunSide.GetClientRect(&rect);

	float fRatioW, fRatioH;
	fRatioW = rect.Width()*1.0/nWidth;
	fRatioH = rect.Height()*1.0/nHeight;

	if (fRatioH<fRatioW)
		m_fRatio = fRatioH;
	else
		m_fRatio = fRatioW;

	// 加入1条水平直线用于裁剪拱型
	CUnit* pLine = new CLine;
	POINT2DF pt1, pt2;
	pt1.x = 0;
	pt1.y = 0.75*nHeight;
	pt2.x = nWidth-1;
	pt2.y = 0.75*nHeight;
	pLine->m_FirstPoint = pt1;
	pLine->m_SecondPoint = pt2;
	m_pCanvas->GetGraph()->GetGraphByIdx(GRAPH_LAYER_TUNARC)->PushBackUnit(pLine);
	//////////////////////////////////////////////////////////////////////////
	// 
	m_ctlShowWidth.SetBuddy(GetDlgItem(IDC_EDT_SHOW_WIDTH));
	m_ctlShowWidth.SetRange(0, 10000);
	m_ctlShowWidth.SetPos(0);

	m_ctlMoveH.SetScrollRange(0, 10000);
	m_ctlMoveH.SetScrollPos(0);

	m_ctlMoveV.SetScrollRange(0, 10000);
	m_ctlMoveV.SetScrollPos(0);

	m_ctlMoveV2.SetScrollRange(0, 10000);
	m_ctlMoveV2.SetScrollPos(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DlgGraphTunSide::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// 获得显示图像的外接矩形
	CDC* pDC = m_stcTunSide.GetDC();
	m_pCanvas->SetZoomFactor(m_fRatio, m_fRatio);

	int nWidth, nHeight;
	nWidth = m_pCanvas->GetWidth()*m_fRatio;
	nHeight = m_pCanvas->GetHeight()*m_fRatio;
	CRect rect(0,0,nWidth,nHeight);
	m_pCanvas->Draw(pDC,rect,rect);
}

void DlgGraphTunSide::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);

	if (pScrollBar->GetDlgCtrlID() == IDC_SCR_MOVE_H)
	{
		int nMinPos, nMaxPos;
		pScrollBar->GetScrollRange(&nMinPos, &nMaxPos);

		switch (nSBCode)
		{
			case SB_LEFT:
			case SB_LINELEFT:
				if (nPos > nMinPos)
				{
					pScrollBar->SetScrollPos(--nPos);
				}
				MoveTunArc(-5, 0);
				break;
			case SB_RIGHT:
			case SB_LINERIGHT:
				if (nPos < nMaxPos)
				{
					pScrollBar->SetScrollPos(++nPos);
				}
				MoveTunArc(5, 0);
				break;
		}
	}

	Invalidate(FALSE);
}

void DlgGraphTunSide::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if (nSBCode == SB_ENDSCROLL)
		return;	// Reject spurious messages

	if (pScrollBar->GetDlgCtrlID() == IDC_SPIN_SHOW_WIDTH)
	{
		CString strValue;
		strValue.Format("%5.1f", (double)nPos/10.0);
		((CSpinButtonCtrl*)pScrollBar)->GetBuddy()->SetWindowText(strValue);

		m_fExpand = nPos/10.0;
		ExpandTunArc(m_fExpand);
	}
	else if (pScrollBar->GetDlgCtrlID() == IDC_SCR_MOVE_V)
	{
		int nMinPos, nMaxPos;
		pScrollBar->GetScrollRange(&nMinPos, &nMaxPos);

		switch (nSBCode)
		{
			case SB_TOP:
			case SB_LINEUP:
				if (nPos > nMinPos)
				{
					pScrollBar->SetScrollPos(--nPos);
				}
				MoveTunArc(0, -5);
				break;
			case SB_BOTTOM:
			case SB_LINEDOWN:
				if (nPos < nMaxPos)
				{
					pScrollBar->SetScrollPos(++nPos);
				}
				MoveTunArc(0, 5);
				break;
		}
	}
	else if (pScrollBar->GetDlgCtrlID() == IDC_SCR_MOVE_V2)
	{
		int nMinPos, nMaxPos;
		pScrollBar->GetScrollRange(&nMinPos, &nMaxPos);

		switch (nSBCode)
		{
			case SB_TOP:
			case SB_LINEUP:
				if (nPos > nMinPos)
				{
					pScrollBar->SetScrollPos(--nPos);
				}
				MoveHLine(-5);
				break;
			case SB_BOTTOM:
			case SB_LINEDOWN:
				if (nPos < nMaxPos)
				{
					pScrollBar->SetScrollPos(++nPos);
				}
				MoveHLine(5);
				break;
		}
	}

	Invalidate(FALSE);

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void DlgGraphTunSide::OnOk() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if (m_fShowWidth!=0 && m_fRealWidth==0)
	{
		AfxMessageBox("请输入完整参数!");
		return;
	}

	CDialog::OnOK();
}

// 将隧道断面边界扩展
//void DlgGraphTunSide::ExpandTunArc(float fDist)
//{
//	CGraph* pGraph = m_pCanvas->GetGraph()->GetGraphByIdx(GRAPH_LAYER_TUNARC);
//	CUnit* pTunArcUnit = pGraph->GetUnitByIdx(0);
//	CArray<POINT2DF,POINT2DF>* pPoints = pTunArcUnit->m_pPointList;
//	int nSize = pPoints->GetSize();
//	CRect rect = pTunArcUnit->GetPolyRect();
//	int i;
//
//	// 若隧道断面层不只一个断面，说明已经加入了扩展断面，先将其删除
//	if (pGraph->Size() > 2)
//	{
//		for (i=pGraph->Size()-1; i>=2; i--)
//			pGraph->DeleteUnit(i);
//	}
//
//	// 求拱顶弧的圆心坐标
//	POINT2DF ptCenter, ptCenterEx, pt;
//	ptCenter.x = rect.CenterPoint().x;	// 重心在中线上，故取外接矩形的中心点横坐标为重心坐标
//
//	// 求拱顶圆弧的半径
//	float fRadius;
//	POINT2DF pt1, pt2;
//	CTilTunArc* pTunArc = m_pCanvas->GetGraph()->GetTunArc();
//	vector<TUNARC_UNIT>* pTunArcUnits = pTunArc->GetTunArcUnits();
//	int nSize1 = pTunArcUnits->size();
//	for (i=0; i<nSize1; i++)
//	{
//		pt1 = (*pTunArcUnits)[i].pt1;
//		pt2 = (*pTunArcUnits)[i].pt2;
//		// 满足列条件的图元为拱顶圆弧，则可直接求出半径
//		// (注意：只有在中心点在路面中线上时才满足pt1.x+pt2.x==0
//		if (fabs(pt1.x+pt2.x)+fabs(pt1.y-pt2.y)<0.001)
//		{
//			fRadius = get_distance(pt1, (*pTunArcUnits)[i].ptCenter);
//			ptCenter.y = rect.top+fRadius;
//		}
//	}
//
//	ptCenter.y = rect.top+fRadius;
//
//	// 求扩展的倍数
//	float fRatio;
//	fRatio = (fDist+fRadius)/fRadius;
//
//	// 求扩展后的边界
//	CUnit* pTunArcUnitEx = new CPolygon;
//	for (i=0; i<nSize; i++)
//	{
//		pt = pPoints->GetAt(i);
//		pt.x *= fRatio;
//		pt.y *= fRatio;
//		pTunArcUnitEx->m_pPointList->Add(pt);
//	}
//
//	// 求扩展后的重心坐标
//	CRect rectEx = pTunArcUnitEx->GetPolyRect();
//	ptCenterEx.y = rectEx.top+fRadius*fRatio;
//	ptCenterEx.x = rectEx.CenterPoint().x;
//
//	// 重新计算扩展后与原隧道断面重心重合的边界
//	for (i=0; i<nSize; i++)
//	{
//		pt = pTunArcUnitEx->m_pPointList->GetAt(i);
//		pt.x = ptCenter.x-(ptCenterEx.x-pt.x);
//		pt.y = ptCenter.y-(ptCenterEx.y-pt.y);
//		pTunArcUnitEx->m_pPointList->SetAt(i, pt);
//	}
//
//	// 将扩展后的边界加入拱形层中，以便显示
//	pGraph->PushBackUnit(pTunArcUnitEx);
//}
//
// 将隧道断面边界扩展
// 将底面的扩展去掉
void DlgGraphTunSide::ExpandTunArc(const float& fDist)
{
	int i;
	CTilTunArc* pTunSrc = m_pCanvas->GetGraph()->GetTunArc();
	vector<PTUNARC_UNIT>* pSrcUnits = pTunSrc->GetTunArcUnits();

	CGraph* pGraph = m_pCanvas->GetGraph()->GetGraphByIdx(GRAPH_LAYER_TUNARC);
	// 若隧道断面层不只一个断面，说明已经加入了扩展断面，先将其删除
	if (pGraph->Size() > 2)
	{
		for (i=pGraph->Size()-1; i>=2; i--)
		{
			delete pGraph->GetUnitByIdx(i);
			pGraph->DeleteUnit(i);
		}
	}

	int nSize, nType;
	POINT2DF pt1, pt2, ptCenter, ptEx1, ptEx2;

	CTilTunArc tunDst;
	tunDst = *pTunSrc;
	vector<PTUNARC_UNIT>* pDstUnits = tunDst.GetTunArcUnits();

	nSize = pSrcUnits->size();
	// 将底面的扩展去掉
	for (i=0; i<nSize; i++)
	{
		pt1 = (*pSrcUnits)[i]->pt1;
		pt2 = (*pSrcUnits)[i]->pt2;
		nType = (*pSrcUnits)[i]->nType;
		if (nType == Graph_Line)
			OffLine(pt1, pt2, fDist, ptEx1, ptEx2);
		else if (nType == Graph_RoundArc)
		{
			ptCenter = (*pSrcUnits)[i]->ptCenter;
			OffArc(pt1, pt2, ptCenter, fDist, ptEx1, ptEx2);
		}

		(*pDstUnits)[i]->pt1 = ptEx1;
		(*pDstUnits)[i]->pt2 = ptEx2;
	}

	// 将隧道断面尺寸转化成多边形图形
	CUnit* pTunArc = new CPolygon;
	tunDst.Transferto(pTunArc, 125, 500, 0.125);

	// 将多边形图形压入堆栈
	pGraph->PushBackUnit(pTunArc);
}

// 平移隧道边界
void DlgGraphTunSide::MoveTunArc(int nX, int nY)
{
	// 重置平移后的中心点坐标
	CGraph* pGraph = m_pCanvas->GetGraph()->GetGraphByIdx(GRAPH_LAYER_TUNARC);
	CTilTunArc* pTunArc = m_pCanvas->GetGraph()->GetTunArc();
	POINT2DF ptImgCenter = pTunArc->GetImgCenter();
	ptImgCenter.x += nX;
	ptImgCenter.y += nY;
	pTunArc->SetImgCenter(ptImgCenter);

	// 平移隧道图形层
	for (int i=0; i<pGraph->Size(); i++)
	{
		CUnit* pTunArcUnit = pGraph->GetUnitByIdx(i);
		int nSize = pTunArcUnit->GetPointNum();
		for (int j=0; j<nSize; j++)
		{
			POINT2DF pt = pTunArcUnit->m_pPointList->GetAt(j);
			pt.x += nX;
			pt.y += nY;
			pTunArcUnit->m_pPointList->SetAt(j, pt);
		}
	}
}

// 竖向移动水平裁剪线
void DlgGraphTunSide::MoveHLine(int nY)
{
	CGraph* pGraph = m_pCanvas->GetGraph()->GetGraphByIdx(GRAPH_LAYER_TUNARC);
	CUnit* pHLine = pGraph->GetUnitByIdx(1);	// 图元1为水平直线
	POINT2DF pt1, pt2;

	pt1 = pHLine->m_FirstPoint;
	pt2 = pHLine->m_SecondPoint;

	pt1.y += nY;
	pt2.y += nY;

	pHLine->m_FirstPoint = pt1;
	pHLine->m_SecondPoint = pt2;
}

// 求得水平裁剪线与断面的交点
void DlgGraphTunSide::GetInterPoints(POINT2DF& pt1, POINT2DF& pt2)
{
	// 求得
	int i;
	int nSize;
	POINT2DF ptA1, ptA2, ptCenter, ptB1, ptB2;
	int nType;
	BOOL bMark1 = FALSE;

	CTilGraph* pGraph = m_pCanvas->GetGraph();
	CTilTunArc* pTunArc = pGraph->GetTunArc();
	vector<PTUNARC_UNIT>* pTunArcUnits = pTunArc->GetTunArcUnits();
	CUnit* pUnit = pGraph->GetGraphByIdx(GRAPH_LAYER_TUNARC)->GetUnitByIdx(1);
	ptB1 = pUnit->m_FirstPoint;
	ptB2 = pUnit->m_SecondPoint;
	nSize = pTunArcUnits->size();
	for (i=0; i<nSize; i++)
	{
		ptA1 = (*pTunArcUnits)[i]->pt1;
		ptA2 = (*pTunArcUnits)[i]->pt2;
		nType = (*pTunArcUnits)[i]->nType;
		if (nType == Graph_Line)
		{
			if (!bMark1 && get_inter_point(ptA1, ptA2, ptB1, ptB2, pt1))
				bMark1 = TRUE;
			if (bMark1 && get_inter_point(ptA1, ptA2, ptB1, ptB2, pt2))
				return;
		}
		else if (nType == Graph_RoundArc)
		{
			ptCenter = (*pTunArcUnits)[i]->ptCenter;
			if (!bMark1)
			{
				int nRet = get_inter_point(ptB1, ptB2, ptA1, ptCenter, ptA2, pt1, pt2);
				if (nRet == 1)
					bMark1 = TRUE;
				else if (nRet == 2)
					return;
			}
			else
			{
				POINT2DF ptTmp;
				int nRet = get_inter_point(ptB1, ptB2, ptA1, ptCenter, ptA2, pt2, ptTmp);
				if (nRet == 1)
					return;
			}
		}
	}
}

/* ==================================================================
 * 函数名称：  DlgGraphTunSide::OffLine()
 * 作者：      [Leng]
 * 创建于:     [2009-3-31 12:13:45]
 * 参数列表： 
               [const POINT2DF& ptA1] - [In] 原线段端点1
               [const POINT2DF& ptA2] - [In] 原线段端点2
               [BOOL bClockWise] - [In] 要求ptA1,ptA2与平移后的线段上某点形成顺时针或反时针旋转
               [const float& fDist] - [In] 平移距离
               [POINT2DF& ptB1] - [In] 平移后线段端点1
               [POINT2DF& ptB2] - [In] 平移后线段端点2
注释 :	根据bClockWise参数可区别所求线段在原线段的哪一侧
		ptA1与ptA2为断面上逆时针方向上的前后相邻的两点，
		求线段平面到断面外部的线段
==================================================================*/
void DlgGraphTunSide::OffLine(const POINT2DF& ptA1, const POINT2DF& ptA2,
			const float& fDist, POINT2DF& ptB1, POINT2DF& ptB2)
{
	POINT2DF pt1, pt2;
	double k,b,A,B,C,fSum;
	if (fabs(ptA1.x-ptA2.x)>0.001)
	{
		k = (ptA1.y-ptA2.y)/(ptA1.x-ptA2.x);
		if (fabs(k)>0.001)
		{
			k = -1/k;
			b = ptA1.y-k*ptA1.x;

			// 计算第1点
			A = 1+1/(k*k);
			B = 2*(1/k*(ptA1.y-b)-ptA1.x);
			C = ptA1.x*ptA1.x+(ptA1.y-b)*(ptA1.y-b)-fDist*fDist;

			pt1.x = (-B+sqrt(B*B-4*A*C))/(2*A);
			pt1.y = -1/k*pt1.x+b;

			pt2.x = (-B-sqrt(B*B-4*A*C))/(2*A);
			pt2.y = -1/k*pt2.x+b;

			fSum = ptA1.x*ptA2.y-ptA1.y*ptA2.x;
			fSum += ptA2.x*pt1.y-ptA2.y*pt1.x;
			fSum += pt1.x*ptA1.y-pt1.y*ptA1.x;
			if (fSum>0)
				ptB1 = pt1;
			else
				ptB1 = pt2;

			// 计算第2点
			k = -1/k;
			b = ptA2.y-k*ptA2.x;

			A = 1+1/(k*k);
			B = 2*(1/k*(ptA2.y-b)-ptA2.x);
			C = ptA2.x*ptA2.x+(ptA2.y-b)*(ptA2.y-b)-fDist*fDist;

			pt1.x = (-B+sqrt(B*B-4*A*C))/(2*A);
			pt1.y = -1/k*pt1.x+b;

			pt2.x = (-B-sqrt(B*B-4*A*C))/(2*A);
			pt2.y = -1/k*pt2.x+b;

			fSum = ptA1.x*ptA2.y-ptA1.y*ptA2.x;
			fSum += ptA2.x*pt1.y-ptA2.y*pt1.x;
			fSum += pt1.x*ptA1.y-pt1.y*ptA1.x;
			if (fSum>0)
				ptB2 = pt1;
			else
				ptB2 = pt2;
		}
		else	// if (fabs(ptA1.x-ptA2.x)<=0.001)
		{
			// 求第1点
			pt1.x = ptA1.x;
			pt1.y = ptA1.y-fDist;

			pt2.x = ptA1.x;
			pt2.y = ptA1.y+fDist;

			fSum = ptA1.x*ptA2.y-ptA1.y*ptA2.x;
			fSum += ptA2.x*pt1.y-ptA2.y*pt1.x;
			fSum += pt1.x*ptA1.y-pt1.y*ptA1.x;
			if (fSum>0)
				ptB1 = pt1;
			else
				ptB1 = pt2;

			// 求第2点
			pt1.x = ptA2.x;
			pt1.y = ptA2.y-fDist;

			pt2.x = ptA2.x;
			pt2.y = ptA2.y+fDist;

			fSum = ptA1.x*ptA2.y-ptA1.y*ptA2.x;
			fSum += ptA2.x*pt1.y-ptA2.y*pt1.x;
			fSum += pt1.x*ptA1.y-pt1.y*ptA1.x;
			if (fSum>0)
				ptB2 = pt1;
			else
				ptB2 = pt2;
		}
	}
	else
	{
		// 求第1点
		pt1.x = ptA1.x-fDist;
		pt1.y = ptA1.y;

		pt2.x = ptA1.x+fDist;
		pt2.y = ptA1.y;

		fSum = ptA1.x*ptA2.y-ptA1.y*ptA2.x;
		fSum += ptA2.x*pt1.y-ptA2.y*pt1.x;
		fSum += pt1.x*ptA1.y-pt1.y*ptA1.x;
		if (fSum>0)
			ptB1 = pt1;
		else
			ptB1 = pt2;

		// 求第2点
		pt1.x = ptA2.x-fDist;
		pt1.y = ptA2.y;

		pt2.x = ptA2.x+fDist;
		pt2.y = ptA2.y;

		fSum = ptA1.x*ptA2.y-ptA1.y*ptA2.x;
		fSum += ptA2.x*pt1.y-ptA2.y*pt1.x;
		fSum += pt1.x*ptA1.y-pt1.y*ptA1.x;
		if (fSum>0)
			ptB2 = pt1;
		else
			ptB2 = pt2;
	}
}

/* ==================================================================
 * 函数名称：  DlgGraphTunSide::OffArc()
 * 作者：      [Leng]
 * 创建于:     [2009-3-31 12:13:53]
 * 参数列表： 
               [const POINT2DF& ptA1] - [In]
               [const POINT2DF& ptA2] - [In]
               [const POINT2DF& ptCenter] - [In]
               [const float& fDist] - [In]
               [POINT2DF& ptB1] - [In]
               [POINT2DF& ptB2] - [In]
注释 :	
				
==================================================================*/
void DlgGraphTunSide::OffArc(const POINT2DF& ptA1, const POINT2DF& ptA2, const POINT2DF& ptCenter,
			const float& fDist,	POINT2DF& ptB1, POINT2DF& ptB2)
{
	double k,b,A,B,C;
	POINT2DF pt1, pt2, ptA;

	for (int i=0; i<2; i++)
	{
		if (i==0)
			ptA = ptA1;
		else
			ptA = ptA2;

		if (fabs(ptA.x-ptCenter.x)>0.001)
		{
			k = (ptA.y-ptCenter.y)/(ptA.x-ptCenter.x);
			b = ptCenter.y-k*ptCenter.x;

			A = 1+k*k;
			B = 2*(k*(b-ptA.y)-ptA.x);
			C = ptA.x*ptA.x+(b-ptA.y)*(b-ptA.y)-fDist*fDist;

			pt1.x = (-B+sqrt(B*B-4*A*C))/(2*A);
			pt1.y = k*pt1.x+b;

			pt2.x = (-B-sqrt(B*B-4*A*C))/(2*A);
			pt2.y = k*pt2.x+b;
		}
		else
		{
			pt1.x = ptA.x;
			pt1.y = ptA.y-fDist;

			pt2.x = ptA.x;
			pt2.y = ptA.y+fDist;
		}

		if (i == 0)
		{
			if (point_in_line(pt1, ptA, ptCenter) == POINT_IN_EX_LINE)
				ptB1 = pt1;
			else
				ptB1 = pt2;
		}
		else
		{
			if (point_in_line(pt1, ptA, ptCenter) == POINT_IN_EX_LINE)
				ptB2 = pt1;
			else
				ptB2 = pt2;
		}
	}
}

// 生成地质展开图
void DlgGraphTunSide::OnGeoExpand() 
{
//	// TODO: Add your control notification handler code here
//	if (m_fRealWidth<=0 || m_fShowWidth<=0)
//		return;
//
//	CTilTunArc* pTunArc = m_pCanvas->GetGraph()->GetTunArc();
//	vector<TUNARC_UNIT>* pUnits = pTunArc->GetTunArcUnits();
//	POINT2DF ptCenter = pTunArc->GetImgCenter();
//	int nNum = pUnits->size();
//
//	// 最后一条边为隧道底边，不预考虑
//	int i,j,k;
//	for (i=0; i<nNum-1; i++)
//	{
//		// 若为直线
//		TUNARC_UNIT unit = pUnits[i];
//		if (unit.nType == Graph_Line)
//		{
//			if (unit.pt1.x < ptCenter.x)
//			{
//				for (j=unit.pt1.x; j>= unit.pt2.x; j--)
//				{
//					for (k=j)
//				}
//			}
//
//		}
//	}
}

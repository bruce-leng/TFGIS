// DlgImageGeometryCorrect.cpp : implementation file
//

#include "stdafx.h"
#include "tilsys.h"
#include "DlgImageGeometryCorrect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

	// 设外接矩形长、宽比在[0.5,2.0]之间
#define RATIO_MIN 0.5
#define RATIO_MAX 2.0
	// 设目标面积最大值为50
#define AREA_MIN 20 
#define AREA_MAX 200
	// 设十字线标记半径为4
#define RAIDUS 4
/////////////////////////////////////////////////////////////////////////////
// DlgImageGeometryCorrect dialog


DlgImageGeometryCorrect::DlgImageGeometryCorrect(CWnd* pParent /*=NULL*/)
	: CDialog(DlgImageGeometryCorrect::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgImageGeometryCorrect)
	m_dbEdt1NextX = 0.0;
	m_dbEdt2NextX = 0.0;
	m_dbEdt3NextX = 0.0;
	m_dbEdt4NextX = 0.0;
	m_dbEdt5NextX = 0.0;
	m_dbEdt6NextX = 0.0;
	m_dbEdt7NextX = 0.0;
	m_dbEdt8NextX = 0.0;
	m_dbEdt9NextX = 0.0;	//.....

	m_dbEdt1NextY = 0.0;
	m_dbEdt2NextY = 0.0;
	m_dbEdt3NextY = 0.0;
	m_dbEdt4NextY = 0.0;
	m_dbEdt5NextY = 0.0;
	m_dbEdt6NextY = 0.0;
	m_dbEdt7NextY = 0.0;
	m_dbEdt8NextY = 0.0;
	m_dbEdt9NextY = 0.0;	//.....

	m_dbEdt1PrevX = 0.0;
	m_dbEdt2PrevX = 0.0;
	m_dbEdt3PrevX = 0.0;
	m_dbEdt4PrevX = 0.0;
	m_dbEdt5PrevX = 0.0;
	m_dbEdt6PrevX = 0.0;
	m_dbEdt7PrevX = 0.0;
	m_dbEdt8PrevX = 0.0;
	m_dbEdt9PrevX = 0.0;	//.....
	
	m_dbEdt1PrevY = 0.0;
	m_dbEdt2PrevY = 0.0;
	m_dbEdt3PrevY = 0.0;
	m_dbEdt4PrevY = 0.0;
	m_dbEdt5PrevY = 0.0;
	m_dbEdt6PrevY = 0.0;
	m_dbEdt7PrevY = 0.0;
	m_dbEdt8PrevY = 0.0;
	m_dbEdt9PrevY = 0.0;
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCanvas = NULL;
}

DlgImageGeometryCorrect::~DlgImageGeometryCorrect()
{
	if (m_pCanvas)
		delete m_pCanvas;
}

void DlgImageGeometryCorrect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgImageGeometryCorrect)
	DDX_Control(pDX, IDC_IMAGE, m_ctlImage);
	DDX_Text(pDX, IDC_EDT1_NEXT_X, m_dbEdt1NextX);
	DDX_Text(pDX, IDC_EDT2_NEXT_X, m_dbEdt2NextX);
	DDX_Text(pDX, IDC_EDT3_NEXT_X, m_dbEdt3NextX);
	DDX_Text(pDX, IDC_EDT4_NEXT_X, m_dbEdt4NextX);
	DDX_Text(pDX, IDC_EDT5_NEXT_X, m_dbEdt5NextX);
	DDX_Text(pDX, IDC_EDT6_NEXT_X, m_dbEdt6NextX);
	DDX_Text(pDX, IDC_EDT7_NEXT_X, m_dbEdt7NextX);
	DDX_Text(pDX, IDC_EDT8_NEXT_X, m_dbEdt8NextX);
	DDX_Text(pDX, IDC_EDT9_NEXT_X, m_dbEdt9NextX);	//.....

	DDX_Text(pDX, IDC_EDT1_NEXT_Y, m_dbEdt1NextY);
	DDX_Text(pDX, IDC_EDT2_NEXT_Y, m_dbEdt2NextY);
	DDX_Text(pDX, IDC_EDT3_NEXT_Y, m_dbEdt3NextY);
	DDX_Text(pDX, IDC_EDT4_NEXT_Y, m_dbEdt4NextY);
	DDX_Text(pDX, IDC_EDT5_NEXT_Y, m_dbEdt5NextY);
	DDX_Text(pDX, IDC_EDT6_NEXT_Y, m_dbEdt6NextY);
	DDX_Text(pDX, IDC_EDT7_NEXT_Y, m_dbEdt7NextY);
	DDX_Text(pDX, IDC_EDT8_NEXT_Y, m_dbEdt8NextY);
	DDX_Text(pDX, IDC_EDT9_NEXT_Y, m_dbEdt9NextY);	//.....

	DDX_Text(pDX, IDC_EDT1_PREV_X, m_dbEdt1PrevX);
	DDX_Text(pDX, IDC_EDT2_PREV_X, m_dbEdt2PrevX);
	DDX_Text(pDX, IDC_EDT3_PREV_X, m_dbEdt3PrevX);
	DDX_Text(pDX, IDC_EDT4_PREV_X, m_dbEdt4PrevX);
	DDX_Text(pDX, IDC_EDT5_PREV_X, m_dbEdt5PrevX);
	DDX_Text(pDX, IDC_EDT6_PREV_X, m_dbEdt6PrevX);
	DDX_Text(pDX, IDC_EDT7_PREV_X, m_dbEdt7PrevX);
	DDX_Text(pDX, IDC_EDT8_PREV_X, m_dbEdt8PrevX);
	DDX_Text(pDX, IDC_EDT9_PREV_X, m_dbEdt9PrevX);	//.....
	
	DDX_Text(pDX, IDC_EDT1_PREV_Y, m_dbEdt1PrevY);
	DDX_Text(pDX, IDC_EDT2_PREV_Y, m_dbEdt2PrevY);
	DDX_Text(pDX, IDC_EDT3_PREV_Y, m_dbEdt3PrevY);
	DDX_Text(pDX, IDC_EDT4_PREV_Y, m_dbEdt4PrevY);
	DDX_Text(pDX, IDC_EDT5_PREV_Y, m_dbEdt5PrevY);
	DDX_Text(pDX, IDC_EDT6_PREV_Y, m_dbEdt6PrevY);
	DDX_Text(pDX, IDC_EDT7_PREV_Y, m_dbEdt7PrevY);
	DDX_Text(pDX, IDC_EDT8_PREV_Y, m_dbEdt8PrevY);
	DDX_Text(pDX, IDC_EDT9_PREV_Y, m_dbEdt9PrevY);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(DlgImageGeometryCorrect, CDialog)
	//{{AFX_MSG_MAP(DlgImageGeometryCorrect)
	ON_BN_CLICKED(IDC_CTRL_PT_EXTRACT, OnCtrlPtExtract)
	ON_BN_CLICKED(IDC_CTRL_PT_DEL, OnCtrlPtDel)
	ON_BN_CLICKED(IDC_CTRL_PT_ADD, OnCtrlPtAdd)
	ON_BN_CLICKED(IDC_IMAGE_CORRECT, OnImageCorrect)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgImageGeometryCorrect message handlers

BOOL DlgImageGeometryCorrect::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	CTilCanvasMgr* pMgr = GetCanvasMgr();
	CTilCanvas* pCanvas = pMgr->GetCurCanvas();
	m_pCanvas = new CTilCanvas(*pCanvas);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DlgImageGeometryCorrect::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect srcRect(0, 0, m_pCanvas->GetWidth(), m_pCanvas->GetHeight());
	CRect dstRect;
	m_ctlImage.GetClientRect(&dstRect);
	m_pCanvas->Draw(m_ctlImage.GetDC(), dstRect, srcRect);

	// 用十字线标记控制点
	DrawCrossLine(m_ctlImage.GetDC(), m_ctrlPoints);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void DlgImageGeometryCorrect::OnLButtonUp(UINT nFlags, CPoint point) 
{
	POINT ctrlPoint;
	CImage* pImage = m_pCanvas->GetImage()->GetCurImage();

	// 察看点是否在图像显示控件中
	CRect rect;
	m_ctlImage.GetClientRect(&rect);

	// 将点坐标转化成图像显示控件中该点的坐标
	ClientToScreen(&point);
	m_ctlImage.ScreenToClient(&point);

	// 如果点不在图像显示控件中,则直接退出
	if (!rect.PtInRect(point))
		return;

	// 将图像显示控件中点坐标转换为图像中的点坐标
	ctrlPoint.x = point.x*1.0/rect.Width()*pImage->GetWidth()+0.5;
	ctrlPoint.y = point.y*1.0/rect.Height()*pImage->GetHeight();
	ctrlPoint.y = pImage->GetHeight()-1-ctrlPoint.y+0.5;

	// 对控制点进行区域生长
	// 区域生长阈值
#define RGN_GRW_THRSHLD	50

	// 获得区域生长区域的游程
	ObjRunLenCode tmpRLC;
	pImage->ObjRgnGrow(ctrlPoint, RGN_GRW_THRSHLD, &tmpRLC);

	// 如果是添加控制点
	if (bAdd)
	{
		// 对区域进行分析
		int objArea = tmpRLC.ObjArea(0);

		// 如果面积不满足条件,则直接退出
		if (objArea<AREA_MIN || objArea>AREA_MAX)
			return;

/*
		CRect objRect;
		tmpRLC.ObjRect(0, objRect);

		// 如果长宽比不满足条件,则直接退出
		float fRatio = objRect.Width()*1.0/objRect.Height();
		if (fRatio<RATIO_MIN || fRatio>RATIO_MAX)
			return;
*/

		// 获得生长区域的的中心点坐标
		POINT2DF ptCenter;
		tmpRLC.ObjCenter(0, ptCenter);

		// 将图像中点坐标转换为图像显示控件中该点坐标
		ptCenter.x = ptCenter.x*1.0/pImage->GetWidth()*rect.Width();
		ptCenter.y = (pImage->GetHeight()-ptCenter.y-1)*1.0/pImage->GetHeight()*rect.Height();

		// 若没有相同的点,则将控制点压入堆栈
		for (int i=0; i<m_ctrlPoints.size(); i++)
		{
			if (int(m_ctrlPoints[i].x)==int(ptCenter.x)
				&& int(m_ctrlPoints[i].y)==int(ptCenter.y))
				break;
		}
		if (i >= m_ctrlPoints.size())
			m_ctrlPoints.push_back(ptCenter);

		// 更新以点为中心的十字外接矩形区域数据
		// 将点坐标转换到对话框坐标系下
		POINT ptTmp;
		ptTmp.x = ptCenter.x;
		ptTmp.y = ptCenter.y;
		m_ctlImage.ClientToScreen(&ptTmp);
		ScreenToClient(&ptTmp);

		// 以点为中心的十字线外接矩形
		CRect rtTmp;
		rtTmp.left = ptCenter.x-RAIDUS;
		rtTmp.right = ptCenter.x+RAIDUS+1;;
		rtTmp.top = ptCenter.y-RAIDUS;
		rtTmp.bottom = ptCenter.y+RAIDUS+1;

		InvalidateRect(&rtTmp);
	}
	else
	{
		// 若是删除控制点,则查看控制点是否有在区域生长形成的目标中
		PPRUNLENGTHCODE_MGR ppRLCMgr = tmpRLC.GetObjAll();
		for (int i=0; i<m_ctrlPoints.size(); i++)
		{
			// 将控制点的图像显示控件中的坐标转换成图像坐标
			POINT ptTmp;
			ptTmp.x = m_ctrlPoints[i].x*1.0/rect.Width()*pImage->GetWidth()+0.5;
			ptTmp.y = m_ctrlPoints[i].y*1.0/rect.Height()*pImage->GetHeight();
			ptTmp.y = pImage->GetHeight()-1-ptTmp.y+0.5;

			// 若控制点在区域生长形成的目标中,则直接跳出循环
			for (int j=0; j<ppRLCMgr[0]->nRunNum; j++)
			{
				if (ptTmp.y==ppRLCMgr[0]->RC[j].y
					&& ptTmp.x>=ppRLCMgr[0]->RC[j].xStart
					&& ptTmp.x<=ppRLCMgr[0]->RC[j].xEnd)
					break;
			}

			// 如果j循环没有到末尾就中止,则删除第i点,不再删除其它点,直接退出
			if (j < ppRLCMgr[0]->nRunNum)
			{
				// 更新以点为中心的十字外接矩形区域数据
				// 将点坐标转换到对话框坐标系下
				POINT ctrlPt;
				ctrlPt.x = m_ctrlPoints[i].x;
				ctrlPt.y = m_ctrlPoints[i].y;
				m_ctlImage.ClientToScreen(&ctrlPt);
				ScreenToClient(&ctrlPt);

				// 以点为中心的十字线外接矩形
				CRect ctrlRt;
				ctrlRt.left = ctrlPt.x-RAIDUS;
				ctrlRt.right = ctrlPt.x+RAIDUS+1;;
				ctrlRt.top = ctrlPt.y-RAIDUS;
				ctrlRt.bottom = ctrlPt.y+RAIDUS+1;

				// 删除第i点,更新该区域后退出
				m_ctrlPoints.erase(m_ctrlPoints.begin()+i);
				InvalidateRect(&ctrlRt);
				break;
			}
		}
	}

	// 显示控制点坐标
	ShowCtrlPointsCoord();

	CDialog::OnLButtonUp(nFlags, point);
}

// 控制点提取
void DlgImageGeometryCorrect::OnCtrlPtExtract() 
{
	// 点击该按钮后,将按钮变灰,即不允许再点击该按钮
	GetDlgItem(IDC_CTRL_PT_EXTRACT)->EnableWindow(FALSE);

	// 用最佳阈值对图像二值化
	CImage* pImage = m_pCanvas->GetImage()->GetCurImage();
	int nThreshold = pImage->GetBestThreshold();
	pImage->Threshold(nThreshold);

	// 获取控制点并在图中作上标记
	CTRLPOINTS ctrlPts;
	ObjPtAutoExtract(pImage, ctrlPts);
	TransferCoord(ctrlPts, m_ctrlPoints);
	Invalidate();

	// 显示控制点坐标
	ShowCtrlPointsCoord();
}

// 错误控制点手动删除
void DlgImageGeometryCorrect::OnCtrlPtDel() 
{
	// TODO: Add your control notification handler code here
	bAdd = FALSE;
}

// 控制点手动添加
void DlgImageGeometryCorrect::OnCtrlPtAdd() 
{
	// TODO: Add your control notification handler code here
	bAdd = TRUE;
}

// 图像几何校正
void DlgImageGeometryCorrect::OnImageCorrect() 
{
	// TODO: Add your control notification handler code here
	bAdd = FALSE;
}

void DlgImageGeometryCorrect::OnOk() 
{
	// TODO: Add your control notification handler code here
	
}

// 自动取得目标控制点,设图像已经为二值图像
void DlgImageGeometryCorrect::ObjPtAutoExtract(CImage* pBinImg, CTRLPOINTS& ctrlPoints)
{
	int i, j;

	// 取得二值图像的游程编码
	ObjRunLenCode tmpRLC;
	pBinImg->ObjAllRLC(&tmpRLC, 255);

	// 如果没有目标,则退出
	int nObjNum = tmpRLC.GetObjNum();
	if (nObjNum == 0)
		return;

	PPRUNLENGTHCODE_MGR ppRLCMgr = tmpRLC.GetObjAll();

	// 求所有目标的面积(像素数)及外接矩形
	int* pObjArea = new int [nObjNum];
	memset(pObjArea, 0, sizeof(int)*nObjNum);
	CRect* pObjRect = new CRect [nObjNum];
	for (i=0; i<nObjNum; i++)
	{
		// 面积累加
		for (j=0; j<ppRLCMgr[i]->nRunNum; j++)
		{
			pObjArea[i] += ppRLCMgr[i]->RC[j].xEnd-ppRLCMgr[i]->RC[j].xStart+1;
		}

		// 外接矩形
		tmpRLC.ObjRect(ppRLCMgr[i], pObjRect[i]);
	}

	// 根据面积和外接矩形查找目标控制点
	// 单独存储满足条件的目标
	vector< int > vecIdx;
	vector< int > vecArea;
	vector< PRUNLENGTHCODE_MGR > vecRLC;
	for (i=0; i<nObjNum; i++)
	{
		float fRatio;
		fRatio = pObjRect[i].Width()*1.0/pObjRect[i].Height();

		// 将长宽比、面积满足条件的目标压入堆栈
		if (fRatio>=RATIO_MIN && fRatio<=RATIO_MAX)
		{
			// 对面积满足条件的目标进行标记
			if (pObjArea[i]>=AREA_MIN && pObjArea[i]<=AREA_MAX)
			{
				// 记录满足条件点的序号、面积、游程
				vecIdx.push_back(i);
				vecArea.push_back(pObjArea[i]);
				vecRLC.push_back(ppRLCMgr[i]);
			}
		}
	}

	// 面积按从大到小对目标排序
	int nTmp;
	PRUNLENGTHCODE_MGR pItem;
	for (i=0; i<vecArea.size(); i++)
	{
		for (j=i+1; j<vecArea.size(); j++)
		{
			if (vecArea[i] < vecArea[j])
			{
				nTmp = vecArea[i];
				vecArea[i] = vecArea[j];
				vecArea[j] = nTmp;

				nTmp = vecIdx[i];
				vecIdx[i] = vecIdx[j];
				vecIdx[j] = nTmp;

				pItem = vecRLC[i];
				vecRLC[i] = vecRLC[j];
				vecRLC[j] = pItem;
			}
		}
	}

	// 获得目标重心坐标
	POINT2DF* pCenter = new POINT2DF [vecRLC.size()];
	memset(pCenter, 0, sizeof(POINT2DF)*vecRLC.size());

	for (i=0; i<vecRLC.size(); i++)
	{
		tmpRLC.ObjCenter(vecRLC[i], pCenter[i]);
	}

	// 返回数据(最多取前9个点)
	int nNum = (vecRLC.size()<9) ? vecRLC.size() : 9;
	for (i=0; i<nNum; i++)
		ctrlPoints.push_back(pCenter[i]);

	delete [] pCenter;
	delete [] pObjRect;
	delete [] pObjArea;
}

// 手动取得目标控制点
//int DlgImageGeometryCorrect::ObjPtManualExtract(CImage* pBinImg, POINT point, CTRLPOINTS& ctrlPoints)
//{
//	return 0;
//}

// 手动删除错误目标控制点
//int DlgImageGeometryCorrect::ObjPtManualDel(CImage* pBinImg, POINT point, CTRLPOINTS& ctrlPoints)
//{
//	return 0;
//}

// 将控制点在图像中的坐标转换成显示图像控制中的坐标
void DlgImageGeometryCorrect::TransferCoord(CTRLPOINTS& srcCtrlPts, CTRLPOINTS& dstCtrlPts)
{
	CImage* pImage = m_pCanvas->GetImage()->GetCurImage();
	CRect ctrlRect;
	m_ctlImage.GetClientRect(&ctrlRect);

	// 坐标转换
	for (int i=0; i<srcCtrlPts.size(); i++)
	{
		POINT2DF pt;
		pt.x = srcCtrlPts[i].x*1.0/pImage->GetWidth()*ctrlRect.Width();
		pt.y = srcCtrlPts[i].y*1.0/pImage->GetHeight()*ctrlRect.Height();

		// 由于图像左下角为原点,y轴向上,显示图像控制左上角为原点,y轴向下,故需要转换
		pt.y = ctrlRect.Height()-pt.y-1;

		dstCtrlPts.push_back(pt);
	}
}

// 画出以控制点为中心的红十字形
void DlgImageGeometryCorrect::DrawCrossLine(CDC* pDC, CTRLPOINTS& ctrlPoints)
{
	// 以控制点为中心设置十字线标记
	CPen pen(PS_SOLID, 1, RGB(255, 0, 0));
	CPen* pOldPen = pDC->SelectObject(&pen);
	CRect ctrlRect;
	m_ctlImage.GetClientRect(&ctrlRect);

	for (int i=0; i<ctrlPoints.size(); i++)
	{
		// 十字线显示范围
		RECT rect;
		rect.left = (ctrlPoints[i].x-RAIDUS>0) ? ctrlPoints[i].x-RAIDUS : 0;
		rect.right = (ctrlPoints[i].x+RAIDUS<ctrlRect.Width()) ? ctrlPoints[i].x+RAIDUS : ctrlRect.Width();
		rect.top = (ctrlPoints[i].y-RAIDUS>0) ? ctrlPoints[i].y-RAIDUS : 0;
		rect.bottom = (ctrlPoints[i].y+RAIDUS<ctrlRect.Height()) ? ctrlPoints[i].y+RAIDUS : ctrlRect.Height();

		pDC->MoveTo(ctrlPoints[i].x, rect.top);
		pDC->LineTo(ctrlPoints[i].x, rect.bottom+1);
		pDC->MoveTo(rect.left, ctrlPoints[i].y);
		pDC->LineTo(rect.right+1, ctrlPoints[i].y);
	}

	pDC->SelectObject(pOldPen);
}

/*
// 用红色十字线标记控制点坐标
void DlgImageGeometryCorrect::set_cross_line(CTRLPOINTS& ctrlPoints)
{
	// 将控制点的图像坐标转换成对象框中显示图像控制中的坐标
	vector<POINT2DF> convertPts;

	CImage* pImage = m_pCanvas->GetImage()->GetCurImage();
	CRect dstRect;
	m_ctlImage.GetClientRect(&dstRect);

	// 坐标转换
	for (int i=0; i<convertPts.size(); i++)
	{
		convertPts[i].x = ctrlPoints[i].x*1.0/pImage->GetWidth()*dstRect.Width();
		convertPts[i].y = ctrlPoints[i].y*1.0/pImage->GetHeight()*dstRect.Height();

		// 由于图像左下角为原点,y轴向上,显示图像控制左上角为原点,y轴向下,故需要转换
		convertPts[i].y = dstRect.Height()-convertPts[i].y-1;
	}

	// 设十字线标记半径为3
#define RAIDUS 4

	// 以控制点为中心设置十字线标记
	CDC* pDC = m_ctlImage.GetDC();
	CPen pen(PS_SOLID, 1, RGB(255, 0, 0));
	CPen* pOldPen = pDC->SelectObject(&pen);

	for (i=0; i<convertPts.size(); i++)
	{
		// 十字线显示范围
		RECT rect;
		rect.left = (convertPts[i].x-RAIDUS>0) ? convertPts[i].x-RAIDUS : 0;
		rect.right = (convertPts[i].x+RAIDUS<dstRect.Width()) ? convertPts[i].x+RAIDUS : dstRect.Width();
		rect.top = (convertPts[i].y-RAIDUS>0) ? convertPts[i].y-RAIDUS : 0;
		rect.bottom = (convertPts[i].y+RAIDUS<dstRect.Height()) ? convertPts[i].y+RAIDUS : dstRect.Height();

		pDC->MoveTo(convertPts[i].x, rect.top);
		pDC->LineTo(convertPts[i].x, rect.bottom+1);
		pDC->MoveTo(rect.left, convertPts[i].y);
		pDC->LineTo(rect.right+1, convertPts[i].y);
	}

	pDC->SelectObject(pOldPen);
}
*/
// 将目标控制点坐标显示在"校正前控制点坐标"组合框中
void DlgImageGeometryCorrect::ShowCtrlPointsCoord()
{
	if (m_ctrlPoints.size() > 0)
	{
		m_dbEdt1PrevX = m_ctrlPoints[0].x;
		m_dbEdt1PrevY = m_ctrlPoints[0].y;
	}
	else
	{
		m_dbEdt1PrevX = 0;
		m_dbEdt1PrevY = 0;
	}

	if (m_ctrlPoints.size() > 1)
	{
		m_dbEdt2PrevX = m_ctrlPoints[1].x;
		m_dbEdt2PrevY = m_ctrlPoints[1].y;
	}
	else
	{
		m_dbEdt2PrevX = 0;
		m_dbEdt2PrevY = 0;
	}

	if (m_ctrlPoints.size() > 2)
	{
		m_dbEdt3PrevX = m_ctrlPoints[2].x;
		m_dbEdt3PrevY = m_ctrlPoints[2].y;
	}
	else
	{
		m_dbEdt3PrevX = 0;
		m_dbEdt3PrevY = 0;
	}

	if (m_ctrlPoints.size() > 3)
	{
		m_dbEdt4PrevX = m_ctrlPoints[3].x;
		m_dbEdt4PrevY = m_ctrlPoints[3].y;
	}
	else
	{
		m_dbEdt4PrevX = 0;
		m_dbEdt4PrevY = 0;
	}

	if (m_ctrlPoints.size() > 4)
	{
		m_dbEdt5PrevX = m_ctrlPoints[4].x;
		m_dbEdt5PrevY = m_ctrlPoints[4].y;
	}
	else
	{
		m_dbEdt5PrevX = 0;
		m_dbEdt5PrevY = 0;
	}

	if (m_ctrlPoints.size() > 5)
	{
		m_dbEdt6PrevX = m_ctrlPoints[5].x;
		m_dbEdt6PrevY = m_ctrlPoints[5].y;
	}
	else
	{
		m_dbEdt6PrevX = 0;
		m_dbEdt6PrevY = 0;
	}

	if (m_ctrlPoints.size() > 6)
	{
		m_dbEdt7PrevX = m_ctrlPoints[6].x;
		m_dbEdt7PrevY = m_ctrlPoints[6].y;
	}
	else
	{
		m_dbEdt7PrevX = 0;
		m_dbEdt7PrevY = 0;
	}

	if (m_ctrlPoints.size() > 7)
	{
		m_dbEdt8PrevX = m_ctrlPoints[7].x;
		m_dbEdt8PrevY = m_ctrlPoints[7].y;
	}
	else
	{
		m_dbEdt8PrevX = 0;
		m_dbEdt8PrevY = 0;
	}

	if (m_ctrlPoints.size() > 8)
	{
		m_dbEdt9PrevX = m_ctrlPoints[8].x;
		m_dbEdt9PrevY = m_ctrlPoints[8].y;
	}
	else
	{
		m_dbEdt9PrevX = 0;
		m_dbEdt9PrevY = 0;
	}

	UpdateData(FALSE);
}

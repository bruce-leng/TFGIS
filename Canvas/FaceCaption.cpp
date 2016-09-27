// FaceCaption.cpp: implementation of the CFaceCaption class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "canvas.h"
#include "FaceCaption.h"
#include "TilGraph.h"
#include "Graph\Line.h"
#include "Graph\Polygon.h"
#include "MathComput.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFaceCaption::CFaceCaption()
{
	m_pCanvas = NULL;
	m_fDepth = -1;
	m_fAveJointLen = -1;
	m_fAveRockSize = -1;
	m_fDiscontinueFreq = -1;
	m_fRQD = -1;
	m_fDividLines = -1;
	m_fEdgeLines = -1;
	m_fRockCrack = -1;
	m_fAveRed = m_fAveGreen = m_fAveBlue = -1;
	m_fMean2DevRed = m_fMean2DevGreen = m_fMean2DevBlue = -1;
	m_fRockWeathRed = m_fRockWeathGreen = m_fRockWeathBlue = -1;

	m_fAngle1 = -1;
	m_fAngle2 = -1;
	m_fAngle3 = -1;
	m_fAngle4 = -1;

	// 2009.08.28
	m_pImage = NULL;
	m_pObjFace = NULL;
}

// 从CTilCanvas类对象中拷贝数据
void CFaceCaption::CopyFrom(CTilCanvas* pCanvas)
{
	if (m_pCanvas)
		delete m_pCanvas;
	m_pCanvas = new CTilCanvas(*pCanvas);

	CGraph* pTunArc = pCanvas->GetGraph()->GetGraphByIdx(GRAPH_LAYER_TUNARC);
	// 如果隧道开挖轮廓线不存在，则将图像边界当作轮廓线处理
	if (pTunArc->Size() == 0)
	{
		// 设置图像的默认外轮廓
		m_pCanvas->SetDefaultTunArc();

		TRACE("隧道开挖轮廓线不存在!设置图片的外接矩形为轮廓线!");
	}

	m_fDepth = -1;
	m_fAveJointLen = -1;
	m_fAveRockSize = -1;
	m_fDiscontinueFreq = -1;
	m_fRQD = -1;
	m_fDividLines = -1;
	m_fEdgeLines = -1;
	m_fRockCrack = -1;
	m_fAveRed = m_fAveGreen = m_fAveBlue = -1;
	m_fMean2DevRed = m_fMean2DevGreen = m_fMean2DevBlue = -1;
	m_fRockWeathRed = m_fRockWeathGreen = m_fRockWeathBlue = -1;

	m_fAngle1 = -1;
	m_fAngle2 = -1;
	m_fAngle3 = -1;
	m_fAngle4 = -1;

	// 2009.08.28
	m_pImage = NULL;
	m_pObjFace = NULL;
}

CFaceCaption::~CFaceCaption()
{
	if (m_pCanvas)
		delete m_pCanvas;
	if (m_pImage)
		delete m_pImage;
	if (m_pObjFace)
		delete m_pObjFace;
}

// 单位面积裂隙总长度(m/m2)
float CFaceCaption::JointLengthofUnitArea()
{
	// 求掌子面开挖轮廓线或掌子面图片外接矩形的面积
	long lArea;
	int i, j, k;

	// 获得开挖轮廓线或掌子面图片外接矩形的图元指针
	CTilGraph* pGraphMgr = m_pCanvas->GetGraph();
	CGraph* pTunArc = pGraphMgr->GetGraphByIdx(GRAPH_LAYER_TUNARC);
	CUnit* pUnit = pTunArc->GetUnitByIdx(0);

	// 获得面积
	int nSize = pUnit->m_pPointList->GetSize();
	POINT2DF* pPts = new POINT2DF[nSize];
	for (i=0; i<nSize; i++)
		pPts[i] = pUnit->m_pPointList->GetAt(i);
	lArea = get_poly_area(pPts, nSize);
	delete [] pPts;

	// 求节理总长度
	POINT2DF pt1, pt2;
	double dbJointLen = 0.0;
	nSize = pGraphMgr->Size();
	for (i=1; i<nSize; i++)
	{
		CGraph* pGraph = pGraphMgr->GetGraphByIdx(i);
		int nUnitNum = pGraph->Size();
		for (j=0; j<nUnitNum; j++)
		{
			CUnit* pUnit = pGraph->GetUnitByIdx(j);
			if (!pUnit->IsKindOf(RUNTIME_CLASS(CPolygon))
				|| pUnit->IsPolyClose())
				continue;

			POINT2DF pt1, pt2;
			pt1 = pUnit->m_pPointList->GetAt(0);
			for (k=1; k<pUnit->m_pPointList->GetSize(); k++)
			{
				pt2 = pUnit->m_pPointList->GetAt(k);
				dbJointLen += get_distance(pt1, pt2);
				pt1 = pt2;
			}
		}
	}

	// 单位面积节理长度
	m_fAveJointLen = float(dbJointLen*100/lArea);

	return m_fAveJointLen;
}

// 平均块体尺寸(m)
// float fLineDist: 割线间距，单位为厘米
float CFaceCaption::AveRockSize(float fLineDist)
{
	CTilGraph* pGraphMgr = m_pCanvas->GetGraph();
	CGraph* pTunArc = pGraphMgr->GetGraphByIdx(GRAPH_LAYER_TUNARC);
	ASSERT(pTunArc->Size()!=0);

	// 求节理、裂隙的平均间距
	// 首先，将图形层转化成图像层
	CImage* pImage = graph2image(pGraphMgr);

	// 按每间距fLineDist求节理的平均间隔
	BOOL bMark;
	int i,j,nStart, nEnd, nNum = 0;
	long lTotalSize=0;
	int nWidth = pImage->GetWidth();
	int nHeight = pImage->GetHeight();
	// 水平方向求
	for (i=0; i<nHeight; i+=fLineDist)
	{
		bMark = FALSE;
		for (j=1; j<nWidth-1; j++)
		{
			RGBQUAD rgb1,rgb2;
			rgb1 = pImage->GetPixelColor(j,i);
			if (rgb1.rgbRed==255)
			{
				if (!bMark)
				{
					nStart = j;
					bMark = TRUE;
				}
			}
			else
			{
				// 如果前一个像素值为255，而当前像素值为0
				rgb2 = pImage->GetPixelColor(j-1,i);
				if (rgb2.rgbRed==255)
				{
					nEnd = j-1;
					nNum ++;
				}
			}
		}
		lTotalSize += nEnd-nStart+1;
	}

	// 求垂直方向
	for (i=0; i<nWidth; i+=fLineDist)
	{
		bMark = FALSE;
		for (j=1; j<nHeight-1; j++)
		{
			RGBQUAD rgb1,rgb2;
			rgb1 = pImage->GetPixelColor(i,j);
			if (rgb1.rgbRed==255)
			{
				if (!bMark)
				{
					nStart = j;
					bMark = TRUE;
				}
			}
			else
			{
				// 如果前一个像素值为255，而当前像素值为0
				rgb2 = pImage->GetPixelColor(i,j-1);
				if (rgb2.rgbRed==255)
				{
					nEnd = j-1;
					nNum ++;
				}
			}
		}
		lTotalSize += nEnd-nStart+1;
	}

	// 平均块体尺寸
	m_fAveRockSize = float(lTotalSize*1.0/nNum/100);
	delete pImage;

	return m_fAveRockSize;
}

// 不连续频率-为平均块体倒数
float CFaceCaption::DiscontinueFreqency(float fLineDist)
{
	m_fAveRockSize = 1/AveRockSize(fLineDist);
	return m_fAveRockSize;
}

// 取得掌子面上的RQD值
// float fLineDist: 割线间距, 单位为厘米
// float fMinDist: 最小距离, 单位为厘米
float CFaceCaption::RQD(float fLineDist, float fMinDist)
{
	CTilGraph* pGraphMgr = m_pCanvas->GetGraph();
	CGraph* pTunArc = pGraphMgr->GetGraphByIdx(GRAPH_LAYER_TUNARC);
	ASSERT(pTunArc->Size()!=0);

	// 求节理、裂隙的平均间距
	// 首先，将图形层转化成图像层
	CImage* pImage = graph2image(pGraphMgr);

	// 按每间距fLineDist求节理的平均间隔
	BOOL bMark;
	int i,j,nStart0, nStart, nEnd, nDist;
	long lTotalSize=0, lSize=0;
	int nWidth = pImage->GetWidth();
	int nHeight = pImage->GetHeight();

	// 水平方向求
	RGBQUAD rgb1,rgb2;
	for (i=0; i<nHeight; i+=fLineDist)
	{
		bMark = FALSE;
		rgb1 = pImage->GetPixelColor(0,i);
		for (j=1; j<nWidth-1; j++)
		{
			rgb2 = pImage->GetPixelColor(j,i);
			if (rgb1.rgbRed==0 && rgb2.rgbRed==255)
			{
				nStart = j;
				if (!bMark)
				{
					nStart0 = nStart;
					bMark = TRUE;
				}
			}
			else if (rgb1.rgbRed==255 && rgb2.rgbRed==0)
			{
				// 如果前一个像素值为255，而当前像素值为0
				nEnd = j-1;
				nDist = nEnd-nStart+1;
				if (nDist>=fMinDist)
					lSize += nDist;
			}
			rgb1 = rgb2;
		}

		// 如果bMark==true,说明第i行有目标颜色
		if (bMark)
			lTotalSize += nEnd-nStart0+1;
	}

	// 求垂直方向
	for (i=0; i<nWidth; i+=fLineDist)
	{
		bMark = FALSE;
		rgb1 = pImage->GetPixelColor(i,0);
		for (j=1; j<nHeight-1; j++)
		{
			rgb2 = pImage->GetPixelColor(i,j);
			if (rgb1.rgbRed==0 && rgb2.rgbRed==255)
			{
				nStart = j;
				if (!bMark)
				{
					nStart0 = nStart;
					bMark = TRUE;
				}
			}
			else if (rgb1.rgbRed==255 && rgb2.rgbRed==0)
			{
				// 如果前一个像素值为255，而当前像素值为0
				nEnd = j-1;
				nDist = nEnd-nStart+1;
				if (nDist>=fMinDist)
					lSize += nDist;
			}
			rgb1 = rgb2;
		}

		// 如果bMark==true,说明第i行有目标颜色
		if (bMark)
			lTotalSize += nEnd-nStart0+1;
	}

	// 平均RQD值
	m_fRQD = float(lSize*1.0/lTotalSize);
	delete pImage;

	return m_fRQD;
}

// 单位面积割线数量
float CFaceCaption::DividLinesofUnitArea(float fLineDist)
{
	CTilGraph* pGraphMgr = m_pCanvas->GetGraph();
	CGraph* pTunArc = pGraphMgr->GetGraphByIdx(GRAPH_LAYER_TUNARC);
	ASSERT(pTunArc->Size()!=0);

	// 求节理、裂隙的平均间距
	// 首先，将图形层转化成图像层
	CImage* pImage = graph2image(pGraphMgr);

	// 按每间距fLineDist求节理的平均间隔
	BOOL bMark;
	int nDividLines=0;
	int i,j,nStart0, nStart, nEnd;
	long lTotalSize=0, lSize=0;
	int nWidth = pImage->GetWidth();
	int nHeight = pImage->GetHeight();

	// 水平方向求
	RGBQUAD rgb1,rgb2;
	for (i=0; i<nHeight; i+=fLineDist)
	{
		bMark = FALSE;
		rgb1 = pImage->GetPixelColor(0,i);
		for (j=1; j<nWidth-1; j++)
		{
			rgb2 = pImage->GetPixelColor(j,i);
			if (rgb1.rgbRed==0 && rgb2.rgbRed==255)
			{
				nStart = j;
				if (!bMark)
				{
					nStart0 = nStart;
					bMark = TRUE;
				}
			}
			else if (rgb1.rgbRed==255 && rgb2.rgbRed==0)
			{
				// 如果前一个像素值为255，而当前像素值为0
				nEnd = j-1;
				nDividLines ++;
			}
			rgb1 = rgb2;
		}

//		// 如果bMark==true,说明第i行有目标颜色
//		if (bMark)
//			lTotalSize += nEnd-nStart0+1;
	}

	// 求垂直方向
	for (i=0; i<nWidth; i+=fLineDist)
	{
		bMark = FALSE;
		rgb1 = pImage->GetPixelColor(i,0);
		for (j=1; j<nHeight-1; j++)
		{
			rgb2 = pImage->GetPixelColor(i,j);
			if (rgb1.rgbRed==0 && rgb2.rgbRed==255)
			{
				nStart = j;
				if (!bMark)
				{
					nStart0 = nStart;
					bMark = TRUE;
				}
			}
			else if (rgb1.rgbRed==255 && rgb2.rgbRed==0)
			{
				// 如果前一个像素值为255，而当前像素值为0
				nEnd = j-1;
				nDividLines ++;
			}
			rgb1 = rgb2;
		}

//		// 如果bMark==true,说明第i行有目标颜色
//		if (bMark)
//			lTotalSize += nEnd-nStart0+1;
	}

	delete pImage;

	// 求掌子面面积,此处为平方厘米
	ObjRunLenCode* pObjRLC = facearea(pGraphMgr);
	float fFaceArea = pObjRLC->ObjArea(0);
	delete pObjRLC;

	m_fDividLines = nDividLines*1.0/fFaceArea*10000;
	return m_fDividLines;
}

// 单位面积边界线数量
float CFaceCaption::EdgeLinesofUnitArea()
{
	int i,j;
	int nEdgeLines = 0;
	float fFaceArea;

	// 统计边界线条数
	CTilGraph* pGraphMgr = m_pCanvas->GetGraph();
	for (i=0; i<pGraphMgr->Size(); i++)
	{
		CGraph* pGraph = pGraphMgr->GetGraphByIdx(i);
		for (j=0; j<pGraph->Size(); j++)
		{
			CUnit* pUnit = pGraph->GetUnitByIdx(j);
			// 计算多段线数量
			if (i!=GRAPH_LAYER_TUNARC
				&& (pUnit->IsKindOf(RUNTIME_CLASS(CPolygon))
				|| pUnit->IsPolyClose()))
				nEdgeLines ++;
		}
	}

	// 求掌子面面积,此处为平方厘米
	ObjRunLenCode* pObjRLC = facearea(pGraphMgr);
	fFaceArea = pObjRLC->ObjArea(0);
	delete pObjRLC;

	m_fEdgeLines = nEdgeLines/fFaceArea*10000;
	return m_fEdgeLines;
}

// 掌子面岩体破碎程度评价指标
float CFaceCaption::RockCrackDegree()
{
	if (m_fDividLines < 0)
		m_fDividLines = DividLinesofUnitArea(10);
	if (m_fEdgeLines < 0)
		m_fEdgeLines = EdgeLinesofUnitArea();

	m_fRockCrack = m_fDividLines+m_fEdgeLines;
	return m_fRockCrack;
}

// 掌子面上的平均灰度值
RGBQUAD CFaceCaption::GreyAveValue(float& fRed, float& fGreen, float& fBlue)
{
	int i;
	long nNum=0;
	RGBQUAD rgb;
	long double ldRed = 0, ldGreen = 0, ldBlue = 0;

	// 求掌子面区域的游程编码
	CTilGraph* pGraphMgr = m_pCanvas->GetGraph();

	ObjRunLenCode* pObjRLC = facearea(pGraphMgr);

	// 取得掌子面图像数据
	CImage* pImage = m_pCanvas->GetImage()->GetCurImage();
	int nWidth = pImage->GetWidth();
	int nHeight = pImage->GetHeight();

	// 求掌子面区域像素的最大值和最小值
	char cMaxR,cMaxG,cMaxB,cMinR,cMinG,cMinB;
	cMaxR = cMaxG = cMaxB = 0;
	cMinR = cMinG = cMinB = 255;
	int nRunNum = pObjRLC->GetObj(0)->nRunNum;
	RUNLENGTHCODE* rc = pObjRLC->GetObj(0)->RC;
	for (i=0; i<nRunNum; i++)
	{
		int xStart, xEnd, x, y;
		xStart = rc[i].xStart;
		xEnd   = rc[i].xEnd;
		y	   = rc[i].y;

		for (x=xStart; x<=xEnd; x++)
		{
			RGBQUAD tmp = pImage->GetPixelColor(x,y);
			if (cMaxR < tmp.rgbRed)		cMaxR = tmp.rgbRed;
			if (cMaxG < tmp.rgbGreen)	cMaxG = tmp.rgbGreen;
			if (cMaxB < tmp.rgbBlue)	cMaxB = tmp.rgbBlue;

			if (cMinR > tmp.rgbRed)		cMinR = tmp.rgbRed;
			if (cMinG > tmp.rgbGreen)	cMinG = tmp.rgbGreen;
			if (cMinB > tmp.rgbBlue)	cMinB = tmp.rgbBlue;
		}
	}

	// 由于实际情况下掌子面上光照不均匀引起局部区域过亮或过暗
	// 对这部分像素进行限制
	cMaxR -= (cMaxR-cMinR)*0.2;
	cMaxG -= (cMaxG-cMinG)*0.2;
	cMaxB -= (cMaxB-cMinB)*0.2;
	cMinR += (cMaxR-cMinR)*0.2;
	cMinG += (cMaxG-cMinG)*0.2;
	cMinB += (cMaxB-cMinB)*0.2;

	// 统计掌子面图像中掌子面区域的灰度值和
	for (i=0; i<nRunNum; i++)
	{
		int xStart, xEnd, x, y;
		xStart = rc[i].xStart;
		xEnd   = rc[i].xEnd;
		y	   = rc[i].y;

		for (x=xStart; x<=xEnd; x++)
		{
			RGBQUAD tmp = pImage->GetPixelColor(x,y);
			ldRed += BOUND(tmp.rgbRed, cMinR, cMaxR);
			ldGreen += BOUND(tmp.rgbGreen, cMinG, cMaxG);
			ldBlue += BOUND(tmp.rgbBlue, cMinB, cMaxB);
		}

		nNum += xEnd-xStart+1;
	}

	// 求掌子面图像中掌子面区域的平均灰度值
	m_fAveRed = fRed = ldRed/nNum;
	m_fAveGreen = fGreen = ldGreen/nNum;
	m_fAveBlue = fBlue = ldBlue/nNum;

	rgb.rgbRed   = fRed;
	rgb.rgbGreen = fGreen;
	rgb.rgbBlue  = fBlue;

	delete pObjRLC;

	return rgb;
}

// 掌子面上的灰度均方差
void CFaceCaption::GreyMean2Deviation(float& fRed, float& fGreen, float& fBlue)
{
	int i;
	long nNum = 0;

	// 求掌子面的灰度均值
	float fAveRed, fAveGreen, fAveBlue;
	if (m_fAveRed < 0)
		GreyAveValue(fAveRed, fAveGreen, fAveBlue);
	else
	{
		fAveRed = m_fAveRed;
		fAveGreen = m_fAveGreen;
		fAveBlue = m_fAveBlue;
	}

	long double ldRed = 0, ldGreen = 0, ldBlue = 0;

	// 计算掌子面上所有像素灰度值与灰度均值之差的平方和
	CTilGraph* pGraphMgr = m_pCanvas->GetGraph();
	ObjRunLenCode* pObjRLC = facearea(pGraphMgr);

	// 取得掌子面图像数据
	CImage* pImage = m_pCanvas->GetImage()->GetCurImage();
	int nWidth = pImage->GetWidth();
	int nHeight = pImage->GetHeight();

	// 统计掌子面图像中掌子面区域的灰度值和
	int nRunNum = pObjRLC->GetObj(0)->nRunNum;
	RUNLENGTHCODE* rc = pObjRLC->GetObj(0)->RC;
	for (i=0; i<nRunNum; i++)
	{
		int xStart, xEnd, x, y;
		xStart = rc[i].xStart;
		xEnd   = rc[i].xEnd;
		y	   = rc[i].y;

		for (x=xStart; x<=xEnd; x++)
		{
			RGBQUAD tmp = pImage->GetPixelColor(x,y);
			ldRed += (tmp.rgbRed-fAveRed)*(tmp.rgbRed-fAveRed);
			ldGreen += (tmp.rgbGreen-fAveGreen)*(tmp.rgbGreen-fAveGreen);
			ldBlue += (tmp.rgbBlue-fAveBlue)*(tmp.rgbBlue-fAveBlue);
		}

		nNum += xEnd-xStart+1;
	}

	// 求掌子面图像中掌子面区域的平均灰度值
	m_fMean2DevRed = fRed = sqrt(ldRed/nNum);
	m_fMean2DevGreen = fGreen = sqrt(ldGreen/nNum);
	m_fMean2DevBlue = fBlue = sqrt(ldBlue/nNum);

	delete pObjRLC;
}

// 掌子面岩体风化程度评价指标
void CFaceCaption::RockWeathingDegree(float& fRed, float& fGreen, float& fBlue)
{
	if (m_fAveRed < 0)
		GreyAveValue(m_fAveRed, m_fAveGreen, m_fAveBlue);
	if (m_fMean2DevRed < 0)
		GreyMean2Deviation(m_fMean2DevRed, m_fMean2DevGreen, m_fMean2DevBlue);

	m_fRockWeathRed = fRed = 0.1*m_fAveRed+0.9*m_fMean2DevRed;
	m_fRockWeathGreen = fGreen = 0.1*m_fAveGreen+0.9*m_fMean2DevGreen;
	m_fRockWeathBlue = fBlue = 0.1*m_fAveBlue+0.9*m_fMean2DevBlue;
}

// 掌子面评价描述
void CFaceCaption::Description()
{
	if (m_fAveJointLen > 0)
	{
		if (m_fAveJointLen < 0.2)
			m_strDesc = "裂隙数量很少,";
		else if (m_fAveJointLen < 0.5)
			m_strDesc = "裂隙数量较少,";
		else if (m_fAveJointLen < 1.0)
			m_strDesc = "裂隙数量一般,";
		else if (m_fAveJointLen < 3.0)
			m_strDesc = "裂隙数量较多,";
		else
			m_strDesc = "裂隙数量很多,";
	}

	if (m_fAveRockSize > 0)
	{
		if (m_fAveRockSize < 0.02)
			m_strDesc += "结构面间距极窄,";
		else if (m_fAveRockSize < 0.06)
			m_strDesc += "结构面间距很窄,";
		else if (m_fAveRockSize < 0.20)
			m_strDesc += "结构面间距窄,";
		else if (m_fAveRockSize < 0.60)
			m_strDesc += "结构面间距中等,";
		else if (m_fAveRockSize < 2.00)
			m_strDesc += "结构面间距宽的,";
		else if (m_fAveRockSize < 6.00)
			m_strDesc += "结构面间距很宽,";
		else
			m_strDesc += "结构面间距极宽,";
	}

	if (m_fRQD > 0)
	{
		if (m_fRQD < 0.25)
			m_strDesc += "岩体质量很差";
		else if (m_fRQD < 0.5)
			m_strDesc += "岩体质量差";
		else if (m_fRQD < 0.75)
			m_strDesc += "岩体质量中等";
		else if (m_fRQD < 0.90)
			m_strDesc += "岩体质量好";
		else
			m_strDesc += "岩体质量很好";
	}
}

// 求多边形面积
/*
double CFaceCaption::area(CArray<POINT2DF, POINT2DF>* pPts)
{
	int nSize = pPts->GetSize();
	if (nSize <= 0)
		return 0;

	double dbArea = 0.0;
	POINT2DF pt1, pt2;
	pt1 = pPts->GetAt(0);
	for (int i=1; i<nSize; i++)
	{
		pt2 = pPts->GetAt(i);
		dbArea += pt1.x*pt2.y-pt1.y*pt2.x;
		pt1 = pt2;
	}
	pt2 = pPts->GetAt(0);
	dbArea += pt1.x*pt2.y-pt1.y*pt2.x;
	dbArea = (dbArea>0)?dbArea:-dbArea;

	return dbArea;
}
*/

// 将所有图形层上的边界线图形转化成一个图像层
CImage* CFaceCaption::graph2image(CTilGraph* pGraphMgr)
{
	int i, j, k;
	// 将图形中的隧道断面尺寸作为外接矩形
	CGraph* pTunArc = pGraphMgr->GetGraphByIdx(GRAPH_LAYER_TUNARC);
	CRect rect = pTunArc->GetUnitByIdx(0)->GetPolyRect();
	rect.InflateRect(10,10);

	// 画出转化的图形层
	int nWidth = rect.Width();
	int nHeight = rect.Height();
	CImage* pImage = new CImage;
	pImage->Create(nWidth, nHeight, 24, CXIMAGE_FORMAT_BMP);
	for (i=0; i<nHeight; i++)
	{
		pImage->DrawLine(0, nWidth-1, i, i, RGB(255,255,255));
	}

	// 获得隧道轮廓线或外接矩形的多边形
	CUnit* pUnit = pTunArc->GetUnitByIdx(0);
	int nNum = pUnit->m_pPointList->GetSize();
	POINT* pPts = new POINT[nNum];
	POINT2DF pt;
	for (i=0; i<nNum; i++)
	{
		pt = pUnit->m_pPointList->GetAt(i); // 加上了相对左上角的偏移
		pPts[i].x = pt.x;
		pPts[i].y = pt.y;
	}

	// 绘制隧道轮廓线或外接矩形的多边形
	for (i=1; i<nNum; i++)
	{
		pImage->DrawLine(pPts[i-1].x, pPts[i].x, nHeight-pPts[i-1].y-1, nHeight-pPts[i].y-1, RGB(0,0,0));
	}
	pImage->DrawLine(pPts[i-1].x, pPts[0].x, nHeight-pPts[i-1].y-1, nHeight-pPts[0].y-1, RGB(0,0,0));

	// 设置隧道轮廓线或外接矩形的多边形区域
	CRgn rgn;
	rgn.CreatePolygonRgn(pPts, nNum, WINDING);

	// 求图中的游程编码
	ObjRunLenCode rlc;
	pImage->RunLengthcode(&rlc);
	
	// 将隧道轮廓线或外接矩形的多边形的外侧区域设置为白色
	// 根据游程编码是否在多边形区域内设置游程编码颜色
	int y;
	POINT tmp;
	int nRLCNum = rlc.GetObjNum();
	for (i=0; i<nRLCNum; i++)
	{
		PRUNLENGTHCODE_MGR rlcObj = rlc.GetObj(i);
		int nRunNum = rlcObj->nRunNum;
		PRUNLENGTHCODE rc = rlcObj->RC;
		for (j=0; j<nRunNum; j++)
		{
			tmp.x = rc[j].xStart;
			tmp.y = nHeight-rc[j].y-1;
			if (!rgn.PtInRegion(tmp))
			{
				pImage->DrawLine(rc[j].xStart, rc[j].xEnd, rc[j].y, rc[j].y, RGB(0,0,0));
			}
		}
	}

	// 绘制节理、裂隙图层中的边界线
	// 在图像层上画出所有图形
	for (i=1; i<=2; i++) // i=1为岩层节理层，i=2为岩层目标层，均绘制出来
	{
		CGraph* pGraph = pGraphMgr->GetGraphByIdx(i);
		for (j=0; j<pGraph->Size(); j++)
		{
			CUnit* pUnit = pGraph->GetUnitByIdx(j);
			
			// 绘出节理、裂隙边界线
			POINT2DF pt1, pt2;
			pt1 = pUnit->GetPoint(0);
			for (k=1; k<pUnit->GetPointNum(); k++)
			{
				pt2 = pUnit->GetPoint(k);
				pImage->DrawLine(pt1.x, pt2.x, nHeight-pt1.y-1, nHeight-pt2.y-1, RGB(0,0,0));
				pt1 = pt2;
			}
			// 如果曲线封闭，则首尾相连
			if (pUnit->IsPolyClose())
			{
				pt2 = pUnit->GetPoint(0);
				pImage->DrawLine(pt1.x, pt2.x, nHeight-pt1.y-1, nHeight-pt2.y-1, RGB(0,0,0));
			}
		}
	}

	delete [] pPts;

	return pImage;
}

// 求掌子面区域的游程编码
ObjRunLenCode* CFaceCaption::facearea(CTilGraph* pGraphMgr)
{
	int i, j, k;
	// 将图形中的隧道断面尺寸作为外接矩形
	CGraph* pTunArc = pGraphMgr->GetGraphByIdx(GRAPH_LAYER_TUNARC);
	CRect rect = pTunArc->GetUnitByIdx(0)->GetPolyRect();
	rect.InflateRect(10,10);
	
	// 画出转化的图形层
	int nWidth = rect.Width();
	int nHeight = rect.Height();
	CImage* pImage = new CImage;
	pImage->Create(nWidth, nHeight, 24, CXIMAGE_FORMAT_BMP);
	for (i=0; i<nHeight; i++)
	{
		pImage->DrawLine(0, nWidth-1, i, i, RGB(255,255,255));
	}
	
	// 获得隧道轮廓线或外接矩形的多边形
	CUnit* pUnit = pTunArc->GetUnitByIdx(0);
	int nNum = pUnit->m_pPointList->GetSize();
	POINT* pPts = new POINT[nNum];
	POINT2DF pt;
	for (i=0; i<nNum; i++)
	{
		pt = pUnit->m_pPointList->GetAt(i); // 加上了相对左上角的偏移
		pPts[i].x = pt.x;
		pPts[i].y = pt.y;
	}
	
	// 绘制隧道轮廓线或外接矩形的多边形
	for (i=1; i<nNum; i++)
	{
		pImage->DrawLine(pPts[i-1].x, pPts[i].x, nHeight-pPts[i-1].y-1, nHeight-pPts[i].y-1, RGB(0,0,0));
	}
	pImage->DrawLine(pPts[i-1].x, pPts[0].x, nHeight-pPts[i-1].y-1, nHeight-pPts[0].y-1, RGB(0,0,0));
	
	// 设置隧道轮廓线或外接矩形的多边形区域
	CRgn rgn;
	rgn.CreatePolygonRgn(pPts, nNum, WINDING);
	
	// 求图中的游程编码
	ObjRunLenCode rlc;
	pImage->RunLengthcode(&rlc);
	
	// 将隧道轮廓线或外接矩形的多边形的外侧区域设置为黑色
	// 根据游程编码是否在多边形区域内设置游程编码颜色
	int y;
	POINT tmp;
	int nRLCNum = rlc.GetObjNum();
	for (i=0; i<nRLCNum; i++)
	{
		PRUNLENGTHCODE_MGR rlcObj = rlc.GetObj(i);
		int nRunNum = rlcObj->nRunNum;
		PRUNLENGTHCODE rc = rlcObj->RC;
		for (j=0; j<nRunNum; j++)
		{
			tmp.x = rc[j].xStart;
			tmp.y = nHeight-rc[j].y-1;
			if (!rgn.PtInRegion(tmp))
			{
				pImage->DrawLine(rc[j].xStart, rc[j].xEnd, rc[j].y, rc[j].y, RGB(0,0,0));
			}
		}
	}

	// 求出掌子面区域的面积
	ObjRunLenCode* pObjRLC = new ObjRunLenCode;
	pImage->ObjAllRLC(pObjRLC, 255);

	delete [] pPts;
	delete pImage;

	return pObjRLC;
}

// 单位面积边界线数量
float CFaceCaption::AveEdgeLines()
{
	if (m_pImage!=NULL)
		delete m_pImage;
	m_pImage = new CImage(*m_pCanvas->GetImage()->GetCurImage());
	m_pImage->UnsharpMask();
	m_pImage->EdgeCanny();
	m_pImage->MorphThining();

	outfaceobjrlc();
	
	// 将掌子面区域以外部分置为黑色
	int nObjNum = m_pObjFace->GetObjNum();
	PPRUNLENGTHCODE_MGR ppMgr = m_pObjFace->GetObjAll();
	for (int k=0; k<nObjNum; k++)
	{
		int xStart,xEnd,y,nRunNum;
		nRunNum = ppMgr[k]->nRunNum;

		for (int m=0; m<nRunNum; m++)
		{
			// 如果为某行的第一个或最后一个游程,则设为黑色
			xStart = ppMgr[k]->RC[m].xStart;
			xEnd = ppMgr[k]->RC[m].xEnd;
			y = ppMgr[k]->RC[m].y;
			for (int x=xStart; x<=xEnd; x++)
				m_pImage->SetPixelColor(x,y,RGB(0,0,0));
		}
	}
	m_pImage->Save("D:\\test.jpg", CXIMAGE_FORMAT_JPG);

	// 求出边界线数量
	ObjRunLenCode objRLC;
	m_pImage->ObjAllRLC(&objRLC);
	int nLines = objRLC.GetObjNum();

	return nLines/m_fFaceArea*10000;
}

// 掌子面上单位面积的岩体破碎程度评价指标
float CFaceCaption::AveRockCracks()
{
	if (m_pImage!=NULL)
		delete m_pImage;
	m_pImage = new CImage(*m_pCanvas->GetImage()->GetCurImage());
	m_pImage->UnsharpMask();
	m_pImage->EdgeCanny();
	m_pImage->MorphThining();

	outfaceobjrlc();
	
	// 将掌子面区域以外部分置为白色,与目标边界颜色相同
	int nObjNum = m_pObjFace->GetObjNum();
	PPRUNLENGTHCODE_MGR ppMgr = m_pObjFace->GetObjAll();
	for (int k=0; k<nObjNum; k++)
	{
		int xStart,xEnd,y,nRunNum;
		nRunNum = ppMgr[k]->nRunNum;

		for (int m=0; m<nRunNum; m++)
		{
			// 如果为某行的第一个或最后一个游程,则设为白色
			xStart = ppMgr[k]->RC[m].xStart;
			xEnd = ppMgr[k]->RC[m].xEnd;
			y = ppMgr[k]->RC[m].y;
			for (int x=xStart; x<=xEnd; x++)
				m_pImage->SetPixelColor(x,y,RGB(255,255,255));
		}
	}

	// 按每间距fLineDist求节理的平均间隔
	BOOL bMark;
	int nDividLines=0;
	int i,j,nStart, nEnd;
	long lTotalSize=0, lSize=0;
	int nWidth = m_pImage->GetWidth();
	int nHeight = m_pImage->GetHeight();

	// 水平方向求
	RGBQUAD rgb1,rgb2;
	for (i=0; i<nHeight; i++)
	{
		bMark = FALSE;
		rgb1 = m_pImage->GetPixelColor(0,i);
		for (j=1; j<nWidth-1; j++)
		{
			rgb2 = m_pImage->GetPixelColor(j,i);
			if (rgb1.rgbRed==255 && rgb2.rgbRed==0)
			{
				nStart = j;
			}
			else if (rgb1.rgbRed==0 && rgb2.rgbRed==255)
			{
				// 如果前一个像素值为255，而当前像素值为0
				nEnd = j-1;
				nDividLines ++;
			}
			rgb1 = rgb2;
		}
	}

	// 求垂直方向
	for (i=0; i<nWidth; i++)
	{
		bMark = FALSE;
		rgb1 = m_pImage->GetPixelColor(i,0);
		for (j=1; j<nHeight-1; j++)
		{
			rgb2 = m_pImage->GetPixelColor(i,j);
			if (rgb1.rgbRed==255 && rgb2.rgbRed==0)
			{
				nStart = j;
			}
			else if (rgb1.rgbRed==0 && rgb2.rgbRed==255)
			{
				// 如果前一个像素值为255，而当前像素值为0
				nEnd = j-1;
				nDividLines ++;
			}
			rgb1 = rgb2;
		}
	}

	// 求掌子面面积,此处为平方厘米
	return nDividLines*1.0/m_fFaceArea*10000;
}

ObjRunLenCode* CFaceCaption::outfaceobjrlc()
{
	// 
	int nWidth = m_pCanvas->GetWidth();
	int nHeight = m_pCanvas->GetHeight();

	// 创建一幅图像，其所有像素灰度值均为白色
	CImage image;
	image.Create(nWidth, nHeight, 24, CXIMAGE_FORMAT_BMP);
	for (int i=0; i<nWidth; i++)
	{
		for (int j=0; j<nHeight; j++)
		{
			image.SetPixelColor(i,j,RGB(255,255,255));
		}
	}

	// 取得隧道开挖轮廓图形层
	CGraph* pGraph = m_pCanvas->GetGraph()->GetGraphByIdx(GRAPH_LAYER_TUNARC);
	if (pGraph->Size() == 0)
		return NULL;

	CUnit* pTunArc = pGraph->GetUnitByIdx(0);
	POINT2DF pt1, pt2;
	pt1 = pTunArc->GetPoint(0);
	for (i=1; i<pTunArc->GetPointNum(); i++)
	{
		pt2 = pTunArc->GetPoint(i);
		image.DrawLine(pt1.x,pt2.x,nHeight-pt1.y,nHeight-pt2.y,RGB(0,0,0));
		pt1 = pt2;
	}

	pt2 = pTunArc->GetPoint(0);
	image.DrawLine(pt1.x, pt2.x, nHeight-pt1.y, nHeight-pt2.y, RGB(0,0,0));

	// 将掌子面区域的边界线设置为黑色，边界线以外的区域设置为白色
	// 掌子面以外的区域设置为灰色
	ObjRunLenCode runLenCode;
	image.RunLengthcode(&runLenCode, 255);
	int nObjNum = runLenCode.GetObjNum();
	PPRUNLENGTHCODE_MGR ppMgr = runLenCode.GetObjAll();
	for (int k=0; k<nObjNum; k++)
	{
		int xStart,xEnd,y,nRunNum;
		nRunNum = ppMgr[k]->nRunNum;

		for (int m=0; m<nRunNum; m++)
		{
			// 如果是某行的第一个或最后一个游程,
			// 则该游程位于掌子面以外，则设为黑色
			if (m==0 || m==nRunNum-1)
			{
				xStart = ppMgr[k]->RC[m].xStart;
				xEnd = ppMgr[k]->RC[m].xEnd;
				y = ppMgr[k]->RC[m].y;
				for (int x=xStart; x<=xEnd; x++)
				{
					image.SetPixelColor(x,y,RGB(0,0,0));
				}
			}
		}
	}

//	image.Save("D:\\test0.jpg",CXIMAGE_FORMAT_JPG);

	// 求出掌子面区域的面积
	if (m_pObjFace)
		delete m_pObjFace;
	m_pObjFace = new ObjRunLenCode;
	image.ObjAllRLC(m_pObjFace, 0);
	m_fFaceArea = nWidth*nHeight-m_pObjFace->ObjArea(0);

	return m_pObjFace;
}

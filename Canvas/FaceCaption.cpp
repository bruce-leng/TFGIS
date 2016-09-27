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

// ��CTilCanvas������п�������
void CFaceCaption::CopyFrom(CTilCanvas* pCanvas)
{
	if (m_pCanvas)
		delete m_pCanvas;
	m_pCanvas = new CTilCanvas(*pCanvas);

	CGraph* pTunArc = pCanvas->GetGraph()->GetGraphByIdx(GRAPH_LAYER_TUNARC);
	// ���������������߲����ڣ���ͼ��߽統�������ߴ���
	if (pTunArc->Size() == 0)
	{
		// ����ͼ���Ĭ��������
		m_pCanvas->SetDefaultTunArc();

		TRACE("������������߲�����!����ͼƬ����Ӿ���Ϊ������!");
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

// ��λ�����϶�ܳ���(m/m2)
float CFaceCaption::JointLengthofUnitArea()
{
	// �������濪�������߻�������ͼƬ��Ӿ��ε����
	long lArea;
	int i, j, k;

	// ��ÿ��������߻�������ͼƬ��Ӿ��ε�ͼԪָ��
	CTilGraph* pGraphMgr = m_pCanvas->GetGraph();
	CGraph* pTunArc = pGraphMgr->GetGraphByIdx(GRAPH_LAYER_TUNARC);
	CUnit* pUnit = pTunArc->GetUnitByIdx(0);

	// ������
	int nSize = pUnit->m_pPointList->GetSize();
	POINT2DF* pPts = new POINT2DF[nSize];
	for (i=0; i<nSize; i++)
		pPts[i] = pUnit->m_pPointList->GetAt(i);
	lArea = get_poly_area(pPts, nSize);
	delete [] pPts;

	// ������ܳ���
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

	// ��λ���������
	m_fAveJointLen = float(dbJointLen*100/lArea);

	return m_fAveJointLen;
}

// ƽ������ߴ�(m)
// float fLineDist: ���߼�࣬��λΪ����
float CFaceCaption::AveRockSize(float fLineDist)
{
	CTilGraph* pGraphMgr = m_pCanvas->GetGraph();
	CGraph* pTunArc = pGraphMgr->GetGraphByIdx(GRAPH_LAYER_TUNARC);
	ASSERT(pTunArc->Size()!=0);

	// �������϶��ƽ�����
	// ���ȣ���ͼ�β�ת����ͼ���
	CImage* pImage = graph2image(pGraphMgr);

	// ��ÿ���fLineDist������ƽ�����
	BOOL bMark;
	int i,j,nStart, nEnd, nNum = 0;
	long lTotalSize=0;
	int nWidth = pImage->GetWidth();
	int nHeight = pImage->GetHeight();
	// ˮƽ������
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
				// ���ǰһ������ֵΪ255������ǰ����ֵΪ0
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

	// ��ֱ����
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
				// ���ǰһ������ֵΪ255������ǰ����ֵΪ0
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

	// ƽ������ߴ�
	m_fAveRockSize = float(lTotalSize*1.0/nNum/100);
	delete pImage;

	return m_fAveRockSize;
}

// ������Ƶ��-Ϊƽ�����嵹��
float CFaceCaption::DiscontinueFreqency(float fLineDist)
{
	m_fAveRockSize = 1/AveRockSize(fLineDist);
	return m_fAveRockSize;
}

// ȡ���������ϵ�RQDֵ
// float fLineDist: ���߼��, ��λΪ����
// float fMinDist: ��С����, ��λΪ����
float CFaceCaption::RQD(float fLineDist, float fMinDist)
{
	CTilGraph* pGraphMgr = m_pCanvas->GetGraph();
	CGraph* pTunArc = pGraphMgr->GetGraphByIdx(GRAPH_LAYER_TUNARC);
	ASSERT(pTunArc->Size()!=0);

	// �������϶��ƽ�����
	// ���ȣ���ͼ�β�ת����ͼ���
	CImage* pImage = graph2image(pGraphMgr);

	// ��ÿ���fLineDist������ƽ�����
	BOOL bMark;
	int i,j,nStart0, nStart, nEnd, nDist;
	long lTotalSize=0, lSize=0;
	int nWidth = pImage->GetWidth();
	int nHeight = pImage->GetHeight();

	// ˮƽ������
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
				// ���ǰһ������ֵΪ255������ǰ����ֵΪ0
				nEnd = j-1;
				nDist = nEnd-nStart+1;
				if (nDist>=fMinDist)
					lSize += nDist;
			}
			rgb1 = rgb2;
		}

		// ���bMark==true,˵����i����Ŀ����ɫ
		if (bMark)
			lTotalSize += nEnd-nStart0+1;
	}

	// ��ֱ����
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
				// ���ǰһ������ֵΪ255������ǰ����ֵΪ0
				nEnd = j-1;
				nDist = nEnd-nStart+1;
				if (nDist>=fMinDist)
					lSize += nDist;
			}
			rgb1 = rgb2;
		}

		// ���bMark==true,˵����i����Ŀ����ɫ
		if (bMark)
			lTotalSize += nEnd-nStart0+1;
	}

	// ƽ��RQDֵ
	m_fRQD = float(lSize*1.0/lTotalSize);
	delete pImage;

	return m_fRQD;
}

// ��λ�����������
float CFaceCaption::DividLinesofUnitArea(float fLineDist)
{
	CTilGraph* pGraphMgr = m_pCanvas->GetGraph();
	CGraph* pTunArc = pGraphMgr->GetGraphByIdx(GRAPH_LAYER_TUNARC);
	ASSERT(pTunArc->Size()!=0);

	// �������϶��ƽ�����
	// ���ȣ���ͼ�β�ת����ͼ���
	CImage* pImage = graph2image(pGraphMgr);

	// ��ÿ���fLineDist������ƽ�����
	BOOL bMark;
	int nDividLines=0;
	int i,j,nStart0, nStart, nEnd;
	long lTotalSize=0, lSize=0;
	int nWidth = pImage->GetWidth();
	int nHeight = pImage->GetHeight();

	// ˮƽ������
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
				// ���ǰһ������ֵΪ255������ǰ����ֵΪ0
				nEnd = j-1;
				nDividLines ++;
			}
			rgb1 = rgb2;
		}

//		// ���bMark==true,˵����i����Ŀ����ɫ
//		if (bMark)
//			lTotalSize += nEnd-nStart0+1;
	}

	// ��ֱ����
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
				// ���ǰһ������ֵΪ255������ǰ����ֵΪ0
				nEnd = j-1;
				nDividLines ++;
			}
			rgb1 = rgb2;
		}

//		// ���bMark==true,˵����i����Ŀ����ɫ
//		if (bMark)
//			lTotalSize += nEnd-nStart0+1;
	}

	delete pImage;

	// �����������,�˴�Ϊƽ������
	ObjRunLenCode* pObjRLC = facearea(pGraphMgr);
	float fFaceArea = pObjRLC->ObjArea(0);
	delete pObjRLC;

	m_fDividLines = nDividLines*1.0/fFaceArea*10000;
	return m_fDividLines;
}

// ��λ����߽�������
float CFaceCaption::EdgeLinesofUnitArea()
{
	int i,j;
	int nEdgeLines = 0;
	float fFaceArea;

	// ͳ�Ʊ߽�������
	CTilGraph* pGraphMgr = m_pCanvas->GetGraph();
	for (i=0; i<pGraphMgr->Size(); i++)
	{
		CGraph* pGraph = pGraphMgr->GetGraphByIdx(i);
		for (j=0; j<pGraph->Size(); j++)
		{
			CUnit* pUnit = pGraph->GetUnitByIdx(j);
			// ������������
			if (i!=GRAPH_LAYER_TUNARC
				&& (pUnit->IsKindOf(RUNTIME_CLASS(CPolygon))
				|| pUnit->IsPolyClose()))
				nEdgeLines ++;
		}
	}

	// �����������,�˴�Ϊƽ������
	ObjRunLenCode* pObjRLC = facearea(pGraphMgr);
	fFaceArea = pObjRLC->ObjArea(0);
	delete pObjRLC;

	m_fEdgeLines = nEdgeLines/fFaceArea*10000;
	return m_fEdgeLines;
}

// ��������������̶�����ָ��
float CFaceCaption::RockCrackDegree()
{
	if (m_fDividLines < 0)
		m_fDividLines = DividLinesofUnitArea(10);
	if (m_fEdgeLines < 0)
		m_fEdgeLines = EdgeLinesofUnitArea();

	m_fRockCrack = m_fDividLines+m_fEdgeLines;
	return m_fRockCrack;
}

// �������ϵ�ƽ���Ҷ�ֵ
RGBQUAD CFaceCaption::GreyAveValue(float& fRed, float& fGreen, float& fBlue)
{
	int i;
	long nNum=0;
	RGBQUAD rgb;
	long double ldRed = 0, ldGreen = 0, ldBlue = 0;

	// ��������������γ̱���
	CTilGraph* pGraphMgr = m_pCanvas->GetGraph();

	ObjRunLenCode* pObjRLC = facearea(pGraphMgr);

	// ȡ��������ͼ������
	CImage* pImage = m_pCanvas->GetImage()->GetCurImage();
	int nWidth = pImage->GetWidth();
	int nHeight = pImage->GetHeight();

	// ���������������ص����ֵ����Сֵ
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

	// ����ʵ��������������Ϲ��ղ���������ֲ�������������
	// ���ⲿ�����ؽ�������
	cMaxR -= (cMaxR-cMinR)*0.2;
	cMaxG -= (cMaxG-cMinG)*0.2;
	cMaxB -= (cMaxB-cMinB)*0.2;
	cMinR += (cMaxR-cMinR)*0.2;
	cMinG += (cMaxG-cMinG)*0.2;
	cMinB += (cMaxB-cMinB)*0.2;

	// ͳ��������ͼ��������������ĻҶ�ֵ��
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

	// ��������ͼ���������������ƽ���Ҷ�ֵ
	m_fAveRed = fRed = ldRed/nNum;
	m_fAveGreen = fGreen = ldGreen/nNum;
	m_fAveBlue = fBlue = ldBlue/nNum;

	rgb.rgbRed   = fRed;
	rgb.rgbGreen = fGreen;
	rgb.rgbBlue  = fBlue;

	delete pObjRLC;

	return rgb;
}

// �������ϵĻҶȾ�����
void CFaceCaption::GreyMean2Deviation(float& fRed, float& fGreen, float& fBlue)
{
	int i;
	long nNum = 0;

	// ��������ĻҶȾ�ֵ
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

	// �������������������ػҶ�ֵ��ҶȾ�ֵ֮���ƽ����
	CTilGraph* pGraphMgr = m_pCanvas->GetGraph();
	ObjRunLenCode* pObjRLC = facearea(pGraphMgr);

	// ȡ��������ͼ������
	CImage* pImage = m_pCanvas->GetImage()->GetCurImage();
	int nWidth = pImage->GetWidth();
	int nHeight = pImage->GetHeight();

	// ͳ��������ͼ��������������ĻҶ�ֵ��
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

	// ��������ͼ���������������ƽ���Ҷ�ֵ
	m_fMean2DevRed = fRed = sqrt(ldRed/nNum);
	m_fMean2DevGreen = fGreen = sqrt(ldGreen/nNum);
	m_fMean2DevBlue = fBlue = sqrt(ldBlue/nNum);

	delete pObjRLC;
}

// ����������绯�̶�����ָ��
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

// ��������������
void CFaceCaption::Description()
{
	if (m_fAveJointLen > 0)
	{
		if (m_fAveJointLen < 0.2)
			m_strDesc = "��϶��������,";
		else if (m_fAveJointLen < 0.5)
			m_strDesc = "��϶��������,";
		else if (m_fAveJointLen < 1.0)
			m_strDesc = "��϶����һ��,";
		else if (m_fAveJointLen < 3.0)
			m_strDesc = "��϶�����϶�,";
		else
			m_strDesc = "��϶�����ܶ�,";
	}

	if (m_fAveRockSize > 0)
	{
		if (m_fAveRockSize < 0.02)
			m_strDesc += "�ṹ���༫խ,";
		else if (m_fAveRockSize < 0.06)
			m_strDesc += "�ṹ�����խ,";
		else if (m_fAveRockSize < 0.20)
			m_strDesc += "�ṹ����խ,";
		else if (m_fAveRockSize < 0.60)
			m_strDesc += "�ṹ�����е�,";
		else if (m_fAveRockSize < 2.00)
			m_strDesc += "�ṹ������,";
		else if (m_fAveRockSize < 6.00)
			m_strDesc += "�ṹ����ܿ�,";
		else
			m_strDesc += "�ṹ���༫��,";
	}

	if (m_fRQD > 0)
	{
		if (m_fRQD < 0.25)
			m_strDesc += "���������ܲ�";
		else if (m_fRQD < 0.5)
			m_strDesc += "����������";
		else if (m_fRQD < 0.75)
			m_strDesc += "���������е�";
		else if (m_fRQD < 0.90)
			m_strDesc += "����������";
		else
			m_strDesc += "���������ܺ�";
	}
}

// ���������
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

// ������ͼ�β��ϵı߽���ͼ��ת����һ��ͼ���
CImage* CFaceCaption::graph2image(CTilGraph* pGraphMgr)
{
	int i, j, k;
	// ��ͼ���е��������ߴ���Ϊ��Ӿ���
	CGraph* pTunArc = pGraphMgr->GetGraphByIdx(GRAPH_LAYER_TUNARC);
	CRect rect = pTunArc->GetUnitByIdx(0)->GetPolyRect();
	rect.InflateRect(10,10);

	// ����ת����ͼ�β�
	int nWidth = rect.Width();
	int nHeight = rect.Height();
	CImage* pImage = new CImage;
	pImage->Create(nWidth, nHeight, 24, CXIMAGE_FORMAT_BMP);
	for (i=0; i<nHeight; i++)
	{
		pImage->DrawLine(0, nWidth-1, i, i, RGB(255,255,255));
	}

	// �����������߻���Ӿ��εĶ����
	CUnit* pUnit = pTunArc->GetUnitByIdx(0);
	int nNum = pUnit->m_pPointList->GetSize();
	POINT* pPts = new POINT[nNum];
	POINT2DF pt;
	for (i=0; i<nNum; i++)
	{
		pt = pUnit->m_pPointList->GetAt(i); // ������������Ͻǵ�ƫ��
		pPts[i].x = pt.x;
		pPts[i].y = pt.y;
	}

	// ������������߻���Ӿ��εĶ����
	for (i=1; i<nNum; i++)
	{
		pImage->DrawLine(pPts[i-1].x, pPts[i].x, nHeight-pPts[i-1].y-1, nHeight-pPts[i].y-1, RGB(0,0,0));
	}
	pImage->DrawLine(pPts[i-1].x, pPts[0].x, nHeight-pPts[i-1].y-1, nHeight-pPts[0].y-1, RGB(0,0,0));

	// ������������߻���Ӿ��εĶ��������
	CRgn rgn;
	rgn.CreatePolygonRgn(pPts, nNum, WINDING);

	// ��ͼ�е��γ̱���
	ObjRunLenCode rlc;
	pImage->RunLengthcode(&rlc);
	
	// ����������߻���Ӿ��εĶ���ε������������Ϊ��ɫ
	// �����γ̱����Ƿ��ڶ���������������γ̱�����ɫ
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

	// ���ƽ�����϶ͼ���еı߽���
	// ��ͼ����ϻ�������ͼ��
	for (i=1; i<=2; i++) // i=1Ϊ�Ҳ����㣬i=2Ϊ�Ҳ�Ŀ��㣬�����Ƴ���
	{
		CGraph* pGraph = pGraphMgr->GetGraphByIdx(i);
		for (j=0; j<pGraph->Size(); j++)
		{
			CUnit* pUnit = pGraph->GetUnitByIdx(j);
			
			// ���������϶�߽���
			POINT2DF pt1, pt2;
			pt1 = pUnit->GetPoint(0);
			for (k=1; k<pUnit->GetPointNum(); k++)
			{
				pt2 = pUnit->GetPoint(k);
				pImage->DrawLine(pt1.x, pt2.x, nHeight-pt1.y-1, nHeight-pt2.y-1, RGB(0,0,0));
				pt1 = pt2;
			}
			// ������߷�գ�����β����
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

// ��������������γ̱���
ObjRunLenCode* CFaceCaption::facearea(CTilGraph* pGraphMgr)
{
	int i, j, k;
	// ��ͼ���е��������ߴ���Ϊ��Ӿ���
	CGraph* pTunArc = pGraphMgr->GetGraphByIdx(GRAPH_LAYER_TUNARC);
	CRect rect = pTunArc->GetUnitByIdx(0)->GetPolyRect();
	rect.InflateRect(10,10);
	
	// ����ת����ͼ�β�
	int nWidth = rect.Width();
	int nHeight = rect.Height();
	CImage* pImage = new CImage;
	pImage->Create(nWidth, nHeight, 24, CXIMAGE_FORMAT_BMP);
	for (i=0; i<nHeight; i++)
	{
		pImage->DrawLine(0, nWidth-1, i, i, RGB(255,255,255));
	}
	
	// �����������߻���Ӿ��εĶ����
	CUnit* pUnit = pTunArc->GetUnitByIdx(0);
	int nNum = pUnit->m_pPointList->GetSize();
	POINT* pPts = new POINT[nNum];
	POINT2DF pt;
	for (i=0; i<nNum; i++)
	{
		pt = pUnit->m_pPointList->GetAt(i); // ������������Ͻǵ�ƫ��
		pPts[i].x = pt.x;
		pPts[i].y = pt.y;
	}
	
	// ������������߻���Ӿ��εĶ����
	for (i=1; i<nNum; i++)
	{
		pImage->DrawLine(pPts[i-1].x, pPts[i].x, nHeight-pPts[i-1].y-1, nHeight-pPts[i].y-1, RGB(0,0,0));
	}
	pImage->DrawLine(pPts[i-1].x, pPts[0].x, nHeight-pPts[i-1].y-1, nHeight-pPts[0].y-1, RGB(0,0,0));
	
	// ������������߻���Ӿ��εĶ��������
	CRgn rgn;
	rgn.CreatePolygonRgn(pPts, nNum, WINDING);
	
	// ��ͼ�е��γ̱���
	ObjRunLenCode rlc;
	pImage->RunLengthcode(&rlc);
	
	// ����������߻���Ӿ��εĶ���ε������������Ϊ��ɫ
	// �����γ̱����Ƿ��ڶ���������������γ̱�����ɫ
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

	// �����������������
	ObjRunLenCode* pObjRLC = new ObjRunLenCode;
	pImage->ObjAllRLC(pObjRLC, 255);

	delete [] pPts;
	delete pImage;

	return pObjRLC;
}

// ��λ����߽�������
float CFaceCaption::AveEdgeLines()
{
	if (m_pImage!=NULL)
		delete m_pImage;
	m_pImage = new CImage(*m_pCanvas->GetImage()->GetCurImage());
	m_pImage->UnsharpMask();
	m_pImage->EdgeCanny();
	m_pImage->MorphThining();

	outfaceobjrlc();
	
	// ���������������ⲿ����Ϊ��ɫ
	int nObjNum = m_pObjFace->GetObjNum();
	PPRUNLENGTHCODE_MGR ppMgr = m_pObjFace->GetObjAll();
	for (int k=0; k<nObjNum; k++)
	{
		int xStart,xEnd,y,nRunNum;
		nRunNum = ppMgr[k]->nRunNum;

		for (int m=0; m<nRunNum; m++)
		{
			// ���Ϊĳ�еĵ�һ�������һ���γ�,����Ϊ��ɫ
			xStart = ppMgr[k]->RC[m].xStart;
			xEnd = ppMgr[k]->RC[m].xEnd;
			y = ppMgr[k]->RC[m].y;
			for (int x=xStart; x<=xEnd; x++)
				m_pImage->SetPixelColor(x,y,RGB(0,0,0));
		}
	}
	m_pImage->Save("D:\\test.jpg", CXIMAGE_FORMAT_JPG);

	// ����߽�������
	ObjRunLenCode objRLC;
	m_pImage->ObjAllRLC(&objRLC);
	int nLines = objRLC.GetObjNum();

	return nLines/m_fFaceArea*10000;
}

// �������ϵ�λ�������������̶�����ָ��
float CFaceCaption::AveRockCracks()
{
	if (m_pImage!=NULL)
		delete m_pImage;
	m_pImage = new CImage(*m_pCanvas->GetImage()->GetCurImage());
	m_pImage->UnsharpMask();
	m_pImage->EdgeCanny();
	m_pImage->MorphThining();

	outfaceobjrlc();
	
	// ���������������ⲿ����Ϊ��ɫ,��Ŀ��߽���ɫ��ͬ
	int nObjNum = m_pObjFace->GetObjNum();
	PPRUNLENGTHCODE_MGR ppMgr = m_pObjFace->GetObjAll();
	for (int k=0; k<nObjNum; k++)
	{
		int xStart,xEnd,y,nRunNum;
		nRunNum = ppMgr[k]->nRunNum;

		for (int m=0; m<nRunNum; m++)
		{
			// ���Ϊĳ�еĵ�һ�������һ���γ�,����Ϊ��ɫ
			xStart = ppMgr[k]->RC[m].xStart;
			xEnd = ppMgr[k]->RC[m].xEnd;
			y = ppMgr[k]->RC[m].y;
			for (int x=xStart; x<=xEnd; x++)
				m_pImage->SetPixelColor(x,y,RGB(255,255,255));
		}
	}

	// ��ÿ���fLineDist������ƽ�����
	BOOL bMark;
	int nDividLines=0;
	int i,j,nStart, nEnd;
	long lTotalSize=0, lSize=0;
	int nWidth = m_pImage->GetWidth();
	int nHeight = m_pImage->GetHeight();

	// ˮƽ������
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
				// ���ǰһ������ֵΪ255������ǰ����ֵΪ0
				nEnd = j-1;
				nDividLines ++;
			}
			rgb1 = rgb2;
		}
	}

	// ��ֱ����
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
				// ���ǰһ������ֵΪ255������ǰ����ֵΪ0
				nEnd = j-1;
				nDividLines ++;
			}
			rgb1 = rgb2;
		}
	}

	// �����������,�˴�Ϊƽ������
	return nDividLines*1.0/m_fFaceArea*10000;
}

ObjRunLenCode* CFaceCaption::outfaceobjrlc()
{
	// 
	int nWidth = m_pCanvas->GetWidth();
	int nHeight = m_pCanvas->GetHeight();

	// ����һ��ͼ�����������ػҶ�ֵ��Ϊ��ɫ
	CImage image;
	image.Create(nWidth, nHeight, 24, CXIMAGE_FORMAT_BMP);
	for (int i=0; i<nWidth; i++)
	{
		for (int j=0; j<nHeight; j++)
		{
			image.SetPixelColor(i,j,RGB(255,255,255));
		}
	}

	// ȡ�������������ͼ�β�
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

	// ������������ı߽�������Ϊ��ɫ���߽����������������Ϊ��ɫ
	// �������������������Ϊ��ɫ
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
			// �����ĳ�еĵ�һ�������һ���γ�,
			// ����γ�λ�����������⣬����Ϊ��ɫ
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

	// �����������������
	if (m_pObjFace)
		delete m_pObjFace;
	m_pObjFace = new ObjRunLenCode;
	image.ObjAllRLC(m_pObjFace, 0);
	m_fFaceArea = nWidth*nHeight-m_pObjFace->ObjArea(0);

	return m_pObjFace;
}

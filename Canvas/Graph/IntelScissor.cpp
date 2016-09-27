// IntelScissor.cpp: implementation of the CIntelScissor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
// #include "tilsys.h"
// #include "TilSysView.h"
#include "IntelScissor.h"
#include <float.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////////
#ifndef CLAMP
#define CLAMP(x,l,u) ((x)<(l)?(l):((x)>(u)?(u):(x)))
#endif

#define  EXTEND_BY         0.2 /* proportion to expand cost map by */
#define  FIXED             5   /* additional fixed size to expand cost map */
#define  MAX_GRADIENT      179.606  /* == sqrt (127^2 + 127^2) */
#define  MIN_GRADIENT      63  /* gradients < this are directionless */
#ifndef PI
	#define  PI 3.14159265
#endif
#define  PI_2 1.570796325
#define  SQRT2 1.4142135623730950488016887242097
#define	 SQRT2_2					 SQRT2/2
//#define  GRADIENT_SEARCH   8  /* how far to look when snapping to an edge */
#define  OMEGA_D           0.2
#define  OMEGA_G           0.8
#define  PIXEL_COST(x)     ((x) >> 8)
/* sentinel to mark seed point in ?cost? map */
#define  SEED_POINT        9
#define  PIXEL_DIR(x)      ((x) & 0x000000ff)
/*  where to move on a given link direction  */
static int move[8][2] = /*8����*/
{
  {  1,  0 },
  {  0,  1 },
  { -1,  1 },
  {  1,  1 },
  { -1,  0 },
  {  0, -1 },
  {  1, -1 },
  { -1, -1 },
};
/* IE:
 * '---+---+---`
 * | 7 | 5 | 6 |
 * +---+---+---+
 * | 4 |   | 0 |
 * +---+---+---+
 * | 2 | 1 | 3 |
 * `---+---+---'
 */

//��ǰ��İ������������
static	int nN[8][2] = {{1,0}, {0,1}, {-1, 0},			//	|5|1|4|
					{0,-1},			 {1, -1},			//	|2|X|0|
					{-1, -1}, {-1, 1}, {1, 1}};			//	|6|3|7|

typedef struct __POINTVALUE
{
	int nX, nY;	//�������
	int nValue;	//����ۻ���ֵ
}POINTVALUE, *PPOINTVALUE;

typedef struct __POINTCAPTION
{
	BOOL bFlag;	//���Ƿ�ʹ�õı��
	int	 nCost;	//��ǰ�㵽���ӵ���ۻ�����
	POINT ptDir;	//��ǰ��ָ�����һ��������λ������
}POINTCAPTION, *PPOINTCAPTION;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CIntelScissor::CIntelScissor()
{
	int i, j;
	m_pPtList = NULL;
	m_bDrag = FALSE;
	m_pSubMap = NULL;	//׷��2005.10.29
	//{׷��2005.10.31
	m_pMark = NULL;
	//{׷��2005.10.31

	int nRadius = GRADIENT_SEARCH >> 1;

	for(i = 0; i < 256; i ++)
	{
		m_nDgnlWeights[i] = i * SQRT2;
	}

	/*  compute the distance weights  */
	for (i = 0; i < GRADIENT_SEARCH; i++)
	{
		for (j = 0; j < GRADIENT_SEARCH; j++)
		{
			m_fDistWeights[i * GRADIENT_SEARCH + j] =
				1.0 / (1 + sqrt (pow((i - nRadius), 2) + pow((j - nRadius), 2)));
		}
	}

	m_pSrcImg = m_pDstImg = NULL;
}

// ���ü����������ܼ��������ı���ͼ��
bool CIntelScissor::SetImage(CImage* pImage)
{
	if (pImage==NULL || !pImage->IsValid())
		return false;

	// �õ�ԭʼͼ��
	if (!m_pSrcImg)
		m_pSrcImg = new CImage(*pImage);
//	m_pSrcImg->Copy(*pImage);

	long double dbRatio;
	m_pSrcImg->GetZoomFactor(dbRatio, dbRatio);

	// �õ����ź��ͼ��
	if (!m_pDstImg)	 m_pDstImg = new CImage();
	m_pSrcImg->Resample(pImage->GetWidth()*dbRatio, pImage->GetHeight()*dbRatio, 2, m_pDstImg);

	return TRUE;
}

CIntelScissor::~CIntelScissor()
{
	if (m_pDstImg)	delete m_pDstImg;
	if (m_pSrcImg)	delete m_pSrcImg;

	if (m_pSubMap)	delete [] (BYTE*)m_pSubMap;
	if (m_pMark)	delete [] (BYTE*)m_pMark;
	ReleaseArray(&m_pPtList);
}

// �����ݶȷ��Ⱥ��ݶȷ���, ���صľ����е���������Ϊint��
// int�͵���16λ�洢�ݶȷ���,��16λ�洢�ݶȷ���
PMATRIX CIntelScissor::GetGradMap()
{
	CImage* pBlur = new CImage(*m_pDstImg);

	//��ȡƽ��������
	long blur_32[]={1,1,1,1,24,1,1,1,1};
	pBlur->Filter(blur_32,3,32,0);

	// ��ƽ�����ͨ�����ȡ��ˮƽ������ݶ�ֵ
	CImage* pHorz = new CImage(*pBlur);
	long horz_deriv[]={1,0,-1,2,0,-2,1,0,-1};
	pHorz->Filter(horz_deriv,3,4,0);
	BYTE* pHorzB = pHorz->GetBits();

	// ��ƽ�����ͨ�����ȡ�ô�ֱ������ݶ�ֵ
	CImage* pVert = new CImage(*pBlur);
	long vert_deriv[]={1,2,1,0,0,0,-1,-2,-1};
	pVert->Filter(vert_deriv,3,4,0);
	BYTE* pVertB = pVert->GetBits();

	int i, j;
	int nWidth = m_pDstImg->GetWidth();
	int nHeight = m_pDstImg->GetHeight();
	PMATRIX pMaxGrad = (PMATRIX)new int[(3+nWidth*nHeight)*sizeof(int)];
	int* pElem = (int*)pMaxGrad->elem;

	float fMaxH, fMaxV, fGrad;
	float fMax = FLT_MIN;
	float fMin = FLT_MAX;
	int nEffWidth = m_pDstImg->GetEffWidth();
	for(i = 0; i < nHeight; i ++)
	{
		for(j = 0; j < nWidth; j ++)
		{
			if(i == 0 || j == 0 || i == nHeight - 1 || j == nWidth - 1)
			{
				pElem[i*nWidth+j] = (0 << 16) + 255;
				continue;
			}
			//TRACE("ppHorzR[%d][%d] = %f,", i, j, ppHorzR[i][j]);
			// ���ص��ˮƽ����ݶȷ���
			fMaxH = abs(pHorzB[i*nEffWidth+j*3])-128;
			if(abs(abs(pHorzB[i*nEffWidth+j*3+1])-128) > abs(fMaxH))
				fMaxH = abs(pHorzB[i*nEffWidth+j*3+1]) - 128;
			if(abs(abs(pHorzB[i*nEffWidth+j*3+2])-128) > abs(fMaxH))
				fMaxH = abs(pHorzB[i*nEffWidth+j*3+2])-128;

			// ���ص�Ĵ�ֱ����ݶȷ���
			fMaxV = abs(pVertB[i*nEffWidth+j*3])-128;
			if(abs(abs(pVertB[i*nEffWidth+j*3+1])-128) > abs(fMaxV))
				fMaxV = abs(pVertB[i*nEffWidth+j*3+1]) - 128;
			if(abs(abs(pVertB[i*nEffWidth+j*3+2])-128) > abs(fMaxV))
				fMaxV = abs(pVertB[i*nEffWidth+j*3+2])-128;

			//TRACE("fMaxH = %f, fMaxV = %f\n", fMaxH, fMaxV);
			fGrad = sqrt(fMaxH * fMaxH + fMaxV * fMaxV);
			pElem[i*nWidth+j] = (int)((fGrad) * 255 / MAX_GRADIENT) << 16;
			if(fGrad > MIN_GRADIENT)
			{
				float fDir;
				if(!fMaxH)
					fDir = (fMaxV > 0) ? PI_2 : -PI_2;
				else
					fDir = atan(fMaxV / fMaxH);
				pElem[i*pMaxGrad->nWidth+j] += (int)(254 * (fDir + PI_2) / PI);
			}
			else
				pElem[i*pMaxGrad->nWidth+j] += 255;
		}
	}

	delete pVert;
	delete pHorz;
	delete pBlur;

	return pMaxGrad;
}

//�ҵ��Ե�(*pX,*pY)ΪԲ��,�뾶Ϊ16��Բ�������ڵ�����ݶ�
//������������ݶȴ��ĵ������ֵ����(x,y)
//pMapΪԭͼ����ݶ�ͼ
void CIntelScissor::FindMaxGrad(int *pX, int *pY)
{
	int nWidth  = m_pDstImg->GetWidth();
	int nHeight = m_pDstImg->GetHeight();

	//����������ݶȵ������뾶Ϊ16
	int nRadius = GRADIENT_SEARCH >> 1;

	/*  calculate the extent of the search  */
	int nX = CLAMP(*pX, 0, nWidth);
	int nY = CLAMP(*pY, 0, nHeight);

	int nX1 = CLAMP(nX - nRadius, 0, nWidth);
	int nY1 = CLAMP(nY - nRadius, 0, nHeight);
	int nX2 = CLAMP(nX + nRadius + 1, 0, nWidth);
	int nY2 = CLAMP(nY + nRadius + 1, 0, nHeight);

	// ȡ����ͼ����ݶ�ͼ��
	PMATRIX pGrad = GetSubMap(nX1, nY1, nX2, nY2);
	int* pElem = (int*)pGrad->elem;
	*pX = nX - nX1;
	*pY = nY - nY1;
	int nMaxGrad = 0;
	int i, j, nGrad;
	for(i = 0; i < nY2-nY1; i ++)
	{
		for(j = 0; j < nX2-nX1; j ++)
		{
			nGrad = pElem[i*pGrad->nWidth+j]>>16; //���ص���ݶȷ���
			nGrad *= m_fDistWeights[(i-((nY-nY1)-nRadius))
					*GRADIENT_SEARCH+(j-((nX-nX1)-nRadius))];
			if(nMaxGrad < nGrad)
			{
				nMaxGrad = nGrad;
				*pX = j;
				*pY = i;
			}
		}
	}
	*pX += nX1;
	*pY += nY1;

	delete [] (char*)pGrad;
}

// ����ݶ�ͼpMap��(nX, nY)����ݶȷ���*pGrad�ͷ���*pDir
void CIntelScissor::GradMapValue(const PMATRIX pMap,
									int nX, int nY,
									int *pGrad, int *pDir)
{
	int value = ((int*)pMap->elem)[nY*pMap->nWidth+nX];

  	*pGrad = (WORD)(value >> 16);
	*pDir  = (WORD)(value & 0xff);
}

//�����(nX, nY)����nPixel�趨�ķ����ϵ����Ӵ���
//int CIntelScissor::CalculateLink(const PIPLMATRIX pMap, int nX, int nY, int nPixel, int nLink)
//{
//	int nValue = 0;
//	int nGrad1, nDir1, nGrad2, nDir2;
//
//	GradMapValue(pMap, nX, nY, &nGrad1, &nDir1);
//
//	nGrad1 = 255 - nGrad1;
//
//	if(nLink > 1)	//nLink = 2��3,���պ���2,3,6,7�ĸ��Խ��߷����е�ĳ��������
//		nValue = m_nDgnlWeights[nGrad1] * OMEGA_G;
//	else					//nLink = 0��1,���պ���0,1,4,5�ĸ���ʮ�ַ����ĳ��������
//		nValue = nGrad1 * OMEGA_G;
//
//	/*  calculate the contribution of the gradient direction  */
//	nX += nPixel & 0xff;
//	nY += (nPixel & 0xff00) >> 8;
//	GradMapValue(pMap, nX, nY, &nGrad2, &nDir2);
//
//  nValue += (m_nDirValue[nDir1][nLink] + m_nDirValue[nDir2][nLink]) * OMEGA_D;
//
//  return nValue;
//}

//�����(nX, nY)����nPixel�趨�ķ����ϵ����Ӵ���
//��(nNx,nNy)�ǵ�(nX,nY)��8�����е�ĳ����
int CIntelScissor::CalculateLink(PMATRIX pMap, int nX, int nY, int nNx, int nNy)
{
	int nValue;
	int nGrad1, nDir1, nGrad2, nDir2;
	float fD1x, fD1y, fD2x, fD2y, fLx, fLy, fD1, fD2, fD12;
	float f1, f2, fAbs;

	/*  calculate the contribution of the gradient direction  */
	GradMapValue(pMap, nX, nY, &nGrad1, &nDir1);
	f1 = cos(nDir1 * PI / 255 - PI_2);		//D(1) = [Ix(1), Iy(1)]
	f2 = sin(nDir1 * PI / 255 - PI_2);
	fD1x = f2;
	fD1y = -f1;
	/*  calculate the contribution of the gradient magnitude  */
	GradMapValue(pMap, nNx, nNy, &nGrad2, &nDir2);
	f1 = cos(nDir2 * PI / 255 - PI_2);		//D(1) = [Ix(1), Iy(1)]
	f2 = sin(nDir2 * PI / 255 - PI_2);
	fD2x = f2;
	fD2y = -f1;

	fAbs = sqrt(pow((nX-nNx), 2) + pow((nY-nNy), 2));
	if(fD1x * (nNx - nX) + fD1y * (nNy - nY) >= 0)
	{
		fLx = nNx - nX;
		fLy = nNy - nY;
	}
	else
	{
		fLx = nX - nNx;
		fLy = nY - nNy;
	}

	fD1 = fD1x * fLx + fD1y * fLy;
	fD2 = fLx * fD2x + fLy * fD2y;

	fD12 = OMEGA_D * (acos(fD1) + acos(fD2));	//0<fD12<4/3
	nValue = fD12 * 3 * 255 / 4;	//��fD12��ֵ�޶���0��255֮��
	return nValue;
}

//����FindOptimalPath�Ľ��pBuf�ҳ�����ֹ�㵽��ʼ�������·��
CPtrArray* CIntelScissor::PlotPixels(PMATRIX pCost,
							 int nX1, int nY1,
							 int nXs, int nYs,
							 int nXe, int nYe)
{

	POINTCAPTION* pElem = (POINTCAPTION*)pCost->elem;
	//�ɷ���ͼ�Ƶ����·��
	CPtrArray* pPtList = new CPtrArray;
	POINT* pPoint = NULL;

	int nX, nY, nDx, nDy, nSeedX, nSeedY;
	nX = nXe - nX1;
	nY = nYe - nY1;
	nSeedX = nXs - nX1;
	nSeedY = nYs - nY1;

//	pPoint = new POINT;
//	pPoint->x = nX + nX1;
//	pPoint->y = nY + nY1;
//	pPtList->Add(pPoint);
	while(nX != nSeedX || nY != nSeedY)
	{
		nDx = pElem[nY*pCost->nWidth+nX].ptDir.x;
		nDy = pElem[nY*pCost->nWidth+nX].ptDir.y;
		nX += nDx;
		nY += nDy;
		pPoint = new POINT;
		pPoint->x = nX + nX1;
		pPoint->y = nY + nY1;
		pPtList->Add(pPoint);
	}

	// ����Ĵ洢˳����
	int nSize = pPtList->GetSize();
	CPtrArray* pPtList1 = new CPtrArray;
	for(int i = 0; i < nSize; i ++)
	{
		pPoint = (POINT*)pPtList->GetAt(nSize-1-i);
		pPtList1->Add(pPoint);
	}
	pPtList->RemoveAll();
	delete pPtList;

	return pPtList1;
}

//�ҳ�����ʼ��(nXs,nYs)����(nXe,nYe)�����·��,��������ֹ�㵽��ʼ��������ֵ
//pMapָ��������ͼ����ݶ�ͼ,
CPtrArray* CIntelScissor::GetOptimalPath(/*const PIPLMATRIX pMap,*/
						int nXs, int nYs,
						int nXe, int nYe)
{
	int i, j;
	int nWidth = m_pDstImg->GetWidth();
	int nHeight = m_pDstImg->GetHeight();

	//�ҵ���ֹ�㸽��������ݶȴ��ĵ�����
	FindMaxGrad(&nXe, &nYe);

	//�õ�ptS��ptE�����γɵľ��εı߽�
	int nX1 = min(nXs, nXe);
	int nY1 = min(nYs, nYe);
	int nX2 = max(nXs, nXe) + 1;
	int nY2 = max(nYs, nYe) + 1;

	CPoint ptS1, ptE1;
	int nXs1 = CLAMP(nXs, 0, nWidth - 1);
	int nYs1 = CLAMP(nYs, 0, nHeight - 1);
	int nXe1 = CLAMP(nXe, 0, nWidth - 1);
	int nYe1 = CLAMP(nYe, 0, nHeight - 1);

  /*  expand the boundaries past the ending points by
   *  some percentage of width and height.  This serves the following purpose:
   *  It gives the algorithm more area to search so better solutions
   *  are found.  This is particularly helpful in finding "bumps" which
   *  fall outside the bounding box represented by the start and end
   *  coordinates of the "curve".
   */
	int nEWidth  = (nX2 - nX1) * EXTEND_BY + FIXED;
	int nEHeight = (nY2 - nY1) * EXTEND_BY + FIXED;
	if (nXe1 >= nXs1)
		nX2 += CLAMP (nEWidth, 0, nWidth - nX2);
	else
		nX1 -= CLAMP (nEWidth, 0, nX1);
	if (nYe1 >= nYs1)
		nY2 += CLAMP (nEHeight, 0, nHeight - nY2);
	else
		nY1 -= CLAMP (nEHeight, 0, nY1);
	//////////////////////////////////////////////////////////////////////////
	static int s_Xs1 = INT_MIN;
	static int s_Ys1 = INT_MIN;
	static int s_X1, s_Y1, s_X2, s_Y2;
	if (s_Xs1 != nXs1 || s_Ys1 != nYs1)	//�����㲻ͬ,������ȡ���ݶ�ͼ
	{
		if (m_pSubMap)	delete [] (BYTE*)m_pSubMap;
		if (m_pMark)	delete [] (BYTE*)m_pMark;
		m_pMark = NULL;

		s_Xs1 = nXs1;
		s_Ys1 = nYs1;
		s_X1 = nX1;
		s_Y1 = nY1;
		s_X2 = nX2;
		s_Y2 = nY2;
		m_pSubMap = GetSubMap(nX1, nY1, nX2, nY2);
	}
	else	//�����㲻��
	{
		//�����㲻��,�������������ӱ���ʼ���任������ʼ��Сʱ,����ȡ���ݶ�ͼ
		if ((s_X2 - nXs1) * (nX2 - nXs1) < 0)
		{
			s_X1 = nX1;
			s_Y1 = nY1;
			s_Y2 = nY2;
			s_X2 = nX2;
		}
		//��������ͬ������ƶ���Χ������,�����»�ȡͼ���ݶ�ͼ
		else if (s_X1 > nX1 || s_Y1 > nY1 || s_X2 < nX2 || s_Y2 < nY2)
		{
			if(s_X1 > nX1)		s_X1 = nX1;
			if(s_Y1 > nY1)		s_Y1 = nY1;
			if(s_X2 < nX2)		s_X2 = nX2;
			if(s_Y2 < nY2)		s_Y2 = nY2;

			if (m_pSubMap)	delete [] (BYTE*)m_pSubMap;
			if (m_pMark)	delete [] (BYTE*)m_pMark;
			m_pMark = NULL;
			m_pSubMap = GetSubMap(s_X1, s_Y1, s_X2, s_Y2);
		}
		else	//�������ƶ���Χû������,��ֱ�ӻ�ȡ��ʼ�㵽��ֹ������·��
		{
			//�ɷ���ͼ�Ƶ����·��
			CPtrArray* pPtList = PlotPixels(m_pMark, s_X1, s_Y1, nXs1, nYs1, nXe1, nYe1);

			return pPtList;
		}
	}

	//���ڶ�̬�洢���л�����б�ĵ�ַ
	CPtrArray* pList = new CPtrArray;

	//���ڴ洢��ǰ��ı�ǡ��ܴ��ۡ�ָ��λ���������
	nWidth  = m_pSubMap->nWidth;
	nHeight = m_pSubMap->nHeight;
	if (m_pMark) delete [] (BYTE*)m_pMark;
	m_pMark = (PMATRIX)new char [3*sizeof(int)+nWidth*nHeight*sizeof(POINTCAPTION)];
	m_pMark->nWidth = nWidth;
	m_pMark->nHeight= nHeight;
	POINTCAPTION* pMark1 = (POINTCAPTION*)m_pMark->elem;

	for(i = 0; i < nHeight; i ++)
	{
		for(j = 0; j < nWidth; j ++)
		{
			pMark1[i*nWidth+j].bFlag = FALSE;
			pMark1[i*nWidth+j].nCost = INT_MAX;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//����߽����п�����и߶�
	//���ӵ��ڵ�ǰ���ݶ�ͼ�е�����
	int nSeedX = nXs1 - s_X1;
	int nSeedY = nYs1 - s_Y1;
	pMark1[nSeedY*nWidth+nSeedX].nCost = 0;

	PPOINTVALUE pPtValue = new POINTVALUE;
	pPtValue->nX = nSeedX;
	pPtValue->nY = nSeedY;
	pPtValue->nValue = 0;
	pList->Add(pPtValue);

	int nMinV, nIdx;
	int nMinX, nMinY, nNx, nNy;
	PPOINTVALUE pvTemp = NULL;
	PPOINTVALUE pvTemp1 = NULL;
	while(pList->GetSize())
	{
		nMinV = INT_MAX;
		//�ҳ���С���۵�
		for(i = 0; i < pList->GetSize(); i ++)
		{
			pPtValue = (PPOINTVALUE)pList->GetAt(i);
			if(nMinV > pPtValue->nValue)
			{
				nMinV = pPtValue->nValue;
				nMinX = pPtValue->nX;
				nMinY = pPtValue->nY;
				nIdx = i;
			}
		}
		pvTemp1 = (PPOINTVALUE)pList->GetAt(nIdx);
		pList->RemoveAt(nIdx);
		delete pvTemp1;

		pMark1[nMinY*nWidth+nMinX].bFlag = TRUE;
		for(i = 0; i < 8; i ++)
		{
			nNx = nMinX + nN[i][0];
			nNy = nMinY + nN[i][1];
			//�жϵ�(nNx, nNy)�Ƿ������ݶ�ͼ��
			if((nNx < 0 || nNx >= nWidth)	|| (nNy < 0 || nNy >= nHeight))
				continue;
			//����ǰ�����������,�����ѭ��
			if(pMark1[nNy*nWidth+nNx].bFlag)
				continue;
			//�����(nNx,nNy)��(nMinX, nMinY)�����Ӵ���
			int nGrad, nDir;
			GradMapValue(m_pSubMap, nNx, nNy, &nGrad, &nDir);
			nGrad = 255 - nGrad;
			if(i >= 4)	//�жϵ�(nNx,nNy)�Ƿ��ڵ�ǰ��(nMinX,nMinY)���ĸ�����ͨ����������
				nGrad = m_nDgnlWeights[nGrad]; 

			//int nLinkCost = CalculateLink(m_pSubMap, nMinX, nMinY, nNx, nNy);

			pvTemp = new POINTVALUE;
			pvTemp->nValue = nMinV + nGrad;// * 0.75 + nLinkCost * 0.25;	//�ۻ�����
			pvTemp->nX = nNx;
			pvTemp->nY = nNy;

			//�жϵ�pvTemp�Ƿ���pList��
			nIdx = -1;
			for(j = 0; j < pList->GetSize(); j ++)
			{
				pvTemp1 = (PPOINTVALUE)pList->GetAt(j);
				if(pvTemp1->nX == pvTemp->nX && pvTemp1->nY == pvTemp->nY)
				{
					nIdx = j;
					break;
				}
			}
			//���pvTemp��pList���������С�ڸõ���ǰ�������ô���,�򵯳��õ�
			if(nIdx >= 0 && pvTemp->nValue < pMark1[nNy*nWidth+nNx].nCost)
			{
				pvTemp1 = (PPOINTVALUE)pList->GetAt(nIdx);
				pList->RemoveAt(nIdx);
				delete pvTemp1;
			}

			//�жϵ�pvTemp�Ƿ���pList��
			nIdx = -1;
			for(j = 0; j < pList->GetSize(); j ++)
			{
				pvTemp1 = (PPOINTVALUE)pList->GetAt(j);
				if(pvTemp1->nX == pvTemp->nX && pvTemp1->nY == pvTemp->nY)
				{
					nIdx = j;
					break;
				}
			}
			//���pvTemp����pList���򽫸õ���뵽pList��
			if(nIdx < 0)
			{
				pMark1[nNy*nWidth+nNx].nCost = pvTemp->nValue;
				pMark1[nNy*nWidth+nNx].ptDir.x = -nN[i][0];
				pMark1[nNy*nWidth+nNx].ptDir.y = -nN[i][1];
				pList->Add(pvTemp);
			}
			else
				delete pvTemp;
		}
	}
	pList->RemoveAll();		//���Ӧ��ûʲô�ô�
	delete pList;

	//�ɷ���ͼ�Ƶ����·��
	CPtrArray* pPtList = PlotPixels(m_pMark, s_X1, s_Y1, nXs1, nYs1, nXe1, nYe1);

	return pPtList;
}

void CIntelScissor::ReleaseArray(CPtrArray** ppPtList)
{
	if(ppPtList[0])
	{
		for(int i = 0; i < ppPtList[0]->GetSize(); i ++)
		{
			POINT* pPt = (POINT*)ppPtList[0]->GetAt(i);
			delete pPt;
		}
		ppPtList[0]->RemoveAll();
		delete ppPtList[0];
		ppPtList[0] = NULL;
	}
}
//{�޸�2005.11.01
//�������·���ϵ��µ����ӵ�����к�
//pPtList1 : ԭ���·��, pPtList2 :���ڵ����·��
int	CIntelScissor::PathCooling(CPtrArray* pPtList1, CPtrArray* pPtList2)
{
	int nSize = pPtList1->GetSize();
	if(nSize > pPtList2->GetSize())
		nSize = pPtList2->GetSize();
	int i, j;
	POINT *pPt1, *pPt2;
	if(nSize < 20)
		return -1;
	else
	{
		for(i = 0; i < nSize; i ++)
		{
			pPt1 = (POINT*)pPtList1->GetAt(i);
			pPt2 = (POINT*)pPtList2->GetAt(i);
			if(abs(pPt1->x - pPt2->x)*abs(pPt1->y - pPt2->y) <= 4)
				m_nTimes[i] ++;
			else
				break;
		}
		for(; i < MAXPOINTS; i ++)
			m_nTimes[i] = 0;
	}
	nSize = pPtList2->GetSize();
	//�������ƶ�5��ĳ������ʼ����Զ�ĵ���Ȼû�б仯,�򽫸õ���Ϊ���ӵ�
	for(i = MAXPOINTS-1; i >= 20; i --)
	{
		if(m_nTimes[i] > 5 && nSize-i > 8)
		{
			pPt1 = (POINT*)pPtList1->GetAt(i);
			m_nXs = pPt1->x;
			m_nYs = pPt1->y;
			//�������㣬׼����һ��·����ȴ
			for(j = 0; j < MAXPOINTS; j ++)
				m_nTimes[j] = 0;
			return i;
		}
	}
	return -1;
}
//}�޸�2005.11.01

//�ж��������Ƿ��ص�,�ж�����Ϊ��������ƽ���Ƿ�С��36
BOOL CIntelScissor::IsOverlap(POINT* pPt1, POINT* pPt2)
{
	if(pow(pPt1->x - pPt2->x, 2) + pow(pPt1->y - pPt2->y, 2) < GRADIENT_SEARCH)
		return TRUE;

	return FALSE;
}

POINT* CIntelScissor::GetPointBuf()
{
	return m_vPoints.begin();
}

int CIntelScissor::GetPointNum()
{
	return m_vPoints.size();
}

//���߽�ۺϲ����ۺϽ�����뵽m_vPoints��
void CIntelScissor::EdgeMerge()
{
	ObjEdgeList objSrcObjEdge, objEdgeMerge;
//	objSrcObjEdge.m_nObjMaxNum = 1;
	objSrcObjEdge.m_nObjNum = 1;
//	objSrcObjEdge.m_ppEdgeObj = new PEDGE_OBJECT;
	objSrcObjEdge.m_ppEdgeObj[0] = new EDGE_OBJECT;
	objSrcObjEdge.m_ppEdgeObj[0]->nEdgeMaxNum = 1;
	objSrcObjEdge.m_ppEdgeObj[0]->nEdgeNum = 1;
	objSrcObjEdge.m_ppEdgeObj[0]->ppEdgeLst = new PEDGE_LIST;
	objSrcObjEdge.m_ppEdgeObj[0]->ppEdgeLst[0] = 
		(PEDGE_LIST)new char [1*sizeof(int)+m_vPoints.size()*sizeof(POINT)];
	objSrcObjEdge.m_ppEdgeObj[0]->ppEdgeLst[0]->nEdgeLen = m_vPoints.size();

	// ȡ��Դͼ���������
	long double dbRatio;
	m_pSrcImg->GetZoomFactor(dbRatio, dbRatio);

	//���߽�����괦���ͼ��δ����ʱ�����ص�����
	POINT* pEdge = objSrcObjEdge.m_ppEdgeObj[0]->ppEdgeLst[0]->edge;
	for (int i=0; i<m_vPoints.size(); i++)
	{
		pEdge[i].x = m_vPoints[i].x/dbRatio;
		pEdge[i].y = m_vPoints[i].y/dbRatio;
	}

	// ��10Ϊ������ֵ���б߽����
	m_pDstImg->ObjEdgeMerge(&objSrcObjEdge, 3, &objEdgeMerge);

	// ����Ϻ�ı߽�����괦��ɵ�ǰ����ͼ���еĵ�����
	m_pPointList->RemoveAll();
	int nEdgeLen = objEdgeMerge.m_ppEdgeObj[0]->ppEdgeLst[0]->nEdgeLen;
	POINT* pDstEdge = objEdgeMerge.m_ppEdgeObj[0]->ppEdgeLst[0]->edge;
	for (i=0; i<nEdgeLen; i++)
	{
		// �洢��Ϻ�δ���ŵĵ�����
		POINT2DF pt;
		pt.x = pDstEdge[i].x;
		pt.y = pDstEdge[i].y;
		m_pPointList->Add(pt);
	}
}

// ���ͼ��Դ�����Ͻ�����Ϊ(nX1,nY1),���½�����Ϊ(nX2,nY2)(�������õ�)
// �ľ��������R,G,B����ͼ,�������ͼ������ݶ�ͼ
PMATRIX CIntelScissor::GetSubMap(int nX1, int nY1, int nX2, int nY2)
{
	int i, j;

	//��CRect(nX1,nY1,nX2,nY2)�������ľ��������������Ҹ���չһ�����ؿ��
	//ͬʱ�������Ѿ�����ͼ��߽�����
	int nL, nT, nR, nB;
//	nL = CLAMP(nL-1, 0, m_pDstImg->GetWidth());
	nL = (nX1 > 0) ? nX1 - 1 : nX1;
	nT = (nY1 > 0) ? nY1 - 1 : nY1;
	nR = (nX2 < m_pDstImg->GetWidth()) ? nX2 + 1 : nX2;
	nB = (nY2 < m_pDstImg->GetHeight()) ? nY2 + 1 : nY2;
	int nWidth = nR - nL;
	int nHeight = nB - nT;

	// ��ȡ����ͼ��
	CRect rect(nL, nT, nR, nB);
	CImage* pImage = new CImage();
	m_pDstImg->Crop(rect, pImage);

	// ��ͼ������ת�������
// 	CImage* pBlur = new CImage(*pImage);
// 	long blur_32[]={1,1,1,1,24,1,1,1,1};
// 	pBlur->Filter(blur_32,3,32,0);
// 
	PMATRIX pSrc = (PMATRIX)new BYTE [3*sizeof(int)+pImage->GetWidth()*pImage->GetHeight()*3*sizeof(float)];
	pSrc->nWidth = pImage->GetWidth();
	pSrc->nHeight = pImage->GetHeight();
	pSrc->nDepth = 3;
	float* pSrc1 = (float*)pSrc->elem;
	for (i=0; i<pSrc->nHeight; i++)
	{
		for (j=0; j<pSrc->nWidth; j++)
		{
			RGBQUAD rgb = pImage->GetPixelColor(j,i);
			pSrc1[i*nWidth*3+j*3  ] = rgb.rgbBlue;
			pSrc1[i*nWidth*3+j*3+1] = rgb.rgbGreen;
			pSrc1[i*nWidth*3+j*3+2] = rgb.rgbRed;
		}
	}

	//��ȡƽ��������
	long blur_32[]={1,1,1,1,24,1,1,1,1};
	PMATRIX pBlur = convolution(pSrc, blur_32, 3, 32);
//	pBlur->Filter(blur_32,3,32,0);

	// ��ƽ�����ͨ�����ȡ��ˮƽ������ݶ�ֵ
//	CImage* pHorz = new CImage(*pBlur);
	long horz_deriv[]={1,0,-1,2,0,-2,1,0,-1};
 	PMATRIX pHorz = convolution(pBlur, horz_deriv, 3, 4);
//	pHorz->Filter(horz_deriv,3,4,0);

	// ��ƽ�����ͨ�����ȡ�ô�ֱ������ݶ�ֵ
//	CImage* pVert = new CImage(*pBlur);
	long vert_deriv[]={1,2,1,0,0,0,-1,-2,-1};
	PMATRIX pVert = convolution(pBlur, vert_deriv, 3, 4);
//	pVert->Filter(vert_deriv,3,4,0);

	float* pHorz1 = (float*)pHorz->elem;
	float* pVert1 = (float*)pVert->elem;

	PMATRIX pMaxG = (PMATRIX)new BYTE [(3+pImage->GetWidth()*pImage->GetHeight())*sizeof(int)];
	pMaxG->nWidth = pImage->GetWidth();
	pMaxG->nHeight= pImage->GetHeight();
	int* pMaxG1 = (int*)pMaxG->elem;
	memset(pMaxG1, 0, pMaxG->nWidth*pMaxG->nHeight*sizeof(int));

	float fMaxH, fMaxV, fGrad;
	float fMax = FLT_MIN;
	float fMin = FLT_MAX;
	int nEffWidth = pImage->GetEffWidth();
	for(i = 0; i < nHeight; i ++)
	{
		for(j = 0; j < nWidth; j ++)
		{
			if(i==0 || j==0 || i==nHeight-1 || j==nWidth-1)
			{
				pMaxG1[(nHeight-i-1)*pMaxG->nWidth+j] = (0 << 16) + 255;
				continue;
			}
			fMaxH = pHorz1[i*pHorz->nWidth*3+j*3+0];
			if(abs(abs(pHorz1[i*pHorz->nWidth*3+j*3+1])/* - 128*/) > abs(fMaxH))
				fMaxH = pHorz1[i*pHorz->nWidth*3+j*3+1]/* - 128*/;
			if(abs(abs(pHorz1[i*pHorz->nWidth*3+j*3+2])/* - 128*/) > abs(fMaxH))
				fMaxH = pHorz1[i*pHorz->nWidth*3+j*3+2]/* - 128*/;

			fMaxV = pVert1[i*pHorz->nWidth*3+j*3+0];
			if(abs(abs(pVert1[i*pHorz->nWidth*3+j*3+1])/* - 128*/) > abs(fMaxV))
				fMaxV = pVert1[i*pHorz->nWidth*3+j*3+1]/* - 128*/;
			if(abs(abs(pVert1[i*pHorz->nWidth*3+j*3+2])/* - 128*/) > abs(fMaxV))
				fMaxV = pVert1[i*pHorz->nWidth*3+j*3+2]/* - 128*/;

			//TRACE("fMaxH = %f, fMaxV = %f\n", fMaxH, fMaxV);
			fGrad = sqrt(fMaxH * fMaxH + fMaxV * fMaxV);
			pMaxG1[(nHeight-i-1)*pMaxG->nWidth+j] = (int)((fGrad) * SQRT2_2)/*5 / MAX_GRADIENT*/<< 16;
//		if(fGrad > MIN_GRADIENT)
//		{
				float fDir;
				if(!fMaxH)
					fDir = (fMaxV > 0) ? PI_2 : -PI_2;
				else
					fDir = atan(fMaxV / fMaxH);
				pMaxG1[(nHeight-i-1)*pMaxG->nWidth+j] += (int)(255 * (fDir + PI_2) / PI);
//		}
//		else
//			ppMaxG[i][j] += 255;
		}
	}

	//ȥ�����߽粿�ֵ�ֵ
	int nL1, nT1;
	nL1 = (nX1 == nL) ? 0 : 1;
	nT1 = (nY1 == nT) ? 0 : 1;
	nWidth = nX2 - nX1;
	nHeight = nY2 - nY1;
	PMATRIX pDstG = (PMATRIX)new BYTE [(3+nWidth*nHeight)*sizeof(int)];
	pDstG->nWidth = nWidth;
	pDstG->nHeight = nHeight;

	int* pDstG1 = (int*)pDstG->elem;
	for (i=nT1; i<nT1+nHeight; i++)
	{
		for (j=nL1; j<nL1+nWidth; j++)
		{
			pDstG1[(i-nT1)*nWidth+(j-nL1)] = pMaxG1[i*pMaxG->nWidth+j];
		}
	}

	// �������???
	for(i = 0; i < nHeight; i ++)
	{
		for(j = 0; j < nWidth; j ++)
		{
			if((i==0 && nT1==0)	|| (j==0 && nL1==0)
				|| (i==nHeight-1 && nY2==nB)
				|| (j==nWidth-1 && nX2==nR))
				pDstG1[i*nWidth+j] = (0 << 16) + 255;
		}
	}

	delete pMaxG;
	delete pVert;
	delete pHorz;
	delete pBlur;
	delete pSrc;
	delete pImage;

	return pDstG;
}

// ͼ���������ؾ�����ָ�룬����Ϊ������
PMATRIX CIntelScissor::convolution(CImage* pImage, long* kernel, int nSize, int kFactor)
{
	int i, j;
	int nWidth  = pImage->GetWidth()+nSize/2*2;
	int nHeight = pImage->GetHeight()+nSize/2*2;
	PMATRIX pSrc = (PMATRIX)new BYTE [3*sizeof(int)+nWidth*nHeight*sizeof(BYTE)*3];
	pSrc->nWidth = nWidth;
	pSrc->nHeight = nHeight;
	char* pSrc1 = (char*)pSrc->elem;

	// ��ԭͼ��߽�����nSize/2���������һ��������
	for (i=nSize/2; i<nHeight-nSize/2; i++)
	{
		// ���
		for (j=0; j<nSize/2; j++)
		{
			RGBQUAD rgb;
			rgb = pImage->GetPixelColor(nSize/2-j,i);
			pSrc1[i*nWidth*3+j*3  ] = rgb.rgbBlue;
			pSrc1[i*nWidth*3+j*3+1] = rgb.rgbGreen;
			pSrc1[i*nWidth*3+j*3+2] = rgb.rgbRed;
		}

		// �ұ�
		for (j=nWidth-nSize/2-1; j<nWidth; j++)
		{
			RGBQUAD rgb;
			rgb = pImage->GetPixelColor(nWidth-j,i);
			pSrc1[i*nWidth*3+j*3  ] = rgb.rgbBlue;
			pSrc1[i*nWidth*3+j*3+1] = rgb.rgbGreen;
			pSrc1[i*nWidth*3+j*3+2] = rgb.rgbRed;
		}
	}

	for (j=0; j<nWidth; j++)
	{
		//  �ϱ�
		for (i=0; i<nSize/2; i++)
		{
			RGBQUAD rgb;
			rgb = pImage->GetPixelColor(j,nSize/2-i);
			pSrc1[i*nWidth*3+j*3  ] = rgb.rgbBlue;
			pSrc1[i*nWidth*3+j*3+1] = rgb.rgbGreen;
			pSrc1[i*nWidth*3+j*3+2] = rgb.rgbRed;
		}

		//  �±�
		for (i=nHeight-nSize/2-1; i<nHeight; i++)
		{
			RGBQUAD rgb;
			rgb = pImage->GetPixelColor(j,nHeight-i);
			pSrc1[i*nWidth*3+j*3  ] = rgb.rgbBlue;
			pSrc1[i*nWidth*3+j*3+1] = rgb.rgbGreen;
			pSrc1[i*nWidth*3+j*3+2] = rgb.rgbRed;
		}
	}

	nWidth = pImage->GetWidth();
	nHeight = pImage->GetHeight();
	PMATRIX pRet = (PMATRIX)new BYTE [3*sizeof(int)+nWidth*nHeight*sizeof(float)];
	pRet->nWidth = nWidth;
	pRet->nHeight = nHeight;
	float* pRet1 = (float*)pRet->elem;

	// ͼ����
	for (i=nSize/2; i<nHeight-nSize/2; i++)
	{
		for (j=nSize/2; j<nWidth-nSize/2; j++)
		{
			float fR, fG, fB;
			fR = fG = fB = 0.0;
			for (int m=-nSize/2; m<=nSize/2; m++)
			{
				for (int n=-nSize/2; n<=nSize/2; n++)
				{
					fB += pSrc1[(i+m)*nWidth*3+(j+n)*3  ]*kernel[(m+nSize/2)*nSize+(n+nSize/2)]/kFactor;
					fG += pSrc1[(i+m)*nWidth*3+(j+n)*3+1]*kernel[(m+nSize/2)*nSize+(n+nSize/2)]/kFactor;
					fR += pSrc1[(i+m)*nWidth*3+(j+n)*3+2]*kernel[(m+nSize/2)*nSize+(n+nSize/2)]/kFactor;
				}
			}

			float fMax;
			fMax = (abs(fB)>abs(fG)) ? (abs(fB)>abs(fR)?fB:fR) : (abs(fG)>abs(fR)?fG:fR);
			pRet1[(i-nSize/2)*pRet->nWidth+(j-nSize/2)] = fMax;
		}
	}

	return pRet;
}

// ��ͼ�������о�������ؾ�����ָ�룬����Ϊ������
PMATRIX CIntelScissor::convolution(PMATRIX pImg, long* kernel, int nSize, int kFactor)
{
	int i, j, k;
	int nWidth  = pImg->nWidth+nSize/2*2;
	int nHeight = pImg->nHeight+nSize/2*2;
	PMATRIX pSrc = (PMATRIX)new BYTE [3*sizeof(int)+nWidth*nHeight*sizeof(float)*3];
	pSrc->nWidth = nWidth;
	pSrc->nHeight = nHeight;
	float* pSrc1 = (float*)pSrc->elem;
	float* pImg1 = (float*)pImg->elem;

	long* kern = new long [9];
	for (i=0; i<nSize; i++)
	{
		for (j=0; j<nSize; j++)
			kern[i*nSize+j] = kernel[(nSize-i-1)*nSize+(nSize-j-1)];
	}

	// ��ԭͼ��߽�����nSize/2���������һ��������
	for (i=nSize/2; i<nHeight-nSize/2; i++)
	{
		// ���
		for (j=0; j<nSize/2; j++)
		{
			pSrc1[i*nWidth*3+j*3  ] = pImg1[(i-nSize/2)*pImg->nWidth*3+(nSize/2-j-1)*3  ];
			pSrc1[i*nWidth*3+j*3+1] = pImg1[(i-nSize/2)*pImg->nWidth*3+(nSize/2-j-1)*3+1];
			pSrc1[i*nWidth*3+j*3+2] = pImg1[(i-nSize/2)*pImg->nWidth*3+(nSize/2-j-1)*3+2];
		}

		// �м�
		for (j=nSize/2; j<nWidth-nSize/2; j++)
		{
			pSrc1[i*nWidth*3+j*3  ] = pImg1[(i-nSize/2)*pImg->nWidth*3+(j-nSize/2)*3  ];
			pSrc1[i*nWidth*3+j*3+1] = pImg1[(i-nSize/2)*pImg->nWidth*3+(j-nSize/2)*3+1];
			pSrc1[i*nWidth*3+j*3+2] = pImg1[(i-nSize/2)*pImg->nWidth*3+(j-nSize/2)*3+2];
		}

		// �ұ�
		for (j=nWidth-nSize/2,k=0; j<nWidth; j++,k++)
		{
			pSrc1[i*nWidth*3+j*3  ] = pImg1[(i-nSize/2)*pImg->nWidth*3+(pImg->nWidth-k-1)*3  ];
			pSrc1[i*nWidth*3+j*3+1] = pImg1[(i-nSize/2)*pImg->nWidth*3+(pImg->nWidth-k-1)*3+1];
			pSrc1[i*nWidth*3+j*3+2] = pImg1[(i-nSize/2)*pImg->nWidth*3+(pImg->nWidth-k-1)*3+2];
		}
	}

	for (j=0; j<nWidth; j++)
	{
		//  �ϱ�
		for (i=0,k=nSize/2*2-1; i<nSize/2; i++,k--)
		{
			pSrc1[i*nWidth*3+j*3  ] = pSrc1[k*nWidth*3+j*3  ];
			pSrc1[i*nWidth*3+j*3+1] = pSrc1[k*nWidth*3+j*3+1];
			pSrc1[i*nWidth*3+j*3+2] = pSrc1[k*nWidth*3+j*3+2];
		}
		
		//  �±�
		for (i=nHeight-nSize/2,k=i-1; i<nHeight; i++,k--)
		{
			pSrc1[i*nWidth*3+j*3  ] = pSrc1[k*nWidth*3+j*3  ];
			pSrc1[i*nWidth*3+j*3+1] = pSrc1[k*nWidth*3+j*3+1];
			pSrc1[i*nWidth*3+j*3+2] = pSrc1[k*nWidth*3+j*3+2];
		}
	}

	PMATRIX pRet = (PMATRIX)new BYTE [3*sizeof(int)+pImg->nWidth*pImg->nHeight*sizeof(float)*3];
	pRet->nWidth = pImg->nWidth;
	pRet->nHeight = pImg->nHeight;
	pRet->nDepth = 3;
	float* pRet1 = (float*)pRet->elem;
	
	// ͼ����
	for (i=nSize/2; i<nHeight-nSize/2; i++)
	{
		for (j=nSize/2; j<nWidth-nSize/2; j++)
		{
			float fR, fG, fB;
			fR = fG = fB = 0.0;
			for (int m=-nSize/2; m<=nSize/2; m++)
			{
				for (int n=-nSize/2; n<=nSize/2; n++)
				{
					fB += pSrc1[(i+m)*nWidth*3+(j+n)*3  ]*kern[(m+nSize/2)*nSize+(n+nSize/2)];
					fG += pSrc1[(i+m)*nWidth*3+(j+n)*3+1]*kern[(m+nSize/2)*nSize+(n+nSize/2)];
					fR += pSrc1[(i+m)*nWidth*3+(j+n)*3+2]*kern[(m+nSize/2)*nSize+(n+nSize/2)];
				}
			}
			pRet1[(i-nSize/2)*pRet->nWidth*3+(j-nSize/2)*3  ] = fB/kFactor;
			pRet1[(i-nSize/2)*pRet->nWidth*3+(j-nSize/2)*3+1] = fG/kFactor;
			pRet1[(i-nSize/2)*pRet->nWidth*3+(j-nSize/2)*3+2] = fR/kFactor;
		}
	}

	delete [] kern;
	delete [] pSrc;

	return pRet;
}

void CIntelScissor::Save()
{
// 	CTilSysApp* pApp = (CTilSysApp*)AfxGetApp();
// 	CTilSysView* pView = (CTilSysView*)pApp->GetView(RUNTIME_CLASS(CScrollView));
// 	CZoom zoom = pView->GetZoom();
// 
// 	long double dbRatio = 
// 
// 	int nNumInSlice = m_pDataFile->GetObjNumInSlice(m_nSliceIdx);
// 	m_pDataFile->SetObjNumInSlice(m_nSliceIdx, nNumInSlice + 1);
// 
// 	CPoint pt;
// 	int nNum = m_vPoints.size();
// 	for(int i = 0; i < nNum; i ++)
// 	{
// 		pt = zoom.GetOriPos(m_vPoints[i]);
// 		m_pDataFile->SetObjPtInSlice(m_nSliceIdx, nNumInSlice, i, pt.x, pt.y);
// 	}
// 	m_pDataFile->SetObjPtNumInSlice(m_nSliceIdx, nNumInSlice, nNum);
// 	m_pDataFile->SetObjDrawType(m_nSliceIdx, nNumInSlice, DT_POLY);
}

void CIntelScissor::Add(CDC *pDC, CPoint point)
{
	//////////////////////////////////////////////////////////////////////////
	m_nXs = point.x;
	m_nYs = point.y;
	FindMaxGrad(&m_nXs, &m_nYs);
	//////////////////////////////////////////////////////////////////////////

	m_bDrag = FALSE;
}

void CIntelScissor::OnLButtonDown(CDC *pDC, PGRAPHCAPTION pCaption, CPoint point)
{
	// �������ѡ��ǰͼ��
	if(m_HaveFindFirst)
	{
		pCaption->L_iPointCount=IsOnMarginPoint(point);
		if(pCaption->L_iPointCount>=0)
		{
			pCaption->m_bHaveFindSecond=true;
			pCaption->m_CurrentDrawTool=Graph_Magic;
			pCaption->m_CurrentDrawStatus=Change_Status;
		}
		if(pCaption->L_iPointCount<-1)
		{  
			pCaption->m_bHaveFindSecond=true;
			pCaption->m_CurrentDrawTool=Graph_Magic;
			pCaption->m_CurrentDrawStatus=Drag_Status;
		}
	}//*******if(L_pPolygon->m_HaveFindFirst)

	if(!pCaption->m_bHaveFindSecond&&IsInRgn(point))
	{
		if(!m_HaveFindFirst)
		{
			m_HaveFindFirst=true;
			pCaption->m_bHaveFindFirst=true;
			ShowSelectPoint(pDC);
		}
		
		else
		{
			this->m_HaveFindFirst=false;
			pCaption->m_bHaveFindFirst=true;
			this->ShowSelectPoint(pDC);
		}
	}
	if(pCaption->m_bHaveFindSecond)
	{
		pCaption->m_pCurrentUnit=this;
		m_DrawingMode=pDC->GetROP2();

		ShowSelectPoint(pDC);
		DrawOldReferencePoint(pDC,GetDstPoint(m_FoundPoint));
		DrawStatic(pDC);
	}
}

void CIntelScissor::OnMouseMove(CDC*pDC,PGRAPHCAPTION pCaption,CPoint point)
{
	// ���Ϊ��ͼ״̬
	if(pCaption->m_CurrentDrawStatus==Draw_Status)
	{
// 		if(pCaption->m_LBDownTimes!=0)
// 		{

// 			pDC->SetROP2(R2_NOTXORPEN);
// 			//m_DrawingMode=pDC->GetROP2();
// 			DrawMask(pDC,GetDstPoint(pCaption->m_SecondPoint));
// 			pCaption->m_SecondPoint=GetSrcPoint(point);
// 			pDC->SetROP2(this->m_DrawingMode);
// 			DrawMask(pDC,GetDstPoint(pCaption->m_SecondPoint));

//			TRACE("\n---CIntelScissor::OnMouseMove---");
			//////////////////////////////////////////////////////////////////////////
			// �������ܼ�������
			int nOldMode = pDC->SetROP2(R2_NOT);
			CPen pen(m_PenStyle,m_PenWidth,m_PenColor);
			CPen* pOldPen = pDC->SelectObject(&pen);

			POINT *pPt, *pPt1;
			int i, j;

			m_nXe = point.x;
			m_nYe = point.y;
			FindMaxGrad(&m_nXe, &m_nYe);
			CPtrArray* pPtList = NULL;
			pPtList = GetOptimalPath(m_nXs, m_nYs, m_nXe, m_nYe);

			// ���������˶˵�,�����δ�Ӷ˵���
			if (!m_bDrag)
			{
				//�����ӵ㵽��ǰ�������δ洢
				if(m_pPtList)
				{
					int nSize = m_pPtList->GetSize();
					for(i = 0; i < nSize; i ++)
					{
						pPt = (POINT*)m_pPtList->GetAt(i);
						m_vPoints.push_back(*pPt);
					}
					//����༭��
					pPt = (POINT*)m_pPtList->GetAt(nSize-1);
					pDC->Rectangle(pPt->x-2, pPt->y-2, pPt->x+2, pPt->y+2);
				}
				else
					pDC->Rectangle(point.x-3, point.y-3, point.x+3, point.y+3);
			}
			else	//�����µ����ӵ�
			{
				int nIdx = PathCooling(m_pPtList, pPtList);
				if(nIdx > 0)
				{
					pPt = (POINT*)m_pPtList->GetAt(nIdx);
					m_nXs = pPt->x;
					m_nYs = pPt->y;
					int nSize = m_pPtList->GetSize();

					// ɾ��ԭ���ӵ������һ��
					for(i = nIdx; i < nSize; i ++)
					{
						pPt = (POINT*)m_pPtList->GetAt(i);
						pDC->SetPixel(pPt->x, pPt->y, RGB(0, 0, 0));
						delete pPt;
					}
					m_pPtList->RemoveAt(nIdx, nSize-nIdx);

					ReleaseArray(&pPtList);
					m_bDrag = FALSE;
					pDC->SetROP2(nOldMode);
 					pDC->SelectObject(pOldPen);
// 					pDC->SelectObject(pBrush);
					return;
				}
			}

			//���µ���ֹ�㻭���µĴ�����
			if(m_bDrag)
			{
				int nSize1 = m_pPtList->GetSize();
				int nSize2 = pPtList->GetSize();
				//ɾ��ԭ�����������´������ϲ�ͬ�����ص�
				for(i = 0; i < nSize1; i ++)
				{
					pPt = (POINT*)m_pPtList->GetAt(i);
					for(j = 0; j < nSize2; j ++)
					{
						pPt1 = (POINT*)pPtList->GetAt(j);
						if(pPt->x == pPt1->x && pPt->y == pPt1->y)
							break;
					}
					if(j >= nSize2)
						pDC->SetPixel(pPt->x, pPt->y, RGB(0, 0, 0));
				}
				//����´���������ԭ�������ϲ�ͬ�����ص�
				for(i = 0; i < nSize2; i ++)
				{
					pPt = (POINT*)pPtList->GetAt(i);
					for(j = 0; j < nSize1; j ++)
					{
						pPt1 = (POINT*)m_pPtList->GetAt(j);
						if(pPt->x == pPt1->x && pPt->y == pPt1->y)
							break;
					}
					if(j >= nSize1)
						pDC->SetPixel(pPt->x, pPt->y, RGB(0, 0, 0));
				}
			}
			else	//���������������ӵ�,��ֱ�ӻ����´������ϵĵ�
			{
				int nSize = pPtList->GetSize();
				for(i = 0; i < nSize; i ++)
				{
					pPt = (POINT*)pPtList->GetAt(i);
					pDC->SetPixel(pPt->x, pPt->y, RGB(0, 0, 0));
				}
				m_bDrag = TRUE;
			}
			ReleaseArray(&m_pPtList);
			m_pPtList = pPtList;
			pDC->SetROP2(nOldMode);
// 			pDC->SelectObject(pOldpen);
// 			pDC->SelectObject(pBrush);
			//////////////////////////////////////////////////////////////////////////
// 		}
 	}
	// ���Ϊ�ı�ĳ���߽��λ��
	if(pCaption->m_CurrentDrawStatus==Change_Status)
	{
		pDC->SetROP2(R2_NOTXORPEN);
		ExchangeDraw(pDC,GetDstPoint(pCaption->m_SecondPoint));
		pCaption->m_SecondPoint=GetSrcPoint(point);
		ExchangeDraw(pDC,GetDstPoint(pCaption->m_SecondPoint));
	}
	// ���ͼ�δ����϶�״̬
	if(pCaption->m_CurrentDrawStatus==Drag_Status)
	{
		for(int i=0;i<m_pPointList->GetSize();i++)
		{
			POINT2DF point=m_pPointList->GetAt(i);
			point.x += pCaption->m_SecondPoint.x-pCaption->m_FirstPoint.x;
			point.y += pCaption->m_SecondPoint.y-pCaption->m_FirstPoint.y;
			m_pPointList->SetAt(i,point);
		}
	}
}

void CIntelScissor::DrawEnd(CDC*pDC,CPoint point)
{
	CPen pen;
	CBrush brush;
	int oldDrawingMode=pDC->SetROP2(this->m_DrawingMode);
	pen.CreatePen(m_PenStyle,m_PenWidth,m_PenColor);
	CPen* pOldPen=(CPen*)pDC->SelectObject(&pen);
	LOGBRUSH brushlog;
	brushlog.lbColor=m_BrushColor;
	brushlog.lbHatch=m_BrushHatch;
	brushlog.lbStyle=m_BrushStyle;

	brush.CreateBrushIndirect(&brushlog);

	CBrush*pBrush=(CBrush*)pDC->SelectObject(&brush);
	int OldBkMode=pDC->SetBkMode(m_BkMode);
	COLORREF OldColor=pDC->SetBkColor(m_BackgroundColor);

	//////////////////////////////////////////////////////////////////////////
	// ������ܼ�������
	m_bDrag = FALSE;

	long double dbRatio;
	m_pSrcImg->GetZoomFactor(dbRatio, dbRatio);
	
	// ɾ�����һ����Ȼ���ٽ�ԭ���Ĵ��������ݱ�������,������˵�����
	int i;
	POINT* pPt;
	for(i = 0; i < m_pPtList->GetSize(); i ++)
	{
		pPt = (POINT*)m_pPtList->GetAt(i);
		m_vPoints.push_back(*pPt);
	}

	//�ҳ����һ��Ԫ��
	vector<POINT>::iterator	Iter, IterSeed;
	Iter = m_vPoints.end();
	Iter --;
	IterSeed = m_vPoints.begin();
	//������ӵ�����һ�㰴��ĳ�������غ�,�����ӵ�����һ����������
	if(IsOverlap(Iter, IterSeed))
	{
		CPtrArray* pPtList = GetOptimalPath(IterSeed->x, IterSeed->y,
			Iter->x, Iter->y);
		for(i = 0; i < pPtList->GetSize(); i ++)
		{
			pPt = (POINT*)pPtList->GetAt(i);
			m_vPoints.push_back(*pPt);
		}
		ReleaseArray(&pPtList);
	}

	//���߽�ۺϲ����ϲ����ۺϽ�����뵽m_vPoints��
	EdgeMerge();

	// �����еĵ���ʾ����
	for(Iter = m_vPoints.begin(); Iter != m_vPoints.end()-1; Iter++)
	{
		pDC->MoveTo(Iter->x, Iter->y);
		pDC->LineTo((Iter+1)->x, (Iter+1)->y);
	}
	pDC->LineTo(Iter->x, Iter->y);

	if (m_bClose)
	{
		pDC->MoveTo(Iter->x, Iter->y);
		Iter = m_vPoints.begin();
		pDC->LineTo(Iter->x, Iter->y);
	}
	//////////////////////////////////////////////////////////////////////////

// 	if (m_bClose)
// 	{
// 		pDC->MoveTo(point);
// 		pDC->LineTo(GetDstPoint(m_pPointList->GetAt(0)));
// 	}
// 	
	pDC->SelectObject(pBrush);
	pDC->SelectObject(pOldPen);
	pDC->SetBkMode(OldBkMode);
	pDC->SetBkColor(OldColor);
	pDC->SetROP2(oldDrawingMode);
	
}


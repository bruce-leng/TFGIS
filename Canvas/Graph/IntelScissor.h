// IntelScissor.h: interface for the CIntelScissor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INTELSCISSOR_H__D8DD3833_63B5_4BEA_8089_A6E014BE10ED__INCLUDED_)
#define AFX_INTELSCISSOR_H__D8DD3833_63B5_4BEA_8089_A6E014BE10ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Sobelˮƽ���ģ��,ʹ��ʱÿԪ��ֵ/4
static int horz_deriv[9] = {1,0,-1,2,0,-2,1,0,-1};
// Sobel��ֱ���ģ��,ʹ��ʱÿԪ��ֵ/4
static int vert_deriv[9] = {1,2,1,0,0,0,-1,-2,-1};
// ƽ�����ģ��,ʹ��ʱÿԪ��ֵ/32
static int blur_32[9]	 = {1,1,1,1,24,1,1,1,1};
//��¼����·������ʼ�����ֹ��
// typedef struct __SEPOINTS
// {
// 	int nX1, nY1;
// 	int nX2, nY2;
// 	int nXs, nYs;
// 	int nXe, nYe;
// }SEPOINTS, *PSEPOINTS;
// �������
typedef struct __MATRIX
{
	int nWidth, nHeight, nDepth;	// �����ȡ��߶�
	BYTE elem[1];					// ����Ԫ��
}MATRIX, *PMATRIX, **PPMATRIX;

#define  GRADIENT_SEARCH   16  /* how far to look when snapping to an edge */
#define	 MAXPOINTS	400
#include "..\Image.h"
#include "Polygon.h"

// ���ܼ���
class AFX_EXT_CLASS CIntelScissor : public CPolygon
{
public:
	void Save();
	CIntelScissor();
	virtual ~CIntelScissor();

	// ���ü����������ܼ��������ı���ͼ��
	bool SetImage(CImage* pImage);

	virtual void Add(CDC *pDC, CPoint point);
	virtual void OnLButtonDown(CDC *pDC, PGRAPHCAPTION pCaption, CPoint point);
	virtual void OnMouseMove(CDC*pDC,PGRAPHCAPTION pCaption,CPoint point);
	virtual void DrawEnd(CDC*pDC,CPoint point);

private:
	float m_fDistWeights[GRADIENT_SEARCH * GRADIENT_SEARCH];
public:
	int GetPointNum();
	POINT* GetPointBuf();

	CPtrArray* PlotPixels(const PMATRIX pCost, int nX, int nY, int nXs, int nYs, int nXe, int nYe);
	int CalculateLink(const PMATRIX pMap, int nX, int nY, int nNx, int nNy);
	void GradMapValue(const PMATRIX pMap, int nX, int nY, int *pGrad, int *pDir);
	void	FindMaxGrad(int *pX, int *pY);
	CPtrArray* GetOptimalPath(int nXs, int nYs, int nXe, int nYe);
	PMATRIX GetGradMap();
	PMATRIX GetSubMap(int nX1, int nY1, int nX2, int nY2);

private:
	void	ReleaseArray(CPtrArray** ppPtList);
	int		PathCooling(CPtrArray* pPtList1, CPtrArray* pPtList2);
private:
	int m_nDgnlWeights[256];
	CImage* m_pSrcImg;	// δ����ͼ���ָ��
	CImage* m_pDstImg;	// ���ź�ͼ���ָ��

	PMATRIX m_pSubMap;
	PMATRIX m_pMark;

	BOOL m_bDrag;
private:

//	void GetZoomBits(RECT& rect, PIPLMATRIX pDataR, PIPLMATRIX pDataG, PIPLMATRIX pDataB);
	void EdgeMerge();
	// ͼ���������ؾ�����ָ��
	PMATRIX convolution(CImage* pImage, long* kernel, int nSize, int kFactor);
	PMATRIX convolution(PMATRIX pImg, long* kernel, int nSize, int kFactor);
	BOOL IsOverlap(POINT* pPt1, POINT* pPt2);
	BOOL m_bCopy;
	//{׷��2005.10.31
	int m_nTimes[MAXPOINTS];
	//}׷��2005.10.31
	CPtrArray*	m_pPtList;							//��ǰ�Ĵ���������,ע�����һ����������ӵ�
	vector<POINT>	m_vPoints;
	int m_nXs, m_nYs, m_nXe, m_nYe;
//	int m_nOffXs, m_nOffYs, m_nOffXe, m_nOffYe;
};

#endif // !defined(AFX_INTELSCISSOR_H__D8DD3833_63B5_4BEA_8089_A6E014BE10ED__INCLUDED_)

// MathComput.cpp: implementation of the CMathComput class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <cmath>
#include "MathComput.h"
#include "TilCore.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



// �����߶εĽ��㣬�н����򷵻�TRUE�����򷵻�FALSE
BOOL get_inter_point(POINT ptA1, POINT ptA2, POINT ptB1, POINT ptB2, POINT& interPt)
{
	POINT2DF ptFA1, ptFA2, ptFB1, ptFB2;
	ptFA1.x = ptA1.x;	ptFA1.y = ptA1.y;
	ptFA2.x = ptA2.x;	ptFA2.y = ptA2.y;
	ptFB1.x = ptB1.x;	ptFB1.y = ptB1.y;
	ptFB2.x = ptB2.x;	ptFB2.y = ptB2.y;

	POINT2DF interFPt;
	BOOL bRet;
	bRet = get_inter_point(ptFA1, ptFA2, ptFB1, ptFB2, interFPt);
	interPt.x = int(interFPt.x+0.5);
	interPt.y = int(interFPt.y+0.5);

	return bRet;
}

BOOL get_inter_point(POINT2DF ptA1, POINT2DF ptA2, POINT2DF ptB1, POINT2DF ptB2, POINT2DF& interPt)
{
	if(ptA1.x == ptA2.x && ptA1.y == ptA2.y)	return FALSE;
	if(ptB1.x == ptB2.x && ptB1.y == ptB2.y)	return FALSE;

	float k1, k2;	//�����߶ε�б��
	float c1, c2;	//���߶εĽؾ�

	//������߶�б�ʶ�����
	if(ptA1.x!=ptA2.x && ptB1.x!=ptB2.x)
	{
		// ������߶ζ���ˮƽ�ģ���û�н�������߶��غ�
		if(ptA1.y==ptA2.y && ptB1.y==ptB2.y)
			return FALSE;

		// б��
		k1 = (ptA1.y-ptA2.y)*1.0/(ptA1.x - ptA2.x);
		k2 = (ptB1.y-ptB2.y)*1.0/(ptB1.x - ptB2.x);

		// �����ֱ��ƽ��
		if (k1-k2>0.000001 && k1-k2<0.000001)
			return FALSE;

		// �ؾ�
		c1 = ptA1.y-k1*ptA1.x;
		c2 = ptB1.y-k2*ptB1.x;

		// ��ֱ�߽���
		interPt.x = (c2-c1)/(k1-k2);
		interPt.y = k1*interPt.x+c1;

		// �ж���ֱ�߽����Ƿ������߶���
		if(interPt.x>=min(ptA1.x, ptA2.x) && interPt.x<=max(ptA1.x, ptA2.x)
		&& interPt.x>=min(ptB1.x, ptB2.x) && interPt.x<=max(ptB1.x, ptB2.x))
			return TRUE;
		else
			return FALSE;
	}
	else if(ptA1.x==ptA2.x && ptB1.x!=ptB2.x)	//�����һ��ֱ����ֱ,�ڶ�����б��
	{
		k2 = (ptB1.y-ptB2.y)*1.0/(ptB1.x-ptB2.x);
		c2 = ptB1.y-k2*ptB1.x;

		interPt.x = ptA1.x;
		interPt.y = k2*interPt.x+c2;

		// �ж���ֱ�߽����Ƿ������߶���
		if(interPt.y>=min(ptA1.y, ptA2.y) && interPt.y<=max(ptA1.y, ptA2.y)
		&& interPt.y>=min(ptB1.y, ptB2.y) && interPt.y<=max(ptB1.y, ptB2.y))
			return TRUE;
		else
			return FALSE;
	}
	else if(ptA1.x!=ptA2.x && ptB1.x==ptB2.x)	//����ڶ���ֱ����ֱ,��һ����б��
	{
		k1 = (ptA1.y-ptA2.y)*1.0 /(ptA1.x-ptA2.x);
		c1 = ptA1.y-k1*ptA1.x;

		interPt.x = ptB1.x;
		interPt.y = k1*interPt.x+c1;

		if(interPt.y>=min(ptA1.y, ptA2.y) && interPt.y<=max(ptA1.y, ptA2.y)
		&& interPt.y>=min(ptB1.y, ptB2.y) && interPt.y<=max(ptB1.y, ptB2.y))
			return TRUE;
		else
			return FALSE;
	}
	else// if(ptA1.x == ptA2.x && ptB1.x == ptB2.x)	//�����ֱ��б�ʶ�������
	{
		return FALSE;
	}
}

// ����߶��뻡�ߵĽ���
/* ==================================================================
 * �������ƣ�  get_inter_point()
 * ���ߣ�      [Leng]
 * ������:     [2009-3-30 14:32:00]
 * ����ֵ��    [BOOL] - 0:�޽���,1:��1������,2:��2������
 * �����б� 
               [const POINT2DF& pt1] - [In] �߶�1�Ķ˵�1
               [const POINT2DF& pt2] - [In] �߶�1�Ķ˵�2
               [const POINT2DF& ptArc1] - [In] Բ���Ķ˵�1
               [const POINT2DF& ptCenter] - [In] Բ����Բ��
               [const float& fAngle] - [In] Բ�����ſ�����
               [const POINT2DF& ptArc2] - [In] Բ���Ķ˵�2
               [POINT2DF& ptInter] - [In]
ע�� :	����ֵΪ0ʱ��ptInter1��ptInter2����Ч������ֵΪ1ʱ��ptInter1
		��Ч��ptInter2��Ч������ֵΪ2ʱ��ptInter1��ptInter2����Ч
==================================================================*/
int get_inter_point(const POINT2DF& pt1, const POINT2DF& pt2,
					const POINT2DF& ptArc1, const POINT2DF& ptCenter,
					const POINT2DF& ptArc2,
					POINT2DF& ptInter1, POINT2DF& ptInter2)
{
	POINT2DF ptL1, ptL2, ptA1, ptO, ptA2, ptI;

	// ��Բ����Բ��ptCenterΪԭ�����½�������ϵ
	ptL1 = pt1-ptCenter;
	ptL2 = pt2-ptCenter;
	ptA1 = ptArc1-ptCenter;
	ptA2 = ptArc2-ptCenter;
	ptO.x = ptO.y = 0;

	float r = get_distance(ptA1, ptO);

	// ��ԭ�㵽ֱ�ߵľ���
	float fDist = point_line_dist(ptO, ptL1, ptL2);
	// ���ԭ�㵽ֱ�ߵľ�����ڰ뾶����û�н���
	if (fDist > r)
		return 0;

	// ��ֱ�߷��̼���Բ�Ľ���
	double k, b;
	POINT2DF ptX1, ptX2;
	if (ptL1.x!=ptL2.x)
	{
		k = (ptL1.y-ptL2.y)/(ptL1.x-ptL2.x);
		b = ptL1.y-k*ptL1.x;
		ptX1.x = (-k*b+sqrt(k*k*b*b-(1+k*k)*(b*b-r*r)))/(1+k*k);
		ptX1.y = k*ptX1.x+b;
		ptX2.x = (-k*b-sqrt(k*k*b*b-(1+k*k)*(b*b-r*r)))/(1+k*k);
		ptX2.y = k*ptX2.x+b;

	}
	// ���Ϊ����
	else
	{
		ptX1.x = ptL1.x;
		ptX1.y = sqrt(r*r-ptX1.x*ptX1.x);
		ptX2.x = ptX1.x;
		ptX2.y = -ptX1.y;
	}

	// ͨ����������жϵ���˳ʱ�뻹����ʱ������˳ʱ��Ϊ������ʱ��Ϊ����
	float sum;
	sum = ptA1.x*ptX1.y-ptA1.y*ptX1.x;
	sum += ptX1.x*ptA2.y-ptX1.y*ptA2.x;
	sum += ptA2.x*ptA1.y-ptA2.y*ptA1.x;
	if (sum > 0)
	{
		ptInter1.x = ptX1.x+ptCenter.x;
		ptInter1.y = ptX1.y+ptCenter.y;
	}

	float sum1;
	sum1 = ptA1.x*ptX2.y-ptA1.y*ptX2.x;
	sum1 += ptX2.x*ptA2.y-ptX2.y*ptA2.x;
	sum1 += ptA2.x*ptA1.y-ptA2.y*ptA1.x;
	if (sum1 > 0)
	{
		ptInter2.x = ptX2.x+ptCenter.x;
		ptInter2.y = ptX2.y+ptCenter.y;
	}

	if (sum*sum1>0)
	{
		// �������ֵ������0����û�н��㣬������2������
		if (sum>0)
			return 0;
		// �����������ʵ��Ϊͬһ�㣬��ֻ��1�����㣬�򷵻�1
		else if (ptInter1.x==ptInter2.x && ptInter1.y==ptInter2.y)
			return 1;
		else
			return 2;
	}
	else
	{
		// ���ֻ��sum����0����ptInter2Ϊ���㣬����ptInter1
		if (sum>0)
			ptInter1 = ptInter2;
		return 1;
	}

}

// �������������
double get_poly_area(POINT* pPoly, int nNum)
{
	POINT2DF* pFPoly = new POINT2DF [nNum];
	for (int i=0; i<nNum; i++)
	{
		pFPoly[i].x = pPoly[i].x;
		pFPoly[i].y = pPoly[i].y;
	}

	double dbArea = get_poly_area(pFPoly, nNum);
	delete [] pFPoly;

	return dbArea;
}

// �������������
double get_poly_area(POINT2DF* pPoly, int nNum)
{
	double dbArea = 0.0;

	for (int i=0; i<nNum-1; i++)
	{
		dbArea += pPoly[i].x*pPoly[i+1].y-pPoly[i].y*pPoly[i+1].x;
	}
	dbArea += pPoly[i].x*pPoly[0].y-pPoly[i].y*pPoly[0].x;

	dbArea = (dbArea>0) ? dbArea : -dbArea;

	return dbArea/2;
}

// ��������ľ���
float get_distance(POINT pt1, POINT pt2)
{
	return sqrt((pt1.x-pt2.x)*(pt1.x-pt2.x)+(pt1.y-pt2.y)*(pt1.y-pt2.y));
}

float get_distance(POINT2DF pt1, POINT2DF pt2)
{
	return sqrt((pt1.x-pt2.x)*(pt1.x-pt2.x)+(pt1.y-pt2.y)*(pt1.y-pt2.y));
}

// ����㵽ֱ�ߵľ���
// pt1,pt2��ֱ�߶ε����˵�
float point_line_dist(const POINT pt, const POINT pt1, const POINT pt2)
{
	POINT2DF ptL;
	ptL.x = (double)pt.x;
	ptL.y = (double)pt.y;

	POINT2DF pt1L;
	pt1L.x = (double)pt1.x;
	pt1L.y = (double)pt1.y;

	POINT2DF pt2L;
	pt2L.x = (double)pt2.x;
	pt2L.y = (double)pt2.y;

	return point_line_dist(ptL, pt1L, pt2L);
}

// ����㵽ֱ�ߵľ���
// pt1,pt2��ֱ�߶ε����˵�
float point_line_dist(const POINT2DF pt, const POINT2DF pt1, const POINT2DF pt2)
{
	float distance;
	float k;	//ֱ��б��
	float A, B, C;		//ϵ��
	float fTemp;
	
	//����ֱ�����˵�ĺ��������
	if(pt1.x == pt2.x)
	{
		//���ֱ�ߵ������˵�û���غ�
		if(pt1.y != pt2.y)
		{
			if(pt.x >= pt1.x)
				distance = pt.x - pt1.x;
			else
				distance = pt1.x - pt.x;
		}
	}
	else	//����ֱ�����˵�ĺ����겻���
	{
		//�����ֱ�ߵķ���Ax + By + C = 0
		k = (pt1.y-pt2.y) * 1.0 / (pt1.x-pt2.x * 1.0);
		A = k;
		B = -1;
		C = pt1.y - k * pt1.x;
		
		fTemp = A * pt.x + B * pt.y + C;
		//��㵽ֱ�ߵľ���
		if(fTemp >= 0)
			distance = fTemp / sqrt(A * A + B * B);
		else
			distance = - fTemp / sqrt(A * A + B * B);
	}
	
	return distance;
}

// �жϵ��Ƿ����߶���
int point_in_line(POINT point, POINT pt1, POINT pt2)
{
	POINT2DF pointF, ptF1, ptF2;
	pointF.x = point.x;
	pointF.y = point.y;
	ptF1.x = pt1.x;	ptF1.y = pt1.y;
	ptF2.x = pt2.x;	ptF2.y = pt2.y;

	return point_in_line(pointF, ptF1, ptF2);
}

int point_in_line(POINT2DF point, POINT2DF pt1, POINT2DF pt2)
{
	// rg = abs(y - (k * x + c))
	float k, c, rg;
	float fMaxX, fMinX, fMaxY, fMinY;

	fMaxX = max(pt1.x, pt2.x);
	fMinX = min(pt1.x, pt2.x);
	fMaxY = max(pt1.y, pt2.y);
	fMinY = min(pt1.y, pt2.y);

	if(fabs(pt1.x-pt2.x) < 0.00001)
	{
		if(point.x != pt1.x)
			return POINT_NOT_IN_LINE;
		else
		{
			if(point.y >= fMinY && point.y <= fMaxY)
				return POINT_IN_LINE;
			else
				return POINT_IN_EX_LINE;
		}
	}
	else
	{
		k = (pt1.y - pt2.y) * 1.0 / (pt1.x - pt2.x);	//б��
		c = pt1.y - k * pt1.x;		//�ؾ�
		rg = fabs(point.y - (k * point.x + c));
		if((point.x >= fMinX && point.x <= fMaxX)
		&& (point.y >= fMinY && point.y <= fMaxY))
		{
			if(rg <= 0.01)
				return POINT_IN_LINE;
			else
				return POINT_NOT_IN_LINE;
		}
		else
		{
			if (rg <= 0.01)
				return POINT_IN_EX_LINE;
			else
				return POINT_NOT_IN_LINE;
		}
	}
}

// �жϵ���߶������Ĵ������߶��ϻ����ӳ�����
// BOOL get_vert_point_in_line(const POINT point, const POINT pt1,
// 							const POINT pt2, POINT& ptVert)
// {
// 	POINT2DF pt1_1, pt2_1, point_1, ptVert_1;
// 	pt1_1.x = pt1.x;	pt1_1.y = pt1.y;
// 	pt2_1.x = pt2.x;	pt2_1.y = pt2.y;
// 	point_1.x = point.x;	point_1.y = point.y;
// 
// 	BOOL bRet;
// 	bRet = get_vert_point_in_line(point_1, pt1_1, pt2_1, ptVert_1);
// 	return bRet;
// }
// 
BOOL get_vert_point_in_line(const POINT2DF point, const POINT2DF pt1,
							const POINT2DF pt2, POINT2DF& ptVert)
{
	ASSERT(pt1.x!=pt2.x || pt1.y!=pt2.y);
	
	// ����߶�Ϊ����
	if (pt1.x == pt2.x)
	{
		ptVert.x = pt1.x;
		ptVert.y = point.y;
		return TRUE;
	}
	
	// ����߶�Ϊˮƽ��
	if (pt1.y == pt2.y)
	{
		ptVert.x = point.x;
		ptVert.y = pt1.y;
		return TRUE;
	}
	
	// ����߶μȲ���ֱҲˮƽ
	float k, b;
	k = (pt1.y-pt2.y)*1.0/(pt1.x-pt2.x);
	k = -1.0/k;
	b = point.y-k*point.x;
	
	// �����pt1,pt2�����߶δ�ֱ�Ҿ���point��ֱ���ϵ���һ����
	POINT2DF point1;
	point1.x = point.x+10;
	point1.y = k*point1.x+b;

	// �󽻵�
	BOOL bRet;
	bRet = get_inter_point(pt1, pt2, point, point1, ptVert);
	
	return bRet;
}

// �����ABC�Ļ���ֵ
double get_angle(POINT ptA, POINT ptB, POINT ptC)
{
	double a = get_distance(ptB, ptC);
	double b = get_distance(ptA, ptC);
	double c = get_distance(ptA, ptB);

	double cosB = (b*b+c*c-a*a)/(2*b*c);
	double angleB = acos(cosB);

	return angleB;
}

// �����ABC�Ļ���ֵ
double get_angle(POINT2DF ptA, POINT2DF ptB, POINT2DF ptC)
{
	double a = get_distance(ptB, ptC);
	double b = get_distance(ptA, ptC);
	double c = get_distance(ptA, ptB);

	double cosB = (a*a+c*c-b*b)/(2*a*c);
	double angleB = acos(cosB);

	return angleB;
}

// �����Զ˵�pt1��pt2Ϊ�˵��ֱ�ߵ���б�Ƕ�
float get_angle(POINT pt1, POINT pt2)
{
	POINT2DF ptD1, ptD2;
	ptD1.x = pt1.x;	ptD1.y = pt1.y;
	ptD2.x = pt2.x; ptD2.y = pt2.y;

	return get_angle(ptD1, ptD2);
}

// �����Զ˵�pt1��pt2Ϊ�˵��ֱ�ߵ���б����
// ��б�Ƕ���[0,180)��Χ��
float get_angle(POINT2DF pt1, POINT2DF pt2)
{
	ASSERT(pt1.x!=pt2.x || pt1.y!=pt2.y);

	float k;	//ֱ��б��
	float fAngle;	// ���ؽǶ�ֵ
	
	//����ֱ�����˵�ĺ��������
	if(pt1.x == pt2.x)
	{
		//���ֱ�ߵ������˵�û���غ�
		fAngle = 90.0;
	}
	else	//����ֱ�����˵�ĺ����겻���
	{
		//�����ֱ�ߵķ���Ax + By + C = 0
		k = (pt1.y-pt2.y) * 1.0 / (pt1.x-pt2.x * 1.0);
		fAngle = atan(k)/PI*180.0;
		if (fAngle<0)
			fAngle += 180;
	}

	return fAngle;
}

BOOL CalProperty(POINT* pBuf1, int nNum1, PROPERTY& pt1, int nWidth, int nHeight)
{
	int i;

	float fVlue = 1.0 / (2 * nWidth * nHeight);
	float fWidth = 1.0 / nWidth;
	float fHeight = 1.0 / nHeight;

	pt1.fArea = 0;
	pt1.ptMean.x = pBuf1[0].x;
	pt1.ptMean.y = pBuf1[0].y * fHeight;

	pt1.left = pBuf1[0].x;
	pt1.right = pBuf1[0].x;
	pt1.top = pBuf1[0].y;
	pt1.bottom = pBuf1[0].y;

	float fCCW = 0;
	for(i = 1; i < nNum1 - 1; ++i)
	{
		fCCW += 1.0*(pBuf1[i].x * pBuf1[i - 1].y - pBuf1[i - 1].x * pBuf1[i].y);
		pt1.fArea += fabs(fCCW);
		pt1.ptMean.x += pBuf1[i].x;
		pt1.ptMean.y += pBuf1[i].y;
		
		pt1.left = pt1.left < pBuf1[i].x ? pt1.left : pBuf1[i].x;
		pt1.right = pt1.right > pBuf1[i].x ? pt1.right : pBuf1[i].x;

		pt1.top = pt1.top < pBuf1[i].y ? pt1.top : pBuf1[i].y;
		pt1.bottom = pt1.bottom > pBuf1[i].y ? pt1.bottom : pBuf1[i].y;
	}

	//���������Ӱ��
	pt1.fArea = sqrt(fabs(pt1.fArea * fVlue));
	pt1.ptMean.x *= fWidth;	
	pt1.ptMean.y *= fHeight;	

	pt1.left *= fWidth;
	pt1.right *= fWidth;
	pt1.top *= fHeight;
	pt1.bottom *= fHeight;

	pt1.ptMean.x /= nNum1;
	pt1.ptMean.y /= nNum1;

	if (fCCW > 0)
	{
		return true;
	}
	else
		return FALSE;
}

void CalProperty(PPOINT2DF pBuf1, int nNum1, PROPERTY& pt1, int nWidth, int nHeight)
{
	int i;

	float fVlue = 1.0 / (2 * nWidth * nHeight);
	float fWidth = 1.0 / nWidth;
	float fHeight = 1.0 / nHeight;

	pt1.fArea = 0;
	pt1.ptMean.x = pBuf1[0].x;
	pt1.ptMean.y = pBuf1[0].y * fHeight;
	
	pt1.left = pBuf1[0].x;
	pt1.right = pBuf1[0].x;
	pt1.top = pBuf1[0].y;
	pt1.bottom = pBuf1[0].y;

	for(i = 1; i < nNum1 - 1; ++i)
	{
		pt1.fArea += fabs((pBuf1[i].x * pBuf1[i - 1].y - pBuf1[i - 1].x * pBuf1[i].y));
		pt1.ptMean.x += pBuf1[i].x;
		pt1.ptMean.y += pBuf1[i].y;

		pt1.left = pt1.left < pBuf1[i].x ? pt1.left : pBuf1[i].x;
		pt1.right = pt1.right > pBuf1[i].x ? pt1.right : pBuf1[i].x;

		pt1.top = pt1.top < pBuf1[i].y ? pt1.top : pBuf1[i].y;
		pt1.bottom = pt1.bottom > pBuf1[i].y ? pt1.bottom : pBuf1[i].y;
	}

	//���������Ӱ��
	pt1.fArea = sqrt(fabs(pt1.fArea * fVlue));
	pt1.ptMean.x *= fWidth;	
	pt1.ptMean.y *= fHeight;	

	pt1.left *= fWidth;
	pt1.right *= fWidth;
	pt1.top *= fHeight;
	pt1.bottom *= fHeight;

	pt1.ptMean.x /= nNum1;
	pt1.ptMean.y /= nNum1;
}

void CalProperty(PPOINT_3F pBuf1, int nNum1, PROPERTY& pt1, int nWidth, int nHeight)
{
	int i;
	
	
	float fVlue = 1.0 / (2 * nWidth * nHeight);
	float fWidth = 1.0 / nWidth;
	float fHeight = 1.0 / nHeight;
	
	pt1.fArea = 0;
	pt1.ptMean.x = pBuf1[0].x;
	pt1.ptMean.y = pBuf1[0].y * fHeight;
	
	pt1.left = pBuf1[0].x;
	pt1.right = pBuf1[0].x;
	pt1.top = pBuf1[0].y;
	pt1.bottom = pBuf1[0].y;
	
	for(i = 1; i < nNum1 - 1; ++i)
	{
		pt1.fArea += fabs(1.0*(pBuf1[i].x * pBuf1[i - 1].y - pBuf1[i - 1].x * pBuf1[i].y));
		pt1.ptMean.x += pBuf1[i].x;
		pt1.ptMean.y += pBuf1[i].y;
		
		pt1.left = pt1.left < pBuf1[i].x ? pt1.left : pBuf1[i].x;
		pt1.right = pt1.right > pBuf1[i].x ? pt1.right : pBuf1[i].x;
		
		pt1.top = pt1.top < pBuf1[i].y ? pt1.top : pBuf1[i].y;
		pt1.bottom = pt1.bottom > pBuf1[i].y ? pt1.bottom : pBuf1[i].y;
	}
	
	//���������Ӱ��
	pt1.fArea = sqrt(fabs(pt1.fArea * fVlue));
	pt1.ptMean.x *= fWidth;	
	pt1.ptMean.y *= fHeight;
	
	pt1.left *= fWidth;
	pt1.right *= fWidth;
	pt1.top *= fHeight;
	pt1.bottom *= fHeight;
	
	pt1.ptMean.x /= nNum1;
	pt1.ptMean.y /= nNum1;
}

// �����ε���Ӿ���
void get_rect(POINT* pPoly, int nNum, RECT& rect)
{

}

void get_rect(PPOINT2DF* pPoly, int nNum, RECT& rect)
{

}

// �����δ洢��ĳĿ�갴��˳ʱ��洢,�����ҡ�����Ϊ��
long clockwise(PPOINT pPts, int nNum)
{
	int i;
	long lArea = 0;
	for (i=1; i<nNum; i++)
	{
		lArea += pPts[i-1].x*pPts[i].y-pPts[i-1].y*pPts[i].x;
	}
	lArea += pPts[i-1].x*pPts[0].y-pPts[i-1].y*pPts[0].x;

	if (lArea < 0)
	{
		lArea = -lArea;
		// ����
		for (i=0; i<nNum/2; i++)
		{
			POINT pt;
			pt = pPts[i];
			pPts[i] = pPts[nNum-i-1];
			pPts[nNum-i-1] = pt;
		}
	}
	return lArea;
}

float clockwise(PPOINT2DF pPts, int nNum)
{
	float fArea = 0.0;
	for (int i=1; i<nNum; i++)
	{
		fArea += pPts[i-1].x*pPts[i].y-pPts[i-1].y*pPts[i].x;
	}
	fArea += pPts[i-1].x*pPts[0].y-pPts[i-1].y*pPts[0].x;

	if (fArea < 0)
	{
		fArea = -fArea;
		// ����
		for (i=0; i<nNum/2; i++)
		{
			POINT2DF pt;
			pt = pPts[i];
			pPts[i] = pPts[nNum-i-1];
			pPts[nNum-i-1] = pt;
		}
	}
	return fArea;
}

// ���ݵ�һ�㡢Բ�ļ���ת�Ƕ���ڶ�������
// ע��:�ǶȾ�Ϊ����, ����ת����Ϊ˳ʱ��
void get_arc_point(const POINT2DF& pt1, const POINT2DF& ptCenter,
							   double dbAngle, POINT2DF& pt2)
{
	float fRadius = get_distance(pt1, ptCenter);

	POINT2DF pt;
	double dbAngleA;

	// ��ptCenterΪԲ�ģ�����ֱ������ϵ(x����Ϊ��,y����Ϊ��)
	if (pt1.x<=ptCenter.x && pt1.y>=ptCenter.y)
	{
		// ���pt�ڵ�2���ޣ���pt��ptCenter�����Ͻǣ�
		dbAngleA = acos(BOUND((pt1.x-ptCenter.x)/fRadius,-1,1));
	}
	else if (pt1.x>=ptCenter.x && pt1.y>=ptCenter.y)
	{
		// ���pt�ڵ�1���ޣ���pt��ptCenter�����Ͻǣ�
		dbAngleA = acos(BOUND((pt1.x-ptCenter.x)/fRadius,-1,1));
	}
	else if (pt1.x>=ptCenter.x && pt1.y<= ptCenter.y)
	{
		// ���pt�ڵ�4���ޣ���pt��ptCenter�����½ǣ�
		dbAngleA = -acos(BOUND((pt1.x-ptCenter.x)/fRadius,-1,1));
	}
	else
	{
		// ���pt�ڵ�3���ޣ���pt��ptCenter�����½ǣ�
		dbAngleA = -PI+acos(BOUND((ptCenter.x-pt1.x)/fRadius,-1,1));
	}

	dbAngle = dbAngleA-dbAngle;

	pt2.x = ptCenter.x+fRadius*cos(dbAngle);
	pt2.y = ptCenter.y+fRadius*sin(dbAngle);
}


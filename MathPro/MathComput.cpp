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



// 求两线段的交点，有交点则返回TRUE，否则返回FALSE
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

	float k1, k2;	//两条线段的斜率
	float c1, c2;	//两线段的截距

	//如果两线段斜率都存在
	if(ptA1.x!=ptA2.x && ptB1.x!=ptB2.x)
	{
		// 如果两线段都是水平的，则没有交点或两线段重合
		if(ptA1.y==ptA2.y && ptB1.y==ptB2.y)
			return FALSE;

		// 斜率
		k1 = (ptA1.y-ptA2.y)*1.0/(ptA1.x - ptA2.x);
		k2 = (ptB1.y-ptB2.y)*1.0/(ptB1.x - ptB2.x);

		// 如果两直线平行
		if (k1-k2>0.000001 && k1-k2<0.000001)
			return FALSE;

		// 截距
		c1 = ptA1.y-k1*ptA1.x;
		c2 = ptB1.y-k2*ptB1.x;

		// 两直线交点
		interPt.x = (c2-c1)/(k1-k2);
		interPt.y = k1*interPt.x+c1;

		// 判断两直线交点是否在两线段上
		if(interPt.x>=min(ptA1.x, ptA2.x) && interPt.x<=max(ptA1.x, ptA2.x)
		&& interPt.x>=min(ptB1.x, ptB2.x) && interPt.x<=max(ptB1.x, ptB2.x))
			return TRUE;
		else
			return FALSE;
	}
	else if(ptA1.x==ptA2.x && ptB1.x!=ptB2.x)	//如果第一条直线竖直,第二条是斜线
	{
		k2 = (ptB1.y-ptB2.y)*1.0/(ptB1.x-ptB2.x);
		c2 = ptB1.y-k2*ptB1.x;

		interPt.x = ptA1.x;
		interPt.y = k2*interPt.x+c2;

		// 判断两直线交点是否在两线段上
		if(interPt.y>=min(ptA1.y, ptA2.y) && interPt.y<=max(ptA1.y, ptA2.y)
		&& interPt.y>=min(ptB1.y, ptB2.y) && interPt.y<=max(ptB1.y, ptB2.y))
			return TRUE;
		else
			return FALSE;
	}
	else if(ptA1.x!=ptA2.x && ptB1.x==ptB2.x)	//如果第二条直线竖直,第一条是斜线
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
	else// if(ptA1.x == ptA2.x && ptB1.x == ptB2.x)	//如果两直线斜率都不存在
	{
		return FALSE;
	}
}

// 获得线段与弧线的交点
/* ==================================================================
 * 函数名称：  get_inter_point()
 * 作者：      [Leng]
 * 创建于:     [2009-3-30 14:32:00]
 * 返回值：    [BOOL] - 0:无交点,1:有1个交点,2:有2个交点
 * 参数列表： 
               [const POINT2DF& pt1] - [In] 线段1的端点1
               [const POINT2DF& pt2] - [In] 线段1的端点2
               [const POINT2DF& ptArc1] - [In] 圆弧的端点1
               [const POINT2DF& ptCenter] - [In] 圆弧的圆心
               [const float& fAngle] - [In] 圆弧的张开弧度
               [const POINT2DF& ptArc2] - [In] 圆弧的端点2
               [POINT2DF& ptInter] - [In]
注释 :	返回值为0时，ptInter1和ptInter2都无效，返回值为1时，ptInter1
		有效，ptInter2无效，返回值为2时，ptInter1和ptInter2都有效
==================================================================*/
int get_inter_point(const POINT2DF& pt1, const POINT2DF& pt2,
					const POINT2DF& ptArc1, const POINT2DF& ptCenter,
					const POINT2DF& ptArc2,
					POINT2DF& ptInter1, POINT2DF& ptInter2)
{
	POINT2DF ptL1, ptL2, ptA1, ptO, ptA2, ptI;

	// 以圆弧的圆心ptCenter为原点重新建立坐标系
	ptL1 = pt1-ptCenter;
	ptL2 = pt2-ptCenter;
	ptA1 = ptArc1-ptCenter;
	ptA2 = ptArc2-ptCenter;
	ptO.x = ptO.y = 0;

	float r = get_distance(ptA1, ptO);

	// 求原点到直线的距离
	float fDist = point_line_dist(ptO, ptL1, ptL2);
	// 如果原点到直线的距离大于半径，则没有交点
	if (fDist > r)
		return 0;

	// 求直线方程及与圆的交点
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
	// 如果为垂线
	else
	{
		ptX1.x = ptL1.x;
		ptX1.y = sqrt(r*r-ptX1.x*ptX1.x);
		ptX2.x = ptX1.x;
		ptX2.y = -ptX1.y;
	}

	// 通过面积正负判断点是顺时针还是逆时针排序（顺时针为负，逆时针为正）
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
		// 如果两个值都大于0，则没有交点，否则有2个交点
		if (sum>0)
			return 0;
		// 如果两个交点实际为同一点，即只有1个交点，则返回1
		else if (ptInter1.x==ptInter2.x && ptInter1.y==ptInter2.y)
			return 1;
		else
			return 2;
	}
	else
	{
		// 如果只有sum大于0，则ptInter2为交点，赋给ptInter1
		if (sum>0)
			ptInter1 = ptInter2;
		return 1;
	}

}

// 求多边形区域面积
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

// 求多边形区域面积
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

// 计算两点的距离
float get_distance(POINT pt1, POINT pt2)
{
	return sqrt((pt1.x-pt2.x)*(pt1.x-pt2.x)+(pt1.y-pt2.y)*(pt1.y-pt2.y));
}

float get_distance(POINT2DF pt1, POINT2DF pt2)
{
	return sqrt((pt1.x-pt2.x)*(pt1.x-pt2.x)+(pt1.y-pt2.y)*(pt1.y-pt2.y));
}

// 计算点到直线的距离
// pt1,pt2分直线段的两端点
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

// 计算点到直线的距离
// pt1,pt2分直线段的两端点
float point_line_dist(const POINT2DF pt, const POINT2DF pt1, const POINT2DF pt2)
{
	float distance;
	float k;	//直线斜率
	float A, B, C;		//系数
	float fTemp;
	
	//如是直线两端点的横坐标相等
	if(pt1.x == pt2.x)
	{
		//如果直线的两个端点没有重合
		if(pt1.y != pt2.y)
		{
			if(pt.x >= pt1.x)
				distance = pt.x - pt1.x;
			else
				distance = pt1.x - pt.x;
		}
	}
	else	//如是直线两端点的横坐标不相等
	{
		//先求出直线的方程Ax + By + C = 0
		k = (pt1.y-pt2.y) * 1.0 / (pt1.x-pt2.x * 1.0);
		A = k;
		B = -1;
		C = pt1.y - k * pt1.x;
		
		fTemp = A * pt.x + B * pt.y + C;
		//求点到直线的距离
		if(fTemp >= 0)
			distance = fTemp / sqrt(A * A + B * B);
		else
			distance = - fTemp / sqrt(A * A + B * B);
	}
	
	return distance;
}

// 判断点是否在线段上
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
		k = (pt1.y - pt2.y) * 1.0 / (pt1.x - pt2.x);	//斜率
		c = pt1.y - k * pt1.x;		//截距
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

// 判断点对线段所做的垂足在线段上还是延长线上
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
	
	// 如果线段为垂线
	if (pt1.x == pt2.x)
	{
		ptVert.x = pt1.x;
		ptVert.y = point.y;
		return TRUE;
	}
	
	// 如果线段为水平线
	if (pt1.y == pt2.y)
	{
		ptVert.x = point.x;
		ptVert.y = pt1.y;
		return TRUE;
	}
	
	// 如果线段既不垂直也水平
	float k, b;
	k = (pt1.y-pt2.y)*1.0/(pt1.x-pt2.x);
	k = -1.0/k;
	b = point.y-k*point.x;
	
	// 求得与pt1,pt2所在线段垂直且经过point的直线上的另一个点
	POINT2DF point1;
	point1.x = point.x+10;
	point1.y = k*point1.x+b;

	// 求交点
	BOOL bRet;
	bRet = get_inter_point(pt1, pt2, point, point1, ptVert);
	
	return bRet;
}

// 计算∠ABC的弧度值
double get_angle(POINT ptA, POINT ptB, POINT ptC)
{
	double a = get_distance(ptB, ptC);
	double b = get_distance(ptA, ptC);
	double c = get_distance(ptA, ptB);

	double cosB = (b*b+c*c-a*a)/(2*b*c);
	double angleB = acos(cosB);

	return angleB;
}

// 计算∠ABC的弧度值
double get_angle(POINT2DF ptA, POINT2DF ptB, POINT2DF ptC)
{
	double a = get_distance(ptB, ptC);
	double b = get_distance(ptA, ptC);
	double c = get_distance(ptA, ptB);

	double cosB = (a*a+c*c-b*b)/(2*a*c);
	double angleB = acos(cosB);

	return angleB;
}

// 计算以端点pt1、pt2为端点的直线的倾斜角度
float get_angle(POINT pt1, POINT pt2)
{
	POINT2DF ptD1, ptD2;
	ptD1.x = pt1.x;	ptD1.y = pt1.y;
	ptD2.x = pt2.x; ptD2.y = pt2.y;

	return get_angle(ptD1, ptD2);
}

// 计算以端点pt1、pt2为端点的直线的倾斜弧度
// 倾斜角度在[0,180)范围内
float get_angle(POINT2DF pt1, POINT2DF pt2)
{
	ASSERT(pt1.x!=pt2.x || pt1.y!=pt2.y);

	float k;	//直线斜率
	float fAngle;	// 返回角度值
	
	//如是直线两端点的横坐标相等
	if(pt1.x == pt2.x)
	{
		//如果直线的两个端点没有重合
		fAngle = 90.0;
	}
	else	//如是直线两端点的横坐标不相等
	{
		//先求出直线的方程Ax + By + C = 0
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

	//减弱面积的影响
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

	//减弱面积的影响
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
	
	//减弱面积的影响
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

// 求多边形的外接矩形
void get_rect(POINT* pPoly, int nNum, RECT& rect)
{

}

void get_rect(PPOINT2DF* pPoly, int nNum, RECT& rect)
{

}

// 将依次存储的某目标按点顺时针存储,以向右、向下为正
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
		// 反向
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
		// 反向
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

// 根据第一点、圆心及旋转角度求第二点坐标
// 注意:角度均为弧度, 且旋转方向为顺时针
void get_arc_point(const POINT2DF& pt1, const POINT2DF& ptCenter,
							   double dbAngle, POINT2DF& pt2)
{
	float fRadius = get_distance(pt1, ptCenter);

	POINT2DF pt;
	double dbAngleA;

	// 以ptCenter为圆心，建立直角坐标系(x向右为正,y向上为正)
	if (pt1.x<=ptCenter.x && pt1.y>=ptCenter.y)
	{
		// 如果pt在第2象限（即pt在ptCenter的左上角）
		dbAngleA = acos(BOUND((pt1.x-ptCenter.x)/fRadius,-1,1));
	}
	else if (pt1.x>=ptCenter.x && pt1.y>=ptCenter.y)
	{
		// 如果pt在第1象限（即pt在ptCenter的右上角）
		dbAngleA = acos(BOUND((pt1.x-ptCenter.x)/fRadius,-1,1));
	}
	else if (pt1.x>=ptCenter.x && pt1.y<= ptCenter.y)
	{
		// 如果pt在第4象限（即pt在ptCenter的右下角）
		dbAngleA = -acos(BOUND((pt1.x-ptCenter.x)/fRadius,-1,1));
	}
	else
	{
		// 如果pt在第3象限（即pt在ptCenter的左下角）
		dbAngleA = -PI+acos(BOUND((ptCenter.x-pt1.x)/fRadius,-1,1));
	}

	dbAngle = dbAngleA-dbAngle;

	pt2.x = ptCenter.x+fRadius*cos(dbAngle);
	pt2.y = ptCenter.y+fRadius*sin(dbAngle);
}


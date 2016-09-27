// MathComput.h: interface for the CMathComput class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATHCOMPUT_H)
#define AFX_MATHCOMPUT_H

#include "TilPoint.h"
#include "MathDef.h"

#define BOUND(b,a1,a2) (b<a1)?a1:(b>a2)?a2:b

// 获得两线段的交点，有则返回TRUE，否则返回FALSE
AFX_EXT_API BOOL get_inter_point(POINT ptA1, POINT ptA2, POINT ptB1, POINT ptB2, POINT& interPt);
AFX_EXT_API BOOL get_inter_point(POINT2DF ptA1, POINT2DF ptA2, POINT2DF ptB1, POINT2DF ptB2, POINT2DF& interPt);
// 获得线段与弧线的交点
AFX_EXT_API int get_inter_point(const POINT2DF& pt1, const POINT2DF& pt2,
								const POINT2DF& ptArc1, const POINT2DF& ptCenter,
								const POINT2DF& ptArc2,
								POINT2DF& ptInter1, POINT2DF& ptInter2);
// 求多边形区域面积
AFX_EXT_API double get_poly_area(POINT* pPoly, int nNum);
AFX_EXT_API double get_poly_area(POINT2DF* pPoly, int nNum);
// 计算两点的距离
AFX_EXT_API float get_distance(POINT pt1, POINT pt2);
AFX_EXT_API float get_distance(POINT2DF pt1, POINT2DF pt2);
// 计算点到直线的距离
AFX_EXT_API float point_line_dist(const POINT pt, const POINT pt1, const POINT pt2);
AFX_EXT_API float point_line_dist(const POINT2DF pt, const POINT2DF pt1, const POINT2DF pt2);
// 判断点是否在线段上
AFX_EXT_API int point_in_line(POINT point, POINT pt1, POINT pt2);
AFX_EXT_API int point_in_line(POINT2DF point, POINT2DF pt1, POINT2DF pt2);
// 判断点对线段所做的垂足在线段上还是延长线上
//AFX_EXT_API BOOL get_vert_point_in_line(const POINT point, const POINT pt1, const POINT pt2, POINT& ptVert);
AFX_EXT_API BOOL get_vert_point_in_line(const POINT2DF point, const POINT2DF pt1, const POINT2DF pt2, POINT2DF& ptVert);
// 计算∠ABC的弧度值
AFX_EXT_API double get_angle(POINT ptA, POINT ptB, POINT ptC);
AFX_EXT_API double get_angle(POINT2DF ptA, POINT2DF ptB, POINT2DF ptC);
// 计算直线的倾斜弧度
AFX_EXT_API float get_angle(POINT pt1, POINT pt2);
AFX_EXT_API float get_angle(POINT2DF pt1, POINT2DF pt2);
// 将依次存储的某目标按点顺时针存储
AFX_EXT_API long clockwise(PPOINT pPts, int nNum);
AFX_EXT_API float clockwise(PPOINT2DF pPts, int nNum);
// 根据第一点、圆心及旋转角度求第二点坐标
AFX_EXT_API void get_arc_point(const POINT2DF& pt1, const POINT2DF& ptCenter,
							   double dbAngle, POINT2DF& pt2);


//////////////////////////////////////////////////////////////////////////
// 计算由顶点构成的多边形的属性
typedef struct __PROPERTY
{
	POINT2DF ptMean;				//重心坐标
	double  fArea;					//面积
	double left,right,top,bottom;	//外接矩形边界
	double fPrmDivArea;				//周长与面积之比
} PROPERTY;

// 计算机多边形目标的属性
AFX_EXT_API BOOL CalProperty(POINT* pBuf, int nNum, PROPERTY& pt, int nWidth, int nHeight);
AFX_EXT_API void CalProperty(PPOINT2DF pBuf, int nNum, PROPERTY& pt, int nWidth, int nHeight);
AFX_EXT_API void CalProperty(PPOINT_3F pBuf, int nNum, PROPERTY& pt, int nWidth, int nHeight);
// 求多边形的外接矩形
AFX_EXT_API void get_rect(POINT* pPoly, int nNum, RECT& rect);
AFX_EXT_API void get_rect(PPOINT2DF* pPoly, int nNum, RECT& rect);
#endif // !defined(AFX_MATHCOMPUT_H__E9C94228_FD53_4C37_BF2C_46A6A49FB9E9__INCLUDED_)

#ifndef SPACE_RELATION_H
#define SPACE_RELATION_H

#include "TilPoint.h"
#include "MathDef.h"

AFX_EXT_API bool is_a_line(const POINT_3F& pt1, const POINT_3F& pt2);
// 根据两点取得一条直线的参数
AFX_EXT_API void get_a_line(const POINT_3F& pt1, const POINT_3F& pt2, float* pLine);
// 测试一个点是否在某平面上
AFX_EXT_API bool is_point_in_face(float* pFace, const POINT_3F& point);
// 判断一个点是否在线段上
AFX_EXT_API int point_in_line(const POINT_3F& pt, const POINT_3F& pt1, const POINT_3F& pt2);
// 测试三点是否构成一个平面
AFX_EXT_API bool is_a_face(const POINT_3F& pt1, const POINT_3F& pt2, const POINT_3F& pt3);
// 根据三点式求平面
AFX_EXT_API void get_a_face(const POINT_3F& pt1, const POINT_3F& pt2, const POINT_3F& pt3, float* pFace);
// 得到多边形所在的平面
AFX_EXT_API void get_a_face(const PPOINT_3F pPolygon, int num, float* pFace);
// 测试许多个点是否在一个平面上
AFX_EXT_API bool is_a_face(const PPOINT_3F pPolygon, int num);
// 求点到平面的距离
AFX_EXT_API float point_line_distance(float* pFace, const POINT_3F& point);
// 求线段与平面是否有交点，若有，则求出该交点
AFX_EXT_API int get_line_face_interpoint(const POINT_3F& pt1, const POINT_3F& pt2, 
										 float* pFace, POINT_3F& point);
// 求一个平面旋转一个角度后的平面,旋转方向根据右手判别准则绕给定的直线段进行
AFX_EXT_API void rotate_face(const POINT_3F& ptAxisHead, const POINT_3F& ptAxisRear,
							  const POINT_3F& ptSrc, float fAngle, POINT_3F& ptDst);
// 求一个平面与另一个平面上的多边形的交点
AFX_EXT_API void get_faces_intersections(float* pFace, 
										POINT_3F* pPolygon, int num,
										PPPOINT_3F ppPts, int& ptNum);
// 获得两点间的距离
AFX_EXT_API float get_distance(const POINT_3F& pt1, const POINT_3F& pt2);
// 对多边形边界上的点沿边界线进行排序(这些点指一个平面与一多边形边界的交点)
AFX_EXT_API void sort_intersections(PPOINT_3F pPolygon, int num, PPOINT_3F pPts, int ptNum);
// 如果三个面的交点存在,则求出这三个面的交点
AFX_EXT_API bool get_3faces_interpoint(float* fFace1, float* fFace2, float* fFace3, POINT_3F& pt);



typedef struct __tagCROSSPOINT
{
	// nS为顶点标志值(0:表示(lX,lY)为入边交点,1:表示(lX,lY)
	// 为出边交点,2:表示(lX,lY)为多边形在圆内的点,3:记录的圆心及半径
	int nS;
	double x, y, z;	// 点或圆心
	float r;		// 半径
	struct __tagCROSSPOINT *next;
} CROSSPOINT, *PCROSSPOINT, **PPCROSSPOINT;

// 求多边形与圆的交集
AFX_EXT_API PCROSSPOINT poly_circle_section(const POINT2DF& ptCenter, float fRadius, 
											const POINT2DF* pPolygon, int nVerNum);
// 判断点是否在多边形中
AFX_EXT_API bool pt_in_polygon(const POINT2DF& pt, const POINT2DF* pPolygon, int nVerNum);


// 计算空间点绕任意向量旋转后的坐标
int rotate_point(double *p1,double *p2,double *rotateP,double sita,double *pAftefRotate);

#endif
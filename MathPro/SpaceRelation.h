#ifndef SPACE_RELATION_H
#define SPACE_RELATION_H

#include "TilPoint.h"
#include "MathDef.h"

AFX_EXT_API bool is_a_line(const POINT_3F& pt1, const POINT_3F& pt2);
// ��������ȡ��һ��ֱ�ߵĲ���
AFX_EXT_API void get_a_line(const POINT_3F& pt1, const POINT_3F& pt2, float* pLine);
// ����һ�����Ƿ���ĳƽ����
AFX_EXT_API bool is_point_in_face(float* pFace, const POINT_3F& point);
// �ж�һ�����Ƿ����߶���
AFX_EXT_API int point_in_line(const POINT_3F& pt, const POINT_3F& pt1, const POINT_3F& pt2);
// ���������Ƿ񹹳�һ��ƽ��
AFX_EXT_API bool is_a_face(const POINT_3F& pt1, const POINT_3F& pt2, const POINT_3F& pt3);
// ��������ʽ��ƽ��
AFX_EXT_API void get_a_face(const POINT_3F& pt1, const POINT_3F& pt2, const POINT_3F& pt3, float* pFace);
// �õ���������ڵ�ƽ��
AFX_EXT_API void get_a_face(const PPOINT_3F pPolygon, int num, float* pFace);
// �����������Ƿ���һ��ƽ����
AFX_EXT_API bool is_a_face(const PPOINT_3F pPolygon, int num);
// ��㵽ƽ��ľ���
AFX_EXT_API float point_line_distance(float* pFace, const POINT_3F& point);
// ���߶���ƽ���Ƿ��н��㣬���У�������ý���
AFX_EXT_API int get_line_face_interpoint(const POINT_3F& pt1, const POINT_3F& pt2, 
										 float* pFace, POINT_3F& point);
// ��һ��ƽ����תһ���ǶȺ��ƽ��,��ת������������б�׼���Ƹ�����ֱ�߶ν���
AFX_EXT_API void rotate_face(const POINT_3F& ptAxisHead, const POINT_3F& ptAxisRear,
							  const POINT_3F& ptSrc, float fAngle, POINT_3F& ptDst);
// ��һ��ƽ������һ��ƽ���ϵĶ���εĽ���
AFX_EXT_API void get_faces_intersections(float* pFace, 
										POINT_3F* pPolygon, int num,
										PPPOINT_3F ppPts, int& ptNum);
// ��������ľ���
AFX_EXT_API float get_distance(const POINT_3F& pt1, const POINT_3F& pt2);
// �Զ���α߽��ϵĵ��ر߽��߽�������(��Щ��ָһ��ƽ����һ����α߽�Ľ���)
AFX_EXT_API void sort_intersections(PPOINT_3F pPolygon, int num, PPOINT_3F pPts, int ptNum);
// ���������Ľ������,�������������Ľ���
AFX_EXT_API bool get_3faces_interpoint(float* fFace1, float* fFace2, float* fFace3, POINT_3F& pt);



typedef struct __tagCROSSPOINT
{
	// nSΪ�����־ֵ(0:��ʾ(lX,lY)Ϊ��߽���,1:��ʾ(lX,lY)
	// Ϊ���߽���,2:��ʾ(lX,lY)Ϊ�������Բ�ڵĵ�,3:��¼��Բ�ļ��뾶
	int nS;
	double x, y, z;	// ���Բ��
	float r;		// �뾶
	struct __tagCROSSPOINT *next;
} CROSSPOINT, *PCROSSPOINT, **PPCROSSPOINT;

// ��������Բ�Ľ���
AFX_EXT_API PCROSSPOINT poly_circle_section(const POINT2DF& ptCenter, float fRadius, 
											const POINT2DF* pPolygon, int nVerNum);
// �жϵ��Ƿ��ڶ������
AFX_EXT_API bool pt_in_polygon(const POINT2DF& pt, const POINT2DF* pPolygon, int nVerNum);


// ����ռ��������������ת�������
int rotate_point(double *p1,double *p2,double *rotateP,double sita,double *pAftefRotate);

#endif
// MathComput.h: interface for the CMathComput class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATHCOMPUT_H)
#define AFX_MATHCOMPUT_H

#include "TilPoint.h"
#include "MathDef.h"

#define BOUND(b,a1,a2) (b<a1)?a1:(b>a2)?a2:b

// ������߶εĽ��㣬���򷵻�TRUE�����򷵻�FALSE
AFX_EXT_API BOOL get_inter_point(POINT ptA1, POINT ptA2, POINT ptB1, POINT ptB2, POINT& interPt);
AFX_EXT_API BOOL get_inter_point(POINT2DF ptA1, POINT2DF ptA2, POINT2DF ptB1, POINT2DF ptB2, POINT2DF& interPt);
// ����߶��뻡�ߵĽ���
AFX_EXT_API int get_inter_point(const POINT2DF& pt1, const POINT2DF& pt2,
								const POINT2DF& ptArc1, const POINT2DF& ptCenter,
								const POINT2DF& ptArc2,
								POINT2DF& ptInter1, POINT2DF& ptInter2);
// �������������
AFX_EXT_API double get_poly_area(POINT* pPoly, int nNum);
AFX_EXT_API double get_poly_area(POINT2DF* pPoly, int nNum);
// ��������ľ���
AFX_EXT_API float get_distance(POINT pt1, POINT pt2);
AFX_EXT_API float get_distance(POINT2DF pt1, POINT2DF pt2);
// ����㵽ֱ�ߵľ���
AFX_EXT_API float point_line_dist(const POINT pt, const POINT pt1, const POINT pt2);
AFX_EXT_API float point_line_dist(const POINT2DF pt, const POINT2DF pt1, const POINT2DF pt2);
// �жϵ��Ƿ����߶���
AFX_EXT_API int point_in_line(POINT point, POINT pt1, POINT pt2);
AFX_EXT_API int point_in_line(POINT2DF point, POINT2DF pt1, POINT2DF pt2);
// �жϵ���߶������Ĵ������߶��ϻ����ӳ�����
//AFX_EXT_API BOOL get_vert_point_in_line(const POINT point, const POINT pt1, const POINT pt2, POINT& ptVert);
AFX_EXT_API BOOL get_vert_point_in_line(const POINT2DF point, const POINT2DF pt1, const POINT2DF pt2, POINT2DF& ptVert);
// �����ABC�Ļ���ֵ
AFX_EXT_API double get_angle(POINT ptA, POINT ptB, POINT ptC);
AFX_EXT_API double get_angle(POINT2DF ptA, POINT2DF ptB, POINT2DF ptC);
// ����ֱ�ߵ���б����
AFX_EXT_API float get_angle(POINT pt1, POINT pt2);
AFX_EXT_API float get_angle(POINT2DF pt1, POINT2DF pt2);
// �����δ洢��ĳĿ�갴��˳ʱ��洢
AFX_EXT_API long clockwise(PPOINT pPts, int nNum);
AFX_EXT_API float clockwise(PPOINT2DF pPts, int nNum);
// ���ݵ�һ�㡢Բ�ļ���ת�Ƕ���ڶ�������
AFX_EXT_API void get_arc_point(const POINT2DF& pt1, const POINT2DF& ptCenter,
							   double dbAngle, POINT2DF& pt2);


//////////////////////////////////////////////////////////////////////////
// �����ɶ��㹹�ɵĶ���ε�����
typedef struct __PROPERTY
{
	POINT2DF ptMean;				//��������
	double  fArea;					//���
	double left,right,top,bottom;	//��Ӿ��α߽�
	double fPrmDivArea;				//�ܳ������֮��
} PROPERTY;

// ����������Ŀ�������
AFX_EXT_API BOOL CalProperty(POINT* pBuf, int nNum, PROPERTY& pt, int nWidth, int nHeight);
AFX_EXT_API void CalProperty(PPOINT2DF pBuf, int nNum, PROPERTY& pt, int nWidth, int nHeight);
AFX_EXT_API void CalProperty(PPOINT_3F pBuf, int nNum, PROPERTY& pt, int nWidth, int nHeight);
// �����ε���Ӿ���
AFX_EXT_API void get_rect(POINT* pPoly, int nNum, RECT& rect);
AFX_EXT_API void get_rect(PPOINT2DF* pPoly, int nNum, RECT& rect);
#endif // !defined(AFX_MATHCOMPUT_H__E9C94228_FD53_4C37_BF2C_46A6A49FB9E9__INCLUDED_)

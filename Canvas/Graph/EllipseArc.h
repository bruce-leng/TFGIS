#ifndef	CELLIPSEARC
#define CELLIPSEARC
#include "Unit.h"
#define ROUND(a) ((int)(a+0.5))
class AFX_EXT_CLASS CEllipseArc:public CUnit
{
 DECLARE_SERIAL(CEllipseArc)

public:
	CEllipseArc();
	virtual void Serialize(CArchive &ar);
	virtual void OnContextMenu(CWnd *pWnd, CPoint point);
	virtual void OnMenuitemCirCu(CDC*pDC,PGRAPHCAPTION pCaption);
	virtual void OnMouseMove(CDC*pDC,PGRAPHCAPTION pCaption,CPoint point);
	virtual void OnLButtonDown(CDC *pDC, PGRAPHCAPTION pCaption, CPoint point);
	virtual void ShowSelectPoint(CDC *pDC);
	virtual int IsOnMarginPoint(CPoint point);
	
	virtual BOOL IsInRgn(CPoint point);
	virtual CPoint ComputeIntersectionPointSecond(CPoint point);
	virtual CPoint ComputeIntersectionPointFirst(CPoint point);
	virtual void DrawStatic(CDC*pDC);
	virtual void DrawActive(CDC *pDC,CPoint point);
	virtual void DrawStaticStepOne(CDC*pDC,CPoint movingPoint);
	virtual void PdcSetPixel(CDC *pDC, int xCenter, int yCenter, COLORREF m_PenColor);
	virtual void ellipsePlotPoints(CDC*pDC,int xCenter, int yCenter, int x, int y);
	virtual void ellipseMidpoint(CDC*pDC,int xCenter, int yCenter, int Rx, int Ry);
	virtual float ComputSloap(CPoint firstPoint, CPoint secondPoint);
	virtual int  ComputRadiusY(CPoint firstPoint,CPoint secondPoint);
	virtual int ComputRadiusX(CPoint firstPoint,CPoint secondPoint);
	virtual void DrawActiveStepOne(CDC *pDC,CPoint movingPoint);

	BOOL m_IsDrawingArc;
	double m_CurrentAngle;
	BOOL   m_IsCirCu;
};
#endif
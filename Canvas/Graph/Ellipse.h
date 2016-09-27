#ifndef CELLIPSE
#define CELLIPSE
#include "Unit.h"
#define ROUND(a) ((int)(a+0.5))
class AFX_EXT_CLASS CEllipse:public CUnit
{
public:
	CEllipse();
	
	virtual void Serialize(CArchive &ar);
	virtual void DrawStatic(CDC*pDC);
	virtual void OnContextMenu(CWnd *pWnd, CPoint point);
	virtual void OnMenuitemCirCu(CDC *pDC, PGRAPHCAPTION pCaption);
	virtual void OnMouseMove(CDC*pDC,PGRAPHCAPTION pCaption,CPoint point);
	virtual void OnLButtonDown(CDC *pDC, PGRAPHCAPTION pCaption, CPoint point);
	virtual void DrawActive(CDC *pDC,CPoint point);
	virtual CRect GetBoundingRect();
	virtual void ShowSelectPoint(CDC*pDC);
	virtual void ComputeOffSet();
	virtual void ShowMovingLine(CDC*pDC,CPoint firstPoint, CPoint secondPoint);
	virtual int ComputRadiusY(CPoint firstPoint,CPoint secondPoint);
	virtual int ComputRadiusX(CPoint firstPoint,CPoint secondPoint);
	virtual void PdcSetPixel(CDC*pDC,int xCenter,int yCenter,COLORREF m_PenColor);
	
	virtual float ComputSloap(CPoint firstPoint, CPoint secondPoint);
	virtual void ellipsePlotPoints(CDC*pDC,int xCenter, int yCenter, int x, int y);
	virtual void ellipseMidpoint(CDC*pDC,int xCenter,int yCenter,int Rx,int Ry);
	virtual BOOL IsInRgn(CPoint point);
	virtual int IsOnMarginPoint(CPoint point);

public:
  CPoint  m_MarginPoint[5];
	int m_OffSetX;
	int m_OffSetY;
	BOOL m_IsCirCu;
	
	DECLARE_SERIAL(CEllipse)
};
#endif

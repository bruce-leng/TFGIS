#ifndef POLYGON
#define POLYGON
#include "Unit.h"
class AFX_EXT_CLASS CPolygon:public CUnit
{
	DECLARE_SERIAL(CPolygon)
public:
	CPolygon();
	CPolygon(CPolygon& polygon);
	virtual ~CPolygon();
	virtual void Initial();
	void Serialize(CArchive &ar);

	virtual void OnMouseMove(CDC*pDC,PGRAPHCAPTION pCaption,CPoint point);
	virtual void OnLButtonDown(CDC *pDC, PGRAPHCAPTION pCaption, CPoint point);
	virtual void DrawActive(CDC *pDC,CPoint point);
	virtual void ShowSelectPoint(CDC *pDC);
	virtual CRect GetBoundingRect();

	// 改变某个边界点的x位置
	virtual void ExchangeDraw(CDC *pDC, CPoint point);
	virtual BOOL IsInRgn(CPoint point);
	virtual int IsOnMarginPoint(CPoint point);
	// 若为封闭多边形,画出最后一点和第一点之间的线段,否则不画
	virtual void DrawEnd(CDC*pDC,CPoint point);
	// 画出当前鼠标移动时鼠标点和最后一点间的线段
	virtual void DrawMask(CDC*pDC,CPoint point);
	virtual void DrawStatic(CDC*pDC);
	// virtual void DrawStaticVInvert(CDC* pDC);	// 画垂直翻转多段线(追加2014.04.15)
	virtual int GetPointNum();
	virtual POINT2DF GetPoint(int nIdx);
	virtual RECT GetPolyRect();
	// 将宽度缩放dbZoomX比率,高度缩放dbZoomY,将缩放后的尺寸设为原始尺寸(08.09.17)
	virtual void Reset(long double dbZoomX, long double dbZoomY);
};
#endif

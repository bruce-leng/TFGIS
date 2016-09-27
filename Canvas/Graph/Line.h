#ifndef LINE
#define LINE
#include "Unit.h"
class AFX_EXT_CLASS CLine:public CUnit
{
public:
	CLine();
	CLine(CLine& line);
	virtual void Initial();
	virtual CRgn* GetRgn();
	virtual void OnMouseMove(CDC*pDC,PGRAPHCAPTION pCaption,CPoint point);
	virtual void OnLButtonDown(CDC *pDC, PGRAPHCAPTION pCaption, CPoint point);
	virtual void DrawStatic(CDC*pDC);
	virtual void DrawActive(CDC *pDC,CPoint point);
	virtual BOOL IsInRgn(CPoint point);
	virtual void ShowSelectPoint(CDC*pDC);
	virtual void Circumrotate(CPoint first,CPoint second);
	virtual int IsOnMarginPoint(CPoint point);
	DECLARE_SERIAL(CLine)
};
#endif

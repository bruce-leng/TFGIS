#ifndef CROUNDARC
#define CROUNDARC
#include"Unit.h"
class AFX_EXT_CLASS CRoundArc:public CUnit
{
	DECLARE_SERIAL(CRoundArc)
public:
	CRoundArc();
	virtual void Serialize(CArchive &ar);
	virtual void OnMouseMove(CDC*pDC,PGRAPHCAPTION pCaption,CPoint point);
	virtual void OnLButtonDown(CDC *pDC, PGRAPHCAPTION pCaption, CPoint point);
	virtual int IsOnMarginPoint(CPoint point);
	virtual void ShowSelectPoint(CDC*pDC);
	virtual BOOL IsInRgn(CPoint point);
	virtual void DrawActive(CDC *pDC,CPoint point);
	virtual double ComputeSloap2(CPoint firstpoint,CPoint secondpoint);
	virtual double ComputeSloap1(CPoint firstpoint,CPoint secondpoint);
	virtual void DrawStatic(CDC*pDC);
};
#endif

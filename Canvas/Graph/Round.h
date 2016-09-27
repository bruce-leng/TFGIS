#ifndef CROUND
#define CROUND
#include "Unit.h"
class AFX_EXT_CLASS CRound:public CUnit
{
  DECLARE_SERIAL(CRound)
public:
	CRound();
	virtual void Serialize(CArchive &ar);

	virtual void OnLButtonDown(CDC *pDC, PGRAPHCAPTION pCaption, CPoint point);
	virtual void OnMouseMove(CDC*pDC,PGRAPHCAPTION pCaption,CPoint point);
	virtual void DrawActive(CDC *pDC,CPoint point);
	virtual void ShowSelectPoint(CDC *pDC);
	virtual int IsOnMarginPoint(CPoint point);
	virtual BOOL IsInRgn(CPoint point);
	virtual int ComputRadious(CPoint centerPoint, CPoint movingPoint);
	virtual void  DrawStatic(CDC*pDC);
};
#endif
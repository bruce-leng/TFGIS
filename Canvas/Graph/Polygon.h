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

	// �ı�ĳ���߽���xλ��
	virtual void ExchangeDraw(CDC *pDC, CPoint point);
	virtual BOOL IsInRgn(CPoint point);
	virtual int IsOnMarginPoint(CPoint point);
	// ��Ϊ��ն����,�������һ��͵�һ��֮����߶�,���򲻻�
	virtual void DrawEnd(CDC*pDC,CPoint point);
	// ������ǰ����ƶ�ʱ��������һ�����߶�
	virtual void DrawMask(CDC*pDC,CPoint point);
	virtual void DrawStatic(CDC*pDC);
	// virtual void DrawStaticVInvert(CDC* pDC);	// ����ֱ��ת�����(׷��2014.04.15)
	virtual int GetPointNum();
	virtual POINT2DF GetPoint(int nIdx);
	virtual RECT GetPolyRect();
	// ���������dbZoomX����,�߶�����dbZoomY,�����ź�ĳߴ���Ϊԭʼ�ߴ�(08.09.17)
	virtual void Reset(long double dbZoomX, long double dbZoomY);
};
#endif

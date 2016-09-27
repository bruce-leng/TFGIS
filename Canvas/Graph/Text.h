#ifndef	CTEXT
#define CTEXT
#include"Unit.h"
class CTextEdit;	
class AFX_EXT_CLASS CText :public CUnit
{
DECLARE_SERIAL(CText)

public:
	CText();
	CText(CText& text);
	~CText();
	virtual void ShowSelectPoint(CDC*pDC);
	virtual int IsOnMarginPoint(CPoint point);
	virtual BOOL IsInRgn(CPoint point);
	void OnMenuitemOk(CDC *pDC, PGRAPHCAPTION pCaption);
	virtual void OnMouseMove(CDC*pDC,PGRAPHCAPTION pCaption,CPoint point);
	virtual void DrawStatic(CDC*pDC);
	virtual void ChangeFontSize(ENUMLOGFONT *lpelf);
	virtual void ChangeFont(ENUMLOGFONT *lpelf);
	virtual void SetFont(ENUMLOGFONT *lpelf);
	
	void SetTextEditPosition(CDC*pDC,PGRAPHCAPTION pCaption,CPoint point);
	virtual void OnLButtonDown(CDC*pDC,PGRAPHCAPTION pCaption,CPoint point);
	virtual void Serialize(CArchive &ar);
	CString m_String;
	BOOL m_HaveFinish;
	BOOL m_bHaveCreat;
	long m_Width;
	long m_Hight;
	POINT2DF m_PositionPoint;	// Ô­Ê¼µã
};
#endif
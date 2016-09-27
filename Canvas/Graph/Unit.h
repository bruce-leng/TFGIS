# ifndef UNIT
# define UNIT

#include "TilPoint.h"

class CDLineProperty;
class CUnit;
typedef struct tagGraphCaption
{
	int m_CurrentDrawTool;
	int	m_CurrentDrawStatus;
	CClientDC *m_pDrawDC;
	POINT2DF m_FirstPoint;
	POINT2DF m_SecondPoint;
	CUnit *m_pCurrentUnit;
	BOOL m_IsDrawing;
	CDLineProperty* m_pDLineProperty;
	BOOL m_IsPolygonFirst;
	CBitmap *m_pBitmap;
	int m_LBDownTimes;
	BOOL m_bHaveFindFirst;
	BOOL m_bHaveFindSecond;
	int L_iPointCount;
	POINT2DF m_LastDownPoint;
	POINT2DF m_MinPoint;
	POINT2DF m_MaxPoint;
	CRect  m_InvalidRect;		// 刷新的目标区域
	POINT2DF m_ChangeingPoint;
	CString string;
	POINT2DF m_CenterPoint;
	BOOL m_HaveAxis;
	BOOL m_IsLocked;
	int  m_Rtimes;
	int  m_Index;
	BOOL m_IsMouseCap;
	BOOL m_IsLastMove;
	BOOL m_bClose;	// 标记多边形是否封闭(08.09.15)
} GRAPHCAPTION, *PGRAPHCAPTION, **PPGRAPHCAPTION;

#include "GraphDefine.h"
#include "TILdef.h"
#include <afxtempl.h>
#include <math.h>
#include"resource.h"
class CTextEdit;
class AFX_EXT_CLASS CUnit:public CObject
{
public:
	DECLARE_SERIAL(CUnit)
	UINT m_PenStyle;
	int m_PenWidth;
	int m_BkMode;
	int m_DrawingMode;
	COLORREF m_PenColor;
	COLORREF m_BrushColor;
	COLORREF m_BackgroundColor;
	POINT2DF m_FirstPoint;	// 移动前的点
	POINT2DF m_SecondPoint;	// 移动后的点
	POINT2DF m_ThirdPoint;
	POINT2DF m_CenterPoint;
	POINT2DF m_FourthPoint;
	POINT2DF m_FifthPoint;
	POINT2DF m_MovingPoint;
	POINT2DF m_MinPoint;
	POINT2DF m_MaxPoint;
	POINT2DF m_IntersectionPointFirst;
	POINT2DF m_IntersectionPointSecond;
	CRect  m_BoundingRect;
	CRgn   *m_pRgn;
	CArray<POINT2DF,POINT2DF>*m_pPointList;	// 原始点
	CArray<POINT2DF,POINT2DF>m_PointArray;
	UINT m_BrushStyle;
	LONG m_BrushHatch;
	int  m_FoundIndex;
	POINT2DF m_FoundPoint;// 原始点
	int m_Radious;
	int    m_Xr;
	int    m_Yr;
	BOOL   m_HaveLBUp;
	double m_Cos;
	double m_Sin;
	double m_Sloap1;
	double m_Sloap2;
	double m_AngleFirst;
	double m_AngleSecond;
	BOOL  m_HaveFindFirst;
	CTextEdit* m_pTextEdit;
	CView* m_pView;			// 文本所在视图(08.03.11)
	CFont *m_pFont;
	int m_DrawStatus;
	int m_nFillMapIdx;//填充图元序号
	
public:
	CUnit();
	CUnit(CUnit&unit);
	CUnit operator=(CUnit&unit);
	virtual ~CUnit();
	virtual void Initial();
	virtual void Serialize(CArchive&ar);
	virtual void DrawStatic(CDC*pDC){};

	void OnMENUITEMSize();
	void SetBrushOrg(CDC*pDC,CBrush*brush);
	virtual void OnMenuitemCirCu(CDC*pDC,PGRAPHCAPTION pCaption);
	virtual void OnMenuitemOk(CDC *pDC, PGRAPHCAPTION pCaption);
	virtual void OnContextMenu(CWnd* pWnd, CPoint point);
	virtual void OnLButtonDown(CDC*pDC,PGRAPHCAPTION pCaption,CPoint point);
	virtual void OnLButtonDblClk(CDC*pDC,PGRAPHCAPTION pCaption,CPoint point);
	virtual void OnMouseMove(CDC*pDC,PGRAPHCAPTION pCaption,CPoint point);

	virtual void ChangeFontSize(ENUMLOGFONT*lpelf);
	virtual void ChangeFont(ENUMLOGFONT *lpelf);
	virtual void SetFont(ENUMLOGFONT* lpelf);

	virtual CPoint ComputeIntersectionPointSecond(CPoint point);
	virtual CPoint ComputeIntersectionPointFirst(CPoint point);

	virtual void DrawStaticStepOne(CDC*pDC,CPoint movingPoint);
	virtual void DrawActiveStepOne(CDC*pDC,CPoint movingPoint);
	virtual void DrawActive(CDC*pDC,CPoint point);
	virtual void DrawOldReferencePoint(CDC*pDC,CPoint point);

	virtual double ComputeSloap2(CPoint firstpoint,CPoint secondpoint);
	virtual double ComputeSloap1(CPoint firstpoint,CPoint secondpoint);

	virtual void ShowSelectPoint(CDC*pDC);
	virtual void ShowMovingLine(CDC*pDC,CPoint firstPoint,CPoint secondPoint);

	virtual int ComputRadious(CPoint centerPoint,CPoint movingPoint);
	virtual int ComputRadiusX(CPoint firstPoint,CPoint secondPoint);
	virtual int ComputRadiusY(CPoint firstPoint,CPoint secondPoint);
	virtual float ComputSloap(CPoint firstPoint,CPoint secondPoint);

	virtual void ellipseMidpoint(CDC*pDC,int xCenter,int yCenter,int Rx,int Ry);
	virtual void ExchangeDraw(CDC*pDC,CPoint point);
	virtual void DrawEnd(CDC*pDC,CPoint point);
	virtual void DrawMask(CDC*pDC,CPoint point);
	virtual void DrawMask(CDC*pDC,CPoint first,CPoint second);
	virtual void Circumrotate(CPoint first,CPoint second);

	virtual int IsOnMarginPoint(CPoint point);
	virtual BOOL IsInRgn(CPoint point);

	virtual CRgn* GetRgn();
	virtual	CRect GetBoundingRect();

public:
	//////////////////////////////////////////////////////////////////////////
	// 添加(07.07.05)
	long double m_dbZoomX;
	long double m_dbZoomY;
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// 添加(07.07.29)
	int m_nPreLink;		// 前向链接
	int m_nNextLink;	// 后向链接
	//////////////////////////////////////////////////////////////////////////
	// 是否封闭曲线(08.09.15)
	BOOL m_bClose;
	void SetPolyClose(BOOL bClose = TRUE);
	BOOL IsPolyClose();

	void SetFillMap(int nMapIdx){m_nFillMapIdx=nMapIdx;}
  
	//////////////////////////////////////////////////////////////////////////
	// 将宽度缩放dbZoomX比率,高度缩放dbZoomY,将缩放后的尺寸设为原始尺寸(08.09.17)
	virtual void Reset(long double dbZoomX, long double dbZoomY);
	// 添加(07.07.05)
	virtual void SetZoomFactor(long double dbZoomX, long double dbZoomY);
	virtual void GetZoomFactor(long double & dbZoomX, long double & dbZoomY);
	virtual POINT2DF GetSrcPoint(const CPoint& ptDst);
	virtual CPoint GetDstPoint(const POINT2DF& ptSrc);
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// 添加(07.07.29)
	virtual void SetPreLink(int nPrevLink);
	virtual int  GetPreLink();

	virtual void SetNextLink(int nNextLink);
	virtual int  GetNextLink();

	virtual void SetLink(int nPrevLink, int nNextLink);
	virtual void GetLink(int& nPrevLink, int& nNextLink);

	// 获取点数及点坐标(08.09.08)
	virtual int GetPointNum();
	virtual POINT2DF GetPoint(int nIdx);
	// 取得多边形的外接矩形(08.09.16)
	virtual RECT GetPolyRect();
	//////////////////////////////////////////////////////////////////////////
	virtual void CopyUnit(CUnit& unit);
	// 是否处于选择状态
	BOOL IsSelected() { return m_HaveFindFirst; };
	void SetSelect(BOOL haveFindFirst) { m_HaveFindFirst=haveFindFirst;	};
};
#endif

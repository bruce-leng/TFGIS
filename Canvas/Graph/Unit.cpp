#include "stdafx.h"
#include"Unit.h"
#include "TextEdit.h"

IMPLEMENT_SERIAL(CUnit,CObject,1)
CUnit::CUnit()
{
	Initial();
}
CUnit::CUnit(CUnit&unit)
{
	*this = unit;
	m_FirstPoint=unit.m_FirstPoint;
	m_SecondPoint=unit.m_SecondPoint;
	m_PenColor=unit.m_PenColor;
	m_PenWidth=unit.m_PenWidth;
	m_PenStyle=unit.m_PenStyle;
	
	m_pRgn=NULL;
	
	//////////////////////////////////////////////////////////////////////////
	//追加(07.07.30)
	m_DrawingMode=unit.m_DrawingMode;
	m_BkMode=unit.m_BkMode;
	m_BackgroundColor=unit.m_BackgroundColor;
	//////////////////////////////////////////////////////////////////////////
	m_nFillMapIdx=-1;
}

CUnit CUnit::operator =(CUnit&unit)
{
	m_FirstPoint=unit.m_FirstPoint;
	m_SecondPoint=unit.m_SecondPoint;
	m_PenColor=unit.m_PenColor;
	m_PenWidth=unit.m_PenWidth;
	m_PenStyle=unit.m_PenStyle;
	m_pRgn=NULL;
	
	//////////////////////////////////////////////////////////////////////////
	//追加(07.07.30)
	m_DrawingMode=unit.m_DrawingMode;
	m_BkMode=unit.m_BkMode;
	m_BackgroundColor=unit.m_BackgroundColor;
	//////////////////////////////////////////////////////////////////////////
	m_nFillMapIdx=unit.m_nFillMapIdx;
	
	return *this;
}

void CUnit::CopyUnit(CUnit& unit)
{
	*this=unit;
	m_FirstPoint=unit.m_FirstPoint;
	m_SecondPoint=unit.m_SecondPoint;
	m_dbZoomX=unit.m_dbZoomX;
	m_dbZoomY=unit.m_dbZoomY;
	m_PenStyle=unit.m_PenStyle;
	m_PenWidth=unit.m_PenWidth;
	m_BkMode=unit.m_BkMode;
	m_DrawingMode=unit.m_DrawingMode;
	m_PenColor=unit.m_PenColor;
	m_BrushColor=unit.m_BrushColor;
	m_BackgroundColor=unit.m_BackgroundColor;
	m_FirstPoint=unit.m_FirstPoint;
	m_SecondPoint=unit.m_SecondPoint;
	m_ThirdPoint=unit.m_ThirdPoint;
	m_CenterPoint=unit.m_CenterPoint;
	m_FourthPoint=unit.m_FourthPoint;
	m_FifthPoint=unit.m_FifthPoint;
	m_MovingPoint=unit.m_MovingPoint;
	m_MinPoint=unit.m_MinPoint;
	m_MaxPoint=unit.m_MaxPoint;
	m_IntersectionPointFirst=unit.m_IntersectionPointFirst;
	m_IntersectionPointSecond=unit.m_IntersectionPointSecond;
	m_BoundingRect=unit.m_BoundingRect;

	// m_pRgn只是个临时CRgn指针，不需要将unit.m_pRgn复制过来
	m_pRgn=NULL;
	
	m_pPointList=NULL;
	if (unit.m_pPointList)
	{
		m_pPointList=new CArray<POINT2DF, POINT2DF>;
		for(int i=0; i<unit.m_pPointList->GetSize(); i++)
		{
			POINT2DF point=unit.m_pPointList->GetAt(i);
			m_pPointList->Add(point);
		}
	}
	
	for(int i=0; i<unit.m_PointArray.GetSize(); i++)
	{
		POINT2DF point=unit.m_PointArray.GetAt(i);
		m_PointArray.Add(point);
	}
	
	m_BrushStyle=unit.m_BrushStyle;
	m_BrushHatch=unit.m_BrushHatch;
	m_FoundIndex=unit.m_FoundIndex;
	m_FoundPoint=unit.m_FoundPoint;
	m_Radious=unit.m_Radious;
	m_Xr=unit.m_Xr;
	m_Yr=unit.m_Yr;
	m_HaveLBUp=unit.m_HaveLBUp;
	m_Cos=unit.m_Cos;
	m_Sin=unit.m_Sin;
	m_Sloap1=unit.m_Sloap1;
	m_Sloap2=unit.m_Sloap2;
	m_AngleFirst=unit.m_AngleFirst;
	m_AngleSecond=unit.m_AngleSecond;
	m_HaveFindFirst=unit.m_HaveFindFirst;
	
	m_pTextEdit=NULL;
	if (unit.m_pTextEdit)
		m_pTextEdit=new CTextEdit(*unit.m_pTextEdit);
	
	m_pFont=NULL;
	if (unit.m_pFont)
	{
		LOGFONT logFont;
		unit.m_pFont->GetLogFont(&logFont);
		m_pFont=new CFont;
		m_pFont->CreateFontIndirect(&logFont);
	}
	
	m_DrawStatus=unit.m_DrawStatus;
	m_dbZoomX=unit.m_dbZoomX;
	m_dbZoomY=unit.m_dbZoomY;
	m_nPreLink=unit.m_nPreLink;
	m_nNextLink=unit.m_nNextLink;
	m_nFillMapIdx=unit.m_nFillMapIdx;
}

CUnit::~CUnit()
{
	if (m_pRgn)	delete m_pRgn;
}

void CUnit::Initial()
{
	m_BkMode=TRANSPARENT;
	m_pRgn=NULL;
	m_FirstPoint=POINT2DF(0,0);
	m_SecondPoint=POINT2DF(0,0);
	m_PenColor=RGB(255,0,0);
	m_BrushColor=RGB(0,0,0);
	m_BackgroundColor=RGB(255,255,255);
	m_PenWidth=1;
	m_PenStyle=PS_SOLID;
	
	m_DrawingMode=R2_COPYPEN;
	m_HaveLBUp=false;
	m_HaveFindFirst=false;
	m_DrawStatus=Draw_Status;
	//////////////////////////////////////////////////////////////////////////
	// 追加(07.07.29)
	m_dbZoomX=m_dbZoomY=1.0;
	m_nPreLink=m_nNextLink=-1;
	//////////////////////////////////////////////////////////////////////////
	
	// 追加(08.09.15)
	m_bClose = TRUE;
}

void CUnit::Serialize(CArchive &ar)
{
  if(ar.IsStoring())
  {
		ar<<m_PenColor<<m_PenStyle<<m_PenWidth<<m_DrawingMode<<m_BkMode<<m_BackgroundColor;
		ar<<m_FirstPoint.x<<m_FirstPoint.y<<m_SecondPoint.x<<m_SecondPoint.y;
  }
  else
  {
		ar>>m_PenColor>>m_PenStyle>>m_PenWidth>>m_DrawingMode>>m_BkMode>>m_BackgroundColor;
		ar>>m_FirstPoint.x>>m_FirstPoint.y>>m_SecondPoint.x>>m_SecondPoint.y;
  }
}

CRgn* CUnit::GetRgn()
{
  if(m_pRgn==NULL)
  {
		m_pRgn=new CRgn;
		CPoint point[2];

		point[0]=CPoint(int(m_FirstPoint.x), int(m_FirstPoint.y));
		point[1]=CPoint(int(m_SecondPoint.x), int(m_SecondPoint.y));
		m_pRgn->CreatePolygonRgn(point,2,ALTERNATE);
  }
  return m_pRgn;
}

BOOL CUnit::IsInRgn(CPoint point)
{
	POINT2DF pt = GetSrcPoint(point);

	this->GetRgn();
	return (m_pRgn->PtInRegion(int(pt.x), int(pt.y)));
}

void CUnit::ShowMovingLine(CDC*pDC,CPoint firstPoint, CPoint secondPoint)
{
	CPen m_pen;
	m_pen.CreatePen(PS_SOLID,1,RGB(0,0,255));
	CPen *OldPen=pDC->SelectObject(&m_pen);
	int oldDrawingMode=pDC->SetROP2(R2_NOTXORPEN);
	int oldBkMode=pDC->SetBkMode(OPAQUE); 
	pDC->MoveTo(firstPoint);
	pDC->LineTo(secondPoint.x,secondPoint.y);
	pDC->SelectObject(OldPen);
	pDC->SetROP2(oldDrawingMode);
	pDC->SetBkMode(oldBkMode);
}

void CUnit::DrawOldReferencePoint(CDC *pDC, CPoint point)
{
	CBrush brush;
	brush.CreateSolidBrush(RGB(250,0,250));
	CPen m_pen;
	m_pen.CreatePen(PS_SOLID,1,RGB(0,0,255));
	CPen *OldPen=pDC->SelectObject(&m_pen);
	int oldBkMode=pDC->SetBkMode(OPAQUE); 
	CBrush *OldBrush=pDC->SelectObject(&brush);
	int oldDrawingMode=pDC->SetROP2(R2_NOTXORPEN);
	CRect rect;
	rect.SetRect(point,point);
	rect.InflateRect(3,3);
	pDC->Rectangle(rect);
	pDC->SelectObject(OldPen);
	pDC->SelectObject(OldBrush);
	pDC->SetROP2(oldDrawingMode);
	pDC->SetBkMode(oldBkMode);
}

void CUnit::SetBrushOrg(CDC *pDC, CBrush*brush)
{
	if(pDC!=NULL&&brush!=NULL)
	{
		if(pDC->m_hDC!=NULL&&brush->m_hObject!=NULL)
		{
			CPoint pointB(0,0);
			pDC->LPtoDP(&pointB);
			pointB.x%=8;
			pointB.y%=8;
			brush->UnrealizeObject();
			
			pDC->SetBrushOrg(pointB);
		}
	}
}

int CUnit::IsOnMarginPoint(CPoint point)
{
	return 0;
}

void CUnit::Circumrotate(CPoint first,CPoint second)
{
}

void CUnit::OnLButtonDown(CDC*pDC,PGRAPHCAPTION pCaption,CPoint point)
{
}

void CUnit::DrawMask(CDC*pDC,CPoint first,CPoint second)
{
}

void CUnit::DrawMask(CDC *pDC, CPoint point)
{
}

void CUnit::DrawEnd(CDC*pDC,CPoint point)
{
}

void CUnit::ExchangeDraw(CDC *pDC, CPoint point)
{
}

int CUnit::ComputRadious(CPoint centerPoint, CPoint movingPoint)
{
	return 0;
}

void CUnit::ellipseMidpoint(CDC*pDC,int xCenter, int yCenter, int Rx, int Ry)
{
}

float CUnit::ComputSloap(CPoint firstPoint, CPoint secondPoint)
{
	return 0;
}

int CUnit::ComputRadiusY(CPoint firstPoint,CPoint secondPoint)
{
	return 0;
}

int CUnit::ComputRadiusX(CPoint firstPoint,CPoint secondPoint)
{
	return 0;
}

void CUnit::ShowSelectPoint(CDC*pDC)
{
}

CRect CUnit::GetBoundingRect()
{
	return 0;
}

double CUnit::ComputeSloap1(CPoint firstpoint, CPoint secondpoint)
{
	return 0;
}

double CUnit::ComputeSloap2(CPoint firstpoint, CPoint secondpoint)
{
	return 0;
}

void CUnit::DrawActive(CDC *pDC,CPoint point)
{
}

void CUnit::DrawActiveStepOne(CDC *pDC,CPoint movingPoint)
{
}

void CUnit::DrawStaticStepOne(CDC *pDC,CPoint movingPoint)
{
}

CPoint CUnit::ComputeIntersectionPointFirst(CPoint point)
{
	return 0;
}

CPoint CUnit::ComputeIntersectionPointSecond(CPoint point)
{
	return 0;
}

void CUnit::OnMouseMove(CDC*pDC,PGRAPHCAPTION pCaption,CPoint point)
{
}

void CUnit::OnLButtonDblClk(CDC *pDC,PGRAPHCAPTION pCaption, CPoint point)
{
}

void CUnit::OnMenuitemOk(CDC *pDC, PGRAPHCAPTION pCaption)
{
}

void CUnit::OnMenuitemCirCu(CDC *pDC, PGRAPHCAPTION pCaption)
{
}

void CUnit::SetFont(ENUMLOGFONT *lpelf)
{
}

void CUnit::ChangeFont(ENUMLOGFONT *lpelf)
{
}

void CUnit::ChangeFontSize(ENUMLOGFONT *lpelf)
{
}

void CUnit::OnContextMenu(CWnd *pWnd, CPoint point)
{
}

void CUnit::OnMENUITEMSize()
{
}

//////////////////////////////////////////////////////////////////////////
// 将宽度缩放dbZoomX比率,高度缩放dbZoomY,将缩放后的尺寸设为原始尺寸(08.09.17)
void CUnit::Reset(long double dbZoomX, long double dbZoomY)
{
}
// 添加(07.07.05)
void CUnit::SetZoomFactor(long double dbZoomX, long double dbZoomY)
{
	ASSERT(dbZoomX > 0 && dbZoomY > 0);
	m_dbZoomX = dbZoomX;
	m_dbZoomY = dbZoomY;
}

void CUnit::GetZoomFactor(long double& dbZoomX, long double& dbZoomY)
{
	dbZoomX = m_dbZoomX;
	dbZoomY = m_dbZoomY;
}

// 得到未缩放前的点
POINT2DF CUnit::GetSrcPoint(const CPoint& ptDst)
{
	POINT2DF point;
	point.x = ptDst.x/m_dbZoomX;
	point.y = ptDst.y/m_dbZoomY;

	return point;
}

// 得到缩放后的点
CPoint CUnit::GetDstPoint(const POINT2DF& ptSrc)
{
	CPoint point;
	point.x = long(ptSrc.x*m_dbZoomX+0.5);	// 四舍五入
	point.y = long(ptSrc.y*m_dbZoomY+0.5);

	return point;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 添加(07.07.29)
void CUnit::SetPreLink(int nPrevLink){ m_nPreLink = nPrevLink; }
int CUnit::GetPreLink(){ return m_nPreLink; }
void CUnit::SetNextLink(int nNextLink){ m_nNextLink = nNextLink; }
int CUnit::GetNextLink(){ return m_nNextLink; }
void CUnit::SetLink(int nPrevLink, int nNextLink)
{ m_nPreLink = nPrevLink; m_nNextLink = nNextLink; }
void CUnit::GetLink(int& nPrevLink, int& nNextLink)
{ nPrevLink = m_nPreLink; nNextLink = m_nNextLink; }
//////////////////////////////////////////////////////////////////////////

// 得到点数
int CUnit::GetPointNum()
{
	return 0;
}

POINT2DF CUnit::GetPoint(int nIdx)
{
	return POINT2DF(0,0);
}

void CUnit::SetPolyClose(BOOL bClose)
{
	m_bClose = bClose;
}

BOOL CUnit::IsPolyClose()
{
	return m_bClose;
}

RECT CUnit::GetPolyRect()
{
	RECT rect;
	rect.left = rect.right = rect.top = rect.bottom = 0;

	return rect;
}
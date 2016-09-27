#include "stdafx.h"
#include "Round.h"
IMPLEMENT_SERIAL(CRound,CUnit,1)
CRound::CRound()
{
  Initial();
 
  m_BrushStyle=BS_HOLLOW;
  m_BrushHatch=HS_CROSS;
}

void CRound::DrawStatic(CDC*pDC)
{
  CPen m_pen;
  CBrush m_brush;
  int oldDrawingMode=pDC->SetROP2(m_DrawingMode);
  m_pen.CreatePen(m_PenStyle,m_PenWidth,m_PenColor);
  CPen* pen=(CPen*)pDC->SelectObject(&m_pen);
  LOGBRUSH brushlog;
  brushlog.lbColor=m_BrushColor;
  brushlog.lbHatch=m_BrushHatch;
  brushlog.lbStyle=m_BrushStyle;

  m_brush.CreateBrushIndirect(&brushlog);
  
  CBrush*brush=(CBrush*)pDC->SelectObject(&m_brush);
  SetBrushOrg(pDC,&m_brush);
	int OldBkMode=pDC->SetBkMode(m_BkMode);
	COLORREF OldColor=pDC->SetBkColor(m_BackgroundColor);
	
	// CPoint lpoint=m_SecondPoint.x-m_FirstPoint.x>m_SecondPoint.y-m_FirstPoint.y?CPoint(m_SecondPoint.x,m_FirstPoint.y+m_SecondPoint.x-m_FirstPoint.x):CPoint(m_FirstPoint.x+m_SecondPoint.y-m_FirstPoint.y,m_SecondPoint.y);
	//m_SecondPoint=lpoint;
	pDC->Ellipse(CRect(GetDstPoint(m_FirstPoint),GetDstPoint(m_SecondPoint))); 
	pDC->SelectObject(brush);
	pDC->SelectObject(pen);
	pDC->SetBkMode(OldBkMode);
	pDC->SetBkColor(OldColor);
	pDC->SetROP2(oldDrawingMode);
}

int CRound::ComputRadious(CPoint centerPoint, CPoint movingPoint)
{
	m_Radious=int(sqrt((centerPoint.x-movingPoint.x)*(centerPoint.x-movingPoint.x)+(centerPoint.y-movingPoint.y)*(centerPoint.y-movingPoint.y)));
	CPoint cpoint;
	cpoint=centerPoint;
	cpoint.Offset(-m_Radious,-m_Radious);
	this->m_FirstPoint=GetSrcPoint(cpoint);
	cpoint=centerPoint;
	cpoint.Offset(m_Radious,m_Radious);
	this->m_SecondPoint=GetSrcPoint(cpoint);
	return 0;
}

BOOL CRound::IsInRgn(CPoint point)
{
	POINT2DF FirstPoint, SecondPoint;
	FirstPoint.x=m_FirstPoint.x<m_SecondPoint.x?m_FirstPoint.x:m_SecondPoint.x;
	FirstPoint.y=m_FirstPoint.y<m_SecondPoint.y?m_FirstPoint.y:m_SecondPoint.y;
	SecondPoint.x=m_FirstPoint.x>m_SecondPoint.x?m_FirstPoint.x:m_SecondPoint.x;
	SecondPoint.y=m_FirstPoint.y>m_SecondPoint.y?m_FirstPoint.y:m_SecondPoint.y;
	POINT pt1=GetDstPoint(FirstPoint);
	POINT pt2=GetDstPoint(SecondPoint);
	
	CRgn rgn1;
	rgn1.CreateEllipticRgn(pt1.x-4,pt1.y-4,pt2.x+4,pt2.y+4);
	CRgn rgn2;
	rgn2.CreateEllipticRgn(pt1.x+4,pt1.y+4,pt2.x-4,pt2.y-4);
	
	return rgn1.PtInRegion(point)&&!rgn2.PtInRegion(point);
}

int CRound::IsOnMarginPoint(CPoint point)
{
	POINT2DF FirstPoint;
	POINT2DF SecondPoint;
	FirstPoint.x=m_FirstPoint.x<m_SecondPoint.x?m_FirstPoint.x:m_SecondPoint.x;
	FirstPoint.y=m_FirstPoint.y<m_SecondPoint.y?m_FirstPoint.y:m_SecondPoint.y;
	SecondPoint.x=m_FirstPoint.x>m_SecondPoint.x?m_FirstPoint.x:m_SecondPoint.x;
	SecondPoint.y=m_FirstPoint.y>m_SecondPoint.y?m_FirstPoint.y:m_SecondPoint.y;
	POINT pt1=GetDstPoint(FirstPoint);
	POINT pt2=GetDstPoint(SecondPoint);
	CRect rect(point,point);
	rect.InflateRect(CSize(4,4));
	if(rect.PtInRect(CPoint(pt1.x+(pt2.x-pt1.x)/2,pt1.y)))
	{
		m_FoundPoint=GetSrcPoint(CPoint(pt1.x+(pt2.x-pt1.x)/2,pt1.y));
		return 1;
	}   
	if(rect.PtInRect(CPoint(pt2.x,pt1.y+(pt2.y-pt1.y)/2)))
	{
		m_FoundPoint=GetSrcPoint(CPoint(pt2.x,pt1.y+(pt2.y-pt1.y)/2));
		return 2;
	}
	if(rect.PtInRect(CPoint(pt2.x+(pt1.x-pt2.x)/2,pt2.y)))
	{
		m_FoundPoint=GetSrcPoint(CPoint(pt2.x+(pt1.x-pt1.x)/2,pt1.y));
		return 3;
	}
	if(rect.PtInRect(CPoint(pt1.x,pt2.y+(pt1.y-pt2.y)/2)))
	{
		m_FoundPoint=GetSrcPoint(CPoint(pt1.x,pt2.y+(pt1.y-pt2.y)/2));
		return 4;
	}
	if(rect.PtInRect(GetDstPoint(m_CenterPoint)))
	{
		m_FoundPoint=m_CenterPoint;
		return 5;
	}
  return 0;
}

void CRound::Serialize(CArchive &ar)
{
	CUnit::Serialize(ar);
	if(ar.IsStoring())
	{
		ar<<m_BrushStyle<<m_BkMode<<m_BrushHatch<<m_BrushColor;
		ar<<m_CenterPoint.x<<m_CenterPoint.y;
	}
	else
	{
		ar>>m_BrushStyle>>m_BkMode>>m_BrushHatch>>m_BrushColor;
		ar>>m_CenterPoint.x>>m_CenterPoint.y;
	}
}

void CRound::ShowSelectPoint(CDC *pDC)
{
	CBrush brush;
	brush.CreateSolidBrush(RGB(0,0,255));
	CPen m_pen;
	m_pen.CreatePen(PS_SOLID,1,RGB(0,0,255));
	CPen *OldPen=pDC->SelectObject(&m_pen);
	int oldBkMode=pDC->SetBkMode(OPAQUE); 
	CBrush *OldBrush=pDC->SelectObject(&brush);
	int oldDrawingMode=pDC->SetROP2(R2_NOTXORPEN);
	CRect rect;

	POINT pt=GetDstPoint(POINT2DF(m_FirstPoint.x+(m_SecondPoint.x-m_FirstPoint.x)/2,m_FirstPoint.y));
	rect.SetRect(pt,pt);
	rect.InflateRect(3,3);
	pDC->Rectangle(rect);
	pt=GetDstPoint(POINT2DF(m_SecondPoint.x,m_FirstPoint.y+(m_SecondPoint.y-m_FirstPoint.y)/2));
	rect.SetRect(pt,pt);
	rect.InflateRect(3,3);
	pDC->Rectangle(rect);
	pt=GetDstPoint(POINT2DF(m_SecondPoint.x+(m_FirstPoint.x-m_SecondPoint.x)/2,m_SecondPoint.y));
	rect.SetRect(pt,pt);
	rect.InflateRect(3,3);
	pDC->Rectangle(rect);
	pt=GetDstPoint(POINT2DF(m_FirstPoint.x,m_SecondPoint.y+(m_FirstPoint.y-m_SecondPoint.y)/2));
	rect.SetRect(pt,pt);
	rect.InflateRect(3,3);
	pDC->Rectangle(rect);

	pDC->SelectObject(OldBrush);
  brush.DeleteObject();
  brush.CreateSolidBrush(RGB(255,0,0));
  pDC->SelectObject(&brush);
	rect.SetRect(GetDstPoint(m_CenterPoint),GetDstPoint(m_CenterPoint));
	rect.InflateRect(3,3);
	pDC->Rectangle(rect);
	pDC->SelectObject(OldPen);
	pDC->SetBkMode(oldBkMode); 
	pDC->SelectObject(OldBrush);
	pDC->SetROP2(oldDrawingMode);
}

void CRound::DrawActive(CDC *pDC,CPoint point)
{
  CPen m_pen;
  CBrush m_brush;
  
  m_pen.CreatePen(m_PenStyle,m_PenWidth,m_PenColor);
  CPen* pen=(CPen*)pDC->SelectObject(&m_pen);
  LOGBRUSH brushlog;
  brushlog.lbColor=m_BrushColor;
  brushlog.lbHatch=m_BrushHatch;
  brushlog.lbStyle=m_BrushStyle;
	
  m_brush.CreateBrushIndirect(&brushlog);
	SetBrushOrg(pDC,&m_brush);
  CBrush*brush=(CBrush*)pDC->SelectObject(&m_brush);
	
	int OldBkMode=pDC->SetBkMode(m_BkMode);
	COLORREF OldColor=pDC->SetBkColor(m_BackgroundColor);
	
	// CPoint lpoint=m_SecondPoint.x-m_FirstPoint.x>m_SecondPoint.y-m_FirstPoint.y?CPoint(m_SecondPoint.x,m_FirstPoint.y+m_SecondPoint.x-m_FirstPoint.x):CPoint(m_FirstPoint.x+m_SecondPoint.y-m_FirstPoint.y,m_SecondPoint.y);
	//m_SecondPoint=lpoint;
	pDC->Ellipse(CRect(GetDstPoint(m_FirstPoint),GetDstPoint(m_SecondPoint))); 
	pDC->SelectObject(brush);
	pDC->SelectObject(pen);
	pDC->SetBkMode(OldBkMode);
	pDC->SetBkColor(OldColor);
}

void CRound::OnLButtonDown(CDC *pDC, PGRAPHCAPTION pCaption, CPoint point)
{
  if(m_HaveFindFirst)
	{
		pCaption->L_iPointCount=IsOnMarginPoint(point);
		if(pCaption->L_iPointCount!=0)
		{
			pCaption->m_bHaveFindSecond=true;
			pCaption->m_CurrentDrawTool=Graph_Round;
			pCaption->m_CurrentDrawStatus=Change_Status;
			pCaption->m_FirstPoint=m_FirstPoint;
			pCaption->m_SecondPoint=m_SecondPoint;
			
		}//******if(L_iPointCount!=0)
		
		if(pCaption->L_iPointCount==5)
		{
			pCaption->m_FirstPoint=m_FirstPoint;
			pCaption->m_SecondPoint=m_SecondPoint;
			pCaption->m_CurrentDrawTool=Graph_Round;
			pCaption->m_CurrentDrawStatus=Drag_Status;
		}
	}//********if(L_pRound->m_HaveFindFirst)
  
	if(!pCaption->m_bHaveFindSecond&&IsInRgn(point))
  {
		if(!m_HaveFindFirst)
		{
			m_HaveFindFirst=true;
			pCaption->m_bHaveFindFirst=true;
			ShowSelectPoint(pDC);
		}
		else
		{
			this->m_HaveFindFirst=false;
			pCaption->m_bHaveFindFirst=true;
			ShowSelectPoint(pDC);
		}
  }
	
	if(pCaption->m_bHaveFindSecond)
	{   
		pCaption->m_pCurrentUnit=this;
		m_MovingPoint=GetSrcPoint(point);
		m_DrawingMode=pDC->GetROP2();
		//m_pCurrentUnit->m_MovingPoint=point;
		ShowSelectPoint(pDC);
		DrawOldReferencePoint(pDC,GetDstPoint(m_FoundPoint));
		DrawStatic(pDC);
	}
}//******for(i=0;!m_bHaveFindFirst&&i<pDoc->m_RoundArray.GetSize();i++)


void CRound::OnMouseMove(CDC *pDC, PGRAPHCAPTION pCaption, CPoint point)
{
	if(pCaption->m_CurrentDrawStatus==Drag_Status)
	{
		m_FirstPoint.x+=pCaption->m_SecondPoint.x-pCaption->m_FirstPoint.x;
		m_FirstPoint.y+=pCaption->m_SecondPoint.y-pCaption->m_FirstPoint.y;
		m_SecondPoint.x+=pCaption->m_SecondPoint.x-pCaption->m_FirstPoint.x;
		m_SecondPoint.y+=pCaption->m_SecondPoint.y-pCaption->m_FirstPoint.y;
		m_CenterPoint.x+=pCaption->m_SecondPoint.x-pCaption->m_FirstPoint.x;
		m_CenterPoint.y+=pCaption->m_SecondPoint.y-pCaption->m_FirstPoint.y;
	}
	else
	{
		ShowMovingLine(pDC,GetDstPoint(m_CenterPoint),GetDstPoint(pCaption->m_SecondPoint));
		this->DrawActive(pDC,point);
		ComputRadious(GetDstPoint(m_CenterPoint),point);
		pCaption->m_SecondPoint=GetSrcPoint(point);
		
		this->DrawActive(pDC,point);
		ShowMovingLine(pDC,GetDstPoint(m_CenterPoint),GetDstPoint(pCaption->m_SecondPoint));
	}
}
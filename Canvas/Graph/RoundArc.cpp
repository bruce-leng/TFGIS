#include "stdafx.h"
#include "RoundArc.h"
IMPLEMENT_SERIAL(CRoundArc,CUnit,1)
CRoundArc::CRoundArc()
{

}

void CRoundArc::DrawStatic(CDC *pDC)
{
  CPen m_pen;
  CBrush m_brush;
  int oldDrawingMode=pDC->SetROP2(this->m_DrawingMode);
  m_pen.CreatePen(m_PenStyle,m_PenWidth,m_PenColor);
  CPen* pen=(CPen*)pDC->SelectObject(&m_pen);
  LOGBRUSH brushlog;
  brushlog.lbColor=m_BrushColor;
  brushlog.lbHatch=m_BrushHatch;
  brushlog.lbStyle=m_BrushStyle;

  m_brush.CreateBrushIndirect(&brushlog);
 
  CBrush*brush=(CBrush*)pDC->SelectObject(&m_brush);
 
  int OldBkMode=pDC->SetBkMode(m_BkMode);
  COLORREF OldColor=pDC->SetBkColor(m_BackgroundColor);
  CRect rect;
  POINT centerPoint=GetDstPoint(m_CenterPoint);
  rect.SetRect(centerPoint.x-m_Xr,centerPoint.y-m_Yr,centerPoint.x+m_Xr,centerPoint.y+m_Yr);
  pDC->Arc(rect,GetDstPoint(m_FirstPoint),GetDstPoint(m_ThirdPoint));

  pDC->SelectObject(brush);
  pDC->SelectObject(pen);
  pDC->SetBkMode(OldBkMode);
  pDC->SetBkColor(OldColor);
  pDC->SetROP2(oldDrawingMode);
}

double CRoundArc::ComputeSloap1(CPoint firstpoint, CPoint secondpoint)
{
	if(firstpoint.y==secondpoint.y)
	{
		m_Sloap1=-9999999999999.0;
	}
	else
	{
		m_Sloap1=-double((firstpoint.x-secondpoint.x))/double((firstpoint.y-secondpoint.y));
	}
	m_Sloap2=m_Sloap1;
	m_CenterPoint=GetSrcPoint(CPoint((firstpoint.x+secondpoint.x)/2,(firstpoint.y+secondpoint.y)/2));
	m_Xr=0;//sqrt((firstpoint.x-secondpoint.x)*(firstpoint.x-secondpoint.x)+(firstpoint.y-firstpoint.y)*(firstpoint.y-firstpoint.y));
	return m_Sloap1;
}

double CRoundArc::ComputeSloap2(CPoint firstpoint, CPoint secondpoint)
{
	if(firstpoint.y==secondpoint.y)
	{
	  m_Sloap2=-999999999.0;
	}
	else
	{
	 m_Sloap2=-double((firstpoint.x-secondpoint.x))/double((firstpoint.y-secondpoint.y));
	}

	this->m_CenterPoint.x=((m_FirstPoint.y-m_ThirdPoint.y)/2-m_Sloap1*(m_FirstPoint.x+m_SecondPoint.x)/2+m_Sloap2*(m_SecondPoint.x+m_ThirdPoint.x)/2)/(m_Sloap2-m_Sloap1);
	this->m_CenterPoint.y=((m_FirstPoint.y+m_SecondPoint.y)/2*m_Sloap2-(m_SecondPoint.y+m_ThirdPoint.y)/2*m_Sloap1+m_Sloap1*m_Sloap2*(m_ThirdPoint.x-m_FirstPoint.x)/2)/(m_Sloap2-m_Sloap1);

	m_Xr=sqrt((m_CenterPoint.x-m_ThirdPoint.x)*(m_CenterPoint.x-m_ThirdPoint.x)+(m_CenterPoint.y-m_ThirdPoint.y)*(m_CenterPoint.y-m_ThirdPoint.y));	

	return m_Sloap2;
}

void CRoundArc::DrawActive(CDC *pDC,CPoint point)
{
  
	ShowMovingLine(pDC,GetDstPoint(m_FirstPoint),GetDstPoint(m_SecondPoint));
	if(m_Xr==0)
		return;
	ShowMovingLine(pDC,GetDstPoint(m_FirstPoint),GetDstPoint(m_CenterPoint));
	ShowMovingLine(pDC,GetDstPoint(m_SecondPoint),GetDstPoint(m_CenterPoint));
	ShowMovingLine(pDC,GetDstPoint(m_SecondPoint),GetDstPoint(m_ThirdPoint));
	
  CPen m_pen;
  CBrush m_brush;
  
  m_pen.CreatePen(m_PenStyle,m_PenWidth,m_PenColor);
  CPen* pen=(CPen*)pDC->SelectObject(&m_pen);
  LOGBRUSH brushlog;
  brushlog.lbColor=m_BrushColor;
  brushlog.lbHatch=m_BrushHatch;
  brushlog.lbStyle=m_BrushStyle;
	
  m_brush.CreateBrushIndirect(&brushlog);
	
  CBrush*brush=(CBrush*)pDC->SelectObject(&m_brush);
	
  int OldBkMode=pDC->SetBkMode(m_BkMode);
  COLORREF OldColor=pDC->SetBkColor(m_BackgroundColor);
  CRect rect;
  POINT centerPoint=GetDstPoint(m_CenterPoint);
  rect.SetRect(centerPoint.x-m_Xr,centerPoint.y-m_Xr,centerPoint.x+m_Xr,centerPoint.y+m_Xr);
  pDC->Arc(rect,GetDstPoint(m_FirstPoint),GetDstPoint(m_ThirdPoint));
	
  pDC->SelectObject(brush);
	pDC->SelectObject(pen);
	pDC->SetBkMode(OldBkMode);
	pDC->SetBkColor(OldColor);
}

void CRoundArc::Serialize(CArchive &ar)
{
	CUnit::Serialize(ar);
	if(ar.IsStoring())
	{
	 ar<<m_ThirdPoint.x<<m_ThirdPoint.y<<m_Xr;
	 ar<<m_CenterPoint.x<<m_CenterPoint.y;
	}
	else
	{
	 ar>>m_ThirdPoint.x>>m_ThirdPoint.y>>m_Xr;
	 ar>>m_CenterPoint.x>>m_CenterPoint.y;
	}
}

BOOL CRoundArc::IsInRgn(CPoint point)
{
  POINT centerPoint=GetDstPoint(m_CenterPoint);
	if(abs(sqrt((point.x-centerPoint.x)*(point.x-centerPoint.x)
				+(point.y-centerPoint.y)*(point.y-centerPoint.y))-m_Xr)<=2)
		return true;

	return false;
}

void CRoundArc::ShowSelectPoint(CDC *pDC)
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
	rect.SetRect(GetDstPoint(m_FirstPoint),GetDstPoint(m_FirstPoint));
	rect.InflateRect(3,3);
	pDC->Rectangle(rect);
	
	rect.SetRect(GetDstPoint(m_SecondPoint),GetDstPoint(m_SecondPoint));
	rect.InflateRect(3,3);
	pDC->Rectangle(rect);
	
	rect.SetRect(GetDstPoint(m_ThirdPoint),GetDstPoint(m_ThirdPoint));
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

int CRoundArc::IsOnMarginPoint(CPoint point)
{
	CRect rect;
	rect.SetRect(point,point);
	rect.InflateRect(4,4);
	if(rect.PtInRect(GetDstPoint(m_FirstPoint)))
	{
		this->m_FoundPoint=m_FirstPoint;
		return 1;
	}
	if(rect.PtInRect(GetDstPoint(m_SecondPoint)))
	{
		this->m_FoundPoint=m_SecondPoint;
		return 2;
	}
	if(rect.PtInRect(GetDstPoint(m_ThirdPoint)))
	{
		this->m_FoundPoint=m_ThirdPoint;
		return 3;
	}
	if(rect.PtInRect(GetDstPoint(m_CenterPoint)))
	{
		this->m_FoundPoint=m_CenterPoint;
		return 4;
	}
	return 0;
}

void CRoundArc::OnLButtonDown(CDC *pDC, PGRAPHCAPTION pCaption, CPoint point)
{
 if(m_HaveFindFirst)
	{ 
		pCaption->L_iPointCount=IsOnMarginPoint(point);
		if(pCaption->L_iPointCount!=0)
		{   
			pCaption->m_bHaveFindSecond=true;
			pCaption->m_pCurrentUnit=this;
			m_DrawingMode=pDC->GetROP2();
			DrawActive(pDC,point); 
			ShowSelectPoint(pDC);
			DrawOldReferencePoint(pDC,GetDstPoint(m_FoundPoint));
			m_HaveFindFirst=true;
			pCaption->m_CurrentDrawTool=Graph_EllipseArc;
			pCaption->m_CurrentDrawStatus=Change_Status;
			pCaption->m_FirstPoint=m_FirstPoint;
			pCaption->m_SecondPoint=m_SecondPoint;

			switch (pCaption->L_iPointCount)
			{
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				pCaption->m_CurrentDrawTool=Graph_RoundArc;
				pCaption->m_CurrentDrawStatus=Drag_Status;
				break;
			default:
				;
			}
		}//***** if(L_iPointCount!=0)
	}
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
			m_HaveFindFirst=false;
			pCaption->m_bHaveFindFirst=true;
			ShowSelectPoint(pDC);
		}
	}
}

void CRoundArc::OnMouseMove(CDC *pDC, PGRAPHCAPTION pCaption, CPoint point)
{
	if(pCaption->m_CurrentDrawStatus==Drag_Status)
	{
		m_FirstPoint.x+=pCaption->m_SecondPoint.x-pCaption->m_FirstPoint.x;
		m_FirstPoint.y+=pCaption->m_SecondPoint.y-pCaption->m_FirstPoint.y;
		m_SecondPoint.x+=pCaption->m_SecondPoint.x-pCaption->m_FirstPoint.x;
		m_SecondPoint.y+=pCaption->m_SecondPoint.y-pCaption->m_FirstPoint.y;
		m_ThirdPoint.x+=pCaption->m_SecondPoint.x-pCaption->m_FirstPoint.x;
		m_ThirdPoint.y+=pCaption->m_SecondPoint.y-pCaption->m_FirstPoint.y;
		m_CenterPoint.x+=pCaption->m_SecondPoint.x-pCaption->m_FirstPoint.x;
		m_CenterPoint.y+=pCaption->m_SecondPoint.y-pCaption->m_FirstPoint.y;
	}
	else
	{
		pDC->SetROP2(R2_NOTXORPEN);
		if(pCaption->m_LBDownTimes==1&&pCaption->m_CurrentDrawStatus!=Change_Status)
		{
			ShowMovingLine(pDC,GetDstPoint(pCaption->m_FirstPoint),
								GetDstPoint(pCaption->m_SecondPoint));
			pCaption->m_SecondPoint=GetSrcPoint(point);
			ShowMovingLine(pDC,GetDstPoint(pCaption->m_FirstPoint),point);
		}
		if(pCaption->m_LBDownTimes==2&&pCaption->m_CurrentDrawStatus!=Change_Status)
		{
			DrawActive(pDC,point);
			m_ThirdPoint=GetSrcPoint(point);
			ComputeSloap2(GetDstPoint(m_SecondPoint),GetDstPoint(m_ThirdPoint));
			DrawActive(pDC,point);
		}
		if(pCaption->m_CurrentDrawStatus==Change_Status)
		{
			DrawActive(pDC,point);
			if(pCaption->L_iPointCount==1)
			{
				m_FirstPoint=GetSrcPoint(point);
				ComputeSloap1(GetDstPoint(m_FirstPoint),GetDstPoint(m_SecondPoint));
				ComputeSloap2(GetDstPoint(m_SecondPoint),GetDstPoint(m_ThirdPoint));
			}
			if(pCaption->L_iPointCount==2)
			{
				m_SecondPoint=GetSrcPoint(point);
				ComputeSloap1(GetDstPoint(m_FirstPoint),GetDstPoint(m_SecondPoint));
				ComputeSloap2(GetDstPoint(m_SecondPoint),GetDstPoint(m_ThirdPoint));
			}
			if(pCaption->L_iPointCount==3)
			{
				m_ThirdPoint=GetSrcPoint(point);
				ComputeSloap2(GetDstPoint(m_SecondPoint),GetDstPoint(m_ThirdPoint));
			}
	     DrawActive(pDC,point);
			 pCaption->m_SecondPoint=GetSrcPoint(point);
		}//******if(m_CurrentDrawStatus==Exchange_Status)
	}
}

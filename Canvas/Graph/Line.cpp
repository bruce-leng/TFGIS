#include "stdafx.h"
#include "Line.h"

IMPLEMENT_SERIAL(CLine,CUnit,1)
void CLine::DrawStatic(CDC*pDC)
{
	CPen m_pen;
	m_pen.CreatePen(m_PenStyle,m_PenWidth,m_PenColor);
	CPen* pen=(CPen*)pDC->SelectObject(&m_pen);
	int oldDrawingMode=pDC->SetROP2(m_DrawingMode);
	COLORREF OldColor=pDC->SetBkColor(m_BackgroundColor);
	int OldBkMode=pDC->SetBkMode(m_BkMode);
	
	pDC->MoveTo(GetDstPoint(m_FirstPoint));
	pDC->LineTo(GetDstPoint(m_SecondPoint));
	pDC->SelectObject(pen);
	pDC->SetROP2(oldDrawingMode);
	pDC->SetBkMode(m_BkMode);
	pDC->SetBkColor(OldColor);
}

CLine::CLine()
{ 
	CUnit::Initial();
}

CLine::CLine(CLine& line)
{
  m_FirstPoint=line.m_FirstPoint;
  m_SecondPoint=line.m_SecondPoint;
  m_PenColor=line.m_PenColor;
  m_PenWidth=line.m_PenWidth;
  m_PenStyle=line.m_PenStyle;

  m_pRgn=NULL;

	//////////////////////////////////////////////////////////////////////////
	//×·¼Ó(07.07.30)
	m_DrawingMode=line.m_DrawingMode;
	m_BkMode=line.m_BkMode;
	m_BackgroundColor=line.m_BackgroundColor;
	//////////////////////////////////////////////////////////////////////////
}

CRgn* CLine::GetRgn()
{
  if(m_pRgn==NULL)
		m_pRgn=new CRgn; 
	else m_pRgn->DeleteObject();
	{
		CPoint point[4];
		CPoint FirstPoint0=GetDstPoint(m_FirstPoint);
		CPoint SecondPoint0=GetDstPoint(m_SecondPoint);
		CPoint FirstPoint, SecondPoint;
		if((FirstPoint0.x-SecondPoint0.x)*(FirstPoint0.y-SecondPoint0.y)<0)
		{
			if(FirstPoint0.x>SecondPoint0.x)
			{ 
				FirstPoint=SecondPoint0;
				SecondPoint=FirstPoint0;   
			}
			else
			{
				FirstPoint=FirstPoint0;
				SecondPoint=SecondPoint0;
			}
			FirstPoint.x -= 4+m_PenWidth;
			FirstPoint.y += 4+m_PenWidth;

			SecondPoint.x += 4+m_PenWidth;
			SecondPoint.y += 4+m_PenWidth;

			point[0].x=FirstPoint.x-3-m_PenWidth;
			point[0].y=FirstPoint.y-4-m_PenWidth;

			point[1].x=FirstPoint.x+3+m_PenWidth;
			point[1].y=FirstPoint.y+4+m_PenWidth;
			
			point[2].x=SecondPoint.x+3+m_PenWidth;
			point[2].y=SecondPoint.y+4+m_PenWidth;
			
			point[3].x=SecondPoint.x-3-m_PenWidth;
			point[3].y=SecondPoint.y-4-m_PenWidth;
		}

		if((FirstPoint0.x-SecondPoint0.x)*(FirstPoint0.y-SecondPoint0.y)>0)
		{
			if(FirstPoint0.x>SecondPoint0.x)
			{ 
				FirstPoint=FirstPoint0;
				SecondPoint=SecondPoint0;   
			}
			else
			{
				FirstPoint=SecondPoint0;
				SecondPoint=FirstPoint0; 
			}
			FirstPoint.x -= 4+m_PenWidth;
			FirstPoint.y -= 4+m_PenWidth;

			SecondPoint.x += 4+m_PenWidth;
			SecondPoint.y += 4+m_PenWidth;

			point[0].x=FirstPoint.x+3+m_PenWidth;
			point[0].y=FirstPoint.y-4-m_PenWidth;
			
			point[1].x=FirstPoint.x-3-m_PenWidth;
			point[1].y=FirstPoint.y+4+m_PenWidth;
			
			point[2].x=SecondPoint.x-3-m_PenWidth;
			point[2].y=SecondPoint.y+4+m_PenWidth;
			
			point[3].x=SecondPoint.x+3+m_PenWidth;
			point[3].y=SecondPoint.y-4-m_PenWidth;
		}

		if((FirstPoint0.x-SecondPoint0.x)==0)
		{
			point[0].x=FirstPoint0.x-3-m_PenWidth;
			point[0].y=FirstPoint0.y-4-m_PenWidth;
			
			point[1].x=FirstPoint0.x+3+m_PenWidth;
			point[1].y=FirstPoint0.y-4-m_PenWidth;
			
			point[2].x=SecondPoint0.x+3+m_PenWidth;
			point[2].y=SecondPoint0.y+4+m_PenWidth;
			
			point[3].x=SecondPoint0.x-3-m_PenWidth;
			point[3].y=SecondPoint0.y+4+m_PenWidth;
		}

		if((FirstPoint0.y-SecondPoint0.y)==0)
		{
			point[0].x=FirstPoint0.x-4-m_PenWidth;
			point[0].y=FirstPoint0.y-3-m_PenWidth;
			
			point[1].x=FirstPoint0.x-4-m_PenWidth;
			point[1].y=FirstPoint0.y+3+m_PenWidth;
			
			point[2].x=SecondPoint0.x+4+m_PenWidth;
			point[2].y=SecondPoint0.y+3+m_PenWidth;
			
			point[3].x=SecondPoint0.x+4+m_PenWidth;
			point[3].y=SecondPoint0.y-3-m_PenWidth;
		}
		m_pRgn->CreatePolygonRgn(point,4,ALTERNATE);
  }
  return m_pRgn;
}

void CLine::Initial()
{
  m_FirstPoint=POINT2DF(0,0);
  m_SecondPoint=POINT2DF(0,0);
  m_PenColor=RGB(255,0,0);
  m_PenWidth=1;
  m_PenStyle=PS_SOLID;
}

int CLine::IsOnMarginPoint(CPoint point)
{
	CPoint FirstPoint=GetDstPoint(m_FirstPoint);
	CPoint SecondPoint=GetDstPoint(m_SecondPoint);

	CRect L_Rect1;
	CRect L_Rect2;
	CRect L_Rect3;
	L_Rect1.SetRect(FirstPoint,FirstPoint);
	L_Rect2.SetRect(SecondPoint,SecondPoint);
	L_Rect3.SetRect((SecondPoint.x+FirstPoint.x)/2,(SecondPoint.y+FirstPoint.y)/2,
									(SecondPoint.x+FirstPoint.x)/2,(SecondPoint.y+FirstPoint.y)/2);
	L_Rect1.InflateRect(5+m_PenWidth,5+m_PenWidth);
	L_Rect2.InflateRect(5+m_PenWidth,5+m_PenWidth);
	L_Rect3.InflateRect(5+m_PenWidth,5+m_PenWidth); 
	if(L_Rect1.PtInRect(point))
	{   
		  m_FoundPoint=m_FirstPoint;
			return 1;
	}
	if(L_Rect2.PtInRect(point))
	{
		m_FoundPoint=m_SecondPoint;
		return 2;
	}
	if(L_Rect3.PtInRect(point))
	{
		m_FoundPoint=POINT2DF((m_SecondPoint.x+m_FirstPoint.x)/2,(m_SecondPoint.y+m_FirstPoint.y)/2);
		return 3;
	}

	return 0;
}

void CLine::Circumrotate(CPoint first,CPoint second)
{
	CPoint FirstPoint=GetDstPoint(m_FirstPoint);
	CPoint SecondPoint=GetDstPoint(m_SecondPoint);

	double k;
	int x;
	if(FirstPoint.x-SecondPoint.x==0)
		k=99999999999;
	else  k=double(first.y-second.y)/(first.x-second.x);
	
	x=(k*(first.y+second.y)+(first.x+second.x)+(k*k-1)*FirstPoint.x-2*k*FirstPoint.y)/(k*k+1);
	FirstPoint.y=k*(x-FirstPoint.x)+FirstPoint.y;
	FirstPoint.x=x;
	x=(k*(first.y+second.y)+(first.x+second.x)+(k*k-1)*SecondPoint.x-2*k*SecondPoint.y)/(k*k+1);
	SecondPoint.y=k*(x-SecondPoint.x)+SecondPoint.y;
	SecondPoint.x=x;

	m_FirstPoint=GetSrcPoint(FirstPoint);
	m_SecondPoint=GetSrcPoint(SecondPoint);
}

void CLine::ShowSelectPoint(CDC*pDC)
{
	CPoint FirstPoint=GetDstPoint(m_FirstPoint);
	CPoint SecondPoint=GetDstPoint(m_SecondPoint);

	CBrush brush;
	brush.CreateSolidBrush(RGB(0,0,255));
	CPen m_pen;
	m_pen.CreatePen(PS_SOLID,1,RGB(0,0,255));
	CPen *OldPen=pDC->SelectObject(&m_pen);
	int oldBkMode=pDC->SetBkMode(OPAQUE); 
	CBrush *OldBrush=pDC->SelectObject(&brush);
	int oldDrawingMode=pDC->SetROP2(R2_NOTXORPEN);
	CRect rect(FirstPoint,FirstPoint);
	rect.InflateRect(3,3);
	pDC->Rectangle(rect);
	
	rect=CRect(SecondPoint,SecondPoint);
	rect.InflateRect(3,3);
	pDC->Rectangle(rect);
	pDC->SelectObject(OldBrush);
	brush.DeleteObject();
	brush.CreateSolidBrush(RGB(255,0,0));
	OldBrush=pDC->SelectObject(&brush);
	rect=CRect((FirstPoint.x+SecondPoint.x)/2,
						 (FirstPoint.y+SecondPoint.y)/2,
						 (FirstPoint.x+SecondPoint.x)/2,
						 (FirstPoint.y+SecondPoint.y)/2);
	rect.InflateRect(3,3);
	pDC->Rectangle(rect);
	
	pDC->SelectObject(OldPen);
	pDC->SetBkMode(oldBkMode); 
	pDC->SelectObject(OldBrush);
	pDC->SetROP2(oldDrawingMode);
}

BOOL CLine::IsInRgn(CPoint point)
{
	this->GetRgn();
	return(m_pRgn->PtInRegion(point));
}

void CLine::DrawActive(CDC *pDC,CPoint point)
{
	CPen m_pen;
	m_pen.CreatePen(m_PenStyle,m_PenWidth,m_PenColor);
	CPen* pen=(CPen*)pDC->SelectObject(&m_pen);
	
	COLORREF OldColor=pDC->SetBkColor(m_BackgroundColor);
	int OldBkMode=pDC->SetBkMode(m_BkMode);
	
	pDC->MoveTo(GetDstPoint(m_FirstPoint));
	pDC->LineTo(GetDstPoint(m_SecondPoint));
	pDC->SelectObject(pen);

	pDC->SetBkMode(m_BkMode);
	pDC->SetBkColor(OldColor);
}

void CLine::OnLButtonDown(CDC *pDC, PGRAPHCAPTION pCaption, CPoint point)
{
	if(m_HaveFindFirst)
	{
		pCaption->L_iPointCount=IsOnMarginPoint(point);
		if(pCaption->L_iPointCount==1)
		{ 
			pCaption->m_bHaveFindSecond=true;
			pCaption->m_CurrentDrawTool=Graph_Line;
			pCaption->m_CurrentDrawStatus=Change_Status;
			pCaption->m_FirstPoint=m_SecondPoint;
			pCaption->m_SecondPoint=GetSrcPoint(point);
		}
		if(pCaption->L_iPointCount==2)
		{   
			pCaption->m_bHaveFindSecond=true;
			pCaption->m_CurrentDrawTool=Graph_Line;
			pCaption->m_CurrentDrawStatus=Change_Status;
			pCaption->m_FirstPoint=m_FirstPoint;
			pCaption->m_SecondPoint=GetSrcPoint(point);
		}
		if(pCaption->L_iPointCount==3)
		{
			pCaption->m_bHaveFindSecond=true;
			pCaption->m_CurrentDrawTool=Graph_Line;
			pCaption->m_CurrentDrawStatus=Drag_Status;
			pCaption->m_FirstPoint=m_FirstPoint;
			pCaption->m_SecondPoint=m_SecondPoint;
		}
	}//**********if(L_pDLine->m_HaveFindFirst)
	if((!pCaption->m_bHaveFindSecond)&&IsInRgn(point))
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
			this->ShowSelectPoint(pDC);
		}
	}
	
	if(pCaption->m_bHaveFindSecond)
	{
		pCaption->m_pCurrentUnit=this;
		m_DrawingMode=pDC->GetROP2();
		ShowSelectPoint(pDC);
		DrawStatic(pDC);
		DrawOldReferencePoint(pDC,GetDstPoint(m_FoundPoint));
	}
}

void CLine::OnMouseMove(CDC *pDC, PGRAPHCAPTION pCaption, CPoint point)
{
	if(pCaption->m_CurrentDrawStatus==Drag_Status)
	{
		m_FirstPoint.x+=pCaption->m_SecondPoint.x-pCaption->m_FirstPoint.x;
		m_FirstPoint.y+=pCaption->m_SecondPoint.y-pCaption->m_FirstPoint.y;

		m_SecondPoint.x+=pCaption->m_SecondPoint.x-pCaption->m_FirstPoint.x;
		m_SecondPoint.y+=pCaption->m_SecondPoint.y-pCaption->m_FirstPoint.y;
	}
	else
	{
		pDC->SetROP2(R2_NOTXORPEN);
		//m_DrawingMode=pDC->GetROP2();
		this->DrawActive(pDC,point);
		pCaption->m_SecondPoint=GetSrcPoint(point);
		//m_pCurrentUnit->m_FirstPoint=m_FirstPoint;
		m_SecondPoint=pCaption->m_SecondPoint;
		this->DrawActive(pDC,point);
	}
}

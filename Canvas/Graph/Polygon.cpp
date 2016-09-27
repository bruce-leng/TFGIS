#include "stdafx.h"
#include "Polygon.h"
IMPLEMENT_SERIAL(CPolygon,CUnit,1)
CPolygon::CPolygon()
{
	Initial();
}

CPolygon::CPolygon(CPolygon& polygon)
{
	CUnit::Initial();

	m_bClose = polygon.m_bClose;

	m_PenColor=polygon.m_PenColor;
	m_PenWidth=polygon.m_PenWidth;
	m_PenStyle=polygon.m_PenStyle;

	m_pRgn=NULL;

	m_DrawingMode=polygon.m_DrawingMode;
	m_BkMode=polygon.m_BkMode;
	m_BackgroundColor=polygon.m_BackgroundColor;

	m_nPreLink=polygon.m_nPreLink;
	m_nNextLink=polygon.m_nNextLink;
	
	m_pPointList=new CArray<POINT2DF, POINT2DF>;
	for (int i=0; i<polygon.m_pPointList->GetSize(); i++)
	{
		POINT2DF point=polygon.m_pPointList->GetAt(i);
		m_pPointList->Add(point);
	}
}

CPolygon::~CPolygon()
{
	if (m_pPointList)
		delete m_pPointList;
}

void CPolygon::Serialize(CArchive &ar)
{
	CUnit::Serialize(ar);
	if(ar.IsStoring())
	{
		ar<<m_PenColor<<m_PenStyle<<m_PenWidth<<m_bClose<<m_nPreLink<<m_nNextLink;
	}
	else
	{
		ar>>m_PenColor>>m_PenStyle>>m_PenWidth>>m_bClose>>m_nPreLink>>m_nNextLink;
	}
	m_pPointList->Serialize(ar);
}

void CPolygon::Initial()
{
  CUnit::Initial();
  m_pPointList=new CArray<POINT2DF,POINT2DF>;
}

void CPolygon::DrawStatic(CDC *pDC)
{
	if (m_pPointList->GetSize()==0)
		return;

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
	m_FirstPoint=m_pPointList->GetAt(0);
	pDC->MoveTo(GetDstPoint(m_FirstPoint));
	for(int i=0;i<m_pPointList->GetSize();i++)
	{
		pDC->LineTo(GetDstPoint(m_pPointList->GetAt(i)));
	}

	if (m_bClose)
	{
		pDC->LineTo(GetDstPoint(m_pPointList->GetAt(0)));

		if(m_nFillMapIdx>=0)
		{
			//绘制填充图元
			CRect rect = GetPolyRect();
			long double dbZoom;
			GetZoomFactor(dbZoom,dbZoom);
			rect.left*=dbZoom;
			rect.top*=dbZoom;
			rect.right*=dbZoom;
			rect.bottom*=dbZoom;

			int nPtNum = GetPointNum();
			POINT* pPts = new POINT [nPtNum];
			for (int k=0; k<nPtNum; k++)
			{
				POINT2DF pt = GetPoint(k);
				pPts[k] = GetDstPoint(pt);
			}
			CRgn rgn;
			rgn.CreatePolygonRgn(pPts, nPtNum, WINDING);
			for (int m=rect.left; m<rect.right; m+=15)
			{
				for (int n=rect.top; n<rect.bottom; n+=15)
				{
					if(rgn.PtInRegion(CPoint(m,n)))
						pDC->Ellipse(m-2,n-2,m+2,n+2);
				}
			}
			delete [] pPts;
		}
	}
//	pDC->SelectObject(brush);
	pDC->SelectObject(pen);
	pDC->SetBkMode(OldBkMode);
	pDC->SetBkColor(OldColor);
	pDC->SetROP2(oldDrawingMode);
}
void CPolygon::DrawMask(CDC *pDC, CPoint point)
{
	CPen m_pen;
  CBrush m_brush;
  //int oldDrawingMode=pDC->SetROP2(this->m_DrawingMode);
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
	pDC->MoveTo(GetDstPoint(m_pPointList->GetAt(m_pPointList->GetSize()-1)));
	pDC->LineTo(point);
	pDC->SelectObject(brush);
	pDC->SelectObject(pen);
	pDC->SetBkMode(OldBkMode);
	pDC->SetBkColor(OldColor);
	//pDC->SetROP2(oldDrawingMode);
}

void CPolygon::DrawEnd(CDC*pDC,CPoint point)
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

	if (m_bClose)
	{
		pDC->MoveTo(point);
		pDC->LineTo(GetDstPoint(m_pPointList->GetAt(0)));
	}

	pDC->SelectObject(brush);
	pDC->SelectObject(pen);
	pDC->SetBkMode(OldBkMode);
	pDC->SetBkColor(OldColor);
	pDC->SetROP2(oldDrawingMode);
}

int CPolygon::IsOnMarginPoint(CPoint point)
{
  CRect rect;
  CPoint firstPoint;
  CPoint secondPoint;
  firstPoint=secondPoint=GetDstPoint(m_pPointList->GetAt(0));
  for(int i=0;i<m_pPointList->GetSize();i++)
  {
		secondPoint=GetDstPoint(m_pPointList->GetAt(i));
		rect=CRect(secondPoint,secondPoint);
		rect.InflateRect(4,4);
		if(rect.PtInRect(point))
		{ 
			m_FoundIndex=i;
			m_FoundPoint=m_pPointList->GetAt(i);
			return i;
		} 
		rect.SetRect(CPoint((firstPoint.x+secondPoint.x)/2,(firstPoint.y+secondPoint.y)/2),
								 CPoint((firstPoint.x+secondPoint.x)/2,(firstPoint.y+secondPoint.y)/2));
		rect.InflateRect(4,4);
		if(rect.PtInRect(point))
		{
			m_FoundPoint=GetSrcPoint(CPoint((firstPoint.x+secondPoint.x)/2,
																		 (firstPoint.y+secondPoint.y)/2));
			return -i-10;
		}   
		firstPoint=secondPoint;
  }
  return -1;
}

BOOL CPolygon::IsInRgn(CPoint point)
{
	CPoint point4[4];
	CPoint FirstPoint0,SecondPoint0;
	CPoint FirstPoint,SecondPoint;
	int nSize = m_pPointList->GetSize();
	if (nSize == 0)
		return FALSE;
	m_FirstPoint=m_pPointList->GetAt(m_pPointList->GetSize()-1);
	CRgn rgn;
	for(int i=0;i<m_pPointList->GetSize();i++)
	{
		m_SecondPoint=m_pPointList->GetAt(i);
		FirstPoint0=GetDstPoint(m_FirstPoint);
		SecondPoint0=GetDstPoint(m_SecondPoint);
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

			point4[0].x=FirstPoint.x-3-m_PenWidth;
			point4[0].y=FirstPoint.y-4-m_PenWidth;
			
			point4[1].x=FirstPoint.x+3+m_PenWidth;
			point4[1].y=FirstPoint.y+4+m_PenWidth;
			
			point4[2].x=SecondPoint.x+3+m_PenWidth;
			point4[2].y=SecondPoint.y+4+m_PenWidth;
			
			point4[3].x=SecondPoint.x-3-m_PenWidth;
			point4[3].y=SecondPoint.y-4-m_PenWidth;
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

			point4[0].x=FirstPoint.x+3+m_PenWidth;
			point4[0].y=FirstPoint.y-4-m_PenWidth;
			
			point4[1].x=FirstPoint.x-3-m_PenWidth;
			point4[1].y=FirstPoint.y+4+m_PenWidth;
			
			point4[2].x=SecondPoint.x-3-m_PenWidth;
			point4[2].y=SecondPoint.y+4+m_PenWidth;
			
			point4[3].x=SecondPoint.x+3+m_PenWidth;
			point4[3].y=SecondPoint.y-4-m_PenWidth;
		}
		
		if((FirstPoint0.x-SecondPoint0.x)==0)
		{
			point4[0].x=FirstPoint0.x-3-m_PenWidth;
			point4[0].y=FirstPoint0.y-4-m_PenWidth;
			
			point4[1].x=FirstPoint0.x+3+m_PenWidth;
			point4[1].y=FirstPoint0.y-4-m_PenWidth;
			
			point4[2].x=SecondPoint0.x+3+m_PenWidth;
			point4[2].y=SecondPoint0.y+4+m_PenWidth;
			
			point4[3].x=SecondPoint0.x-3-m_PenWidth;
			point4[3].y=SecondPoint0.y+4+m_PenWidth;
		}
		
		if((FirstPoint0.y-SecondPoint0.y)==0)
		{
			point4[0].x=FirstPoint0.x-4-m_PenWidth;
			point4[0].y=FirstPoint0.y-3-m_PenWidth;
			
			point4[1].x=FirstPoint0.x-4-m_PenWidth;
			point4[1].y=FirstPoint0.y+3+m_PenWidth;
			
			point4[2].x=SecondPoint0.x+4+m_PenWidth;
			point4[2].y=SecondPoint0.y+3+m_PenWidth;
			
			point4[3].x=SecondPoint0.x+4+m_PenWidth;
			point4[3].y=SecondPoint0.y-3-m_PenWidth;
		}
		m_FirstPoint=m_SecondPoint;  
		rgn.CreatePolygonRgn(point4,4,ALTERNATE);
		if(rgn.PtInRegion(point))
			return true;
		rgn.DeleteObject();
	}

  return false;
}

void CPolygon::ExchangeDraw(CDC *pDC, CPoint point)
{
 //m_FoundIndex
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
  
	m_SecondPoint=m_pPointList->GetAt((m_FoundIndex+1)%m_pPointList->GetSize());
	m_FirstPoint=m_pPointList->GetAt(m_FoundIndex==0? m_pPointList->GetSize()-1:m_FoundIndex-1);

	if (!m_bClose)	// 若为多段线
	{
		if (m_FoundIndex==0)
		{
			pDC->MoveTo(point);
			pDC->LineTo(GetDstPoint(m_SecondPoint));
		}
		else if (m_FoundIndex==m_pPointList->GetSize()-1)
		{
			pDC->MoveTo(point);
			pDC->LineTo(GetDstPoint(m_FirstPoint));
		}
		else
		{
			pDC->MoveTo(GetDstPoint(m_FirstPoint));
			pDC->LineTo(point);
			pDC->MoveTo(point);
			pDC->LineTo(GetDstPoint(m_SecondPoint));
		}
	}
	else	// 若为多边形
	{
		pDC->MoveTo(GetDstPoint(m_FirstPoint));
		pDC->LineTo(point);
		pDC->MoveTo(point);
		pDC->LineTo(GetDstPoint(m_SecondPoint));
	}

	m_pPointList->SetAt(m_FoundIndex,GetSrcPoint(point));
	pDC->SelectObject(brush);
	pDC->SelectObject(pen);
	pDC->SetBkMode(OldBkMode);
	pDC->SetBkColor(OldColor);
	pDC->SetROP2(oldDrawingMode);
}

void CPolygon::ShowSelectPoint(CDC *pDC)
{
  CRect rect;
  POINT2DF firstPoint;
  POINT2DF secondPoint;
  firstPoint=secondPoint=m_pPointList->GetAt(0);
  CBrush brush;
  brush.CreateSolidBrush(RGB(0,0,255));
  CPen m_pen;
  m_pen.CreatePen(PS_SOLID,1,RGB(0,0,255));
  CPen *OldPen=pDC->SelectObject(&m_pen);
  int oldBkMode=pDC->SetBkMode(OPAQUE); 
  CBrush *OldBrush=pDC->SelectObject(&brush);
  int oldDrawingMode=pDC->SetROP2(R2_NOTXORPEN);
  for(int i=0;i<m_pPointList->GetSize();i++)
  {
   rect=CRect(GetDstPoint(m_pPointList->GetAt(i)),
							GetDstPoint(m_pPointList->GetAt(i)));
   rect.InflateRect(3,3);
   pDC->Rectangle(rect);
  }
   
  pDC->SelectObject(OldBrush);
  brush.DeleteObject();
  brush.CreateSolidBrush(RGB(255,0,0));
  pDC->SelectObject(&brush);
   firstPoint=m_pPointList->GetAt(0);
   //secondPoint=m_pPointList->GetAt(1);
  for(i=1;i<m_pPointList->GetSize();i++)
  {
   secondPoint=m_pPointList->GetAt(i);
	 POINT2DF pt;
	 pt.x = (firstPoint.x+secondPoint.x)/2;
	 pt.y = (firstPoint.y+secondPoint.y)/2;
   rect.SetRect(GetDstPoint(pt),GetDstPoint(pt));
   rect.InflateRect(3,3);
   pDC->Rectangle(rect);
   firstPoint=secondPoint;
  }
  
  pDC->SelectObject(OldPen);
  pDC->SetBkMode(oldBkMode); 
  pDC->SelectObject(OldBrush);
  pDC->SetROP2(oldDrawingMode);
}

void CPolygon::DrawActive(CDC *pDC,CPoint point)
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
	
  CBrush*brush=(CBrush*)pDC->SelectObject(&m_brush);
	
	int OldBkMode=pDC->SetBkMode(m_BkMode);
	COLORREF OldColor=pDC->SetBkColor(m_BackgroundColor);
	m_FirstPoint=m_pPointList->GetAt(0);
	pDC->MoveTo(GetDstPoint(m_FirstPoint));
	for(int i=0;i<m_pPointList->GetSize();i++)
	{
		pDC->LineTo(GetDstPoint(m_pPointList->GetAt(i)));
	}
	if (m_bClose)
		pDC->LineTo(GetDstPoint(m_pPointList->GetAt(0)));
	pDC->SelectObject(brush);
	pDC->SelectObject(pen);
	pDC->SetBkMode(OldBkMode);
	pDC->SetBkColor(OldColor);
}

void CPolygon::OnLButtonDown(CDC *pDC, PGRAPHCAPTION pCaption, CPoint point)
{
  if(m_HaveFindFirst)
  {
		pCaption->L_iPointCount=IsOnMarginPoint(point);
		if(pCaption->L_iPointCount>=0)
		{     
			pCaption->m_bHaveFindSecond=true;
			pCaption->m_CurrentDrawTool=Graph_Polygon;
			pCaption->m_CurrentDrawStatus=Change_Status;
			
		}
		// 被选中图形进行移动
		if(pCaption->L_iPointCount<-1)
		{  
			pCaption->m_bHaveFindSecond=true;
			pCaption->m_CurrentDrawTool=Graph_Polygon;
			pCaption->m_CurrentDrawStatus=Drag_Status;

			pCaption->m_MinPoint=pCaption->m_MaxPoint=m_pPointList->GetAt(0);

			for (int i=0; i<GetPointNum(); i++)
			{
				POINT2DF pt = GetPoint(i);
			   pCaption->m_MinPoint.x=min(pCaption->m_MinPoint.x,m_pPointList->GetAt(i).x);
			   pCaption->m_MinPoint.y=min(pCaption->m_MinPoint.y,m_pPointList->GetAt(i).y);
			   pCaption->m_MaxPoint.x=max(pCaption->m_MaxPoint.x,m_pPointList->GetAt(i).x);
			   pCaption->m_MaxPoint.y=max(pCaption->m_MaxPoint.y,m_pPointList->GetAt(i).y);
			}
		}
	}//*******if(L_pPolygon->m_HaveFindFirst)

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
			this->ShowSelectPoint(pDC);
		}
	}
	if(pCaption->m_bHaveFindSecond)
  {
		pCaption->m_pCurrentUnit=this;
		m_DrawingMode=pDC->GetROP2();
		ShowSelectPoint(pDC);
		DrawOldReferencePoint(pDC,GetDstPoint(m_FoundPoint));
		DrawStatic(pDC);
	}
}

void CPolygon::OnMouseMove(CDC *pDC, PGRAPHCAPTION pCaption, CPoint point)
{
	if(pCaption->m_CurrentDrawStatus==Draw_Status)
	{
		if(pCaption->m_LBDownTimes!=0)
		{
			pDC->SetROP2(R2_NOTXORPEN);
			//m_DrawingMode=pDC->GetROP2();
			DrawMask(pDC,GetDstPoint(pCaption->m_SecondPoint));
			pCaption->m_SecondPoint=GetSrcPoint(point);
			pDC->SetROP2(this->m_DrawingMode);
			DrawMask(pDC,GetDstPoint(pCaption->m_SecondPoint));
		}
	}
	if(pCaption->m_CurrentDrawStatus==Change_Status)
	{
		pDC->SetROP2(R2_NOTXORPEN);
		ExchangeDraw(pDC,GetDstPoint(pCaption->m_SecondPoint));
		pCaption->m_SecondPoint=GetSrcPoint(point);
		ExchangeDraw(pDC,GetDstPoint(pCaption->m_SecondPoint));
	}
	if(pCaption->m_CurrentDrawStatus==Drag_Status)
	{
		for(int i=0;i<m_pPointList->GetSize();i++)
		{
			POINT2DF pt=m_pPointList->GetAt(i);
			pt.x += pCaption->m_SecondPoint.x-pCaption->m_FirstPoint.x;
			pt.y += pCaption->m_SecondPoint.y-pCaption->m_FirstPoint.y;
			m_pPointList->SetAt(i,pt);
		}

		// 此处设置移动起始点和终止点
// 		m_FirstPoint = pCaption->m_FirstPoint;
// 		m_SecondPoint = pCaption->m_SecondPoint;
	}
}

// 得到多边形顶点数(08.09.08)
int CPolygon::GetPointNum()
{
	return m_pPointList->GetSize();
}

POINT2DF CPolygon::GetPoint(int nIdx)
{
	return m_pPointList->GetAt(nIdx);
}

// 取得多边形的外接矩形
RECT CPolygon::GetPolyRect()
{
	RECT rect;
	rect.left   = INT_MAX;
	rect.top    = INT_MAX;
	rect.right  = INT_MIN;
	rect.bottom = INT_MIN;

	for (int i=0; i<GetPointNum(); i++)
	{
		POINT2DF pt = GetPoint(i);
		if (rect.left	> pt.x)		rect.left	= pt.x;
		if (rect.top	> pt.y)		rect.top	= pt.y;
		if (rect.right	< pt.x)		rect.right	= pt.x;
		if (rect.bottom	< pt.y)		rect.bottom	= pt.y;
	}

	rect.right++;
	rect.bottom++;

	return rect;
}

// 将宽度缩放dbZoomX比率,高度缩放dbZoomY,将缩放后的尺寸设为原始尺寸(08.09.17)
void CPolygon::Reset(long double dbZoomX, long double dbZoomY)
{
	for(int i=0;i<m_pPointList->GetSize();i++)
	{
		POINT2DF pt = m_pPointList->GetAt(i);
		pt.x *= dbZoomX;
		pt.y *= dbZoomY;
		m_pPointList->SetAt(i, pt);
	}
}

CRect CPolygon::GetBoundingRect()
{
	m_MinPoint=m_MaxPoint=m_pPointList->GetAt(0);

	for (int i=0; i<GetPointNum(); i++)
	{
		POINT2DF pt = GetPoint(i);
	   m_MinPoint.x=min(m_MinPoint.x,m_pPointList->GetAt(i).x);
	   m_MinPoint.y=min(m_MinPoint.y,m_pPointList->GetAt(i).y);
	   m_MaxPoint.x=max(m_MaxPoint.x,m_pPointList->GetAt(i).x);
	   m_MaxPoint.y=max(m_MaxPoint.y,m_pPointList->GetAt(i).y);
	}

	this->m_BoundingRect.SetRect(m_MinPoint.x,m_MinPoint.y,m_MaxPoint.x,m_MaxPoint.y);
	return this->m_BoundingRect;
}
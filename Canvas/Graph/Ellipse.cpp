#include "stdafx.h"
#include "Ellipse.h"
IMPLEMENT_SERIAL(CEllipse,CUnit,1)
void CEllipse::DrawStatic(CDC*pDC)
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
	SetBrushOrg(pDC,&m_brush);
	CBrush*brush=(CBrush*)pDC->SelectObject(&m_brush);

	int OldBkMode=pDC->SetBkMode(m_BkMode);
	COLORREF OldColor=pDC->SetBkColor(m_BackgroundColor);

	POINT centerPoint = GetDstPoint(m_CenterPoint);
	ellipseMidpoint(pDC,centerPoint.x,centerPoint.y,this->m_Xr,this->m_Yr);
	pDC->SelectObject(brush);
	pDC->SelectObject(pen);
	pDC->SetBkMode(OldBkMode);
	pDC->SetBkColor(OldColor);
	pDC->SetROP2(oldDrawingMode);
}

void CEllipse::Serialize(CArchive &ar)
{
	CUnit::Serialize(ar);
  if(ar.IsStoring())
	{
	  ar<<m_BrushStyle<<m_BkMode<<m_BrushHatch<<m_BrushColor;
	  ar<<m_Sin<<m_Cos;
	  ar<<m_Xr<<m_Yr;
	  ar<<m_CenterPoint.x<<m_CenterPoint.y;
	}
	else
	{
	  ar>>m_BrushStyle>>m_BkMode>>m_BrushHatch>>m_BrushColor;
	  ar>>m_Sin>>m_Cos;
	  ar>>m_Xr>>m_Yr;
	  ar>>m_CenterPoint.x>>m_CenterPoint.y;
	}
}

BOOL CEllipse::IsInRgn(CPoint point)
{
    
  POINT centerPoint=GetDstPoint(m_CenterPoint);
	double vh[3][3]={m_Cos,-m_Sin,centerPoint.x*(1-m_Cos)+centerPoint.y*m_Sin,
		               m_Sin, m_Cos,centerPoint.y*(1-m_Cos)-centerPoint.x*m_Sin,
								   0,0,1
				          };       
       
  double xy[3];
  CPoint point1,point2,point3,point4,point11,point22,point33,point44;
  point1=CPoint(centerPoint.x-m_Xr,centerPoint.y+m_Yr);
  point2=CPoint(centerPoint.x+m_Xr,centerPoint.y+m_Yr);
  point3=CPoint(centerPoint.x+m_Xr,centerPoint.y-m_Yr);
  point4=CPoint(centerPoint.x-m_Xr,centerPoint.y-m_Yr);
  point11=CPoint(centerPoint.x-m_Xr/1.2,centerPoint.y+m_Yr/1.2);
  point22=CPoint(centerPoint.x+m_Xr/1.2,centerPoint.y+m_Yr/1.2);
  point33=CPoint(centerPoint.x+m_Xr/1.2,centerPoint.y-m_Yr/1.2);
  point44=CPoint(centerPoint.x-m_Xr/1.2,centerPoint.y-m_Yr/1.2);
	   
  CPoint pointList1[]={point1,point2,point3,point4};
  CPoint pointList2[]={point11,point22,point33,point44};
  
  for(int i=0;i<4;i++)
  {
  xy[0]=pointList1[i].x;
  xy[1]=pointList1[i].y;
  xy[2]=1;
  pointList1[i].x=vh[0][0]*xy[0]+vh[0][1]*xy[1]+vh[0][2]*xy[2];
  pointList1[i].y=vh[1][0]*xy[0]+vh[1][1]*xy[1]+vh[1][2]*xy[2];
  
  xy[0]=pointList2[i].x;
  xy[1]=pointList2[i].y;
  xy[2]=1;
  pointList2[i].x=vh[0][0]*xy[0]+vh[0][1]*xy[1]+vh[0][2]*xy[2];
  pointList2[i].y=vh[1][0]*xy[0]+vh[1][1]*xy[1]+vh[1][2]*xy[2];
  }

  CRgn rgn1;
  rgn1.CreatePolygonRgn(pointList1,4,ALTERNATE);
  CRgn rgn2;
  rgn2.CreatePolygonRgn(pointList2,4,ALTERNATE);
  if(rgn1.PtInRegion(point)&&!rgn2.PtInRegion(point))
	  return true;
  
  return false;
}

int CEllipse::IsOnMarginPoint(CPoint point)
{
	CRect rect;
	for(int i=0;i<5;i++)
	{
		rect=CRect(this->m_MarginPoint[i],this->m_MarginPoint[i]);
		rect.InflateRect(4,4);
		if(rect.PtInRect(point))
		{   
			m_FoundPoint.x=m_MarginPoint[i].x;
			m_FoundPoint.y=m_MarginPoint[i].y;
			return i+1;
		}
	}
	return 0;
}

void CEllipse::ellipseMidpoint(CDC*pDC,int xCenter, int yCenter, int Rx, int Ry)
{
	int Rx2=Rx*Rx;
	int Ry2=Ry*Ry;
	int twoRx2=2*Rx2;
	int twoRy2=2*Ry2;
	int p;
	int x=0;
	int y=Ry;
	int px=0;
	int py=twoRx2*y;
	ellipsePlotPoints(pDC,xCenter,yCenter,x,y);
	p=ROUND(Ry2-(Rx2*Ry)+(0.25*Rx2));
	while(px<py)
	{
		x++;
		px+=twoRy2;
		if(p<0)
			p+=Ry2+px;
		else
		{
			y--;
			py-=twoRx2;
			p+=Ry2+px-py;
		}
		ellipsePlotPoints(pDC,xCenter,yCenter,x,y);
	}
	p=ROUND(Ry2*(x+0.5)*(x+0.5)+Rx2*(y-1)*(y-1)-Rx2*Ry2);
	while(y>0)
	{
		y--;
		py-=twoRx2;
		if(p>0)
			p+=Rx2-py;
		else
		{
			x++;
			px+=twoRy2;
			p+=Rx2-py+px;
		}
		ellipsePlotPoints(pDC,xCenter,yCenter,x,y);
	}
}

void CEllipse::ellipsePlotPoints(CDC*pDC,int xCenter, int yCenter, int x, int y)
{
 /* CPen m_pen;
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
*/
	PdcSetPixel(pDC,xCenter+x,yCenter+y,m_PenColor);
	PdcSetPixel(pDC,xCenter-x,yCenter+y,m_PenColor);
	PdcSetPixel(pDC,xCenter+x,yCenter-y,m_PenColor);
	PdcSetPixel(pDC,xCenter-x,yCenter-y,m_PenColor);
/*
 pDC->SelectObject(brush);
 pDC->SelectObject(pen);
 pDC->SetBkMode(OldBkMode);
 pDC->SetBkColor(OldColor);
 pDC->SetROP2(oldDrawingMode);
 */
}

float CEllipse::ComputSloap(CPoint firstPoint, CPoint secondPoint)
{
	m_SecondPoint=GetSrcPoint(secondPoint);
	double sloap;
	if(firstPoint.x==secondPoint.x)
	{
		sloap=9999999999999.0;
	}
	else
	{
		sloap=double((firstPoint.y-secondPoint.y))/double((firstPoint.x-secondPoint.x));
	}
	
  if(sloap<0)
	{
		m_Cos=fabs(sloap/sqrt(sloap*sloap+1.0));
		m_Sin=1.0/sqrt(sloap*sloap+1.0);
	}
	else
	{
		m_Cos=fabs(sloap/sqrt(sloap*sloap+1.0));
		m_Sin=-1.0/sqrt(sloap*sloap+1.0);
	}
	
	return 0;
}

void CEllipse::PdcSetPixel(CDC *pDC, int xCenter, int yCenter, COLORREF m_PenColor)
{
	//xCenter+x,yCenter+y
	POINT centerPoint=GetDstPoint(m_CenterPoint);
	double vh[3][3]={m_Cos,-m_Sin,centerPoint.x*(1-m_Cos)+centerPoint.y*m_Sin,
					 m_Sin, m_Cos,centerPoint.y*(1-m_Cos)-centerPoint.x*m_Sin,
					 0,0,1
					};       
  
	double xy[3]={xCenter,yCenter,1};
	double x[3]={0,0,0};
	 
	x[0]=vh[0][0]*xy[0]+vh[0][1]*xy[1]+vh[0][2]*xy[2];
	x[1]=vh[1][0]*xy[0]+vh[1][1]*xy[1]+vh[1][2]*xy[2];
	if(x[0]-int(x[0])>=0.5)
		  x[0]+=1;
	if(x[1]-int(x[1])>=0.5)
		  x[1]+=1;
	
	for(int i=0;i<this->m_PenWidth;i++)
		pDC->SetPixel(x[0]+i,x[1]-i,m_PenColor);

	//pDC->LineTo(x[0],x[1]);
	// pDC->LineTo(x[0]+1,x[1]-1);
}

int CEllipse::ComputRadiusX(CPoint firstPoint,CPoint secondPoint)
{
	m_Xr=sqrt((firstPoint.x-secondPoint.x)*(firstPoint.x-secondPoint.x)
			+(firstPoint.y-secondPoint.y)*(firstPoint.y-secondPoint.y));
	return 0;
}

int CEllipse::ComputRadiusY(CPoint firstPoint,CPoint secondPoint)
{
  m_Yr=sqrt((firstPoint.x-secondPoint.x)*(firstPoint.x-secondPoint.x)
			+(firstPoint.y-secondPoint.y)*(firstPoint.y-secondPoint.y));
  return 0;
}

void CEllipse::ShowMovingLine(CDC*pDC,CPoint firstPoint, CPoint secondPoint)
{
	CPen m_pen;
	m_pen.CreatePen(PS_SOLID,1,RGB(0,0,255));
	CPen *OldPen=pDC->SelectObject(&m_pen);
	pDC->MoveTo(firstPoint);
	pDC->LineTo(secondPoint);
	pDC->SelectObject(OldPen);
}

void CEllipse::ComputeOffSet()
{
	POINT centerPoint=GetDstPoint(m_CenterPoint);
	double vh[3][3]={m_Cos,-m_Sin,centerPoint.x*(1-m_Cos)+centerPoint.y*m_Sin,
		           m_Sin, m_Cos,centerPoint.y*(1-m_Cos)-centerPoint.x*m_Sin,
				   0,0,1
					};       
  
      double xy1[3]={centerPoint.x,centerPoint.y,1};
	  m_OffSetX=vh[0][0]*xy1[0]+vh[0][1]*xy1[1]+vh[0][2]*xy1[2];
	  m_OffSetY=vh[1][0]*xy1[0]+vh[1][1]*xy1[1]+vh[1][2]*xy1[2];
}

void CEllipse::ShowSelectPoint(CDC *pDC)
{
	POINT centerPoint=GetDstPoint(m_CenterPoint);
	double vh[3][3]={m_Cos,-m_Sin,centerPoint.x*(1-m_Cos)+centerPoint.y*m_Sin,
					 m_Sin, m_Cos,centerPoint.y*(1-m_Cos)-centerPoint.x*m_Sin,
					 0,0,1
					};       

	double xy1[3]={-m_Xr+centerPoint.x,centerPoint.y,1};
	m_MarginPoint[0].x=vh[0][0]*xy1[0]+vh[0][1]*xy1[1]+vh[0][2]*xy1[2];
	m_MarginPoint[0].y=vh[1][0]*xy1[0]+vh[1][1]*xy1[1]+vh[1][2]*xy1[2];

	double xy2[3]={centerPoint.x,centerPoint.y-m_Yr,1};
	m_MarginPoint[1].x=vh[0][0]*xy2[0]+vh[0][1]*xy2[1]+vh[0][2]*xy2[2];
	m_MarginPoint[1].y=vh[1][0]*xy2[0]+vh[1][1]*xy2[1]+vh[1][2]*xy2[2]; 
	
	double xy3[3]={centerPoint.x+m_Xr,centerPoint.y,1};
	m_MarginPoint[2].x=vh[0][0]*xy3[0]+vh[0][1]*xy3[1]+vh[0][2]*xy3[2];
	m_MarginPoint[2].y=vh[1][0]*xy3[0]+vh[1][1]*xy3[1]+vh[1][2]*xy3[2];
	
	double xy4[3]={centerPoint.x,m_Yr+centerPoint.y,1};
	m_MarginPoint[3].x=vh[0][0]*xy4[0]+vh[0][1]*xy4[1]+vh[0][2]*xy4[2];
	m_MarginPoint[3].y=vh[1][0]*xy4[0]+vh[1][1]*xy4[1]+vh[1][2]*xy4[2];
	
	double xy5[3]={centerPoint.x,centerPoint.y,1};
	m_MarginPoint[4].x=vh[0][0]*xy5[0]+vh[0][1]*xy5[1]+vh[0][2]*xy5[2];
	m_MarginPoint[4].y=vh[1][0]*xy5[0]+vh[1][1]*xy5[1]+vh[1][2]*xy5[2];
	
	CBrush brush;
	brush.CreateSolidBrush(RGB(0,0,255));
	CPen m_pen;
	m_pen.CreatePen(PS_SOLID,1,RGB(0,0,255));
	CPen *OldPen=pDC->SelectObject(&m_pen);
	int oldBkMode=pDC->SetBkMode(OPAQUE); 
	CBrush *OldBrush=pDC->SelectObject(&brush);
	int oldDrawingMode=pDC->SetROP2(R2_NOTXORPEN);
	CRect rect;
	for(int i=0;i<4;i++)
	{
		rect.SetRect(m_MarginPoint[i],m_MarginPoint[i]);
		rect.InflateRect(3,3);
		pDC->Rectangle(rect);
	}
	pDC->SelectObject(OldBrush);
	brush.DeleteObject();
	brush.CreateSolidBrush(RGB(255,0,0));
	pDC->SelectObject(&brush);
  rect.SetRect(m_MarginPoint[4],m_MarginPoint[4]);
  rect.InflateRect(3,3);
  pDC->Rectangle(rect);
	pDC->SelectObject(OldPen);
	pDC->SetBkMode(oldBkMode); 
	pDC->SelectObject(OldBrush);
	pDC->SetROP2(oldDrawingMode);
}

CRect CEllipse::GetBoundingRect()
{
	POINT centerPoint=GetDstPoint(m_CenterPoint);
	double vh[3][3]={m_Cos,-m_Sin,centerPoint.x*(1-m_Cos)+centerPoint.y*m_Sin,
					 m_Sin, m_Cos,centerPoint.y*(1-m_Cos)-centerPoint.x*m_Sin,
					 0,0,1
					};
	double xy1[3]={-m_Xr+centerPoint.x,centerPoint.y-m_Yr,1};
	int left=vh[0][0]*xy1[0]+vh[0][1]*xy1[1]+vh[0][2]*xy1[2];
	int top=vh[1][0]*xy1[0]+vh[1][1]*xy1[1]+vh[1][2]*xy1[2];
	
	double xy2[3]={centerPoint.x+m_Xr,centerPoint.y-m_Yr,1};
	int left2=vh[0][0]*xy2[0]+vh[0][1]*xy2[1]+vh[0][2]*xy2[2];
	int top2=vh[1][0]*xy2[0]+vh[1][1]*xy2[1]+vh[1][2]*xy2[2]; 
	
	double xy3[3]={centerPoint.x+m_Xr,centerPoint.y+m_Yr,1};
	int right=vh[0][0]*xy3[0]+vh[0][1]*xy3[1]+vh[0][2]*xy3[2];
	int bottom=vh[1][0]*xy3[0]+vh[1][1]*xy3[1]+vh[1][2]*xy3[2];
	
	double xy4[3]={centerPoint.x-m_Xr,m_Yr+centerPoint.y,1};
	int right2=vh[0][0]*xy4[0]+vh[0][1]*xy4[1]+vh[0][2]*xy4[2];
	int bottom2=vh[1][0]*xy4[0]+vh[1][1]*xy4[1]+vh[1][2]*xy4[2];
	
	int miny=min(min(top,top2),min(bottom,bottom2));
	int minx=min(min(right,right2),min(left,left2));
	int maxy=max(max(top,top2),max(bottom,bottom2));
	int maxx=max(max(right,right2),max(left,left2));

	m_BoundingRect.left=minx;
	m_BoundingRect.top=miny;
	m_BoundingRect.right=maxx;
	m_BoundingRect.bottom=maxy;

	return this->m_BoundingRect;
}

void CEllipse::DrawActive(CDC *pDC,CPoint point)
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

	POINT centerPoint=GetDstPoint(m_CenterPoint);
	ellipseMidpoint(pDC,centerPoint.x,centerPoint.y,this->m_Xr,this->m_Yr);
	pDC->SelectObject(brush);
	pDC->SelectObject(pen);
	pDC->SetBkMode(OldBkMode);
	pDC->SetBkColor(OldColor);
}

void CEllipse::OnLButtonDown(CDC *pDC, PGRAPHCAPTION pCaption, CPoint point)
{
	if(m_HaveFindFirst)
	{
    pCaption->L_iPointCount=IsOnMarginPoint(point);
		if(pCaption->L_iPointCount!=0)
		{
			pCaption->m_bHaveFindSecond=true;
			pCaption->m_CurrentDrawTool=Graph_Ellipse;
			pCaption->m_CurrentDrawStatus=Change_Status;
			
			switch(pCaption->L_iPointCount)
			{
			case 1:
				pCaption->m_FirstPoint=m_SecondPoint;
				pCaption->m_SecondPoint=m_FirstPoint;
				break;
				
			case 2:
				pCaption->m_FirstPoint=m_FirstPoint;
				pCaption->m_SecondPoint=m_SecondPoint;
				break;
				
			case 3:
				pCaption->m_FirstPoint=m_FirstPoint;
				pCaption->m_SecondPoint=m_SecondPoint;
				break;
				
			case 4:
				pCaption->m_FirstPoint=m_SecondPoint;
				pCaption->m_SecondPoint=m_FirstPoint;
				break;
				
			case 5:
				pCaption->m_FirstPoint=m_FirstPoint;
				pCaption->m_SecondPoint=m_SecondPoint;
				pCaption->m_CurrentDrawTool=Graph_Ellipse;
				pCaption->m_CurrentDrawStatus=Drag_Status;
				break;
			}
		}//******if(L_iPointCount!=0)
	}//**********if(L_pEllipse->m_HaveFindFirst)
  
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
	
	if(pCaption->m_bHaveFindSecond)
	{   
		pCaption->m_pCurrentUnit=this;
		m_DrawingMode=pDC->GetROP2();
		DrawStatic(pDC);
		DrawOldReferencePoint(pDC,GetDstPoint(m_FoundPoint));
		ShowSelectPoint(pDC);
	}
}

void CEllipse::OnMouseMove(CDC *pDC, PGRAPHCAPTION pCaption, CPoint point)
{
	POINT centerPoint;
	if(pCaption->m_CurrentDrawStatus==Drag_Status)
	{
		if(!m_IsCirCu)
		{
			m_CenterPoint.x += pCaption->m_SecondPoint.x-pCaption->m_FirstPoint.x;
			m_CenterPoint.y += pCaption->m_SecondPoint.y-pCaption->m_FirstPoint.y;
		}
	   if(m_IsCirCu)
	   {
		   centerPoint=GetDstPoint(m_CenterPoint);
		   ComputSloap(centerPoint,point);
	   }
	}
	else
	{
	   centerPoint=GetDstPoint(m_CenterPoint);
		//pDC->SetROP2(R2_NOTXORPEN);
		if(pCaption->m_LBDownTimes==2&&pCaption->m_CurrentDrawStatus!=Change_Status)
	  {
			ShowMovingLine(pDC,centerPoint,point);
			ellipseMidpoint(pDC,centerPoint.x,centerPoint.y,m_Xr,m_Yr);
			ComputRadiusX(centerPoint,point);
			//m_DrawingMode=pDC->GetROP2();
			pCaption->m_SecondPoint=GetSrcPoint(point);
			ellipseMidpoint(pDC,centerPoint.x,centerPoint.y,m_Xr,m_Yr);
			ShowMovingLine(pDC,centerPoint,point);
		}
		if(pCaption->m_CurrentDrawStatus==Change_Status)
		{
			ellipseMidpoint(pDC,centerPoint.x,centerPoint.y,m_Xr,m_Yr);
			ShowMovingLine(pDC,centerPoint,GetDstPoint(pCaption->m_SecondPoint));

			pCaption->m_SecondPoint=GetSrcPoint(point);
			if(pCaption->L_iPointCount==1||pCaption->L_iPointCount==3)
				ComputRadiusX(centerPoint,point);
			if(pCaption->L_iPointCount==2||pCaption->L_iPointCount==4)
				ComputRadiusY(centerPoint,point);
			
			//m_DrawingMode=pDC->GetROP2();
			ShowMovingLine(pDC,centerPoint,point);
			
			ellipseMidpoint(pDC,centerPoint.x,centerPoint.y,m_Xr,m_Yr);
	  }
		
		if(pCaption->m_LBDownTimes==1&&pCaption->m_CurrentDrawStatus!=Change_Status)
		{
			ShowMovingLine(pDC,GetDstPoint(pCaption->m_FirstPoint),
							   GetDstPoint(pCaption->m_SecondPoint));
			pCaption->m_SecondPoint=GetSrcPoint(point);
			ShowMovingLine(pDC,GetDstPoint(pCaption->m_FirstPoint),point);
		}
	}
}

void CEllipse::OnMenuitemCirCu(CDC *pDC, PGRAPHCAPTION pCaption)
{
  m_IsCirCu=!m_IsCirCu;
}

CEllipse::CEllipse()
{
	CUnit::Initial();
	m_IsCirCu=false;
}

void CEllipse::OnContextMenu(CWnd *pWnd, CPoint point)
{
//CEastDrawView*pView=(CEastDrawView*)pWnd;
//if(pView->m_CurrentDrawStatus==Drag_Status)
//{
//CMenu menuTextEdit;
//menuTextEdit.LoadMenu(IDR_MENU_Canvas_Edit);
//menuTextEdit.CheckMenuItem(ID_CirCu,m_IsCirCu?MF_CHECKED:MF_UNCHECKED);
//menuTextEdit.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,pWnd);
//}
}

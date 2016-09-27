// CanvasHistogram.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "CanvasHistogram.h"
#include "StaticCanvas.h"
#include "NewMenu.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCanvasHistogram

CCanvasHistogram::CCanvasHistogram()
{
	for (int i=0; i<4; i++)
		m_bShowFlags[i] = FALSE;
	m_bLogState = FALSE;
}

CCanvasHistogram::~CCanvasHistogram()
{
}


BEGIN_MESSAGE_MAP(CCanvasHistogram, CStatic)
	//{{AFX_MSG_MAP(CCanvasHistogram)
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_WM_PAINT()
	ON_COMMAND(ID_HISTOGRAM_RED, OnHistogramRed)
	ON_COMMAND(ID_HISTOGRAM_GREEN, OnHistogramGreen)
	ON_COMMAND(ID_HISTOGRAM_BLUE, OnHistogramBlue)
	ON_COMMAND(ID_HISTOGRAM_GRAY, OnHistogramGray)
	ON_COMMAND(ID_HISTOGRAM_PROPERTY, OnHistogramProperty)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCanvasHistogram message handlers

int CCanvasHistogram::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void CCanvasHistogram::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if (point.x == -1 && point.y == -1){
		//keystroke invocation
		CRect rect;
		GetClientRect(rect);
		ClientToScreen(rect);

		point = rect.TopLeft();
		point.Offset(5, 5);
	}

	CNewMenu menu;
	CMenu* pPopup;
	VERIFY(menu.LoadMenu(IDR_POPUP_CANVAS_HISTOGRAM));//IDR_POPUP_STATIC_CANVAS));	
	menu.SetMenuText(ID_HISTOGRAM_RED, _T("红色直方图"), MF_BYCOMMAND);
	menu.SetMenuText(ID_HISTOGRAM_GREEN, _T("绿色直方图"), MF_BYCOMMAND);
	menu.SetMenuText(ID_HISTOGRAM_BLUE, _T("蓝色直方图"), MF_BYCOMMAND);
	menu.SetMenuText(ID_HISTOGRAM_GRAY, _T("灰度平均直方图"), MF_BYCOMMAND);
	menu.SetMenuText(ID_HISTOGRAM_PROPERTY, _T("显示属性设置Log"), MF_BYCOMMAND);

	menu.CheckMenuItem(ID_HISTOGRAM_RED, m_bShowFlags[0] ? MF_CHECKED : MF_UNCHECKED);
	menu.CheckMenuItem(ID_HISTOGRAM_GREEN, m_bShowFlags[1] ? MF_CHECKED : MF_UNCHECKED);
	menu.CheckMenuItem(ID_HISTOGRAM_BLUE, m_bShowFlags[2] ? MF_CHECKED : MF_UNCHECKED);
	menu.CheckMenuItem(ID_HISTOGRAM_GRAY, m_bShowFlags[3] ? MF_CHECKED : MF_UNCHECKED);
	menu.CheckMenuItem(ID_HISTOGRAM_PROPERTY, m_bLogState ? MF_CHECKED : MF_UNCHECKED);

	pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
		pWndPopupOwner);
}

void CCanvasHistogram::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect r;
	GetClientRect(&r);

	CBrush b(::GetSysColor(COLOR_BTNHIGHLIGHT));
	CBrush* pOldBrush = dc.SelectObject(&b);
	dc.Rectangle(&r);
	dc.SelectObject(pOldBrush);

	r.DeflateRect(10,10);
	//r.Width()
	int nRatioX, nRatioY;
	nRatioX = r.Width() / 10;
	nRatioY = r.Height() / 10;

	r.SetRect(r.left,r.top, r.left + 10 * nRatioX,r.top + 10 * nRatioY);

	CPen gridPen(PS_DOT,1,::GetSysColor(COLOR_BTNFACE));
	CPen* pOldPen = dc.SelectObject(&gridPen);
	for (int gx=r.left;gx<=r.left + 10 * nRatioX;gx+=nRatioX){
		dc.MoveTo(gx,r.bottom);
		dc.LineTo(gx,r.top);// + 10 * nRatioY);
	}
	for (int gy=r.top;gy<=r.top + 10 * nRatioY;gy+=nRatioY){
		dc.MoveTo(r.left,gy);
		dc.LineTo(r.left + 10 * nRatioX,gy);
	}
	dc.SelectObject(pOldPen);

	// 标注以灰度值为横坐标的刻度
	//{追加2005.08.21
	/*
	CString str;
	CFont font;
	font.CreateFont(nRatioX/30,nRatioY/30,0,0,5,0,0,0,0,1,2,1,34,"MS Sans Serif");
	dc.SetTextAlign(TA_CENTER);
	dc.SetBkMode(TRANSPARENT);
	CFont* pOldFont = dc.SelectObject(&font);
	for(gx=r.left;gx<=r.left + 10 * nRatioX;gx+=2*nRatioX)
	{
		str.Format("%2.0f", (gx-r.left)/nRatioX*25.5);
		dc.TextOut(gx,r.bottom-3,str);
	}
	dc.SelectObject(pOldFont);
	*/
	//TRACE("nRatioX=%d,nRatioY=%d\n",nRatioX,nRatioY);
	//}追加2005.08.21

	static COLORREF clr[4] = {0x000000FF, 0x0000FF00, 0x00FF0000, 0x008F8F8F};
	for(int idx = 0; idx < 4; ++idx)
	{
		draw_idx(&dc, idx, clr[idx]);
	}
}

void CCanvasHistogram::Redraw()
{
	UpdateWindow();
	InvalidateRect(NULL);
}

void CCanvasHistogram::draw_idx(CDC* pDC, int nIdx, COLORREF clr)
{
	// 如果设置为不显示,则返回
	if (!m_bShowFlags[nIdx])
		return;

	CRect r;
	GetClientRect(&r);
	r.DeflateRect(10,10);	
	
	CPen pNewPen(PS_SOLID, 1, clr);
	CPen* pOldPen = pDC->SelectObject(&pNewPen);

	float xratio,yratio;
	xratio = r.Width()/256.0;

	int ybase = r.bottom;
	if(m_bLogState)
	{
		float yratio = r.Height()/(float)log10((float)(m_lMax+1));
		pDC->MoveTo(r.left, (int)(ybase-log10((float)(1+m_lHisto[nIdx][0]))*yratio));
		for (int x=1; x<256; x++)
			pDC->LineTo(r.left+xratio*x, (int)(ybase-log10((float)(1+m_lHisto[nIdx][x]))*yratio));
	}
	else
	{
		float yratio = r.Height()/(float)m_lMax;
		pDC->MoveTo(r.left, (int)(ybase-m_lHisto[nIdx][0]*yratio));
		for (int x=1; x<256; x++)
			pDC->LineTo(r.left+xratio*x, (int)(ybase-m_lHisto[nIdx][x]*yratio));
	}

	pDC->SelectObject(pOldPen);
}

void CCanvasHistogram::OnHistogramRed() 
{
	m_bShowFlags[0] = !m_bShowFlags[0];
	Redraw();
}

void CCanvasHistogram::OnHistogramGreen() 
{
	m_bShowFlags[1] = !m_bShowFlags[1];
	Redraw();
}

void CCanvasHistogram::OnHistogramBlue() 
{
	m_bShowFlags[2] = !m_bShowFlags[2];
	Redraw();
}

void CCanvasHistogram::OnHistogramGray() 
{
	m_bShowFlags[3] = !m_bShowFlags[3];
	Redraw();
}

void CCanvasHistogram::OnHistogramProperty() 
{
	m_bLogState = !m_bLogState;
	Redraw();
}

void CCanvasHistogram::SetData(long* red, long* green, long* blue, long* gray)
{
	int i;
	long lSum = 0;
	m_lMax = 0;

	// 红色分量
	for (i=0; i<256; i++)
	{
		m_lHisto[0][i] = red[i];
		if (m_lMax<red[i])
			m_lMax = red[i];
	}

	// 绿色分量
	for (i=0; i<256; i++)
	{
		m_lHisto[1][i] = green[i];
		if (m_lMax<green[i])
			m_lMax = green[i];
	}

	// 蓝色分量
	for (i=0; i<256; i++)
	{
		m_lHisto[2][i] = blue[i];
		if (m_lMax<blue[i])
			m_lMax = blue[i];
	}

	// 绿色分量
	for (i=0; i<256; i++)
	{
		m_lHisto[3][i] = gray[i];
		if (m_lMax<gray[i])
			m_lMax = gray[i];
	}
}

void CCanvasHistogram::get_max_min()
{
	m_dbMin = 15000000;	// 大于图像能容纳的最大像素数
	m_dbMax = 0;
	for (int i=0; i<4; i++)
	{
		for (int j=0; j<256; j++)
		{
			m_dbMax = (m_dbMax>m_lHisto[i][j])?m_dbMax:m_lHisto[i][j];
			m_dbMin = (m_dbMin<m_lHisto[i][j])?m_dbMin:m_lHisto[i][j];
		}
	}
}
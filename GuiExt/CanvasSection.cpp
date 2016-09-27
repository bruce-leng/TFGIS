// CanvasSection.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "CanvasSection.h"
#include <float.h>
#include "TilGraph.h"
#include "NewMenu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCanvasSection

CCanvasSection::CCanvasSection()
{
	m_pGraphSection = NULL;
	m_pProfile = NULL;
	m_lDK = -1;
	m_lStartDK = m_lEndDK = -1;
	m_nDrawID = 0;
}

CCanvasSection::~CCanvasSection()
{
	if (m_pGraphSection)
		delete m_pGraphSection;
	if (m_pProfile)
		delete m_pProfile;
}


BEGIN_MESSAGE_MAP(CCanvasSection, CStatic)
	//{{AFX_MSG_MAP(CCanvasSection)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_SECTION_NORMAL, OnSectionNormal)
	ON_UPDATE_COMMAND_UI(ID_SECTION_NORMAL, OnUpdateSectionNormal)
	ON_COMMAND(ID_SECTION_PROFILE, OnSectionProfile)
	ON_UPDATE_COMMAND_UI(ID_SECTION_PROFILE, OnUpdateSectionProfile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCanvasSection message handlers

int CCanvasSection::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CCanvasSection::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect r;
	GetClientRect(&r);

	CBrush b(::GetSysColor(COLOR_BTNHIGHLIGHT));
	CBrush* pOldBrush = dc.SelectObject(&b);
	dc.Rectangle(&r);
	dc.SelectObject(pOldBrush);
	
	// Do not call CStatic::OnPaint() for painting messages
	if (m_nDrawID == 1)
		DrawProfile(&dc);
	else if (m_nDrawID == 2)
		DrawNormalSection(&dc);
}

void CCanvasSection::Redraw(int nDrawID)
{
	m_nDrawID = nDrawID;
	UpdateWindow();
	InvalidateRect(NULL);
}

// 绘制横剖面
void CCanvasSection::DrawNormalSection(CDC* pDC)
{
	if (m_pProfile==0)
		return;
	
	
	//////////////////////////////////////////////////////////////////////////
	// 绘出纵剖面底部里程
	CRect rect;
	GetClientRect(&rect);
	rect.DeflateRect(10, 10, 10, 20);
	
	CFont font;
	font.CreateFont(rect.Width()/300,rect.Height()/300,0,0,5,0,0,0,0,1,2,1,34,"MS Sans Serif");
	pDC->SetBkMode(TRANSPARENT);
	CFont* pOldFont = pDC->SelectObject(&font);
	
	CString str;
	str.Format("ZK%d+%d",m_lStartDK/1000,m_lStartDK%1000);
	pDC->SetTextAlign(TA_LEFT);
	pDC->TextOut(rect.left, rect.bottom, str);
	str.Format("ZK%d+%d",m_lEndDK/1000,m_lEndDK%1000);
	pDC->SetTextAlign(TA_RIGHT);
	pDC->TextOut(rect.right, rect.bottom, str);
	
	draw_graph(pDC, rect, *m_pProfile);
}

// 绘制纵剖面
void CCanvasSection::DrawProfile(CDC* pDC)
{
	if (!m_pGraphSection || m_lDK<0)
		return;
	
	//////////////////////////////////////////////////////////////////////////
	// 绘出剖面底部里程
	CRect rect;
	GetClientRect(&rect);
	CFont font;
	font.CreateFont(rect.Width()/300,rect.Height()/300,0,0,5,0,0,0,0,1,2,1,34,"MS Sans Serif");
	pDC->SetTextAlign(TA_CENTER);
	pDC->SetBkMode(TRANSPARENT);
	CFont* pOldFont = pDC->SelectObject(&font);

	CString str;
	str.Format("ZK%d+%d",m_lDK/1000,m_lDK%1000);
	pDC->TextOut(rect.Width()/2, rect.bottom-20, str);

	//////////////////////////////////////////////////////////////////////////
	// 画出横剖面图形
	rect.DeflateRect(10, 10, 10, 20);
	draw_graph(pDC, rect, *m_pGraphSection);

}

// 纵断面
void CCanvasSection::SetNormalSection(long lDK, CTilGraph& graphMgr)
{
	m_lDK = lDK;

	if (m_pGraphSection)
		delete m_pGraphSection;
	m_pGraphSection = new CTilGraph(graphMgr);
}

// 设置横断面
void CCanvasSection::SetProfile(long lStartDK, long lEndDK, CTilGraph& graphMgr)
{
	m_lStartDK = lStartDK;
	m_lEndDK = lEndDK;

	if (m_pProfile)
		delete m_pProfile;
	m_pProfile = new CTilGraph(graphMgr);
}

// 在指定矩形内画出图形
void CCanvasSection::draw_graph(CDC* pDC, CRect& rect, CTilGraph& graphMgr)
{
	int i, j, k;

	//////////////////////////////////////////////////////////////////////////
	// 绘出剖面图形
	// 将图形的左上角设为原点,
	// 求图形的外接矩形
	double dbLeft, dbTop, dbRight, dbBottom;
	dbLeft = dbTop = FLT_MAX;
	dbRight = dbBottom = -FLT_MAX;
	for (i=0; i<graphMgr.Size(); i++)
	{
		CGraph* pGraph = graphMgr.GetGraphByIdx(i);
		for (j=0; j<pGraph->Size(); j++)
		{
			// 设图形层中全部为多边形边界线
			CUnit* pUnit = pGraph->GetUnitByIdx(j);
			for (k=0; k<pUnit->GetPointNum(); k++)
			{
				POINT2DF pt = pUnit->m_pPointList->GetAt(k);
				if (dbLeft>pt.x)	dbLeft = pt.x;
				if (dbRight<pt.x)	dbRight = pt.x;
				if (dbTop>pt.y)		dbTop = pt.y;
				if (dbBottom<pt.y)	dbBottom = pt.y;
			}
		}
	}

	// 求缩放率
	double dbRatioX, dbRatioY;
	dbRatioX = rect.Width()*1.0/(dbRight-dbLeft);
	dbRatioY = rect.Height()*1.0/(dbBottom-dbTop);

	// 在窗口中绘出曲线图
	for (i=0; i<graphMgr.Size(); i++)
	{
		CGraph* pGraph = graphMgr.GetGraphByIdx(i);
		for (j=0; j<pGraph->Size(); j++)
		{
			// 设图形层中全部为多段线边界线
			CUnit* pUnit = pGraph->GetUnitByIdx(j);
			int nNum = pUnit->GetPointNum();
			if (nNum == 0)
				continue;

			POINT2DF pt1, pt2;
			pt1 = pUnit->m_pPointList->GetAt(0);
			pt1.x -= dbLeft;
			pt1.y -= dbTop;
			pt1.x *= dbRatioX;
			pt1.y *= dbRatioY;
			pDC->MoveTo((long)pt1.x+10, (long)pt1.y+10);
			for (k=1; k<nNum; k++)
			{
				pt2 = pUnit->m_pPointList->GetAt(k);
				pt2.x -= dbLeft;
				pt2.y -= dbTop;
				pt2.x *= dbRatioX;
				pt2.y *= dbRatioY;
				pDC->LineTo((long)pt2.x+10, (long)pt2.y+10);
			}
			if (i==GRAPH_LAYER_TUNARC)
				pDC->LineTo((long)pt1.x+10, (long)pt1.y+10);
		}
	}

}

void CCanvasSection::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// CG: This block was added by the Pop-up Menu component
	if (point.x == -1 && point.y == -1)
	{
		//keystroke invocation
		CRect rect;
		GetClientRect(rect);
		ClientToScreen(rect);

		point = rect.TopLeft();
		point.Offset(5, 5);
	}

	CNewMenu menu;
	VERIFY(menu.LoadMenu(IDR_POPUP_CANVAS_SECTION));
	menu.SetMenuText(ID_SECTION_NORMAL, _T("横剖面"), MF_BYCOMMAND);
	menu.SetMenuText(ID_SECTION_PROFILE, _T("纵剖面"), MF_BYCOMMAND);
	menu.CheckMenuItem(ID_SECTION_NORMAL, m_nDrawID==1 ? MF_CHECKED : MF_UNCHECKED);
	menu.CheckMenuItem(ID_SECTION_PROFILE, m_nDrawID==2 ? MF_CHECKED : MF_UNCHECKED);


	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
		pWndPopupOwner);
}

void CCanvasSection::OnSectionProfile() 
{
	// TODO: Add your command handler code here

	Redraw(2);
}

void CCanvasSection::OnUpdateSectionProfile(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CCanvasSection::OnSectionNormal() 
{
	// TODO: Add your command handler code here
	Redraw(1);
}

void CCanvasSection::OnUpdateSectionNormal(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}


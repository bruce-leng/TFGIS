// GuiDataDraw.cpp: implementation of the CGuiDataDraw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TilSys.h"
#include "GuiDataDraw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CGuiDataDraw, CGuiControlBar)

CGuiDataDraw::CGuiDataDraw()
{

}

CGuiDataDraw::~CGuiDataDraw()
{

}

BEGIN_MESSAGE_MAP(CGuiDataDraw, CGuiControlBar)
	ON_WM_CREATE()
END_MESSAGE_MAP()

int CGuiDataDraw::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CGuiControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_TabSolExplorer.Create(WS_VISIBLE | WS_CHILD, CRect(0,0,0,0), this, 0x9999))
		return -1;

	const DWORD dwStyle = WS_VISIBLE | WS_CHILD | SS_NOTIFY;
	if (!m_Histogram.Create("Histogram", dwStyle, CRect(0,0,0,0), &m_TabSolExplorer, 1))
		return -1;
	if (!m_Section.Create("Section", dwStyle, CRect(0,0,0,0), &m_TabSolExplorer, 2))
		return -1;

	m_TabSolExplorer.SetImageList(IDB_DBPROJECTS, 16, 5, RGB(255, 0, 0));
	m_TabSolExplorer.Addtab(&m_Histogram, "灰度直方图", 1);
	m_TabSolExplorer.Addtab(&m_Section, "地质剖面图", 2);

	/*
	if (!m_Others.Create("Others", dwStyle, CRect(0,0,0,0), &m_TabSolExplorer, 3))
		return -1;
	m_TabSolExplorer.Addtab(&m_Others, "其它", 3);
	*/

	return 0;
}

void CGuiDataDraw::InvalidateHistogram()
{
	m_Histogram.Redraw();
	m_Histogram.UpdateWindow();
	m_Histogram.InvalidateRect(NULL);
}

void CGuiDataDraw::InvalidateSection()
{
	m_Section.Redraw(0);
	m_Section.UpdateWindow();
	m_Section.InvalidateRect(NULL);
}
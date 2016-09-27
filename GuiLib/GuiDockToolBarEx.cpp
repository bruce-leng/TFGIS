#include "stdafx.h"
#include "guidocktoolbarex.h"
#include "GuiDrawLayer.h"

/////////////////////////////////////////////////////////////////////////////
// CGuiDockToolBarEx

CGuiDockToolBarEx::CGuiDockToolBarEx()
{
	m_clrface=GuiDrawLayer::GetRGBColorTabs();
	m_Size=24;
}

CGuiDockToolBarEx::~CGuiDockToolBarEx()
{

}


BEGIN_MESSAGE_MAP(CGuiDockToolBarEx, CControlBar)
	//{{AFX_MSG_MAP(CGuiDockToolBarEx)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_NCPAINT()
	ON_WM_NCCALCSIZE()
	ON_WM_SYSCOLORCHANGE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGuiDockToolBarEx message handlers

BOOL CGuiDockToolBarEx::Create(CWnd* pParentWnd,DWORD dwAling) 
{
	// TODO: Add your specialized code here and/or call the base class
/*		ASSERT (  (dwAling & CBRS_ALIGN_TOP) &&
				  (dwAling & CBRS_ALIGN_RIGHT) &&
				  (dwAling & CBRS_ALIGN_BOTTOM) &&
				  (dwAling & CBRS_ALIGN_LEFT) );
*/	ASSERT ( dwAling & CBRS_ALIGN_ANY);
	SetBarStyle (dwAling);
	DWORD dwStyle = CCS_NOPARENTALIGN|CCS_NOMOVEY|CCS_NODIVIDER|CCS_NORESIZE;
	if (pParentWnd->GetStyle() & WS_THICKFRAME)
		dwStyle |= SBARS_SIZEGRIP;
	dwStyle |= WS_CHILD|WS_VISIBLE;
	return CControlBar::Create(0,0,dwStyle,
						CRect(0,0,0,0),pParentWnd,0x9999);
}

int CGuiDockToolBarEx::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

BOOL CGuiDockToolBarEx::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	CBrush cbr;
	cbr.CreateSolidBrush(m_clrface);
	CRect m_rect;
	GetClientRect(m_rect);
	pDC->FillRect(m_rect,&cbr);
	return TRUE;

}


void CGuiDockToolBarEx::OnSysColorChange()
{
	m_clrface=GuiDrawLayer::GetRGBColorTabs();
	CControlBar::OnSysColorChange();
}

void CGuiDockToolBarEx::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CControlBar::OnPaint() for painting messages
}


void CGuiDockToolBarEx::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	// TODO: Add your message handler code here and/or call default
	CControlBar::OnNcCalcSize(bCalcValidRects, lpncsp);
}

void CGuiDockToolBarEx::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
}

CSize CGuiDockToolBarEx::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	if (bHorz)
		return CSize(32767,m_Size);
	else
		return CSize(m_Size,32767);
}



// TilSysView.cpp : implementation of the CTilSysView class
//

#include "stdafx.h"
#include "TilSysView.h"
#include "TilSys.h"
#include "TilCanvas.h"
#include "DlgRulerSet.h"
#include "MathComput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTilSysView

IMPLEMENT_DYNCREATE(CTilSysView, CSysView)

BEGIN_MESSAGE_MAP(CTilSysView, CSysView)
	//{{AFX_MSG_MAP(CTilSysView)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_VIEW_RULER_SET, OnViewRulerSet)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RULER_SET, OnUpdateViewRulerSet)
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	// Standard printing commands
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTilSysView construction/destruction

CTilSysView::CTilSysView() : m_nLine(0)
{
}

CTilSysView::~CTilSysView()
{
}

void CTilSysView::OnDraw(CDC* pDC) 
{
	// TODO: Add your specialized code here and/or call the base class
	CSysView::OnDraw(pDC);
}

void CTilSysView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_nLine == 0)
		CSysView::OnLButtonDown(nFlags, point);
}

void CTilSysView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_nLine == 0)
		CSysView::OnLButtonUp(nFlags, point);
	else
	{
		// 画比例尺
		CDC* pDC = GetDC();
		CPen pen(PS_SOLID, 1, RGB(255,0,0));
		CPen* pOldPen = pDC->SelectObject(&pen);

		OnPrepareDC(pDC);
		POINT pt = point;
		pDC->DPtoLP(&pt);

		if (m_nLine == 1)
		{
			pDC->MoveTo(pt);
			pDC->LineTo(pt);

			m_ptRuler[0] = pt;
			m_ptRuler[1] = pt;

			m_nLine = 2;
		}
		else if (m_nLine == 2)
		{
			m_nLine = 0;

			// 弹出比例尺设置对话框
			DlgRulerSet dlg;

			// 设置映射模式，使每逻辑单元为0.1mm
//			int nMode = pDC->SetMapMode(MM_LOMETRIC);
			CPoint pt1(m_ptRuler[0]), pt2(m_ptRuler[1]);
			pDC->DPtoLP(&pt1);
			pDC->DPtoLP(&pt2);

			long double dbRatio;
			GetCanvasMgr()->GetCurCanvas()->GetZoomFactor(dbRatio, dbRatio);
			float fDist = get_distance(pt1, pt2);
			dlg.m_fPixelWidth = fDist/dbRatio;
			if (dlg.DoModal() == IDOK)
				m_fRuler = dlg.m_fRealRuler;

			Invalidate();
		}

		pDC->SelectObject(pOldPen);
	}
}

void CTilSysView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_nLine == 0)
		CSysView::OnMouseMove(nFlags, point);
	else
	{
		// 画比例尺
		CDC* pDC = GetDC();
		CPen pen(PS_SOLID, 1, RGB(255,0,0));
		CPen* pOldPen = pDC->SelectObject(&pen);

		pDC->SetROP2(R2_NOTXORPEN);
		OnPrepareDC(pDC);
		POINT pt = point;
		pDC->DPtoLP(&pt);

		if (m_nLine == 2)
		{
			pDC->MoveTo(m_ptRuler[0]);
			pDC->LineTo(m_ptRuler[1]);
			m_ptRuler[1] = pt;
			pDC->MoveTo(m_ptRuler[0]);
			pDC->LineTo(m_ptRuler[1]);
		}

		pDC->SelectObject(pOldPen);
	}
}

// 设置比例尺
void CTilSysView::OnViewRulerSet() 
{
	// TODO: Add your command handler code here
	m_nLine = 1;
}

void CTilSysView::OnUpdateViewRulerSet(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CTilCanvas* pCanvas = GetCanvasMgr()->GetCurCanvas();
	if (pCanvas == NULL)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		if (m_nLine == 0)
			pCmdUI->SetCheck(0);
		else
			pCmdUI->SetCheck(1);
	}
}

BOOL CTilSysView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	static HCURSOR hCursor;
	if (m_nLine == 0)
		return CSysView::OnSetCursor(pWnd, nHitTest, message);
	else
	{
		if (m_nLine == 1)
			// 设置十字光标
			hCursor = ::SetCursor(AfxGetApp()->LoadCursor(ID_Draw));
		else
			::SetCursor(hCursor);
	}

	return TRUE;
}

// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "TilSys.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_CLOSE()
	ON_WM_MDIACTIVATE()
	ON_COMMAND(ID_VIEW_RULER, OnViewRuler)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RULER, OnUpdateViewRuler)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
	
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers
void CChildFrame::ShowRulers(BOOL bShow)
{
    m_Rulers.ShowRulers(bShow);
}

void CChildFrame::UpdateRulersInfo(stRULER_INFO stRulerInfo)
{
	// 如果显示了标尺就更新标尺，否则不更新
	if (m_Rulers.IsShow())
		m_Rulers.UpdateRulersInfo(stRulerInfo);
}

void CChildFrame::ActivateFrame(int nCmdShow) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CMDIChildWnd::ActivateFrame(nCmdShow);
}

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (!m_Rulers.CreateRulers(this, pContext)) {
		TRACE("Error creation of rulers\n");
		AfxMessageBox("");
		return CMDIChildWnd::OnCreateClient(lpcs, pContext);
	}

	return TRUE;
}

void CChildFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CMDIChildWnd::OnClose();
	CTilSysApp* pApp = (CTilSysApp*)AfxGetApp();
	pApp->CloseDocument();
}

void CChildFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) 
{
	::SendMessage(AfxGetMainWnd()->m_hWnd, WM_UPDATE_GRAPH_TOOL, bActivate,0);

	CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
}

void CChildFrame::OnViewRuler() 
{
	// TODO: Add your command handler code here
	if (m_Rulers.IsShow())
		m_Rulers.ShowRulers(FALSE);
	else
		m_Rulers.ShowRulers(TRUE);
}

void CChildFrame::OnUpdateViewRuler(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_Rulers.IsShow());
}

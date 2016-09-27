// OpenGlFrm.cpp : implementation file
//

#include "stdafx.h"
#include "TilSys.h"
#include "OpenGlFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COpenGlFrm

IMPLEMENT_DYNCREATE(COpenGlFrm, CMDIChildWnd)

COpenGlFrm::COpenGlFrm()
{
}

COpenGlFrm::~COpenGlFrm()
{
}


BEGIN_MESSAGE_MAP(COpenGlFrm, CMDIChildWnd)
	//{{AFX_MSG_MAP(COpenGlFrm)
	ON_WM_CLOSE()
	ON_WM_MDIACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpenGlFrm message handlers

BOOL COpenGlFrm::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style &= ~(LONG)FWS_ADDTOTITLE;	

	return CMDIChildWnd::PreCreateWindow(cs);
}

void COpenGlFrm::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CMDIChildWnd::OnClose();
  CTilSysApp* pApp = (CTilSysApp*)AfxGetApp();
  pApp->CloseDocument();
}

void COpenGlFrm::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) 
{
	::SendMessage(AfxGetMainWnd()->m_hWnd, WM_UPDATE_OPENGL_TOOL, bActivate, 0);

	CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
}

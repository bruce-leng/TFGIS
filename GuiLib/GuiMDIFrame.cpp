/****************************************************************************
 *																			*		 
 *								GuiToolKit  								*	
 *							 (MFC extension)								*			 
 * Created by Francisco Campos G. www.beyondata.com fcampos@beyondata.com	*
 *--------------------------------------------------------------------------*		   
 *																			*
 * This program is free software;so you are free to use it any of your		*
 * applications (Freeware, Shareware, Commercial),but leave this header		*
 * intact.																	*
 *																			*
 * These files are provided "as is" without warranty of any kind.			*
 *																			*
 *			       GuiToolKit is forever FREE CODE !!!!!					*
 *																			*
 *--------------------------------------------------------------------------*
 * Created by: Francisco Campos G.											*
 * Bug Fixes and improvements : (Add your name)								*
 * -Francisco Campos														*				
 * -Serge Koroleuve															*
 *																			*	
 ****************************************************************************/



#include "stdafx.h"
#include "GuiMDIFrame.h"
#include "GuiMiniFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGuiMDIFrame

IMPLEMENT_DYNCREATE(CGuiMDIFrame, CMDIFrameWnd)

CGuiMDIFrame::CGuiMDIFrame()
{
	
}

CGuiMDIFrame::~CGuiMDIFrame()
{

	
}


BEGIN_MESSAGE_MAP(CGuiMDIFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CGuiMDIFrame)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//**************************************************************************

BOOL CGuiMDIFrame::PreTranslateMessage(MSG* pMsg) 
{
	if (m_wndMenuBar.TranslateFrameMessage(pMsg))
		return TRUE;
	
	return CMDIFrameWnd::PreTranslateMessage(pMsg);
}

void CGuiMDIFrame::OnFilePrintPreview() 
{
	// TODO: Add your command handler code here
	
}

BOOL CGuiMDIFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	return CMDIFrameWnd::PreCreateWindow(cs);
}



BOOL CGuiMDIFrame::InitMDITabbed()
{
	if(::IsWindow(m_MdiTabbed.GetSafeHwnd())) return TRUE;
	if (!m_MdiTabbed.Create(WS_VISIBLE|WS_CHILD,CRect(0,0,0,0),this,0x333))
		return FALSE;
	if (::IsWindow(m_wndMenuBar.GetSafeHwnd())) m_wndMenuBar.SetTabbed(TRUE);
	
	return TRUE;
}
//*************************************************************************
BOOL CGuiMDIFrame::PreCreateWindow(CREATESTRUCT& cs, UINT nIconID)
{
	cs.lpszClass = AfxRegisterWndClass( 0, NULL, NULL,
		AfxGetApp()->LoadIcon(nIconID));
	ASSERT(cs.lpszClass);

	return CMDIFrameWnd::PreCreateWindow(cs);
}
//*************************************************************************
void CGuiMDIFrame::ShowHideBar(CGuiControlBar* pBar)
{
	if (pBar->IsWindowVisible())
		ShowControlBar(pBar, FALSE, FALSE);
	else
		ShowControlBar(pBar, TRUE, FALSE);
}


void CGuiMDIFrame::ShowHideBar(CControlBar* pBar)
{
	if (pBar->IsWindowVisible())
		ShowControlBar(pBar, FALSE, FALSE);
	else
		ShowControlBar(pBar, TRUE, FALSE);
}
//*************************************************************************
void CGuiMDIFrame::EnableDocking(DWORD dwDockStyle)
{
	ASSERT((dwDockStyle & ~ (CBRS_ALIGN_ANY | CBRS_FLOAT_MULTI)) == 0);


	m_pFloatingFrameClass = RUNTIME_CLASS(CMiniDockFrameWnd);
	CGuiDocBarExten* pDock;
	DWORD dwStyle = WS_CHILD| WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	DWORD dwstyle;
	DockSpecialBars();

	pDock = new CGuiDocBarExten();
	pDock->Create(this,dwStyle|CBRS_TOP, AFX_IDW_DOCKBAR_TOP);
	dwstyle = pDock->GetBarStyle();
	dwstyle &= ~CBRS_BORDER_ANY;
	pDock->SetBarStyle(dwstyle);
	
	pDock = new CGuiDocBarExten();
	pDock->Create(this, dwStyle|CBRS_BOTTOM, AFX_IDW_DOCKBAR_BOTTOM);
	dwstyle = pDock->GetBarStyle();
	dwstyle &= ~CBRS_BORDER_ANY;
	pDock->SetBarStyle(dwstyle);

	pDock = new CGuiDocBarExten();
	pDock->Create(this,dwStyle|CBRS_LEFT, AFX_IDW_DOCKBAR_LEFT);
	dwstyle = pDock->GetBarStyle();
	dwstyle &= ~CBRS_BORDER_ANY;
	pDock->SetBarStyle(dwstyle);

	pDock = new CGuiDocBarExten();
	pDock->Create(this,dwStyle|CBRS_RIGHT, AFX_IDW_DOCKBAR_RIGHT);
	dwstyle = pDock->GetBarStyle();
	dwstyle &= ~CBRS_BORDER_ANY;
	pDock->SetBarStyle(dwstyle);
	m_pFloatingFrameClass = RUNTIME_CLASS(CGuiMiniFrame);


}


DWORD CGuiMDIFrame::CanDock(CRect rect, DWORD dwDockStyle, CDockBar** ppDockBar)
{
	// dwDockStyle -- allowable styles of bar
	// don't allow to dock to floating unless multi is specified
	dwDockStyle &= CBRS_ALIGN_ANY|CBRS_FLOAT_MULTI;

	if (ppDockBar != NULL)
		*ppDockBar = NULL;
	POSITION pos = m_listControlBars.GetHeadPosition();
	while (pos != NULL)
	{
		CDockBar* pDockBar = (CDockBar*)m_listControlBars.GetNext(pos);
		if (pDockBar->IsDockBar() && pDockBar->IsWindowVisible() &&
			(pDockBar->m_dwStyle & dwDockStyle & CBRS_ALIGN_ANY) &&
			(!pDockBar->m_bFloating ||
				(dwDockStyle & pDockBar->m_dwStyle & CBRS_FLOAT_MULTI)))
		{
			CRect rectBar;
			pDockBar->GetWindowRect(&rectBar);
			if (rectBar.Width() == 0)
				rectBar.right++;
			if (rectBar.Height() == 0)
				rectBar.bottom++;
			if (rectBar.IntersectRect(rectBar, rect))
			{
				if (ppDockBar != NULL)
					*ppDockBar = pDockBar;
				return pDockBar->m_dwStyle & dwDockStyle;
			}
		}
	}
	return 0;
}


void CGuiMDIFrame::DockSpecialBars()
{
	
	DWORD dwStyle = WS_CHILD| WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	m_dockTop=new CGuiDocBarExtenEx();
	m_dockTop->Create(this,dwStyle|CBRS_TOP,CBRS_ALIGN_TOP);
	DWORD dwstyle;
	dwstyle = m_dockTop->GetBarStyle();
	dwstyle &= ~CBRS_BORDER_ANY;
	m_dockTop->SetBarStyle(dwstyle);
	
	m_dockBottom=new CGuiDocBarExtenEx();
	m_dockBottom->Create(this,dwStyle|CBRS_BOTTOM,CBRS_ALIGN_BOTTOM);
	dwstyle = m_dockBottom->GetBarStyle();
	dwstyle &= ~CBRS_BORDER_ANY;
	m_dockBottom->SetBarStyle(dwstyle);

	m_dockLeft=new CGuiDocBarExtenEx();
	m_dockLeft->Create(this,dwStyle|CBRS_LEFT,CBRS_ALIGN_LEFT);
	dwstyle = m_dockLeft->GetBarStyle();
	dwstyle &= ~CBRS_BORDER_ANY;
	m_dockLeft->SetBarStyle(dwstyle);
	
	m_dockRight=new CGuiDocBarExtenEx();
	m_dockRight->Create(this,dwStyle|CBRS_RIGHT,CBRS_ALIGN_RIGHT);
	dwstyle = m_dockRight->GetBarStyle();
	dwstyle &= ~CBRS_BORDER_ANY;
	m_dockRight->SetBarStyle(dwstyle);
	

/*	m_dockHideTop.Create(this,CBRS_ALIGN_TOP);
	m_dockHideTop.ShowWindow(SW_HIDE);
	m_dockHideBottom.Create(this,CBRS_ALIGN_BOTTOM);
	m_dockHideBottom.ShowWindow(SW_HIDE);
	m_dockHideLeft.Create(this,CBRS_ALIGN_LEFT);
	m_dockHideLeft.ShowWindow(SW_HIDE);
	m_dockHideRight.Create(this,CBRS_ALIGN_RIGHT);
	m_dockHideRight.ShowWindow(SW_HIDE);
*/

	m_dockHideTop.Create(this,CBRS_ALIGN_TOP);
	dwstyle = m_dockHideTop.GetBarStyle();
	dwstyle &= CBRS_HIDE_INPLACE;
	m_dockHideTop.SetBarStyle(dwstyle);

	m_dockHideBottom.Create(this,CBRS_ALIGN_BOTTOM);
	dwstyle = m_dockHideBottom.GetBarStyle();
	dwstyle &= CBRS_HIDE_INPLACE;
	m_dockHideBottom.SetBarStyle(dwstyle);
	
	m_dockHideLeft.Create(this,CBRS_ALIGN_LEFT);
	dwstyle = m_dockHideLeft.GetBarStyle();
	dwstyle &= CBRS_HIDE_INPLACE;
	m_dockHideLeft.SetBarStyle(dwstyle);
	
	m_dockHideRight.Create(this,CBRS_ALIGN_RIGHT);
	dwstyle = m_dockHideRight.GetBarStyle();
	dwstyle &= CBRS_HIDE_INPLACE;
	m_dockHideRight.SetBarStyle(dwstyle);
	

	m_dockToolbarTop.Create(this,CBRS_ALIGN_TOP);
	m_dockToolbarBottom.Create(this,CBRS_ALIGN_BOTTOM);
	m_dockToolbarLeft.Create(this,CBRS_ALIGN_LEFT);
	m_dockToolbarRight.Create(this,CBRS_ALIGN_RIGHT);

}
/////////////////////////////////////////////////////////////////////////////
// CGuiMDIFrame message handlers
//***********************************************************************
BOOL CGuiMDIFrame::DestroyWindow()
{
	CWinApp* pApp = AfxGetApp();
	TCHAR szSection[256];
	wsprintf(szSection, "%s-Main", sProfile);
	CRect rc;
	GetWindowRect(rc);
	pApp->WriteProfileString(szSection, NULL, NULL);
	pApp->WriteProfileInt(szSection, "left", rc.left);
	pApp->WriteProfileInt(szSection, "right", rc.right);
	pApp->WriteProfileInt(szSection, "bottom", rc.bottom);
	pApp->WriteProfileInt(szSection, "top", rc.top);
	
	SaveBarState(sProfile);
	SavePosBar(sProfile);
	return  CFrameWnd::DestroyWindow();
}

void CGuiMDIFrame::SavePosBar(CString szBars)
{
	POSITION pos = m_listControlBars.GetHeadPosition();
    while (pos != NULL)
    {
        CGuiControlBar* pBar = (CGuiControlBar*) m_listControlBars.GetNext(pos);
        ASSERT(pBar != NULL);
        if (pBar->IsKindOf(RUNTIME_CLASS(CGuiControlBar)))
            pBar->SaveBar(szBars);
    }
}

void CGuiMDIFrame::LoadPosBar(CString szBars)
{
	POSITION pos = m_listControlBars.GetHeadPosition();
    while (pos != NULL)
    {
        CGuiControlBar* pBar = (CGuiControlBar*) m_listControlBars.GetNext(pos);
        ASSERT(pBar != NULL);
        if (pBar->IsKindOf(RUNTIME_CLASS(CGuiControlBar)))
            pBar->LoadStateBar(szBars);
    }
}

//***********************************************************************
BOOL CGuiMDIFrame::VerifyBarState(LPCTSTR lpszProfileName)
{
    CDockState state;
    state.LoadState(lpszProfileName);

    for (int i = 0; i < state.m_arrBarInfo.GetSize(); i++)
    {
        CControlBarInfo* pInfo = (CControlBarInfo*)state.m_arrBarInfo[i];
        ASSERT(pInfo != NULL);
        int nDockedCount = pInfo->m_arrBarID.GetSize();
        if (nDockedCount > 0)
        {
            // dockbar
            for (int j = 0; j < nDockedCount; j++)
            {
                UINT nID = (UINT) pInfo->m_arrBarID[j];
                if (nID == 0) continue; // row separator
                if (nID > 0xFFFF)
                    nID &= 0xFFFF; // placeholder - get the ID
                if (GetControlBar(nID) == NULL)
                    return FALSE;
            }
        }

        if (!pInfo->m_bFloating) // floating dockbars can be created later
            if (GetControlBar(pInfo->m_nBarID) == NULL)
                return FALSE; // invalid bar ID
    }

    return TRUE;
}


void CGuiMDIFrame::LoadBars()
{
	
	CWinApp* pApp = AfxGetApp();
	TCHAR szSection[256];
 	wsprintf(szSection, "%s-Main", sProfile);
	CRect rc=CRect(0,0,0,0);
	rc.left=pApp->GetProfileInt(szSection, "left",10);
	rc.top=pApp->GetProfileInt(szSection, "top",10);
	rc.bottom=pApp->GetProfileInt(szSection, "bottom",400);
	rc.right=pApp->GetProfileInt(szSection, "right",600);
	MoveWindow(rc);
	if (VerifyBarState(sProfile))
	{
		LoadBarState(sProfile);
		LoadPosBar(sProfile);
	}
}
void CGuiMDIFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	// TODO: Add your specialized code here and/or call the base class

	CMDIFrameWnd::OnUpdateFrameTitle(bAddToTitle);
	if(::IsWindow(m_MdiTabbed.GetSafeHwnd()))
		m_MdiTabbed.UpdateWindows();

}




int CGuiMDIFrame::InitMenu(UINT uIDMenu)
{
	if (!m_wndMenuBar.CreateEx(this, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_SIZE_DYNAMIC ) ||
		!m_wndMenuBar.LoadMenuBar(uIDMenu))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}	
	m_NewMenu.LoadMenu(uIDMenu);

	m_wndCool.Install(this);
	m_wndCool.LoadToolbar(uIDMenu);
	return 0;
}

//int CGuiMDIFrame::InitMenu(UINT uIDMenu)

int CGuiMDIFrame::InitStatusBar(UINT *indicators,int nSize)
{
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  nSize))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	return 0;
}



int CGuiMDIFrame::InitToolBar(UINT uIDMenu)
{

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(0,0,0,0),uIDMenu) ||
		!m_wndToolBar.LoadToolBar(uIDMenu))
		{
		TRACE0("Failed to create toolbar\n");
		return -1; // fail to create
	}
// new code:  Serge Koroleuve

	IMAGEINFO ii;
	m_wndToolBar.GetToolBarCtrl().GetImageList()->GetImageInfo(0, &ii);
	int cx = ii.rcImage.right - ii.rcImage.left;
	int cy = ii.rcImage.bottom - ii.rcImage.top;
	int nCount = m_wndToolBar.GetToolBarCtrl().GetImageList()->GetImageCount();

	CImageList imageList;
	CBitmap bitmap;

	bitmap.LoadBitmap(uIDMenu);
	imageList.Create(cx, cy, ILC_COLORDDB|ILC_MASK, nCount, 0);
	imageList.Add(&bitmap, RGB(192,192,192));
	m_wndToolBar.SendMessage(TB_SETIMAGELIST, 0, (LPARAM)imageList.m_hImageList);
	imageList.Detach();
	bitmap.Detach();
// end of new code
return 0;
}

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
 *																			*	
 ****************************************************************************/



#include "stdafx.h"
#include "GuiStatusBar.h"
#include "GuiDrawLayer.h"

// CGuiStatusBar

IMPLEMENT_DYNAMIC(CGuiStatusBar, CStatusBar)
CGuiStatusBar::CGuiStatusBar()
{
	m_Style=DEVSTUDIO;
}

CGuiStatusBar::~CGuiStatusBar()
{
}


BEGIN_MESSAGE_MAP(CGuiStatusBar, CStatusBar)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CGuiStatusBar message handlers

void CGuiStatusBar::SetStyle(Style nStyle)
{
	m_Style=nStyle;
}
void CGuiStatusBar::PreSubclassWindow() 
{
	ModifyStyle(0,BS_OWNERDRAW);
	CStatusBar::PreSubclassWindow();
}

void CGuiStatusBar::OnPaint()
{
	CRect rcClient,rcTemp;
	CBrush cb;
	CPaintDC dc(this); 
	GetClientRect(&rcClient);
	cb.CreateSolidBrush(GuiDrawLayer::GetRGBColorFace());
	CWnd::DefWindowProc( WM_PAINT, (WPARAM)dc.m_hDC, 0 );
	rcTemp=rcClient;
	rcClient.bottom=rcClient.top+2;
	rcClient.right+=50;
	dc.FillRect(rcClient,&cb);
	Drawpanels(&dc);
	cb.DeleteObject();
}

void  CGuiStatusBar::Drawpanels(CDC *pDC)
{
	int iCont=GetCount();
	CRect rcCli,rcCliente;
	CString m_cad;
	int OldMode=pDC->SetBkMode(TRANSPARENT);
	for (int i=0; i < iCont; i++)
	{
		GetItemRect(i,&rcCli);
		GetPaneText(i, m_cad);
		if (i == iCont-1)
		{
			CSize m_sChar=pDC->GetTextExtent(m_cad,m_cad.GetLength());
			if (rcCli.Width() < m_sChar.cx)
			{
				int dif=m_sChar.cx-rcCli.Width();
				rcCli.right=rcCli.left+m_sChar.cx-dif;
			}
		}
		if (m_Style == DEVSTUDIO)
			pDC->Draw3dRect(rcCli,GuiDrawLayer::GetRGBColorShadow(),GuiDrawLayer::GetRGBColorShadow());
		else
			pDC->Draw3dRect(rcCli,GuiDrawLayer::GetRGBColorTabs(),GuiDrawLayer::GetRGBColorTabs());
	//	ClearRect(pDC,rcCli);
		if (i == iCont-1)
		{
			rcCli.left=rcCli.right;
			rcCli.right=rcCli.left+2;
			pDC->Draw3dRect(rcCli,GuiDrawLayer::GetRGBColorFace(),GuiDrawLayer::GetRGBColorFace());
			rcCli.right+=1;
			pDC->Draw3dRect(rcCli,GuiDrawLayer::GetRGBColorFace(),GuiDrawLayer::GetRGBColorFace());
			rcCli.right+=1;
			pDC->Draw3dRect(rcCli,GuiDrawLayer::GetRGBColorFace(),GuiDrawLayer::GetRGBColorFace());

		}

		
		//rcCli.left+=1;
	
	}
	pDC->SetBkMode(OldMode);
	GetClientRect(&rcCliente);
	if ((rcCliente.right - rcCli.right) != 0)
	{
		CBrush cb;
		cb.CreateSolidBrush(GuiDrawLayer::GetRGBColorFace());
		rcCliente.left = rcCli.right;
		rcCliente.bottom= rcCliente.top+6;
		pDC->FillRect(rcCliente,&cb);
		//DrawLB(&rcCliente,pDC);
	}
}

void CGuiStatusBar::ClearRect(CDC* pDC,CRect rc)
{
	CRect rcCli=rc;
	rcCli.left+=1;
	rcCli.top+=1;
	for (int i=0; i < 3; i++)
	{
		rcCli.right-=1;
		rcCli.bottom-=1;
        pDC->Draw3dRect(rcCli,GuiDrawLayer::GetRGBColorFace(),GuiDrawLayer::GetRGBColorFace());
	}
}
void CGuiStatusBar::DrawLB(CRect* rcLb,CDC* pDC)
{
	pDC->SelectStockObject(COLOR_3DSHADOW);
	int gap=10;
	for (int i=0; i< 3; i++)
	{
		pDC->MoveTo(rcLb->left+gap,rcLb->bottom);
		pDC->LineTo(rcLb->right,rcLb->top+gap+1);
		gap+=2;
	}
}

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
#include "resource.h"
#include "guitabbed.h"
#include "GuiBaseTab.h"
#include "..\header\guitabbed.h"
#define SCROLLCLOSE 0x886
#define SCROLLLEFT  0x887
#define SCROLLRIGHT 0x888



CGuiTabbed::CGuiTabbed(void)
{
	m_nDif=0;
}

CGuiTabbed::~CGuiTabbed(void)
{
}



void CGuiTabbed::Drawtabs(CDC* dc)
{
	int m_StrPos=0;
	BOOL m_ViewAlltab=TRUE;
	CRect m_rClient;
	CBrush cbr;
	CPen light(PS_SOLID,1,GetSysColor(COLOR_BTNHIGHLIGHT));
	CPen Dark(PS_SOLID,1,GetSysColor(COLOR_BTNSHADOW));
	CPen Black(PS_SOLID,1,GetSysColor(BLACK_PEN));	
	CRect rectText;
	CFont* m_fontOld=dc->SelectObject(&m_cfontBold);
	
	CSize mszBtn=GetSizeButton();
	int m_iSizeButton=mszBtn.cx;
	m_rectTab.DeflateRect(1,1);
	m_rectTab.bottom+=1;
	cbr.CreateSolidBrush(GuiDrawLayer::GetRGBColorTabs());
	dc->FillRect(m_rectTab,&cbr);
	m_rectTab.InflateRect(1,1);

	CRect rectT=m_rectTab;
	rectT.right-=m_iSizeButton+2;
	int mSizeTotal=m_iMinValRec;
	m_toolBtn[0].EnableWindow(FALSE);
	m_toolBtn[1].EnableWindow(FALSE);
	m_toolBtnC.EnableWindow(FALSE);
	if (mSizeTotal > m_rectTab.Width())
	{
		
		m_toolBtn[1].EnableWindow(TRUE);
		if (m_nDif < 0) 
			m_toolBtn[0].EnableWindow(TRUE);
		if((mSizeTotal-(abs(m_nDif)-20)) < m_rectTab.Width())
			m_toolBtn[1].EnableWindow(FALSE);
	}
	else
		m_nDif=0;
	
	CRect rectScroll; 
	CRgn reg;	
	CRect rectRecorte;
	rectRecorte=rectT;
	reg.CreateRectRgnIndirect(rectRecorte);
	dc->SelectClipRgn(&reg);
	int m_right=m_nDif;
	for ( int iCont=0; iCont< m_Numtabs;iCont++)
	{	

		CGuiTab* ctb=(CGuiTab*)m_pArray[iCont];
		CRect mrt=ctb->rect;		
		int ancho=mrt.Width();
		mrt.left=m_right;
		mrt.right=mrt.left+ancho;
		CPoint Coor[4];
		//si es la carpeta seleccionada, se debe 
		//eliminar las lineas oscuras de encima
		if (iCont ==m_iSelectTab)
		{
			CBrush cbr;
			CRect  m_rectTabAux=mrt;
			cbr.CreateSysColorBrush(COLOR_BTNFACE);
			m_rectTabAux.top+=3;
			m_rectTabAux.bottom+=2;
			dc->FillRect(m_rectTabAux,&cbr);
			CPen* pOldPen=dc->SelectObject(&light);
			dc->MoveTo(m_rectTabAux.left,m_rectTabAux.top);
			dc->LineTo(m_rectTabAux.left,m_rectTabAux.bottom);
			//linea de arriba
			dc->MoveTo(m_rectTabAux.left,m_rectTabAux.top);
			dc->LineTo(m_rectTabAux.right,m_rectTabAux.top);
			//linea derecha observe que se pinta dos veces para
			//dar el efecto de redondeada en la puntas
			dc->SelectObject(pOldPen);
			pOldPen=dc->SelectObject(&Black);
			dc->MoveTo(m_rectTabAux.right-1,m_rectTabAux.top+1);
			dc->LineTo(m_rectTabAux.right-1,m_rectTabAux.bottom);
			dc->SelectObject(pOldPen);
			//pDC->TextOut(rCText.left+3,rCText.top,m_caption);

		}
		else
		{
			if ((iCont+1) != m_iSelectTab)
			{
				CPen* pOldPen=dc->SelectObject(&Dark);
				dc->MoveTo(mrt.right-1,mrt.top+3);
				dc->LineTo(mrt.right-1,mrt.bottom+1);
				dc->SelectObject(pOldPen);
			}
				
			
		}
		CRect m_rectTabAux= mrt;
		m_rectTabAux.top+=4;
		m_rectTabAux.left+=4;
		int OldMode=dc->SetBkMode(TRANSPARENT);

		COLORREF clrtext;
		
		if (iCont !=m_iSelectTab)
		{
			clrtext=dc->SetTextColor(GuiDrawLayer::GetRGBColorGrayText());
			dc->SelectObject(&m_cfont);
		}
		dc->DrawText(ctb->lpMsg,m_rectTabAux,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
		
		if (iCont !=m_iSelectTab)
		{
			dc->SetTextColor(clrtext);
			dc->SelectObject(&m_cfontBold);
		}	
		m_right=mrt.right;
		ctb->rect=mrt;
		dc->SetBkMode(OldMode);
	}


	dc->SelectClipRgn(NULL);
	
	CRect rcCli;
	GetClientRect(rcCli);
    rcCli.top=m_rectTab.top;
	rcCli.bottom=m_rectTab.bottom;
	rcCli.left=rcCli.right-(m_iSizeButton);
	int nacum=0;
	for (int i=0; i<3;i++)
	{
		CSize szBtn=m_toolBtn[i].GetSizeButton();
		m_toolBtn[i].MoveWindow(rcCli.left+nacum-1,rcCli.top+2,szBtn.cx,rcCli.Height()-4);
		nacum+=szBtn.cx;
	}
	GetClientRect(rcCli);
	dc->Draw3dRect(rcCli,GuiDrawLayer::GetRGBColorShadow(),GuiDrawLayer::GetRGBColorShadow());
	m_toolBtnC.EnableWindow(TRUE);
	cbr.DeleteObject();
	dc->SelectObject(m_fontOld);
}


BEGIN_MESSAGE_MAP(CGuiTabbed, CGuiTabWnd)
	ON_WM_CREATE()
	ON_WM_SYSCOLORCHANGE()
	ON_COMMAND(SCROLLLEFT,OnScrollLeft)
	ON_COMMAND(SCROLLRIGHT,OnScrollRight)
	ON_COMMAND(SCROLLCLOSE,OnScrollClose)
END_MESSAGE_MAP()

void  CGuiTabbed::OnScrollLeft()
{
	m_nDif+=10;
	CClientDC dc(this);
	Drawtabs(&dc);	
	//RecalLayout();
	
}

void  CGuiTabbed::OnScrollRight()
{
	m_nDif-=10;
	CClientDC dc(this);
	Drawtabs(&dc);	
	//RecalLayout();
}

void  CGuiTabbed::OnScrollClose()
{
	//no es nuestro problema lo debe manejar la clase derivada
}


int CGuiTabbed::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CGuiTabWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_img.Create(IDB_DOCKBAR,9,11,RGB(255,0,255)))
	{
		TRACE0("error imagelist");
	}

	m_toolBtn[0].Create(_T(""),WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW,CRect(0,0,0,0),this,SCROLLLEFT);
	m_toolBtn[0].SetStyleButton(CGuiNormalButton::STL_FLAT);
	m_toolBtn[0].SetToolTip("Scroll Left");
	m_toolBtn[0].SethIcon(m_img.ExtractIcon(4));
	m_toolBtn[0].SetColor(GuiDrawLayer::GetRGBColorTabs());
	m_toolBtn[0].ShowDark(FALSE);
	m_toolBtn[1].Create(_T(""),WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW,CRect(0,0,0,0),this,SCROLLRIGHT);
	m_toolBtn[1].SetStyleButton(CGuiNormalButton::STL_FLAT);
	m_toolBtn[1].SetToolTip("Scroll Right");
	m_toolBtn[1].SethIcon(m_img.ExtractIcon(2));
	m_toolBtn[1].SetColor(GuiDrawLayer::GetRGBColorTabs());
	m_toolBtn[1].ShowDark(FALSE);
	m_toolBtnC.Create(_T(""),WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW,CRect(0,0,0,0),this,SCROLLCLOSE);
	m_toolBtnC.SetStyleButton(CGuiNormalButton::STL_FLAT);
	m_toolBtnC.SetToolTip("Close");
	m_toolBtnC.SethIcon(m_img.ExtractIcon(10));
	m_toolBtnC.SetColor(GuiDrawLayer::GetRGBColorTabs());
	m_toolBtnC.ShowDark(FALSE);
	m_toolBtn[0].EnableWindow(FALSE);
	m_toolBtn[1].EnableWindow(FALSE);
	SetALingTabs(CGuiTabWnd::ALN_TOP); //los tabs arriba
	SetStyle(CGuiTabWnd::SHADOW);	   //no debe acondicionar el tamaño de los tabs		return 0;
	SetTypeTab(CGuiTabWnd::TYPE_MDI);
	SetActualTitle(FALSE);
	return 0;
}

CSize CGuiTabbed::GetSizeButton()
{
	CSize szresult=CSize(0,0);
	for (int i=0; i < 3; i++)
	{
		CSize sztemp=m_toolBtn[i].GetSizeButton();
		szresult.cx+=sztemp.cx;
		szresult.cy+=sztemp.cy;
	}
	return szresult;
}

void CGuiTabbed::OnSysColorChange() 
{
	CGuiTabWnd::OnSysColorChange();
	m_toolBtn[0].SetColor(GuiDrawLayer::GetRGBColorTabs());	
	m_toolBtn[1].SetColor(GuiDrawLayer::GetRGBColorTabs());
	m_toolBtn[2].SetColor(GuiDrawLayer::GetRGBColorTabs());	
	m_toolBtn[0].Invalidate();
    m_toolBtn[0].UpdateWindow();	
	m_toolBtn[1].Invalidate();
    m_toolBtn[1].UpdateWindow();
	m_toolBtn[2].Invalidate();
    m_toolBtn[2].UpdateWindow();
	Invalidate ();
	UpdateWindow ();
}


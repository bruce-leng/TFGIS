// StyleBar.cpp : implementation file
//
#include "stdafx.h"
#include "TilSys.h"
#include "TilSysDoc.h"
#include "TilSysView.h"
#include "StyleBar.h"
//#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WIDTH_HIEIGHT_RATIO 0.45
/////////////////////////////////////////////////////////////////////////////
// CStyleBar

BEGIN_MESSAGE_MAP(CStyleBar, CToolBar)
	//{{AFX_MSG_MAP(CStyleBar)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	ON_COMMAND(ID_CHAR_BOLD, OnCharBold)
	//}}AFX_MSG_MAP
  ON_CBN_SELENDOK (IDC_FONTNAMES, OnSelectFont)
  ON_CBN_SELENDOK (IDC_FONTSIZES, OnSelectSize)
  END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStyleBar message handlers

int CStyleBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	static int nFontSizes[] = {1,2,3,4,5,6,7,
		8, 9, 10, 11, 12, 14, 16, 18, 20, 22, 24, 26, 28, 32, 36, 48, 72
	};
	
	if (CToolBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!LoadToolBar (IDR_STYLE_BAR))
		return -1;
	
	CClientDC dc (this);
	m_font.CreatePointFont (80, _T ("MS Sans Serif"));
	CFont* pOldFont = dc.SelectObject (&m_font);
	
	TEXTMETRIC tm;
	dc.GetTextMetrics (&tm);
	int cxChar = tm.tmAveCharWidth;
	int cyChar = tm.tmHeight + tm.tmExternalLeading;
	
	dc.SelectObject (pOldFont);
	
	SetButtonInfo (4, IDC_FONTNAMES, TBBS_SEPARATOR, cxChar * 32);
	
	CRect rect;
	GetItemRect (4, &rect);
	rect.bottom = rect.top + (cyChar * 16);
	
	if (!m_wndFontNames.Create (WS_CHILD | WS_VISIBLE | WS_VSCROLL |
		CBS_DROPDOWNLIST | CBS_SORT, rect, this, IDC_FONTNAMES))
		return -1;
	
	m_wndFontNames.SetFont (&m_font);
	InitTypefaceList (&dc);
	
	SetButtonInfo (6, IDC_FONTSIZES, TBBS_SEPARATOR, cxChar * 12);
	
	GetItemRect (6, &rect);
	rect.bottom = rect.top + (cyChar * 14);
	
	if (!m_wndFontSizes.Create (WS_CHILD | WS_VISIBLE | WS_VSCROLL |
		CBS_DROPDOWNLIST, rect, this, IDC_FONTSIZES))
		return -1;
	
	m_wndFontSizes.SetFont (&m_font);
	
	CString string;
	int nCount = sizeof (nFontSizes) / sizeof (int);
	for (int k=0; k<nCount; k++) {
		string.Format (_T ("%d"), nFontSizes[k]);            
		m_wndFontSizes.AddString (string);
	}
	m_wndFontSizes.SetCurSel(2);
	
	m_wndFontNames.SetCurSel(m_wndFontNames.FindString(0,"Arial"));
	
	isBoldUp=true;
	isUnderlineUp=true;
	isItalicUp=true;
	 
	return 0;
}

void CStyleBar::OnSelectFont ()
{
	TCHAR szFaceName[LF_FACESIZE];
	int nIndex = m_wndFontNames.GetCurSel ();
	m_wndFontNames.GetLBText (nIndex, szFaceName);
	
	//	CTilSysView* pView = ((CTilSysApp*)AfxGetApp())->GetView(RUNTIME_CLASS(CScrollView));
	CMDIChildWnd*pChild=((CMDIFrameWnd*)AfxGetMainWnd())->MDIGetActive();
	CTilSysView*pView=(CTilSysView*)pChild->GetActiveView();
	
	ENUMLOGFONT lpelf;
	m_FontMap.Lookup(nIndex,lpelf);
	
	TCHAR szSize[8];
	int nIndex1 = m_wndFontSizes.GetCurSel ();
	m_wndFontSizes.GetLBText (nIndex1, szSize);
	
	int nSize = atoi (szSize) *5;
	float fmultiple=WIDTH_HIEIGHT_RATIO;//float(nSize)/float(lpelf.elfLogFont.lfHeight);
	lpelf.elfLogFont.lfHeight=nSize;
	lpelf.elfLogFont.lfWidth=nSize*fmultiple;
    
	lpelf.elfLogFont.lfWeight=this->isBoldUp?FW_NORMAL:FW_BOLD;
	lpelf.elfLogFont.lfItalic=this->isItalicUp?0:1;
	lpelf.elfLogFont.lfUnderline=this->isUnderlineUp?0:1;
	
	if (pView->m_GraphCaption.m_LBDownTimes==1)
		pView->m_GraphCaption.m_pCurrentUnit->ChangeFont(&lpelf);
}

void CStyleBar::OnSelectSize ()
{
	TCHAR szSize[8];
	int nIndex = m_wndFontSizes.GetCurSel ();
	m_wndFontSizes.GetLBText (nIndex, szSize);
	
	int nSize = atoi (szSize) *5; // Need twips
	
  CMDIChildWnd*pChild=((CMDIFrameWnd*)AfxGetMainWnd())->MDIGetActive();
	CTilSysView*pView=(CTilSysView*)pChild->GetActiveView();
	
	ENUMLOGFONT lpelf;
	nIndex = m_wndFontNames.GetCurSel ();
	this->m_FontMap.Lookup(nIndex,lpelf);

	float fmultiple=WIDTH_HIEIGHT_RATIO;//float(nSize)/float(lpelf.elfLogFont.lfHeight);
	lpelf.elfLogFont.lfHeight=nSize;
	lpelf.elfLogFont.lfWidth=nSize*fmultiple;
	
	lpelf.elfLogFont.lfWeight=this->isBoldUp?FW_NORMAL:FW_BOLD;
	lpelf.elfLogFont.lfItalic=this->isItalicUp?0:1;
	lpelf.elfLogFont.lfUnderline=this->isUnderlineUp?0:1;

	if (pView->m_GraphCaption.m_pCurrentUnit!=NULL)
		pView->m_GraphCaption.m_pCurrentUnit->ChangeFontSize(&lpelf);
}

void CStyleBar::InitTypefaceList (CDC* pDC)
{
    ::EnumFontFamilies (pDC->m_hDC, NULL,
        (FONTENUMPROC) EnumFontNameProc, (LPARAM) this);
}

int CALLBACK CStyleBar::EnumFontNameProc (ENUMLOGFONT* lpelf,
    NEWTEXTMETRIC* lpntm, int nFontType, LPARAM lParam)
{
	CStyleBar* pWnd = (CStyleBar*) lParam;
	int i=pWnd->m_wndFontNames.GetCount();
	 if (nFontType & TRUETYPE_FONTTYPE)
	 {
		 pWnd->m_wndFontNames.InsertString(i,lpelf->elfLogFont.lfFaceName);
		 pWnd->m_FontMap[i]=*lpelf;
	 }
	 
	 return 1;
}

void CStyleBar::OnCharBold() 
{
	TCHAR szSize[8];
	int nIndex = m_wndFontSizes.GetCurSel ();
	m_wndFontSizes.GetLBText (nIndex, szSize);
	
	int nSize = atoi (szSize) *5; 
	
	CMDIChildWnd*pChild=((CMDIFrameWnd*)AfxGetMainWnd())->MDIGetActive();
	CTilSysView*pView=(CTilSysView*)pChild->GetActiveView();
	
	ENUMLOGFONT lpelf;
	nIndex = m_wndFontNames.GetCurSel ();
	this->m_FontMap.Lookup(nIndex,lpelf);
	
	float fmultiple=WIDTH_HIEIGHT_RATIO;//float(nSize)/float(lpelf.elfLogFont.lfHeight);
	lpelf.elfLogFont.lfHeight=nSize;
	lpelf.elfLogFont.lfWidth=nSize*fmultiple;
	
	isBoldUp=!isBoldUp;
	lpelf.elfLogFont.lfWeight=this->isBoldUp?FW_NORMAL:FW_BOLD;
	lpelf.elfLogFont.lfItalic=this->isItalicUp?0:1;
	lpelf.elfLogFont.lfUnderline=this->isUnderlineUp?0:1;
	if(pView->m_GraphCaption.m_LBDownTimes==1)
		pView->m_GraphCaption.m_pCurrentUnit->ChangeFontSize(&lpelf);
}
void CStyleBar::OnCharItalic()
{
	TCHAR szSize[8];
	int nIndex = m_wndFontSizes.GetCurSel ();
	m_wndFontSizes.GetLBText (nIndex, szSize);
	
	int nSize = atoi (szSize) *5; 
	
	CMDIChildWnd*pChild=((CMDIFrameWnd*)AfxGetMainWnd())->MDIGetActive();
	CTilSysView*pView=(CTilSysView*)pChild->GetActiveView();
	
	ENUMLOGFONT lpelf;
	nIndex = m_wndFontNames.GetCurSel ();
	this->m_FontMap.Lookup(nIndex,lpelf);
	
	float fmultiple=WIDTH_HIEIGHT_RATIO;//float(nSize)/float(lpelf.elfLogFont.lfHeight);
	lpelf.elfLogFont.lfHeight=nSize;
	lpelf.elfLogFont.lfWidth=nSize*fmultiple;
	
	isItalicUp=!isItalicUp;
	lpelf.elfLogFont.lfWeight=this->isBoldUp?FW_NORMAL:FW_BOLD;
	lpelf.elfLogFont.lfItalic=this->isItalicUp?0:1;
	lpelf.elfLogFont.lfUnderline=this->isUnderlineUp?0:1;
	if (pView->m_GraphCaption.m_LBDownTimes==1)
		pView->m_GraphCaption.m_pCurrentUnit->ChangeFontSize(&lpelf);
}

void CStyleBar::OnCharUnderline()
{
	static BOOL isUp=true;
	TCHAR szSize[8];
	int nIndex = m_wndFontSizes.GetCurSel ();
	m_wndFontSizes.GetLBText (nIndex, szSize);
	
	int nSize = atoi (szSize) *5; 
	
	CMDIChildWnd*pChild=((CMDIFrameWnd*)AfxGetMainWnd())->MDIGetActive();
	CTilSysView*pView=(CTilSysView*)pChild->GetActiveView();
	
	ENUMLOGFONT lpelf;
	nIndex = m_wndFontNames.GetCurSel ();
	this->m_FontMap.Lookup(nIndex,lpelf);
	
	float fmultiple=WIDTH_HIEIGHT_RATIO;//float(nSize)/float(lpelf.elfLogFont.lfHeight);
	lpelf.elfLogFont.lfHeight=nSize;
	lpelf.elfLogFont.lfWidth=nSize*fmultiple;
	
	isUnderlineUp=!isUnderlineUp;
	lpelf.elfLogFont.lfWeight=this->isBoldUp?FW_NORMAL:FW_BOLD;
	lpelf.elfLogFont.lfItalic=this->isItalicUp?0:1;
	lpelf.elfLogFont.lfUnderline=this->isUnderlineUp?0:1;

	if (pView->m_GraphCaption.m_LBDownTimes==1)
		pView->m_GraphCaption.m_pCurrentUnit->ChangeFontSize(&lpelf);
}


void CStyleBar::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CToolBar::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: Add your message handler code here

}

void CStyleBar::IniFont()
{
 this->OnSelectFont();
}

// DlgMeshGeoVirThrill.cpp : implementation file
//

#include "stdafx.h"
#include "tilsys.h"
#include "DlgMeshGeoVirThrill.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMeshGeoVirThrill dialog


CDlgMeshGeoVirThrill::CDlgMeshGeoVirThrill(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMeshGeoVirThrill::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMeshGeoVirThrill)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgMeshGeoVirThrill::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMeshGeoVirThrill)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMeshGeoVirThrill, CDialog)
	//{{AFX_MSG_MAP(CDlgMeshGeoVirThrill)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMeshGeoVirThrill message handlers

void CDlgMeshGeoVirThrill::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CWnd* pWnd = (CWnd*)GetDlgItem(IDC_STATIC);
	CRect rect;
	pWnd->GetClientRect(rect);

	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_TUNNEL);

	CxImage image;
	image.CreateFromHBITMAP((HBITMAP)bitmap.GetSafeHandle());
	image.Draw(pWnd->GetDC()->GetSafeHdc(), rect);
	
	// Do not call CDialog::OnPaint() for painting messages
}

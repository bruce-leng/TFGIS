// DlgOtherOptions.cpp : implementation file
//

#include "stdafx.h"
#include "tilsys.h"
#include "DlgOtherOptions.h"
#include "TilSysDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOtherOptions dialog


CDlgOtherOptions::CDlgOtherOptions(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOtherOptions::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOtherOptions)
	m_nPrintTarget = 0;
	//}}AFX_DATA_INIT
	CTilSysDoc* pDoc = (CTilSysDoc*)((CTilSysApp*)AfxGetApp())->GetDocument();
	m_nPrintTarget = pDoc->m_FaceInfoPrint.GetPrintTarget();
}


void CDlgOtherOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOtherOptions)
	DDX_Radio(pDX, IDC_RADIO1, m_nPrintTarget);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgOtherOptions, CDialog)
	//{{AFX_MSG_MAP(CDlgOtherOptions)
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgOtherOptions message handlers


BOOL CDlgOtherOptions::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgOtherOptions::OnOk() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	CDialog::OnOK();
}

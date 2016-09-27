// DlgThreshold.cpp : implementation file
//

#include "stdafx.h"
#include "TilSys.h"
#include "DlgThreshold.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgThreshold dialog


DlgThreshold::DlgThreshold(CWnd* pParent /*=NULL*/)
	: CDialog(DlgThreshold::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgThreshold)
	m_level = 0;
	//}}AFX_DATA_INIT
}


void DlgThreshold::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgThreshold)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_canc);
	DDX_Text(pDX, IDC_EDIT1, m_level);
	DDV_MinMaxByte(pDX, m_level, 0, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgThreshold, CDialog)
	//{{AFX_MSG_MAP(DlgThreshold)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgThreshold message handlers

BOOL DlgThreshold::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DlgThreshold::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
}

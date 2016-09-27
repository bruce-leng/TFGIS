// DlgTunnelNames.cpp : implementation file
//

#include "stdafx.h"
#include "tilsys.h"
#include "DlgTunnelNames.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PRJNUM 22	// 隧道项目数量
/////////////////////////////////////////////////////////////////////////////
// DlgTunnelNames dialog


DlgTunnelNames::DlgTunnelNames(CWnd* pParent /*=NULL*/)
	: CDialog(DlgTunnelNames::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgTunnelNames)
	m_strLineName = _T("");
	m_strPartIdx = _T("");
	m_strTunName = _T("");
	m_strPartName = _T("");
	//}}AFX_DATA_INIT
}

DlgTunnelNames::~DlgTunnelNames()
{
}

void DlgTunnelNames::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgTunnelNames)
	DDX_Text(pDX, IDC_EDT_LINE_NAME, m_strLineName);
	DDV_MaxChars(pDX, m_strLineName, 60);
	DDX_Text(pDX, IDC_EDT_PART_IDX, m_strPartIdx);
	DDV_MaxChars(pDX, m_strPartIdx, 20);
	DDX_Text(pDX, IDC_EDT_TUN_NAME, m_strTunName);
	DDV_MaxChars(pDX, m_strTunName, 100);
	DDX_CBString(pDX, IDC_CMB_PART_NAME, m_strPartName);
	DDV_MaxChars(pDX, m_strPartName, 60);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(DlgTunnelNames, CDialog)
	//{{AFX_MSG_MAP(DlgTunnelNames)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgTunnelNames message handlers

BOOL DlgTunnelNames::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_strLineName = m_prjCaption.strLineName;
	m_strTunName = m_prjCaption.strTunName;
	m_strPartName = m_prjCaption.strPartName;
	m_strPartIdx = m_prjCaption.strPartIdx;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DlgTunnelNames::SetPrjCaption(PRJCAPTION& prjCaption)
{
	m_prjCaption = prjCaption;
}

PRJCAPTION DlgTunnelNames::GetPrjCaption()
{
	return m_prjCaption;
}

void DlgTunnelNames::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	m_prjCaption.strLineName = m_strLineName;
	m_prjCaption.strTunName = m_strTunName;
	m_prjCaption.strPartName = m_strPartName;
	m_prjCaption.strPartIdx = m_strPartIdx;

	CDialog::OnOK();
}

// Process.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "Progress.h"
#include "EdgeFind.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL g_bFinished = FALSE;
int g_Position = 0;
UINT EdgeFindThreadProc(LPVOID pParam)
{
	CTilCanvas* pCanvas = (CTilCanvas*)pParam;

	CEdgeFind* pEdgeFind = new CEdgeFind(pCanvas);
	pEdgeFind->FindEdge(40, 50, 30, 20, g_Position);
	delete pEdgeFind;
	
	pCanvas->GetGraph()->SetModifiedFlag(TRUE);
	g_bFinished = TRUE;

	return 0; // ends the thread
}

/////////////////////////////////////////////////////////////////////////////
// CProgress dialog


CProgress::CProgress(CWnd* pParent /*=NULL*/)
	: CDialog(CProgress::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProgress)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgress)
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProgress, CDialog)
	//{{AFX_MSG_MAP(CProgress)
	ON_WM_CREATE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgress message handlers

BOOL CProgress::Create()
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD);
}

int CProgress::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CenterWindow( GetDesktopWindow());
	return 0;
}

BOOL CProgress::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_progress.EnableToolTips(FALSE);
	m_progress.SetRange(0, 100);
	m_progress.SetStep(1);
	m_progress.SetPos(0);
	
	m_nTimer = SetTimer(1, 100, NULL); // 1/10 second
	ASSERT(m_nTimer != 0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// 启动Canny算子边界查找对话框
void CProgress::StartEdgeFind(CTilCanvas* pCanvas)
{
	AfxBeginThread(EdgeFindThreadProc, pCanvas, THREAD_PRIORITY_NORMAL);
}

void CProgress::OnTimer(UINT nIDEvent) 
{
	m_progress.SetPos(g_Position);
	if (g_Position == 100)
	{
		CDialog::OnOK();
		g_Position = 0;
	}
	
	CDialog::OnTimer(nIDEvent);
}

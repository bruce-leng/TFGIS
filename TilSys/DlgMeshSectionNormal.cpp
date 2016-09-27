// DlgMeshSectionNormal.cpp : implementation file
//

#include "stdafx.h"
#include "tilsys.h"
#include "DlgMeshSectionNormal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMeshSectionNormal dialog


CDlgMeshSectionNormal::CDlgMeshSectionNormal(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMeshSectionNormal::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMeshSectionNormal)
	m_nCrossSectionDK = 0;
	//}}AFX_DATA_INIT
}


void CDlgMeshSectionNormal::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMeshSectionNormal)
	DDX_Text(pDX, IDC_CROSS_SECTION_DK, m_nCrossSectionDK);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMeshSectionNormal, CDialog)
	//{{AFX_MSG_MAP(CDlgMeshSectionNormal)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMeshSectionNormal message handlers

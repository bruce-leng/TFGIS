// DlgMeshGeoPredict.cpp : implementation file
//

#include "stdafx.h"
#include "tilsys.h"
#include "DlgMeshGeoPredict.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMeshGeoPredict dialog


CDlgMeshGeoPredict::CDlgMeshGeoPredict(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMeshGeoPredict::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMeshGeoPredict)
	m_fPredictDK = 0.0f;
	//}}AFX_DATA_INIT
}


void CDlgMeshGeoPredict::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMeshGeoPredict)
	DDX_Text(pDX, IDC_PREDICT_DK, m_fPredictDK);
	DDV_MinMaxFloat(pDX, m_fPredictDK, 0.f, 1.e+010f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMeshGeoPredict, CDialog)
	//{{AFX_MSG_MAP(CDlgMeshGeoPredict)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMeshGeoPredict message handlers

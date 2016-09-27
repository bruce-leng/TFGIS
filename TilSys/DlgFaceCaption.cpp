// FaceCaption.cpp : implementation file
//

#include "stdafx.h"
#include "tilsys.h"
#include "DlgFaceCaption.h"
#include "TilComFunction.h"
#include "TilSysDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFaceCaption dialog


CDlgFaceCaption::CDlgFaceCaption(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFaceCaption::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFaceCaption)
	m_strFaceDK = _T("");
	m_strFaceMile = _T("");
	//}}AFX_DATA_INIT
}


void CDlgFaceCaption::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFaceCaption)
	DDX_CBString(pDX, IDC_FACE_DK, m_strFaceDK);
	DDX_Text(pDX, IDC_FACE_MILE, m_strFaceMile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFaceCaption, CDialog)
	//{{AFX_MSG_MAP(CDlgFaceCaption)
	ON_EN_SETFOCUS(IDC_FACE_MILE, OnSetfocusFaceMile)
	ON_EN_KILLFOCUS(IDC_FACE_MILE, OnKillfocusFaceMile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFaceCaption message handlers

BOOL CDlgFaceCaption::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// 得到文件名，即里程
	CTilImage* pImage = GetCanvasMgr()->GetCurCanvas()->GetImage();
	CString strFileName = pImage->GetPathName(); // 完整路径
	m_strSrcFileName = get_file_name(strFileName); // 仅保存原始文件名(无后缀)
	CString strFaceDKMile = get_file_name(strFileName);
	// 获得里程前缀、里程数
	CString strM, strKM;
	int nLength = strFaceDKMile.GetLength();
	for (int i = nLength-1; i >= 0; i --)
	{
		char ch = strFaceDKMile.GetAt(i);
		if (ch == '+')
		{
			strM = strFaceDKMile.Right(nLength - i - 1);
		}
		else if ((ch < '0' || ch > '9') && ch != '+' && ch != '.')
		{
			strKM = strFaceDKMile.Right(nLength - i - 1);
			m_strFaceDK = strFaceDKMile.Left(i + 1);
			m_dbFaceMile = atoi(strKM) * 1000 + atof(strM);
			m_strFaceMile = strKM;
			break;
		}
	}

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgFaceCaption::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	/*
	CEdit* pImageX = (CEdit*)GetDlgItem(IDC_FACE_CENTER_IMAGE_X);
	if (!pImageX->GetModify())
	{
		AfxMessageBox("请将掌子面信息输入完整!", MB_OK);
		pImageX->SetFocus();
		return;
	}

	CEdit* pImageY = (CEdit*)GetDlgItem(IDC_FACE_CENTER_IMAGE_Y);
	if (!pImageY->GetModify())
	{
		AfxMessageBox("请将掌子面信息输入完整!", MB_OK);
		pImageY->SetFocus();
		return;
	}

	CEdit* pGrndX = (CEdit*)GetDlgItem(IDC_FACE_CENTER_GRND_X);
	if (!pGrndX->GetModify())
	{
		AfxMessageBox("请将掌子面信息输入完整!", MB_OK);
		pGrndX->SetFocus();
		return;
	}

	CEdit* pGrndY = (CEdit*)GetDlgItem(IDC_FACE_CENTER_GRND_Y);
	if (!pGrndY->GetModify())
	{
		AfxMessageBox("请将掌子面信息输入完整!", MB_OK);
		pGrndY->SetFocus();
		return;
	}

	CEdit* pGrndZ = (CEdit*)GetDlgItem(IDC_FACE_CENTER_GRND_Z);
	if (!pGrndZ->GetModify())
	{
		AfxMessageBox("请将掌子面信息输入完整!", MB_OK);
		pGrndZ->SetFocus();
		return;
	}

	CEdit* pAngle = (CEdit*)GetDlgItem(IDC_FACE_ANGLE);
	if (!pAngle->GetModify())
	{
		AfxMessageBox("请将掌子面信息输入完整!", MB_OK);
		pAngle->SetFocus();
		return;
	}
	*/

// 	if (!pImageX->GetModify() || !pImageY->GetModify() || !pGrndX->GetModify()
// 		|| !pGrndY->GetModify() || !pGrndZ->GetModify() || pAngle->GetModify())
// 	{
// 		AfxMessageBox("请将掌子面信息输入完整!", MB_OK);
// 		return;
// 	}

	CString strFaceDKMile = m_strFaceDK + m_strFaceMile;
	// 如果新的文件名与原来的相同,则直接返回
	if (m_strSrcFileName == strFaceDKMile)
	{
		CDialog::OnCancel();
		return;
	}

	// 将原来的文件更名为新的文件名
	CTilCanvas* pCanvas = GetCanvasMgr()->GetCurCanvas();
	pCanvas->RenameAll(m_strSrcFileName, strFaceDKMile);
	pCanvas->SetModifiedFlag();

	// 更将目录树
	CTilSysApp* pApp = (CTilSysApp*)AfxGetApp();
	CTilSysDoc* pDoc = (CTilSysDoc*)pApp->GetDocument();
	pDoc->UpdateDrawTree();

	CDialog::OnOK();
}

// 当获得焦点时，设置显示方式为"13456"的方式
void CDlgFaceCaption::OnSetfocusFaceMile() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_strFaceMile.Format("%d.%d", int(m_dbFaceMile * 10) / 10, int(m_dbFaceMile * 10) % 10);
	UpdateData(FALSE);
}

// 当失去焦点时，设置显示方式如“DK3+345"的方式
void CDlgFaceCaption::OnKillfocusFaceMile() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_dbFaceMile = atof(m_strFaceMile);
	m_strFaceMile.Format("%d+%d.%d", int(m_dbFaceMile) / 1000, int(m_dbFaceMile) % 1000, int(m_dbFaceMile * 10) % 10);
	UpdateData(FALSE);
}

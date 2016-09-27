// DlgFaceInfoSaveXml.cpp : implementation file
//

#include "stdafx.h"
#include "tilsys.h"
#include "DlgFaceInfoSaveXml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace MSXML2;
/////////////////////////////////////////////////////////////////////////////
// CDlgFaceInfoSaveXml dialog


CDlgFaceInfoSaveXml::CDlgFaceInfoSaveXml(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFaceInfoSaveXml::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFaceInfoSaveXml)
	m_strDstXmlPathFile = _T("");
	m_strSrcXmlPathFile = _T("");
	//}}AFX_DATA_INIT
}


void CDlgFaceInfoSaveXml::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFaceInfoSaveXml)
	DDX_Text(pDX, IDC_EDT_DST_XML_PATH_FILE, m_strDstXmlPathFile);
	DDX_Text(pDX, IDC_EDT_SRC_XML_PATH_FILE, m_strSrcXmlPathFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFaceInfoSaveXml, CDialog)
	//{{AFX_MSG_MAP(CDlgFaceInfoSaveXml)
	ON_BN_CLICKED(IDC_BTN_SRC_XML_PATH_FILE, OnBtnSrcXmlPathFile)
	ON_BN_CLICKED(IDC_BTN_DST_XML_PATH_FILE, OnBtnDstXmlPathFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFaceInfoSaveXml message handlers

void CDlgFaceInfoSaveXml::OnBtnSrcXmlPathFile() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE,"��ѡ���뵱ǰ���������ݵ�ԭʼxml�ļ�·��",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"(*.xml)|*.xml||");
	if(dlg.DoModal() == IDOK)
	{
		m_strSrcXmlPathFile= dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CDlgFaceInfoSaveXml::OnBtnDstXmlPathFile() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE,"��ѡ�񱣴浱ǰ���������ݵ�xml�ļ�·��",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"(*.xml)|*.xml||");
	if(dlg.DoModal() == IDOK)
	{
		m_strDstXmlPathFile= dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CDlgFaceInfoSaveXml::OnOK() 
{
	UpdateData(TRUE);

	// ��ԭʼ�ļ�������Ŀ��·����������Ŀ���ļ�
	if(!::CopyFile(m_strSrcXmlPathFile,m_strDstXmlPathFile,FALSE))
	{
		AfxMessageBox("��ԭʼxml�ļ����Ƹ�Ŀ��xml�ļ�����!");
		return;
	}

	CDialog::OnOK();
}

// DlgFaceInfoEx.cpp : implementation file
//

#include "stdafx.h"
#include "tilsys.h"
#include "SysView.h"
#include "DlgFaceInfoEx.h"
#include "TilComFunction.h"
#include "MathComput.h"
#include "DlgFaceInfoSaveXml.h"
#include <float.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;
/////////////////////////////////////////////////////////////////////////////
// CDlgFaceInfoEx dialog


CDlgFaceInfoEx::CDlgFaceInfoEx(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFaceInfoEx::IDD, pParent)
{
	// ����������
	m_nFaceNum = 0;
	m_nFaceIdx = 0;
	m_pFacePhoto = NULL;

	//{{AFX_DATA_INIT(CDlgFaceInfoEx)
	m_strConstructGrade = _T("");
	m_strDesignGrade = _T("");
	m_strExcaveDir = _T("");
	m_strExcavePos = _T("");
	m_strExcaveWay = _T("");
	m_strPartIdxName = _T("");
	m_strTunName = _T("");
	m_DateTime = 0;
	m_fFaceArea = 0.0f;
	m_fFaceDepth = 0.0f;
	m_fFaceHeight = 0.0f;
	m_fFaceWidth = 0.0f;
	m_strRecSupervision = _T("");
	m_strFaceDesc = _T("");
	m_strFacePhoto = _T("");
	m_strFaceMile = _T("");
	m_bCrackStatusBFZK = FALSE;
	m_bCrackStatusJYLT = FALSE;
	m_bCrackStatusKK = FALSE;
	m_bCrackStatusMJ = FALSE;
	m_bCrackStatusOther = FALSE;
	m_bCrackWidth1 = FALSE;
	m_bCrackWidth1_3 = FALSE;
	m_bCrackWidth3_5 = FALSE;
	m_bCrackWidth5 = FALSE;
	m_bCrackWidthOther = FALSE;
	m_bExcaveStatusCQZH = FALSE;
	m_bExcaveStatusOther = FALSE;
	m_bExcaveStatusSCDK = FALSE;
	m_bExcaveStatusZW = FALSE;
	m_bExcaveStatusJSZH = FALSE;
	m_bFaceStatusOther = FALSE;
	m_bFaceStatusWD = FALSE;
	m_bFaceStatusZMBNZW = FALSE;
	m_bFaceStatusZMDK = FALSE;
	m_bFaceStatusZMJC = FALSE;
	m_bGushingStatusSS = FALSE;
	m_bGushingStatusTBD = FALSE;
	m_bGushingStatusWS = FALSE;
	m_bGushingStatusZTSR = FALSE;
	m_bGushingStatusYCPC = FALSE;
	m_bRockStrengthOther = FALSE;
	m_bRockStrengthRY = FALSE;
	m_bRockStrengthYY = FALSE;
	m_bRockStrengthJIRY = FALSE;
	m_bRockStrengthJORY = FALSE;
	m_bWeatheringOther = FALSE;
	m_bWeatheringQIFH = FALSE;
	m_bWeatheringRFH = FALSE;
	m_bWeatheringWFH = FALSE;
	m_bWeatheringQUFH = FALSE;
	m_strCrackStatusOtherDesc = _T("");
	m_strCrackWidthOtherDesc = _T("");
	m_strExcaveStatusOtherDesc = _T("");
	m_strFaceStatusOtherDesc = _T("");
	m_strRockStrengthOtherDesc = _T("");
	m_strWeatheringOtherDesc = _T("");
	m_strConstructUnit = _T("");
	m_strRecUnit = _T("");
	m_strLithology = _T("");
	m_strID = _T("");
	//}}AFX_DATA_INIT
}

CDlgFaceInfoEx::~CDlgFaceInfoEx()
{
	if (m_pFacePhoto != NULL)
		delete m_pFacePhoto;
}

void CDlgFaceInfoEx::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFaceInfoEx)
	DDX_Control(pDX, IDC_EDT_ID, m_edtID);
	DDX_Control(pDX, IDC_EDT_REC_UNIT, m_edtRecUnit);
	DDX_Control(pDX, IDC_EDT_CONSTRUCT_UNIT, m_edtConstructUnit);
	DDX_Control(pDX, IDC_EDT_WEATHERING_OTHER_DESC, m_edtWeatheringOtherDesc);
	DDX_Control(pDX, IDC_EDT_ROCK_STRENGTH_OTHER_DESC, m_edtRockStrengthOtherDesc);
	DDX_Control(pDX, IDC_EDT_FACE_STATUS_OTHER_DESC, m_edtFaceStatusOtherDesc);
	DDX_Control(pDX, IDC_EDT_EXCAVE_STATUS_OTHER_DESC, m_edtExcaveStatusOtherDesc);
	DDX_Control(pDX, IDC_EDT_CRACK_WIDTH_OTHER_DESC, m_edtCrackWidthOtherDesc);
	DDX_Control(pDX, IDC_EDT_CRACK_STATUS_OTHER_DESC, m_edtCrackStatusOtherDesc);
	DDX_Control(pDX, IDC_RECORD_COPY_PREV, m_btnRecCopyPrev);
	DDX_Control(pDX, IDC_EDT_FACE_DEPTH, m_edtFaceDepth);
	DDX_Control(pDX, IDC_EDT_TUN_NAME, m_edtTunName);
	DDX_Control(pDX, IDC_EDT_PART_IDX_NAME, m_edtPartIdxName);
	DDX_Control(pDX, IDC_EDT_REC_SUPERVISION, m_edtRecSupervision);
	DDX_Control(pDX, IDC_EDT_FACE_WIDTH, m_edtFaceWidth);
	DDX_Control(pDX, IDC_EDT_FACE_MILE, m_edtFaceMile);
	DDX_Control(pDX, IDC_EDT_FACE_HEIGHT, m_edtFaceHeight);
	DDX_Control(pDX, IDC_EDT_FACE_DESC, m_edtFaceDesc);
	DDX_Control(pDX, IDC_EDT_FACE_AREA, m_edtFaceArea);
	DDX_Control(pDX, IDC_DATETIME, m_ctlDateTime);
	DDX_Control(pDX, IDC_CMB_EXCAVE_WAY, m_cmbExcaveWay);
	DDX_Control(pDX, IDC_CMB_EXCAVE_POS, m_cmbExcavePos);
	DDX_Control(pDX, IDC_CMB_EXCAVE_DIR, m_cmbExcaveDir);
	DDX_Control(pDX, IDC_CMB_DESIGN_GRADE, m_cmbDesignGrade);
	DDX_Control(pDX, IDC_CMB_CONSTRUCT_GRADE, m_cmbConstructGrade);
	DDX_Control(pDX, IDC_STATIC_FACE_PHOTO, m_stcFacePhoto);
	DDX_Control(pDX, IDC_RECORD_NEXT, m_btnRecNext);
	DDX_Control(pDX, IDC_RECORD_MODIFY, m_btnRecModify);
	DDX_Control(pDX, IDC_RECORD_LAST, m_btnRecLast);
	DDX_Control(pDX, IDC_RECORD_FIRST, m_btnRecFirst);
	DDX_Control(pDX, IDC_RECORD_PREV, m_btnRecPrev);
	DDX_CBString(pDX, IDC_CMB_CONSTRUCT_GRADE, m_strConstructGrade);
	DDV_MaxChars(pDX, m_strConstructGrade, 10);
	DDX_CBString(pDX, IDC_CMB_DESIGN_GRADE, m_strDesignGrade);
	DDV_MaxChars(pDX, m_strDesignGrade, 10);
	DDX_CBString(pDX, IDC_CMB_EXCAVE_DIR, m_strExcaveDir);
	DDV_MaxChars(pDX, m_strExcaveDir, 20);
	DDX_CBString(pDX, IDC_CMB_EXCAVE_POS, m_strExcavePos);
	DDV_MaxChars(pDX, m_strExcavePos, 20);
	DDX_CBString(pDX, IDC_CMB_EXCAVE_WAY, m_strExcaveWay);
	DDV_MaxChars(pDX, m_strExcaveWay, 40);
	DDX_Text(pDX, IDC_EDT_PART_IDX_NAME, m_strPartIdxName);
	DDV_MaxChars(pDX, m_strPartIdxName, 60);
	DDX_Text(pDX, IDC_EDT_TUN_NAME, m_strTunName);
	DDV_MaxChars(pDX, m_strTunName, 100);
	DDX_DateTimeCtrl(pDX, IDC_DATETIME, m_DateTime);
	DDX_Text(pDX, IDC_EDT_FACE_AREA, m_fFaceArea);
	DDX_Text(pDX, IDC_EDT_FACE_DEPTH, m_fFaceDepth);
	DDX_Text(pDX, IDC_EDT_FACE_HEIGHT, m_fFaceHeight);
	DDX_Text(pDX, IDC_EDT_FACE_WIDTH, m_fFaceWidth);
	DDX_Text(pDX, IDC_EDT_REC_SUPERVISION, m_strRecSupervision);
	DDV_MaxChars(pDX, m_strRecSupervision, 20);
	DDX_Text(pDX, IDC_EDT_FACE_DESC, m_strFaceDesc);
	DDV_MaxChars(pDX, m_strFaceDesc, 660);
	DDX_Text(pDX, IDC_STATIC_FACE_PHOTO, m_strFacePhoto);
	DDV_MaxChars(pDX, m_strFacePhoto, 100);
	DDX_Text(pDX, IDC_EDT_FACE_MILE, m_strFaceMile);
	DDV_MaxChars(pDX, m_strFaceMile, 40);
	DDX_Check(pDX, IDC_CHECK_CRACK_STATUS_BFZK, m_bCrackStatusBFZK);
	DDX_Check(pDX, IDC_CHECK_CRACK_STATUS_JYLT, m_bCrackStatusJYLT);
	DDX_Check(pDX, IDC_CHECK_CRACK_STATUS_KK, m_bCrackStatusKK);
	DDX_Check(pDX, IDC_CHECK_CRACK_STATUS_MJ, m_bCrackStatusMJ);
	DDX_Check(pDX, IDC_CHECK_CRACK_STATUS_OTHER, m_bCrackStatusOther);
	DDX_Check(pDX, IDC_CHECK_CRACK_WIDTH_1, m_bCrackWidth1);
	DDX_Check(pDX, IDC_CHECK_CRACK_WIDTH_1_3, m_bCrackWidth1_3);
	DDX_Check(pDX, IDC_CHECK_CRACK_WIDTH_3_5, m_bCrackWidth3_5);
	DDX_Check(pDX, IDC_CHECK_CRACK_WIDTH_5, m_bCrackWidth5);
	DDX_Check(pDX, IDC_CHECK_CRACK_WIDTH_OTHER, m_bCrackWidthOther);
	DDX_Check(pDX, IDC_CHECK_EXCAVE_STATUS_CQZH, m_bExcaveStatusCQZH);
	DDX_Check(pDX, IDC_CHECK_EXCAVE_STATUS_OTHER, m_bExcaveStatusOther);
	DDX_Check(pDX, IDC_CHECK_EXCAVE_STATUS_SCDK, m_bExcaveStatusSCDK);
	DDX_Check(pDX, IDC_CHECK_EXCAVE_STATUS_ZW, m_bExcaveStatusZW);
	DDX_Check(pDX, IDC_CHECK_EXCAVE_STATUS_JSZH, m_bExcaveStatusJSZH);
	DDX_Check(pDX, IDC_CHECK_FACE_STATUS_OTHER, m_bFaceStatusOther);
	DDX_Check(pDX, IDC_CHECK_FACE_STATUS_WD, m_bFaceStatusWD);
	DDX_Check(pDX, IDC_CHECK_FACE_STATUS_ZMBNZW, m_bFaceStatusZMBNZW);
	DDX_Check(pDX, IDC_CHECK_FACE_STATUS_ZMDK, m_bFaceStatusZMDK);
	DDX_Check(pDX, IDC_CHECK_FACE_STATUS_ZMJC, m_bFaceStatusZMJC);
	DDX_Check(pDX, IDC_CHECK_GUSHING_STATUS_SS, m_bGushingStatusSS);
	DDX_Check(pDX, IDC_CHECK_GUSHING_STATUS_TBD, m_bGushingStatusTBD);
	DDX_Check(pDX, IDC_CHECK_GUSHING_STATUS_WS, m_bGushingStatusWS);
	DDX_Check(pDX, IDC_CHECK_GUSHING_STATUS_ZTSR, m_bGushingStatusZTSR);
	DDX_Check(pDX, IDC_CHECK_GUSHING_STATUS_YCPC, m_bGushingStatusYCPC);
	DDX_Check(pDX, IDC_CHECK_ROCK_STRENGTH_OTHER, m_bRockStrengthOther);
	DDX_Check(pDX, IDC_CHECK_ROCK_STRENGTH_RY, m_bRockStrengthRY);
	DDX_Check(pDX, IDC_CHECK_ROCK_STRENGTH_YY, m_bRockStrengthYY);
	DDX_Check(pDX, IDC_CHECK_ROCK_STRENGTH_JIRY, m_bRockStrengthJIRY);
	DDX_Check(pDX, IDC_CHECK_ROCK_STRENGTH_JORY, m_bRockStrengthJORY);
	DDX_Check(pDX, IDC_CHECK_WEATHERING_OTHER, m_bWeatheringOther);
	DDX_Check(pDX, IDC_CHECK_WEATHERING_QIFH, m_bWeatheringQIFH);
	DDX_Check(pDX, IDC_CHECK_WEATHERING_RFH, m_bWeatheringRFH);
	DDX_Check(pDX, IDC_CHECK_WEATHERING_WFH, m_bWeatheringWFH);
	DDX_Check(pDX, IDC_CHECK_WEATHERING_QUFH, m_bWeatheringQUFH);
	DDX_Text(pDX, IDC_EDT_CRACK_STATUS_OTHER_DESC, m_strCrackStatusOtherDesc);
	DDV_MaxChars(pDX, m_strCrackStatusOtherDesc, 50);
	DDX_Text(pDX, IDC_EDT_CRACK_WIDTH_OTHER_DESC, m_strCrackWidthOtherDesc);
	DDV_MaxChars(pDX, m_strCrackWidthOtherDesc, 50);
	DDX_Text(pDX, IDC_EDT_EXCAVE_STATUS_OTHER_DESC, m_strExcaveStatusOtherDesc);
	DDV_MaxChars(pDX, m_strExcaveStatusOtherDesc, 50);
	DDX_Text(pDX, IDC_EDT_FACE_STATUS_OTHER_DESC, m_strFaceStatusOtherDesc);
	DDV_MaxChars(pDX, m_strFaceStatusOtherDesc, 50);
	DDX_Text(pDX, IDC_EDT_ROCK_STRENGTH_OTHER_DESC, m_strRockStrengthOtherDesc);
	DDV_MaxChars(pDX, m_strRockStrengthOtherDesc, 50);
	DDX_Text(pDX, IDC_EDT_WEATHERING_OTHER_DESC, m_strWeatheringOtherDesc);
	DDV_MaxChars(pDX, m_strWeatheringOtherDesc, 50);
	DDX_Text(pDX, IDC_EDT_CONSTRUCT_UNIT, m_strConstructUnit);
	DDX_Text(pDX, IDC_EDT_REC_UNIT, m_strRecUnit);
	DDX_Text(pDX, IDC_EDT_LITHOLOGY, m_strLithology);
	DDX_Text(pDX, IDC_EDT_ID, m_strID);
	DDV_MaxChars(pDX, m_strID, 20);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFaceInfoEx, CDialog)
	//{{AFX_MSG_MAP(CDlgFaceInfoEx)
	ON_BN_CLICKED(IDC_RECORD_FIRST, OnRecordFirst)
	ON_BN_CLICKED(IDC_RECORD_PREV, OnRecordPrev)
	ON_BN_CLICKED(IDC_RECORD_MODIFY, OnRecordModify)
	ON_BN_CLICKED(IDC_RECORD_NEXT, OnRecordNext)
	ON_BN_CLICKED(IDC_RECORD_LAST, OnRecordLast)
	ON_BN_CLICKED(IDC_RECORD_COPY_PREV, OnRecordCopyPrev)
	ON_EN_SETFOCUS(IDC_EDT_FACE_MILE, OnSetfocusEdtFaceMile)
	ON_EN_KILLFOCUS(IDC_EDT_FACE_MILE, OnKillfocusEdtFaceMile)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECK_FACE_STATUS_OTHER, OnCheckFaceStatusOther)
	ON_BN_CLICKED(IDC_CHECK_EXCAVE_STATUS_OTHER, OnCheckExcaveStatusOther)
	ON_BN_CLICKED(IDC_CHECK_ROCK_STRENGTH_OTHER, OnCheckRockStrengthOther)
	ON_BN_CLICKED(IDC_CHECK_WEATHERING_OTHER, OnCheckWeatheringOther)
	ON_BN_CLICKED(IDC_CHECK_CRACK_WIDTH_OTHER, OnCheckCrackWidthOther)
	ON_BN_CLICKED(IDC_CHECK_CRACK_STATUS_OTHER, OnCheckCrackStatusOther)
	ON_BN_CLICKED(IDC_RECORD_FROM_XML, OnRecordFromXml)
	ON_BN_CLICKED(IDC_RECORD_TO_XML, OnRecordToXml)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFaceInfoEx message handlers
BOOL CDlgFaceInfoEx::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// ��ȡ�ļ�
// 	ReadFaceInfoFile("D:\\test1.fif");
// 	ReadFaceInfoFile("D:\\test2.fif");

	// ����������ƺ͹�������ʼ�ղ����޸�
	m_edtTunName.SetReadOnly(TRUE);
	m_edtPartIdxName.SetReadOnly(TRUE);

	// ��������������ȷ����ť״̬
	//m_nFaceNum = 7;
	if (m_nFaceNum == 1)
	{
		// ��ֻ��һ����������Ƭʱ,���ó��޸İ�ť��������а�ť��Ч
		m_btnRecFirst.EnableWindow(FALSE);
		m_btnRecPrev.EnableWindow(FALSE);
		m_btnRecNext.EnableWindow(FALSE);
		m_btnRecLast.EnableWindow(FALSE);

		m_nFaceIdx = 0;
		SetShowedFaceInfo(*m_faceInfoLst.GetFaceInfoPtr(m_nFaceIdx));
	}
	else
	{
		// ���ж����������Ƭʱ
		// ��ȡFaceInfo����
		m_nFaceIdx = GetCanvasMgr()->GetCurCanvasIdx();
		if (m_nFaceIdx < 0)	m_nFaceIdx = 0;
		SetShowedFaceInfo(*m_faceInfoLst.GetFaceInfoPtr(m_nFaceIdx));

		// ���ð�ť״̬
		if (m_nFaceIdx == 0)
		{
			m_btnRecFirst.EnableWindow(FALSE);
			m_btnRecPrev.EnableWindow(FALSE);
			m_btnRecNext.EnableWindow(TRUE);
			m_btnRecLast.EnableWindow(TRUE);
		}
		else if (m_nFaceIdx == m_nFaceNum-1)
		{
			m_btnRecFirst.EnableWindow(TRUE);
			m_btnRecPrev.EnableWindow(TRUE);
			m_btnRecNext.EnableWindow(FALSE);
			m_btnRecLast.EnableWindow(FALSE);
		}
		else
		{
			m_btnRecFirst.EnableWindow(TRUE);
			m_btnRecPrev.EnableWindow(TRUE);
			m_btnRecNext.EnableWindow(TRUE);
			m_btnRecLast.EnableWindow(TRUE);
		}
	}

	m_btnRecModify.EnableWindow(TRUE);
	m_btnRecCopyPrev.EnableWindow(FALSE);
	
	// �������г�button��Ŀؼ���״̬ΪFALSE
	SetCtrlsStatus(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgFaceInfoEx::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	if (m_pFacePhoto != NULL)
	{
		CDC* pDC = m_stcFacePhoto.GetDC();
		CRect rect;
		m_stcFacePhoto.GetClientRect(&rect);
		m_pFacePhoto->Draw2(pDC->GetSafeHdc(), rect);
	}
	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgFaceInfoEx::OnRecordFirst() 
{
	// ��������޸�״̬������ԭ����ʾ��FaceInfo
	if (!m_btnRecModify.IsWindowEnabled())
	{
		FACEINFOCANVAS faceInfoCanvas;
		GetShowedFaceInfo(faceInfoCanvas);
		m_faceInfoLst.SaveFaceInfoToBuff(m_nFaceIdx, faceInfoCanvas);
		m_btnRecCopyPrev.EnableWindow(FALSE);
	}

	m_nFaceIdx = 0;

	// ���ø��ؼ���Ҫ��ʾ��ֵ
	SetShowedFaceInfo(*m_faceInfoLst.GetFaceInfoPtr(m_nFaceIdx));

	// ���ð�ť״̬
	m_btnRecFirst.EnableWindow(FALSE);
	m_btnRecPrev.EnableWindow(FALSE);
	m_btnRecNext.EnableWindow(TRUE);
	m_btnRecLast.EnableWindow(TRUE);
}

void CDlgFaceInfoEx::OnRecordPrev() 
{
	// ��������޸�״̬������ԭ����ʾ��FaceInfo
	if (!m_btnRecModify.IsWindowEnabled())
	{
		FACEINFOCANVAS faceInfoCanvas;
		GetShowedFaceInfo(faceInfoCanvas);
		m_faceInfoLst.SaveFaceInfoToBuff(m_nFaceIdx, faceInfoCanvas);
		if (m_nFaceIdx-1 == 0)
			m_btnRecCopyPrev.EnableWindow(FALSE);
	}

	m_nFaceIdx --;

	// ��ȡ���ؼ���Ҫ��ʾ��ֵ
	SetShowedFaceInfo(*m_faceInfoLst.GetFaceInfoPtr(m_nFaceIdx));

	// ��������ťǰ��ʾ�����һ����������Ƭ
	if (m_nFaceIdx == 0)
	{
		// ������������ʾ�ĵ�һ����Ƭ
		m_btnRecFirst.EnableWindow(FALSE);
		m_btnRecPrev.EnableWindow(FALSE);
	}

	// ���ð�ť״̬
	if (m_nFaceIdx+2 == m_nFaceNum)
	{
		m_btnRecNext.EnableWindow(TRUE);
		m_btnRecLast.EnableWindow(TRUE);
	}
}

void CDlgFaceInfoEx::OnRecordNext() 
{
	// ��������޸�״̬������ԭ����ʾ��FaceInfo
	if (!m_btnRecModify.IsWindowEnabled())
	{
		FACEINFOCANVAS faceInfoCanvas;
		GetShowedFaceInfo(faceInfoCanvas);
		m_faceInfoLst.SaveFaceInfoToBuff(m_nFaceIdx, faceInfoCanvas);
		m_btnRecCopyPrev.EnableWindow(TRUE);
	}
	
	m_nFaceIdx ++;

	// ���ø��ؼ���Ҫ��ʾ��ֵ
	SetShowedFaceInfo(*m_faceInfoLst.GetFaceInfoPtr(m_nFaceIdx));

	// ���ð�ť״̬
	if (m_nFaceIdx-1 == 0)
	{
		m_btnRecFirst.EnableWindow(TRUE);
		m_btnRecPrev.EnableWindow(TRUE);
	}

	// ���ú�������ť״̬
	if (m_nFaceIdx+1 == m_nFaceNum)
	{
		// ��������ť�������һ��������
		m_btnRecNext.EnableWindow(FALSE);
		m_btnRecLast.EnableWindow(FALSE);
	}
}

void CDlgFaceInfoEx::OnRecordLast() 
{
	// ��������޸�״̬������ԭ����ʾ��FaceInfo
	if (!m_btnRecModify.IsWindowEnabled())
	{
		FACEINFOCANVAS faceInfoCanvas;
		GetShowedFaceInfo(faceInfoCanvas);
		m_faceInfoLst.SaveFaceInfoToBuff(m_nFaceIdx, faceInfoCanvas);
		m_btnRecCopyPrev.EnableWindow(TRUE);
	}
	
	m_nFaceIdx = m_nFaceNum-1;

	// ��ȡ���ؼ���Ҫ��ʾ��ֵ
	SetShowedFaceInfo(*m_faceInfoLst.GetFaceInfoPtr(m_nFaceIdx));

	// ���ð�ť״̬
	m_btnRecFirst.EnableWindow(TRUE);
	m_btnRecPrev.EnableWindow(TRUE);
	m_btnRecNext.EnableWindow(FALSE);
	m_btnRecLast.EnableWindow(FALSE);
}

void CDlgFaceInfoEx::OnRecordModify() 
{
	// TODO: Add your control notification handler code here
	SetCtrlsStatus(TRUE);
	m_edtFaceMile.SetReadOnly(TRUE);
	m_btnRecModify.EnableWindow(FALSE);
	if (m_nFaceIdx > 0)
		m_btnRecCopyPrev.EnableWindow(TRUE);
}

void CDlgFaceInfoEx::OnRecordCopyPrev() 
{
	// TODO: Add your control notification handler code here
	SetShowedFaceInfo(*m_faceInfoLst.GetFaceInfoPtr(m_nFaceIdx-1),TRUE);
}

void CDlgFaceInfoEx::OnOK() 
{
	BeginWaitCursor();
	
	// ��������޸�״̬�������±���������������Ϣ
	if (!m_btnRecModify.IsWindowEnabled())
	{
		FACEINFOCANVAS faceInfoCanvas;
		GetShowedFaceInfo(faceInfoCanvas);
		m_faceInfoLst.SaveFaceInfoToBuff(m_nFaceIdx, faceInfoCanvas);
		m_faceInfoLst.SaveAllFaceInfoFiles(m_strPath);
	}
	
	EndWaitCursor();

	CDialog::OnOK();
}

// ���ó�CButton��ť������пؼ���״̬
void CDlgFaceInfoEx::SetCtrlsStatus(BOOL bEnable)
{
	// ��ӡ���
	m_edtID.SetReadOnly(!bEnable);

	// �������еĿؼ�˳�����ÿؼ�״̬
	m_edtRecUnit.SetReadOnly(!bEnable);
	m_edtConstructUnit.SetReadOnly(!bEnable);
	m_edtRecSupervision.SetReadOnly(!bEnable);
	// ���������ǰ׺�����ֱ������ϵͳ�е��ļ������������ڴ˴��޸�
	m_edtFaceMile.SetReadOnly(!bEnable);

	m_edtFaceWidth.SetReadOnly(!bEnable);
	m_edtFaceHeight.SetReadOnly(!bEnable);
	m_edtFaceArea.SetReadOnly(!bEnable);
	m_edtFaceDepth.SetReadOnly(!bEnable);

	m_cmbExcaveWay.EnableWindow(bEnable);
	m_cmbExcavePos.EnableWindow(bEnable);
	m_ctlDateTime.EnableWindow(bEnable);
	m_cmbExcaveDir.EnableWindow(bEnable);
	m_cmbDesignGrade.EnableWindow(bEnable);
	m_cmbConstructGrade.EnableWindow(bEnable);

	// ������״̬
	GetDlgItem(IDC_CHECK_FACE_STATUS_WD)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_FACE_STATUS_ZMDK)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_FACE_STATUS_ZMJC)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_FACE_STATUS_ZMBNZW)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_FACE_STATUS_OTHER)->EnableWindow(bEnable);
	m_edtFaceStatusOtherDesc.SetReadOnly(!(bEnable && m_bFaceStatusOther));

	// ë������״̬
	GetDlgItem(IDC_CHECK_EXCAVE_STATUS_ZW)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_EXCAVE_STATUS_SCDK)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_EXCAVE_STATUS_JSZH)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_EXCAVE_STATUS_CQZH)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_EXCAVE_STATUS_OTHER)->EnableWindow(bEnable);
	m_edtExcaveStatusOtherDesc.SetReadOnly(!(bEnable && m_bExcaveStatusOther));

	// ��ʯǿ��
	GetDlgItem(IDC_CHECK_ROCK_STRENGTH_YY)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ROCK_STRENGTH_JORY)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ROCK_STRENGTH_RY)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ROCK_STRENGTH_JIRY)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ROCK_STRENGTH_OTHER)->EnableWindow(bEnable);
	m_edtRockStrengthOtherDesc.SetReadOnly(!(bEnable && m_bRockStrengthOther));

	// �绯�̶�
	GetDlgItem(IDC_CHECK_WEATHERING_WFH)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_WEATHERING_RFH)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_WEATHERING_QIFH)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_WEATHERING_QUFH)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_WEATHERING_OTHER)->EnableWindow(bEnable);
	m_edtWeatheringOtherDesc.SetReadOnly(!(bEnable && m_bWeatheringOther));

	// ��϶���
	GetDlgItem(IDC_CHECK_CRACK_WIDTH_5)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_CRACK_WIDTH_3_5)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_CRACK_WIDTH_1_3)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_CRACK_WIDTH_1)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_CRACK_WIDTH_OTHER)->EnableWindow(bEnable);
	m_edtCrackWidthOtherDesc.SetReadOnly(!(bEnable && m_bCrackWidthOther));

	// ��϶��̬
	GetDlgItem(IDC_CHECK_CRACK_STATUS_MJ)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_CRACK_STATUS_BFZK)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_CRACK_STATUS_KK)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_CRACK_STATUS_JYLT)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_CRACK_STATUS_OTHER)->EnableWindow(bEnable);
	m_edtCrackStatusOtherDesc.SetReadOnly(!(bEnable && m_bCrackStatusOther));

	// ӿˮ״̬
	GetDlgItem(IDC_CHECK_GUSHING_STATUS_WS)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_GUSHING_STATUS_SS)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_GUSHING_STATUS_ZTSR)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_GUSHING_STATUS_YCPC)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_GUSHING_STATUS_TBD)->EnableWindow(bEnable);

	// ����������
	m_edtFaceDesc.SetReadOnly(!bEnable);
}

// ��ifo�ļ��ж�ȡ��������Ϣ
BOOL CDlgFaceInfoEx::ReadFaceInfoAndCanvas(CString strFileName, CTilCanvas* pCanvas)
{
	// ���������Ƿ��ȡ�ɹ�����������������1
	m_nFaceNum ++;

	m_strPath = get_file_path(strFileName);

	BOOL bRet = m_faceInfoLst.ReadFaceInfoAndCanvas(strFileName, pCanvas);
	
	return bRet;
}

// �������пؼ�����ʾ����Ϣ
// ��FACEINFO�����ж�ȡֵ��ʾ���ؼ���
void CDlgFaceInfoEx::SetShowedFaceInfo(FACEINFOCANVAS& faceInfo, BOOL bCopyPrev)
{
	m_strTunName = faceInfo.strTunName;
	m_strPartIdxName = faceInfo.strPartIdxName;
	m_strID = faceInfo.strID;
	m_strRecUnit = faceInfo.strRecUnit;
	m_strConstructUnit = faceInfo.strConstructUnit;
	m_strRecSupervision = faceInfo.strRecSupervision;

	// ������Ǹ�����һ����¼,��������̵���ʾ��������
	if (!bCopyPrev)
		m_strFaceMile = faceInfo.strFaceMile;
// 	if (!bCopyPrev)
// 	{
// 		m_fFaceMile = faceInfo.fFaceMile;
// 		if (m_fFaceMile < 0) // ��С��0�����ǲ�������ļ���
// 		{
// 			m_strFaceMile = m_strFaceDK;
// 		}
// 		else
// 		{
// 			int nKiloMile = int(m_fFaceMile)/1000;
// 			m_strFaceMile.Format("%d+%4.1f", nKiloMile, m_fFaceMile-nKiloMile*1000);
// 			m_strFaceMile = m_strFaceDK + m_strFaceMile;
// 		}
// 	}

	m_fFaceWidth = faceInfo.fFaceWidth;
	m_fFaceHeight = faceInfo.fFaceHeight;
	m_fFaceArea = faceInfo.fFaceArea;
	m_strExcaveWay = faceInfo.strExcaveWay;
	m_strExcavePos = faceInfo.strExcavePos;
	m_DateTime = faceInfo.dateTime;
	m_fFaceDepth = faceInfo.fFaceDepth;
	m_strExcaveDir = faceInfo.strExcaveDir;
	m_strLithology = faceInfo.strLithology;
	m_strDesignGrade = faceInfo.strDesignGrade;
	m_strConstructGrade = faceInfo.strConstructGrade;

	// ������״̬
	m_bFaceStatusWD		= faceInfo.bFaceStatusWD;
	m_bFaceStatusZMDK	= faceInfo.bFaceStatusZMDK;
	m_bFaceStatusZMJC	= faceInfo.bFaceStatusZMJC;
	m_bFaceStatusZMBNZW = faceInfo.bFaceStatusZMBNZW;
	m_bFaceStatusOther	= faceInfo.bFaceStatusOther;
	m_strFaceStatusOtherDesc = faceInfo.strFaceStatusOtherDesc;

	// ë������״̬
	m_bExcaveStatusZW	= faceInfo.bExcaveStatusZW;
	m_bExcaveStatusSCDK = faceInfo.bExcaveStatusSCDK;
	m_bExcaveStatusJSZH = faceInfo.bExcaveStatusJSZH;
	m_bExcaveStatusCQZH = faceInfo.bExcaveStatusCQZH;
	m_bExcaveStatusOther= faceInfo.bExcaveStatusOther;
	m_strExcaveStatusOtherDesc = faceInfo.strExcaveStatusOtherDesc;

	// ����ǿ��
	m_bRockStrengthYY	= faceInfo.bRockStrengthYY;
	m_bRockStrengthJORY = faceInfo.bRockStrengthJORY;
	m_bRockStrengthRY	= faceInfo.bRockStrengthRY;
	m_bRockStrengthJIRY = faceInfo.bRockStrengthJIRY;
	m_bRockStrengthOther= faceInfo.bRockStrengthOther;
	m_strRockStrengthOtherDesc = faceInfo.strRockStrengthOtherDesc;

	// �绯�̶�
	m_bWeatheringWFH	= faceInfo.bWeatheringWFH;
	m_bWeatheringRFH	= faceInfo.bWeatheringRFH;
	m_bWeatheringQIFH	= faceInfo.bWeatheringQIFH;
	m_bWeatheringQUFH	= faceInfo.bWeatheringQUFH;
	m_bWeatheringOther	= faceInfo.bWeatheringOther;
	m_strWeatheringOtherDesc = faceInfo.strWeatheringOtherDesc;

	// ��϶���
	m_bCrackWidth5		= faceInfo.bCrackWidth5;
	m_bCrackWidth3_5	= faceInfo.bCrackWidth3_5;
	m_bCrackWidth1_3	= faceInfo.bCrackWidth1_3;
	m_bCrackWidth1		= faceInfo.bCrackWidth1;
	m_bCrackWidthOther	= faceInfo.bCrackWidthOther;
	m_strCrackWidthOtherDesc = faceInfo.strCrackWidthOtherDesc;

	// ��϶��̬
	m_bCrackStatusMJ	= faceInfo.bCrackStatusMJ;
	m_bCrackStatusBFZK	= faceInfo.bCrackStatusBFZK;
	m_bCrackStatusKK	= faceInfo.bCrackStatusKK;
	m_bCrackStatusJYLT	= faceInfo.bCrackStatusJYLT;
	m_bCrackStatusOther = faceInfo.bCrackStatusOther;
	m_strCrackStatusOtherDesc = faceInfo.strCrackStatusOtherDesc;

	// ӿˮ״̬
	m_bGushingStatusWS   = faceInfo.bGushingStatusWS;
	m_bGushingStatusSS	 = faceInfo.bGushingStatusSS;
	m_bGushingStatusZTSR = faceInfo.bGushingStatusZTSR;
	m_bGushingStatusYCPC = faceInfo.bGushingStatusYCPC;
	m_bGushingStatusTBD  = faceInfo.bGushingStatusTBD;

	m_strFacePhoto = faceInfo.strFacePhoto;

	// ������Ǹ�����һ����¼������ʾ��ͼ��
	if (!bCopyPrev)
	{
		if (m_pFacePhoto != NULL)
			delete m_pFacePhoto;
		m_strFacePhoto = m_strPath + '\\' + m_strFacePhoto;
		m_pFacePhoto = new CImage(m_strFacePhoto, CXIMAGE_FORMAT_JPG);

		Invalidate(FALSE);
	}

	m_strFaceDesc = faceInfo.strFaceDesc;

	int nYear=m_DateTime.GetYear();
	int nYear1=faceInfo.dateTime.GetYear();
	m_ctlDateTime.SetTime(&m_DateTime);

	UpdateData(FALSE);
}

// ���ؼ��е�ֵ������FACEINFO������
void CDlgFaceInfoEx::GetShowedFaceInfo(FACEINFOCANVAS& faceInfo)
{
	UpdateData(TRUE);

	faceInfo.strFaceDK=m_strFaceDK;
//	faceInfo.fFaceMile=m_fFaceMile;

	faceInfo.strTunName = m_strTunName;
	faceInfo.strPartIdxName = m_strPartIdxName;
	faceInfo.strID = m_strID;
	faceInfo.strRecUnit = m_strRecUnit;
	faceInfo.strConstructUnit = m_strConstructUnit;
	faceInfo.strRecSupervision = m_strRecSupervision;
	faceInfo.strFaceMile = m_strFaceMile;
	faceInfo.fFaceWidth = m_fFaceWidth;
	faceInfo.fFaceHeight = m_fFaceHeight;
	faceInfo.fFaceArea = m_fFaceArea;
	faceInfo.strExcaveWay = m_strExcaveWay;
	faceInfo.strExcavePos = m_strExcavePos;
	faceInfo.dateTime = m_DateTime;
	faceInfo.fFaceDepth = m_fFaceDepth;
	faceInfo.strExcaveDir = m_strExcaveDir;
	faceInfo.strLithology = m_strLithology;
	faceInfo.strDesignGrade = m_strDesignGrade;
	faceInfo.strConstructGrade = m_strConstructGrade;

	// ������״̬
	faceInfo.bFaceStatusWD = m_bFaceStatusWD;
	faceInfo.bFaceStatusZMDK = m_bFaceStatusZMDK;
	faceInfo.bFaceStatusZMJC = m_bFaceStatusZMJC;
	faceInfo.bFaceStatusZMBNZW = m_bFaceStatusZMBNZW;
	faceInfo.bFaceStatusOther = m_bFaceStatusOther;
	faceInfo.strFaceStatusOtherDesc = m_strFaceStatusOtherDesc;

	// ë������״̬
	faceInfo.bExcaveStatusZW = m_bExcaveStatusZW;
	faceInfo.bExcaveStatusSCDK = m_bExcaveStatusSCDK;
	faceInfo.bExcaveStatusJSZH = m_bExcaveStatusJSZH;
	faceInfo.bExcaveStatusCQZH = m_bExcaveStatusCQZH;
	faceInfo.bExcaveStatusOther = m_bExcaveStatusOther;
	faceInfo.strExcaveStatusOtherDesc = m_strExcaveStatusOtherDesc;

	// ��ʯǿ��
	faceInfo.bRockStrengthYY = m_bRockStrengthYY;
	faceInfo.bRockStrengthJORY = m_bRockStrengthJORY;
	faceInfo.bRockStrengthRY = m_bRockStrengthRY;
	faceInfo.bRockStrengthJIRY = m_bRockStrengthJIRY;
	faceInfo.bRockStrengthOther = m_bRockStrengthOther;
	faceInfo.strRockStrengthOtherDesc = m_strRockStrengthOtherDesc;

	// �绯�̶�
	faceInfo.bWeatheringWFH = m_bWeatheringWFH;
	faceInfo.bWeatheringRFH = m_bWeatheringRFH;
	faceInfo.bWeatheringQIFH = m_bWeatheringQIFH;
	faceInfo.bWeatheringQUFH = m_bWeatheringQUFH;
	faceInfo.bWeatheringOther = m_bWeatheringOther;
	faceInfo.strWeatheringOtherDesc = m_strWeatheringOtherDesc;

	// ��϶���
	faceInfo.bCrackWidth5 = m_bCrackWidth5;
	faceInfo.bCrackWidth3_5 = m_bCrackWidth3_5;
	faceInfo.bCrackWidth1_3 = m_bCrackWidth1_3;
	faceInfo.bCrackWidth1 = m_bCrackWidth1;
	faceInfo.bCrackWidthOther = m_bCrackWidthOther;
	faceInfo.strCrackWidthOtherDesc = m_strCrackWidthOtherDesc;

	// ��϶��̬
	faceInfo.bCrackStatusMJ = m_bCrackStatusMJ;
	faceInfo.bCrackStatusBFZK = m_bCrackStatusBFZK;
	faceInfo.bCrackStatusKK = m_bCrackStatusKK;
	faceInfo.bCrackStatusJYLT = m_bCrackStatusJYLT;
	faceInfo.bCrackStatusOther = m_bCrackStatusOther;
	faceInfo.strCrackStatusOtherDesc = m_strCrackStatusOtherDesc;

	// ӿˮ״̬
	faceInfo.bGushingStatusWS = m_bGushingStatusWS;
	faceInfo.bGushingStatusSS = m_bGushingStatusSS;
	faceInfo.bGushingStatusZTSR = m_bGushingStatusZTSR;
	faceInfo.bGushingStatusYCPC = m_bGushingStatusYCPC;
	faceInfo.bGushingStatusTBD = m_bGushingStatusTBD;

	// faceInfo.strFacePhoto = m_strFacePhoto; //��������Ƭ�������¸�ֵ
	faceInfo.strFaceDesc = m_strFaceDesc;
}

// Ϊ���뽹��ʱ������������ʾ��ʽ
void CDlgFaceInfoEx::OnSetfocusEdtFaceMile() 
{
	// TODO: Add your control notification handler code here
	/*
	if (int(m_fFaceMile*10) == 0)
		m_strFaceMile = "";
	else
		m_strFaceMile.Format("%d.%d",int(m_fFaceMile),int(m_fFaceMile-int(m_fFaceMile))*10);
	UpdateData(FALSE);
	*/
}

// ʧȥ����ʱ������������ʾ��ʽ
void CDlgFaceInfoEx::OnKillfocusEdtFaceMile() 
{
	// TODO: Add your control notification handler code here
	/*
	UpdateData(TRUE);

	m_fFaceMile = atof(m_strFaceMile);
	int nKiloMile = int(m_fFaceMile)/1000;
	m_strFaceMile.Format("%d+%4.1f", nKiloMile, m_fFaceMile-nKiloMile*1000);
	UpdateData(FALSE);
	*/
}

void CDlgFaceInfoEx::OnCheckFaceStatusOther() 
{
	UpdateData(TRUE);

	((CEdit*)GetDlgItem(IDC_EDT_FACE_STATUS_OTHER))->SetReadOnly(!m_bFaceStatusOther);//EnableWindow(m_bFaceStatusOther);
}

void CDlgFaceInfoEx::OnCheckExcaveStatusOther() 
{
	UpdateData(TRUE);
	
	((CEdit*)GetDlgItem(IDC_EDT_EXCAVE_STATUS_OTHER))->SetReadOnly(!m_bExcaveStatusOther);
}

void CDlgFaceInfoEx::OnCheckRockStrengthOther() 
{
	UpdateData(TRUE);
	
	((CEdit*)GetDlgItem(IDC_EDT_ROCK_STRENGTH_OTHER))->SetReadOnly(!m_bRockStrengthOther);
}

void CDlgFaceInfoEx::OnCheckWeatheringOther() 
{
	UpdateData(TRUE);
	
	((CEdit*)GetDlgItem(IDC_EDT_WEATHERING_OTHER))->SetReadOnly(!m_bWeatheringOther);
}

void CDlgFaceInfoEx::OnCheckCrackWidthOther() 
{
	UpdateData(TRUE);
	
	((CEdit*)GetDlgItem(IDC_EDT_CRACK_WIDTH_OTHER))->SetReadOnly(!m_bCrackWidthOther);
}

void CDlgFaceInfoEx::OnCheckCrackStatusOther() 
{
	UpdateData(TRUE);
	
	((CEdit*)GetDlgItem(IDC_EDT_CRACK_STATUS_OTHER))->SetReadOnly(!m_bCrackStatusOther);
}

// ��������ȡ��߶ȡ����
void CDlgFaceInfoEx::GetPolyCaption(float& fWidth, float& fHeight, float& fArea)
{
	CTilCanvasMgr* pMgr = GetCanvasMgr();
	CGraph* pGraph = pMgr->GetCanvasByIdx(m_nFaceNum-1)->GetGraph()->GetGraphByIdx(GRAPH_LAYER_TUNARC);
	if (pGraph->Size() == 0)
	{
		fWidth = fHeight = 0;
		fArea = 0.0;
		return;
	}

	CUnit* pTunArc = pGraph->GetUnitByIdx(0);
	int nNum = pTunArc->GetPointNum();
	POINT2DF* pPoints = new POINT2DF[nNum];
	double dbLeft, dbTop, dbRight, dbBottom;
	dbLeft = dbTop = FLT_MAX;
	dbRight = dbBottom = FLT_MIN;
	for (int i=0; i<nNum; i++)
	{
		pPoints[i] = pTunArc->GetPoint(i);

		if (dbLeft > pPoints[i].x)	dbLeft = pPoints[i].x;
		if (dbRight < pPoints[i].x)	dbRight = pPoints[i].x;
		if (dbTop > pPoints[i].y)	dbTop = pPoints[i].y;
		if (dbBottom < pPoints[i].y)dbBottom = pPoints[i].y;
	}

	fWidth = int((dbRight-dbLeft)/10)/10.0;	// ��λ����
	fHeight = int((dbBottom-dbTop)/10)/10.0;	// ��λ����
	fArea = int(get_poly_area(pPoints, nNum)/1000)/10.0;	// ��λ��ƽ����

	delete [] pPoints;
}

void CDlgFaceInfoEx::OnRecordFromXml() 
{
	::CoInitialize(NULL);

	CFileDialog dlg(TRUE,"��xml�ļ���¼",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"(*.xml)|*.xml||");
	if(dlg.DoModal()==IDOK)
	{
		CString strPathName=dlg.GetPathName();
		LPCSTR str=strPathName.GetBuffer(strPathName.GetLength());
// 		FILE* file;
// 		file=fopen(str,"r+");
// 		fscanf(file,"%s\n")

		// ��ȡxml�ļ�
		MSXML2::IXMLDOMDocumentPtr pDoc;
		HRESULT hr;
		hr=pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument40));
		if(FAILED(hr))
		{  
			MessageBox("�޷�����DOMDocument���������Ƿ�װ��MS XML Parser ���п�!"); 
			return ;
		} 
		// �����ļ�
		BOOL bSuccess=pDoc->load(str);

		FACEINFOCANVAS* pFaceInfo=m_faceInfoLst.GetFaceInfoPtr(m_nFaceIdx);
		MSXML2::IXMLDOMNodePtr  pNode;
		CString strPara;

		// ��ȡ���ǰ׺
// 		pNode=pDoc->selectSingleNode("//LegendPrefix");
// 		if(pNode==NULL)
// 		{
// 			MessageBox("xml�ļ��������ǰ׺,�޷���������!");
// 			return;
// 		}
// 		strPara=(LPCSTR)pNode->Gettext();
// 		if(strPara!=pFaceInfo->strFaceDK)
// 		{
// 			MessageBox("��ǰ���������ǰ׺��xml�ļ��е����ǰ׺��ƥ��,�޷���������!");
// 			return;
// 		}
// 
// 		// ��ȡ���
// 		pNode=pDoc->selectSingleNode("//Legend");
// 		if(pNode==NULL)
// 		{
// 			MessageBox("xml�ļ������������,�޷���������!");
// 			return;
// 		}
// 		strPara=(LPCSTR)pNode->Gettext();
// 		float fPara=atof(strPara);
// 		if(fabs(fPara-pFaceInfo->fFaceMile)>=0.1)
// 		{
// 			MessageBox("��ǰ�����������xml�ļ��е���̲�ƥ��,�޷���������!");
// 			return;
// 		}

		pFaceInfo->fFaceMile=atof(pFaceInfo->strFaceMile);
		
		//�����в�����ΪTunnelId�Ľڵ�,"//"��ʾ������һ����� 
// 		pNode=pDoc->selectSingleNode("//TunnelId");
// 		pFaceInfo->hb=(LPCSTR)pNode->Gettext();	

		//��������
		pNode=pDoc->selectSingleNode("//Remark");
		if(pNode!=NULL)
		{
			strPara=(LPCTSTR)pNode->Gettext();
			pFaceInfo->strFaceDesc=strPara;
		}

		// ���ڷ���(����)
		// ����ʱ��
		pNode=pDoc->selectSingleNode("//ConstructTime");
		if(pNode!=NULL)
		{
			strPara=(LPCSTR)pNode->Gettext();
			int nYear,nMonth,nDay,nHour,nMin,nSec;
			sscanf(strPara,"%d-%d-%dT%d:%d:%dZ",&nYear,&nMonth,&nDay,&nHour,&nMin,&nSec);
			if(nYear>1000)
				pFaceInfo->dateTime=CTime(nYear,nMonth,nDay,nHour,nMin,nSec);
		}

		// ʩ������
		pNode=pDoc->selectSingleNode("//ConstructStyleName");
		if(pNode!=NULL)
			pFaceInfo->strExcaveWay=(LPCSTR)pNode->Gettext();

		// ���ڲ�λ
		pNode=pDoc->selectSingleNode("//ConstructPartName");
		if(pNode!=NULL)
			pFaceInfo->strExcavePos=(LPCSTR)pNode->Gettext();

		// ����
		pNode=pDoc->selectSingleNode("//Depth");
		if(pNode!=NULL)
			pFaceInfo->fFaceHeight=atof(pNode->Gettext());

		// ������(��������ȴ���0�������µ���Ŀ�ȣ������õ��룬���ݿ����������Զ�����)
		pNode=pDoc->selectSingleNode("//ConstructWidth");
		if(pNode!=NULL)
		{
			float fConstructWidth=atof(pNode->Gettext());
			if(fConstructWidth>0)
				pFaceInfo->fFaceWidth=fConstructWidth;
		}
		
		// ����߶�(��������ȴ���0�������µ���Ŀ�ȣ������õ��룬���ݿ����������Զ�����)
		pNode=pDoc->selectSingleNode("//ConstructHeight");
		if(pNode!=NULL)
		{
			float fConstructHeight=atof(pNode->Gettext());
			pFaceInfo->fFaceHeight=fConstructHeight;
		}
		
		// �������
		pNode=pDoc->selectSingleNode("//ConstructArea");
		if(pNode!=NULL)
			pFaceInfo->fFaceArea=atof(pNode->Gettext());

		// Χ������
		pNode=pDoc->selectSingleNode("//WallRockLithology");
		if(pNode!=NULL)
			pFaceInfo->strLithology=(LPCSTR)pNode->Gettext();
		// Χ����Ƽ���
		pNode=pDoc->selectSingleNode("//WallRockGradeByDesignName");
		if(pNode!=NULL)
			pFaceInfo->strDesignGrade=(LPCSTR)pNode->Gettext();
		// ʩ�����ü���
		pNode=pDoc->selectSingleNode("//WallRockGradeByRealName");
		if(pNode!=NULL)
			pFaceInfo->strConstructGrade=(LPCSTR)pNode->Gettext();
		// ������״̬
		pNode=pDoc->selectSingleNode("//FaceStateValues");
		if(pNode!=NULL)
		{
			strPara=(LPCSTR)pNode->Gettext();
			if(strPara=="�ȶ�")
			{
				pFaceInfo->bFaceStatusWD=TRUE;
				pFaceInfo->strFaceStatusWD=strPara;
			}
			else if(strPara=="�������")
			{
				pFaceInfo->bFaceStatusZMDK=TRUE;
				pFaceInfo->strFaceStatusZMDK=strPara;
			}
			else if(strPara=="���漷��")
			{
				pFaceInfo->bFaceStatusZMJC=TRUE;
				pFaceInfo->strFaceStatusZMJC=strPara;
			}
			else if(strPara=="���治������")
			{
				pFaceInfo->bFaceStatusZMBNZW=TRUE;
				pFaceInfo->strFaceStatusZMBNZW=strPara;
			}
		}

		// Χ�ҷ绯�̶�
		pNode=pDoc->selectSingleNode("//RockWeatheringLevelValues");
		if(pNode!=NULL)
		{
			strPara=(LPCSTR)pNode->Gettext();
			if(strPara=="΢�绯")
			{
				pFaceInfo->bWeatheringWFH=TRUE;
				pFaceInfo->strWeatheringWFH="΢�绯";
			}
			else if(strPara=="���绯")
			{
				pFaceInfo->bWeatheringRFH=TRUE;
				pFaceInfo->strWeatheringWFH="���绯";
			}
			else if(strPara=="ǿ�绯")
			{
				pFaceInfo->bWeatheringQIFH=TRUE;
				pFaceInfo->strWeatheringWFH="ǿ�绯";
			}
			else if(strPara=="ȫ�绯")
			{
				pFaceInfo->bWeatheringQUFH=TRUE;
				pFaceInfo->strWeatheringWFH="ȫ�绯";
			}
		}

		// ���Όѹǿ��
		pNode=pDoc->selectSingleNode("//RockStrengthValues");
		if(pNode!=NULL)
		{
			strPara=(LPCSTR)pNode->Gettext();
			if(!strPara.IsEmpty())
			{
				float fCompressiveStrength=atof(pNode->Gettext());
				if(fCompressiveStrength>30)
				{
					pFaceInfo->bRockStrengthYY=TRUE;
					pFaceInfo->strRockStrengthYY="30~60(Ӳ��)";
				}
				else if(fCompressiveStrength>15)
				{
					pFaceInfo->bRockStrengthJORY=TRUE;
					pFaceInfo->strRockStrengthJORY="15~30(������)";
				}
				else if(fCompressiveStrength>5)
				{
					pFaceInfo->bRockStrengthRY=TRUE;
					pFaceInfo->strRockStrengthRY="5~15(����)";
				}
				else if(fCompressiveStrength<=5)
				{
					pFaceInfo->bRockStrengthJIRY=TRUE;
					pFaceInfo->strRockStrengthJIRY="<5(������)";
				}
			}
		}

		// ë������״̬
		pNode=pDoc->selectSingleNode("//InitialConstructStateValues");
		if(pNode!=NULL)
		{
			strPara=(LPCSTR)pNode->Gettext();
			if(strPara=="����")
			{
				pFaceInfo->bExcaveStatusZW=TRUE;
				pFaceInfo->strExcaveStatusZW=strPara;
			}
			else if(strPara=="��ʱ���ɳڵ���")
			{
				pFaceInfo->bExcaveStatusSCDK=TRUE;
				pFaceInfo->strExcaveStatusSCDK=strPara;
			}
			else if(strPara=="������������ʱ֧��")
			{
				pFaceInfo->bExcaveStatusJSZH=TRUE;
				pFaceInfo->strExcaveStatusJSZH=strPara;
			}
			else if(strPara=="Ҫ��ǰ֧��")
			{
				pFaceInfo->bExcaveStatusCQZH=TRUE;
				pFaceInfo->strExcaveStatusCQZH=strPara;
			}
		}
		// ��϶״̬
		pNode=pDoc->selectSingleNode("//FissureStatusValues");
		if(pNode!=NULL)
		{
			strPara=(LPCSTR)pNode->Gettext();
			if(strPara=="�ܼ�")
			{
				pFaceInfo->bCrackStatusMJ=TRUE;
				pFaceInfo->strCrackStatusMJ=strPara;
			}
			else if(strPara=="�����ſ�")
			{
				pFaceInfo->bCrackStatusBFZK=TRUE;
				pFaceInfo->strCrackStatusBFZK=strPara;
			}
			else if(strPara=="����")
			{
				pFaceInfo->bCrackStatusKK=TRUE;
				pFaceInfo->strCrackStatusKK=strPara;
			}
			else if(strPara=="����ճ��")
			{
				pFaceInfo->bCrackStatusJYLT=TRUE;
				pFaceInfo->strCrackStatusJYLT=strPara;
			}
		}
		// ��϶���
		pNode=pDoc->selectSingleNode("//FissureWidthValues");
		if(pNode!=NULL)
		{
			strPara=(LPCSTR)pNode->Gettext();
			if(strPara==">5")
			{
				pFaceInfo->bCrackWidth5=TRUE;
				pFaceInfo->strCrackWidth5=">5";
			}
			else if(strPara=="3~5")
			{
				pFaceInfo->bCrackWidth3_5=TRUE;
				pFaceInfo->strCrackWidth3_5="3~5";
			}
			else if(strPara=="1~3")
			{
				pFaceInfo->bCrackWidth1_3=TRUE;
				pFaceInfo->strCrackWidth1_3="1~3";
			}
			else if(strPara=="<1")
			{
				pFaceInfo->bCrackWidth1=TRUE;
				pFaceInfo->strCrackWidth1="<1";
			}
		}
		// ����ˮ״̬
		pNode=pDoc->selectSingleNode("//UnderWaterStateValues");
		if(pNode!=NULL)
		{
			strPara=(LPCSTR)pNode->Gettext();
			if(strPara=="��ˮ")
			{
				pFaceInfo->bGushingStatusWS=TRUE;
				pFaceInfo->strGushingStatusWS=strPara;
			}
			else if(strPara=="��ˮ")
			{
				pFaceInfo->bGushingStatusSS=TRUE;
				pFaceInfo->strGushingStatusSS=strPara;
			}
			else if(strPara=="����ʪ��")
			{
				pFaceInfo->bGushingStatusZTSR=TRUE;
				pFaceInfo->strGushingStatusZTSR=strPara;
			}
			else if(strPara=="���������")
			{
				pFaceInfo->bGushingStatusYCPC=TRUE;
				pFaceInfo->strGushingStatusYCPC=strPara;
			}
			else if(strPara=="�ر��")
			{
				pFaceInfo->bGushingStatusTBD=TRUE;
				pFaceInfo->strGushingStatusTBD=strPara;
			}
		}

		SetShowedFaceInfo(*pFaceInfo);
	}
}

void CDlgFaceInfoEx::OnRecordToXml() 
{
	// TODO: Add your control notification handler code here
	::CoInitialize(NULL);
	
	UpdateData(TRUE);

	CDlgFaceInfoSaveXml dlg;
	if(dlg.DoModal()==IDOK)
	{
		// ��ȡxml�ļ�
		MSXML2::IXMLDOMDocumentPtr pDoc;
		HRESULT hr;
		hr=pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument40));
		if(FAILED(hr))
		{  
			MessageBox("�޷�����DOMDocument���������Ƿ�װ��MS XML Parser ���п�!"); 
			return ;
		} 
		// �����ļ�
		CString strPathName=dlg.m_strDstXmlPathFile;
		pDoc->load(strPathName.GetBuffer(strPathName.GetLength()));

		FACEINFOCANVAS faceInfoCanvas;
		GetShowedFaceInfo(faceInfoCanvas);
		m_faceInfoLst.SaveFaceInfoToBuff(m_nFaceIdx, faceInfoCanvas);
		FACEINFO* pFaceInfo=m_faceInfoLst.GetFaceInfoPtr(m_nFaceIdx);
		MSXML2::IXMLDOMNodePtr  pNode;
		CString strPara;

		// �������ǰ׺��Ϣ
// 		pNode=pDoc->selectSingleNode("//LegendPrefix");
// 		strPara=(LPCSTR)pNode->Gettext();
// 		if(strPara!=pFaceInfo->strFaceDK)
// 		{
// 			MessageBox("��ǰ���������ǰ׺��xml�ļ��е����ǰ׺��ƥ��,�޷������������!","������ʾ");
// 			return;
// 		}
// 
// 		// ���������Ϣ
// 		pNode=pDoc->selectSingleNode("//Legend");
// 		strPara=(LPCSTR)pNode->Gettext();
// 		if(atof(strPara)-pFaceInfo->fFaceMile>=0.1)
// 		{
// 			MessageBox("��ǰ�����������xml�ļ��е���̲�ƥ��,�޷������������!","������ʾ");
// 			return;
// 		}

		// ���ڷ���(����)
		// ����ʱ��
		pNode=pDoc->selectSingleNode("//ConstructTime");
		if(pNode!=NULL)
			pNode->Puttext((const char*)pFaceInfo->dateTime.Format("%Y-%m-%dT%H:%M:%SZ"));

		// ʩ������
		pNode=pDoc->selectSingleNode("//ConstructStyleName");
		if(pNode!=NULL)
			pNode->Puttext((const char*)pFaceInfo->strExcaveWay);

		// ���ڲ�λ
		pNode=pDoc->selectSingleNode("//ConstructPartName");
		if(pNode!=NULL)
			pNode->Puttext((const char*)pFaceInfo->strExcavePos);

		// ����
		pNode=pDoc->selectSingleNode("//Depth");
		if(pNode!=NULL)
		{
			strPara.Format("%.1f",pFaceInfo->fFaceDepth);
			pNode->Puttext((const char*)strPara);
		}

		// ������
		pNode=pDoc->selectSingleNode("//ConstructWidth");
		if(pNode!=NULL)
		{
			strPara.Format("%.1f",pFaceInfo->fFaceWidth);
			pNode->Puttext((const char*)strPara);
		}

		// ����߶�
		pNode=pDoc->selectSingleNode("//ConstructHeight");
		if(pNode!=NULL)
		{
			strPara.Format("%.1f",pFaceInfo->fFaceHeight);
			pNode->Puttext((const char*)strPara);
		}
		
		// �������
		pNode=pDoc->selectSingleNode("//ConstructArea");
		if(pNode!=NULL)
		{
			strPara.Format("%.1f",pFaceInfo->fFaceArea);
			pNode->Puttext((const char*)strPara);
		}

		// Χ������
		pNode=pNode->selectSingleNode("//WallRockLithology");
		if(pNode!=NULL)
			pNode->Puttext((const char*)pFaceInfo->strLithology);
		// Χ����Ƽ���
		pNode=pDoc->selectSingleNode("//WallRockGradeByDesignName");
		if(pNode!=NULL)
			pNode->Puttext((const char*)pFaceInfo->strDesignGrade);
		// ʩ�����ü���
		pNode=pDoc->selectSingleNode("//WallRockGradeByRealName");
		if(pNode!=NULL)
			pNode->Puttext((const char*)pFaceInfo->strConstructGrade);
		else
		{
			pNode=pDoc->selectSingleNode("//RockBaseInfo");
			if(pNode!=NULL)
			{
				MSXML2::IXMLDOMNodePtr pWallRockGradeByRealName=pDoc->createElement((_bstr_t)"WallRockGradeByRealName");
				pWallRockGradeByRealName->Puttext((const char*)pFaceInfo->strConstructGrade);
				pNode->appendChild(pWallRockGradeByRealName);
				pNode->appendChild(pDoc->createTextNode("\n  "));
			}
		}
		// ������״̬
		pNode=pDoc->selectSingleNode("//FaceStateValues");
		if(pNode!=NULL)
		{
			if(pFaceInfo->bFaceStatusWD)
				pNode->Puttext("�ȶ�");
			else if(pFaceInfo->bFaceStatusZMDK)
				pNode->Puttext("�������");
			else if(pFaceInfo->bFaceStatusZMJC)
				pNode->Puttext("���漷��");
			else if(pFaceInfo->bFaceStatusZMBNZW)
				pNode->Puttext("���治������");
		}
		else
		{
			pNode=pDoc->selectSingleNode("//TunnelFaceStability");
			if(pNode!=NULL)
			{
				MSXML2::IXMLDOMNodePtr pFaceStateValues=pDoc->createElement((_bstr_t)"FaceStateValues");
				if(pFaceInfo->bFaceStatusWD)
					pFaceStateValues->Puttext("�ȶ�");
				else if(pFaceInfo->bFaceStatusZMDK)
					pFaceStateValues->Puttext("�������");
				else if(pFaceInfo->bFaceStatusZMJC)
					pFaceStateValues->Puttext("���漷��");
				else if(pFaceInfo->bFaceStatusZMBNZW)
					pFaceStateValues->Puttext("���治������");
				pNode->appendChild(pFaceStateValues);
				pNode->appendChild(pDoc->createTextNode("\n  "));
			}
		}

		// Χ�ҷ绯�̶�
		pNode=pDoc->selectSingleNode("//RockWeatheringLevelValues");
		if(pNode!=NULL)
		{
			if(pFaceInfo->bWeatheringWFH)
				pNode->Puttext("΢�绯");
			else if(pFaceInfo->bWeatheringRFH)
				pNode->Puttext("���绯");
			else if(pFaceInfo->bWeatheringQIFH)
				pNode->Puttext("ǿ�绯");
			else if(pFaceInfo->bWeatheringQUFH)
				pNode->Puttext("ȫ�绯");
		}
		else
		{
			pNode=pDoc->selectSingleNode("//RockBaseInfo");
			if(pNode!=NULL)
			{
				MSXML2::IXMLDOMNodePtr pRockWeatheringLevelValues=pDoc->createElement((_bstr_t)"RockWeatheringLevelValues");
				if(pFaceInfo->bWeatheringWFH)
					pRockWeatheringLevelValues->Puttext("΢�绯");
				else if(pFaceInfo->bWeatheringRFH)
					pRockWeatheringLevelValues->Puttext("���绯");
				else if(pFaceInfo->bWeatheringQIFH)
					pRockWeatheringLevelValues->Puttext("ǿ�绯");
				else if(pFaceInfo->bWeatheringQUFH)
					pRockWeatheringLevelValues->Puttext("ȫ�绯");
				pNode->appendChild(pRockWeatheringLevelValues);
				pNode->appendChild(pDoc->createTextNode("\n  "));
			}
		}

		// ���Όѹǿ��
		pNode=pDoc->selectSingleNode("//RockStrengthValues");
		if(pNode!=NULL)
		{
			if(pFaceInfo->bRockStrengthYY)
				pNode->Puttext("30~60");
			else if(pFaceInfo->bRockStrengthJORY)
				pNode->Puttext("15~30");
			else if(pFaceInfo->bRockStrengthRY)
				pNode->Puttext("5~15");
			else if(pFaceInfo->bRockStrengthJIRY)
				pNode->Puttext("<5");
			else
				pNode->Puttext(">60");
		}
		else
		{
			pNode=pDoc->selectSingleNode("//RockBaseInfo");
			if(pNode!=NULL)
			{
				MSXML2::IXMLDOMNodePtr pRockStrengthValues=pDoc->createElement((_bstr_t)"RockStrengthValues");
				if(pFaceInfo->bRockStrengthYY)
					pRockStrengthValues->Puttext("30~60");
				else if(pFaceInfo->bRockStrengthJORY)
					pRockStrengthValues->Puttext("15~30");
				else if(pFaceInfo->bRockStrengthRY)
					pRockStrengthValues->Puttext("5~15");
				else if(pFaceInfo->bRockStrengthJIRY)
					pRockStrengthValues->Puttext("<5");
				else
					pRockStrengthValues->Puttext(">60");

				pNode->appendChild(pRockStrengthValues);
				pNode->appendChild(pDoc->createTextNode("\n  "));
			}
		}

		// ë������״̬
		pNode=pDoc->selectSingleNode("//InitialConstructStateValues");
		if(pNode!=NULL)
		{
			if(pFaceInfo->bExcaveStatusZW)
				pNode->Puttext("����");
			else if(pFaceInfo->bExcaveStatusSCDK)
				pNode->Puttext("��ʱ���ɳڵ���");
			else if(pFaceInfo->bExcaveStatusJSZH)
				pNode->Puttext("������������ʱ֧��");
			else if(pFaceInfo->bExcaveStatusCQZH)
				pNode->Puttext("�賬ǰ֧��");
		}
		else
		{
			pNode=pDoc->selectSingleNode("//TunnelFaceStability");
			if(pNode!=NULL)
			{
				MSXML2::IXMLDOMNodePtr pInitialConstructStateValues=pDoc->createElement((_bstr_t)"InitialConstructStateValues");
				if(pFaceInfo->bFaceStatusWD)
					pInitialConstructStateValues->Puttext("����");
				else if(pFaceInfo->bExcaveStatusSCDK)
					pInitialConstructStateValues->Puttext("��ʱ���ɳڵ���");
				else if(pFaceInfo->bExcaveStatusJSZH)
					pInitialConstructStateValues->Puttext("������������ʱ֧��");
				else if(pFaceInfo->bExcaveStatusCQZH)
					pInitialConstructStateValues->Puttext("�賬ǰ֧��");
				pNode->appendChild(pInitialConstructStateValues);
				pNode->appendChild(pDoc->createTextNode("\n  "));
			}
		}

		// ��϶״̬
		pNode=pDoc->selectSingleNode("//FissureStatusValues");
		if(pNode!=NULL)
		{
			if(pFaceInfo->bCrackStatusMJ)
				pNode->Puttext("�ܼ�");
			else if(pFaceInfo->bCrackStatusBFZK)
				pNode->Puttext("�����ſ�");
			else if(pFaceInfo->bCrackStatusKK)
				pNode->Puttext("����");
			else if(pFaceInfo->bCrackStatusJYLT)
				pNode->Puttext("����ճ��");
		}

		// ��϶���
		pNode=pDoc->selectSingleNode("//FissureWidthValues");
		if(pNode!=NULL)
		{
			if(pFaceInfo->bCrackWidth5)
				pNode->Puttext(">5");
			else if(pFaceInfo->bCrackWidth3_5)
				pNode->Puttext("3~5");
			else if(pFaceInfo->bCrackWidth1_3)
				pNode->Puttext("1~3");
			else if(pFaceInfo->bCrackWidth1)
				pNode->Puttext("<1");
		}
		
		// ����ˮ״̬
		pNode=pDoc->selectSingleNode("//UnderWaterState");
		if(pNode!=NULL)
		{
			if(pFaceInfo->bGushingStatusWS)
				pNode->Puttext("��ˮ");
			else if(pFaceInfo->bGushingStatusSS)
				pNode->Puttext("��ˮ");
			else if(pFaceInfo->bGushingStatusZTSR)
				pNode->Puttext("����ʪ��");
			else if(pFaceInfo->bGushingStatusYCPC)
				pNode->Puttext("���������");
			else if(pFaceInfo->bGushingStatusTBD)
				pNode->Puttext("�ر��");
		}
		else
		{
			pNode=pDoc->selectSingleNode("//TunnelFaceGroundwater");
			if(pNode!=NULL)
			{
				MSXML2::IXMLDOMNodePtr pUnderWaterState=pDoc->createElement((_bstr_t)"UnderWaterState");
				MSXML2::IXMLDOMNodePtr pUnderWaterStateValues=pDoc->createElement((_bstr_t)"UnderWaterStateValues");
				if(pFaceInfo->bGushingStatusWS)
				{
					pUnderWaterState->Puttext("��ˮ");
					pNode->appendChild(pUnderWaterState);
					pNode->appendChild(pDoc->createTextNode("\n		"));
					pUnderWaterStateValues->Puttext("��ˮ");
					pNode->appendChild(pUnderWaterStateValues);
					pNode->appendChild(pDoc->createTextNode("\n  "));
				}
				else if(pFaceInfo->bGushingStatusSS)
				{
					pUnderWaterState->Puttext("��ˮ");
					pNode->appendChild(pUnderWaterState);
					pNode->appendChild(pDoc->createTextNode("\n		"));
					pUnderWaterStateValues->Puttext("��ˮ");
					pNode->appendChild(pUnderWaterStateValues);
					pNode->appendChild(pDoc->createTextNode("\n  "));
				}
				else if(pFaceInfo->bGushingStatusZTSR)
				{
					pUnderWaterState->Puttext("����ʪ��");
					pNode->appendChild(pUnderWaterState);
					pNode->appendChild(pDoc->createTextNode("\n		"));
					pUnderWaterStateValues->Puttext("����ʪ��");
					pNode->appendChild(pUnderWaterStateValues);
					pNode->appendChild(pDoc->createTextNode("\n  "));
				}
				else if(pFaceInfo->bGushingStatusYCPC)
				{
					pUnderWaterState->Puttext("���������");
					pNode->appendChild(pUnderWaterState);
					pNode->appendChild(pDoc->createTextNode("\n		"));
					pUnderWaterStateValues->Puttext("���������");
					pNode->appendChild(pUnderWaterStateValues);
					pNode->appendChild(pDoc->createTextNode("\n  "));
				}
				else if(pFaceInfo->bGushingStatusTBD)
				{
					pUnderWaterState->Puttext("�ر��");
					pNode->appendChild(pUnderWaterState);
					pNode->appendChild(pDoc->createTextNode("\n		"));
					pUnderWaterStateValues->Puttext("�ر��");
					pNode->appendChild(pUnderWaterStateValues);
					pNode->appendChild(pDoc->createTextNode("\n  "));
				}
			}
		}

		pNode=pDoc->selectSingleNode("//Remark");
		if(pNode)
		{
			pNode->Puttext((const char*)pFaceInfo->strFaceDesc);
		}

		pDoc->save(dlg.m_strDstXmlPathFile.GetBuffer(dlg.m_strDstXmlPathFile.GetLength()));
	}
}

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
	// 掌子面数量
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
	
	// 读取文件
// 	ReadFaceInfoFile("D:\\test1.fif");
// 	ReadFaceInfoFile("D:\\test2.fif");

	// 设置隧道名称和工点名称始终不能修改
	m_edtTunName.SetReadOnly(TRUE);
	m_edtPartIdxName.SetReadOnly(TRUE);

	// 根据掌子面数量确定按钮状态
	//m_nFaceNum = 7;
	if (m_nFaceNum == 1)
	{
		// 当只有一幅掌子面照片时,设置除修改按钮以外的所有按钮无效
		m_btnRecFirst.EnableWindow(FALSE);
		m_btnRecPrev.EnableWindow(FALSE);
		m_btnRecNext.EnableWindow(FALSE);
		m_btnRecLast.EnableWindow(FALSE);

		m_nFaceIdx = 0;
		SetShowedFaceInfo(*m_faceInfoLst.GetFaceInfoPtr(m_nFaceIdx));
	}
	else
	{
		// 当有多幅掌子面照片时
		// 读取FaceInfo数据
		m_nFaceIdx = GetCanvasMgr()->GetCurCanvasIdx();
		if (m_nFaceIdx < 0)	m_nFaceIdx = 0;
		SetShowedFaceInfo(*m_faceInfoLst.GetFaceInfoPtr(m_nFaceIdx));

		// 设置按钮状态
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
	
	// 设置所有除button外的控件的状态为FALSE
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
	// 如果处于修改状态，保存原来显示的FaceInfo
	if (!m_btnRecModify.IsWindowEnabled())
	{
		FACEINFOCANVAS faceInfoCanvas;
		GetShowedFaceInfo(faceInfoCanvas);
		m_faceInfoLst.SaveFaceInfoToBuff(m_nFaceIdx, faceInfoCanvas);
		m_btnRecCopyPrev.EnableWindow(FALSE);
	}

	m_nFaceIdx = 0;

	// 设置各控件需要显示的值
	SetShowedFaceInfo(*m_faceInfoLst.GetFaceInfoPtr(m_nFaceIdx));

	// 设置按钮状态
	m_btnRecFirst.EnableWindow(FALSE);
	m_btnRecPrev.EnableWindow(FALSE);
	m_btnRecNext.EnableWindow(TRUE);
	m_btnRecLast.EnableWindow(TRUE);
}

void CDlgFaceInfoEx::OnRecordPrev() 
{
	// 如果处于修改状态，保存原来显示的FaceInfo
	if (!m_btnRecModify.IsWindowEnabled())
	{
		FACEINFOCANVAS faceInfoCanvas;
		GetShowedFaceInfo(faceInfoCanvas);
		m_faceInfoLst.SaveFaceInfoToBuff(m_nFaceIdx, faceInfoCanvas);
		if (m_nFaceIdx-1 == 0)
			m_btnRecCopyPrev.EnableWindow(FALSE);
	}

	m_nFaceIdx --;

	// 读取各控件需要显示的值
	SetShowedFaceInfo(*m_faceInfoLst.GetFaceInfoPtr(m_nFaceIdx));

	// 如果点击按钮前显示的最后一幅掌子面照片
	if (m_nFaceIdx == 0)
	{
		// 如果点击后是显示的第一幅照片
		m_btnRecFirst.EnableWindow(FALSE);
		m_btnRecPrev.EnableWindow(FALSE);
	}

	// 设置按钮状态
	if (m_nFaceIdx+2 == m_nFaceNum)
	{
		m_btnRecNext.EnableWindow(TRUE);
		m_btnRecLast.EnableWindow(TRUE);
	}
}

void CDlgFaceInfoEx::OnRecordNext() 
{
	// 如果处于修改状态，保存原来显示的FaceInfo
	if (!m_btnRecModify.IsWindowEnabled())
	{
		FACEINFOCANVAS faceInfoCanvas;
		GetShowedFaceInfo(faceInfoCanvas);
		m_faceInfoLst.SaveFaceInfoToBuff(m_nFaceIdx, faceInfoCanvas);
		m_btnRecCopyPrev.EnableWindow(TRUE);
	}
	
	m_nFaceIdx ++;

	// 设置各控件需要显示的值
	SetShowedFaceInfo(*m_faceInfoLst.GetFaceInfoPtr(m_nFaceIdx));

	// 设置按钮状态
	if (m_nFaceIdx-1 == 0)
	{
		m_btnRecFirst.EnableWindow(TRUE);
		m_btnRecPrev.EnableWindow(TRUE);
	}

	// 设置后两个按钮状态
	if (m_nFaceIdx+1 == m_nFaceNum)
	{
		// 如果点击按钮后是最后一幅掌子面
		m_btnRecNext.EnableWindow(FALSE);
		m_btnRecLast.EnableWindow(FALSE);
	}
}

void CDlgFaceInfoEx::OnRecordLast() 
{
	// 如果处于修改状态，保存原来显示的FaceInfo
	if (!m_btnRecModify.IsWindowEnabled())
	{
		FACEINFOCANVAS faceInfoCanvas;
		GetShowedFaceInfo(faceInfoCanvas);
		m_faceInfoLst.SaveFaceInfoToBuff(m_nFaceIdx, faceInfoCanvas);
		m_btnRecCopyPrev.EnableWindow(TRUE);
	}
	
	m_nFaceIdx = m_nFaceNum-1;

	// 读取各控件需要显示的值
	SetShowedFaceInfo(*m_faceInfoLst.GetFaceInfoPtr(m_nFaceIdx));

	// 设置按钮状态
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
	
	// 如果处于修改状态，则重新保存所有掌子面信息
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

// 设置除CButton按钮外的所有控件的状态
void CDlgFaceInfoEx::SetCtrlsStatus(BOOL bEnable)
{
	// 打印编号
	m_edtID.SetReadOnly(!bEnable);

	// 按界面中的控件顺序设置控件状态
	m_edtRecUnit.SetReadOnly(!bEnable);
	m_edtConstructUnit.SetReadOnly(!bEnable);
	m_edtRecSupervision.SetReadOnly(!bEnable);
	// 掌子面里程前缀及里程直接沿用系统中的文件名，不允许在此处修改
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

	// 掌子面状态
	GetDlgItem(IDC_CHECK_FACE_STATUS_WD)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_FACE_STATUS_ZMDK)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_FACE_STATUS_ZMJC)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_FACE_STATUS_ZMBNZW)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_FACE_STATUS_OTHER)->EnableWindow(bEnable);
	m_edtFaceStatusOtherDesc.SetReadOnly(!(bEnable && m_bFaceStatusOther));

	// 毛开挖面状态
	GetDlgItem(IDC_CHECK_EXCAVE_STATUS_ZW)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_EXCAVE_STATUS_SCDK)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_EXCAVE_STATUS_JSZH)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_EXCAVE_STATUS_CQZH)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_EXCAVE_STATUS_OTHER)->EnableWindow(bEnable);
	m_edtExcaveStatusOtherDesc.SetReadOnly(!(bEnable && m_bExcaveStatusOther));

	// 岩石强度
	GetDlgItem(IDC_CHECK_ROCK_STRENGTH_YY)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ROCK_STRENGTH_JORY)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ROCK_STRENGTH_RY)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ROCK_STRENGTH_JIRY)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ROCK_STRENGTH_OTHER)->EnableWindow(bEnable);
	m_edtRockStrengthOtherDesc.SetReadOnly(!(bEnable && m_bRockStrengthOther));

	// 风化程度
	GetDlgItem(IDC_CHECK_WEATHERING_WFH)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_WEATHERING_RFH)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_WEATHERING_QIFH)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_WEATHERING_QUFH)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_WEATHERING_OTHER)->EnableWindow(bEnable);
	m_edtWeatheringOtherDesc.SetReadOnly(!(bEnable && m_bWeatheringOther));

	// 裂隙宽度
	GetDlgItem(IDC_CHECK_CRACK_WIDTH_5)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_CRACK_WIDTH_3_5)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_CRACK_WIDTH_1_3)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_CRACK_WIDTH_1)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_CRACK_WIDTH_OTHER)->EnableWindow(bEnable);
	m_edtCrackWidthOtherDesc.SetReadOnly(!(bEnable && m_bCrackWidthOther));

	// 裂隙形态
	GetDlgItem(IDC_CHECK_CRACK_STATUS_MJ)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_CRACK_STATUS_BFZK)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_CRACK_STATUS_KK)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_CRACK_STATUS_JYLT)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_CRACK_STATUS_OTHER)->EnableWindow(bEnable);
	m_edtCrackStatusOtherDesc.SetReadOnly(!(bEnable && m_bCrackStatusOther));

	// 涌水状态
	GetDlgItem(IDC_CHECK_GUSHING_STATUS_WS)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_GUSHING_STATUS_SS)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_GUSHING_STATUS_ZTSR)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_GUSHING_STATUS_YCPC)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_GUSHING_STATUS_TBD)->EnableWindow(bEnable);

	// 掌子面描述
	m_edtFaceDesc.SetReadOnly(!bEnable);
}

// 从ifo文件中读取掌子面信息
BOOL CDlgFaceInfoEx::ReadFaceInfoAndCanvas(CString strFileName, CTilCanvas* pCanvas)
{
	// 不管正面是否读取成功，掌子面数量均加1
	m_nFaceNum ++;

	m_strPath = get_file_path(strFileName);

	BOOL bRet = m_faceInfoLst.ReadFaceInfoAndCanvas(strFileName, pCanvas);
	
	return bRet;
}

// 保存所有控件中显示的信息
// 从FACEINFO对象中读取值显示到控件中
void CDlgFaceInfoEx::SetShowedFaceInfo(FACEINFOCANVAS& faceInfo, BOOL bCopyPrev)
{
	m_strTunName = faceInfo.strTunName;
	m_strPartIdxName = faceInfo.strPartIdxName;
	m_strID = faceInfo.strID;
	m_strRecUnit = faceInfo.strRecUnit;
	m_strConstructUnit = faceInfo.strConstructUnit;
	m_strRecSupervision = faceInfo.strRecSupervision;

	// 如果不是复制上一条记录,掌子面里程的显示单独处理
	if (!bCopyPrev)
		m_strFaceMile = faceInfo.strFaceMile;
// 	if (!bCopyPrev)
// 	{
// 		m_fFaceMile = faceInfo.fFaceMile;
// 		if (m_fFaceMile < 0) // 若小于0，则是不规则的文件名
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

	// 掌子面状态
	m_bFaceStatusWD		= faceInfo.bFaceStatusWD;
	m_bFaceStatusZMDK	= faceInfo.bFaceStatusZMDK;
	m_bFaceStatusZMJC	= faceInfo.bFaceStatusZMJC;
	m_bFaceStatusZMBNZW = faceInfo.bFaceStatusZMBNZW;
	m_bFaceStatusOther	= faceInfo.bFaceStatusOther;
	m_strFaceStatusOtherDesc = faceInfo.strFaceStatusOtherDesc;

	// 毛开挖面状态
	m_bExcaveStatusZW	= faceInfo.bExcaveStatusZW;
	m_bExcaveStatusSCDK = faceInfo.bExcaveStatusSCDK;
	m_bExcaveStatusJSZH = faceInfo.bExcaveStatusJSZH;
	m_bExcaveStatusCQZH = faceInfo.bExcaveStatusCQZH;
	m_bExcaveStatusOther= faceInfo.bExcaveStatusOther;
	m_strExcaveStatusOtherDesc = faceInfo.strExcaveStatusOtherDesc;

	// 岩体强度
	m_bRockStrengthYY	= faceInfo.bRockStrengthYY;
	m_bRockStrengthJORY = faceInfo.bRockStrengthJORY;
	m_bRockStrengthRY	= faceInfo.bRockStrengthRY;
	m_bRockStrengthJIRY = faceInfo.bRockStrengthJIRY;
	m_bRockStrengthOther= faceInfo.bRockStrengthOther;
	m_strRockStrengthOtherDesc = faceInfo.strRockStrengthOtherDesc;

	// 风化程度
	m_bWeatheringWFH	= faceInfo.bWeatheringWFH;
	m_bWeatheringRFH	= faceInfo.bWeatheringRFH;
	m_bWeatheringQIFH	= faceInfo.bWeatheringQIFH;
	m_bWeatheringQUFH	= faceInfo.bWeatheringQUFH;
	m_bWeatheringOther	= faceInfo.bWeatheringOther;
	m_strWeatheringOtherDesc = faceInfo.strWeatheringOtherDesc;

	// 裂隙宽度
	m_bCrackWidth5		= faceInfo.bCrackWidth5;
	m_bCrackWidth3_5	= faceInfo.bCrackWidth3_5;
	m_bCrackWidth1_3	= faceInfo.bCrackWidth1_3;
	m_bCrackWidth1		= faceInfo.bCrackWidth1;
	m_bCrackWidthOther	= faceInfo.bCrackWidthOther;
	m_strCrackWidthOtherDesc = faceInfo.strCrackWidthOtherDesc;

	// 裂隙形态
	m_bCrackStatusMJ	= faceInfo.bCrackStatusMJ;
	m_bCrackStatusBFZK	= faceInfo.bCrackStatusBFZK;
	m_bCrackStatusKK	= faceInfo.bCrackStatusKK;
	m_bCrackStatusJYLT	= faceInfo.bCrackStatusJYLT;
	m_bCrackStatusOther = faceInfo.bCrackStatusOther;
	m_strCrackStatusOtherDesc = faceInfo.strCrackStatusOtherDesc;

	// 涌水状态
	m_bGushingStatusWS   = faceInfo.bGushingStatusWS;
	m_bGushingStatusSS	 = faceInfo.bGushingStatusSS;
	m_bGushingStatusZTSR = faceInfo.bGushingStatusZTSR;
	m_bGushingStatusYCPC = faceInfo.bGushingStatusYCPC;
	m_bGushingStatusTBD  = faceInfo.bGushingStatusTBD;

	m_strFacePhoto = faceInfo.strFacePhoto;

	// 如果不是复制上一条记录，就显示新图像
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

// 将控件中的值保存在FACEINFO对象中
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

	// 掌子面状态
	faceInfo.bFaceStatusWD = m_bFaceStatusWD;
	faceInfo.bFaceStatusZMDK = m_bFaceStatusZMDK;
	faceInfo.bFaceStatusZMJC = m_bFaceStatusZMJC;
	faceInfo.bFaceStatusZMBNZW = m_bFaceStatusZMBNZW;
	faceInfo.bFaceStatusOther = m_bFaceStatusOther;
	faceInfo.strFaceStatusOtherDesc = m_strFaceStatusOtherDesc;

	// 毛开挖面状态
	faceInfo.bExcaveStatusZW = m_bExcaveStatusZW;
	faceInfo.bExcaveStatusSCDK = m_bExcaveStatusSCDK;
	faceInfo.bExcaveStatusJSZH = m_bExcaveStatusJSZH;
	faceInfo.bExcaveStatusCQZH = m_bExcaveStatusCQZH;
	faceInfo.bExcaveStatusOther = m_bExcaveStatusOther;
	faceInfo.strExcaveStatusOtherDesc = m_strExcaveStatusOtherDesc;

	// 岩石强度
	faceInfo.bRockStrengthYY = m_bRockStrengthYY;
	faceInfo.bRockStrengthJORY = m_bRockStrengthJORY;
	faceInfo.bRockStrengthRY = m_bRockStrengthRY;
	faceInfo.bRockStrengthJIRY = m_bRockStrengthJIRY;
	faceInfo.bRockStrengthOther = m_bRockStrengthOther;
	faceInfo.strRockStrengthOtherDesc = m_strRockStrengthOtherDesc;

	// 风化程度
	faceInfo.bWeatheringWFH = m_bWeatheringWFH;
	faceInfo.bWeatheringRFH = m_bWeatheringRFH;
	faceInfo.bWeatheringQIFH = m_bWeatheringQIFH;
	faceInfo.bWeatheringQUFH = m_bWeatheringQUFH;
	faceInfo.bWeatheringOther = m_bWeatheringOther;
	faceInfo.strWeatheringOtherDesc = m_strWeatheringOtherDesc;

	// 裂隙宽度
	faceInfo.bCrackWidth5 = m_bCrackWidth5;
	faceInfo.bCrackWidth3_5 = m_bCrackWidth3_5;
	faceInfo.bCrackWidth1_3 = m_bCrackWidth1_3;
	faceInfo.bCrackWidth1 = m_bCrackWidth1;
	faceInfo.bCrackWidthOther = m_bCrackWidthOther;
	faceInfo.strCrackWidthOtherDesc = m_strCrackWidthOtherDesc;

	// 裂隙形态
	faceInfo.bCrackStatusMJ = m_bCrackStatusMJ;
	faceInfo.bCrackStatusBFZK = m_bCrackStatusBFZK;
	faceInfo.bCrackStatusKK = m_bCrackStatusKK;
	faceInfo.bCrackStatusJYLT = m_bCrackStatusJYLT;
	faceInfo.bCrackStatusOther = m_bCrackStatusOther;
	faceInfo.strCrackStatusOtherDesc = m_strCrackStatusOtherDesc;

	// 涌水状态
	faceInfo.bGushingStatusWS = m_bGushingStatusWS;
	faceInfo.bGushingStatusSS = m_bGushingStatusSS;
	faceInfo.bGushingStatusZTSR = m_bGushingStatusZTSR;
	faceInfo.bGushingStatusYCPC = m_bGushingStatusYCPC;
	faceInfo.bGushingStatusTBD = m_bGushingStatusTBD;

	// faceInfo.strFacePhoto = m_strFacePhoto; //掌子面照片不能重新赋值
	faceInfo.strFaceDesc = m_strFaceDesc;
}

// 为输入焦点时，设置数据显示格式
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

// 失去焦点时，设置数据显示格式
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

// 获得区域宽度、高度、面积
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

	fWidth = int((dbRight-dbLeft)/10)/10.0;	// 单位：米
	fHeight = int((dbBottom-dbTop)/10)/10.0;	// 单位：米
	fArea = int(get_poly_area(pPoints, nNum)/1000)/10.0;	// 单位：平方米

	delete [] pPoints;
}

void CDlgFaceInfoEx::OnRecordFromXml() 
{
	::CoInitialize(NULL);

	CFileDialog dlg(TRUE,"打开xml文件记录",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"(*.xml)|*.xml||");
	if(dlg.DoModal()==IDOK)
	{
		CString strPathName=dlg.GetPathName();
		LPCSTR str=strPathName.GetBuffer(strPathName.GetLength());
// 		FILE* file;
// 		file=fopen(str,"r+");
// 		fscanf(file,"%s\n")

		// 读取xml文件
		MSXML2::IXMLDOMDocumentPtr pDoc;
		HRESULT hr;
		hr=pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument40));
		if(FAILED(hr))
		{  
			MessageBox("无法创建DOMDocument对象，请检查是否安装了MS XML Parser 运行库!"); 
			return ;
		} 
		// 加载文件
		BOOL bSuccess=pDoc->load(str);

		FACEINFOCANVAS* pFaceInfo=m_faceInfoLst.GetFaceInfoPtr(m_nFaceIdx);
		MSXML2::IXMLDOMNodePtr  pNode;
		CString strPara;

		// 读取里程前缀
// 		pNode=pDoc->selectSingleNode("//LegendPrefix");
// 		if(pNode==NULL)
// 		{
// 			MessageBox("xml文件中无里程前缀,无法导入数据!");
// 			return;
// 		}
// 		strPara=(LPCSTR)pNode->Gettext();
// 		if(strPara!=pFaceInfo->strFaceDK)
// 		{
// 			MessageBox("当前掌子面里程前缀与xml文件中的里程前缀不匹配,无法导入数据!");
// 			return;
// 		}
// 
// 		// 读取里程
// 		pNode=pDoc->selectSingleNode("//Legend");
// 		if(pNode==NULL)
// 		{
// 			MessageBox("xml文件中无里程数据,无法导入数据!");
// 			return;
// 		}
// 		strPara=(LPCSTR)pNode->Gettext();
// 		float fPara=atof(strPara);
// 		if(fabs(fPara-pFaceInfo->fFaceMile)>=0.1)
// 		{
// 			MessageBox("当前掌子面里程与xml文件中的里程不匹配,无法导入数据!");
// 			return;
// 		}

		pFaceInfo->fFaceMile=atof(pFaceInfo->strFaceMile);
		
		//在树中查找名为TunnelId的节点,"//"表示在任意一层查找 
// 		pNode=pDoc->selectSingleNode("//TunnelId");
// 		pFaceInfo->hb=(LPCSTR)pNode->Gettext();	

		//岩体描述
		pNode=pDoc->selectSingleNode("//Remark");
		if(pNode!=NULL)
		{
			strPara=(LPCTSTR)pNode->Gettext();
			pFaceInfo->strFaceDesc=strPara;
		}

		// 开挖方向(暂无)
		// 开挖时间
		pNode=pDoc->selectSingleNode("//ConstructTime");
		if(pNode!=NULL)
		{
			strPara=(LPCSTR)pNode->Gettext();
			int nYear,nMonth,nDay,nHour,nMin,nSec;
			sscanf(strPara,"%d-%d-%dT%d:%d:%dZ",&nYear,&nMonth,&nDay,&nHour,&nMin,&nSec);
			if(nYear>1000)
				pFaceInfo->dateTime=CTime(nYear,nMonth,nDay,nHour,nMin,nSec);
		}

		// 施工工法
		pNode=pDoc->selectSingleNode("//ConstructStyleName");
		if(pNode!=NULL)
			pFaceInfo->strExcaveWay=(LPCSTR)pNode->Gettext();

		// 开挖部位
		pNode=pDoc->selectSingleNode("//ConstructPartName");
		if(pNode!=NULL)
			pFaceInfo->strExcavePos=(LPCSTR)pNode->Gettext();

		// 埋深
		pNode=pDoc->selectSingleNode("//Depth");
		if(pNode!=NULL)
			pFaceInfo->fFaceHeight=atof(pNode->Gettext());

		// 断面宽度(如果导入宽度大于0，则用新导入的宽度，否则不用导入，根据开挖轮廓线自动生成)
		pNode=pDoc->selectSingleNode("//ConstructWidth");
		if(pNode!=NULL)
		{
			float fConstructWidth=atof(pNode->Gettext());
			if(fConstructWidth>0)
				pFaceInfo->fFaceWidth=fConstructWidth;
		}
		
		// 断面高度(如果导入宽度大于0，则用新导入的宽度，否则不用导入，根据开挖轮廓线自动生成)
		pNode=pDoc->selectSingleNode("//ConstructHeight");
		if(pNode!=NULL)
		{
			float fConstructHeight=atof(pNode->Gettext());
			pFaceInfo->fFaceHeight=fConstructHeight;
		}
		
		// 开挖面积
		pNode=pDoc->selectSingleNode("//ConstructArea");
		if(pNode!=NULL)
			pFaceInfo->fFaceArea=atof(pNode->Gettext());

		// 围岩岩性
		pNode=pDoc->selectSingleNode("//WallRockLithology");
		if(pNode!=NULL)
			pFaceInfo->strLithology=(LPCSTR)pNode->Gettext();
		// 围岩设计级别
		pNode=pDoc->selectSingleNode("//WallRockGradeByDesignName");
		if(pNode!=NULL)
			pFaceInfo->strDesignGrade=(LPCSTR)pNode->Gettext();
		// 施工采用级别
		pNode=pDoc->selectSingleNode("//WallRockGradeByRealName");
		if(pNode!=NULL)
			pFaceInfo->strConstructGrade=(LPCSTR)pNode->Gettext();
		// 掌子面状态
		pNode=pDoc->selectSingleNode("//FaceStateValues");
		if(pNode!=NULL)
		{
			strPara=(LPCSTR)pNode->Gettext();
			if(strPara=="稳定")
			{
				pFaceInfo->bFaceStatusWD=TRUE;
				pFaceInfo->strFaceStatusWD=strPara;
			}
			else if(strPara=="正面掉块")
			{
				pFaceInfo->bFaceStatusZMDK=TRUE;
				pFaceInfo->strFaceStatusZMDK=strPara;
			}
			else if(strPara=="正面挤出")
			{
				pFaceInfo->bFaceStatusZMJC=TRUE;
				pFaceInfo->strFaceStatusZMJC=strPara;
			}
			else if(strPara=="正面不能自稳")
			{
				pFaceInfo->bFaceStatusZMBNZW=TRUE;
				pFaceInfo->strFaceStatusZMBNZW=strPara;
			}
		}

		// 围岩风化程度
		pNode=pDoc->selectSingleNode("//RockWeatheringLevelValues");
		if(pNode!=NULL)
		{
			strPara=(LPCSTR)pNode->Gettext();
			if(strPara=="微风化")
			{
				pFaceInfo->bWeatheringWFH=TRUE;
				pFaceInfo->strWeatheringWFH="微风化";
			}
			else if(strPara=="弱风化")
			{
				pFaceInfo->bWeatheringRFH=TRUE;
				pFaceInfo->strWeatheringWFH="弱风化";
			}
			else if(strPara=="强风化")
			{
				pFaceInfo->bWeatheringQIFH=TRUE;
				pFaceInfo->strWeatheringWFH="强风化";
			}
			else if(strPara=="全风化")
			{
				pFaceInfo->bWeatheringQUFH=TRUE;
				pFaceInfo->strWeatheringWFH="全风化";
			}
		}

		// 单轴抗压强度
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
					pFaceInfo->strRockStrengthYY="30~60(硬岩)";
				}
				else if(fCompressiveStrength>15)
				{
					pFaceInfo->bRockStrengthJORY=TRUE;
					pFaceInfo->strRockStrengthJORY="15~30(较软岩)";
				}
				else if(fCompressiveStrength>5)
				{
					pFaceInfo->bRockStrengthRY=TRUE;
					pFaceInfo->strRockStrengthRY="5~15(软岩)";
				}
				else if(fCompressiveStrength<=5)
				{
					pFaceInfo->bRockStrengthJIRY=TRUE;
					pFaceInfo->strRockStrengthJIRY="<5(极软岩)";
				}
			}
		}

		// 毛开挖面状态
		pNode=pDoc->selectSingleNode("//InitialConstructStateValues");
		if(pNode!=NULL)
		{
			strPara=(LPCSTR)pNode->Gettext();
			if(strPara=="自稳")
			{
				pFaceInfo->bExcaveStatusZW=TRUE;
				pFaceInfo->strExcaveStatusZW=strPara;
			}
			else if(strPara=="随时间松弛掉块")
			{
				pFaceInfo->bExcaveStatusSCDK=TRUE;
				pFaceInfo->strExcaveStatusSCDK=strPara;
			}
			else if(strPara=="自稳困难需随时支护")
			{
				pFaceInfo->bExcaveStatusJSZH=TRUE;
				pFaceInfo->strExcaveStatusJSZH=strPara;
			}
			else if(strPara=="要超前支护")
			{
				pFaceInfo->bExcaveStatusCQZH=TRUE;
				pFaceInfo->strExcaveStatusCQZH=strPara;
			}
		}
		// 裂隙状态
		pNode=pDoc->selectSingleNode("//FissureStatusValues");
		if(pNode!=NULL)
		{
			strPara=(LPCSTR)pNode->Gettext();
			if(strPara=="密集")
			{
				pFaceInfo->bCrackStatusMJ=TRUE;
				pFaceInfo->strCrackStatusMJ=strPara;
			}
			else if(strPara=="部分张开")
			{
				pFaceInfo->bCrackStatusBFZK=TRUE;
				pFaceInfo->strCrackStatusBFZK=strPara;
			}
			else if(strPara=="开口")
			{
				pFaceInfo->bCrackStatusKK=TRUE;
				pFaceInfo->strCrackStatusKK=strPara;
			}
			else if(strPara=="夹有粘土")
			{
				pFaceInfo->bCrackStatusJYLT=TRUE;
				pFaceInfo->strCrackStatusJYLT=strPara;
			}
		}
		// 裂隙宽度
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
		// 地下水状态
		pNode=pDoc->selectSingleNode("//UnderWaterStateValues");
		if(pNode!=NULL)
		{
			strPara=(LPCSTR)pNode->Gettext();
			if(strPara=="无水")
			{
				pFaceInfo->bGushingStatusWS=TRUE;
				pFaceInfo->strGushingStatusWS=strPara;
			}
			else if(strPara=="渗水")
			{
				pFaceInfo->bGushingStatusSS=TRUE;
				pFaceInfo->strGushingStatusSS=strPara;
			}
			else if(strPara=="整体湿润")
			{
				pFaceInfo->bGushingStatusZTSR=TRUE;
				pFaceInfo->strGushingStatusZTSR=strPara;
			}
			else if(strPara=="渗出或喷出")
			{
				pFaceInfo->bGushingStatusYCPC=TRUE;
				pFaceInfo->strGushingStatusYCPC=strPara;
			}
			else if(strPara=="特别大")
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
		// 读取xml文件
		MSXML2::IXMLDOMDocumentPtr pDoc;
		HRESULT hr;
		hr=pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument40));
		if(FAILED(hr))
		{  
			MessageBox("无法创建DOMDocument对象，请检查是否安装了MS XML Parser 运行库!"); 
			return ;
		} 
		// 加载文件
		CString strPathName=dlg.m_strDstXmlPathFile;
		pDoc->load(strPathName.GetBuffer(strPathName.GetLength()));

		FACEINFOCANVAS faceInfoCanvas;
		GetShowedFaceInfo(faceInfoCanvas);
		m_faceInfoLst.SaveFaceInfoToBuff(m_nFaceIdx, faceInfoCanvas);
		FACEINFO* pFaceInfo=m_faceInfoLst.GetFaceInfoPtr(m_nFaceIdx);
		MSXML2::IXMLDOMNodePtr  pNode;
		CString strPara;

		// 保存里程前缀信息
// 		pNode=pDoc->selectSingleNode("//LegendPrefix");
// 		strPara=(LPCSTR)pNode->Gettext();
// 		if(strPara!=pFaceInfo->strFaceDK)
// 		{
// 			MessageBox("当前掌子面里程前缀与xml文件中的里程前缀不匹配,无法保存更新数据!","错误提示");
// 			return;
// 		}
// 
// 		// 保存里程信息
// 		pNode=pDoc->selectSingleNode("//Legend");
// 		strPara=(LPCSTR)pNode->Gettext();
// 		if(atof(strPara)-pFaceInfo->fFaceMile>=0.1)
// 		{
// 			MessageBox("当前掌子面里程与xml文件中的里程不匹配,无法保存更新数据!","错误提示");
// 			return;
// 		}

		// 开挖方向(暂无)
		// 开挖时间
		pNode=pDoc->selectSingleNode("//ConstructTime");
		if(pNode!=NULL)
			pNode->Puttext((const char*)pFaceInfo->dateTime.Format("%Y-%m-%dT%H:%M:%SZ"));

		// 施工工法
		pNode=pDoc->selectSingleNode("//ConstructStyleName");
		if(pNode!=NULL)
			pNode->Puttext((const char*)pFaceInfo->strExcaveWay);

		// 开挖部位
		pNode=pDoc->selectSingleNode("//ConstructPartName");
		if(pNode!=NULL)
			pNode->Puttext((const char*)pFaceInfo->strExcavePos);

		// 埋深
		pNode=pDoc->selectSingleNode("//Depth");
		if(pNode!=NULL)
		{
			strPara.Format("%.1f",pFaceInfo->fFaceDepth);
			pNode->Puttext((const char*)strPara);
		}

		// 断面宽度
		pNode=pDoc->selectSingleNode("//ConstructWidth");
		if(pNode!=NULL)
		{
			strPara.Format("%.1f",pFaceInfo->fFaceWidth);
			pNode->Puttext((const char*)strPara);
		}

		// 断面高度
		pNode=pDoc->selectSingleNode("//ConstructHeight");
		if(pNode!=NULL)
		{
			strPara.Format("%.1f",pFaceInfo->fFaceHeight);
			pNode->Puttext((const char*)strPara);
		}
		
		// 开挖面积
		pNode=pDoc->selectSingleNode("//ConstructArea");
		if(pNode!=NULL)
		{
			strPara.Format("%.1f",pFaceInfo->fFaceArea);
			pNode->Puttext((const char*)strPara);
		}

		// 围岩岩性
		pNode=pNode->selectSingleNode("//WallRockLithology");
		if(pNode!=NULL)
			pNode->Puttext((const char*)pFaceInfo->strLithology);
		// 围岩设计级别
		pNode=pDoc->selectSingleNode("//WallRockGradeByDesignName");
		if(pNode!=NULL)
			pNode->Puttext((const char*)pFaceInfo->strDesignGrade);
		// 施工采用级别
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
		// 掌子面状态
		pNode=pDoc->selectSingleNode("//FaceStateValues");
		if(pNode!=NULL)
		{
			if(pFaceInfo->bFaceStatusWD)
				pNode->Puttext("稳定");
			else if(pFaceInfo->bFaceStatusZMDK)
				pNode->Puttext("正面掉块");
			else if(pFaceInfo->bFaceStatusZMJC)
				pNode->Puttext("正面挤出");
			else if(pFaceInfo->bFaceStatusZMBNZW)
				pNode->Puttext("正面不能自稳");
		}
		else
		{
			pNode=pDoc->selectSingleNode("//TunnelFaceStability");
			if(pNode!=NULL)
			{
				MSXML2::IXMLDOMNodePtr pFaceStateValues=pDoc->createElement((_bstr_t)"FaceStateValues");
				if(pFaceInfo->bFaceStatusWD)
					pFaceStateValues->Puttext("稳定");
				else if(pFaceInfo->bFaceStatusZMDK)
					pFaceStateValues->Puttext("正面掉块");
				else if(pFaceInfo->bFaceStatusZMJC)
					pFaceStateValues->Puttext("正面挤出");
				else if(pFaceInfo->bFaceStatusZMBNZW)
					pFaceStateValues->Puttext("正面不能自稳");
				pNode->appendChild(pFaceStateValues);
				pNode->appendChild(pDoc->createTextNode("\n  "));
			}
		}

		// 围岩风化程度
		pNode=pDoc->selectSingleNode("//RockWeatheringLevelValues");
		if(pNode!=NULL)
		{
			if(pFaceInfo->bWeatheringWFH)
				pNode->Puttext("微风化");
			else if(pFaceInfo->bWeatheringRFH)
				pNode->Puttext("弱风化");
			else if(pFaceInfo->bWeatheringQIFH)
				pNode->Puttext("强风化");
			else if(pFaceInfo->bWeatheringQUFH)
				pNode->Puttext("全风化");
		}
		else
		{
			pNode=pDoc->selectSingleNode("//RockBaseInfo");
			if(pNode!=NULL)
			{
				MSXML2::IXMLDOMNodePtr pRockWeatheringLevelValues=pDoc->createElement((_bstr_t)"RockWeatheringLevelValues");
				if(pFaceInfo->bWeatheringWFH)
					pRockWeatheringLevelValues->Puttext("微风化");
				else if(pFaceInfo->bWeatheringRFH)
					pRockWeatheringLevelValues->Puttext("弱风化");
				else if(pFaceInfo->bWeatheringQIFH)
					pRockWeatheringLevelValues->Puttext("强风化");
				else if(pFaceInfo->bWeatheringQUFH)
					pRockWeatheringLevelValues->Puttext("全风化");
				pNode->appendChild(pRockWeatheringLevelValues);
				pNode->appendChild(pDoc->createTextNode("\n  "));
			}
		}

		// 单轴抗压强度
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

		// 毛开挖面状态
		pNode=pDoc->selectSingleNode("//InitialConstructStateValues");
		if(pNode!=NULL)
		{
			if(pFaceInfo->bExcaveStatusZW)
				pNode->Puttext("自稳");
			else if(pFaceInfo->bExcaveStatusSCDK)
				pNode->Puttext("随时间松弛掉块");
			else if(pFaceInfo->bExcaveStatusJSZH)
				pNode->Puttext("自稳困难需随时支护");
			else if(pFaceInfo->bExcaveStatusCQZH)
				pNode->Puttext("需超前支护");
		}
		else
		{
			pNode=pDoc->selectSingleNode("//TunnelFaceStability");
			if(pNode!=NULL)
			{
				MSXML2::IXMLDOMNodePtr pInitialConstructStateValues=pDoc->createElement((_bstr_t)"InitialConstructStateValues");
				if(pFaceInfo->bFaceStatusWD)
					pInitialConstructStateValues->Puttext("自稳");
				else if(pFaceInfo->bExcaveStatusSCDK)
					pInitialConstructStateValues->Puttext("随时间松弛掉块");
				else if(pFaceInfo->bExcaveStatusJSZH)
					pInitialConstructStateValues->Puttext("自稳困难需随时支护");
				else if(pFaceInfo->bExcaveStatusCQZH)
					pInitialConstructStateValues->Puttext("需超前支护");
				pNode->appendChild(pInitialConstructStateValues);
				pNode->appendChild(pDoc->createTextNode("\n  "));
			}
		}

		// 裂隙状态
		pNode=pDoc->selectSingleNode("//FissureStatusValues");
		if(pNode!=NULL)
		{
			if(pFaceInfo->bCrackStatusMJ)
				pNode->Puttext("密集");
			else if(pFaceInfo->bCrackStatusBFZK)
				pNode->Puttext("部分张开");
			else if(pFaceInfo->bCrackStatusKK)
				pNode->Puttext("开口");
			else if(pFaceInfo->bCrackStatusJYLT)
				pNode->Puttext("夹有粘土");
		}

		// 裂隙宽度
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
		
		// 地下水状态
		pNode=pDoc->selectSingleNode("//UnderWaterState");
		if(pNode!=NULL)
		{
			if(pFaceInfo->bGushingStatusWS)
				pNode->Puttext("无水");
			else if(pFaceInfo->bGushingStatusSS)
				pNode->Puttext("渗水");
			else if(pFaceInfo->bGushingStatusZTSR)
				pNode->Puttext("整体湿润");
			else if(pFaceInfo->bGushingStatusYCPC)
				pNode->Puttext("渗出或喷出");
			else if(pFaceInfo->bGushingStatusTBD)
				pNode->Puttext("特别大");
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
					pUnderWaterState->Puttext("无水");
					pNode->appendChild(pUnderWaterState);
					pNode->appendChild(pDoc->createTextNode("\n		"));
					pUnderWaterStateValues->Puttext("无水");
					pNode->appendChild(pUnderWaterStateValues);
					pNode->appendChild(pDoc->createTextNode("\n  "));
				}
				else if(pFaceInfo->bGushingStatusSS)
				{
					pUnderWaterState->Puttext("渗水");
					pNode->appendChild(pUnderWaterState);
					pNode->appendChild(pDoc->createTextNode("\n		"));
					pUnderWaterStateValues->Puttext("渗水");
					pNode->appendChild(pUnderWaterStateValues);
					pNode->appendChild(pDoc->createTextNode("\n  "));
				}
				else if(pFaceInfo->bGushingStatusZTSR)
				{
					pUnderWaterState->Puttext("整体湿润");
					pNode->appendChild(pUnderWaterState);
					pNode->appendChild(pDoc->createTextNode("\n		"));
					pUnderWaterStateValues->Puttext("整体湿润");
					pNode->appendChild(pUnderWaterStateValues);
					pNode->appendChild(pDoc->createTextNode("\n  "));
				}
				else if(pFaceInfo->bGushingStatusYCPC)
				{
					pUnderWaterState->Puttext("渗出或喷出");
					pNode->appendChild(pUnderWaterState);
					pNode->appendChild(pDoc->createTextNode("\n		"));
					pUnderWaterStateValues->Puttext("渗出或喷出");
					pNode->appendChild(pUnderWaterStateValues);
					pNode->appendChild(pDoc->createTextNode("\n  "));
				}
				else if(pFaceInfo->bGushingStatusTBD)
				{
					pUnderWaterState->Puttext("特别大");
					pNode->appendChild(pUnderWaterState);
					pNode->appendChild(pDoc->createTextNode("\n		"));
					pUnderWaterStateValues->Puttext("特别大");
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

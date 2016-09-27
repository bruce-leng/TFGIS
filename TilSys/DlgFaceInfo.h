#if !defined(AFX_DLGFACEINFO_H__53198456_BF1D_4390_AFEF_FCFA2F9B2A77__INCLUDED_)
#define AFX_DLGFACEINFO_H__53198456_BF1D_4390_AFEF_FCFA2F9B2A77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFaceInfo.h : header file
//
#include <vector>
#include "Image.h"
#include "FaceInfoList.h"
#include "TilCanvas.h"


using namespace std;
/////////////////////////////////////////////////////////////////////////////
// CDlgFaceInfo dialog

class CDlgFaceInfo : public CDialog
{
// Construction
public:
	CDlgFaceInfo(CWnd* pParent = NULL);   // standard constructor
	~CDlgFaceInfo();

	// ��������Ƭ����
	int m_nFaceNum;
	// ��ǰ��������Ϣ������
	int m_nFaceIdx;
	// ������������Ϣָ��
	CFaceInfoList m_faceInfoLst;

// Dialog Data
	CString m_strFaceDK;	// ���ǰ׺
	float m_fFaceMile;		// �����������Ҫ��������
	CImage* m_pFacePhoto;	// ������ͼ����ʾ��Ҫ�������� 
	CString m_strPath;		// ���ڴ洢���������·��
	//{{AFX_DATA(CDlgFaceInfo)
	enum { IDD = IDD_FACE_INFO };
	CEdit	m_edtWeatheringOtherDesc;
	CEdit	m_edtRockStrengthOtherDesc;
	CEdit	m_edtFaceStatusOtherDesc;
	CEdit	m_edtExcaveStatusOtherDesc;
	CEdit	m_edtCrackWidthOtherDesc;
	CEdit	m_edtCrackStatusOtherDesc;
	CButton	m_btnRecCopyPrev;
	CEdit	m_edtFaceDepth;
	CEdit	m_edtTunName;
	CEdit	m_edtPartIdxName;
	CEdit	m_edtRecSupervision;
	CEdit	m_edtRecStaff;
	CEdit	m_edtRecReview;
	CEdit	m_edtFaceWidth;
	CEdit	m_edtFaceMile;
	CEdit	m_edtFaceHeight;
	CEdit	m_edtFaceDesc;
	CEdit	m_edtFaceArea;
	CDateTimeCtrl	m_ctlDateTime;
	CComboBox	m_cmbExcaveWay;
	CComboBox	m_cmbExcavePos;
	CComboBox	m_cmbExcaveDir;
	CComboBox	m_cmbDesignGrade;
	CComboBox	m_cmbConstructGrade;
	CStatic	m_stcFacePhoto;
	CButton	m_btnRecNext;
	CButton	m_btnRecModify;
	CButton	m_btnRecLast;
	CButton	m_btnRecFirst;
	CButton	m_btnRecPrev;
	CString	m_strConstructGrade;
	CString	m_strDesignGrade;
	CString	m_strExcaveDir;
	CString	m_strExcavePos;
	CString	m_strExcaveWay;
	CString	m_strPartIdxName;
	CString	m_strTunName;
	CTime	m_DateTime;
	float	m_fFaceArea;
	float	m_fFaceDepth;
	float	m_fFaceHeight;
	float	m_fFaceWidth;
	CString	m_strRecReview;
	CString	m_strRecStaff;
	CString	m_strRecSupervision;
	CString	m_strFaceDesc;
	CString	m_strFacePhoto;
	CString	m_strFaceMile;
	BOOL	m_bCrackStatusBFZK;
	BOOL	m_bCrackStatusJYLT;
	BOOL	m_bCrackStatusKK;
	BOOL	m_bCrackStatusMJ;
	BOOL	m_bCrackStatusOther;
	BOOL	m_bCrackWidth1;
	BOOL	m_bCrackWidth1_3;
	BOOL	m_bCrackWidth3_5;
	BOOL	m_bCrackWidth5;
	BOOL	m_bCrackWidthOther;
	BOOL	m_bExcaveStatusCQZH;
	BOOL	m_bExcaveStatusOther;
	BOOL	m_bExcaveStatusSCDK;
	BOOL	m_bExcaveStatusZW;
	BOOL	m_bExcaveStatusJSZH;
	BOOL	m_bFaceStatusOther;
	BOOL	m_bFaceStatusWD;
	BOOL	m_bFaceStatusZMBNZW;
	BOOL	m_bFaceStatusZMDK;
	BOOL	m_bFaceStatusZMJC;
	BOOL	m_bGushingStatusSS;
	BOOL	m_bGushingStatusTBD;
	BOOL	m_bGushingStatusWS;
	BOOL	m_bGushingStatusZTSR;
	BOOL	m_bGushingStatusYCPC;
	BOOL	m_bRockStrengthOther;
	BOOL	m_bRockStrengthRY;
	BOOL	m_bRockStrengthYY;
	BOOL	m_bRockStrengthJIRY;
	BOOL	m_bRockStrengthJORY;
	BOOL	m_bWeatheringOther;
	BOOL	m_bWeatheringQIFH;
	BOOL	m_bWeatheringRFH;
	BOOL	m_bWeatheringWFH;
	BOOL	m_bWeatheringQUFH;
	CString	m_strCrackStatusOtherDesc;
	CString	m_strCrackWidthOtherDesc;
	CString	m_strExcaveStatusOtherDesc;
	CString	m_strFaceStatusOtherDesc;
	CString	m_strRockStrengthOtherDesc;
	CString	m_strWeatheringOtherDesc;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFaceInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFaceInfo)
	afx_msg void OnRecordFirst();
	afx_msg void OnRecordPrev();
	afx_msg void OnRecordModify();
	afx_msg void OnRecordNext();
	afx_msg void OnRecordLast();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnRecordCopyPrev();
	afx_msg void OnSetfocusEdtFaceMile();
	afx_msg void OnKillfocusEdtFaceMile();
	afx_msg void OnPaint();
	afx_msg void OnCheckFaceStatusOther();
	afx_msg void OnCheckExcaveStatusOther();
	afx_msg void OnCheckRockStrengthOther();
	afx_msg void OnCheckWeatheringOther();
	afx_msg void OnCheckCrackWidthOther();
	afx_msg void OnCheckCrackStatusOther();
	afx_msg void OnRecordFromXml(); // ��xml�ļ��ж�ȡ��ǰ����������
	afx_msg void OnRecordToXml();   // ����ǰ�������¼���浽xml�ļ���
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	// �������пؼ���״̬
	void SetCtrlsStatus(BOOL bEnable);
	// ��ifo�ļ��ж�ȡ��������Ϣ
	BOOL ReadFaceInfoAndCanvas(CString strFileName, CTilCanvas* pCanvas);

private:
	// ��FACEINFO�����ж�ȡֵ��ʾ���ؼ���
	void SetShowedFaceInfo(FACEINFOCANVAS& faceInfo, BOOL bCopyPrev = FALSE);
	// ���ؼ��е�ֵ������FACEINFO������
	void GetShowedFaceInfo(FACEINFOCANVAS& faceInfo);
	// ��������ȡ��߶ȡ����
	void GetPolyCaption(float& fWidth, float& fHeight, float& fArea);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFACEINFO_H__53198456_BF1D_4390_AFEF_FCFA2F9B2A77__INCLUDED_)

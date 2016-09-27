// FaceInfoList.h: interface for the CFaceInfoList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FACEINFOLIST_H__6E6ECA86_8BD3_4DB4_989C_8384207DFD6C__INCLUDED_)
#define AFX_FACEINFOLIST_H__6E6ECA86_8BD3_4DB4_989C_8384207DFD6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxtempl.h>
#include <vector>
#include "TilCanvas.h"

using namespace std;

// ��������Ϣ�ṹ��
typedef struct __tagFACEINFO
{
	// ������ƣ��ṹ��ż����ƣ����
	CString strTunName, strPartIdxName, strID;
	// ��¼��λ��ʩ����λ����¼�ˣ������ˣ�������ʦ
	CString strRecUnit, strConstructUnit, strRecStaff, strRecReview, strRecSupervision;
	// �������������
	CString strFaceMile;
	// ���������ǰ׺
	CString strFaceDK;
	// ��������̣���ȣ��߶ȣ����
	float fFaceMile, fFaceWidth, fFaceHeight, fFaceArea;	
	// ���ڷ�ʽ����λ
	CString strExcaveWay, strExcavePos, strOther;
	// ����ʱ��
	CTime dateTime;
	// ����
	float fFaceDepth;
	// ���ڷ���Χ�����ԣ���Ƽ���ʩ������
	CString strExcaveDir, strLithology, strDesignGrade, strConstructGrade;
	//������״̬������
	BOOL bFaceStatusWD, bFaceStatusZMDK, bFaceStatusZMJC, bFaceStatusZMBNZW, bFaceStatusOther;
	CString strFaceStatusWD, strFaceStatusZMDK, strFaceStatusZMJC, strFaceStatusZMBNZW, strFaceStatusOther, strFaceStatusOtherDesc;
	// ë������״̬������
	BOOL bExcaveStatusZW, bExcaveStatusSCDK, bExcaveStatusJSZH, bExcaveStatusCQZH, bExcaveStatusOther;
	CString strExcaveStatusZW, strExcaveStatusSCDK, strExcaveStatusJSZH, strExcaveStatusCQZH, strExcaveStatusOther, strExcaveStatusOtherDesc;
	// ����ǿ�ȼ�����
	BOOL bRockStrengthYY, bRockStrengthJORY, bRockStrengthRY, bRockStrengthJIRY, bRockStrengthOther;
	CString strRockStrengthYY, strRockStrengthJORY, strRockStrengthRY, strRockStrengthJIRY, strRockStrengthOther, strRockStrengthOtherDesc;
	// �绯�̶ȼ�����
	BOOL bWeatheringWFH, bWeatheringRFH, bWeatheringQIFH, bWeatheringQUFH, bWeatheringOther;
	CString strWeatheringWFH, strWeatheringRFH, strWeatheringQIFH, strWeatheringQUFH, strWeatheringOther, strWeatheringOtherDesc;
	// ��϶��ȼ�����
	BOOL bCrackWidth5, bCrackWidth3_5, bCrackWidth1_3, bCrackWidth1, bCrackWidthOther;
	CString strCrackWidth5, strCrackWidth3_5, strCrackWidth1_3, strCrackWidth1, strCrackWidthOther, strCrackWidthOtherDesc;
	// ��϶��̬������
	BOOL bCrackStatusMJ, bCrackStatusBFZK, bCrackStatusKK, bCrackStatusJYLT, bCrackStatusOther;
	CString strCrackStatusMJ, strCrackStatusBFZK, strCrackStatusKK, strCrackStatusJYLT, strCrackStatusOther, strCrackStatusOtherDesc;
	// ӿˮ״̬������
	BOOL bGushingStatusWS, bGushingStatusSS, bGushingStatusZTSR, bGushingStatusYCPC, bGushingStatusTBD;
	CString strGushingStatusWS, strGushingStatusSS, strGushingStatusZTSR, strGushingStatusYCPC, strGushingStatusTBD;
	// ��������Ƭ����������
	CString strFacePhoto, strFaceDesc;
	// ������ͼ��ָ��
	// 		CTilGraph* pGraph;
	// 		// ������ͼ��ָ��
	// 		CTilImage* pImage;
}FACEINFO;
// ���������������Ϣ�ṹ�廭��
typedef struct __tagFACEINFOCANVAS : public FACEINFO
{
	// �����滭��ָ��
	CTilCanvas* pCanvas;
	// �洢��ӡ֮ǰ���������еĻ��������ű���
	long double dbOriZoomX, dbOriZoomY;
	// �洢��ӡ֮ǰ�������ڵ���ʾ/����״̬
	BOOL bShowImage, bShowGraph;
	// �洢��ӡǰ���������еĻ����е�ͼԪ����ɫ
	COLORREF** ppLayerClrs;
}FACEINFOCANVAS;

class CFaceInfoList  
{
public:
	CFaceInfoList();
	virtual ~CFaceInfoList();

private:
	// ������������Ϣָ��
	vector<FACEINFOCANVAS*> m_vpFaceInfo;

public:
	// ����ڴ�
	void Clear();
	// ��ifo�ļ��ж�ȡ��������Ϣ
	BOOL ReadFaceInfoFileToList(CString strFileName);
	// ��ȡifo�ļ�����������Ϣ,ͬʱ��������Ϣ����
	BOOL ReadFaceInfoAndCanvas(CString strFileIFO, CTilCanvas* pCanvas);
	// �������������ʼ�ͼԪ��ɫ�ָ�����ӡǰ��״̬
	void RestoreOriCanvasSetting();
	// ���ָ��������ṹָ��
	FACEINFOCANVAS* GetFaceInfoPtr(int nFaceIdx);
	// ����ǰ��������Ϣ������������
	void SaveFaceInfoToBuff(int nFaceIdx, FACEINFOCANVAS& faceInfo);
	// ��������������Ϣ����
	void SaveAllFaceInfoFiles(CString strFilePath);
	// �����������Ϣָ��
	vector<FACEINFOCANVAS*>* GetFaceInfoList() { return &m_vpFaceInfo; };
};

#endif // !defined(AFX_FACEINFOLIST_H__6E6ECA86_8BD3_4DB4_989C_8384207DFD6C__INCLUDED_)

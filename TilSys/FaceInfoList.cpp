// FaceInfoList.cpp: implementation of the CFaceInfoList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TilSys.h"
#include "FaceInfoList.h"
#include "TilComFunction.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFaceInfoList::CFaceInfoList()
{

}

CFaceInfoList::~CFaceInfoList()
{
	Clear();
}

void CFaceInfoList::Clear()
{
	for (int i=0; i<m_vpFaceInfo.size(); i++)
	{
		// ɾ���洢�Ļ����е�ͼԪ��ɫ
		CTilGraph* pGraphMgr = m_vpFaceInfo[i]->pCanvas->GetGraph();
		int nLayerNum = pGraphMgr->Size();
		for (int j=0; j<nLayerNum; j++)
		{
			delete [] m_vpFaceInfo[i]->ppLayerClrs[j];
		}
		delete [] m_vpFaceInfo[i]->ppLayerClrs;

		delete m_vpFaceInfo[i];
	}
	m_vpFaceInfo.clear();
}

// ��ifo�ļ��ж�ȡ��������Ϣ
BOOL CFaceInfoList::ReadFaceInfoFileToList(CString strFileName)
{
	CFile file;
	if (!file.Open(strFileName, CFile::modeRead))
	{
		// �����ȡ�ļ�ʧ�ܣ��½��ṹ����˳�
		FACEINFOCANVAS* pFI = new FACEINFOCANVAS;
		CTilCanvasMgr* pMgr = GetCanvasMgr();
		pFI->strTunName = pMgr->GetPrjInfo().strTunName;
		CString strPart = pMgr->GetPrjInfo().strPartName;
		if (!pMgr->GetPrjInfo().strPartIdx.IsEmpty())
			strPart = pMgr->GetPrjInfo().strPartIdx + "#" + strPart;
		pFI->strPartIdxName = strPart;
		pFI->strFaceMile = get_file_name(strFileName);
 		pFI->strFaceDK = get_std_dk(strFileName);
 		pFI->fFaceMile = get_std_mileage(strFileName);
		pFI->dateTime = CTime::GetCurrentTime();
		pFI->fFaceWidth = 0.0;
		pFI->fFaceHeight = 0.0;
		pFI->fFaceArea = 0.0;
		pFI->fFaceDepth = 0.0;

		// ������״̬
		pFI->bFaceStatusWD = FALSE;
		pFI->bFaceStatusZMDK = FALSE;
		pFI->bFaceStatusZMJC = FALSE;
		pFI->bFaceStatusZMBNZW = FALSE;
		pFI->bFaceStatusOther = FALSE;

		// ë������״̬
		pFI->bExcaveStatusZW = FALSE;
		pFI->bExcaveStatusSCDK = FALSE;
		pFI->bExcaveStatusJSZH = FALSE;
		pFI->bExcaveStatusCQZH = FALSE;
		pFI->bExcaveStatusOther = FALSE;

		// ǿʯǿ��
		pFI->bRockStrengthYY = FALSE;
		pFI->bRockStrengthJORY = FALSE;
		pFI->bRockStrengthRY = FALSE;
		pFI->bRockStrengthJIRY = FALSE;
		pFI->bRockStrengthOther = FALSE;

		// ����绯�̶�
		pFI->bWeatheringWFH = FALSE;
		pFI->bWeatheringRFH = FALSE;
		pFI->bWeatheringQIFH = FALSE;
		pFI->bWeatheringQUFH = FALSE;
		pFI->bWeatheringOther = FALSE;

		// ��϶���
		pFI->bCrackWidth5 = FALSE;
		pFI->bCrackWidth3_5 = FALSE;
		pFI->bCrackWidth1_3 = FALSE;
		pFI->bCrackWidth1 = FALSE;
		pFI->bCrackWidthOther = FALSE;

		// ��϶״̬
		pFI->bCrackStatusMJ = FALSE;
		pFI->bCrackStatusBFZK = FALSE;
		pFI->bCrackStatusKK = FALSE;
		pFI->bCrackStatusJYLT = FALSE;
		pFI->bCrackStatusOther = FALSE;

		// ӿˮ״̬
		pFI->bGushingStatusWS = FALSE;
		pFI->bGushingStatusSS = FALSE;
		pFI->bGushingStatusZTSR = FALSE;
		pFI->bGushingStatusYCPC = FALSE;
		pFI->bGushingStatusTBD = FALSE;

		// �������ļ�����
		pFI->strFacePhoto = get_file_name_ext(get_path_image(strFileName));

		m_vpFaceInfo.push_back(pFI);
		return FALSE;
	}

	// ��ȡ�ļ�
	CArchive ar(&file, CArchive::load);
	FACEINFOCANVAS* pFI = new FACEINFOCANVAS;
	ar >> pFI->strTunName >> pFI->strPartIdxName >> pFI->strID;							// ������ƣ���������
	ar >> pFI->strRecUnit >> pFI->strConstructUnit;	// ��¼��λ��ʩ����λ
	ar >> pFI->strRecStaff >> pFI->strRecReview >> pFI->strRecSupervision;	// ��¼�����ˣ�������ʦ
	ar >> pFI->strFaceMile;											// ���������
// 	ar >> pFI->strFaceDK >> pFI->fFaceMile;									// ���ǰ׺�����
// 	pFI->strFaceDK = get_std_dk(strFileName);						// ���¸����ļ�����ȡ���ǰ׺
// 	pFI->fFaceMile = get_std_mileage(strFileName);					// ���¸����ļ�����ȡ���������
	ar >> pFI->fFaceWidth >> pFI->fFaceHeight >> pFI->fFaceArea;			// �������ȣ��߶ȣ����
	ar >> pFI->strExcaveWay >> pFI->strExcavePos >> pFI->dateTime;			// ���ڷ�������λ��ʱ��
	ar >> pFI->fFaceDepth >> pFI->strExcaveDir;								// ������ڷ���
	ar >> pFI->strLithology >> pFI->strDesignGrade >> pFI->strConstructGrade;// ���ԣ����Χ�Ҽ���ʩ�����ü���

	// ������״̬
	CString strTemp;
	ar >> pFI->bFaceStatusWD >> pFI->strFaceStatusWD;
	ar >> pFI->bFaceStatusZMDK >> pFI->strFaceStatusZMDK;
	ar >> pFI->bFaceStatusZMJC >> pFI->strFaceStatusZMJC;
	ar >> pFI->bFaceStatusZMBNZW >> pFI->strFaceStatusZMBNZW;
	ar >> pFI->bFaceStatusOther >> pFI->strFaceStatusOther;
	ar >> pFI->strFaceStatusOtherDesc;
	
	// ë������״̬
	ar >> pFI->bExcaveStatusZW >> pFI->strExcaveStatusZW;
	ar >> pFI->bExcaveStatusSCDK >> pFI->strExcaveStatusSCDK;
	ar >> pFI->bExcaveStatusJSZH >> pFI->strExcaveStatusJSZH;
	ar >> pFI->bExcaveStatusCQZH >> pFI->strExcaveStatusCQZH;
	ar >> pFI->bExcaveStatusOther >> pFI->strExcaveStatusOther;
	ar >> pFI->strExcaveStatusOtherDesc;
	
	// ǿʯǿ��
	ar >> pFI->bRockStrengthYY >> pFI->strRockStrengthYY;
	ar >> pFI->bRockStrengthJORY >> pFI->strRockStrengthJORY;
	ar >> pFI->bRockStrengthRY >> pFI->strRockStrengthRY;
	ar >> pFI->bRockStrengthJIRY >> pFI->strRockStrengthJIRY;
	ar >> pFI->bRockStrengthOther >> pFI->strRockStrengthOther;
	ar >> pFI->strRockStrengthOtherDesc;
	
	// ����绯�̶�
	ar >> pFI->bWeatheringWFH >> pFI->strWeatheringWFH;
	ar >> pFI->bWeatheringRFH >> pFI->strWeatheringRFH;
	ar >> pFI->bWeatheringQIFH >> pFI->strWeatheringQIFH;
	ar >> pFI->bWeatheringQUFH >> pFI->strWeatheringQUFH;
	ar >> pFI->bWeatheringOther >> pFI->strWeatheringOther;
	ar >> pFI->strWeatheringOtherDesc;
	
	// ��϶���
	ar >> pFI->bCrackWidth5 >> pFI->strCrackWidth5;
	ar >> pFI->bCrackWidth3_5 >> pFI->strCrackWidth3_5;
	ar >> pFI->bCrackWidth1_3 >> pFI->strCrackWidth1_3;
	ar >> pFI->bCrackWidth1 >> pFI->strCrackWidth1;
	ar >> pFI->bCrackWidthOther >> pFI->strCrackWidthOther;
	ar >> pFI->strCrackWidthOtherDesc;
	
	// ��϶״̬
	ar >> pFI->bCrackStatusMJ >> pFI->strCrackStatusMJ;
	ar >> pFI->bCrackStatusBFZK >> pFI->strCrackStatusBFZK;
	ar >> pFI->bCrackStatusKK >> pFI->strCrackStatusKK;
	ar >> pFI->bCrackStatusJYLT >> pFI->strCrackStatusJYLT;
	ar >> pFI->bCrackStatusOther >> pFI->strCrackStatusOther;
	ar >> pFI->strCrackStatusOtherDesc;
	
	// ӿˮ״̬
	ar >> pFI->bGushingStatusWS >> pFI->strGushingStatusWS;
	ar >> pFI->bGushingStatusSS >> pFI->strGushingStatusSS;
	ar >> pFI->bGushingStatusZTSR >> pFI->strGushingStatusZTSR;
	ar >> pFI->bGushingStatusYCPC >> pFI->strGushingStatusYCPC;
	ar >> pFI->bGushingStatusTBD >> pFI->strGushingStatusTBD;
	
	
	// ��������Ƭ����������������
	ar >> pFI->strFacePhoto >> pFI->strFaceDesc;

	m_vpFaceInfo.push_back(pFI);

	ar.Close();
	file.Close();

	return TRUE;
}

// ���ָ��������ṹָ��
FACEINFOCANVAS* CFaceInfoList::GetFaceInfoPtr(int nFaceIdx)
{
	ASSERT(nFaceIdx>=0 && nFaceIdx<m_vpFaceInfo.size());
	return m_vpFaceInfo[nFaceIdx];
}

// ����ǰ��������Ϣ������������
void CFaceInfoList::SaveFaceInfoToBuff(int nFaceIdx, FACEINFOCANVAS& faceInfo)
{
	ASSERT(nFaceIdx>=0 && nFaceIdx<m_vpFaceInfo.size());
	m_vpFaceInfo[nFaceIdx]->strTunName = faceInfo.strTunName;
	m_vpFaceInfo[nFaceIdx]->strPartIdxName = faceInfo.strPartIdxName;
	m_vpFaceInfo[nFaceIdx]->strID = faceInfo.strID;
	m_vpFaceInfo[nFaceIdx]->strRecUnit = faceInfo.strRecUnit;
	m_vpFaceInfo[nFaceIdx]->strConstructUnit = faceInfo.strConstructUnit;
	m_vpFaceInfo[nFaceIdx]->strRecStaff = faceInfo.strRecStaff;
	m_vpFaceInfo[nFaceIdx]->strRecReview = faceInfo.strRecReview;
	m_vpFaceInfo[nFaceIdx]->strRecSupervision = faceInfo.strRecSupervision;
	m_vpFaceInfo[nFaceIdx]->strFaceMile = faceInfo.strFaceMile;
	m_vpFaceInfo[nFaceIdx]->fFaceWidth = faceInfo.fFaceWidth;
	m_vpFaceInfo[nFaceIdx]->fFaceHeight = faceInfo.fFaceHeight;
	m_vpFaceInfo[nFaceIdx]->fFaceArea = faceInfo.fFaceArea;
	m_vpFaceInfo[nFaceIdx]->strExcaveWay = faceInfo.strExcaveWay;
	m_vpFaceInfo[nFaceIdx]->strExcavePos = faceInfo.strExcavePos;
	m_vpFaceInfo[nFaceIdx]->dateTime = faceInfo.dateTime;
	m_vpFaceInfo[nFaceIdx]->fFaceDepth = faceInfo.fFaceDepth;
	m_vpFaceInfo[nFaceIdx]->strExcaveDir = faceInfo.strExcaveDir;
	m_vpFaceInfo[nFaceIdx]->strLithology = faceInfo.strLithology;
	m_vpFaceInfo[nFaceIdx]->strDesignGrade = faceInfo.strDesignGrade;
	m_vpFaceInfo[nFaceIdx]->strConstructGrade = faceInfo.strConstructGrade;
	m_vpFaceInfo[nFaceIdx]->bFaceStatusWD = faceInfo.bFaceStatusWD;
	m_vpFaceInfo[nFaceIdx]->bFaceStatusZMDK = faceInfo.bFaceStatusZMDK;
	m_vpFaceInfo[nFaceIdx]->bFaceStatusZMJC = faceInfo.bFaceStatusZMJC;
	m_vpFaceInfo[nFaceIdx]->bFaceStatusZMBNZW = faceInfo.bFaceStatusZMBNZW;
	m_vpFaceInfo[nFaceIdx]->bFaceStatusOther = faceInfo.bFaceStatusOther;
	m_vpFaceInfo[nFaceIdx]->strFaceStatusOtherDesc = faceInfo.strFaceStatusOtherDesc;
	m_vpFaceInfo[nFaceIdx]->bExcaveStatusZW = faceInfo.bExcaveStatusZW;
	m_vpFaceInfo[nFaceIdx]->bExcaveStatusSCDK = faceInfo.bExcaveStatusSCDK;
	m_vpFaceInfo[nFaceIdx]->bExcaveStatusJSZH = faceInfo.bExcaveStatusJSZH;
	m_vpFaceInfo[nFaceIdx]->bExcaveStatusCQZH = faceInfo.bExcaveStatusCQZH;
	m_vpFaceInfo[nFaceIdx]->bExcaveStatusOther = faceInfo.bExcaveStatusOther;
	m_vpFaceInfo[nFaceIdx]->strExcaveStatusOtherDesc = faceInfo.strExcaveStatusOtherDesc;
	m_vpFaceInfo[nFaceIdx]->bRockStrengthYY = faceInfo.bRockStrengthYY;
	m_vpFaceInfo[nFaceIdx]->bRockStrengthJORY = faceInfo.bRockStrengthJORY;
	m_vpFaceInfo[nFaceIdx]->bRockStrengthRY = faceInfo.bRockStrengthRY;
	m_vpFaceInfo[nFaceIdx]->bRockStrengthJIRY = faceInfo.bRockStrengthJIRY;
	m_vpFaceInfo[nFaceIdx]->bRockStrengthOther = faceInfo.bRockStrengthOther;
	m_vpFaceInfo[nFaceIdx]->strRockStrengthOtherDesc = faceInfo.strRockStrengthOtherDesc;
	m_vpFaceInfo[nFaceIdx]->bWeatheringWFH = faceInfo.bWeatheringWFH;
	m_vpFaceInfo[nFaceIdx]->bWeatheringRFH = faceInfo.bWeatheringRFH;
	m_vpFaceInfo[nFaceIdx]->bWeatheringQIFH = faceInfo.bWeatheringQIFH;
	m_vpFaceInfo[nFaceIdx]->bWeatheringQUFH = faceInfo.bWeatheringQUFH;
	m_vpFaceInfo[nFaceIdx]->bWeatheringOther = faceInfo.bWeatheringOther;
	m_vpFaceInfo[nFaceIdx]->strWeatheringOtherDesc = faceInfo.strWeatheringOtherDesc;
	m_vpFaceInfo[nFaceIdx]->bCrackWidth5 = faceInfo.bCrackWidth5;
	m_vpFaceInfo[nFaceIdx]->bCrackWidth3_5 = faceInfo.bCrackWidth3_5;
	m_vpFaceInfo[nFaceIdx]->bCrackWidth1_3 = faceInfo.bCrackWidth1_3;
	m_vpFaceInfo[nFaceIdx]->bCrackWidth1 = faceInfo.bCrackWidth1;
	m_vpFaceInfo[nFaceIdx]->bCrackWidthOther = faceInfo.bCrackWidthOther;
	m_vpFaceInfo[nFaceIdx]->strCrackWidthOtherDesc = faceInfo.strCrackWidthOtherDesc;
	m_vpFaceInfo[nFaceIdx]->bCrackStatusMJ = faceInfo.bCrackStatusMJ;
	m_vpFaceInfo[nFaceIdx]->bCrackStatusBFZK = faceInfo.bCrackStatusBFZK;
	m_vpFaceInfo[nFaceIdx]->bCrackStatusKK = faceInfo.bCrackStatusKK;
	m_vpFaceInfo[nFaceIdx]->bCrackStatusJYLT = faceInfo.bCrackStatusJYLT;
	m_vpFaceInfo[nFaceIdx]->bCrackStatusOther = faceInfo.bCrackStatusOther;
	m_vpFaceInfo[nFaceIdx]->strCrackStatusOtherDesc = faceInfo.strCrackStatusOtherDesc;
	m_vpFaceInfo[nFaceIdx]->bGushingStatusWS = faceInfo.bGushingStatusWS;
	m_vpFaceInfo[nFaceIdx]->bGushingStatusSS = faceInfo.bGushingStatusSS;
	m_vpFaceInfo[nFaceIdx]->bGushingStatusZTSR = faceInfo.bGushingStatusZTSR;
	m_vpFaceInfo[nFaceIdx]->bGushingStatusYCPC = faceInfo.bGushingStatusYCPC;
	m_vpFaceInfo[nFaceIdx]->bGushingStatusTBD = faceInfo.bGushingStatusTBD;
	m_vpFaceInfo[nFaceIdx]->strFaceDesc = faceInfo.strFaceDesc;
}

// ��ȡifo�ļ�����������Ϣ,ͬʱ��������Ϣ����
BOOL CFaceInfoList::ReadFaceInfoAndCanvas(CString strFileIFO, CTilCanvas* pCanvas)
{
	// ����ifo�ļ��е���������Ϣ
	ReadFaceInfoFileToList(strFileIFO);

	// ���뻭��ָ��
	int nSize = m_vpFaceInfo.size();
	m_vpFaceInfo[nSize-1]->pCanvas = pCanvas;

	// ���¶�ȡ�������Ƶ���Ϣ
	PRJCAPTION prjInfo = GetCanvasMgr()->GetPrjInfo();
	m_vpFaceInfo[nSize-1]->strTunName = prjInfo.strTunName;
	if (!prjInfo.strPartIdx.IsEmpty())
		m_vpFaceInfo[nSize-1]->strPartIdxName = prjInfo.strPartIdx + "#" + prjInfo.strPartName;
	else
		m_vpFaceInfo[nSize-1]->strPartIdxName = prjInfo.strPartName;

	// ����������ԭʼ"��ʾ/����"��Ϣ
	m_vpFaceInfo[nSize-1]->bShowImage = pCanvas->GetImage()->GetShowMode();
	m_vpFaceInfo[nSize-1]->bShowGraph = pCanvas->GetGraph()->GetShowMode();

	// ����������ԭʼ���Ŵ�С
	pCanvas->GetZoomFactor(m_vpFaceInfo[nSize-1]->dbOriZoomX, m_vpFaceInfo[nSize-1]->dbOriZoomY);

	// �������������и�ͼԪ��ԭʼ��ɫ
	CTilGraph* pGraphMgr = pCanvas->GetGraph();
	int nLayerNum = pGraphMgr->Size();
	m_vpFaceInfo[nSize-1]->ppLayerClrs = new COLORREF*[nLayerNum];
	for (int i=0; i<nLayerNum; i++)
	{
		CGraph* pGraph = pGraphMgr->GetGraphByIdx(i);
		int nUnitNum = pGraph->Size();
		m_vpFaceInfo[nSize-1]->ppLayerClrs[i] = new COLORREF[nUnitNum];
		for (int j=0; j<nUnitNum; j++)
		{
			CUnit* pUnit = pGraph->GetUnitByIdx(j);
			// ����ԭ����ͼԪ��ɫ
			m_vpFaceInfo[nSize-1]->ppLayerClrs[i][j] = pUnit->m_PenColor;
			// �ڴ�ӡʱ����ͼԪ��ɫΪ��ɫ
			pUnit->m_PenColor = RGB(0, 0, 0);
		}
	}

	// ����ͼ��·��
	m_vpFaceInfo[nSize-1]->strFacePhoto = get_file_name_ext(pCanvas->GetImage()->GetPathName());

	return TRUE;
}

// ��������"����/��ʾ"�������ʼ�ͼԪ��ɫ�ָ�����ӡǰ��״̬
void CFaceInfoList::RestoreOriCanvasSetting()
{
	int nNum = m_vpFaceInfo.size();
	for (int i=0; i<nNum; i++)
	{
		// �ָ�"����/��ʾ"����
		m_vpFaceInfo[i]->pCanvas->GetImage()->SetShowMode(m_vpFaceInfo[i]->bShowImage);
		m_vpFaceInfo[i]->pCanvas->GetGraph()->SetShowMode(m_vpFaceInfo[i]->bShowGraph);

		// �ָ����������ű���
		m_vpFaceInfo[i]->pCanvas->SetZoomFactor(m_vpFaceInfo[i]->dbOriZoomX, m_vpFaceInfo[i]->dbOriZoomY);

		// �ָ������и�ͼԪ��ԭʼ��ɫ
		CTilGraph* pGraphMgr = m_vpFaceInfo[i]->pCanvas->GetGraph();
		int nLayerNum = pGraphMgr->Size();
		for (int j=0; j<nLayerNum; j++)
		{
			CGraph* pGraph = pGraphMgr->GetGraphByIdx(j);
			int nUnitNum = pGraph->Size();
			for (int k=0; k<nUnitNum; k++)
			{
				CUnit* pUnit = pGraph->GetUnitByIdx(k);
				// �ָ�ԭ����ͼԪ��ɫ
				pUnit->m_PenColor = m_vpFaceInfo[i]->ppLayerClrs[j][k];
			}
		}
	}
}

// ��������������Ϣ����
void CFaceInfoList::SaveAllFaceInfoFiles(CString strFilePath)
{
	for (int i=0; i<m_vpFaceInfo.size(); i++)
	{
		FACEINFOCANVAS* pFI = m_vpFaceInfo[i];

		// �������ļ�������Ѿ����ڣ���ض���0
		CFile file;
		CString strFaceFile = get_path_info(strFilePath + '\\' + pFI->strFacePhoto);
		if (!file.Open(strFaceFile, CFile::modeCreate | CFile::modeWrite))
			continue;

		CArchive ar(&file, CArchive::store);
		ar << pFI->strTunName << pFI->strPartIdxName << pFI->strID;				// ������ƣ���������,���
		ar << pFI->strRecUnit << pFI->strConstructUnit;							// ��¼��λ��ʩ����λ
		ar << pFI->strRecStaff << pFI->strRecReview << pFI->strRecSupervision;	// ��¼�����ˣ�������ʦ
		ar << pFI->strFaceMile;													// ���������
		ar << pFI->fFaceWidth << pFI->fFaceHeight << pFI->fFaceArea;			// �������ȣ��߶ȣ����
		ar << pFI->strExcaveWay << pFI->strExcavePos << pFI->dateTime;			// ���ڷ�������λ��ʱ��
		ar << pFI->fFaceDepth << pFI->strExcaveDir;								// ������ڷ���
		ar << pFI->strLithology << pFI->strDesignGrade << pFI->strConstructGrade;// Χ�����ԣ����Χ�Ҽ���ʩ�����ü���

		// �����״̬���ڼ�״̬����
		CString strTmp;
		// ������״̬
		strTmp = "�ȶ�";
		ar << pFI->bFaceStatusWD << strTmp;
		strTmp = "�������";
		ar << pFI->bFaceStatusZMDK << strTmp;
		strTmp = "���漷��";
		ar << pFI->bFaceStatusZMJC << strTmp;
		strTmp = "���治������";
		ar << pFI->bFaceStatusZMBNZW << strTmp;
		strTmp = "����:";
		ar << pFI->bFaceStatusOther << strTmp;
		ar << pFI->strFaceStatusOtherDesc;
		
		// ë������״̬
		strTmp = "����";
		ar << pFI->bExcaveStatusZW << strTmp;
		strTmp = "��ʱ���ɳڡ�����";
		ar << pFI->bExcaveStatusSCDK << strTmp;
		strTmp = "�������ѣ�Ҫ��ʱ֧��";
		ar << pFI->bExcaveStatusJSZH << strTmp;
		strTmp = "Ҫ��ǰ֧��";
		ar << pFI->bExcaveStatusCQZH << strTmp;
		strTmp = "����:";
		ar << pFI->bExcaveStatusOther << strTmp;
		ar << pFI->strExcaveStatusOtherDesc;
		
		// ��ʯǿ��
		strTmp = "30~60(Ӳ��)";
		ar << pFI->bRockStrengthYY << strTmp;
		strTmp = "15~30(������)";
		ar << pFI->bRockStrengthJORY << strTmp;
		strTmp = "5~15(����)";
		ar << pFI->bRockStrengthRY << strTmp;
		strTmp = "<5(������)";
		ar << pFI->bRockStrengthJIRY << strTmp;
		strTmp = "����:";
		ar << pFI->bRockStrengthOther << strTmp;
		ar << pFI->strRockStrengthOtherDesc;
		
		// �绯�̶�
		strTmp = "΢�绯";
		ar << pFI->bWeatheringWFH << strTmp;
		strTmp = "���绯";
		ar << pFI->bWeatheringRFH << strTmp;
		strTmp = "ǿ�绯";
		ar << pFI->bWeatheringQIFH << strTmp;
		strTmp = "ȫ�绯";
		ar << pFI->bWeatheringQUFH << strTmp;
		strTmp = "����:";
		ar << pFI->bWeatheringOther << strTmp;
		ar << pFI->strWeatheringOtherDesc;
		
		// ��϶���
		strTmp = ">5";
		ar << pFI->bCrackWidth5 << strTmp;
		strTmp = "3~5";
		ar << pFI->bCrackWidth3_5 << strTmp;
		strTmp = "1~3";
		ar << pFI->bCrackWidth1_3 << strTmp;
		strTmp = "<1";
		ar << pFI->bCrackWidth1 << strTmp;
		strTmp = "����:";
		ar << pFI->bCrackWidthOther << strTmp;
		ar << pFI->strCrackWidthOtherDesc;
		
		// ��϶��̬
		strTmp = "�ܼ�";
		ar << pFI->bCrackStatusMJ << strTmp;
		strTmp = "�����ſ�";
		ar << pFI->bCrackStatusBFZK << strTmp;
		strTmp = "����";
		ar << pFI->bCrackStatusKK << strTmp;
		strTmp = "����ճ��";
		ar << pFI->bCrackStatusJYLT << strTmp;
		strTmp = "����:";
		ar << pFI->bCrackStatusOther << strTmp;
		ar << pFI->strCrackStatusOtherDesc;
		
		// ӿˮ״̬
		strTmp = "��ˮ";
		ar << pFI->bGushingStatusWS << strTmp;
		strTmp = "��ˮ";
		ar << pFI->bGushingStatusSS << strTmp;
		strTmp = "����ʪ��";
		ar << pFI->bGushingStatusZTSR << strTmp;
		strTmp = "���������";
		ar << pFI->bGushingStatusYCPC << strTmp;
		strTmp = "�ر��";
		ar << pFI->bGushingStatusTBD << strTmp;

		// ��������Ƭ����������������
		ar << pFI->strFacePhoto << pFI->strFaceDesc;							

		ar.Close();
		file.Close();
	}
}

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
		// 删除存储的画布中的图元颜色
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

// 从ifo文件中读取掌子面信息
BOOL CFaceInfoList::ReadFaceInfoFileToList(CString strFileName)
{
	CFile file;
	if (!file.Open(strFileName, CFile::modeRead))
	{
		// 如果读取文件失败，新建结构体后退出
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

		// 掌子面状态
		pFI->bFaceStatusWD = FALSE;
		pFI->bFaceStatusZMDK = FALSE;
		pFI->bFaceStatusZMJC = FALSE;
		pFI->bFaceStatusZMBNZW = FALSE;
		pFI->bFaceStatusOther = FALSE;

		// 毛开挖面状态
		pFI->bExcaveStatusZW = FALSE;
		pFI->bExcaveStatusSCDK = FALSE;
		pFI->bExcaveStatusJSZH = FALSE;
		pFI->bExcaveStatusCQZH = FALSE;
		pFI->bExcaveStatusOther = FALSE;

		// 强石强度
		pFI->bRockStrengthYY = FALSE;
		pFI->bRockStrengthJORY = FALSE;
		pFI->bRockStrengthRY = FALSE;
		pFI->bRockStrengthJIRY = FALSE;
		pFI->bRockStrengthOther = FALSE;

		// 岩体风化程度
		pFI->bWeatheringWFH = FALSE;
		pFI->bWeatheringRFH = FALSE;
		pFI->bWeatheringQIFH = FALSE;
		pFI->bWeatheringQUFH = FALSE;
		pFI->bWeatheringOther = FALSE;

		// 裂隙宽度
		pFI->bCrackWidth5 = FALSE;
		pFI->bCrackWidth3_5 = FALSE;
		pFI->bCrackWidth1_3 = FALSE;
		pFI->bCrackWidth1 = FALSE;
		pFI->bCrackWidthOther = FALSE;

		// 裂隙状态
		pFI->bCrackStatusMJ = FALSE;
		pFI->bCrackStatusBFZK = FALSE;
		pFI->bCrackStatusKK = FALSE;
		pFI->bCrackStatusJYLT = FALSE;
		pFI->bCrackStatusOther = FALSE;

		// 涌水状态
		pFI->bGushingStatusWS = FALSE;
		pFI->bGushingStatusSS = FALSE;
		pFI->bGushingStatusZTSR = FALSE;
		pFI->bGushingStatusYCPC = FALSE;
		pFI->bGushingStatusTBD = FALSE;

		// 仅保存文件名称
		pFI->strFacePhoto = get_file_name_ext(get_path_image(strFileName));

		m_vpFaceInfo.push_back(pFI);
		return FALSE;
	}

	// 读取文件
	CArchive ar(&file, CArchive::load);
	FACEINFOCANVAS* pFI = new FACEINFOCANVAS;
	ar >> pFI->strTunName >> pFI->strPartIdxName >> pFI->strID;							// 隧道名称，工点名称
	ar >> pFI->strRecUnit >> pFI->strConstructUnit;	// 记录单位，施工单位
	ar >> pFI->strRecStaff >> pFI->strRecReview >> pFI->strRecSupervision;	// 记录，复核，监理工程师
	ar >> pFI->strFaceMile;											// 掌子面里程
// 	ar >> pFI->strFaceDK >> pFI->fFaceMile;									// 里程前缀，里程
// 	pFI->strFaceDK = get_std_dk(strFileName);						// 重新根据文件名读取里程前缀
// 	pFI->fFaceMile = get_std_mileage(strFileName);					// 重新根据文件名读取掌子面里程
	ar >> pFI->fFaceWidth >> pFI->fFaceHeight >> pFI->fFaceArea;			// 掌子面宽度，高度，面积
	ar >> pFI->strExcaveWay >> pFI->strExcavePos >> pFI->dateTime;			// 开挖方法，部位，时间
	ar >> pFI->fFaceDepth >> pFI->strExcaveDir;								// 埋深，开挖方向
	ar >> pFI->strLithology >> pFI->strDesignGrade >> pFI->strConstructGrade;// 岩性，设计围岩级别，施工采用级别

	// 掌子面状态
	CString strTemp;
	ar >> pFI->bFaceStatusWD >> pFI->strFaceStatusWD;
	ar >> pFI->bFaceStatusZMDK >> pFI->strFaceStatusZMDK;
	ar >> pFI->bFaceStatusZMJC >> pFI->strFaceStatusZMJC;
	ar >> pFI->bFaceStatusZMBNZW >> pFI->strFaceStatusZMBNZW;
	ar >> pFI->bFaceStatusOther >> pFI->strFaceStatusOther;
	ar >> pFI->strFaceStatusOtherDesc;
	
	// 毛开挖面状态
	ar >> pFI->bExcaveStatusZW >> pFI->strExcaveStatusZW;
	ar >> pFI->bExcaveStatusSCDK >> pFI->strExcaveStatusSCDK;
	ar >> pFI->bExcaveStatusJSZH >> pFI->strExcaveStatusJSZH;
	ar >> pFI->bExcaveStatusCQZH >> pFI->strExcaveStatusCQZH;
	ar >> pFI->bExcaveStatusOther >> pFI->strExcaveStatusOther;
	ar >> pFI->strExcaveStatusOtherDesc;
	
	// 强石强度
	ar >> pFI->bRockStrengthYY >> pFI->strRockStrengthYY;
	ar >> pFI->bRockStrengthJORY >> pFI->strRockStrengthJORY;
	ar >> pFI->bRockStrengthRY >> pFI->strRockStrengthRY;
	ar >> pFI->bRockStrengthJIRY >> pFI->strRockStrengthJIRY;
	ar >> pFI->bRockStrengthOther >> pFI->strRockStrengthOther;
	ar >> pFI->strRockStrengthOtherDesc;
	
	// 岩体风化程度
	ar >> pFI->bWeatheringWFH >> pFI->strWeatheringWFH;
	ar >> pFI->bWeatheringRFH >> pFI->strWeatheringRFH;
	ar >> pFI->bWeatheringQIFH >> pFI->strWeatheringQIFH;
	ar >> pFI->bWeatheringQUFH >> pFI->strWeatheringQUFH;
	ar >> pFI->bWeatheringOther >> pFI->strWeatheringOther;
	ar >> pFI->strWeatheringOtherDesc;
	
	// 裂隙宽度
	ar >> pFI->bCrackWidth5 >> pFI->strCrackWidth5;
	ar >> pFI->bCrackWidth3_5 >> pFI->strCrackWidth3_5;
	ar >> pFI->bCrackWidth1_3 >> pFI->strCrackWidth1_3;
	ar >> pFI->bCrackWidth1 >> pFI->strCrackWidth1;
	ar >> pFI->bCrackWidthOther >> pFI->strCrackWidthOther;
	ar >> pFI->strCrackWidthOtherDesc;
	
	// 裂隙状态
	ar >> pFI->bCrackStatusMJ >> pFI->strCrackStatusMJ;
	ar >> pFI->bCrackStatusBFZK >> pFI->strCrackStatusBFZK;
	ar >> pFI->bCrackStatusKK >> pFI->strCrackStatusKK;
	ar >> pFI->bCrackStatusJYLT >> pFI->strCrackStatusJYLT;
	ar >> pFI->bCrackStatusOther >> pFI->strCrackStatusOther;
	ar >> pFI->strCrackStatusOtherDesc;
	
	// 涌水状态
	ar >> pFI->bGushingStatusWS >> pFI->strGushingStatusWS;
	ar >> pFI->bGushingStatusSS >> pFI->strGushingStatusSS;
	ar >> pFI->bGushingStatusZTSR >> pFI->strGushingStatusZTSR;
	ar >> pFI->bGushingStatusYCPC >> pFI->strGushingStatusYCPC;
	ar >> pFI->bGushingStatusTBD >> pFI->strGushingStatusTBD;
	
	
	// 掌子面照片，掌子面其它描述
	ar >> pFI->strFacePhoto >> pFI->strFaceDesc;

	m_vpFaceInfo.push_back(pFI);

	ar.Close();
	file.Close();

	return TRUE;
}

// 获得指定掌子面结构指针
FACEINFOCANVAS* CFaceInfoList::GetFaceInfoPtr(int nFaceIdx)
{
	ASSERT(nFaceIdx>=0 && nFaceIdx<m_vpFaceInfo.size());
	return m_vpFaceInfo[nFaceIdx];
}

// 将当前掌子面信息保存至缓存中
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

// 读取ifo文件中掌子面信息,同时将画布信息导入
BOOL CFaceInfoList::ReadFaceInfoAndCanvas(CString strFileIFO, CTilCanvas* pCanvas)
{
	// 读入ifo文件中的掌子面信息
	ReadFaceInfoFileToList(strFileIFO);

	// 导入画布指针
	int nSize = m_vpFaceInfo.size();
	m_vpFaceInfo[nSize-1]->pCanvas = pCanvas;

	// 重新读取工程名称等信息
	PRJCAPTION prjInfo = GetCanvasMgr()->GetPrjInfo();
	m_vpFaceInfo[nSize-1]->strTunName = prjInfo.strTunName;
	if (!prjInfo.strPartIdx.IsEmpty())
		m_vpFaceInfo[nSize-1]->strPartIdxName = prjInfo.strPartIdx + "#" + prjInfo.strPartName;
	else
		m_vpFaceInfo[nSize-1]->strPartIdxName = prjInfo.strPartName;

	// 保留画布的原始"显示/隐藏"信息
	m_vpFaceInfo[nSize-1]->bShowImage = pCanvas->GetImage()->GetShowMode();
	m_vpFaceInfo[nSize-1]->bShowGraph = pCanvas->GetGraph()->GetShowMode();

	// 保留画布的原始缩放大小
	pCanvas->GetZoomFactor(m_vpFaceInfo[nSize-1]->dbOriZoomX, m_vpFaceInfo[nSize-1]->dbOriZoomY);

	// 保留保留画布中各图元的原始颜色
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
			// 保存原来的图元颜色
			m_vpFaceInfo[nSize-1]->ppLayerClrs[i][j] = pUnit->m_PenColor;
			// 在打印时设置图元颜色为黑色
			pUnit->m_PenColor = RGB(0, 0, 0);
		}
	}

	// 保存图像路径
	m_vpFaceInfo[nSize-1]->strFacePhoto = get_file_name_ext(pCanvas->GetImage()->GetPathName());

	return TRUE;
}

// 将画布的"隐藏/显示"及缩放率及图元颜色恢复到打印前的状态
void CFaceInfoList::RestoreOriCanvasSetting()
{
	int nNum = m_vpFaceInfo.size();
	for (int i=0; i<nNum; i++)
	{
		// 恢复"隐藏/显示"功能
		m_vpFaceInfo[i]->pCanvas->GetImage()->SetShowMode(m_vpFaceInfo[i]->bShowImage);
		m_vpFaceInfo[i]->pCanvas->GetGraph()->SetShowMode(m_vpFaceInfo[i]->bShowGraph);

		// 恢复画布的缩放比例
		m_vpFaceInfo[i]->pCanvas->SetZoomFactor(m_vpFaceInfo[i]->dbOriZoomX, m_vpFaceInfo[i]->dbOriZoomY);

		// 恢复画布中各图元的原始颜色
		CTilGraph* pGraphMgr = m_vpFaceInfo[i]->pCanvas->GetGraph();
		int nLayerNum = pGraphMgr->Size();
		for (int j=0; j<nLayerNum; j++)
		{
			CGraph* pGraph = pGraphMgr->GetGraphByIdx(j);
			int nUnitNum = pGraph->Size();
			for (int k=0; k<nUnitNum; k++)
			{
				CUnit* pUnit = pGraph->GetUnitByIdx(k);
				// 恢复原来的图元颜色
				pUnit->m_PenColor = m_vpFaceInfo[i]->ppLayerClrs[j][k];
			}
		}
	}
}

// 将所有掌子面信息保存
void CFaceInfoList::SaveAllFaceInfoFiles(CString strFilePath)
{
	for (int i=0; i<m_vpFaceInfo.size(); i++)
	{
		FACEINFOCANVAS* pFI = m_vpFaceInfo[i];

		// 创建新文件，如果已经存在，则截短至0
		CFile file;
		CString strFaceFile = get_path_info(strFilePath + '\\' + pFI->strFacePhoto);
		if (!file.Open(strFaceFile, CFile::modeCreate | CFile::modeWrite))
			continue;

		CArchive ar(&file, CArchive::store);
		ar << pFI->strTunName << pFI->strPartIdxName << pFI->strID;				// 隧道名称，工点名称,编号
		ar << pFI->strRecUnit << pFI->strConstructUnit;							// 记录单位，施工单位
		ar << pFI->strRecStaff << pFI->strRecReview << pFI->strRecSupervision;	// 记录，复核，监理工程师
		ar << pFI->strFaceMile;													// 掌子面里程
		ar << pFI->fFaceWidth << pFI->fFaceHeight << pFI->fFaceArea;			// 掌子面宽度，高度，面积
		ar << pFI->strExcaveWay << pFI->strExcavePos << pFI->dateTime;			// 开挖方法，部位，时间
		ar << pFI->fFaceDepth << pFI->strExcaveDir;								// 埋深，开挖方向
		ar << pFI->strLithology << pFI->strDesignGrade << pFI->strConstructGrade;// 围岩岩性，设计围岩级别，施工采用级别

		// 保存各状态开挖及状态描述
		CString strTmp;
		// 掌子面状态
		strTmp = "稳定";
		ar << pFI->bFaceStatusWD << strTmp;
		strTmp = "正面掉块";
		ar << pFI->bFaceStatusZMDK << strTmp;
		strTmp = "正面挤出";
		ar << pFI->bFaceStatusZMJC << strTmp;
		strTmp = "下面不能自稳";
		ar << pFI->bFaceStatusZMBNZW << strTmp;
		strTmp = "其它:";
		ar << pFI->bFaceStatusOther << strTmp;
		ar << pFI->strFaceStatusOtherDesc;
		
		// 毛开挖面状态
		strTmp = "自稳";
		ar << pFI->bExcaveStatusZW << strTmp;
		strTmp = "随时间松弛、掉块";
		ar << pFI->bExcaveStatusSCDK << strTmp;
		strTmp = "自稳困难，要及时支护";
		ar << pFI->bExcaveStatusJSZH << strTmp;
		strTmp = "要超前支护";
		ar << pFI->bExcaveStatusCQZH << strTmp;
		strTmp = "其它:";
		ar << pFI->bExcaveStatusOther << strTmp;
		ar << pFI->strExcaveStatusOtherDesc;
		
		// 岩石强度
		strTmp = "30~60(硬岩)";
		ar << pFI->bRockStrengthYY << strTmp;
		strTmp = "15~30(较软岩)";
		ar << pFI->bRockStrengthJORY << strTmp;
		strTmp = "5~15(软岩)";
		ar << pFI->bRockStrengthRY << strTmp;
		strTmp = "<5(极软岩)";
		ar << pFI->bRockStrengthJIRY << strTmp;
		strTmp = "其它:";
		ar << pFI->bRockStrengthOther << strTmp;
		ar << pFI->strRockStrengthOtherDesc;
		
		// 风化程度
		strTmp = "微风化";
		ar << pFI->bWeatheringWFH << strTmp;
		strTmp = "弱风化";
		ar << pFI->bWeatheringRFH << strTmp;
		strTmp = "强风化";
		ar << pFI->bWeatheringQIFH << strTmp;
		strTmp = "全风化";
		ar << pFI->bWeatheringQUFH << strTmp;
		strTmp = "其它:";
		ar << pFI->bWeatheringOther << strTmp;
		ar << pFI->strWeatheringOtherDesc;
		
		// 裂隙宽度
		strTmp = ">5";
		ar << pFI->bCrackWidth5 << strTmp;
		strTmp = "3~5";
		ar << pFI->bCrackWidth3_5 << strTmp;
		strTmp = "1~3";
		ar << pFI->bCrackWidth1_3 << strTmp;
		strTmp = "<1";
		ar << pFI->bCrackWidth1 << strTmp;
		strTmp = "其它:";
		ar << pFI->bCrackWidthOther << strTmp;
		ar << pFI->strCrackWidthOtherDesc;
		
		// 裂隙形态
		strTmp = "密集";
		ar << pFI->bCrackStatusMJ << strTmp;
		strTmp = "部分张开";
		ar << pFI->bCrackStatusBFZK << strTmp;
		strTmp = "开口";
		ar << pFI->bCrackStatusKK << strTmp;
		strTmp = "夹有粘土";
		ar << pFI->bCrackStatusJYLT << strTmp;
		strTmp = "其它:";
		ar << pFI->bCrackStatusOther << strTmp;
		ar << pFI->strCrackStatusOtherDesc;
		
		// 涌水状态
		strTmp = "无水";
		ar << pFI->bGushingStatusWS << strTmp;
		strTmp = "渗水";
		ar << pFI->bGushingStatusSS << strTmp;
		strTmp = "整体湿润";
		ar << pFI->bGushingStatusZTSR << strTmp;
		strTmp = "渗出或喷出";
		ar << pFI->bGushingStatusYCPC << strTmp;
		strTmp = "特别大";
		ar << pFI->bGushingStatusTBD << strTmp;

		// 掌子面照片，掌子面其它描述
		ar << pFI->strFacePhoto << pFI->strFaceDesc;							

		ar.Close();
		file.Close();
	}
}

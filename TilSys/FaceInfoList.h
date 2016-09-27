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

// 掌子面信息结构体
typedef struct __tagFACEINFO
{
	// 隧道名称，结构编号及名称，编号
	CString strTunName, strPartIdxName, strID;
	// 记录单位，施工单位，记录人，复核人，监理工程师
	CString strRecUnit, strConstructUnit, strRecStaff, strRecReview, strRecSupervision;
	// 掌子面里程名称
	CString strFaceMile;
	// 掌子面里程前缀
	CString strFaceDK;
	// 掌子面里程，宽度，高度，面积
	float fFaceMile, fFaceWidth, fFaceHeight, fFaceArea;	
	// 开挖方式，部位
	CString strExcaveWay, strExcavePos, strOther;
	// 开挖时间
	CTime dateTime;
	// 埋深
	float fFaceDepth;
	// 开挖方向，围岩岩性，设计级别，施工级别
	CString strExcaveDir, strLithology, strDesignGrade, strConstructGrade;
	//掌子面状态及描述
	BOOL bFaceStatusWD, bFaceStatusZMDK, bFaceStatusZMJC, bFaceStatusZMBNZW, bFaceStatusOther;
	CString strFaceStatusWD, strFaceStatusZMDK, strFaceStatusZMJC, strFaceStatusZMBNZW, strFaceStatusOther, strFaceStatusOtherDesc;
	// 毛开挖面状态及描述
	BOOL bExcaveStatusZW, bExcaveStatusSCDK, bExcaveStatusJSZH, bExcaveStatusCQZH, bExcaveStatusOther;
	CString strExcaveStatusZW, strExcaveStatusSCDK, strExcaveStatusJSZH, strExcaveStatusCQZH, strExcaveStatusOther, strExcaveStatusOtherDesc;
	// 岩体强度及描述
	BOOL bRockStrengthYY, bRockStrengthJORY, bRockStrengthRY, bRockStrengthJIRY, bRockStrengthOther;
	CString strRockStrengthYY, strRockStrengthJORY, strRockStrengthRY, strRockStrengthJIRY, strRockStrengthOther, strRockStrengthOtherDesc;
	// 风化程度及描述
	BOOL bWeatheringWFH, bWeatheringRFH, bWeatheringQIFH, bWeatheringQUFH, bWeatheringOther;
	CString strWeatheringWFH, strWeatheringRFH, strWeatheringQIFH, strWeatheringQUFH, strWeatheringOther, strWeatheringOtherDesc;
	// 裂隙宽度及描述
	BOOL bCrackWidth5, bCrackWidth3_5, bCrackWidth1_3, bCrackWidth1, bCrackWidthOther;
	CString strCrackWidth5, strCrackWidth3_5, strCrackWidth1_3, strCrackWidth1, strCrackWidthOther, strCrackWidthOtherDesc;
	// 裂隙形态及描述
	BOOL bCrackStatusMJ, bCrackStatusBFZK, bCrackStatusKK, bCrackStatusJYLT, bCrackStatusOther;
	CString strCrackStatusMJ, strCrackStatusBFZK, strCrackStatusKK, strCrackStatusJYLT, strCrackStatusOther, strCrackStatusOtherDesc;
	// 涌水状态及描述
	BOOL bGushingStatusWS, bGushingStatusSS, bGushingStatusZTSR, bGushingStatusYCPC, bGushingStatusTBD;
	CString strGushingStatusWS, strGushingStatusSS, strGushingStatusZTSR, strGushingStatusYCPC, strGushingStatusTBD;
	// 掌子面照片，其它描述
	CString strFacePhoto, strFaceDesc;
	// 掌子面图形指针
	// 		CTilGraph* pGraph;
	// 		// 掌子面图像指针
	// 		CTilImage* pImage;
}FACEINFO;
// 掌子面地质素描信息结构体画布
typedef struct __tagFACEINFOCANVAS : public FACEINFO
{
	// 掌子面画布指针
	CTilCanvas* pCanvas;
	// 存储打印之前在主窗口中的画布的缩放比例
	long double dbOriZoomX, dbOriZoomY;
	// 存储打印之前在主窗口的显示/隐藏状态
	BOOL bShowImage, bShowGraph;
	// 存储打印前的主窗口中的画面中的图元的颜色
	COLORREF** ppLayerClrs;
}FACEINFOCANVAS;

class CFaceInfoList  
{
public:
	CFaceInfoList();
	virtual ~CFaceInfoList();

private:
	// 所有掌子面信息指针
	vector<FACEINFOCANVAS*> m_vpFaceInfo;

public:
	// 清除内存
	void Clear();
	// 从ifo文件中读取掌子面信息
	BOOL ReadFaceInfoFileToList(CString strFileName);
	// 读取ifo文件中掌子面信息,同时将画布信息导入
	BOOL ReadFaceInfoAndCanvas(CString strFileIFO, CTilCanvas* pCanvas);
	// 将画布的缩放率及图元颜色恢复到打印前的状态
	void RestoreOriCanvasSetting();
	// 获得指定掌子面结构指针
	FACEINFOCANVAS* GetFaceInfoPtr(int nFaceIdx);
	// 将当前掌子面信息保存至缓存中
	void SaveFaceInfoToBuff(int nFaceIdx, FACEINFOCANVAS& faceInfo);
	// 将所有掌子面信息保存
	void SaveAllFaceInfoFiles(CString strFilePath);
	// 获得掌子面信息指针
	vector<FACEINFOCANVAS*>* GetFaceInfoList() { return &m_vpFaceInfo; };
};

#endif // !defined(AFX_FACEINFOLIST_H__6E6ECA86_8BD3_4DB4_989C_8384207DFD6C__INCLUDED_)

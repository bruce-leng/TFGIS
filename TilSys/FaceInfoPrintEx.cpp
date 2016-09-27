// FaceInfoPrintEx.cpp: implementation of the FaceInfoPrintEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tilsys.h"
#include "FaceInfoPrintEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FaceInfoPrintEx::FaceInfoPrintEx()
{
	// 表格区域
	m_Rect.SetRect(120, -350, 1970, -2680);
	// 表格区域内行高(除最后的"附图"行,该行高度较大)
	m_nRowHeight = 125;
}

FaceInfoPrintEx::~FaceInfoPrintEx()
{

}

//打印页眉页脚
void FaceInfoPrintEx::PrintHeaderFooter(CDC* pDC)
{

}

//打印报表头
void FaceInfoPrintEx::PrintFaceInfoHead(CDC* pDC)
{
	UINT ReportHeadFormat = DT_SINGLELINE|DT_CENTER|DT_VCENTER;
	UINT mutiFormat = ReportHeadFormat&(~DT_SINGLELINE);

	//单元格
	CRect CellRect;
	CString CellString;
	int x1,y1,x2,y2;

	//字体
	CFont *oldfont;
	//保存原字体
	oldfont=pDC->GetCurrentFont();
	
	//////////////////////////////////////////////////////////////////////////
	// 创建标题字体
	CFont TitleFont;
	TitleFont.CreatePointFont(220,_T("黑体"),pDC);
	pDC->SelectObject(&TitleFont);

	// 标题"掌子面地质素描记录表"
	x1 = m_Rect.left;
	x2 = m_Rect.right;
	y1 = m_Rect.top + 260;
	y2 = m_Rect.top + 150;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("掌子面地质素描记录表");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);

	//////////////////////////////////////////////////////////////////////////
	//创建表头字体
	CFont MyFont;
	MyFont.CreatePointFont(105,_T("宋体"),pDC);
	// 将表头字体加粗
// 	LOGFONT logFont;
// 	MyFont.GetLogFont(&logFont);
// 	logFont.lfWeight = FW_BOLD;
// 	CFont MyFont1;
// 	MyFont1.CreateFontIndirect(&logFont);
	pDC->SelectObject(&MyFont);
	
	// 工程名称
	x1 = m_Rect.left;
	x2 = x1+230;
	y1 = m_Rect.top+m_nRowHeight;
	y2 = m_Rect.top;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("工程名称:");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// 施工里程
	x1 = 850;
	x2 = x1+230;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("施工里程:");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// 编号
	x1 = 1470;
	x2 = x1+140;
	y1 = m_Rect.top+m_nRowHeight;
	y2 = m_Rect.top;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("编号:");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	//////////////////////////////////////////////////////////////////////////
	// 序号
	x1 = m_Rect.left;
	x2 = 257;
	y1 = m_Rect.top;
	y2 = m_Rect.top-m_nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("序号");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);

	// 编号1~9
	for (int i=0; i<9; i++)
	{
		y1 = y2;
		y2 = y2-m_nRowHeight;
		CellRect.SetRect(x1,y1,x2,y2);
		CellString.Format("%d", i+1);
		pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	}
	
	// 附图
	y1 = y2;
	y2 = m_Rect.bottom;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("附图");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);

	//////////////////////////////////////////////////////////////////////////
	// 项目名称
	x1 = x2;
	x2 = x1+250;
	y1 = m_Rect.top;
	y2 = y1-m_nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("项目名称");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);

	// 掌子面尺寸
	y1 = y2;
	y2 = y1-m_nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("掌子面尺寸");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// 掌子面状态
	y1 = y2;
	y2 = y1-m_nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("掌子面状态");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);

	// 围岩岩性
	y1 = y2;
	y2 = y1-m_nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("围岩岩性");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// 岩石强度(MPa)
	y1 = y2;
	y2 = y1-m_nRowHeight;
	y1 -= 30;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("岩石强度\n(MPa)");
	pDC->DrawText(CellString,CellRect,mutiFormat);

	// 风化程度
	y1 = y2;
	y2 = y1-m_nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("风化程度");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);

	// 裂隙间距(mm)
	y1 = y2;
	y2 = y1-m_nRowHeight;
	y1 -= 30;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("裂隙间距\n(mm)");
	pDC->DrawText(CellString,CellRect,mutiFormat);

	// 裂隙形态
	y1 = y2;
	y2 = y1-m_nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("裂隙形态");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// 涌水状态
	y1 = y2;
	y2 = y1-m_nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("涌水状态");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// 掌子面级别划分
	y1 = y2;
	y2 = y1-m_nRowHeight;
	y1 -= 30;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("围岩级别\n划分");
	pDC->DrawText(CellString,CellRect,mutiFormat);
	
	// 附掌子面照片
// 	x1 += 20;
// 	x2 = x1+300;
// 	y1 = y2;
// 	y2 = y1-60;
// 	CellRect.SetRect(x1,y1,x2,y2);
// 	CellString = _T("掌子面照片");
// 	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// 掌子面其它描述
// 	x1 = 1115+20;
// 	x2 = x1+300;
// 	CellRect.SetRect(x1,y1,x2,y2);
// 	CellString = _T("附掌子面照片:");
// 	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	//////////////////////////////////////////////////////////////////////////
	// 状态描述
	x1 = 507;
	x2 = m_Rect.right;
	y1 = m_Rect.top;
	y2 = y1-m_nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("状态描述");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// 开挖宽度(m)
	x2 = 645;
	y1 = y2-10;
	y2 = y1-m_nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("开挖\n宽度\n(m)");
	pDC->DrawText(CellString,CellRect,mutiFormat);
	//////////////////////////////////////////////////////////////////////////
	
	// 开挖高度(m)
	x1 = 775;
	x2 = x1+145;
	y1 = m_Rect.top-m_nRowHeight;
	y2 = y1-m_nRowHeight;
	y1 -= 10;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("开挖\n高度\n(m)");
	pDC->DrawText(CellString,CellRect,mutiFormat);
	
	// 开挖面积(m)
	x1 = 1120;
	x2 = x1+144;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("开挖\n面积\n(m2)");
	pDC->DrawText(CellString,CellRect,mutiFormat);
	
	// 开挖方式
	x1 = 1417;
	x2 = x1+93;
	y1 -= 20;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("开挖\n方式");
	pDC->DrawText(CellString,CellRect,mutiFormat);
	
	// 其它
	x1 = 1696;
	x2 = x1+109;
	y1 += 30;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("其它");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);

	//////////////////////////////////////////////////////////////////////////
	// 制表
	x1 = m_Rect.left;
	x2 = x1+200;
	y1 = m_Rect.bottom;
	y2 = y1-m_nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("填表单位:");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);

	// 复核
	x1 = 650;
	x2 = x1+200;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("施工单位:");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);

	// 现场监理工程师
	x1 = 1200;
	x2 = x1+260;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("监理工程师:");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
		
	pDC->SelectObject(oldfont);	
}

// 打印报表记录
void FaceInfoPrintEx::PrintFaceInfoRecord(CDC* pDC, int nIdx)
{
	// 行高(除最后的"附图"行,该行高度较大)
	int m_nRowHeight = 125;
	
	//单元格
	CRect CellRect;
	CString CellString;
	int x1,y1,x2,y2;
	
	//字体
	CFont *oldfont;
	//保存原字体
	oldfont=pDC->GetCurrentFont();
	
	//创建表头字体
	CFont MyFont;
	MyFont.CreatePointFont(105,_T("宋体"),pDC);
	pDC->SelectObject(&MyFont);

	// 得到当前地质素描记录
	FACEINFOCANVAS* pFI = m_FaceInfoRecords.GetFaceInfoList()->at(nIdx);
	
	//////////////////////////////////////////////////////////////////////////
	UINT ReportHeadFormat = DT_SINGLELINE|DT_CENTER|DT_VCENTER;
	UINT leftFormat = ReportHeadFormat&(~DT_CENTER);
	UINT mutiFormat = ReportHeadFormat&(~DT_SINGLELINE);

	// 工程名称
	x1 = m_Rect.left+210;
	x2 = x1+500;
	y1 = m_Rect.top+m_nRowHeight;
	y2 = m_Rect.top;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = pFI->strTunName+pFI->strPartIdxName;
	pDC->DrawText(CellString,CellRect,leftFormat);
	
	// 施工里程
	x1 = 1060;
	x2 = x1+390;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = pFI->strFaceMile;
// 	if (int(pFI->fFaceMile*10)%10 != 0)
// 		CellString.Format("%s%d+%d.%d",pFI->strFaceDK,int(pFI->fFaceMile)/1000,
// 									int(pFI->fFaceMile)%1000,int(pFI->fFaceMile*10)%10);
// 	else
// 		CellString.Format("%s%d+%d",pFI->strFaceDK,int(pFI->fFaceMile)/1000,
// 										int(pFI->fFaceMile)%1000);
	pDC->DrawText(CellString,CellRect,leftFormat);
	
	// 编号
	x1 = 1600;
	x2 = m_Rect.right;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = pFI->strID;
	pDC->DrawText(CellString,CellRect,leftFormat);

	//////////////////////////////////////////////////////////////////////////
	// 开挖宽度(m)
	x1 = 645;
	x2 = x1+130;
	y1 = m_Rect.top-m_nRowHeight;
	y2 = y1-m_nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	if (int(pFI->fFaceWidth*10)%10 != 0)
		CellString.Format("%d.%d", int(pFI->fFaceWidth), int(pFI->fFaceWidth*10)%10);
	else
		CellString.Format("%d", int(pFI->fFaceWidth));
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// 开挖高度(m)
	x1 = 920;
	x2 = x1+200;
	CellRect.SetRect(x1,y1,x2,y2);
	if (int(pFI->fFaceHeight*10)%10 != 0)
		CellString.Format("%d.%d", int(pFI->fFaceHeight), int(pFI->fFaceHeight*10)%10);
	else
		CellString.Format("%d", int(pFI->fFaceHeight));
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);

	// 开挖面积(m2)
	x1 = 1263;
	x2 = x1+154;
	CellRect.SetRect(x1,y1,x2,y2);
	if (int(pFI->fFaceArea*10)%10 != 0)
		CellString.Format("%d.%d", int(pFI->fFaceArea), int(pFI->fFaceArea*10)%10);
	else
		CellString.Format("%d", int(pFI->fFaceArea));
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// 开挖方式
	x1 = 1510;
	x2 = x1+186;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = pFI->strExcaveWay;
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// 其它
	x1 = 1800;
	x2 = m_Rect.right;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = pFI->strOther;
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);

	//////////////////////////////////////////////////////////////////////////
	// 掌子面状态
	CellString.Empty();
	// 稳定
	if (pFI->bFaceStatusWD)
		CellString = pFI->strFaceStatusWD + ",";
	// 正面掉块
	if (pFI->bFaceStatusZMDK)
		CellString += pFI->strFaceStatusZMDK + ",";
	// 正面挤出
	if (pFI->bFaceStatusZMJC)
		CellString += pFI->strFaceStatusZMJC + ",";
	// 正面不能自稳
	if (pFI->bFaceStatusZMBNZW)
		CellString += pFI->strFaceStatusZMBNZW + ",";
	// 其它
	if (pFI->bFaceStatusOther)
		CellString += _T("其它:")+pFI->strFaceStatusOtherDesc;

	x1 = m_Rect.left + 387;
	x2 = m_Rect.right;
	y1 = m_Rect.top-2*m_nRowHeight;
	y2 = y1-m_nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	//////////////////////////////////////////////////////////////////////////
	// 围岩岩性(2015-9-19修改)
	CellString = pFI->strLithology;

	x1 = m_Rect.left + 387;
	x2 = m_Rect.right;
	y1 = m_Rect.top-3*m_nRowHeight;
	y2 = y1-m_nRowHeight;
	CellRect.SetRect(x1+10,y1-10,x2,y2);
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	//////////////////////////////////////////////////////////////////////////
	// 岩石强度
	CellString.Empty();
	// 30~60(硬岩)
	if (pFI->bRockStrengthYY)
		CellString = pFI->strRockStrengthYY + ",";
	// 15~30(较软岩)
	if (pFI->bRockStrengthJORY)
		CellString += pFI->strRockStrengthJORY + ",";
	// 5~15(软岩)
	if (pFI->bRockStrengthRY)
		CellString += pFI->strRockStrengthRY + ",";
	// <5(极软岩)
	if (pFI->bRockStrengthJIRY)
		CellString += pFI->strRockStrengthJIRY + ",";
	// 其它
	if (pFI->bRockStrengthOther)
		CellString += _T("其它:")+pFI->strRockStrengthOtherDesc;

	x1 = m_Rect.left + 387;
	x2 = m_Rect.right;
	y1 = m_Rect.top-4*m_nRowHeight;
	y2 = y1-m_nRowHeight;
	CellRect.SetRect(x1+10,y1-10,x2,y2);
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	//////////////////////////////////////////////////////////////////////////
	// 风化程度
	CellString.Empty();
	// 微风化
	if (pFI->bWeatheringWFH)
		CellString = pFI->strWeatheringWFH + ",";
	// 弱风化
	if (pFI->bWeatheringRFH)
		CellString += pFI->strWeatheringRFH + ",";
	// 强风化
	if (pFI->bWeatheringQIFH)
		CellString += pFI->strWeatheringQIFH + ",";
	// 全风化
	if (pFI->bWeatheringQUFH)
		CellString += pFI->strWeatheringQUFH + ",";
	// 其它
	if (pFI->bWeatheringOther)
		CellString += _T("其它:")+pFI->strWeatheringOtherDesc;

	x1 = m_Rect.left + 387;
	x2 = m_Rect.right;
	y1 = m_Rect.top-5*m_nRowHeight;
	y2 = y1-m_nRowHeight;
	CellRect.SetRect(x1+10,y1-10,x2,y2);
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	//////////////////////////////////////////////////////////////////////////
	// 裂隙间距(mm)
	CellString.Empty();
	// >5
	if (pFI->bCrackWidth5)
		CellString = pFI->strCrackWidth5 + ",";
	// 3~5
	if (pFI->bCrackWidth3_5)
		CellString += pFI->strCrackWidth3_5 + ",";
	// 1~3
	if (pFI->bCrackWidth1_3)
		CellString += pFI->strCrackWidth1_3 + ",";
	// <1
	if (pFI->bCrackWidth1)
		CellString += pFI->strCrackWidth1 + ",";
	// 其它
	if (pFI->bCrackWidthOther)
		CellString += _T("其它:")+pFI->strCrackWidthOtherDesc;

	x1 = m_Rect.left + 387;
	x2 = m_Rect.right;
	y1 = m_Rect.top-6*m_nRowHeight;
	y2 = y1-m_nRowHeight;
	CellRect.SetRect(x1+10,y1-10,x2,y2);
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	//////////////////////////////////////////////////////////////////////////
	// 裂隙形态
	CellString.Empty();
	// 密集
	if (pFI->bCrackStatusMJ)
		CellString = pFI->strCrackStatusMJ;
	// 部分张开
	if (pFI->bCrackStatusBFZK)
		CellString += pFI->strCrackStatusBFZK + ",";
	// 开口
	if (pFI->bCrackStatusKK)
		CellString += pFI->strCrackStatusKK + ",";
	// 夹有粘土
	if (pFI->bCrackStatusJYLT)
		CellString += pFI->strCrackStatusJYLT + ",";
	// 其它
	if (pFI->bCrackStatusOther)
		CellString += _T("其它:")+pFI->strCrackStatusOtherDesc;

	x1 = m_Rect.left + 387;
	x2 = m_Rect.right;
	y1 = m_Rect.top-7*m_nRowHeight;
	y2 = y1-m_nRowHeight;
	CellRect.SetRect(x1+10,y1-10,x2,y2);
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	//////////////////////////////////////////////////////////////////////////
	// 涌水状态
	// 无水
	if (pFI->bGushingStatusWS)
		CellString = pFI->strGushingStatusWS + ",";
	// 渗水
	if (pFI->bGushingStatusSS)
		CellString += pFI->strGushingStatusSS + ",";
	// 整体湿润
	if (pFI->bGushingStatusZTSR)
		CellString += pFI->strGushingStatusZTSR + ",";
	// 涌出或喷出
	if (pFI->bGushingStatusYCPC)
		CellString += pFI->strGushingStatusYCPC + ",";
	// 特别大
	if (pFI->bGushingStatusTBD)
		CellString += pFI->strGushingStatusTBD;

	x1 = m_Rect.left + 387;
	x2 = m_Rect.right;
	y1 = m_Rect.top-8*m_nRowHeight;
	y2 = y1-m_nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	//////////////////////////////////////////////////////////////////////////
	// 围岩级别划分
	x1 = 507+10;
	x2 = m_Rect.right;
	y1 = m_Rect.top-9*m_nRowHeight;
	y2 = y1-m_nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = pFI->strDesignGrade;
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// 打印掌子面素描图(其在掌子面照片下方)
	x1 = 257+20;
	x2 = x1+862-40;
	y1 = m_Rect.bottom+(m_Rect.top-10*m_nRowHeight-m_Rect.bottom)/2-10;
	y2 = m_Rect.bottom+20;
	CellRect.SetRect(x1,y1,x2,y2);
	long double dbZoomX, dbZoomY;
	{
		// 掌子面素描图形
		CTilGraph* pGraph = pFI->pCanvas->GetGraph();
		// 在打印时重新设置缩放比例后显示
		dbZoomX = abs(CellRect.Width())*1.0/pGraph->GetWidth();
		dbZoomY = abs(CellRect.Height())*1.0/pGraph->GetHeight();
		pGraph->SetZoomFactor(dbZoomX, dbZoomY);
		pGraph->SetShowMode(TRUE);
		pGraph->DrawInvert(pDC, CellRect.TopLeft());
	}

	// 打印掌子面照片
	y2 = y1+20;
	y1 = m_Rect.top-10*m_nRowHeight-10;
	CellRect.SetRect(x1,y1,x2,y2);
	{
		// 掌子面照片
		CTilImage* pImage = pFI->pCanvas->GetImage();
		dbZoomX = abs(CellRect.Width())*1.0/pImage->GetWidth();
		dbZoomY = abs(CellRect.Height())*1.0/pImage->GetHeight();
		pImage->SetZoomFactor(dbZoomX, dbZoomY);
		pImage->SetShowMode(TRUE);
		pImage->Stretch(pDC, CellRect);
	}

	// 掌子面其它描述
	x1 = 1119+20;
	x2 = m_Rect.right-30;
	y1 = m_Rect.top-10*m_nRowHeight-10;
	y2 = m_Rect.bottom+80;
	CellRect.SetRect(x1,y1,x2+20,y2-60);
	CellString = "    " + pFI->strFaceDesc;
	// 加入换行符
	// 获得可换行的字符串的显示的高度
	CDC memDC;
	CRect rt(CellRect);
	memDC.CreateCompatibleDC(pDC);
	memDC.SelectObject(MyFont);
	int RtHeight = memDC.DrawText(CellString, rt, DT_CALCRECT|DT_CENTER|DT_EDITCONTROL|DT_WORDBREAK);
	int CellHeight = CellRect.Height();
	if (abs(CellHeight) > abs(RtHeight))
		CellRect.top -= (abs(CellHeight) - abs(RtHeight))/2;
	pDC->DrawText(CellString,CellRect,DT_WORDBREAK);

	//////////////////////////////////////////////////////////////////////////
	// 相关人员
	// 填表单位
	x1 = m_Rect.left+200;
	x2 = x1+400;
	y1 = m_Rect.bottom;
	y2 = y1-m_nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = pFI->strRecUnit;
	pDC->DrawText(CellString,CellRect,leftFormat);
	
	// 施工单位
	x1 = 850;
	x2 = x1+400;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = pFI->strConstructUnit;
	pDC->DrawText(CellString,CellRect,leftFormat);
	
	// 监理工程师
	x1 = 1460;
	x2 = x1+200;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = pFI->strRecSupervision;
	pDC->DrawText(CellString,CellRect,leftFormat);	

	// 年月日
	x1 = 1700;
	x2 = m_Rect.right;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = pFI->dateTime.Format("%Y年%m月%d日");
	pDC->DrawText(CellString,CellRect,leftFormat);

	pDC->SelectObject(oldfont);
}

// 画打印报表表格
void FaceInfoPrintEx::PrintFaceInfoGrid(CDC* pDC)
{
	//表格纵横
	int y;
	
	// 行高(除最后的"附图"行,该行高度较大)
	int m_nRowHeight = 125;
	
	// 画水平线
	for (int i=0; i<11; i++)
	{
		y = m_Rect.top-i*m_nRowHeight;
		DrawHLine(y, m_Rect.left, m_Rect.right, pDC);
	}
	// 画最后一条水平线
	DrawHLine(m_Rect.bottom, m_Rect.left, m_Rect.right, pDC);
	
	// 画竖直线
	// 左边界
	DrawVLine(m_Rect.left, m_Rect.top, m_Rect.bottom, pDC);
	// "编号"的右边界
	DrawVLine(257, m_Rect.top, m_Rect.bottom, pDC);
	// "项目名称"的右边界
	DrawVLine(507, m_Rect.top, m_Rect.top-10*m_nRowHeight, pDC);
	// "开挖宽度"的右边界
	DrawVLine(645, m_Rect.top-m_nRowHeight, m_Rect.top-2*m_nRowHeight, pDC);
	// "开挖宽度(m)"右侧数字栏的右边界
	DrawVLine(775, m_Rect.top-m_nRowHeight, m_Rect.top-2*m_nRowHeight, pDC);
	// "开挖高度"的右边界
	DrawVLine(920, m_Rect.top-m_nRowHeight, m_Rect.top-2*m_nRowHeight, pDC);
	// "开挖高度"右侧数字栏的右边界
	DrawVLine(1119, m_Rect.top-m_nRowHeight, m_Rect.top-2*m_nRowHeight, pDC);
	// "开挖面积"的右边界
	DrawVLine(1263, m_Rect.top-m_nRowHeight, m_Rect.top-2*m_nRowHeight, pDC);
	// "开挖面积"右侧数字栏的右边界
	DrawVLine(1417, m_Rect.top-m_nRowHeight, m_Rect.top-2*m_nRowHeight, pDC);
	// "开挖方式"的右边界
	DrawVLine(1510, m_Rect.top-m_nRowHeight, m_Rect.top-2*m_nRowHeight, pDC);
	// "开挖方式"右侧栏的右边界
	DrawVLine(1696, m_Rect.top-m_nRowHeight, m_Rect.top-2*m_nRowHeight, pDC);
	// "其它"栏的右边界
	DrawVLine(1800, m_Rect.top-m_nRowHeight, m_Rect.top-2*m_nRowHeight, pDC);
	// 最右边界
	DrawVLine(m_Rect.right, m_Rect.top, m_Rect.bottom, pDC);
	// 下方"掌子面其它描述"右侧边界
	DrawVLine(1119, m_Rect.top-10*m_nRowHeight, m_Rect.bottom, pDC);
}

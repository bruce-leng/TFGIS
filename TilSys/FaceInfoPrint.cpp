// FaceInfoPrint.cpp: implementation of the FaceInfoPrint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tilsys.h"
#include "FaceInfoPrint.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FaceInfoPrint::FaceInfoPrint()
{
	// 默认打印掌子面照片
	m_nPrintTarget = 0;
}

FaceInfoPrint::~FaceInfoPrint()
{

}

// 设置打印对象,0: 打印图像,1:打印素描图
void FaceInfoPrint::SetPrintTarget(int nFacePhoto)
{
	ASSERT(nFacePhoto == 0 || nFacePhoto == 1);
	m_nPrintTarget = nFacePhoto;
}

int  FaceInfoPrint::GetPrintTarget()
{
	return m_nPrintTarget;
}

//打印页眉页脚
void FaceInfoPrint::PrintHeaderFooter(CDC* pDC)
{

}

//打印报表头
void FaceInfoPrint::PrintFaceInfoHead(CDC* pDC)
{
	UINT ReportHeadFormat = DT_SINGLELINE|DT_CENTER|DT_VCENTER;
	UINT mutiFormat = ReportHeadFormat&(~DT_SINGLELINE);

	// 表格区域
	CRect rect(120, -530, 1970, -2440);
	
	// 行高(除最后的"附图"行,该行高度较大)
	int nRowHeight = 125;

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
	x1 = rect.left;
	x2 = rect.right;
	y1 = rect.top + 260;
	y2 = rect.top + 150;
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
	x1 = rect.left;
	x2 = x1+230;
	y1 = rect.top+nRowHeight;
	y2 = rect.top;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("工程名称:");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// 施工里程
	x1 = 850;
	x2 = x1+230;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("施工里程:");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// 日期
	x1 = 1470;
	x2 = x1+140;
	y1 = rect.top+nRowHeight;
	y2 = rect.top;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("日期:");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	//////////////////////////////////////////////////////////////////////////
	// 编号
	x1 = rect.left;
	x2 = 257;
	y1 = rect.top;
	y2 = rect.top-nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("编号");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);

	// 编号1~9
	for (int i=0; i<9; i++)
	{
		y1 = y2;
		y2 = y2-nRowHeight;
		CellRect.SetRect(x1,y1,x2,y2);
		CellString.Format("%d", i+1);
		pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	}
	
	// 附图
	y1 = y2;
	y2 = rect.bottom;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("附图");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);

	//////////////////////////////////////////////////////////////////////////
	// 项目名称
	x1 = x2;
	x2 = x1+250;
	y1 = rect.top;
	y2 = y1-nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("项目名称");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);

	// 掌子面尺寸
	y1 = y2;
	y2 = y1-nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("掌子面尺寸");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// 掌子面状态
	y1 = y2;
	y2 = y1-nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("掌子面状态");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);

	// 毛开挖面状态
	y1 = y2;
	y2 = y1-nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("毛开挖面状态");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// 岩石强度(MPa)
	y1 = y2;
	y2 = y1-nRowHeight;
	y1 -= 30;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("岩石强度\n(MPa)");
	pDC->DrawText(CellString,CellRect,mutiFormat);

	// 风化程度
	y1 = y2;
	y2 = y1-nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("风化程度");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);

	// 裂隙宽度(mm)
	y1 = y2;
	y2 = y1-nRowHeight;
	y1 -= 30;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("裂隙宽度\n(mm)");
	pDC->DrawText(CellString,CellRect,mutiFormat);

	// 裂隙形态
	y1 = y2;
	y2 = y1-nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("裂隙形态");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// 涌水状态
	y1 = y2;
	y2 = y1-nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("涌水状态");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// 掌子面级别划分
	y1 = y2;
	y2 = y1-nRowHeight;
	y1 -= 30;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("围岩级别\n划分");
	pDC->DrawText(CellString,CellRect,mutiFormat);
	
	// 掌子面其它描述
	x1 += 20;
	x2 = x1+300;
	y1 = y2;
	y2 = y1-60;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("掌子面其它描述:");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// 附掌子面照片
	x1 = 1115+20;
	x2 = x1+300;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("附掌子面照片:");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	//////////////////////////////////////////////////////////////////////////
	// 状态描述
	x1 = 507;
	x2 = rect.right;
	y1 = rect.top;
	y2 = y1-nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("状态描述");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// 开挖宽度(m)
	x2 = 645;
	y1 = y2-10;
	y2 = y1-nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("开挖\n宽度\n(m)");
	pDC->DrawText(CellString,CellRect,mutiFormat);
	//////////////////////////////////////////////////////////////////////////
	
	// 开挖高度(m)
	x1 = 755;
	x2 = x1+144;
	y1 = rect.top-nRowHeight;
	y2 = y1-nRowHeight;
	y1 -= 10;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("开挖\n高度\n(m)");
	pDC->DrawText(CellString,CellRect,mutiFormat);
	
	// 开挖面积(m)
	x1 = 1119;
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
	x1 = rect.left;
	x2 = x1+150;
	y1 = rect.bottom;
	y2 = y1-nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("制表:");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);

	// 复核
	x1 = 560;
	x2 = x1+130;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("复核:");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// 现场监理工程师
	x1 = 1030;
	x2 = x1+360;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("现场监理工程师:");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
		
	pDC->SelectObject(oldfont);	
}

// 打印报表记录
void FaceInfoPrint::PrintFaceInfoRecord(CDC* pDC, int nIdx)
{
	// 表格区域
	CRect rect(120, -530, 1970, -2440);
	
	// 行高(除最后的"附图"行,该行高度较大)
	int nRowHeight = 125;
	
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
	x1 = rect.left+210;
	x2 = x1+500;
	y1 = rect.top+nRowHeight;
	y2 = rect.top;
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
	
	// 日期
	x1 = 1600;
	x2 = rect.right;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = pFI->dateTime.Format("%Y-%m-%d");
	pDC->DrawText(CellString,CellRect,leftFormat);
	
	//////////////////////////////////////////////////////////////////////////
	// 开挖宽度(m)
	x1 = 645;
	x2 = x1+110;
	y1 = rect.top-nRowHeight;
	y2 = y1-nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	if (int(pFI->fFaceWidth*10)%10 != 0)
		CellString.Format("%d.%d", int(pFI->fFaceWidth), int(pFI->fFaceWidth*10)%10);
	else
		CellString.Format("%d", int(pFI->fFaceWidth));
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// 开挖高度(m)
	x1 = 899;
	x2 = x1+220;
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
	x2 = rect.right;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = pFI->strOther;
	pDC->DrawText(CellString,CellRect,DT_WORDBREAK);

	//////////////////////////////////////////////////////////////////////////
	// 掌子面状态
	// 稳定
	if (pFI->bFaceStatusWD)
	{
		x1 = 507;
		x2 = x1+248;
		y1 = rect.top-2*nRowHeight;
		y2 = y1-nRowHeight;
		CellRect.SetRect(x1,y1,x2,y2);
		CellString = pFI->strFaceStatusWD;
		pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	}
	// 正面掉块
	if (pFI->bFaceStatusZMDK)
	{
		x1 = 755;
		x2 = x1+364;
		y1 = rect.top-2*nRowHeight;
		y2 = y1-nRowHeight;
		CellRect.SetRect(x1,y1,x2,y2);
		CellString = pFI->strFaceStatusZMDK;
		pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	}
	// 正面挤出
	if (pFI->bFaceStatusZMJC)
	{
		x1 = 1119;
		x2 = x1+298;
		y1 = rect.top-2*nRowHeight;
		y2 = y1-nRowHeight;
		CellRect.SetRect(x1,y1,x2,y2);
		CellString = pFI->strFaceStatusZMJC;
		pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	}
	// 正面不能自稳
	if (pFI->bFaceStatusZMBNZW)
	{
		x1 = 1417;
		x2 = x1+279;
		y1 = rect.top-2*nRowHeight;
		y2 = y1-nRowHeight;
		CellRect.SetRect(x1,y1,x2,y2);
		CellString = pFI->strFaceStatusZMBNZW;
		pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	}
	// 其它
	if (pFI->bFaceStatusOther)
	{
		x1 = 1696;
		x2 = rect.right;
		y1 = rect.top-2*nRowHeight;
		y2 = y1-nRowHeight;
		CellRect.SetRect(x1+10,y1-10,x2,y2);
		CellString = _T("其它:")+pFI->strFaceStatusOtherDesc;
		pDC->DrawText(CellString,CellRect,DT_WORDBREAK);
	}
	//////////////////////////////////////////////////////////////////////////
	// 毛开挖面状态
	// 自稳
	if (pFI->bExcaveStatusZW)
	{
		x1 = 507;
		x2 = x1+248;
		y1 = rect.top-3*nRowHeight;
		y2 = y1-nRowHeight;
		CellRect.SetRect(x1,y1,x2,y2);
		CellString = pFI->strExcaveStatusZW;
		pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	}
	// 随时间松弛、掉块
	if (pFI->bExcaveStatusSCDK)
	{
		x1 = 755;
		x2 = x1+364;
		y1 = rect.top-3*nRowHeight;
		y2 = y1-nRowHeight;
		CellRect.SetRect(x1,y1,x2,y2);
		CellString = pFI->strExcaveStatusSCDK;
		pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	}
	// 自稳困难、要及时支护
	if (pFI->bExcaveStatusJSZH)
	{
		x1 = 1119;
		x2 = x1+298;
		y1 = rect.top-3*nRowHeight;
		y2 = y1-nRowHeight;
		y1 -= 30;
		CellRect.SetRect(x1,y1,x2,y2);
		CellString = pFI->strExcaveStatusJSZH.Left(12)+"\n"+pFI->strExcaveStatusJSZH.Right(8);
		pDC->DrawText(CellString,CellRect,mutiFormat);
	}
	// 要超前支护
	if (pFI->bExcaveStatusCQZH)
	{
		x1 = 1417;
		x2 = x1+279;
		y1 = rect.top-3*nRowHeight;
		y2 = y1-nRowHeight;
		CellRect.SetRect(x1,y1,x2,y2);
		CellString = pFI->strExcaveStatusCQZH;
		pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	}
	// 其它
	if (pFI->bExcaveStatusOther)
	{
		x1 = 1696;
		x2 = rect.right;
		y1 = rect.top-3*nRowHeight;
		y2 = y1-nRowHeight;
		CellRect.SetRect(x1+10,y1-10,x2,y2);
		CellString = _T("其它:")+pFI->strExcaveStatusOtherDesc;
		pDC->DrawText(CellString,CellRect,DT_WORDBREAK);
	}
	//////////////////////////////////////////////////////////////////////////
	// 岩石强度
	// 30~60(硬岩)
	if (pFI->bRockStrengthYY)
	{
		x1 = 507;
		x2 = x1+248;
		y1 = rect.top-4*nRowHeight;
		y2 = y1-nRowHeight;
		CellRect.SetRect(x1,y1,x2,y2);
		CellString = pFI->strRockStrengthYY;
		pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	}
	// 15~30(较软岩)
	if (pFI->bRockStrengthJORY)
	{
		x1 = 755;
		x2 = x1+364;
		y1 = rect.top-4*nRowHeight;
		y2 = y1-nRowHeight;
		CellRect.SetRect(x1,y1,x2,y2);
		CellString = pFI->strRockStrengthJORY;
		pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	}
	// 5~15(软岩)
	if (pFI->bRockStrengthRY)
	{
		x1 = 1119;
		x2 = x1+298;
		y1 = rect.top-4*nRowHeight;
		y2 = y1-nRowHeight;
		y1 -= 30;
		CellRect.SetRect(x1,y1,x2,y2);
		CellString = pFI->strRockStrengthRY;
		pDC->DrawText(CellString,CellRect,mutiFormat);
	}
	// <5(极软岩)
	if (pFI->bRockStrengthJIRY)
	{
		x1 = 1417;
		x2 = x1+279;
		y1 = rect.top-4*nRowHeight;
		y2 = y1-nRowHeight;
		CellRect.SetRect(x1,y1,x2,y2);
		CellString = pFI->strRockStrengthJIRY;
		pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	}
	// 其它
	if (pFI->bRockStrengthOther)
	{
		x1 = 1696;
		x2 = rect.right;
		y1 = rect.top-4*nRowHeight;
		y2 = y1-nRowHeight;
		CellRect.SetRect(x1+10,y1-10,x2,y2);
		CellString = _T("其它:")+pFI->strRockStrengthOtherDesc;
		pDC->DrawText(CellString,CellRect,DT_WORDBREAK);
	}
	//////////////////////////////////////////////////////////////////////////
	// 风化程度
	// 微风化
	if (pFI->bWeatheringWFH)
	{
		x1 = 507;
		x2 = x1+248;
		y1 = rect.top-5*nRowHeight;
		y2 = y1-nRowHeight;
		CellRect.SetRect(x1,y1,x2,y2);
		CellString = pFI->strWeatheringWFH;
		pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	}
	// 弱风化
	if (pFI->bWeatheringRFH)
	{
		x1 = 755;
		x2 = x1+364;
		y1 = rect.top-5*nRowHeight;
		y2 = y1-nRowHeight;
		CellRect.SetRect(x1,y1,x2,y2);
		CellString = pFI->strWeatheringRFH;
		pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	}
	// 强风化
	if (pFI->bWeatheringQIFH)
	{
		x1 = 1119;
		x2 = x1+298;
		y1 = rect.top-5*nRowHeight;
		y2 = y1-nRowHeight;
		CellRect.SetRect(x1,y1,x2,y2);
		CellString = pFI->strWeatheringQIFH;
		pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	}
	// 全风化
	if (pFI->bWeatheringQUFH)
	{
		x1 = 1417;
		x2 = x1+279;
		y1 = rect.top-5*nRowHeight;
		y2 = y1-nRowHeight;
		CellRect.SetRect(x1,y1,x2,y2);
		CellString = pFI->strWeatheringQUFH;
		pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	}
	// 其它
	if (pFI->bWeatheringOther)
	{
		x1 = 1696;
		x2 = rect.right;
		y1 = rect.top-5*nRowHeight;
		y2 = y1-nRowHeight;
		CellRect.SetRect(x1+10,y1-10,x2,y2);
		CellString = _T("其它:")+pFI->strWeatheringOtherDesc;
		pDC->DrawText(CellString,CellRect,DT_WORDBREAK);
	}
	//////////////////////////////////////////////////////////////////////////
	// 裂隙宽度(mm)
	// >5
	if (pFI->bCrackWidth5)
	{
		x1 = 507;
		x2 = x1+248;
		y1 = rect.top-6*nRowHeight;
		y2 = y1-nRowHeight;
		CellRect.SetRect(x1,y1,x2,y2);
		CellString = pFI->strCrackWidth5;
		pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	}
	// 3~5
	if (pFI->bCrackWidth3_5)
	{
		x1 = 755;
		x2 = x1+364;
		y1 = rect.top-6*nRowHeight;
		y2 = y1-nRowHeight;
		CellRect.SetRect(x1,y1,x2,y2);
		CellString = pFI->strCrackWidth3_5;
		pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	}
	// 1~3
	if (pFI->bCrackWidth1_3)
	{
		x1 = 1119;
		x2 = x1+298;
		y1 = rect.top-6*nRowHeight;
		y2 = y1-nRowHeight;
		CellRect.SetRect(x1,y1,x2,y2);
		CellString = pFI->strCrackWidth1_3;
		pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	}
	// <1
	if (pFI->bCrackWidth1)
	{
		x1 = 1417;
		x2 = x1+279;
		y1 = rect.top-6*nRowHeight;
		y2 = y1-nRowHeight;
		CellRect.SetRect(x1,y1,x2,y2);
		CellString = pFI->strCrackWidth1;
		pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	}
	// 其它
	if (pFI->bCrackWidthOther)
	{
		x1 = 1696;
		x2 = rect.right;
		y1 = rect.top-6*nRowHeight;
		y2 = y1-nRowHeight;
		CellRect.SetRect(x1+10,y1-10,x2,y2);
		CellString = _T("其它:")+pFI->strCrackWidthOtherDesc;
		pDC->DrawText(CellString,CellRect,DT_WORDBREAK);
	}
	//////////////////////////////////////////////////////////////////////////
	// 裂隙形态
	// 密集
	if (pFI->bCrackStatusMJ)
	{
		x1 = 507;
		x2 = x1+248;
		y1 = rect.top-7*nRowHeight;
		y2 = y1-nRowHeight;
		CellRect.SetRect(x1,y1,x2,y2);
		CellString = pFI->strCrackStatusMJ;
		pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	}
	// 部分张开
	if (pFI->bCrackStatusBFZK)
	{
		x1 = 755;
		x2 = x1+364;
		y1 = rect.top-7*nRowHeight;
		y2 = y1-nRowHeight;
		CellRect.SetRect(x1,y1,x2,y2);
		CellString = pFI->strCrackStatusBFZK;
		pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	}
	// 开口
	if (pFI->bCrackStatusKK)
	{
		x1 = 1119;
		x2 = x1+298;
		y1 = rect.top-7*nRowHeight;
		y2 = y1-nRowHeight;
		CellRect.SetRect(x1,y1,x2,y2);
		CellString = pFI->strCrackStatusKK;
		pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	}
	// 夹有粘土
	if (pFI->bCrackStatusJYLT)
	{
		x1 = 1417;
		x2 = x1+279;
		y1 = rect.top-7*nRowHeight;
		y2 = y1-nRowHeight;
		CellRect.SetRect(x1,y1,x2,y2);
		CellString = pFI->strCrackStatusJYLT;
		pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	}
	// 其它
	if (pFI->bCrackStatusOther)
	{
		x1 = 1696;
		x2 = rect.right;
		y1 = rect.top-7*nRowHeight;
		y2 = y1-nRowHeight;
		CellRect.SetRect(x1+10,y1-10,x2,y2);
		CellString = _T("其它:")+pFI->strCrackStatusOtherDesc;
		pDC->DrawText(CellString,CellRect,DT_WORDBREAK);
	}
	//////////////////////////////////////////////////////////////////////////
	// 涌水状态
	// 无水
	if (pFI->bGushingStatusWS)
	{
		x1 = 507;
		x2 = x1+248;
		y1 = rect.top-8*nRowHeight;
		y2 = y1-nRowHeight;
		CellRect.SetRect(x1,y1,x2,y2);
		CellString = pFI->strGushingStatusWS;
		pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	}
	// 渗水
	if (pFI->bGushingStatusSS)
	{
		x1 = 755;
		x2 = x1+364;
		y1 = rect.top-8*nRowHeight;
		y2 = y1-nRowHeight;
		CellRect.SetRect(x1,y1,x2,y2);
		CellString = pFI->strGushingStatusSS;
		pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	}
	// 整体湿润
	if (pFI->bGushingStatusZTSR)
	{
		x1 = 1119;
		x2 = x1+298;
		y1 = rect.top-8*nRowHeight;
		y2 = y1-nRowHeight;
		CellRect.SetRect(x1,y1,x2,y2);
		CellString = pFI->strGushingStatusZTSR;
		pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	}
	// 涌出或喷出
	if (pFI->bGushingStatusYCPC)
	{
		x1 = 1417;
		x2 = x1+279;
		y1 = rect.top-8*nRowHeight;
		y2 = y1-nRowHeight;
		CellRect.SetRect(x1,y1,x2,y2);
		CellString = pFI->strGushingStatusYCPC;
		pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	}
	// 特别大
	if (pFI->bGushingStatusTBD)
	{
		x1 = 1696;
		x2 = rect.right;
		y1 = rect.top-8*nRowHeight;
		y2 = y1-nRowHeight;
		CellRect.SetRect(x1,y1,x2,y2);
		CellString = pFI->strGushingStatusTBD;
		pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	}
	//////////////////////////////////////////////////////////////////////////
	// 围岩级别划分
	x1 = 507+10;
	x2 = rect.right;
	y1 = rect.top-9*nRowHeight;
	y2 = y1-nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("围岩设计级别:")+pFI->strDesignGrade+_T(",围岩施工级别:")+pFI->strConstructGrade;
	pDC->DrawText(CellString,CellRect,leftFormat);
	
	// 掌子面其它描述
	x1 = 257+20;
	x2 = x1+862-20;
	y1 = y1-nRowHeight-80;
	y2 = rect.bottom;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = "    " + pFI->strFaceDesc;
	// 加入换行符
	pDC->DrawText(CellString,CellRect,DT_WORDBREAK);

	// 根据设置确定是打印掌子面照片还是素描图形
	x1 = 1119+20;
	x2 = rect.right-30;
	y1 = rect.top-10*nRowHeight-10;
	y2 = rect.bottom+80;
	CellRect.SetRect(x1,y1,x2+20,y2-60);
	long double dbZoomX, dbZoomY;
	if (m_nPrintTarget == 0) // 打印掌子面照片
	{
		// 掌子面照片
		CTilImage* pImage = pFI->pCanvas->GetImage();
		dbZoomX = abs(CellRect.Width())*1.0/pImage->GetWidth();
		dbZoomY = abs(CellRect.Height())*1.0/pImage->GetHeight();
		pImage->SetZoomFactor(dbZoomX, dbZoomY);
		pImage->Stretch(pDC, CellRect);
	}
	else // m_nPrintTarget == 1, 即打印掌子面素描图形
	{
		// 掌子面素描图形
		CTilGraph* pGraph = pFI->pCanvas->GetGraph();
		// 在打印时重新设置缩放比例后显示
		dbZoomX = abs(CellRect.Width())*1.0/pGraph->GetWidth();
		dbZoomY = abs(CellRect.Height())*1.0/pGraph->GetHeight();
		pGraph->SetZoomFactor(dbZoomX, dbZoomY);
		pGraph->DrawInvert(pDC, CellRect.TopLeft());
	}

	//////////////////////////////////////////////////////////////////////////
	// 相关人员
	// 制表
	x1 = 255;
	x2 = x1+270;
	y1 = rect.bottom;
	y2 = y1-nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = pFI->strRecStaff;
	pDC->DrawText(CellString,CellRect,leftFormat);
	
	// 复核
	x1 = 680;
	x2 = x1+300;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = pFI->strRecReview;
	pDC->DrawText(CellString,CellRect,leftFormat);
	
	// 现场监理工程师
	x1 = 1360;
	x2 = x1+500;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = pFI->strRecSupervision;
	pDC->DrawText(CellString,CellRect,leftFormat);	

	pDC->SelectObject(oldfont);
}

// 画打印报表表格
void FaceInfoPrint::PrintFaceInfoGrid(CDC* pDC)
{
	//表格纵横
	int y;
	
	// 表格区域
	CRect rect(120, -530, 1970, -2440);
	
	// 行高(除最后的"附图"行,该行高度较大)
	int nRowHeight = 125;
	
	// 画水平线
	for (int i=0; i<11; i++)
	{
		y = rect.top-i*nRowHeight;
		DrawHLine(y, rect.left, rect.right, pDC);
	}
	// 画最后一条水平线
	DrawHLine(rect.bottom, rect.left, rect.right, pDC);
	
	// 画竖直线
	// 左边界
	DrawVLine(rect.left, rect.top, rect.bottom, pDC);
	// "编号"的右边界
	DrawVLine(257, rect.top, rect.bottom, pDC);
	// "项目名称"的右边界
	DrawVLine(507, rect.top, rect.top-10*nRowHeight, pDC);
	// "开挖宽度"的右边界
	DrawVLine(645, rect.top-nRowHeight, rect.top-2*nRowHeight, pDC);
	// "开挖宽度(m)"右侧数字栏的右边界
	DrawVLine(755, rect.top-nRowHeight, rect.top-9*nRowHeight, pDC);
	// "开挖高度"的右边界
	DrawVLine(899, rect.top-nRowHeight, rect.top-2*nRowHeight, pDC);
	// "开挖高度"右侧数字栏的右边界
	DrawVLine(1119, rect.top-nRowHeight, rect.top-9*nRowHeight, pDC);
	// "开挖面积"的右边界
	DrawVLine(1263, rect.top-nRowHeight, rect.top-2*nRowHeight, pDC);
	// "开挖面积"右侧数字栏的右边界
	DrawVLine(1417, rect.top-nRowHeight, rect.top-9*nRowHeight, pDC);
	// "开挖方式"的右边界
	DrawVLine(1510, rect.top-nRowHeight, rect.top-2*nRowHeight, pDC);
	// "开挖方式"右侧栏的右边界
	DrawVLine(1696, rect.top-nRowHeight, rect.top-9*nRowHeight, pDC);
	// "其它"栏的右边界
	DrawVLine(1800, rect.top-nRowHeight, rect.top-2*nRowHeight, pDC);
	// 最右边界
	DrawVLine(rect.right, rect.top, rect.bottom, pDC);
	// 下方"掌子面其它描述"右侧边界
	DrawVLine(1119, rect.top-10*nRowHeight, rect.bottom, pDC);
}

//画竖直直线
void   FaceInfoPrint::DrawVLine(int x_up, int y_up, int y_bottom,CDC* pDC)
{
	pDC->MoveTo(x_up, y_up);
	pDC->LineTo(x_up,y_bottom);
}

//画水平直线
void   FaceInfoPrint::DrawHLine(int y_left,int x_left, int x_right,CDC* pDC)
{
	pDC->MoveTo(x_left, y_left);
	pDC->LineTo( x_right,y_left);
}

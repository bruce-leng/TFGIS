#include "StdAfx.h"
#include "guioutput.h"
#include "resource.h"


CLogInfo	CGuiOutPut::m_EditOutput;
CListCtrl CGuiOutPut::m_ImgCaption;
CComboListCtrl CGuiOutPut::m_LayerInfo;

CGuiOutPut::CGuiOutPut(void)
{
}

CGuiOutPut::~CGuiOutPut(void)
{
}
BEGIN_MESSAGE_MAP(CGuiOutPut, CGuiControlBar)
	ON_WM_CREATE()
END_MESSAGE_MAP()

int CGuiOutPut::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CGuiControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_TabSolExplorer.Create(WS_VISIBLE|WS_CHILD,CRect(0,0,0,0),this,0x9999))
		return -1;

	DWORD dwStyleReport = LVS_REPORT|LVS_SHOWSELALWAYS|LVS_ALIGNTOP|LVS_SINGLESEL|LVS_AUTOARRANGE;
	if (!m_ImgCaption.Create(dwStyleReport,CRect(0,0,0,0), &m_TabSolExplorer, 2))
		return -1;

	dwStyleReport |= LVS_EDITLABELS;
	if (!m_LayerInfo.Create(dwStyleReport,CRect(0,0,0,0), &m_TabSolExplorer, 2))
		return -1;

	DWORD dwStyleListBox = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;
	if (!m_EditOutput.Create(dwStyleListBox,CRect(0,0,0,0), &m_TabSolExplorer, 2))
		return -1;

	/*
	if (!m_EditFind.Create(WS_CHILD|WS_VISIBLE,CRect(0,0,0,0), &m_TabSolExplorer, 2))
		return -1;
	*/

	if (!m_Pattern.Create(WS_CHILD|WS_VISIBLE,CRect(0,0,0,0), &m_TabSolExplorer, 2))
		return -1;
	m_Pattern.Initialize();

	set_tab_sol_explorer_info();
	set_image_caption_info();
	set_layer_info();

	return 0;
}

void CGuiOutPut::set_tab_sol_explorer_info(void)
{
	m_TabSolExplorer.SetImageList(IDB_DBPROJECTS, 16,8, RGB (255, 0, 0));
	m_TabSolExplorer.Addtab(&m_EditOutput,"状态信息",1);
	m_TabSolExplorer.Addtab(&m_ImgCaption,"图像属性",2);
	m_TabSolExplorer.Addtab(&m_LayerInfo,"特征参数",3);
	//m_TabSolExplorer.Addtab(&m_Pattern,"图元",4);
}

void CGuiOutPut::set_image_caption_info(void)
{
	m_ImgCaption.InsertColumn(0, "属性");
	m_ImgCaption.InsertColumn(1, "参数");
	m_ImgCaption.InsertItem(0, "宽度");
	m_ImgCaption.InsertItem(1, "高度");
	m_ImgCaption.InsertItem(2, "颜色");
	m_ImgCaption.InsertItem(3, "文件名");

	m_ImgCaption.SetColumnWidth(0, -1);
	m_ImgCaption.SetColumnWidth(1, -1);
}

void CGuiOutPut::set_layer_info(void)
{
	m_LayerInfo.InsertColumn(0, "掌子面里程", LVCFMT_LEFT, 80);
// 	m_LayerInfo.InsertColumn(1, "开挖方式", LVCFMT_LEFT, 60);
// 	m_LayerInfo.InsertColumn(2, "隧道埋深(m)", LVCFMT_LEFT, 80);
// 	m_LayerInfo.InsertColumn(3, "围岩岩性", LVCFMT_LEFT, 120);
	m_LayerInfo.InsertColumn(1, "单位面积裂隙长度(m/m2)", LVCFMT_LEFT, 150);
	m_LayerInfo.InsertColumn(2, "平均裂隙间距(m)", LVCFMT_LEFT, 110);
	m_LayerInfo.InsertColumn(3, "RQD(%)", LVCFMT_LEFT, 50);
	m_LayerInfo.InsertColumn(4, "单位面积割线段段数", LVCFMT_LEFT, 80);
	m_LayerInfo.InsertColumn(5, "单位面积边界线条数", LVCFMT_LEFT, 80);
	m_LayerInfo.InsertColumn(6, "岩体破碎程度评价指标", LVCFMT_LEFT, 80);
	m_LayerInfo.InsertColumn(7, "掌子面灰度均值", LVCFMT_LEFT, 80);
	m_LayerInfo.InsertColumn(8, "掌子面灰度均方差", LVCFMT_LEFT, 80);
	m_LayerInfo.InsertColumn(9, "掌子面岩体风化程度评价指标", LVCFMT_LEFT, 80);
	m_LayerInfo.InsertColumn(10, "掌子面岩体评价", LVCFMT_LEFT, 400);

	/*
	m_LayerInfo.InsertColumn(7, "组1视倾角", LVCFMT_LEFT, 80);
	m_LayerInfo.InsertColumn(8, "组2视倾角", LVCFMT_LEFT, 80);
	m_LayerInfo.InsertColumn(9, "组3视倾角", LVCFMT_LEFT, 80);
	m_LayerInfo.InsertColumn(10, "组4视倾角", LVCFMT_LEFT, 80);
	*/

	// 设置只读属性
	m_LayerInfo.SetReadOnlyColumns(0);
	m_LayerInfo.SetReadOnlyColumns(1);

	// 设置掩码
	CString strValidChars = "0123456789.";
	for (int i = 2; i <= 10; i ++)
	{	//digital only edit
		m_LayerInfo.SetColumnValidEditCtrlCharacters(strValidChars,i);
	}

	m_LayerInfo.EnableVScroll(); 			
	m_LayerInfo.SetExtendedStyle(LVS_EX_FULLROWSELECT);
}

void LogInfo(const CString& str)
{
	CGuiOutPut::m_EditOutput.AddString(str);
	int nCount = CGuiOutPut::m_EditOutput.GetCount();
	if (nCount > 0)
		CGuiOutPut::m_EditOutput.SetCurSel(nCount-1);
}

void ImageInfo(int nWidth, int nHeight, int nDepth, const CString& strFileName)
{
	CString str;
	str.Format("%d", nWidth);
	CGuiOutPut::m_ImgCaption.SetItemText(0, 1, str);

	str.Format("%d", nHeight);
	CGuiOutPut::m_ImgCaption.SetItemText(1, 1, str);

	str.Format("%d位", nDepth);
	CGuiOutPut::m_ImgCaption.SetItemText(2, 1, str);

	CGuiOutPut::m_ImgCaption.SetItemText(3, 1, strFileName);

	CGuiOutPut::m_ImgCaption.SetColumnWidth(0, -1);
	CGuiOutPut::m_ImgCaption.SetColumnWidth(1, -1);
}

void ObjectInfo(int nLayNo, int nObjNo,  float fRealArea,
				float fCenX, float fCenY, float fLeft, float fRight,
				float fTop,	float fBottom,float fGraySum,
				float fGrayAve, float fGrayMax, float fGrayMin)
{
	int i= CGuiOutPut::m_LayerInfo.GetItemCount();

	// 重新加入对象参数时先清除掉以前的内容
	if ((0 == nLayNo) && (0 == nObjNo))
	{
		CGuiOutPut::m_LayerInfo.DeleteAllItems();
		i = 0;
	}

	CString str;
	str.Format("%d", nLayNo);
	CGuiOutPut::m_LayerInfo.InsertItem(i, str);

	str.Format("%d", nObjNo);
	CGuiOutPut::m_LayerInfo.SetItemText(i, 1, str);

	str.Format("%f", fRealArea);
	CGuiOutPut::m_LayerInfo.SetItemText(i, 2, str);

	str.Format("%f", fCenX);
	CGuiOutPut::m_LayerInfo.SetItemText(i, 3, str);

	str.Format("%f", fCenY);
	CGuiOutPut::m_LayerInfo.SetItemText(i, 4, str);

	str.Format("%f", fLeft);
	CGuiOutPut::m_LayerInfo.SetItemText(i, 5, str);

	str.Format("%f", fRight);
	CGuiOutPut::m_LayerInfo.SetItemText(i, 6, str);

	str.Format("%f", fTop);
	CGuiOutPut::m_LayerInfo.SetItemText(i, 7, str);

	str.Format("%f", fBottom);
	CGuiOutPut::m_LayerInfo.SetItemText(i, 8, str);

	str.Format("%d", static_cast<int>(fGraySum));
	CGuiOutPut::m_LayerInfo.SetItemText(i, 9, str);

	str.Format("%f", fGrayAve);
	CGuiOutPut::m_LayerInfo.SetItemText(i, 10, str);

	str.Format("%d", static_cast<int>(fGrayMax));
	CGuiOutPut::m_LayerInfo.SetItemText(i, 11, str);

	str.Format("%d", static_cast<int>(fGrayMin));
	CGuiOutPut::m_LayerInfo.SetItemText(i, 12, str);

	i ++;
}

// 显示掌子面属性
void FaceCaptionInfo(deque<CFaceCaption*>& dFaceCaption)
{
	CGuiOutPut::m_LayerInfo.DeleteAllItems();

	CString str;
	int nCount = dFaceCaption.size();
	for (int i=0; i<nCount; i++)
	{
		// 掌子面里程
		CGuiOutPut::m_LayerInfo.InsertItem(i, dFaceCaption[i]->m_strDK);
// 		// 施工方法
// 		CGuiOutPut::m_LayerInfo.SetItemText(i, 1, dFaceCaption[i]->m_strMethod);
// 		// 隧道埋深
// 		if (dFaceCaption[i]->m_fDepth>=0)
// 		{
// 			str.Format("%6.1f", dFaceCaption[i]->m_fDepth);
// 			CGuiOutPut::m_LayerInfo.SetItemText(i, 2, str);
// 		}
// 		// 围岩岩性
// 		CGuiOutPut::m_LayerInfo.SetItemText(i, 3, dFaceCaption[i]->m_strLith);

		// 单位面积平均节理长度(m/m2)
		if (dFaceCaption[i]->m_fAveJointLen>=0)
		{
			str.Format("%5.2f", dFaceCaption[i]->m_fAveJointLen);
			CGuiOutPut::m_LayerInfo.SetItemText(i, 1, str);
		}

		// 平均裂隙间距(m)
		if (dFaceCaption[i]->m_fAveRockSize>=0)
		{
			str.Format("%5.2f", dFaceCaption[i]->m_fAveRockSize);
			CGuiOutPut::m_LayerInfo.SetItemText(i, 2, str);
		}

		// RQD(岩石质量指标)
		if (dFaceCaption[i]->m_fRQD>=0)
		{
			str.Format("%3.0f", dFaceCaption[i]->m_fRQD*100);
			CGuiOutPut::m_LayerInfo.SetItemText(i, 3, str+'%');
		}

		// 单位面积的割线段段数目
		if (dFaceCaption[i]->m_fDividLines>=0)
		{
			str.Format("%5.2f", dFaceCaption[i]->m_fDividLines);
			CGuiOutPut::m_LayerInfo.SetItemText(i, 4, str);
		}

		// 单位面积边界线条数
		if (dFaceCaption[i]->m_fEdgeLines>=0)
		{
			str.Format("%5.2f", dFaceCaption[i]->m_fEdgeLines);
			CGuiOutPut::m_LayerInfo.SetItemText(i, 5, str);
		}

		// 岩体破碎质量指标
		if (dFaceCaption[i]->m_fRockCrack>=0)
		{
			str.Format("%5.2f", dFaceCaption[i]->m_fRockCrack);
			CGuiOutPut::m_LayerInfo.SetItemText(i, 6, str);
		}

		// 掌子面平均灰度值
		if (dFaceCaption[i]->m_fRQD>=0)
		{
			str.Format("(%5.2f,%5.2f,%5.2f)",
				dFaceCaption[i]->m_fAveRed,
				dFaceCaption[i]->m_fAveGreen,
				dFaceCaption[i]->m_fAveBlue);
			CGuiOutPut::m_LayerInfo.SetItemText(i, 7, str);
		}

		// 掌子面灰度均方差
		if (dFaceCaption[i]->m_fRQD>=0)
		{
			str.Format("(%5.2f,%5.2f,%5.2f)",
				dFaceCaption[i]->m_fMean2DevRed,
				dFaceCaption[i]->m_fMean2DevGreen,
				dFaceCaption[i]->m_fMean2DevBlue);
			CGuiOutPut::m_LayerInfo.SetItemText(i, 8, str);
		}

		// 岩体破碎质量指标
		if (dFaceCaption[i]->m_fRockCrack>=0)
		{
			str.Format("(%5.2f,%5.2f,%5.2f)",
				dFaceCaption[i]->m_fRockWeathRed,
				dFaceCaption[i]->m_fRockWeathGreen,
				dFaceCaption[i]->m_fRockWeathBlue);
			CGuiOutPut::m_LayerInfo.SetItemText(i, 9, str);
		}

		// 掌子面岩体描述
		CGuiOutPut::m_LayerInfo.SetItemText(i, 10, dFaceCaption[i]->m_strDesc);

		/*
		// 组1视倾角
		if (dFaceCaption[i]->m_fAngle1>=0)
		{
			str.Format("%4.1f", dFaceCaption[i]->m_fAngle1);
			CGuiOutPut::m_LayerInfo.SetItemText(i, 7, str);
		}

		// 组2视倾角
		if (dFaceCaption[i]->m_fAngle2>=0)
		{
			str.Format("%4.1f", dFaceCaption[i]->m_fAngle2);
			CGuiOutPut::m_LayerInfo.SetItemText(i, 8, str);
		}

		// 组3视倾角
		if (dFaceCaption[i]->m_fAngle3>=0)
		{
			str.Format("%4.1f", dFaceCaption[i]->m_fAngle3);
			CGuiOutPut::m_LayerInfo.SetItemText(i, 9, str);
		}

		// 组4视倾角
		if (dFaceCaption[i]->m_fAngle4>=0)
		{
			str.Format("%4.1f", dFaceCaption[i]->m_fAngle4);
			CGuiOutPut::m_LayerInfo.SetItemText(i, 10, str);
		}
		*/
	}
}

// 删除所有掌子面特征信息
void FaceCaptionInfoClear(deque<CFaceCaption*>& dFaceCaption)
{
	int nNum = dFaceCaption.size();
	for (int i=0; i<nNum; i++)
	{
		delete dFaceCaption[i];
	}
	dFaceCaption.clear();

	CGuiOutPut::m_LayerInfo.DeleteAllItems();
	CGuiOutPut::m_LayerInfo.UpdateData(FALSE);
	CGuiOutPut::m_LayerInfo.Invalidate(TRUE);
	CGuiOutPut::m_LayerInfo.UpdateWindow();
}

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
	m_TabSolExplorer.Addtab(&m_EditOutput,"״̬��Ϣ",1);
	m_TabSolExplorer.Addtab(&m_ImgCaption,"ͼ������",2);
	m_TabSolExplorer.Addtab(&m_LayerInfo,"��������",3);
	//m_TabSolExplorer.Addtab(&m_Pattern,"ͼԪ",4);
}

void CGuiOutPut::set_image_caption_info(void)
{
	m_ImgCaption.InsertColumn(0, "����");
	m_ImgCaption.InsertColumn(1, "����");
	m_ImgCaption.InsertItem(0, "���");
	m_ImgCaption.InsertItem(1, "�߶�");
	m_ImgCaption.InsertItem(2, "��ɫ");
	m_ImgCaption.InsertItem(3, "�ļ���");

	m_ImgCaption.SetColumnWidth(0, -1);
	m_ImgCaption.SetColumnWidth(1, -1);
}

void CGuiOutPut::set_layer_info(void)
{
	m_LayerInfo.InsertColumn(0, "���������", LVCFMT_LEFT, 80);
// 	m_LayerInfo.InsertColumn(1, "���ڷ�ʽ", LVCFMT_LEFT, 60);
// 	m_LayerInfo.InsertColumn(2, "�������(m)", LVCFMT_LEFT, 80);
// 	m_LayerInfo.InsertColumn(3, "Χ������", LVCFMT_LEFT, 120);
	m_LayerInfo.InsertColumn(1, "��λ�����϶����(m/m2)", LVCFMT_LEFT, 150);
	m_LayerInfo.InsertColumn(2, "ƽ����϶���(m)", LVCFMT_LEFT, 110);
	m_LayerInfo.InsertColumn(3, "RQD(%)", LVCFMT_LEFT, 50);
	m_LayerInfo.InsertColumn(4, "��λ������߶ζ���", LVCFMT_LEFT, 80);
	m_LayerInfo.InsertColumn(5, "��λ����߽�������", LVCFMT_LEFT, 80);
	m_LayerInfo.InsertColumn(6, "��������̶�����ָ��", LVCFMT_LEFT, 80);
	m_LayerInfo.InsertColumn(7, "������ҶȾ�ֵ", LVCFMT_LEFT, 80);
	m_LayerInfo.InsertColumn(8, "������ҶȾ�����", LVCFMT_LEFT, 80);
	m_LayerInfo.InsertColumn(9, "����������绯�̶�����ָ��", LVCFMT_LEFT, 80);
	m_LayerInfo.InsertColumn(10, "��������������", LVCFMT_LEFT, 400);

	/*
	m_LayerInfo.InsertColumn(7, "��1�����", LVCFMT_LEFT, 80);
	m_LayerInfo.InsertColumn(8, "��2�����", LVCFMT_LEFT, 80);
	m_LayerInfo.InsertColumn(9, "��3�����", LVCFMT_LEFT, 80);
	m_LayerInfo.InsertColumn(10, "��4�����", LVCFMT_LEFT, 80);
	*/

	// ����ֻ������
	m_LayerInfo.SetReadOnlyColumns(0);
	m_LayerInfo.SetReadOnlyColumns(1);

	// ��������
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

	str.Format("%dλ", nDepth);
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

	// ���¼���������ʱ���������ǰ������
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

// ��ʾ����������
void FaceCaptionInfo(deque<CFaceCaption*>& dFaceCaption)
{
	CGuiOutPut::m_LayerInfo.DeleteAllItems();

	CString str;
	int nCount = dFaceCaption.size();
	for (int i=0; i<nCount; i++)
	{
		// ���������
		CGuiOutPut::m_LayerInfo.InsertItem(i, dFaceCaption[i]->m_strDK);
// 		// ʩ������
// 		CGuiOutPut::m_LayerInfo.SetItemText(i, 1, dFaceCaption[i]->m_strMethod);
// 		// �������
// 		if (dFaceCaption[i]->m_fDepth>=0)
// 		{
// 			str.Format("%6.1f", dFaceCaption[i]->m_fDepth);
// 			CGuiOutPut::m_LayerInfo.SetItemText(i, 2, str);
// 		}
// 		// Χ������
// 		CGuiOutPut::m_LayerInfo.SetItemText(i, 3, dFaceCaption[i]->m_strLith);

		// ��λ���ƽ��������(m/m2)
		if (dFaceCaption[i]->m_fAveJointLen>=0)
		{
			str.Format("%5.2f", dFaceCaption[i]->m_fAveJointLen);
			CGuiOutPut::m_LayerInfo.SetItemText(i, 1, str);
		}

		// ƽ����϶���(m)
		if (dFaceCaption[i]->m_fAveRockSize>=0)
		{
			str.Format("%5.2f", dFaceCaption[i]->m_fAveRockSize);
			CGuiOutPut::m_LayerInfo.SetItemText(i, 2, str);
		}

		// RQD(��ʯ����ָ��)
		if (dFaceCaption[i]->m_fRQD>=0)
		{
			str.Format("%3.0f", dFaceCaption[i]->m_fRQD*100);
			CGuiOutPut::m_LayerInfo.SetItemText(i, 3, str+'%');
		}

		// ��λ����ĸ��߶ζ���Ŀ
		if (dFaceCaption[i]->m_fDividLines>=0)
		{
			str.Format("%5.2f", dFaceCaption[i]->m_fDividLines);
			CGuiOutPut::m_LayerInfo.SetItemText(i, 4, str);
		}

		// ��λ����߽�������
		if (dFaceCaption[i]->m_fEdgeLines>=0)
		{
			str.Format("%5.2f", dFaceCaption[i]->m_fEdgeLines);
			CGuiOutPut::m_LayerInfo.SetItemText(i, 5, str);
		}

		// ������������ָ��
		if (dFaceCaption[i]->m_fRockCrack>=0)
		{
			str.Format("%5.2f", dFaceCaption[i]->m_fRockCrack);
			CGuiOutPut::m_LayerInfo.SetItemText(i, 6, str);
		}

		// ������ƽ���Ҷ�ֵ
		if (dFaceCaption[i]->m_fRQD>=0)
		{
			str.Format("(%5.2f,%5.2f,%5.2f)",
				dFaceCaption[i]->m_fAveRed,
				dFaceCaption[i]->m_fAveGreen,
				dFaceCaption[i]->m_fAveBlue);
			CGuiOutPut::m_LayerInfo.SetItemText(i, 7, str);
		}

		// ������ҶȾ�����
		if (dFaceCaption[i]->m_fRQD>=0)
		{
			str.Format("(%5.2f,%5.2f,%5.2f)",
				dFaceCaption[i]->m_fMean2DevRed,
				dFaceCaption[i]->m_fMean2DevGreen,
				dFaceCaption[i]->m_fMean2DevBlue);
			CGuiOutPut::m_LayerInfo.SetItemText(i, 8, str);
		}

		// ������������ָ��
		if (dFaceCaption[i]->m_fRockCrack>=0)
		{
			str.Format("(%5.2f,%5.2f,%5.2f)",
				dFaceCaption[i]->m_fRockWeathRed,
				dFaceCaption[i]->m_fRockWeathGreen,
				dFaceCaption[i]->m_fRockWeathBlue);
			CGuiOutPut::m_LayerInfo.SetItemText(i, 9, str);
		}

		// ��������������
		CGuiOutPut::m_LayerInfo.SetItemText(i, 10, dFaceCaption[i]->m_strDesc);

		/*
		// ��1�����
		if (dFaceCaption[i]->m_fAngle1>=0)
		{
			str.Format("%4.1f", dFaceCaption[i]->m_fAngle1);
			CGuiOutPut::m_LayerInfo.SetItemText(i, 7, str);
		}

		// ��2�����
		if (dFaceCaption[i]->m_fAngle2>=0)
		{
			str.Format("%4.1f", dFaceCaption[i]->m_fAngle2);
			CGuiOutPut::m_LayerInfo.SetItemText(i, 8, str);
		}

		// ��3�����
		if (dFaceCaption[i]->m_fAngle3>=0)
		{
			str.Format("%4.1f", dFaceCaption[i]->m_fAngle3);
			CGuiOutPut::m_LayerInfo.SetItemText(i, 9, str);
		}

		// ��4�����
		if (dFaceCaption[i]->m_fAngle4>=0)
		{
			str.Format("%4.1f", dFaceCaption[i]->m_fAngle4);
			CGuiOutPut::m_LayerInfo.SetItemText(i, 10, str);
		}
		*/
	}
}

// ɾ������������������Ϣ
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

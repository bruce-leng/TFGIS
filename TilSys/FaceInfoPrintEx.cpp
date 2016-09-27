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
	// �������
	m_Rect.SetRect(120, -350, 1970, -2680);
	// ����������и�(������"��ͼ"��,���и߶Ƚϴ�)
	m_nRowHeight = 125;
}

FaceInfoPrintEx::~FaceInfoPrintEx()
{

}

//��ӡҳüҳ��
void FaceInfoPrintEx::PrintHeaderFooter(CDC* pDC)
{

}

//��ӡ����ͷ
void FaceInfoPrintEx::PrintFaceInfoHead(CDC* pDC)
{
	UINT ReportHeadFormat = DT_SINGLELINE|DT_CENTER|DT_VCENTER;
	UINT mutiFormat = ReportHeadFormat&(~DT_SINGLELINE);

	//��Ԫ��
	CRect CellRect;
	CString CellString;
	int x1,y1,x2,y2;

	//����
	CFont *oldfont;
	//����ԭ����
	oldfont=pDC->GetCurrentFont();
	
	//////////////////////////////////////////////////////////////////////////
	// ������������
	CFont TitleFont;
	TitleFont.CreatePointFont(220,_T("����"),pDC);
	pDC->SelectObject(&TitleFont);

	// ����"��������������¼��"
	x1 = m_Rect.left;
	x2 = m_Rect.right;
	y1 = m_Rect.top + 260;
	y2 = m_Rect.top + 150;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("��������������¼��");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);

	//////////////////////////////////////////////////////////////////////////
	//������ͷ����
	CFont MyFont;
	MyFont.CreatePointFont(105,_T("����"),pDC);
	// ����ͷ����Ӵ�
// 	LOGFONT logFont;
// 	MyFont.GetLogFont(&logFont);
// 	logFont.lfWeight = FW_BOLD;
// 	CFont MyFont1;
// 	MyFont1.CreateFontIndirect(&logFont);
	pDC->SelectObject(&MyFont);
	
	// ��������
	x1 = m_Rect.left;
	x2 = x1+230;
	y1 = m_Rect.top+m_nRowHeight;
	y2 = m_Rect.top;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("��������:");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// ʩ�����
	x1 = 850;
	x2 = x1+230;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("ʩ�����:");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// ���
	x1 = 1470;
	x2 = x1+140;
	y1 = m_Rect.top+m_nRowHeight;
	y2 = m_Rect.top;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("���:");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	//////////////////////////////////////////////////////////////////////////
	// ���
	x1 = m_Rect.left;
	x2 = 257;
	y1 = m_Rect.top;
	y2 = m_Rect.top-m_nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("���");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);

	// ���1~9
	for (int i=0; i<9; i++)
	{
		y1 = y2;
		y2 = y2-m_nRowHeight;
		CellRect.SetRect(x1,y1,x2,y2);
		CellString.Format("%d", i+1);
		pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	}
	
	// ��ͼ
	y1 = y2;
	y2 = m_Rect.bottom;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("��ͼ");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);

	//////////////////////////////////////////////////////////////////////////
	// ��Ŀ����
	x1 = x2;
	x2 = x1+250;
	y1 = m_Rect.top;
	y2 = y1-m_nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("��Ŀ����");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);

	// ������ߴ�
	y1 = y2;
	y2 = y1-m_nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("������ߴ�");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// ������״̬
	y1 = y2;
	y2 = y1-m_nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("������״̬");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);

	// Χ������
	y1 = y2;
	y2 = y1-m_nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("Χ������");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// ��ʯǿ��(MPa)
	y1 = y2;
	y2 = y1-m_nRowHeight;
	y1 -= 30;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("��ʯǿ��\n(MPa)");
	pDC->DrawText(CellString,CellRect,mutiFormat);

	// �绯�̶�
	y1 = y2;
	y2 = y1-m_nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("�绯�̶�");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);

	// ��϶���(mm)
	y1 = y2;
	y2 = y1-m_nRowHeight;
	y1 -= 30;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("��϶���\n(mm)");
	pDC->DrawText(CellString,CellRect,mutiFormat);

	// ��϶��̬
	y1 = y2;
	y2 = y1-m_nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("��϶��̬");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// ӿˮ״̬
	y1 = y2;
	y2 = y1-m_nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("ӿˮ״̬");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// �����漶�𻮷�
	y1 = y2;
	y2 = y1-m_nRowHeight;
	y1 -= 30;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("Χ�Ҽ���\n����");
	pDC->DrawText(CellString,CellRect,mutiFormat);
	
	// ����������Ƭ
// 	x1 += 20;
// 	x2 = x1+300;
// 	y1 = y2;
// 	y2 = y1-60;
// 	CellRect.SetRect(x1,y1,x2,y2);
// 	CellString = _T("��������Ƭ");
// 	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// ��������������
// 	x1 = 1115+20;
// 	x2 = x1+300;
// 	CellRect.SetRect(x1,y1,x2,y2);
// 	CellString = _T("����������Ƭ:");
// 	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	//////////////////////////////////////////////////////////////////////////
	// ״̬����
	x1 = 507;
	x2 = m_Rect.right;
	y1 = m_Rect.top;
	y2 = y1-m_nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("״̬����");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// ���ڿ��(m)
	x2 = 645;
	y1 = y2-10;
	y2 = y1-m_nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("����\n���\n(m)");
	pDC->DrawText(CellString,CellRect,mutiFormat);
	//////////////////////////////////////////////////////////////////////////
	
	// ���ڸ߶�(m)
	x1 = 775;
	x2 = x1+145;
	y1 = m_Rect.top-m_nRowHeight;
	y2 = y1-m_nRowHeight;
	y1 -= 10;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("����\n�߶�\n(m)");
	pDC->DrawText(CellString,CellRect,mutiFormat);
	
	// �������(m)
	x1 = 1120;
	x2 = x1+144;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("����\n���\n(m2)");
	pDC->DrawText(CellString,CellRect,mutiFormat);
	
	// ���ڷ�ʽ
	x1 = 1417;
	x2 = x1+93;
	y1 -= 20;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("����\n��ʽ");
	pDC->DrawText(CellString,CellRect,mutiFormat);
	
	// ����
	x1 = 1696;
	x2 = x1+109;
	y1 += 30;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("����");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);

	//////////////////////////////////////////////////////////////////////////
	// �Ʊ�
	x1 = m_Rect.left;
	x2 = x1+200;
	y1 = m_Rect.bottom;
	y2 = y1-m_nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("���λ:");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);

	// ����
	x1 = 650;
	x2 = x1+200;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("ʩ����λ:");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);

	// �ֳ�������ʦ
	x1 = 1200;
	x2 = x1+260;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("������ʦ:");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
		
	pDC->SelectObject(oldfont);	
}

// ��ӡ�����¼
void FaceInfoPrintEx::PrintFaceInfoRecord(CDC* pDC, int nIdx)
{
	// �и�(������"��ͼ"��,���и߶Ƚϴ�)
	int m_nRowHeight = 125;
	
	//��Ԫ��
	CRect CellRect;
	CString CellString;
	int x1,y1,x2,y2;
	
	//����
	CFont *oldfont;
	//����ԭ����
	oldfont=pDC->GetCurrentFont();
	
	//������ͷ����
	CFont MyFont;
	MyFont.CreatePointFont(105,_T("����"),pDC);
	pDC->SelectObject(&MyFont);

	// �õ���ǰ���������¼
	FACEINFOCANVAS* pFI = m_FaceInfoRecords.GetFaceInfoList()->at(nIdx);
	
	//////////////////////////////////////////////////////////////////////////
	UINT ReportHeadFormat = DT_SINGLELINE|DT_CENTER|DT_VCENTER;
	UINT leftFormat = ReportHeadFormat&(~DT_CENTER);
	UINT mutiFormat = ReportHeadFormat&(~DT_SINGLELINE);

	// ��������
	x1 = m_Rect.left+210;
	x2 = x1+500;
	y1 = m_Rect.top+m_nRowHeight;
	y2 = m_Rect.top;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = pFI->strTunName+pFI->strPartIdxName;
	pDC->DrawText(CellString,CellRect,leftFormat);
	
	// ʩ�����
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
	
	// ���
	x1 = 1600;
	x2 = m_Rect.right;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = pFI->strID;
	pDC->DrawText(CellString,CellRect,leftFormat);

	//////////////////////////////////////////////////////////////////////////
	// ���ڿ��(m)
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
	
	// ���ڸ߶�(m)
	x1 = 920;
	x2 = x1+200;
	CellRect.SetRect(x1,y1,x2,y2);
	if (int(pFI->fFaceHeight*10)%10 != 0)
		CellString.Format("%d.%d", int(pFI->fFaceHeight), int(pFI->fFaceHeight*10)%10);
	else
		CellString.Format("%d", int(pFI->fFaceHeight));
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);

	// �������(m2)
	x1 = 1263;
	x2 = x1+154;
	CellRect.SetRect(x1,y1,x2,y2);
	if (int(pFI->fFaceArea*10)%10 != 0)
		CellString.Format("%d.%d", int(pFI->fFaceArea), int(pFI->fFaceArea*10)%10);
	else
		CellString.Format("%d", int(pFI->fFaceArea));
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// ���ڷ�ʽ
	x1 = 1510;
	x2 = x1+186;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = pFI->strExcaveWay;
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// ����
	x1 = 1800;
	x2 = m_Rect.right;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = pFI->strOther;
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);

	//////////////////////////////////////////////////////////////////////////
	// ������״̬
	CellString.Empty();
	// �ȶ�
	if (pFI->bFaceStatusWD)
		CellString = pFI->strFaceStatusWD + ",";
	// �������
	if (pFI->bFaceStatusZMDK)
		CellString += pFI->strFaceStatusZMDK + ",";
	// ���漷��
	if (pFI->bFaceStatusZMJC)
		CellString += pFI->strFaceStatusZMJC + ",";
	// ���治������
	if (pFI->bFaceStatusZMBNZW)
		CellString += pFI->strFaceStatusZMBNZW + ",";
	// ����
	if (pFI->bFaceStatusOther)
		CellString += _T("����:")+pFI->strFaceStatusOtherDesc;

	x1 = m_Rect.left + 387;
	x2 = m_Rect.right;
	y1 = m_Rect.top-2*m_nRowHeight;
	y2 = y1-m_nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	//////////////////////////////////////////////////////////////////////////
	// Χ������(2015-9-19�޸�)
	CellString = pFI->strLithology;

	x1 = m_Rect.left + 387;
	x2 = m_Rect.right;
	y1 = m_Rect.top-3*m_nRowHeight;
	y2 = y1-m_nRowHeight;
	CellRect.SetRect(x1+10,y1-10,x2,y2);
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	//////////////////////////////////////////////////////////////////////////
	// ��ʯǿ��
	CellString.Empty();
	// 30~60(Ӳ��)
	if (pFI->bRockStrengthYY)
		CellString = pFI->strRockStrengthYY + ",";
	// 15~30(������)
	if (pFI->bRockStrengthJORY)
		CellString += pFI->strRockStrengthJORY + ",";
	// 5~15(����)
	if (pFI->bRockStrengthRY)
		CellString += pFI->strRockStrengthRY + ",";
	// <5(������)
	if (pFI->bRockStrengthJIRY)
		CellString += pFI->strRockStrengthJIRY + ",";
	// ����
	if (pFI->bRockStrengthOther)
		CellString += _T("����:")+pFI->strRockStrengthOtherDesc;

	x1 = m_Rect.left + 387;
	x2 = m_Rect.right;
	y1 = m_Rect.top-4*m_nRowHeight;
	y2 = y1-m_nRowHeight;
	CellRect.SetRect(x1+10,y1-10,x2,y2);
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	//////////////////////////////////////////////////////////////////////////
	// �绯�̶�
	CellString.Empty();
	// ΢�绯
	if (pFI->bWeatheringWFH)
		CellString = pFI->strWeatheringWFH + ",";
	// ���绯
	if (pFI->bWeatheringRFH)
		CellString += pFI->strWeatheringRFH + ",";
	// ǿ�绯
	if (pFI->bWeatheringQIFH)
		CellString += pFI->strWeatheringQIFH + ",";
	// ȫ�绯
	if (pFI->bWeatheringQUFH)
		CellString += pFI->strWeatheringQUFH + ",";
	// ����
	if (pFI->bWeatheringOther)
		CellString += _T("����:")+pFI->strWeatheringOtherDesc;

	x1 = m_Rect.left + 387;
	x2 = m_Rect.right;
	y1 = m_Rect.top-5*m_nRowHeight;
	y2 = y1-m_nRowHeight;
	CellRect.SetRect(x1+10,y1-10,x2,y2);
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	//////////////////////////////////////////////////////////////////////////
	// ��϶���(mm)
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
	// ����
	if (pFI->bCrackWidthOther)
		CellString += _T("����:")+pFI->strCrackWidthOtherDesc;

	x1 = m_Rect.left + 387;
	x2 = m_Rect.right;
	y1 = m_Rect.top-6*m_nRowHeight;
	y2 = y1-m_nRowHeight;
	CellRect.SetRect(x1+10,y1-10,x2,y2);
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	//////////////////////////////////////////////////////////////////////////
	// ��϶��̬
	CellString.Empty();
	// �ܼ�
	if (pFI->bCrackStatusMJ)
		CellString = pFI->strCrackStatusMJ;
	// �����ſ�
	if (pFI->bCrackStatusBFZK)
		CellString += pFI->strCrackStatusBFZK + ",";
	// ����
	if (pFI->bCrackStatusKK)
		CellString += pFI->strCrackStatusKK + ",";
	// ����ճ��
	if (pFI->bCrackStatusJYLT)
		CellString += pFI->strCrackStatusJYLT + ",";
	// ����
	if (pFI->bCrackStatusOther)
		CellString += _T("����:")+pFI->strCrackStatusOtherDesc;

	x1 = m_Rect.left + 387;
	x2 = m_Rect.right;
	y1 = m_Rect.top-7*m_nRowHeight;
	y2 = y1-m_nRowHeight;
	CellRect.SetRect(x1+10,y1-10,x2,y2);
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	//////////////////////////////////////////////////////////////////////////
	// ӿˮ״̬
	// ��ˮ
	if (pFI->bGushingStatusWS)
		CellString = pFI->strGushingStatusWS + ",";
	// ��ˮ
	if (pFI->bGushingStatusSS)
		CellString += pFI->strGushingStatusSS + ",";
	// ����ʪ��
	if (pFI->bGushingStatusZTSR)
		CellString += pFI->strGushingStatusZTSR + ",";
	// ӿ�������
	if (pFI->bGushingStatusYCPC)
		CellString += pFI->strGushingStatusYCPC + ",";
	// �ر��
	if (pFI->bGushingStatusTBD)
		CellString += pFI->strGushingStatusTBD;

	x1 = m_Rect.left + 387;
	x2 = m_Rect.right;
	y1 = m_Rect.top-8*m_nRowHeight;
	y2 = y1-m_nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	//////////////////////////////////////////////////////////////////////////
	// Χ�Ҽ��𻮷�
	x1 = 507+10;
	x2 = m_Rect.right;
	y1 = m_Rect.top-9*m_nRowHeight;
	y2 = y1-m_nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = pFI->strDesignGrade;
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// ��ӡ����������ͼ(������������Ƭ�·�)
	x1 = 257+20;
	x2 = x1+862-40;
	y1 = m_Rect.bottom+(m_Rect.top-10*m_nRowHeight-m_Rect.bottom)/2-10;
	y2 = m_Rect.bottom+20;
	CellRect.SetRect(x1,y1,x2,y2);
	long double dbZoomX, dbZoomY;
	{
		// ����������ͼ��
		CTilGraph* pGraph = pFI->pCanvas->GetGraph();
		// �ڴ�ӡʱ�����������ű�������ʾ
		dbZoomX = abs(CellRect.Width())*1.0/pGraph->GetWidth();
		dbZoomY = abs(CellRect.Height())*1.0/pGraph->GetHeight();
		pGraph->SetZoomFactor(dbZoomX, dbZoomY);
		pGraph->SetShowMode(TRUE);
		pGraph->DrawInvert(pDC, CellRect.TopLeft());
	}

	// ��ӡ��������Ƭ
	y2 = y1+20;
	y1 = m_Rect.top-10*m_nRowHeight-10;
	CellRect.SetRect(x1,y1,x2,y2);
	{
		// ��������Ƭ
		CTilImage* pImage = pFI->pCanvas->GetImage();
		dbZoomX = abs(CellRect.Width())*1.0/pImage->GetWidth();
		dbZoomY = abs(CellRect.Height())*1.0/pImage->GetHeight();
		pImage->SetZoomFactor(dbZoomX, dbZoomY);
		pImage->SetShowMode(TRUE);
		pImage->Stretch(pDC, CellRect);
	}

	// ��������������
	x1 = 1119+20;
	x2 = m_Rect.right-30;
	y1 = m_Rect.top-10*m_nRowHeight-10;
	y2 = m_Rect.bottom+80;
	CellRect.SetRect(x1,y1,x2+20,y2-60);
	CellString = "    " + pFI->strFaceDesc;
	// ���뻻�з�
	// ��ÿɻ��е��ַ�������ʾ�ĸ߶�
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
	// �����Ա
	// ���λ
	x1 = m_Rect.left+200;
	x2 = x1+400;
	y1 = m_Rect.bottom;
	y2 = y1-m_nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = pFI->strRecUnit;
	pDC->DrawText(CellString,CellRect,leftFormat);
	
	// ʩ����λ
	x1 = 850;
	x2 = x1+400;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = pFI->strConstructUnit;
	pDC->DrawText(CellString,CellRect,leftFormat);
	
	// ������ʦ
	x1 = 1460;
	x2 = x1+200;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = pFI->strRecSupervision;
	pDC->DrawText(CellString,CellRect,leftFormat);	

	// ������
	x1 = 1700;
	x2 = m_Rect.right;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = pFI->dateTime.Format("%Y��%m��%d��");
	pDC->DrawText(CellString,CellRect,leftFormat);

	pDC->SelectObject(oldfont);
}

// ����ӡ������
void FaceInfoPrintEx::PrintFaceInfoGrid(CDC* pDC)
{
	//����ݺ�
	int y;
	
	// �и�(������"��ͼ"��,���и߶Ƚϴ�)
	int m_nRowHeight = 125;
	
	// ��ˮƽ��
	for (int i=0; i<11; i++)
	{
		y = m_Rect.top-i*m_nRowHeight;
		DrawHLine(y, m_Rect.left, m_Rect.right, pDC);
	}
	// �����һ��ˮƽ��
	DrawHLine(m_Rect.bottom, m_Rect.left, m_Rect.right, pDC);
	
	// ����ֱ��
	// ��߽�
	DrawVLine(m_Rect.left, m_Rect.top, m_Rect.bottom, pDC);
	// "���"���ұ߽�
	DrawVLine(257, m_Rect.top, m_Rect.bottom, pDC);
	// "��Ŀ����"���ұ߽�
	DrawVLine(507, m_Rect.top, m_Rect.top-10*m_nRowHeight, pDC);
	// "���ڿ��"���ұ߽�
	DrawVLine(645, m_Rect.top-m_nRowHeight, m_Rect.top-2*m_nRowHeight, pDC);
	// "���ڿ��(m)"�Ҳ����������ұ߽�
	DrawVLine(775, m_Rect.top-m_nRowHeight, m_Rect.top-2*m_nRowHeight, pDC);
	// "���ڸ߶�"���ұ߽�
	DrawVLine(920, m_Rect.top-m_nRowHeight, m_Rect.top-2*m_nRowHeight, pDC);
	// "���ڸ߶�"�Ҳ����������ұ߽�
	DrawVLine(1119, m_Rect.top-m_nRowHeight, m_Rect.top-2*m_nRowHeight, pDC);
	// "�������"���ұ߽�
	DrawVLine(1263, m_Rect.top-m_nRowHeight, m_Rect.top-2*m_nRowHeight, pDC);
	// "�������"�Ҳ����������ұ߽�
	DrawVLine(1417, m_Rect.top-m_nRowHeight, m_Rect.top-2*m_nRowHeight, pDC);
	// "���ڷ�ʽ"���ұ߽�
	DrawVLine(1510, m_Rect.top-m_nRowHeight, m_Rect.top-2*m_nRowHeight, pDC);
	// "���ڷ�ʽ"�Ҳ������ұ߽�
	DrawVLine(1696, m_Rect.top-m_nRowHeight, m_Rect.top-2*m_nRowHeight, pDC);
	// "����"�����ұ߽�
	DrawVLine(1800, m_Rect.top-m_nRowHeight, m_Rect.top-2*m_nRowHeight, pDC);
	// ���ұ߽�
	DrawVLine(m_Rect.right, m_Rect.top, m_Rect.bottom, pDC);
	// �·�"��������������"�Ҳ�߽�
	DrawVLine(1119, m_Rect.top-10*m_nRowHeight, m_Rect.bottom, pDC);
}

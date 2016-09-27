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
	// Ĭ�ϴ�ӡ��������Ƭ
	m_nPrintTarget = 0;
}

FaceInfoPrint::~FaceInfoPrint()
{

}

// ���ô�ӡ����,0: ��ӡͼ��,1:��ӡ����ͼ
void FaceInfoPrint::SetPrintTarget(int nFacePhoto)
{
	ASSERT(nFacePhoto == 0 || nFacePhoto == 1);
	m_nPrintTarget = nFacePhoto;
}

int  FaceInfoPrint::GetPrintTarget()
{
	return m_nPrintTarget;
}

//��ӡҳüҳ��
void FaceInfoPrint::PrintHeaderFooter(CDC* pDC)
{

}

//��ӡ����ͷ
void FaceInfoPrint::PrintFaceInfoHead(CDC* pDC)
{
	UINT ReportHeadFormat = DT_SINGLELINE|DT_CENTER|DT_VCENTER;
	UINT mutiFormat = ReportHeadFormat&(~DT_SINGLELINE);

	// �������
	CRect rect(120, -530, 1970, -2440);
	
	// �и�(������"��ͼ"��,���и߶Ƚϴ�)
	int nRowHeight = 125;

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
	x1 = rect.left;
	x2 = rect.right;
	y1 = rect.top + 260;
	y2 = rect.top + 150;
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
	x1 = rect.left;
	x2 = x1+230;
	y1 = rect.top+nRowHeight;
	y2 = rect.top;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("��������:");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// ʩ�����
	x1 = 850;
	x2 = x1+230;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("ʩ�����:");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// ����
	x1 = 1470;
	x2 = x1+140;
	y1 = rect.top+nRowHeight;
	y2 = rect.top;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("����:");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	//////////////////////////////////////////////////////////////////////////
	// ���
	x1 = rect.left;
	x2 = 257;
	y1 = rect.top;
	y2 = rect.top-nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("���");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);

	// ���1~9
	for (int i=0; i<9; i++)
	{
		y1 = y2;
		y2 = y2-nRowHeight;
		CellRect.SetRect(x1,y1,x2,y2);
		CellString.Format("%d", i+1);
		pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	}
	
	// ��ͼ
	y1 = y2;
	y2 = rect.bottom;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("��ͼ");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);

	//////////////////////////////////////////////////////////////////////////
	// ��Ŀ����
	x1 = x2;
	x2 = x1+250;
	y1 = rect.top;
	y2 = y1-nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("��Ŀ����");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);

	// ������ߴ�
	y1 = y2;
	y2 = y1-nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("������ߴ�");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// ������״̬
	y1 = y2;
	y2 = y1-nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("������״̬");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);

	// ë������״̬
	y1 = y2;
	y2 = y1-nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("ë������״̬");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// ��ʯǿ��(MPa)
	y1 = y2;
	y2 = y1-nRowHeight;
	y1 -= 30;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("��ʯǿ��\n(MPa)");
	pDC->DrawText(CellString,CellRect,mutiFormat);

	// �绯�̶�
	y1 = y2;
	y2 = y1-nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("�绯�̶�");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);

	// ��϶���(mm)
	y1 = y2;
	y2 = y1-nRowHeight;
	y1 -= 30;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("��϶���\n(mm)");
	pDC->DrawText(CellString,CellRect,mutiFormat);

	// ��϶��̬
	y1 = y2;
	y2 = y1-nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("��϶��̬");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// ӿˮ״̬
	y1 = y2;
	y2 = y1-nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("ӿˮ״̬");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// �����漶�𻮷�
	y1 = y2;
	y2 = y1-nRowHeight;
	y1 -= 30;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("Χ�Ҽ���\n����");
	pDC->DrawText(CellString,CellRect,mutiFormat);
	
	// ��������������
	x1 += 20;
	x2 = x1+300;
	y1 = y2;
	y2 = y1-60;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("��������������:");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// ����������Ƭ
	x1 = 1115+20;
	x2 = x1+300;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("����������Ƭ:");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	//////////////////////////////////////////////////////////////////////////
	// ״̬����
	x1 = 507;
	x2 = rect.right;
	y1 = rect.top;
	y2 = y1-nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("״̬����");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// ���ڿ��(m)
	x2 = 645;
	y1 = y2-10;
	y2 = y1-nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("����\n���\n(m)");
	pDC->DrawText(CellString,CellRect,mutiFormat);
	//////////////////////////////////////////////////////////////////////////
	
	// ���ڸ߶�(m)
	x1 = 755;
	x2 = x1+144;
	y1 = rect.top-nRowHeight;
	y2 = y1-nRowHeight;
	y1 -= 10;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("����\n�߶�\n(m)");
	pDC->DrawText(CellString,CellRect,mutiFormat);
	
	// �������(m)
	x1 = 1119;
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
	x1 = rect.left;
	x2 = x1+150;
	y1 = rect.bottom;
	y2 = y1-nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("�Ʊ�:");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);

	// ����
	x1 = 560;
	x2 = x1+130;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("����:");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
	
	// �ֳ�������ʦ
	x1 = 1030;
	x2 = x1+360;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("�ֳ�������ʦ:");
	pDC->DrawText(CellString,CellRect,ReportHeadFormat);
		
	pDC->SelectObject(oldfont);	
}

// ��ӡ�����¼
void FaceInfoPrint::PrintFaceInfoRecord(CDC* pDC, int nIdx)
{
	// �������
	CRect rect(120, -530, 1970, -2440);
	
	// �и�(������"��ͼ"��,���и߶Ƚϴ�)
	int nRowHeight = 125;
	
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
	x1 = rect.left+210;
	x2 = x1+500;
	y1 = rect.top+nRowHeight;
	y2 = rect.top;
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
	
	// ����
	x1 = 1600;
	x2 = rect.right;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = pFI->dateTime.Format("%Y-%m-%d");
	pDC->DrawText(CellString,CellRect,leftFormat);
	
	//////////////////////////////////////////////////////////////////////////
	// ���ڿ��(m)
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
	
	// ���ڸ߶�(m)
	x1 = 899;
	x2 = x1+220;
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
	x2 = rect.right;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = pFI->strOther;
	pDC->DrawText(CellString,CellRect,DT_WORDBREAK);

	//////////////////////////////////////////////////////////////////////////
	// ������״̬
	// �ȶ�
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
	// �������
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
	// ���漷��
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
	// ���治������
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
	// ����
	if (pFI->bFaceStatusOther)
	{
		x1 = 1696;
		x2 = rect.right;
		y1 = rect.top-2*nRowHeight;
		y2 = y1-nRowHeight;
		CellRect.SetRect(x1+10,y1-10,x2,y2);
		CellString = _T("����:")+pFI->strFaceStatusOtherDesc;
		pDC->DrawText(CellString,CellRect,DT_WORDBREAK);
	}
	//////////////////////////////////////////////////////////////////////////
	// ë������״̬
	// ����
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
	// ��ʱ���ɳڡ�����
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
	// �������ѡ�Ҫ��ʱ֧��
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
	// Ҫ��ǰ֧��
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
	// ����
	if (pFI->bExcaveStatusOther)
	{
		x1 = 1696;
		x2 = rect.right;
		y1 = rect.top-3*nRowHeight;
		y2 = y1-nRowHeight;
		CellRect.SetRect(x1+10,y1-10,x2,y2);
		CellString = _T("����:")+pFI->strExcaveStatusOtherDesc;
		pDC->DrawText(CellString,CellRect,DT_WORDBREAK);
	}
	//////////////////////////////////////////////////////////////////////////
	// ��ʯǿ��
	// 30~60(Ӳ��)
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
	// 15~30(������)
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
	// 5~15(����)
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
	// <5(������)
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
	// ����
	if (pFI->bRockStrengthOther)
	{
		x1 = 1696;
		x2 = rect.right;
		y1 = rect.top-4*nRowHeight;
		y2 = y1-nRowHeight;
		CellRect.SetRect(x1+10,y1-10,x2,y2);
		CellString = _T("����:")+pFI->strRockStrengthOtherDesc;
		pDC->DrawText(CellString,CellRect,DT_WORDBREAK);
	}
	//////////////////////////////////////////////////////////////////////////
	// �绯�̶�
	// ΢�绯
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
	// ���绯
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
	// ǿ�绯
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
	// ȫ�绯
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
	// ����
	if (pFI->bWeatheringOther)
	{
		x1 = 1696;
		x2 = rect.right;
		y1 = rect.top-5*nRowHeight;
		y2 = y1-nRowHeight;
		CellRect.SetRect(x1+10,y1-10,x2,y2);
		CellString = _T("����:")+pFI->strWeatheringOtherDesc;
		pDC->DrawText(CellString,CellRect,DT_WORDBREAK);
	}
	//////////////////////////////////////////////////////////////////////////
	// ��϶���(mm)
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
	// ����
	if (pFI->bCrackWidthOther)
	{
		x1 = 1696;
		x2 = rect.right;
		y1 = rect.top-6*nRowHeight;
		y2 = y1-nRowHeight;
		CellRect.SetRect(x1+10,y1-10,x2,y2);
		CellString = _T("����:")+pFI->strCrackWidthOtherDesc;
		pDC->DrawText(CellString,CellRect,DT_WORDBREAK);
	}
	//////////////////////////////////////////////////////////////////////////
	// ��϶��̬
	// �ܼ�
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
	// �����ſ�
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
	// ����
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
	// ����ճ��
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
	// ����
	if (pFI->bCrackStatusOther)
	{
		x1 = 1696;
		x2 = rect.right;
		y1 = rect.top-7*nRowHeight;
		y2 = y1-nRowHeight;
		CellRect.SetRect(x1+10,y1-10,x2,y2);
		CellString = _T("����:")+pFI->strCrackStatusOtherDesc;
		pDC->DrawText(CellString,CellRect,DT_WORDBREAK);
	}
	//////////////////////////////////////////////////////////////////////////
	// ӿˮ״̬
	// ��ˮ
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
	// ��ˮ
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
	// ����ʪ��
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
	// ӿ�������
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
	// �ر��
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
	// Χ�Ҽ��𻮷�
	x1 = 507+10;
	x2 = rect.right;
	y1 = rect.top-9*nRowHeight;
	y2 = y1-nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = _T("Χ����Ƽ���:")+pFI->strDesignGrade+_T(",Χ��ʩ������:")+pFI->strConstructGrade;
	pDC->DrawText(CellString,CellRect,leftFormat);
	
	// ��������������
	x1 = 257+20;
	x2 = x1+862-20;
	y1 = y1-nRowHeight-80;
	y2 = rect.bottom;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = "    " + pFI->strFaceDesc;
	// ���뻻�з�
	pDC->DrawText(CellString,CellRect,DT_WORDBREAK);

	// ��������ȷ���Ǵ�ӡ��������Ƭ��������ͼ��
	x1 = 1119+20;
	x2 = rect.right-30;
	y1 = rect.top-10*nRowHeight-10;
	y2 = rect.bottom+80;
	CellRect.SetRect(x1,y1,x2+20,y2-60);
	long double dbZoomX, dbZoomY;
	if (m_nPrintTarget == 0) // ��ӡ��������Ƭ
	{
		// ��������Ƭ
		CTilImage* pImage = pFI->pCanvas->GetImage();
		dbZoomX = abs(CellRect.Width())*1.0/pImage->GetWidth();
		dbZoomY = abs(CellRect.Height())*1.0/pImage->GetHeight();
		pImage->SetZoomFactor(dbZoomX, dbZoomY);
		pImage->Stretch(pDC, CellRect);
	}
	else // m_nPrintTarget == 1, ����ӡ����������ͼ��
	{
		// ����������ͼ��
		CTilGraph* pGraph = pFI->pCanvas->GetGraph();
		// �ڴ�ӡʱ�����������ű�������ʾ
		dbZoomX = abs(CellRect.Width())*1.0/pGraph->GetWidth();
		dbZoomY = abs(CellRect.Height())*1.0/pGraph->GetHeight();
		pGraph->SetZoomFactor(dbZoomX, dbZoomY);
		pGraph->DrawInvert(pDC, CellRect.TopLeft());
	}

	//////////////////////////////////////////////////////////////////////////
	// �����Ա
	// �Ʊ�
	x1 = 255;
	x2 = x1+270;
	y1 = rect.bottom;
	y2 = y1-nRowHeight;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = pFI->strRecStaff;
	pDC->DrawText(CellString,CellRect,leftFormat);
	
	// ����
	x1 = 680;
	x2 = x1+300;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = pFI->strRecReview;
	pDC->DrawText(CellString,CellRect,leftFormat);
	
	// �ֳ�������ʦ
	x1 = 1360;
	x2 = x1+500;
	CellRect.SetRect(x1,y1,x2,y2);
	CellString = pFI->strRecSupervision;
	pDC->DrawText(CellString,CellRect,leftFormat);	

	pDC->SelectObject(oldfont);
}

// ����ӡ������
void FaceInfoPrint::PrintFaceInfoGrid(CDC* pDC)
{
	//����ݺ�
	int y;
	
	// �������
	CRect rect(120, -530, 1970, -2440);
	
	// �и�(������"��ͼ"��,���и߶Ƚϴ�)
	int nRowHeight = 125;
	
	// ��ˮƽ��
	for (int i=0; i<11; i++)
	{
		y = rect.top-i*nRowHeight;
		DrawHLine(y, rect.left, rect.right, pDC);
	}
	// �����һ��ˮƽ��
	DrawHLine(rect.bottom, rect.left, rect.right, pDC);
	
	// ����ֱ��
	// ��߽�
	DrawVLine(rect.left, rect.top, rect.bottom, pDC);
	// "���"���ұ߽�
	DrawVLine(257, rect.top, rect.bottom, pDC);
	// "��Ŀ����"���ұ߽�
	DrawVLine(507, rect.top, rect.top-10*nRowHeight, pDC);
	// "���ڿ��"���ұ߽�
	DrawVLine(645, rect.top-nRowHeight, rect.top-2*nRowHeight, pDC);
	// "���ڿ��(m)"�Ҳ����������ұ߽�
	DrawVLine(755, rect.top-nRowHeight, rect.top-9*nRowHeight, pDC);
	// "���ڸ߶�"���ұ߽�
	DrawVLine(899, rect.top-nRowHeight, rect.top-2*nRowHeight, pDC);
	// "���ڸ߶�"�Ҳ����������ұ߽�
	DrawVLine(1119, rect.top-nRowHeight, rect.top-9*nRowHeight, pDC);
	// "�������"���ұ߽�
	DrawVLine(1263, rect.top-nRowHeight, rect.top-2*nRowHeight, pDC);
	// "�������"�Ҳ����������ұ߽�
	DrawVLine(1417, rect.top-nRowHeight, rect.top-9*nRowHeight, pDC);
	// "���ڷ�ʽ"���ұ߽�
	DrawVLine(1510, rect.top-nRowHeight, rect.top-2*nRowHeight, pDC);
	// "���ڷ�ʽ"�Ҳ������ұ߽�
	DrawVLine(1696, rect.top-nRowHeight, rect.top-9*nRowHeight, pDC);
	// "����"�����ұ߽�
	DrawVLine(1800, rect.top-nRowHeight, rect.top-2*nRowHeight, pDC);
	// ���ұ߽�
	DrawVLine(rect.right, rect.top, rect.bottom, pDC);
	// �·�"��������������"�Ҳ�߽�
	DrawVLine(1119, rect.top-10*nRowHeight, rect.bottom, pDC);
}

//����ֱֱ��
void   FaceInfoPrint::DrawVLine(int x_up, int y_up, int y_bottom,CDC* pDC)
{
	pDC->MoveTo(x_up, y_up);
	pDC->LineTo(x_up,y_bottom);
}

//��ˮƽֱ��
void   FaceInfoPrint::DrawHLine(int y_left,int x_left, int x_right,CDC* pDC)
{
	pDC->MoveTo(x_left, y_left);
	pDC->LineTo( x_right,y_left);
}

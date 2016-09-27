// FaceInfoPrint.h: interface for the FaceInfoPrint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FACEINFOPRINT_H__AD8B3356_CC0F_4B35_8D41_257969DA5D0B__INCLUDED_)
#define AFX_FACEINFOPRINT_H__AD8B3356_CC0F_4B35_8D41_257969DA5D0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FaceInfoList.h"

class FaceInfoPrint  
{
public:
	FaceInfoPrint();
	virtual ~FaceInfoPrint();

	// ���ô�ӡ����,0: ��ӡͼ��,1:��ӡ����ͼ
	void SetPrintTarget(int nFacePhoto);
	int  GetPrintTarget();
public:	// ׷��2014.04.13
	//��ӡҳüҳ��
	void PrintHeaderFooter(CDC* pDC);
	//��ӡ����ͷ
	void PrintFaceInfoHead(CDC* pDC);
	// ��ӡ�����¼
	void PrintFaceInfoRecord(CDC* pDC, int nIdx);
	// ��ӡ������
	void PrintFaceInfoGrid(CDC* pDC);
	
	CFaceInfoList m_FaceInfoRecords;

protected:
	// ���ô�ӡ������ͼ��������������ͼ,0: ��ӡͼ��,1:��ӡ����ͼ
	int m_nPrintTarget;
	//��ֱ
	void  DrawVLine(int x_up, int y_up, int y_bottom,CDC* pDC=NULL);
	//ˮƽ
	void  DrawHLine(int y_left,int x_left, int x_right,CDC* pDC=NULL);
	// �����ֱ��
	void  DrawLine(int x_left, int y_left, int x_right, int y_right,CDC* pDC=NULL);
};

#endif // !defined(AFX_FACEINFOPRINT_H__AD8B3356_CC0F_4B35_8D41_257969DA5D0B__INCLUDED_)

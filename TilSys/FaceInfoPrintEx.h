// FaceInfoPrintEx.h: interface for the FaceInfoPrintEx class.
//
//////////////////////////////////////////////////////////////////////

/************************************
  REVISION LOG ENTRY
  ά����Ա: leng
  ά��ʱ�� 2015-5-17 10:29:27
  �޸�ʱ�� 2015-9-17
  ˵��: Ӧ����5��Ҫ����FaceInfoPrint��Ļ������޸Ĵ�ӡ����
 ************************************/


#if !defined(AFX_FACEINFOPRINTEX_H__86EA1EAC_FDA9_48E6_9A57_52BD14A46ADF__INCLUDED_)
#define AFX_FACEINFOPRINTEX_H__86EA1EAC_FDA9_48E6_9A57_52BD14A46ADF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FaceInfoPrint.h"
#include "FaceInfoList.h"

class FaceInfoPrintEx : public FaceInfoPrint 
{
public:
	FaceInfoPrintEx();
	virtual ~FaceInfoPrintEx();

public:	// ׷��2014.04.13
	//��ӡҳüҳ��
	void PrintHeaderFooter(CDC* pDC);
	//��ӡ����ͷ
	void PrintFaceInfoHead(CDC* pDC);
	// ��ӡ������
	void PrintFaceInfoGrid(CDC* pDC);
	// ��ӡ�����¼
	void PrintFaceInfoRecord(CDC* pDC, int nIdx);

protected:
	CRect m_Rect;
	int m_nRowHeight;
};

#endif // !defined(AFX_FACEINFOPRINTEX_H__86EA1EAC_FDA9_48E6_9A57_52BD14A46ADF__INCLUDED_)

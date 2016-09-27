// FaceInfoPrintEx.h: interface for the FaceInfoPrintEx class.
//
//////////////////////////////////////////////////////////////////////

/************************************
  REVISION LOG ENTRY
  维护人员: leng
  维护时间 2015-5-17 10:29:27
  修改时间 2015-9-17
  说明: 应中铁5局要求，在FaceInfoPrint类的基础上修改打印报表
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

public:	// 追加2014.04.13
	//打印页眉页脚
	void PrintHeaderFooter(CDC* pDC);
	//打印报表头
	void PrintFaceInfoHead(CDC* pDC);
	// 打印报表表格
	void PrintFaceInfoGrid(CDC* pDC);
	// 打印报表记录
	void PrintFaceInfoRecord(CDC* pDC, int nIdx);

protected:
	CRect m_Rect;
	int m_nRowHeight;
};

#endif // !defined(AFX_FACEINFOPRINTEX_H__86EA1EAC_FDA9_48E6_9A57_52BD14A46ADF__INCLUDED_)

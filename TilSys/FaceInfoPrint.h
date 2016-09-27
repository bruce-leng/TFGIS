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

	// 设置打印对象,0: 打印图像,1:打印素描图
	void SetPrintTarget(int nFacePhoto);
	int  GetPrintTarget();
public:	// 追加2014.04.13
	//打印页眉页脚
	void PrintHeaderFooter(CDC* pDC);
	//打印报表头
	void PrintFaceInfoHead(CDC* pDC);
	// 打印报表记录
	void PrintFaceInfoRecord(CDC* pDC, int nIdx);
	// 打印报表表格
	void PrintFaceInfoGrid(CDC* pDC);
	
	CFaceInfoList m_FaceInfoRecords;

protected:
	// 设置打印掌子面图像还是掌子面素描图,0: 打印图像,1:打印素描图
	int m_nPrintTarget;
	//竖直
	void  DrawVLine(int x_up, int y_up, int y_bottom,CDC* pDC=NULL);
	//水平
	void  DrawHLine(int y_left,int x_left, int x_right,CDC* pDC=NULL);
	// 两点间直线
	void  DrawLine(int x_left, int y_left, int x_right, int y_right,CDC* pDC=NULL);
};

#endif // !defined(AFX_FACEINFOPRINT_H__AD8B3356_CC0F_4B35_8D41_257969DA5D0B__INCLUDED_)

// TilCanvasMgr.h: interface for the CTilCanvasMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TILCANVASMGR_H__2459C0F4_34AB_47A5_A2C9_F5D9D3EEAB0C__INCLUDED_)
#define AFX_TILCANVASMGR_H__2459C0F4_34AB_47A5_A2C9_F5D9D3EEAB0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// 项目名称
typedef struct __tagPRJCAPTION
{
	CString strLineName;	// 线路名称
	CString strTunName;		// 隧道名称
	CString strPartName;	// 洞室名称
	CString strPartIdx;		// 洞室编号
} PRJCAPTION;

#include "TilCanvas.h"
class AFX_EXT_CLASS CTilCanvasMgr  
{
public:
	enum { ADD_SIZE = 100};
	CTilCanvasMgr();
	virtual ~CTilCanvasMgr();

	void New();
	BOOL Open(const CString& strProjName);
	BOOL Save();
	BOOL SaveAs(const CString& strProjName);

	CString GetPathName();

	CTilCanvas* GetCanvasByIdx(int nIdx);
	CTilCanvas* GetFirstCanvas();
	CTilCanvas* GetLastCanvas();
	CTilCanvas* GetPrevCanvas();
	CTilCanvas* GetCurCanvas();
	CTilCanvas* GetNextCanvas();

	void SetCurCanvasIdx(int nIdx);
	int	 GetCurCanvasIdx();

	void SetZoomFactor(long double dbZoomX, long double dbZoomY);
	void GetZoomFactor(long double& dbZoomX, long double& dbZoomY);

	int Size();

	void SetModifiedFlag(BOOL bFlag = TRUE);
	BOOL IsModified();

	void RemoveAll();
	BOOL RemoveAt(int nIdx);

	BOOL PushFrontCanvas(CTilCanvas* pCanvas);
	BOOL PushBackCanvas(CTilCanvas* pCanvas);
	BOOL InsertCanvas(CTilCanvas* pCanvas, int nIdx, BOOL bAfter = TRUE);

	BOOL PushFrontCanvas(const CString& strPathImage, const CString& strPathGraph);
	BOOL PushBackCanvas(const CString& strPathImage, const CString& strPathGraph);
	BOOL InsertCanvas(const CString& strPathImage, const CString& strPathGraph, int nIdx, BOOL bAfter = TRUE);

	// 将序号为nIdx的画布放到nIdx2的前面或后面
	BOOL DragCanvasTo(int nIdx1, int nIdx2, BOOL bAfter=TRUE);
//	BOOL DragCanvasTo(int nIdx1, int nIdx2);

	// 按照里程大小自动排列画布
	void AutoArrayCanvas(BOOL bOrder);
private:
	BOOL m_bModified;
	long double m_dbZoomX;
	long double m_dbZoomY;
	int m_nIdx;
	int m_nNum;
	int m_nTotalNum;
	CTilCanvas** m_ppCanvas;

	CString m_strProjName;

public:
	PRJCAPTION m_PrjCap;	// 当前隧道名称等信息
	void SetPrjInfo(PRJCAPTION& prjCap);
	PRJCAPTION GetPrjInfo();

	void clear_mem();
};

#endif // !defined(AFX_TILCANVASMGR_H__2459C0F4_34AB_47A5_A2C9_F5D9D3EEAB0C__INCLUDED_)

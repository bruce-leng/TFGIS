#if !defined(AFX_STATICCANVAS_H__5330CD72_EAB8_4C02_BD3C_1C777F7E9490__INCLUDED_)
#define AFX_STATICCANVAS_H__5330CD72_EAB8_4C02_BD3C_1C777F7E9490__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StaticCanvas.h : header file
//
#include "GuiExtStd.h"

//////////////////////////////////////////////////////////////////////////
class CData
{
public:
	CData();
	~CData();
	
	BOOL Create(int nLen);	
	void ClearMem();
	void MaxMin(double& dbMax, double& dbMin);
	inline int GetLen() const {return m_nDataLen;};
	//{×·¼Ó2005.08.19
	inline void SetData(double* pDataBuf)
	{
		memcpy(m_pDataBuf, pDataBuf, sizeof(double)*m_nDataLen);
	};
	//}×·¼Ó2005.08.19
	inline double* GetData() const {return m_pDataBuf;};
	inline BOOL GetState() const {return m_bDraw;};
	inline void SetState(BOOL bDraw) {m_bDraw = bDraw;};
	inline void TogState() {m_bDraw = !m_bDraw;};
	inline void SetClr(COLORREF clr) {m_clr = clr;};
	inline COLORREF GetClr() {return m_clr;};

private:
	int m_nDataLen;
	double* m_pDataBuf;
	BOOL m_bDraw;
	COLORREF m_clr;
};

//////////////////////////////////////////////////////////////////////////
class CCanvasInfo
{
public:
	CCanvasInfo();
	~CCanvasInfo();
	
	BOOL Create(int nCount, int nLen);
	BOOL ChangeHist(int nIdx, int nLen);
	void ClearAllMem();
	void ClearIdxMem(int nIdx);
	
	inline int GetCount(){return m_nCount;};
	inline BOOL GetBinState(int nIdx) const {return m_pHist[nIdx]->GetState();};
	inline void TogBinState(int nIdx){m_pHist[nIdx]->TogState();};
	inline BOOL GetLogState() {return m_bLog;};
	inline void TogLogState() {m_bLog = ~m_bLog;};
	inline CData* GetCData(int nIdx) {return m_pHist[nIdx];};

private:
	int m_nCount;
	CData** m_pHist;
	BOOL m_bLog;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATICCANVAS_H__5330CD72_EAB8_4C02_BD3C_1C777F7E9490__INCLUDED_)

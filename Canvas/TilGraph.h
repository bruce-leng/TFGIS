// TilGraph.h: interface for the CTilGraph class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TILGRAPH_H__AC7B6A5B_5188_488A_B03A_F5144AB62AE2__INCLUDED_)
#define AFX_TILGRAPH_H__AC7B6A5B_5188_488A_B03A_F5144AB62AE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "Graph\TilTunArc.h"

class AFX_EXT_CLASS CTilGraph  
{
public:
	CTilGraph();
	CTilGraph(CTilGraph& graph);
	CTilGraph(const CString& strPathName);
	virtual ~CTilGraph();

	BOOL Open(const CString& strPathName);
	BOOL Save();
	BOOL SaveAs(const CString& strPathName);

	void SetModifiedFlag(BOOL bFlag = TRUE);
	BOOL IsModified() const;

	void	SetPathName(const CString& strPathName);
	CString GetPathName() const;

	void SetCurIdx(int nIdx);
	int  GetCurIdx() const;

 	void SetZoomFactor(long double dbZoomX, long double dbZoomY);
 	void GetZoomFactor(long double& dbZoomX, long double& dbZoomY);

	int Size();

	void SetShowMode(BOOL bShow = TRUE);
	BOOL GetShowMode() const;

	void Draw(CDC* pDC);
	void Draw(CDC* pDC, const POINT& ptTopLeft);
	void DrawInvert(CDC* pDC, const POINT& ptTopLeft);	// ���ó���y���ϻ�ͼ(��ӡʱʹ��)
	void DrawLayer(CDC* pDC, int nIdx);
//	void StretchAll(CDC* pDC, const CRect& rectSrc, const CRect& rectDst);

	void PushBackGraph(CGraph* pGraph);
	void PushFrontGraph(CGraph* pGraph);
	void InsertGraph(CGraph* pGraph, int nIdx, BOOL bAfter = TRUE);

	void DeleteGraph(int nIdx);

	CGraph* GetGraphByIdx(int nIdx);
	CGraph* GetCurGraph();
	CGraph* GetPreGraph();
	CGraph* GetNextGraph();

	int GetWidth();
	int GetHeight();

	// ��������ߴ�
	BOOL ImportTunArc(const CString& strPathName);
	// ȡ������ߴ�
	CTilTunArc* GetTunArc() { return &m_TunArc; };
	// ��������ߴ�
	void SetTunArc(CTilTunArc& tunArc) {m_TunArc=tunArc;};
	// ��������������ⲿ��ͼ��
	BOOL ClearTunOutside();

private:
	int m_nIdx;
 	long double m_dbZoomX;
 	long double m_dbZoomY;
	BOOL m_bModified;
	BOOL m_bShow;
	CString m_strPathName;
	CObArray m_GraphArray;

	// �������ͼ����
	CTilTunArc m_TunArc;
};

#endif // !defined(AFX_TILGRAPH_H__AC7B6A5B_5188_488A_B03A_F5144AB62AE2__INCLUDED_)

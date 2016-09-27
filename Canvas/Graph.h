// Graph.h: interface for the CGraph class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPH_H__E2719691_998A_44D7_A0AF_C36C3874A611__INCLUDED_)
#define AFX_GRAPH_H__E2719691_998A_44D7_A0AF_C36C3874A611__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph\Unit.h"
class AFX_EXT_CLASS CGraph : public CObject
{
public:
	CGraph();
	CGraph(CGraph& graph);
	CGraph(const CString& strPathName);
	virtual ~CGraph();

	virtual void Serialize(CArchive& ar);

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

	void DrawAll(CDC* pDC);
	//	void StretchAll(CDC* pDC, const CRect& rectSrc, const CRect& rectDst);

	void PushBackUnit(CUnit* pUnit);
	void PushFrontUnit(CUnit* pUnit);
	void InsertUnit(CUnit* pUnit, int nIdx, BOOL bAfter = TRUE);

	void RemoveAll();
	void DeleteUnit(int nIdx);

	//…Ë÷√∑‚±’«˙œﬂÃÓ≥‰Õº‘™(2016.01.24)
	void SetFillMap(int nMapIdx){m_nMapIdx=nMapIdx;}

	CUnit* GetUnitByIdx(int nIdx);
	CUnit* GetCurUnit();
	CUnit* GetPreUnit();
	CUnit* GetNextUnit();

private:
	int m_nIdx;
	long double m_dbZoomX;
	long double m_dbZoomY;
	BOOL m_bModified;
	BOOL m_bShow;
	CString m_strPathName;
	CObArray m_UnitArray;
	int m_nMapIdx;//ÃÓ≥‰Õº–Ú∫≈
};

#endif // !defined(AFX_GRAPH_H__E2719691_998A_44D7_A0AF_C36C3874A611__INCLUDED_)

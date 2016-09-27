// Graph.cpp: implementation of the CGraph class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "canvas.h"
#include "Graph.h"
#include "TilGraph.h"
//#include "Graph/Ellipse.h"
//#include "Graph/EllipseArc.h"
#include "Graph/Line.h"
#include "Graph/Polygon.h"
//#include "Graph/Rectangle.h"
//#include "Graph/Round.h"
//#include "Graph/RoundArc.h"
#include "Graph/Text.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGraph::CGraph()
{
	m_bModified = FALSE;
	m_nIdx = -1;
	m_bShow = TRUE;
	m_dbZoomX = m_dbZoomY = 1.0;
	m_nMapIdx = -1;
}

CGraph::CGraph(CGraph& graph)
{
	m_nIdx = -1;
	m_bModified = FALSE;
	m_bShow = TRUE;
	m_dbZoomX = graph.m_dbZoomX;
	m_dbZoomY = graph.m_dbZoomY;
	for (int i = 0; i < graph.Size(); i ++)
	{
		CUnit* pTemp=NULL;
		CUnit* pUnit=graph.GetUnitByIdx(i);
		if (pUnit->IsKindOf(RUNTIME_CLASS(CLine)))
			pTemp = new CLine(*((CLine*)pUnit));
		else if (pUnit->IsKindOf(RUNTIME_CLASS(CPolygon)))
			pTemp = new CPolygon(*((CPolygon*)pUnit));
		else if (pUnit->IsKindOf(RUNTIME_CLASS(CText)))
			pTemp = new CText(*((CText*)pUnit));

		m_UnitArray.Add(pTemp);
	}
	m_nMapIdx = graph.m_nMapIdx;
}

CGraph::CGraph(const CString& strPathName)
{
	Open(strPathName);
}

CGraph::~CGraph()
{
	for (int i = 0; i < m_UnitArray.GetSize(); i ++)
		delete m_UnitArray.GetAt(i);
}

void CGraph::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// 保存类的数量
		ar << m_UnitArray.GetSize();

		// 保存图形
		for (int i=0; i<m_UnitArray.GetSize(); i++)
		{
			CUnit* pUnit = (CUnit*)m_UnitArray.GetAt(i);
			//		if (pUnit->IsKindOf(RUNTIME_CLASS(CEllipse)))
			//			ar << Graph_Ellipse;
			//		else if (pUnit->IsKindOf(RUNTIME_CLASS(CEllipseArc)))
			//			ar << Graph_EllipseArc;
			/*else */if (pUnit->IsKindOf(RUNTIME_CLASS(CLine)))
				ar << Graph_Line;
			else if (pUnit->IsKindOf(RUNTIME_CLASS(CPolygon)))
				ar << Graph_Polygon;
			//		else if (pUnit->IsKindOf(RUNTIME_CLASS(CRectangle)))
			//			ar << Graph_Rectangle;
			//		else if (pUnit->IsKindOf(RUNTIME_CLASS(CRound)))
			//			ar << Graph_Round;
			//		else if (pUnit->IsKindOf(RUNTIME_CLASS(CRoundArc)))
			//			ar << Graph_RoundArc;
			else if (pUnit->IsKindOf(RUNTIME_CLASS(CText)))
				ar << Graph_Text;
			//		else if (pUnit->IsKindOf(RUNTIME_CLASS(CMagic)))
			//			ar << Graph_Magic;
			
			pUnit->Serialize(ar);
		}
	}
	else	// 读取
	{
		// 类的数量
		int nSize;
		ar >> nSize;
		
		// 读取图形
		CUnit* pUnit = NULL;
		for (int i=0; i<nSize; i++)
		{
			int nType;
			ar >> nType;
			switch (nType)
			{
	//		case Graph_Ellipse:
	//			pUnit = new CEllipse;
	//			break;
	//		case Graph_EllipseArc:
	//			pUnit = new CEllipseArc;
	//			break;
			case Graph_Line:
				pUnit = new CLine;
				break;
			case Graph_Polygon:
				pUnit = new CPolygon;
				break;
	//		case Graph_Rectangle:
	//			pUnit = new CRectangle;
	//			break;
	//		case Graph_Round:
	//			pUnit = new CRound;
	//			break;
	//		case Graph_RoundArc:
	//			pUnit = new CRoundArc;
	//			break;
	//		case Graph_Magic:
	////			pUnit = new CDrawMagic;
	//			break;
			case Graph_Text:
				pUnit = new CText;
				break;
			}
			pUnit->Serialize(ar);
			m_UnitArray.Add(pUnit);
		}
	}
}

BOOL CGraph::Open(const CString& strPathName)
{
	m_UnitArray.RemoveAll();
// 	m_nIdx = -1;
// 	m_bModified = FALSE;
// 	m_bShow = TRUE;
// 	m_dbZoomX = m_dbZoomY = 1.0;
	m_strPathName = strPathName;
	CFile file;
	if (!file.Open(strPathName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeRead))
	{
		TRACE0("Failed to open graph file.\n");
		return FALSE;
	}
	// 若是新建立的文件，则直接返回
	if (file.GetLength() == 0)
	{
		file.Close();
		return TRUE;
	}

	CArchive ar(&file, CArchive::load);
	Serialize(ar);

	ar.Close();
	file.Close();

	return FALSE;
}

BOOL CGraph::Save()
{
	return SaveAs(m_strPathName);
}

BOOL CGraph::SaveAs(const CString& strPathName)
{
	CFile file;
	if (!file.Open(strPathName, CFile::modeCreate | CFile::modeWrite))
	{
		TRACE0("Failed to save graph file.\n");
		return FALSE;
	}
	CArchive ar(&file, CArchive::store);
	Serialize(ar);
//	m_UnitArray.Serialize(ar);

	m_bModified = FALSE;
	m_strPathName = strPathName;

	ar.Close();
	file.Close();
	return FALSE;
}

void CGraph::SetModifiedFlag(BOOL bFlag)
{
	m_bModified = bFlag;
}

BOOL CGraph::IsModified()  const
{
	return m_bModified;
}

void CGraph::SetPathName(const CString& strPathName)
{
	if (m_strPathName != strPathName)
	{
		m_strPathName = strPathName;
	}
}

CString CGraph::GetPathName() const
{
	return m_strPathName;
}

void CGraph::SetCurIdx(int nIdx)
{
	ASSERT(nIdx >= 0 && nIdx < m_UnitArray.GetSize());
	m_nIdx = nIdx;
}

int  CGraph::GetCurIdx() const
{
	return m_nIdx;
}

void CGraph::SetZoomFactor(long double dbZoomX, long double dbZoomY)
{
	m_dbZoomX = dbZoomX;
	m_dbZoomY = dbZoomY;

	// 设置每个图元的缩放率
	for (int i = 0; i < Size(); i ++)
	{
 		CUnit* pUnit = (CUnit*)m_UnitArray.GetAt(i);
 		pUnit->SetZoomFactor(m_dbZoomX, m_dbZoomY);
	}
}

void CGraph::GetZoomFactor(long double& dbZoomX, long double& dbZoomY)
{
dbZoomX = m_dbZoomX;
dbZoomY = m_dbZoomY;
}

int CGraph::Size()
{
	return m_UnitArray.GetSize();
}

void CGraph::SetShowMode(BOOL bShow)
{
	m_bShow = bShow;
}

BOOL CGraph::GetShowMode() const
{
	return m_bShow;
}

void CGraph::DrawAll(CDC* pDC)
{
	if (!m_bShow)
		return;
	
	for (int i = 0; i < Size(); i ++)
	{
		CUnit*pUnit=(CUnit*)m_UnitArray.GetAt(i);
		pUnit->m_DrawingMode = R2_COPYPEN;
		// pUnit->m_PenColor = RGB(255,0,0);
		//	pUnit->m_PenWidth = 1;
		pUnit->SetFillMap(m_nMapIdx);
		pUnit->DrawStatic(pDC);
		if(pUnit->m_HaveFindFirst)
			pUnit->ShowSelectPoint(pDC);
	}
}

void CGraph::PushBackUnit(CUnit* pUnit)
{
	pUnit->SetZoomFactor(m_dbZoomX, m_dbZoomY);
	m_nIdx = m_UnitArray.Add(pUnit);
}

void CGraph::PushFrontUnit(CUnit* pUnit)
{
	pUnit->SetZoomFactor(m_dbZoomX, m_dbZoomY);
	m_UnitArray.InsertAt(0, pUnit);
	m_nIdx ++;
}

void CGraph::InsertUnit(CUnit* pUnit, int nIdx, BOOL bAfter)
{
	pUnit->SetZoomFactor(m_dbZoomX, m_dbZoomY);
	m_UnitArray.InsertAt(nIdx, pUnit);
}

void CGraph::RemoveAll()
{
	for (int i=0; i<m_UnitArray.GetSize(); i++)
	{
		delete m_UnitArray.GetAt(i);
	}
	m_UnitArray.RemoveAll();
	m_nIdx = -1;
}

void CGraph::DeleteUnit(int nIdx)
{
	ASSERT(nIdx >= 0 && nIdx < m_UnitArray.GetSize());
	m_UnitArray.RemoveAt(nIdx);
}

CUnit* CGraph::GetUnitByIdx(int nIdx)
{
	ASSERT(nIdx >= 0 && nIdx < m_UnitArray.GetSize());
	return (CUnit*)m_UnitArray.GetAt(nIdx);
}

CUnit* CGraph::GetCurUnit()
{
	if (m_nIdx < 0)
		return NULL;
	return (CUnit*)m_UnitArray.GetAt(m_nIdx);
}

CUnit* CGraph::GetPreUnit()
{
	if (m_nIdx > 0)
	{
		m_nIdx --;
		return (CUnit*)m_UnitArray.GetAt(m_nIdx-1);
	}
	else
		return NULL;
}

CUnit* CGraph::GetNextUnit()
{
	if (m_nIdx+1 < m_UnitArray.GetSize())
	{
		m_nIdx ++;
		return (CUnit*)m_UnitArray.GetAt(m_nIdx+1);
	}
	else
		return NULL;
}

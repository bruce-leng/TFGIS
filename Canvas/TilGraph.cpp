// TilGraph.cpp: implementation of the CTilGraph class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TilGraph.h"
//#include "Graph/Ellipse.h"
//#include "Graph/EllipseArc.h"
#include "Graph/Line.h"
#include "Graph/Polygon.h"
//#include "Graph/Rectangle.h"
//#include "Graph/Round.h"
//#include "Graph/RoundArc.h"
#include "Graph/Text.h"
#include "ximage.h"
#include "TilComFunction.h"
#include <float.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTilGraph::CTilGraph()
{
	m_bModified = FALSE;
	m_nIdx = -1;
	m_bShow = TRUE;
	m_dbZoomX = m_dbZoomY = 1.0;
}

CTilGraph::CTilGraph(CTilGraph& graph)
{
	m_nIdx = -1;
	m_bModified = FALSE;
	m_bShow = TRUE;
	m_dbZoomX = graph.m_dbZoomX;
	m_dbZoomY = graph.m_dbZoomY;

	// ���������ߴ粢ת����ͼ�β㱣��
	m_TunArc = graph.m_TunArc;

	// ��������ͼ�β�
	for (int i=0; i<graph.Size(); i++)
	{
		CGraph* pGraph = new CGraph(*(graph.GetGraphByIdx(i)));
		m_GraphArray.Add(pGraph);
	}

	// ���õ�ǰĬ��ͼ�β�Ϊ�����
	m_nIdx = GRAPH_LAYER_JOINT;
}

CTilGraph::CTilGraph(const CString& strPathName)
{
	Open(strPathName);
}

CTilGraph::~CTilGraph()
{
	for (int i = 0; i < m_GraphArray.GetSize(); i ++)
		delete m_GraphArray.GetAt(i);
}

BOOL CTilGraph::Open(const CString& strPathName)
{
	m_GraphArray.RemoveAll();
	m_nIdx = -1;
	m_bModified = FALSE;
	m_bShow = TRUE;
	m_dbZoomX = m_dbZoomY = 1.0;
	m_strPathName = strPathName;
	CFile file;
	if (!file.Open(strPathName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeRead))
	{
		TRACE0("Failed to open graph file.\n");
		return FALSE;
	}

	// �����½������ļ��������Ҳ�ͼ�β㡢������������β�
	if (file.GetLength() == 0)
	{
		file.Close();

		// ��1��:�½�������β�
		CGraph* pTunArc = new CGraph;
		m_GraphArray.Add(pTunArc);

		// ��2��:�½��Ҳ�ͼ�β�
		CGraph* pRocks = new CGraph;
		m_GraphArray.Add(pRocks);

		// ��3��:�½������
		CGraph* pJoint = new CGraph;
		m_GraphArray.Add(pJoint);

		// ��4��:������Ϣ��
		CGraph* pOther = new CGraph;
		m_GraphArray.Add(pOther);

		// ���õ�ǰĬ��ͼ�β�Ϊ�����
		m_nIdx = GRAPH_LAYER_JOINT;

		return TRUE;
	}

	CArchive ar(&file, CArchive::load);

	// ��ȡ����
	int nSize;
	ar >> nSize;

	//////////////////////////////////////////////////////////////////////////
// 	// ��ȡ�������ߴ粢ת���ɶ���ߴ��,����ߴ��Ϊ��0��(090327)
// 	if (m_TunArc.Serialize(ar))
// 	{
// 		// �������ж���ߴ�ʱ��������ߴ�ת��������ͼ�β���
// 		CUnit* pTunArc = new CPolygon;
// 		m_TunArc.Transferto(pTunArc, 125, 500, 0.125);
// 
// 		CGraph* pLayerTunArc = new CGraph;
// 		int nNum = pTunArc->GetPointNum();
// 		pLayerTunArc->PushBackUnit(pTunArc);
// 		m_GraphArray.Add(pLayerTunArc);
// 	}
// 	else
// 	{
// 		// ��δ�洢����ߴ�ʱ������һ���յĶ���ߴ�ͼ��
// 		CGraph* pLayerTunArc = new CGraph;
// 		m_GraphArray.Add(pLayerTunArc);
// 	}

	//////////////////////////////////////////////////////////////////////////

	// ��ȡ
	for (int i=0; i<nSize; i++)
	{
		CGraph* pGraph = new CGraph;
		pGraph->Serialize(ar);
		m_GraphArray.Add(pGraph);
	}

	// ���õ�ǰĬ��ͼ�β�Ϊ�����
	m_nIdx = GRAPH_LAYER_JOINT;

	ar.Close();
	file.Close();

	return FALSE;
}

BOOL CTilGraph::Save()
{
	return SaveAs(m_strPathName);
}

BOOL CTilGraph::SaveAs(const CString& strPathName)
{
	CFile file;
	if (!file.Open(strPathName, CFile::modeCreate | CFile::modeWrite))
	{
		TRACE0("Failed to save graph file.\n");
		return FALSE;
	}

	// �洢ͼ��������ͼ��
	CArchive ar(&file, CArchive::store);
	ar << m_GraphArray.GetSize();

	// ����������������ѱ�ɾ���������m_TunArc�е��������ߴ�����
// 	CGraph* pGraph = (CGraph*)m_GraphArray.GetAt(GRAPH_LAYER_TUNARC);
// 	if (pGraph->Size()==0)
// 		m_TunArc.ClearMem();
// 	else
// 	{
// 		// ����������汻�ƶ������䱻�ƶ��������ݱ�����pGraph����
// 		// ����ʱ��Ҫ������λ�Ʊ仯����ı���������λ��
// // 		CUnit* pUnit = pGraph->GetUnitByIdx(0);
// // 		m_TunArc.Move(pUnit->m_SecondPoint.x - pUnit->m_FirstPoint.x,
// // 						pUnit->m_SecondPoint.y - pUnit->m_FirstPoint.y);
// 	}
// 
// 	// �洢����ߴ�����
// 	m_TunArc.Serialize(ar);
// 	pGraph->SetModifiedFlag(FALSE);

	// ����ж���ߴ����ݣ���i=1��ʼ��������ͼ��
	// i=0ʱpGraphΪ���濪�������ߣ����ر��棬��Ϊm_TunArcΪ
	// ����������ԭʼ��״����
	for (int i=0; i < m_GraphArray.GetSize(); i ++)
	{
		CGraph* pGraph = (CGraph*)m_GraphArray.GetAt(i);
		pGraph->Serialize(ar);
		pGraph->SetModifiedFlag(FALSE);
	}
//	m_GraphArray.Serialize(ar);

	m_bModified = FALSE;
	m_strPathName = strPathName;

	ar.Close();
	file.Close();
	return FALSE;
}

void CTilGraph::SetModifiedFlag(BOOL bFlag)
{
	m_bModified = bFlag;
}

BOOL CTilGraph::IsModified()  const
{
	for (int i=0; i<m_GraphArray.GetSize(); i++)
	{
 		CGraph* pGraph = (CGraph*)m_GraphArray.GetAt(i);
		if (pGraph->IsModified())
			return TRUE;
	}

	return m_bModified;
}

void CTilGraph::SetPathName(const CString& strPathName)
{
	if (m_strPathName != strPathName)
	{
		m_strPathName = strPathName;
		m_bModified = TRUE;
	}
}

CString CTilGraph::GetPathName() const
{
	return m_strPathName;
}

void CTilGraph::SetCurIdx(int nIdx)
{
	ASSERT(nIdx >= 0 && nIdx < m_GraphArray.GetSize());
	m_nIdx = nIdx;
}

int  CTilGraph::GetCurIdx() const
{
	return m_nIdx;
}

void CTilGraph::SetZoomFactor(long double dbZoomX, long double dbZoomY)
{
	m_dbZoomX = dbZoomX;
	m_dbZoomY = dbZoomY;

	// ����ÿ��ͼԪ��������
	for (int i = 0; i < Size(); i ++)
	{
 		CGraph* pUnit = (CGraph*)m_GraphArray.GetAt(i);
 		pUnit->SetZoomFactor(m_dbZoomX, m_dbZoomY);
	}
}

void CTilGraph::GetZoomFactor(long double& dbZoomX, long double& dbZoomY)
{
dbZoomX = m_dbZoomX;
dbZoomY = m_dbZoomY;
}
 
int CTilGraph::Size()
{
	return m_GraphArray.GetSize();
}

void CTilGraph::SetShowMode(BOOL bShow)
{
	m_bShow = bShow;
}

BOOL CTilGraph::GetShowMode() const
{
	return m_bShow;
}

// ��ʾ����ͼ�β�
void CTilGraph::Draw(CDC* pDC)
{
	if (!m_bShow)
		return;

	for (int i = 0; i < Size(); i ++)
	{
		CGraph*pGraph=(CGraph*)m_GraphArray.GetAt(i);
		if(i != GRAPH_LAYER_TUNARC)
			pGraph->SetFillMap(0);
		pGraph->DrawAll(pDC);
	}
}

void CTilGraph::Draw(CDC* pDC, const POINT& ptTopLeft)
{
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	
	int nZoomWidth = (GetWidth()+20)*m_dbZoomX;
	int nZoomHeight = (GetHeight()+20)*m_dbZoomY;
	
	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pDC, nZoomWidth, nZoomHeight);
	
	CBitmap* pOldBitmap = (CBitmap*)memDC.SelectObject(&memBitmap);
	
	// ���ñ���ɫ
	char* pBits = new char [nZoomWidth*4*nZoomHeight];
	memset(pBits, 255, sizeof(char)*nZoomWidth*4*nZoomHeight);
	memBitmap.SetBitmapBits(sizeof(char)*nZoomWidth*4*nZoomHeight, pBits);
	delete [] pBits;
	
	Draw(&memDC);
	
	pDC->BitBlt(ptTopLeft.x, ptTopLeft.y, nZoomWidth, nZoomHeight, &memDC, 0, 0, SRCCOPY);
	
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
}

// ���ó���y���ϻ�ͼ(��ӡʱʹ��)
void CTilGraph::DrawInvert(CDC* pDC, const POINT& ptTopLeft)
{
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	
	int nZoomWidth = (GetWidth()+20)*m_dbZoomX;
	int nZoomHeight = (GetHeight()+20)*m_dbZoomY;
	
	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pDC, nZoomWidth, nZoomHeight);
	
	CBitmap* pOldBitmap = (CBitmap*)memDC.SelectObject(&memBitmap);
	
	// ���ñ���ɫ
	char* pBits = new char [nZoomWidth*4*nZoomHeight];
	memset(pBits, 255, nZoomWidth*4*nZoomHeight);
	memBitmap.SetBitmapBits(nZoomWidth*4*nZoomHeight, pBits);
	delete [] pBits;
	
	Draw(&memDC);

	// ��תͼ���ŵ�ָ��λ��
	pDC->StretchBlt(ptTopLeft.x, ptTopLeft.y, nZoomWidth, -nZoomHeight, 
								&memDC, 0, 0, nZoomWidth, nZoomHeight, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
}

// ��ʾ��nIdx��ͼ�β�
void CTilGraph::DrawLayer(CDC* pDC, int nIdx)
{
	if (!m_bShow)
		return;
	
	CGraph*pGraph=(CGraph*)m_GraphArray.GetAt(nIdx);
	pGraph->DrawAll(pDC);
}

void CTilGraph::PushBackGraph(CGraph* pGraph)
{
	pGraph->SetZoomFactor(m_dbZoomX, m_dbZoomY);
	m_nIdx = m_GraphArray.Add(pGraph);
}

void CTilGraph::PushFrontGraph(CGraph* pGraph)
{
	pGraph->SetZoomFactor(m_dbZoomX, m_dbZoomY);
	m_GraphArray.InsertAt(0, pGraph);
	m_nIdx ++;
}
void CTilGraph::InsertGraph(CGraph* pGraph, int nIdx, BOOL bAfter)
{
	pGraph->SetZoomFactor(m_dbZoomX, m_dbZoomY);
	m_GraphArray.InsertAt(nIdx, pGraph);
}

void CTilGraph::DeleteGraph(int nIdx)
{
	ASSERT(nIdx >= 0 && nIdx < m_GraphArray.GetSize());
	delete m_GraphArray.GetAt(nIdx);
	m_GraphArray.RemoveAt(nIdx);
}

CGraph* CTilGraph::GetGraphByIdx(int nIdx)
{
	ASSERT(nIdx >= 0 && nIdx < m_GraphArray.GetSize());
//	m_bModified = TRUE;
	return (CGraph*)m_GraphArray.GetAt(nIdx);
}

CGraph* CTilGraph::GetCurGraph()
{
	if (m_nIdx < 0)
		return NULL;
	return (CGraph*)m_GraphArray.GetAt(m_nIdx);
}

CGraph* CTilGraph::GetPreGraph()
{
	if (m_nIdx > 0)
	{
		m_nIdx --;
		return (CGraph*)m_GraphArray.GetAt(m_nIdx-1);
	}
	else
		return NULL;
}

CGraph* CTilGraph::GetNextGraph()
{
	if (m_nIdx+1 < m_GraphArray.GetSize())
	{
		m_nIdx ++;
		return (CGraph*)m_GraphArray.GetAt(m_nIdx+1);
	}
	else
		return NULL;
}

int CTilGraph::GetWidth()
{
	double dbLeft, dbRight;
	dbLeft = FLT_MAX;
	dbRight = FLT_MIN;

	CGraph* pGraph = GetGraphByIdx(0);	// GRAPH_LAYER_TUNARC��
	if (pGraph->Size() > 0)
	{
		CUnit* pTunArc = pGraph->GetUnitByIdx(0);
		int nNum = pTunArc->GetPointNum();
		for (int i=0; i<nNum; i++)
		{
			POINT2DF pt = pTunArc->GetPoint(i);
			if (dbLeft > pt.x)	dbLeft = pt.x;
			if (dbRight < pt.x)	dbRight = pt.x;
		}
	}
	else
	{
		// ��1,2��Ϊ�Ҳ�Ŀ���ͽ����(����GRAPH_LAYER_ROCKS,GRAPH_LAYER_JOINT��)
		for (int i=1; i<3; i++)
		{
			pGraph = GetGraphByIdx(i);
			int nUnitNum = pGraph->Size();
			for (int j=0; j<nUnitNum; j++)
			{
				CUnit* pUnit = pGraph->GetUnitByIdx(j);
				int nNum = pUnit->GetPointNum();
				for (int k=0; k<nNum; k++)
				{
					POINT2DF pt = pUnit->GetPoint(k);
					if (dbLeft > pt.x)	dbLeft = pt.x;
					if (dbRight < pt.x)	dbRight = pt.x;
				}
			}
		}
	}
	return int(dbRight-dbLeft+0.5);
}

int CTilGraph::GetHeight()
{
	double dbTop, dbBottom;
	dbTop = FLT_MAX;
	dbBottom = FLT_MIN;

	CGraph* pGraph = GetGraphByIdx(GRAPH_LAYER_TUNARC);
	if (pGraph->Size() > 0)
	{
		CUnit* pTunArc = pGraph->GetUnitByIdx(0);
		int nNum = pTunArc->GetPointNum();
		for (int i=0; i<nNum; i++)
		{
			POINT2DF pt = pTunArc->GetPoint(i);
			if (dbTop > pt.y)	dbTop = pt.y;
			if (dbBottom < pt.y)dbBottom = pt.y;
		}
	}
	else
	{
		// ��1,2��Ϊ�Ҳ�Ŀ���ͽ����(����GRAPH_LAYER_ROCKS,GRAPH_LAYER_JOINT��)
		for (int i=1; i<3; i++)
		{
			pGraph = GetGraphByIdx(i);
			int nUnitNum = pGraph->Size();
			for (int j=0; j<nUnitNum; j++)
			{
				CUnit* pUnit = pGraph->GetUnitByIdx(j);
				int nNum = pUnit->GetPointNum();
				for (int k=0; k<nNum; k++)
				{
					POINT2DF pt = pUnit->GetPoint(k);
					if (dbTop > pt.y)	dbTop = pt.y;
					if (dbBottom < pt.y)dbBottom = pt.y;
				}
			}
		}
	}
	return int(dbBottom-dbTop+0.5);
}

// ��������ߴ�
BOOL CTilGraph::ImportTunArc(const CString& strPathName)
{
	if (m_TunArc.Import(strPathName))
	{
		CGraph* pLayerTunArc = (CGraph*)m_GraphArray.GetAt(0);
		CUnit* pTunArc;
		if (pLayerTunArc->Size()>0)
			pTunArc = pLayerTunArc->GetUnitByIdx(0);
		else
		{
			pTunArc = new CPolygon;
			pLayerTunArc->PushBackUnit(pTunArc);
		}
		m_TunArc.Transferto(pTunArc,125, 500, 0.125);
		pTunArc->m_PenColor = RGB(0,0,0);	// ��ɫ

		return TRUE;
	}

	return FALSE;
}

// ��������������ⲿ��ͼ��
BOOL CTilGraph::ClearTunOutside()
{
	CGraph* pTunArc = GetGraphByIdx(GRAPH_LAYER_TUNARC);
	CGraph* pRocks = GetGraphByIdx(GRAPH_LAYER_ROCKS);
	CGraph* pJoints = GetGraphByIdx(GRAPH_LAYER_JOINT);

	// ���������β�����
	if (pTunArc->Size() == 0)
		return FALSE;

	// ��ȡ��������γɵĶ����
	int i, j;
	int nSize = pTunArc->GetUnitByIdx(0)->GetPointNum();
	POINT* pPoints = new POINT [nSize];
	for (i=0; i<nSize; i++)
	{
		POINT2DF pt = pTunArc->GetUnitByIdx(0)->GetPoint(i);
		pPoints[i].x = int(pt.x+0.5);
		pPoints[i].y = int(pt.y+0.5);
	}

	// ȡ���������ͼ������
	CRgn rgn;
	rgn.CreatePolygonRgn(pPoints, nSize, WINDING);

	// ɾ��������������ͼ��
	nSize = pJoints->Size();
	for (i=nSize-1; i>=0; i--)
	{
		CUnit* pUnit = pJoints->GetUnitByIdx(i);
		if (pUnit->IsKindOf(RUNTIME_CLASS(CLine)))
		{
			POINT pt1, pt2;
			pt1.x = int(pUnit->m_FirstPoint.x+0.5);
			pt1.y = int(pUnit->m_FirstPoint.y+0.5);
			pt2.x = int(pUnit->m_SecondPoint.x+0.5);
			pt2.y = int(pUnit->m_SecondPoint.y+0.5);

			if (!rgn.PtInRegion(pt1) || !rgn.PtInRegion(pt2))
			{
				delete pJoints->GetUnitByIdx(i);
				pJoints->DeleteUnit(i);
			}
		}
		else if (pUnit->IsKindOf(RUNTIME_CLASS(CPolygon)))
		{
			try
			{
				for (j=0; j<pUnit->GetPointNum(); j++)
				{
					POINT pt;
					pt.x = int(pUnit->GetPoint(j).x+0.5);
					pt.y = int(pUnit->GetPoint(j).y+0.5);
					if (!rgn.PtInRegion(pt))
					{
						delete pJoints->GetUnitByIdx(i);
						pJoints->DeleteUnit(i);
						break;
					}
				}
			}
			catch (CException* e)
			{
				TRACE("\n��%d��ͼԪ�ĵ���Ϊ%d�����ڵ�%d�����������\n",i,pUnit->GetPointNum(),j);
				CString str;
				for (j=0;j<pUnit->GetPointNum(); j++)
				{
					POINT pt;
					pt.x = int(pUnit->GetPoint(j).x+0.5);
					pt.y = int(pUnit->GetPoint(j).y+0.5);
					TRACE("��%d��:(%d,%d)\n",pt.x,pt.y);
				}
			}
		}
	}

	delete [] pPoints;

	return TRUE;
}

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

	// 读隧道断面尺寸并转化成图形层保存
	m_TunArc = graph.m_TunArc;

	// 读入所有图形层
	for (int i=0; i<graph.Size(); i++)
	{
		CGraph* pGraph = new CGraph(*(graph.GetGraphByIdx(i)));
		m_GraphArray.Add(pGraph);
	}

	// 设置当前默认图形层为节理层
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

	// 若是新建立的文件，则建立岩层图形层、节理层和隧道拱形层
	if (file.GetLength() == 0)
	{
		file.Close();

		// 第1层:新建隧道拱形层
		CGraph* pTunArc = new CGraph;
		m_GraphArray.Add(pTunArc);

		// 第2层:新建岩层图形层
		CGraph* pRocks = new CGraph;
		m_GraphArray.Add(pRocks);

		// 第3层:新建节理层
		CGraph* pJoint = new CGraph;
		m_GraphArray.Add(pJoint);

		// 第4层:其它信息层
		CGraph* pOther = new CGraph;
		m_GraphArray.Add(pOther);

		// 设置当前默认图形层为节理层
		m_nIdx = GRAPH_LAYER_JOINT;

		return TRUE;
	}

	CArchive ar(&file, CArchive::load);

	// 读取层数
	int nSize;
	ar >> nSize;

	//////////////////////////////////////////////////////////////////////////
// 	// 读取隧道断面尺寸并转化成断面尺寸层,断面尺寸层为第0层(090327)
// 	if (m_TunArc.Serialize(ar))
// 	{
// 		// 当保存有断面尺寸时，将断面尺寸转化至断面图形层中
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
// 		// 当未存储断面尺寸时，保存一个空的断面尺寸图层
// 		CGraph* pLayerTunArc = new CGraph;
// 		m_GraphArray.Add(pLayerTunArc);
// 	}

	//////////////////////////////////////////////////////////////////////////

	// 读取
	for (int i=0; i<nSize; i++)
	{
		CGraph* pGraph = new CGraph;
		pGraph->Serialize(ar);
		m_GraphArray.Add(pGraph);
	}

	// 设置当前默认图形层为节理层
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

	// 存储图形数量及图形
	CArchive ar(&file, CArchive::store);
	ar << m_GraphArray.GetSize();

	// 如果隧道断面层数据已被删除，则清除m_TunArc中的隧道断面尺寸数据
// 	CGraph* pGraph = (CGraph*)m_GraphArray.GetAt(GRAPH_LAYER_TUNARC);
// 	if (pGraph->Size()==0)
// 		m_TunArc.ClearMem();
// 	else
// 	{
// 		// 假如隧道断面被移动过，其被移动过的数据保存在pGraph当中
// 		// 保存时需要根据其位移变化情况改变隧道断面的位置
// // 		CUnit* pUnit = pGraph->GetUnitByIdx(0);
// // 		m_TunArc.Move(pUnit->m_SecondPoint.x - pUnit->m_FirstPoint.x,
// // 						pUnit->m_SecondPoint.y - pUnit->m_FirstPoint.y);
// 	}
// 
// 	// 存储断面尺寸数据
// 	m_TunArc.Serialize(ar);
// 	pGraph->SetModifiedFlag(FALSE);

	// 如果有断面尺寸数据，从i=1开始保存后面的图层
	// i=0时pGraph为断面开挖轮廓线，不必保存，因为m_TunArc为
	// 开挖轮廓线原始形状参数
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

	// 设置每个图元的缩放率
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

// 显示所有图形层
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
	
	// 设置背景色
	char* pBits = new char [nZoomWidth*4*nZoomHeight];
	memset(pBits, 255, sizeof(char)*nZoomWidth*4*nZoomHeight);
	memBitmap.SetBitmapBits(sizeof(char)*nZoomWidth*4*nZoomHeight, pBits);
	delete [] pBits;
	
	Draw(&memDC);
	
	pDC->BitBlt(ptTopLeft.x, ptTopLeft.y, nZoomWidth, nZoomHeight, &memDC, 0, 0, SRCCOPY);
	
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
}

// 设置成正y向上画图(打印时使用)
void CTilGraph::DrawInvert(CDC* pDC, const POINT& ptTopLeft)
{
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	
	int nZoomWidth = (GetWidth()+20)*m_dbZoomX;
	int nZoomHeight = (GetHeight()+20)*m_dbZoomY;
	
	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pDC, nZoomWidth, nZoomHeight);
	
	CBitmap* pOldBitmap = (CBitmap*)memDC.SelectObject(&memBitmap);
	
	// 设置背景色
	char* pBits = new char [nZoomWidth*4*nZoomHeight];
	memset(pBits, 255, nZoomWidth*4*nZoomHeight);
	memBitmap.SetBitmapBits(nZoomWidth*4*nZoomHeight, pBits);
	delete [] pBits;
	
	Draw(&memDC);

	// 翻转图像后放到指定位置
	pDC->StretchBlt(ptTopLeft.x, ptTopLeft.y, nZoomWidth, -nZoomHeight, 
								&memDC, 0, 0, nZoomWidth, nZoomHeight, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
}

// 显示第nIdx个图形层
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

	CGraph* pGraph = GetGraphByIdx(0);	// GRAPH_LAYER_TUNARC层
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
		// 第1,2层为岩层目标层和节理层(即：GRAPH_LAYER_ROCKS,GRAPH_LAYER_JOINT层)
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
		// 第1,2层为岩层目标层和节理层(即：GRAPH_LAYER_ROCKS,GRAPH_LAYER_JOINT层)
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

// 输入隧道尺寸
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
		pTunArc->m_PenColor = RGB(0,0,0);	// 黑色

		return TRUE;
	}

	return FALSE;
}

// 清除掌子面区域外部的图形
BOOL CTilGraph::ClearTunOutside()
{
	CGraph* pTunArc = GetGraphByIdx(GRAPH_LAYER_TUNARC);
	CGraph* pRocks = GetGraphByIdx(GRAPH_LAYER_ROCKS);
	CGraph* pJoints = GetGraphByIdx(GRAPH_LAYER_JOINT);

	// 如果隧道拱形不存在
	if (pTunArc->Size() == 0)
		return FALSE;

	// 获取隧道拱形形成的多段线
	int i, j;
	int nSize = pTunArc->GetUnitByIdx(0)->GetPointNum();
	POINT* pPoints = new POINT [nSize];
	for (i=0; i<nSize; i++)
	{
		POINT2DF pt = pTunArc->GetUnitByIdx(0)->GetPoint(i);
		pPoints[i].x = int(pt.x+0.5);
		pPoints[i].y = int(pt.y+0.5);
	}

	// 取得隧道断面图形区域
	CRgn rgn;
	rgn.CreatePolygonRgn(pPoints, nSize, WINDING);

	// 删除隧道断面以外的图形
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
				TRACE("\n第%d个图元的点数为%d个，在第%d个点出现问题\n",i,pUnit->GetPointNum(),j);
				CString str;
				for (j=0;j<pUnit->GetPointNum(); j++)
				{
					POINT pt;
					pt.x = int(pUnit->GetPoint(j).x+0.5);
					pt.y = int(pUnit->GetPoint(j).y+0.5);
					TRACE("第%d点:(%d,%d)\n",pt.x,pt.y);
				}
			}
		}
	}

	delete [] pPoints;

	return TRUE;
}

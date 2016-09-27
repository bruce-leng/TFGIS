// TilCanvasMgr.cpp: implementation of the CTilCanvasMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TilCanvasMgr.h"
#include "TilComFunction.h"
#include "Graph\Polygon.h"
#include "Progress.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define VERSION "1.0.0.1"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTilCanvasMgr::CTilCanvasMgr() : m_ppCanvas(NULL)
{
	m_bModified = FALSE;
	m_nIdx = -1;
	m_nNum = m_nTotalNum = 0;
	m_dbZoomX = m_dbZoomY = 1.0;
}

CTilCanvasMgr::~CTilCanvasMgr()
{
	clear_mem();
}

void CTilCanvasMgr::New()
{
	clear_mem();
	m_bModified = FALSE;
	m_nIdx = -1;
	m_nNum = 0;
	m_nTotalNum = ADD_SIZE;
	m_strProjName.Empty();

	m_PrjCap.strLineName.Empty();
	m_PrjCap.strPartIdx.Empty();
	m_PrjCap.strPartName.Empty();
	m_PrjCap.strTunName.Empty();

	m_ppCanvas = new CTilCanvas* [m_nTotalNum];
	for (int i = 0; i < m_nTotalNum; i ++)
		m_ppCanvas[i] = NULL;
}

BOOL CTilCanvasMgr::Open(const CString& strProjName)
{
	clear_mem();
	m_bModified = FALSE;
	m_nIdx = -1;
	m_strProjName = strProjName;

	// 得到打开文件的路径（不包含文件名）
	CString strTmp = strProjName;
	for (int i=strTmp.GetLength()-1; i>=0; i--)
	{
		if (strTmp.GetAt(i) == '\\')
		{
			strTmp.Delete(i+1, strTmp.GetLength()-(i+1));
			break;
		}
	}

	CFile file;
	if (!file.Open(strProjName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeRead))
		return FALSE;

	m_dbZoomX = 1.0;
	m_dbZoomY = 1.0;

	CArchive ar(&file, CArchive::load);

	//  设置进度条
	CProgress proDlg;
	proDlg.Create();
	proDlg.SetWindowText("打开工程文件...");
	proDlg.m_progress.SetRange(0, 100);
	proDlg.m_progress.SetStep(1);
	proDlg.m_progress.SetPos(0);
	proDlg.OnInitDialog();
	proDlg.ShowWindow(SW_SHOWNORMAL);

	TRY 
	{
		// 版本形式如Version=1.0.0.1
		CString strVersion;	// 版本号未作判别使用
		ar >> strVersion;
		
		// 画布数量及总量
		ar >> m_nNum;
		m_nTotalNum = (m_nNum/ADD_SIZE+1)*ADD_SIZE;
		
		m_ppCanvas = new CTilCanvas* [m_nTotalNum];
		for (int i = 0; i < m_nTotalNum; i ++)
			m_ppCanvas[i] = NULL;

		// 建立图形图像层
		CString strImage, strGraph;
		for (i = 0; i < m_nNum; i ++)
		{
			// 建立图形图像层
			ar >> strImage;
			ar >> strGraph;

			// 将相对路径变成绝对路径
			for (int j=strImage.GetLength()-1; j>=0; j--)
			{
				if (strImage.GetAt(j) == '\\')
				{
					strImage = strImage.Right(strImage.GetLength()-(j+1));
					strGraph = strGraph.Right(strGraph.GetLength()-(j+1));
					break;
				}
			}
			strImage = strTmp+strImage;
			strGraph = strTmp+strGraph;

			m_ppCanvas[i] = new CTilCanvas(strImage, strGraph);

			// 读取并设置画布属性（注意：画布属性存储在项目管理文件中）///////////////////
			FACECAPTION faceCaption;
			ar >> faceCaption.bImgCenter;
			ar >> faceCaption.ptImgCenter.x >> faceCaption.ptImgCenter.y;

			ar >> faceCaption.bGrndCenter;
			ar >> faceCaption.ptGrndCenter.x;
			ar >> faceCaption.ptGrndCenter.y;
			ar >> faceCaption.ptGrndCenter.z;

			ar >> faceCaption.bAngle;
			ar >> faceCaption.fAngle;

			ar >> m_PrjCap.strLineName;
			ar >> m_PrjCap.strTunName;
			ar >> m_PrjCap.strPartName;
			ar >> m_PrjCap.strPartIdx;

			m_ppCanvas[i]->SetCaption(faceCaption);
			m_ppCanvas[i]->SetZoomFactor(1.0, 1.0);
			m_ppCanvas[i]->SetModifiedFlag(FALSE);

			// 向CMainFrame类对象发送消息，显示导入切片成功的信息
			CWnd* pWnd = AfxGetMainWnd();
			pWnd->SendMessage(WM_USER+0x3004, i, NULL);

			proDlg.m_progress.SetPos(100 * i / m_nNum);
		}
	}
	CATCH (CFileException, e)
	{
#ifdef _DEBUG
		afxDump << "Error during reading " << e->m_cause << "\n";
#endif
		AfxMessageBox("Error during reading file project");
		file.Close();

		proDlg.m_progress.SetPos(100);
		proDlg.DestroyWindow();

		return FALSE;
	}
	END_CATCH

	ar.Close();
	file.Close();

	proDlg.m_progress.SetPos(100);
	proDlg.DestroyWindow();

	return TRUE;
}
/*
BOOL CTilCanvasMgr::Open(const CString& strProjName)
{
	clear_mem();
	m_bModified = FALSE;
	m_nIdx = -1;
	m_strProjName = strProjName;
	CStdioFile file;
	if (!file.Open(strProjName, CFile::modeRead|CFile::typeText))
		return FALSE;

	CString strVersion;	// 版本号未作判别使用
	CString strLine;
	TRY
	{
		// 版本形式如Version=1.0.0.1
		file.ReadString(strLine);
		if (strLine.Left(8) != "Version=")	return FALSE;
		strVersion = strLine.Right(strLine.GetLength()-8);

		// 文件数量形如SliceNum=24
		file.ReadString(strLine);
		if (strLine.Left(9) != "SliceNum=")	return FALSE;
		m_nNum = atoi(strLine.Right(strLine.GetLength()-9));
		m_nTotalNum = (m_nNum/ADD_SIZE+1)*ADD_SIZE;

		m_ppCanvas = new CTilCanvas* [m_nTotalNum];
		for (int i = 0; i < m_nTotalNum; i ++)
			m_ppCanvas[i] = NULL;

		CString strImage, strGraph;
		for (i = 0; i < m_nNum; i ++)
		{
			file.ReadString(strImage);
			file.ReadString(strGraph);
			m_ppCanvas[i] = new CTilCanvas(strImage, strGraph);
		}
	}
	CATCH(CFileException, e)
	{
		#ifdef _DEBUG
				afxDump << "Error during reading " << e->m_cause << "\n";
		#endif
		AfxMessageBox("Error during reading file project");
		file.Close();
		return FALSE;
	}
	END_CATCH

	file.Close();
	return TRUE;
}
*/

BOOL CTilCanvasMgr::Save()
{
	return SaveAs(m_strProjName);
}

BOOL CTilCanvasMgr::SaveAs(const CString& strProjName)
{
	CFile file;
	if (!file.Open(strProjName, CFile::modeCreate | CFile::modeWrite))
		return FALSE;
	
	CArchive ar(&file, CArchive::store);

	TRY 
	{
		// 版本形式如Version=1.0.0.1
		CString strVersion;	// 版本号未作判别使用
		strVersion = VERSION;
		ar << strVersion;
		
		// 画布数量及总量
		ar << m_nNum;
//		m_nTotalNum = (m_nNum/ADD_SIZE+1)*ADD_SIZE;
		
// 		m_ppCanvas = new CTilCanvas* [m_nTotalNum];
// 		for (int i = 0; i < m_nTotalNum; i ++)
// 			m_ppCanvas[i] = NULL;
		// 得到打开文件的路径（不包含文件名）
		CString strTmp = strProjName;
		for (int i=strTmp.GetLength()-1; i>=0; i--)
		{
			if (strTmp.GetAt(i) == '\\')
			{
				strTmp.Delete(i+1, strTmp.GetLength()-(i+1));
				break;
			}
		}

		// 建立图形图像层
		CString strImage, strGraph;
		for (i = 0; i < m_nNum; i ++)
		{
			// 保存图形图像层
			strImage = m_ppCanvas[i]->GetImage()->GetPathName();
			strGraph = m_ppCanvas[i]->GetGraph()->GetPathName();

			// 将绝对路径转变成相对路径后保存
			int nLength, nLength1;
			nLength = strTmp.GetLength();
			nLength1 = strImage.GetLength();
			strImage = strImage.Right(nLength1-nLength);
			strGraph = strGraph.Right(nLength1-nLength);

			ar << strImage;
			ar << strGraph;

			// 保存并设置画布属性（注意：画布属性存储在项目管理文件中）///////////////////
			FACECAPTION faceCaption;
			faceCaption = m_ppCanvas[i]->GetCaption();
			ar << faceCaption.bImgCenter;
			ar << faceCaption.ptImgCenter.x << faceCaption.ptImgCenter.y;

			ar << faceCaption.bGrndCenter;
			ar << faceCaption.ptGrndCenter.x;
			ar << faceCaption.ptGrndCenter.y;
			ar << faceCaption.ptGrndCenter.z;

			ar << faceCaption.bAngle;
			ar << faceCaption.fAngle;

			ar << m_PrjCap.strLineName;
			ar << m_PrjCap.strTunName;
			ar << m_PrjCap.strPartName;
			ar << m_PrjCap.strPartIdx;

			m_ppCanvas[i]->SetModifiedFlag(FALSE);
		}
	}
	CATCH (CFileException, e)
	{
		#ifdef _DEBUG
				afxDump << "Error during saving " << e->m_cause << "\n";
		#endif
		AfxMessageBox("Error during saving file project");
		file.Close();
		return FALSE;
	}
	END_CATCH

	ar.Close();
	file.Close();

	// 设置当前项目文件路径
	m_bModified = FALSE;
	m_strProjName = strProjName;

	return TRUE;
}

CString CTilCanvasMgr::GetPathName()
{
	return m_strProjName;
}

CTilCanvas* CTilCanvasMgr::GetCanvasByIdx(int nIdx)
{
	ASSERT(nIdx >= 0 && nIdx < m_nNum);
	return m_ppCanvas[nIdx];
}

CTilCanvas* CTilCanvasMgr::GetFirstCanvas()
{
	if (m_nNum == 0)
		return NULL;
	return m_ppCanvas[0];
}

CTilCanvas* CTilCanvasMgr::GetLastCanvas()
{
	if (m_nIdx < 0)
		return NULL;
	return m_ppCanvas[m_nNum-1];
}

CTilCanvas* CTilCanvasMgr::GetPrevCanvas()
{
	if (m_nIdx < 0)
		return NULL;
	else if (m_nIdx == 0)
		return m_ppCanvas[0];
	else
		return m_ppCanvas[m_nIdx-1];
}

CTilCanvas* CTilCanvasMgr::GetCurCanvas()
{
	if (m_nIdx < 0)
		return NULL;
	return m_ppCanvas[m_nIdx];
}

CTilCanvas* CTilCanvasMgr::GetNextCanvas()
{
	if (m_nIdx < 0)
		return NULL;
	else if (m_nIdx+1 >= m_nNum)
		return m_ppCanvas[m_nNum-1];
	else
		return m_ppCanvas[m_nIdx+1];
}

void CTilCanvasMgr::SetCurCanvasIdx(int nIdx)
{
	ASSERT(nIdx >= 0 && nIdx < m_nNum);
	m_nIdx = nIdx;
}

int	 CTilCanvasMgr::GetCurCanvasIdx()
{
	return m_nIdx;
}

void CTilCanvasMgr::SetZoomFactor(long double dbZoomX, long double dbZoomY)
{
	m_dbZoomX = dbZoomX;
	m_dbZoomY = dbZoomY;

	int i;
	for (i = 0; i < m_nNum; i ++)
	{
		m_ppCanvas[i]->SetZoomFactor(dbZoomX, dbZoomY);
	}	
}

void CTilCanvasMgr::GetZoomFactor(long double& dbZoomX, long double& dbZoomY)
{
	dbZoomX = m_dbZoomX;
	dbZoomY = m_dbZoomY;
}

int CTilCanvasMgr::Size()
{
	return m_nNum;
}

void CTilCanvasMgr::RemoveAll()
{
	New();
}

BOOL CTilCanvasMgr::RemoveAt(int nIdx)
{
	ASSERT(nIdx >= 0 && nIdx < m_nNum);

	int i, j;

	// 如果
	if ((m_nNum-1)%ADD_SIZE == 0 && (m_nNum-1)/ADD_SIZE >= 1)
	{
		int nTotalNum = m_nTotalNum-ADD_SIZE;
		CTilCanvas** ppCanvas = new CTilCanvas* [nTotalNum];
		for (i = 0; i < m_nTotalNum; i ++)
			ppCanvas[i] = NULL;

		for(i = 0; i < nIdx; i ++)
			ppCanvas[i] = m_ppCanvas[i];
		delete m_ppCanvas[nIdx];
		for (i = nIdx+1; i < m_nNum; i ++)
			ppCanvas[i-1] = m_ppCanvas[i];

		m_nNum --;
		m_nTotalNum = nTotalNum;
		
		delete [] m_ppCanvas;
		m_ppCanvas = ppCanvas;
	}
	else
	{
		delete m_ppCanvas[nIdx];
		for (i = nIdx+1; i < m_nNum; i ++)
			m_ppCanvas[i-1] = m_ppCanvas[i];
		m_ppCanvas[m_nNum-1] = NULL;

		m_nNum --;

		// 调整相邻掌子面岩层节理的链接
		// 设置前一张切片的后向无链接
		CTilGraph* pGraphMgr = NULL;
		if (nIdx-1>=0)
		{
			pGraphMgr = m_ppCanvas[nIdx-1]->GetGraph();
			for (i=0; i<pGraphMgr->Size(); i++)
			{
				CGraph* pGraph = pGraphMgr->GetGraphByIdx(i);
				for (j=0; j<pGraph->Size(); j++)
				{
					CUnit* pUnit = pGraph->GetUnitByIdx(j);
					if (pUnit->IsKindOf(RUNTIME_CLASS(CPolygon)))
						pUnit->SetNextLink(-1);
				}
			}
		}

		// 设置后一张切片的前向链接
		if (nIdx+1<m_nNum)
		{
			pGraphMgr = m_ppCanvas[nIdx+1]->GetGraph();
			for (i=0; i<pGraphMgr->Size(); i++)
			{
				CGraph* pGraph = pGraphMgr->GetGraphByIdx(i);
				for (j=0; j<pGraph->Size(); j++)
				{
					CUnit* pUnit = pGraph->GetUnitByIdx(j);
					if (pUnit->IsKindOf(RUNTIME_CLASS(CPolygon)))
						pUnit->SetPreLink(-1);
				}
			}
		}
	}

	// 如果当前切片被删除
	if (nIdx == m_nIdx)
	{
		if (nIdx > 0)
			m_nIdx = nIdx-1;
		else
		{
			if (m_nNum > 0)
				m_nIdx = 0;
			else
				m_nIdx = -1;
		}
	}
	else if (nIdx < m_nIdx)
		m_nIdx --;

	m_bModified = TRUE;

	return TRUE;
}

BOOL CTilCanvasMgr::PushFrontCanvas(CTilCanvas* pCanvas)
{
	pCanvas->SetZoomFactor(m_dbZoomX, m_dbZoomY);

	int i;

	if (m_nNum%ADD_SIZE == 0)
	{
		int nTotalNum = m_nTotalNum+ADD_SIZE;
		CTilCanvas** ppCanvas = new CTilCanvas* [nTotalNum];
		for (i = 0; i < m_nNum; i ++)
			ppCanvas[i] = NULL;

		ppCanvas[0] = pCanvas;
		for (i = 0; i < m_nNum; i ++)
			ppCanvas[i+1] = m_ppCanvas[i];

		delete [] m_ppCanvas;
		m_ppCanvas = ppCanvas;

		m_nNum ++;
		m_nTotalNum = nTotalNum;
	}
	else
	{
		for (i = m_nNum; i > 0; i ++)
			m_ppCanvas[i] = m_ppCanvas[i-1];
		m_ppCanvas[0] = pCanvas;

		m_nNum ++;
	}

	m_bModified = TRUE;
	m_nIdx ++;

	return TRUE;
}

BOOL CTilCanvasMgr::PushBackCanvas(CTilCanvas* pCanvas)
{
	pCanvas->SetZoomFactor(m_dbZoomX, m_dbZoomY);

	int i;

	if (m_nNum%ADD_SIZE == 0)
	{
		int nTotalNum = m_nTotalNum+ADD_SIZE;
		CTilCanvas** ppCanvas = new CTilCanvas* [nTotalNum];
		for (i = 0; i < m_nNum; i ++)
			ppCanvas[i] = NULL;

		for (i = 0; i < m_nNum; i ++)
			ppCanvas[i] = m_ppCanvas[i];
		ppCanvas[m_nNum] = pCanvas;

		delete [] m_ppCanvas;
		m_ppCanvas = ppCanvas;

		m_nNum ++;
		m_nTotalNum = nTotalNum;
	}
	else
	{
		m_ppCanvas[m_nNum] = pCanvas;

		m_nNum ++;
	}

	m_bModified = TRUE;

	return TRUE;
}

BOOL CTilCanvasMgr::InsertCanvas(CTilCanvas* pCanvas, int nIdx, BOOL bAfter)
{
	ASSERT(nIdx >= 0 && nIdx < m_nNum);
	pCanvas->SetZoomFactor(m_dbZoomX, m_dbZoomY);

	int i;

	if (m_nNum%ADD_SIZE == 0)
	{
		int nTotalNum = m_nTotalNum+ADD_SIZE;
		CTilCanvas** ppCanvas = new CTilCanvas* [nTotalNum];
		for (i = 0; i < m_nNum; i ++)
			ppCanvas[i] = NULL;

		if (bAfter)	// 插入后面
		{
			for (i = 0; i <= nIdx; i ++)
				ppCanvas[i] = m_ppCanvas[i];
			ppCanvas[nIdx+1] = pCanvas;
			for (i = nIdx+1; i < m_nNum; i ++)
				ppCanvas[i+1] = m_ppCanvas[i];

			if (m_nIdx > nIdx)
				m_nIdx ++;
		}
		else	// 插入前面
		{
			for (i = 0; i < nIdx; i ++)
				ppCanvas[i] = m_ppCanvas[i];
			ppCanvas[nIdx] = pCanvas;
			for (i = nIdx; i < m_nNum; i ++)
				ppCanvas[i+1] = m_ppCanvas[i];

			if (m_nIdx > nIdx)
				m_nIdx ++;
		}
	}

	m_bModified = TRUE;

	return TRUE;
}

BOOL CTilCanvasMgr::PushFrontCanvas(const CString& strPathImage, const CString& strPathGraph)
{
	CTilCanvas* pCanvas = new CTilCanvas(strPathImage, strPathGraph);
	return PushFrontCanvas(pCanvas);
}

BOOL CTilCanvasMgr::PushBackCanvas(const CString& strPathImage, const CString& strPathGraph)
{
	CTilCanvas* pCanvas = new CTilCanvas(strPathImage, strPathGraph);
	return PushBackCanvas(pCanvas);
}

BOOL CTilCanvasMgr::InsertCanvas(const CString& strPathImage, const CString& strPathGraph, int nIdx, BOOL bAfter)
{
	CTilCanvas* pCanvas = new CTilCanvas(strPathImage, strPathGraph);
	return InsertCanvas(pCanvas, nIdx, bAfter);
}

// 当nIdx1<nIdx2时,将序号为nIdx1的画布放到nIdx2的后面
// 当nIdx1>nIdx2时,将序号为nIdx1的画布放到nIdx2的前面
//BOOL CTilCanvasMgr::DragCanvasTo(int nIdx1, int nIdx2)
//{
//	if (nIdx1 < nIdx2)
//		return DragCanvasTo(nIdx1, nIdx2, TRUE);
//	else
//		return DragCanvasTo(nIdx1, nIdx2, FALSE);
//}

// 将序号为nIdx1的画布放到nIdx2的前面或后面
BOOL CTilCanvasMgr::DragCanvasTo(int nIdx1, int nIdx2, BOOL bAfter)
{
	ASSERT(nIdx1 >= 0 && nIdx1 < m_nNum);
	ASSERT(nIdx2 >= 0 && nIdx2 < m_nNum);
	ASSERT(nIdx1 != nIdx2);

	int i;
	CTilCanvas* pTmp = m_ppCanvas[nIdx1];

	// 如果将第nIdx1张切片拖到第nIdx2张切片后面
	if (bAfter)
	{
		if (nIdx1 < nIdx2)
		{
			for (i = nIdx1+1; i <= nIdx2; i ++)
				m_ppCanvas[i-1] = m_ppCanvas[i];
			m_ppCanvas[i-1] = pTmp;
			m_nIdx = nIdx2;
		}
		else
		{
			for (i = nIdx1; i > nIdx2+1; i --)
				m_ppCanvas[i] = m_ppCanvas[i-1];
			m_ppCanvas[i] = pTmp;
			m_nIdx = nIdx2+1;
		}
	}
	else
	{
		if (nIdx1 < nIdx2)
		{
			for (i = nIdx1+1; i < nIdx2; i ++)
				m_ppCanvas[i-1] = m_ppCanvas[i];
			m_ppCanvas[i-1] = pTmp;
			m_nIdx = nIdx2-1;
		}
		else
		{
			for (i = nIdx1; i > nIdx2; i --)
				m_ppCanvas[i] = m_ppCanvas[i-1];
			m_ppCanvas[i] = pTmp;
			m_nIdx = nIdx2;
		}
	}

	// 顺序交换后，链接目标也将重新设置

	// 将拖动的序号为nIdx1的画布设为当前画布
	m_bModified = TRUE;

	return TRUE;
}

void CTilCanvasMgr::AutoArrayCanvas(BOOL bOrder)
{
	int i, j, nStdNum, nUnStdNum;

	int* pIdx = new int [m_nNum];
	long* pMile = new long [m_nNum];

	CTilCanvas** ppCanvas = new CTilCanvas* [m_nTotalNum];
	for (i = 0; i < m_nTotalNum; i ++)
		ppCanvas[i] = NULL;

	CString strPathName;
	nStdNum = nUnStdNum = 0;
	for (i = 0; i < m_nNum; i ++)
	{
		strPathName = m_ppCanvas[i]->GetImage()->GetPathName();
		// 将非标准名称的切片直接放到画布列表前面
		if (!is_std_mileage(strPathName))
		{
			ppCanvas[nUnStdNum] = m_ppCanvas[i];
			nUnStdNum ++;
		}
		else
		{
			pMile[nStdNum] = get_std_mileage(strPathName);
			pIdx[nStdNum] = i;
			nStdNum ++;
		}
	}

	// 对标准里程文件画布切片进行排序
	int nFlag = bOrder ? 1 : -1;
	for (i = 0; i < nStdNum-1; i ++)
	{
		for (j = i+1; j < nStdNum; j ++)
		{
			if (nFlag*pMile[i] > nFlag*pMile[j])
			{
				int tmp = pIdx[j];
				pIdx[j] = pIdx[i];
				pIdx[i] = tmp;

				long lTmp = pMile[j];
				pMile[j] = pMile[i];
				pMile[i] = lTmp;
			}
		}
	}

	for (i = 0; i < nStdNum; i ++)
	{
		ppCanvas[nUnStdNum] = m_ppCanvas[pIdx[i]];
	 	nUnStdNum ++;
	}

	// 判断是否排序后与排序前有所不同
	for (i = 0; i < m_nNum; i ++)
	{
		if (ppCanvas[i] != m_ppCanvas[i])
			break;
	}
	if (i < m_nNum)
		m_bModified = TRUE;

	delete [] m_ppCanvas;
	m_ppCanvas = ppCanvas;

	delete [] pIdx;
	delete [] pMile;
}

void CTilCanvasMgr::clear_mem()
{
	if (m_ppCanvas)
	{
		for (int i = 0; i < m_nNum; i ++)
			delete m_ppCanvas[i];
		delete [] m_ppCanvas;
		m_ppCanvas = NULL;
	}
}

void CTilCanvasMgr::SetModifiedFlag(BOOL bFlag)
{
	m_bModified = bFlag;
}

BOOL CTilCanvasMgr::IsModified()
{
	// 如果项目文件或画布中掌子面信息被修改，则认为项目被修改
	for (int i=0; i<m_nNum; i++)
	{
		m_bModified = m_bModified || m_ppCanvas[i]->IsModified();
	}

	return m_bModified;
}

void CTilCanvasMgr::SetPrjInfo(PRJCAPTION& prjCap)
{
	m_PrjCap = prjCap;
}

PRJCAPTION CTilCanvasMgr::GetPrjInfo()
{
	return m_PrjCap;
}
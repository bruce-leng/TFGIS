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

	// �õ����ļ���·�����������ļ�����
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

	//  ���ý�����
	CProgress proDlg;
	proDlg.Create();
	proDlg.SetWindowText("�򿪹����ļ�...");
	proDlg.m_progress.SetRange(0, 100);
	proDlg.m_progress.SetStep(1);
	proDlg.m_progress.SetPos(0);
	proDlg.OnInitDialog();
	proDlg.ShowWindow(SW_SHOWNORMAL);

	TRY 
	{
		// �汾��ʽ��Version=1.0.0.1
		CString strVersion;	// �汾��δ���б�ʹ��
		ar >> strVersion;
		
		// ��������������
		ar >> m_nNum;
		m_nTotalNum = (m_nNum/ADD_SIZE+1)*ADD_SIZE;
		
		m_ppCanvas = new CTilCanvas* [m_nTotalNum];
		for (int i = 0; i < m_nTotalNum; i ++)
			m_ppCanvas[i] = NULL;

		// ����ͼ��ͼ���
		CString strImage, strGraph;
		for (i = 0; i < m_nNum; i ++)
		{
			// ����ͼ��ͼ���
			ar >> strImage;
			ar >> strGraph;

			// �����·����ɾ���·��
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

			// ��ȡ�����û������ԣ�ע�⣺�������Դ洢����Ŀ�����ļ��У�///////////////////
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

			// ��CMainFrame���������Ϣ����ʾ������Ƭ�ɹ�����Ϣ
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

	CString strVersion;	// �汾��δ���б�ʹ��
	CString strLine;
	TRY
	{
		// �汾��ʽ��Version=1.0.0.1
		file.ReadString(strLine);
		if (strLine.Left(8) != "Version=")	return FALSE;
		strVersion = strLine.Right(strLine.GetLength()-8);

		// �ļ���������SliceNum=24
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
		// �汾��ʽ��Version=1.0.0.1
		CString strVersion;	// �汾��δ���б�ʹ��
		strVersion = VERSION;
		ar << strVersion;
		
		// ��������������
		ar << m_nNum;
//		m_nTotalNum = (m_nNum/ADD_SIZE+1)*ADD_SIZE;
		
// 		m_ppCanvas = new CTilCanvas* [m_nTotalNum];
// 		for (int i = 0; i < m_nTotalNum; i ++)
// 			m_ppCanvas[i] = NULL;
		// �õ����ļ���·�����������ļ�����
		CString strTmp = strProjName;
		for (int i=strTmp.GetLength()-1; i>=0; i--)
		{
			if (strTmp.GetAt(i) == '\\')
			{
				strTmp.Delete(i+1, strTmp.GetLength()-(i+1));
				break;
			}
		}

		// ����ͼ��ͼ���
		CString strImage, strGraph;
		for (i = 0; i < m_nNum; i ++)
		{
			// ����ͼ��ͼ���
			strImage = m_ppCanvas[i]->GetImage()->GetPathName();
			strGraph = m_ppCanvas[i]->GetGraph()->GetPathName();

			// ������·��ת������·���󱣴�
			int nLength, nLength1;
			nLength = strTmp.GetLength();
			nLength1 = strImage.GetLength();
			strImage = strImage.Right(nLength1-nLength);
			strGraph = strGraph.Right(nLength1-nLength);

			ar << strImage;
			ar << strGraph;

			// ���沢���û������ԣ�ע�⣺�������Դ洢����Ŀ�����ļ��У�///////////////////
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

	// ���õ�ǰ��Ŀ�ļ�·��
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

	// ���
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

		// ���������������Ҳ���������
		// ����ǰһ����Ƭ�ĺ���������
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

		// ���ú�һ����Ƭ��ǰ������
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

	// �����ǰ��Ƭ��ɾ��
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

		if (bAfter)	// �������
		{
			for (i = 0; i <= nIdx; i ++)
				ppCanvas[i] = m_ppCanvas[i];
			ppCanvas[nIdx+1] = pCanvas;
			for (i = nIdx+1; i < m_nNum; i ++)
				ppCanvas[i+1] = m_ppCanvas[i];

			if (m_nIdx > nIdx)
				m_nIdx ++;
		}
		else	// ����ǰ��
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

// ��nIdx1<nIdx2ʱ,�����ΪnIdx1�Ļ����ŵ�nIdx2�ĺ���
// ��nIdx1>nIdx2ʱ,�����ΪnIdx1�Ļ����ŵ�nIdx2��ǰ��
//BOOL CTilCanvasMgr::DragCanvasTo(int nIdx1, int nIdx2)
//{
//	if (nIdx1 < nIdx2)
//		return DragCanvasTo(nIdx1, nIdx2, TRUE);
//	else
//		return DragCanvasTo(nIdx1, nIdx2, FALSE);
//}

// �����ΪnIdx1�Ļ����ŵ�nIdx2��ǰ������
BOOL CTilCanvasMgr::DragCanvasTo(int nIdx1, int nIdx2, BOOL bAfter)
{
	ASSERT(nIdx1 >= 0 && nIdx1 < m_nNum);
	ASSERT(nIdx2 >= 0 && nIdx2 < m_nNum);
	ASSERT(nIdx1 != nIdx2);

	int i;
	CTilCanvas* pTmp = m_ppCanvas[nIdx1];

	// �������nIdx1����Ƭ�ϵ���nIdx2����Ƭ����
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

	// ˳�򽻻�������Ŀ��Ҳ����������

	// ���϶������ΪnIdx1�Ļ�����Ϊ��ǰ����
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
		// ���Ǳ�׼���Ƶ���Ƭֱ�ӷŵ������б�ǰ��
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

	// �Ա�׼����ļ�������Ƭ��������
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

	// �ж��Ƿ������������ǰ������ͬ
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
	// �����Ŀ�ļ��򻭲�����������Ϣ���޸ģ�����Ϊ��Ŀ���޸�
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
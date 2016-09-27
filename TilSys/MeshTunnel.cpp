// MeshTunnel.cpp: implementation of the CMeshTunnel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tilsys.h"
#include "MeshTunnel.h"
#include "TilComFunction.h"
#include "MathComput.h"
#include <float.h>
#include "SpaceRelation.h"
#include "triangle.h"
#include "Graph\Polygon.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define MESH_SCALE 0.000641
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMeshTunnel::CMeshTunnel()
{
	m_lStartDK = 0;
	m_lEndDK   = 0;

	CTilCanvasMgr* pMgr = GetCanvasMgr();
	int nSize = pMgr->Size();
	if (nSize > 0)
	{
		CString strStartPath = pMgr->GetCanvasByIdx(0)->GetGraph()->GetPathName();
		CString strEndPath   = pMgr->GetCanvasByIdx(nSize-1)->GetGraph()->GetPathName();
		m_lStartDK = get_std_mileage(strStartPath);
		m_lEndDK   = get_std_mileage(strEndPath);
	}
}

CMeshTunnel::~CMeshTunnel()
{

}

void CMeshTunnel::SetRangeDK(const long& lStartDK, const long& lEndDK)
{
	m_lStartDK = lStartDK;
	m_lEndDK   = lEndDK;
}

void CMeshTunnel::GetRangeDK(long& lStartDK, long& lEndDK)
{
	lStartDK = m_lStartDK;
	lEndDK	 = m_lEndDK;
}

// �ж��Ƿ������趨����̷�Χ�ڵ��������ͼ�ζ�������
BOOL CMeshTunnel::IsTunArcParaInputed()
{
	CString strPathName;
	CTilCanvasMgr* pMgr = GetCanvasMgr();

	// �����Ϊ0������Ϊ�жϵ������е��������ͼ���Ƿ�������
	for (int i=0; i<pMgr->Size(); i++)
	{
		CTilGraph* pGraphMgr = pMgr->GetCanvasByIdx(i)->GetGraph();
		strPathName = pGraphMgr->GetPathName();
		long lDK = get_std_mileage(strPathName);

		// �������Ĳ���ʽ��������lDK��m_lStartDK��m_lEndDK֮��
		if ((m_lStartDK-lDK)*(m_lEndDK-lDK)<=0)
		{
			CGraph* pGraph = pGraphMgr->GetGraphByIdx(GRAPH_LAYER_TUNARC);
			if (pGraph->Size()==0)
				return FALSE;
		}
	}

	return TRUE;
}

// �������������Ƭ
void CMeshTunnel::CreateTunArc(CSceneGraph3d& scene)
{
	CTilCanvasMgr* pMgr = GetCanvasMgr();
	int nCanvasNum = pMgr->Size();
	if (nCanvasNum <= 0)
		return;

	// �������粢�����������ӵ�����
	CMesh3d* pMesh = new CMesh3d;
	pMesh->SetMeshType(SIDE_FACE);
	pMesh->SetTextureIndex(0);
	scene.Add(pMesh);

	// ���������ж������������Ƭ����
	pMesh->SetNbVertex(0);
	pMesh->SetNbFace(0);

	// ȡ��������ĵ��ͼ��������Ϊ��ά����ԭ��
	int i,j;
	POINT_3F pt1[40], pt2[40];
	int nNum1, nNum2;

	int nFaceIdx = 0;
	int nVerIdx = 0;

	// ������ͼ����ƫ��
	CTilCanvas* pCanvas = pMgr->GetCanvasByIdx(0);
	int nWidth = pCanvas->GetWidth();
	CGraph* pTunArc = pCanvas->GetGraph()->GetGraphByIdx(GRAPH_LAYER_TUNARC);

//	pMesh->SetLockOffset(-ptImgCenter.x*MESH_SCALE, -ptImgCenter.y*MESH_SCALE);
	nNum1 = pTunArc->GetUnitByIdx(0)->GetPointNum(); 
	for (j=0; j<nNum1; j++)
	{
		// ȡ�õ�0��Ŀ�꼴�������������Ŀ���ϵĵ�
		POINT2DF pt = pTunArc->GetUnitByIdx(0)->GetPoint(j);

		// ���������ĵ�Ϊ���ĵĸ�������
		pt1[j].x = pt.x/nWidth;
		pt1[j].y = pt.y/nWidth;
		pt1[j].z = 0.0;
	}

	// ��������������ά��ͼ�е�λ��
	float fDist = 0.0;
	long lK1, lK2;
	lK1 = get_std_mileage(pMgr->GetCanvasByIdx(0)->GetGraph()->GetPathName());

	for (i=1; i<nCanvasNum; i++)
	{
		pCanvas = pMgr->GetCanvasByIdx(i);
		pTunArc = pCanvas->GetGraph()->GetGraphByIdx(GRAPH_LAYER_TUNARC);
		if (pTunArc->Size()==0)
			continue;

		nNum2 = pTunArc->GetUnitByIdx(0)->GetPointNum();
		lK2 = get_std_mileage(pMgr->GetCanvasByIdx(i)->GetGraph()->GetPathName());
		fDist += abs(lK2-lK1)*100*MESH_SCALE;
		lK1 = lK2;

		//��������,ԭ��λ��ê���λ��
		for (j=0; j<nNum2; j++)
		{
			POINT2DF pt = pTunArc->GetUnitByIdx(0)->GetPoint(j);
			pt2[j].x = pt.x/nWidth;
			pt2[j].y = pt.y/nWidth;
			pt2[j].z = fDist;
		}

		ConstructRockMass(scene,pMesh,pt1,nNum1,pt2,nNum2,i,i);
//		ConstructSidePlaneA(scene, pMesh, ARC_FACE, pt1, nNum1, pt2, nNum2, i, i);

		for (j=0; j<nNum2; j++)
		{
			pt1[j] = pt2[j];
		}
		nNum1 = nNum2;
	}

	pMesh->Rebuild();
}

// �������������(09.06.07)
void CMeshTunnel::CreateTunFace(CSceneGraph3d& scene)
{
	/*
	CTilCanvasMgr* pMgr = GetCanvasMgr();

	// �������粢�����������ӵ�����
	CMesh3d* pMesh = new CMesh3d;
	pMesh->SetMeshType(MESH_FACE);
	pMesh->SetTextureIndex(7);
	scene.Add(pMesh);

	// ���������ж������������Ƭ����
	pMesh->SetNbVertex(0);
	pMesh->SetNbFace(0);

	// ȡ��������ĵ��ͼ��������Ϊ��ά����ԭ��
	int i,j;
	POINT_3F pt1[40], pt2[40];
	int nNum1, nNum2;

	int nFaceIdx = 0;
	int nVerIdx = 0;

	// ������ͼ����ƫ��
	CTilCanvas* pCanvas = pMgr->GetCanvasByIdx(0);
	int nWidth = pCanvas->GetWidth();
	CGraph* pTunArc = pCanvas->GetGraph()->GetGraphByIdx(GRAPH_LAYER_TUNARC);

//	pMesh->SetLockOffset(-ptImgCenter.x*MESH_SCALE, -ptImgCenter.y*MESH_SCALE);
	nNum1 = pTunArc->GetUnitByIdx(0)->GetPointNum(); 
	for (j=0; j<nNum1; j++)
	{
		// ȡ�õ�0��Ŀ�꼴�������������Ŀ���ϵĵ�
		POINT2DF pt = pTunArc->GetUnitByIdx(0)->GetPoint(j);

		// ���������ĵ�Ϊ���ĵĸ�������
		pt1[j].x = pt.x/nWidth;
		pt1[j].y = pt.y/nWidth;
		pt1[j].z = 0.0;
	}

	// ��������������ά��ͼ�е�λ��
	float fDist = 0.0;
	long lK1, lK2;
	lK1 = get_std_mileage(pMgr->GetCanvasByIdx(0)->GetGraph()->GetPathName());

	for (i=1; i<nCanvasNum; i++)
	{
		pCanvas = pMgr->GetCanvasByIdx(i);
		pTunArc = pCanvas->GetGraph()->GetGraphByIdx(GRAPH_LAYER_TUNARC);
		if (pTunArc->Size()==0)
			continue;

		nNum2 = pTunArc->GetUnitByIdx(0)->GetPointNum();
		lK2 = get_std_mileage(pMgr->GetCanvasByIdx(i)->GetGraph()->GetPathName());
		fDist += abs(lK2-lK1)*100*MESH_SCALE;
		lK1 = lK2;

		//��������,ԭ��λ��ê���λ��
		for (j=0; j<nNum2; j++)
		{
			POINT2DF pt = pTunArc->GetUnitByIdx(0)->GetPoint(j);
			pt2[j].x = pt.x/nWidth;
			pt2[j].y = pt.y/nWidth;
			pt2[j].z = fDist;
		}

//		ConstructRockMass(scene,pMesh,pt1,nNum1,pt2,nNum2,i,i);
		ConstructSidePlaneA(scene, pMesh, ARC_FACE, pt1, nNum1, pt2, nNum2, i, i);

		for (j=0; j<nNum2; j++)
		{
			pt1[j] = pt2[j];
		}
		nNum1 = nNum2;
	}

	pMesh->Rebuild();
	*/
}

// ��������ṹ��
void CMeshTunnel::CreateStructures(CSceneGraph3d& scene)
{
	int nCanvasNum = GetCanvasMgr()->Size();

	// �ͷŵ���ǰ�����ж���
	scene.FreeVertext();

	POINT_3F ptCenter;
	POINT2DF ptImgCenter;
	POINT_3F pt1[400], pt2[400];
	int nNum1, nNum2, i, k, m;
	int nLink, m1, k1;
	long lK1, lK2;

	CTilCanvasMgr* pMgr = GetCanvasMgr();
	CTilCanvas* pCanvas1 = pMgr->GetCanvasByIdx(0);
	int nWidth = pCanvas1->GetWidth();
	CGraph* pGraph1 = pCanvas1->GetGraph()->GetGraphByIdx(GRAPH_LAYER_TUNARC);
	lK1 = get_std_mileage(pCanvas1->GetGraph()->GetPathName());

	float fDist1, fDist2;
	for (int nIdx=1; nIdx<3; nIdx++)
	for (m=0; m<nCanvasNum; m++)
	{
		CTilCanvas* pCanvas1 = GetCanvasMgr()->GetCanvasByIdx(m);
		// ���Ϊ��,����100���������,�������MESH_SCALE����
		lK1 = get_std_mileage(pCanvas1->GetGraph()->GetPathName());
		fDist1 = abs(lK1-m_lStartDK)*100*MESH_SCALE;

		// ȡ���Ҳ�Ŀ������Ҳ����Ŀ
		CGraph* pGraph1 = pCanvas1->GetGraph()->GetGraphByIdx(nIdx);
		int nRockNum = pGraph1->Size();

		for (k=0; k<nRockNum; k++)
		{
			CUnit* pUnit1 = pGraph1->GetUnitByIdx(k);

			// ��ǰ������Ϊ-1�����Ŀ���Ѿ���ǰ�汻������ˣ������ﲻ��Ϊ-1
//			int nPreLnk = pUnit1->GetPreLink();
//			int nNxtLnk = pUnit1->GetNextLink();
			if (pUnit1->GetPreLink()!=-1 || pUnit1->GetNextLink()==-1)
				continue;

			// ����������󲢽�����ӵ�����
			CMesh3d* pMesh = new CMesh3d;
			scene.Add(pMesh);
			pMesh->SetTextureIndex(m);

			// ���������ж������������Ƭ����
			pMesh->SetMeshID(m, k);
			pMesh->SetNbVertex(0);
			pMesh->SetNbFace(0);

			m1 = m;		//��Ƭ��
			k1 = k;		//��Ƭ�еĶ����

			// Ϊǰ��������Ӧ��Ŀ�괴������
			nLink = pUnit1->GetNextLink();
			if (nLink != -1)
			{
				// ������ĵ�
				nNum1 = pUnit1->GetPointNum();
				for (i=0; i<nNum1; i++)
				{
					POINT2DF pt = pUnit1->GetPoint(i);
					pt1[i].x = pt.x/nWidth;
					pt1[i].y = pt.y/nWidth;
					pt1[i].z = fDist1;
				}
			}

			while (nLink != -1)
			{
				// �õ���һ���������ӵ�Ŀ��Ķ�����
				CTilCanvas* pCanvas2 = pMgr->GetCanvasByIdx(m1+1);
				CGraph* pGraph2 = pCanvas2->GetGraph()->GetGraphByIdx(nIdx);
				CUnit* pUnit2 = pGraph2->GetUnitByIdx(nLink);

				// �����ļ���ȡ�����������
				lK2 = get_std_mileage(pCanvas2->GetGraph()->GetPathName());
				fDist2 = abs(lK2-m_lStartDK)*100*MESH_SCALE;

				nNum2 = pUnit2->GetPointNum();
				for (i=0; i<nNum2; i++)
				{
					POINT2DF pt = pUnit2->GetPoint(i);
					pt2[i].x = pt.x/nWidth;
					pt2[i].y = pt.y/nWidth;
					pt2[i].z = fDist2;
				}

				if (!pUnit1->IsPolyClose())
					ConstructSidePlaneA(scene, pMesh, STRUCT_FACE, pt1, nNum1, pt2, nNum2, m1, m1);
				else
				{
					pMesh->SetMeshType(TOP_FACE);
					pMesh->SetTextureIndex(8);
//					ConstructRockMass(scene,pMesh,pt1,nNum1,pt2,nNum2,m1,m1);
//					ConstructSidePlaneA(scene, pMesh, STRUCT_FACE, pt1, nNum1, pt2, nNum2, m1+1, m1+1);
					ConstructFace(scene, MESH_FACE, pt1, nNum1, m1, m1);
					ConstructFace(scene, MESH_FACE, pt2, nNum2, m1+1, m1+1);
				}

				nNum1 = nNum2;
				for (i=0; i<nNum2; i++)
					pt1[i] = pt2[i];

				m1 = m1+1;
				k1 = nLink;
				nLink = pUnit2->GetNextLink();
				pUnit1 = pUnit2;
				pCanvas1 = pCanvas2;
				lK1 = lK2;
			}
		}	
	}
}

// ��ȡ���������ͼ��
// lDK: ���������
BOOL CMeshTunnel::NormalSection(long lDK, CTilGraph& graphMgr)
{
	CTilCanvasMgr* pMgr = GetCanvasMgr();

	// �������Ĳ���ʽ��������lDK����m_lStartDK��m_lEndDK֮��
	if ((m_lStartDK-lDK)*(m_lEndDK-lDK)>0)
		return FALSE;

	int i,j,k,m,n;
	float fDist, fDist1, fDist2;
	POINT_3F pt1[40], pt2[40];
	int nNum1, nNum2;
	long lDK1, lDK2;
	int nNextLnkIdx;
	CTilGraph *pGraphMgr1, *pGraphMgr2;
	pGraphMgr1 = pMgr->GetCanvasByIdx(0)->GetGraph();
	CString strPathName1 = pGraphMgr1->GetPathName();

	lDK1 = get_std_mileage(strPathName1);
	for (i=1; i<pMgr->Size(); i++)
	{
		pGraphMgr2 = pMgr->GetCanvasByIdx(i)->GetGraph();
		CString strPathName2 = pGraphMgr2->GetPathName();
		lDK2 = get_std_mileage(strPathName2);

		// ���lDK��lDK1��lDK2֮��
		if ((lDK1-lDK)*(lDK2-lDK)<=0)
		{
			// ȡ��i-1��i�������������
			// ȡ���Ҳ�Ŀ������Ҳ����Ŀ
			int nRockNum = pGraphMgr1->Size();
			for (j=0; j<3; j++)
			{
				// ���������ߴ缰��صĵ�1�����ݼ���
				if (j==0)
				{
					// �����1������ 
					CUnit* pSrcTunArc = pGraphMgr1->GetGraphByIdx(0)->GetUnitByIdx(0);
					CGraph* pGraph = new CGraph;
					CUnit* pUnit = new CPolygon;
					pUnit->m_pPointList->Append(*pSrcTunArc->m_pPointList);
					pGraph->PushBackUnit(pUnit);
					graphMgr.PushBackGraph(pGraph);

					// �����������ߴ�
					CTilTunArc* pTunArc = pGraphMgr1->GetTunArc();
					POINT2DF ptImgCenter1 = pTunArc->GetImgCenter();
					graphMgr.GetTunArc()->SetImgCenter(ptImgCenter1);
					vector<PTUNARC_UNIT>* pTunSrcUnits = pTunArc->GetTunArcUnits();
					vector<PTUNARC_UNIT>* pTunDstUnits = graphMgr.GetTunArc()->GetTunArcUnits();
					for (k=0; k<pTunSrcUnits->size(); k++)
					{
						PTUNARC_UNIT pUnits = new TUNARC_UNIT;
						*pUnits = *((*pTunSrcUnits)[k]);
						pTunDstUnits->insert(pTunDstUnits->begin(), pUnits);
					}
				}

				CGraph* pGraph = new CGraph;
				CGraph* pGraph1 = pGraphMgr1->GetGraphByIdx(j);
				int nUnitNum1 =pGraph1->Size();
				for (k=0; k<nUnitNum1; k++)
				{
					CUnit* pUnit1 = pGraph1->GetUnitByIdx(k);
					nNextLnkIdx = pUnit1->GetNextLink();
					if (pUnit1->GetNextLink() < 0)
						continue;

					// ȡ��ԭ��
					fDist1 = (lDK1-m_lStartDK)*100;

					// ȡ��Ŀ��1�ĵ���,��Ϊ��һ������Ŀ����ڣ���ֱ����Ϊ��Ŀ��Ϊ�����
					nNum1 = pUnit1->GetPointNum();
					for (m=0; m<nNum1; m++)
					{
						POINT2DF pt = pUnit1->GetPoint(m);
						pt1[m].x = pt.x;
						pt1[m].y = pt.y;
						pt1[m].z = fDist1;
					}

					// ȡ����һ����Ŀ��ĵ�
					fDist2 = (lDK2-m_lStartDK)*100;
					CUnit* pUnit2 = pGraphMgr2->GetGraphByIdx(j)->GetUnitByIdx(nNextLnkIdx);
					// ȡ��Ŀ��1�ĵ���,��Ϊ��һ������Ŀ����ڣ���ֱ����Ϊ��Ŀ��Ϊ�����
					nNum2 = pUnit2->GetPointNum();
					for (m=0; m<nNum2; m++)
					{
						POINT2DF pt = pUnit2->GetPoint(m);
						pt2[m].x = pt.x;
						pt2[m].y = pt.y;
						pt2[m].z = fDist2;
					}

					// ���Ӷ�Ӧ��
					PPPOINT_3F ppLink21;
					int *pNum21;
					if (pUnit1->IsPolyClose())
						link_faces(pt1, nNum1, pt2, nNum2, &ppLink21, &pNum21);
					else
						link_line_segments(pt1, nNum1, pt2, nNum2, &ppLink21, &pNum21);

					// ȡ�ú�����ͼ��
					PPOINT_3F pPredFace;
					int nNum;
					fDist = (lDK-m_lStartDK)*100;
					float fFace[4] = {0, 0, 1, -fDist};
					predict_inner_face(ppLink21, pNum21, nNum2, fFace, &pPredFace, nNum);

					// ��Ԥ��ĺ��������ͼ����
					CUnit* pUnit = new CPolygon;
					pUnit->SetPolyClose(pUnit1->IsPolyClose());
					for (m=0; m<nNum; m++)
					{
						POINT2DF pt;
						pt.x = pPredFace[m].x;
						pt.y = pPredFace[m].y;
						pUnit->m_pPointList->Add(pt);
					}
					pGraph->PushBackUnit(pUnit);

					for (n=0; n<nNum2; n++)
						delete ppLink21[n];
					delete [] ppLink21;
					delete [] pNum21;
					delete [] pPredFace;
				}
				graphMgr.PushBackGraph(pGraph);
			}

			return TRUE;
		}

		lDK1 = lDK2;
		pGraphMgr1 = pGraphMgr2;
	}

	// �������lDK������֪����̷�Χ��,������ⲿԤ��,Ĭ�������һ����̺���
// 	// ������2�Ż���
// 	pGraphMgr1 = pMgr->GetCanvasByIdx(pMgr->Size()-2)->GetGraph();
// 	strPathName1 = pGraphMgr1->GetPathName();
// 
// 	// �ҵ��ļ�·��
// 	CString strPath;
// 	for (i=strPathName1.GetLength()-1; i>=0; i--)
// 	{
// 		if (strPathName1.GetAt(i)=='\'))
// 			strPath = strPathName1.Left(i+1);
// 	}
// 
// 	// ����һ���µĻ������µ�ͼ��ͼ��
// 	CTilImage* pImageMgr = NULL;
// 	CTilGraph* pGraphMgr = NULL;
// 	CString strPathImage, strPathGraph;
// 	
// 	// ����ͼ���ļ�����
// 	strPathImage = strPath + lDK + '.' + EXT_IMAGE;
// 	CImage image;
// 	image.
// 	::CopyFile(str, strPathImage, TRUE);
// 	//				pImageMgr = new CTilImage(strPathImage);
// 	
// 	// ����ͼ���ļ�����
// 	strPathGraph = get_path_graph(str);
// 	HANDLE hHandle = ::CreateFile(strPathGraph, 0, 0, NULL, OPEN_ALWAYS,
// 		FILE_ATTRIBUTE_NORMAL, NULL);
// 	::CloseHandle(hHandle);
// 	//				pGraphMgr = new CTilGraph(strPathGraph);
// 	if (pMgr->PushBackCanvas(new CTilCanvas(strPathImage, strPathGraph)))
// 		LogInfo("װ����Ƭ�ļ� "+str+" �ɹ�!");
// 
// 	CTilCanvas* pCanvas = new CTilCanvas();
// 	pMgr->PushBackCanvas(pCanvas);
// 	CTilGraph* pGraphLayer = new CTilGraph();
// 	pGraphLayer->


	return FALSE;
}

// ��ȡ������ͼ��
// lStartDK: ��������ʼ���
// lEndDK: ��������ʼ���
BOOL CMeshTunnel::Profile(long& lStartDK, long& lEndDK, CTilGraph& graphMgr)
{
	if (lStartDK==0 && lEndDK==0)
	{
		lStartDK = m_lStartDK;
		lEndDK = m_lEndDK;
	}

	// �涨������ʼ��������̱����ڸ�����̷�Χ��
	ASSERT((lStartDK-m_lStartDK)*(lStartDK-m_lEndDK)<=0);
	ASSERT((lEndDK-m_lStartDK)*(lEndDK-m_lEndDK)<=0);

	CTilCanvasMgr* pMgr = GetCanvasMgr();

	// �����붼Ϊ0����Ĭ��Ϊ������̷�Χ�ڵ�������
	if (lStartDK==lEndDK && lStartDK==0)
	{
		lStartDK = m_lStartDK;
		lEndDK = m_lEndDK;
	}

	// ������ʾ����йص�ǰ��������Ƭ�����
	int i,k,m,n,nIdx1, nIdx2;
	long lK1, lK2;
	CTilCanvas* pCanvas1 = pMgr->GetCanvasByIdx(0);
	CGraph* pGraph1 = pCanvas1->GetGraph()->GetGraphByIdx(GRAPH_LAYER_TUNARC);
	lK1 = get_std_mileage(pCanvas1->GetGraph()->GetPathName());
	for (i=1; i<pMgr->Size(); i++)
	{
		CTilCanvas* pCanvas2 = pMgr->GetCanvasByIdx(i);
		CGraph* pGraph2 = pCanvas2->GetGraph()->GetGraphByIdx(GRAPH_LAYER_TUNARC);
		lK2 = get_std_mileage(pCanvas2->GetGraph()->GetPathName());
		if ((lK1-lStartDK)*(lK2-lStartDK)<=0)
			nIdx1 = i-1;
		else if ((lK1-lEndDK)*(lK2-lEndDK)<=0)
			nIdx2 = i;

		lK1 = lK2;
	}

	// ȡ�ø������������ߴ�õ���������ͼ��
	CGraph* pGraph=new CGraph;
	CUnit* pUnit1 = new CPolygon;
	CUnit* pUnit2 = new CPolygon;
	for (i=nIdx1; i<=nIdx2; i++)
	{
		CTilGraph* pGraphMgr = pMgr->GetCanvasByIdx(i)->GetGraph();
		long lK = get_std_mileage(pGraphMgr->GetPathName());
		POINT2DF ptImgCenter = pGraphMgr->GetTunArc()->GetImgCenter();
		CGraph* pGraph = pGraphMgr->GetGraphByIdx(GRAPH_LAYER_TUNARC);
		if (pGraph->Size()==0)
			continue;

		CUnit* pTunUnit = pGraph->GetUnitByIdx(0);
		CRect rect = pTunUnit->GetPolyRect();
		POINT2DF pt;
		pt.x = (lK-m_lStartDK)*100;
		pt.y = rect.top-ptImgCenter.y;
		pUnit1->m_pPointList->Add(pt);
		pt.y = rect.bottom-ptImgCenter.y;
		pUnit2->m_pPointList->Add(pt);
	}
	for (i=pUnit2->GetPointNum()-1; i>=0; i--)
	{
		POINT2DF pt = pUnit2->m_pPointList->GetAt(i);
		pUnit1->m_pPointList->Add(pt);
	}
	pUnit1->SetPolyClose(TRUE);
	pGraph->PushBackUnit(pUnit1);
	graphMgr.PushBackGraph(pGraph);
	delete pUnit2;

	POINT_3F ptCenter;
	POINT2DF ptImgCenter;
	POINT_3F pt1[40], pt2[40];
	int nNum1, nNum2;
	int nLink, m1, k1;

	pCanvas1 = pMgr->GetCanvasByIdx(0);
	pGraph1 = pCanvas1->GetGraph()->GetGraphByIdx(GRAPH_LAYER_TUNARC);
	lK1 = get_std_mileage(pCanvas1->GetGraph()->GetPathName());

	float fDist1, fDist2;
	deque< deque<POINT_3F> > ddProfile;
	for (int nIdx=2; i<3; i++)
	for (m=nIdx1; m<=nIdx2; m++)
	{
		CTilCanvas* pCanvas1 = GetCanvasMgr()->GetCanvasByIdx(m);
		CGraph* pGraph1 = pCanvas1->GetGraph()->GetGraphByIdx(nIdx);
		// ���Ϊ��,����100���������,�������MESH_SCALE����
		lK1 = get_std_mileage(pCanvas1->GetGraph()->GetPathName());
		fDist1 = abs(lK1-m_lStartDK)*100;

		int nRockNum = pGraph1->Size();

		for (k=0; k<nRockNum; k++)
		{
			CUnit* pUnit1 = pGraph1->GetUnitByIdx(k);

			// ��ǰ������Ϊ-1�����Ŀ���Ѿ���ǰ�汻������ˣ������ﲻ��Ϊ-1
			int nPreLnk = pUnit1->GetPreLink();
			int nNxtLnk = pUnit1->GetNextLink();
			if (pUnit1->GetPreLink()!=-1 || pUnit1->GetNextLink()==-1)
				continue;

			m1 = m;		// ��Ƭ��
			k1 = k;		// ��Ƭ�еĶ����

			// Ϊǰ��������Ӧ��Ŀ�괴������
			nLink = pUnit1->GetNextLink();
			if (nLink != -1)
			{
				// ������ĵ�
				ptImgCenter = pCanvas1->GetGraph()->GetTunArc()->GetImgCenter();

//				TRACE("\n");
				nNum1 = pUnit1->GetPointNum();
				for (i=0; i<nNum1; i++)
				{
					POINT2DF pt = pUnit1->GetPoint(i);
					pt1[i].x = pt.x;
					pt1[i].y = pt.y;
					pt1[i].z = fDist1;
//					TRACE("pt[%d](%4.3f,%4.3f,%4.3f)\n", m1, pt.x, pt.y, pt1[i].z);
//					TRACE("pt[%d](%4.3f,%4.3f,%4.3f)\n", m1, pt1[i].x, pt1[i].y, pt1[i].z);
					pt1[i].x = ptImgCenter.x-pt1[i].x;
					pt1[i].y -= ptImgCenter.y;
				}
			}

			while (nLink != -1)
			{
				// �õ���һ���������ӵ�Ŀ��Ķ�����
				CTilCanvas* pCanvas2 = pMgr->GetCanvasByIdx(m1+1);
				CGraph* pGraph2 = pCanvas2->GetGraph()->GetGraphByIdx(nIdx);
				CUnit* pUnit2 = pGraph2->GetUnitByIdx(nLink);

				// �����ļ���ȡ�����������
				lK2 = get_std_mileage(pCanvas2->GetGraph()->GetPathName());
				fDist2 = abs(lK2-m_lStartDK)*100;

				// ������ĵ�
				ptImgCenter = pCanvas2->GetGraph()->GetTunArc()->GetImgCenter();

				nNum2 = pUnit2->GetPointNum();
				for (i=0; i<nNum2; i++)
				{
					POINT2DF pt = pUnit2->GetPoint(i);
					pt2[i].x = pt.x;
					pt2[i].y = pt.y;
					pt2[i].z = fDist2;
//					TRACE("pt[%d](%4.3f,%4.3f,%4.3f)\n", m1+1, pt.x, pt.y, pt2[i].z);
//					TRACE("pt[%d](%4.3f,%4.3f, %4.3f)\n", m1+1, pt2[i].x, pt2[i].y, pt2[i].z);
					pt2[i].x = ptImgCenter.x-pt2[i].x;
					pt2[i].y -= ptImgCenter.y;
				}

				// ���Ӷ�Ӧ��
				PPPOINT_3F ppLink21;
				int *pNum21;
				if (pUnit1->IsPolyClose())
					link_faces(pt1, nNum1, pt2, nNum2, &ppLink21, &pNum21);
				else
					link_line_segments(pt1, nNum1, pt2, nNum2, &ppLink21, &pNum21);

				// ȡ��������ͼ��
				deque< deque<POINT_3F> > ddLines;
				float fFace[4] = {1, 0, 0, 0};
				predict_profile_face(ppLink21, pNum21, nNum2, fFace, ddLines);

				// ��������洢
				for (n=0; n<ddLines.size(); n++)
					ddProfile.push_back(ddLines[n]);

				for (n=0; n<nNum2; n++)
					delete ppLink21[n];
				delete [] ppLink21;
				delete [] pNum21;

				nNum1 = nNum2;
				for (n=0; n<nNum2; n++)
					pt1[n] = pt2[n];

				m1 = m1+1;
				k1 = nLink;
				nLink = pUnit2->GetNextLink();
				pUnit1 = pUnit2;
				pCanvas1 = pCanvas2;
				lK1 = lK2;
			}
		}
	}

	// �ϲ����ж����
	merge_line_segments(ddProfile);

	// ����õĸ����������߶κϲ�
	pGraph = new CGraph;
	for (m=0; m<ddProfile.size(); m++)
	{
		CUnit* pUnit = new CPolygon;
		pUnit->SetPolyClose(FALSE);
		for (n=0; n<ddProfile[m].size(); n++)
		{
			POINT2DF pt;
			pt.x = ddProfile[m][n].z;	// ��Ϊ�������棬�ʽ�z������ƽ��x����ֵ
			pt.y = ddProfile[m][n].y;
			pUnit->m_pPointList->Add(pt);
		}
		pGraph->PushBackUnit(pUnit);
	}
	graphMgr.PushBackGraph(pGraph);

	return FALSE;
}

// Ԥ�����lDK��������ͼ��
BOOL CMeshTunnel::PreNormalSection(long lDK)
{
	return TRUE;
}

// ������ά�������ṹ��
void CMeshTunnel::ConstructSidePlaneA(CSceneGraph3d &scene, 
	CMesh3d* pMesh, MESH_TYPE type,
	PPOINT_3F pObj1, int nNum1, // ��һ������߻�����
	PPOINT_3F pObj2, int nNum2,	// �ڶ�������߻�����
	int nTextureIdx, int nCanvasIdx)
{
	int nVerNum = pMesh->NbVertex();
	CArray3d<CVertex3d> *pArrayVertex = pMesh->GetArrayVertex();
	CArray3d<CFace3d> *pArrayFace = pMesh->GetArrayFace();
	pMesh->SetMeshType(type);

	//����ê��ƫ��,������������
	CTilCanvasMgr* pMgr = GetCanvasMgr();
	CTilTunArc* pTunArc = pMgr->GetCanvasByIdx(nCanvasIdx)->GetGraph()->GetTunArc();
	POINT2DF ptImgCenter = pTunArc->GetImgCenter();

	int i,j;
	int* pNum21;
	PPPOINT_3F ppLink21;
	// �������������
	if (type==SIDE_FACE || type==ARC_FACE || type==TOP_FACE || type==MESH_ARC || type==MESH_ROCK)
	{
		clock_wise(pObj1,nNum1);
		clock_wise(pObj2,nNum2);
		link_faces(pObj1, nNum1, pObj2, nNum2, &ppLink21, &pNum21);
	}
	// ����ǽṹ��
	else if (type == STRUCT_FACE)
		link_line_segments(pObj1, nNum1, pObj2, nNum2, &ppLink21, &pNum21);
	else
		return;

	// ��Ӷ���
	for (i=0; i<nNum2; ++i)
		pArrayVertex->Add(new CVertex3d(pObj2[i].x, pObj2[i].y, pObj2[i].z));
	for (i=0; i<nNum1; ++i)
		pArrayVertex->Add(new CVertex3d(pObj1[i].x, pObj1[i].y, pObj1[i].z));

	int nIdx = pMesh->NbFace();
	CVertex3d** ppData = pArrayVertex->GetData();
	CVertex3d** ppVertex2 = &ppData[nVerNum];
	CVertex3d** ppVertex1 = &ppData[nVerNum+nNum2];

	int nIdx1 = 0;
	CVertex3d *pV1, *pV2, *pV3;

	// ����������Ƭ
	pV1 = ppVertex2[0];
	pV2 = ppVertex1[nIdx1++];
	for (i = 0; i < nNum2; i ++)
	{
		for (j = 2; j < pNum21[i]; j ++)
		{
			pV3 = ppVertex1[nIdx1++];
			CFace3d* pFace = new CFace3d(pV1, pV2, pV3);
			pArrayFace->SetAtGrow(nIdx++,pFace);

			pV2 = pV3;
			nIdx1 %= nNum1;
		}

		if (i+1 < nNum2)
		{
			pV3 = ppVertex2[i+1];
			CFace3d* pFace = new CFace3d(pV1, pV2, pV3);
			pArrayFace->SetAtGrow(nIdx++,pFace);

			pV1 = pV3;
		}
		else
		{
			// ����Ƿ��������ǽṹ��
			if (type==SIDE_FACE || type==TOP_FACE || type==ARC_FACE || type==MESH_ARC || type==MESH_ROCK)
			{
				pV3 = ppVertex2[0];
				CFace3d* pFace = new CFace3d(pV1, pV2, pV3);
				pArrayFace->SetAtGrow(nIdx++,pFace);
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Ԥ��ǰ���������
	/*
	if (type!=ARC_FACE && nCanvasIdx==6)
	{
		float fFace[] = {0, 0, 1, -35*100*MESH_SCALE};
		PPOINT_3F pObj3;
		int nNum3;
		predict_outer_face(ppLink21, pNum21, nNum2, fFace, &pObj3, nNum3);

		int* pNum32;
		PPPOINT_3F ppLink32;
		// �������������
		if (type == SIDE_FACE)
			link_faces(pObj2, nNum2, pObj3, nNum3, &ppLink32, &pNum32);
		// ����������ṹ��
		else if (type == STRUCT_FACE)
			link_line_segments(pObj2, nNum2, pObj3, nNum3, &ppLink32, &pNum32);
		else
			return;

		// ��Ӷ���
		for (i=0; i<nNum3; ++i)
			pArrayVertex->Add(new CVertex3d(pObj3[i].x, pObj3[i].y, pObj3[i].z));
		for (i=0; i<nNum2; ++i)
			pArrayVertex->Add(new CVertex3d(pObj2[i].x, pObj2[i].y, pObj2[i].z));

		nIdx = pMesh->NbFace();
		ppData = pArrayVertex->GetData();
		CVertex3d** ppVertex3 = &ppData[nVerNum];
		ppVertex2 = &ppData[nVerNum+nNum2];

		nIdx1 = 0;

		// ����������Ƭ
		pV1 = ppVertex3[0];
		pV2 = ppVertex2[nIdx1++];
		for (i = 0; i < nNum3; i ++)
		{
			for (j = 2; j < pNum32[i]; j ++)
			{
				pV3 = ppVertex2[nIdx1++];
				CFace3d* pFace = new CFace3d(pV1, pV2, pV3);
				pArrayFace->SetAtGrow(nIdx++,pFace);

				pV2 = pV3;
				nIdx1 %= nNum2;
			}

			if (i+1 < nNum3)
			{
				pV3 = ppVertex3[i+1];
				CFace3d* pFace = new CFace3d(pV1, pV2, pV3);
				pArrayFace->SetAtGrow(nIdx++,pFace);

				pV1 = pV3;
			}
			else
			{
				// ����Ƿ��������ǽṹ��
				if (type == SIDE_FACE)
				{
					pV3 = ppVertex3[0];
					CFace3d* pFace = new CFace3d(pV1, pV2, pV3);
					pArrayFace->SetAtGrow(nIdx++,pFace);
				}
			}
		}
		delete [] pObj3;
		for (i=0; i<nNum3; i++)
			delete [] ppLink32[i];
		delete [] ppLink32;
		delete [] pNum32;
	}
	*/

	for (i=0; i<nNum2; i++)
		delete [] ppLink21[i];
	delete [] ppLink21;
	delete [] pNum21;

}

// ������ά�������������ϵ��Ҳ�����
void CMeshTunnel::ConstructFace(CSceneGraph3d& scene, MESH_TYPE type,
								PPOINT_3F pObj, int nNum,
								int nTextureIdx, int nCanvasIdx)
{
	int i;

	CMesh3d* pMesh = new CMesh3d;//�����������
	pMesh->SetMeshType(type);
	scene.Add(pMesh);//�����������ӵ�����
	pMesh->SetNbVertex(nNum);//���������ж������
	pMesh->SetNbFace(0);//��������������Ƭ����

	//����ê��ƫ��,������������
//	CTilCanvasMgr* pMgr = GetCanvasMgr();
//	CTilTunArc* pTunArc = pMgr->GetCanvasByIdx(nCanvasIdx)->GetGraph()->GetTunArc();
//	POINT2DF ptImgCenter = pTunArc->GetImgCenter();
//	pMesh->SetLockOffset(ptImgCenter.x*MESH_SCALE, ptImgCenter.y*MESH_SCALE);
	pMesh->SetTextureIndex(nTextureIdx);

	// ������綥��
	CArray3d<CVertex3d> *pArrayVertex = pMesh->GetArrayVertex();
	for (i = 0; i < nNum; ++i)
	{
		pArrayVertex->SetAt(i,new CVertex3d(pObj[i].x, pObj[i].y, pObj[i].z));
	}

	// ��ȡ����������Ƭ
	CArray3d<CFace3d> *pArrayFace = pMesh->GetArrayFace();
	CTriFace faceTop(pArrayFace, pArrayVertex, 0);
	Vector3F* pVC = new Vector3F [nNum];
	for (i=0; i<nNum; ++i)
	{
		pVC[i][0] = pObj[i].x;
		pVC[i][1] = pObj[i].y;
		pVC[i][2] = pObj[i].z;
	}
	Vector3F vcNorm(0,0,1);
	faceTop.Triangulate(pVC, vcNorm, nNum);
	pMesh->Rebuild();

	delete [] pVC;
}

// ��������ͬ���ϵĶ���εĶ����Ӧ��������
// PPOINT_3F pPolygon1, int nNum1, : ��һ�������涥�����꼰������
// PPOINT_3F pPolygon2, int nNum2, : �ڶ��������涥�����꼰������
// PPPOINT_3F ppLink21, int* pNum21 : ��һ������������Ķ�Ӧ������
// ע��: ppPolygon����ָ��������ڴ棬�����һ��ָ��û�з����ڴ�
void CMeshTunnel::link_faces(PPOINT_3F pPolygon1, int nNum1,
							PPOINT_3F pPolygon2, int nNum2,
							POINT_3F*** pppLink21, int** ppNum21)
{
	// ȫ��ת����˳ʱ�뷽��
	clock_wise(pPolygon1, nNum1);
	clock_wise(pPolygon2, nNum2);

	float MinLen, fMinTemp;
	int MinNum;
	int i;

	PROPERTY pro1, pro2, proMean, proDiff;
	CalProperty(pPolygon1, nNum1, pro1, 1, 1);
	CalProperty(pPolygon2, nNum2, pro2, 1, 1);

	proMean.fArea = (pro1.fArea + pro2.fArea) / 2;
	proMean.ptMean.x = (pro1.ptMean.x + pro2.ptMean.x) / 2;
	proMean.ptMean.y = (pro1.ptMean.y + pro2.ptMean.y) / 2;

	proDiff.fArea = pro2.fArea - pro1.fArea;
	proDiff.ptMean.x = pro2.ptMean.x - pro1.ptMean.x;
	proDiff.ptMean.y = pro2.ptMean.y - pro1.ptMean.y;

	if(pro1.fArea < 0.000001)
		return;

	if (fabs(pro1.right - pro1.left) < 0.000001 && fabs(pro1.bottom - pro1.top) < 0.000001)
		return;

	float fscale = float(pro2.fArea / pro1.fArea);
	float fscalex = float(fabs(pro2.right - pro2.left) / fabs(pro1.right - pro1.left));
	float fscaley = float(fabs(pro2.bottom - pro2.top) / fabs(pro1.bottom - pro1.top));

	MinLen = FLT_MAX;
	MinNum = 0;

  // ������һ���������̵ĵ㲢�����
	for(i = 0;i<nNum1;i++)
	{
		fMinTemp = fabs(pPolygon2[0].x - pro2.ptMean.x-(pPolygon1[i].x - pro1.ptMean.x) * fscalex)
						+ fabs(pPolygon2[0].y - pro2.ptMean.y - (pPolygon1[i].y - pro1.ptMean.y)* fscaley);

		if(MinLen>fMinTemp)
		{
			MinLen=fMinTemp;
			MinNum=i;
		}
	}

	float fCX, fCY, fCZ;
	float fLen1, fLen2;
	float fWeight = 0.5;
	int nNextWeight = 0;
	int nLocalFace = 0;
	int nIdx1, nIdx2;

	*pppLink21 = new PPOINT_3F [nNum2];
	*ppNum21 = new int [nNum2];
	PPOINT_3F pTemp = new POINT_3F [3 * (nNum1 + nNum2)];
	int nPtNum = 0;
	bool flag1, flag2;

	flag1 = false;
	flag2 = false;
	nIdx2 = 0;
	nIdx1 = MinNum;
	pTemp[0] = pPolygon2[0];
	pTemp[1] = pPolygon1[MinNum];
	nPtNum = 2;
	while (1)
	{
		fCX = ((pPolygon2[nIdx2%nNum2].x - pro2.ptMean.x)
				 + (pPolygon1[nIdx1%nNum1].x - pro1.ptMean.x) * fscalex) / 2.0;
		fCY = ((pPolygon2[nIdx2%nNum2].y - pro2.ptMean.y)
				 + (pPolygon1[nIdx1%nNum1].y - pro1.ptMean.y) * fscaley) / 2.0;
		fCZ = (pPolygon2[nIdx2%nNum2].z + pPolygon1[nIdx1%nNum1].z) / 2.0;

		// �߶�1-2���е��3,4�ľ���
		fLen2 = fabs(fCX - (pPolygon2[(nIdx2+1)%nNum2].x - pro2.ptMean.x))
					+ fabs(fCY - (pPolygon2[(nIdx2+1)%nNum2].y - pro2.ptMean.y));
		fLen1 = fabs(fCX - (pPolygon1[(nIdx1+1)%nNum1].x - pro1.ptMean.x) * fscalex)
					+ fabs(fCY - (pPolygon1[(nIdx1+1)%nNum1].y - pro1.ptMean.y) * fscaley);

		if (nNextWeight == 2)			 fLen1 *= fWeight;
		if (nNextWeight == 1)			 fLen2 *= fWeight;

		// ����е��3����
		if (fLen1 < fLen2)
		{
			nNextWeight = 1;
			pTemp[nPtNum ++] = pPolygon1[(nIdx1+1)%nNum1];

			nIdx1 ++;
			nIdx1 %= nNum1;
			flag1 = true;
		}
		else
		{
			nNextWeight = 2;
			(*pppLink21)[nIdx2] = new POINT_3F [nPtNum];
			memcpy((*pppLink21)[nIdx2], pTemp, sizeof(POINT_3F)*nPtNum);
			(*ppNum21)[nIdx2 ++] = nPtNum;

			nIdx2 %= nNum2;

			pTemp[0] = pPolygon2[nIdx2];
			pTemp[1] = pTemp[nPtNum-1];
			nPtNum = 2;
			flag2 = true;
		}

		// ������Ƭ��������1
		++nLocalFace;

#if 1
		if (flag1 && flag2)
		{
			if (nIdx1 == MinNum)
			{
				(*pppLink21)[nIdx2] = new POINT_3F [nPtNum];
				memcpy((*pppLink21)[nIdx2], pTemp, sizeof(POINT_3F)*nPtNum);
				(*ppNum21)[nIdx2 ++] = nPtNum;
				for (i = nIdx2; i < nNum2; i ++)
				{
					(*pppLink21)[i] = new POINT_3F [2];
					(*pppLink21)[i][0] = pPolygon2[i];
					(*pppLink21)[i][1] = pPolygon1[MinNum];
					(*ppNum21)[i] = 2;
				}
				break;
			}
			else if (nIdx2 == 0)
			{
				// �������㿽������һ��Ĵ洢λ��
				pTemp[0] = (*pppLink21)[0][0];
				nPtNum = 1;
				while (nIdx1 != MinNum)
				{
					pTemp[nPtNum ++] = pPolygon1[nIdx1 ++];
					nIdx1 %= nNum1;
				}
				memcpy(pTemp+nPtNum, (*pppLink21)[0]+1, sizeof(POINT_3F)*((*ppNum21)[0]-1));
				delete [] (*pppLink21)[0];

				(*ppNum21)[0] += nPtNum-1;
				(*pppLink21)[0] = new POINT_3F [ (*ppNum21)[0] ];
				memcpy((*pppLink21)[0], pTemp, sizeof(POINT_3F)*(*ppNum21)[0]);
				break;
			}
		}
#else
		// �����һ������ĵ�ǰ12�����ǵ�һ�μ����12����
		if (nIdx2 == 0 && nIdx1 == MinNum)
		{
 			if (pTemp[0] == ppLink21[0][0])
			{
				memcpy(pTemp+nPtNum, (*pppLink21)[0]+1, sizeof(POINT_3F)*(pNum21[0]-1));
				delete [] (*pppLink21)[0];
				pNum21[0] += nPtNum-1;
				(*pppLink21)[0] = new POINT_3F [ pNum21[0] ];
				memcpy((*pppLink21)[0], pTemp, sizeof(POINT_3F)*pNum21[0]);
			}
			break;
		}
#endif

		if (nLocalFace >= 3 * (nNum1 + nNum2))
			break;
	}

	delete [] pTemp;
}

// ��������ͬ���ϵĶ���ߵĶ����Ӧ��������
void CMeshTunnel::link_line_segments(PPOINT_3F pPolygon1, int nNum1,
									PPOINT_3F pPolygon2, int nNum2,
									POINT_3F*** pppLink21, int** ppNum21)
{
	if (nNum1<=0 || nNum2<=0)
		return;

	int i;

	PROPERTY pro1, pro2, proMean, proDiff;
	CalProperty(pPolygon1, nNum1, pro1, 1, 1);
	CalProperty(pPolygon2, nNum2, pro2, 1, 1);

	proMean.fArea = (pro1.fArea + pro2.fArea) / 2;
	proMean.ptMean.x = (pro1.ptMean.x + pro2.ptMean.x) / 2;
	proMean.ptMean.y = (pro1.ptMean.y + pro2.ptMean.y) / 2;

	proDiff.fArea = pro2.fArea - pro1.fArea;
	proDiff.ptMean.x = pro2.ptMean.x - pro1.ptMean.x;
	proDiff.ptMean.y = pro2.ptMean.y - pro1.ptMean.y;

	if(pro1.fArea < 0.000001)
		return;

	if (fabs(pro1.right - pro1.left) < 0.000001 && fabs(pro1.bottom - pro1.top) < 0.000001)
		return;

	float fscale = float(pro2.fArea / pro1.fArea);
	float fscalex = float(fabs(pro2.right - pro2.left) / fabs(pro1.right - pro1.left));
	float fscaley = float(fabs(pro2.bottom - pro2.top) / fabs(pro1.bottom - pro1.top));

	float fCX, fCY, fCZ;
	float fLen1, fLen2;
	float fWeight = 0.5;
	int nNextWeight = 0;
	int nIdx1, nIdx2;

	*ppNum21 = new int [nNum2];
	*pppLink21 = new PPOINT_3F [nNum2];
	PPOINT_3F pTemp = new POINT_3F [nNum1+nNum2];
	int nPtNum = 0;

	nIdx2 = 0;
	nIdx1 = 0;
	pTemp[0] = pPolygon2[0];
	pTemp[1] = pPolygon1[0];
	nPtNum = 2;
	while (1)
	{
		fCX = ((pPolygon2[nIdx2].x - pro2.ptMean.x)
			 + (pPolygon1[nIdx1].x - pro1.ptMean.x) * fscalex) / 2.0;
		fCY = ((pPolygon2[nIdx2].y - pro2.ptMean.y)
			 + (pPolygon1[nIdx1].y - pro1.ptMean.y) * fscaley) / 2.0;
		fCZ = (pPolygon2[nIdx2].z + pPolygon1[nIdx1].z) / 2.0;

		// �߶�1-2���е��3,4�ľ���
		fLen2 = fabs(fCX - (pPolygon2[nIdx2+1].x - pro2.ptMean.x))
			  + fabs(fCY - (pPolygon2[nIdx2+1].y - pro2.ptMean.y));
		fLen1 = fabs(fCX - (pPolygon1[nIdx1+1].x - pro1.ptMean.x) * fscalex)
			  + fabs(fCY - (pPolygon1[nIdx1+1].y - pro1.ptMean.y) * fscaley);

		if (nNextWeight == 2)			 fLen1 *= fWeight;
		if (nNextWeight == 1)			 fLen2 *= fWeight;

		// ����е��3����
		if (fLen1 < fLen2)
		{
			nNextWeight = 1;
			pTemp[nPtNum ++] = pPolygon1[nIdx1+1];

			nIdx1 ++;
		}
		else
		{
			nNextWeight = 2;
			(*pppLink21)[nIdx2] = new POINT_3F [nPtNum];
			memcpy((*pppLink21)[nIdx2], pTemp, sizeof(POINT_3F)*nPtNum);
			(*ppNum21)[nIdx2 ++] = nPtNum;

			pTemp[0] = pPolygon2[nIdx2];
			pTemp[1] = pTemp[nPtNum-1];
			nPtNum = 2;
		}

		// �����һ������ߵ�ͷ��
		if (nIdx1 == nNum1-1)
		{
			(*pppLink21)[nIdx2] = new POINT_3F [nPtNum];
			memcpy((*pppLink21)[nIdx2], pTemp, sizeof(POINT_3F)*nPtNum);
			(*ppNum21)[nIdx2 ++] = nPtNum;
			for (i = nIdx2; i < nNum2; i ++)
			{
				(*pppLink21)[i] = new POINT_3F [2];
				(*pppLink21)[i][0] = pPolygon2[i];
				(*pppLink21)[i][1] = pPolygon1[nNum1-1];
				(*ppNum21)[i] = 2;
			}
			break;
		}
		// ����ڶ�������ߵ�ͷ��
		else if (nIdx2 == nNum2-1)
		{
			for (i=nIdx1+1; i<nNum1; i++)
			{
				pTemp[nPtNum++] = pPolygon1[i];
			}
			(*pppLink21)[nNum2-1] = new POINT_3F [nPtNum];
			memcpy((*pppLink21)[nNum2-1], pTemp, sizeof(POINT_3F)*nPtNum);
			(*ppNum21)[nIdx2] = nPtNum;
			break;
		}
	}

	delete [] pTemp;
}

// ��������������Ԥ����һ��������
// PPPOINT_3F ppLinkFace12, int nNum12 : ����������Ķ�Ӧ������
// double* pFace3, : Ԥ����
// PPOINT_3F pPolygon3, int& nNum3: ���Ԥ�����ϵĶ�����
void CMeshTunnel::predict_outer_face(PPPOINT_3F ppLink21, int* pNum21, int nNum2,
									float* pFace3, PPPOINT_3F ppPolygon3, int& nNum)
{
	POINT_3F point;
	float fFaceA[4], fFaceB[4];

	nNum = 0;
	*ppPolygon3 = new POINT_3F [nNum2];
	for (int i = 2; i < nNum2; i ++)
	{
		// ��Ϊ�ṹ��ʱȡ��ֱ����Ԥ����Ľ���
		if (i==2)
		{
			get_line_face_interpoint(ppLink21[i-2][0], ppLink21[i-2][1], pFace3, point);
			(*ppPolygon3)[nNum ++] = point;
		}

		get_a_face(ppLink21[i-2][0], ppLink21[i-1][0], ppLink21[i-1][1], fFaceA);
		get_a_face(ppLink21[i-1][0], ppLink21[i][0], ppLink21[i][1], fFaceB);

		get_3faces_interpoint(fFaceA, fFaceB, pFace3, point);
		(*ppPolygon3)[nNum ++] = point;

		if (i==nNum2-1)
		{
			get_line_face_interpoint(ppLink21[i][0], ppLink21[i][pNum21[i]-1], pFace3, point);
			(*ppPolygon3)[nNum ++] = point;
		}
	}

//	get_a_face(ppLink21[i-2][0], ppLink21[i-1][0], ppLink21[i-1][1], fFaceA);
//	get_a_face(ppLink21[i-1][0], ppLink21[0][0], ppLink21[0][1], fFaceB);
//	get_3faces_interpoint(fFaceA, fFaceB, pFace3, point);
//	(*ppPolygon3)[nNum ++] = point;
//
//	get_a_face(ppLink21[i-1][0], ppLink21[0][0], ppLink21[0][1], fFaceA);
//	get_a_face(ppLink21[0][0], ppLink21[1][0], ppLink21[1][1], fFaceB);
//	get_3faces_interpoint(fFaceA, fFaceB, pFace3, point);
//	(*ppPolygon3)[nNum ++] = point;
}

// Ԥ������������֮�����һ������
// PPPOINT_3F ppLink13, int nNum13 : ����������Ķ�Ӧ������
// double* pFace2, : Ԥ����
// PPOINT_3F pPolygon2, int& nNum2: ���Ԥ�����ϵĶ����*/
void CMeshTunnel::predict_inner_face(PPPOINT_3F ppLink31, int* pNum31, int nNum3,
								float* pFace2, PPPOINT_3F ppPolygon2, int& nNum2)
{
	int i, j, nIdx;
	POINT_3F point;

	nNum2 = 0;
	for (i = 0; i < nNum3; i ++)
		nNum2 += pNum31[i]-1;

	// ������һ�����ǵ�һ

	*ppPolygon2 = new POINT_3F [nNum2];

	nIdx = 0;
	for (i = 0; i < nNum3; i ++)
	{
		for (j = 1; j < pNum31[i]; j ++)
		{
			get_line_face_interpoint(ppLink31[i][0], ppLink31[i][j], pFace2, point);
			(*ppPolygon2)[nIdx ++] = point;
		}
	}

	nNum2 = nIdx;
}

// Ԥ������������֮�����һ������������
// PPPOINT_3F ppLinkFace12, int nNum1, : һ��������֮��ĵ������
// double* pFace2	: 
// PPOINT_3F pPolygon2, int& nNum: ���Ԥ�����ϵĶ����
void predict_outer_face(PPPOINT_3F ppLink12, int* pNum12, int nNum1,
						float* pFace2, 
						PPPOINT_3F ppPolygon2, int& nNum2)
{

}

// Ԥ������������֮�����һ��������
// PPPOINT_3F ppLinkFace12, int nNum1, : һ��������֮��ĵ������
// PPPOINT_3F ppLinkFace23, int nNum2, : ����������֮��ĵ������
// double* pFace4,	: 
// PPOINT_3F pPolygon4, int& nNum
void CMeshTunnel::predict_outer_face(PPPOINT_3F ppLink21, int* pNum21, int nNum2,
									PPPOINT_3F ppLink32, int* pNum32, int nNum3,
									double* pFace4, 
									PPPOINT_3F ppPolygon4, int& nNum4)
{

}

// Ԥ������������֮�����һ������
// PPPOINT_3F ppLink31, int* pNum31, int nNum3 : ����������Ķ�Ӧ������
// double* pFace2 : Ԥ��������
// PPOINT_3F pPolygon2, int& nNum2: ���Ԥ�����ϵĶ�����
void CMeshTunnel::predict_profile_face(PPPOINT_3F ppLink31, int* pNum31, int nNum3,
									float* pFace2, deque< deque<POINT_3F> >& ddPts31)
{
	ASSERT(nNum3>0);

	int i, j;
	BOOL bIsClosed = FALSE;

	// ����ռ�洢ƽ�����������Ƭ�Ľ���
	POINT_3F pt[3];
	PPOINT_3F pPt=NULL;
	int nNum;
	for (i=0; i<nNum3; i++)
	{
		pt[0] = ppLink31[i][0];
		for (j=2; j<pNum31[i]; j++)
		{
			pt[1] = ppLink31[i][j-1];
			pt[2] = ppLink31[i][j];
			get_faces_intersections(pFace2, pt, 3, &pPt, nNum);
			if (nNum==2)
			{
				deque<POINT_3F> dPt;
				dPt.push_back(pPt[0]);
				dPt.push_back(pPt[1]);
				ddPts31.push_back(dPt);
			}
			delete pPt;
			pPt = NULL;
		}
	}

	// �����3�����ϸ��߶�����������Ƭ��ƽ��Ľ���
	for (i=1; i<nNum3; i++)
	{
		pt[0] = ppLink31[i-1][0];
		pt[1] = ppLink31[i][1];
		pt[2] = ppLink31[i][0];
		get_faces_intersections(pFace2, pt, 3, &pPt, nNum);
		if (nNum==2)
		{
			deque<POINT_3F> dPt;
			dPt.push_back(pPt[0]);
			dPt.push_back(pPt[1]);
			ddPts31.push_back(dPt);
		}
		delete pPt;
		pPt = NULL;
	}

	// ����Ƿ�����ߣ�������һ������һ�����߶�����������Ƭ��ƽ��Ľ���
	// ������ĵ�ʽ��������˵���Ƿ������
	if (ppLink31[0][1] == ppLink31[nNum3-1][ pNum31[nNum3-1]-1 ])
	{
		bIsClosed = TRUE;
		pt[0] = ppLink31[nNum3-1][0];
		pt[1] = ppLink31[0][1];
		pt[2] = ppLink31[0][0];
		get_faces_intersections(pFace2, pt, 3, &pPt, nNum);
		// �����2�����㣬���н��߶Σ��ű���
		if (nNum==2)
		{
			deque<POINT_3F> dPt;
			dPt.push_back(pPt[0]);
			dPt.push_back(pPt[1]);
			ddPts31.push_back(dPt);
		}
		delete pPt;
		pPt = NULL;
	}

	// �����е��߶κϲ���һ���γ�������
	merge_line_segments(ddPts31);

	// �����������ߵ�1������Ϊ�ڵ�3�߽����ϵĵ�
	int nLines = ddPts31.size();
	for (i=0; i<nLines; i++)
	{
		POINT_3F pt, pt1, pt2;
		pt = ddPts31[i][0];
		for (j=1; j<nNum3; j++)
		{
			pt1 = ppLink31[j-1][0];
			pt2 = ppLink31[j][0];
			if (point_in_line(pt, pt1, pt2)==POINT_IN_LINE)
			{
				goto step;
			}
		}
		// ����Ƿ������,�������1�������һ�����߶�
		if (bIsClosed)
		{
			pt1 = ppLink31[0][0];
			pt2 = ppLink31[nNum3-1][0];
			if (point_in_line(pt, pt1, pt2)==POINT_IN_LINE)
			{
				goto step;
			}
		}

		// ��������ߵ�1��û���ڵ�3�߽�����,���������е�Ĵ洢˳����
		reverse(ddPts31[i].begin(), ddPts31[i].end());
step:
		continue;
	}
}

// ��������ͬ�˵���߶λ����ߺϲ�
void CMeshTunnel::merge_line_segments(deque< deque<POINT_3F> >& ddPts)
{
	// �����е��߶κϲ���һ���γ�������
	int nLines, nLines1;
	while (1)
	{
		nLines1 = ddPts.size();
		nLines = nLines1;
		for (int i=0; i<nLines; i++)
		{
			for (int j=nLines-1; j>i; j--)
			{
				// ���ĳ�˵���ͬ����ǰ��˵㷴���ѹ���ͷ
				if (ddPts[i][0]==ddPts[j][0])
				{
					reverse(ddPts[j].begin(), ddPts[j].end());
					ddPts[i].insert(ddPts[i].begin(), ddPts[j].begin(), ddPts[j].end()-1);
					ddPts.erase(ddPts.begin()+j);
				}
				// �������һ�˵���ͬ����ǰ��˵�ѹ���ͷ
				else if (ddPts[i][0]==ddPts[j][1])
				{
					ddPts[i].insert(ddPts[i].begin(), ddPts[j].begin(), ddPts[j].end()-1);
					ddPts.erase(ddPts.begin()+j);
				}
				// �����һ�˵����1���˵���ͬ�����ڶ�βѹ���̶˵�
				else if (ddPts[i][ddPts[i].size()-1]==ddPts[j][0])
				{
					ddPts[i].insert(ddPts[i].end(), ddPts[j].begin()+1, ddPts[j].end());
					ddPts.erase(ddPts.begin()+j);
				}
				// �����һ�˵����2���˵���ͬ�����ڶ�βѹ���1��
				else if (ddPts[i][ddPts[i].size()-1]==ddPts[j][1])
				{
					reverse(ddPts[j].begin(), ddPts[j].end());
					ddPts[i].insert(ddPts[i].end(), ddPts[j].begin()+1, ddPts[j].end());
					ddPts.erase(ddPts.begin()+j);
				}
			}
			nLines = ddPts.size();
		}

		// �����ʽ��ȣ���˵�����������ߺϲ����
		if (nLines1==nLines)
			break;
	}
}

float CMeshTunnel::clock_wise(PPOINT2DF pPts, int nNum)
{
	float fArea = 0.0;
	for (int i=1; i<nNum; i++)
	{
		fArea += pPts[i-1].x*pPts[i].y-pPts[i-1].y*pPts[i].x;
	}
	fArea += pPts[i-1].x*pPts[0].y-pPts[i-1].y*pPts[0].x;

	if (fArea < 0)
	{
		fArea = -fArea;
		// ����
		for (i=0; i<nNum/2; i++)
		{
			POINT2DF pt;
			pt = pPts[i];
			pPts[i] = pPts[nNum-i-1];
			pPts[nNum-i-1] = pt;
		}
	}
	return fArea;
}

// ��ƽ���еĶ�����ϵĹؼ��㰴˳ʱ��洢
// ע�⣺�˴�Ĭ��z������ͬ
float CMeshTunnel::clock_wise(PPOINT_3F pPts, int nNum)
{
	float fArea = 0.0;
	for (int i=1; i<nNum; i++)
	{
		fArea += pPts[i-1].x*pPts[i].y-pPts[i-1].y*pPts[i].x;
	}
	fArea += pPts[i-1].x*pPts[0].y-pPts[i-1].y*pPts[0].x;

	if (fArea < 0)
	{
		fArea = -fArea;
		// ����
		for (i=0; i<nNum/2; i++)
		{
			POINT_3F pt;
			pt = pPts[i];
			pPts[i] = pPts[nNum-i-1];
			pPts[nNum-i-1] = pt;
		}
	}
	return fArea;
}

// �����������Ķ�Ӧ��ϵ��
// ע�⣬���������������������ձ�ʹ��
BOOL CMeshTunnel::VirtualGeoDrill(CSceneGraph3d& scene, /*PPOINT_3F pPts, int nNum, */float fRadius)
{
//	ASSERT(nNum > 0);

	int nNum = 2;
	POINT_3F pPts[2];
	// Ӳ�Լ������������
	pPts[0].x = 750;	pPts[0].y = 450;	pPts[0].z = 0;
	pPts[1].x =	750;	pPts[1].y = 450;	pPts[1].z = 4000;

	int nDividNum = 10;
	PPOINT2DF pDivdPts;

	divide_circle(fRadius, nDividNum, pDivdPts);
	for (int i=0; i<nDividNum; i++)
	{
		pDivdPts[i].x += pPts[0].x;
		pDivdPts[i].y += pPts[0].y;
	}

	// ��õ�һ��Բ�ı߽��
	PPOINT_3F pPt0 = new POINT_3F [nDividNum];
	for (i=0; i<nDividNum; i++)
	{
		pPt0[i].x = pDivdPts[i].x;
		pPt0[i].y = pDivdPts[i].y;
		pPt0[i].z = pPts[0].z;
	}

	// ���2����Բ�ı߽�㣨����Բ�����1��Բ��ļн�Ϊ45��)
	PPOINT_3F pPt1 = new POINT_3F [nDividNum];
	for (i=0; i<nDividNum; i++)
	{
		pPt1[i].x = pPt0[i].x;
		pPt1[i].y = pPt0[i].y;
		pPt1[i].z = (pPts[1].z-pPts[0].z);
	}

	TRACE("\n");
	for (i=0; i<nDividNum; i++)
	{
		pPt0[i].x *= MESH_SCALE;
		pPt0[i].y *= MESH_SCALE;
		pPt0[i].z *= MESH_SCALE;

		pPt1[i].x *= MESH_SCALE;
		pPt1[i].y *= MESH_SCALE;
		pPt1[i].z *= MESH_SCALE;
	}

	// ����������󲢽�����ӵ�����
	CMesh3d* pMesh = new CMesh3d;
	scene.Add(pMesh);
	pMesh->SetTextureIndex(0);

	ConstructSidePlaneA(scene, pMesh, MESH_ROCK, pPt0, nDividNum, pPt1, nDividNum, 0, 0);

	delete [] pPt0;
	delete [] pPt1;
	delete [] pDivdPts;

	pMesh->Rebuild();

	return TRUE;
}

// ��Բ���Ƕ�ƽ�֣�����ÿ��ƽ�ֵ�����꣬�����δ洢
void CMeshTunnel::divide_circle(float fRadius, int nNum, PPOINT2DF& pPts)
{
	ASSERT(nNum > 0);
	double dbAngle = 2*PI/nNum;

	pPts = new POINT2DF [nNum];
	for (int i=0; i<nNum; i ++)
	{
		double dbAngle1 =dbAngle*i;
		POINT2DF pt;
		pt.x = fRadius*cos(dbAngle1);
		pt.y = fRadius*sin(dbAngle1);
		pPts[i] = pt;
	}
}

void CMeshTunnel::divide_circle(POINT2DF ptCenter, float fRadius, int nNum, PPOINT2DF& pPts)
{
	ASSERT(nNum > 0);
	double dbAngle = 2*PI/nNum;

	pPts = new POINT2DF [nNum];
	for (int i=0; i<nNum; i ++)
	{
		double dbAngle1 =dbAngle*i;
		pPts[i].x = fRadius*cos(dbAngle1);
		pPts[i].y = fRadius*sin(dbAngle1);
		pPts[i].x += ptCenter.x;
		pPts[i].y += ptCenter.y;
	}
}

// �ָ�Բ������Բ���Ի���ƽ���ָ�Ķ˵����ӱ�ʾ
// ��˳ʱ�뷽��,�����˵���д洢
void CMeshTunnel::divide_arc(const POINT2DF& pt1, const POINT2DF& pt2,
							 const POINT2DF& ptCenter, PPOINT2DF& pPts, int& nNum)
{
	float fRadius = sqrt(pt1.x*pt1.x+pt1.y*pt1.y);
	float fAngle = get_angle(pt1,ptCenter,pt2);
	float fArea = 0;
	fArea += pt1.x*pt2.y-pt1.y*pt2.x;
	fArea += pt2.x*ptCenter.y-pt2.y*ptCenter.x;
	fArea += ptCenter.x*pt1.y-ptCenter.y*pt1.x;

	// �����������ʱ���˳ʱ�����
	if (fArea<0)
	{
		fAngle = 2*PI-fAngle;
	}

	// ������ƽ���Ƕ�
	nNum = fAngle/AVE_ANGLE+0.5;
	nNum = (nNum<2)?2:nNum;
	float fAveAngle = fAngle/nNum;

	// ������תfAveAngle�ǶȺ�ĵ�
	pPts = new POINT2DF [nNum];
	POINT2DF pt = pt1;
	float fAngle1 = 0;
	pPts[0] = pt1;
	for (int i=1; i<nNum; i++)
	{
		get_arc_point(pt, ptCenter, -fAveAngle, pPts[i]);
		pt = pPts[i];
	}
//	pPts[i] = pt2;

}

// �������������ṹ���Ľ���
/* ==================================================================
 * �������ƣ�  CMeshTunnel::VirtualGeoDrill()
 * ���ߣ�      [Leng]
 * ������:     [2009-4-24 12:04:30]
 * �����б� 
               [CSceneGraph3d& scene] - [In]
               [POINT2DF ptCenter] - [In] ���ĵ�����,��λΪ����
               [ float fRadius] - [In] ������װ뾶
ע�� :	
				
==================================================================*/
void CMeshTunnel::VirtualGeoDrill(CSceneGraph3d& scene,	POINT2DF ptCenter, float fRadius)
{
	int i,j,k,m,nIdx;
	int nMeshNum, nFaceNum, nVerNum;
//	deque<PCROSSPOINT> dSection;
	PPOINT_3F pObj = NULL;

	// ����ת��
	ptCenter.x *= MESH_SCALE;
	ptCenter.y *= MESH_SCALE;
	fRadius *= MESH_SCALE;
	CMesh3d* pMesh = new CMesh3d;

	nMeshNum = scene.NbObject();
	for (i=0; i<nMeshNum; i++)
	{
		CMesh3d* pMesh = (CMesh3d*)scene.GetAt(i);
		nFaceNum = pMesh->NbFace();
		for (j=0; j<nFaceNum; j++)
		{
			CFace3d* pFace = pMesh->GetFace(j);
			nVerNum = pFace->NbVertex();
			POINT2DF* pPts = new POINT2DF [nVerNum];
//			TRACE("\n");
			for (k=0; k<nVerNum; k++)
			{
				CVertex3d* pVertex = pFace->v(k);
				pPts[k].x =pVertex->x();
				pPts[k].y =pVertex->y();
//				TRACE("%9.6f,%9.6f\n",pPts[k].x, pPts[k].y);
			}

			clock_wise(pPts, nVerNum);

			// ��������Բ�Ľ���
			PCROSSPOINT pCrossPt = poly_circle_section(ptCenter,fRadius,pPts,nVerNum);
			if (pCrossPt == NULL)
			{
				if (pt_in_polygon(ptCenter,pPts,nVerNum))
				{
					pCrossPt = new CROSSPOINT;
					pCrossPt->nS = 3;
					pCrossPt->x  = ptCenter.x;
					pCrossPt->y  = ptCenter.y;
					pCrossPt->r  = fRadius;
					pCrossPt->next = NULL;
//					dSection.push_back(pCrossPt);
				}
				// ���û�н���,�򷵻ؼ���ѭ��
				else
				{
					delete [] pPts;
					continue;
				}
			}

			// �����Ƭ����ƽ��ķ���
			PPOINT_3F pPolygon = new POINT_3F [nVerNum];
			for (k=0; k<nVerNum; k++)
			{
				CVertex3d* pVertex = pFace->v(k);
				pPolygon[k].x = pVertex->x();
				pPolygon[k].y = pVertex->y();
				pPolygon[k].z = pVertex->z();
			}
			float fFace[4];
	
			get_a_face(pPolygon,nVerNum,fFace);
			delete [] pPolygon;

			// ������õı߽�㽨��ȡ����ά����
			PCROSSPOINT p, p1;
			p = pCrossPt;
			while (p != NULL)
			{
				p->z = -(fFace[0]*p->x+fFace[1]*p->y+fFace[3])/fFace[2];
				p = p->next;
			}

			// ������ת���ɶ�����д洢
			// ��Բ��߽�ת���ɶ���
			p = pCrossPt;
			if (p!=NULL && p->next==NULL)
			{
				PPOINT2DF pRoundPts;
				divide_circle(ptCenter, fRadius, 10, pRoundPts);
				pObj = new POINT_3F [10];
				for (m=0; m<10; m++)
				{
					pObj[m].x = pRoundPts[m].x;
					pObj[m].y = pRoundPts[m].y;
					pObj[m].z = -(fFace[0]*pObj[m].x+fFace[1]*pObj[m].y+fFace[3])/fFace[2];
				}
				nIdx = 10;
				delete [] pRoundPts;
			}
			else
			{
				nIdx = 0;
				p = p1 = pCrossPt;
				pObj = new POINT_3F [100];
				while (p != NULL)
				{
					p1 = p->next;
					if (p1 != NULL)
					{
						// �����һ���߶�
						if (p->nS==0 || p->nS==2)
						{
							pObj[nIdx].x = p->x;
							pObj[nIdx].y = p->y;
							pObj[nIdx].z = p->z;
							nIdx ++;
						}
						// �����һ�λ�,�򽫻��ֶκ󱣴�
						else if (p->nS==1)
						{
							int nNum;
							POINT2DF pt1, pt2,*pArcPts;
							pt1.x = p->x;
							pt1.y = p->y;
							pt2.x = p1->x;
							pt2.y = p1->y;
							divide_arc(pt1,pt2,ptCenter,pArcPts,nNum);
							for (m=0; m<nNum; m++)
							{
								pObj[nIdx].x = pArcPts[m].x;
								pObj[nIdx].y = pArcPts[m].y;
								pObj[nIdx].z = -(fFace[0]*pArcPts[m].x+fFace[1]*pArcPts[m].y+fFace[3])/fFace[2];
								nIdx ++;
							}
							delete [] pArcPts;
						}
						p = p1;
					}
					else
						break;
				}

				// �������һ�����һ��֮��ı���
				// ������߶�
				if (p->nS==0 || p->nS==2)
				{
					pObj[nIdx].x = p->x;
					pObj[nIdx].y = p->y;
					pObj[nIdx].z = p->z;
					nIdx ++;
				}
				// �����Բ��,��Բ���ֶκ󱣴�
				else if (p->nS==1)
				{
					int nNum;
					POINT2DF pt1, pt2,*pArcPts;
					pt1.x = p->x;			pt1.y = p->y;
					pt2.x = pCrossPt->x;	pt2.y = pCrossPt->y;
					divide_arc(pt1,pt2,ptCenter,pArcPts,nNum);
					for (m=0; m<nNum; m++)
					{
						pObj[nIdx].x = pArcPts[m].x;
						pObj[nIdx].y = pArcPts[m].y;
						pObj[nIdx].z = -(fFace[0]*pArcPts[m].x+fFace[1]*pArcPts[m].y+fFace[3])/fFace[2];
						nIdx ++;
					}
					delete [] pArcPts;
				}
			}

//			clock_wise(pObj, nIdx);
			if (nIdx==3)
			{
				CVertex3d* pV1 = new CVertex3d(pObj[0].x,pObj[0].y,pObj[0].z);
				CVertex3d* pV2 = new CVertex3d(pObj[1].x,pObj[1].y,pObj[1].z);
				CVertex3d* pV3 = new CVertex3d(pObj[2].x,pObj[2].y,pObj[2].z);
				CFace3d* pFace = new CFace3d(pV1,pV2,pV3);
				CMesh3d* pMesh = new CMesh3d();
				pMesh->AddFace(pFace);
				pMesh->AddVertex(pV1);
				pMesh->AddVertex(pV2);
				pMesh->AddVertex(pV3);
				pMesh->SetMeshType(TOP_FACE);
				pMesh->SetTextureIndex(0);
				scene.Add(pMesh);
			}
			else
				ConstructFace(scene,MESH_ROCK,pObj,nIdx,4,4);

			p = p1 = pCrossPt;
			while (p != NULL)
			{
				p1 = p->next;
				delete p;
				p = p1;
			}
			delete [] pObj;
			delete [] pPts;
		}
	}
	scene.Add(pMesh);
}

void CMeshTunnel::ConstructRockMass(CSceneGraph3d &pSceneGraph, CMesh3d* pMesh,
									PPOINT_3F pObj1, int nNum1,
									PPOINT_3F pObj2, int nNum2,
									int nTextureIdx, int nCurSliceIdx)
{
	clock_wise(pObj1,nNum1);
	clock_wise(pObj2,nNum2);

	float MinLen, fMinTemp; 
	int MinNum;
	int i, j;

	PROPERTY pro1, pro2, proMean, prodiff;
	CalProperty(pObj1, nNum1, pro1, 1, 1);
	CalProperty(pObj2, nNum2, pro2, 1, 1);

	proMean.fArea = (pro1.fArea + pro2.fArea) / 2;
	proMean.ptMean.x = (pro1.ptMean.x + pro2.ptMean.x) / 2;
	proMean.ptMean.y = (pro1.ptMean.y + pro2.ptMean.y) / 2;

	prodiff.fArea = pro1.fArea - pro2.fArea;
	prodiff.ptMean.x = pro1.ptMean.x - pro2.ptMean.x;
	prodiff.ptMean.y = pro1.ptMean.y - pro2.ptMean.y;

//	prodiff.ptMean.x = prodiff.ptMean.y =0;

	if(pro2.fArea < 0.000001)
		return;

	if (fabs(pro2.right - pro2.left) < 0.000001 && fabs(pro2.bottom - pro2.top) < 0.000001) 
		return;

	float fscale = pro1.fArea / pro2.fArea;
	float fscalex = fabs(pro1.right - pro1.left) / fabs(pro2.right - pro2.left);
	float fscaley = fabs(pro1.bottom - pro1.top) / fabs(pro2.bottom - pro2.top);

	MinLen = FLT_MAX;
	MinNum = 0;

	for(i = 0;i<nNum2;i++)  //������һ���������̵ĵ㲢�����
	{
//		fMinTemp = fabs(pObj1[0].x-(pObj2[i].x + prodiff.ptMean.x)* fscalex) + fabs(pObj1[0].y-(pObj2[i].y + prodiff.ptMean.y)* fscaley);
		fMinTemp = fabs(pObj1[0].x - pro1.ptMean.x-(pObj2[i].x - pro2.ptMean.x) * fscalex) + fabs(pObj1[0].y - pro1.ptMean.x - (pObj2[i].y - pro2.ptMean.y)* fscaley);

		if(MinLen>fMinTemp)
		{
			MinLen=fMinTemp;
		  MinNum=i;
		}
	}

	int nVerNum = pMesh->NbVertex();
	CArray3d<CVertex3d> *pArrayVertex = pMesh->GetArrayVertex();//��ȡ�����ж������
	CArray3d<CFace3d> *pArrayFace = pMesh->GetArrayFace();//��ȡ����������Ƭ����
	
	for (i = 0; i < nNum1; ++i)//��Ӷ���
	{
		pArrayVertex->Add(new CVertex3d(pObj1[i].x, pObj1[i].y, pObj1[i].z));
	}

	for (i=0; i<nNum2; ++i)
	{
		pArrayVertex->Add(new CVertex3d(pObj2[(i + MinNum) % nNum2].x, pObj2[(i + MinNum) % nNum2].y, pObj2[(i + MinNum) % nNum2].z));
	}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//  ����ߵĻ���
	CVertex3d** pOne = new CVertex3d*[nNum1];
	CVertex3d** pTwo = new CVertex3d*[nNum2];
	CVertex3d** pV;

	pV = pArrayVertex->GetData();
	for(i = 0;i<nNum1;i++)
		pOne[i] = pV[nVerNum + i];     //����ǰnNum1��������һ����Ƭ
	for(j = 0;j<nNum2;j++)   //�Ѻ�nNum1��nNum1+nNum2�����ڶ�����Ƭ
        pTwo[j] = pV[nVerNum + j + nNum1]; 

	int nIdx1 = 0,nIdx2 = 0;
	CVertex3d* pStart1 = pOne[0];
	CVertex3d* pStart2 = pTwo[0];
	CVertex3d* p1 = pStart1;
	CVertex3d* p2 = pStart2;
	CVertex3d* p3 = pOne[++nIdx1];
	CVertex3d* p4 = pTwo[++nIdx2];
	float fCX, fCY, fCZ; //�е�����
	float fLen1, fLen2;
    i = pMesh->NbFace();   //���i��������Ϊ0
	int nNextWeight = 0;
	float fWeight = 0.5;
	int nLocalFace = 0;
	while(1)
    {
		fCZ = (p1->z() + p2->z()) / 2.0;
		fCX = ((p1->x() - pro1.ptMean.x) + (p2->x() - pro2.ptMean.x)* fscalex) / 2.0;
		fCY = ((p1->y() - pro1.ptMean.y) + (p2->y() - pro2.ptMean.y)* fscalex) / 2.0;

		fLen1 = fabs(fCX - (p3->x() - pro1.ptMean.x)) + fabs(fCY - (p3->y() - pro1.ptMean.y));
		fLen2 = fabs(fCX - (p4->x() - pro2.ptMean.x) * fscalex) + fabs(fCY - (p4->y() - pro2.ptMean.y) * fscaley);

		if (nNextWeight == 2)
			 fLen1 *= fWeight;

		if (nNextWeight == 1)
			 fLen2 *= fWeight;

		CFace3d* pFace = new CFace3d;

		if(fLen1 < fLen2)
		{
			nNextWeight = 1;
//			pFace->Set(p1, p3, p2);
			p1 = p3;
			p3 = pOne[(++nIdx1)%nNum1];
		}
		else
		{
			nNextWeight = 2;
			pFace->Set(p1, p4, p2);
			p2 = p4;
			p4 = pTwo[(++nIdx2)%nNum2];
		}

//		pArrayFace->SetAtGrow(i++,pFace);
		++nLocalFace;

		if(DWORD(p1) == DWORD(pStart1) && DWORD(p2) == DWORD(pStart2))
			break;

		if(nLocalFace >= 3 * (nNum1 + nNum2))
			break;
	}
	delete [] pOne;
	delete [] pTwo;

// ����߻��ƽ���
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//

	//���ǻ��ϱ��棬�γ�һ����άƽ�棬�������Խ���������ͼ
	{
		CMesh3d* pMesh1 = new CMesh3d;//�����������
		pMesh1->SetMeshType(MESH_FACE);
		pSceneGraph.Add(pMesh1);//�����������ӵ�����

//////////////////////////////////////////////////////////////////////////
		//����ê��ƫ��,������������
//		
//		pMesh->SetLockOffset(x[0] / nWidth, y[0] / nHeight);
//////////////////////////////////////////////////////////////////////////

		pMesh1->SetNbVertex(nNum1);//���������ж������
		pMesh1->SetNbFace(0);//��������������Ƭ����

		CArray3d<CVertex3d> *pArrayVertex = pMesh1->GetArrayVertex();//��ȡ�����ж������
		CArray3d<CFace3d> *pArrayFace = pMesh1->GetArrayFace();//��ȡ����������Ƭ����

		for (i = 0; i < nNum1; ++i)//��Ӷ���
		{
			pArrayVertex->SetAt(i,new CVertex3d(pObj1[i].x, pObj1[i].y, pObj1[i].z));
		}

		pMesh1->SetTextureIndex(nTextureIdx-1);

		CTriFace faceTop(pArrayFace, pArrayVertex, 0);
		Vector3F* pVC = new Vector3F [nNum1];
		for(i = 0; i < nNum1; ++i)
		{
			pVC[i][0] = pObj1[i].x;
			pVC[i][1] = pObj1[i].y;
			pVC[i][2] = pObj1[i].z;
		}

		Vector3F vcNorm(0,0,1);
		faceTop.Triangulate(pVC, vcNorm, nNum1);
		delete [] pVC;
 		pMesh1->Rebuild();//���㷨��(���ڶ���,ÿ������Ƭ,ÿ������)
	}

	/*
	//���ǻ��ϱ��棬�γ�һ����άƽ�棬�������Խ���������ͼ
	{
		CMesh3d* pMesh2 = new CMesh3d;//�����������
		pMesh2->SetMeshType(MESH_FACE);
		pSceneGraph.Add(pMesh2);//�����������ӵ�����

		pMesh2->SetNbVertex(nNum2);//���������ж������
		pMesh2->SetNbFace(0);//��������������Ƭ����
		//////////////////////////////////////////////////////////////////////////
		//����ê��ƫ��,������������
//		pMesh2->SetLockOffset(x[0] / nWidth, y[0] / nHeight);

		CArray3d<CVertex3d> *pArrayVertex = pMesh2->GetArrayVertex();//��ȡ�����ж������
		CArray3d<CFace3d> *pArrayFace = pMesh2->GetArrayFace();//��ȡ����������Ƭ����

		for (i = 0; i < nNum2; ++i)//��Ӷ���
		{
			pArrayVertex->SetAt(i,new CVertex3d(pObj2[i].x, pObj2[i].y, pObj2[i].z));
		}

		pMesh2->SetTextureIndex(nTextureIdx);

		CTriFace faceTop(pArrayFace, pArrayVertex, 0);
		Vector3F* pVC = new Vector3F [nNum2];
		for(i = 0; i < nNum2; ++i)
		{
			pVC[i][0] = pObj2[i].x;
			pVC[i][1] = pObj2[i].y;
			pVC[i][2] = pObj2[i].z;
		}

		Vector3F vcNorm(0,0,1);
		faceTop.Triangulate(pVC, vcNorm, nNum2);
		delete [] pVC;
 		pMesh2->Rebuild();//���㷨��(���ڶ���,ÿ������Ƭ,ÿ������)
	}
	*/
	pMesh->Rebuild();//���㷨��(���ڶ���,ÿ������Ƭ,ÿ������)

}

// ������ǰ���ڱ�ǽ�еĻ������λ��
float CMeshTunnel::get_ratio(PPOINT_3F pObj, int nNum, POINT_3F pt)
{
	if (pt==pObj[0])
		return 0;

	float fSum, fLen;
	fSum = fLen = 0;
	for (int i=1; i<nNum; i++)
	{
		fSum += get_distance(pObj[i-1],pObj[i]);
		if (pt==pObj[i])
			fLen = fSum;
	}

	return fLen/fSum;
}

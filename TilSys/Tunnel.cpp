// Tunnel.cpp: implementation of the CTunnel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tilsys.h"
#include "Tunnel.h"
#include "TilComFunction.h"
#include "triangle.h"
#include "SpaceRelation.h"
#include "MathComput.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define MESH_SCALE 0.000641
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTunnel::CTunnel()
{

}

CTunnel::~CTunnel()
{
	clear_mem();
}

void CTunnel::clear_mem()
{
	for (int i=0; i<m_vFaces.size(); i++)
	{
		for (int j=0; j<m_vFaces[i]->nLineNum; j++)
		{
			delete [] (char*)(m_vFaces[i]->line[j]);
		}
		delete [] m_vFaces[i]->line;
		delete [] m_vFaces[i];
	}
	m_vFaces.clear();
}

// 导入并设置掌子面尺寸
void CTunnel::ImportData(CTilCanvasMgr* pMgr)
{
	clear_mem();

	int i,j,k;

	int nCanvasNum = pMgr->Size();

	for (i=0; i<nCanvasNum; i++)
	{
		CTilCanvas* pCanvas = pMgr->GetCanvasByIdx(i);
		int nWidth = pCanvas->GetWidth();
		CGraph* pGraphA = pCanvas->GetGraph()->GetGraphByIdx(GRAPH_LAYER_TUNARC);
		// 如果没有设置隧道开挖轮廓线，则直接不处理该掌子面
		if(pGraphA->Size()==0)
			continue;

		CGraph* pGraphR = pCanvas->GetGraph()->GetGraphByIdx(GRAPH_LAYER_ROCKS);

		// 新的掌子面信息
		PFACE pFace = new FACE;
		pFace->lDK = get_std_mileage(pCanvas->GetGraph()->GetPathName());
		pFace->nLineNum = pGraphA->Size()+pGraphR->Size();
		pFace->line = new PLINE [pGraphA->Size()+pGraphR->Size()];
		m_vFaces.push_back(pFace);

		// 第1条边界线为隧道开挖轮廓线
		PPLINE ppLine = pFace->line;
		int nPtNum = pGraphA->GetUnitByIdx(0)->GetPointNum();
		ppLine[0] = (PLINE)new char [sizeof(MESH_TYPE)+sizeof(int)*3+sizeof(POINT_3F)*nPtNum];
		ppLine[0]->type = MESH_ARC;
		ppLine[0]->nPtNum = nPtNum;
		for (j=0; j<nPtNum; j++)
		{
			// 取得第0个目标即隧道断面轮廓层目标上的点
			POINT2DF pt	= pGraphA->GetUnitByIdx(0)->GetPoint(j);

			// 设置以中心点为中心的各点坐标
			ppLine[0]->pt[j].x = pt.x/nWidth;
			ppLine[0]->pt[j].y = pt.y/nWidth;
			ppLine[0]->pt[j].z = (pFace->lDK - m_vFaces[0]->lDK)*100*MESH_SCALE;
		}
		clock_wise(ppLine[0]->pt,nPtNum);

		// 后面的在岩层目标层上的边界线
		for (j=0; j<pGraphR->Size(); j++)
		{
			int nPtNum = pGraphR->GetUnitByIdx(j)->GetPointNum();
			ppLine[j+1] = (PLINE)new char [sizeof(MESH_TYPE)+sizeof(int)*3+sizeof(POINT_3F)*nPtNum];

			// 若为封闭曲线，则为岩体，否则为结构面
			if (pGraphR->GetUnitByIdx(j)->IsPolyClose())
				ppLine[j+1]->type = MESH_ROCK;
			else
				ppLine[j+1]->type = MESH_STRUCT;

			ppLine[j+1]->nPreLnk = pGraphR->GetUnitByIdx(j)->GetPreLink();
			ppLine[j+1]->nNxtLnk = pGraphR->GetUnitByIdx(j)->GetNextLink();
			ppLine[j+1]->nPtNum = nPtNum;

			for (k=0; k<nPtNum; k++)
			{
				// 取得第j个边界目标的第k个点
				POINT2DF pt = pGraphR->GetUnitByIdx(j)->GetPoint(k);

				// 设置以中心点为中心的各点坐标
				ppLine[j+1]->pt[k].x = pt.x/nWidth;
				ppLine[j+1]->pt[k].y = pt.y/nWidth;
				ppLine[j+1]->pt[k].z = (pFace->lDK - m_vFaces[0]->lDK)*100*MESH_SCALE;
			}

			if (ppLine[j+1]->type == MESH_ROCK)
				clock_wise(ppLine[j+1]->pt, nPtNum);
//			else
		}
	}

}

/* ==================================================================
 * 函数名称：  CTunnel::CreateTunArc()
 * 作者：      [Bill]
 * 创建于:     [2011-12-22 13:34:15]
 * 参数列表： 
               [CSceneGraph3d& scene] - [In] 场景
               [float fPreDist] - [In] 预测距离
注释 :	创建隧道拱形
				
==================================================================*/
void CTunnel::CreateTunArc(CSceneGraph3d& scene,float fPreDist)
{
	// 创建网络并将网格对象添加到场景
	CMesh3d* pMesh = new CMesh3d;
	pMesh->SetMeshType(MESH_ARC);
	scene.Add(pMesh);

	// 设置网格中顶点个数和三角片个数
	pMesh->SetNbVertex(0);
	pMesh->SetNbFace(0);

	// 取得隧道中心点的图像坐标作为三维坐标原点
	POINT_3F *pPt1, *pPt2;
	int nNum1, nNum2;

	nNum1 = m_vFaces[0]->line[0]->nPtNum;
	pPt1 = m_vFaces[0]->line[0]->pt;

	int nFaceNum = m_vFaces.size();
	for (int i=1; i<nFaceNum; i++)
	{
		nNum2 = m_vFaces[i]->line[0]->nPtNum;
		pPt2 = m_vFaces[i]->line[0]->pt;

		MeshRock(pMesh, MESH_ARC, pPt1, nNum1, pPt2, nNum2, i, i);

		pPt1 = pPt2;
		nNum1 = nNum2;
	}

	// 至少要大于 0.5米才预测
// 	if (fPreDist > 0.5)
// 	{
// 		PPOINT_3F pPtNext = new POINT_3F [nNum2];
// 		memcpy(pPtNext, pPt2, sizeof(POINT_3F)*nNum2);
// 		for (i=0; i<nNum2; i++)
// 			pPtNext[i].y += fPreDist;
// 
// 		MeshRock(pMesh, MESH_ARC, pPt2, nNum2, pPtNext, nNum2, i, i);
// 	}
// 	else if (fPreDist < -0.5)
// 	{
// 		pPt1 = m_vFaces[0]->line[0]->pt;
// 		PPOINT_3F pPtPrev = new POINT_3F [nNum1];
// 		memcpy(pPtPrev, pPt1, sizeof(POINT_3F)*nNum1);
// 		for (i=0; i<nNum2; i++)
// 			pPtPrev[i].y += fPreDist;
// 		
// 		MeshRock(pMesh, MESH_ARC, pPtPrev, nNum1, pPtPrev, nNum1, 0, 0);
// 	}

	pMesh->Rebuild();
}

// 创建隧道掌子面
void CTunnel::CreateTunFace(CSceneGraph3d& scene)
{
	int i,j;

	int nFaceNum = m_vFaces.size();
	for (i=0; i<nFaceNum; i++)
	{
		CMesh3d* pMesh = new CMesh3d;//创建网格对象
		pMesh->SetMeshType(MESH_FACE);
		scene.Add(pMesh);//将网格对象添加到场景

		int nPtNum = m_vFaces[i]->line[0]->nPtNum;
		pMesh->SetNbVertex(nPtNum);//设置网格中顶点个数
		pMesh->SetNbFace(0);//设置网格中三角片个数
		pMesh->SetTextureIndex(i);//设置锚点偏移,调整纹理坐标

		CArray3d<CVertex3d> *pArrayVertex = pMesh->GetArrayVertex();// 添加网络顶点
		CArray3d<CFace3d> *pArrayFace = pMesh->GetArrayFace();// 提取网格中三角片

		POINT_3F* pPt = m_vFaces[i]->line[0]->pt;
		for (j=0; j<nPtNum; ++j)
		{
			pArrayVertex->SetAt(j,new CVertex3d(pPt[j].x, pPt[j].y, pPt[j].z));
		}

		CTriFace faceTop(pArrayFace, pArrayVertex, 0);
		Vector3F* pVC = new Vector3F [nPtNum];
		for (j=0; j<nPtNum; ++j)
		{
			pVC[j][0] = pPt[j].x;
			pVC[j][1] = pPt[j].y;
			pVC[j][2] = pPt[j].z;
		}
		Vector3F vcNorm(0,0,1);
		faceTop.Triangulate(pVC, vcNorm, nPtNum);
		delete [] pVC;

		pMesh->Rebuild();
	}
}

/* ==================================================================
 * 函数名称：  CTunnel::CreateTunStruct()
 * 作者：      [Bill]
 * 创建于:     [2011-12-22 13:30:16]
 * 参数列表： 
               [CSceneGraph3d& scene] - [In]	场景
               [float fPreDist] - [In] 预测距离
注释 :	创建隧道内的结构面	
==================================================================*/
void CTunnel::CreateTunStruct(CSceneGraph3d& scene,float fPreDist)
{
	int i,j;
	int nPtNum1, nPtNum2;
	POINT_3F *pPt1, *pPt2;

	int nFaceNum = m_vFaces.size();
	for (i=0; i<nFaceNum; i++)
	{
		int nLineNum = m_vFaces[i]->nLineNum;
		PPLINE ppLine = m_vFaces[i]->line;
		for (j=1; j<nLineNum; j++)
		{
			if (ppLine[j]->nPreLnk!=-1 || ppLine[j]->nNxtLnk==-1)
				continue;

			// 创建网格对象并将其添加到场景
			CMesh3d* pMesh = new CMesh3d;
			scene.Add(pMesh);
			pMesh->SetTextureIndex(i);

			// 设置网格中顶点个数和三角片个数
			pMesh->SetMeshID(i, j);
			pMesh->SetNbVertex(0);
			pMesh->SetNbFace(0);

			int nFaceIdx = i;		//切片号

			// 为前后两个对应的目标创建连接
			int nLineIdx = ppLine[j]->nNxtLnk;
			if (nLineIdx != -1)
			{
				nPtNum1 = ppLine[j]->nPtNum;
				pPt1 = ppLine[j]->pt;
			}

			while (nLineIdx != -1)
			{
				// 得到下一画布被连接的目标的顶点数
				PPLINE ppLine2 = m_vFaces[nFaceIdx+1]->line;
				nPtNum2 = ppLine2[nLineIdx+1]->nPtNum;
				pPt2 = ppLine2[nLineIdx+1]->pt;

				if (ppLine2[nLineIdx+1]->type == MESH_STRUCT)
				{
					pMesh->SetMeshType(MESH_STRUCT);
					MeshStruct(pMesh, pPt1, nPtNum1, pPt2, nPtNum2, nFaceIdx, nFaceIdx);
				}
				else
					MeshRock(pMesh, MESH_ROCK, pPt1, nPtNum1, pPt2, nPtNum2, nFaceIdx, nFaceIdx);

				nPtNum1 = nPtNum2;
				pPt1 = pPt2;

				nFaceIdx ++;
				nLineIdx = ppLine2[nLineIdx+1]->nNxtLnk;
			}
		}
	}
}

// 创建虚拟地质钻孔
void CTunnel::CreateVirGeoDrill(CSceneGraph3d& scene, POINT2DF ptCenter, float fRadius)
{
	/*
	int i,j,k,m,nIdx;
	int nMeshNum, nFaceNum, nVerNum;
	PPOINT_3F pObj = NULL;

	// 参数转化
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
			for (k=0; k<nVerNum; k++)
			{
				CVertex3d* pVertex = pFace->v(k);
				pPts[k].x =pVertex->x();
				pPts[k].y =pVertex->y();
			}

			clock_wise(pPts, nVerNum);

			// 求多边形与圆的交集
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
				}
				// 如果没有交集,则返回继续循环
				else
				{
					delete [] pPts;
					continue;
				}
			}

			// 求得面片所在平面的方程
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

			// 根据求得的边界点建立取得三维坐标
			PCROSSPOINT p, p1;
			p = pCrossPt;
			while (p != NULL)
			{
				p->z = -(fFace[0]*p->x+fFace[1]*p->y+fFace[3])/fFace[2];
				p = p->next;
			}

			// 将坐标转化成顶点进行存储
			// 将圆面边界转化成顶点
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
						// 如果是一条线段
						if (p->nS==0 || p->nS==2)
						{
							pObj[nIdx].x = p->x;
							pObj[nIdx].y = p->y;
							pObj[nIdx].z = p->z;
							nIdx ++;
						}
						// 如果是一段弧,则将弧分段后保存
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

				// 分析最后一点与第一点之间的边线
				// 如果是线段
				if (p->nS==0 || p->nS==2)
				{
					pObj[nIdx].x = p->x;
					pObj[nIdx].y = p->y;
					pObj[nIdx].z = p->z;
					nIdx ++;
				}
				// 如果是圆弧,则将圆弧分段后保存
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
	*/
}

// 创建三维岩体
void CTunnel::MeshRock(
CMesh3d* pMesh, MESH_TYPE type,
PPOINT_3F pObj1, int nNum1, // 第一个多边形
PPOINT_3F pObj2, int nNum2,	// 第二个多边形
int nTextureIdx, int nCanvasIdx)
{
	int nVerNum = pMesh->NbVertex();
	CArray3d<CVertex3d> *pArrayVertex = pMesh->GetArrayVertex();
	CArray3d<CFace3d> *pArrayFace = pMesh->GetArrayFace();
	pMesh->SetMeshType(type);

	//设置锚点偏移,调整纹理坐标
	CTilCanvasMgr* pMgr = GetCanvasMgr();
	CTilTunArc* pTunArc = pMgr->GetCanvasByIdx(nCanvasIdx)->GetGraph()->GetTunArc();
	POINT2DF ptImgCenter = pTunArc->GetImgCenter();

	int i,j;
	int* pNum21;
	PPPOINT_3F ppLink21;

	// 建立前后两个岩层目标的连接关系
	clock_wise(pObj1,nNum1);
	clock_wise(pObj2,nNum2);
	link_faces(pObj1, nNum1, pObj2, nNum2, &ppLink21, &pNum21);

	// 添加顶点
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

	// 设置三角面片
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
			pV3 = ppVertex2[0];
			CFace3d* pFace = new CFace3d(pV1, pV2, pV3);
			pArrayFace->SetAtGrow(nIdx++,pFace);
		}
	}

	for (i=0; i<nNum2; i++)
		delete [] ppLink21[i];
	delete [] ppLink21;
	delete [] pNum21;
}

// 创建结构面
void CTunnel::MeshStruct(
CMesh3d* pMesh, 
PPOINT_3F pObj1, int nNum1, // 第一个多段线
PPOINT_3F pObj2, int nNum2,	// 第二个多段线
int nTextureIdx, int nCanvasIdx)
{
	int nVerNum = pMesh->NbVertex();
	CArray3d<CVertex3d> *pArrayVertex = pMesh->GetArrayVertex();
	CArray3d<CFace3d> *pArrayFace = pMesh->GetArrayFace();

	//设置锚点偏移,调整纹理坐标
	CTilCanvasMgr* pMgr = GetCanvasMgr();
	CTilTunArc* pTunArc = pMgr->GetCanvasByIdx(nCanvasIdx)->GetGraph()->GetTunArc();
	POINT2DF ptImgCenter = pTunArc->GetImgCenter();

	int i,j;
	int* pNum21;
	PPPOINT_3F ppLink21;

	// 建立前后两个岩层目标的连接关系
//	clock_wise(pObj1,nNum1);
//	clock_wise(pObj2,nNum2);
	link_line_segments(pObj1, nNum1, pObj2, nNum2, &ppLink21, &pNum21);

	// 添加顶点
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

	// 设置三角面片
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
	}

	for (i=0; i<nNum2; i++)
		delete [] ppLink21[i];
	delete [] ppLink21;
	delete [] pNum21;
}


// 将两个不同面上的多边形的顶点对应连接起来
// PPOINT_3F pPolygon1, int nNum1, : 第一个掌子面顶点坐标及顶点数
// PPOINT_3F pPolygon2, int nNum2, : 第二个掌子面顶点坐标及顶点数
// PPPOINT_3F ppLink21, int* pNum21 : 第一、二个掌子面的对应点连接
// 注意: ppPolygon二级指针分配了内存，里面的一级指针没有分配内存
void CTunnel::link_faces(PPOINT_3F pPolygon1, int nNum1,
							PPOINT_3F pPolygon2, int nNum2,
							POINT_3F*** pppLink21, int** ppNum21)
{
	// 全部转化成顺时针方向
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

  // 求出离第一个点距离最短的点并标出来
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

		// 线段1-2的中点距3,4的距离
		fLen2 = fabs(fCX - (pPolygon2[(nIdx2+1)%nNum2].x - pro2.ptMean.x))
					+ fabs(fCY - (pPolygon2[(nIdx2+1)%nNum2].y - pro2.ptMean.y));
		fLen1 = fabs(fCX - (pPolygon1[(nIdx1+1)%nNum1].x - pro1.ptMean.x) * fscalex)
					+ fabs(fCY - (pPolygon1[(nIdx1+1)%nNum1].y - pro1.ptMean.y) * fscaley);

		if (nNextWeight == 2)			 fLen1 *= fWeight;
		if (nNextWeight == 1)			 fLen2 *= fWeight;

		// 如果中点距3更近
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

		// 三角面片数量增加1
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
				// 将后续点拷贝到第一点的存储位置
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
		// 如果下一个计算的当前12层点对是第一次计算的12层点对
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

// 将两个不同面上的多段线的顶点对应连接起来
void CTunnel::link_line_segments(PPOINT_3F pPolygon1, int nNum1,
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

		// 线段1-2的中点距3,4的距离
		fLen2 = fabs(fCX - (pPolygon2[nIdx2+1].x - pro2.ptMean.x))
			  + fabs(fCY - (pPolygon2[nIdx2+1].y - pro2.ptMean.y));
		fLen1 = fabs(fCX - (pPolygon1[nIdx1+1].x - pro1.ptMean.x) * fscalex)
			  + fabs(fCY - (pPolygon1[nIdx1+1].y - pro1.ptMean.y) * fscaley);

		if (nNextWeight == 2)			 fLen1 *= fWeight;
		if (nNextWeight == 1)			 fLen2 *= fWeight;

		// 如果中点距3更近
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

		// 如果第一条多段线到头了
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
		// 如果第二条多段线到头了
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

// 根据两个掌子面预测下一个掌子面
// PPPOINT_3F ppLinkFace12, int nNum12 : 两个掌子面的对应点连接
// double* pFace3, : 预测面
// PPOINT_3F pPolygon3, int& nNum3: 输出预测面上的多这形
void CTunnel::predict_outer_face(PPPOINT_3F ppLink21, int* pNum21, int nNum2,
									float* pFace3, PPPOINT_3F ppPolygon3, int& nNum)
{
	POINT_3F point;
	float fFaceA[4], fFaceB[4];

	nNum = 0;
	*ppPolygon3 = new POINT_3F [nNum2];
	for (int i = 2; i < nNum2; i ++)
	{
		// 当为结构面时取得直线与预测面的交点
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

// 预测两个掌子面之间的另一掌子面
// PPPOINT_3F ppLink13, int nNum13 : 两个掌子面的对应点连接
// double* pFace2, : 预测面
// PPOINT_3F pPolygon2, int& nNum2: 输出预测面上的多这形*/
void CTunnel::predict_inner_face(PPPOINT_3F ppLink31, int* pNum31, int nNum3,
								float* pFace2, PPPOINT_3F ppPolygon2, int& nNum2)
{
	int i, j, nIdx;
	POINT_3F point;

	nNum2 = 0;
	for (i = 0; i < nNum3; i ++)
		nNum2 += pNum31[i]-1;

	// 如果最后一个点是第一

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

// 预测三个掌子面之后的下一个掌子面
// PPPOINT_3F ppLinkFace12, int nNum1, : 一、二个面之间的点的连接
// PPPOINT_3F ppLinkFace23, int nNum2, : 二、三个面之间的点的连接
// double* pFace4,	: 
// PPOINT_3F pPolygon4, int& nNum
void CTunnel::predict_inner_face(PPPOINT_3F ppLink21, int* pNum21, int nNum2,
									PPPOINT_3F ppLink32, int* pNum32, int nNum3,
									double* pFace4, 
									PPPOINT_3F ppPolygon4, int& nNum4)
{
}

// 预测两个掌子面之间的另一纵剖面
// PPPOINT_3F ppLink31, int* pNum31, int nNum3 : 两个掌子面的对应点连接
// double* pFace2 : 预测纵剖面
// PPOINT_3F pPolygon2, int& nNum2: 输出预测面上的多这形
void CTunnel::predict_profile_face(PPPOINT_3F ppLink31, int* pNum31, int nNum3,
									float* pFace2, deque< deque<POINT_3F> >& ddPts31)
{
	ASSERT(nNum3>0);

	int i, j;
	BOOL bIsClosed = FALSE;

	// 分配空间存储平面与各三角面片的交点
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

	// 计算第3条边上各线段所在三角面片与平面的交点
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

	// 如果是封闭曲线，则计算第一点和最后一点间的线段所在三角面片与平面的交点
	// 若下面的等式成立，则说明是封闭曲线
	if (ppLink31[0][1] == ppLink31[nNum3-1][ pNum31[nNum3-1]-1 ])
	{
		bIsClosed = TRUE;
		pt[0] = ppLink31[nNum3-1][0];
		pt[1] = ppLink31[0][1];
		pt[2] = ppLink31[0][0];
		get_faces_intersections(pFace2, pt, 3, &pPt, nNum);
		// 如果有2个交点，即有交线段，才保存
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

	// 将所有的线段合并在一起形成剖面线
	merge_line_segments(ddPts31);

	// 将所有剖面线第1点设置为在第3边界线上的点
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
		// 如果是封闭曲线,则分析第1点与最后一点间的线段
		if (bIsClosed)
		{
			pt1 = ppLink31[0][0];
			pt2 = ppLink31[nNum3-1][0];
			if (point_in_line(pt, pt1, pt2)==POINT_IN_LINE)
			{
				goto step;
			}
		}

		// 如果剖面线第1点没有在第3边界线上,则将剖面线中点的存储顺序反向
		reverse(ddPts31[i].begin(), ddPts31[i].end());
step:
		continue;
	}
}

// 将具有相同端点的线段或多段线合并
void CTunnel::merge_line_segments(deque< deque<POINT_3F> >& ddPts)
{
	// 将所有的线段合并在一起形成剖面线
	int nLines, nLines1;
	while (1)
	{
		nLines1 = ddPts.size();
		nLines = nLines1;
		for (int i=0; i<nLines; i++)
		{
			for (int j=nLines-1; j>i; j--)
			{
				// 如果某端点相同，则将前面端点反向后压入队头
				if (ddPts[i][0]==ddPts[j][0])
				{
					reverse(ddPts[j].begin(), ddPts[j].end());
					ddPts[i].insert(ddPts[i].begin(), ddPts[j].begin(), ddPts[j].end()-1);
					ddPts.erase(ddPts.begin()+j);
				}
				// 如果与另一端点相同，则将前面端点压入队头
				else if (ddPts[i][0]==ddPts[j][1])
				{
					ddPts[i].insert(ddPts[i].begin(), ddPts[j].begin(), ddPts[j].end()-1);
					ddPts.erase(ddPts.begin()+j);
				}
				// 如果另一端点与第1个端点相同，则在队尾压入后继端点
				else if (ddPts[i][ddPts[i].size()-1]==ddPts[j][0])
				{
					ddPts[i].insert(ddPts[i].end(), ddPts[j].begin()+1, ddPts[j].end());
					ddPts.erase(ddPts.begin()+j);
				}
				// 如果另一端点与第2个端点相同，则在队尾压入第1点
				else if (ddPts[i][ddPts[i].size()-1]==ddPts[j][1])
				{
					reverse(ddPts[j].begin(), ddPts[j].end());
					ddPts[i].insert(ddPts[i].end(), ddPts[j].begin()+1, ddPts[j].end());
					ddPts.erase(ddPts.begin()+j);
				}
			}
			nLines = ddPts.size();
		}

		// 如果下式相等，则说明所有剖面线合并完毕
		if (nLines1==nLines)
			break;
	}
}

float CTunnel::clock_wise(PPOINT2DF pPts, int nNum)
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
		// 反向
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

// 将平面中的多段线上的关键点按顺时针存储
// 注意：此处默认z坐标相同
float CTunnel::clock_wise(PPOINT_3F pPts, int nNum)
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
		// 反向
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

// 将平面中的多段线上的关键点按先上后下、先左后右的顺序存储
bool CTunnel::sequence(PPOINT_3F pPts, int nNum)
{
	// 将前面一半的重心与后面一半的重心相比较
	int i;
	POINT_3F ptCenter, ptCenter1, ptCenter2;
	ptCenter1.x = ptCenter1.y = ptCenter1.z = 0.0;
	ptCenter2.x = ptCenter2.y = ptCenter2.z = 0.0;

	for (i=0; i<nNum/2; i++)
	{
		ptCenter1.x += pPts[i].x;
		ptCenter1.y += pPts[i].y;
		ptCenter1.z += pPts[i].z;
	}
	ptCenter1.x /= nNum/2;
	ptCenter1.y /= nNum/2;
	ptCenter1.z /= nNum/2;


	for (i=nNum/2; i<nNum; i++)
	{
		ptCenter2.x += pPts[i].x;
		ptCenter2.y += pPts[i].y;
		ptCenter2.z += pPts[i].z;
	}
	ptCenter2.x /= nNum-nNum/2;
	ptCenter2.y /= nNum-nNum/2;
	ptCenter2.z /= nNum-nNum/2;

	ptCenter = ptCenter1-ptCenter2;

	int nIdx;
	if (fabs(ptCenter.x) < fabs(ptCenter.y))
	{
		if (fabs(ptCenter.y) < fabs(ptCenter.z))
			nIdx = 3;
		else
			nIdx = 2;
	}
	else
	{
		if (fabs(ptCenter.x) < fabs(ptCenter.z))
			nIdx = 3;
		else
			nIdx = 1;
	}

	return TRUE;
}

// 将圆按角度平分，计算每条平分点的坐标，并依次存储
void CTunnel::divide_circle(float fRadius, int nNum, PPOINT2DF& pPts)
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

void CTunnel::divide_circle(POINT2DF ptCenter, float fRadius, int nNum, PPOINT2DF& pPts)
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

// 分割圆弧，将圆弧以弧上平均分割的端点连接表示
// 沿顺时针方向,两个端点均有存储
void CTunnel::divide_arc(const POINT2DF& pt1, const POINT2DF& pt2,
							 const POINT2DF& ptCenter, PPOINT2DF& pPts, int& nNum)
{
	float fRadius = sqrt(pt1.x*pt1.x+pt1.y*pt1.y);
	float fAngle = get_angle(pt1,ptCenter,pt2);
	float fArea = 0;
	fArea += pt1.x*pt2.y-pt1.y*pt2.x;
	fArea += pt2.x*ptCenter.y-pt2.y*ptCenter.x;
	fArea += ptCenter.x*pt1.y-ptCenter.y*pt1.x;

	// 按面积分析逆时针和顺时针情况
	if (fArea<0)
	{
		fAngle = 2*PI-fAngle;
	}

	// 段数及平均角度
	nNum = fAngle/AVE_ANGLE+0.5;
	nNum = (nNum<2)?2:nNum;
	float fAveAngle = fAngle/nNum;

	// 计算旋转fAveAngle角度后的点
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

// MeshTunnel.h: interface for the CMeshTunnel class.
//
//////////////////////////////////////////////////////////////////////

/************************************
  REVISION LOG ENTRY
  维护人员: Leng
  维护时间 2009-4-2 15:05:53
  说明: 创建和管理隧道三维地质结构
 ************************************/


#if !defined(AFX_MESHTUNNEL_H__927C7EEB_D086_47F4_87FE_880FA1A14D3E__INCLUDED_)
#define AFX_MESHTUNNEL_H__927C7EEB_D086_47F4_87FE_880FA1A14D3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SceneGraph3d.h"
#include "Mesh3d.h"
class CMeshTunnel  
{
public:
	CMeshTunnel();
	virtual ~CMeshTunnel();

	// 设置隧道显示的里程范围
	void SetRangeDK(const long& lStartDK, const long& lEndDK);
	// 取得隧道显示的里程范围
	void GetRangeDK(long& lStartDK, long& lEndDK);
	// 设置中心点里程
	void SetCenterDK(const long& lCenterDK) {m_lCenterDK = lCenterDK;};
	// 取得中心点里程
	long GetCenterDK() { return m_lCenterDK;};

	// 导入并设置掌子面尺寸
	void ImportData();
	// 判断是否所有设定的里程范围内的隧道断面图形都已输入
	BOOL IsTunArcParaInputed();
	// 创建隧道拱形面片
	void CreateTunArc(CSceneGraph3d& scene);
	// 创建隧道掌子面(09.06.07)
	void CreateTunFace(CSceneGraph3d& scene);
	// 创建隧道结构面
	void CreateStructures(CSceneGraph3d& scene);
	// 截取横截面剖面图形
	BOOL NormalSection(long lDK, CTilGraph& graphMgr);
	// 截取纵剖面图形
	BOOL Profile(long& lStartDK, long& lEndDK, CTilGraph& graphMgr);
	// 预测里程lDK的掌子面图形
	BOOL PreNormalSection(long lDK);

private:
	long m_lStartDK; // 起始里程
	long m_lEndDK;	 // 终止里程
	long m_lCenterDK;// 中心点里程

protected:
	
	// 创建单个三维岩体侧面结构面面形
	void ConstructSidePlaneA(CSceneGraph3d &scene,
		CMesh3d* pMesh, MESH_TYPE type,
		PPOINT_3F pObj1, int nNum1,
		PPOINT_3F pObj2, int nNum2,
		int nTextureIdx, int nCanvasIdx);
	// 创建单个三维岩体在掌子面上的岩层剖面面形
	void ConstructFace(CSceneGraph3d& scene, MESH_TYPE type, 
		PPOINT_3F pObj, int nNum,
		int nTextureIdx, int nCanvasIdx);
	// 创建岩体图形
	void ConstructRockMass(CSceneGraph3d &pSceneGraph, CMesh3d* pMesh,
		PPOINT_3F pObj1, int nNum1,
		PPOINT_3F pObj2, int nNum2,
		int nTextureIdx, int nCurSliceIdx);

private:
	// 将两个不同面上的多边形的顶点对应连接起来
	void link_faces(PPOINT_3F pPolygon1, int nNum1,
					PPOINT_3F pPolygon2, int nNum2,
					POINT_3F*** pppLinkFace21, int** ppNum21);
	// 将两个不同面上的多段线的顶点对应连接起来
	void link_line_segments(PPOINT_3F pLineSegs1, int nNum1,
							PPOINT_3F pLineSegs2, int nNum2,
							POINT_3F*** pppLinkSeg21, int** pNum21);
	// 预测下一个掌子面
	void predict_outer_face(PPPOINT_3F ppLink21, int* pNum21, int nNum2,
							float* pFace3,
							PPPOINT_3F ppPolygon3, int& nNum);
	// 预测两个掌子面之间的另一横剖面掌子面
	void predict_inner_face(PPPOINT_3F ppLink31, int* pNum31, int nNum3,
							float* pFace2, 
							PPPOINT_3F ppPolygon2, int& nNum2);
	// 预测三个掌子面之后的下一个掌子面
	void predict_outer_face(PPPOINT_3F ppLink21, int* pNum21, int nNum2,
							PPPOINT_3F ppLink32, int* pNum32, int nNum3,
							double* pFace4, 
							PPPOINT_3F ppPolygon4, int& nNum4);
	// 预测两个掌子面之间的另一纵剖面
	void predict_profile_face(PPPOINT_3F ppLink31, int* pNum31, int nNum3,
							float* pFace2, deque< deque<POINT_3F> >& ddPts31);
	// 将具有相同端点的线段或多段线合并
	void merge_line_segments(deque< deque<POINT_3F> >& ddPts);
	// 将平面中的多段线上的关键点按顺时针存储
	float clock_wise(PPOINT2DF pPts, int nNum);
	float clock_wise(PPOINT_3F pPts, int nNum);

//////////////////////////////////////////////////////////////////////////
// 虚拟地质钻孔
public:
	// 建立给定点间的对应关系，
	BOOL VirtualGeoDrill(CSceneGraph3d& scene, /*PPOINT_3F pPts, int nNum,*/ float fRadius);
	// 求虚拟钻孔与各结构面间的交集
	void VirtualGeoDrill(CSceneGraph3d& scene, POINT2DF ptCenter, float fRadius);
private:
	// 将圆按角度平分，计算每条平分点的坐标，并依次存储
	void divide_circle(float fRadius, int nNum, PPOINT2DF& pPts);
	void divide_circle(POINT2DF ptCenter, float fRadius, int nNum, PPOINT2DF& pPts);
	// 分割圆弧，将圆弧以弧上平均分割的端点连接表示
	void divide_arc(const POINT2DF& pt1, const POINT2DF& pt2, const POINT2DF& ptCenter, 
					PPOINT2DF& pPts, int& nNum);

private:
	// 用于隧道边墙及拱顶贴图
	// 分析当前点在边墙中的环向相对位置
	float get_ratio(PPOINT_3F pObj, int nNum, POINT_3F pt);
};

#endif // !defined(AFX_MESHTUNNEL_H__927C7EEB_D086_47F4_87FE_880FA1A14D3E__INCLUDED_)

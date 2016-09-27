// Tunnel.h: interface for the CTunnel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TUNNEL_H__A492D1E6_F080_4E24_A453_4CF5A51291C4__INCLUDED_)
#define AFX_TUNNEL_H__A492D1E6_F080_4E24_A453_4CF5A51291C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Mesh3d.h"
#include "SceneGraph3d.h"
#include "TilSys.h"

// 掌子面上的边界线
typedef struct __tagLINE
{
	MESH_TYPE type;	// 边界线类型,如岩体、地质钻孔、边墙及拱部、结构面、掌子面、预测面等
	int nPreLnk;
	int nNxtLnk;	// 前向链接、后向链接
	int nPtNum;		// 边界线的点数
	POINT_3F pt[1];	// 点界点坐标
} LINE, *PLINE, **PPLINE;

typedef struct __tagFACE
{
	long lDK;		// 掌子面里程
	int nLineNum;	// 掌子面上边界线数目
	PPLINE line;	// 边界线
} FACE, *PFACE, **PPFACE;

class CTunnel
{
public:
	CTunnel();
	virtual ~CTunnel();

	// 导入并设置掌子面尺寸
	void ImportData(CTilCanvasMgr* pMgr);

	// 创建隧道拱形
	void CreateTunArc(CSceneGraph3d& scene,float fPreDist=0);
	// 创建隧道掌子面
	void CreateTunFace(CSceneGraph3d& scene);
	// 创建隧道内的结构面
	void CreateTunStruct(CSceneGraph3d& scene,float fPreDist=0);
	// 创建虚拟地质钻孔
	void CreateVirGeoDrill(CSceneGraph3d& scene, POINT2DF ptCenter, float fRadius);

public:
	// 创建三维岩体
	void MeshRock(
	CMesh3d* pMesh, MESH_TYPE type,
	PPOINT_3F pObj1, int nNum1, // 第一个多边形
	PPOINT_3F pObj2, int nNum2,	// 第二个多边形
	int nTextureIdx, int nCanvasIdx);
	// 创建结构面
	void MeshStruct(
	CMesh3d* pMesh, 
	PPOINT_3F pObj1, int nNum1, // 第一个多段线
	PPOINT_3F pObj2, int nNum2,	// 第二个多段线
	int nTextureIdx, int nCanvasIdx);
	// 创建一个面
	void MeshFace(CMesh3d* pMesh,
		PPOINT_3F pObj, int nNum,
		int nTextureIdx, int nCanvasIdx);

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
	void predict_inner_face(PPPOINT_3F ppLink21, int* pNum21, int nNum2,
							PPPOINT_3F ppLink32, int* pNum32, int nNum3,
							double* pFace4, 
							PPPOINT_3F ppPolygon4, int& nNum4);
	// 预测两个掌子面之间的另一纵剖面
	void predict_profile_face(PPPOINT_3F ppLink31, int* pNum31, int nNum3,
							float* pFace2, deque< deque<POINT_3F> >& ddPts31);
	// 将具有相同端点的线段或多段线合并
	void merge_line_segments(deque< deque<POINT_3F> >& ddPts);
	// 将平面中的多边形上的关键点按顺时针存储
	float clock_wise(PPOINT2DF pPts, int nNum);
	float clock_wise(PPOINT_3F pPts, int nNum);
	// 将平面中的多段线上的关键点按先上后下、先左后右的顺序存储
	bool sequence(PPOINT_3F pPts, int nNum);

	// 将圆按角度平分，计算每条平分点的坐标，并依次存储
	void divide_circle(float fRadius, int nNum, PPOINT2DF& pPts);
	void divide_circle(POINT2DF ptCenter, float fRadius, int nNum, PPOINT2DF& pPts);
	// 分割圆弧，将圆弧以弧上平均分割的端点连接表示
	void divide_arc(const POINT2DF& pt1, const POINT2DF& pt2, const POINT2DF& ptCenter, 
					PPOINT2DF& pPts, int& nNum);

	void clear_mem();
private:

	vector< PFACE > m_vFaces;	// 掌子面指针 
};

#endif // !defined(AFX_TUNNEL_H__A492D1E6_F080_4E24_A453_4CF5A51291C4__INCLUDED_)

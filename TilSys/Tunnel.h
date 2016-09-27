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

// �������ϵı߽���
typedef struct __tagLINE
{
	MESH_TYPE type;	// �߽�������,�����塢������ס���ǽ���������ṹ�桢�����桢Ԥ�����
	int nPreLnk;
	int nNxtLnk;	// ǰ�����ӡ���������
	int nPtNum;		// �߽��ߵĵ���
	POINT_3F pt[1];	// ��������
} LINE, *PLINE, **PPLINE;

typedef struct __tagFACE
{
	long lDK;		// ���������
	int nLineNum;	// �������ϱ߽�����Ŀ
	PPLINE line;	// �߽���
} FACE, *PFACE, **PPFACE;

class CTunnel
{
public:
	CTunnel();
	virtual ~CTunnel();

	// ���벢����������ߴ�
	void ImportData(CTilCanvasMgr* pMgr);

	// �����������
	void CreateTunArc(CSceneGraph3d& scene,float fPreDist=0);
	// �������������
	void CreateTunFace(CSceneGraph3d& scene);
	// ��������ڵĽṹ��
	void CreateTunStruct(CSceneGraph3d& scene,float fPreDist=0);
	// ��������������
	void CreateVirGeoDrill(CSceneGraph3d& scene, POINT2DF ptCenter, float fRadius);

public:
	// ������ά����
	void MeshRock(
	CMesh3d* pMesh, MESH_TYPE type,
	PPOINT_3F pObj1, int nNum1, // ��һ�������
	PPOINT_3F pObj2, int nNum2,	// �ڶ��������
	int nTextureIdx, int nCanvasIdx);
	// �����ṹ��
	void MeshStruct(
	CMesh3d* pMesh, 
	PPOINT_3F pObj1, int nNum1, // ��һ�������
	PPOINT_3F pObj2, int nNum2,	// �ڶ��������
	int nTextureIdx, int nCanvasIdx);
	// ����һ����
	void MeshFace(CMesh3d* pMesh,
		PPOINT_3F pObj, int nNum,
		int nTextureIdx, int nCanvasIdx);

private:
	// ��������ͬ���ϵĶ���εĶ����Ӧ��������
	void link_faces(PPOINT_3F pPolygon1, int nNum1,
					PPOINT_3F pPolygon2, int nNum2,
					POINT_3F*** pppLinkFace21, int** ppNum21);
	// ��������ͬ���ϵĶ���ߵĶ����Ӧ��������
	void link_line_segments(PPOINT_3F pLineSegs1, int nNum1,
							PPOINT_3F pLineSegs2, int nNum2,
							POINT_3F*** pppLinkSeg21, int** pNum21);
	// Ԥ����һ��������
	void predict_outer_face(PPPOINT_3F ppLink21, int* pNum21, int nNum2,
							float* pFace3,
							PPPOINT_3F ppPolygon3, int& nNum);
	// Ԥ������������֮�����һ������������
	void predict_inner_face(PPPOINT_3F ppLink31, int* pNum31, int nNum3,
							float* pFace2, 
							PPPOINT_3F ppPolygon2, int& nNum2);
	// Ԥ������������֮�����һ��������
	void predict_inner_face(PPPOINT_3F ppLink21, int* pNum21, int nNum2,
							PPPOINT_3F ppLink32, int* pNum32, int nNum3,
							double* pFace4, 
							PPPOINT_3F ppPolygon4, int& nNum4);
	// Ԥ������������֮�����һ������
	void predict_profile_face(PPPOINT_3F ppLink31, int* pNum31, int nNum3,
							float* pFace2, deque< deque<POINT_3F> >& ddPts31);
	// ��������ͬ�˵���߶λ����ߺϲ�
	void merge_line_segments(deque< deque<POINT_3F> >& ddPts);
	// ��ƽ���еĶ�����ϵĹؼ��㰴˳ʱ��洢
	float clock_wise(PPOINT2DF pPts, int nNum);
	float clock_wise(PPOINT_3F pPts, int nNum);
	// ��ƽ���еĶ�����ϵĹؼ��㰴���Ϻ��¡�������ҵ�˳��洢
	bool sequence(PPOINT_3F pPts, int nNum);

	// ��Բ���Ƕ�ƽ�֣�����ÿ��ƽ�ֵ�����꣬�����δ洢
	void divide_circle(float fRadius, int nNum, PPOINT2DF& pPts);
	void divide_circle(POINT2DF ptCenter, float fRadius, int nNum, PPOINT2DF& pPts);
	// �ָ�Բ������Բ���Ի���ƽ���ָ�Ķ˵����ӱ�ʾ
	void divide_arc(const POINT2DF& pt1, const POINT2DF& pt2, const POINT2DF& ptCenter, 
					PPOINT2DF& pPts, int& nNum);

	void clear_mem();
private:

	vector< PFACE > m_vFaces;	// ������ָ�� 
};

#endif // !defined(AFX_TUNNEL_H__A492D1E6_F080_4E24_A453_4CF5A51291C4__INCLUDED_)

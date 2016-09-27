// MeshTunnel.h: interface for the CMeshTunnel class.
//
//////////////////////////////////////////////////////////////////////

/************************************
  REVISION LOG ENTRY
  ά����Ա: Leng
  ά��ʱ�� 2009-4-2 15:05:53
  ˵��: �����͹��������ά���ʽṹ
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

	// ���������ʾ����̷�Χ
	void SetRangeDK(const long& lStartDK, const long& lEndDK);
	// ȡ�������ʾ����̷�Χ
	void GetRangeDK(long& lStartDK, long& lEndDK);
	// �������ĵ����
	void SetCenterDK(const long& lCenterDK) {m_lCenterDK = lCenterDK;};
	// ȡ�����ĵ����
	long GetCenterDK() { return m_lCenterDK;};

	// ���벢����������ߴ�
	void ImportData();
	// �ж��Ƿ������趨����̷�Χ�ڵ��������ͼ�ζ�������
	BOOL IsTunArcParaInputed();
	// �������������Ƭ
	void CreateTunArc(CSceneGraph3d& scene);
	// �������������(09.06.07)
	void CreateTunFace(CSceneGraph3d& scene);
	// ��������ṹ��
	void CreateStructures(CSceneGraph3d& scene);
	// ��ȡ���������ͼ��
	BOOL NormalSection(long lDK, CTilGraph& graphMgr);
	// ��ȡ������ͼ��
	BOOL Profile(long& lStartDK, long& lEndDK, CTilGraph& graphMgr);
	// Ԥ�����lDK��������ͼ��
	BOOL PreNormalSection(long lDK);

private:
	long m_lStartDK; // ��ʼ���
	long m_lEndDK;	 // ��ֹ���
	long m_lCenterDK;// ���ĵ����

protected:
	
	// ����������ά�������ṹ������
	void ConstructSidePlaneA(CSceneGraph3d &scene,
		CMesh3d* pMesh, MESH_TYPE type,
		PPOINT_3F pObj1, int nNum1,
		PPOINT_3F pObj2, int nNum2,
		int nTextureIdx, int nCanvasIdx);
	// ����������ά�������������ϵ��Ҳ���������
	void ConstructFace(CSceneGraph3d& scene, MESH_TYPE type, 
		PPOINT_3F pObj, int nNum,
		int nTextureIdx, int nCanvasIdx);
	// ��������ͼ��
	void ConstructRockMass(CSceneGraph3d &pSceneGraph, CMesh3d* pMesh,
		PPOINT_3F pObj1, int nNum1,
		PPOINT_3F pObj2, int nNum2,
		int nTextureIdx, int nCurSliceIdx);

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
	void predict_outer_face(PPPOINT_3F ppLink21, int* pNum21, int nNum2,
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

//////////////////////////////////////////////////////////////////////////
// ����������
public:
	// �����������Ķ�Ӧ��ϵ��
	BOOL VirtualGeoDrill(CSceneGraph3d& scene, /*PPOINT_3F pPts, int nNum,*/ float fRadius);
	// �������������ṹ���Ľ���
	void VirtualGeoDrill(CSceneGraph3d& scene, POINT2DF ptCenter, float fRadius);
private:
	// ��Բ���Ƕ�ƽ�֣�����ÿ��ƽ�ֵ�����꣬�����δ洢
	void divide_circle(float fRadius, int nNum, PPOINT2DF& pPts);
	void divide_circle(POINT2DF ptCenter, float fRadius, int nNum, PPOINT2DF& pPts);
	// �ָ�Բ������Բ���Ի���ƽ���ָ�Ķ˵����ӱ�ʾ
	void divide_arc(const POINT2DF& pt1, const POINT2DF& pt2, const POINT2DF& ptCenter, 
					PPOINT2DF& pPts, int& nNum);

private:
	// ���������ǽ��������ͼ
	// ������ǰ���ڱ�ǽ�еĻ������λ��
	float get_ratio(PPOINT_3F pObj, int nNum, POINT_3F pt);
};

#endif // !defined(AFX_MESHTUNNEL_H__927C7EEB_D086_47F4_87FE_880FA1A14D3E__INCLUDED_)

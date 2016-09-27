// EdgeFind.h: interface for the CEdgeFind class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDGEFIND_H__0DA19253_FFD8_46F7_9227_867875A52CEC__INCLUDED_)
#define AFX_EDGEFIND_H__0DA19253_FFD8_46F7_9227_867875A52CEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define ANGLEDIFF 10	// �ǶȲ���ֵ

typedef struct __tagLINECAP
{
	POINT pt1, pt2;	// �˵�
	float fAngle;		// ��б�Ƕ�
} LINECAP, *PLINECAP, **PPLINECAP;

typedef struct __tagRELATION
{
	int nLine1, nEnd1;	// ��1���߶��ϵĶ˵����
	int nLine2, nEnd2;	// ��2���߶��ϵĶ˵����
	float fRelation;	// �����ֵ
} Relation, *PRelation, **PPRelation;

#include "TilCanvas.h"
class AFX_EXT_CLASS CEdgeFind  
{
public:
	CEdgeFind(CTilCanvas* pCanvas);
	virtual ~CEdgeFind();

public:
	// ���ұ߽磬ע�⣺�˴�ֻ�����ֵϸ��ͼ��
	// int& nPosition: ��������λ��,���ú���Ϊ�߳�ʱ,���ڱ�ע�߳̽���
	BOOL FindEdge(const float& fDsumT, const float& fDminT, const float& fAngleT, const float& fLengthT, int& nPosition);
private:
	CEdgeFind();

	CTilCanvas* m_pCanvas;
	// �������ڴ洢ͼ������
	typedef struct __tagMATRIX
	{
		int nWidth, nHeight;
		BYTE elem[1];
	} MATRIX, *PMATRIX, **PPMATRIX;

private:
	// ��Ŀ�꿽�����ڴ���
	BOOL CopyBuffer(PRUNLENGTHCODE_MGR pRLCMgr, MATRIX** ppMatrix);
	// ����Ŀ��߽����ϵĵ�
	void FindEdgePoints(MATRIX* pMatrix, deque<POINT>& dPoints);
	// �ж��Ƿ��Ƿ�ձ߽��ߣ������ǣ���ȡ��һ���߽�˵�
	BOOL IsClosed(MATRIX* pMatrix, POINT& ptEnd);
	// �����ֱ�߼�н�
	float GetAngle(const LINECAP& line1, const LINECAP& line2);
	// �����ֱ�߼�˵㵽��һֱ�ߵľ����
	float GetDistSum(const LINECAP& line1, const int& nIdx1, const LINECAP& line2, const int& nIdx2);
	// �ж���ֱ�ߵĶ˵㵽��һ���߶εĴ����Ƿ����߶���
	BOOL IsVertPtInLine(const LINECAP& line1, const LINECAP& line2);
	// ������߶ζ˵�����С���룬����¼��С�����
	float GetMinDist(const LINECAP& line1, const LINECAP& line2, float& fMinDist, int& nIdx1, int& nIdx2);
	// �����������������߽��ߵ�����������
	BOOL BuildAdjacent(const vector<LINECAP>& lineCaps1,
		const vector<LINECAP>& lineCaps2, 
		const float& fDsumT, const float& fDminT,
		const float& fAngleT, const float& fLengthT,
		int& nEnd1, int& nEnd2, float& fRelation);
	// �����뵱ǰ�߽�����������һ���߽���
	BOOL FindNextNeighborEdge(
		const vector< vector< Relation* > >& vvRelation, 
		const int& nLine1, const int& nEnd1,
		int& nLine2, int& nEnd2);
	// ������С���˷��Զ���߽����������
	LINECAP LineFit(vector<LINECAP>& vLine);
	// �Զ����ɵĶ���߽����������
	void LinesFit(const deque<POINT>& dPoints, const float& fDist, vector<LINECAP>& vLines);
	// ͳ�ƽǶȷ�Χֵ
	void StatAngles(const vector<vector<LINECAP> >& vvLines, int* pNum);
	// ͳ�Ʊ߽���
	void StatLines(const vector<vector<LINECAP> >& vvLines, vector<int>& vSum);
};

#endif // !defined(AFX_EDGEFIND_H__0DA19253_FFD8_46F7_9227_867875A52CEC__INCLUDED_)

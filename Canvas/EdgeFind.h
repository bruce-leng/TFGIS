// EdgeFind.h: interface for the CEdgeFind class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDGEFIND_H__0DA19253_FFD8_46F7_9227_867875A52CEC__INCLUDED_)
#define AFX_EDGEFIND_H__0DA19253_FFD8_46F7_9227_867875A52CEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define ANGLEDIFF 10	// 角度差阈值

typedef struct __tagLINECAP
{
	POINT pt1, pt2;	// 端点
	float fAngle;		// 倾斜角度
} LINECAP, *PLINECAP, **PPLINECAP;

typedef struct __tagRELATION
{
	int nLine1, nEnd1;	// 第1条线段上的端点序号
	int nLine2, nEnd2;	// 第2条线段上的端点序号
	float fRelation;	// 相关性值
} Relation, *PRelation, **PPRelation;

#include "TilCanvas.h"
class AFX_EXT_CLASS CEdgeFind  
{
public:
	CEdgeFind(CTilCanvas* pCanvas);
	virtual ~CEdgeFind();

public:
	// 查找边界，注意：此处只处理二值细化图像
	// int& nPosition: 进度条的位置,当该函数为线程时,用于标注线程进度
	BOOL FindEdge(const float& fDsumT, const float& fDminT, const float& fAngleT, const float& fLengthT, int& nPosition);
private:
	CEdgeFind();

	CTilCanvas* m_pCanvas;
	// 矩阵，用于存储图像数据
	typedef struct __tagMATRIX
	{
		int nWidth, nHeight;
		BYTE elem[1];
	} MATRIX, *PMATRIX, **PPMATRIX;

private:
	// 将目标拷贝至内存中
	BOOL CopyBuffer(PRUNLENGTHCODE_MGR pRLCMgr, MATRIX** ppMatrix);
	// 查找目标边界线上的点
	void FindEdgePoints(MATRIX* pMatrix, deque<POINT>& dPoints);
	// 判断是否是封闭边界线，若不是，则取得一个边界端点
	BOOL IsClosed(MATRIX* pMatrix, POINT& ptEnd);
	// 获得两直线间夹角
	float GetAngle(const LINECAP& line1, const LINECAP& line2);
	// 获得两直线间端点到另一直线的距离和
	float GetDistSum(const LINECAP& line1, const int& nIdx1, const LINECAP& line2, const int& nIdx2);
	// 判断两直线的端点到另一条线段的垂足是否在线段上
	BOOL IsVertPtInLine(const LINECAP& line1, const LINECAP& line2);
	// 获得两线段端点间的最小距离，并记录最小距离点
	float GetMinDist(const LINECAP& line1, const LINECAP& line2, float& fMinDist, int& nIdx1, int& nIdx2);
	// 查找满足条件的两边界线的相连的两点
	BOOL BuildAdjacent(const vector<LINECAP>& lineCaps1,
		const vector<LINECAP>& lineCaps2, 
		const float& fDsumT, const float& fDminT,
		const float& fAngleT, const float& fLengthT,
		int& nEnd1, int& nEnd2, float& fRelation);
	// 查找与当前边界线相连的下一条边界线
	BOOL FindNextNeighborEdge(
		const vector< vector< Relation* > >& vvRelation, 
		const int& nLine1, const int& nEnd1,
		int& nLine2, int& nEnd2);
	// 利用最小二乘法对多段线进行线性拟合
	LINECAP LineFit(vector<LINECAP>& vLine);
	// 对多点组成的多段线进行线性拟合
	void LinesFit(const deque<POINT>& dPoints, const float& fDist, vector<LINECAP>& vLines);
	// 统计角度范围值
	void StatAngles(const vector<vector<LINECAP> >& vvLines, int* pNum);
	// 统计边界线
	void StatLines(const vector<vector<LINECAP> >& vvLines, vector<int>& vSum);
};

#endif // !defined(AFX_EDGEFIND_H__0DA19253_FFD8_46F7_9227_867875A52CEC__INCLUDED_)

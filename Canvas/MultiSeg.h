// MultiSeg.h: interface for the CMultiSeg class.
//
//////////////////////////////////////////////////////////////////////

/************************************
  REVISION LOG ENTRY
  维护人员: Leng
  维护时间 2008-12-12 11:27:30
  说明: 用于对隧道多段线节理进行分析
 ************************************/


#if !defined(AFX_MULTISEG_H__AEBA67B5_2AA4_4616_90B4_C57D87EDC1E1__INCLUDED_)
#define AFX_MULTISEG_H__AEBA67B5_2AA4_4616_90B4_C57D87EDC1E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"

#include <vector>
using namespace std;

class AFX_EXT_CLASS CMultiSeg  
{
public:
	CMultiSeg();
	CMultiSeg(CGraph& graph);
	virtual ~CMultiSeg();

public:
	// 多段线长度
	float GetMultiSegLength(int nStructIdx, float fScale);
	// 同组多段线的平均长度
	float GetAveLength(int nGroupIdx, float fScale);	
	// 同组结构面平均间距
	float GetAveDistance(int nGroupIdx, float fScale);
    // 同组组结构面视倾角
	float GetArentDip(int nStructIdx);
	// 断面不同产状组数
	int GetGroupNum();

private:
	CGraph* m_pGraph;

	vector< vector<int> > m_vvGroup;
};

#endif // !defined(AFX_MULTISEG_H__AEBA67B5_2AA4_4616_90B4_C57D87EDC1E1__INCLUDED_)

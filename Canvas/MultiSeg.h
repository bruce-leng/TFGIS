// MultiSeg.h: interface for the CMultiSeg class.
//
//////////////////////////////////////////////////////////////////////

/************************************
  REVISION LOG ENTRY
  ά����Ա: Leng
  ά��ʱ�� 2008-12-12 11:27:30
  ˵��: ���ڶ��������߽�����з���
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
	// ����߳���
	float GetMultiSegLength(int nStructIdx, float fScale);
	// ͬ�����ߵ�ƽ������
	float GetAveLength(int nGroupIdx, float fScale);	
	// ͬ��ṹ��ƽ�����
	float GetAveDistance(int nGroupIdx, float fScale);
    // ͬ����ṹ�������
	float GetArentDip(int nStructIdx);
	// ���治ͬ��״����
	int GetGroupNum();

private:
	CGraph* m_pGraph;

	vector< vector<int> > m_vvGroup;
};

#endif // !defined(AFX_MULTISEG_H__AEBA67B5_2AA4_4616_90B4_C57D87EDC1E1__INCLUDED_)

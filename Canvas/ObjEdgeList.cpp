// ObjEdgeList.cpp: implementation of the ObjEdgeList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "canvas.h"
#include "ObjEdgeList.h"
#include "TilComFunction.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ObjEdgeList::ObjEdgeList() :
m_nObjMaxNum(OBJ_MAX_NUM),
m_nObjNum(0)
{
	m_ppEdgeObj = new PEDGE_OBJECT [OBJ_MAX_NUM];
}

ObjEdgeList::~ObjEdgeList()
{
	Clear();
	if (m_ppEdgeObj)
		delete [] m_ppEdgeObj;
}

// 清除内存
void ObjEdgeList::Clear()
{
	for (int i=0; i<m_nObjNum; i++)
	{
		if (m_ppEdgeObj[i]->ppEdgeLst)
		{
			for (int j=0; j<m_ppEdgeObj[i]->nEdgeNum; j++)
			{
				if (m_ppEdgeObj[i]->ppEdgeLst[j])
					delete [] (char*)m_ppEdgeObj[i]->ppEdgeLst[j];
			}
			delete [] (char*)m_ppEdgeObj[i]->ppEdgeLst;
		}
		delete m_ppEdgeObj[i];
		m_ppEdgeObj[i] = NULL;
	}

	m_nObjNum = 0;
}

// 获得第nIdx个目标区域的面积
long ObjEdgeList::ObjArea(int nIdx, int& nArea)
{
	return 0;
}

// 获得目标pObject的面积
long ObjEdgeList::ObjArea(PEDGE_OBJECT pObject, int& nArea)
{
	return 0;
}


// ObjEdgeList.h: interface for the ObjEdgeList class.
//
//////////////////////////////////////////////////////////////////////

/************************************
  REVISION LOG ENTRY
  维护人员: leng
  维护时间 2008-6-17 10:41:30
  说明: 对边界链码进行管理
 ************************************/


#if !defined OBJEDGELIST
#define OBJEDGELIST_H

// 一个目标的边界链码
typedef struct __EDGE_LIST__
{
	int nEdgeLen;			//边界长度
	POINT edge[1];		//边界上所有像素点坐标
} EDGE_LIST, *PEDGE_LIST, **PPEDGE_LIST;

typedef  struct __EDGE_OBJECT__
{
	int nEdgeMaxNum;	// 预计最大边界数目
	int nEdgeNum;			//当前目标的边界数目(包括内外边界)
	PPEDGE_LIST ppEdgeLst;  // ppEdgeLst[0]定义为目标的外边界
							// ppEdgeLst[1...nEdgeNum-1]为目标的内边界	
}EDGE_OBJECT, *PEDGE_OBJECT, **PPEDGE_OBJECT;

//编码方式：绝对像素坐标
//该类仅对边界链码本身执行操作，不对图像进行操作
class AFX_EXT_CLASS ObjEdgeList  
{
public:
	ObjEdgeList();
	virtual ~ObjEdgeList();

	int m_nObjMaxNum;	//预计最大目标数目
	int m_nObjNum;	//实际目标数目
	PPEDGE_OBJECT m_ppEdgeObj;	//所有边界的指针

public:
	// 清除内存
	void Clear();
	// 实际目标数
	int GetObjNum() { return m_nObjNum; };
	// 获得所有边界的指针
	PPEDGE_OBJECT GetObjAllEdgeLst() { return m_ppEdgeObj; };
	// 删除包括种子点ptSeed的目标的边界链码
	int ObjDelEdgeLst(POINT ptSeed);
	// 获得第nIdx个目标区域的面积
	long ObjArea(int nIdx, int& nArea);
	// 获得目标pObject的面积
	long ObjArea(PEDGE_OBJECT pObject, int& nArea);
};

#endif // !defined(OBJEDGELIST_H)

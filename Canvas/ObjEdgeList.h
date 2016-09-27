// ObjEdgeList.h: interface for the ObjEdgeList class.
//
//////////////////////////////////////////////////////////////////////

/************************************
  REVISION LOG ENTRY
  ά����Ա: leng
  ά��ʱ�� 2008-6-17 10:41:30
  ˵��: �Ա߽�������й���
 ************************************/


#if !defined OBJEDGELIST
#define OBJEDGELIST_H

// һ��Ŀ��ı߽�����
typedef struct __EDGE_LIST__
{
	int nEdgeLen;			//�߽糤��
	POINT edge[1];		//�߽����������ص�����
} EDGE_LIST, *PEDGE_LIST, **PPEDGE_LIST;

typedef  struct __EDGE_OBJECT__
{
	int nEdgeMaxNum;	// Ԥ�����߽���Ŀ
	int nEdgeNum;			//��ǰĿ��ı߽���Ŀ(��������߽�)
	PPEDGE_LIST ppEdgeLst;  // ppEdgeLst[0]����ΪĿ�����߽�
							// ppEdgeLst[1...nEdgeNum-1]ΪĿ����ڱ߽�	
}EDGE_OBJECT, *PEDGE_OBJECT, **PPEDGE_OBJECT;

//���뷽ʽ��������������
//������Ա߽����뱾��ִ�в���������ͼ����в���
class AFX_EXT_CLASS ObjEdgeList  
{
public:
	ObjEdgeList();
	virtual ~ObjEdgeList();

	int m_nObjMaxNum;	//Ԥ�����Ŀ����Ŀ
	int m_nObjNum;	//ʵ��Ŀ����Ŀ
	PPEDGE_OBJECT m_ppEdgeObj;	//���б߽��ָ��

public:
	// ����ڴ�
	void Clear();
	// ʵ��Ŀ����
	int GetObjNum() { return m_nObjNum; };
	// ������б߽��ָ��
	PPEDGE_OBJECT GetObjAllEdgeLst() { return m_ppEdgeObj; };
	// ɾ���������ӵ�ptSeed��Ŀ��ı߽�����
	int ObjDelEdgeLst(POINT ptSeed);
	// ��õ�nIdx��Ŀ����������
	long ObjArea(int nIdx, int& nArea);
	// ���Ŀ��pObject�����
	long ObjArea(PEDGE_OBJECT pObject, int& nArea);
};

#endif // !defined(OBJEDGELIST_H)

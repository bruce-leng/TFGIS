// ObjRunLenCode.h: interface for the ObjRunLenCode class.
//
//////////////////////////////////////////////////////////////////////

/************************************
  REVISION LOG ENTRY
  ά����Ա: Leng
  ά��ʱ�� 2008-6-17 10:55:14
  ˵��: ���γ̱�����й���
 ************************************/


#if !defined OBJRUNLENCODE_H
#define OBJRUNLENCODE_H

#include "Image.h"
#include "TilPoint.h"

// �����γ�(08.06.17)
typedef struct __RUNLENGTH_CODE
{
	BOOL bObserved;	// Ԥ��
	BOOL bFlags;	// ���
	int y;			//��ʼ��
	int xStart;		//��ʼ��
	int xEnd;		//��ֹ�У�ע�⣺���������ֹ������
} RUNLENGTHCODE, *PRUNLENGTHCODE, **PPRUNLENGTHCODE;

// ĳ��Ŀ��������γ�(08.06.17)
typedef struct __RUNLENGTH_CODE_MGR
{
	int nRunNum;			//��ǰĿ����γ���
	RUNLENGTHCODE RC[1];	//��ǰĿ��������γ�
} RUNLENGTHCODE_MGR, *PRUNLENGTHCODE_MGR, **PPRUNLENGTHCODE_MGR;

class AFX_EXT_CLASS ObjRunLenCode  
{
public:
	friend class CImage;

	ObjRunLenCode();
	virtual ~ObjRunLenCode();

	// �������
	void Clear();
	// ����Ŀ����ɫ
	UCHAR GetObjColor() { return m_uchObjClr; };
	// ���ʵ��Ŀ����Ŀ
	int GetObjNum() { return m_nObjNum; };
	// ��������Ŀ���ָ��
	PPRUNLENGTHCODE_MGR GetObjAll() { return m_ppObjRun; };
	// ���ص�nIdx��Ŀ���ָ��
	PRUNLENGTHCODE_MGR  GetObj(int nIdx) { return m_ppObjRun[nIdx]; };
	// ������ӵ�ptSeed���ڵ�Ŀ��
	PRUNLENGTHCODE_MGR  GetObj(POINT ptSeed);
	// ɾ����nIdx��Ŀ���ָ��
	BOOL ObjDel(int nIdx);
	// ��nIdx��Ŀ�����Ӿ���
	void ObjRect(int nIdx, CRect& rect);
	// ���Ŀ��pObject����Ӿ���
	void ObjRect(PRUNLENGTHCODE_MGR pObject, CRect& rect);
	// ��õ�nIdx��Ŀ����������
	long ObjArea(int nIdx);
	// ���Ŀ��pObject�����
	long ObjArea(PRUNLENGTHCODE_MGR pObject);
	// ��õ�nIdx��Ŀ������ĵ�����
	void ObjCenter(int nIdx, POINT2DF& ptCenter);
	// ���Ŀ��pObject�����ĵ�����
	void ObjCenter(PRUNLENGTHCODE_MGR pObject, POINT2DF& ptCenter);

protected:
	UCHAR m_uchObjClr;
	int m_nObjMaxNum;		//Ԥ������Ŀ������
	int m_nObjNum;		//ʵ��Ŀ����
	PPRUNLENGTHCODE_MGR m_ppObjRun;	//����Ŀ���ָ��

//	// ����Ŀ����ɫ
//	void SetObjColor(UCHAR uObjClr=255) { m_uchObjClr = uObjClr; };
//	// �����γ̱���
//	int AllRLC(PBYTE pBits, int nWidth, int nHeight, int nBpp);
//	// ����Ŀ����γ̱���
//	int ObjAllRLC();
//	// ������ptSeed��Ŀ����γ̱���
//	int ObjRLC(POINT ptSeed);
//	// ɾ��������ptSeed��Ŀ����γ̱���
//	int ObjDelRLC(POINT ptSeed);

public:
};

#endif // !defined(OBJRUNLENCODE_H)

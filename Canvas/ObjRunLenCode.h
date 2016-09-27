// ObjRunLenCode.h: interface for the ObjRunLenCode class.
//
//////////////////////////////////////////////////////////////////////

/************************************
  REVISION LOG ENTRY
  维护人员: Leng
  维护时间 2008-6-17 10:55:14
  说明: 对游程编码进行管理
 ************************************/


#if !defined OBJRUNLENCODE_H
#define OBJRUNLENCODE_H

#include "Image.h"
#include "TilPoint.h"

// 单个游程(08.06.17)
typedef struct __RUNLENGTH_CODE
{
	BOOL bObserved;	// 预留
	BOOL bFlags;	// 标记
	int y;			//起始行
	int xStart;		//起始列
	int xEnd;		//终止列，注意：编码包含终止列坐标
} RUNLENGTHCODE, *PRUNLENGTHCODE, **PPRUNLENGTHCODE;

// 某个目标的所有游程(08.06.17)
typedef struct __RUNLENGTH_CODE_MGR
{
	int nRunNum;			//当前目标的游程数
	RUNLENGTHCODE RC[1];	//当前目标的所有游程
} RUNLENGTHCODE_MGR, *PRUNLENGTHCODE_MGR, **PPRUNLENGTHCODE_MGR;

class AFX_EXT_CLASS ObjRunLenCode  
{
public:
	friend class CImage;

	ObjRunLenCode();
	virtual ~ObjRunLenCode();

	// 清除内容
	void Clear();
	// 返回目标颜色
	UCHAR GetObjColor() { return m_uchObjClr; };
	// 获得实际目标数目
	int GetObjNum() { return m_nObjNum; };
	// 返回所有目标的指针
	PPRUNLENGTHCODE_MGR GetObjAll() { return m_ppObjRun; };
	// 返回第nIdx个目标的指针
	PRUNLENGTHCODE_MGR  GetObj(int nIdx) { return m_ppObjRun[nIdx]; };
	// 获得种子点ptSeed所在的目标
	PRUNLENGTHCODE_MGR  GetObj(POINT ptSeed);
	// 删除第nIdx个目标的指针
	BOOL ObjDel(int nIdx);
	// 第nIdx个目标的外接矩形
	void ObjRect(int nIdx, CRect& rect);
	// 获得目标pObject的外接矩形
	void ObjRect(PRUNLENGTHCODE_MGR pObject, CRect& rect);
	// 获得第nIdx个目标区域的面积
	long ObjArea(int nIdx);
	// 获得目标pObject的面积
	long ObjArea(PRUNLENGTHCODE_MGR pObject);
	// 获得第nIdx个目标的中心点坐标
	void ObjCenter(int nIdx, POINT2DF& ptCenter);
	// 获得目标pObject的中心点坐标
	void ObjCenter(PRUNLENGTHCODE_MGR pObject, POINT2DF& ptCenter);

protected:
	UCHAR m_uchObjClr;
	int m_nObjMaxNum;		//预计最大的目标总数
	int m_nObjNum;		//实际目标数
	PPRUNLENGTHCODE_MGR m_ppObjRun;	//所有目标的指针

//	// 设置目标颜色
//	void SetObjColor(UCHAR uObjClr=255) { m_uchObjClr = uObjClr; };
//	// 所有游程编码
//	int AllRLC(PBYTE pBits, int nWidth, int nHeight, int nBpp);
//	// 所有目标的游程编码
//	int ObjAllRLC();
//	// 包括点ptSeed的目标的游程编码
//	int ObjRLC(POINT ptSeed);
//	// 删除包括点ptSeed的目标的游程编码
//	int ObjDelRLC(POINT ptSeed);

public:
};

#endif // !defined(OBJRUNLENCODE_H)

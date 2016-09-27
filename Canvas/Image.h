// Image.h: interface for the CImage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGE_H__DBA9054A_1CDB_4386_A1C6_81641DB88F89__INCLUDED_)
#define AFX_IMAGE_H__DBA9054A_1CDB_4386_A1C6_81641DB88F89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ximage.h"
#include "Memmap.h"
#include "ObjRunLenCode.h"
#include "ObjEdgeList.h"
#include <queue>
using namespace std;

class AFX_EXT_CLASS CImage : public CxImage , public CObject 
{
public:
	friend class ObjRunLenCode;

	CImage();
	CImage(const CImage& image);
	CImage(const CString& strPathName, DWORD imagetype);
	virtual ~CImage();

	long Draw(CDC* pDC);
//	long Draw(CDC* pDC, const RECT& rectDst, const RECT& rectSrc);

	void SetZoomFactor(long double dbZoomX, long double dbZoomY);
	void GetZoomFactor(long double& dbZoomX, long double& dbZoomY);
	
	void SetModifiedFlag(BOOL bFlag = TRUE);
	BOOL IsModified() const;

private:
	long double m_dbZoomX;
	long double m_dbZoomY;
	BOOL m_bModified;

	CMemMapFile *m_pMemFile;

//////////////////////////////////////////////////////////////////////////
// 二值图像处理算法
public: // 自己添加的算法
	// 下面这几个直方图函数在CxImage类中存在,但不知何故一直不能调用，故在这里再实现一次
#if CXIMAGE_SUPPORT_DSP
	bool HistogramStretch(long method = 0);
	bool HistogramEqualize();
	bool HistogramNormalize();
	bool HistogramRoot();
	bool HistogramLog();
	long Histogram(long* red, long* green = 0, long* blue = 0, long* gray = 0, long colorspace = 0);
#endif

	// 得到最佳分割阈值
	int  GetBestThreshold();
	// 局部最佳分割
	void BestThreshold();
	// Canny算子
	void EdgeCanny();
	// 数学形态学细化
	BOOL MorphThin();
	BOOL MorphThining();

	// 取得所有目标游程编码（外边界按8邻域搜索）
	void ObjAllRLC(ObjRunLenCode* pObjAllRLC, UCHAR uObjClr=255);
	// 取得单个目标的游程编码（外边界按8邻域搜索）
	void ObjRLC(ObjRunLenCode* pObjAllRLC, POINT ptSeed, UCHAR uObjClr=255);
	// 取得所有目标的边界链码（外边界按8邻域搜索）
	void ObjAllEdgeLst(ObjEdgeList* pObjEdgeLst, UCHAR uObjClr=255);
	// 取得单个目标的边界链码（外边界按8邻域搜索）
	void ObjEdgeLst(ObjEdgeList* pObjEdgeLst, POINT ptSeed, UCHAR uObjClr=255);
	// 边界拟合
	void ObjEdgeMerge(ObjEdgeList* pSrcEdgeLst, float fDist, ObjEdgeList* pDstEdgeLst);
	// 区域生长
	void ObjRgnGrow(POINT ptSeed, UCHAR uGrwThrshld, ObjRunLenCode* pRgnGrow);
	// 删除目标内部背景色
	void ObjDelInner(ObjEdgeList* pObjEdgeLst);
	// 设置目标颜色
	void SetObjColor(ObjRunLenCode* pObjAllRLC, COLORREF rgb);
	// 取得颜色为uObjClr的所有游程编码
	void RunLengthcode(ObjRunLenCode* pObjAllRLC, UCHAR uObjClr=255);

//////////////////////////////////////////////////////////////////////////
// 灰度图像处理算法
public:
	// 灰度非线性变换
	void GreyNonLinearTransfer(float a, float b, float c);
	// 动态范围压缩
	void DynCompress(float C, float s);
	// 梯度倒数加权平滑
	void WeightGradientCountDownSmooth(int length);
};

#endif // !defined(AFX_IMAGE_H__DBA9054A_1CDB_4386_A1C6_81641DB88F89__INCLUDED_)

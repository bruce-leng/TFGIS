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
// ��ֵͼ�����㷨
public: // �Լ���ӵ��㷨
	// �����⼸��ֱ��ͼ������CxImage���д���,����֪�ι�һֱ���ܵ��ã�����������ʵ��һ��
#if CXIMAGE_SUPPORT_DSP
	bool HistogramStretch(long method = 0);
	bool HistogramEqualize();
	bool HistogramNormalize();
	bool HistogramRoot();
	bool HistogramLog();
	long Histogram(long* red, long* green = 0, long* blue = 0, long* gray = 0, long colorspace = 0);
#endif

	// �õ���ѷָ���ֵ
	int  GetBestThreshold();
	// �ֲ���ѷָ�
	void BestThreshold();
	// Canny����
	void EdgeCanny();
	// ��ѧ��̬ѧϸ��
	BOOL MorphThin();
	BOOL MorphThining();

	// ȡ������Ŀ���γ̱��루��߽簴8����������
	void ObjAllRLC(ObjRunLenCode* pObjAllRLC, UCHAR uObjClr=255);
	// ȡ�õ���Ŀ����γ̱��루��߽簴8����������
	void ObjRLC(ObjRunLenCode* pObjAllRLC, POINT ptSeed, UCHAR uObjClr=255);
	// ȡ������Ŀ��ı߽����루��߽簴8����������
	void ObjAllEdgeLst(ObjEdgeList* pObjEdgeLst, UCHAR uObjClr=255);
	// ȡ�õ���Ŀ��ı߽����루��߽簴8����������
	void ObjEdgeLst(ObjEdgeList* pObjEdgeLst, POINT ptSeed, UCHAR uObjClr=255);
	// �߽����
	void ObjEdgeMerge(ObjEdgeList* pSrcEdgeLst, float fDist, ObjEdgeList* pDstEdgeLst);
	// ��������
	void ObjRgnGrow(POINT ptSeed, UCHAR uGrwThrshld, ObjRunLenCode* pRgnGrow);
	// ɾ��Ŀ���ڲ�����ɫ
	void ObjDelInner(ObjEdgeList* pObjEdgeLst);
	// ����Ŀ����ɫ
	void SetObjColor(ObjRunLenCode* pObjAllRLC, COLORREF rgb);
	// ȡ����ɫΪuObjClr�������γ̱���
	void RunLengthcode(ObjRunLenCode* pObjAllRLC, UCHAR uObjClr=255);

//////////////////////////////////////////////////////////////////////////
// �Ҷ�ͼ�����㷨
public:
	// �Ҷȷ����Ա任
	void GreyNonLinearTransfer(float a, float b, float c);
	// ��̬��Χѹ��
	void DynCompress(float C, float s);
	// �ݶȵ�����Ȩƽ��
	void WeightGradientCountDownSmooth(int length);
};

#endif // !defined(AFX_IMAGE_H__DBA9054A_1CDB_4386_A1C6_81641DB88F89__INCLUDED_)

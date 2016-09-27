// Veins.h: interface for the CVeins class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VEINS_H__4D8B2247_4002_4CC3_A299_6FC2885BE6DC__INCLUDED_)
#define AFX_VEINS_H__4D8B2247_4002_4CC3_A299_6FC2885BE6DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Image.h"

// 颗粒含量分布
typedef struct _GRAINCONTENT_DISTRIBUTING
{
	int nGroupObjNum[4];   // 面积分布。面积(以像素为单位)小于100，大于100并且小于200,
	long nGroupObjAreaSum[4];   // 大于200并且小于500，大于500四种情况.
	float fGrainContent[4];
}GrainContent;

// 纹理统计信息 
typedef struct _Vein_MesoInfor
{
	float* pLenAreRadio;   // 目标边界长度与面积比
	GrainContent GrainCont;	
}VeinStatInfor;

class AFX_EXT_CLASS CVeins  
{
public:
	CVeins(CImage& image);
	virtual ~CVeins();	
	
	bool CreatAccrMatrix();			//生成共生矩阵
	float Vein2Mom();				//纹理二阶矩
	float Entropy();				//熵
	float Contrast();				//对比度
    float Homogen(float k);			//均匀性
	double RowFrequency(int nL);	//行频
	double ColFrequency(int nL);	//列频
	double LDFrequency(int nL);		// left diagonal frequency 左对角频
	double RDFrequency(int nL);		// right diagonal frequency 右对角频

	// 获得纹理相关属性
	void CreateCoMatrix();
	float Vein2Mom(int nIdx);			//纹理二阶矩
	float Entropy(int nIdx);			//熵
	float Contrast(int nIdx);			//对比度
    float Homogen(int nIdx, float k);   //均匀性

public:
	VeinStatInfor GrainContentDis();   // 颗粒含量分布

protected:
	CVeins();

	CImage *m_pImage;
	float m_AccrMatrix[3][256][256];

	UCHAR *m_pImageGreyMatrix;
	VeinStatInfor m_VeinStatInfor;
		
	int m_nWidth;   //图像宽度
	int m_nHeight;   //图像高度
	float *m_pData;    
	
	//纹理描述符
	float m_fVein2Mom;   //纹理二阶矩
	float m_fEntropy;   //熵
	float m_fContrast;   //对比度
	float m_fHomogen;   //均匀性
};

#endif // !defined(AFX_VEINS_H__4D8B2247_4002_4CC3_A299_6FC2885BE6DC__INCLUDED_)

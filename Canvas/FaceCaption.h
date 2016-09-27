// FaceCaption.h: interface for the CFaceCaption class.
//
//////////////////////////////////////////////////////////////////////

/************************************
  REVISION LOG ENTRY
  维护人员: Leng
  维护时间 2009-4-1 21:01:34
  说明: 对掌子面图形层进行分析
 ************************************/


#if !defined(AFX_FACECAPTION_H__6AF1F2F4_2AD5_420F_BDE4_5B0144285098__INCLUDED_)
#define AFX_FACECAPTION_H__6AF1F2F4_2AD5_420F_BDE4_5B0144285098__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <AFXTEMPL.H>
#include "TilPoint.h"
#include "Image.h"
#include "TilGraph.h"
#include "TilCanvas.h"

class AFX_EXT_CLASS CFaceCaption  
{
public:
	CFaceCaption();
	virtual ~CFaceCaption();

	// 从CTilCanvas类对象中拷贝数据
	void CopyFrom(CTilCanvas* pCanvas);

	// 单位面积裂隙总长度(m/m2)
	float JointLengthofUnitArea();
	// 平均块体尺寸(m)
	float AveRockSize(float fLineDist);
	// 不连续频率-为平均块体倒数
	float DiscontinueFreqency(float fLineDist);
	// 取得掌子面上的RQD值
	float RQD(float fLineDist, float fMinDist);

	// 2009.8.21追加
	// 单位面积割线数量
	float DividLinesofUnitArea(float fLineDist);
	// 单位面积边界线数量
	float EdgeLinesofUnitArea();
	// 掌子面岩体破碎程度评价指标
	float RockCrackDegree();
	// 掌子面上的平均灰度值
	RGBQUAD GreyAveValue(float& fRed, float& fGreen, float& fBlue);
	// 掌子面上的灰度均方差
	void GreyMean2Deviation(float& fRed, float& fGreen, float& fBlue);
	// 掌子面岩体风化程度评价指标
	void RockWeathingDegree(float& fRed, float& fGreen, float& fBlue);
	// 掌子面评价描述
	void Description();

	// 掌子面属性
	CString m_strDK;		// 掌子面里程
	CString m_strMethod;	// 开挖方法
	float m_fDepth;			// 开挖深度
	CString m_strLith;		// 围岩岩性
	float m_fAveJointLen;	// 单位面积裂隙总长度(m/m2)
	float m_fAveRockSize;	// 平均岩体尺寸(m)
	float m_fDiscontinueFreq;	// 不连续频率
	float m_fRQD;			// 岩体质量指标

	// 2009.8.21追加
	float m_fDividLines;									// 单位面积割线数量
	float m_fEdgeLines;										// 单位面积边界线数量
	float m_fRockCrack;										// 掌子面岩体破碎程度评价指标
	float m_fAveRed, m_fAveGreen, m_fAveBlue;				// 掌子面上的平均灰度值
	float m_fMean2DevRed, m_fMean2DevGreen, m_fMean2DevBlue;// 掌子面上的灰度均方差
	float m_fRockWeathRed, m_fRockWeathGreen, m_fRockWeathBlue;	// 掌子面岩体风化程度评价指标

	CString m_strDesc;		// 掌子面评价描述(2014.3.30)


	float m_fAngle1;		// 组1节理视倾角
	float m_fAngle2;		// 组2节理视倾角
	float m_fAngle3;		// 组3节理视倾角
	float m_fAngle4;		// 组4节理视倾角

private:
	// 求多边形面积
	//double area(CArray<POINT2DF, POINT2DF>* pPts);
	// 将所有图形层转化成一个图像层
	CImage* graph2image(CTilGraph* pGraphMgr);
	// 求掌子面区域的游程编码
	ObjRunLenCode* facearea(CTilGraph* pGraphMgr);

	CTilCanvas* m_pCanvas;

	// 直接根据Canny算子检测的掌子面边界进行特征提取(2009.08.28)
public:
	// 单位面积边界线数量
	float AveEdgeLines();
	// 掌子面上单位面积的岩体破碎程度评价指标
	float AveRockCracks();
private:
	// 获得单像素边界以外的
	// 对图像进行边缘canny边缘检测，然后再根据掌子面边界取清除掌子面以外的边界
	// 后再形成图像
	ObjRunLenCode* outfaceobjrlc();
	CImage* m_pImage;
	ObjRunLenCode* m_pObjFace;
	float m_fFaceArea;
};

#endif // !defined(AFX_FACECAPTION_H__6AF1F2F4_2AD5_420F_BDE4_5B0144285098__INCLUDED_)

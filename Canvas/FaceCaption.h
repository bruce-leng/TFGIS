// FaceCaption.h: interface for the CFaceCaption class.
//
//////////////////////////////////////////////////////////////////////

/************************************
  REVISION LOG ENTRY
  ά����Ա: Leng
  ά��ʱ�� 2009-4-1 21:01:34
  ˵��: ��������ͼ�β���з���
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

	// ��CTilCanvas������п�������
	void CopyFrom(CTilCanvas* pCanvas);

	// ��λ�����϶�ܳ���(m/m2)
	float JointLengthofUnitArea();
	// ƽ������ߴ�(m)
	float AveRockSize(float fLineDist);
	// ������Ƶ��-Ϊƽ�����嵹��
	float DiscontinueFreqency(float fLineDist);
	// ȡ���������ϵ�RQDֵ
	float RQD(float fLineDist, float fMinDist);

	// 2009.8.21׷��
	// ��λ�����������
	float DividLinesofUnitArea(float fLineDist);
	// ��λ����߽�������
	float EdgeLinesofUnitArea();
	// ��������������̶�����ָ��
	float RockCrackDegree();
	// �������ϵ�ƽ���Ҷ�ֵ
	RGBQUAD GreyAveValue(float& fRed, float& fGreen, float& fBlue);
	// �������ϵĻҶȾ�����
	void GreyMean2Deviation(float& fRed, float& fGreen, float& fBlue);
	// ����������绯�̶�����ָ��
	void RockWeathingDegree(float& fRed, float& fGreen, float& fBlue);
	// ��������������
	void Description();

	// ����������
	CString m_strDK;		// ���������
	CString m_strMethod;	// ���ڷ���
	float m_fDepth;			// �������
	CString m_strLith;		// Χ������
	float m_fAveJointLen;	// ��λ�����϶�ܳ���(m/m2)
	float m_fAveRockSize;	// ƽ������ߴ�(m)
	float m_fDiscontinueFreq;	// ������Ƶ��
	float m_fRQD;			// ��������ָ��

	// 2009.8.21׷��
	float m_fDividLines;									// ��λ�����������
	float m_fEdgeLines;										// ��λ����߽�������
	float m_fRockCrack;										// ��������������̶�����ָ��
	float m_fAveRed, m_fAveGreen, m_fAveBlue;				// �������ϵ�ƽ���Ҷ�ֵ
	float m_fMean2DevRed, m_fMean2DevGreen, m_fMean2DevBlue;// �������ϵĻҶȾ�����
	float m_fRockWeathRed, m_fRockWeathGreen, m_fRockWeathBlue;	// ����������绯�̶�����ָ��

	CString m_strDesc;		// ��������������(2014.3.30)


	float m_fAngle1;		// ��1���������
	float m_fAngle2;		// ��2���������
	float m_fAngle3;		// ��3���������
	float m_fAngle4;		// ��4���������

private:
	// ���������
	//double area(CArray<POINT2DF, POINT2DF>* pPts);
	// ������ͼ�β�ת����һ��ͼ���
	CImage* graph2image(CTilGraph* pGraphMgr);
	// ��������������γ̱���
	ObjRunLenCode* facearea(CTilGraph* pGraphMgr);

	CTilCanvas* m_pCanvas;

	// ֱ�Ӹ���Canny���Ӽ���������߽����������ȡ(2009.08.28)
public:
	// ��λ����߽�������
	float AveEdgeLines();
	// �������ϵ�λ�������������̶�����ָ��
	float AveRockCracks();
private:
	// ��õ����ر߽������
	// ��ͼ����б�Եcanny��Ե��⣬Ȼ���ٸ���������߽�ȡ�������������ı߽�
	// �����γ�ͼ��
	ObjRunLenCode* outfaceobjrlc();
	CImage* m_pImage;
	ObjRunLenCode* m_pObjFace;
	float m_fFaceArea;
};

#endif // !defined(AFX_FACECAPTION_H__6AF1F2F4_2AD5_420F_BDE4_5B0144285098__INCLUDED_)

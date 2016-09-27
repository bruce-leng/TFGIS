// Veins.h: interface for the CVeins class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VEINS_H__4D8B2247_4002_4CC3_A299_6FC2885BE6DC__INCLUDED_)
#define AFX_VEINS_H__4D8B2247_4002_4CC3_A299_6FC2885BE6DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Image.h"

// ���������ֲ�
typedef struct _GRAINCONTENT_DISTRIBUTING
{
	int nGroupObjNum[4];   // ����ֲ������(������Ϊ��λ)С��100������100����С��200,
	long nGroupObjAreaSum[4];   // ����200����С��500������500�������.
	float fGrainContent[4];
}GrainContent;

// ����ͳ����Ϣ 
typedef struct _Vein_MesoInfor
{
	float* pLenAreRadio;   // Ŀ��߽糤���������
	GrainContent GrainCont;	
}VeinStatInfor;

class AFX_EXT_CLASS CVeins  
{
public:
	CVeins(CImage& image);
	virtual ~CVeins();	
	
	bool CreatAccrMatrix();			//���ɹ�������
	float Vein2Mom();				//������׾�
	float Entropy();				//��
	float Contrast();				//�Աȶ�
    float Homogen(float k);			//������
	double RowFrequency(int nL);	//��Ƶ
	double ColFrequency(int nL);	//��Ƶ
	double LDFrequency(int nL);		// left diagonal frequency ��Խ�Ƶ
	double RDFrequency(int nL);		// right diagonal frequency �ҶԽ�Ƶ

	// ��������������
	void CreateCoMatrix();
	float Vein2Mom(int nIdx);			//������׾�
	float Entropy(int nIdx);			//��
	float Contrast(int nIdx);			//�Աȶ�
    float Homogen(int nIdx, float k);   //������

public:
	VeinStatInfor GrainContentDis();   // ���������ֲ�

protected:
	CVeins();

	CImage *m_pImage;
	float m_AccrMatrix[3][256][256];

	UCHAR *m_pImageGreyMatrix;
	VeinStatInfor m_VeinStatInfor;
		
	int m_nWidth;   //ͼ����
	int m_nHeight;   //ͼ��߶�
	float *m_pData;    
	
	//����������
	float m_fVein2Mom;   //������׾�
	float m_fEntropy;   //��
	float m_fContrast;   //�Աȶ�
	float m_fHomogen;   //������
};

#endif // !defined(AFX_VEINS_H__4D8B2247_4002_4CC3_A299_6FC2885BE6DC__INCLUDED_)

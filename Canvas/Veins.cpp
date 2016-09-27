// Veins.cpp: implementation of the CVeins class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "math.h"
#include "Veins.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define AccrMatrixInd 256

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVeins::CVeins()
{
//	m_pData = NULL;
//	m_pImage = NULL;
//	m_pImageGreyMatrix = NULL;
//	m_VeinStatInfor.pLenAreRadio = NULL;
}

CVeins::CVeins(CImage& image)
{
	m_pData = NULL;
	m_pImage = NULL;
	m_pImageGreyMatrix = NULL;
	m_VeinStatInfor.pLenAreRadio = NULL;
	m_pImage = new CImage(image);
}

CVeins::~CVeins()
{
	if (m_pData != NULL)
		delete [] m_pData;
	
	if (m_pImage != NULL)	
		delete  m_pImage;
	
	if (m_pImageGreyMatrix != NULL)
		delete [] m_pImageGreyMatrix;

	if (m_VeinStatInfor.pLenAreRadio != NULL)
		delete [] m_VeinStatInfor.pLenAreRadio;
}

// �Ҷȹ�������
bool CVeins::CreatAccrMatrix()
{
	ASSERT(m_pImage != NULL);
	// ��ȡͼ���Ⱥ͸߶�
	m_nWidth = m_pImage->GetWidth();
	m_nHeight = m_pImage->GetHeight();
	
	// ��̬�ڴ����
	m_pImageGreyMatrix = new UCHAR[m_nWidth*m_nHeight];   

	RGBQUAD rgb;
 
	// ��ȡͼ�����ص�Ҷ�ֵ 	
	for(int i = 0; i < m_nHeight; i++)
	{
		for(int j = 0; j < m_nWidth; j++)
		{
			// ȡ��(j,i)������
			rgb = m_pImage->GetPixelColor(j, i); 
			// �������ص�Ҷ�ֵ
			*(m_pImageGreyMatrix + i*m_nWidth + j) = (rgb.rgbBlue+rgb.rgbGreen+rgb.rgbRed)/3;   
		}
	}

	m_pData = new float[AccrMatrixInd*AccrMatrixInd];
	
	// ��ʼ������
	memset(m_pData, 0, sizeof(float)*AccrMatrixInd*AccrMatrixInd);
	
	UCHAR *p1;
	UCHAR *p2;
	p1 = m_pImageGreyMatrix;
	p2 = p1+m_nWidth+1;
	
	// ����λ������ƥ����ֵĴ���ֵ
	for (int sr=0; sr<m_nHeight-1; sr++)   
	{
		for (int sc=0; sc<m_nWidth-1 ; sc++)  
		{
			(*(m_pData+(*p1)*AccrMatrixInd+(*p2)))++;
			p1++;
			p2++;
		}
		p1++;
		p2++;
	}
	
	int nSum;
	// ����Ŀ�����Ԫ��ֵ�ĺ�
	nSum = (m_nHeight-1)*(m_nWidth-1);
	
	// ���ɹ�������	
	float *p = m_pData;
	for (int k=0; k < AccrMatrixInd*AccrMatrixInd; k++)
	{
		*p /= nSum;
		p ++;
	}
	
	return TRUE;   
}

// ������׾�
float CVeins::Vein2Mom()
{
	m_fVein2Mom = 0.0;
	
	// �жϹ�������Ԫ�ض���0ֵ
	if (m_nWidth*m_nHeight == 0)
	{
		return 0;
	}
	
	// ����������׾�ֵ
	else
	{
		float *p = m_pData;
		for (int i = 0; i < AccrMatrixInd*AccrMatrixInd; i++)
		{
			// ������׾ؼ��㹫ʽ
 			m_fVein2Mom += (*p) * (*p);
			p++;
		}
	}
    
	return m_fVein2Mom;   	
}

// ��ֵ
float CVeins::Entropy()
{
	m_fEntropy = 0;
	// �жϹ�������Ԫ���Ƿ���0ֵ
	if (m_nWidth*m_nHeight == 0)
	{
		return 0;
	}
	
	// ������ֵ
	else
	{
		float *p = m_pData;
		for (int i = 0; i < AccrMatrixInd*AccrMatrixInd; i++)
		{
			// ��ֵ���㹫ʽ
			if (*p != 0)			
				m_fEntropy -= (*p) * ((float)log10(*p));		
			
			p++;
		}		
	}

	return m_fEntropy; 
}

// ����Աȶ�
float CVeins::Contrast()
{
	m_fContrast = 0;
	// �жϹ�������Ԫ���Ƿ���0ֵ
	if (m_nWidth*m_nHeight == 0)
	{
		return 0;
	}
	
	// ����������ڷ���ֵʱ����Աȶ�
	else
	{
		float *p = m_pData;
		for (int dr=0; dr<AccrMatrixInd; dr++)
		{
			for (int dc=0; dc<AccrMatrixInd; dc++)
			{
				
				if ((*p != 0) && (dr != dc))
					// ����Աȶȹ�ʽ
					m_fContrast += (*p) * abs(dc-dr);
				
				p++;
			}
		}
	}

	return m_fContrast;   
}

// ����ֵ
float CVeins::Homogen(float k)
{
	// �жϹ�������Ԫ���Ƿ���0ֵ
	if (m_nWidth*m_nHeight == 0)
	{
		return 0;
	}
	
	// ����������ڷ�0ֵʱ�������ֵ
	else
	{
		m_fHomogen = 0;
		float *p = m_pData;		
		for (int dr=0; dr<AccrMatrixInd; dr++)
		{
			for (int dc=0; dc<AccrMatrixInd; dc++)
			{				
				if ((*p) != 0.0)				
					// �������ֵ��ʽ
					m_fHomogen += (*p) /(k+abs(dc-dr)); 			
				
				p++;
			}
		}
	}

	return m_fHomogen;   
}

// ����ͳ����Ϣ
VeinStatInfor CVeins::GrainContentDis()
{
	ASSERT(m_pImage != NULL);
	
	/*UCHAR uObjClr;*/
	// Ŀ��ı߽������Ŀ���γ̱������ռ� 
	ObjEdgeList* pSrcEdgeLst = new ObjEdgeList;
	ObjRunLenCode* pSrcObjRunLenCode = new ObjRunLenCode;

	// ȡ������Ŀ��ı߽�����(��߽簴8��������)
	m_pImage->ObjAllEdgeLst(pSrcEdgeLst);
	// ȡ������Ŀ���γ̱���(��߽簴8��������)
    m_pImage->ObjAllRLC(pSrcObjRunLenCode);
	
	// ������б߽��ָ��
	PPEDGE_OBJECT ppEgeObj = pSrcEdgeLst->GetObjAllEdgeLst();
	
	// Ŀ�����
	int nObjNum = pSrcObjRunLenCode->GetObjNum();
	/*int nObjNum = pSrcEdgeLst->GetObjNum();*/
	
	// ����Ŀ�������Ŀ��߽糤�ȡ�Ŀ��߽��������ͳ�������ȵĴ洢�ռ�
	long* pObjArea = new long[nObjNum];
	long* pObjEdgeLen = new long[nObjNum];
	long* pObjEgePixNum = new long[nObjNum];
	m_VeinStatInfor.pLenAreRadio = new float[nObjNum];	

	// ��ʼ��
	memset(pObjEdgeLen, 0, sizeof(long)*nObjNum);
	memset(m_VeinStatInfor.GrainCont.nGroupObjNum, 0, sizeof(int)*4);
	memset(m_VeinStatInfor.GrainCont.nGroupObjAreaSum, 0, sizeof(long)*4);
	
	int i, j, nTemp;
	// ��߽簴8��������ʱ�������ص�֮�����������1����1.414�����ص�߳�, ����(x1, y1)��(x2,y2)Ϊ��������,
	// (x2-x1)*(y2-y1)��ֵ��-1,0,1,�����������(x2-x1)*(y2-y1)Ϊ-1,1ʱ���������ص�֮�����Ϊ1.414�����ص㣻
	// ��(x2-x1)*(y2-y1)Ϊ0ʱ���������ص�֮�����Ϊ1�����ص� ��
	double a[3] = {1.414,1.0,1.414};	
	
	for (i=0; i<nObjNum; i++)
	{
		// Ŀ�����
		pObjArea[i] = pSrcObjRunLenCode->ObjArea(i);		
		
		pObjEgePixNum[i] = ppEgeObj[i]->ppEdgeLst[0]->nEdgeLen;
        for (j=0; j<pObjEgePixNum[i]-1; j++)
        {
			nTemp = (ppEgeObj[i]->ppEdgeLst[0]->edge[j+1].x - ppEgeObj[i]->ppEdgeLst[0]->edge[j].x)\
				*(ppEgeObj[i]->ppEdgeLst[0]->edge[j+1].y - ppEgeObj[i]->ppEdgeLst[0]->edge[j].y);			
			
			pObjEdgeLen[i] += a[nTemp+1];
        }
		nTemp = (ppEgeObj[i]->ppEdgeLst[0]->edge[0].x - ppEgeObj[i]->ppEdgeLst[0]->edge[pObjEgePixNum[i]-1].x)\
				*(ppEgeObj[i]->ppEdgeLst[0]->edge[0].y - ppEgeObj[i]->ppEdgeLst[0]->edge[pObjEgePixNum[i]-1].y);
		// Ŀ��߽糤��
		pObjEdgeLen[i] += a[nTemp+1];
		
        // Ŀ��߽糤���������
		m_VeinStatInfor.pLenAreRadio[i] = pObjEdgeLen[i]/pObjArea[i];
		
		// ͳ��ÿ����������������
		nTemp = (int) pObjArea[i]/100;
		switch(nTemp)
		{
		case 0:
			m_VeinStatInfor.GrainCont.nGroupObjNum[0]++;
			m_VeinStatInfor.GrainCont.nGroupObjAreaSum[0] += pObjArea[i];
			break;
		case 1:
			m_VeinStatInfor.GrainCont.nGroupObjNum[1]++;
			m_VeinStatInfor.GrainCont.nGroupObjAreaSum[1] += pObjArea[i];
			break;
		case 2:
		case 3:
		case 4:
			m_VeinStatInfor.GrainCont.nGroupObjNum[2]++;
			m_VeinStatInfor.GrainCont.nGroupObjAreaSum[2] += pObjArea[i];
			break;
		default:
			m_VeinStatInfor.GrainCont.nGroupObjNum[3]++;
			m_VeinStatInfor.GrainCont.nGroupObjAreaSum[3] += pObjArea[i];
			break;
		}		
	}
	
	// �����������
	for (i=0; i<4; i++)
	{
		m_VeinStatInfor.GrainCont.fGrainContent[i] = m_VeinStatInfor.GrainCont.nGroupObjAreaSum[i]/(m_nWidth*m_nHeight);
	}
				
	delete [] pObjArea;
	delete [] pObjEdgeLen;
	delete [] pObjEgePixNum;
	delete pSrcEdgeLst;
    delete pSrcObjRunLenCode;
	
	return m_VeinStatInfor;
}

void CVeins::CreateCoMatrix()
{
	ASSERT(m_pImage != NULL);

	int i,j;

	// ��ȡͼ���Ⱥ͸߶�
	int nWidth = m_pImage->GetWidth();
	int nHeight = m_pImage->GetHeight();
	
	memset(m_AccrMatrix, 0, sizeof(float)*3*256*256);
	
	RGBQUAD rgb1, rgb2;
	// ����λ������ƥ����ֵĴ���ֵ
	for (i=0; i<nWidth-1; i++)   
	{
		for (j=0; j<nHeight-1; j++)  
		{
			// ȡ��(j,i)������
			rgb1 = m_pImage->GetPixelColor(i, j);
			rgb2 = m_pImage->GetPixelColor(i+1, j+1);

			m_AccrMatrix[0][rgb1.rgbRed][rgb2.rgbRed] ++;
			m_AccrMatrix[1][rgb1.rgbGreen][rgb2.rgbGreen] ++;
			m_AccrMatrix[2][rgb1.rgbBlue][rgb2.rgbBlue] ++;
		}
	}

	int nSum;
	// ����Ŀ�����Ԫ��ֵ�ĺ�
	nSum = (nHeight-1)*(nWidth-1);

	// ���ɹ�������	
	for (i=0; i<256; i++)
		for (j=0; j<256; j++)
		{
			m_AccrMatrix[0][i][j] /= nSum;
			m_AccrMatrix[1][i][j] /= nSum;
			m_AccrMatrix[2][i][j] /= nSum;
		}
}

// ������׾�
float CVeins::Vein2Mom(int nIdx)
{
	float fVein2Mom = 0.0;
	
	// ����������׾�ֵ
	for (int i=0; i<AccrMatrixInd; i++)
	{
		for (int j=0; j<AccrMatrixInd; j++)
		{
			// ������׾ؼ��㹫ʽ
 			fVein2Mom += m_AccrMatrix[nIdx][i][j]*m_AccrMatrix[nIdx][i][j];
		}
	}
    
	return fVein2Mom;   	
}

// ��ֵ
float CVeins::Entropy(int nIdx)
{
	float fEntropy = 0;

	for (int i=0; i<AccrMatrixInd; i++)
	{
		for (int j=0; j<AccrMatrixInd; j++)
		{
			// ��ֵ���㹫ʽ
			if (m_AccrMatrix[nIdx][i][j] != 0)
			fEntropy -= m_AccrMatrix[nIdx][i][j] * ((float)log10(m_AccrMatrix[nIdx][i][j]));		
		}
	}

	return fEntropy; 
}

// ����Աȶ�
float CVeins::Contrast(int nIdx)
{
	float fContrast = 0;
	// ����������ڷ���ֵʱ����Աȶ�
	for (int i=0; i<AccrMatrixInd; i++)
	{
		for (int j=0; j<AccrMatrixInd; j++)
		{
			// ����Աȶȹ�ʽ
			fContrast += (m_AccrMatrix[nIdx][i][j]) * abs(i-j);
		}
	}

	return fContrast;   
}

// ����ֵ
float CVeins::Homogen(int nIdx, float k)
{
	float fHomogen = 0.0;

	// ����������ڷ�0ֵʱ�������ֵ
	for (int i=0; i<AccrMatrixInd; i++)
	{
		for (int j=0; j<AccrMatrixInd; j++)
		{
			// �������ֵ��ʽ
			fHomogen += m_AccrMatrix[nIdx][i][j] /(k+abs(i-j));
		}
	}

	return fHomogen;   
}

//��Ƶ
double CVeins::RowFrequency(int nL)
{
	int i,j, nDiff;
	double dbRl = 0.0;

	int nWidth = m_pImage->GetWidth();
	int nHeight = m_pImage->GetHeight();

	for (i=0; i<nWidth; i++)
	{
		for (j=0; j<nHeight-nL; j++)
		{
			nDiff = m_pImageGreyMatrix[j*nWidth+i]-m_pImageGreyMatrix[(j+nL)*nWidth+i];
			dbRl += nDiff*nDiff;
		}
	}

	dbRl /= nWidth*(nHeight-nL);

	return sqrt(dbRl);
}

//��Ƶ
double CVeins::ColFrequency(int nL)
{
	int i,j, nDiff;
	double dbRl = 0.0;

	int nWidth = m_pImage->GetWidth();
	int nHeight = m_pImage->GetHeight();

	for (i=0; i<nWidth-nL; i++)
	{
		for (j=0; j<nHeight; j++)
		{
			nDiff = m_pImageGreyMatrix[j*nWidth+i]-m_pImageGreyMatrix[j*nWidth+i+nL];
			dbRl += nDiff*nDiff;
		}
	}

	dbRl /= (nWidth-nL)*nHeight;

	return sqrt(dbRl);
}

// left diagonal frequency ��Խ�Ƶ
double CVeins::LDFrequency(int nL)
{
	int i,j, nDiff;
	double dbRl = 0.0;

	int nWidth = m_pImage->GetWidth();
	int nHeight = m_pImage->GetHeight();

	for (i=nL; i<nWidth; i++)
	{
		for (j=0; j<nHeight-nL; j++)
		{
			nDiff = m_pImageGreyMatrix[j*nWidth+i]-m_pImageGreyMatrix[(j+nL)*nWidth+i-nL];
			dbRl += nDiff*nDiff;
		}
	}

	dbRl /= (nWidth-nL)*(nHeight-nL);

	return sqrt(dbRl);
}

// right diagonal frequency �ҶԽ�Ƶ
double CVeins::RDFrequency(int nL)
{
	int i,j, nDiff;
	double dbRl = 0.0;

	int nWidth = m_pImage->GetWidth();
	int nHeight = m_pImage->GetHeight();

	for (i=0; i<nWidth-nL; i++)
	{
		for (j=0; j<nHeight-nL; j++)
		{
			nDiff = m_pImageGreyMatrix[j*nWidth+i]-m_pImageGreyMatrix[(j+nL)*nWidth+i+nL];
			dbRl += nDiff*nDiff;
		}
	}

	dbRl /= (nWidth-nL)*(nHeight-nL);

	return sqrt(dbRl);
}

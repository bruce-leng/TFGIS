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

// 灰度共生矩阵
bool CVeins::CreatAccrMatrix()
{
	ASSERT(m_pImage != NULL);
	// 获取图像宽度和高度
	m_nWidth = m_pImage->GetWidth();
	m_nHeight = m_pImage->GetHeight();
	
	// 动态内存分配
	m_pImageGreyMatrix = new UCHAR[m_nWidth*m_nHeight];   

	RGBQUAD rgb;
 
	// 获取图像像素点灰度值 	
	for(int i = 0; i < m_nHeight; i++)
	{
		for(int j = 0; j < m_nWidth; j++)
		{
			// 取点(j,i)的像素
			rgb = m_pImage->GetPixelColor(j, i); 
			// 计算像素点灰度值
			*(m_pImageGreyMatrix + i*m_nWidth + j) = (rgb.rgbBlue+rgb.rgbGreen+rgb.rgbRed)/3;   
		}
	}

	m_pData = new float[AccrMatrixInd*AccrMatrixInd];
	
	// 初始化矩阵
	memset(m_pData, 0, sizeof(float)*AccrMatrixInd*AccrMatrixInd);
	
	UCHAR *p1;
	UCHAR *p2;
	p1 = m_pImageGreyMatrix;
	p2 = p1+m_nWidth+1;
	
	// 符合位置算子匹配出现的次数值
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
	// 计算目标矩阵元素值的和
	nSum = (m_nHeight-1)*(m_nWidth-1);
	
	// 生成共生矩阵	
	float *p = m_pData;
	for (int k=0; k < AccrMatrixInd*AccrMatrixInd; k++)
	{
		*p /= nSum;
		p ++;
	}
	
	return TRUE;   
}

// 纹理二阶矩
float CVeins::Vein2Mom()
{
	m_fVein2Mom = 0.0;
	
	// 判断共生矩阵元素都是0值
	if (m_nWidth*m_nHeight == 0)
	{
		return 0;
	}
	
	// 计算纹理二阶矩值
	else
	{
		float *p = m_pData;
		for (int i = 0; i < AccrMatrixInd*AccrMatrixInd; i++)
		{
			// 纹理二阶矩计算公式
 			m_fVein2Mom += (*p) * (*p);
			p++;
		}
	}
    
	return m_fVein2Mom;   	
}

// 熵值
float CVeins::Entropy()
{
	m_fEntropy = 0;
	// 判断共生矩阵元素是否都是0值
	if (m_nWidth*m_nHeight == 0)
	{
		return 0;
	}
	
	// 计算熵值
	else
	{
		float *p = m_pData;
		for (int i = 0; i < AccrMatrixInd*AccrMatrixInd; i++)
		{
			// 熵值计算公式
			if (*p != 0)			
				m_fEntropy -= (*p) * ((float)log10(*p));		
			
			p++;
		}		
	}

	return m_fEntropy; 
}

// 纹理对比度
float CVeins::Contrast()
{
	m_fContrast = 0;
	// 判断共生矩阵元素是否都是0值
	if (m_nWidth*m_nHeight == 0)
	{
		return 0;
	}
	
	// 共生矩阵存在非零值时计算对比度
	else
	{
		float *p = m_pData;
		for (int dr=0; dr<AccrMatrixInd; dr++)
		{
			for (int dc=0; dc<AccrMatrixInd; dc++)
			{
				
				if ((*p != 0) && (dr != dc))
					// 计算对比度公式
					m_fContrast += (*p) * abs(dc-dr);
				
				p++;
			}
		}
	}

	return m_fContrast;   
}

// 均匀值
float CVeins::Homogen(float k)
{
	// 判断共生矩阵元素是否都是0值
	if (m_nWidth*m_nHeight == 0)
	{
		return 0;
	}
	
	// 共生矩阵存在非0值时计算均匀值
	else
	{
		m_fHomogen = 0;
		float *p = m_pData;		
		for (int dr=0; dr<AccrMatrixInd; dr++)
		{
			for (int dc=0; dc<AccrMatrixInd; dc++)
			{				
				if ((*p) != 0.0)				
					// 计算均匀值公式
					m_fHomogen += (*p) /(k+abs(dc-dr)); 			
				
				p++;
			}
		}
	}

	return m_fHomogen;   
}

// 纹理统计信息
VeinStatInfor CVeins::GrainContentDis()
{
	ASSERT(m_pImage != NULL);
	
	/*UCHAR uObjClr;*/
	// 目标的边界链码和目标游程编码分配空间 
	ObjEdgeList* pSrcEdgeLst = new ObjEdgeList;
	ObjRunLenCode* pSrcObjRunLenCode = new ObjRunLenCode;

	// 取得所有目标的边界链码(外边界按8邻域搜索)
	m_pImage->ObjAllEdgeLst(pSrcEdgeLst);
	// 取得所有目标游程编码(外边界按8邻域搜索)
    m_pImage->ObjAllRLC(pSrcObjRunLenCode);
	
	// 获得所有边界的指针
	PPEDGE_OBJECT ppEgeObj = pSrcEdgeLst->GetObjAllEdgeLst();
	
	// 目标个数
	int nObjNum = pSrcObjRunLenCode->GetObjNum();
	/*int nObjNum = pSrcEdgeLst->GetObjNum();*/
	
	// 分配目标面积、目标边界长度、目标边界像素数和长度面积比的存储空间
	long* pObjArea = new long[nObjNum];
	long* pObjEdgeLen = new long[nObjNum];
	long* pObjEgePixNum = new long[nObjNum];
	m_VeinStatInfor.pLenAreRadio = new float[nObjNum];	

	// 初始化
	memset(pObjEdgeLen, 0, sizeof(long)*nObjNum);
	memset(m_VeinStatInfor.GrainCont.nGroupObjNum, 0, sizeof(int)*4);
	memset(m_VeinStatInfor.GrainCont.nGroupObjAreaSum, 0, sizeof(long)*4);
	
	int i, j, nTemp;
	// 外边界按8邻域搜索时相邻像素点之间有两种情况1倍和1.414倍像素点边长, 例如(x1, y1)和(x2,y2)为相邻像素,
	// (x2-x1)*(y2-y1)的值有-1,0,1,三种情况。当(x2-x1)*(y2-y1)为-1,1时，相邻像素点之间距离为1.414倍像素点；
	// 当(x2-x1)*(y2-y1)为0时，相邻像素点之间距离为1倍像素点 。
	double a[3] = {1.414,1.0,1.414};	
	
	for (i=0; i<nObjNum; i++)
	{
		// 目标面积
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
		// 目标边界长度
		pObjEdgeLen[i] += a[nTemp+1];
		
        // 目标边界长度与面积比
		m_VeinStatInfor.pLenAreRadio[i] = pObjEdgeLen[i]/pObjArea[i];
		
		// 统计每组面积个数与面积和
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
	
	// 各组颗粒含量
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

	// 获取图像宽度和高度
	int nWidth = m_pImage->GetWidth();
	int nHeight = m_pImage->GetHeight();
	
	memset(m_AccrMatrix, 0, sizeof(float)*3*256*256);
	
	RGBQUAD rgb1, rgb2;
	// 符合位置算子匹配出现的次数值
	for (i=0; i<nWidth-1; i++)   
	{
		for (j=0; j<nHeight-1; j++)  
		{
			// 取点(j,i)的像素
			rgb1 = m_pImage->GetPixelColor(i, j);
			rgb2 = m_pImage->GetPixelColor(i+1, j+1);

			m_AccrMatrix[0][rgb1.rgbRed][rgb2.rgbRed] ++;
			m_AccrMatrix[1][rgb1.rgbGreen][rgb2.rgbGreen] ++;
			m_AccrMatrix[2][rgb1.rgbBlue][rgb2.rgbBlue] ++;
		}
	}

	int nSum;
	// 计算目标矩阵元素值的和
	nSum = (nHeight-1)*(nWidth-1);

	// 生成共生矩阵	
	for (i=0; i<256; i++)
		for (j=0; j<256; j++)
		{
			m_AccrMatrix[0][i][j] /= nSum;
			m_AccrMatrix[1][i][j] /= nSum;
			m_AccrMatrix[2][i][j] /= nSum;
		}
}

// 纹理二阶矩
float CVeins::Vein2Mom(int nIdx)
{
	float fVein2Mom = 0.0;
	
	// 计算纹理二阶矩值
	for (int i=0; i<AccrMatrixInd; i++)
	{
		for (int j=0; j<AccrMatrixInd; j++)
		{
			// 纹理二阶矩计算公式
 			fVein2Mom += m_AccrMatrix[nIdx][i][j]*m_AccrMatrix[nIdx][i][j];
		}
	}
    
	return fVein2Mom;   	
}

// 熵值
float CVeins::Entropy(int nIdx)
{
	float fEntropy = 0;

	for (int i=0; i<AccrMatrixInd; i++)
	{
		for (int j=0; j<AccrMatrixInd; j++)
		{
			// 熵值计算公式
			if (m_AccrMatrix[nIdx][i][j] != 0)
			fEntropy -= m_AccrMatrix[nIdx][i][j] * ((float)log10(m_AccrMatrix[nIdx][i][j]));		
		}
	}

	return fEntropy; 
}

// 纹理对比度
float CVeins::Contrast(int nIdx)
{
	float fContrast = 0;
	// 共生矩阵存在非零值时计算对比度
	for (int i=0; i<AccrMatrixInd; i++)
	{
		for (int j=0; j<AccrMatrixInd; j++)
		{
			// 计算对比度公式
			fContrast += (m_AccrMatrix[nIdx][i][j]) * abs(i-j);
		}
	}

	return fContrast;   
}

// 均匀值
float CVeins::Homogen(int nIdx, float k)
{
	float fHomogen = 0.0;

	// 共生矩阵存在非0值时计算均匀值
	for (int i=0; i<AccrMatrixInd; i++)
	{
		for (int j=0; j<AccrMatrixInd; j++)
		{
			// 计算均匀值公式
			fHomogen += m_AccrMatrix[nIdx][i][j] /(k+abs(i-j));
		}
	}

	return fHomogen;   
}

//行频
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

//列频
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

// left diagonal frequency 左对角频
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

// right diagonal frequency 右对角频
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

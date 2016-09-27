// MultiSeg.cpp: implementation of the CMultiSeg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "canvas.h"
#include "MultiSeg.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMultiSeg::CMultiSeg()
{
	m_pGraph = NULL;
}

CMultiSeg::CMultiSeg(CGraph& graph)
{
	m_pGraph = new CGraph(graph);
}

CMultiSeg::~CMultiSeg()
{
	if (m_pGraph != NULL)
		delete m_pGraph;
}

// 获取断面不同产状组数
int CMultiSeg::GetGroupNum()
{
	ASSERT(m_pGraph != NULL);
	
	int i, j, k, nGroupNum, nStructNum, nLinePointNum, nStructSum;
	float f_xSum, f_ySum, f_xySum, f_xxSum ;
	vector <int> vGroupStructInd;
	
	CUnit *pUnit = NULL;	
    
	// 初始化
	nGroupNum = 0;	
	nStructNum = 0;
	
	// 结构面总数
    nStructSum = m_pGraph->Size(); 
	ASSERT(nStructSum > 0);
	
	// 分配结构面所拟合线性方程参数存储空间
	float *pSlope = new float[nStructSum];	   
	float *pSita = new float[nStructSum];	
	
	for (i=0; i<nStructSum; i++)
	{
		f_xSum = f_ySum = f_xySum = f_xxSum = 0.0;
		pUnit = m_pGraph->GetUnitByIdx(i);
		// 结构面多段线顶点数目
		nLinePointNum = pUnit->m_pPointList->GetSize(); 	
		ASSERT(nLinePointNum > 1);
		
		// 结构面线性拟合特征数据
		for (j=0; j<nLinePointNum; j++)
		{
			POINT2DF pt = pUnit->m_pPointList->GetAt(j);
			
			f_xSum += pt.x;
			f_ySum += pt.y;
			f_xySum += pt.x*pt.y;
			f_xxSum += pt.x*pt.x;
		}
		
		if (abs(f_xSum*f_xSum - nLinePointNum*f_xxSum) > 0.000001)
		{
			// 结构面所拟合线性方程斜率
			pSlope[i] = (f_xSum*f_ySum - nLinePointNum*f_xySum)/(f_xSum*f_xSum - nLinePointNum*f_xxSum);
			
			// 结构面倾角			
			if((int)(pSita[i] = atan(pSlope[i])*180/3.1416) < 0)
			{
				pSita[i] = 180 + pSita[i];
			}
		}
		
		// 结构面倾角
		else
			pSita[i] = 90;
	}
	
	// 统计结构面组数		
	for (i=0; i<nStructSum; i++)
	{
		// 判别当前索引值结构面与比当前结构面索引值小的结构面是否平行
		for(k=0; k<i&&i<nStructSum; k++)
		{	
			if ((int)abs(pSita[i] - pSita[k]) < 10)
			{
				i++;
				k = 0;
			}
		}
		
		nStructNum++;
		vGroupStructInd.push_back(i);
		
		// 判别比当前索引值高的结构面与当前索引值结构面是否平行
		for (j=i+1; j<nStructSum; j++)
		{
			if((int)abs(pSita[i] - pSita[j]) < 10)
			{
				nStructNum++;
				vGroupStructInd.push_back(j);
				
				if (j==i+1)				
					i++;				
			}
		}
		
		// 记录结构面组数
		nGroupNum++;
		m_vvGroup.push_back(vGroupStructInd);
		vGroupStructInd.clear();
		
		if (nStructNum >= nStructSum)
			break;
	}
	
	delete[] pSlope;	
	delete[] pSita;	
	
	return nGroupNum;	
}

// 获取结构面倾角
float CMultiSeg::GetArentDip(int nStructInd)
{
	ASSERT(m_pGraph != NULL);
	ASSERT(nStructInd >= 0);
	
	int i, nLinePointNum;
	float f_xSum, f_ySum, f_xySum, f_xxSum, fSlope, fSita;
	
	f_xSum = f_ySum = f_xySum = f_xxSum = 0.0;
	
	CUnit *pUnit = NULL;
	
	// 指定多段线
	pUnit = m_pGraph->GetUnitByIdx(nStructInd);
	// 多段线顶点数目
	nLinePointNum = pUnit->m_pPointList->GetSize();
	ASSERT(nLinePointNum > 1);
	
	// 结构面线性拟合特征数据
	for (i=0; i<nLinePointNum; i++)
	{
		POINT2DF pt = pUnit->m_pPointList->GetAt(i);
		
		f_xSum += pt.x;
		f_ySum += pt.y;
		f_xySum += pt.x*pt.y;
		f_xxSum += pt.x*pt.x;
	}
	
	// 结构面倾角
	if (abs(f_xSum*f_xSum - nLinePointNum*f_xxSum) > 0.000001)
	{
		// 结构面所拟合线性方程斜率
		fSlope = (f_xSum*f_ySum - nLinePointNum*f_xySum)/(f_xSum*f_xSum - nLinePointNum*f_xxSum);
		
		fSita = atan(fSlope)*180/3.1416;
		
		if(fSita > 0)
		{
			return fSita;
		}
		
		else
		{
			return (180+fSita);
		}
	}
	
	else
	{
		return 90;
	}	
}

// 获取结构面平均间距
// 未考虑同组内平均角度不为90度，但包含个别结构面角度为90度的情况
float CMultiSeg::GetAveDistance(int nGroupIdx, float fScale)
{
	ASSERT(m_pGraph != NULL);
	ASSERT(nGroupIdx >= 0 && fScale > 1);
	
	int i, j, nGroupStructNum, nLinePointNum;
	float f_xSum, f_ySum, f_xySum, f_xxSum;
	float fAveDistance, fInterceptMax, fInterceptMin, fAveAngle, fAngleSum;	
    
	CUnit *pUnit = NULL;
	fAngleSum = 0.0;
	
    // 同组结构面个数
	nGroupStructNum = m_vvGroup[nGroupIdx].size();
	ASSERT(nGroupStructNum > 0);
	
	if(nGroupStructNum < 2)
	{
		return 0;	
	}

	// 分配拟合线性方程的参数存储空间
	float *pAngle = new float[nGroupStructNum];
	float *pSlope = new float[nGroupStructNum];
	float *pIntercept = new float[nGroupStructNum];

	// 计算结构面拟合线性方程斜率、截距和倾角
	for (i=0; i<nGroupStructNum; i++)
	{
		f_xSum = f_ySum = f_xySum = f_xxSum = 0.0;
		// 指定多段线 
		pUnit = m_pGraph->GetUnitByIdx(m_vvGroup[nGroupIdx][i]);
		// 多段线顶点数目
		nLinePointNum = pUnit->m_pPointList->GetSize();
		ASSERT(nLinePointNum > 1);		
		
		// 结构面线性拟合特征数据
		for (j=0; j<nLinePointNum; j++)
		{
			POINT2DF pt = pUnit->m_pPointList->GetAt(j);
            
			f_xSum += pt.x;
			f_ySum += pt.y;
			f_xySum += pt.x*pt.y;
			f_xxSum += pt.x*pt.x;
		}
		
		if (abs(f_xSum*f_xSum - nLinePointNum*f_xxSum)>0.000001)
		{
			// 结构面所拟合线性方程斜率
			pSlope[i] = (f_xSum*f_ySum - nLinePointNum*f_xySum)/(f_xSum*f_xSum - nLinePointNum*f_xxSum);
			// 结构面倾角
			if (pSlope[i] >= 0)
			{
				pAngle[i] = atan(pSlope[i])*180/3.1416;
			}
			else
			{
				pAngle[i] = 180 + atan(pSlope[i])*180/3.1416;
			}

			// 结构面所拟合线性方程的截距
			pIntercept[i] = (f_ySum - f_xSum*pSlope[i])/nLinePointNum;
		}
		else
		{
			pAngle[i] = 90;
		}
		
		// 同组结构面倾角和
		fAngleSum += pAngle[i];
	}

	// 同组结构面倾角平均值
	fAveAngle = fAngleSum/nGroupStructNum;
	
	// 同组结构面平均间距
	if (abs(fAveAngle-90) > 0.000001)
	{
		// 初始化最大和最小截距值
		for (i=0; i<nGroupStructNum; i++)
		{
			if (abs(pAngle[i]-90)>0.000001)
			{
				fInterceptMax = fInterceptMin = pIntercept[i];
				break;	
			}
		}

		// 同组结构面线性拟合方程中最大截距值和最小截距值
		for (j=i+1; j<nGroupStructNum; j++)
		{	
			if ((pIntercept[j] > fInterceptMax) && (abs(pAngle[j]-90)>0.000001))
			{
				fInterceptMax = pIntercept[j];
			}
			else if((pIntercept[j] < fInterceptMin) && (abs(pAngle[j]-90)>0.000001))
			{
				fInterceptMin = pIntercept[j];
			}
		}		
		
		fAveDistance = fScale * (fInterceptMax-fInterceptMin)/(nGroupStructNum-1)/sqrt(1+atan(fAveAngle)*atan(fAveAngle));
	}

	else
	{
		float *px = new float[nGroupStructNum];

		for (i=0; i<nGroupStructNum; i++)
		{
			if (abs(pAngle[i]-90) > 0.000001)
			{
				px[i] = -pIntercept[i]/pSlope[i];
			}

			else
			{
				f_xSum = 0.0;
				pUnit = m_pGraph->GetUnitByIdx(m_vvGroup[nGroupIdx][i]);
				// 多段线顶点数目
				nLinePointNum = pUnit->m_pPointList->GetSize();
				for (j=0; j<nLinePointNum; j++)
				{
					POINT2DF pt = pUnit->m_pPointList->GetAt(j);					
					f_xSum += pt.x;
				}
				px[i] = f_xSum/nLinePointNum;
			}
		}
        
		// 最大和最小横坐标值
		float fMax, fMin;
		fMax = fMin = px[0];

		for (i=1; i<nGroupStructNum; i++)
		{
			if (fMax < px[i])
			{
				fMax = px[i];
			}
			if (fMin > px[i])
			{
				fMin = px[i];
			}
		}
		// 同组结构面平均间距
		fAveDistance = fScale*(fMax-fMin)/(nGroupStructNum-1);

		delete [] px;
	}	
	
	delete [] pAngle;
	delete [] pSlope;
	delete [] pIntercept;
	
	return fAveDistance;	
}

// 结构面延伸长度
float CMultiSeg::GetMultiSegLength(int nStructIdx, float fScale)
{
	ASSERT(m_pGraph != NULL);
	ASSERT(nStructIdx >= 0 && fScale > 1);
	
	
	int i, nLinePointNum;
	float fLength;
	
	fLength = 0.0;

	CUnit *pUnit = NULL;
    
	// 指定结构面
	pUnit = m_pGraph->GetUnitByIdx(nStructIdx);
	// 多段线顶点数
	nLinePointNum = pUnit->m_pPointList->GetSize();
	ASSERT(nLinePointNum > 0);
    
	// 计算结构面延伸长度
	for (i=0; i<nLinePointNum-1; i++)
	{
		POINT2DF pt1 = pUnit->m_pPointList->GetAt(i);
		POINT2DF pt2 = pUnit->m_pPointList->GetAt(i+1);
		fLength += sqrt((pt1.x-pt2.x)*(pt1.x-pt2.x) + (pt1.y-pt2.y)*(pt1.y-pt2.y));
	}

	return fLength*fScale;
}

// 同组多段线平均长度
float CMultiSeg::GetAveLength(int nGroupIdx, float fScale)
{
	ASSERT(m_pGraph != NULL);
	ASSERT(nGroupIdx >= 0 && fScale > 1);
    
	float fLength;
	int i, nGroupStructNum;
	
	// 同组结构面组数
	nGroupStructNum = m_vvGroup[nGroupIdx].size();
	/*float *pLength = new float[nGroupStructNum];*/
	
	fLength = 0.0;
	for (i=0; i<nGroupStructNum; i++)
	{
		// 指定结构面
		fLength += GetMultiSegLength(m_vvGroup[nGroupIdx][i], fScale);
	}
	// 返回同组多段线平均长度
	return fLength*fScale/nGroupStructNum;	
}
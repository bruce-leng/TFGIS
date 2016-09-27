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

// ��ȡ���治ͬ��״����
int CMultiSeg::GetGroupNum()
{
	ASSERT(m_pGraph != NULL);
	
	int i, j, k, nGroupNum, nStructNum, nLinePointNum, nStructSum;
	float f_xSum, f_ySum, f_xySum, f_xxSum ;
	vector <int> vGroupStructInd;
	
	CUnit *pUnit = NULL;	
    
	// ��ʼ��
	nGroupNum = 0;	
	nStructNum = 0;
	
	// �ṹ������
    nStructSum = m_pGraph->Size(); 
	ASSERT(nStructSum > 0);
	
	// ����ṹ����������Է��̲����洢�ռ�
	float *pSlope = new float[nStructSum];	   
	float *pSita = new float[nStructSum];	
	
	for (i=0; i<nStructSum; i++)
	{
		f_xSum = f_ySum = f_xySum = f_xxSum = 0.0;
		pUnit = m_pGraph->GetUnitByIdx(i);
		// �ṹ�����߶�����Ŀ
		nLinePointNum = pUnit->m_pPointList->GetSize(); 	
		ASSERT(nLinePointNum > 1);
		
		// �ṹ�����������������
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
			// �ṹ����������Է���б��
			pSlope[i] = (f_xSum*f_ySum - nLinePointNum*f_xySum)/(f_xSum*f_xSum - nLinePointNum*f_xxSum);
			
			// �ṹ�����			
			if((int)(pSita[i] = atan(pSlope[i])*180/3.1416) < 0)
			{
				pSita[i] = 180 + pSita[i];
			}
		}
		
		// �ṹ�����
		else
			pSita[i] = 90;
	}
	
	// ͳ�ƽṹ������		
	for (i=0; i<nStructSum; i++)
	{
		// �б�ǰ����ֵ�ṹ����ȵ�ǰ�ṹ������ֵС�Ľṹ���Ƿ�ƽ��
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
		
		// �б�ȵ�ǰ����ֵ�ߵĽṹ���뵱ǰ����ֵ�ṹ���Ƿ�ƽ��
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
		
		// ��¼�ṹ������
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

// ��ȡ�ṹ�����
float CMultiSeg::GetArentDip(int nStructInd)
{
	ASSERT(m_pGraph != NULL);
	ASSERT(nStructInd >= 0);
	
	int i, nLinePointNum;
	float f_xSum, f_ySum, f_xySum, f_xxSum, fSlope, fSita;
	
	f_xSum = f_ySum = f_xySum = f_xxSum = 0.0;
	
	CUnit *pUnit = NULL;
	
	// ָ�������
	pUnit = m_pGraph->GetUnitByIdx(nStructInd);
	// ����߶�����Ŀ
	nLinePointNum = pUnit->m_pPointList->GetSize();
	ASSERT(nLinePointNum > 1);
	
	// �ṹ�����������������
	for (i=0; i<nLinePointNum; i++)
	{
		POINT2DF pt = pUnit->m_pPointList->GetAt(i);
		
		f_xSum += pt.x;
		f_ySum += pt.y;
		f_xySum += pt.x*pt.y;
		f_xxSum += pt.x*pt.x;
	}
	
	// �ṹ�����
	if (abs(f_xSum*f_xSum - nLinePointNum*f_xxSum) > 0.000001)
	{
		// �ṹ����������Է���б��
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

// ��ȡ�ṹ��ƽ�����
// δ����ͬ����ƽ���ǶȲ�Ϊ90�ȣ�����������ṹ��Ƕ�Ϊ90�ȵ����
float CMultiSeg::GetAveDistance(int nGroupIdx, float fScale)
{
	ASSERT(m_pGraph != NULL);
	ASSERT(nGroupIdx >= 0 && fScale > 1);
	
	int i, j, nGroupStructNum, nLinePointNum;
	float f_xSum, f_ySum, f_xySum, f_xxSum;
	float fAveDistance, fInterceptMax, fInterceptMin, fAveAngle, fAngleSum;	
    
	CUnit *pUnit = NULL;
	fAngleSum = 0.0;
	
    // ͬ��ṹ�����
	nGroupStructNum = m_vvGroup[nGroupIdx].size();
	ASSERT(nGroupStructNum > 0);
	
	if(nGroupStructNum < 2)
	{
		return 0;	
	}

	// ����������Է��̵Ĳ����洢�ռ�
	float *pAngle = new float[nGroupStructNum];
	float *pSlope = new float[nGroupStructNum];
	float *pIntercept = new float[nGroupStructNum];

	// ����ṹ��������Է���б�ʡ��ؾ�����
	for (i=0; i<nGroupStructNum; i++)
	{
		f_xSum = f_ySum = f_xySum = f_xxSum = 0.0;
		// ָ������� 
		pUnit = m_pGraph->GetUnitByIdx(m_vvGroup[nGroupIdx][i]);
		// ����߶�����Ŀ
		nLinePointNum = pUnit->m_pPointList->GetSize();
		ASSERT(nLinePointNum > 1);		
		
		// �ṹ�����������������
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
			// �ṹ����������Է���б��
			pSlope[i] = (f_xSum*f_ySum - nLinePointNum*f_xySum)/(f_xSum*f_xSum - nLinePointNum*f_xxSum);
			// �ṹ�����
			if (pSlope[i] >= 0)
			{
				pAngle[i] = atan(pSlope[i])*180/3.1416;
			}
			else
			{
				pAngle[i] = 180 + atan(pSlope[i])*180/3.1416;
			}

			// �ṹ����������Է��̵Ľؾ�
			pIntercept[i] = (f_ySum - f_xSum*pSlope[i])/nLinePointNum;
		}
		else
		{
			pAngle[i] = 90;
		}
		
		// ͬ��ṹ����Ǻ�
		fAngleSum += pAngle[i];
	}

	// ͬ��ṹ�����ƽ��ֵ
	fAveAngle = fAngleSum/nGroupStructNum;
	
	// ͬ��ṹ��ƽ�����
	if (abs(fAveAngle-90) > 0.000001)
	{
		// ��ʼ��������С�ؾ�ֵ
		for (i=0; i<nGroupStructNum; i++)
		{
			if (abs(pAngle[i]-90)>0.000001)
			{
				fInterceptMax = fInterceptMin = pIntercept[i];
				break;	
			}
		}

		// ͬ��ṹ��������Ϸ��������ؾ�ֵ����С�ؾ�ֵ
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
				// ����߶�����Ŀ
				nLinePointNum = pUnit->m_pPointList->GetSize();
				for (j=0; j<nLinePointNum; j++)
				{
					POINT2DF pt = pUnit->m_pPointList->GetAt(j);					
					f_xSum += pt.x;
				}
				px[i] = f_xSum/nLinePointNum;
			}
		}
        
		// ������С������ֵ
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
		// ͬ��ṹ��ƽ�����
		fAveDistance = fScale*(fMax-fMin)/(nGroupStructNum-1);

		delete [] px;
	}	
	
	delete [] pAngle;
	delete [] pSlope;
	delete [] pIntercept;
	
	return fAveDistance;	
}

// �ṹ�����쳤��
float CMultiSeg::GetMultiSegLength(int nStructIdx, float fScale)
{
	ASSERT(m_pGraph != NULL);
	ASSERT(nStructIdx >= 0 && fScale > 1);
	
	
	int i, nLinePointNum;
	float fLength;
	
	fLength = 0.0;

	CUnit *pUnit = NULL;
    
	// ָ���ṹ��
	pUnit = m_pGraph->GetUnitByIdx(nStructIdx);
	// ����߶�����
	nLinePointNum = pUnit->m_pPointList->GetSize();
	ASSERT(nLinePointNum > 0);
    
	// ����ṹ�����쳤��
	for (i=0; i<nLinePointNum-1; i++)
	{
		POINT2DF pt1 = pUnit->m_pPointList->GetAt(i);
		POINT2DF pt2 = pUnit->m_pPointList->GetAt(i+1);
		fLength += sqrt((pt1.x-pt2.x)*(pt1.x-pt2.x) + (pt1.y-pt2.y)*(pt1.y-pt2.y));
	}

	return fLength*fScale;
}

// ͬ������ƽ������
float CMultiSeg::GetAveLength(int nGroupIdx, float fScale)
{
	ASSERT(m_pGraph != NULL);
	ASSERT(nGroupIdx >= 0 && fScale > 1);
    
	float fLength;
	int i, nGroupStructNum;
	
	// ͬ��ṹ������
	nGroupStructNum = m_vvGroup[nGroupIdx].size();
	/*float *pLength = new float[nGroupStructNum];*/
	
	fLength = 0.0;
	for (i=0; i<nGroupStructNum; i++)
	{
		// ָ���ṹ��
		fLength += GetMultiSegLength(m_vvGroup[nGroupIdx][i], fScale);
	}
	// ����ͬ������ƽ������
	return fLength*fScale/nGroupStructNum;	
}
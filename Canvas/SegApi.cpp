#include "stdafx.h"
#include <math.h>
#include "SegApi.h"

/*************************************************************************
 *
 * \�������ƣ�
 *   GaussianSmooth()
 *
 * \�������:
 *   unsigned char * pUnchImg				- ָ��ͼ�����ݵ�ָ��
 *   int nWidth											- ͼ�����ݿ��
 *   int nHeight										- ͼ�����ݸ߶�
 *   double dSigma									- ��˹�����ı�׼��
 *   unsigned char * pUnchSmthdImg	- ָ�򾭹�ƽ��֮���ͼ������
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   Ϊ����ֹ���������ø�˹�˲���ͼ������˲����˲��ȶ�x������У�Ȼ���
 *   y������С�
 *   
 *************************************************************************
 */
void GaussianSmooth(unsigned char *pUnchImg, int nWidth, int nHeight, 
										double sigma, unsigned char * pUnchSmthdImg)
{
	// ѭ�����Ʊ���
  int y;
	int x;
	
	int i;
	
	// ��˹�˲��������鳤��
	
	int nWindowSize;
	
	//  ���ڳ��ȵ�1/2
	int	nHalfLen;   
	
	// һά��˹�����˲���
	double *pdKernel ;
	
	// ��˹ϵ����ͼ�����ݵĵ��
	double  dDotMul     ;
	
	// ��˹�˲�ϵ�����ܺ�
	double  dWeightSum     ;          
  
	// �м����
	double * pdTmp ;
	
	// �����ڴ�
	pdTmp = new double[nWidth*nHeight];

	// ����һά��˹�����˲���
	MakeGauss(sigma, &pdKernel, &nWindowSize) ;

	// MakeGauss���ش��ڵĳ��ȣ����ô˱������㴰�ڵİ볤
	nHalfLen = nWindowSize / 2;
	
  // x��������˲�
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			dDotMul		= 0;
			dWeightSum = 0;
			for(i=(-nHalfLen); i<=nHalfLen; i++)
			{
				// �ж��Ƿ���ͼ���ڲ�
				if( (i+x) >= 0  && (i+x) < nWidth )
				{
					dDotMul += (double)pUnchImg[y*nWidth + (i+x)] * pdKernel[nHalfLen+i];
					dWeightSum += pdKernel[nHalfLen+i];
				}
			}
			pdTmp[y*nWidth + x] = dDotMul/dWeightSum ;
		}
	}
	
	// y��������˲�
	for(x=0; x<nWidth; x++)
	{
		for(y=0; y<nHeight; y++)
		{
			dDotMul		= 0;
			dWeightSum = 0;
			for(i=(-nHalfLen); i<=nHalfLen; i++)
			{
				// �ж��Ƿ���ͼ���ڲ�
				if( (i+y) >= 0  && (i+y) < nHeight )
				{
					dDotMul += (double)pdTmp[(y+i)*nWidth + x] * pdKernel[nHalfLen+i];
					dWeightSum += pdKernel[nHalfLen+i];
				}
			}
			pUnchSmthdImg[y*nWidth + x] = (unsigned char)(int)dDotMul/dWeightSum ;
		}
	}

	// �ͷ��ڴ�
	delete []pdKernel;
	pdKernel = NULL ;
	
	delete []pdTmp;
	pdTmp = NULL;
}

/*************************************************************************
 *
 * \�������ƣ�
 *   MakeGauss()
 *
 * \�������:
 *   double sigma									        - ��˹�����ı�׼��
 *   double **pdKernel										- ָ���˹���������ָ��
 *   int *pnWindowSize										- ���ݵĳ���
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ���������������һ��һά�ĸ�˹�������������ݣ������ϸ�˹���ݵĳ���Ӧ
 *   �������޳��ģ�����Ϊ�˼���ļ򵥺��ٶȣ�ʵ�ʵĸ�˹����ֻ�������޳���
 *   pnWindowSize�������ݳ���
 *   
 *************************************************************************
 */
void MakeGauss(double sigma, double **pdKernel, int *pnWindowSize)
{
	// ѭ�����Ʊ���
	int i   ;
	
	// ��������ĵ�
	int nCenter;

	// �����ĳһ�㵽���ĵ�ľ���
	double  dDis  ; 

	double PI = 3.14159;
	// �м����
	double  dValue; 
	double  dSum  ;
	dSum = 0 ; 
	
	// ���鳤�ȣ����ݸ����۵�֪ʶ��ѡȡ[-3*sigma, 3*sigma]���ڵ����ݡ�
	// ��Щ���ݻḲ�Ǿ��󲿷ֵ��˲�ϵ��
	*pnWindowSize = 1 + 2 * ceil(3 * sigma);
	
	// ����
	nCenter = (*pnWindowSize) / 2;
	
	// �����ڴ�
	*pdKernel = new double[*pnWindowSize] ;
	
	for(i=0; i< (*pnWindowSize); i++)
	{
		dDis = (double)(i - nCenter);
		dValue = exp(-(1/2)*dDis*dDis/(sigma*sigma)) / (sqrt(2 * PI) * sigma );
		(*pdKernel)[i] = dValue ;
		dSum += dValue;
	}
	
	// ��һ��
	for(i=0; i<(*pnWindowSize) ; i++)
	{
		(*pdKernel)[i] /= dSum;
	}
}

/*************************************************************************
 *
 * \�������ƣ�
 *   DirGrad()
 *
 * \�������:
 *   unsigned char *pUnchSmthdImg					- ������˹�˲����ͼ��
 *   int nWidht														- ͼ����
 *   int nHeight      										- ͼ��߶�
 *   int *pnGradX                         - x����ķ�����
 *   int *pnGradY                         - y����ķ�����
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ����������㷽���������õ�΢��������(-1 0 1) �� (-1 0 1)'(ת��)
 *   �����ʱ��Ա߽����ز��������⴦��
 *   
 *   
 *************************************************************************
 */
void DirGrad(unsigned char *pUnchSmthdImg, int nWidth, int nHeight,
										 int *pnGradX , int *pnGradY)
{
	// ѭ�����Ʊ���
	int y ;
	int x ;
	
	// ����x����ķ��������ڱ߽�������˴�����ֹҪ���ʵ����س���
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			pnGradX[y*nWidth+x] = (int) ( pUnchSmthdImg[y*nWidth+min(nWidth-1,x+1)]  
													          - pUnchSmthdImg[y*nWidth+max(0,x-1)]     );
		}
	}

	// ����y����ķ��������ڱ߽�������˴�����ֹҪ���ʵ����س���
	for(x=0; x<nWidth; x++)
	{
		for(y=0; y<nHeight; y++)
		{
			pnGradY[y*nWidth+x] = (int) ( pUnchSmthdImg[min(nHeight-1,y+1)*nWidth + x]  
																		- pUnchSmthdImg[max(0,y-1)*nWidth+ x ]     );
		}
	}
}

/*************************************************************************
 *
 * \�������ƣ�
 *   GradMagnitude()
 *
 * \�������:
 *   int *pnGradX                         - x����ķ�����
 *   int *pnGradY                         - y����ķ�����
 *   int nWidht														- ͼ����
 *   int nHeight      										- ͼ��߶�
 *   int *pnMag                           - �ݶȷ���   
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ����������÷����������ݶȷ��ȣ���������DirGrad��������Ľ��
 *   
 *************************************************************************
 */
void GradMagnitude(int *pnGradX, int *pnGradY, int nWidth, int nHeight, int *pnMag)
{
	
	// ѭ�����Ʊ���
	int y ;
	int x ;

	// �м����
	double dSqtOne;
	double dSqtTwo;
	
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			dSqtOne = pnGradX[y*nWidth + x] * pnGradX[y*nWidth + x];
			dSqtTwo = pnGradY[y*nWidth + x] * pnGradY[y*nWidth + x];
			pnMag[y*nWidth + x] = (int)(sqrt(dSqtOne + dSqtTwo) + 0.5);
		}
	}
}

/*************************************************************************
 *
 * \�������ƣ�
 *   NonmaxSuppress()
 *
 * \�������:
 *   int *pnMag                - �ݶ�ͼ
 *   int *pnGradX							 - x����ķ�����	
 *   int *pnGradY              - y����ķ�����
 *   int nWidth                - ͼ�����ݿ��
 *   int nHeight               - ͼ�����ݸ߶�
 *   unsigned char *pUnchRst   - ����NonmaxSuppress�����Ľ��
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ��ֹ�ݶ�ͼ�зǾֲ���ֵ������ء�
 *   
 *************************************************************************
 */
void NonmaxSuppress(int *pnMag, int *pnGradX, int *pnGradY, int nWidth, 
										int nHeight,	unsigned char *pUnchRst)
{
	// ѭ�����Ʊ���
	int y ;
	int x ;
	int nPos;

	// x�����ݶȷ���
	int gx  ;
	int gy  ;

	// ��ʱ����
	int g1, g2, g3, g4 ;
	double weight  ;
	double dTmp1   ;
	double dTmp2   ;
	double dTmp    ;
	
	// ����ͼ���Ե����Ϊ�����ܵı߽��
	for(x=0; x<nWidth; x++)		
	{
		pUnchRst[x] = 0 ;
		pUnchRst[(nHeight-1)*nWidth+x] = 0;
	}
	for(y=0; y<nHeight; y++)		
	{
		pUnchRst[y*nWidth] = 0 ;
		pUnchRst[y*nWidth + nWidth-1] = 0;
	}

	for(y=1; y<nHeight-1; y++)
	{
		for(x=1; x<nWidth-1; x++)
		{
			nPos = y*nWidth + x ;
			
			// �����ǰ���ص��ݶȷ���Ϊ0�����Ǳ߽��
			if(pnMag[nPos] == 0 )
			{
				pUnchRst[nPos] = 0 ;
			}
			else
			{
				// ��ǰ���ص��ݶȷ���
				dTmp = pnMag[nPos] ;
				
				// x��y������
				gx = pnGradX[nPos]  ;
				gy = pnGradY[nPos]  ;

				// ���������y������x������˵�������ķ�����ӡ�������y������
				if (abs(gy) > abs(gx)) 
				{
					// �����ֵ�ı���
					weight = fabs(gx)/fabs(gy); 

					g2 = pnMag[nPos-nWidth] ; 
					g4 = pnMag[nPos+nWidth] ;
					
					// ���x��y��������ķ������ķ�����ͬ
					// C�ǵ�ǰ���أ���g1-g4��λ�ù�ϵΪ��
					//	g1 g2 
					//		 C         
					//		 g4 g3 
					if (gx*gy > 0) 
					{ 					
						g1 = pnMag[nPos-nWidth-1] ;
						g3 = pnMag[nPos+nWidth+1] ;
					} 

					// ���x��y��������ķ������ķ����෴
					// C�ǵ�ǰ���أ���g1-g4��λ�ù�ϵΪ��
					//	   g2 g1
					//	   C         
					//	g3 g4  
					else 
					{ 
						g1 = pnMag[nPos-nWidth+1] ;
						g3 = pnMag[nPos+nWidth-1] ;
					} 
				}
				
				// ���������x������y������˵�������ķ�����ӡ�������x����
				// ����ж���������x������y������ȵ����
				else
				{
					// �����ֵ�ı���
					weight = fabs(gy)/fabs(gx); 
					
					g2 = pnMag[nPos+1] ; 
					g4 = pnMag[nPos-1] ;
					
					// ���x��y��������ķ������ķ�����ͬ
					// C�ǵ�ǰ���أ���g1-g4��λ�ù�ϵΪ��
					//	g3   
					//	g4 C g2       
					//       g1
					if (gx*gy > 0) 
					{				
						g1 = pnMag[nPos+nWidth+1] ;
						g3 = pnMag[nPos-nWidth-1] ;
					} 
					// ���x��y��������ķ������ķ����෴
					// C�ǵ�ǰ���أ���g1-g4��λ�ù�ϵΪ��
					//	     g1
					//	g4 C g2       
					//  g3     
					else 
					{ 
						g1 = pnMag[nPos-nWidth+1] ;
						g3 = pnMag[nPos+nWidth-1] ;
					}
				}

				// ��������g1-g4���ݶȽ��в�ֵ
				{
					dTmp1 = weight*g1 + (1-weight)*g2 ;
					dTmp2 = weight*g3 + (1-weight)*g4 ;
					
					// ��ǰ���ص��ݶ��Ǿֲ������ֵ
					// �õ�����Ǹ��߽��
					if(dTmp>=dTmp1 && dTmp>=dTmp2)
					{
						pUnchRst[nPos] = 128 ;
					}
					else
					{
						// �������Ǳ߽��
						pUnchRst[nPos] = 0 ;
					}
				}
			} //else
		} // for

	}
} 

/*************************************************************************
 *
 * \�������ƣ�
 *   EstimateThreshold()
 *
 * \�������:
 *   int *pnMag               - �ݶȷ���ͼ
 *	 int nWidth               - ͼ�����ݿ��
 *	 int nHeight              - ͼ�����ݸ߶�
 *   int *pnThdHigh           - ����ֵ
 *   int *pnThdLow            - ����ֵ
 *	 double dRatioLow         - ����ֵ�͸���ֵ֮��ı���
 *	 double dRatioHigh        - ����ֵռͼ�����������ı���
 *   unsigned char *pUnchEdge - ����non-maximum����������
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ����non-maximum����������pUnchEdge��ͳ��pnMag��ֱ��ͼ��ȷ����ֵ��
 *   ��������ֻ��ͳ��pUnchEdge�п���Ϊ�߽�����Щ���ء�Ȼ������ֱ��ͼ��
 *   ����dRatioHigh���ø���ֵ���洢��pnThdHigh������dRationLow�͸���ֵ��
 *   ���õ���ֵ���洢��*pnThdLow��dRatioHigh��һ�ֱ����������ݶ�С��
 *   *pnThdHigh��������Ŀռ��������Ŀ�ı�����dRationLow����*pnThdHigh
 *   ��*pnThdLow�ı��������������canny�㷨��ԭ������߸�����һ�����䡣
 *
 *************************************************************************
 */
void EstimateThreshold(int *pnMag, int nWidth, int nHeight, int *pnThdHigh,int *pnThdLow, 
											 unsigned char * pUnchEdge, double dRatioHigh, double dRationLow) 
{ 
	// ѭ�����Ʊ���
	int y;
	int x;
	int k;
	
	// ������Ĵ�С���ݶ�ֵ�ķ�Χ�йأ�������ñ�������㷨����ô�ݶȵķ�Χ���ᳬ��pow(2,10)
	int nHist[1024] ;

	// ���ܵı߽���Ŀ
	int nEdgeNb     ;

	// ����ݶ�ֵ
	int nMaxMag     ;

	int nHighCount  ;

	nMaxMag = 0     ; 
	
	// ��ʼ��
	for(k=0; k<1024; k++) 
	{
		nHist[k] = 0; 
	}

	// ͳ��ֱ��ͼ��Ȼ������ֱ��ͼ������ֵ
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			// ֻ��ͳ����Щ�����Ǳ߽�㣬���һ�û�д����������
			if(pUnchEdge[y*nWidth+x]==128)
			{
				nHist[ pnMag[y*nWidth+x] ]++;
			}
		}
	}

	nEdgeNb = nHist[0]  ;
	nMaxMag = 0         ;
	// ͳ�ƾ����������ֵ��ֹ(non-maximum suppression)�����ж�������
	for(k=1; k<1024; k++)
	{
		if(nHist[k] != 0)
		{
			// ����ݶ�ֵ
			nMaxMag = k;
		}
		
		// �ݶ�Ϊ0�ĵ��ǲ�����Ϊ�߽���
		// ����non-maximum suppression���ж�������
		nEdgeNb += nHist[k];
	}

	// �ݶȱȸ���ֵ*pnThdHighС�����ص�����Ŀ
	nHighCount = (int)(dRatioHigh * nEdgeNb +0.5);
	
	k = 1;
	nEdgeNb = nHist[1];
	
	// �������ֵ
	while( (k<(nMaxMag-1)) && (nEdgeNb < nHighCount) )
	{
		k++;
		nEdgeNb += nHist[k];
	}

	// ���ø���ֵ
	*pnThdHigh = k ;

	// ���õ���ֵ
	*pnThdLow  = (int)((*pnThdHigh) * dRationLow+ 0.5);
}

/*************************************************************************
 *
 * \�������ƣ�
 *   Hysteresis()
 *
 * \�������:
 *   int *pnMag               - �ݶȷ���ͼ
 *	 int nWidth               - ͼ�����ݿ��
 *	 int nHeight              - ͼ�����ݸ߶�
 *	 double dRatioLow         - ����ֵ�͸���ֵ֮��ı���
 *	 double dRatioHigh        - ����ֵռͼ�����������ı���
 *   unsigned char *pUnchEdge - ��¼�߽��Ļ�����
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ������ʵ�����ơ��������󡱵�һ�����ܣ�Ҳ���ǣ��ȵ���EstimateThreshold
 *   �����Ծ���non-maximum����������pUnchSpr����һ������ֵ��Ȼ���ж�
 *   pUnchSpr�п��ܵı߽�����(=128)���ݶ��ǲ��Ǵ��ڸ���ֵnThdHigh�������
 *   ����ֵ�󣬸õ㽫��Ϊһ���߽����㣬����TraceEdge�������Ѷ�Ӧ�ñ߽�
 *   �����������ҳ�������󣬵������������ʱ�������������û�б���־��
 *   �߽�㣬��ô��һ�����Ǳ߽�㡣
 *   
 *************************************************************************
 */
void Hysteresis(int *pnMag, int nWidth, int nHeight, double dRatioLow, 
								double dRatioHigh, unsigned char *pUnchEdge)
{
	// ѭ�����Ʊ���
	int y;
	int x;

	int nThdHigh ;
	int nThdLow  ;

	int nPos;

	// ����TraceEdge��Ҫ�ĵ���ֵ���Լ�Hysteresis����ʹ�õĸ���ֵ
	EstimateThreshold(pnMag, nWidth, nHeight, &nThdHigh, 
		               &nThdLow, pUnchEdge,dRatioHigh, dRatioLow);

  // ���ѭ������Ѱ�Ҵ���nThdHigh�ĵ㣬��Щ�㱻���������߽�㣬Ȼ����
	// TraceEdge���������ٸõ��Ӧ�ı߽�
   for(y=0; y<nHeight; y++)
	 {
      for(x=0; x<nWidth; x++)
			{
				nPos = y*nWidth + x ; 

				// ����������ǿ��ܵı߽�㣬�����ݶȴ��ڸ���ֵ����������Ϊ
				// һ���߽�����
				if((pUnchEdge[nPos] == 128) && (pnMag[nPos] >= nThdHigh))
				{
					// ���øõ�Ϊ�߽��
					pUnchEdge[nPos] = 255;


					TraceEdge(y, x, nThdLow, pUnchEdge, pnMag, nWidth);
					//////////////////////////////////////////////////////////////////////////
					/*
					{ 
						// ��8�������ؽ��в�ѯ
						int xNb[8] = {1, 1, 0,-1,-1,-1, 0, 1} ;
						int yNb[8] = {0, 1, 1, 1,0 ,-1,-1,-1} ;

						int yy,y1 ;
						int xx,x1 ;
						x1 = x;
						y1 = y;
						
						int k  ;
						
						for(k=0; k<8; k++)
						{
							yy = y1 + yNb[k] ;
							xx = x1 + xNb[k] ;
							// ���������Ϊ���ܵı߽�㣬��û�д����
							// �����ݶȴ�����ֵ
							if(pUnchEdge[yy*nWidth+xx] == 128  && pnMag[yy*nWidth+xx]>=nThdLow)
							{
								// �Ѹõ����ó�Ϊ�߽��
								pUnchEdge[yy*nWidth+xx] = 255;

								// �Ըõ�Ϊ���Ľ��и���
								k = 0;
								x1 = xx;
								y1 = yy;
								// TraceEdge(yy, xx, nLowThd, pUnchEdge, pnMag, nWidth);
							}
						}
					}
					*/
					//////////////////////////////////////////////////////////////////////////
				
				}
      }
   }

	 // ��Щ��û�б�����Ϊ�߽��������Ѿ������ܳ�Ϊ�߽��
   for(y=0; y<nHeight; y++)
	 {
		 for(x=0; x<nWidth; x++)
		 {
			 nPos = y*nWidth + x ; 
			 if(pUnchEdge[nPos] != 255)
			 {
				 // ����Ϊ�Ǳ߽��
				 pUnchEdge[nPos] = 0 ;
			 }
		 }
	 }
}

/*************************************************************************
 *
 * \�������ƣ�
 *   Canny()
 *
 * \�������:
 *   unsigned char *pUnchImage- ͼ������
 *	 int nWidth               - ͼ�����ݿ��
 *	 int nHeight              - ͼ�����ݸ߶�
 *   double sigma             - ��˹�˲��ı�׼����
 *	 double dRatioLow         - ����ֵ�͸���ֵ֮��ı���
 *	 double dRatioHigh        - ����ֵռͼ�����������ı���
 *   unsigned char *pUnchEdge - canny���Ӽ����ķָ�ͼ
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   canny�ָ����ӣ�����Ľ��������pUnchEdge�У��߼�1(255)��ʾ�õ�Ϊ
 *   �߽�㣬�߼�0(0)��ʾ�õ�Ϊ�Ǳ߽�㡣�ú����Ĳ���sigma��dRatioLow
 *   dRatioHigh������Ҫָ���ġ���Щ������Ӱ��ָ��߽����Ŀ�Ķ���
 *************************************************************************
 */
void Canny(unsigned char *pUnchImage, int nWidth, int nHeight, double sigma,
					 double dRatioLow, double dRatioHigh, unsigned char *pUnchEdge)
{
	// ������˹�˲����ͼ������
	unsigned char * pUnchSmooth ;
  
	// ָ��x��������ָ��
	int * pnGradX ; 

	// ָ��y��������ָ��
	int * pnGradY ;

	// �ݶȵķ���
	int * pnGradMag ;

	pUnchSmooth  = new unsigned char[nWidth*nHeight] ;
	pnGradX      = new int [nWidth*nHeight]          ;
	pnGradY      = new int [nWidth*nHeight]          ;
	pnGradMag    = new int [nWidth*nHeight]          ;

	// ��ԭͼ������˲�
	GaussianSmooth(pUnchImage, nWidth, nHeight, sigma, pUnchSmooth) ;

	// ���㷽����
	DirGrad(pUnchSmooth, nWidth, nHeight, pnGradX, pnGradY) ;

	// �����ݶȵķ���
	GradMagnitude(pnGradX, pnGradY, nWidth, nHeight, pnGradMag) ;

	// Ӧ��non-maximum ����
	NonmaxSuppress(pnGradMag, pnGradX, pnGradY, nWidth, nHeight, pUnchEdge) ;

	// Ӧ��Hysteresis���ҵ����еı߽�
	Hysteresis(pnGradMag, nWidth, nHeight, dRatioLow, dRatioHigh, pUnchEdge);


	// �ͷ��ڴ�
	delete pnGradX      ;
	pnGradX      = NULL ;
	delete pnGradY      ;
	pnGradY      = NULL ;
	delete pnGradMag    ;
	pnGradMag    = NULL ;
	delete pUnchSmooth ;
	pUnchSmooth  = NULL ;
}

/*************************************************************************
 *
 * \�������ƣ�
 *   TraceEdge()
 *
 * \�������:
 *   int    x									- ��������x���� 
 *   int    y									- ��������y����
 *   int nLowThd							- �ж�һ�����Ƿ�Ϊ�߽��ĵ���ֵ
 *   unsigned char *pUnchEdge - ��¼�߽��Ļ�����
 *   int *pnMag               - �ݶȷ���ͼ
 *   int nWidth               - ͼ�����ݿ��
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   �ݹ����  
 *   ��(x,y)������������б߽��ĸ��٣�����ֻ����pUnchEdge��û�д�����
 *   �����Ǳ߽�����Щ����(=128),����ֵΪ0�����õ㲻�����Ǳ߽�㣬����ֵ
 *   Ϊ255�����õ��Ѿ�������Ϊ�߽�㣬�����ٿ���
 *   
 *   
 *************************************************************************
 */
void TraceEdge (int y, int x, int nLowThd, unsigned char *pUnchEdge, int *pnMag, int nWidth) 
{ 
	/*
	// ��8�������ؽ��в�ѯ
	int xNb[8] = {1, 1, 0,-1,-1,-1, 0, 1} ;
	int yNb[8] = {0, 1, 1, 1,0 ,-1,-1,-1} ;

	int yy ;
	int xx ;
	
	int k  ;

	for(k=0; k<8; k++)
	{
		yy = y + yNb[k] ;
		xx = x + xNb[k] ;
		// ���������Ϊ���ܵı߽�㣬��û�д����
		// �����ݶȴ�����ֵ
		if(pUnchEdge[yy*nWidth+xx] == 128  && pnMag[yy*nWidth+xx]>=nLowThd)
		{
			// �Ѹõ����ó�Ϊ�߽��
			pUnchEdge[yy*nWidth+xx] = 255;

			// �Ըõ�Ϊ���Ľ��и���
			TraceEdge(yy, xx, nLowThd, pUnchEdge, pnMag, nWidth);
		}
	}
	*/

	//////////////////////////////////////////////////////////////////////////
	// ���治���õݹ����ʵ�ֺ�������
	// ��8�������ؽ��в�ѯ
	int xNb[8] = {1, 1, 0,-1,-1,-1, 0, 1} ;
	int yNb[8] = {0, 1, 1, 1,0 ,-1,-1,-1} ;

	typedef struct NODE
	{
		int x,y;
		int nDir; // 0~7
		NODE* pPreLnk;
	} NODE, *PNODE;

	PNODE pStart = new NODE;

	PNODE pNode = pStart;
	pNode->x = x;
	pNode->y = y;
	pNode->nDir = 0;
	pNode->pPreLnk = NULL;	// ͷ�ڵ��ǰ������Ϊ��

	while (pNode != NULL)
	{
		PNODE pNode1 = new NODE;
		pNode1->x = pNode->x+xNb[pNode->nDir];
		pNode1->y = pNode->y+yNb[pNode->nDir];
		pNode1->nDir = 0;
		pNode1->pPreLnk = pNode;

		pNode = pNode1;

		// ���������Ϊ���ܵı߽�㣬��û�д����
		// �����ݶȴ�����ֵ
		if(pUnchEdge[pNode->y*nWidth+pNode->x] == 128  && pnMag[pNode->y*nWidth+pNode->x]>=nLowThd)
		{
			// �Ѹõ����ó�Ϊ�߽��
			pUnchEdge[pNode->y*nWidth+pNode->x] = 255;
		}
		else
		{
			pNode = pNode->pPreLnk;
			delete pNode1;

			pNode->nDir += 1;
			while (pNode->nDir >= 8)
			{
				pNode1 = pNode;
				pNode = pNode->pPreLnk;
				delete pNode1;
				if (pNode == NULL)
					break;

				pNode->nDir += 1;
			}
		}
	}
} 

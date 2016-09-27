#include "stdafx.h"
#include <math.h>
#include "SegApi.h"

/*************************************************************************
 *
 * \函数名称：
 *   GaussianSmooth()
 *
 * \输入参数:
 *   unsigned char * pUnchImg				- 指向图象数据的指针
 *   int nWidth											- 图象数据宽度
 *   int nHeight										- 图象数据高度
 *   double dSigma									- 高斯函数的标准差
 *   unsigned char * pUnchSmthdImg	- 指向经过平滑之后的图象数据
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   为了抑止噪声，采用高斯滤波对图象进行滤波，滤波先对x方向进行，然后对
 *   y方向进行。
 *   
 *************************************************************************
 */
void GaussianSmooth(unsigned char *pUnchImg, int nWidth, int nHeight, 
										double sigma, unsigned char * pUnchSmthdImg)
{
	// 循环控制变量
  int y;
	int x;
	
	int i;
	
	// 高斯滤波器的数组长度
	
	int nWindowSize;
	
	//  窗口长度的1/2
	int	nHalfLen;   
	
	// 一维高斯数据滤波器
	double *pdKernel ;
	
	// 高斯系数与图象数据的点乘
	double  dDotMul     ;
	
	// 高斯滤波系数的总和
	double  dWeightSum     ;          
  
	// 中间变量
	double * pdTmp ;
	
	// 分配内存
	pdTmp = new double[nWidth*nHeight];

	// 产生一维高斯数据滤波器
	MakeGauss(sigma, &pdKernel, &nWindowSize) ;

	// MakeGauss返回窗口的长度，利用此变量计算窗口的半长
	nHalfLen = nWindowSize / 2;
	
  // x方向进行滤波
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			dDotMul		= 0;
			dWeightSum = 0;
			for(i=(-nHalfLen); i<=nHalfLen; i++)
			{
				// 判断是否在图象内部
				if( (i+x) >= 0  && (i+x) < nWidth )
				{
					dDotMul += (double)pUnchImg[y*nWidth + (i+x)] * pdKernel[nHalfLen+i];
					dWeightSum += pdKernel[nHalfLen+i];
				}
			}
			pdTmp[y*nWidth + x] = dDotMul/dWeightSum ;
		}
	}
	
	// y方向进行滤波
	for(x=0; x<nWidth; x++)
	{
		for(y=0; y<nHeight; y++)
		{
			dDotMul		= 0;
			dWeightSum = 0;
			for(i=(-nHalfLen); i<=nHalfLen; i++)
			{
				// 判断是否在图象内部
				if( (i+y) >= 0  && (i+y) < nHeight )
				{
					dDotMul += (double)pdTmp[(y+i)*nWidth + x] * pdKernel[nHalfLen+i];
					dWeightSum += pdKernel[nHalfLen+i];
				}
			}
			pUnchSmthdImg[y*nWidth + x] = (unsigned char)(int)dDotMul/dWeightSum ;
		}
	}

	// 释放内存
	delete []pdKernel;
	pdKernel = NULL ;
	
	delete []pdTmp;
	pdTmp = NULL;
}

/*************************************************************************
 *
 * \函数名称：
 *   MakeGauss()
 *
 * \输入参数:
 *   double sigma									        - 高斯函数的标准差
 *   double **pdKernel										- 指向高斯数据数组的指针
 *   int *pnWindowSize										- 数据的长度
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   这个函数可以生成一个一维的高斯函数的数字数据，理论上高斯数据的长度应
 *   该是无限长的，但是为了计算的简单和速度，实际的高斯数据只能是有限长的
 *   pnWindowSize就是数据长度
 *   
 *************************************************************************
 */
void MakeGauss(double sigma, double **pdKernel, int *pnWindowSize)
{
	// 循环控制变量
	int i   ;
	
	// 数组的中心点
	int nCenter;

	// 数组的某一点到中心点的距离
	double  dDis  ; 

	double PI = 3.14159;
	// 中间变量
	double  dValue; 
	double  dSum  ;
	dSum = 0 ; 
	
	// 数组长度，根据概率论的知识，选取[-3*sigma, 3*sigma]以内的数据。
	// 这些数据会覆盖绝大部分的滤波系数
	*pnWindowSize = 1 + 2 * ceil(3 * sigma);
	
	// 中心
	nCenter = (*pnWindowSize) / 2;
	
	// 分配内存
	*pdKernel = new double[*pnWindowSize] ;
	
	for(i=0; i< (*pnWindowSize); i++)
	{
		dDis = (double)(i - nCenter);
		dValue = exp(-(1/2)*dDis*dDis/(sigma*sigma)) / (sqrt(2 * PI) * sigma );
		(*pdKernel)[i] = dValue ;
		dSum += dValue;
	}
	
	// 归一化
	for(i=0; i<(*pnWindowSize) ; i++)
	{
		(*pdKernel)[i] /= dSum;
	}
}

/*************************************************************************
 *
 * \函数名称：
 *   DirGrad()
 *
 * \输入参数:
 *   unsigned char *pUnchSmthdImg					- 经过高斯滤波后的图象
 *   int nWidht														- 图象宽度
 *   int nHeight      										- 图象高度
 *   int *pnGradX                         - x方向的方向导数
 *   int *pnGradY                         - y方向的方向导数
 * \返回值:
 *   无
 *
 * \说明:
 *   这个函数计算方向倒数，采用的微分算子是(-1 0 1) 和 (-1 0 1)'(转置)
 *   计算的时候对边界象素采用了特殊处理
 *   
 *   
 *************************************************************************
 */
void DirGrad(unsigned char *pUnchSmthdImg, int nWidth, int nHeight,
										 int *pnGradX , int *pnGradY)
{
	// 循环控制变量
	int y ;
	int x ;
	
	// 计算x方向的方向导数，在边界出进行了处理，防止要访问的象素出界
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			pnGradX[y*nWidth+x] = (int) ( pUnchSmthdImg[y*nWidth+min(nWidth-1,x+1)]  
													          - pUnchSmthdImg[y*nWidth+max(0,x-1)]     );
		}
	}

	// 计算y方向的方向导数，在边界出进行了处理，防止要访问的象素出界
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
 * \函数名称：
 *   GradMagnitude()
 *
 * \输入参数:
 *   int *pnGradX                         - x方向的方向导数
 *   int *pnGradY                         - y方向的方向导数
 *   int nWidht														- 图象宽度
 *   int nHeight      										- 图象高度
 *   int *pnMag                           - 梯度幅度   
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   这个函数利用方向倒数计算梯度幅度，方向倒数是DirGrad函数计算的结果
 *   
 *************************************************************************
 */
void GradMagnitude(int *pnGradX, int *pnGradY, int nWidth, int nHeight, int *pnMag)
{
	
	// 循环控制变量
	int y ;
	int x ;

	// 中间变量
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
 * \函数名称：
 *   NonmaxSuppress()
 *
 * \输入参数:
 *   int *pnMag                - 梯度图
 *   int *pnGradX							 - x方向的方向导数	
 *   int *pnGradY              - y方向的方向导数
 *   int nWidth                - 图象数据宽度
 *   int nHeight               - 图象数据高度
 *   unsigned char *pUnchRst   - 经过NonmaxSuppress处理后的结果
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   抑止梯度图中非局部极值点的象素。
 *   
 *************************************************************************
 */
void NonmaxSuppress(int *pnMag, int *pnGradX, int *pnGradY, int nWidth, 
										int nHeight,	unsigned char *pUnchRst)
{
	// 循环控制变量
	int y ;
	int x ;
	int nPos;

	// x方向梯度分量
	int gx  ;
	int gy  ;

	// 临时变量
	int g1, g2, g3, g4 ;
	double weight  ;
	double dTmp1   ;
	double dTmp2   ;
	double dTmp    ;
	
	// 设置图象边缘部分为不可能的边界点
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
			
			// 如果当前象素的梯度幅度为0，则不是边界点
			if(pnMag[nPos] == 0 )
			{
				pUnchRst[nPos] = 0 ;
			}
			else
			{
				// 当前象素的梯度幅度
				dTmp = pnMag[nPos] ;
				
				// x，y方向导数
				gx = pnGradX[nPos]  ;
				gy = pnGradY[nPos]  ;

				// 如果方向导数y分量比x分量大，说明导数的方向更加“趋向”于y分量。
				if (abs(gy) > abs(gx)) 
				{
					// 计算插值的比例
					weight = fabs(gx)/fabs(gy); 

					g2 = pnMag[nPos-nWidth] ; 
					g4 = pnMag[nPos+nWidth] ;
					
					// 如果x，y两个方向的方向导数的符号相同
					// C是当前象素，与g1-g4的位置关系为：
					//	g1 g2 
					//		 C         
					//		 g4 g3 
					if (gx*gy > 0) 
					{ 					
						g1 = pnMag[nPos-nWidth-1] ;
						g3 = pnMag[nPos+nWidth+1] ;
					} 

					// 如果x，y两个方向的方向导数的符号相反
					// C是当前象素，与g1-g4的位置关系为：
					//	   g2 g1
					//	   C         
					//	g3 g4  
					else 
					{ 
						g1 = pnMag[nPos-nWidth+1] ;
						g3 = pnMag[nPos+nWidth-1] ;
					} 
				}
				
				// 如果方向导数x分量比y分量大，说明导数的方向更加“趋向”于x分量
				// 这个判断语句包含了x分量和y分量相等的情况
				else
				{
					// 计算插值的比例
					weight = fabs(gy)/fabs(gx); 
					
					g2 = pnMag[nPos+1] ; 
					g4 = pnMag[nPos-1] ;
					
					// 如果x，y两个方向的方向导数的符号相同
					// C是当前象素，与g1-g4的位置关系为：
					//	g3   
					//	g4 C g2       
					//       g1
					if (gx*gy > 0) 
					{				
						g1 = pnMag[nPos+nWidth+1] ;
						g3 = pnMag[nPos-nWidth-1] ;
					} 
					// 如果x，y两个方向的方向导数的符号相反
					// C是当前象素，与g1-g4的位置关系为：
					//	     g1
					//	g4 C g2       
					//  g3     
					else 
					{ 
						g1 = pnMag[nPos-nWidth+1] ;
						g3 = pnMag[nPos+nWidth-1] ;
					}
				}

				// 下面利用g1-g4对梯度进行插值
				{
					dTmp1 = weight*g1 + (1-weight)*g2 ;
					dTmp2 = weight*g3 + (1-weight)*g4 ;
					
					// 当前象素的梯度是局部的最大值
					// 该点可能是个边界点
					if(dTmp>=dTmp1 && dTmp>=dTmp2)
					{
						pUnchRst[nPos] = 128 ;
					}
					else
					{
						// 不可能是边界点
						pUnchRst[nPos] = 0 ;
					}
				}
			} //else
		} // for

	}
} 

/*************************************************************************
 *
 * \函数名称：
 *   EstimateThreshold()
 *
 * \输入参数:
 *   int *pnMag               - 梯度幅度图
 *	 int nWidth               - 图象数据宽度
 *	 int nHeight              - 图象数据高度
 *   int *pnThdHigh           - 高阈值
 *   int *pnThdLow            - 低阈值
 *	 double dRatioLow         - 低阈值和高阈值之间的比例
 *	 double dRatioHigh        - 高阈值占图象象素总数的比例
 *   unsigned char *pUnchEdge - 经过non-maximum处理后的数据
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   经过non-maximum处理后的数据pUnchEdge，统计pnMag的直方图，确定阈值。
 *   本函数中只是统计pUnchEdge中可能为边界点的那些象素。然后利用直方图，
 *   根据dRatioHigh设置高阈值，存储到pnThdHigh。利用dRationLow和高阈值，
 *   设置低阈值，存储到*pnThdLow。dRatioHigh是一种比例：表明梯度小于
 *   *pnThdHigh的象素数目占象素总数目的比例。dRationLow表明*pnThdHigh
 *   和*pnThdLow的比例，这个比例在canny算法的原文里，作者给出了一个区间。
 *
 *************************************************************************
 */
void EstimateThreshold(int *pnMag, int nWidth, int nHeight, int *pnThdHigh,int *pnThdLow, 
											 unsigned char * pUnchEdge, double dRatioHigh, double dRationLow) 
{ 
	// 循环控制变量
	int y;
	int x;
	int k;
	
	// 该数组的大小和梯度值的范围有关，如果采用本程序的算法，那么梯度的范围不会超过pow(2,10)
	int nHist[1024] ;

	// 可能的边界数目
	int nEdgeNb     ;

	// 最大梯度值
	int nMaxMag     ;

	int nHighCount  ;

	nMaxMag = 0     ; 
	
	// 初始化
	for(k=0; k<1024; k++) 
	{
		nHist[k] = 0; 
	}

	// 统计直方图，然后利用直方图计算阈值
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			// 只是统计那些可能是边界点，并且还没有处理过的象素
			if(pUnchEdge[y*nWidth+x]==128)
			{
				nHist[ pnMag[y*nWidth+x] ]++;
			}
		}
	}

	nEdgeNb = nHist[0]  ;
	nMaxMag = 0         ;
	// 统计经过“非最大值抑止(non-maximum suppression)”后有多少象素
	for(k=1; k<1024; k++)
	{
		if(nHist[k] != 0)
		{
			// 最大梯度值
			nMaxMag = k;
		}
		
		// 梯度为0的点是不可能为边界点的
		// 经过non-maximum suppression后有多少象素
		nEdgeNb += nHist[k];
	}

	// 梯度比高阈值*pnThdHigh小的象素点总数目
	nHighCount = (int)(dRatioHigh * nEdgeNb +0.5);
	
	k = 1;
	nEdgeNb = nHist[1];
	
	// 计算高阈值
	while( (k<(nMaxMag-1)) && (nEdgeNb < nHighCount) )
	{
		k++;
		nEdgeNb += nHist[k];
	}

	// 设置高阈值
	*pnThdHigh = k ;

	// 设置低阈值
	*pnThdLow  = (int)((*pnThdHigh) * dRationLow+ 0.5);
}

/*************************************************************************
 *
 * \函数名称：
 *   Hysteresis()
 *
 * \输入参数:
 *   int *pnMag               - 梯度幅度图
 *	 int nWidth               - 图象数据宽度
 *	 int nHeight              - 图象数据高度
 *	 double dRatioLow         - 低阈值和高阈值之间的比例
 *	 double dRatioHigh        - 高阈值占图象象素总数的比例
 *   unsigned char *pUnchEdge - 记录边界点的缓冲区
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   本函数实现类似“磁滞现象”的一个功能，也就是，先调用EstimateThreshold
 *   函数对经过non-maximum处理后的数据pUnchSpr估计一个高阈值，然后判断
 *   pUnchSpr中可能的边界象素(=128)的梯度是不是大于高阈值nThdHigh，如果比
 *   该阈值大，该点将作为一个边界的起点，调用TraceEdge函数，把对应该边界
 *   的所有象素找出来。最后，当整个搜索完毕时，如果还有象素没有被标志成
 *   边界点，那么就一定不是边界点。
 *   
 *************************************************************************
 */
void Hysteresis(int *pnMag, int nWidth, int nHeight, double dRatioLow, 
								double dRatioHigh, unsigned char *pUnchEdge)
{
	// 循环控制变量
	int y;
	int x;

	int nThdHigh ;
	int nThdLow  ;

	int nPos;

	// 估计TraceEdge需要的低阈值，以及Hysteresis函数使用的高阈值
	EstimateThreshold(pnMag, nWidth, nHeight, &nThdHigh, 
		               &nThdLow, pUnchEdge,dRatioHigh, dRatioLow);

  // 这个循环用来寻找大于nThdHigh的点，这些点被用来当作边界点，然后用
	// TraceEdge函数来跟踪该点对应的边界
   for(y=0; y<nHeight; y++)
	 {
      for(x=0; x<nWidth; x++)
			{
				nPos = y*nWidth + x ; 

				// 如果该象素是可能的边界点，并且梯度大于高阈值，该象素作为
				// 一个边界的起点
				if((pUnchEdge[nPos] == 128) && (pnMag[nPos] >= nThdHigh))
				{
					// 设置该点为边界点
					pUnchEdge[nPos] = 255;


					TraceEdge(y, x, nThdLow, pUnchEdge, pnMag, nWidth);
					//////////////////////////////////////////////////////////////////////////
					/*
					{ 
						// 对8邻域象素进行查询
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
							// 如果该象素为可能的边界点，又没有处理过
							// 并且梯度大于阈值
							if(pUnchEdge[yy*nWidth+xx] == 128  && pnMag[yy*nWidth+xx]>=nThdLow)
							{
								// 把该点设置成为边界点
								pUnchEdge[yy*nWidth+xx] = 255;

								// 以该点为中心进行跟踪
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

	 // 那些还没有被设置为边界点的象素已经不可能成为边界点
   for(y=0; y<nHeight; y++)
	 {
		 for(x=0; x<nWidth; x++)
		 {
			 nPos = y*nWidth + x ; 
			 if(pUnchEdge[nPos] != 255)
			 {
				 // 设置为非边界点
				 pUnchEdge[nPos] = 0 ;
			 }
		 }
	 }
}

/*************************************************************************
 *
 * \函数名称：
 *   Canny()
 *
 * \输入参数:
 *   unsigned char *pUnchImage- 图象数据
 *	 int nWidth               - 图象数据宽度
 *	 int nHeight              - 图象数据高度
 *   double sigma             - 高斯滤波的标准方差
 *	 double dRatioLow         - 低阈值和高阈值之间的比例
 *	 double dRatioHigh        - 高阈值占图象象素总数的比例
 *   unsigned char *pUnchEdge - canny算子计算后的分割图
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   canny分割算子，计算的结果保存在pUnchEdge中，逻辑1(255)表示该点为
 *   边界点，逻辑0(0)表示该点为非边界点。该函数的参数sigma，dRatioLow
 *   dRatioHigh，是需要指定的。这些参数会影响分割后边界点数目的多少
 *************************************************************************
 */
void Canny(unsigned char *pUnchImage, int nWidth, int nHeight, double sigma,
					 double dRatioLow, double dRatioHigh, unsigned char *pUnchEdge)
{
	// 经过高斯滤波后的图象数据
	unsigned char * pUnchSmooth ;
  
	// 指向x方向导数的指针
	int * pnGradX ; 

	// 指向y方向导数的指针
	int * pnGradY ;

	// 梯度的幅度
	int * pnGradMag ;

	pUnchSmooth  = new unsigned char[nWidth*nHeight] ;
	pnGradX      = new int [nWidth*nHeight]          ;
	pnGradY      = new int [nWidth*nHeight]          ;
	pnGradMag    = new int [nWidth*nHeight]          ;

	// 对原图象进行滤波
	GaussianSmooth(pUnchImage, nWidth, nHeight, sigma, pUnchSmooth) ;

	// 计算方向导数
	DirGrad(pUnchSmooth, nWidth, nHeight, pnGradX, pnGradY) ;

	// 计算梯度的幅度
	GradMagnitude(pnGradX, pnGradY, nWidth, nHeight, pnGradMag) ;

	// 应用non-maximum 抑制
	NonmaxSuppress(pnGradMag, pnGradX, pnGradY, nWidth, nHeight, pUnchEdge) ;

	// 应用Hysteresis，找到所有的边界
	Hysteresis(pnGradMag, nWidth, nHeight, dRatioLow, dRatioHigh, pUnchEdge);


	// 释放内存
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
 * \函数名称：
 *   TraceEdge()
 *
 * \输入参数:
 *   int    x									- 跟踪起点的x坐标 
 *   int    y									- 跟踪起点的y坐标
 *   int nLowThd							- 判断一个点是否为边界点的低阈值
 *   unsigned char *pUnchEdge - 记录边界点的缓冲区
 *   int *pnMag               - 梯度幅度图
 *   int nWidth               - 图象数据宽度
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   递归调用  
 *   从(x,y)坐标出发，进行边界点的跟踪，跟踪只考虑pUnchEdge中没有处理并且
 *   可能是边界点的那些象素(=128),象素值为0表明该点不可能是边界点，象素值
 *   为255表明该点已经被设置为边界点，不必再考虑
 *   
 *   
 *************************************************************************
 */
void TraceEdge (int y, int x, int nLowThd, unsigned char *pUnchEdge, int *pnMag, int nWidth) 
{ 
	/*
	// 对8邻域象素进行查询
	int xNb[8] = {1, 1, 0,-1,-1,-1, 0, 1} ;
	int yNb[8] = {0, 1, 1, 1,0 ,-1,-1,-1} ;

	int yy ;
	int xx ;
	
	int k  ;

	for(k=0; k<8; k++)
	{
		yy = y + yNb[k] ;
		xx = x + xNb[k] ;
		// 如果该象素为可能的边界点，又没有处理过
		// 并且梯度大于阈值
		if(pUnchEdge[yy*nWidth+xx] == 128  && pnMag[yy*nWidth+xx]>=nLowThd)
		{
			// 把该点设置成为边界点
			pUnchEdge[yy*nWidth+xx] = 255;

			// 以该点为中心进行跟踪
			TraceEdge(yy, xx, nLowThd, pUnchEdge, pnMag, nWidth);
		}
	}
	*/

	//////////////////////////////////////////////////////////////////////////
	// 下面不采用递归调用实现函数功能
	// 对8邻域象素进行查询
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
	pNode->pPreLnk = NULL;	// 头节点的前向链接为空

	while (pNode != NULL)
	{
		PNODE pNode1 = new NODE;
		pNode1->x = pNode->x+xNb[pNode->nDir];
		pNode1->y = pNode->y+yNb[pNode->nDir];
		pNode1->nDir = 0;
		pNode1->pPreLnk = pNode;

		pNode = pNode1;

		// 如果该象素为可能的边界点，又没有处理过
		// 并且梯度大于阈值
		if(pUnchEdge[pNode->y*nWidth+pNode->x] == 128  && pnMag[pNode->y*nWidth+pNode->x]>=nLowThd)
		{
			// 把该点设置成为边界点
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

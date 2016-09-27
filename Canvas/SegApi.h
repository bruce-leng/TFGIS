#ifndef _SEG_API_H
#define _SEG_API_H

#include "TILdef.h"

	// canny Ыузг
void GaussianSmooth(unsigned char *pUnchImg, int nWidth, int nHeight, double sigma, unsigned char * pUnchSmthdImg);
void MakeGauss(double sigma, double **pdKernel, int *pnWindowSize) ; 
void DirGrad(unsigned char *pUnchSmthdImg, int nWidth, int nHeight,
						 int *pnGradX , int *pnGradY);
void GradMagnitude(int *pnGradX, int *pnGradY, int nWidth, int nHeight, int *pnMag) ; 
void NonmaxSuppress(int *pnMag, int *pnGradX, int *pnGradY, int nWidth, int nHeight,	unsigned char *pUnchRst);
void EstimateThreshold(int *pnMag, int nWidth, int nHeight, int *pnThdHigh,int *pnThdLow, 
											 unsigned char * pUnchEdge, double dRatioHigh, double dRationLow) ;
void Canny(unsigned char *pUnchImage, int nWidth, int nHeight, double sigma,
					 double dRatioLow, double dRatioHigh, unsigned char *pUnchEdge) ;
void Hysteresis(int *pnMag, int nWidth, int nHeight, double dRatioLow, 
								double dRatioHigh, unsigned char *pUnchEdge) ;
void TraceEdge (int y, int x, int nLowThd, unsigned char *pUnchEdge, int *pnMag, int nWidth) ;

#endif
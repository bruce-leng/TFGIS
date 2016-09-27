#include "StdAfx.h"
#include "TilSys.h"
#include "TilSysDoc.h"
#include "TilCanvas.h"
#include "Image.h"
#include "DlgThreshold.h"
#include "DlgPointStre.h"
#include "ximage.h"
#include "EdgeFind.h"
#include "TilComFunction.h"
#include <process.h>

// 图像阈值分割
void CTilSysDoc::OnImageThreshold() 
{
	BeginWaitCursor();

	CTilImage* pImageMgr = GetCanvasMgr()->GetCurCanvas()->GetImage();
	CImage* pImage = pImageMgr->GetCurImage();

#if 0
	CImage image1(*pImage);
	CImage image2(*pImage);

	long kernel[]={-1,0,1,-2,0,2,-1,0,1};
	image1.Filter(kernel,3,1,0);

	long kernel1[]={1,2,1,0,0,0,-1,-2,-1};
	image2.Filter(kernel1,3,1,0);

	for (int i=0; i<pImage->GetWidth(); i++)
	{
		for (int j=0; j<pImage->GetHeight(); j++)
		{
			RGBQUAD rgb1 = image1.GetPixelColor(i,j);
			RGBQUAD rgb2 = image2.GetPixelColor(i,j);
			int nMag = sqrt(rgb1.rgbBlue*rgb1.rgbBlue+rgb2.rgbBlue*rgb2.rgbBlue);
			pImage->SetPixelColor(i,j,RGB(nMag,nMag,nMag));
		}
	}
#endif

#if 0
	long kernel[] = {-2,-4,-4,-4,-2,-4,0,8,0,-4,-4,8,24,8,-4,-4,0,8,0,-4,-2,-4,-4,-4,-2};
	pImage->Filter(kernel, 5, 1, 0);
#endif

#if 1
	DlgThreshold dlg;
	if (dlg.DoModal() == IDOK)
	{
		pImage->Threshold(dlg.m_level);
	}
#endif

#if 0
	// 图像最小值、中值、最大值滤波或邻域平均
#define EDGE 1
	pImage->GrayScale();
	CImage image(*pImage);
	int nWidth = pImage->GetWidth();
	int nHeight = pImage->GetHeight();
	for (int i=EDGE; i<nWidth-EDGE; i++)
	{
		for (int j=EDGE; j<nHeight-EDGE; j++)
		{
			RGBQUAD rgb;
			int nAve=0;
			for (int m=-EDGE; m<=EDGE; m++)
			{
				for (int n=-EDGE; n<=EDGE; n++)
				{
					rgb = pImage->GetPixelColor(i+m,j+n);
					nAve += rgb.rgbBlue;
				}
			}
			image.SetPixelColor(i,j, RGB(nAve/9, nAve/9, nAve/9));
		}
	}
	pImage->Copy(image);
#endif

#if 0
	// 中值滤波
	pImage->Median();
#endif

#if 0
	// 自动图像二值化
	int nVal = pImage->GetBestThreshold();
	pImage->Threshold(nVal);
#endif

	pImageMgr->SetModifiedFlag();
	UpdateAllViews(NULL);
	EndWaitCursor();
}

#include "Veins.h"
void ImageColorize()
{
//	CTilImage* pImageMgr = GetCanvasMgr()->GetCurCanvas()->GetImage();
//	CImage* pImage = pImageMgr->GetCurImage();
//	pImage->HistogramEqualize();
}
int g_nIdx = 0;
void CTilSysDoc::OnImageColorize() 
{
	// 图像均衡化
	CImage* pImage = GetCanvasMgr()->GetCurCanvas()->GetImage()->GetCurImage();
	pImage->Light(39,20);
	pImage->SetModifiedFlag(TRUE);

	UpdateAllViews(NULL);
	EndWaitCursor();
}

void CTilSysDoc::OnImageNormalize() 
{
	m_lMenuCommand = ID_IMAGE_NORMALIZE;
	m_hThread = (HANDLE)_beginthread(RunImageThread, 0, this);
}

void CTilSysDoc::OnImageLighten() 
{
	m_lMenuCommand = ID_IMAGE_LIGHTEN;
	m_hThread = (HANDLE)_beginthread(RunImageThread, 0, this);
}

void CTilSysDoc::OnImageDarken() 
{
	m_lMenuCommand = ID_IMAGE_DARKEN;
	m_hThread = (HANDLE)_beginthread(RunImageThread, 0, this);
}

void CTilSysDoc::OnImageContrast() 
{
	m_lMenuCommand = ID_IMAGE_CONTRAST;
	m_hThread = (HANDLE)_beginthread(RunImageThread, 0, this);
}

void CTilSysDoc::OnImageLesscontrast() 
{
	m_lMenuCommand = ID_IMAGE_LESSCONTRAST;
	m_hThread = (HANDLE)_beginthread(RunImageThread, 0, this);
}

void CTilSysDoc::OnImageMedian() 
{
	m_lMenuCommand = ID_IMAGE_MEDIAN;
	m_hThread = (HANDLE)_beginthread(RunImageThread, 0, this);
}

void CTilSysDoc::OnImageErode() 
{
	m_lMenuCommand = ID_IMAGE_ERODE;
	m_hThread = (HANDLE)_beginthread(RunImageThread, 0, this);
}

// 图像腐蚀
void CTilSysDoc::OnImageDilate() 
{
	m_lMenuCommand = ID_IMAGE_DILATE;
	m_hThread = (HANDLE)_beginthread(RunImageThread, 0, this);
}

void CTilSysDoc::OnImageBlur() 
{
	m_lMenuCommand = ID_IMAGE_BLUR;
	m_hThread = (HANDLE)_beginthread(RunImageThread, 0, this);
}

void CTilSysDoc::OnImageSoften() 
{
	m_lMenuCommand = ID_IMAGE_SOFTEN;
	m_hThread = (HANDLE)_beginthread(RunImageThread, 0, this);
}

void CTilSysDoc::OnImageGaussian3x3() 
{
	m_lMenuCommand = ID_IMAGE_GAUSSIAN3X3;
	m_hThread = (HANDLE)_beginthread(RunImageThread, 0, this);
}

void CTilSysDoc::OnImageGaussian5x5() 
{
	m_lMenuCommand = ID_IMAGE_GAUSSIAN5X5;
	m_hThread = (HANDLE)_beginthread(RunImageThread, 0, this);
}

void CTilSysDoc::OnImageSharpen() 
{
	m_lMenuCommand = ID_IMAGE_SHARPEN;
	m_hThread = (HANDLE)_beginthread(RunImageThread, 0, this);
}

void CTilSysDoc::OnUpdateImageThreshold(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetCanvasMgr()->GetCurCanvas() != NULL && m_hThread == NULL);
}

void CTilSysDoc::OnUpdateImageColorize(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetCanvasMgr()->GetCurCanvas() != NULL && m_hThread == NULL);
}

void CTilSysDoc::OnUpdateImageLighten(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetCanvasMgr()->GetCurCanvas() != NULL && m_hThread == NULL);
}

void CTilSysDoc::OnUpdateImageDarken(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetCanvasMgr()->GetCurCanvas() != NULL && m_hThread == NULL);
}

void CTilSysDoc::OnUpdateImageContrast(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetCanvasMgr()->GetCurCanvas() != NULL && m_hThread == NULL);
}

void CTilSysDoc::OnUpdateImageLesscontrast(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetCanvasMgr()->GetCurCanvas() != NULL && m_hThread == NULL);
}

void CTilSysDoc::OnUpdateImageMedian(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetCanvasMgr()->GetCurCanvas() != NULL && m_hThread == NULL);
}

void CTilSysDoc::OnUpdateImageErode(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetCanvasMgr()->GetCurCanvas() != NULL && m_hThread == NULL);
}

void CTilSysDoc::OnUpdateImageDilate(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetCanvasMgr()->GetCurCanvas() != NULL && m_hThread == NULL);
}

void CTilSysDoc::OnUpdateImageContour(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetCanvasMgr()->GetCurCanvas() != NULL && m_hThread == NULL);
}

void CTilSysDoc::OnUpdateImageBlur(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetCanvasMgr()->GetCurCanvas() != NULL && m_hThread == NULL);
}

void CTilSysDoc::OnUpdateImageSoften(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetCanvasMgr()->GetCurCanvas() != NULL && m_hThread == NULL);
}

void CTilSysDoc::OnUpdateImageGaussian3x3(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetCanvasMgr()->GetCurCanvas() != NULL && m_hThread == NULL);
}

void CTilSysDoc::OnUpdateImageGaussian5x5(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetCanvasMgr()->GetCurCanvas() != NULL && m_hThread == NULL);
}

void CTilSysDoc::OnUpdateImageSharpen(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetCanvasMgr()->GetCurCanvas() != NULL && m_hThread == NULL);
}

void CTilSysDoc::OnUpdateImageEdge(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetCanvasMgr()->GetCurCanvas() != NULL && m_hThread == NULL);
}

void CTilSysDoc::OnImageStretch() 
{
	// TODO: Add your command handler code here
	// 循环变量
	int i;

	// 各颜色分量的灰度分布密度
	CImage* pImage = GetCanvasMgr()->GetCurCanvas()->GetImage()->GetCurImage();
	long lGray[256];
	pImage->Histogram(NULL,NULL,NULL,lGray,0);

	long lArea = pImage->GetWidth()*pImage->GetHeight();

	// 创建对话框
	CDlgPointStre dlg;
	dlg.m_pDoc = this;

	// 传递灰度分布密度数据给面板类
	for (i = 0; i <256; i++)
		dlg.m_fIntensity[i] = lGray[i]*1.0/lArea;

	// 显示对话框，由用户进行灰度折线变换
	dlg.DoModal();
}

void CTilSysDoc::OnUpdateImageStretch(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetCanvasMgr()->GetCurCanvas()!=NULL && m_hThread == NULL);
}

void CTilSysDoc::OnImageUsm() 
{
	// TODO: Add your command handler code here
	m_lMenuCommand = ID_IMAGE_USM;
	m_hThread = (HANDLE)_beginthread(RunImageThread, 0, this);
}

void CTilSysDoc::OnUpdateImageUsm(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetCanvasMgr()->GetCurCanvas()!=NULL && m_hThread == NULL);
}

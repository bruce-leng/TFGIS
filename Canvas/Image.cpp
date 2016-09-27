// Image.cpp: implementation of the CImage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Image.h"
#include "TilComFunction.h"
#include "MathComput.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CImage::CImage()
{
	m_dbZoomX = 1.0;
	m_dbZoomY = 1.0;
	m_bModified = FALSE;
}

CImage::CImage(const CImage& image)
{
	Copy(image);

	m_dbZoomX = image.m_dbZoomX;
	m_dbZoomY = image.m_dbZoomY;
	m_bModified = image.m_bModified;
}

CImage::CImage(const CString& strPathName, DWORD imagetype) :
CxImage(strPathName, imagetype)
{
	m_dbZoomX = 1.0;
	m_dbZoomY = 1.0;
	m_bModified = FALSE;
//
//	char Path[MAX_PATH];
//	::GetTempPath(MAX_PATH, Path);
//	::GetTempFileName(Path, _T("TS@"), 0, Path);
//	// Check for valid bmp file
//	CFile file;
//	CFileException ex;
//
//	// Try to open file
//	if(!file.Open(Path, CFile::modeReadWrite  | CFile::typeBinary,&ex))
//	{
//		#ifdef _DEBUG
//		  afxDump << "File could not be opened " << ex.m_cause << "\n";
//		#endif
//		AfxMessageBox("Unable to open file for reading");
//	}
//
//	// File header
//	TRY
//	{
//		file.Write(info.pImage, head.biWidth*head.biHeight*head.biBitCount/8);
//	}
//	CATCH(CFileException, e)
//	{
//#ifdef _DEBUG
//		afxDump << "Error during reading " << e->m_cause << "\n";
//#endif
//		AfxMessageBox("Error during reading file header");
//		file.Close();
//	}
//	END_CATCH	
//	file.Close();
//
//	// Success, also set FileName
//	m_pMemFile = new CMemMapFile;
//	m_pMemFile->MapFile(Path);
//	info.pImage = (unsigned char*)m_pMemFile->Open();
}

CImage::~CImage()
{
//	if(m_pMemFile)
//	{
//		m_pMemFile->UnMap();
//		delete m_pMemFile;
//		m_pMemFile = NULL;
//	}
}

long CImage::Draw(CDC* pDC)
{
	if (m_dbZoomX == 1.0 && m_dbZoomY == 1.0)
		return CxImage::Draw(pDC->GetSafeHdc());
	else
	{
		long lWidth = long(GetWidth()*m_dbZoomX+0.5);
		long lHeight = long(GetHeight()*m_dbZoomY+0.5);
		return CxImage::Stretch(pDC->GetSafeHdc(), 0, 0, lWidth, lHeight);
	}	
}

//long CImage::Draw(CDC* pDC, const RECT& rectDst, const RECT& rectSrc)
//{
//	RECT rect;
//	rect.left = rectSrc.left*1.0/m_dbZoomX+0.5;
//	rect.right = rectSrc.right*1.0/m_dbZoomX+0.5;
//	rect.top = rectSrc.top*1.0/m_dbZoomY+0.5;
//	rect.bottom rectSrc.bottom*1.0/m_dbZoomY+0.5;
//
//	return Stretch(pDC->GetSafeHdc(), rectDst, rect);
//}
//
void CImage::SetZoomFactor(long double dbZoomX, long double dbZoomY)
{
	ASSERT(dbZoomX > 0 && dbZoomY > 0);
	m_dbZoomX = dbZoomX;
	m_dbZoomY = dbZoomY;
}

void CImage::GetZoomFactor(long double& dbZoomX, long double& dbZoomY)
{
	dbZoomX = m_dbZoomX;
	dbZoomY = m_dbZoomY;
}

void CImage::SetModifiedFlag(BOOL bFlag)
{
	m_bModified = bFlag;
}

BOOL CImage::IsModified() const
{
	return m_bModified;
}

// 由差别分析法得到最佳分割阈值(08.06.19)
// 修改(08.08.31)
int CImage::GetBestThreshold()
{
	CImage image(*this);
	if (!image.GetDIB()) return false;
	if (image.GetBpp() == 1) return false;

	image.GrayScale();

	// 返回的变量及全体像素的灰度和
	int nThre, lSum;
	nThre = lSum = 0;

	// 各灰度级的灰度数
	long lPixNum[256];
	memset(lPixNum, 0, 256 * sizeof(long));

	// 统计图像中各灰度级像素的个数
	for (long i=0; i<head.biHeight; i++)
	{
		for (long j=0; j<head.biWidth; j++)
		{
			BYTE color = image.GetPixelGray(j,i);
			lPixNum[color] ++;
			lSum += color;
		}
	}

	// 设分割阈值为t,将{0,1,...,t-1}和{t,t+1,...,255}当作两组,
	// 采用两组的平均值的方差(组间方差)与各组的方差(组内方差)之比
	// 当这个差别比成为最大时的阈值t即为所求
	// 组1和组2的像素数
	long lNum1 = 0;
	long lNum2 = 0;

	// 组1和组2的平均灰度
	double dbMean1, dbMean2;

	// 组1和组2的灰度和
	long lSum1, lSum2;

	// 组间方差
	double dbR2b;

	// 设初始分割阈值为1,即<1的为1组,>=1的为1组
	// 第1组的像素数
	lNum1 = lPixNum[0];

	// 第2组的像素数
	for (i = 1; i < 256; i ++)
		lNum2 += lPixNum[i];

	// 组1和组2的像素初始灰度和
	lSum1 = 0;
	lSum2 = lSum - lSum1;

	// 最大组间方差
	double dbMaxR2b = 0.0;

	// 
	for (i = 1; i < 256; i ++)
	{
		// 第1组均值
		if (0 == lNum1)
			dbMean1 = 0;
		else
			dbMean1 = lSum1 * 1.0 / lNum1;

		// 第2组均值
		if (0 == lNum2)
			dbMean2 = 0;
		else
			dbMean2 = lSum2 * 1.0 / lNum2;

		// 组间方差
		dbR2b = (lNum1 * lNum2 * (dbMean1-dbMean2) * (dbMean1-dbMean2))
						/ (double(lNum1 + lNum2) * double(lNum1 + lNum2));

		// 找最大组间方差值
		if (dbMaxR2b < dbR2b)
		{
			dbMaxR2b = dbR2b;
			nThre = i;
		}

		//
		lNum1 += lPixNum[i];
		lNum2 -= lPixNum[i];

		lSum1 += lPixNum[i] * i;
		lSum2 -= lPixNum[i] * i;
	}

	// 特殊情况处理
	if (0 == nThre)
		nThre = 1;

	return nThre;
}

void CImage::BestThreshold()
{
	int nWidth = GetWidth();
	int nHeight = GetHeight();
}

#include "SegApi.h"
// Canny算子
void CImage::EdgeCanny()
{
	int i,j;

	// 图像宽度、高度
	int nWidth = (int)GetWidth();
	int nHeight = (int)GetHeight();

	// 开辟内存，存储图象数据
	LPBYTE lpSrcDIBBits = new BYTE [nWidth * nHeight];

	// canny算子计算后的结果
  LPBYTE lpDstDIBBits = new BYTE [nWidth * nHeight];

	// 读数据
	for (i=0; i < nHeight; i ++)
	{
		for (j=0; j < nWidth; j ++)
		{
			lpSrcDIBBits[nWidth * i + j] = GetPixelGray(j, i);
		}
	}
	
	// 调用canny函数进行边界提取
	Canny(lpSrcDIBBits, nWidth, nHeight, 0.4, 0.4, 0.79, lpDstDIBBits);
	
	// 写数据
	for (i = 0; i < nHeight; i ++)
	{
		for (j = 0; j < nWidth; j ++)
		{
			UCHAR uGrey = lpDstDIBBits[nWidth * i + j];
			SetPixelColor(j, i, RGB(uGrey, uGrey, uGrey));
		}
	}

	// 释放内存
	delete [] lpSrcDIBBits;
	delete [] lpDstDIBBits;

}

// 对二值图像进行细化
BOOL CImage::MorphThin()
{
	//定义变量
	unsigned char *pBits;
	int lWidth,lHeight;
	
	//获得图像指针
	pBits = GetBits();

	lWidth = GetWidth();
	lHeight = GetHeight();

	/////////////////////////////////////////////////////////
	RGBQUAD rgbSrc;
	
	//脏标记
	BOOL bModified;

	//循环变量
	long i;
	long j;
	int  n;
	int  m;

	//四个条件
	BOOL bCondition1;
	BOOL bCondition2;
	BOOL bCondition3;
	BOOL bCondition4;

	//计数器
	unsigned char nCount;
	
	//像素值
	unsigned char pixel;

	//5×5相邻区域像素值
	unsigned char neighbour[5][5];

	// 暂时分配内存，初始化新分配的内存，设定初始值为255
	CImage newImage(*this);
	newImage.Clear(0);
	
	Negative();

	bModified=TRUE;

	while(bModified)
	{

		bModified = FALSE;

		for(j = 2; j <lHeight-2; j++)
		{
			for(i = 2;i <lWidth-2; i++)
			{

				bCondition1 = FALSE;
				bCondition2 = FALSE;
				bCondition3 = FALSE;
				bCondition4 = FALSE;

				//由于使用5×5的结构元素，为防止越界，所以不处理外围的几行和几列像素

				// 指向源图像倒数第j行，第i个象素的指针	
				rgbSrc = GetPixelColor(i, j);

				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel = (unsigned char)rgbSrc.rgbBlue;

				//如果源图像中当前点为白色，则跳过
				if(pixel == 255)
					continue;
				//目标图像中含有0和255外的其它灰度值
				else if(pixel != 0)
					continue;

				//获得当前点相邻的5×5区域内像素值，白色用0代表，黑色用1代表
				for (m = 0;m < 5;m++ )
				{
					for (n = 0;n < 5;n++)
					{
						rgbSrc = GetPixelColor(n-2,(4-m)-2);
						pixel = (unsigned char)rgbSrc.rgbBlue;
						neighbour[m][n] =(255 - pixel) / 255;
					}
				}
				//逐个判断条件。
				//判断2<=NZ(P1)<=6
				nCount =  neighbour[1][1] + neighbour[1][2] + neighbour[1][3] \
						+ neighbour[2][1] + neighbour[2][3] + \
						+ neighbour[3][1] + neighbour[3][2] + neighbour[3][3];
				if ( nCount >= 2 && nCount <=6)
					bCondition1 = TRUE;

				//判断Z0(P1)=1
				nCount = 0;
				if (neighbour[1][2] == 0 && neighbour[1][1] == 1)
					nCount++;
				if (neighbour[1][1] == 0 && neighbour[2][1] == 1)
					nCount++;
				if (neighbour[2][1] == 0 && neighbour[3][1] == 1)
					nCount++;
				if (neighbour[3][1] == 0 && neighbour[3][2] == 1)
					nCount++;
				if (neighbour[3][2] == 0 && neighbour[3][3] == 1)
					nCount++;
				if (neighbour[3][3] == 0 && neighbour[2][3] == 1)
					nCount++;
				if (neighbour[2][3] == 0 && neighbour[1][3] == 1)
					nCount++;
				if (neighbour[1][3] == 0 && neighbour[1][2] == 1)
					nCount++;
				if (nCount == 1)
					bCondition2 = TRUE;

				//判断P2*P4*P8=0 or Z0(p2)!=1
				if (neighbour[1][2]*neighbour[2][1]*neighbour[2][3] == 0)
					bCondition3 = TRUE;
				else
				{
					nCount = 0;
					if (neighbour[0][2] == 0 && neighbour[0][1] == 1)
						nCount++;
					if (neighbour[0][1] == 0 && neighbour[1][1] == 1)
						nCount++;
					if (neighbour[1][1] == 0 && neighbour[2][1] == 1)
						nCount++;
					if (neighbour[2][1] == 0 && neighbour[2][2] == 1)
						nCount++;
					if (neighbour[2][2] == 0 && neighbour[2][3] == 1)
						nCount++;
					if (neighbour[2][3] == 0 && neighbour[1][3] == 1)
						nCount++;
					if (neighbour[1][3] == 0 && neighbour[0][3] == 1)
						nCount++;
					if (neighbour[0][3] == 0 && neighbour[0][2] == 1)
						nCount++;
					if (nCount != 1)
						bCondition3 = TRUE;
				}

				//判断P2*P4*P6=0 or Z0(p4)!=1
				if (neighbour[1][2]*neighbour[2][1]*neighbour[3][2] == 0)
					bCondition4 = TRUE;
				else
				{
					nCount = 0;
					if (neighbour[1][1] == 0 && neighbour[1][0] == 1)
						nCount++;
					if (neighbour[1][0] == 0 && neighbour[2][0] == 1)
						nCount++;
					if (neighbour[2][0] == 0 && neighbour[3][0] == 1)
						nCount++;
					if (neighbour[3][0] == 0 && neighbour[3][1] == 1)
						nCount++;
					if (neighbour[3][1] == 0 && neighbour[3][2] == 1)
						nCount++;
					if (neighbour[3][2] == 0 && neighbour[2][2] == 1)
						nCount++;
					if (neighbour[2][2] == 0 && neighbour[1][2] == 1)
						nCount++;
					if (neighbour[1][2] == 0 && neighbour[1][1] == 1)
						nCount++;
					if (nCount != 1)
						bCondition4 = TRUE;
				}
				if(bCondition1 && bCondition2 && bCondition3 && bCondition4)
				{
					newImage.SetPixelColor(i, j, RGB(0, 0, 0));
					bModified = TRUE;
				}
				else
				{
					newImage.SetPixelColor(i, j, RGB(255, 255, 255));
				}
			}
		}
		// 复制腐蚀后的图像
//		Copy(newImage);
	}
	// 复制腐蚀后的图像
	Copy(newImage);

	// 返回
	return TRUE;
}

#include "afxtempl.h"
BOOL CImage::MorphThining()
{
	RGBQUAD						rgb;
	LONG              x,y;
	int								num;
	BOOL              Finished;
	int               nw,n,ne,w,e,sw,s,se;
	static int erasetable[256]={
								0,0,1,1,0,0,1,1,
								1,1,0,1,1,1,0,1,
								1,1,0,0,1,1,1,1,
								0,0,0,0,0,0,0,1,
								
								0,0,1,1,0,0,1,1,
								1,1,0,1,1,1,0,1,
								1,1,0,0,1,1,1,1,
								0,0,0,0,0,0,0,1,
								
								1,1,0,0,1,1,0,0,
								0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,
								
								1,1,0,0,1,1,0,0,
								1,1,0,1,1,1,0,1,
								0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,

								0,0,1,1,0,0,1,1,
								1,1,0,1,1,1,0,1,
								1,1,0,0,1,1,1,1,
								0,0,0,0,0,0,0,1,
								
								0,0,1,1,0,0,1,1,
								1,1,0,1,1,1,0,1,
								1,1,0,0,1,1,1,1,
								0,0,0,0,0,0,0,0,
								
								1,1,0,0,1,1,0,0,
								0,0,0,0,0,0,0,0,
								1,1,0,0,1,1,1,1,
								0,0,0,0,0,0,0,0,

								1,1,0,0,1,1,0,0,
								1,1,0,1,1,1,0,0,
								1,1,0,0,1,1,1,0,
								1,1,0,0,1,0,0,0
						  };

	long lWidth = GetWidth();
	long lHeight = GetHeight();
	long x1, y1, x2, y2;

	// 图像反色，将边界线变成黑色
	Negative();
	CImage image(*this);
	
	Finished=FALSE;
	while(!Finished){
		Finished=TRUE;
		for (y=1;y<lHeight-1;y++){
			x1 = 0; y1 = lHeight-y-1;
			x2 = x1; y2 = y1;
			x=1; 
			while(x<lWidth-1){
				rgb = GetPixelColor(x1+x, y);
				if(rgb.rgbBlue==0){
					w = (unsigned char)GetPixelColor(x1+x-1, y).rgbBlue;
					e = (unsigned char)GetPixelColor(x1+x+1, y).rgbBlue;
					
					if( (w==255)|| (e==255)){
						nw = (unsigned char)GetPixelColor(x1+x-1, y+1).rgbBlue;
						n  = (unsigned char)GetPixelColor(x1+x  , y+1).rgbBlue;
						ne = (unsigned char)GetPixelColor(x1+x+1, y+1).rgbBlue;
						sw = (unsigned char)GetPixelColor(x1+x-1, y-1).rgbBlue;
						s  = (unsigned char)GetPixelColor(x1+x  , y-1).rgbBlue;
						se = (unsigned char)GetPixelColor(x1+x+1, y-1).rgbBlue;
						
						num=nw/255+n/255*2+ne/255*4+w/255*8+e/255*16+sw/255*32+s/255*64+se/255*128;
						if(erasetable[num]==1){
							SetPixelColor(x1+x, y, RGB(255, 255, 255));
							image.SetPixelColor(x2+x, y, RGB(255, 255, 255));
							Finished=FALSE;
							x++;
						}
					}
				}
				x++;
			}
		}
		
		for (x=1;x<lWidth-1;x++){ 
			y=1;
			while(y<lHeight-1){
				x1 = 0; 	y1 = lHeight-y-1;
				x2 = x1;  y2 = y1;
				
				rgb = GetPixelColor(x1+x, y);
				if(rgb.rgbBlue==0){
					n = (unsigned char)GetPixelColor(x1+x, y+1).rgbBlue;
					s = (unsigned char)GetPixelColor(x1+x, y-1).rgbBlue;
					
					if( (n==255)|| (s==255)){
						nw = (unsigned char)GetPixelColor(x1+x-1, y+1).rgbBlue;
						ne = (unsigned char)GetPixelColor(x1+x+1, y+1).rgbBlue;
						w  = (unsigned char)GetPixelColor(x1+x-1, y  ).rgbBlue;
						e  = (unsigned char)GetPixelColor(x1+x+1, y  ).rgbBlue;
						sw = (unsigned char)GetPixelColor(x1+x-1, y-1).rgbBlue;
						se = (unsigned char)GetPixelColor(x1+x+1, y-1).rgbBlue;
						
						num=nw/255+n/255*2+ne/255*4+w/255*8+e/255*16+sw/255*32+s/255*64+se/255*128;
						if(erasetable[num]==1){
							SetPixelColor(x1+x, y, RGB(255, 255, 255));
							image.SetPixelColor(x2+x, y, RGB(255, 255, 255));
							Finished=FALSE;
							y++;
						}
					}
				}
				y++;
			}
		} 
	}

	// 将边界线反色成白色
	image.Negative();
	Copy(image);

	//////////////////////////////////////////////////////////////////////////
	// 若当前点的8邻域有3个目标像素点，或无目标像素点，则将当前点标记为背景像素
	CArray<POINT,  POINT> keyPts;
	int nCount;
	int i,j,m;
	POINT pt;
	for (i=1; i<lWidth-1; i++)
	{
		for (j=1; j<lHeight-1; j++)
		{
			rgb = GetPixelColor(i, j);
			if (rgb.rgbBlue != 255)
				continue;

			nCount =0;
			for (m=-1; m<=1; m++)
			{
				for (n=-1; n<=1; n++)
				{
					rgb = GetPixelColor(i+m, j+n);
					nCount += rgb.rgbBlue/255;
				}
			}

			// 若当前像素8邻域有3个目标像素点或无目标像素点
			if (nCount>=4)	// nCount包含了中心点
			{
				pt.x = i;
				pt.y = j;
				keyPts.Add(pt);
			}
		}
	}

	for (i=0; i<keyPts.GetSize(); i++)
	{
		pt = keyPts.GetAt(i);
		SetPixelColor(pt.x, pt.y, RGB(0,0,0));
	}

	for (i=1; i<lWidth-1; i++)
	{
		for (j=1; j<lHeight-1; j++)
		{
			rgb = GetPixelColor(i, j);
			if (rgb.rgbBlue != 255)
				continue;

			nCount =0;
			for (m=-1; m<=1; m++)
			{
				for (n=-1; n<=1; n++)
				{
					rgb = GetPixelColor(i+m, j+n);
					nCount += rgb.rgbBlue/255;
				}
			}

			// 若当前像素8邻域有3个目标像素点或无目标像素点
			if (nCount==1)	// nCount包含了中心点
			{
				SetPixelColor(i,j,RGB(0,0,0));
			}
		}
	}

	return TRUE;
}

// 取得所有目标游程编码,每个目标的游程编码分别存储(08.06.19)（外边界按8邻域搜索）
void CImage::ObjAllRLC(ObjRunLenCode* pObjAllRLC, UCHAR uObjClr/*=255*/)
{
	int i, x, y, nSum, nNum, h, w;
	queue< RUNLENGTHCODE > que;
	RUNLENGTHCODE item;
	BOOL bFlag = FALSE;

	// 图像宽度、高度
	int nWidth = (int)GetWidth();
	int nHeight = (int)GetHeight();

	// 取得所有对象色的游程编码
	ObjRunLenCode tmpRLC;
	RunLengthcode(&tmpRLC, uObjClr);

	nNum = 0;
	for (h=0; h<tmpRLC.m_nObjNum; h++)
	{
		for (w=0; w<tmpRLC.m_ppObjRun[h]->nRunNum; w++)
		{
			if (tmpRLC.m_ppObjRun[h]->RC[w].y>=0 && !tmpRLC.m_ppObjRun[h]->RC[w].bFlags)
			{
				tmpRLC.m_ppObjRun[h]->RC[w].bFlags = TRUE;
				que.push(tmpRLC.m_ppObjRun[h]->RC[w]);

				//如果存在新的对象,则设标志位为TRUE
				bFlag = TRUE;
			}

			// 
			while(!que.empty())
			{
				// 取得队头元素并弹出
				item = que.front();
				que.pop();

				//********************************************************
				//如果当前游程的上一行存在且上一行的游程数大于0
				if (item.y-1>=0 && tmpRLC.m_ppObjRun[item.y-1]->nRunNum>0)
				{
					y = item.y-1;

					//若当前游程的右边界是图像右边界
					if (item.xEnd+1 == nWidth)
					{
						for (i=tmpRLC.m_ppObjRun[y]->nRunNum-1; i>=0; i--)
						{
							if (tmpRLC.m_ppObjRun[y]->RC[i].xEnd+1 >= item.xStart)
							{
								if (!tmpRLC.m_ppObjRun[y]->RC[i].bFlags)
								{
									tmpRLC.m_ppObjRun[y]->RC[i].bFlags = TRUE;
									que.push(tmpRLC.m_ppObjRun[y]->RC[i]);
								}
							}
							else
								break;
						}
					}
					else	//若当前游程的右边界"不是"图像右边界
					{
						x = item.xEnd+1;
						nSum = tmpRLC.m_ppObjRun[y]->nRunNum;

						//如果在最后一个游程的右边
						if (x > tmpRLC.m_ppObjRun[y]->RC[nSum-1].xEnd)
						{
							for (i=nSum-1; i>=0; i--)
							{
								if (tmpRLC.m_ppObjRun[y]->RC[i].xEnd+1 >= item.xStart)
								{
									//如果该游程未被标记过(即y!=-1),则进队列
									if (!tmpRLC.m_ppObjRun[y]->RC[i].bFlags)
									{
										tmpRLC.m_ppObjRun[y]->RC[i].bFlags = TRUE;
										que.push(tmpRLC.m_ppObjRun[y]->RC[i]);
									}
								}
								else
									break;
							}
						}
						// 如果在第一个游程起始位置和最后一个游程末端位置之间
						else if (x>=tmpRLC.m_ppObjRun[y]->RC[0].xStart
							&& x<=tmpRLC.m_ppObjRun[y]->RC[nSum-1].xEnd)
						{
							for(i = nSum-1; i >= 0; i --)
							{
								if(i > 0)
								{
									while (x>tmpRLC.m_ppObjRun[y]->RC[i-1].xEnd
										&& x<tmpRLC.m_ppObjRun[y]->RC[i].xStart)
										x--;
								}

								if (x<=tmpRLC.m_ppObjRun[y]->RC[i].xEnd
									&& x>=tmpRLC.m_ppObjRun[y]->RC[i].xStart)
								{
									if (!tmpRLC.m_ppObjRun[y]->RC[i].bFlags
										&& x+1>=item.xStart)
									{
										tmpRLC.m_ppObjRun[y]->RC[i].bFlags = TRUE;
										que.push(tmpRLC.m_ppObjRun[y]->RC[i]);
									}

									nSum = i;
									break;
								}
							}

							if (nSum > 0)
							{
								for (i=nSum-1; i>=0; i--)
								{
									if (tmpRLC.m_ppObjRun[y]->RC[i].xEnd+1 >= item.xStart)
									{
										if (!tmpRLC.m_ppObjRun[y]->RC[i].bFlags)
										{
											tmpRLC.m_ppObjRun[y]->RC[i].bFlags = TRUE;
											que.push(tmpRLC.m_ppObjRun[y]->RC[i]);
										}
									}
									else
										break;
								}
							} // if (nSum > 0)
						} //
					} // if (item.xEnd+1 == nWidth)
				}	// if (item.y-1>=0 && tmpRLC.m_ppObjRun[y]->nRunNum>0)

				//********************************************************
				//如果当前游程的下一行存在且下一行的游程数大于0
				if (item.y+1<nHeight && tmpRLC.m_ppObjRun[item.y+1]->nRunNum>0)
				{
					y = item.y+1;

					//若当前游程的右边界是图像右边界
					if (item.xEnd+1 == nWidth)
					{
						for (i=tmpRLC.m_ppObjRun[y]->nRunNum-1; i>=0; i--)
						{
							if (tmpRLC.m_ppObjRun[y]->RC[i].xEnd+1 >= item.xStart)
							{
								if (!tmpRLC.m_ppObjRun[y]->RC[i].bFlags)
								{
									tmpRLC.m_ppObjRun[y]->RC[i].bFlags = TRUE;
									que.push(tmpRLC.m_ppObjRun[y]->RC[i]);
								}
							}
							else
								break;
						}
					}
					else		//若当前游程的右边界"不是"图像右边界
					{
						x = item.xEnd+1;
						nSum = tmpRLC.m_ppObjRun[y]->nRunNum;

						//如果在最后一个游程的右边
						if (x > tmpRLC.m_ppObjRun[y]->RC[nSum-1].xEnd)
						{
							for(i=nSum-1; i>=0; i--)
							{
								if (tmpRLC.m_ppObjRun[y]->RC[i].xEnd+1 >= item.xStart)
								{
									//如果该游程未被标记过(即y!=-1),则进队列
									if (!tmpRLC.m_ppObjRun[y]->RC[i].bFlags)
									{
										tmpRLC.m_ppObjRun[y]->RC[i].bFlags = TRUE;
										que.push(tmpRLC.m_ppObjRun[y]->RC[i]);
									}
								}
								else
									break;
							}
						}
						//如果在第一个游程起始位置和最后一个游程末端位置之间
						else if (x>=tmpRLC.m_ppObjRun[y]->RC[0].xStart
							&& x<=tmpRLC.m_ppObjRun[y]->RC[nSum-1].xEnd)
						{
							for(i=nSum-1; i>=0; i--)
							{
								if(i > 0)
								{
									while (x>tmpRLC.m_ppObjRun[y]->RC[i-1].xEnd
										&& x<tmpRLC.m_ppObjRun[y]->RC[i].xStart)
										x--;
								}

								if (x<=tmpRLC.m_ppObjRun[y]->RC[i].xEnd
									&& x>=tmpRLC.m_ppObjRun[y]->RC[i].xStart)
								{
									if (!tmpRLC.m_ppObjRun[y]->RC[i].bFlags
										&& x+1>=item.xStart)
									{
										tmpRLC.m_ppObjRun[y]->RC[i].bFlags = TRUE;
										que.push(tmpRLC.m_ppObjRun[y]->RC[i]);
									}

									nSum = i;
									break;
								}
							}

							if(nSum > 0)
							{
								for(i=nSum-1; i>=0; i--)
								{
									if (tmpRLC.m_ppObjRun[y]->RC[i].xEnd+1 >= item.xStart)
									{
										if (!tmpRLC.m_ppObjRun[y]->RC[i].bFlags)
										{
											tmpRLC.m_ppObjRun[y]->RC[i].bFlags = TRUE;
											que.push(tmpRLC.m_ppObjRun[y]->RC[i]);
										}
									}
									else
										break;
								}
							} // if(nSum > 0)
						}
					} // if(item.xEnd+1 == nWidth)
				} // if(item.y+1<nHeight && tmpRLC.m_ppObjRun[y]->nRunNum>0)
			} // while (!que.empty())
			
			//***********************将打上标记的游程编码单独存储***********************
			if (nNum==0 && bFlag)
			{
				nNum++;
				bFlag = FALSE;
				nSum = 0;

				for (y=0; y<tmpRLC.m_nObjNum; y++)
				{
					for (i=0; i<tmpRLC.m_ppObjRun[y]->nRunNum; i++)
					{
						if (tmpRLC.m_ppObjRun[y]->RC[i].bFlags && tmpRLC.m_ppObjRun[y]->RC[i].y>=0)
							nSum++;
					}
				}

				// 为第一个目标的游程分配内存
				pObjAllRLC->m_ppObjRun[0] = (PRUNLENGTHCODE_MGR)new char [sizeof(int)+sizeof(RUNLENGTHCODE)*nSum];
				pObjAllRLC->m_ppObjRun[0]->nRunNum = nSum;

				nSum = 0;
				for (y=0; y<tmpRLC.m_nObjNum; y++)
				{
					for (i=0; i<tmpRLC.m_ppObjRun[y]->nRunNum; i++)
					{
						if (tmpRLC.m_ppObjRun[y]->RC[i].bFlags && tmpRLC.m_ppObjRun[y]->RC[i].y>=0)
						{
							pObjAllRLC->m_ppObjRun[0]->RC[nSum].xStart = tmpRLC.m_ppObjRun[y]->RC[i].xStart;
							pObjAllRLC->m_ppObjRun[0]->RC[nSum].xEnd = tmpRLC.m_ppObjRun[y]->RC[i].xEnd;
							pObjAllRLC->m_ppObjRun[0]->RC[nSum].y = tmpRLC.m_ppObjRun[y]->RC[i].y;
							pObjAllRLC->m_ppObjRun[0]->RC[nSum].bFlags = FALSE;

							// 用-1标记该游程,表示该游程已经被记录过
							tmpRLC.m_ppObjRun[y]->RC[i].y = -1;

							// 统计目标中的游程数量
							nSum++;
						}
					}
				}
			}
			else if(nNum>0 && bFlag)
			{
				nNum ++;
				bFlag = FALSE;
				nSum = 0;

				for (y=0; y<tmpRLC.m_nObjNum; y++)
				{
					for (i=0; i<tmpRLC.m_ppObjRun[y]->nRunNum; i++)
					{
						if (tmpRLC.m_ppObjRun[y]->RC[i].bFlags && tmpRLC.m_ppObjRun[y]->RC[i].y>=0)
							nSum++;
					}
				}

				pObjAllRLC->m_ppObjRun[nNum-1] = (PRUNLENGTHCODE_MGR)new char [sizeof(int)+sizeof(RUNLENGTHCODE)*nSum];
				pObjAllRLC->m_ppObjRun[nNum-1]->nRunNum = nSum;
				nSum = 0;

				for (y=0; y<tmpRLC.m_nObjNum; y++)
				{
					for (i=0; i<tmpRLC.m_ppObjRun[y]->nRunNum; i++)
					{
						if (tmpRLC.m_ppObjRun[y]->RC[i].bFlags && tmpRLC.m_ppObjRun[y]->RC[i].y>=0)
						{
							pObjAllRLC->m_ppObjRun[nNum-1]->RC[nSum].xStart = tmpRLC.m_ppObjRun[y]->RC[i].xStart;
							pObjAllRLC->m_ppObjRun[nNum-1]->RC[nSum].xEnd = tmpRLC.m_ppObjRun[y]->RC[i].xEnd;
							pObjAllRLC->m_ppObjRun[nNum-1]->RC[nSum].y = tmpRLC.m_ppObjRun[y]->RC[i].y;
							pObjAllRLC->m_ppObjRun[nNum-1]->RC[nSum].bFlags = FALSE;

							tmpRLC.m_ppObjRun[y]->RC[i].y = -1;
							nSum++;
						}
					}
				}	
			}
		}
	}

	//保存对象的数目
	pObjAllRLC->m_nObjNum = nNum;
}

// 取得单个目标的游程编码（外边界按8邻域搜索）
void CImage::ObjRLC(ObjRunLenCode* pObjAllRLC, POINT ptSeed, UCHAR uObjClr/*=255*/)
{
	// 设ObjRunLenCode类对象在调用该函数时已经存在
	ASSERT(pObjAllRLC != NULL);

	// 清除内存对象
	pObjAllRLC->Clear();

	// 如果种子点颜色不是目标色,则退出
	if (((RGBQUAD)GetPixelColor(ptSeed.x, ptSeed.y)).rgbRed != uObjClr)
		return;

	int i, x, y, nSum;
	queue< RUNLENGTHCODE > que;
	RUNLENGTHCODE item;

	// 获得图像宽度和高度
	int nWidth = GetWidth();
	int nHeight = GetHeight();

	x = ptSeed.x;
	y = ptSeed.y;

	// 取得游程编码
	ObjRunLenCode tmpRLC;
	RunLengthcode(&tmpRLC, uObjClr);

	PPRUNLENGTHCODE_MGR ppObjRun = tmpRLC.GetObjAll();

	// 取得第y行的游程数目
	nSum = tmpRLC.m_ppObjRun[y]->nRunNum;

	// 将种子点所在游程压入队列
	for(i=0; i<nSum; i++)
	{
		if (x>=ppObjRun[y]->RC[i].xStart && x<=ppObjRun[y]->RC[i].xEnd)
		{
			ppObjRun[y]->RC[i].bFlags = TRUE;
			que.push(ppObjRun[y]->RC[i]);
			break;
		}
	}

	while (!que.empty())
	{
		// 从队头获得游程并弹出
		item = que.front();
		que.pop();
		y = item.y;

		//********************************************************
		//如果当前游程的上一行存在且上一行的游程数大于0
		if (item.y-1>=0 && ppObjRun[item.y-1]->nRunNum>0)
		{
			y = item.y-1;

			// 若当前游程的右边界是图像右边界
			if (item.xEnd+1 == nWidth)
			{
				for (i=ppObjRun[y]->nRunNum-1; i>=0; i--)
				{
					if (ppObjRun[y]->RC[i].xEnd+1 >= item.xStart)
					{
						if (!ppObjRun[y]->RC[i].bFlags)
						{
							ppObjRun[y]->RC[i].bFlags = TRUE;
							que.push(ppObjRun[y]->RC[i]);
						}
					}
					else
						break;
				}
			}
			else //若当前游程的右边界"不是"图像右边界
			{
				x = item.xEnd+1;
				nSum = ppObjRun[y]->nRunNum;

				// 如果在最后一个游程的右边
				if (x > ppObjRun[y]->RC[nSum-1].xEnd)
				{
					for (i=nSum-1; i>= 0; i--)
					{
						if (ppObjRun[y]->RC[i].xEnd+1 >= item.xStart)
						{
							//如果该游程未被标记过(即bFlags = FALSE),则进队列
							if (!ppObjRun[y]->RC[i].bFlags)
							{
								ppObjRun[y]->RC[i].bFlags = TRUE;
								que.push(ppObjRun[y]->RC[i]);
							}
						}
						else
							break;
					}
				}
				// 如果在第一个游程起始位置和最后一个游程末端位置之间
				else if (x>=ppObjRun[y]->RC[0].xStart && x<=ppObjRun[y]->RC[nSum-1].xEnd)
				{
					for (i=nSum-1; i>=0; i--)
					{
						if (i > 0)
						{
							while(x>ppObjRun[y]->RC[i-1].xEnd
								&& x<ppObjRun[y]->RC[i].xStart)
								x--;
						}
						
						if (x<=ppObjRun[y]->RC[i].xEnd
							&& x>=ppObjRun[y]->RC[i].xStart)
						{
							if (!ppObjRun[y]->RC[i].bFlags
								&& x+1>=item.xStart)
							{
								ppObjRun[y]->RC[i].bFlags = TRUE;
								que.push(ppObjRun[y]->RC[i]);
							}
							nSum = i;
							break;
						}
					}

					if (nSum > 0)
					{
						for (i=nSum-1; i>=0; i--)
						{
							if(ppObjRun[y]->RC[i].xEnd+1 >= item.xStart)
							{
								if(!ppObjRun[y]->RC[i].bFlags)
								{
									ppObjRun[y]->RC[i].bFlags = TRUE;
									que.push(ppObjRun[y]->RC[i]);
								}
							}
							else
								break;
						}
					}//if(nSum > 0)
				}
			}//if(item.xEnd+1 == lWidth)
		}//if(item.y-1 >= 0 && pRCMgr->ppObjRun[y]->nRunNum > 0)

		//********************************************************
		//如果当前游程的下一行存在且下一行的游程数大于0
		if (item.y+1<nHeight && ppObjRun[item.y+1]->nRunNum>0)
		{
			y = item.y+1;

			// 若当前游程的右边界是图像右边界
			if (item.xEnd+1 == nWidth)
			{
				for (i=ppObjRun[y]->nRunNum-1; i>=0; i--)
				{
					if (ppObjRun[y]->RC[i].xEnd+1 >= item.xStart)
					{
						if (!ppObjRun[y]->RC[i].bFlags)
						{
							ppObjRun[y]->RC[i].bFlags = TRUE;
							que.push(ppObjRun[y]->RC[i]);
						}
					}
					else
						break;
				}
			}
			else //若当前游程的右边界"不是"图像右边界
			{
				x = item.xEnd+1;
				nSum = ppObjRun[y]->nRunNum;

				// 如果在最后一个游程的右边
				if (x > ppObjRun[y]->RC[nSum-1].xEnd)
				{
					for (i=nSum-1; i>=0; i--)
					{
						if (ppObjRun[y]->RC[i].xEnd+1 >= item.xStart)
						{
							// 如果该游程未被标记过(即y!=-1),则进队列
							if(!ppObjRun[y]->RC[i].bFlags)
							{
								ppObjRun[y]->RC[i].bFlags = TRUE;
								que.push(ppObjRun[y]->RC[i]);
							}
						}
						else
							break;
					}
				}
				//如果在第一个游程起始位置和最后一个游程末端位置之间
				else if(x>=ppObjRun[y]->RC[0].xStart
					&& x<=ppObjRun[y]->RC[nSum-1].xEnd)
				{
					for(i=nSum-1; i>=0; i--)
					{
						if(i > 0)
						{
							while(x>ppObjRun[y]->RC[i-1].xEnd
							   && x<ppObjRun[y]->RC[i].xStart)
							 x--;
						}
						
						if (x<=ppObjRun[y]->RC[i].xEnd
							&& x>=ppObjRun[y]->RC[i].xStart)
						{
							if (!ppObjRun[y]->RC[i].bFlags
								&& x+1>=item.xStart)
							{
								ppObjRun[y]->RC[i].bFlags = TRUE;
								que.push(ppObjRun[y]->RC[i]);
							}
							nSum = i;
							break;
						}
					}

					if (nSum > 0)
					{
						for (i=nSum-1; i>=0; i--)
						{
							if (ppObjRun[y]->RC[i].xEnd+1 >= item.xStart)
							{
								if (!ppObjRun[y]->RC[i].bFlags)
								{
									ppObjRun[y]->RC[i].bFlags = TRUE;
									que.push(ppObjRun[y]->RC[i]);
								}
							}
							else
								break;
						}
					}//if(nSum > 0)
				}
			}//if(item.xEnd+1 == lWidth)
		}//if(item.y+1 < lHeight && pRCMgr->ppObjRun[y]->nRunNum > 0)
	}//while(!queue.IsEmpty())

	//***********************将打上标记的游程编码单独存储***********************
	pObjAllRLC->m_nObjNum = 1;
	nSum = 0;

	// 统计被标记的游程数目
	for (y=0; y<tmpRLC.m_nObjNum; y++)
	{
		for (i=0; i<tmpRLC.m_ppObjRun[y]->nRunNum; i++)
		{
			if (tmpRLC.m_ppObjRun[y]->RC[i].bFlags)
				nSum++;
		}
	}

	pObjAllRLC->m_ppObjRun[0] = (PRUNLENGTHCODE_MGR)new char[sizeof(int)+sizeof(RUNLENGTHCODE)*nSum];
	pObjAllRLC->m_ppObjRun[0]->nRunNum = nSum;

	// 存储种子点所在目标的所有游程
	nSum = 0;
	for (y=0; y<tmpRLC.m_nObjNum; y++)
	{
		for (i=0; i<tmpRLC.m_ppObjRun[y]->nRunNum; i++)
		{
			if (tmpRLC.m_ppObjRun[y]->RC[i].bFlags)
			{
				pObjAllRLC->m_ppObjRun[0]->RC[nSum].xStart = tmpRLC.m_ppObjRun[y]->RC[i].xStart;
				pObjAllRLC->m_ppObjRun[0]->RC[nSum].xEnd = tmpRLC.m_ppObjRun[y]->RC[i].xEnd;
				pObjAllRLC->m_ppObjRun[0]->RC[nSum].y = tmpRLC.m_ppObjRun[y]->RC[i].y;
				pObjAllRLC->m_ppObjRun[0]->RC[nSum].bFlags = FALSE;

				nSum++;
			}
		}
	}
}

// 取得所有目标的边界链码（外边界按8邻域搜索）
void CImage::ObjAllEdgeLst(ObjEdgeList* pObjEdgeLst, UCHAR uObjClr/*=255*/)
{
	ASSERT(pObjEdgeLst != NULL);
	pObjEdgeLst->Clear();

	ObjRunLenCode tmpAllRLC;
	ObjAllRLC(&tmpAllRLC, uObjClr);

	int i;
	POINT point;

	// 直接把单个物体的边沿对象指针指向的地址赋给所有物体边沿对象的某个指针
	pObjEdgeLst->m_nObjNum = tmpAllRLC.m_nObjNum;
	for (i=0; i<pObjEdgeLst->m_nObjNum; i++)
	{
		point.x = tmpAllRLC.m_ppObjRun[i]->RC[0].xStart;
		point.y = tmpAllRLC.m_ppObjRun[i]->RC[0].y;

		ObjEdgeList tmpEdgeLst;
		ObjEdgeLst(&tmpEdgeLst, point, uObjClr);

		pObjEdgeLst->m_ppEdgeObj[i] = new EDGE_OBJECT [pObjEdgeLst->m_nObjNum];
		pObjEdgeLst->m_ppEdgeObj[i]->nEdgeNum = tmpEdgeLst.m_ppEdgeObj[0]->nEdgeNum;
		pObjEdgeLst->m_ppEdgeObj[i]->ppEdgeLst = tmpEdgeLst.m_ppEdgeObj[0]->ppEdgeLst;
		tmpEdgeLst.m_ppEdgeObj[0]->ppEdgeLst = NULL;
	}
}

// 取得单个目标的边界链码（外边界按8邻域搜索）
void CImage::ObjEdgeLst(ObjEdgeList* pObjEdgeLst, POINT ptSeed, UCHAR uObjClr/*=255*/)
{
	ASSERT(pObjEdgeLst != NULL);
	pObjEdgeLst->Clear();

#define EDGE_MARK -1	// 包围边界的外面的标记

	int i, j, x, y, nLen;
	vector< POINT > vec;

	CPoint point, nextPoint, point1;
	int nOri, nNextOri, nOri1;

	int nWidth = GetWidth();
	int nHeight = GetHeight();

	// 如果种子点颜色不是目标色,则退出
	if (((RGBQUAD)GetPixelColor(ptSeed.x, ptSeed.y)).rgbRed != uObjClr)
		return;

	ObjRunLenCode tmpRLC;
	ObjRLC(&tmpRLC, ptSeed, uObjClr);

	// 得到目标的外接矩形
	CRect rect(INT_MAX, INT_MAX, INT_MIN, INT_MIN);
 	PRUNLENGTHCODE_MGR pRLCMgr = tmpRLC.m_ppObjRun[0];
	for (i=0; i<pRLCMgr->nRunNum; i++)
	{
		rect.left = (rect.left<pRLCMgr->RC[i].xStart) ? rect.left : pRLCMgr->RC[i].xStart;
		rect.right = (rect.right>pRLCMgr->RC[i].xEnd) ? rect.right : pRLCMgr->RC[i].xEnd;
		rect.top = (rect.top<pRLCMgr->RC[i].y) ? rect.top : pRLCMgr->RC[i].y;
		rect.bottom = (rect.bottom>pRLCMgr->RC[i].y) ? rect.bottom : pRLCMgr->RC[i].y;
	}
	rect.right++;
	rect.bottom++;

	// 临时新建矩阵,将原图像的边界扩展一个象素,先全部置为背景色
	int nMWidth = rect.Width()+2;
	int nMHeight = rect.Height()+2;
	short* pMatrix = new short [nMWidth*nMHeight];
	if (pMatrix == NULL)
		return;
	memset(pMatrix, 255-uObjClr, sizeof(UCHAR)*nMWidth*nMHeight);

	// 将目标拷贝至新建临时矩阵中
	int nOffX = rect.left;
	int nOffY = rect.top;
	for (i=0; i<pRLCMgr->nRunNum; i++)
	{
		for (j=pRLCMgr->RC[i].xStart; j<=pRLCMgr->RC[i].xEnd; j++)
			pMatrix[(pRLCMgr->RC[i].y-nOffY+1)*nMWidth+(j-nOffX+1)] = uObjClr;
	}

	//****************************查找对象的外边界链码****************************
	// 扩展边界后的起始边缘象素点为目标的第一个边界点
	x = pRLCMgr->RC[0].xStart;
	y = pRLCMgr->RC[0].y;
	point.x = x-nOffX+1;
	point.y = y-nOffY+1;
	vec.push_back(point);

	// 定义8邻域的平移坐标
	CPoint move8[8];
	move8[0].x = 1, move8[0].y = 0;	// 第1点
	move8[1].x = 1, move8[1].y =-1;	// 第2点
	move8[2].x = 0, move8[2].y =-1;	// 第3点
	move8[3].x =-1, move8[3].y =-1;	// 第4点
	move8[4].x =-1, move8[4].y = 0;	// 第5点
	move8[5].x =-1, move8[5].y = 1;	// 第6点
	move8[6].x = 0, move8[6].y = 1;	// 第7点
	move8[7].x = 1, move8[7].y = 1;	// 第8点

	// 定义4邻域的平移坐标
	CPoint move4[4];
	move4[0].x = 1, move4[0].y = 0;	// 第1点
	move4[1].x = 0, move4[1].y =-1;	// 第2点
	move4[2].x =-1, move4[2].y = 0;	// 第3点
	move4[3].x = 0, move4[3].y = 1;	// 第4点

	// 当查找到当前点时，查找当前点的8邻域和4邻域点的起始方向按如下方式进行
	int nPrevOri8[] = {7,7,1,1,3,3,5,5};
	int nPrevOri4[] = {3,0,1,2};

	//*******************查找外边界点,同时在外边界以外作上标记********************
	// 从第4点开始查找点point的下一个8邻域的颜色为uObjClr的点
	for (i=5; i<5+8; i++)
	{
		nextPoint = point+move8[i%8];
		if (pMatrix[nextPoint.y*nMWidth+nextPoint.x] == uObjClr)
		{
			nNextOri = i%8;
			break;
		}

		// 将查找过的点标记为EDGE_MARK
		pMatrix[nextPoint.y*nMWidth+nextPoint.x] = EDGE_MARK;
	}

	// 如果只有一个颜色为uObjClr的目标点,则直接存储该点
	if (i == 13)
	{
		nLen = 1;
	}
	else
	{
		// 记下第一个点的坐标和下一个点相对该点的8邻域方向
		point1 = nextPoint;
		nOri1 = nNextOri;

		while (TRUE)
		{
			point = nextPoint;
			nOri = nPrevOri8[nNextOri];
			vec.push_back(point);

			// 从第nOri方向开始查找8邻域点
			for (i=nOri; i<nOri+8; i++)
			{
				nextPoint = point+move8[i%8];
				if (pMatrix[nextPoint.y*nMWidth+nextPoint.x] == uObjClr)
				{
					nNextOri = i%8;
					break;
				}

				// 将查找过的颜色非uObjClr的点标记为EDGE_MARK
				pMatrix[nextPoint.y*nMWidth+nextPoint.x] = EDGE_MARK;
			}

			// 再次遇到第二个点时停止
			if (nextPoint==point1 && nNextOri==nOri1)
			{
				// 最后一个点即第一个点,将其弹出
				vec.pop_back();
				break;
			}
		}

		nLen = vec.size();
	}

	// 分配内存空间存储外边界
	pObjEdgeLst->m_nObjNum = 1;
	pObjEdgeLst->m_ppEdgeObj[0] = new EDGE_OBJECT;
	pObjEdgeLst->m_ppEdgeObj[0]->nEdgeMaxNum = OBJ_MAX_NUM;
	pObjEdgeLst->m_ppEdgeObj[0]->nEdgeNum = 1;
	pObjEdgeLst->m_ppEdgeObj[0]->ppEdgeLst = new PEDGE_LIST [OBJ_EDGE_MAX_NUM];
	pObjEdgeLst->m_ppEdgeObj[0]->ppEdgeLst[0] = (PEDGE_LIST)new char
		[sizeof(int)+sizeof(POINT)*nLen];
	pObjEdgeLst->m_ppEdgeObj[0]->ppEdgeLst[0]->nEdgeLen = nLen;

	for (i=0; i<nLen; i++)
	{
		point = vec.back();
		vec.pop_back();
		pObjEdgeLst->m_ppEdgeObj[0]->ppEdgeLst[0]->edge[i].x = point.x-1+nOffX;
		pObjEdgeLst->m_ppEdgeObj[0]->ppEdgeLst[0]->edge[i].y = point.y-1+nOffY;
	}
	//**************************结束对象的外边界链码的查找**************************

	//*****************************查找对象的各个内边界*****************************

	int nNum = 0;
	for (i=0; i<pRLCMgr->nRunNum; i++)
	{
		// 在ppchTemp中,x,y坐标值都要加一
		x = pRLCMgr->RC[i].xEnd-nOffX+1;
		y = pRLCMgr->RC[i].y-nOffY+1;

		// 如果(x+1,y)的值为背景色
		if ((pMatrix[y*nMWidth+x+1]!=uObjClr)
			&& (pMatrix[y*nMWidth+x+1]!=EDGE_MARK))
		{
			nNum++;
			point.x = x+1;
			point.y = y;
			vec.push_back(point);

			// 查找4邻域内背景色点
			for (j=3; j<3+4; j++)
			{
				nextPoint = point+move4[j%4];
				if (pMatrix[nextPoint.y*nMWidth+nextPoint.x] != uObjClr)
				{
					nNextOri = j%4;
					break;
				}
			}

			// 如果只有一个背景色点
			if (j == 3+4)
			{
				nLen = 1;
			}
			else
			{
				point1 = nextPoint;
				nOri1 = nNextOri;

				while (TRUE)
				{
					point = nextPoint;
					nOri = nPrevOri4[nNextOri];
					vec.push_back(point);

					// 查找4邻域内背景色点
					for (j=nOri; j<nOri+4; j++)
					{
						nextPoint = point+move4[j%4];
						if (pMatrix[nextPoint.y*nMWidth+nextPoint.x] != uObjClr)
						{
							nNextOri = j%4;
							break;
						}
					}
					
					//再次遇到第二个点时停止
					if(nextPoint == point1 && nNextOri == nOri1 )
					{
						// 最后一个点即第一个点,将其弹出
						vec.pop_back();
						break;
					}
				}

				nLen = vec.size();
			}
			
			pObjEdgeLst->m_ppEdgeObj[0]->ppEdgeLst[nNum] = (PEDGE_LIST)new char
				[sizeof(int)+sizeof(POINT)*nLen];
			pObjEdgeLst->m_ppEdgeObj[0]->ppEdgeLst[nNum]->nEdgeLen = nLen;

			for(j=0; j<nLen; j++)
			{
				point = vec.back();
				vec.pop_back();

				// 给内边界点作上标记
				pMatrix[point.y*nMWidth+point.x] = EDGE_MARK;
				pObjEdgeLst->m_ppEdgeObj[0]->ppEdgeLst[nNum]->edge[j].x = point.x-1+nOffX;
				pObjEdgeLst->m_ppEdgeObj[0]->ppEdgeLst[nNum]->edge[j].y = point.y-1+nOffY;
			}
		} // if (pMatrix[y*(rect.Width()+2)+x+1)
	} // for (i=0; i<pRLCMgr->nRunNum; i++)
	//***************************结束对象各个内边界的查找***************************

	pObjEdgeLst->m_ppEdgeObj[0]->nEdgeNum = nNum+1;

	delete [] pMatrix;
}

// 边界拟合
/* ==================================================================
 * 函数名称：  ObjEdgeMerge()
 * 作者：      [Leng]
 * 创建于:     [2008-12-4 22:34:39]
 * 参数列表： 
               [ObjEdgeList* pSrcEdgeLst] - [In]	源边界链码
               [ ObjEdgeList* pDstEdgeLst] - [In]	处理结果边界链码
               [ float fDist] - [In]	边界距离阈值
注释 :	
				
==================================================================*/
void CImage::ObjEdgeMerge(ObjEdgeList* pSrcEdgeLst, float fDist, ObjEdgeList* pDstEdgeLst)
{
	int i, j, k, m, n;
	POINT point, start, end;

	// 边界拟合结果中的目标数目与原边界链码中的目标数目相等
//	pDstEdgeLst->m_nObjMaxNum = pSrcEdgeLst->m_nObjNum;
	pDstEdgeLst->m_nObjNum = pSrcEdgeLst->m_nObjNum;
//	pDstEdgeLst->m_ppEdgeObj = new PEDGE_OBJECT [pDstEdgeLst->m_nObjNum];

	// 边界拟合结果中各目标的边界数目与原边界链码中的对应数目相等
	PPEDGE_OBJECT ppSrcObj = pSrcEdgeLst->m_ppEdgeObj;
	PPEDGE_OBJECT ppDstObj = pDstEdgeLst->m_ppEdgeObj;
// 	PPEDGE_OBJECT ppSrcObj = pSrcEdgeLst->GetObjAllEdgeLst();
// 	PPEDGE_OBJECT ppDstObj = pDstEdgeLst->GetObjAllEdgeLst();
	for (i=0; i<pDstEdgeLst->m_nObjNum; i++)
	{
		ppDstObj[i] = new EDGE_OBJECT;
		ppDstObj[i]->nEdgeMaxNum = ppSrcObj[i]->nEdgeNum;
		ppDstObj[i]->nEdgeNum = ppSrcObj[i]->nEdgeNum;
		ppDstObj[i]->ppEdgeLst = new PEDGE_LIST [ppSrcObj[i]->nEdgeNum];
	}	

	// 定义变量用于存储边界拟合结果并进行边界拟合
	vector<POINT> vEdgePts;
	for (i=0; i<pSrcEdgeLst->m_nObjNum; i++)
	{
		for (j=0; j<ppSrcObj[i]->nEdgeNum; j++)
		{
			k = 0;

			// 存储各条边界起始点
			POINT pt = ppSrcObj[i]->ppEdgeLst[j]->edge[0];
			vEdgePts.push_back(pt);

			PPEDGE_LIST ppSrcEdge = ppSrcObj[i]->ppEdgeLst;
			PPEDGE_LIST ppDstEdge = ppDstObj[i]->ppEdgeLst;
			while (k < ppSrcEdge[j]->nEdgeLen)
			{
				start = ppSrcEdge[j]->edge[k];
				for (m=k+2; ; m++)
				{
					if (m >= ppSrcEdge[j]->nEdgeLen)
						break;

					end = ppSrcEdge[j]->edge[m];
					for (n=k+1; n<m; n++)
					{
						point = ppSrcEdge[j]->edge[n];
						// 若两端点重合,则继续
						if (start.x==end.x && start.y==end.y)
							continue;
						else if (point_line_dist(point, start, end) >= fDist)
						{
							vEdgePts.push_back(ppSrcEdge[j]->edge[m-1]);
							k = m-1;
							break;
						}
					}
					if (point_line_dist(point, start, end) >= fDist)
						break;;
				}
				// 判定边界链码末端链码的处理情况
				if (m >= ppSrcEdge[j]->nEdgeLen)
				{
					// 当满足下面条件时,m = j+2,第j点为最后一点,否则此时第j+1点(即m-1点)为最后一点
					if (m-1 >= ppSrcEdge[j]->nEdgeLen)
						break;
					else
					{
						vEdgePts.push_back(ppSrcEdge[j]->edge[m-1]);
						break;
					}
				}
			}

			ppDstEdge[j] = (PEDGE_LIST)new char [1*sizeof(int)+vEdgePts.size()*sizeof(POINT)];
			ppDstEdge[j]->nEdgeLen = vEdgePts.size();
			for (k=0; k<vEdgePts.size(); k++)
			{
				ppDstEdge[j]->edge[k] = vEdgePts[k];
			}
		}
	}
}
// 取得所有游程编码(08.06.19)
void CImage::RunLengthcode(ObjRunLenCode* pObjAllRLC, UCHAR uObjClr/*=255*/)
{
	// 设ObjRunLenCode类对象在调用该函数时已经存在
	ASSERT(pObjAllRLC != NULL);

	// 清除内存对象
	pObjAllRLC->Clear();

	// 图像宽度及高度
	int nWidth, nHeight;
	nWidth = (int)GetWidth();
	nHeight = (int)GetHeight();

	// 此处设每一行为一个游程目标，故目标数取nHeight
	pObjAllRLC->m_nObjNum = nHeight;

	// 游程编码
	int nNum, nStart;
	for (int i=0; i<nHeight; i++)
	{
		nNum = 0;

		// 设每一行的游程编码最多有(nWidth/2+1)个
		pObjAllRLC->m_ppObjRun[i] = (PRUNLENGTHCODE_MGR)new UCHAR [sizeof(int)+(nWidth/2+1)*sizeof(RUNLENGTHCODE)];

		for (int j=0; j<nWidth; j++)
		{
			RGBQUAD RGBj = (RGBQUAD)GetPixelColor(j, i);
			RGBQUAD RGBj1 = (RGBQUAD)GetPixelColor(j-1, i);

			if (j == 0)		//如果是第i行的第一个元素
			{
				if(RGBj.rgbRed == uObjClr)
					nStart = 0;
			}
			else if(j > 0 && j+1 < nWidth)	//如果是第i行的第一个和最后一个元素之间的元素
			{
				if(RGBj1.rgbRed == uObjClr && RGBj.rgbRed != uObjClr)
				{
					pObjAllRLC->m_ppObjRun[i]->RC[nNum].bFlags = FALSE;
					pObjAllRLC->m_ppObjRun[i]->RC[nNum].y = i;
					pObjAllRLC->m_ppObjRun[i]->RC[nNum].xStart = nStart;
					pObjAllRLC->m_ppObjRun[i]->RC[nNum].xEnd = j-1;

					nNum++;
				}
				if(RGBj1.rgbRed != uObjClr && RGBj.rgbRed == uObjClr)
				{
					nStart = j;
				}
			}
			else	//如果是第i行的最后一个像素
			{
				if(RGBj1.rgbRed == uObjClr && RGBj.rgbRed != uObjClr)
				{
					pObjAllRLC->m_ppObjRun[i]->RC[nNum].bFlags = FALSE;
					pObjAllRLC->m_ppObjRun[i]->RC[nNum].y = i;
					pObjAllRLC->m_ppObjRun[i]->RC[nNum].xStart = nStart;
					pObjAllRLC->m_ppObjRun[i]->RC[nNum].xEnd = j-1;

					nNum++;
				}
				if(RGBj1.rgbRed != uObjClr && RGBj.rgbRed == uObjClr)
				{
					pObjAllRLC->m_ppObjRun[i]->RC[nNum].bFlags = FALSE;
					pObjAllRLC->m_ppObjRun[i]->RC[nNum].y = i;
					pObjAllRLC->m_ppObjRun[i]->RC[nNum].xStart = j;
					pObjAllRLC->m_ppObjRun[i]->RC[nNum].xEnd = j;

					nNum++;
				}
				if(RGBj1.rgbRed == uObjClr && RGBj.rgbRed == uObjClr)
				{
					pObjAllRLC->m_ppObjRun[i]->RC[nNum].bFlags = FALSE;
					pObjAllRLC->m_ppObjRun[i]->RC[nNum].y = i;
					pObjAllRLC->m_ppObjRun[i]->RC[nNum].xStart = nStart;
					pObjAllRLC->m_ppObjRun[i]->RC[nNum].xEnd = j;

					nNum++;
				}
			}
			pObjAllRLC->m_ppObjRun[i]->nRunNum = nNum;
		}
	}
}

// 根据生长阈值对种子点进行生长
/* ==================================================================
 * 函数名称：  CImage::ObjRgnGrow()
 * 作者：      [Leng]
 * 创建于:     [2008-6-26 14:20:35]
 * 参数列表： 
               [POINT ptSeed] - [In] 区域生长种子点
               [ UCHAR uGrwThrshld] - [In]　区域生长阈值
               [ ObjRunLenCode* pRgnGrow] - [In]　存储区域生长结果游程编码
注释 :	
				
==================================================================*/
void CImage::ObjRgnGrow(POINT ptSeed, UCHAR uGrwThrshld, ObjRunLenCode* pRgnGrow)
{
	ASSERT(pRgnGrow != NULL);

	int i, j;

	// 设置8邻域相对坐标
	POINT move[8] = {
		{ 1, 0 },
		{ 1, 1 },
		{ 0, 1 },
		{-1, 1 },
		{-1, 0 },
		{-1,-1 },
		{ 0,-1 }};

	// 设置标记矩阵,用于标记该像素点是否被查找过
	CImage* pImage = new CImage;
	pImage->Copy(*this);
	for (i=0; i<pImage->GetHeight(); i++)
	{
		for (j=0;j<pImage->GetWidth(); j++)
		{
			pImage->SetPixelColor(j, i, RGB(0,0,0));
		}
	}

	RGBQUAD rgb, rgb1;
	UCHAR clr, clr1;
	CPoint pt, pt1;

	rgb = GetPixelColor(ptSeed.x, ptSeed.y);
	clr = (rgb.rgbRed+rgb.rgbGreen+rgb.rgbBlue)/3.0+0.5;

	queue< POINT > que;
	que.push(ptSeed);
	pImage->SetPixelColor(ptSeed.x, ptSeed.y, RGB(255,255,255));

	while (!que.empty())
	{
		// 弹出队尾元素
		pt = que.front();
		que.pop();

		for (i=0; i<8; i++)
		{
			pt1 = pt+move[i];
			if (IsInside(pt1.x, pt1.y))
			{
				rgb1 = GetPixelColor(pt1.x, pt1.y);
				clr1 = (rgb1.rgbRed+rgb1.rgbGreen+rgb1.rgbBlue)/3.0+0.5;

				// 将满足条件的点压入堆栈并打上标记
				if (abs(clr-clr1)<=uGrwThrshld && pImage->GetPixelColor(pt1.x, pt1.y).rgbRed==0)
				{
					que.push(pt1);
					pImage->SetPixelColor(pt1.x, pt1.y, RGB(255,255,255));
				}
			}
		}
	}

	// 获得区域生长目标的游程
	pImage->ObjRLC(pRgnGrow, ptSeed);

	delete pImage;
}

// 删除目标内部背景色
//void CImage::ObjDelInner(ObjEdgeList* pObjEdgeLst)
//{
//}

// 设置目标颜色
void CImage::SetObjColor(ObjRunLenCode* pObjAllRLC, COLORREF rgb)
{
	int i,j,k;
	int nObjNum = pObjAllRLC->GetObjNum();
	PPRUNLENGTHCODE_MGR ppRLCMgr = pObjAllRLC->GetObjAll();

	POINT pt;
	for (i=0; i<nObjNum; i++)
	{
		int nRCNum = ppRLCMgr[i]->nRunNum;
		for (j=0; j<nRCNum; j++)
		{
			int xStart, xEnd, y;
			xStart = ppRLCMgr[i]->RC[j].xStart;
			xEnd = ppRLCMgr[i]->RC[j].xEnd;
			y = ppRLCMgr[i]->RC[j].y;
			for (k=xStart; k<=xEnd; k++)
			{
				pt.x = k;
				pt.y = y;
				SetPixelColor(pt.x, pt.y, rgb);
			}
		}
	}
}

void CImage::GreyNonLinearTransfer(float a, float b, float c)
{
	int i,j;
	BYTE grey;

	for (i=0; i<GetWidth(); i++)
	{
		for (j=0; j<GetHeight(); j++)
		{
			grey = GetPixelGray(i,j);
			grey = a+log(grey+1)/(b*log(c));
			grey = (grey<255)?((grey>0)?grey:0):255;
			SetPixelColor(i,j,RGB(grey,grey,grey));
		}
	}
}

// 动态范围压缩
void CImage::DynCompress(float C, float s)
{
	int i,j;
	BYTE grey;
	RGBQUAD rgb;

	for (i=0; i<GetWidth(); i++)
	{
		for (j=0; j<GetHeight(); j++)
		{
			rgb = GetPixelColor(i,j);
			grey = 105.9*log(1+fabs(rgb.rgbBlue));
			rgb.rgbBlue = (grey<255)?((grey>0)?grey:0):255;

			grey = 105.9*log(1+fabs(rgb.rgbGreen));
			rgb.rgbGreen = (grey<255)?((grey>0)?grey:0):255;

			grey = 105.9*log(1+fabs(rgb.rgbRed));
			rgb.rgbRed = (grey<255)?((grey>0)?grey:0):255;

			SetPixelColor(i,j,rgb);
		}
	}
}

// 梯度倒数加权平滑
void CImage::WeightGradientCountDownSmooth(int length)
{
	int i,j,m,n,idx;
	BYTE grey,grey1;
	double dbSum = 0.0;
	float* pKenel = new float [length*length];
	float* pCntDwn = new float [length*length];

	for (i=length/2; i<GetWidth()-length/2; i++)
	{
		for (j=length/2; j<GetHeight()-length/2; j++)
		{
			grey = GetPixelGray(i,j);
			for (m=-length/2; m<=length/2; m++)
			{
				for (n=-length/2; n<=+length/2; n++)
				{
					if (m==0 && n==0)
						continue;

					grey1 = GetPixelGray(i+m,j+n);
					idx = (n+length/2)*length+(m+length/2);
					if (grey == grey1)
					{
						pCntDwn[idx] = 0;
						continue;
					}

					pCntDwn[idx] = 1/abs(grey-grey1);
					dbSum += pCntDwn[idx];
				}
			}

			// 求权值
			for (m=-length/2; m<=length/2; m++)
			{
				for (n=-length/2; n<=+length/2; n++)
				{
					idx = (n+length/2)*length+(m+length/2);
					if (m==0 && n==0)
					{
						pKenel[idx] = 0.5;
						continue;
					}

					pKenel[idx] = pCntDwn[idx]/(2*dbSum);
				}
			}

			// 求输出结果
			dbSum = 0.0;
			for (m=-length/2; m<=length/2; m++)
			{
				for (n=-length/2; n<=+length/2; n++)
				{
					grey1 = GetPixelGray(i+m,j+n);
					idx = (n+length/2)*length+(m+length/2);
					dbSum += grey1*pKenel[idx];
				}
			}

			// 设置输出
			dbSum = (dbSum<255)?((dbSum>0)?dbSum:0):255;
			SetPixelColor(i,j,RGB(dbSum,dbSum,dbSum));
		}
	}
}


#if CXIMAGE_SUPPORT_DSP
////////////////////////////////////////////////////////////////////////////////
long CImage::Histogram(long* red, long* green, long* blue, long* gray, long colorspace)
{
	return CxImage::Histogram(red,green,blue,gray,colorspace);
}
////////////////////////////////////////////////////////////////////////////////
// HistogramStretch function by <dave> : dave(at)posortho(dot)com and <nipper>
bool CImage::HistogramStretch(long method)
{
	return CxImage::HistogramStretch(method);
}
////////////////////////////////////////////////////////////////////////////////
// HistogramEqualize function by <dave> : dave(at)posortho(dot)com
bool CImage::HistogramEqualize()
{
	return CxImage::HistogramEqualize();
}
////////////////////////////////////////////////////////////////////////////////
// HistogramNormalize function by <dave> : dave(at)posortho(dot)com
bool CImage::HistogramNormalize()
{
	return CxImage::HistogramNormalize();
}
////////////////////////////////////////////////////////////////////////////////
// HistogramLog function by <dave> : dave(at)posortho(dot)com
bool CImage::HistogramLog()
{
	return CxImage::HistogramLog();
}

////////////////////////////////////////////////////////////////////////////////
// HistogramRoot function by <dave> : dave(at)posortho(dot)com
bool CImage::HistogramRoot()
{
	return CxImage::HistogramRoot();
}
////////////////////////////////////////////////////////////////////////////////
#endif
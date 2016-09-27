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

// �ɲ��������õ���ѷָ���ֵ(08.06.19)
// �޸�(08.08.31)
int CImage::GetBestThreshold()
{
	CImage image(*this);
	if (!image.GetDIB()) return false;
	if (image.GetBpp() == 1) return false;

	image.GrayScale();

	// ���صı�����ȫ�����صĻҶȺ�
	int nThre, lSum;
	nThre = lSum = 0;

	// ���Ҷȼ��ĻҶ���
	long lPixNum[256];
	memset(lPixNum, 0, 256 * sizeof(long));

	// ͳ��ͼ���и��Ҷȼ����صĸ���
	for (long i=0; i<head.biHeight; i++)
	{
		for (long j=0; j<head.biWidth; j++)
		{
			BYTE color = image.GetPixelGray(j,i);
			lPixNum[color] ++;
			lSum += color;
		}
	}

	// ��ָ���ֵΪt,��{0,1,...,t-1}��{t,t+1,...,255}��������,
	// ���������ƽ��ֵ�ķ���(��䷽��)�����ķ���(���ڷ���)֮��
	// ��������ȳ�Ϊ���ʱ����ֵt��Ϊ����
	// ��1����2��������
	long lNum1 = 0;
	long lNum2 = 0;

	// ��1����2��ƽ���Ҷ�
	double dbMean1, dbMean2;

	// ��1����2�ĻҶȺ�
	long lSum1, lSum2;

	// ��䷽��
	double dbR2b;

	// ���ʼ�ָ���ֵΪ1,��<1��Ϊ1��,>=1��Ϊ1��
	// ��1���������
	lNum1 = lPixNum[0];

	// ��2���������
	for (i = 1; i < 256; i ++)
		lNum2 += lPixNum[i];

	// ��1����2�����س�ʼ�ҶȺ�
	lSum1 = 0;
	lSum2 = lSum - lSum1;

	// �����䷽��
	double dbMaxR2b = 0.0;

	// 
	for (i = 1; i < 256; i ++)
	{
		// ��1���ֵ
		if (0 == lNum1)
			dbMean1 = 0;
		else
			dbMean1 = lSum1 * 1.0 / lNum1;

		// ��2���ֵ
		if (0 == lNum2)
			dbMean2 = 0;
		else
			dbMean2 = lSum2 * 1.0 / lNum2;

		// ��䷽��
		dbR2b = (lNum1 * lNum2 * (dbMean1-dbMean2) * (dbMean1-dbMean2))
						/ (double(lNum1 + lNum2) * double(lNum1 + lNum2));

		// �������䷽��ֵ
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

	// �����������
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
// Canny����
void CImage::EdgeCanny()
{
	int i,j;

	// ͼ���ȡ��߶�
	int nWidth = (int)GetWidth();
	int nHeight = (int)GetHeight();

	// �����ڴ棬�洢ͼ������
	LPBYTE lpSrcDIBBits = new BYTE [nWidth * nHeight];

	// canny���Ӽ����Ľ��
  LPBYTE lpDstDIBBits = new BYTE [nWidth * nHeight];

	// ������
	for (i=0; i < nHeight; i ++)
	{
		for (j=0; j < nWidth; j ++)
		{
			lpSrcDIBBits[nWidth * i + j] = GetPixelGray(j, i);
		}
	}
	
	// ����canny�������б߽���ȡ
	Canny(lpSrcDIBBits, nWidth, nHeight, 0.4, 0.4, 0.79, lpDstDIBBits);
	
	// д����
	for (i = 0; i < nHeight; i ++)
	{
		for (j = 0; j < nWidth; j ++)
		{
			UCHAR uGrey = lpDstDIBBits[nWidth * i + j];
			SetPixelColor(j, i, RGB(uGrey, uGrey, uGrey));
		}
	}

	// �ͷ��ڴ�
	delete [] lpSrcDIBBits;
	delete [] lpDstDIBBits;

}

// �Զ�ֵͼ�����ϸ��
BOOL CImage::MorphThin()
{
	//�������
	unsigned char *pBits;
	int lWidth,lHeight;
	
	//���ͼ��ָ��
	pBits = GetBits();

	lWidth = GetWidth();
	lHeight = GetHeight();

	/////////////////////////////////////////////////////////
	RGBQUAD rgbSrc;
	
	//����
	BOOL bModified;

	//ѭ������
	long i;
	long j;
	int  n;
	int  m;

	//�ĸ�����
	BOOL bCondition1;
	BOOL bCondition2;
	BOOL bCondition3;
	BOOL bCondition4;

	//������
	unsigned char nCount;
	
	//����ֵ
	unsigned char pixel;

	//5��5������������ֵ
	unsigned char neighbour[5][5];

	// ��ʱ�����ڴ棬��ʼ���·�����ڴ棬�趨��ʼֵΪ255
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

				//����ʹ��5��5�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ�������Χ�ļ��кͼ�������

				// ָ��Դͼ������j�У���i�����ص�ָ��	
				rgbSrc = GetPixelColor(i, j);

				//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
				pixel = (unsigned char)rgbSrc.rgbBlue;

				//���Դͼ���е�ǰ��Ϊ��ɫ��������
				if(pixel == 255)
					continue;
				//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
				else if(pixel != 0)
					continue;

				//��õ�ǰ�����ڵ�5��5����������ֵ����ɫ��0������ɫ��1����
				for (m = 0;m < 5;m++ )
				{
					for (n = 0;n < 5;n++)
					{
						rgbSrc = GetPixelColor(n-2,(4-m)-2);
						pixel = (unsigned char)rgbSrc.rgbBlue;
						neighbour[m][n] =(255 - pixel) / 255;
					}
				}
				//����ж�������
				//�ж�2<=NZ(P1)<=6
				nCount =  neighbour[1][1] + neighbour[1][2] + neighbour[1][3] \
						+ neighbour[2][1] + neighbour[2][3] + \
						+ neighbour[3][1] + neighbour[3][2] + neighbour[3][3];
				if ( nCount >= 2 && nCount <=6)
					bCondition1 = TRUE;

				//�ж�Z0(P1)=1
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

				//�ж�P2*P4*P8=0 or Z0(p2)!=1
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

				//�ж�P2*P4*P6=0 or Z0(p4)!=1
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
		// ���Ƹ�ʴ���ͼ��
//		Copy(newImage);
	}
	// ���Ƹ�ʴ���ͼ��
	Copy(newImage);

	// ����
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

	// ͼ��ɫ�����߽��߱�ɺ�ɫ
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

	// ���߽��߷�ɫ�ɰ�ɫ
	image.Negative();
	Copy(image);

	//////////////////////////////////////////////////////////////////////////
	// ����ǰ���8������3��Ŀ�����ص㣬����Ŀ�����ص㣬�򽫵�ǰ����Ϊ��������
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

			// ����ǰ����8������3��Ŀ�����ص����Ŀ�����ص�
			if (nCount>=4)	// nCount���������ĵ�
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

			// ����ǰ����8������3��Ŀ�����ص����Ŀ�����ص�
			if (nCount==1)	// nCount���������ĵ�
			{
				SetPixelColor(i,j,RGB(0,0,0));
			}
		}
	}

	return TRUE;
}

// ȡ������Ŀ���γ̱���,ÿ��Ŀ����γ̱���ֱ�洢(08.06.19)����߽簴8����������
void CImage::ObjAllRLC(ObjRunLenCode* pObjAllRLC, UCHAR uObjClr/*=255*/)
{
	int i, x, y, nSum, nNum, h, w;
	queue< RUNLENGTHCODE > que;
	RUNLENGTHCODE item;
	BOOL bFlag = FALSE;

	// ͼ���ȡ��߶�
	int nWidth = (int)GetWidth();
	int nHeight = (int)GetHeight();

	// ȡ�����ж���ɫ���γ̱���
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

				//��������µĶ���,�����־λΪTRUE
				bFlag = TRUE;
			}

			// 
			while(!que.empty())
			{
				// ȡ�ö�ͷԪ�ز�����
				item = que.front();
				que.pop();

				//********************************************************
				//�����ǰ�γ̵���һ�д�������һ�е��γ�������0
				if (item.y-1>=0 && tmpRLC.m_ppObjRun[item.y-1]->nRunNum>0)
				{
					y = item.y-1;

					//����ǰ�γ̵��ұ߽���ͼ���ұ߽�
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
					else	//����ǰ�γ̵��ұ߽�"����"ͼ���ұ߽�
					{
						x = item.xEnd+1;
						nSum = tmpRLC.m_ppObjRun[y]->nRunNum;

						//��������һ���γ̵��ұ�
						if (x > tmpRLC.m_ppObjRun[y]->RC[nSum-1].xEnd)
						{
							for (i=nSum-1; i>=0; i--)
							{
								if (tmpRLC.m_ppObjRun[y]->RC[i].xEnd+1 >= item.xStart)
								{
									//������γ�δ����ǹ�(��y!=-1),�������
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
						// ����ڵ�һ���γ���ʼλ�ú����һ���γ�ĩ��λ��֮��
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
				//�����ǰ�γ̵���һ�д�������һ�е��γ�������0
				if (item.y+1<nHeight && tmpRLC.m_ppObjRun[item.y+1]->nRunNum>0)
				{
					y = item.y+1;

					//����ǰ�γ̵��ұ߽���ͼ���ұ߽�
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
					else		//����ǰ�γ̵��ұ߽�"����"ͼ���ұ߽�
					{
						x = item.xEnd+1;
						nSum = tmpRLC.m_ppObjRun[y]->nRunNum;

						//��������һ���γ̵��ұ�
						if (x > tmpRLC.m_ppObjRun[y]->RC[nSum-1].xEnd)
						{
							for(i=nSum-1; i>=0; i--)
							{
								if (tmpRLC.m_ppObjRun[y]->RC[i].xEnd+1 >= item.xStart)
								{
									//������γ�δ����ǹ�(��y!=-1),�������
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
						//����ڵ�һ���γ���ʼλ�ú����һ���γ�ĩ��λ��֮��
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
			
			//***********************�����ϱ�ǵ��γ̱��뵥���洢***********************
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

				// Ϊ��һ��Ŀ����γ̷����ڴ�
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

							// ��-1��Ǹ��γ�,��ʾ���γ��Ѿ�����¼��
							tmpRLC.m_ppObjRun[y]->RC[i].y = -1;

							// ͳ��Ŀ���е��γ�����
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

	//����������Ŀ
	pObjAllRLC->m_nObjNum = nNum;
}

// ȡ�õ���Ŀ����γ̱��루��߽簴8����������
void CImage::ObjRLC(ObjRunLenCode* pObjAllRLC, POINT ptSeed, UCHAR uObjClr/*=255*/)
{
	// ��ObjRunLenCode������ڵ��øú���ʱ�Ѿ�����
	ASSERT(pObjAllRLC != NULL);

	// ����ڴ����
	pObjAllRLC->Clear();

	// ������ӵ���ɫ����Ŀ��ɫ,���˳�
	if (((RGBQUAD)GetPixelColor(ptSeed.x, ptSeed.y)).rgbRed != uObjClr)
		return;

	int i, x, y, nSum;
	queue< RUNLENGTHCODE > que;
	RUNLENGTHCODE item;

	// ���ͼ���Ⱥ͸߶�
	int nWidth = GetWidth();
	int nHeight = GetHeight();

	x = ptSeed.x;
	y = ptSeed.y;

	// ȡ���γ̱���
	ObjRunLenCode tmpRLC;
	RunLengthcode(&tmpRLC, uObjClr);

	PPRUNLENGTHCODE_MGR ppObjRun = tmpRLC.GetObjAll();

	// ȡ�õ�y�е��γ���Ŀ
	nSum = tmpRLC.m_ppObjRun[y]->nRunNum;

	// �����ӵ������γ�ѹ�����
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
		// �Ӷ�ͷ����γ̲�����
		item = que.front();
		que.pop();
		y = item.y;

		//********************************************************
		//�����ǰ�γ̵���һ�д�������һ�е��γ�������0
		if (item.y-1>=0 && ppObjRun[item.y-1]->nRunNum>0)
		{
			y = item.y-1;

			// ����ǰ�γ̵��ұ߽���ͼ���ұ߽�
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
			else //����ǰ�γ̵��ұ߽�"����"ͼ���ұ߽�
			{
				x = item.xEnd+1;
				nSum = ppObjRun[y]->nRunNum;

				// ��������һ���γ̵��ұ�
				if (x > ppObjRun[y]->RC[nSum-1].xEnd)
				{
					for (i=nSum-1; i>= 0; i--)
					{
						if (ppObjRun[y]->RC[i].xEnd+1 >= item.xStart)
						{
							//������γ�δ����ǹ�(��bFlags = FALSE),�������
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
				// ����ڵ�һ���γ���ʼλ�ú����һ���γ�ĩ��λ��֮��
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
		//�����ǰ�γ̵���һ�д�������һ�е��γ�������0
		if (item.y+1<nHeight && ppObjRun[item.y+1]->nRunNum>0)
		{
			y = item.y+1;

			// ����ǰ�γ̵��ұ߽���ͼ���ұ߽�
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
			else //����ǰ�γ̵��ұ߽�"����"ͼ���ұ߽�
			{
				x = item.xEnd+1;
				nSum = ppObjRun[y]->nRunNum;

				// ��������һ���γ̵��ұ�
				if (x > ppObjRun[y]->RC[nSum-1].xEnd)
				{
					for (i=nSum-1; i>=0; i--)
					{
						if (ppObjRun[y]->RC[i].xEnd+1 >= item.xStart)
						{
							// ������γ�δ����ǹ�(��y!=-1),�������
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
				//����ڵ�һ���γ���ʼλ�ú����һ���γ�ĩ��λ��֮��
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

	//***********************�����ϱ�ǵ��γ̱��뵥���洢***********************
	pObjAllRLC->m_nObjNum = 1;
	nSum = 0;

	// ͳ�Ʊ���ǵ��γ���Ŀ
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

	// �洢���ӵ�����Ŀ��������γ�
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

// ȡ������Ŀ��ı߽����루��߽簴8����������
void CImage::ObjAllEdgeLst(ObjEdgeList* pObjEdgeLst, UCHAR uObjClr/*=255*/)
{
	ASSERT(pObjEdgeLst != NULL);
	pObjEdgeLst->Clear();

	ObjRunLenCode tmpAllRLC;
	ObjAllRLC(&tmpAllRLC, uObjClr);

	int i;
	POINT point;

	// ֱ�Ӱѵ�������ı��ض���ָ��ָ��ĵ�ַ��������������ض����ĳ��ָ��
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

// ȡ�õ���Ŀ��ı߽����루��߽簴8����������
void CImage::ObjEdgeLst(ObjEdgeList* pObjEdgeLst, POINT ptSeed, UCHAR uObjClr/*=255*/)
{
	ASSERT(pObjEdgeLst != NULL);
	pObjEdgeLst->Clear();

#define EDGE_MARK -1	// ��Χ�߽������ı��

	int i, j, x, y, nLen;
	vector< POINT > vec;

	CPoint point, nextPoint, point1;
	int nOri, nNextOri, nOri1;

	int nWidth = GetWidth();
	int nHeight = GetHeight();

	// ������ӵ���ɫ����Ŀ��ɫ,���˳�
	if (((RGBQUAD)GetPixelColor(ptSeed.x, ptSeed.y)).rgbRed != uObjClr)
		return;

	ObjRunLenCode tmpRLC;
	ObjRLC(&tmpRLC, ptSeed, uObjClr);

	// �õ�Ŀ�����Ӿ���
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

	// ��ʱ�½�����,��ԭͼ��ı߽���չһ������,��ȫ����Ϊ����ɫ
	int nMWidth = rect.Width()+2;
	int nMHeight = rect.Height()+2;
	short* pMatrix = new short [nMWidth*nMHeight];
	if (pMatrix == NULL)
		return;
	memset(pMatrix, 255-uObjClr, sizeof(UCHAR)*nMWidth*nMHeight);

	// ��Ŀ�꿽�����½���ʱ������
	int nOffX = rect.left;
	int nOffY = rect.top;
	for (i=0; i<pRLCMgr->nRunNum; i++)
	{
		for (j=pRLCMgr->RC[i].xStart; j<=pRLCMgr->RC[i].xEnd; j++)
			pMatrix[(pRLCMgr->RC[i].y-nOffY+1)*nMWidth+(j-nOffX+1)] = uObjClr;
	}

	//****************************���Ҷ������߽�����****************************
	// ��չ�߽�����ʼ��Ե���ص�ΪĿ��ĵ�һ���߽��
	x = pRLCMgr->RC[0].xStart;
	y = pRLCMgr->RC[0].y;
	point.x = x-nOffX+1;
	point.y = y-nOffY+1;
	vec.push_back(point);

	// ����8�����ƽ������
	CPoint move8[8];
	move8[0].x = 1, move8[0].y = 0;	// ��1��
	move8[1].x = 1, move8[1].y =-1;	// ��2��
	move8[2].x = 0, move8[2].y =-1;	// ��3��
	move8[3].x =-1, move8[3].y =-1;	// ��4��
	move8[4].x =-1, move8[4].y = 0;	// ��5��
	move8[5].x =-1, move8[5].y = 1;	// ��6��
	move8[6].x = 0, move8[6].y = 1;	// ��7��
	move8[7].x = 1, move8[7].y = 1;	// ��8��

	// ����4�����ƽ������
	CPoint move4[4];
	move4[0].x = 1, move4[0].y = 0;	// ��1��
	move4[1].x = 0, move4[1].y =-1;	// ��2��
	move4[2].x =-1, move4[2].y = 0;	// ��3��
	move4[3].x = 0, move4[3].y = 1;	// ��4��

	// �����ҵ���ǰ��ʱ�����ҵ�ǰ���8�����4��������ʼ�������·�ʽ����
	int nPrevOri8[] = {7,7,1,1,3,3,5,5};
	int nPrevOri4[] = {3,0,1,2};

	//*******************������߽��,ͬʱ����߽��������ϱ��********************
	// �ӵ�4�㿪ʼ���ҵ�point����һ��8�������ɫΪuObjClr�ĵ�
	for (i=5; i<5+8; i++)
	{
		nextPoint = point+move8[i%8];
		if (pMatrix[nextPoint.y*nMWidth+nextPoint.x] == uObjClr)
		{
			nNextOri = i%8;
			break;
		}

		// �����ҹ��ĵ���ΪEDGE_MARK
		pMatrix[nextPoint.y*nMWidth+nextPoint.x] = EDGE_MARK;
	}

	// ���ֻ��һ����ɫΪuObjClr��Ŀ���,��ֱ�Ӵ洢�õ�
	if (i == 13)
	{
		nLen = 1;
	}
	else
	{
		// ���µ�һ������������һ������Ըõ��8������
		point1 = nextPoint;
		nOri1 = nNextOri;

		while (TRUE)
		{
			point = nextPoint;
			nOri = nPrevOri8[nNextOri];
			vec.push_back(point);

			// �ӵ�nOri����ʼ����8�����
			for (i=nOri; i<nOri+8; i++)
			{
				nextPoint = point+move8[i%8];
				if (pMatrix[nextPoint.y*nMWidth+nextPoint.x] == uObjClr)
				{
					nNextOri = i%8;
					break;
				}

				// �����ҹ�����ɫ��uObjClr�ĵ���ΪEDGE_MARK
				pMatrix[nextPoint.y*nMWidth+nextPoint.x] = EDGE_MARK;
			}

			// �ٴ������ڶ�����ʱֹͣ
			if (nextPoint==point1 && nNextOri==nOri1)
			{
				// ���һ���㼴��һ����,���䵯��
				vec.pop_back();
				break;
			}
		}

		nLen = vec.size();
	}

	// �����ڴ�ռ�洢��߽�
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
	//**************************�����������߽�����Ĳ���**************************

	//*****************************���Ҷ���ĸ����ڱ߽�*****************************

	int nNum = 0;
	for (i=0; i<pRLCMgr->nRunNum; i++)
	{
		// ��ppchTemp��,x,y����ֵ��Ҫ��һ
		x = pRLCMgr->RC[i].xEnd-nOffX+1;
		y = pRLCMgr->RC[i].y-nOffY+1;

		// ���(x+1,y)��ֵΪ����ɫ
		if ((pMatrix[y*nMWidth+x+1]!=uObjClr)
			&& (pMatrix[y*nMWidth+x+1]!=EDGE_MARK))
		{
			nNum++;
			point.x = x+1;
			point.y = y;
			vec.push_back(point);

			// ����4�����ڱ���ɫ��
			for (j=3; j<3+4; j++)
			{
				nextPoint = point+move4[j%4];
				if (pMatrix[nextPoint.y*nMWidth+nextPoint.x] != uObjClr)
				{
					nNextOri = j%4;
					break;
				}
			}

			// ���ֻ��һ������ɫ��
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

					// ����4�����ڱ���ɫ��
					for (j=nOri; j<nOri+4; j++)
					{
						nextPoint = point+move4[j%4];
						if (pMatrix[nextPoint.y*nMWidth+nextPoint.x] != uObjClr)
						{
							nNextOri = j%4;
							break;
						}
					}
					
					//�ٴ������ڶ�����ʱֹͣ
					if(nextPoint == point1 && nNextOri == nOri1 )
					{
						// ���һ���㼴��һ����,���䵯��
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

				// ���ڱ߽�����ϱ��
				pMatrix[point.y*nMWidth+point.x] = EDGE_MARK;
				pObjEdgeLst->m_ppEdgeObj[0]->ppEdgeLst[nNum]->edge[j].x = point.x-1+nOffX;
				pObjEdgeLst->m_ppEdgeObj[0]->ppEdgeLst[nNum]->edge[j].y = point.y-1+nOffY;
			}
		} // if (pMatrix[y*(rect.Width()+2)+x+1)
	} // for (i=0; i<pRLCMgr->nRunNum; i++)
	//***************************������������ڱ߽�Ĳ���***************************

	pObjEdgeLst->m_ppEdgeObj[0]->nEdgeNum = nNum+1;

	delete [] pMatrix;
}

// �߽����
/* ==================================================================
 * �������ƣ�  ObjEdgeMerge()
 * ���ߣ�      [Leng]
 * ������:     [2008-12-4 22:34:39]
 * �����б� 
               [ObjEdgeList* pSrcEdgeLst] - [In]	Դ�߽�����
               [ ObjEdgeList* pDstEdgeLst] - [In]	�������߽�����
               [ float fDist] - [In]	�߽������ֵ
ע�� :	
				
==================================================================*/
void CImage::ObjEdgeMerge(ObjEdgeList* pSrcEdgeLst, float fDist, ObjEdgeList* pDstEdgeLst)
{
	int i, j, k, m, n;
	POINT point, start, end;

	// �߽���Ͻ���е�Ŀ����Ŀ��ԭ�߽������е�Ŀ����Ŀ���
//	pDstEdgeLst->m_nObjMaxNum = pSrcEdgeLst->m_nObjNum;
	pDstEdgeLst->m_nObjNum = pSrcEdgeLst->m_nObjNum;
//	pDstEdgeLst->m_ppEdgeObj = new PEDGE_OBJECT [pDstEdgeLst->m_nObjNum];

	// �߽���Ͻ���и�Ŀ��ı߽���Ŀ��ԭ�߽������еĶ�Ӧ��Ŀ���
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

	// ����������ڴ洢�߽���Ͻ�������б߽����
	vector<POINT> vEdgePts;
	for (i=0; i<pSrcEdgeLst->m_nObjNum; i++)
	{
		for (j=0; j<ppSrcObj[i]->nEdgeNum; j++)
		{
			k = 0;

			// �洢�����߽���ʼ��
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
						// �����˵��غ�,�����
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
				// �ж��߽�����ĩ������Ĵ������
				if (m >= ppSrcEdge[j]->nEdgeLen)
				{
					// ��������������ʱ,m = j+2,��j��Ϊ���һ��,�����ʱ��j+1��(��m-1��)Ϊ���һ��
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
// ȡ�������γ̱���(08.06.19)
void CImage::RunLengthcode(ObjRunLenCode* pObjAllRLC, UCHAR uObjClr/*=255*/)
{
	// ��ObjRunLenCode������ڵ��øú���ʱ�Ѿ�����
	ASSERT(pObjAllRLC != NULL);

	// ����ڴ����
	pObjAllRLC->Clear();

	// ͼ���ȼ��߶�
	int nWidth, nHeight;
	nWidth = (int)GetWidth();
	nHeight = (int)GetHeight();

	// �˴���ÿһ��Ϊһ���γ�Ŀ�꣬��Ŀ����ȡnHeight
	pObjAllRLC->m_nObjNum = nHeight;

	// �γ̱���
	int nNum, nStart;
	for (int i=0; i<nHeight; i++)
	{
		nNum = 0;

		// ��ÿһ�е��γ̱��������(nWidth/2+1)��
		pObjAllRLC->m_ppObjRun[i] = (PRUNLENGTHCODE_MGR)new UCHAR [sizeof(int)+(nWidth/2+1)*sizeof(RUNLENGTHCODE)];

		for (int j=0; j<nWidth; j++)
		{
			RGBQUAD RGBj = (RGBQUAD)GetPixelColor(j, i);
			RGBQUAD RGBj1 = (RGBQUAD)GetPixelColor(j-1, i);

			if (j == 0)		//����ǵ�i�еĵ�һ��Ԫ��
			{
				if(RGBj.rgbRed == uObjClr)
					nStart = 0;
			}
			else if(j > 0 && j+1 < nWidth)	//����ǵ�i�еĵ�һ�������һ��Ԫ��֮���Ԫ��
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
			else	//����ǵ�i�е����һ������
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

// ����������ֵ�����ӵ��������
/* ==================================================================
 * �������ƣ�  CImage::ObjRgnGrow()
 * ���ߣ�      [Leng]
 * ������:     [2008-6-26 14:20:35]
 * �����б� 
               [POINT ptSeed] - [In] �����������ӵ�
               [ UCHAR uGrwThrshld] - [In]������������ֵ
               [ ObjRunLenCode* pRgnGrow] - [In]���洢������������γ̱���
ע�� :	
				
==================================================================*/
void CImage::ObjRgnGrow(POINT ptSeed, UCHAR uGrwThrshld, ObjRunLenCode* pRgnGrow)
{
	ASSERT(pRgnGrow != NULL);

	int i, j;

	// ����8�����������
	POINT move[8] = {
		{ 1, 0 },
		{ 1, 1 },
		{ 0, 1 },
		{-1, 1 },
		{-1, 0 },
		{-1,-1 },
		{ 0,-1 }};

	// ���ñ�Ǿ���,���ڱ�Ǹ����ص��Ƿ񱻲��ҹ�
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
		// ������βԪ��
		pt = que.front();
		que.pop();

		for (i=0; i<8; i++)
		{
			pt1 = pt+move[i];
			if (IsInside(pt1.x, pt1.y))
			{
				rgb1 = GetPixelColor(pt1.x, pt1.y);
				clr1 = (rgb1.rgbRed+rgb1.rgbGreen+rgb1.rgbBlue)/3.0+0.5;

				// �����������ĵ�ѹ���ջ�����ϱ��
				if (abs(clr-clr1)<=uGrwThrshld && pImage->GetPixelColor(pt1.x, pt1.y).rgbRed==0)
				{
					que.push(pt1);
					pImage->SetPixelColor(pt1.x, pt1.y, RGB(255,255,255));
				}
			}
		}
	}

	// �����������Ŀ����γ�
	pImage->ObjRLC(pRgnGrow, ptSeed);

	delete pImage;
}

// ɾ��Ŀ���ڲ�����ɫ
//void CImage::ObjDelInner(ObjEdgeList* pObjEdgeLst)
//{
//}

// ����Ŀ����ɫ
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

// ��̬��Χѹ��
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

// �ݶȵ�����Ȩƽ��
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

			// ��Ȩֵ
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

			// ��������
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

			// �������
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
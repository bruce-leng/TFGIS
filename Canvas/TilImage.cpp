// TilImage.cpp: implementation of the CTilImage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TilImage.h"

#include "TilComFunction.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

ENUM_CXIMAGE_FORMATS get_file_type(const CString& strPathName)
{
	CString strExt = get_file_ext(strPathName);
	strExt.MakeLower();
	
	if (strExt == "bmp" || strExt == EXT_IMAGE)
#if CXIMAGE_SUPPORT_BMP
		return CXIMAGE_FORMAT_BMP;
#endif
#if CXIMAGE_SUPPORT_GIF
	else if(strExt == "gif")
		return CXIMAGE_FORMAT_GIF;
#endif
#if CXIMAGE_SUPPORT_JPG
	else if(strExt == "jpg")
		return CXIMAGE_FORMAT_JPG;
#endif
#if CXIMAGE_SUPPORT_PNG
	else if(strExt == "png")
		return CXIMAGE_FORMAT_PNG;
#endif
#if CXIMAGE_SUPPORT_MNG
	else if(strExt == "mng")
		return CXIMAGE_FORMAT_MNG;
#endif
#if CXIMAGE_SUPPORT_ICO
	else if(strExt == "ico")
		return CXIMAGE_FORMAT_ICO;
#endif
#if CXIMAGE_SUPPORT_TIF
	else if(strExt == "tif")
		return CXIMAGE_FORMAT_TIF;
#endif
#if CXIMAGE_SUPPORT_TGA
	else if(strExt == "tga")
		return CXIMAGE_FORMAT_TGA;
#endif
#if CXIMAGE_SUPPORT_PCX
	else if(strExt == "pcx")
		return CXIMAGE_FORMAT_PCX;
#endif
#if CXIMAGE_SUPPORT_WBMP
	else if(strExt == "wbmp")
		return CXIMAGE_FORMAT_WBMP;
#endif
#if CXIMAGE_SUPPORT_WMF
	else if(strExt == "wmf")
		return CXIMAGE_FORMAT_WMF;
#endif
#if CXIMAGE_SUPPORT_J2K
	else if(strExt == "j2k")
		return CXIMAGE_FORMAT_J2K;
#endif
#if CXIMAGE_SUPPORT_JBG
	else if(strExt == "jbg")
		return CXIMAGE_FORMAT_JBG;
#endif
#if CXIMAGE_SUPPORT_JP2
	else if(strExt == "jp2")
		return CXIMAGE_FORMAT_JP2;
#endif
#if CXIMAGE_SUPPORT_JPC
	else if(strExt == "jpc")
		return CXIMAGE_FORMAT_JPC;
#endif
#if CXIMAGE_SUPPORT_PGX
	else if(strExt == "pgx")
		return CXIMAGE_FORMAT_PGX;
#endif
#if CXIMAGE_SUPPORT_PNM
	else if(strExt == "pnm")
		return CXIMAGE_FORMAT_PNM;
#endif
#if CXIMAGE_SUPPORT_RAS
	else if(strExt == "ras")
		return CXIMAGE_FORMAT_RAS;
#endif
	else
		return CXIMAGE_FORMAT_UNKNOWN;
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTilImage::CTilImage()
{
	m_nIdx = 0;
	m_bModified = FALSE;
	m_bShow = TRUE;
 	m_dbZoomX = m_dbZoomY = 1.0;
}

CTilImage::CTilImage(CTilImage& image)
{
	m_nIdx = image.m_nIdx;
	m_bModified = image.m_bModified;
	m_bShow = image.m_bShow;
 	m_dbZoomX = image.m_dbZoomX;
	m_dbZoomY = image.m_dbZoomY;
//	m_strPathName = strPathName;
	for (int i = 0; i < image.m_ImageArray.GetSize(); i ++)
	{
		CImage* pImage = (CImage*)image.m_ImageArray.GetAt(i);
		CImage* pTemp = new CImage;
		pTemp->Copy(*pImage);

		long double dbZoomX, dbZoomY;
		pImage->GetZoomFactor(dbZoomX, dbZoomY);
		pTemp->SetZoomFactor(dbZoomX, dbZoomY);

		m_ImageArray.Add(pTemp);
	}
}

CTilImage::CTilImage(const CString& strPathName)
{
	Open(strPathName);
}

CTilImage::~CTilImage()
{
	for (int i = 0; i < m_ImageArray.GetSize(); i ++)
		delete m_ImageArray.GetAt(i);
}

BOOL CTilImage::Open(const CString& strPathName)
{
	m_ImageArray.RemoveAll();
	m_nIdx = 0;
	m_bModified = FALSE;
	m_bShow = TRUE;
 	m_dbZoomX = m_dbZoomY = 1.0;
	m_strPathName = strPathName;

	ENUM_CXIMAGE_FORMATS imagetype = get_file_type(strPathName);
	CImage* pImage = new CImage(strPathName, imagetype);
	m_ImageArray.Add(pImage);

	return TRUE;
}

BOOL CTilImage::Save()
{
	return SaveAs(m_strPathName);
}

BOOL CTilImage::SaveAs(const CString& strPathName)
{
	m_bModified = FALSE;
	m_strPathName = strPathName;

	ENUM_CXIMAGE_FORMATS imagetype = get_file_type(m_strPathName);
	CImage* pImage = (CImage*)m_ImageArray.GetAt(m_nIdx);
	pImage->SetModifiedFlag(FALSE);

	return pImage->Save(m_strPathName, imagetype);
}

void CTilImage::SetModifiedFlag(BOOL bFlag)
{
	m_bModified = bFlag;
}

BOOL CTilImage::IsModified()  const
{
	for (int i=0; i<m_ImageArray.GetSize(); i++)
	{
		CImage* pImage = (CImage*)m_ImageArray.GetAt(i);
		if (pImage->IsModified())
			return TRUE;
	}

	return m_bModified;
}

void CTilImage::SetPathName(const CString& strPathName)
{
	if (m_strPathName != strPathName)
	{
		m_strPathName = strPathName;
	}
}

CString CTilImage::GetPathName() const
{
	return m_strPathName;
}

void CTilImage::SetShowMode(BOOL bShow)
{
	m_bShow = bShow;
}

BOOL CTilImage::GetShowMode() const
{
	return m_bShow;
}

void CTilImage::Draw(CDC* pDC)
{
	if (!m_bShow)
		return;

	CImage* pImage = (CImage*)m_ImageArray.GetAt(m_nIdx);
	pImage->Draw(pDC);
}

//void CTilImage::DrawAll(CDC* pDC)
//{
//}
// 
void CTilImage::Stretch(CDC* pDC, const CRect& rect)
{
	if (!m_bShow)
		return;

	CImage* pImage = (CImage*)m_ImageArray.GetAt(m_nIdx);
	pImage->Stretch(pDC->GetSafeHdc(), rect);
}

void CTilImage::SetCurIdx(int nIdx)
{
	m_nIdx = nIdx;
}

int CTilImage::GetCurIdx() const
{
	return m_nIdx;
}

void CTilImage::SetZoomFactor(long double dbZoomX, long double dbZoomY)
{
	m_dbZoomX = dbZoomX;
	m_dbZoomY = dbZoomY;

	// 设置每副图像的缩放率
	for (int i = 0; i < Size(); i ++)
	{
 		CImage* pImage = (CImage*)m_ImageArray.GetAt(i);
 		pImage->SetZoomFactor(dbZoomX, dbZoomY);
	}	
}

void CTilImage::GetZoomFactor(long double& dbZoomX, long double& dbZoomY)
{
	dbZoomX = m_dbZoomX;
	dbZoomY = m_dbZoomY;
}
 
int CTilImage::Size()
{
	return m_ImageArray.GetSize();
}

void CTilImage::PushBackImage(CImage* pImage)
{
	pImage->SetZoomFactor(m_dbZoomX, m_dbZoomY);
	m_ImageArray.Add(pImage);
}

void CTilImage::PushFrontImage(CImage* pImage)
{
	pImage->SetZoomFactor(m_dbZoomX, m_dbZoomY);
	m_ImageArray.InsertAt(0, pImage);
	m_nIdx ++;
}

void CTilImage::InsertImage(CImage* pImage, int nIdx)
{
	ASSERT(nIdx >= 0 && nIdx <= m_ImageArray.GetSize());
	pImage->SetZoomFactor(m_dbZoomX, m_dbZoomY);

	m_ImageArray.InsertAt(nIdx, pImage);
	if (m_nIdx >= nIdx)
		m_nIdx ++;
}

void CTilImage::DeleteImage(int nIdx)
{
	ASSERT(nIdx >= 0 && nIdx < m_ImageArray.GetSize());
	CImage* pImage = (CImage*)m_ImageArray.GetAt(nIdx);
	delete pImage;
	m_ImageArray.RemoveAt(nIdx);
}

CImage* CTilImage::GetImageByIdx(int nIdx)
{
	int nSize = m_ImageArray.GetSize();
	ASSERT(nIdx >= 0 && nIdx < m_ImageArray.GetSize());
	return (CImage*)m_ImageArray.GetAt(nIdx);
}

CImage* CTilImage::GetCurImage()
{
	return (CImage*)m_ImageArray.GetAt(m_nIdx);
}

CImage* CTilImage::GetPreImage()
{
	if (m_nIdx > 0)
	{
		m_nIdx --;
		return (CImage*)m_ImageArray.GetAt(m_nIdx-1);
	}
	else
		return NULL;
}

CImage* CTilImage::GetNextImage()
{
	if (m_nIdx+1 < m_ImageArray.GetSize())
	{
		m_nIdx ++;
		return (CImage*)m_ImageArray.GetAt(m_nIdx+1);
	}
	else
		return NULL;
}

int CTilImage::GetWidth()
{
	CImage* pImage = (CImage*)m_ImageArray.GetAt(m_nIdx);
	return pImage->GetWidth()/**m_dbZoomX*/;
}

int CTilImage::GetHeight()
{
	CImage* pImage = (CImage*)m_ImageArray.GetAt(m_nIdx);
	return pImage->GetHeight()/**m_dbZoomY*/;
}

int CTilImage::GetDepth()
{
	CImage* pImage = (CImage*)m_ImageArray.GetAt(m_nIdx);
	return pImage->GetBpp()/8;
}

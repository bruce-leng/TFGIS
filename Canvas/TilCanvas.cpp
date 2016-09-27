// TilCanvas.cpp: implementation of the CTilCanvas class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TilCanvas.h"
#include "Graph\Polygon.h"

#include "Image.h"
#include "TilComFunction.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTilCanvas::CTilCanvas()
{
	Initial();
}

CTilCanvas::CTilCanvas(const CTilCanvas& canvas)
{
	m_FaceCaption = canvas.m_FaceCaption;
	m_pImage = new CTilImage(*canvas.m_pImage);
	m_pGraph = new CTilGraph(*canvas.m_pGraph);
}

CTilCanvas::CTilCanvas(const CString& strPathImage, const CString& strPathGraph)
{
	Initial();
	Open(strPathImage, strPathGraph);
}

CTilCanvas::CTilCanvas(CTilImage* pImage, CTilGraph* pGraph)
{
	Initial();
	Load(pImage, pGraph);
}

void CTilCanvas::Initial()
{
  m_pImage = NULL;
	m_pGraph = NULL;
	m_bModified = FALSE;
	m_FaceCaption.bImgCenter  = FALSE;
	m_FaceCaption.ptImgCenter.x = 0.0;
	m_FaceCaption.ptImgCenter.y = 0.0;

	m_FaceCaption.bGrndCenter = FALSE;
	m_FaceCaption.ptGrndCenter.x = 0.0;
	m_FaceCaption.ptGrndCenter.y = 0.0;
	m_FaceCaption.ptGrndCenter.z = 0.0;

	m_FaceCaption.bAngle = FALSE;
	m_FaceCaption.fAngle = 0.0;
}

CTilCanvas::~CTilCanvas()
{
	if (m_pImage)	delete m_pImage;
	if (m_pGraph) delete m_pGraph;
}

BOOL CTilCanvas::Open(const CString& strPathImage, const CString& strPathGraph)
{
	if (m_pImage) delete m_pImage;
	if (m_pGraph) delete m_pGraph;

	m_pImage = new CTilImage(strPathImage);

	// ����֮ǰĳЩû�б���ò㣬�ļ���Ϊ��
	if (strPathGraph.IsEmpty())
		m_pGraph = new CTilGraph;
	else
		m_pGraph = new CTilGraph(strPathGraph);

	m_bModified = FALSE;

	return TRUE;
}

BOOL CTilCanvas::Load(CTilImage* pImage, CTilGraph* pGraph)
{
	Initial();
	m_pImage = pImage;
	m_pGraph = pGraph;

	return TRUE;
}

// ����ͼƬ��Ĭ������������������������߲�����ʱʹ��
void CTilCanvas::SetDefaultTunArc()
{
	CGraph* pGraph = m_pGraph->GetGraphByIdx(0);
	CUnit* pUnit = NULL;
	if (pGraph->Size() == 0)
		pUnit = new CPolygon();
	else
	{
		pUnit = pGraph->GetUnitByIdx(0);
		pUnit->m_pPointList->RemoveAll();
	}

	// ��ͼƬ��4����������γ��ı�����ΪĬ�ϵ�������
	POINT2DF pt(0,0);
	pUnit->m_pPointList->Add(pt);
	pt = POINT2DF(0, GetHeight());
	pUnit->m_pPointList->Add(pt);
	pt = POINT2DF(GetWidth(), GetHeight());
	pUnit->m_pPointList->Add(pt);
	pt = POINT2DF(GetWidth(), 0);
	pUnit->m_pPointList->Add(pt);

	if (pGraph->Size() == 0)
		pGraph->PushBackUnit(pUnit);
}

BOOL CTilCanvas::Save()
{
	BOOL bRet1 = FALSE, bRet2 = FALSE;
	if (m_pImage->IsModified())
		bRet1 = m_pImage->Save();
	if (m_pGraph->IsModified())
		bRet2 = m_pGraph->Save();

	return bRet1&&bRet2;
}

BOOL CTilCanvas::SaveAs(const CString& strPathImage, const CString& strPathGraph)
{
	m_pImage->SaveAs(strPathImage);
	m_pGraph->SaveAs(strPathGraph);

	return TRUE;
}

// ���������ļ���(2014.11.28)
void CTilCanvas::RenameAll(CString strOldFileName, CString strNewFileName)
{
	CTilImage* pImage = GetImage();
	CTilGraph* pGraph = GetGraph();
	
	// ����·��
	CString strPath = get_file_path(pImage->GetPathName());

	// ԭ����·�����ļ���
	CString strOldPathSrc = strPath + '\\' + strOldFileName + '.' + "jpg"; // "*.jpg"
	CString strOldPathImage = strPath + '\\' + strOldFileName + '.' + EXT_IMAGE; // "*.img"
	CString strOldPathGraph = strPath + '\\' + strOldFileName + '.' + EXT_GRAPH; // "*.gph"
	CString strOldPathInfo = strPath + '\\' + strOldFileName + '.' + EXT_INFO; // "*.ifo"

	// �µ�·�����ļ���
	CString strNewPathSrc = strPath + '\\' + strNewFileName + '.' + "jpg";
	CString strNewPathImage = strPath + '\\' + strNewFileName + '.' + EXT_IMAGE;
	CString strNewPathGraph = strPath + '\\' + strNewFileName + '.' + EXT_GRAPH;
	CString strNewPathInfo = strPath + '\\' + strNewFileName + '.' + EXT_INFO;
	
	HANDLE hFind;
	WIN32_FIND_DATA wfd;
	hFind = FindFirstFile(strOldPathSrc,&wfd);
	if(hFind != INVALID_HANDLE_VALUE)
	{// �ж�jpg�ļ��Ƿ����
		FindClose(hFind);
		CFile::Rename(strOldPathSrc, strNewPathSrc);
	}
	hFind = FindFirstFile(strOldPathImage,&wfd);
	if(hFind != INVALID_HANDLE_VALUE)
	{// �ж�img�ļ��Ƿ����
		FindClose(hFind);
		CFile::Rename(strOldPathImage, strNewPathImage);
	}
	hFind = FindFirstFile(strOldPathGraph,&wfd);
	if(hFind != INVALID_HANDLE_VALUE)
	{// �ж�gph�ļ��Ƿ����
		FindClose(hFind);
		CFile::Rename(strOldPathGraph, strNewPathGraph);
	}
	hFind = FindFirstFile(strOldPathInfo,&wfd);
	if(hFind != INVALID_HANDLE_VALUE)
	{// �ж�ifo�ļ��Ƿ����
		FindClose(hFind);
		CFile::Rename(strOldPathInfo, strNewPathInfo);
	}
	
	pImage->SetPathName(strNewPathImage);
	pGraph->SetPathName(strNewPathGraph);
}

BOOL CTilCanvas::IsValide() const
{
	return (m_pImage&&m_pGraph);
}

void CTilCanvas::Draw(CDC* pDC)
{
	m_pImage->Draw(pDC);
	m_pGraph->Draw(pDC);
}

// ��ptTopLeftΪ���Ͻǵĵ�������ź��ͼ��
void CTilCanvas::Draw(CDC* pDC, const POINT& ptTopLeft)
{
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	
	long double dbZoomX, dbZoomY;
	GetZoomFactor(dbZoomX, dbZoomY);
	
	int nZoomWidth = GetWidth()*dbZoomX;
	int nZoomHeight = GetHeight()*dbZoomY;

	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pDC, nZoomWidth, nZoomHeight);
	
	CBitmap* pOldBitmap = (CBitmap*)memDC.SelectObject(&memBitmap);

	// ���ñ���ɫ
	char* pBits = new char [nZoomWidth*4*nZoomHeight];
	memset(pBits, 255, sizeof(char)*nZoomWidth*4*nZoomHeight);
	memBitmap.SetBitmapBits(sizeof(char)*nZoomWidth*4*nZoomHeight, pBits);
	delete [] pBits;

	Draw(&memDC);

	pDC->BitBlt(ptTopLeft.x, ptTopLeft.y, nZoomWidth, nZoomHeight, &memDC, 0, 0, SRCCOPY);
	
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
}

// ����rectSrc�ڵ�ͼ��ŵ�rectDst��
void CTilCanvas::Draw(CDC* pDC, const CRect& rectDst, const CRect& rectSrc)
{
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pDC, GetWidth(), GetHeight());

	CBitmap* pOldBitmap = (CBitmap*)memDC.SelectObject(&memBitmap);

	Draw(&memDC);

	::SetStretchBltMode(pDC->GetSafeHdc(), COLORONCOLOR);
	::StretchBlt(pDC->GetSafeHdc(),
		rectDst.left, rectDst.top, rectDst.Width(), rectDst.Height(),
		memDC.GetSafeHdc(), 
		rectSrc.left, rectSrc.top, rectSrc.Width(), rectSrc.Height(), 
		SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
}

// ����pointSrc�㿪ʼ��ͼ��ŵ�rectDst��
void CTilCanvas::Draw(CDC* pDC, const CRect& rectDst, const POINT& pointSrc)
{
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pDC, GetWidth(), GetHeight());

	CBitmap* pOldBitmap = (CBitmap*)memDC.SelectObject(&memBitmap);

	Draw(&memDC);

	::SetStretchBltMode(pDC->GetSafeHdc(), COLORONCOLOR);
	::StretchBlt(pDC->GetSafeHdc(), 
		rectDst.left, rectDst.top, rectDst.Width(), rectDst.Height(),
		memDC.GetSafeHdc(), 
		pointSrc.x, pointSrc.y, GetWidth()-pointSrc.x, GetHeight()-pointSrc.y, 
		SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
}

void CTilCanvas::SetZoomFactor(long double dbZoomX, long double dbZoomY)
{
	m_pImage->SetZoomFactor(dbZoomX, dbZoomY);
	m_pGraph->SetZoomFactor(dbZoomX, dbZoomY);
}

void CTilCanvas::GetZoomFactor(long double& dbZoomX, long double& dbZoomY)
{
	m_pImage->GetZoomFactor(dbZoomX, dbZoomY);
// 	m_pGraph->GetZoomFactor(dbZoomX, dbZoomY);
}
 
CTilImage* CTilCanvas::GetImage()
{
	return m_pImage;
}

CTilGraph* CTilCanvas::GetGraph()
{
	return m_pGraph;
}

int CTilCanvas::GetWidth()
{
	return m_pImage->GetWidth();
}

int CTilCanvas::GetHeight()
{
	return m_pImage->GetHeight();
}

int CTilCanvas::GetDepth()
{
	return m_pImage->GetDepth();
}

// ���������ĵ�ͼ������
void CTilCanvas::SetCenterImgCoord(POINT2DF& ptImgCenter)
{
	m_FaceCaption.ptImgCenter = ptImgCenter;
	m_FaceCaption.bImgCenter = TRUE;

	// ���û����޸ı��
	m_bModified = TRUE;
}

const POINT2DF&	CTilCanvas::GetCenterImgCoord() const
{
	return m_FaceCaption.ptImgCenter;
}

void CTilCanvas::SetCenterImgCoord(double x, double y)
{
	m_FaceCaption.ptImgCenter.x = x;
	m_FaceCaption.ptImgCenter.y = y;
	m_FaceCaption.bImgCenter = TRUE;

	// ���û����޸ı��
	m_bModified = TRUE;
}

void CTilCanvas::GetCenterImgCoord(double& x, double& y)
{
	x = m_FaceCaption.ptImgCenter.x;
	y = m_FaceCaption.ptImgCenter.y;
}

void CTilCanvas::SetCenterGrndCoord(POINT_3F& ptGrndCenter)
{
	m_FaceCaption.ptGrndCenter = ptGrndCenter;
	m_FaceCaption.bGrndCenter = TRUE;

	// ���û����޸ı��
	m_bModified = TRUE;
}

const POINT_3F& CTilCanvas::GetCenterGrndCoord() const
{
	return m_FaceCaption.ptGrndCenter;
}

void CTilCanvas::SetCenterGrndCoord(double x, double y, double z)
{
	m_FaceCaption.ptGrndCenter.x = x;
	m_FaceCaption.ptGrndCenter.y = y;
	m_FaceCaption.ptGrndCenter.z = z;
	m_FaceCaption.bGrndCenter = TRUE;

	// ���û����޸ı��
	m_bModified = TRUE;
}

void CTilCanvas::GetCenterGrndCoord(double& x, double& y, double& z)
{
	x = m_FaceCaption.ptGrndCenter.x;
	y = m_FaceCaption.ptGrndCenter.y;
	z = m_FaceCaption.ptGrndCenter.z;
}

void CTilCanvas::SetFaceAngle(float& fAngle)
{
	m_FaceCaption.fAngle = fAngle;
	m_FaceCaption.bAngle = TRUE;

	// ���û����޸ı��
	m_bModified = TRUE;
}

const float CTilCanvas::GetFaceAngle() const
{
	return m_FaceCaption.fAngle;
}

void CTilCanvas::SetCaption(FACECAPTION& FaceCaption)
{
	m_FaceCaption = FaceCaption;
}

FACECAPTION CTilCanvas::GetCaption() const
{
	return m_FaceCaption;
}

// �ж��������и������Ƿ񶼱�д������
BOOL CTilCanvas::IsFaceCaptionRecorded()
{
	if (m_FaceCaption.bImgCenter
	 && m_FaceCaption.bGrndCenter
	 && m_FaceCaption.bAngle)
		return TRUE;

	return FALSE;
}

// �������û�����ԭʼ�ߴ�
void CTilCanvas::ResetCanvas(int nWidth, int nHeight)
{
	// �������ź��ͼ��Ϊ��ԭʼ�ߴ�
	long double dbZoomX, dbZoomY;
	dbZoomX = nWidth*1.0/GetWidth();
	dbZoomY = nHeight*1.0/GetHeight();
	
	for (int i=0; i<m_pGraph->Size(); i++)
	{
		CGraph* pGraph = m_pGraph->GetGraphByIdx(i);
		for (int j=0; j<pGraph->Size(); j++)
		{
			CUnit* pUnit = pGraph->GetUnitByIdx(j);
			pUnit->Reset(dbZoomX, dbZoomY);
		}
	}

	// ֱ�Ӷ�ͼ������ز���
	m_pImage->GetCurImage()->Resample(nWidth, nHeight, 2);
}

void CTilCanvas::SetModifiedFlag(BOOL bModified)
{
	m_bModified = bModified;
	m_pImage->SetModifiedFlag(bModified);
	m_pGraph->SetModifiedFlag(bModified);
}

BOOL CTilCanvas::IsModified()
{
	if (m_pImage->IsModified())
		return TRUE;
	if (m_pGraph->IsModified())
		return TRUE;

	// ���ͼ�Ρ�ͼ��δ�޸ģ��򿴵�ǰ�������޸�״̬���
	return m_bModified;
}

// int nImgWidth: �����ͼ����
// int nImgHeight: �����ͼ��߶�
void CTilCanvas::SaveGraphToImage(CDC* pDC, const CString& strPathName)
{
	// ����ӽӾ���
	CRect rect;
	rect.left = rect.top = 0;
	rect.right = GetWidth();
	rect.bottom = GetHeight();
	//rect.InflateRect(TunArc_Extend, TunArc_Extend);
	
	HBITMAP hBitmap;
	HDC hMemDC;
	HBITMAP hOldBitmap;
	
	HDC hDC = pDC->GetSafeHdc();
	hBitmap = ::CreateCompatibleBitmap(hDC, rect.Width(), rect.Height());
	hMemDC = ::CreateCompatibleDC(hDC);
	hOldBitmap = (HBITMAP)::SelectObject(hMemDC, hBitmap);
	
	// ���ñ���ɫ
	char* pBits = new char [rect.Width()*4*rect.Height()];
	memset(pBits, 255, sizeof(char)*rect.Width()*4*rect.Height());
	::SetBitmapBits(hBitmap, sizeof(char)*rect.Width()*4*rect.Height(), pBits);
	delete [] pBits;
	
	// ��ͼ
	CTilGraph graph(*m_pGraph);
	graph.SetZoomFactor(1.0, 1.0);
	graph.Draw(CDC::FromHandle(hMemDC));

	CxImage image;
	image.CreateFromHBITMAP(hBitmap);
	CString strExt = get_file_ext(strPathName);
	DWORD imagetype;
	if (strExt=="bmp")
		imagetype = CXIMAGE_FORMAT_BMP;
	else
		imagetype = CXIMAGE_SUPPORT_JPG;
	image.Save(strPathName, imagetype);
	
	::DeleteObject(::SelectObject(hMemDC, hOldBitmap));
	::DeleteDC(hMemDC);
}

// ����ͼ�Ρ�ͼ��һ��ͼ����
void CTilCanvas::SaveGraphImage2Image(CDC* pDC, const CString& strPathName)
{
	// ����ӽӾ���
	CRect rect;
	rect.left = rect.top = 0;
	rect.right = GetWidth();
	rect.bottom = GetHeight();
	//rect.InflateRect(TunArc_Extend, TunArc_Extend);
	
	HBITMAP hBitmap;
	HDC hMemDC;
	HBITMAP hOldBitmap;
	
	HDC hDC = pDC->GetSafeHdc();
	hBitmap = ::CreateCompatibleBitmap(hDC, rect.Width(), rect.Height());
	hMemDC = ::CreateCompatibleDC(hDC);
	hOldBitmap = (HBITMAP)::SelectObject(hMemDC, hBitmap);
	
	// ��ͼ��
	CTilImage image(*m_pImage);
	image.SetZoomFactor(1.0,1.0);
	image.Draw(CDC::FromHandle(hMemDC));

	// ��ͼ
	CTilGraph graph(*m_pGraph);
	graph.SetZoomFactor(1.0, 1.0);
	graph.Draw(CDC::FromHandle(hMemDC));
	
	CxImage imageGrkaph;
	imageGrkaph.CreateFromHBITMAP(hBitmap);
	CString strExt = get_file_ext(strPathName);
	DWORD imagetype;
	if (strExt=="bmp")
		imagetype = CXIMAGE_FORMAT_BMP;
	else
		imagetype = CXIMAGE_SUPPORT_JPG;
	imageGrkaph.Save(strPathName, imagetype);
	
	::DeleteObject(::SelectObject(hMemDC, hOldBitmap));
	::DeleteDC(hMemDC);
}


#include "stdafx.h"
#include "ClientCapture.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define RECTWIDTH(lpRect)     ((lpRect)->right - (lpRect)->left)
#define RECTHEIGHT(lpRect)    ((lpRect)->bottom - (lpRect)->top)

/////////////////////////////////////////////////////////////////////////////
// CClientCapture::CClientCapture - Constructor

CClientCapture::CClientCapture()
{
	m_pBMI = 0;
	m_pBits = 0;
	hDIB = 0;
}

/////////////////////////////////////////////////////////////////////////////
// CClientCapture::~CClientCapture - Destructor

CClientCapture::~CClientCapture()
{
	if(hDIB)
		GlobalFree( hDIB );
	hDIB = 0;
	m_pBMI = 0;
	m_pBits = 0;

}

void CClientCapture::Capture(CDC *dc, CRect rectDIB) 
{
	WriteWindowToDIB(dc, rectDIB);
}

BOOL CClientCapture::WriteWindowToDIB( CDC* dc, CRect rect)
{
	CBitmap 	bitmap;
	CDC 		memDC;

	memDC.CreateCompatibleDC(dc); 
	bitmap.CreateCompatibleBitmap(dc, rect.Width(),rect.Height() );
	
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0, 0, rect.Width(),rect.Height(), dc, 0, 0, SRCCOPY); 

	// �����߼���ɫ��
	if( dc->GetDeviceCaps(RASTERCAPS) & RC_PALETTE )
	{
		UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * 256);
		LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];
		pLP->palVersion = 0x300;

		pLP->palNumEntries = GetSystemPaletteEntries( *dc, 0, 255, pLP->palPalEntry );

		pal.CreatePalette( pLP );

		delete[] pLP;
	}

	memDC.SelectObject(pOldBitmap);

	// ת��DDB��DIB
	hDIB = DDBToDIB( bitmap, BI_RGB, &pal, dc);

	if( hDIB == NULL )
		return FALSE;

	return TRUE;
}

BOOL CClientCapture::Paint(HDC hDC, CPalette * m_pPalette, LPRECT lpDCRect, LPRECT lpDIBRect) const
{

	if (!m_pBMI)
		return FALSE;

	HPALETTE hPal = NULL;           // DIB�ĵ�ɫ��
	HPALETTE hOldPal = NULL;        // ��ǰ�ĵ�ɫ��

	// ���DIB��ɫ�壬Ȼ�������õ��豸������DC��
	if (m_pPalette != NULL)
	{
		hPal = (HPALETTE) m_pPalette->m_hObject;
		hOldPal = ::SelectPalette(hDC, hPal, TRUE);
	}
	//  ����ʹ������ģʽ
	::SetStretchBltMode(hDC, COLORONCOLOR);
	BOOL bSuccess = FALSE;
	//  �ж��Ƿ�ʹ������
	if ((RECTWIDTH(lpDCRect)  == RECTWIDTH(lpDIBRect)) && 
		(RECTHEIGHT(lpDCRect) == RECTHEIGHT(lpDIBRect)))
		bSuccess = ::SetDIBitsToDevice(hDC, lpDCRect->left, lpDCRect->top, 
						RECTWIDTH(lpDCRect), RECTHEIGHT(lpDCRect), lpDIBRect->left,
						(int)Height() - lpDIBRect->top - RECTHEIGHT(lpDIBRect),
					    0, (WORD)Height(), m_pBits, m_pBMI, DIB_RGB_COLORS);   
	else
		bSuccess = ::StretchDIBits(hDC, lpDCRect->left, lpDCRect->top, 
						RECTWIDTH(lpDCRect),RECTHEIGHT(lpDCRect), lpDIBRect->left, 
						lpDIBRect->top,RECTWIDTH(lpDIBRect),RECTHEIGHT(lpDIBRect),
						m_pBits, m_pBMI, DIB_RGB_COLORS, SRCCOPY);    
 
	// ���������ϵĵ�ɫ��
	if (hOldPal != NULL)
	{
		::SelectPalette(hDC, hOldPal, TRUE);
	}

	
   return bSuccess;
}

//  ���溯���Ĺ����ǽ�DDBת����DIB
HANDLE CClientCapture::DDBToDIB( CBitmap& bitmap, DWORD dwCompression, CPalette* pPal, 	CDC *dc) 
{
	BITMAP			bm;
	BITMAPINFOHEADER	bi;
	LPBITMAPINFOHEADER 	lpbi;
	DWORD			dwLen;
	HANDLE			handle;
	HDC 			hDC;
	HPALETTE		hPal;

	ASSERT( bitmap.GetSafeHandle() );

	if( dwCompression == BI_BITFIELDS )
		return NULL;

	// ���û�е�ɫ��
	hPal = (HPALETTE) pPal->GetSafeHandle();
	if (hPal==NULL)
		hPal = (HPALETTE) GetStockObject(DEFAULT_PALETTE);

	// ���λͼ��Ϣ
	bitmap.GetObject(sizeof(bm),(LPSTR)&bm);

	// ��ʼ��λͼ��ͷ��Ϣ
	bi.biSize		= sizeof(BITMAPINFOHEADER);
	bi.biWidth		= bm.bmWidth;
	bi.biHeight 		= bm.bmHeight;
	bi.biPlanes 		= 1;
	bi.biBitCount		= bm.bmPlanes * bm.bmBitsPixel;
	bi.biCompression	= dwCompression;
	bi.biSizeImage		= 0;
	bi.biXPelsPerMeter	= 0;
	bi.biYPelsPerMeter	= 0;
	bi.biClrUsed		= 0;
	bi.biClrImportant	= 0;

	// ������Ϣͷ�Ĵ�С����ɫ��
	int nColors = (1 << bi.biBitCount);
	if( nColors > 256 ) 
		nColors = 0;
	dwLen  = bi.biSize + nColors * sizeof(RGBQUAD);

	// ����豸������
	hDC = dc->GetSafeHdc();
	hPal = SelectPalette(hDC,hPal,FALSE);
	RealizePalette(hDC);
	
	// �����ڴ���Դ����λͼ��Ϣͷ����ɫ��
	if(hDIB) GlobalFree( hDIB );

	hDIB = GlobalAlloc(GMEM_FIXED,dwLen);

	if (!hDIB){
		SelectPalette(hDC,hPal,FALSE);
		return NULL;
	}

	lpbi = (LPBITMAPINFOHEADER)hDIB;

	*lpbi = bi;

	//  ���biSizeImage����ֵ
	GetDIBits(hDC, (HBITMAP)bitmap.GetSafeHandle(), 0L, (DWORD)bi.biHeight,
			(LPBYTE)NULL, (LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS);

	bi = *lpbi;

	// �������0,���ֶ�����
	if (bi.biSizeImage == 0)
	{
		bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8) 
						* bi.biHeight;

		//  �������ѹ���㷨
		if (dwCompression != BI_RGB)
			bi.biSizeImage = (bi.biSizeImage * 3) / 2;
	}

	// �����ڴ���Դ�������е�λ��Ϣ
	dwLen += bi.biSizeImage;
	if (handle = GlobalReAlloc(hDIB, dwLen, GMEM_MOVEABLE))
		hDIB = handle;
	else{
		GlobalFree(hDIB);
		hDIB = 0;
		// ѡ��ԭʼ��ɫ��
		SelectPalette(hDC,hPal,FALSE);

		return NULL;
	}

	// ���λͼ��λ��Ϣ
	lpbi = (LPBITMAPINFOHEADER)hDIB;
	m_pBMI = (LPBITMAPINFO)hDIB;
	m_pBits = (LPBYTE)hDIB + (bi.biSize + nColors * sizeof(RGBQUAD));

	// �����DIB
	BOOL bGotBits = GetDIBits( hDC, (HBITMAP)bitmap.GetSafeHandle(),
				0L,						// ��ʼɨ����
				(DWORD)bi.biHeight,		// ɨ��������
				(LPBYTE) m_pBits, 		// ���λͼ��λ���ݵĵ�ַ
				(LPBITMAPINFO)lpbi,		// ���λͼ��Ϣ�ĵ�ַ
				(DWORD)DIB_RGB_COLORS);	// ʹ��RGB��ɫ��

	if( !bGotBits )
	{
		GlobalFree(hDIB);
		hDIB = 0;
		SelectPalette(hDC,hPal,FALSE);

		return NULL;
	}

	SelectPalette(hDC,hPal,FALSE);

	return hDIB;
}

DWORD CClientCapture::Height() const
{
	if (!m_pBMI)
		return 0;
	// ����DIB�ĸ߶�
	return m_pBMI->bmiHeader.biHeight;
}

void CClientCapture::OnDraw(HDC hDC, CRect rcRect, CRect rect)
{
	Paint(hDC, &pal, rcRect, rect);
}

//  ���溯���Ĺ����ǽ�DIB��Ϣд��λͼ�ļ�
BOOL CClientCapture::WriteDIB( CString csFile)
{
	BITMAPFILEHEADER	hdr;
	LPBITMAPINFOHEADER	lpbi;

	if (!hDIB)
		return FALSE;

	CFile file;
	if( !file.Open( csFile, CFile::modeWrite|CFile::modeCreate) )
		return FALSE;

	lpbi = (LPBITMAPINFOHEADER)hDIB;

	int nColors = 1 << lpbi->biBitCount;
	if( nColors > 256 ) 
		nColors = 0;

	// ��д�ļ�ͷ��Ϣ 
	hdr.bfType		= ((WORD) ('M' << 8) | 'B');	// is always "BM"
	hdr.bfSize		= GlobalSize (hDIB) + sizeof( hdr );
	hdr.bfReserved1 	= 0;
	hdr.bfReserved2 	= 0;
	hdr.bfOffBits		= (DWORD) (sizeof( hdr ) + lpbi->biSize +
						nColors * sizeof(RGBQUAD));

	// д���ļ�ͷ 
	file.Write( &hdr, sizeof(hdr) );
	// д��DIBͷ��λͼ���� 
	file.Write( lpbi, GlobalSize(hDIB) );
	return TRUE;
}

void CClientCapture::Release()
{
	if(hDIB)
		GlobalFree( hDIB );
	hDIB = 0;
	m_pBMI = 0;
	m_pBits = 0;

}

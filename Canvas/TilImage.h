// TilImage.h: interface for the CTilImage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TILIMAGE_H__7D71C271_2D20_4A26_938B_241593EB1C7A__INCLUDED_)
#define AFX_TILIMAGE_H__7D71C271_2D20_4A26_938B_241593EB1C7A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Image.h"

ENUM_CXIMAGE_FORMATS AFX_EXT_CLASS get_file_type(const CString& strPathName);

class CImage;
class AFX_EXT_CLASS CTilImage
{
public:
	CTilImage();
	CTilImage(CTilImage& image);
	CTilImage(const CString& strPathName);
	virtual ~CTilImage();

	BOOL Open(const CString& strPathName);
	BOOL Save();
	BOOL SaveAs(const CString& strPathName);

	void SetModifiedFlag(BOOL bFlag = TRUE);
	BOOL IsModified() const;

	void	SetPathName(const CString& strPathName);
	CString GetPathName() const;

	void SetCurIdx(int nIdx);
	int  GetCurIdx() const;

 	void SetZoomFactor(long double dbZoomX, long double dbZoomY);
 	void GetZoomFactor(long double& dbZoomX, long double& dbZoomY);

	int Size();

	void SetShowMode(BOOL bShow = TRUE);
	BOOL GetShowMode() const;

	void Draw(CDC* pDC);
//	void DrawAll(CDC* pDC);
	void Stretch(CDC* pDC, const CRect& rect);

	void PushBackImage(CImage* pImage);
	void PushFrontImage(CImage* pImage);
	void InsertImage(CImage* pImage, int nIdx);

	void DeleteImage(int nIdx);

	CImage* GetImageByIdx(int nIdx);
	CImage* GetCurImage();
	CImage* GetPreImage();
	CImage* GetNextImage();

	int GetWidth();
	int GetHeight();
	int GetDepth();
private:
	int m_nIdx;
 	long double m_dbZoomX;
 	long double m_dbZoomY;
	BOOL m_bModified;
	BOOL m_bShow;
	CObArray m_ImageArray;
	CString m_strPathName;
};

#endif // !defined(AFX_TILIMAGE_H__7D71C271_2D20_4A26_938B_241593EB1C7A__INCLUDED_)

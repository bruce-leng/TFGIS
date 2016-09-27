// TilCanvas.h: interface for the CTilCanvas class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TILCANVAS_H__9E388199_A4F5_449B_88CB_68BFD76CE1B5__INCLUDED_)
#define AFX_TILCANVAS_H__9E388199_A4F5_449B_88CB_68BFD76CE1B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TilImage.h"
#include "TilGraph.h"

// 隧道掌子面属性
typedef struct __tagFACECAPTION
{
	BOOL	 bImgCenter;	// 标记是否已记录了掌子面中心点在图像中的坐标
	POINT2DF ptImgCenter;	// 掌子面中心点的图像坐标

	BOOL	 bGrndCenter;		// 标记是否已记录了中心点坐标
	POINT_3F ptGrndCenter;		// 当前掌子面的中心坐标

	BOOL	 bAngle;		// 标记是否已记录了中心点角度
	float	 fAngle;		// 当前掌子面的角度弧度（相对于正东方）
} FACECAPTION, *PFACECAPTION, **PPFACECAPTION;

class AFX_EXT_CLASS CTilCanvas
{
public:
	CTilCanvas();
	CTilCanvas(const CTilCanvas& canvas);
	CTilCanvas(const CString& strPathImage, const CString& strPathGraph);
	CTilCanvas(CTilImage* pImage, CTilGraph* pGraph);
	virtual ~CTilCanvas();

	BOOL Open(const CString& strPathImage, const CString& strPathGraph);
	BOOL Load(CTilImage* pImage, CTilGraph* pGraph);
	// 设置图片的默认外轮廓，当隧道开挖轮廓线不存在时使用
	void SetDefaultTunArc();
	BOOL Save();
	BOOL SaveAs(const CString& strPathImage, const CString& strPathGraph);

	// 更改所有文件名(2014.11.28)
	void RenameAll(CString strOldFileName, CString strNewFileName);

	BOOL IsValide() const;

	void Draw(CDC* pDC);
	void Draw(CDC* pDC, const POINT& ptTopLeft);
	void Draw(CDC* pDC, const CRect& rectDst, const CRect& rectSrc);
	void Draw(CDC* pDC, const CRect& rectDst, const POINT& pointSrc);
	// 重新设置画布的原始尺寸
	void ResetCanvas(int nWidth, int nHeight);

 	void SetZoomFactor(long double dbZoomX, long double dbZoomY);
 	void GetZoomFactor(long double& dbZoomX, long double& dbZoomY);

	CTilImage* GetImage();
	CTilGraph* GetGraph();

	int GetWidth();
	int GetHeight();
	int GetDepth();
private:
	CTilImage* m_pImage;
	CTilGraph* m_pGraph;

	BOOL m_bModified;
	//////////////////////////////////////////////////////////////////////////
	// 设置隧道掌子面属性(08.09.13)
public:
	void SetModifiedFlag(BOOL bModified = TRUE);
	BOOL IsModified();

	// 掌子面中心点图像坐标
	void SetCenterImgCoord(POINT2DF& ptImgCenter);
	const POINT2DF&	GetCenterImgCoord() const;

	void SetCenterImgCoord(double x, double y);
	void GetCenterImgCoord(double& x, double& y);

	// 掌子面中心点大地坐标
	void SetCenterGrndCoord(POINT_3F& ptGrndCenter);
	const POINT_3F& GetCenterGrndCoord() const;

	void SetCenterGrndCoord(double x, double y, double z);
	void GetCenterGrndCoord(double& x, double& y, double& z);

	// 掌子面角度
	void SetFaceAngle(float& fAngle);
	const float	GetFaceAngle() const;

	// 掌子面属性
	void SetCaption(FACECAPTION& FaceCaption);
	FACECAPTION GetCaption() const;

	// 判断掌子面中各属性是否都被写入数据
	BOOL	IsFaceCaptionRecorded();

	// 将图形保存成图像
	void SaveGraphToImage(CDC* pDC, const CString& strPathName);
	// 保存图形、图像到一个图像中
	void SaveGraphImage2Image(CDC* pDC, const CString& strPathName);
private:
	FACECAPTION m_FaceCaption;
	// 隧道断面图形
	

	void Initial();
	//////////////////////////////////////////////////////////////////////////
};

#endif // !defined(AFX_TILCANVAS_H__9E388199_A4F5_449B_88CB_68BFD76CE1B5__INCLUDED_)

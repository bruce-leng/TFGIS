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

// �������������
typedef struct __tagFACECAPTION
{
	BOOL	 bImgCenter;	// ����Ƿ��Ѽ�¼�����������ĵ���ͼ���е�����
	POINT2DF ptImgCenter;	// ���������ĵ��ͼ������

	BOOL	 bGrndCenter;		// ����Ƿ��Ѽ�¼�����ĵ�����
	POINT_3F ptGrndCenter;		// ��ǰ���������������

	BOOL	 bAngle;		// ����Ƿ��Ѽ�¼�����ĵ�Ƕ�
	float	 fAngle;		// ��ǰ������ĽǶȻ��ȣ��������������
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
	// ����ͼƬ��Ĭ������������������������߲�����ʱʹ��
	void SetDefaultTunArc();
	BOOL Save();
	BOOL SaveAs(const CString& strPathImage, const CString& strPathGraph);

	// ���������ļ���(2014.11.28)
	void RenameAll(CString strOldFileName, CString strNewFileName);

	BOOL IsValide() const;

	void Draw(CDC* pDC);
	void Draw(CDC* pDC, const POINT& ptTopLeft);
	void Draw(CDC* pDC, const CRect& rectDst, const CRect& rectSrc);
	void Draw(CDC* pDC, const CRect& rectDst, const POINT& pointSrc);
	// �������û�����ԭʼ�ߴ�
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
	// �����������������(08.09.13)
public:
	void SetModifiedFlag(BOOL bModified = TRUE);
	BOOL IsModified();

	// ���������ĵ�ͼ������
	void SetCenterImgCoord(POINT2DF& ptImgCenter);
	const POINT2DF&	GetCenterImgCoord() const;

	void SetCenterImgCoord(double x, double y);
	void GetCenterImgCoord(double& x, double& y);

	// ���������ĵ�������
	void SetCenterGrndCoord(POINT_3F& ptGrndCenter);
	const POINT_3F& GetCenterGrndCoord() const;

	void SetCenterGrndCoord(double x, double y, double z);
	void GetCenterGrndCoord(double& x, double& y, double& z);

	// ������Ƕ�
	void SetFaceAngle(float& fAngle);
	const float	GetFaceAngle() const;

	// ����������
	void SetCaption(FACECAPTION& FaceCaption);
	FACECAPTION GetCaption() const;

	// �ж��������и������Ƿ񶼱�д������
	BOOL	IsFaceCaptionRecorded();

	// ��ͼ�α����ͼ��
	void SaveGraphToImage(CDC* pDC, const CString& strPathName);
	// ����ͼ�Ρ�ͼ��һ��ͼ����
	void SaveGraphImage2Image(CDC* pDC, const CString& strPathName);
private:
	FACECAPTION m_FaceCaption;
	// �������ͼ��
	

	void Initial();
	//////////////////////////////////////////////////////////////////////////
};

#endif // !defined(AFX_TILCANVAS_H__9E388199_A4F5_449B_88CB_68BFD76CE1B5__INCLUDED_)

// TilSysDoc.h : interface of the CTilSysDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TILSYSDOC_H__53A6FDC2_6CD0_4DC3_AB11_AE07DD16BEF7__INCLUDED_)
#define AFX_TILSYSDOC_H__53A6FDC2_6CD0_4DC3_AB11_AE07DD16BEF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SceneGraph3d.h"
#include "Mesh3d.h"
#include "FaceCaption.h"
#include "Tunnel.h"
#include "FaceInfoPrintEx.h"

// 文件操作线程
void RunFileOperationThread(void* pParam);
void RunImageThread(void* pParam);

class CTilSysDoc : public CDocument
{
protected: // create from serialization only
	CTilSysDoc();
	DECLARE_DYNCREATE(CTilSysDoc)

// Attributes
public:
	HANDLE	m_hThread;		// 执行菜单命令的线程
	long	m_lMenuCommand;	// 菜单命令ID
	int		m_nMenuCol;		// 标记调用的是哪一列的菜单命令
	HWND	m_hWnd;			// 视图指针句柄
	CString m_strFileName;	// 打开的文件

	CTunnel m_Tunnel;
	BOOL m_bTunSide;
	BOOL m_bTunStruct;
	BOOL m_bTunFace;
	BOOL m_bSectionNormal;
	BOOL m_bSectionProfile;
// Operations
public:
	// 打印的掌子面素描内容
	FaceInfoPrintEx m_FaceInfoPrint;

	void UpdateDrawTree();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTilSysDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTilSysDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// histogram(09.06.05)
	long m_hr[256];
	long m_hg[256];
	long m_hb[256];
	long m_hgray[256];
	long m_hmax;

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTilSysDoc)
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnImageDilate();
	afx_msg void OnImageThreshold();
	afx_msg void OnUpdateImageDilate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateImageThreshold(CCmdUI* pCmdUI);
	afx_msg void OnViewImage();
	afx_msg void OnViewGraph();
	afx_msg void OnViewZoomin();
	afx_msg void OnViewZoomout();
	afx_msg void OnImageMirror();
	afx_msg void OnImageFlip();
	afx_msg void OnImageNegative();
	afx_msg void OnImageRotatel();
	afx_msg void OnImageRotater();
	afx_msg void OnImageRotate();
	afx_msg void OnImageCrop();
	afx_msg void OnUpdateViewImage(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewGraph(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewZoomin(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewZoomout(CCmdUI* pCmdUI);
	afx_msg void OnUpdateImageMirror(CCmdUI* pCmdUI);
	afx_msg void OnUpdateImageFlip(CCmdUI* pCmdUI);
	afx_msg void OnUpdateImageNegative(CCmdUI* pCmdUI);
	afx_msg void OnUpdateImageRotatel(CCmdUI* pCmdUI);
	afx_msg void OnUpdateImageRotater(CCmdUI* pCmdUI);
	afx_msg void OnUpdateImageRotate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateImageCrop(CCmdUI* pCmdUI);
	afx_msg void OnViewZoom();
	afx_msg void OnUpdateViewZoom(CCmdUI* pCmdUI);
	afx_msg void OnImageColorize();
	afx_msg void OnImageLighten();
	afx_msg void OnImageDarken();
	afx_msg void OnImageContrast();
	afx_msg void OnImageLesscontrast();
	afx_msg void OnImageMedian();
	afx_msg void OnImageErode();
	afx_msg void OnImageBlur();
	afx_msg void OnImageSoften();
	afx_msg void OnImageGaussian3x3();
	afx_msg void OnImageGaussian5x5();
	afx_msg void OnImageSharpen();
	afx_msg void OnUpdateImageColorize(CCmdUI* pCmdUI);
	afx_msg void OnUpdateImageLighten(CCmdUI* pCmdUI);
	afx_msg void OnUpdateImageDarken(CCmdUI* pCmdUI);
	afx_msg void OnUpdateImageContrast(CCmdUI* pCmdUI);
	afx_msg void OnUpdateImageLesscontrast(CCmdUI* pCmdUI);
	afx_msg void OnUpdateImageMedian(CCmdUI* pCmdUI);
	afx_msg void OnUpdateImageErode(CCmdUI* pCmdUI);
	afx_msg void OnUpdateImageContour(CCmdUI* pCmdUI);
	afx_msg void OnUpdateImageBlur(CCmdUI* pCmdUI);
	afx_msg void OnUpdateImageSoften(CCmdUI* pCmdUI);
	afx_msg void OnUpdateImageGaussian3x3(CCmdUI* pCmdUI);
	afx_msg void OnUpdateImageGaussian5x5(CCmdUI* pCmdUI);
	afx_msg void OnUpdateImageSharpen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateImageEdge(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewZoomnormal(CCmdUI* pCmdUI);
	afx_msg void OnViewZoomnormal();
	afx_msg void OnViewGraphLayer(UINT nID);
	afx_msg void OnUpdateViewGraphLayer(CCmdUI* pCmdUI);
	afx_msg void OnOpenglUpdateData();
	afx_msg void OnUpdateOpenglUpdateData(CCmdUI* pCmdUI);
	afx_msg void OnGlVertex();
	afx_msg void OnGlLine();
	afx_msg void OnGlFace();
	afx_msg void OnGlSmooth();
	afx_msg void OnGlLight();
	afx_msg void OnUpdateGlVertex(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGlLine(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGlFace(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGlSmooth(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGlLight(CCmdUI* pCmdUI);
	afx_msg void OnMeshLoop();
	afx_msg void OnUpdateMeshLoop(CCmdUI* pCmdUI);
	afx_msg void OnGlTunarc();
	afx_msg void OnUpdateGlTunarc(CCmdUI* pCmdUI);
	afx_msg void OnGlAddWireframe();
	afx_msg void OnUpdateGlAddWireframe(CCmdUI* pCmdUI);
	afx_msg void OnFileResume();
	afx_msg void OnUpdateFileResume(CCmdUI* pCmdUI);
	afx_msg void OnImageStretch();
	afx_msg void OnUpdateImageStretch(CCmdUI* pCmdUI);
	afx_msg void OnImageNormalize();
	afx_msg void OnUpdateImageNormalize(CCmdUI* pCmdUI);
	afx_msg void OnDrillRange();
	afx_msg void OnUpdateDrillRange(CCmdUI* pCmdUI);
	afx_msg void OnGeoPredict();
	afx_msg void OnUpdateGeoPredict(CCmdUI* pCmdUI);
	afx_msg void OnTunSide();
	afx_msg void OnUpdateTunSide(CCmdUI* pCmdUI);
	afx_msg void OnTunStruct();
	afx_msg void OnUpdateTunStruct(CCmdUI* pCmdUI);
	afx_msg void OnTunDrill();
	afx_msg void OnUpdateTunDrill(CCmdUI* pCmdUI);
	afx_msg void OnTunFace();
	afx_msg void OnUpdateTunFace(CCmdUI* pCmdUI);
	afx_msg void OnSectionNormal();
	afx_msg void OnUpdateSectionNormal(CCmdUI* pCmdUI);
	afx_msg void OnSectionProfile();
	afx_msg void OnUpdateSectionProfile(CCmdUI* pCmdUI);
	afx_msg void OnImageUsm();
	afx_msg void OnUpdateImageUsm(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// 识别掌子面岩体岩性

public:	// OpenGL三维建模函数
	// 渲染场景
	void RenderScene(BOOL bSelectMode = FALSE);
	void CreateMeshByObject();
	void CreateArcFace();
	void ConstructSidePlaneA(CSceneGraph3d &pSceneGraph, CMesh3d* pMesh, 
		PPOINT_3F pObj1, int nNum1, PPOINT_3F pObj2, int nNum2);
		//int nTextureIdx, int nCurSliceIdx);
	void ConstructFace(CSceneGraph3d& pSceneGraph, MESH_TYPE type,
		PPOINT_3F pObj, int nNum, 
		int nTextureIdx, int nCanvasIdx);
	// 创建岩体裂隙
	void ConstructCrack(CSceneGraph3d& pSceneGraph, CMesh3d* pMesh, MESH_TYPE type,
		PPOINT_3F pObj1, int nNum1, PPOINT_3F pObj2, int nNum2, 
		int nTextureIdx, int nCanvasIdx);
	void CreateTexture();

	typedef struct __tagOPENGLSHOWINFO
	{
		BOOL AddWireframe;
		BOOL Smooth;
		float PolygonOffset;
		BOOL Light;
		GLenum Mode;
		BOOL bCreateTextureIsFinished;
		BOOL bShowDoubleFace;
		BOOL bTunArc;
	} OPENGLSHOWINFO, *POPENGLSHOWINFO, **PPOPENGLSHOWINFO;

	OPENGLSHOWINFO m_glShowInfo;
	CSceneGraph3d m_SceneGraph;

	// 掌子面属性，包括里程、埋深、节理特征参数等
	deque<CFaceCaption*> m_dFaceCaption;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TILSYSDOC_H__53A6FDC2_6CD0_4DC3_AB11_AE07DD16BEF7__INCLUDED_)

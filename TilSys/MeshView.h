#if !defined(AFX_MESHVIEW_H__605950C1_DEE8_4D06_AF86_8984B32BC9BD__INCLUDED_)
#define AFX_MESHVIEW_H__605950C1_DEE8_4D06_AF86_8984B32BC9BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MeshView.h : header file
//

#include "ClientCapture.h"
/////////////////////////////////////////////////////////////////////////////
// CMeshView view
class CMeshView : public CView
{
protected:
	CMeshView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMeshView)

// Attributes
public:
	CTilSysDoc* GetDocument();

	/////////////////////////////////////////////////////////////////
	//添加成员函数与成员变量
	BOOL RenderScene();
	BOOL InitializeOpenGL(CDC* pDC);
	BOOL SetupPixelFormat(void);
	void SetLogicalPalette(void);

	HGLRC		m_hRC;			//OpenGL绘制描述表
	HPALETTE	m_hPalette;		//OpenGL调色板
	CDC*	    m_pDC;			//OpenGL设备描述表
	UINT		m_uPalID;		//OpenGL调色板ID号
	/////////////////////////////////////////////////////////////////	

	// Mouse 
	BOOL m_LeftButtonDown;
	BOOL m_RightButtonDown;
	CPoint m_LeftDownPos;
	CPoint m_RightDownPos;
	HCURSOR m_CursorRotation;
	
	// Position, rotation ,scaling
	void InitGeometry(void);
	
	float m_xRotation;
	float m_yRotation;
	float m_zRotation;
	BOOL m_xyRotation;
	
	float m_xTranslation;
	float m_yTranslation;
	float m_zTranslation;
	
	float m_xScaling;
	float m_yScaling;
	float m_zScaling;
	
	float m_SpeedTranslation;
	float m_SpeedRotation;
	
	// Colors
	float m_ClearColorRed;
	float m_ClearColorGreen;
	float m_ClearColorBlue;
	
	// Animation
	float m_StepRotationX;
	float m_StepRotationY;
	float m_StepRotationZ;
	

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMeshView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMeshView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CMeshView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnFilePrintPreview();
	//}}AFX_MSG
	afx_msg void OnOpenGLCtrPal(UINT uID);
	afx_msg void OnUpdateOpenGLCtrPal(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

	CClientCapture m_captureImage;
};

#ifdef _DEBUG
inline CTilSysDoc* CMeshView::GetDocument()
	{	return (CTilSysDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MESHVIEW_H__605950C1_DEE8_4D06_AF86_8984B32BC9BD__INCLUDED_)

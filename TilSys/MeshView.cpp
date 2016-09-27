// MeshView.cpp : implementation file
//

#include "stdafx.h"
#include "TilSys.h"
#include "TilSysDoc.h"
#include "MeshView.h"
#include <gl\glu.h>
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

GLdouble g_Aspect = 1.0;
GLfloat g_fOvy = 45.0;
double g_clip_z = 0.3;
/////////////////////////////////////////////////////////////////////////////
// CMeshView

IMPLEMENT_DYNCREATE(CMeshView, CView)

CMeshView::CMeshView()
{
	// OpenGL
	// Mouse
	m_LeftButtonDown = FALSE;
	m_RightButtonDown = FALSE;
	
	// Colors
	m_ClearColorRed   = 0.0f;
	m_ClearColorGreen = 1.0f;
	m_ClearColorBlue  = 1.0f;
	
	// Animation
	m_StepRotationX = 0.0f;
	m_StepRotationY = 5.0f;
	m_StepRotationZ = 0.0f;
	
	InitGeometry();

	m_uPalID = -1;
}

//********************************************
// InitGeometry
//********************************************
void CMeshView::InitGeometry(void)
{
	m_xRotation = 0.0f;
	m_yRotation = 0.0f;
	m_zRotation = 180.0f;
	
	m_xTranslation = 0.0f;
	m_yTranslation = 0.0f;
	m_zTranslation = -3.0f;//-35.0f;
	
	m_xScaling = 1.0f;
	m_yScaling = 1.0f;
	m_zScaling = 1.0f;

	m_SpeedRotation = 1.0f / 3.0f;
	m_SpeedTranslation = 1.0f / 100.0f;

	m_xyRotation = 1;

	g_Aspect = 1.0;
	g_fOvy = 45.0;
	g_clip_z = 0.3;
}

CMeshView::~CMeshView()
{
}

BEGIN_MESSAGE_MAP(CMeshView, CView)
	//{{AFX_MSG_MAP(CMeshView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)

	ON_COMMAND_RANGE(ID_OPENGL_CTR_1, ID_OPENGL_CTR_17, OnOpenGLCtrPal)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OPENGL_CTR_1, ID_OPENGL_CTR_17, OnUpdateOpenGLCtrPal)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMeshView drawing

int CMeshView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_pDC = new CClientDC(this);
	InitializeOpenGL(m_pDC);
	
	return 0;
}

void CMeshView::OnDraw(CDC* pDC)
{
	CTilSysDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	if (pDC->IsPrinting()) 
	{
		CRect rcDIB;
		GetClientRect(&rcDIB);
		rcDIB.right = rcDIB.Width();
		rcDIB.bottom = rcDIB.Height();

		// 获得打印机页面尺寸(像素)
		int cxPage = pDC->GetDeviceCaps(HORZRES);
		int cyPage = pDC->GetDeviceCaps(VERTRES);

		// 获得打印机每英寸上的像素个数
		int cxInch = pDC->GetDeviceCaps(LOGPIXELSX);
		int cyInch = pDC->GetDeviceCaps(LOGPIXELSY);
		CRect rcDest;
		rcDest.top = rcDest.left = 0;
		rcDest.bottom = (int)(((double)rcDIB.Height() * cxPage * cyInch)
			/ ((double)rcDIB.Width() * cxInch));
		rcDest.right = cxPage;
		m_captureImage.OnDraw(pDC->m_hDC, &rcDest, &rcDIB);
	}
	else 
	{
		wglMakeCurrent( pDC->m_hDC, m_hRC );
		CRect rc;
		GetClientRect(&rc);
		double aspect = (rc.Height() == 0) ? rc.Width() : (double)rc.Width()/(double)rc.Height();
		g_Aspect = aspect;
		glViewport(0,0,rc.Width(),rc.Height());
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		gluPerspective(g_fOvy,g_Aspect ,g_clip_z,1000.0);
		
		glMatrixMode(GL_MODELVIEW);

		glLoadIdentity();
		RenderScene();
		wglMakeCurrent( 0, 0 );
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMeshView diagnostics

#ifdef _DEBUG
void CMeshView::AssertValid() const
{
	CView::AssertValid();
}

void CMeshView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMeshView message handlers

void CMeshView::OnFilePrintPreview() 
{
	CRect rcDIB;
	GetClientRect(&rcDIB);
	OnDraw(m_pDC);
	m_captureImage.Capture(m_pDC, rcDIB);

	CView::OnFilePrintPreview();	

	// 更新非客户区(正常情况下应该可以不用调用这条语句的???)
	((CMainFrame*)AfxGetMainWnd())->RecalcLayout(TRUE);
}

BOOL CMeshView::OnPreparePrinting(CPrintInfo* pInfo) 
{
	// TODO: call DoPreparePrinting to invoke the Print dialog box
	if(!pInfo->m_bPreview)
	{
		CRect rcDIB;
		GetClientRect(&rcDIB);
		OnDraw(m_pDC);
		m_captureImage.Capture(m_pDC, rcDIB);
	}

	return DoPreparePrinting(pInfo);
}

void CMeshView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CView::OnBeginPrinting(pDC, pInfo);
}

void CMeshView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_captureImage.Release();
	
	CView::OnEndPrinting(pDC, pInfo);
}

void CMeshView::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView) 
{
	// TODO: Add your specialized code here and/or call the base class
	CRect rcDIB;
	GetClientRect(&rcDIB);
	OnDraw(m_pDC);
	m_captureImage.Capture(m_pDC, rcDIB);
	
	CView::OnEndPrintPreview(pDC, pInfo, point, pView);
}

//////////////////////////////////////////////////////////
//						初始化openGL场景
//////////////////////////////////////////////////////////
BOOL CMeshView::InitializeOpenGL(CDC* pDC)
{
	m_pDC = pDC;
	SetupPixelFormat();
	//生成绘制描述表
	m_hRC = ::wglCreateContext(m_pDC->GetSafeHdc());
	//置当前绘制描述表
	::wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
	
	glPolygonMode(GL_FRONT,GL_FILL);
	glPolygonMode(GL_BACK,GL_FILL );
	
    glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);
	
	// Lights properties
	float	ambientProperties[]  = {1.0f, 1.0f, 0.3f, 1.0f};
	float	diffuseProperties[]  = {0.8f, 0.8f, 0.8f, 1.0f};
	float	specularProperties[] = {1.0f, 1.0f, 1.0f, 1.0f};
	
	glLightfv( GL_LIGHT0, GL_AMBIENT, ambientProperties);
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseProperties);
	glLightfv( GL_LIGHT0, GL_SPECULAR, specularProperties);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);
	
	glClearColor(m_ClearColorRed,m_ClearColorGreen,m_ClearColorBlue,1.0f);
	glClearDepth(1.0f);
	//	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	
	// Perspective
	CRect rect;
	GetClientRect(&rect);
	
	double aspect = (rect.Height() == 0) ? rect.Width() : (double)rect.Width()/(double)rect.Height();
	TRACE("\n XXXXXX: CSize: %d, %d", rect.Width(), rect.Height());
	gluPerspective(g_fOvy,aspect,0.1,1000.0);
	
	//	glPolygonMode(GL_FRONT,GL_FILL);
	//	glPolygonMode(GL_BACK,GL_POINT);
	
	// Default : lighting
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	
	// Default : blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	
	// Default : material
	float	MatAmbient[]  = {0.0f, 0.33f, 0.50f, 1.0f};
	float	MatDiffuse[]  = {0.5f, 0.5f, 0.5f, 1.0f};
	float	MatSpecular[]  = {0.1f, 0.1f, 0.1f, 1.0f};
	float	MatShininess[]  = { 84 };
	float	MatEmission[]  = {0.0f, 0.0f, 0.0f, 1.0f};
	
	// Back : green
	float MatAmbientBack[]  = {0.0f, 1.0f, 0.0f, 1.0f};
	
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(Gl_LESS);
	
	// Modulate : texture lighting
	glEnable(GL_TEXTURE_2D);
	TRACE("Texture parameters...\n");
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	return TRUE;
}

//////////////////////////////////////////////////////////
//						设置像素格式
//////////////////////////////////////////////////////////
BOOL CMeshView::SetupPixelFormat()
{
	PIXELFORMATDESCRIPTOR pfd;
	
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |  PFD_DOUBLEBUFFER | PFD_STEREO_DONTCARE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cRedBits = 8;
	pfd.cRedShift = 16;
	pfd.cGreenBits = 8;
	pfd.cGreenShift = 8;
	pfd.cBlueBits = 8;
	pfd.cBlueShift = 0;
	pfd.cAlphaBits = 0;
	pfd.cAlphaShift = 0;
	pfd.cAccumBits = 64;
	pfd.cAccumRedBits = 16;
	pfd.cAccumGreenBits = 16;
	pfd.cAccumBlueBits = 16;
	pfd.cAccumAlphaBits = 0;
	pfd.cDepthBits = 32;
	pfd.cStencilBits = 8;
	pfd.cAuxBuffers = 0;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.bReserved = 0;
	pfd.dwLayerMask = 0;
	pfd.dwVisibleMask = 0;
	pfd.dwDamageMask = 0;
	
	int pixelformat;
	pixelformat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);//选择像素格式
	::SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd);	//设置像素格式
	//	if(pfd.dwFlags & PFD_NEED_PALETTE)
	//		SetLogicalPalette();	//设置逻辑调色板
	return TRUE;
}

/////////////////////////////////////////////////////////////////////
//	                  设置逻辑调色板
//////////////////////////////////////////////////////////////////////
void CMeshView::SetLogicalPalette(void)
{
    struct
    {
        WORD Version;
        WORD NumberOfEntries;
        PALETTEENTRY aEntries[256];
    } logicalPalette = { 0x300, 256 };
	
	BYTE reds[] = {0, 36, 72, 109, 145, 182, 218, 255};
	BYTE greens[] = {0, 36, 72, 109, 145, 182, 218, 255};
	BYTE blues[] = {0, 85, 170, 255};
	
    for (int colorNum=0; colorNum<256; ++colorNum)
    {
        logicalPalette.aEntries[colorNum].peRed =
            reds[colorNum & 0x07];
        logicalPalette.aEntries[colorNum].peGreen =
            greens[(colorNum >> 0x03) & 0x07];
        logicalPalette.aEntries[colorNum].peBlue =
            blues[(colorNum >> 0x06) & 0x03];
        logicalPalette.aEntries[colorNum].peFlags = 0;
    }
	
//    m_hPalette = CreatePalette ((LOGPALETTE*)&logicalPalette);
}

BOOL CMeshView::RenderScene() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();
	// Position / translation / scale
	glTranslated(m_xTranslation,m_yTranslation,m_zTranslation);
	glRotatef(m_xRotation, 1.0, 0.0, 0.0);
	glRotatef(m_yRotation, 0.0, 1.0, 0.0);
	glRotatef(m_zRotation, 0.0, 0.0, 1.0);
	glScalef(m_xScaling,m_yScaling,m_zScaling);

	if (CObject3d::g_bDrawText)
	{
		glEnable(GL_TEXTURE_2D);
		TRACE("Texture parameters...\n");

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}
	GetDocument()->RenderScene(FALSE);

	glPopMatrix();

	::SwapBuffers(m_pDC->GetSafeHdc());		//交互缓冲区
	glFlush();
	return TRUE;
}

void CMeshView::OnDestroy() 
{
	CView::OnDestroy();

	//删除调色板和渲染上下文、定时器
	::wglMakeCurrent(0,0);
	::wglDeleteContext( m_hRC);
	if (m_hPalette)
		DeleteObject(m_hPalette);
	if ( m_pDC )
	{
		delete m_pDC;
	}
}

void CMeshView::OnSize(UINT nType, int cx, int cy) 
{
	// TODO: Add your message handler code here
	if (!IsWindowVisible() || !IsWindowEnabled())
		return;
	
	CRect rc;
	GetClientRect(&rc);
	TRACE("\n 1: CSize: %d, %d", rc.Width(), rc.Height());
	
	//	wglMakeCurrent(m_pDC->GetSafeHdc(),m_hRC);

	// Set OpenGL perspective, viewport and mode
	double aspect = (cy == 0) ? cx : (double)cx/(double)cy;
	g_Aspect = aspect;
	glViewport(0,0,cx,cy);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(g_fOvy,g_Aspect,g_clip_z,1000.0);
	
	glMatrixMode(GL_MODELVIEW);
	
	glLoadIdentity();
	
	glDrawBuffer(GL_BACK);
	
	// Release
	//	::ReleaseDC(hWnd,hDC);
	CView::OnSize(nType, cx, cy);
	
}

void CMeshView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_LeftButtonDown = TRUE;
	m_LeftDownPos = point;
	SetCapture();
	
	CView::OnLButtonDown(nFlags, point);
}

void CMeshView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_RightButtonDown = FALSE;
	m_LeftButtonDown = FALSE;
	ReleaseCapture();

	CView::OnLButtonUp(nFlags, point);
}

void CMeshView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	// Both : rotation
	if(m_LeftButtonDown && m_RightButtonDown)
	{
		if(m_xyRotation)
		{
			m_yRotation -= (float)(m_LeftDownPos.x - point.x) * m_SpeedRotation;
			m_xRotation -= (float)(m_LeftDownPos.y - point.y) * m_SpeedRotation;
		}
		else
		{
			m_zRotation -= (float)(m_LeftDownPos.x - point.x) * m_SpeedRotation;
			m_xRotation -= (float)(m_LeftDownPos.y - point.y) * m_SpeedRotation;
		}
		m_LeftDownPos = point;
		m_RightDownPos = point;
		InvalidateRect(NULL,FALSE);
	}
	else
	// Left : x / y translation
	if(m_LeftButtonDown)
	{
		m_xTranslation -= (float)(m_LeftDownPos.x - point.x) * m_SpeedTranslation;
		m_yTranslation += (float)(m_LeftDownPos.y - point.y) * m_SpeedTranslation;
		m_LeftDownPos = point;
		InvalidateRect(NULL,FALSE);
	}
	else
	// Right : z translation
	if(m_RightButtonDown)
	{
		m_zTranslation += (float)(m_RightDownPos.y - point.y) * m_SpeedTranslation;
		m_RightDownPos = point;
		InvalidateRect(NULL,FALSE);
	}

	CView::OnMouseMove(nFlags, point);
}

BOOL CMeshView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CMeshView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_RightButtonDown = TRUE;
	m_RightDownPos = point;
	SetCapture();

	CView::OnRButtonDown(nFlags, point);
}

void CMeshView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_RightButtonDown = FALSE;
	m_LeftButtonDown = FALSE;
	ReleaseCapture();

	CView::OnRButtonUp(nFlags, point);
}

void CMeshView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	int nIdx;
	switch(nChar) {
	case VK_UP:
		g_clip_z += 0.05;
		
		//		glMatrixMode(GL_PROJECTION);
		//		glLoadIdentity();
		//
		//		gluPerspective(g_fOvy,g_Aspect ,g_clip_z,1000.0);
		//
		//		glMatrixMode(GL_MODELVIEW);
		//
		//		glLoadIdentity();
		break;
	case VK_DOWN:
		g_clip_z -= 0.05;
		//		glMatrixMode(GL_PROJECTION);
		//		glLoadIdentity();
		//
		//		gluPerspective(g_fOvy,g_Aspect ,g_clip_z,1000.0);
		//
		//		glMatrixMode(GL_MODELVIEW);
		//
		//		glLoadIdentity();
		break;
		
	case VK_HOME:
		g_clip_z = 0.3;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		
		gluPerspective(g_fOvy,g_Aspect ,g_clip_z,1000.0);
		
		glMatrixMode(GL_MODELVIEW);
		
		glLoadIdentity();
		break;
	case VK_END:
		InitGeometry();
		break;
	case VK_LEFT:
		nIdx = GetDocument()->m_SceneGraph.GetCurShowIdx();
		GetDocument()->m_SceneGraph.DrawObject(nIdx+1);
		break;
	case VK_RIGHT:
		GetDocument()->m_SceneGraph.DrawObject(GetDocument()->m_SceneGraph.GetCurShowIdx()-1);
		break;
	case VK_F5:
		GetDocument()->m_SceneGraph.DrawObject(-1);
		break;
	case VK_F4:
		CObject3d::g_bDrawText = !CObject3d::g_bDrawText;
		GetDocument()->m_SceneGraph.DrawObject(GetDocument()->m_SceneGraph.GetCurShowIdx());
		break;
	default:
		
		break;
	}
	Invalidate();
	
	RedrawWindow();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CMeshView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;

	return CView::OnEraseBkgnd(pDC);
}

void CMeshView::OnOpenGLCtrPal(UINT uID)
{
	m_uPalID = uID;
	switch (uID)
	{
		case ID_OPENGL_CTR_1:	// 前视图
			m_xRotation = 0.0;
			m_yRotation = 0.0;
			m_zRotation = 180.0;
			break;
		case ID_OPENGL_CTR_2:	// 后视图
			m_xRotation = 0.0;
			m_yRotation = 180.0;
			m_zRotation = 180.0;
			break;
		case ID_OPENGL_CTR_3:	// 左视图
			m_xRotation = 0.0;
			m_yRotation = 90.0;
			m_zRotation = 180.0;
			break;
		case ID_OPENGL_CTR_4:	// 右视图
			m_xRotation = 0.0;
			m_yRotation = -90.0;
			m_zRotation = 180.0;
			break;
		case ID_OPENGL_CTR_5:	// 顶视图
			m_xRotation = 90.0;
			m_yRotation = 0.0;
			m_zRotation = 180.0;
			break;
		case ID_OPENGL_CTR_6:	// 底视图
		case ID_OPENGL_CTR_7:	// 西南视图（SW）
		case ID_OPENGL_CTR_8:	// 东南视图（SE）
		case ID_OPENGL_CTR_9:	// 东北视图（NE）
		case ID_OPENGL_CTR_10:	// 西北视图（NW）
		case ID_OPENGL_CTR_11:	// 放大
		case ID_OPENGL_CTR_12:	// 缩小
		case ID_OPENGL_CTR_13:	// 缩放到视窗大小
		case ID_OPENGL_CTR_14:	// 立体显示
		case ID_OPENGL_CTR_15:	// 取得剖面图
		case ID_OPENGL_CTR_16:
		case ID_OPENGL_CTR_17:
		;
	}
	Invalidate();
}

void CMeshView::OnUpdateOpenGLCtrPal(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pCmdUI->m_nID == m_uPalID);
}

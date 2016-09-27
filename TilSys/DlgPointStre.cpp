// DlgPointStre.cpp : implementation file
//

#include "stdafx.h"
#include "tilsys.h"
#include "DlgPointStre.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPointStre dialog


CDlgPointStre::CDlgPointStre(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPointStre::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPointStre)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bX1 = 50;
	m_bY1 = 50;
	m_bX2 = 200;
	m_bY2 = 200;

	m_lpDIBBits = NULL;
}

CDlgPointStre::~CDlgPointStre()
{
	if (m_pImage != NULL)
		delete m_pImage;
}

void CDlgPointStre::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPointStre)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPointStre, CDialog)
	//{{AFX_MSG_MAP(CDlgPointStre)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPointStre message handlers

BOOL CDlgPointStre::OnInitDialog() 
{
	// 调用默认OnInitDialog函数
	CDialog::OnInitDialog();
	
	// 获取绘制直方图的标签
	CWnd* pWnd = GetDlgItem(IDC_COORD);
	
	// 计算接受鼠标事件的有效区域
	pWnd->GetClientRect(m_MouseRect);
	pWnd->ClientToScreen(&m_MouseRect);
	
	CRect rect;
	GetClientRect(rect);
	ClientToScreen(&rect);
	
	m_MouseRect.top -= rect.top;
	m_MouseRect.left -= rect.left;
	
	// 设置接受鼠标事件的有效区域
	m_MouseRect.top += 25;
	m_MouseRect.left += 10;
	m_MouseRect.bottom = m_MouseRect.top + 255;
	m_MouseRect.right = m_MouseRect.left + 256;
	
	// 初始化拖动状态
	m_iIsDraging = 0;

	// 备份原始图像
	CTilCanvas* pCanvas = GetCanvasMgr()->GetCurCanvas();
	m_pImage = new CImage(*pCanvas->GetImage()->GetCurImage());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPointStre::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// 当用户单击鼠标左键开始拖动
	if(m_MouseRect.PtInRect(point))
	{
		CRect	rectTemp;
		
		// 计算点1临近区域
		rectTemp.left = m_MouseRect.left + m_bX1 - 2;
		rectTemp.right = m_MouseRect.left + m_bX1 + 2;
		rectTemp.top = 255 + m_MouseRect.top - m_bY1 - 2;
		rectTemp.bottom = 255 + m_MouseRect.top - m_bY1 + 2;
		
		// 判断用户是不是想拖动点1
		if (rectTemp.PtInRect(point))
		{
			
			// 设置拖动状态1，拖动点1
			m_iIsDraging = 1;
			
			// 更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
		}
		else
		{
			// 计算点2临近区域
			rectTemp.left = m_MouseRect.left + m_bX2 - 2;
			rectTemp.right = m_MouseRect.left + m_bX2 + 2;
			rectTemp.top = 255 + m_MouseRect.top - m_bY2 - 2;
			rectTemp.bottom = 255 + m_MouseRect.top - m_bY2 + 2;
			
			// 判断用户是不是想拖动点2
			if (rectTemp.PtInRect(point))
			{
				
				// 设置拖动状态为2，拖动点2
				m_iIsDraging = 2;
				
				// 更改光标
				::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
			}
		}
	}
	
	// 默认单击鼠标左键处理事件
	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgPointStre::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// 当用户释放鼠标左键停止拖动
	if (m_iIsDraging != 0)
	{
		// 重置拖动状态
		m_iIsDraging = 0;

		// 初始化脏标记为FALSE
 		m_pDoc->SetModifiedFlag(FALSE);
 
		// 调用GrayStretch()函数进行灰度拉伸
		GrayStretch(m_bX1, m_bY1, m_bX2, m_bY2);
	
		// 设置脏标记
		m_pDoc->SetModifiedFlag(TRUE);

		GetCanvasMgr()->GetCurCanvas()->SetModifiedFlag(TRUE);
	
		// 更新视图
		m_pDoc->UpdateAllViews(NULL);
	}
	
	// 默认释放鼠标左键处理事件
	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgPointStre::OnMouseMove(UINT nFlags, CPoint point) 
{
	// 判断当前光标是否在绘制区域
	if(m_MouseRect.PtInRect(point))
	{
		// 判断是否正在拖动
		if (m_iIsDraging != 0)
		{
			// 判断正在拖动点1还是点2
			if (m_iIsDraging == 1)
			{
				// 判断是否下限<上限
				if (point.x - m_MouseRect.left < m_bX2)
				{
					// 更改下限
					m_bX1 = (BYTE) (point.x - m_MouseRect.left);
				}
				else
				{
					// 下限拖过上限，设置为上限-1
					m_bX1 = m_bX2 - 1;
					
					// 重设鼠标位置
					point.x = m_MouseRect.left + m_bX2 - 1;
				}
				
				// 更改Y坐标
				m_bY1 = (BYTE) (255 + m_MouseRect.top - point.y);
			}
			else
			{
				// 正在拖动点2
				
				// 判断是否上限>下限
				if (point.x - m_MouseRect.left > m_bX1)
				{
					// 更改下限
					m_bX2 = (BYTE) (point.x - m_MouseRect.left);
				}
				else
				{
					// 下限拖过上限，设置为下限＋1
					m_bX2 = m_bX1 + 1;
					
					// 重设鼠标位置
					point.x = m_MouseRect.left + m_bX1 + 1;
				}
				
				// 更改Y坐标
				m_bY2 = (BYTE) (255 + m_MouseRect.top - point.y);
			}
			
			// 更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
			
			// 更新
			UpdateData(FALSE);
			
			// 重绘
			InvalidateRect(m_MouseRect, TRUE);
		}
		else
		{
			CRect	rectTemp1;
			CRect	rectTemp2;
			
			// 计算点1临近区域
			rectTemp1.left = m_MouseRect.left + m_bX1 - 2;
			rectTemp1.right = m_MouseRect.left + m_bX1 + 2;
			rectTemp1.top = 255 + m_MouseRect.top - m_bY1 - 2;
			rectTemp1.bottom = 255 + m_MouseRect.top - m_bY1 + 2;
			
			// 计算点2临近区域
			rectTemp2.left = m_MouseRect.left + m_bX2 - 2;
			rectTemp2.right = m_MouseRect.left + m_bX2 + 2;
			rectTemp2.top = 255 + m_MouseRect.top - m_bY2 - 2;
			rectTemp2.bottom = 255 + m_MouseRect.top - m_bY2 + 2;
			
			// 判断用户在点1或点2旁边
			if ((rectTemp1.PtInRect(point)) || (rectTemp2.PtInRect(point)))
			{
				// 更改光标
				::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
			}
		}
	}
	
	// 默认鼠标移动处理事件
	CDialog::OnMouseMove(nFlags, point);
}

void CDlgPointStre::OnPaint() 
{
	// 字符串
	CString str;
	
	// 循环变量
	LONG i;

	// 最大计数
	float fMaxIntensity = 0;

	// 设备上下文
	CPaintDC dc(this);
	
	// 获取绘制坐标的文本框
	CWnd* pWnd = GetDlgItem(IDC_COORD);
	
	// 指针
	CDC* pDC = pWnd->GetDC();
	pWnd->Invalidate();
	pWnd->UpdateWindow();

	pDC->Rectangle(0,0,330,300);
	
	// 创建画笔对象
	CPen* pPenRed = new CPen;
	
	// 红色画笔
	pPenRed->CreatePen(PS_SOLID, 2, RGB(255,0,0));
	
	// 创建画笔对象
	CPen* pPenBlue = new CPen;
	
	// 蓝色画笔
	pPenBlue->CreatePen(PS_SOLID, 1, RGB(0,0, 255));
	
	// 选中当前红色画笔，并保存以前的画笔
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);
	
	// 绘制坐标轴
	pDC->MoveTo(10,10);
	
	// 垂直轴
	pDC->LineTo(10,280);
	
	// 水平轴
	pDC->LineTo(320,280);
	
	// 写坐标
	str.Format("0");
	pDC->TextOut(10, 281, str);
	
	str.Format("255");
	pDC->TextOut(265, 281, str);
	pDC->TextOut(11, 25, str);
	
	// 绘制X轴箭头
	pDC->LineTo(315,275);
	pDC->MoveTo(320,280);
	pDC->LineTo(315,285);
	
	// 绘制X轴箭头
	pDC->MoveTo(10,10);
	pDC->LineTo(5,15);
	pDC->MoveTo(10,10);
	pDC->LineTo(15,15);
	
	// 计算最大计数值
	for (i = 0; i < 256; i ++)
	{
		// 判断是否大于当前最大值
		if (m_fIntensity[i] > fMaxIntensity)
		{
			// 更新最大值
			fMaxIntensity = m_fIntensity[i];
		}
	}

	// 更改成蓝色画笔
	pDC->SelectObject(pPenBlue);	
	
	// 判断是否有计数
	if (fMaxIntensity > 0)
	{
		// 绘制直方图
		for (i = 0; i < 256; i ++)
		{
			pDC->MoveTo(i + 10, 280);
			pDC->LineTo(i + 10, 281 - (int) (m_fIntensity[i] * 256 / fMaxIntensity));
		}
	}

	// 更改成红色画笔
	pDC->SelectObject(pPenRed);	
	
	// 绘制坐标值
	str.Format("(%d, %d)", m_bX1, m_bY1);
	pDC->TextOut(m_bX1 + 10, 281 - m_bY1, str);
	str.Format("(%d, %d)", m_bX2, m_bY2);
	pDC->TextOut(m_bX2 + 10, 281 - m_bY2, str);
	
	// 绘制用户指定的变换直线
	pDC->MoveTo(10, 280);
	pDC->LineTo(m_bX1 + 10, 280 - m_bY1);
	pDC->LineTo(m_bX2 + 10, 280 - m_bY2);
	pDC->LineTo(265, 25);
	
	// 绘制点边缘的小矩形
	CBrush  brush;
	brush.CreateSolidBrush(RGB(0,255,0));
	
	// 选中刷子
	CGdiObject* pOldBrush = pDC->SelectObject(&brush);
	
	// 绘制小矩形
	pDC->Rectangle(m_bX1 + 10 - 2, 280 - m_bY1 - 2, m_bX1 + 12, 280 - m_bY1 + 2);
	pDC->Rectangle(m_bX2 + 10 - 2, 280 - m_bY2 - 2, m_bX2 + 12, 280 - m_bY2 + 2);

	// 恢复以前的画笔
	pDC->SelectObject(pOldPen);	
	
	// 绘制边缘
	pDC->MoveTo(10,25);
	pDC->LineTo(265,25);
	pDC->LineTo(265,280);
	
	// 删除新的画笔
	delete pPenRed;
	delete pPenBlue;
}

void CDlgPointStre::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if (m_lpDIBBits)
		delete [] m_lpDIBBits;
}

/* ==================================================================
 * 函数名称：  CDlgPointStre::GrayStretch()
 * 作者：      [bleng]
 * 创建于:     [2006-1-5 16:07:44]
 * 参数列表： 
               [BYTE bX1] - [In]	折点一的原始灰度
               [ BYTE bY1] - [In]	折点一的变换灰度
               [ BYTE bX2] - [In]	折点二的原始灰度
               [ BYTE bY2] - [In]	折点二的对应灰度
注释 :	该函数对图像进行灰度折线变换
				
==================================================================*/
void CDlgPointStre::GrayStretch(BYTE bX1, BYTE bY1, BYTE bX2, BYTE bY2)
{
	// 循环变量
	LONG i;
	LONG j;

	// 更改光标形状
	AfxGetApp()->BeginWaitCursor();
	
	// 获得管理类指针
	CTilCanvas* pCanvas = GetCanvasMgr()->GetCurCanvas();
	CImage* pImage = pCanvas->GetImage()->GetCurImage();

	// 获得图像的尺寸
	LONG lWidth  = pCanvas->GetWidth();
	LONG lHeight = pCanvas->GetHeight();
	LONG lDepth  = pCanvas->GetDepth();
	LONG lLineWidth = pImage->GetEffWidth();

	// 读图像数据
	LPBYTE lpDIBBits = pImage->GetBits();
	if (m_lpDIBBits == NULL)
	{
		// 将初始数据读入变量中
		m_lpDIBBits = new BYTE [lLineWidth * lHeight];
		memcpy(m_lpDIBBits, lpDIBBits, lLineWidth * lHeight);
	}
	else
		memcpy(lpDIBBits, m_lpDIBBits, lLineWidth * lHeight);

	// 灰度映射表
	BYTE	bMap[256];

	// 计算灰度映射表
	for (i = 0; i <= bX1; i++)	//[0 —— X1]
	{
		// 判断bX1是否大于0（防止分母为0）
		if (bX1 > 0)
		{
			// 线性变换
			bMap[i] = (BYTE) bY1 * i / bX1;
		}
		else
		{
			// 直接赋值为0
			bMap[i] = 0;
		}
	}
	for (; i <= bX2; i++)		//(X1 —— X2]
	{
		// 判断bX1是否等于bX2（防止分母为0）
		if (bX2 != bX1)
		{
			// 线性变换
			bMap[i] = bY1 + (BYTE) ((bY2 - bY1) * (i - bX1) / (bX2 - bX1));
		}
		else
		{
			// 直接赋值为bY1
			bMap[i] = bY1;
		}
	}
	for (; i < 256; i++)		//(X2 —— 256)
	{
		// 判断bX2是否等于255（防止分母为0）
		if (bX2 != 255)
		{
			// 线性变换
			bMap[i] = bY2 + (BYTE) ((255 - bY2) * (i - bX2) / (255 - bX2));
		}
		else
		{
			// 直接赋值为255
			bMap[i] = 255;
		}
	}
	
	// 对各像素进行灰度转换
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lWidth; j ++)
		{
			unsigned char R = *((unsigned char *)lpDIBBits + lLineWidth * i + j * 3 + 0);
			*((unsigned char *)lpDIBBits + lLineWidth * i + j * 3 + 0) = bMap[R];

			unsigned char G = *((unsigned char *)lpDIBBits + lLineWidth * i + j * 3 + 1);
			*((unsigned char *)lpDIBBits + lLineWidth * i + j * 3 + 1) = bMap[G];

			unsigned char B = *((unsigned char *)lpDIBBits + lLineWidth * i + j * 3 + 2);
			*((unsigned char *)lpDIBBits + lLineWidth * i + j * 3 + 2) = bMap[B];
		}
	}

	// 更新视图数据
	GetParent()->Invalidate(TRUE);

	// 恢复光标
	AfxGetApp()->EndWaitCursor();
}

void CDlgPointStre::OnCancel() 
{
	// 删除原来处理过的图像，用未处理前的图像
	CTilCanvas* pCanvas = GetCanvasMgr()->GetCurCanvas();
	CImage* pImage = pCanvas->GetImage()->GetCurImage();
	pImage->Copy(*m_pImage);
	m_pDoc->UpdateAllViews(NULL);

	CDialog::OnCancel();
}

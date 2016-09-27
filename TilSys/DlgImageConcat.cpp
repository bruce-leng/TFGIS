// DlgImageConcat.cpp: implementation of the DlgImageConcat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tilsys.h"
#include "DlgImageConcat.h"
#include <float.h>
#include "MathComput.h"

//// 设定目标点提取的限定域值
	// 设外接矩形长、宽比在[0.5,2.0]之间
#define RATIO_MIN 0.5
#define RATIO_MAX 2
	// 设目标面积最大值和最小值
#define AREA_MIN 20 
#define AREA_MAX 500
	// 设十字线标记半径为4
#define RAIDUS 4
//////////////////////////////////////////////////////////////////////////
CString g_strPath[N];	// 图像路径
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CWizard, CPropertySheet)

CWizard::CWizard(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CWizard::CWizard(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

CWizard::~CWizard()
{
}

BEGIN_MESSAGE_MAP(CWizard, CPropertySheet)
//{{AFX_MSG_MAP(CWizard)
// NOTE - the ClassWizard will add and remove mapping macros here.
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(Step1, CPropertyPage)

Step1::Step1() : CPropertyPage(Step1::IDD)
{
	//{{AFX_DATA_INIT(Step1)
	m_strMethods = _T("");
	for (int i=0; i<N; i++)
	{
		g_strPath[i] = _T("");
		m_ImagePts[i].x = m_ImagePts[i].y = 0.0;
		m_GroundPts[i].x = m_GroundPts[i].y = m_GroundPts[i].z = 0.0;
	}
	//}}AFX_DATA_INIT
	m_pImages = NULL;
}

Step1::~Step1()
{
}

void Step1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Step1)
	DDX_Control(pDX, IDC_COMBO1, m_cmbxMethods);
	DDX_CBString(pDX, IDC_COMBO1, m_strMethods);
	DDX_Control(pDX, IDC_JOB_METHOD, m_stcJobMethod);
	DDX_Control(pDX, IDC_IMAGE6, m_stcImages[5]);
	DDX_Control(pDX, IDC_IMAGE5, m_stcImages[4]);
	DDX_Control(pDX, IDC_IMAGE4, m_stcImages[3]);
	DDX_Control(pDX, IDC_IMAGE3, m_stcImages[2]);
	DDX_Control(pDX, IDC_IMAGE2, m_stcImages[1]);
	DDX_Control(pDX, IDC_IMAGE1, m_stcImages[0]);
	DDX_Text(pDX, IDC_EDIT_PATH1, g_strPath[0]);
	DDX_Text(pDX, IDC_EDIT_PATH2, g_strPath[1]);
	DDX_Text(pDX, IDC_EDIT_PATH3, g_strPath[2]);
	DDX_Text(pDX, IDC_EDIT_PATH4, g_strPath[3]);
	DDX_Text(pDX, IDC_EDIT_PATH5, g_strPath[4]);
	DDX_Text(pDX, IDC_EDIT_PATH6, g_strPath[5]);
	DDX_Text(pDX, IDC_IMAGE_X1, m_ImagePts[0].x);
	DDX_Text(pDX, IDC_IMAGE_X2, m_ImagePts[1].x);
	DDX_Text(pDX, IDC_IMAGE_X3, m_ImagePts[2].x);
	DDX_Text(pDX, IDC_IMAGE_X4, m_ImagePts[3].x);
	DDX_Text(pDX, IDC_IMAGE_X5, m_ImagePts[4].x);
	DDX_Text(pDX, IDC_IMAGE_X6, m_ImagePts[5].x);
	DDX_Text(pDX, IDC_IMAGE_Y1, m_ImagePts[0].y);
	DDX_Text(pDX, IDC_IMAGE_Y2, m_ImagePts[1].y);
	DDX_Text(pDX, IDC_IMAGE_Y3, m_ImagePts[2].y);
	DDX_Text(pDX, IDC_IMAGE_Y4, m_ImagePts[3].y);
	DDX_Text(pDX, IDC_IMAGE_Y5, m_ImagePts[4].y);
	DDX_Text(pDX, IDC_IMAGE_Y6, m_ImagePts[5].y);

	DDX_Text(pDX, IDC_GRND_X1, m_GroundPts[0].x);
	DDX_Text(pDX, IDC_GRND_Y1, m_GroundPts[0].y);
	DDX_Text(pDX, IDC_GRND_Z1, m_GroundPts[0].z);
	DDX_Text(pDX, IDC_GRND_X2, m_GroundPts[1].x);
	DDX_Text(pDX, IDC_GRND_Y2, m_GroundPts[1].y);
	DDX_Text(pDX, IDC_GRND_Z2, m_GroundPts[1].z);
	DDX_Text(pDX, IDC_GRND_X3, m_GroundPts[2].x);
	DDX_Text(pDX, IDC_GRND_Y3, m_GroundPts[2].y);
	DDX_Text(pDX, IDC_GRND_Z3, m_GroundPts[2].z);
	DDX_Text(pDX, IDC_GRND_X4, m_GroundPts[3].x);
	DDX_Text(pDX, IDC_GRND_Y4, m_GroundPts[3].y);
	DDX_Text(pDX, IDC_GRND_Z4, m_GroundPts[3].z);
	DDX_Text(pDX, IDC_GRND_X5, m_GroundPts[4].x);
	DDX_Text(pDX, IDC_GRND_Y5, m_GroundPts[4].y);
	DDX_Text(pDX, IDC_GRND_Z5, m_GroundPts[4].z);
	DDX_Text(pDX, IDC_GRND_X6, m_GroundPts[5].x);
	DDX_Text(pDX, IDC_GRND_Y6, m_GroundPts[5].y);
	DDX_Text(pDX, IDC_GRND_Z6, m_GroundPts[5].z);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(Step1, CPropertyPage)
//{{AFX_MSG_MAP(Step1)
ON_BN_CLICKED(IDC_LOAD1, OnLoad1)
ON_BN_CLICKED(IDC_LOAD2, OnLoad2)
ON_BN_CLICKED(IDC_LOAD3, OnLoad3)
ON_BN_CLICKED(IDC_LOAD4, OnLoad4)
ON_BN_CLICKED(IDC_LOAD5, OnLoad5)
ON_BN_CLICKED(IDC_LOAD6, OnLoad6)
ON_BN_CLICKED(IDC_RECOGNITION, OnRecognition)
ON_WM_PAINT()
ON_WM_LBUTTONUP()
ON_CBN_DROPDOWN(IDC_COMBO1, OnDropdownCmbxMethods)
ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCmbxMethods)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Step1 message handlers

BOOL Step1::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	//m_cmbxMethods.SetDroppedWidth()

	return TRUE;
}

void Step1::OnDropdownCmbxMethods()
{
	// 设置组合框下拉列表的高度
	CRect rcStatic, rcDrop;
	m_cmbxMethods.GetClientRect(&rcStatic);
    m_cmbxMethods.GetDroppedControlRect(&rcDrop);
    m_cmbxMethods.GetParent()->ScreenToClient(&rcDrop);
    rcDrop.bottom = rcDrop.top+rcStatic.Height()*6;
    m_cmbxMethods.MoveWindow(&rcDrop);
}

void Step1::OnSelchangeCmbxMethods()
{
	CBitmap bitmap;

	switch (m_cmbxMethods.GetCurSel())
	{
	case TWO_STEP:
		bitmap.LoadBitmap(IDB_2_STEP);
		break;
	case THREE_STEP:
		bitmap.LoadBitmap(IDB_3_STEP);
		break;
	case CD:
		break;
	case CRD:
		bitmap.LoadBitmap(IDB_CRD);
		break;
	default:
		;
	}

	// 加载并显示施工方法图
	m_JobMethod.CreateFromHBITMAP((HBITMAP)bitmap.GetSafeHandle());
	m_stcJobMethod.Invalidate(FALSE);
}

BOOL Step1::OnSetActive()
{
	// 将父类的图像成员变量赋给当前图像指针
	CPropertySheet* pParent=(CPropertySheet*)GetParent();
	m_pImages = ((CWizard*)pParent)->m_Images;

	pParent->SetWizardButtons(PSWIZB_NEXT);
	return CPropertyPage::OnSetActive();
}

BOOL Step1::OnKillActive()
{
	// 将图像点坐标赋给父类
	UpdateData(TRUE);

	CPropertySheet* pParent=(CPropertySheet*)GetParent();
	POINT2DF* pImagePts = ((CWizard*)pParent)->m_ImagePts;
	POINT3DF* pGroundPts = ((CWizard*)pParent)->m_GroundPts;
	for (int i=0; i<N; i++)
	{
		pImagePts[i] = m_ImagePts[i];
		pGroundPts[i] = m_GroundPts[i];
	}

	return CPropertyPage::OnKillActive();
}

void Step1::OnLoad1() 
{
	// TODO: Add your control notification handler code here
	static char BASED_CODE szFilter[] = "supported files|*.jpeg;*.jpg;*.bmp|Jpeg Files (*.jpeg,*.jpg)|*.jpeg;*.jpg|Bmp Files (*.bmp)|*.bmp|All Files (*.*)|*.*||";
	CFileDialog dlg(TRUE, "*.*", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_FILEMUSTEXIST, szFilter, NULL);
	if (dlg.DoModal() == IDOK)
	{
		// 显示图像载入路径
		g_strPath[0] = dlg.GetPathName();
		UpdateData(FALSE);
		
		//　载入并显示图像
		m_pImages[0].Load(g_strPath[0]);
		Invalidate(FALSE);
	}
}

void Step1::OnLoad2() 
{
	// TODO: Add your control notification handler code here
	static char BASED_CODE szFilter[] = "supported files|*.jpeg;*.jpg;*.bmp|Jpeg Files (*.jpeg,*.jpg)|*.jpeg;*.jpg|Bmp Files (*.bmp)|*.bmp|All Files (*.*)|*.*||";
	CFileDialog dlg(TRUE, "*.*", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_FILEMUSTEXIST, szFilter, NULL);
	if (dlg.DoModal() == IDOK)
	{
		// 显示图像载入路径
		g_strPath[1] = dlg.GetPathName();
		UpdateData(FALSE);
		
		//　载入并显示图像
		m_pImages[1].Load(g_strPath[1]);
		Invalidate(FALSE);
	}
}

void Step1::OnLoad3() 
{
	// TODO: Add your control notification handler code here
	static char BASED_CODE szFilter[] = "supported files|*.jpeg;*.jpg;*.bmp|Jpeg Files (*.jpeg,*.jpg)|*.jpeg;*.jpg|Bmp Files (*.bmp)|*.bmp|All Files (*.*)|*.*||";
	CFileDialog dlg(TRUE, "*.*", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_FILEMUSTEXIST, szFilter, NULL);
	if (dlg.DoModal() == IDOK)
	{
		// 显示图像载入路径
		g_strPath[2] = dlg.GetPathName();
		UpdateData(FALSE);
		
		//　载入并显示图像
		m_pImages[2].Load(g_strPath[2]);
		Invalidate(FALSE);
	}
}

void Step1::OnLoad4() 
{
	// TODO: Add your control notification handler code here
	static char BASED_CODE szFilter[] = "supported files|*.jpeg;*.jpg;*.bmp|Jpeg Files (*.jpeg,*.jpg)|*.jpeg;*.jpg|Bmp Files (*.bmp)|*.bmp|All Files (*.*)|*.*||";
	CFileDialog dlg(TRUE, "*.*", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_FILEMUSTEXIST, szFilter, NULL);
	if (dlg.DoModal() == IDOK)
	{
		// 显示图像载入路径
		g_strPath[3] = dlg.GetPathName();
		UpdateData(FALSE);
		
		//　载入并显示图像
		m_pImages[3].Load(g_strPath[3]);
		Invalidate(FALSE);
	}
}

void Step1::OnLoad5() 
{
	// TODO: Add your control notification handler code here
	static char BASED_CODE szFilter[] = "supported files|*.jpeg;*.jpg;*.bmp|Jpeg Files (*.jpeg,*.jpg)|*.jpeg;*.jpg|Bmp Files (*.bmp)|*.bmp|All Files (*.*)|*.*||";
	CFileDialog dlg(TRUE, "*.*", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_FILEMUSTEXIST, szFilter, NULL);
	if (dlg.DoModal() == IDOK)
	{
		// 显示图像载入路径
		g_strPath[4] = dlg.GetPathName();
		UpdateData(FALSE);
		
		//　载入并显示图像
		m_pImages[4].Load(g_strPath[4]);
		Invalidate(FALSE);
	}
}

void Step1::OnLoad6() 
{
	// TODO: Add your control notification handler code here
	static char BASED_CODE szFilter[] = "supported files|*.jpeg;*.jpg;*.bmp|Jpeg Files (*.jpeg,*.jpg)|*.jpeg;*.jpg|Bmp Files (*.bmp)|*.bmp|All Files (*.*)|*.*||";
	CFileDialog dlg(TRUE, "*.*", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_FILEMUSTEXIST, szFilter, NULL);
	if (dlg.DoModal() == IDOK)
	{
		// 显示图像载入路径
		g_strPath[5] = dlg.GetPathName();
		UpdateData(FALSE);
		
		//　载入并显示图像
		m_pImages[5].Load(g_strPath[5]);
		Invalidate(FALSE);
	}
}

void Step1::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rect;

	// 显示图像
	for (int i=0; i<N; i++)
	{
		m_stcImages[i].GetClientRect(&rect);
		((CxImage)m_pImages[i]).Draw(m_stcImages[i].GetDC()->GetSafeHdc(), rect);
		
		DrawCrossLine(m_stcImages+i, m_CntrlPts[i]);
	}

	// 显示施工方法图
	m_stcJobMethod.GetClientRect(&rect);
	((CxImage)m_JobMethod).Draw(m_stcJobMethod.GetDC()->GetSafeHdc(), rect);
	// Do not call CPropertyPage::OnPaint() for painting messages
}

void Step1::OnLButtonUp(UINT nFlags, CPoint point)
{
	// 取得每幅图像显示区域所在对话框中的外接矩形
	if (m_Rects[0].IsRectEmpty())
	{
		for (int i=0; i<N; i++)
		{
			m_stcImages[i].GetClientRect(m_Rects+i);
			m_stcImages[i].ClientToScreen(m_Rects+i);
			this->ScreenToClient(m_Rects+i);
		}
	}

	// 查看点在哪一个图像显示控件区域内
	for (int i=0; i<N; i++)
	{
		if (m_Rects[i].PtInRect(point) && m_pImages[i].IsValid())
		{
			ClientToScreen(&point);
			m_stcImages[i].ScreenToClient(&point);

			m_CntrlPts[i].x = point.x;
			m_CntrlPts[i].y = point.y;

			// 反算控件中的点在图像中的点的坐标
			m_ImagePts[i].x = m_CntrlPts[i].x*m_pImages[i].GetWidth()*1.0/m_Rects[i].Width();
			m_ImagePts[i].y = m_CntrlPts[i].y*m_pImages[i].GetHeight()*1.0/m_Rects[i].Height();

			UpdateData(FALSE);
			Invalidate(FALSE);
			break;
		}
	}
}

// 自动识别每幅图像中的标定点
void Step1::OnRecognition()
{
	// 取得每幅图像显示区域所在对话框中的外接矩形
	if (m_Rects[0].IsRectEmpty())
	{
		for (int i=0; i<N; i++)
		{
			m_stcImages[i].GetClientRect(m_Rects+i);
			m_stcImages[i].ClientToScreen(m_Rects+i);
			this->ScreenToClient(m_Rects+i);
		}
	}

	for (int i=0; i<N; i++)
	{
		if (!m_pImages[i].IsValid())
			continue;

		// 用最佳阈值对图像二值化
		CImage image;
		image.Copy(m_pImages[i]);
		int nThreshold = image.GetBestThreshold();
		image.Threshold(nThreshold);
//		image.Save("D:\\1.BMP", CXIMAGE_FORMAT_BMP);

		// 获取控制点并在图中作上标记
		POINT2DF point;
		ObjPtAutoExtract(&image, m_ImagePts[i]);
//		TRACE("ImagePts[%d](%5.2f,%5.2f)\n", i, m_ImagePts[i].x, m_ImagePts[i].y);

		// 由于控制点坐标是以图像左下角为原点的，故需转换成以图像左上角为原点的坐标
		m_ImagePts[i].y = image.GetHeight()-m_ImagePts[i].y-1;

		// 将图像坐标转换成图像显示控件中的坐标
//		int nWidth = image.GetWidth();
//		int nHeight = image.GetHeight();
		m_CntrlPts[i].x = m_ImagePts[i].x*m_Rects[i].Width()*1.0/image.GetWidth();
		m_CntrlPts[i].y = m_ImagePts[i].y*m_Rects[i].Height()*1.0/image.GetHeight();

		// 更新画红色十字标定的区域
		CPoint ptTmp(m_CntrlPts[i].x+m_Rects[i].left, m_CntrlPts[i].y+m_Rects[i].top);
		CRect rtTmp(ptTmp.x-RAIDUS, ptTmp.y-RAIDUS, ptTmp.x+RAIDUS, ptTmp.y+RAIDUS);
		InvalidateRect(rtTmp, FALSE);

		// 保留两位小数
		m_ImagePts[i].x = int(m_ImagePts[i].x*100)/100.0;
		m_ImagePts[i].y = int(m_ImagePts[i].y*100)/100.0;
	}

	// 将标记点坐标显示在编辑控件中
	UpdateData(FALSE);
}

int Step1::ObjPtAutoExtract(CImage *pBinImg, POINT2DF &point)
{
	int i, j;

	// 取得二值图像的游程编码
	ObjRunLenCode tmpRLC;
	pBinImg->ObjAllRLC(&tmpRLC, 255);

	// 如果没有目标,则退出
	int nObjNum = tmpRLC.GetObjNum();
	if (nObjNum == 0)
		return FALSE;

	PPRUNLENGTHCODE_MGR ppRLCMgr = tmpRLC.GetObjAll();

	// 求所有目标的面积(像素数)及外接矩形
	int* pObjArea = new int [nObjNum];
	memset(pObjArea, 0, sizeof(int)*nObjNum);
	CRect* pObjRect = new CRect [nObjNum];
	for (i=0; i<nObjNum; i++)
	{
		// 面积累加
		for (j=0; j<ppRLCMgr[i]->nRunNum; j++)
		{
			pObjArea[i] += ppRLCMgr[i]->RC[j].xEnd-ppRLCMgr[i]->RC[j].xStart+1;
		}

		// 外接矩形
		tmpRLC.ObjRect(ppRLCMgr[i], pObjRect[i]);
	}

	// 根据面积和外接矩形查找目标控制点
	// 单独存储满足条件的目标
	vector< int > vecIdx;
	vector< int > vecArea;
	vector< PRUNLENGTHCODE_MGR > vecRLC;
	vector< float > vecRatio;
	for (i=0; i<nObjNum; i++)
	{
		float fRatio;
		fRatio = pObjRect[i].Width()*1.0/pObjRect[i].Height();

		// 将长宽比、面积满足条件的目标压入堆栈
		if (fRatio>=RATIO_MIN && fRatio<=RATIO_MAX)
		{
			// 对面积满足条件的目标进行标记
			if (pObjArea[i]>=AREA_MIN && pObjArea[i]<=AREA_MAX)
			{
				// 记录满足条件点的序号、面积、游程、长宽比
				vecIdx.push_back(i);
				vecArea.push_back(pObjArea[i]);
				vecRLC.push_back(ppRLCMgr[i]);
				vecRatio.push_back(fRatio);
			}
		}
	}

	// 查找长宽比最接近1的目标
	int nIdx = -1;
	float fMin = FLT_MAX;
	for (i=0; i<vecRatio.size(); i++)
	{
		float fTmp = fabs(vecRatio[i]-1);
		if (fMin > fTmp)
		{
			fMin = fTmp;
			nIdx = i;
		}
	}

	delete [] pObjRect;
	delete [] pObjArea;

	// 获得目标重心坐标
	if (nIdx >= 0)
	{
		tmpRLC.ObjCenter(vecRLC[nIdx], point);
		return TRUE;
	}
	else
		return FALSE;
}

// 画出以控制点为中心的红十字形
void Step1::DrawCrossLine(CStatic* pWnd, POINT2DF& point)
{
	// 以控制点为中心设置十字线标记
	CPen pen(PS_SOLID, 1, RGB(255, 0, 0));
	CDC* pDC = pWnd->GetDC();
	CPen* pOldPen = pDC->SelectObject(&pen);
	CRect rect;
	pWnd->GetClientRect(&rect);

	// 十字线显示范围
	RECT rtTmp;
	rtTmp.left = (point.x-RAIDUS>0) ? point.x-RAIDUS : 0;
	rtTmp.right = (point.x+RAIDUS<rect.Width()) ? point.x+RAIDUS : rect.Width();
	rtTmp.top = (point.y-RAIDUS>0) ? point.y-RAIDUS : 0;
	rtTmp.bottom = (point.y+RAIDUS<rect.Height()) ? point.y+RAIDUS : rect.Height();

	pDC->MoveTo(point.x, rtTmp.top);
	pDC->LineTo(point.x, rtTmp.bottom+1);
	pDC->MoveTo(rtTmp.left, point.y);
	pDC->LineTo(rtTmp.right+1, point.y);

	pDC->SelectObject(pOldPen);
}

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(Step2, CPropertyPage)

Step2::Step2() : CPropertyPage(Step2::IDD)
{
	//{{AFX_DATA_INIT(Step2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pImages = NULL;
	m_nDraw = 0;

	m_hTotalBitmap = NULL;
	m_hOldTotalBitmap = NULL;
	m_hTotalDC = NULL;
}

Step2::~Step2()
{
	if (m_hTotalDC)
	{
		::DeleteDC(m_hTotalDC);
		::DeleteObject(::SelectObject(m_hTotalDC, m_hOldTotalBitmap));
	}
}

void Step2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Step2)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_IMAGE, m_stcImage);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(Step2, CPropertyPage)
//{{AFX_MSG_MAP(Step2)
ON_WM_PAINT()
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_IMAGE_CUT, OnImageCut)
	ON_BN_CLICKED(IDC_IMAGE_SAVE, OnImageSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Step2 message handlers

BOOL Step2::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL Step2::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	CPropertySheet* pParent=(CPropertySheet*)GetParent();
	m_pImages = ((CWizard*)pParent)->m_Images;

	// 得到每幅图像相对于大地坐标的外接矩形坐标区域
	POINT2DF* pImagePts = ((CWizard*)pParent)->m_ImagePts;
	POINT3DF* pGroundPts = ((CWizard*)pParent)->m_GroundPts;
	for (int i=0; i<2; i++)
	{
		m_rtGround[i] = get_image_rect(m_pImages+i, pImagePts[i], pGroundPts[i]);
	}

	// 统计几幅图像相对于大地坐标的外接矩形
	m_rtTotal.left = m_rtTotal.top = INT_MAX;
	m_rtTotal.right = m_rtTotal.bottom = INT_MIN;
	for (i=0; i<2; i++)
	{
		m_rtTotal.left = (m_rtTotal.left<m_rtGround[i].left) ? m_rtTotal.left : m_rtGround[i].left;
		m_rtTotal.top = (m_rtTotal.top<m_rtGround[i].top) ? m_rtTotal.top : m_rtGround[i].top;
		m_rtTotal.right = (m_rtTotal.right>m_rtGround[i].right) ? m_rtTotal.right : m_rtGround[i].right;
		m_rtTotal.bottom = (m_rtTotal.bottom>m_rtGround[i].bottom) ? m_rtTotal.bottom : m_rtGround[i].bottom;
	}

	// 拼接图像
	if (!m_hTotalDC)
	{
		m_hTotalBitmap = ::CreateCompatibleBitmap(m_stcImage.GetDC()->GetSafeHdc(),
											m_rtTotal.Width(), m_rtTotal.Height());
		m_hTotalDC = ::CreateCompatibleDC(NULL);
		m_hOldTotalBitmap = (HBITMAP)::SelectObject(m_hTotalDC, m_hTotalBitmap);
	}

	for (i=0; i<N; i++)
	{
		if (!m_pImages[i].IsValid())
			continue;

		HBITMAP hBitmap;
		HDC hMemDC;
		HBITMAP hOldBitmap;

		hBitmap = ::CreateCompatibleBitmap(m_hTotalDC, m_rtGround[i].Width(), m_rtGround[i].Height());
		hMemDC = ::CreateCompatibleDC(m_hTotalDC);
		hOldBitmap = (HBITMAP)::SelectObject(hMemDC, hBitmap);

		// 将图像在内存映像中画出
		CRect rtSrc(0, 0, m_rtGround[i].Width(), m_rtGround[i].Height());
		((CxImage)m_pImages[i]).Draw(hMemDC, rtSrc);

		// 以几幅图像总的外接矩形的左上角为原点，向下和向右为正，计算几幅图像新的外接矩形区域
		m_rtRect[i].left = m_rtGround[i].left-m_rtTotal.left;
		m_rtRect[i].right = m_rtGround[i].right-m_rtTotal.left;
		m_rtRect[i].top = m_rtGround[i].top-m_rtTotal.top;
		m_rtRect[i].bottom = m_rtGround[i].bottom-m_rtTotal.top;

		// 将几幅内存中的图像在新的内存映像中画出
		::BitBlt(m_hTotalDC,m_rtRect[i].left, m_rtRect[i].top, m_rtRect[i].Width(), m_rtRect[i].Height(),
						hMemDC, 0, 0, SRCPAINT);
		::DeleteObject(::SelectObject(hMemDC, hOldBitmap));
		::DeleteDC(hMemDC);
	}

	// 计算整幅图像的显示比例
	CRect rtImage;
	m_stcImage.GetClientRect(rtImage);
	if (rtImage.Width()*1.0/m_rtTotal.Width() < rtImage.Height()*1.0/m_rtTotal.Height())
		m_fRatio = rtImage.Width()*1.0/m_rtTotal.Width();
	else
		m_fRatio = rtImage.Height()*1.0/m_rtTotal.Height();

	pParent->SetWizardButtons(PSWIZB_NEXT|PSWIZB_BACK);
	return CPropertyPage::OnSetActive();
}

void Step2::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	int i,j;

	// 将内存中的图像经过缩放后显示出来
	HDC hDC = m_stcImage.GetDC()->GetSafeHdc();
	::StretchBlt(hDC, 0, 0, m_rtTotal.Width()*m_fRatio, m_rtTotal.Height()*m_fRatio, m_hTotalDC,
						0, 0, m_rtTotal.Width(), m_rtTotal.Height(), SRCCOPY);

	// 显示画出的多段线
	HPEN hPen = ::CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	HPEN hOldPen = (HPEN)::SelectObject(dc.GetSafeHdc(), hPen);
	for (i=0; i<m_vvPoints.size(); i++)
	{
		for (j=0; j<m_vvPoints[i].size()-1; j++)
		{
			CPoint pt1 = m_vvPoints[i][j];
			CPoint pt2 = m_vvPoints[i][j+1];

			MoveToEx(dc.GetSafeHdc(), pt1.x, pt1.y, NULL);
			LineTo(dc.GetSafeHdc(), pt2.x, pt2.y);
		}
	}
	
	::DeleteDC(hDC);

}

void Step2::OnLButtonUp(UINT nFlags, CPoint point)
{
	CDC* pDC = GetDC();
	CPen pen(PS_SOLID, 1, RGB(255, 0, 0));
	CPen* pOldPen = pDC->SelectObject(&pen);

	// 取得图像显示框在对话框中的外接矩形坐标
	CRect rect;
	m_stcImage.GetClientRect(&rect);
	m_stcImage.ClientToScreen(&rect);
	ScreenToClient(&rect);

//	if (rect.PtInRect(point))
	{
		if (m_nDraw == -1)	// 鼠标双击时
			m_nDraw = 0;
		else if (m_nDraw == 0)	// 鼠标双击后第一次单击时
		{
			// 第一次单击时保存两个点的坐标便于鼠标移动画线
			vector< POINT > vPoints;
			vPoints.push_back(point);
			vPoints.push_back(point);

			m_vvPoints.push_back(vPoints);
			m_nDraw = 1;
		}
		else	// 鼠标第一次单击后
			m_vvPoints.back().push_back(point);
	}

	pDC->SelectObject(pOldPen);
}

void Step2::OnMouseMove(UINT nFlags, CPoint point)
{
	CDC* pDC = GetDC();
	CPen pen(PS_SOLID, 1, RGB(255, 0, 0));
	CPen* pOldPen = pDC->SelectObject(&pen);
	pDC->SetROP2(R2_NOTXORPEN);

	// 鼠标第一次单击后
	if (m_nDraw == 1)
	{
		POINT pt1, pt2;
		pt2 = m_vvPoints.back().back();
		m_vvPoints.back().pop_back();
		pt1 = m_vvPoints.back().back();

		// 删除原来的线段
		pDC->MoveTo(pt1);
		pDC->LineTo(pt2);

		// 增加新的线段
		pDC->MoveTo(pt1);
		pDC->LineTo(point);

		m_vvPoints.back().push_back(point);
	}

	pDC->SelectObject(pOldPen);
}

void Step2::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	m_nDraw = -1;
}

// 图像剪切
void Step2::OnImageCut() 
{
	// TODO: Add your control notification handler code here
	int i,j,k;
	CClientDC dc(this);

	// 清除原显示图像
	::DeleteObject(::SelectObject(m_hTotalDC, m_hOldTotalBitmap));
	m_hTotalBitmap = ::CreateCompatibleBitmap(m_stcImage.GetDC()->GetSafeHdc(), m_rtTotal.Width(), m_rtTotal.Height());
	m_hOldTotalBitmap = (HBITMAP)::SelectObject(m_hTotalDC, m_hTotalBitmap);

	// 计算多段线在原始图像的拼接图像中的坐标
	VVECPOINT vvPoints;
	for (j=0; j<m_vvPoints.size(); j++)
	{
		vector< POINT > vPoints;
		for (k=0; k<m_vvPoints[j].size(); k++)
		{
			POINT pt = m_vvPoints[j][k];
			ClientToScreen(&pt);
			m_stcImage.ScreenToClient(&pt);

			pt.x = pt.x/m_fRatio;
			pt.y = pt.y/m_fRatio;
			vPoints.push_back(pt);
		}
		vvPoints.push_back(vPoints);
	}

	// 计算原始图像与多段线的交集图像，将图像作为最终的图像的一部分
	for (i=0; i<N; i++)
	{
		if (!m_pImages[i].IsValid())
			return;

		// 建立一幅图像，并设前景色为255
		HBITMAP hBitmap;
		hBitmap = ::CreateCompatibleBitmap(dc.GetSafeHdc(), m_rtRect[i].Width(), m_rtRect[i].Height());
		BITMAP bmp;
		CBitmap* pBitmap = CBitmap::FromHandle(hBitmap);
		pBitmap->GetBitmap(&bmp);
		long lSize = bmp.bmHeight*bmp.bmWidthBytes;
		LPBYTE pBits = new BYTE [lSize];
		memset(pBits, 255, lSize);
		::SetBitmapBits(hBitmap, lSize, pBits);

		HDC hDC = ::CreateCompatibleDC(dc.GetSafeHdc());
		HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hDC, hBitmap);

		// 在image中画宽度为3的线段
		HPEN hPen = ::CreatePen(PS_SOLID, 2, RGB(0,0,0));
		HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);

		// 在新图像中画线段
		for (j=0; j<vvPoints.size(); j++)
		{
			for (k=0; k<vvPoints[j].size()-1; k++)
			{
				POINT pt1,pt2;
				pt1 = vvPoints[j][k];
				pt1.x -= m_rtRect[i].left;		// 转换为以左上角为原点的坐标
				pt1.y -= m_rtRect[i].top;
				pt1.y = m_rtRect[i].Height()-pt1.y-1;	// 转换为以左下角为原点的坐标

				pt2 = vvPoints[j][k+1];
				pt2.x -= m_rtRect[i].left;
				pt2.y -= m_rtRect[i].top;
				pt2.y = m_rtRect[i].Height()-pt2.y-1;

				::MoveToEx(hDC, pt1.x , pt1.y, NULL);
				::LineTo(hDC, pt2.x, pt2.y);

//				TRACE("N=%d:pt1(%d,%d), pt2(%d,%d)\n", i, pt1.x, pt1.y, pt2.x, pt2.y);
			}
		}

		::GetBitmapBits(hBitmap, lSize, pBits);
		CImage image;
		image.CreateFromArray(pBits, m_rtRect[i].Width(), m_rtRect[i].Height(),
													bmp.bmBitsPixel, bmp.bmWidthBytes, false);
		CString str;
		str.Format("D:\\Test\\%d.bmp", i);
		image.Save(str, CXIMAGE_FORMAT_BMP);

		// 查找所有目标的外接矩形
		ObjRunLenCode objAllRLC;
		image.ObjAllRLC(&objAllRLC, 255);

		// 求面积最大的目标，并保存
		long lArea, lIdx, lNum;
		lArea = 0;
		lNum = objAllRLC.GetObjNum();
		for (j=0; j<lNum; j++)
		{
			if (lArea < objAllRLC.ObjArea(j))
			{
				lArea = objAllRLC.ObjArea(j);
				lIdx = j;
			}
		}

		// 测试
		CxImage test(m_rtRect[i].Width(), m_rtRect[i].Height(), 24);
		test.Clear(0);
		
		// 将原始图像最大目标区域图像数据放到最后的拼接图像中
		PRUNLENGTHCODE_MGR pMgr = objAllRLC.GetObj(lIdx);
		for (j=0; j<pMgr->nRunNum; j++)
		{
			int y, xStart, xEnd;
			y = pMgr->RC[j].y;
			xStart = pMgr->RC[j].xStart;
			xEnd = pMgr->RC[j].xEnd;

			for (int x=xStart; x<=xEnd; x++)
			{
				int nX, nY;
				nX = x;
				nY = y;

				test.SetPixelColor(nX,nY,RGB(255,255,255));

				RGBQUAD rgb = m_pImages[i].GetPixelColor(nX,nY);
				nY = m_rtRect[i].Height()-nY-1;	// 转换为以当前图像左上角为原点的坐标
				nY += m_rtRect[i].top;			// 转换为以拼接图像的左上角为原点的坐标
				nX += m_rtRect[i].left;
//				nY = m_rtTotal.Height()-nY-1;		// 转换为以拼接图像左下角为原点的坐标
				::SetPixel(m_hTotalDC, nX, nY, RGB(rgb.rgbRed, rgb.rgbGreen, rgb.rgbBlue));
			}
		}

		// 测试
		str.Format("D:\\Test\\result-%d.bmp", i);
//		test.CreateFromHBITMAP(m_hTotalBitmap);
		test.Save(str, CXIMAGE_FORMAT_BMP);

		delete [] pBits;
	}

	Invalidate(TRUE);
}
//
//// 计算原始图像与多段线的交集图像，将图像作为最终的图像的一部分
//CRgn* Step2::get_inter_section(const CRect& rtImage, VVECPOINT vLines)
//{
//	int i, j, k;
//	VINTERPOINT vInterPts;
//	VVECPOINT vvPoly;
//
//	// 对矩形的四条边依次进行分析
//	for (i=0; i<4; i++)
//	{
//		POINT ptA1, ptA2;
//		switch (i)
//		{
//		case 0:	// 记录左边两端点
//			ptA1 = rtImage.TopLeft();
//			ptA2.x = rtImage.left;
//			ptA2.y = rtImage.bottom;
//			break;
//		case 1:	// 记录下边两端点
//			ptA1.x = rtImage.left;
//			ptA1.y = rtImage.bottom;
//			ptA2 = rtImage.BottomRight();
//			break;
//		case 2:	// 记录右边两端点
//			ptA1 = rtImage.BottomRight();
//			ptA2.x = rtImage.right;
//			ptA2.y = rtImage.top;
//			break;
//		case 3:	// 记录上边两端点
//			ptA1.x = rtImage.right;
//			ptA1.y = rtImage.top;
//			ptA2 = rtImage.TopLeft();
//		}
//
//		// 查找交点
//		POINT interPoint;
//		for (j=0; j<vLines.size(); j++)
//		{
//			for (k=0; k<vLines[i].size()-1; k++)
//			{
//				if (get_inter_point(ptA1, ptA2, vLines[j][k], vLines[j][k+1], interPoint))
//				{
//					INTER_POINT inter;
//					inter.pt = interPoint;
//					inter.nLineIdx = j;
//					inter.nPointIdx = k;
//					vInterPts.push_back(inter);
//				}
//			}
//		}
//
//		// 对交点进行排序
//		// 根据与当前边第一个端点的距离对交点排序
//		float fDist1, fDist2;
//		for (j=0; j<vInterPts.size()-1; j++)
//		{
//			fDist1 = get_distance(ptA1, vInterPts[j].pt);
//			for (k=j+1; k<vInterPts.size(); k++)
//			{
//				fDist2 = get_distance(ptA1, vInterPts[k].pt);
//				if (fDist1 > fDist2)
//				{
//					INTER_POINT tmp;
//					tmp = vInterPts[j];
//					vInterPts[j] = vInterPts[k];
//					vInterPts[k] = tmp;
//
//					fDist1 = fDist2;
//				}
//			}
//		}
//
//		// 对最先获得的交点进行分析
//		for (j=0; j<vInterPts.size()-1; j++)
//		{
//			POINT pt1, pt2;
//			pt1 = vInterPts[j].pt;
//			pt2 = m_vvPoints[ vInterPts[j].nLineIdx ][ vInterPts[j].nPointIdx ];
//			double dbAngle = get_angle(ptA1, pt1, pt2);
//
//			// 求点ptA1以pt1为原点顺时针旋转dbAngel角度后的坐标
//			// 先求出ptA1相对于以pt1为原点的向量
//			float a, b;
//			a = ptA1.x-pt1.x;
//			b = ptA1.y-pt1.y;
//
//			// 求旋转后的点相对于pt1点的向量(a+b*i)*(cosθ+i*sinθ)
//			a = a*cos(dbAngle)-b*sin(dbAngle);
//			b = a*sin(dbAngle)+b*cos(dbAngle);
//
//			// 求旋转后点的坐标
//			a += pt1.x;
//			b += pt1.y;
//
//			// 判断三点是否在同一直线上
//			POINT2DF ptL1(pt1.x, pt1.y), ptL2(pt2.x, pt2.y), ptR(a, b);
//			int nRet = point_in_line(ptL1, ptL2, ptR);
//
//			// 如果ptL2和ptR重合或者ptL1在线段ptL2-ptR的延长线上
//			if ((ptL2.x==ptR.x)&&(ptL2.y==ptR.y) || nRet==POINT_IN_EX_LINE)
//			{
//			}
//		}
//	}
//
//	return NULL;
//}
//
// 图像保存
void Step2::OnImageSave() 
{
	// TODO: Add your control notification handler code here
	
}

/* ==================================================================
 * 函数名称：  Step2::get_image_rect()
 * 作者：      [Leng]
 * 创建于:     [2008-8-10 0:07:14]
 * 返回值：    [CRect&] -
 * 参数列表： 
               [CImage* pImage] - [In]	图像坐标
               [ POINT2DF ptImage] - [In]	图像中标定点相对于图像左上角的坐标
               [ POINT2DF ptGround] - [In]	图像中标定点的大地坐标
注释 :	得到图像相对于大地坐标的外接矩形坐标区域
==================================================================*/
CRect Step2::get_image_rect(CImage* pImage, POINT2DF ptImage, POINT3DF ptGround)
{
	// 计算标记点距各条边界的距离
	CRect rect;
	rect.left = ptImage.x;
	rect.right = pImage->GetWidth()-ptImage.x;
	rect.top = ptImage.y;
	rect.bottom = pImage->GetHeight()-ptImage.y;

	// 计算图像的大地坐标边界
	rect.left = ptGround.x-rect.left;
	rect.right = ptGround.x+rect.right;
	rect.top = ptGround.y-rect.top;
	rect.bottom = ptGround.y+rect.bottom;

	return rect;
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(Step3, CPropertyPage)

Step3::Step3() : CPropertyPage(Step3::IDD)
{
	//{{AFX_DATA_INIT(Step3)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

Step3::~Step3()
{
}

BOOL Step3::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// 对多幅图像进行处理，得到一幅完整的全断面图像

	return TRUE;
}

void Step3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Step3)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(Step3, CPropertyPage)
//{{AFX_MSG_MAP(Step3)
// NOTE: the ClassWizard will add message map macros here
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Step3 message handlers

BOOL Step3::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	CPropertySheet* pParent=(CPropertySheet*)GetParent();
	pParent->SetWizardButtons(PSWIZB_FINISH|PSWIZB_BACK);
	return CPropertyPage::OnSetActive();
}

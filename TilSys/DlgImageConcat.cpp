// DlgImageConcat.cpp: implementation of the DlgImageConcat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tilsys.h"
#include "DlgImageConcat.h"
#include <float.h>
#include "MathComput.h"

//// �趨Ŀ�����ȡ���޶���ֵ
	// ����Ӿ��γ��������[0.5,2.0]֮��
#define RATIO_MIN 0.5
#define RATIO_MAX 2
	// ��Ŀ��������ֵ����Сֵ
#define AREA_MIN 20 
#define AREA_MAX 500
	// ��ʮ���߱�ǰ뾶Ϊ4
#define RAIDUS 4
//////////////////////////////////////////////////////////////////////////
CString g_strPath[N];	// ͼ��·��
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
	// ������Ͽ������б�ĸ߶�
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

	// ���ز���ʾʩ������ͼ
	m_JobMethod.CreateFromHBITMAP((HBITMAP)bitmap.GetSafeHandle());
	m_stcJobMethod.Invalidate(FALSE);
}

BOOL Step1::OnSetActive()
{
	// �������ͼ���Ա����������ǰͼ��ָ��
	CPropertySheet* pParent=(CPropertySheet*)GetParent();
	m_pImages = ((CWizard*)pParent)->m_Images;

	pParent->SetWizardButtons(PSWIZB_NEXT);
	return CPropertyPage::OnSetActive();
}

BOOL Step1::OnKillActive()
{
	// ��ͼ������긳������
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
		// ��ʾͼ������·��
		g_strPath[0] = dlg.GetPathName();
		UpdateData(FALSE);
		
		//�����벢��ʾͼ��
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
		// ��ʾͼ������·��
		g_strPath[1] = dlg.GetPathName();
		UpdateData(FALSE);
		
		//�����벢��ʾͼ��
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
		// ��ʾͼ������·��
		g_strPath[2] = dlg.GetPathName();
		UpdateData(FALSE);
		
		//�����벢��ʾͼ��
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
		// ��ʾͼ������·��
		g_strPath[3] = dlg.GetPathName();
		UpdateData(FALSE);
		
		//�����벢��ʾͼ��
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
		// ��ʾͼ������·��
		g_strPath[4] = dlg.GetPathName();
		UpdateData(FALSE);
		
		//�����벢��ʾͼ��
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
		// ��ʾͼ������·��
		g_strPath[5] = dlg.GetPathName();
		UpdateData(FALSE);
		
		//�����벢��ʾͼ��
		m_pImages[5].Load(g_strPath[5]);
		Invalidate(FALSE);
	}
}

void Step1::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rect;

	// ��ʾͼ��
	for (int i=0; i<N; i++)
	{
		m_stcImages[i].GetClientRect(&rect);
		((CxImage)m_pImages[i]).Draw(m_stcImages[i].GetDC()->GetSafeHdc(), rect);
		
		DrawCrossLine(m_stcImages+i, m_CntrlPts[i]);
	}

	// ��ʾʩ������ͼ
	m_stcJobMethod.GetClientRect(&rect);
	((CxImage)m_JobMethod).Draw(m_stcJobMethod.GetDC()->GetSafeHdc(), rect);
	// Do not call CPropertyPage::OnPaint() for painting messages
}

void Step1::OnLButtonUp(UINT nFlags, CPoint point)
{
	// ȡ��ÿ��ͼ����ʾ�������ڶԻ����е���Ӿ���
	if (m_Rects[0].IsRectEmpty())
	{
		for (int i=0; i<N; i++)
		{
			m_stcImages[i].GetClientRect(m_Rects+i);
			m_stcImages[i].ClientToScreen(m_Rects+i);
			this->ScreenToClient(m_Rects+i);
		}
	}

	// �鿴������һ��ͼ����ʾ�ؼ�������
	for (int i=0; i<N; i++)
	{
		if (m_Rects[i].PtInRect(point) && m_pImages[i].IsValid())
		{
			ClientToScreen(&point);
			m_stcImages[i].ScreenToClient(&point);

			m_CntrlPts[i].x = point.x;
			m_CntrlPts[i].y = point.y;

			// ����ؼ��еĵ���ͼ���еĵ������
			m_ImagePts[i].x = m_CntrlPts[i].x*m_pImages[i].GetWidth()*1.0/m_Rects[i].Width();
			m_ImagePts[i].y = m_CntrlPts[i].y*m_pImages[i].GetHeight()*1.0/m_Rects[i].Height();

			UpdateData(FALSE);
			Invalidate(FALSE);
			break;
		}
	}
}

// �Զ�ʶ��ÿ��ͼ���еı궨��
void Step1::OnRecognition()
{
	// ȡ��ÿ��ͼ����ʾ�������ڶԻ����е���Ӿ���
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

		// �������ֵ��ͼ���ֵ��
		CImage image;
		image.Copy(m_pImages[i]);
		int nThreshold = image.GetBestThreshold();
		image.Threshold(nThreshold);
//		image.Save("D:\\1.BMP", CXIMAGE_FORMAT_BMP);

		// ��ȡ���Ƶ㲢��ͼ�����ϱ��
		POINT2DF point;
		ObjPtAutoExtract(&image, m_ImagePts[i]);
//		TRACE("ImagePts[%d](%5.2f,%5.2f)\n", i, m_ImagePts[i].x, m_ImagePts[i].y);

		// ���ڿ��Ƶ���������ͼ�����½�Ϊԭ��ģ�����ת������ͼ�����Ͻ�Ϊԭ�������
		m_ImagePts[i].y = image.GetHeight()-m_ImagePts[i].y-1;

		// ��ͼ������ת����ͼ����ʾ�ؼ��е�����
//		int nWidth = image.GetWidth();
//		int nHeight = image.GetHeight();
		m_CntrlPts[i].x = m_ImagePts[i].x*m_Rects[i].Width()*1.0/image.GetWidth();
		m_CntrlPts[i].y = m_ImagePts[i].y*m_Rects[i].Height()*1.0/image.GetHeight();

		// ���»���ɫʮ�ֱ궨������
		CPoint ptTmp(m_CntrlPts[i].x+m_Rects[i].left, m_CntrlPts[i].y+m_Rects[i].top);
		CRect rtTmp(ptTmp.x-RAIDUS, ptTmp.y-RAIDUS, ptTmp.x+RAIDUS, ptTmp.y+RAIDUS);
		InvalidateRect(rtTmp, FALSE);

		// ������λС��
		m_ImagePts[i].x = int(m_ImagePts[i].x*100)/100.0;
		m_ImagePts[i].y = int(m_ImagePts[i].y*100)/100.0;
	}

	// ����ǵ�������ʾ�ڱ༭�ؼ���
	UpdateData(FALSE);
}

int Step1::ObjPtAutoExtract(CImage *pBinImg, POINT2DF &point)
{
	int i, j;

	// ȡ�ö�ֵͼ����γ̱���
	ObjRunLenCode tmpRLC;
	pBinImg->ObjAllRLC(&tmpRLC, 255);

	// ���û��Ŀ��,���˳�
	int nObjNum = tmpRLC.GetObjNum();
	if (nObjNum == 0)
		return FALSE;

	PPRUNLENGTHCODE_MGR ppRLCMgr = tmpRLC.GetObjAll();

	// ������Ŀ������(������)����Ӿ���
	int* pObjArea = new int [nObjNum];
	memset(pObjArea, 0, sizeof(int)*nObjNum);
	CRect* pObjRect = new CRect [nObjNum];
	for (i=0; i<nObjNum; i++)
	{
		// ����ۼ�
		for (j=0; j<ppRLCMgr[i]->nRunNum; j++)
		{
			pObjArea[i] += ppRLCMgr[i]->RC[j].xEnd-ppRLCMgr[i]->RC[j].xStart+1;
		}

		// ��Ӿ���
		tmpRLC.ObjRect(ppRLCMgr[i], pObjRect[i]);
	}

	// �����������Ӿ��β���Ŀ����Ƶ�
	// �����洢����������Ŀ��
	vector< int > vecIdx;
	vector< int > vecArea;
	vector< PRUNLENGTHCODE_MGR > vecRLC;
	vector< float > vecRatio;
	for (i=0; i<nObjNum; i++)
	{
		float fRatio;
		fRatio = pObjRect[i].Width()*1.0/pObjRect[i].Height();

		// ������ȡ��������������Ŀ��ѹ���ջ
		if (fRatio>=RATIO_MIN && fRatio<=RATIO_MAX)
		{
			// ���������������Ŀ����б��
			if (pObjArea[i]>=AREA_MIN && pObjArea[i]<=AREA_MAX)
			{
				// ��¼�������������š�������γ̡������
				vecIdx.push_back(i);
				vecArea.push_back(pObjArea[i]);
				vecRLC.push_back(ppRLCMgr[i]);
				vecRatio.push_back(fRatio);
			}
		}
	}

	// ���ҳ������ӽ�1��Ŀ��
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

	// ���Ŀ����������
	if (nIdx >= 0)
	{
		tmpRLC.ObjCenter(vecRLC[nIdx], point);
		return TRUE;
	}
	else
		return FALSE;
}

// �����Կ��Ƶ�Ϊ���ĵĺ�ʮ����
void Step1::DrawCrossLine(CStatic* pWnd, POINT2DF& point)
{
	// �Կ��Ƶ�Ϊ��������ʮ���߱��
	CPen pen(PS_SOLID, 1, RGB(255, 0, 0));
	CDC* pDC = pWnd->GetDC();
	CPen* pOldPen = pDC->SelectObject(&pen);
	CRect rect;
	pWnd->GetClientRect(&rect);

	// ʮ������ʾ��Χ
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

	// �õ�ÿ��ͼ������ڴ���������Ӿ�����������
	POINT2DF* pImagePts = ((CWizard*)pParent)->m_ImagePts;
	POINT3DF* pGroundPts = ((CWizard*)pParent)->m_GroundPts;
	for (int i=0; i<2; i++)
	{
		m_rtGround[i] = get_image_rect(m_pImages+i, pImagePts[i], pGroundPts[i]);
	}

	// ͳ�Ƽ���ͼ������ڴ���������Ӿ���
	m_rtTotal.left = m_rtTotal.top = INT_MAX;
	m_rtTotal.right = m_rtTotal.bottom = INT_MIN;
	for (i=0; i<2; i++)
	{
		m_rtTotal.left = (m_rtTotal.left<m_rtGround[i].left) ? m_rtTotal.left : m_rtGround[i].left;
		m_rtTotal.top = (m_rtTotal.top<m_rtGround[i].top) ? m_rtTotal.top : m_rtGround[i].top;
		m_rtTotal.right = (m_rtTotal.right>m_rtGround[i].right) ? m_rtTotal.right : m_rtGround[i].right;
		m_rtTotal.bottom = (m_rtTotal.bottom>m_rtGround[i].bottom) ? m_rtTotal.bottom : m_rtGround[i].bottom;
	}

	// ƴ��ͼ��
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

		// ��ͼ�����ڴ�ӳ���л���
		CRect rtSrc(0, 0, m_rtGround[i].Width(), m_rtGround[i].Height());
		((CxImage)m_pImages[i]).Draw(hMemDC, rtSrc);

		// �Լ���ͼ���ܵ���Ӿ��ε����Ͻ�Ϊԭ�㣬���º�����Ϊ�������㼸��ͼ���µ���Ӿ�������
		m_rtRect[i].left = m_rtGround[i].left-m_rtTotal.left;
		m_rtRect[i].right = m_rtGround[i].right-m_rtTotal.left;
		m_rtRect[i].top = m_rtGround[i].top-m_rtTotal.top;
		m_rtRect[i].bottom = m_rtGround[i].bottom-m_rtTotal.top;

		// �������ڴ��е�ͼ�����µ��ڴ�ӳ���л���
		::BitBlt(m_hTotalDC,m_rtRect[i].left, m_rtRect[i].top, m_rtRect[i].Width(), m_rtRect[i].Height(),
						hMemDC, 0, 0, SRCPAINT);
		::DeleteObject(::SelectObject(hMemDC, hOldBitmap));
		::DeleteDC(hMemDC);
	}

	// ��������ͼ�����ʾ����
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

	// ���ڴ��е�ͼ�񾭹����ź���ʾ����
	HDC hDC = m_stcImage.GetDC()->GetSafeHdc();
	::StretchBlt(hDC, 0, 0, m_rtTotal.Width()*m_fRatio, m_rtTotal.Height()*m_fRatio, m_hTotalDC,
						0, 0, m_rtTotal.Width(), m_rtTotal.Height(), SRCCOPY);

	// ��ʾ�����Ķ����
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

	// ȡ��ͼ����ʾ���ڶԻ����е���Ӿ�������
	CRect rect;
	m_stcImage.GetClientRect(&rect);
	m_stcImage.ClientToScreen(&rect);
	ScreenToClient(&rect);

//	if (rect.PtInRect(point))
	{
		if (m_nDraw == -1)	// ���˫��ʱ
			m_nDraw = 0;
		else if (m_nDraw == 0)	// ���˫�����һ�ε���ʱ
		{
			// ��һ�ε���ʱ����������������������ƶ�����
			vector< POINT > vPoints;
			vPoints.push_back(point);
			vPoints.push_back(point);

			m_vvPoints.push_back(vPoints);
			m_nDraw = 1;
		}
		else	// ����һ�ε�����
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

	// ����һ�ε�����
	if (m_nDraw == 1)
	{
		POINT pt1, pt2;
		pt2 = m_vvPoints.back().back();
		m_vvPoints.back().pop_back();
		pt1 = m_vvPoints.back().back();

		// ɾ��ԭ�����߶�
		pDC->MoveTo(pt1);
		pDC->LineTo(pt2);

		// �����µ��߶�
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

// ͼ�����
void Step2::OnImageCut() 
{
	// TODO: Add your control notification handler code here
	int i,j,k;
	CClientDC dc(this);

	// ���ԭ��ʾͼ��
	::DeleteObject(::SelectObject(m_hTotalDC, m_hOldTotalBitmap));
	m_hTotalBitmap = ::CreateCompatibleBitmap(m_stcImage.GetDC()->GetSafeHdc(), m_rtTotal.Width(), m_rtTotal.Height());
	m_hOldTotalBitmap = (HBITMAP)::SelectObject(m_hTotalDC, m_hTotalBitmap);

	// ����������ԭʼͼ���ƴ��ͼ���е�����
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

	// ����ԭʼͼ�������ߵĽ���ͼ�񣬽�ͼ����Ϊ���յ�ͼ���һ����
	for (i=0; i<N; i++)
	{
		if (!m_pImages[i].IsValid())
			return;

		// ����һ��ͼ�񣬲���ǰ��ɫΪ255
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

		// ��image�л����Ϊ3���߶�
		HPEN hPen = ::CreatePen(PS_SOLID, 2, RGB(0,0,0));
		HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);

		// ����ͼ���л��߶�
		for (j=0; j<vvPoints.size(); j++)
		{
			for (k=0; k<vvPoints[j].size()-1; k++)
			{
				POINT pt1,pt2;
				pt1 = vvPoints[j][k];
				pt1.x -= m_rtRect[i].left;		// ת��Ϊ�����Ͻ�Ϊԭ�������
				pt1.y -= m_rtRect[i].top;
				pt1.y = m_rtRect[i].Height()-pt1.y-1;	// ת��Ϊ�����½�Ϊԭ�������

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

		// ��������Ŀ�����Ӿ���
		ObjRunLenCode objAllRLC;
		image.ObjAllRLC(&objAllRLC, 255);

		// ���������Ŀ�꣬������
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

		// ����
		CxImage test(m_rtRect[i].Width(), m_rtRect[i].Height(), 24);
		test.Clear(0);
		
		// ��ԭʼͼ�����Ŀ������ͼ�����ݷŵ�����ƴ��ͼ����
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
				nY = m_rtRect[i].Height()-nY-1;	// ת��Ϊ�Ե�ǰͼ�����Ͻ�Ϊԭ�������
				nY += m_rtRect[i].top;			// ת��Ϊ��ƴ��ͼ������Ͻ�Ϊԭ�������
				nX += m_rtRect[i].left;
//				nY = m_rtTotal.Height()-nY-1;		// ת��Ϊ��ƴ��ͼ�����½�Ϊԭ�������
				::SetPixel(m_hTotalDC, nX, nY, RGB(rgb.rgbRed, rgb.rgbGreen, rgb.rgbBlue));
			}
		}

		// ����
		str.Format("D:\\Test\\result-%d.bmp", i);
//		test.CreateFromHBITMAP(m_hTotalBitmap);
		test.Save(str, CXIMAGE_FORMAT_BMP);

		delete [] pBits;
	}

	Invalidate(TRUE);
}
//
//// ����ԭʼͼ�������ߵĽ���ͼ�񣬽�ͼ����Ϊ���յ�ͼ���һ����
//CRgn* Step2::get_inter_section(const CRect& rtImage, VVECPOINT vLines)
//{
//	int i, j, k;
//	VINTERPOINT vInterPts;
//	VVECPOINT vvPoly;
//
//	// �Ծ��ε����������ν��з���
//	for (i=0; i<4; i++)
//	{
//		POINT ptA1, ptA2;
//		switch (i)
//		{
//		case 0:	// ��¼������˵�
//			ptA1 = rtImage.TopLeft();
//			ptA2.x = rtImage.left;
//			ptA2.y = rtImage.bottom;
//			break;
//		case 1:	// ��¼�±����˵�
//			ptA1.x = rtImage.left;
//			ptA1.y = rtImage.bottom;
//			ptA2 = rtImage.BottomRight();
//			break;
//		case 2:	// ��¼�ұ����˵�
//			ptA1 = rtImage.BottomRight();
//			ptA2.x = rtImage.right;
//			ptA2.y = rtImage.top;
//			break;
//		case 3:	// ��¼�ϱ����˵�
//			ptA1.x = rtImage.right;
//			ptA1.y = rtImage.top;
//			ptA2 = rtImage.TopLeft();
//		}
//
//		// ���ҽ���
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
//		// �Խ����������
//		// �����뵱ǰ�ߵ�һ���˵�ľ���Խ�������
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
//		// �����Ȼ�õĽ�����з���
//		for (j=0; j<vInterPts.size()-1; j++)
//		{
//			POINT pt1, pt2;
//			pt1 = vInterPts[j].pt;
//			pt2 = m_vvPoints[ vInterPts[j].nLineIdx ][ vInterPts[j].nPointIdx ];
//			double dbAngle = get_angle(ptA1, pt1, pt2);
//
//			// ���ptA1��pt1Ϊԭ��˳ʱ����תdbAngel�ǶȺ������
//			// �����ptA1�������pt1Ϊԭ�������
//			float a, b;
//			a = ptA1.x-pt1.x;
//			b = ptA1.y-pt1.y;
//
//			// ����ת��ĵ������pt1�������(a+b*i)*(cos��+i*sin��)
//			a = a*cos(dbAngle)-b*sin(dbAngle);
//			b = a*sin(dbAngle)+b*cos(dbAngle);
//
//			// ����ת��������
//			a += pt1.x;
//			b += pt1.y;
//
//			// �ж������Ƿ���ͬһֱ����
//			POINT2DF ptL1(pt1.x, pt1.y), ptL2(pt2.x, pt2.y), ptR(a, b);
//			int nRet = point_in_line(ptL1, ptL2, ptR);
//
//			// ���ptL2��ptR�غϻ���ptL1���߶�ptL2-ptR���ӳ�����
//			if ((ptL2.x==ptR.x)&&(ptL2.y==ptR.y) || nRet==POINT_IN_EX_LINE)
//			{
//			}
//		}
//	}
//
//	return NULL;
//}
//
// ͼ�񱣴�
void Step2::OnImageSave() 
{
	// TODO: Add your control notification handler code here
	
}

/* ==================================================================
 * �������ƣ�  Step2::get_image_rect()
 * ���ߣ�      [Leng]
 * ������:     [2008-8-10 0:07:14]
 * ����ֵ��    [CRect&] -
 * �����б� 
               [CImage* pImage] - [In]	ͼ������
               [ POINT2DF ptImage] - [In]	ͼ���б궨�������ͼ�����Ͻǵ�����
               [ POINT2DF ptGround] - [In]	ͼ���б궨��Ĵ������
ע�� :	�õ�ͼ������ڴ���������Ӿ�����������
==================================================================*/
CRect Step2::get_image_rect(CImage* pImage, POINT2DF ptImage, POINT3DF ptGround)
{
	// �����ǵ������߽�ľ���
	CRect rect;
	rect.left = ptImage.x;
	rect.right = pImage->GetWidth()-ptImage.x;
	rect.top = ptImage.y;
	rect.bottom = pImage->GetHeight()-ptImage.y;

	// ����ͼ��Ĵ������߽�
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

	// �Զ��ͼ����д����õ�һ��������ȫ����ͼ��

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

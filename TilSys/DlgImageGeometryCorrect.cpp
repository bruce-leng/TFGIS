// DlgImageGeometryCorrect.cpp : implementation file
//

#include "stdafx.h"
#include "tilsys.h"
#include "DlgImageGeometryCorrect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

	// ����Ӿ��γ��������[0.5,2.0]֮��
#define RATIO_MIN 0.5
#define RATIO_MAX 2.0
	// ��Ŀ��������ֵΪ50
#define AREA_MIN 20 
#define AREA_MAX 200
	// ��ʮ���߱�ǰ뾶Ϊ4
#define RAIDUS 4
/////////////////////////////////////////////////////////////////////////////
// DlgImageGeometryCorrect dialog


DlgImageGeometryCorrect::DlgImageGeometryCorrect(CWnd* pParent /*=NULL*/)
	: CDialog(DlgImageGeometryCorrect::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgImageGeometryCorrect)
	m_dbEdt1NextX = 0.0;
	m_dbEdt2NextX = 0.0;
	m_dbEdt3NextX = 0.0;
	m_dbEdt4NextX = 0.0;
	m_dbEdt5NextX = 0.0;
	m_dbEdt6NextX = 0.0;
	m_dbEdt7NextX = 0.0;
	m_dbEdt8NextX = 0.0;
	m_dbEdt9NextX = 0.0;	//.....

	m_dbEdt1NextY = 0.0;
	m_dbEdt2NextY = 0.0;
	m_dbEdt3NextY = 0.0;
	m_dbEdt4NextY = 0.0;
	m_dbEdt5NextY = 0.0;
	m_dbEdt6NextY = 0.0;
	m_dbEdt7NextY = 0.0;
	m_dbEdt8NextY = 0.0;
	m_dbEdt9NextY = 0.0;	//.....

	m_dbEdt1PrevX = 0.0;
	m_dbEdt2PrevX = 0.0;
	m_dbEdt3PrevX = 0.0;
	m_dbEdt4PrevX = 0.0;
	m_dbEdt5PrevX = 0.0;
	m_dbEdt6PrevX = 0.0;
	m_dbEdt7PrevX = 0.0;
	m_dbEdt8PrevX = 0.0;
	m_dbEdt9PrevX = 0.0;	//.....
	
	m_dbEdt1PrevY = 0.0;
	m_dbEdt2PrevY = 0.0;
	m_dbEdt3PrevY = 0.0;
	m_dbEdt4PrevY = 0.0;
	m_dbEdt5PrevY = 0.0;
	m_dbEdt6PrevY = 0.0;
	m_dbEdt7PrevY = 0.0;
	m_dbEdt8PrevY = 0.0;
	m_dbEdt9PrevY = 0.0;
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCanvas = NULL;
}

DlgImageGeometryCorrect::~DlgImageGeometryCorrect()
{
	if (m_pCanvas)
		delete m_pCanvas;
}

void DlgImageGeometryCorrect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgImageGeometryCorrect)
	DDX_Control(pDX, IDC_IMAGE, m_ctlImage);
	DDX_Text(pDX, IDC_EDT1_NEXT_X, m_dbEdt1NextX);
	DDX_Text(pDX, IDC_EDT2_NEXT_X, m_dbEdt2NextX);
	DDX_Text(pDX, IDC_EDT3_NEXT_X, m_dbEdt3NextX);
	DDX_Text(pDX, IDC_EDT4_NEXT_X, m_dbEdt4NextX);
	DDX_Text(pDX, IDC_EDT5_NEXT_X, m_dbEdt5NextX);
	DDX_Text(pDX, IDC_EDT6_NEXT_X, m_dbEdt6NextX);
	DDX_Text(pDX, IDC_EDT7_NEXT_X, m_dbEdt7NextX);
	DDX_Text(pDX, IDC_EDT8_NEXT_X, m_dbEdt8NextX);
	DDX_Text(pDX, IDC_EDT9_NEXT_X, m_dbEdt9NextX);	//.....

	DDX_Text(pDX, IDC_EDT1_NEXT_Y, m_dbEdt1NextY);
	DDX_Text(pDX, IDC_EDT2_NEXT_Y, m_dbEdt2NextY);
	DDX_Text(pDX, IDC_EDT3_NEXT_Y, m_dbEdt3NextY);
	DDX_Text(pDX, IDC_EDT4_NEXT_Y, m_dbEdt4NextY);
	DDX_Text(pDX, IDC_EDT5_NEXT_Y, m_dbEdt5NextY);
	DDX_Text(pDX, IDC_EDT6_NEXT_Y, m_dbEdt6NextY);
	DDX_Text(pDX, IDC_EDT7_NEXT_Y, m_dbEdt7NextY);
	DDX_Text(pDX, IDC_EDT8_NEXT_Y, m_dbEdt8NextY);
	DDX_Text(pDX, IDC_EDT9_NEXT_Y, m_dbEdt9NextY);	//.....

	DDX_Text(pDX, IDC_EDT1_PREV_X, m_dbEdt1PrevX);
	DDX_Text(pDX, IDC_EDT2_PREV_X, m_dbEdt2PrevX);
	DDX_Text(pDX, IDC_EDT3_PREV_X, m_dbEdt3PrevX);
	DDX_Text(pDX, IDC_EDT4_PREV_X, m_dbEdt4PrevX);
	DDX_Text(pDX, IDC_EDT5_PREV_X, m_dbEdt5PrevX);
	DDX_Text(pDX, IDC_EDT6_PREV_X, m_dbEdt6PrevX);
	DDX_Text(pDX, IDC_EDT7_PREV_X, m_dbEdt7PrevX);
	DDX_Text(pDX, IDC_EDT8_PREV_X, m_dbEdt8PrevX);
	DDX_Text(pDX, IDC_EDT9_PREV_X, m_dbEdt9PrevX);	//.....
	
	DDX_Text(pDX, IDC_EDT1_PREV_Y, m_dbEdt1PrevY);
	DDX_Text(pDX, IDC_EDT2_PREV_Y, m_dbEdt2PrevY);
	DDX_Text(pDX, IDC_EDT3_PREV_Y, m_dbEdt3PrevY);
	DDX_Text(pDX, IDC_EDT4_PREV_Y, m_dbEdt4PrevY);
	DDX_Text(pDX, IDC_EDT5_PREV_Y, m_dbEdt5PrevY);
	DDX_Text(pDX, IDC_EDT6_PREV_Y, m_dbEdt6PrevY);
	DDX_Text(pDX, IDC_EDT7_PREV_Y, m_dbEdt7PrevY);
	DDX_Text(pDX, IDC_EDT8_PREV_Y, m_dbEdt8PrevY);
	DDX_Text(pDX, IDC_EDT9_PREV_Y, m_dbEdt9PrevY);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(DlgImageGeometryCorrect, CDialog)
	//{{AFX_MSG_MAP(DlgImageGeometryCorrect)
	ON_BN_CLICKED(IDC_CTRL_PT_EXTRACT, OnCtrlPtExtract)
	ON_BN_CLICKED(IDC_CTRL_PT_DEL, OnCtrlPtDel)
	ON_BN_CLICKED(IDC_CTRL_PT_ADD, OnCtrlPtAdd)
	ON_BN_CLICKED(IDC_IMAGE_CORRECT, OnImageCorrect)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgImageGeometryCorrect message handlers

BOOL DlgImageGeometryCorrect::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	CTilCanvasMgr* pMgr = GetCanvasMgr();
	CTilCanvas* pCanvas = pMgr->GetCurCanvas();
	m_pCanvas = new CTilCanvas(*pCanvas);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DlgImageGeometryCorrect::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect srcRect(0, 0, m_pCanvas->GetWidth(), m_pCanvas->GetHeight());
	CRect dstRect;
	m_ctlImage.GetClientRect(&dstRect);
	m_pCanvas->Draw(m_ctlImage.GetDC(), dstRect, srcRect);

	// ��ʮ���߱�ǿ��Ƶ�
	DrawCrossLine(m_ctlImage.GetDC(), m_ctrlPoints);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void DlgImageGeometryCorrect::OnLButtonUp(UINT nFlags, CPoint point) 
{
	POINT ctrlPoint;
	CImage* pImage = m_pCanvas->GetImage()->GetCurImage();

	// �쿴���Ƿ���ͼ����ʾ�ؼ���
	CRect rect;
	m_ctlImage.GetClientRect(&rect);

	// ��������ת����ͼ����ʾ�ؼ��иõ������
	ClientToScreen(&point);
	m_ctlImage.ScreenToClient(&point);

	// ����㲻��ͼ����ʾ�ؼ���,��ֱ���˳�
	if (!rect.PtInRect(point))
		return;

	// ��ͼ����ʾ�ؼ��е�����ת��Ϊͼ���еĵ�����
	ctrlPoint.x = point.x*1.0/rect.Width()*pImage->GetWidth()+0.5;
	ctrlPoint.y = point.y*1.0/rect.Height()*pImage->GetHeight();
	ctrlPoint.y = pImage->GetHeight()-1-ctrlPoint.y+0.5;

	// �Կ��Ƶ������������
	// ����������ֵ
#define RGN_GRW_THRSHLD	50

	// �����������������γ�
	ObjRunLenCode tmpRLC;
	pImage->ObjRgnGrow(ctrlPoint, RGN_GRW_THRSHLD, &tmpRLC);

	// �������ӿ��Ƶ�
	if (bAdd)
	{
		// ��������з���
		int objArea = tmpRLC.ObjArea(0);

		// ����������������,��ֱ���˳�
		if (objArea<AREA_MIN || objArea>AREA_MAX)
			return;

/*
		CRect objRect;
		tmpRLC.ObjRect(0, objRect);

		// �������Ȳ���������,��ֱ���˳�
		float fRatio = objRect.Width()*1.0/objRect.Height();
		if (fRatio<RATIO_MIN || fRatio>RATIO_MAX)
			return;
*/

		// �����������ĵ����ĵ�����
		POINT2DF ptCenter;
		tmpRLC.ObjCenter(0, ptCenter);

		// ��ͼ���е�����ת��Ϊͼ����ʾ�ؼ��иõ�����
		ptCenter.x = ptCenter.x*1.0/pImage->GetWidth()*rect.Width();
		ptCenter.y = (pImage->GetHeight()-ptCenter.y-1)*1.0/pImage->GetHeight()*rect.Height();

		// ��û����ͬ�ĵ�,�򽫿��Ƶ�ѹ���ջ
		for (int i=0; i<m_ctrlPoints.size(); i++)
		{
			if (int(m_ctrlPoints[i].x)==int(ptCenter.x)
				&& int(m_ctrlPoints[i].y)==int(ptCenter.y))
				break;
		}
		if (i >= m_ctrlPoints.size())
			m_ctrlPoints.push_back(ptCenter);

		// �����Ե�Ϊ���ĵ�ʮ����Ӿ�����������
		// ��������ת�����Ի�������ϵ��
		POINT ptTmp;
		ptTmp.x = ptCenter.x;
		ptTmp.y = ptCenter.y;
		m_ctlImage.ClientToScreen(&ptTmp);
		ScreenToClient(&ptTmp);

		// �Ե�Ϊ���ĵ�ʮ������Ӿ���
		CRect rtTmp;
		rtTmp.left = ptCenter.x-RAIDUS;
		rtTmp.right = ptCenter.x+RAIDUS+1;;
		rtTmp.top = ptCenter.y-RAIDUS;
		rtTmp.bottom = ptCenter.y+RAIDUS+1;

		InvalidateRect(&rtTmp);
	}
	else
	{
		// ����ɾ�����Ƶ�,��鿴���Ƶ��Ƿ��������������γɵ�Ŀ����
		PPRUNLENGTHCODE_MGR ppRLCMgr = tmpRLC.GetObjAll();
		for (int i=0; i<m_ctrlPoints.size(); i++)
		{
			// �����Ƶ��ͼ����ʾ�ؼ��е�����ת����ͼ������
			POINT ptTmp;
			ptTmp.x = m_ctrlPoints[i].x*1.0/rect.Width()*pImage->GetWidth()+0.5;
			ptTmp.y = m_ctrlPoints[i].y*1.0/rect.Height()*pImage->GetHeight();
			ptTmp.y = pImage->GetHeight()-1-ptTmp.y+0.5;

			// �����Ƶ������������γɵ�Ŀ����,��ֱ������ѭ��
			for (int j=0; j<ppRLCMgr[0]->nRunNum; j++)
			{
				if (ptTmp.y==ppRLCMgr[0]->RC[j].y
					&& ptTmp.x>=ppRLCMgr[0]->RC[j].xStart
					&& ptTmp.x<=ppRLCMgr[0]->RC[j].xEnd)
					break;
			}

			// ���jѭ��û�е�ĩβ����ֹ,��ɾ����i��,����ɾ��������,ֱ���˳�
			if (j < ppRLCMgr[0]->nRunNum)
			{
				// �����Ե�Ϊ���ĵ�ʮ����Ӿ�����������
				// ��������ת�����Ի�������ϵ��
				POINT ctrlPt;
				ctrlPt.x = m_ctrlPoints[i].x;
				ctrlPt.y = m_ctrlPoints[i].y;
				m_ctlImage.ClientToScreen(&ctrlPt);
				ScreenToClient(&ctrlPt);

				// �Ե�Ϊ���ĵ�ʮ������Ӿ���
				CRect ctrlRt;
				ctrlRt.left = ctrlPt.x-RAIDUS;
				ctrlRt.right = ctrlPt.x+RAIDUS+1;;
				ctrlRt.top = ctrlPt.y-RAIDUS;
				ctrlRt.bottom = ctrlPt.y+RAIDUS+1;

				// ɾ����i��,���¸�������˳�
				m_ctrlPoints.erase(m_ctrlPoints.begin()+i);
				InvalidateRect(&ctrlRt);
				break;
			}
		}
	}

	// ��ʾ���Ƶ�����
	ShowCtrlPointsCoord();

	CDialog::OnLButtonUp(nFlags, point);
}

// ���Ƶ���ȡ
void DlgImageGeometryCorrect::OnCtrlPtExtract() 
{
	// ����ð�ť��,����ť���,���������ٵ���ð�ť
	GetDlgItem(IDC_CTRL_PT_EXTRACT)->EnableWindow(FALSE);

	// �������ֵ��ͼ���ֵ��
	CImage* pImage = m_pCanvas->GetImage()->GetCurImage();
	int nThreshold = pImage->GetBestThreshold();
	pImage->Threshold(nThreshold);

	// ��ȡ���Ƶ㲢��ͼ�����ϱ��
	CTRLPOINTS ctrlPts;
	ObjPtAutoExtract(pImage, ctrlPts);
	TransferCoord(ctrlPts, m_ctrlPoints);
	Invalidate();

	// ��ʾ���Ƶ�����
	ShowCtrlPointsCoord();
}

// ������Ƶ��ֶ�ɾ��
void DlgImageGeometryCorrect::OnCtrlPtDel() 
{
	// TODO: Add your control notification handler code here
	bAdd = FALSE;
}

// ���Ƶ��ֶ����
void DlgImageGeometryCorrect::OnCtrlPtAdd() 
{
	// TODO: Add your control notification handler code here
	bAdd = TRUE;
}

// ͼ�񼸺�У��
void DlgImageGeometryCorrect::OnImageCorrect() 
{
	// TODO: Add your control notification handler code here
	bAdd = FALSE;
}

void DlgImageGeometryCorrect::OnOk() 
{
	// TODO: Add your control notification handler code here
	
}

// �Զ�ȡ��Ŀ����Ƶ�,��ͼ���Ѿ�Ϊ��ֵͼ��
void DlgImageGeometryCorrect::ObjPtAutoExtract(CImage* pBinImg, CTRLPOINTS& ctrlPoints)
{
	int i, j;

	// ȡ�ö�ֵͼ����γ̱���
	ObjRunLenCode tmpRLC;
	pBinImg->ObjAllRLC(&tmpRLC, 255);

	// ���û��Ŀ��,���˳�
	int nObjNum = tmpRLC.GetObjNum();
	if (nObjNum == 0)
		return;

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
				// ��¼�������������š�������γ�
				vecIdx.push_back(i);
				vecArea.push_back(pObjArea[i]);
				vecRLC.push_back(ppRLCMgr[i]);
			}
		}
	}

	// ������Ӵ�С��Ŀ������
	int nTmp;
	PRUNLENGTHCODE_MGR pItem;
	for (i=0; i<vecArea.size(); i++)
	{
		for (j=i+1; j<vecArea.size(); j++)
		{
			if (vecArea[i] < vecArea[j])
			{
				nTmp = vecArea[i];
				vecArea[i] = vecArea[j];
				vecArea[j] = nTmp;

				nTmp = vecIdx[i];
				vecIdx[i] = vecIdx[j];
				vecIdx[j] = nTmp;

				pItem = vecRLC[i];
				vecRLC[i] = vecRLC[j];
				vecRLC[j] = pItem;
			}
		}
	}

	// ���Ŀ����������
	POINT2DF* pCenter = new POINT2DF [vecRLC.size()];
	memset(pCenter, 0, sizeof(POINT2DF)*vecRLC.size());

	for (i=0; i<vecRLC.size(); i++)
	{
		tmpRLC.ObjCenter(vecRLC[i], pCenter[i]);
	}

	// ��������(���ȡǰ9����)
	int nNum = (vecRLC.size()<9) ? vecRLC.size() : 9;
	for (i=0; i<nNum; i++)
		ctrlPoints.push_back(pCenter[i]);

	delete [] pCenter;
	delete [] pObjRect;
	delete [] pObjArea;
}

// �ֶ�ȡ��Ŀ����Ƶ�
//int DlgImageGeometryCorrect::ObjPtManualExtract(CImage* pBinImg, POINT point, CTRLPOINTS& ctrlPoints)
//{
//	return 0;
//}

// �ֶ�ɾ������Ŀ����Ƶ�
//int DlgImageGeometryCorrect::ObjPtManualDel(CImage* pBinImg, POINT point, CTRLPOINTS& ctrlPoints)
//{
//	return 0;
//}

// �����Ƶ���ͼ���е�����ת������ʾͼ������е�����
void DlgImageGeometryCorrect::TransferCoord(CTRLPOINTS& srcCtrlPts, CTRLPOINTS& dstCtrlPts)
{
	CImage* pImage = m_pCanvas->GetImage()->GetCurImage();
	CRect ctrlRect;
	m_ctlImage.GetClientRect(&ctrlRect);

	// ����ת��
	for (int i=0; i<srcCtrlPts.size(); i++)
	{
		POINT2DF pt;
		pt.x = srcCtrlPts[i].x*1.0/pImage->GetWidth()*ctrlRect.Width();
		pt.y = srcCtrlPts[i].y*1.0/pImage->GetHeight()*ctrlRect.Height();

		// ����ͼ�����½�Ϊԭ��,y������,��ʾͼ��������Ͻ�Ϊԭ��,y������,����Ҫת��
		pt.y = ctrlRect.Height()-pt.y-1;

		dstCtrlPts.push_back(pt);
	}
}

// �����Կ��Ƶ�Ϊ���ĵĺ�ʮ����
void DlgImageGeometryCorrect::DrawCrossLine(CDC* pDC, CTRLPOINTS& ctrlPoints)
{
	// �Կ��Ƶ�Ϊ��������ʮ���߱��
	CPen pen(PS_SOLID, 1, RGB(255, 0, 0));
	CPen* pOldPen = pDC->SelectObject(&pen);
	CRect ctrlRect;
	m_ctlImage.GetClientRect(&ctrlRect);

	for (int i=0; i<ctrlPoints.size(); i++)
	{
		// ʮ������ʾ��Χ
		RECT rect;
		rect.left = (ctrlPoints[i].x-RAIDUS>0) ? ctrlPoints[i].x-RAIDUS : 0;
		rect.right = (ctrlPoints[i].x+RAIDUS<ctrlRect.Width()) ? ctrlPoints[i].x+RAIDUS : ctrlRect.Width();
		rect.top = (ctrlPoints[i].y-RAIDUS>0) ? ctrlPoints[i].y-RAIDUS : 0;
		rect.bottom = (ctrlPoints[i].y+RAIDUS<ctrlRect.Height()) ? ctrlPoints[i].y+RAIDUS : ctrlRect.Height();

		pDC->MoveTo(ctrlPoints[i].x, rect.top);
		pDC->LineTo(ctrlPoints[i].x, rect.bottom+1);
		pDC->MoveTo(rect.left, ctrlPoints[i].y);
		pDC->LineTo(rect.right+1, ctrlPoints[i].y);
	}

	pDC->SelectObject(pOldPen);
}

/*
// �ú�ɫʮ���߱�ǿ��Ƶ�����
void DlgImageGeometryCorrect::set_cross_line(CTRLPOINTS& ctrlPoints)
{
	// �����Ƶ��ͼ������ת���ɶ��������ʾͼ������е�����
	vector<POINT2DF> convertPts;

	CImage* pImage = m_pCanvas->GetImage()->GetCurImage();
	CRect dstRect;
	m_ctlImage.GetClientRect(&dstRect);

	// ����ת��
	for (int i=0; i<convertPts.size(); i++)
	{
		convertPts[i].x = ctrlPoints[i].x*1.0/pImage->GetWidth()*dstRect.Width();
		convertPts[i].y = ctrlPoints[i].y*1.0/pImage->GetHeight()*dstRect.Height();

		// ����ͼ�����½�Ϊԭ��,y������,��ʾͼ��������Ͻ�Ϊԭ��,y������,����Ҫת��
		convertPts[i].y = dstRect.Height()-convertPts[i].y-1;
	}

	// ��ʮ���߱�ǰ뾶Ϊ3
#define RAIDUS 4

	// �Կ��Ƶ�Ϊ��������ʮ���߱��
	CDC* pDC = m_ctlImage.GetDC();
	CPen pen(PS_SOLID, 1, RGB(255, 0, 0));
	CPen* pOldPen = pDC->SelectObject(&pen);

	for (i=0; i<convertPts.size(); i++)
	{
		// ʮ������ʾ��Χ
		RECT rect;
		rect.left = (convertPts[i].x-RAIDUS>0) ? convertPts[i].x-RAIDUS : 0;
		rect.right = (convertPts[i].x+RAIDUS<dstRect.Width()) ? convertPts[i].x+RAIDUS : dstRect.Width();
		rect.top = (convertPts[i].y-RAIDUS>0) ? convertPts[i].y-RAIDUS : 0;
		rect.bottom = (convertPts[i].y+RAIDUS<dstRect.Height()) ? convertPts[i].y+RAIDUS : dstRect.Height();

		pDC->MoveTo(convertPts[i].x, rect.top);
		pDC->LineTo(convertPts[i].x, rect.bottom+1);
		pDC->MoveTo(rect.left, convertPts[i].y);
		pDC->LineTo(rect.right+1, convertPts[i].y);
	}

	pDC->SelectObject(pOldPen);
}
*/
// ��Ŀ����Ƶ�������ʾ��"У��ǰ���Ƶ�����"��Ͽ���
void DlgImageGeometryCorrect::ShowCtrlPointsCoord()
{
	if (m_ctrlPoints.size() > 0)
	{
		m_dbEdt1PrevX = m_ctrlPoints[0].x;
		m_dbEdt1PrevY = m_ctrlPoints[0].y;
	}
	else
	{
		m_dbEdt1PrevX = 0;
		m_dbEdt1PrevY = 0;
	}

	if (m_ctrlPoints.size() > 1)
	{
		m_dbEdt2PrevX = m_ctrlPoints[1].x;
		m_dbEdt2PrevY = m_ctrlPoints[1].y;
	}
	else
	{
		m_dbEdt2PrevX = 0;
		m_dbEdt2PrevY = 0;
	}

	if (m_ctrlPoints.size() > 2)
	{
		m_dbEdt3PrevX = m_ctrlPoints[2].x;
		m_dbEdt3PrevY = m_ctrlPoints[2].y;
	}
	else
	{
		m_dbEdt3PrevX = 0;
		m_dbEdt3PrevY = 0;
	}

	if (m_ctrlPoints.size() > 3)
	{
		m_dbEdt4PrevX = m_ctrlPoints[3].x;
		m_dbEdt4PrevY = m_ctrlPoints[3].y;
	}
	else
	{
		m_dbEdt4PrevX = 0;
		m_dbEdt4PrevY = 0;
	}

	if (m_ctrlPoints.size() > 4)
	{
		m_dbEdt5PrevX = m_ctrlPoints[4].x;
		m_dbEdt5PrevY = m_ctrlPoints[4].y;
	}
	else
	{
		m_dbEdt5PrevX = 0;
		m_dbEdt5PrevY = 0;
	}

	if (m_ctrlPoints.size() > 5)
	{
		m_dbEdt6PrevX = m_ctrlPoints[5].x;
		m_dbEdt6PrevY = m_ctrlPoints[5].y;
	}
	else
	{
		m_dbEdt6PrevX = 0;
		m_dbEdt6PrevY = 0;
	}

	if (m_ctrlPoints.size() > 6)
	{
		m_dbEdt7PrevX = m_ctrlPoints[6].x;
		m_dbEdt7PrevY = m_ctrlPoints[6].y;
	}
	else
	{
		m_dbEdt7PrevX = 0;
		m_dbEdt7PrevY = 0;
	}

	if (m_ctrlPoints.size() > 7)
	{
		m_dbEdt8PrevX = m_ctrlPoints[7].x;
		m_dbEdt8PrevY = m_ctrlPoints[7].y;
	}
	else
	{
		m_dbEdt8PrevX = 0;
		m_dbEdt8PrevY = 0;
	}

	if (m_ctrlPoints.size() > 8)
	{
		m_dbEdt9PrevX = m_ctrlPoints[8].x;
		m_dbEdt9PrevY = m_ctrlPoints[8].y;
	}
	else
	{
		m_dbEdt9PrevX = 0;
		m_dbEdt9PrevY = 0;
	}

	UpdateData(FALSE);
}

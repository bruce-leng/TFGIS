// DlgGraphObjLnk.cpp : implementation file
//

#include "stdafx.h"
#include "tilsys.h"
#include "DlgGraphObjLnk.h"
#include "Graph/GraphDefine.h"
#include "Graph/Polygon.h"
#include "TilComFunction.h"
#include <float.h>
#include "MathComput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 
typedef struct __tagValuePos
{
	float fValue;
	int x;
	int y;

public:
	struct __tagValuePos() {fValue = FLT_MIN; x = y = INT_MIN;};
	struct __tagValuePos(float f) {fValue = f; x = y = INT_MIN;};
	struct __tagValuePos(float f, int m, int n) {fValue = f; x = m; y = n;};
	struct __tagValuePos(const struct __tagValuePos& f) {fValue = f.fValue; x = f.x; y = f.y;};

	struct __tagValuePos& operator = (double f) {fValue = f; x = y = INT_MIN; return *this;};
	struct __tagValuePos& operator = (const struct __tagValuePos& f) {fValue = f.fValue; x = f.x; y = f.y; return *this;};
	
	BOOL operator < (double f) {if (fValue < f) return true; return false;};
	BOOL operator == (double f) {if (fValue == f) return true; return false;};
	BOOL operator > (double f) {if (fValue > f) return true; return false;};

	BOOL operator < (const struct __tagValuePos& f) {if (fValue < f.fValue) return true; return false;};
	BOOL operator == (const struct __tagValuePos& f) {if (fValue == f.fValue) return true; return false;};
	BOOL operator > (const struct __tagValuePos& f) {if (fValue > f.fValue) return true; return false;};

	inline float& F() {return fValue;};
	inline int& FirstIdx() {return x;};
	inline int& SecondIdx() {return y;};
} VALUEPOS, *PVALUEPOS, **PPVALUEPOS;

typedef vector<VALUEPOS> VCPOS;
typedef vector<VALUEPOS>::iterator VCPOSIT;

// ����ǰ���������������Ҳ�Ŀ������Ӷ�Ӧ��ϵ
void LinkObjects(CTilCanvas* pCanvas1, CTilCanvas* pCanvas2, int nGraphLayer)
{
	int nObjNum1, nObjNum2, nEdge1, nEdge2;
	int i,j,m;
	PROPERTY pt1, pt2;

	CGraph* pGraph1 = pCanvas1->GetGraph()->GetGraphByIdx(nGraphLayer);
	if(pGraph1->Size()==0)
		return;
	CGraph* pGraph2 = pCanvas2->GetGraph()->GetGraphByIdx(nGraphLayer);
	if(pGraph2->Size()==0)
		return;

	nObjNum1 = pGraph1->Size();
	nObjNum2 = pGraph2->Size();

	BOOL *pbLinked = new BOOL [nObjNum1+nObjNum2];
	memset((void*)pbLinked, 0 , sizeof(BOOL)*(nObjNum1+nObjNum2));

	// ȡ�ù��β����Ӿ�����Ϊ��׼���β�
	CRect rtTunArc = pGraph1->GetUnitByIdx(0)->GetPolyRect();

	VCPOS vcpos;
	vcpos.resize(nObjNum1*nObjNum2);

	//��ʼ����һ�������ϵĺ�������
	for (i=0; i<nObjNum1; ++i)
	{	
		CUnit* pUnit = pGraph1->GetUnitByIdx(i);
		pUnit->SetNextLink(-1);
	}

	//��ʼ���ڶ��������ϵ�ǰ������
	for (i=0; i<nObjNum2; ++i)
	{	
		CUnit* pUnit = pGraph2->GetUnitByIdx(i);
		pUnit->SetPreLink(-1);
	}

	// ��������1��2���Ҳ㶥���������
	int nMaxPointNum1 = 0;
	int nMaxPointNum2 = 0;
	for (i=0; i<nObjNum1; i++)
	{
		if (nMaxPointNum1 < pGraph1->GetUnitByIdx(i)->GetPointNum())
			nMaxPointNum1 = pGraph1->GetUnitByIdx(i)->GetPointNum();
	}	
	for (i=0; i<nObjNum2; i++)
	{
		if (nMaxPointNum2 < pGraph2->GetUnitByIdx(i)->GetPointNum())
			nMaxPointNum2 = pGraph2->GetUnitByIdx(i)->GetPointNum();
	}	

	//����㻺��
	POINT2DF* pPoint1 = new POINT2DF [nMaxPointNum1];
	POINT2DF* pPoint2 = new POINT2DF [nMaxPointNum2];

//////////////////////////////////////////////////////////////////////////
////////			��������ͼ
	for(i = 0; i < nObjNum1; ++i)
	{
		CUnit* pUnit1 = pGraph1->GetUnitByIdx(i);
		nEdge1 = pUnit1->GetPointNum();
		for(m = 0; m < nEdge1; ++m)
		{
			pPoint1[m] = pUnit1->GetPoint(m);
			pPoint1[m].x /= rtTunArc.Width();
			pPoint1[m].y /= rtTunArc.Height();
		}
		CalProperty(pPoint1, nEdge1, pt1, 1, 1);		

		for (j=0; j<nObjNum2; ++j)
		{
			CUnit* pUnit2 = pGraph2->GetUnitByIdx(j);
			nEdge2 = pUnit2->GetPointNum();
			for(m = 0; m < nEdge2; ++m)
			{
				pPoint2[m] = pUnit2->GetPoint(m);
				pPoint2[m].x /= rtTunArc.Width();
				pPoint2[m].y /= rtTunArc.Height();
			}
			CalProperty(pPoint2, nEdge2, pt2, 1, 1);		

			// �����1���������ϵ��Ҳ�Ŀ��i���2���������ϵ��Ҳ�Ŀ��j֮��Ľ��Ƴ̶�
			vcpos[i*nObjNum2+j].F() = fabs(pt1.fArea-pt2.fArea)+
 									  fabs(pt1.ptMean.x-pt2.ptMean.x)+
									  fabs(pt1.ptMean.y-pt2.ptMean.y);			
			vcpos[i*nObjNum2+j].FirstIdx() = i;
			vcpos[i*nObjNum2+j].SecondIdx() = j;

		}
	}

//////////////////////////////////////////////////////////////////////////
/////		�ж�Ŀ������----����ڷ�

	sort(vcpos.begin(), vcpos.end());
	VALUEPOS vcTemp;
	for(i = 0; i < nObjNum2 * nObjNum1; ++i)
	{
		vcTemp = vcpos[i];

		if (pbLinked[vcTemp.FirstIdx()] || pbLinked[vcTemp.SecondIdx() + nObjNum1]) 
			continue;

		pbLinked[vcTemp.FirstIdx()] = pbLinked[vcTemp.SecondIdx()+nObjNum1] = TRUE;

		pGraph1->GetUnitByIdx(vcTemp.FirstIdx())->SetNextLink(vcTemp.SecondIdx());
		pGraph2->GetUnitByIdx(vcTemp.SecondIdx())->SetPreLink(vcTemp.FirstIdx());
	}

	delete [] pPoint2;
	delete [] pPoint1;
	delete [] pbLinked;
}

/////////////////////////////////////////////////////////////////////////////
// DlgGraphObjLnk dialog

DlgGraphObjLnk::DlgGraphObjLnk(CWnd* pParent /*=NULL*/)
	: CDialog(DlgGraphObjLnk::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgGraphObjLnk)
	m_strSliceFirstDK = _T("");
	m_strSliceLastDK = _T("");
	m_strSlice1DK = _T("");
	m_strSlice1ZoomFactor = _T("");
	m_strSlice2DK = _T("");
	m_strSlice2ZoomFactor = _T("");
	//}}AFX_DATA_INIT
	m_nGraphLayer = GRAPH_LAYER_ROCKS;
}

void DlgGraphObjLnk::SetCurGraphLayer(int nGraphLayer)
{
	m_nGraphLayer = nGraphLayer;
}

DlgGraphObjLnk::~DlgGraphObjLnk()
{
	if (m_Slice1.pCanvas)	delete m_Slice1.pCanvas;
	if (m_Slice2.pCanvas)	delete m_Slice2.pCanvas;
}

void DlgGraphObjLnk::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgGraphObjLnk)
	DDX_Control(pDX, IDC_BTN_LINKCUR, m_btnLinkCur);
	DDX_Control(pDX, IDC_BTN_LINKALL, m_btnLinkAll);
	DDX_Control(pDX, IDC_SLICE1_ZOOMIN, m_btnSlice1ZoomIn);
	DDX_Control(pDX, IDC_SLICE2_ZOOMOUT, m_btnSlice2ZoomOut);
	DDX_Control(pDX, IDC_SLICE2_ZOOMIN, m_btnSlice2ZoomIn);
	DDX_Control(pDX, IDC_SLICE1_ZOOMOUT, m_btnSlice1ZoomOut);
	DDX_Control(pDX, IDC_SLICE12_LIST, m_ctlSlice12List);
	DDX_Control(pDX, IDC_SLICE2, m_stcSlice2);
	DDX_Control(pDX, IDC_SLICE1, m_stcSlice1);
	DDX_Control(pDX, IDC_SLICE_SCROLLBAR, m_scrSliceBar);
	DDX_Text(pDX, IDC_SLICE_FIRSTDK, m_strSliceFirstDK);
	DDX_Text(pDX, IDC_SLICE_LASTDK, m_strSliceLastDK);
	DDX_Text(pDX, IDC_SLICE1_DK, m_strSlice1DK);
	DDX_Text(pDX, IDC_SLICE1_ZOOMFACTOR, m_strSlice1ZoomFactor);
	DDX_Text(pDX, IDC_SLICE2_DK, m_strSlice2DK);
	DDX_Text(pDX, IDC_SLICE2_ZOOMFACTOR, m_strSlice2ZoomFactor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgGraphObjLnk, CDialog)
	//{{AFX_MSG_MAP(DlgGraphObjLnk)
	ON_BN_CLICKED(IDC_BTN_LINKALL, OnBtnLinkAll)
	ON_BN_CLICKED(IDC_BTN_LINKCUR, OnBtnLinkCur)
	ON_BN_CLICKED(IDC_SLICE1_ZOOMIN, OnSlice1Zoomin)
	ON_BN_CLICKED(IDC_SLICE1_ZOOMOUT, OnSlice1Zoomout)
	ON_BN_CLICKED(IDC_SLICE2_ZOOMIN, OnSlice2Zoomin)
	ON_BN_CLICKED(IDC_SLICE2_ZOOMOUT, OnSlice2Zoomout)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_VALIDATE, OnValiDate)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgGraphObjLnk message handlers

BOOL DlgGraphObjLnk::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//////////////////////////////////////////////////////////////////////////
	m_Slice1.dbMinZoom = 1.0;
	m_Slice1.nIdx = -1;
	m_Slice1.pBtnZoomIn = &m_btnSlice1ZoomIn;
	m_Slice1.pBtnZoomOut = &m_btnSlice1ZoomOut;
	m_Slice1.pCanvas = NULL;
	m_Slice1.pStatic = &m_stcSlice1;
	m_Slice1.pStrDK = &m_strSlice1DK;
	m_Slice1.pStrZoomFactor = &m_strSlice1ZoomFactor;
	m_Slice1.bCusorArrow = TRUE;
	m_Slice1.bDrag = FALSE;

	m_Slice2.dbMinZoom = 1.0;
	m_Slice2.nIdx = -1;
	m_Slice2.pBtnZoomIn = &m_btnSlice2ZoomIn;
	m_Slice2.pBtnZoomOut = &m_btnSlice2ZoomOut;
	m_Slice2.pCanvas = NULL;
	m_Slice2.pStatic = &m_stcSlice2;
	m_Slice2.pStrDK = &m_strSlice2DK;
	m_Slice2.pStrZoomFactor = &m_strSlice2ZoomFactor;
	m_Slice2.bCusorArrow = TRUE;
	m_Slice2.bDrag = FALSE;
	//////////////////////////////////////////////////////////////////////////

	// ���ڵ������Ի����Ҫ����������2�������棬�����°�Ĭ����2������������ͼƬ����
	CTilCanvasMgr* pMgr = GetCanvasMgr();
	if (pMgr->GetCurCanvasIdx() < 0)
	{
		m_Slice1.nIdx = 0;
		m_Slice2.nIdx = 1;
	}
	else if (pMgr->GetCurCanvasIdx() < pMgr->Size()-1)
	{
		m_Slice1.nIdx = pMgr->GetCurCanvasIdx();
		m_Slice2.nIdx = m_Slice1.nIdx+1;
	}
	else // �����ǰ���������һ����̵Ļ���
	{
		m_Slice1.nIdx = pMgr->Size()-2;
		m_Slice2.nIdx = m_Slice1.nIdx+1;
	}
	InitSlice(m_Slice1);
	InitSlice(m_Slice2);

	// ��ʾ���������
	ShowSliceDK();

	UpdateData(FALSE);
	//////////////////////////////////////////////////////////////////////////

	// ����ˮƽ�������Ĺ�������
	SCROLLINFO scrInfo;
	m_scrSliceBar.GetScrollInfo(&scrInfo);
	scrInfo.nPage = 1;
	scrInfo.nMin = 0;
	scrInfo.nMax = pMgr->Size()>1?pMgr->Size()-2:0;
	scrInfo.nPos = m_Slice1.nIdx;
	m_scrSliceBar.SetScrollInfo(&scrInfo);

	// �����б����ʾ����
	m_ctlSlice12List.InsertColumn(0, "��ͼ", LVCFMT_LEFT, 45, 0);
	m_ctlSlice12List.InsertColumn(1, "��ͼ", LVCFMT_LEFT, 45, 1);
	// ��������
	CString str = "0123456789.";
	m_ctlSlice12List.SetColumnValidEditCtrlCharacters(str,0);
	m_ctlSlice12List.SetColumnValidEditCtrlCharacters(str,1);
	m_ctlSlice12List.SetReadOnlyColumns(0);
	m_ctlSlice12List.SetBkColor((COLORREF)GetSysColor(COLOR_GRAYTEXT));
	m_ctlSlice12List.EnableVScroll();
	m_ctlSlice12List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP);

	// ��ȡĿ�������������
	GetLinkData(pMgr, m_vcLink);

	// ��ʾ�б�����
	ShowListCtrlData();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DlgGraphObjLnk::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	DrawSlice(m_Slice1);
	DrawSlice(m_Slice2);

// 	CTilCanvas* pCanvas = new CTilCanvas(*GetCanvasMgr()->GetCanvasByIdx(0));
// 	pCanvas->Draw(&dc);
	// Do not call CDialog::OnPaint() for painting messages
}

void DlgGraphObjLnk::OnBtnLinkAll() 
{
	int i,j;
	int nMin, nMax, nPos;
	m_scrSliceBar.GetScrollRange(&nMin, &nMax);
	nPos = m_scrSliceBar.GetScrollPos();

	for (i=nMin; i<=nMax; i++)
	{
		SetSlice12Caption(i);

		LinkObjects(m_Slice1.pCanvas, m_Slice2.pCanvas, m_nGraphLayer);

		CGraph* pGraph1 = m_Slice1.pCanvas->GetGraph()->GetGraphByIdx(m_nGraphLayer);
		int nSize1 = pGraph1->Size();
		for (j=0; j<nSize1; j++)
		{
			CUnit* pUnit1 = pGraph1->GetUnitByIdx(j);
			m_vcLink[m_Slice1.nIdx][j].nNextLnk = pUnit1->GetNextLink();
		}

		CGraph* pGraph2 = m_Slice2.pCanvas->GetGraph()->GetGraphByIdx(m_nGraphLayer);
		int nSize2 = pGraph2->Size();
		for (j=0; j<nSize2; j++)
		{
			CUnit* pUnit2 = pGraph2->GetUnitByIdx(j);
			m_vcLink[m_Slice2.nIdx][j].nPrevLnk = pUnit2->GetPreLink();
		}
	}

	SetSlice12Caption(nPos);
	InvalidateSlice(m_Slice1);
	InvalidateSlice(m_Slice2);
	ShowListCtrlData();
}

void DlgGraphObjLnk::OnBtnLinkCur() 
{
	int i;
	// TODO: Add your control notification handler code here
	LinkObjects(m_Slice1.pCanvas, m_Slice2.pCanvas, m_nGraphLayer);

	CGraph* pGraph1 = m_Slice1.pCanvas->GetGraph()->GetGraphByIdx(m_nGraphLayer);
	int nSize1 = pGraph1->Size();
	for (i=0; i<nSize1; i++)
	{
		CUnit* pUnit1 = pGraph1->GetUnitByIdx(i);
		m_vcLink[m_Slice1.nIdx][i].nNextLnk = pUnit1->GetNextLink();
	}

	CGraph* pGraph2 = m_Slice2.pCanvas->GetGraph()->GetGraphByIdx(m_nGraphLayer);
	int nSize2 = pGraph2->Size();
	for (i=0; i<nSize2; i++)
	{
		CUnit* pUnit2 = pGraph2->GetUnitByIdx(i);
		m_vcLink[m_Slice2.nIdx][i].nPrevLnk = pUnit2->GetPreLink();
	}

	ShowListCtrlData();
}
// �Ŵ�
void DlgGraphObjLnk::OnSlice1Zoomin() 
{
	ZoomIn(m_Slice1);
}
// ��С
void DlgGraphObjLnk::OnSlice1Zoomout() 
{
	ZoomOut(m_Slice1);
}
// �Ŵ�
void DlgGraphObjLnk::OnSlice2Zoomin() 
{
	ZoomIn(m_Slice2);
}
// ��С
void DlgGraphObjLnk::OnSlice2Zoomout() 
{
	ZoomOut(m_Slice2);
}

void DlgGraphObjLnk::OnOK() 
{
	// TODO: Add extra validation here
	CTilCanvasMgr* pMgr = GetCanvasMgr();
	SetLinkData(pMgr, m_vcLink);

	CDialog::OnOK();
}

void DlgGraphObjLnk::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	int nMinPos, nMaxPos, nCurPos;
	pScrollBar->GetScrollRange(&nMinPos, &nMaxPos);
	nCurPos = pScrollBar->GetScrollPos();
	CTilCanvasMgr* pMgr = GetCanvasMgr();

	// ��ֹ���뽹�����б����,֪ͨCComboListCtrl::OnEndLabelEdit()
	SetFocus();

	switch (nSBCode)
	{
		case SB_PAGELEFT:
		case SB_LINELEFT:
		case SB_LEFT:
			if (nCurPos > nMinPos)
			{
				pScrollBar->SetScrollPos(--nCurPos);
				SetSlice12Caption(nCurPos);
				InvalidateSlice(m_Slice1);
				InvalidateSlice(m_Slice2);
			}
			break;
		case SB_PAGERIGHT:
		case SB_LINERIGHT:
		case SB_RIGHT:
			if (nCurPos < nMaxPos)
			{
				pScrollBar->SetScrollPos(++nCurPos);
				SetSlice12Caption(nCurPos);
				InvalidateSlice(m_Slice1);
				InvalidateSlice(m_Slice2);
			}
			break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			pScrollBar->SetScrollPos(nPos);
			SetSlice12Caption(nPos);
			InvalidateSlice(m_Slice1);
			InvalidateSlice(m_Slice2);
			break;
	}

	// ������ʾ��������̺��б��������
	ShowSliceDK();
	ShowListCtrlData();

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

// ��������ʵ�����
//int DlgGraphObjLnk::GetZoomIdx(long double dbZoom)
//{
//	ASSERT(dbZoom >= m_dbZooms[0] && dbZoom <= m_dbZooms[20]);
//
//	for (int i = 0; i < 20; i ++)
//	{
//		if (dbZoom >= m_dbZooms[i] && dbZoom < m_dbZooms[i+1])
//			return i;
//	}
//	return i;
//}

// ��ʾ���������
void DlgGraphObjLnk::ShowSliceDK()
{
	CTilCanvasMgr* pMgr = GetCanvasMgr();
	if (pMgr->Size() <= 0)
		return;
	else if (pMgr->Size() == 1)
	{
		m_strSlice1DK = get_file_name(pMgr->GetCanvasByIdx(m_Slice1.nIdx)->GetImage()->GetPathName());
		m_strSliceFirstDK = get_file_name(pMgr->GetCanvasByIdx(0)->GetImage()->GetPathName());
		m_strSliceLastDK = m_strSliceFirstDK;
	} else
	{
		m_strSlice1DK = get_file_name(pMgr->GetCanvasByIdx(m_Slice1.nIdx)->GetImage()->GetPathName());
		m_strSlice2DK = get_file_name(pMgr->GetCanvasByIdx(m_Slice2.nIdx)->GetImage()->GetPathName());
		m_strSliceFirstDK = get_file_name(pMgr->GetCanvasByIdx(0)->GetImage()->GetPathName());
		m_strSliceLastDK = get_file_name(pMgr->GetCanvasByIdx(pMgr->Size()-1)->GetImage()->GetPathName());
	}
}

// �����б����ʾ����
void DlgGraphObjLnk::ShowListCtrlData()
{
	m_ctlSlice12List.DeleteAllItems();

	CString str;
	int i;
	for (i = 0; i < m_vcLink[m_Slice1.nIdx].size(); i ++)
	{
		str.Format("%d", i+1);
		m_ctlSlice12List.InsertItem(i, str);
		str.Format("%d", m_vcLink[m_Slice1.nIdx][i].nNextLnk+1);
		m_ctlSlice12List.SetItemText(i, 1, str);
	}
}

// ����ÿ�θı仭�����ʱ�����뻭������ʼ����ʾ����
void DlgGraphObjLnk::InitSlice(SLICE& slice)
{
	CTilCanvasMgr* pMgr = GetCanvasMgr();
	ASSERT(slice.nIdx >= 0 && slice.nIdx < pMgr->Size());

	// ���뻭��
	if (slice.pCanvas) delete slice.pCanvas;
	slice.pCanvas = new CTilCanvas(*pMgr->GetCanvasByIdx(slice.nIdx));
	slice.pCanvas->SetZoomFactor(1.0, 1.0);

	// ����ͼԪ��ʾ���ڵ���ͼΪ��ǰ�Ի�����ͼ
	CTilGraph* pGraphMgr = slice.pCanvas->GetGraph();
	for (int i = 0; i < pGraphMgr->Size(); i ++)
	{
		CGraph* pGraph = pGraphMgr->GetGraphByIdx(i);
		for (int j=0; j<pGraph->Size(); j++)
		{
			CUnit* pUnit = pGraph->GetUnitByIdx(j);
			pUnit->m_pView = (CView*)this;
		}
	}

	CRect rect;
	slice.pStatic->GetClientRect(&slice.rtStatic);

	long double dbZoomX, dbZoomY;
	dbZoomX = slice.rtStatic.Width()*1.0/slice.pCanvas->GetWidth();
	dbZoomY = slice.rtStatic.Height()*1.0/slice.pCanvas->GetHeight();
	if (dbZoomX < dbZoomY)
		slice.dbMinZoom = dbZoomX;
	else
		slice.dbMinZoom = dbZoomY;

	// �������ԭʼ�ߴ�С������С������С������Ϊ1.0
	if (slice.pCanvas->GetWidth() <= slice.rtStatic.Width()
		&& slice.pCanvas->GetHeight() <= slice.rtStatic.Height())
		slice.dbMinZoom = 1.0;

	slice.pCanvas->SetZoomFactor(slice.dbMinZoom, slice.dbMinZoom);

	// �󻭲������Ľ���
	slice.rtCanvas = CRect(0, 0, slice.pCanvas->GetWidth()*slice.dbMinZoom,
								slice.pCanvas->GetHeight()*slice.dbMinZoom);
	slice.ptCanvasCenter = slice.rtCanvas.CenterPoint();
	GetIntersectRect(slice);

	// ��ʾ�����ʡ����ð�ť״̬
	long double dbZoom;
	slice.pCanvas->GetZoomFactor(dbZoom, dbZoom);
	slice.pStrZoomFactor->Format("%d%%", int(dbZoom*100));
	slice.pBtnZoomOut->EnableWindow(FALSE);
	slice.pBtnZoomIn->EnableWindow(TRUE);
}

void DlgGraphObjLnk::SetSlice12Caption(int nIdx1)
{
	// �����������������2���򻭲������б仯���ʲ��Ի���������
	CTilCanvasMgr* pMgr = GetCanvasMgr();
	if (pMgr->Size() <= 2)
		return;

	ASSERT(nIdx1 >= 0 && nIdx1 < pMgr->Size()-1);

	// �����һ����������
	m_Slice1.nIdx = nIdx1;
	InitSlice(m_Slice1);

	// ����ڶ�����������
	int nIdx2 = nIdx1+1;
	if (nIdx2 < pMgr->Size())
	{
		m_Slice2.nIdx = nIdx2;
		InitSlice(m_Slice2);
	}

	UpdateData(FALSE);
}

// ����ͼ��
void DlgGraphObjLnk::InvalidateSlice(SLICE& slice)
{
	CRect rect;
	slice.pStatic->GetClientRect(&rect);
	slice.pStatic->ClientToScreen(&rect);
	ScreenToClient(&rect);
	InvalidateRect(&rect);
}

// ��ȡͼ�������Ľ�����ֻ�ڽ���������ʾͼ������
BOOL DlgGraphObjLnk::GetIntersectRect(SLICE& slice)
{
//	ASSERT(slice.rtCanvas.PtInRect(slice.ptCanvasCenter));

	CRect rectC, rectS, rectTmpC, rectTmpS;
	rectTmpC = slice.rtCanvas;
	rectTmpS = slice.rtStatic;

	// �󻭲�����ʾ����
	CPoint ptCenter = slice.rtStatic.CenterPoint();
	rectTmpS.OffsetRect(slice.ptCanvasCenter.x-ptCenter.x, slice.ptCanvasCenter.y-ptCenter.y);
	rectC.IntersectRect(&slice.rtCanvas, &rectTmpS);
	slice.rtCanvasShow = rectC;
//	TRACE("rectTmpS(%d,%d,%d,%d)\n", rectTmpS.left, rectTmpS.top, rectTmpS.right, rectTmpS.bottom);

	// �������ʾ����������
	rectTmpC.OffsetRect(ptCenter.x-slice.ptCanvasCenter.x, ptCenter.y-slice.ptCanvasCenter.y);
	rectS.IntersectRect(&slice.rtStatic, &rectTmpC);
	slice.rtStaticShow = rectS;

	// �����ʾͼ�񳬳�������Χ����ǿ�н�ͼ����ʾ�޶��ڻ�����Χ��
	// �����޽�
	if (slice.rtStaticShow.left > 0 && slice.rtCanvasShow.right < slice.rtCanvas.right)
	{
		// slice.ptMoveAt.x += -slice.rtCanvasShow.left;
		slice.ptCanvasCenter.x += slice.rtStaticShow.left;
		slice.rtCanvasShow.right += slice.rtStaticShow.left;
		slice.rtStaticShow.left = 0;
	}
	else if (slice.rtStaticShow.right < slice.rtStatic.right && slice.rtCanvasShow.left > 0)
	{
		//slice.ptMoveAt.x -= slice.rtStatic.right-slice.rtStaticShow.right;
		slice.ptCanvasCenter.x -= slice.rtStatic.right-slice.rtStaticShow.right;
		slice.rtCanvasShow.left -= slice.rtStatic.right-slice.rtStaticShow.right;
		slice.rtStaticShow.right = slice.rtStatic.right;
	}

	// ����������ұ߽���ʾ��������򽫻�����ֱ���з���
	if (slice.rtCanvasShow.left == slice.rtCanvas.left
		&& slice.rtCanvasShow.right == slice.rtCanvas.right)
	{
		slice.rtStaticShow.left = slice.rtStatic.CenterPoint().x-slice.rtCanvas.Width()/2;
		slice.rtStaticShow.right = slice.rtStatic.CenterPoint().x+slice.rtCanvas.Width()/2;
	}

	// �����޽�
	if (slice.rtStaticShow.top > 0 && slice.rtCanvasShow.bottom < slice.rtCanvas.bottom)
	{
		//slice.ptMoveAt.y += -slice.rtCanvasShow.top;
		slice.ptCanvasCenter.y += slice.rtStaticShow.top;
		slice.rtCanvasShow.bottom += slice.rtStaticShow.top;
		slice.rtStaticShow.top = 0;
	}
	else if (slice.rtStaticShow.bottom < slice.rtStatic.bottom && slice.rtCanvasShow.top > 0)
	{
		//slice.ptMoveAt.y -= slice.rtCanvasShow.bottom-slice.pCanvas->GetHeight();
		slice.ptCanvasCenter.y -= slice.rtStatic.bottom-slice.rtStaticShow.bottom;
		slice.rtCanvasShow.top -= slice.rtStatic.bottom-slice.rtStaticShow.bottom;
		slice.rtStaticShow.bottom = slice.rtStatic.bottom;
	}

	// ����������±߽���ʾ��������򽫻���ˮƽ���з���
	if (slice.rtCanvasShow.top == slice.rtCanvas.top
		&& slice.rtCanvasShow.bottom == slice.rtCanvas.bottom)
	{
		slice.rtStaticShow.top = slice.rtStatic.CenterPoint().y-slice.rtCanvas.Height()/2;
		slice.rtStaticShow.bottom = slice.rtStatic.CenterPoint().y+slice.rtCanvas.Height()/2;
	}

	return TRUE;
}

void DlgGraphObjLnk::ZoomIn(SLICE& slice)
{
	long double dbZoom;
	slice.pCanvas->GetZoomFactor(dbZoom, dbZoom);

	// ������С���������Ϊ1.0�����ԣ�dbZoom������1.0
	if (dbZoom <= slice.dbMinZoom)
	{
		// ����պñ�m_dbMinZoom1�����һ��������
		long double dbZoomTmp = 1.0;
		while (dbZoomTmp > dbZoom)
			dbZoomTmp /= 1.25;
		dbZoom = dbZoomTmp*1.25;

		slice.pCanvas->SetZoomFactor(dbZoom, dbZoom);

		// ͼ��Ŵ�󣬻������ĵ�����Ҳ�Ŵ���
		dbZoom /= slice.dbMinZoom;
		slice.ptCanvasCenter = CPoint(slice.ptCanvasCenter.x*dbZoom+0.5,
																	slice.ptCanvasCenter.y*dbZoom+0.5);
		slice.pBtnZoomOut->EnableWindow(TRUE);

		// ������������֮��Ľ���
		slice.rtCanvas = CRect(0, 0, slice.pCanvas->GetWidth(), slice.pCanvas->GetHeight());
		slice.pStatic->GetClientRect(slice.rtStatic);
		GetIntersectRect(slice);
	}
	else if (dbZoom < Max_Zoom_Factor)
	{
		dbZoom *= 1.25;
		slice.pCanvas->SetZoomFactor(dbZoom, dbZoom);
		slice.ptCanvasCenter = CPoint(slice.ptCanvasCenter.x*1.25+0.5,
																	slice.ptCanvasCenter.y*1.25+0.5);

		// ������������֮��Ľ���
		slice.rtCanvas = CRect(0, 0, slice.pCanvas->GetWidth(), slice.pCanvas->GetHeight());
		slice.pStatic->GetClientRect(slice.rtStatic);
		GetIntersectRect(slice);

		if (dbZoom >= Max_Zoom_Factor)
			slice.pBtnZoomIn->EnableWindow(FALSE);
	}

	// ��ʾ������
	slice.pCanvas->GetZoomFactor(dbZoom, dbZoom);
	slice.pStrZoomFactor->Format("%d%%", int(dbZoom*100));
	UpdateData(FALSE);

	InvalidateSlice(slice);
}

void DlgGraphObjLnk::ZoomOut(SLICE& slice)
{
	long double dbZoom;
	slice.pCanvas->GetZoomFactor(dbZoom, dbZoom);

	// ������������ʱ���������ʼ�С��Ŵ�ť��Ч
	if (dbZoom >= Max_Zoom_Factor)
	{
		dbZoom /= 1.25;
		slice.pCanvas->SetZoomFactor(dbZoom, dbZoom);
		slice.ptCanvasCenter = CPoint(slice.ptCanvasCenter.x/1.25+0.5,
																	slice.ptCanvasCenter.y/1.25+0.5);

		// ������������֮��Ľ���
		slice.rtCanvas = CRect(0, 0, slice.pCanvas->GetWidth(), slice.pCanvas->GetHeight());
		slice.pStatic->GetClientRect(slice.rtStatic);
		GetIntersectRect(slice);
	}
	else if (dbZoom > slice.dbMinZoom)
	{
		dbZoom /= 1.25;
		if (dbZoom >= slice.dbMinZoom)
		{
			slice.pCanvas->SetZoomFactor(dbZoom, dbZoom);
			slice.ptCanvasCenter = CPoint(slice.ptCanvasCenter.x/1.25+0.5,
																		slice.ptCanvasCenter.y/1.25+0.5);

			// ������������֮��Ľ���
			slice.rtCanvas = CRect(0, 0, slice.pCanvas->GetWidth(), slice.pCanvas->GetHeight());
			slice.pStatic->GetClientRect(slice.rtStatic);
			GetIntersectRect(slice);
		}
		else
		{
			slice.pCanvas->SetZoomFactor(slice.dbMinZoom, slice.dbMinZoom);
			slice.pBtnZoomOut->EnableWindow(FALSE);

			dbZoom = dbZoom*1.25/slice.dbMinZoom;
			slice.ptCanvasCenter = CPoint(slice.ptCanvasCenter.x/dbZoom+0.5,
																		slice.ptCanvasCenter.y/dbZoom+0.5);

			// ������������֮��Ľ���
			slice.rtCanvas = CRect(0, 0, slice.pCanvas->GetWidth(), slice.pCanvas->GetHeight());
			slice.pStatic->GetClientRect(slice.rtStatic);
			GetIntersectRect(slice);
		}
	}

	slice.pCanvas->GetZoomFactor(dbZoom, dbZoom);
	slice.pStrZoomFactor->Format("%d%%", int(dbZoom*100));
	UpdateData(FALSE);

	InvalidateSlice(slice);
}

// ����CPolygon�ı��
void DlgGraphObjLnk::DrawNum(CDC* pDC, SLICE& slice)
{
	int nNum = 0;
	CTilCanvas* pCanvas = slice.pCanvas;
	CTilGraph* pGraphMgr = pCanvas->GetGraph();

	// �õ��Ҳ�Ŀ���
	CGraph* pGraph = pGraphMgr->GetGraphByIdx(m_nGraphLayer);
	for (int i=0; i<pGraph->Size(); i++)
	{
		CUnit* pUnit = pGraph->GetUnitByIdx(i);
		if (!pUnit->IsKindOf(RUNTIME_CLASS(CPolygon)))
			continue;

		nNum ++;

		// �ҵ�CPolygonͼԪ�е���͵�
		POINT2DF pt = pUnit->m_pPointList->GetAt(0);
		double dbMinY = pt.y;
		int nIdx = 0;
		for (int j = 1; j < pUnit->m_pPointList->GetSize(); j ++)
		{
			pt = pUnit->m_pPointList->GetAt(j);
			if (dbMinY > pt.y)
			{
				dbMinY = pt.y;
				nIdx = j;
			}
		}

		// �ҵ�Ŀ�����β�����Ŀ�����
		pt = pUnit->m_pPointList->GetAt(nIdx);
		CPoint point = CPoint(pt.x*pUnit->m_dbZoomX+0.5, pt.y*pUnit->m_dbZoomY+0.5);

		CString str;
		str.Format("%d", nNum);

		pDC->SetTextAlign(TA_CENTER);
//		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(point.x, point.y, str);
	}
}

// ������������ʾ��ͼ�񲿷�
void DlgGraphObjLnk::DrawSlice(SLICE& slice)
{
	// �����ǰ����������,���ܻ�ͼ,ֱ�ӷ���
	if (slice.nIdx < 0)
		return;

	CDC* pDC = slice.pStatic->GetDC();
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pDC, slice.pCanvas->GetWidth(), slice.pCanvas->GetHeight());

	CBitmap* pOldBitmap = memDC.SelectObject(&memBitmap);

	// ���ضԽ�����������β����ʾ
// 	slice.pCanvas->GetGraph()->GetGraphByIdx(GRAPH_LAYER_JOINT)->SetShowMode(FALSE);
// 	slice.pCanvas->GetGraph()->GetGraphByIdx(GRAPH_LAYER_TUNARC)->SetShowMode(FALSE);

	// �����Ҳ�Ŀ���͸��Ҳ�����
	// �����Ҳ�Ŀ�����ɫ
	CGraph* pGraph = slice.pCanvas->GetGraph()->GetGraphByIdx(m_nGraphLayer);
	for (int i=0; i<pGraph->Size(); i++)
	{
		CUnit* pUnit = pGraph->GetUnitByIdx(i);
		pUnit->m_PenColor = RGB(255,255,0);
	}
	slice.pCanvas->Draw(&memDC);
	DrawNum(&memDC, slice);
//
//	// ���������ԭ������ʾ��Χ��ƫ����
//	CPoint ptOffset = slice.rtCanvasShow.CenterPoint()-slice.ptCanvasCenter;
//	slice.rtCanvasShow.OffsetRect(ptOffset);

	// �����ʾ���ֵ�ͼ��
	pDC->BitBlt(slice.rtStaticShow.left, slice.rtStaticShow.top,
		slice.rtStaticShow.Width(), slice.rtStaticShow.Height(),
		&memDC, slice.rtCanvasShow.left, slice.rtCanvasShow.top, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
}

void DlgGraphObjLnk::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// �����һ��ͼ�����������״���Ǳ�׼�ģ�������ڵ�һ��ͼ��
	if (m_Slice1.nIdx >= 0 && !m_Slice1.bCusorArrow)
	{
		m_Slice1.bDrag = TRUE;
		m_Slice1.ptBegin = point;
	}

	// �����һ��ͼ�����������״���Ǳ�׼�ģ�������ڵ�һ��ͼ��
	if (m_Slice2.nIdx >= 0 && !m_Slice2.bCusorArrow)
	{
		m_Slice2.bDrag = TRUE;
		m_Slice2.ptBegin = point;
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void DlgGraphObjLnk::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// ���ͼ��1�����϶�״̬
	if (m_Slice1.nIdx >= 0 && !m_Slice1.bCusorArrow)
	{
		m_Slice1.bDrag = FALSE;
		SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
		m_Slice1.bCusorArrow = TRUE;
	}

	// ���ͼ��2�����϶�״̬
	if (m_Slice2.nIdx >= 0 && !m_Slice2.bCusorArrow)
	{
		m_Slice2.bDrag = FALSE;
		SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
		m_Slice2.bCusorArrow = TRUE;
	}

	CDialog::OnLButtonUp(nFlags, point);
}

void DlgGraphObjLnk::OnMouseMove(UINT nFlags, CPoint point) 
{
	// �ж������Ƿ���ͼ��1��
	if (m_Slice1.nIdx >= 0)
	{
		// ������϶����򱣳���ǰ�������״���ƶ�ͼ��
		if (m_Slice1.bDrag)
		{
			m_Slice1.ptMoveAt = point;

			// ���¼��㻭�������Ľ���
			if (m_Slice1.pCanvas->GetWidth() > m_Slice1.rtStatic.Width())
				m_Slice1.ptCanvasCenter.Offset(0, m_Slice1.ptBegin.y-point.y);
			if (m_Slice1.pCanvas->GetHeight() > m_Slice1.rtStatic.Height())
				m_Slice1.ptCanvasCenter.Offset(m_Slice1.ptBegin.x-point.x, 0);
			CPoint pt = m_Slice1.ptBegin-point;
//			TRACE("ƽ�ƾ���x=%d, y=%d\n", pt.x, pt.y);
			GetIntersectRect(m_Slice1);
			InvalidateSlice(m_Slice1);
		}
		else
		{
			CRect rect1 = m_Slice1.rtStatic;
			m_Slice1.pStatic->ClientToScreen(&rect1);
			ScreenToClient(&rect1);
			if (rect1.PtInRect(point))
			{
				SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEALL)));
				m_Slice1.bCusorArrow = FALSE;
				return;
			}
			else
			{
				SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
				m_Slice1.bCusorArrow = TRUE;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// �ж������Ƿ���ͼ��1��
	if (m_Slice2.nIdx >= 0)
	{
		// ������϶����򱣳���ǰ�������״���ƶ�ͼ��
		if (m_Slice2.bDrag)
		{
			m_Slice2.ptMoveAt = point;

			// ���¼��㻭�������Ľ���
			if (m_Slice2.pCanvas->GetWidth() > m_Slice2.rtStatic.Width())
				m_Slice2.ptCanvasCenter.Offset(0, m_Slice2.ptBegin.y-point.y);
			if (m_Slice2.pCanvas->GetHeight() > m_Slice2.rtStatic.Height())
				m_Slice2.ptCanvasCenter.Offset(m_Slice2.ptBegin.x-point.x, 0);
			CPoint pt = m_Slice2.ptBegin-point;
//			TRACE("ƽ�ƾ���x=%d, y=%d\n", pt.x, pt.y);
			GetIntersectRect(m_Slice2);
			InvalidateSlice(m_Slice2);
		}
		else
		{
			CRect rect2 = m_Slice2.rtStatic;
			m_Slice2.pStatic->ClientToScreen(&rect2);
			ScreenToClient(&rect2);
			if (rect2.PtInRect(point))
			{
				SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEALL)));
				m_Slice2.bCusorArrow = FALSE;
				return;
			}
			else
			{
				SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
				m_Slice2.bCusorArrow = TRUE;
			}
		}
	}

	CDialog::OnMouseMove(nFlags, point);
}

// ��֤������Ͽ����Ϣ����Ч��
// wParam:��Ͽ���
// lParam:#define LV_DISPINFO NMLVDISPINFO��������
LRESULT DlgGraphObjLnk::OnValiDate(WPARAM wParam,LPARAM lParam)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)lParam;

	int nObjIdx = atoi(pDispInfo->item.pszText);
	if (pDispInfo->item.iSubItem == 0)
	{
		if (nObjIdx > m_vcLink[m_Slice1.nIdx].size())
		{
//			MessageBox("��ͼ��ָ��Ŀ�겻����!", "����", MB_OK | MB_ICONHAND);
			m_ctlSlice12List.UpdateData(TRUE);
			return 0;
		}
		m_vcLink[m_Slice2.nIdx][pDispInfo->item.iItem].nPrevLnk = nObjIdx-1;
		m_vcLink[m_Slice1.nIdx][nObjIdx-1].nNextLnk = pDispInfo->item.iItem;
	}	else if (pDispInfo->item.iSubItem == 1)
	{
		if (nObjIdx > m_vcLink[m_Slice2.nIdx].size())
		{
//			MessageBox("��ͼ��ָ��Ŀ�겻����!", "����", MB_OK | MB_ICONHAND);
			m_ctlSlice12List.UpdateData(TRUE);
			return 0;
		}

		// ��Ϊ�洢������0��ʼ����ţ���ʵ��Ŀ��Ҫ��1
		m_vcLink[m_Slice1.nIdx][pDispInfo->item.iItem].nNextLnk = nObjIdx-1;
		m_vcLink[m_Slice2.nIdx][nObjIdx-1].nPrevLnk = pDispInfo->item.iItem; 
	}

	return 0;
}

// ����Ƭǰ�����������ݱ�����vcLink��
void DlgGraphObjLnk::GetLinkData(CTilCanvasMgr* pMgr, vector< vector<LINKOBJ> > &vcLink)
{
	int i, j;

	for (i = 0; i < pMgr->Size(); i ++)
	{
		CTilCanvas* pCanvas = pMgr->GetCanvasByIdx(i);
		CTilGraph* pGraphMgr = pCanvas->GetGraph();
		vector<LINKOBJ> vcTemp;
		LINKOBJ linkObj;

		CGraph* pGraph = pGraphMgr->GetGraphByIdx(m_nGraphLayer);
		for (j=0; j<pGraph->Size(); j++)
		{
			CUnit* pUnit = pGraph->GetUnitByIdx(j);
			linkObj.nPrevLnk = pUnit->GetPreLink();
			linkObj.nNextLnk = pUnit->GetNextLink();
			vcTemp.push_back(linkObj);
		}
		vcLink.push_back(vcTemp);
	}
}

// ��������vcLink�е�����д��pMgr��
void DlgGraphObjLnk::SetLinkData(CTilCanvasMgr* pMgr, vector< vector<LINKOBJ> > &vcLink)
{
	ASSERT(pMgr->Size() == vcLink.size());

	int i, j;

	for (i = 0; i < pMgr->Size(); i ++)
	{
		CTilCanvas* pCanvas = pMgr->GetCanvasByIdx(i);
		CTilGraph* pGraphMgr = pCanvas->GetGraph();

		CGraph* pGraph = pGraphMgr->GetGraphByIdx(m_nGraphLayer);
		ASSERT(pGraph->Size() == vcLink[i].size());

		for (j=0; j<pGraph->Size(); j++)
		{
			LINKOBJ linkObj;
			linkObj.nPrevLnk = vcLink[i][j].nPrevLnk;
			linkObj.nNextLnk = vcLink[i][j].nNextLnk;

			CUnit* pUnit = pGraph->GetUnitByIdx(j);
			pUnit->SetPreLink(vcLink[i][j].nPrevLnk);
			pUnit->SetNextLink(vcLink[i][j].nNextLnk);
		}
	}
}

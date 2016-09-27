#if !defined(AFX_DLGGRAPHOBJLNK_H__638F7E47_48E5_4ED1_8B4D_7A689B32A2B5__INCLUDED_)
#define AFX_DLGGRAPHOBJLNK_H__638F7E47_48E5_4ED1_8B4D_7A689B32A2B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGraphObjLnk.h : header file
//

#include <vector>
using namespace std;

typedef struct __tagLINKOBJ
{
	int nPrevLnk;
	int nNextLnk;
} LINKOBJ, *PLINKOBJ, **PPLINKOBJ;

#include "ComboListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// DlgGraphObjLnk dialog

class DlgGraphObjLnk : public CDialog
{
// Construction
public:
	DlgGraphObjLnk(CWnd* pParent = NULL);   // standard constructor

	void SetCurGraphLayer(int nGraphLayer);
	int m_nGraphLayer;

	virtual ~DlgGraphObjLnk();

	friend class CUnit;
// Dialog Data
	//{{AFX_DATA(DlgGraphObjLnk)
	enum { IDD = IDD_GRAPH_OBJLNK };
	CButton	m_btnLinkCur;
	CButton	m_btnLinkAll;
	CButton	m_btnSlice1ZoomIn;
	CButton	m_btnSlice2ZoomOut;
	CButton	m_btnSlice2ZoomIn;
	CButton	m_btnSlice1ZoomOut;
	CComboListCtrl	m_ctlSlice12List;
	CStatic	m_stcSlice2;
	CStatic	m_stcSlice1;
	CScrollBar	m_scrSliceBar;
	CString	m_strSliceFirstDK;
	CString	m_strSliceLastDK;
	CString	m_strSlice1DK;
	CString	m_strSlice1ZoomFactor;
	CString	m_strSlice2DK;
	CString	m_strSlice2ZoomFactor;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgGraphObjLnk)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	afx_msg LRESULT OnValiDate(WPARAM,LPARAM);

// Implementation
protected:
//	long double m_dbZooms[21];
//	int GetZoomIdx(long double dbZoom);
	typedef struct _tagSLICE
	{
		CTilCanvas* pCanvas;		// ����
		long double dbMinZoom;		// ������
		int nIdx;					// ��������
		CRect rtCanvas;				// �����е���ʾ������Ӿ���
		CRect rtCanvasShow;			// ��ʾ�Ļ�������
		CPoint ptCanvasCenter;		// ���ڱ�ǵ�ǰ��ʾ��������ĵ�

		CPoint ptBegin;				// �϶���ʼ��
		CPoint ptMoveAt;			// �ƶ����ĵ�
		BOOL bCusorArrow;			// �ж��Ƿ��Ǳ�׼�����״
		BOOL bDrag;					// �����Ƿ��϶�

		CStatic* pStatic;			// ���
		CRect rtStatic;				// ����е���ʾ����
		CRect rtStaticShow;			// �������ʾͼ�������

		CButton* pBtnZoomIn;		// �Ŵ�ť
		CButton* pBtnZoomOut;		// ��С��ť
		CString* pStrZoomFactor;	// ������
		CString* pStrDK;			// ���
	} SLICE, *PSLICE;
	SLICE m_Slice1, m_Slice2;

	void InitSlice(SLICE& slice);
	void SetSlice12Caption(int nIdx1);
	void InvalidateSlice(SLICE& slice);
	BOOL GetIntersectRect(SLICE& slice);
	void ZoomIn(SLICE& slice);
	void ZoomOut(SLICE& slice);
	void DrawSlice(SLICE& slice);
	void DrawNum(CDC* pDC, SLICE& slice);

	void ShowSliceDK();
	void ShowListCtrlData();

	vector< vector<LINKOBJ> > m_vcLink;
	void GetLinkData(CTilCanvasMgr* pMgr, vector< vector<LINKOBJ> > &vcLink);
	void SetLinkData(CTilCanvasMgr* pMgr, vector< vector<LINKOBJ> > &vcLink);

	// Generated message map functions
	//{{AFX_MSG(DlgGraphObjLnk)
	afx_msg void OnBtnLinkAll();
	afx_msg void OnBtnLinkCur();
	afx_msg void OnSlice1Zoomin();
	afx_msg void OnSlice1Zoomout();
	afx_msg void OnSlice2Zoomin();
	afx_msg void OnSlice2Zoomout();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGRAPHOBJLNK_H__638F7E47_48E5_4ED1_8B4D_7A689B32A2B5__INCLUDED_)

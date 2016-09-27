// TilSysView.h : interface of the CSysView class
//
/////////////////////////////////////////////////////////////////////////////

/************************************
  REVISION LOG ENTRY
  维护人员: Leng
  维护时间 2008-9-6 12:34:10
  说明: 将该类作为CTilSysView类的基类
 ************************************/

#if !defined(SYSVIEW_H)
#define SYSVIEW_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph/Unit.h"
#include "TilSysDoc.h"
#include "FaceInfoList.h"

class CSysView : public CScrollView
{
protected: // create from serialization only
	CSysView();
	DECLARE_DYNCREATE(CSysView)

// Attributes
public:
	CTilSysDoc* GetDocument();
//	int mPageWidth, mPageHeight;
	// 图形属性
	GRAPHCAPTION m_GraphCaption;
	deque<CFaceCaption> m_dFaceCaption;

	// 判断是否当前掌子面断面图形被选中移动(08.11.07)
	BOOL m_bIsTunArcMoved;
	// 判断是否选中了同一图层上的图形(11.12.20)
	BOOL m_isSameSelLayer;

// Operations
public:
	POINT2DF GetSrcPoint(const CPoint& point);	// 取得缩放前的点
	CPoint GetDstPoint(const POINT2DF& point);	// 取得缩放后的点
	void InvalidateMovingRect(CUnit *pMovingUnit);
	void DeleteSelectedGraph();
	void SetMousePosTextColor(CPoint Logpoint);
	void ComputeMinandMaxPoint(CPoint point);
	void ResetSelectStatus();

	void UpdateRulersInfo(int nMessage, CPoint ScrollPos, CPoint Pos = CPoint(0, 0));

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSysView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL
	afx_msg LRESULT OnChangeItem(WPARAM,LPARAM);
	afx_msg LRESULT OnSelChanged(WPARAM,LPARAM);
	afx_msg LRESULT OnSelDeleted(WPARAM,LPARAM);
	afx_msg LRESULT OnRecogTexture(WPARAM,LPARAM);
	afx_msg LRESULT OnAddToFace(WPARAM, LPARAM);
	afx_msg LRESULT OnNewImage(WPARAM, LPARAM);

// Implementation
public:
	virtual ~CSysView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSysView)
	afx_msg void OnFilePrintPreview();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnGraphArrow();
	afx_msg void OnGraphLine();
	afx_msg void OnGraphPolygon();
	afx_msg void OnGraphMagic();
	afx_msg void OnGraphMove();
	afx_msg void OnGraphText();
	afx_msg void OnImportTunArc();
	afx_msg void OnCharItalic();
	afx_msg void OnCharUnderline();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnGraphObjlink();
	afx_msg void OnUpdateGraphObjlink(CCmdUI* pCmdUI);
	afx_msg void OnImageDistortCorrect();
	afx_msg void OnImageGeometryCorrect();
	afx_msg void OnUpdateImageDistortCorrect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateImageGeometryCorrect(CCmdUI* pCmdUI);
	afx_msg void OnImageConcat();
	afx_msg void OnUpdateGraphArrow(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGraphLine(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGraphPolygon(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGraphMagic(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGraphMove(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGraphText(CCmdUI* pCmdUI);
	afx_msg void OnUpdateImportTunArc(CCmdUI* pCmdUI);
	afx_msg void OnGraphTunside();
	afx_msg void OnUpdateGraphTunside(CCmdUI* pCmdUI);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnFaceCaptionCur();
	afx_msg void OnFaceCaptionAll();
	afx_msg void OnGraphClear();
	afx_msg void OnUpdateGraphClear(CCmdUI* pCmdUI);
	afx_msg void OnGraphEdge();
	afx_msg void OnUpdateGraphEdge(CCmdUI* pCmdUI);
	afx_msg void OnGraphExport();
	afx_msg void OnUpdateGraphExport(CCmdUI* pCmdUI);
	afx_msg void OnGraphExportAll();
	afx_msg void OnUpdateGraphExportAll(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCharItalic(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCharUnderline(CCmdUI* pCmdUI);
	afx_msg void OnCharBold();
	afx_msg void OnUpdateCharBold(CCmdUI* pCmdUI);
	afx_msg void OnFaceInfo();
	afx_msg void OnUpdateFaceInfo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFaceCaptionCur(CCmdUI* pCmdUI);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnExportAll();
	afx_msg void OnUpdateExportAll(CCmdUI* pCmdUI);
	afx_msg void OnExportCurGraphImage();
	afx_msg void OnUpdateExportCurGraphImage(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TilSysView.cpp
inline CTilSysDoc* CSysView::GetDocument()
   { return (CTilSysDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TILSYSVIEW_H__81A627F9_2DE2_434A_80DF_AD96683EB026__INCLUDED_)

#if !defined(AFX_GRAGTREE2_H__6511A6EE_992D_487E_9D29_51BD0ECE39E0__INCLUDED_)
#define AFX_GRAGTREE2_H__6511A6EE_992D_487E_9D29_51BD0ECE39E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDragTree2.h : header file
//

#include "GuiEdit.h"
/////////////////////////////////////////////////////////////////////////////
// CDragTree2 window

class CDragTree2 : public CTreeCtrl
{
// Construction
public:
	CDragTree2();


// Attributes
public:
	void SetDefaultCursor() ;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDragTree2)
	//}}AFX_VIRTUAL

//*********** CURSORS  *********************
	HCURSOR cursor_hand ;
	HCURSOR cursor_arr	;
	HCURSOR cursor_no	;
	bool	isCursorArrow;
//******************************************


//*************  DRAG & DROP ******************
	protected:
	CImageList*	m_pDragImage;
	BOOL		m_bLDragging;
	HTREEITEM	m_hitemDrag,m_hitemDrop;

//*********************************************

// Implementation
public:
	COLORREF memDC_bgColor_bitmap;
	bool	 isImageTiled;
	HTREEITEM m_Item;

	CString		RemoveBuddyCountFromName(CString name);
	int			GetChildCountInGroup(CString group_name);
		
	bool		DeleteChild(CString buddy_name);
	HTREEITEM	MoveChildItem(HTREEITEM hItem, HTREEITEM htiNewParent, HTREEITEM htiAfter);
	HTREEITEM	GetBuddyTreeItem(CString buddy_name);
	HTREEITEM	AddChild(CString buddy_name, CString group_name);
	bool		DeleteGroup(CString group_name);
	HTREEITEM	GetGroupTreeItem( CString  grp_name);
	HTREEITEM	AddGroup(const CString & group_name);
	virtual		~CDragTree2();

	BOOL SetBkImage(UINT nIDResource) ;
	BOOL SetBkImage(LPCTSTR lpszResourceName) ;

	// Generated message map functions
protected:
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	//{{AFX_MSG(cBuddyTree)
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
// 	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnQueryNewPalette();
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg void OnSelDelete();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags );
	afx_msg void OnArrayMinMax();
	afx_msg void OnArrayMaxMin();
	afx_msg void OnSetCoord();
	afx_msg void OnFaceInfo();
	afx_msg void OnPrjCaption();
	//}}AFX_MSG
//	afx_msg void OnSetWorldCoord();
	DECLARE_MESSAGE_MAP()
protected:
	void SelChange();
	//////////////////
	// Ìí¼Ó07.07.07
	CGuiEdit* m_pEdit;	
	HTREEITEM m_hItem;
	int m_nClickNum;
	//////////////////
	CPalette m_pal;
	CBitmap m_bitmap;
	int m_cxBitmap, m_cyBitmap ;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAGTREE2_H__6511A6EE_992D_487E_9D29_51BD0ECE39E0__INCLUDED_)

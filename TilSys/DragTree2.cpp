// CDragTree2.cpp : implementation file
//

#include "stdafx.h"
#include "DragTree2.h"
#include "resource.h"

#include "TilSys.h"
#include "MainFrm.h"
#include "TilComFunction.h"
#include "DlgFaceCaption.h"
#include "DlgFaceInfoEx.h"
#include "DlgTunnelNames.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDragTree2

CDragTree2::CDragTree2()
{
	memDC_bgColor_bitmap = RGB(255,255,255);

	//////////////////////////////////////////////////////////////////////////
	m_pEdit = new CGuiEdit;
	m_hItem = NULL;
	m_nClickNum = 0;
	//////////////////////////////////////////////////////////////////////////

	m_pDragImage = NULL;
	m_bLDragging = FALSE;

	isImageTiled = true ;

	SetDefaultCursor() ;

}

CDragTree2::~CDragTree2()
{
	delete m_pEdit;
}


BEGIN_MESSAGE_MAP(CDragTree2, CTreeCtrl)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CDragTree2)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBeginDrag)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
// 	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemExpanding)
	ON_WM_ERASEBKGND()
	ON_WM_QUERYNEWPALETTE()
	ON_WM_PALETTECHANGED()
	ON_COMMAND(ID_SELE_DELETED, OnSelDelete)
	ON_COMMAND(ID_ARRAY_MIN_MAX, OnArrayMinMax)
	ON_COMMAND(ID_ARRAY_MAX_MIN, OnArrayMaxMin)
	ON_COMMAND(ID_SET_COORD, OnSetCoord)
	ON_COMMAND(ID_FACE_INFO, OnFaceInfo)
	ON_COMMAND(ID_PRJ_CAPTION, OnPrjCaption)
	//}}AFX_MSG_MAP
//	ON_COMMAND(ID_DELETE_IMAGE, OnDeleteImage)
//	ON_COMMAND(ID_SET_WORLD_COORD, OnSetWorldCoord)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDragTree2 message handlers
void CDragTree2::OnContextMenu(CWnd*, CPoint point)
{
	// CG: This block was added by the Pop-up Menu component
	{
		if (point.x == -1 && point.y == -1){
			//keystroke invocation
			CRect rect;
			GetClientRect(rect);
			ClientToScreen(rect);

			point = rect.TopLeft();
			point.Offset(5, 5);
		}

		// 在根上弹出项目管理菜单，在里程文件上弹出文件属性菜单
		CMenu menu;
		CPoint pt = point;
		ScreenToClient(&pt);
		HTREEITEM hItem = HitTest(pt);
		HTREEITEM hRoot = GetRootItem();
		HTREEITEM hParent = GetParentItem(hItem);
		if ((hRoot==hItem) || hRoot==hParent)
			VERIFY(menu.LoadMenu(IDR_POP_MANAGE_PRJ));
		else if (hItem != NULL)
			VERIFY(menu.LoadMenu(IDR_POP_MANAGE_FILE));
		else
			return;

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		CWnd* pWndPopupOwner = this;

//		while (pWndPopupOwner->GetStyle() & WS_CHILD)
//			pWndPopupOwner = pWndPopupOwner->GetParent();

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
			pWndPopupOwner);
	}
}

void CDragTree2::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnClick(pNMHDR, pResult);
	
	CPoint curPoint;
	GetCursorPos(&curPoint);
	OnContextMenu(NULL, curPoint);

	*pResult = 0;
}

HTREEITEM CDragTree2::AddGroup(const CString & group_name)
{
 	HTREEITEM added_group = 0;
	HTREEITEM groupItem ; 

		if ( (groupItem = GetGroupTreeItem(group_name)) != 0 )  return 0;

	HTREEITEM lastAddedGroup = 0;
		
		//insert the group into the tree
		added_group = InsertItem(group_name, 1,1,0,TVI_FIRST);
	    //Associates 32 bit number with this item
	//	SetItemData(added_group,(DWORD)newGroup);
	
		Expand(added_group,TVE_EXPAND);


	return added_group ;
}

HTREEITEM CDragTree2::GetGroupTreeItem(CString grp_name)
{

	HTREEITEM answer = 0;
	
	HTREEITEM groupItem = GetRootItem()/*(TVI_ROOT, TVGN_NEXT)*/;
	while (groupItem != NULL && !answer)	//while there is still something in the tree
	{
	
		TVITEM item;
		TCHAR szText[1024];
		//CString szText = name;
		item.hItem = groupItem;
		//only get the text and the handle of the item
		item.mask = TVIF_TEXT | TVIF_HANDLE;	
		item.pszText = szText;
		item.cchTextMax = 1024;

		GetItem(&item);

		CString thisGroupName = item.pszText;

		if (thisGroupName == grp_name)
		{
			answer = groupItem;
			break ;
		}
		
		//get the next item for the sake of the while loop ending
		groupItem = GetNextItem(groupItem,TVGN_NEXT);
	}
	return answer;
}

bool CDragTree2::DeleteGroup(CString group_name)
{
	if (group_name.IsEmpty() ) return false ;

	HTREEITEM group_item ;
	if ( (group_item = GetGroupTreeItem(group_name)) == 0 ) return false ;

	//delete every child of this group
	HTREEITEM currentItem = GetNextItem(group_item ,TVGN_CHILD);

	while (currentItem != NULL)
	{
		DeleteItem(currentItem);

		//get the next item for the sake of the while loop ending
		currentItem = GetNextItem(currentItem,TVGN_NEXT);
	}

	//delete the actual group now
	DeleteItem(group_item);

	return true ;
}

HTREEITEM CDragTree2::AddChild(CString buddy_name, CString group_name)
{
 
	HTREEITEM buddy_that_was_added;

	//check if the buddy already exists
	if (GetBuddyTreeItem(buddy_name) != 0) return 0;

	HTREEITEM tree_group = GetGroupTreeItem(group_name);

	//if the group actually exists add the buddy to it
	if (tree_group != 0)
	{
		buddy_that_was_added = InsertItem(buddy_name,2, 2,tree_group,TVI_SORT);

		//	SetItemData(buddy_that_was_added,(DWORD)newBuddy);
	}
	//create the group then add the buddy
	else	
	{
		tree_group = AddGroup(group_name);
		buddy_that_was_added = InsertItem(buddy_name, 2, 2,tree_group,TVI_SORT);

	//	SetItemData(buddyWasAdded,(DWORD)newBuddy);
	}


	//this just expands the group the buddy was added to
	HTREEITEM hParent = GetParentItem(buddy_that_was_added);

	if (hParent != NULL)	Expand(hParent, TVE_EXPAND);

	return buddy_that_was_added;
}

HTREEITEM CDragTree2::GetBuddyTreeItem(CString buddy_name)
{
	HTREEITEM answer = 0;
	//get the root group
	HTREEITEM groupItem = GetRootItem();
	while (groupItem != NULL && !answer)	//while there is still something in the tree
	{
		HTREEITEM currentItem = GetNextItem(groupItem,TVGN_CHILD);
		while (currentItem != NULL && !answer)	//while there is still something in the tree
		{
			TVITEM item;
			TCHAR szText[1024];
			//CString szText = name;
			item.hItem = currentItem;
			//only get the text and the handle of the item
			item.mask = TVIF_TEXT | TVIF_HANDLE;	
			item.pszText = szText;
			item.cchTextMax = 1024;
			
			/*BOOL answer = */
			GetItem(&item);
			
			CString thisBuddyName = item.pszText;
			if (thisBuddyName == buddy_name)
				answer = currentItem ;
			else
				answer = 0;
			
			//get the next item for the sake of the while loop ending
			currentItem = GetNextItem(currentItem,TVGN_NEXT);
		}
		groupItem = GetNextItem(groupItem,TVGN_NEXT);
	}
	return answer;
}

void CDragTree2::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CTreeCtrl::OnLButtonDblClk(nFlags, point);
	return;

	HTREEITEM hItem = HitTest(point);
	if (!hItem)
		return;
	SelectItem(hItem);
	m_hItem = hItem;

	// 如果当前选中的不是第三级目录，则退出
	HTREEITEM hParent = GetParentItem(hItem);
	if (!hParent)
		return;
	hParent = GetParentItem(hParent);
	if (!hParent)
		return;

	CRect rect;
	GetItemRect(hItem, &rect, TRUE);
	if (m_nClickNum == 0)
	{
		if (!m_pEdit->Create(ES_AUTOHSCROLL | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_LEFT, 
			rect, this, IDR_TREE_EDIT))
			return;
	}
	else
	{
		m_pEdit->MoveWindow(rect);
	}	

	m_nClickNum = 1;
	m_hItem = hItem;
	m_pEdit->SetWindowText(GetItemText(hItem));
	m_pEdit->SetFont(GetFont());
	m_pEdit->SetFocus();
	m_pEdit->ShowWindow(true);

	// If this is a root node, return
	if (GetParentItem(hItem) == NULL) return ;

}

void CDragTree2::OnRButtonDown(UINT nFlags, CPoint point) 
{
//	HTREEITEM hItem = HitTest(point);
//	if (!hItem)
//		return;
//
//	SelectItem(hItem);
//
//	HTREEITEM hParent = GetParentItem(hItem);
//	if (!hParent)
//		return;
//	hParent = GetParentItem(hParent);
//	if (!hParent)
//		return;
//
//	ClientToScreen(&point);
//	OnContextMenu(NULL, point);

	CTreeCtrl::OnRButtonDown(nFlags, point);
}

void CDragTree2::OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	*pResult = 0;
	
	// So user cant drag root node
	if (GetParentItem(pNMTreeView->itemNew.hItem) == NULL) return ; 

	// Item user started dragging ...
	m_hitemDrag = pNMTreeView->itemNew.hItem;
	m_hitemDrop = NULL;



	m_pDragImage = CreateDragImage(m_hitemDrag);  // get the image list for dragging
	// CreateDragImage() returns NULL if no image list
	// associated with the tree view control
	if( !m_pDragImage )
		return;

	m_bLDragging = TRUE;
	m_pDragImage->BeginDrag(0, CPoint(-15,-15));
	POINT pt = pNMTreeView->ptDrag;
	ClientToScreen( &pt );
	m_pDragImage->DragEnter(NULL, pt);
	SetCapture();

}

void CDragTree2::OnMouseMove(UINT nFlags, CPoint point) 
{
	HTREEITEM	hitem;
	UINT		flags;

	if (m_bLDragging)
	{
		POINT pt = point;
		ClientToScreen( &pt );
		CImageList::DragMove(pt);
		if ((hitem = HitTest(point, &flags)) != NULL)
		{
				CImageList::DragShowNolock(FALSE);
				
			// Tests if dragged item is over another child !
//			  if ( (GetParentItem(hitem) != NULL) && (cursor_no != ::GetCursor())) 
				if(GetChildItem(hitem) != NULL)
			  {
				  ::SetCursor(cursor_no);
				   // Dont want last selected target highlighted after mouse
				   // has moved off of it, do we now ?
				   SelectDropTarget(NULL);
			  }
				else
				{
				if (cursor_arr != ::GetCursor()) ::SetCursor(cursor_arr); 
				SelectDropTarget(hitem);
				}

			m_hitemDrop = hitem;
			CImageList::DragShowNolock(TRUE);
		}
	}
	else 
	{
		// Set cursor to arrow if not dragged
		// Otherwise, cursor will stay hand or arrow depen. on prev setting
		::SetCursor(cursor_arr);
	}

	CTreeCtrl::OnMouseMove(nFlags, point);

}

void CDragTree2::OnLButtonUp(UINT nFlags, CPoint point) 
{

	CTreeCtrl::OnLButtonUp(nFlags, point);

	if (m_bLDragging)
	{
		m_bLDragging = FALSE;
		CImageList::DragLeave(this);
		CImageList::EndDrag();
		ReleaseCapture();

		if(m_pDragImage != NULL) 
		{ 
		delete m_pDragImage; 
		m_pDragImage = NULL; 
		} 

		// Remove drop target highlighting
		SelectDropTarget(NULL);

		if( m_hitemDrag == m_hitemDrop )
			return;

		HTREEITEM	hitem;
		// Make sure pt is over some item
		if ( ((hitem = HitTest(point, &nFlags)) == NULL)  ) return ;
		// Make sure dropped item isnt a child
//		if (GetParentItem(hitem) != NULL) return ;
		if (GetChildItem(hitem) != NULL) return ;
		// If Drag item is an ancestor of Drop item then return
//		HTREEITEM htiParent = m_hitemDrop;
//		while( (htiParent = GetParentItem( htiParent )) != NULL )
//		{
//			if( htiParent == m_hitemDrag ) return;
//		}

//		Expand( m_hitemDrop, TVE_EXPAND ) ;

		HTREEITEM htiNew = MoveChildItem( m_hitemDrag, m_hitemDrop, TVI_LAST );
		DeleteItem(m_hitemDrag);
		SelectItem( htiNew );
	}

}

HTREEITEM CDragTree2::MoveChildItem(HTREEITEM hItem, HTREEITEM htiNewParent, HTREEITEM htiAfter)
{

	TV_INSERTSTRUCT tvstruct;
	HTREEITEM hNewItem;
    CString sText;

	HTREEITEM hParent = GetParentItem(hItem);
	hParent = GetNextItem(hParent, TVGN_CHILD);

	int nIdx1 = 0, nIdx2 = 0;
	while(hParent != hItem)
	{
		hParent = GetNextItem(hParent, TVGN_NEXT);
		++nIdx1;
	}

	hParent = GetParentItem(hItem);
	hParent = GetNextItem(hParent, TVGN_CHILD);	while(hParent != htiNewParent)
	{
		hParent = GetNextItem(hParent, TVGN_NEXT);
		++nIdx2;
	}
	CTilSysApp* pApp = (CTilSysApp*)AfxGetApp();
	CView* pView = (CView*)(pApp->GetView(RUNTIME_CLASS(CScrollView)));

	::SendMessage(pView->m_hWnd, WM_CHANGE_ITEM,nIdx1, nIdx2);	//修改

	hParent = GetParentItem(hItem);
    // get information of the source item
    tvstruct.item.hItem = hItem;
    tvstruct.item.mask = TVIF_CHILDREN | TVIF_HANDLE |     TVIF_IMAGE | TVIF_SELECTEDIMAGE;
    GetItem(&tvstruct.item);  
    sText = GetItemText( hItem );
        
    tvstruct.item.cchTextMax = sText.GetLength();
    tvstruct.item.pszText = sText.LockBuffer();

    //insert the item at proper location
    tvstruct.hParent = hParent;
    tvstruct.hInsertAfter = htiNewParent;
    tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
    hNewItem = InsertItem(&tvstruct);
    sText.ReleaseBuffer();

    //now copy item data and item state.
    SetItemData(hNewItem,GetItemData(hItem));
    SetItemState(hNewItem,GetItemState(hItem,TVIS_STATEIMAGEMASK),TVIS_STATEIMAGEMASK);

	//now delete the old item
	DeleteItem(hItem);

    return hNewItem;
}


bool CDragTree2::DeleteChild(CString buddy_name)
{
	if (buddy_name.IsEmpty()) return false;


	HTREEITEM buddyItem = GetBuddyTreeItem(buddy_name);

	if (buddyItem != 0)
	{
		DeleteItem(buddyItem);
		return true ; 
	}

	return false;
}

int CDragTree2::GetChildCountInGroup(CString group_name)
{

	HTREEITEM group_item = GetGroupTreeItem(group_name);	//get the group
	
	if (group_item == 0)					return -1 ;
	if (!ItemHasChildren(group_item))		return  0 ;

		int total_in_group =0 ;
		
		//delete every child of this group
		HTREEITEM currentItem = GetNextItem(group_item ,TVGN_CHILD);

		while (currentItem != NULL)
		{
			++total_in_group ;

			//get the next item for the sake of the while loop ending
			currentItem = GetNextItem(currentItem,TVGN_NEXT);
		}

	return total_in_group ;
}

CString CDragTree2::RemoveBuddyCountFromName(CString name)
{
	if (name.Find("(") > 0)
	{
		name = name.Left(name.Find("(") - 1);
	}
	else return name;

	return name;
}

/*
void CDragTree2::CollapseAll()
{
        HTREEITEM hti = GetRootItem();
        do{
             Expand( hti, TVE_COLLAPSE ); //     CollapseBranch( hti );
        }while( (hti = GetNextSiblingItem( hti )) != NULL );
}

  */

void CDragTree2::OnPaint() 
{

		// Remove comments from next five lines if you don't need any 
	// specialization beyond adding a background image
//	if( m_bitmap.m_hObject == NULL )
//	{
//		CTreeCtrl::OnPaint();
//		return;
//	}

	CPaintDC dc(this);

	CRect rcClip, rcClient;
	dc.GetClipBox( &rcClip );
	GetClientRect(&rcClient);

	// Create a compatible memory DC 
	CDC memDC;
	memDC.CreateCompatibleDC( &dc );
	
	// Select a compatible bitmap into the memory DC
	CBitmap bitmap, bmpImage;
	bitmap.CreateCompatibleBitmap( &dc, rcClient.Width(), rcClient.Height() );
	memDC.SelectObject( &bitmap );

	
	// First let the control do its default drawing.
	CWnd::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );

	// Draw bitmap in the background if one has been set
	if( m_bitmap.m_hObject != NULL )
	{
		// Now create a mask
		CDC maskDC;
		maskDC.CreateCompatibleDC(&dc);
		CBitmap maskBitmap;

		// Create monochrome bitmap for the mask
		maskBitmap.CreateBitmap( rcClient.Width(), rcClient.Height(), 
						1, 1, NULL );
		maskDC.SelectObject( &maskBitmap );
		memDC.SetBkColor(RGB(255,255,255) /*::GetSysColor( COLOR_WINDOW )*/ );

		// Create the mask from the memory DC
		maskDC.BitBlt( 0, 0, rcClient.Width(), rcClient.Height(), &memDC, 
					rcClient.left, rcClient.top, SRCCOPY );

		
		CDC tempDC;
		tempDC.CreateCompatibleDC(&dc);
		tempDC.SelectObject( &m_bitmap );

		CDC imageDC;
		CBitmap bmpImage;
		imageDC.CreateCompatibleDC( &dc );
		bmpImage.CreateCompatibleBitmap( &dc, rcClient.Width(), 
						rcClient.Height() );
		imageDC.SelectObject( &bmpImage );

		if( dc.GetDeviceCaps(RASTERCAPS) & RC_PALETTE && m_pal.m_hObject != NULL )
		{
			dc.SelectPalette( &m_pal, FALSE );
			dc.RealizePalette();

			imageDC.SelectPalette( &m_pal, FALSE );
		}

		// Get x and y offset
		CRect rcRoot;
		GetItemRect( GetRootItem(), rcRoot, FALSE );
		rcRoot.left = -GetScrollPos( SB_HORZ );

	
		if (isImageTiled)
		{
			// Draw bitmap in tiled manner to imageDC
			for( int i = rcRoot.left; i < rcClient.right; i += m_cxBitmap )
			for( int j = rcRoot.top; j < rcClient.bottom; j += m_cyBitmap )
				imageDC.BitBlt( i, j, m_cxBitmap, m_cyBitmap, &tempDC, 
							0, 0, SRCCOPY );
		}else
		{
			int x=0,y=0 ;
			(m_cxBitmap > rcClient.right) ? x=0:x=(rcClient.right - m_cxBitmap);
			(m_cyBitmap > rcClient.bottom)? y=0:y=(rcClient.bottom - m_cyBitmap);
			imageDC.BitBlt( x, y, m_cxBitmap, m_cyBitmap, &tempDC, 
							0, 0, SRCCOPY );
		}

		// Set the background in memDC to black. Using SRCPAINT with black and any other
		// color results in the other color, thus making black the transparent color
		memDC.SetBkColor( RGB(0,0,0)/*memDC_bgColor_bitmap*/);        
		memDC.SetTextColor(RGB(255,255,255));
		memDC.BitBlt(rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(), &maskDC, 
				rcClip.left, rcClip.top, SRCAND);

		// Set the foreground to black. See comment above.
		imageDC.SetBkColor(RGB(255,255,255));
		imageDC.SetTextColor(RGB(0,0,0));
		imageDC.BitBlt(rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(), &maskDC, 
				rcClip.left, rcClip.top, SRCAND);

		// Combine the foreground with the background
		imageDC.BitBlt(rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(), 
					&memDC, rcClip.left, rcClip.top,SRCPAINT);

		//*****************************
			/*	for( int yy = 0; yy < rcClient.Height(); yy++)
				for( int xx = 0; xx < rcClient.Width(); xx++ )
			{
				if (imageDC.GetPixel(CPoint(xx,yy)) == RGB(0,0,0))
					imageDC.SetPixel(CPoint(xx,yy),RGB(255,255,255));
			}
			 Create a compatible memory DC 48068
			CDC whiteDC;
			whiteDC.CreateCompatibleDC( &dc );
	
			// Select a compatible bitmap into the memory DC
				CBitmap cBmp;
			blankBmp.CreateCompatibleBitmap( &dc, rcClient.Width(), rcClient.Height() );
			whiteDC.SelectObject( &blankBmp );*/
		//*****************************

		// Draw the final image to the screen		
		dc.BitBlt( rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(), 
					&imageDC, rcClip.left, rcClip.top, SRCCOPY );
	}
	else
	{
		dc.BitBlt( rcClip.left, rcClip.top, rcClip.Width(), 
				rcClip.Height(), &memDC, 
				rcClip.left, rcClip.top, SRCCOPY );
	}

}

void CDragTree2::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if( m_bitmap.m_hObject != NULL ) InvalidateRect(NULL);

	CTreeCtrl::OnHScroll(nSBCode, nPos, pScrollBar);

}

void CDragTree2::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if( m_bitmap.m_hObject != NULL )	InvalidateRect(NULL);

	CTreeCtrl::OnVScroll(nSBCode, nPos, pScrollBar);

}

void CDragTree2::OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	if( m_bitmap.m_hObject != NULL ) InvalidateRect(NULL);
	
	*pResult = 0;

}

BOOL CDragTree2::OnEraseBkgnd(CDC* pDC) 
{
	if( m_bitmap.m_hObject != NULL )	return TRUE;

	return CTreeCtrl::OnEraseBkgnd(pDC);

}

BOOL CDragTree2::OnQueryNewPalette() 
{

	CClientDC dc(this);
	if( dc.GetDeviceCaps(RASTERCAPS) & RC_PALETTE && m_pal.m_hObject != NULL )
	{
		dc.SelectPalette( &m_pal, FALSE );
		BOOL result = dc.RealizePalette();
		if( result )
			Invalidate();
		return result;
	}
	
	return CTreeCtrl::OnQueryNewPalette();
}

void CDragTree2::OnPaletteChanged(CWnd* pFocusWnd) 
{
	CTreeCtrl::OnPaletteChanged(pFocusWnd);
	
	if( pFocusWnd == this )
		return;

	OnQueryNewPalette();

}

BOOL CDragTree2::SetBkImage(UINT nIDResource)
{
	return SetBkImage( (LPCTSTR)nIDResource );
}

BOOL CDragTree2::SetBkImage(LPCTSTR lpszResourceName)
{

	// If this is not the first call then Delete GDI objects
	if( m_bitmap.m_hObject != NULL )
		m_bitmap.DeleteObject();
	if( m_pal.m_hObject != NULL )
		m_pal.DeleteObject();
	
	
	HBITMAP hBmp = (HBITMAP)::LoadImage( AfxGetInstanceHandle(), 
			lpszResourceName, IMAGE_BITMAP, 0,0, LR_CREATEDIBSECTION );

	if( hBmp == NULL ) 
		return FALSE;

	m_bitmap.Attach( hBmp );
	BITMAP bm;
	m_bitmap.GetBitmap( &bm );
	m_cxBitmap = bm.bmWidth;
	m_cyBitmap = bm.bmHeight;


	// Create a logical palette for the bitmap
	DIBSECTION ds;
	BITMAPINFOHEADER &bmInfo = ds.dsBmih;
	m_bitmap.GetObject( sizeof(ds), &ds );

	int nColors = bmInfo.biClrUsed ? bmInfo.biClrUsed : 1 << bmInfo.biBitCount;

	// Create a halftone palette if colors > 256. 
	CClientDC dc(NULL);			// Desktop DC
	if( nColors > 256 )
		m_pal.CreateHalftonePalette( &dc );
	else
	{
		// Create the palette

		RGBQUAD *pRGB = new RGBQUAD[nColors];
		CDC memDC;
		memDC.CreateCompatibleDC(&dc);

		memDC.SelectObject( &m_bitmap );
		::GetDIBColorTable( memDC, 0, nColors, pRGB );

		UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * nColors);
		LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];

		pLP->palVersion = 0x300;
		pLP->palNumEntries = nColors;

		for( int i=0; i < nColors; i++)
		{
			pLP->palPalEntry[i].peRed = pRGB[i].rgbRed;
			pLP->palPalEntry[i].peGreen = pRGB[i].rgbGreen;
			pLP->palPalEntry[i].peBlue = pRGB[i].rgbBlue;
			pLP->palPalEntry[i].peFlags = 0;
		}

		m_pal.CreatePalette( pLP );

		delete[] pLP;
		delete[] pRGB;
	}
	Invalidate();

	return TRUE;
}

void CDragTree2::SetDefaultCursor()
{
	// Get the windows directory
	CString strWndDir;
	GetWindowsDirectory(strWndDir.GetBuffer(MAX_PATH), MAX_PATH);
	strWndDir.ReleaseBuffer();
	
	strWndDir += _T("\\winhlp32.exe");
	// This retrieves cursor #106 from winhlp32.exe, which is a hand pointer
	HMODULE hModule = LoadLibrary(strWndDir);
	if (hModule) {
		HCURSOR hHandCursor = ::LoadCursor(hModule, MAKEINTRESOURCE(106));
		if (hHandCursor)
		{
			cursor_hand = CopyCursor(hHandCursor);
		}
		
	}
	FreeLibrary(hModule);
	
	cursor_arr	= ::LoadCursor(NULL, IDC_ARROW);
	cursor_no	= ::LoadCursor(NULL, IDC_NO) ;
}

void CDragTree2::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// 若目录树中的文件名被修改了，则
	if (m_nClickNum == 1)
	{
		CString strFileName;
		m_pEdit->GetWindowText(strFileName);
		SetItemText(m_hItem, strFileName);

		int nIdx = GetCanvasMgr()->GetCurCanvasIdx();
		CTilCanvas* pCanvas = GetCanvasMgr()->GetCurCanvas();
		CString strOldPathImage = pCanvas->GetImage()->GetPathName();
		if (get_file_name(strOldPathImage) != strFileName)
		{
			pCanvas->RenameAll(get_file_name(strOldPathImage), strFileName);
			pCanvas->SetModifiedFlag();
		}

		m_pEdit->ShowWindow(false);

		m_nClickNum = 2;
	}	

//	UpdateData();
	UINT nFlags;
	CPoint curPoint;
	GetCursorPos(&curPoint);
	ScreenToClient(&curPoint);
	HTREEITEM hItem = HitTest(curPoint, &nFlags);

	if ((hItem != NULL) && (TVHT_ONITEM & nFlags))
	{
		//Select(hItem, TVGN_CARET);
		SelectItem(hItem);
		m_hItem = hItem;;
	}

	hItem = GetSelectedItem();

	// 判断是否在第三级目录上
	HTREEITEM hParent = GetParentItem(hItem);
	if (!hParent)
		return;
	hParent = GetParentItem(hParent);
	if (!hParent)
		return;

	hParent = GetNextItem(hParent, TVGN_CHILD);
	hParent = GetNextItem(hParent, TVGN_CHILD);

	int nIdx = 0;
	while(hParent != hItem)
	{
		if(!hParent)
			return;
		
		++nIdx;
		hParent = GetNextItem(hParent, TVGN_NEXT);		
	}

	CTilSysApp* pApp = (CTilSysApp*)AfxGetApp();
	CView* pView = (CView*)(pApp->GetView(RUNTIME_CLASS(CScrollView)));
	if (pView != NULL)
		::SendMessage(pView->m_hWnd, WM_SELE_CHANGED,nIdx,NULL);	//修改

	//*pResult = 0;
}

// void CDragTree2::OnSetFocus(CWnd* pOldWnd)
// {
// 
// }

void CDragTree2::OnKillFocus(CWnd* pNewWnd)
{
// 	SelectDropTarget(m_Item);
}

void CDragTree2::OnSelDelete()
{
	// 确保选择的是最小一级目录
	HTREEITEM hItem = GetSelectedItem();
	if (!hItem)
		return;
	HTREEITEM hParent = GetParentItem(hItem);
	if (!hParent)
		return;
	hParent = GetParentItem(hParent);
	if (!hParent)
		return;

	hParent = GetParentItem(hItem);
	hParent = GetNextItem(hParent, TVGN_CHILD);

	if (AfxMessageBox("你确定要删除当前掌子面图片吗?", MB_OKCANCEL) == IDOK)
	{
		// 获得被选中的掌子面的序号nIdx
		int nIdx = 0;
		while(hParent != hItem)
		{
			++nIdx;
			hParent = GetNextItem(hParent, TVGN_NEXT);		
		}
		
		DeleteItem(hItem);
		
		// 删除被选中掌子面
		CTilSysApp* pApp = (CTilSysApp*)AfxGetApp();
		CView* pView = (CView*)(pApp->GetView(RUNTIME_CLASS(CScrollView)));
		::SendMessage(pView->m_hWnd, WM_SELE_DELETED, nIdx, NULL);
	}
}

void CDragTree2::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags )
{
	if (nChar == VK_DELETE)
		OnSelDelete();
	if (nChar == VK_ESCAPE);

	if (nChar == VK_RETURN)
		OnClick(NULL, NULL);


	CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CDragTree2::OnSetCoord()
{
	CTilCanvas* pCanvas = GetCanvasMgr()->GetCurCanvas();
	POINT2DF ptImage = pCanvas->GetCenterImgCoord();
	POINT_3F ptGrnd  = pCanvas->GetCenterGrndCoord();
	float fAngle	 = pCanvas->GetFaceAngle();

	CDlgFaceCaption dlg;
	dlg.DoModal();
	/*
	dlg.m_dbFaceCenterImageX = ptImage.x;
	dlg.m_dbFaceCenterImageY = ptImage.y;
	dlg.m_dbFaceCenterGrndX = ptGrnd.x;
	dlg.m_dbFaceCenterGrndY = ptGrnd.y;
	dlg.m_dbFaceCenterGrndZ = ptGrnd.z;
	dlg.m_fFaceAngle = fAngle;

	if (dlg.DoModal() == IDOK)
	{
		pCanvas->SetCenterImgCoord(dlg.m_dbFaceCenterImageX, 
								   dlg.m_dbFaceCenterImageY);
		pCanvas->SetCenterGrndCoord(dlg.m_dbFaceCenterGrndX, 
									dlg.m_dbFaceCenterGrndY,
									dlg.m_dbFaceCenterGrndZ);
		pCanvas->SetFaceAngle(dlg.m_fFaceAngle);
	}
	*/
}

// 显示掌子面信息
void CDragTree2::OnFaceInfo()
{
	// 当所有文件命名正确了，则弹出对话框
	CDlgFaceInfoEx dlg(this);
	// 读取ifo文件和对应的画布指针
	CString strPath, strName, strIFO;
	CTilCanvasMgr* pMgr = GetCanvasMgr();
	for (int i=0; i<pMgr->Size(); i++)
	{
		CTilCanvas* pCanvas = pMgr->GetCanvasByIdx(i);
		strPath = get_file_path(pCanvas->GetImage()->GetPathName());
		strName = get_file_name(pCanvas->GetImage()->GetPathName());
		strIFO = strPath + '\\' + strName + ".ifo";
		dlg.ReadFaceInfoAndCanvas(strIFO, pCanvas);
	}

	dlg.DoModal();
	dlg.m_faceInfoLst.RestoreOriCanvasSetting();
}

// 设置项目相关信息
void CDragTree2::OnPrjCaption()
{
	DlgTunnelNames dlg;
	dlg.SetPrjCaption(GetCanvasMgr()->GetPrjInfo());
	if (dlg.DoModal() == IDOK)
	{
		CTilCanvasMgr* pMgr = GetCanvasMgr();
		pMgr->SetPrjInfo(dlg.GetPrjCaption());
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		pFrame->InsertProjToSolExplorer(pMgr);
		pMgr->SetModifiedFlag(TRUE);
	}
}

// 按从小到大排序
void CDragTree2::OnArrayMinMax()
{
	CTilCanvasMgr* pMgr = GetCanvasMgr();
	pMgr->AutoArrayCanvas(TRUE);

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->InsertProjToSolExplorer(pMgr);
}

// 按从大到小排序
void CDragTree2::OnArrayMaxMin()
{
	CTilCanvasMgr* pMgr = GetCanvasMgr();
	pMgr->AutoArrayCanvas(FALSE);

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->InsertProjToSolExplorer(pMgr);
}

void CDragTree2::SelChange()
{

}

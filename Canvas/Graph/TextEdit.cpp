// TextEdit.cpp : implementation file
//

#include "stdafx.h"
#include "TextEdit.h"
//#include "ReginSet.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextEdit

CTextEdit::CTextEdit()
{
	m_IsMoving=false;
	m_IsOnMoving=false;
	m_Font=new CFont;
	m_Font->CreatePointFont (100, _T ("Arial"));
	
}

CTextEdit::CTextEdit(CTextEdit& textEdit)
{
	m_IsMoving=textEdit.m_IsMoving;
	m_IsOnMoving=textEdit.m_IsOnMoving;
	m_FirstPoint=textEdit.m_FirstPoint;
	m_SecondPoint=textEdit.m_SecondPoint;
	m_PositionPoint=textEdit.m_PositionPoint;
	m_Sizing=textEdit.m_Sizing;

	LOGFONT logFont;
	textEdit.m_Font->GetLogFont(&logFont);
	m_Font=new CFont;
	m_Font->CreateFontIndirect(&logFont);
}

CTextEdit::~CTextEdit()
{
	if (m_Font)
		delete m_Font;
}

BEGIN_MESSAGE_MAP(CTextEdit, CEdit)
//{{AFX_MSG_MAP(CTextEdit)
ON_WM_CONTEXTMENU()
ON_COMMAND(ID_UnDo_Text, OnUnDoText)
ON_COMMAND(ID_Cut, OnCut)
ON_COMMAND(ID_Copy, OnCopy)
ON_COMMAND(ID_Past, OnPast)
ON_COMMAND(ID_Delete, OnDelete)
ON_COMMAND(ID_SelectAll, OnSelectAll)
ON_COMMAND(ID_Move, OnMove)
ON_WM_LBUTTONDOWN()
ON_WM_MOUSEMOVE()
ON_WM_LBUTTONUP()
ON_COMMAND(ID_MENUITEM_Size, OnMENUITEMSize)
ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_INSERT_SPECIALCHAR, OnInsertSpecialchar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextEdit message handlers


void CTextEdit::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	CMenu menuTextEdit;
	menuTextEdit.LoadMenu(IDR_MENU_TEXTEDIT);
	menuTextEdit.CheckMenuItem(ID_Move,m_IsMoving?MF_CHECKED:MF_UNCHECKED);
	
	menuTextEdit.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);
}

void CTextEdit::OnUnDoText() 
{
	// TODO: Add your command handler code here
	CEdit::Undo();	
}

void CTextEdit::OnCut() 
{
	// TODO: Add your command handler code here
	CEdit::Cut();	
}

void CTextEdit::OnCopy() 
{
	// TODO: Add your command handler code here
	CEdit::Copy();	
}

void CTextEdit::OnPast() 
{
	// TODO: Add your command handler code here
	CEdit::Paste();	
}

void CTextEdit::OnDelete() 
{
	CEdit::Clear();	
}

void CTextEdit::OnSelectAll() 
{
	CEdit::SetSel(0,-1);
}

void CTextEdit::OnMove() 
{
	// TODO: Add your command handler code here
	
	m_IsMoving=!m_IsMoving;
	
}

void CTextEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_FirstPoint=point;
	CEdit::OnLButtonDown(nFlags, point);
}

void CTextEdit::OnMouseMove(UINT nFlags, CPoint point) 
{
	/*
	if(m_IsOnMoving)
	{ 
		CRect rect;
		this->GetWindowRect(&rect);
		m_PositionPoint.Offset(point.x-m_FirstPoint.x,point.y-m_FirstPoint.y);
		this->MoveWindow(m_PositionPoint.x,m_PositionPoint.y,rect.Width(),rect.Height(),true);	
		this->m_FirstPoint=point;
  }
	*/
	
	CEdit::OnMouseMove(nFlags, point);
}

void CTextEdit::OnLButtonUp(UINT nFlags, CPoint point) 
{
	/*
	if(m_IsOnMoving)
	{
		CRect rect;
		this->GetWindowRect(&rect);
		m_PositionPoint.Offset(point.x-m_FirstPoint.x,point.y-m_FirstPoint.y);
		this->MoveWindow(m_PositionPoint.x,m_PositionPoint.y,rect.Width(),rect.Height(),true);
		this->m_FirstPoint=point;
		m_IsOnMoving=false;
	}
	*/
		
	CEdit::OnLButtonUp(nFlags, point);
}

void CTextEdit::OnMENUITEMSize() 
{
	// TODO: Add your command handler code here
//	CRect rect;
//	GetWindowRect(&rect);
//	CReginSet dRegDlg;
//	dRegDlg.m_Length=rect.Width();
//	
//	dRegDlg.m_Width=rect.Height();
//	
//	if(dRegDlg.DoModal()==IDOK)
//	{
//		this->MoveWindow(m_PositionPoint.x,m_PositionPoint.y,dRegDlg.m_Length,dRegDlg.m_Width);
//	}
}

void CTextEdit::SetMyFont(CFont *font)
{
	LOGFONT lfFont;
	font->GetLogFont(&lfFont);

	m_Font->DeleteObject();
	m_Font->CreateFontIndirect(&lfFont);
	SetFont(m_Font);
}

int CTextEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	this->SetFont(m_Font);
	return 0;
}

void CTextEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

//void CTextEdit::OnSetFocus(CWnd* pOldWnd)
//{
//	
//}

#include "DlgSpecialChar.h"
void CTextEdit::OnInsertSpecialchar() 
{
	// TODO: Add your command handler code here
	// TODO: Add your control notification handler code here
	CDlgSpecialChar Special(this);
	if (Special.DoModal()==IDCANCEL)
		SetFocus();
}

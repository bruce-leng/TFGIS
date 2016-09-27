#include"stdafx.h"
#include"Text.h"
#include"TextEdit.h"
#include "..\GraphDefine.h"

IMPLEMENT_SERIAL(CText,CUnit,1)
CText::CText()
{
	CUnit::Initial();

	// 创建字体
	m_pFont = new CFont;
	m_pFont->CreatePointFont (500,_T("Arial"));
	m_pTextEdit = new CTextEdit;
	m_HaveFinish=false;
	m_bHaveCreat=false;
	m_BkMode = TRANSPARENT;
}

CText::CText(CText& text)
{
	m_FirstPoint=text.m_FirstPoint;
	m_SecondPoint=text.m_SecondPoint;
	m_PenColor=text.m_PenColor;
	m_PenWidth=text.m_PenWidth;
	m_PenStyle=text.m_PenStyle;
	m_pRgn=NULL;
	
	m_DrawingMode=text.m_DrawingMode;
	m_BkMode=text.m_BkMode;
	m_BackgroundColor=text.m_BackgroundColor;
	
	LOGFONT logFont;
	text.m_pFont->GetLogFont(&logFont);
	m_pFont=new CFont;
	m_pFont->CreateFontIndirect(&logFont);

	m_dbZoomX = text.m_dbZoomX;
	m_dbZoomY = text.m_dbZoomY;
	
	m_Width=text.m_Width;
	m_Hight=text.m_Hight;
	m_PositionPoint=text.m_PositionPoint;
	m_String=text.m_String;
	m_HaveFinish=text.m_HaveFinish;
	m_bHaveCreat=false;
	m_pTextEdit = new CTextEdit(*text.m_pTextEdit);
}

CText::~CText()
{
	delete m_pFont;
	delete m_pTextEdit;
}

void CText::Serialize(CArchive &ar)
{
	CUnit::Serialize(ar);
  
	LOGFONT m_lf; 

	if(ar.IsStoring())
	{
		m_pFont->GetLogFont(&m_lf);

		ar<<m_Hight<<m_Width<<m_PositionPoint.x<<m_PositionPoint.y<<m_String;
		ar<<m_lf.lfCharSet;
		ar<<m_lf.lfClipPrecision;
		ar<<m_lf.lfEscapement;
		ar<<m_lf.lfHeight;
		ar<<m_lf.lfItalic;
		ar<<m_lf.lfOrientation;
		ar<<m_lf.lfOutPrecision;
		ar<<m_lf.lfPitchAndFamily;
		ar<<m_lf.lfQuality;	
		ar<<m_lf.lfWidth;
		ar<<m_lf.lfWeight;
		ar<<m_lf.lfUnderline;
		ar<<m_lf.lfStrikeOut;
		CString str=m_lf.lfFaceName;
		ar<<str;
	}
	else
	{
		m_HaveFinish=true;
		ar>>m_Hight>>m_Width>>m_PositionPoint.x>>m_PositionPoint.y>>m_String;

		ar>>m_lf.lfCharSet;
		ar>>m_lf.lfClipPrecision;
		ar>>m_lf.lfEscapement;
		ar>>m_lf.lfHeight;
		ar>>m_lf.lfItalic;
		ar>>m_lf.lfOrientation;
		ar>>m_lf.lfOutPrecision;
		ar>>m_lf.lfPitchAndFamily;
		ar>>m_lf.lfQuality;	
		ar>>m_lf.lfWidth;
		ar>>m_lf.lfWeight;
		ar>>m_lf.lfUnderline;
		ar>>m_lf.lfStrikeOut;
		CString str;
		ar>>str;
		::lstrcpy(m_lf.lfFaceName,str);
		m_pFont->DeleteObject();
		m_pFont->CreateFontIndirect(&m_lf);
		
		m_bHaveCreat=false;
	}
}

void CText::OnLButtonDown(CDC *pDC, PGRAPHCAPTION pCaption, CPoint point)
{
	CWnd* pWnd = AfxGetMainWnd();
	CMDIChildWnd*pChild=((CMDIFrameWnd*)pWnd)->MDIGetActive();
	CView*pView=(CView*)pChild->GetActiveView();

	if(pCaption->m_CurrentDrawStatus==Draw_Status)
	{
		if(pCaption->m_LBDownTimes==1&&!m_pTextEdit->m_IsMoving)
		{
			CPoint point1, point2;
			point1 = point;
			pDC->LPtoDP(&point1);
			point2 = CPoint(point1.x+190, point1.y+190);
			m_pTextEdit->Create(WS_VSCROLL|WS_HSCROLL|ES_MULTILINE | ES_WANTRETURN | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
				CRect(point1,point2), pView, 1);
			this->SetTextEditPosition(pDC,pCaption,point);
			m_pTextEdit->ShowWindow(true);
			pWnd->SendMessage(WM_SHOW_FONT_BAR, true); // pView->ShowFontBar(true);
		}

		if(pCaption->m_LBDownTimes==2&&!m_pTextEdit->m_IsMoving)
		{
			pWnd->SendMessage(WM_SHOW_FONT_BAR, false); // pView->ShowFontBar(false);
			m_pTextEdit->UpdateData(true);
			m_pTextEdit->ShowWindow(false);
			m_HaveFinish=true; 
			m_pTextEdit->GetWindowText(m_String);
			pCaption->m_LBDownTimes=0;
// 			pView->ExchangeSave();
			
			if(this->m_DrawStatus==Change_Status)
			{
				this->m_DrawStatus=Draw_Status;
				pCaption->m_CurrentDrawStatus=Select_Status;
			}

			int nLineCount = m_pTextEdit->GetLineCount();
			CString strText;
			CFont *pOldFond=pDC->SelectObject(m_pFont);
			int pOldBkMode=pDC->SetBkMode(this->m_BkMode);
			COLORREF oldBKColor=pDC->SetBkColor(this->m_BackgroundColor);
			COLORREF oldPenColor=pDC->SetTextColor(this->m_PenColor);
			CBrush brush;
			LOGBRUSH brushlog;
			brushlog.lbColor=m_BrushColor;
			brushlog.lbHatch=m_BrushHatch;
			brushlog.lbStyle=m_BrushStyle;
			brush.CreateBrushIndirect(&brushlog);
			CBrush*pOldBrush=pDC->SelectObject(&brush);
			TEXTMETRIC tm;
			pDC->GetTextMetrics(&tm);
			int maxLength=0;
// 			int maxLineLong;

			for(int i=0;i<nLineCount;i++)
			{
				int LineLength=m_pTextEdit->LineLength(m_pTextEdit->LineIndex(i));
				maxLength=maxLength>LineLength?maxLength:LineLength;
				m_pTextEdit->GetLine(0,strText.GetBuffer(LineLength),LineLength);

				strText.ReleaseBuffer();
				CPoint PositionPoint=GetDstPoint(m_PositionPoint);
				pDC->TextOut(PositionPoint.x,PositionPoint.y+i*(tm.tmHeight+tm.tmExternalLeading),strText.Left(LineLength));
				
				strText.Empty();
			}

			this->m_Width=maxLength*(tm.tmMaxCharWidth);
			this->m_Hight=(nLineCount+1)*(tm.tmHeight+tm.tmExternalLeading);
			CRect rect;
			m_pTextEdit->GetWindowRect(&rect);
			m_Width=m_Width>rect.Width()?m_Width:rect.Width();
			m_Hight=m_Hight>rect.Height()?m_Hight:rect.Height();
			
			pDC->SetBkMode(pOldBkMode);
			pDC->SelectObject(pOldFond);
			pDC->SetBkColor(oldBKColor);
			pDC->SetTextColor(oldPenColor);
			pDC->SelectObject(pOldBrush);
		} //******if(pCaption->m_LBDownTimes==2&&!m_pTextEdit->m_IsMoving)
		if(pCaption->m_LBDownTimes==2&&m_pTextEdit->m_IsMoving)
		{
			CWnd* pWnd = AfxGetMainWnd();
			pWnd->SendMessage(WM_SHOW_FONT_BAR, true); // pCaption->ShowFontBar(true);
			pCaption->m_LBDownTimes=1;
			
// 			pView->ExchangeSave();
			
			m_PositionPoint=GetSrcPoint(point);
			m_pTextEdit->m_PositionPoint=GetSrcPoint(point);
			CRect rect;
			m_pTextEdit->GetWindowRect(&rect);
			pDC->DPtoLP(&rect);
			pDC->DrawFocusRect(CRect(point.x+1,point.y+1,point.x+rect.Width()-1,point.y+rect.Height()-1));
			this->SetTextEditPosition(pDC,pCaption,point);
			this->m_pTextEdit->m_IsMoving=false;
		}
	}

	if(pCaption->m_CurrentDrawStatus==Change_Status)
	{
    pCaption->m_CurrentDrawStatus=Draw_Status;
		pCaption->m_LBDownTimes=1;
	}//********if(pView->m_CurrentDrawStatus==Exchange_Status)
	
	if(pCaption->m_CurrentDrawStatus==Select_Status)
	{
		if(IsOnMarginPoint(point))
		{
			if(m_HaveFindFirst)
			{
				LOGFONT lf;
				m_pFont->GetLogFont(&lf);
				CWnd* pWnd = AfxGetMainWnd();
				pWnd->SendMessage(WM_SHOW_FONT_BAR, true, (LPARAM)&lf); // pCaption->ShowFontBar(true);
				this->m_HaveFindFirst=false;
				pCaption->m_bHaveFindSecond=true;
				this->ShowSelectPoint(pDC);
				CPoint point=GetDstPoint(m_PositionPoint);
				pDC->LPtoDP(&point);
				
				CRect rect;
				m_pTextEdit->GetWindowRect(&rect);
				m_Width=m_Width>rect.Width()?m_Width:rect.Width();
				m_Hight=m_Hight>rect.Height()?m_Hight:rect.Height();
				this->m_pTextEdit->MoveWindow(point.x,point.y,m_Width,m_Hight);
				this->m_pTextEdit->ShowWindow(true);
				this->m_pTextEdit->SetFocus();	// 添加07.07.05
				this->m_pTextEdit->SetFont(m_pFont);
				pCaption->m_CurrentDrawTool=Graph_Text;
				pCaption->m_CurrentDrawStatus=Change_Status;
				pCaption->m_pCurrentUnit=this;
				pCaption->m_LBDownTimes=0;
				m_HaveFinish=false;
				this->m_DrawStatus=Change_Status;
			}
		}
    if(!pCaption->m_bHaveFindSecond&&IsInRgn(point))
		{
			if(!m_HaveFindFirst)
			{
				m_HaveFindFirst=true;
				pCaption->m_bHaveFindFirst=true;
				ShowSelectPoint(pDC);
			}
			else
			{
				m_HaveFindFirst=false;
				pCaption->m_bHaveFindFirst=true;
				ShowSelectPoint(pDC);
			}
			
		}
  }//******if(pView->m_CurrentDrawStatus==Select_Status)
}
 
void CText::SetTextEditPosition(CDC *pDC, PGRAPHCAPTION pCaption, CPoint point)
{
  CRect rect;
  m_pTextEdit->GetWindowRect(&rect);
 
  m_PositionPoint=GetSrcPoint(point);
  m_pTextEdit->m_PositionPoint=GetSrcPoint(point);
  this->m_Width=rect.Width();
  this->m_Hight=rect.Height();
  pDC->LPtoDP(&point);
//  m_pTextEdit->m_PositionPoint=point;	//????????????
  m_pTextEdit->MoveWindow(point.x,point.y,rect.Width(),rect.Height());
}

void CText::SetFont(ENUMLOGFONT *lpelf)
{
 m_pTextEdit->SetMyFont(m_pFont);
}

void CText::ChangeFont(ENUMLOGFONT *lpelf)
{
 m_pFont->DeleteObject();
 m_pFont->CreateFontIndirect(&lpelf->elfLogFont);
 SetFont(lpelf);
}

void CText::ChangeFontSize(ENUMLOGFONT *lpelf)
{
 m_pFont->DeleteObject();
 m_pFont->CreateFontIndirect(&lpelf->elfLogFont);
 SetFont(lpelf);
}

void CText::DrawStatic(CDC *pDC)
{
	if(this->m_HaveFinish)
	{
		CWnd* pWnd = AfxGetMainWnd();
		CMDIChildWnd*pChild=((CMDIFrameWnd*)pWnd)->MDIGetActive();
		CView*pView=(CView*)pChild->GetActiveView();
		
		// 如果pView不为NULL，则pView为主视图
		// 否则m_pView此时为对话框视图，将其指针赋值给pView
		if (pView == NULL)
			pView = m_pView;
		
		CPoint PositionPoint=GetDstPoint(m_PositionPoint);
		if(!m_bHaveCreat)
		{
			m_pTextEdit->DestroyWindow();
			m_pTextEdit->Create(WS_VSCROLL|WS_HSCROLL|ES_MULTILINE | ES_WANTRETURN | WS_CHILD |WS_TABSTOP | WS_BORDER,
				CRect(PositionPoint,CPoint(PositionPoint.x+m_Width,PositionPoint.y+m_Hight)), pView, 1);
			m_bHaveCreat=true;
		}
		m_pTextEdit->SetWindowText(m_String);
		int nLineCount = m_pTextEdit->GetLineCount();
		CString strText;
		
		// 将文字缩放后显示
		LOGFONT logFont;
		m_pFont->GetLogFont(&logFont);
		CFont font;
		logFont.lfHeight *= (m_dbZoomX+m_dbZoomY)/2;
		logFont.lfWidth  *= (m_dbZoomX+m_dbZoomY)/2;
		logFont.lfWeight *= (m_dbZoomX+m_dbZoomY)/2;
		font.CreateFontIndirect(&logFont);
		
		CFont *pOldFond=pDC->SelectObject(&font);
		int pOldBkMode=pDC->SetBkMode(this->m_BkMode);
		COLORREF oldBKColor=pDC->SetBkColor(this->m_BackgroundColor);
		COLORREF oldPenColor=pDC->SetTextColor(this->m_PenColor);
		CBrush brush;
		LOGBRUSH brushlog;
		brushlog.lbColor=m_BrushColor;
		brushlog.lbHatch=m_BrushHatch;
		brushlog.lbStyle=m_BrushStyle;
		brush.CreateBrushIndirect(&brushlog);
		CBrush*pOldBrush=pDC->SelectObject(&brush);
		
		TEXTMETRIC tm;
		pDC->GetTextMetrics(&tm);
		
		for(int i=0;i<nLineCount;i++)
		{
			int LineLength=m_pTextEdit->LineLength(m_pTextEdit->LineIndex(i));
			m_pTextEdit->GetLine(i,strText.GetBuffer(LineLength+1),LineLength+1);
			strText.ReleaseBuffer();
			pDC->TextOut(PositionPoint.x,PositionPoint.y+i*(tm.tmHeight+tm.tmExternalLeading),strText.Left(LineLength));
			strText.Empty();
		}
		pDC->SetBkMode(pOldBkMode);
		pDC->SelectObject(pOldFond);
		pDC->SetBkColor(oldBKColor);
		pDC->SetTextColor(oldPenColor);
		pDC->SelectObject(pOldBrush);
	}
}

void CText::OnMouseMove(CDC *pDC, PGRAPHCAPTION pCaption, CPoint point)
{
  CRect rect;
  m_pTextEdit->GetWindowRect(&rect);
  pDC->DPtoLP(&rect);
  CPen pen;
  pen.CreatePen(PS_SOLID,2,RGB(0,128,0));
  CPen*pOldPen=pDC->SelectObject(&pen);
  if(m_pTextEdit->m_IsMoving)
  {
	  CPoint PositionPoint=GetDstPoint(m_PositionPoint);
	  //this->SetTextEditPosition(pDC,pView,point);
	  //pDC->Rectangle(m_PositionPoint.x+1,m_PositionPoint.y+1,m_PositionPoint.x+rect.Width()-1,m_PositionPoint.y+rect.Height()-1);
	  pDC->DrawFocusRect(CRect(PositionPoint.x+1,PositionPoint.y+1,
		  PositionPoint.x+rect.Width()-1,
		  PositionPoint.y+rect.Height()-1));
	  m_PositionPoint=GetSrcPoint(point);
	  //pDC->Rectangle(m_PositionPoint.x+1,m_PositionPoint.y+1,m_PositionPoint.x+rect.Width()-1,m_PositionPoint.y+rect.Height()-1);
	  pDC->DrawFocusRect(CRect(PositionPoint.x+1,PositionPoint.y+1,
		  PositionPoint.x+rect.Width()-1,
		  PositionPoint.y+rect.Height()-1));
  }
  pDC->SelectObject(pOldPen);
}

void CText::OnMenuitemOk(CDC *pDC, PGRAPHCAPTION pCaption)
{
  if(m_pTextEdit->m_IsMoving)
  {
    CRect rect;
		CPoint PositionPoint=GetDstPoint(m_PositionPoint);
    m_pTextEdit->GetWindowRect(&rect);
    pDC->DPtoLP(&rect);
		pDC->DrawFocusRect(CRect(PositionPoint.x+1,PositionPoint.y+1,
			PositionPoint.x+rect.Width()-1,
			PositionPoint.y+rect.Height()-1));
		SetTextEditPosition(pDC,pCaption,PositionPoint);
		m_pTextEdit->m_IsMoving=false;
  }
}

BOOL CText::IsInRgn(CPoint point)
{
	CRect rect1;
	CRect rect2;
	CPoint PositionPoint=GetDstPoint(m_PositionPoint);
	rect1.SetRect(PositionPoint.x-5,PositionPoint.y-5,
								PositionPoint.x+m_Width+5,PositionPoint.y+m_Hight+5);
	rect2.SetRect(PositionPoint.x+10,PositionPoint.y+10,
								PositionPoint.x+m_Width-10,PositionPoint.y+m_Hight-10);
	return rect1.PtInRect(point)&&!rect2.PtInRect(point);
}

int CText::IsOnMarginPoint(CPoint point)
{
	CRect rect1;
	CRect rect2;
	CPoint PositionPoint=GetDstPoint(m_PositionPoint);

	rect1.SetRect(PositionPoint.x-3,PositionPoint.y-3,
		PositionPoint.x+m_Width+3,PositionPoint.y+m_Hight+3);
	rect2.SetRect(PositionPoint.x+3,PositionPoint.y+3,
		PositionPoint.x+m_Width-3,PositionPoint.y+m_Hight-3);
	if(rect1.PtInRect(point)&&!rect2.PtInRect(point))
		return 1;
	return 0;
}

void CText::ShowSelectPoint(CDC *pDC)
{
	CPen m_pen;
	m_pen.CreatePen(PS_SOLID,1,RGB(0,0,255));
	CPen *OldPen=pDC->SelectObject(&m_pen);
	int oldBkMode=pDC->SetBkMode(TRANSPARENT);
	int OldRop=pDC->SetROP2(R2_NOTXORPEN);   

	CRect rect;
	CPoint PositionPoint=GetDstPoint(m_PositionPoint);
	rect.SetRect(PositionPoint.x,PositionPoint.y,
		PositionPoint.x+m_Width,PositionPoint.y+m_Hight);

	pDC->Rectangle(rect);
	pDC->SelectObject(OldPen);
	pDC->SetBkMode(oldBkMode);
	pDC->SetROP2(OldRop);
}

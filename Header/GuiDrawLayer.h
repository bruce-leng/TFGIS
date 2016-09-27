/****************************************************************************
 *																			*		 
 *								GuiToolKit  								*	
 *							 (MFC extension)								*			 
 * Created by Francisco Campos G. www.beyondata.com fcampos@beyondata.com	*
 *--------------------------------------------------------------------------*		   
 *																			*
 * This program is free software;so you are free to use it any of your		*
 * applications (Freeware, Shareware, Commercial),but leave this header		*
 * intact.																	*
 *																			*
 * These files are provided "as is" without warranty of any kind.			*
 *																			*
 *			       GuiToolKit is forever FREE CODE !!!!!					*
 *																			*
 *--------------------------------------------------------------------------*
 * Created by: Francisco Campos G.											*
 * Bug Fixes and improvements : (Add your name)								*
 * -Francisco Campos														*				
 *																			*	
 ****************************************************************************/





#if !defined(AFX_GUIDRAWLAYER_H__0BFB5AA0_94B4_47FC_8F93_84DB87A364F8__INCLUDED_)
#define AFX_GUIDRAWLAYER_H__0BFB5AA0_94B4_47FC_8F93_84DB87A364F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "GuiLib.h" 
#include "GuiNormalButton.h"
#define NORMALBTN   1
#define OVERBTN   1
#define PRESSBTN  2
class GUILIBDLLEXPORT CGuiControlBarButton 
{
	
public:
	enum GuiType
	{
		GUINORMAL=0,
		GUITOOLBUTTON
	};
	CRect rcArea;
protected:

	int m_nIcon;
	LPCTSTR m_lpMsg;
	CImageList m_imgList;
	GuiType bTypeButton;
	BOOL m_bEnabled;
	CToolTipCtrl   m_toolTip;
public:

	CGuiControlBarButton();
	void SetData(int nIcon,LPCTSTR lpMsg);
	~CGuiControlBarButton();
	void Paint(CDC* pDC,int st,CRect rc,COLORREF clrFondo);
	void SetImageList(UINT nBitmapID, int cx, int nGrow, COLORREF crMask);
	void SetTypeButton(GuiType bTypeb=GUINORMAL);
	void SetEnabled(BOOL bEnabled);
	
	
};

class GUILIBDLLEXPORT GuiTabbedButton: public CGuiNormalButton
{
public:
	GuiTabbedButton();
	virtual ~GuiTabbedButton();
	
protected:
	BOOL	bPress;
	int		m_iMilliseconds;
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	void SetWait(int iMilliseconds=50){m_iMilliseconds=iMilliseconds; };
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

class GUILIBDLLEXPORT GuiDrawLayer  
{
public:
		GuiDrawLayer();
		virtual ~GuiDrawLayer();
public:

	//************************************************************
	void		DrawArrow(CDC* pDC,CRect m_rc,BOOL m_bDown=TRUE);
	HICON		GetIcon(int nIcon);
	static HICON		LoadIconLib(UINT uIcon);
	static COLORREF	GetRGBColorXP();
	static COLORREF	GetRGBFondoXP();
	static COLORREF	GetRGBCaptionXP();
	static COLORREF	GetRGBPressBXP();
	static COLORREF	GetRGBColorFace();
	static COLORREF	GetRGBColorShadow();
	static COLORREF	GetRGBColorBTNHigh();
	static COLORREF	GetRGBColorTabs();
	static COLORREF	GetRGBColorWhite();
	static COLORREF GetRGBTitleMenu();
	static COLORREF GetRGBMenu();
	static COLORREF GetRGBColorGrayText();
	static void DrawCheck(CDC* pDC,CRect m_rcTemp);
	static BOOL		IsGrayed();
	static void DrawShade( CRect Rect,CPoint cp,CDC* pDC);
	static COLORREF DarkenColor( long lScale, COLORREF lColor);
	static COLORREF GetRGBSkinMenu();
	//************************************************************

protected:
	//**********************************
	CImageList	m_img;
	CPoint		mHeight;
	int			m_iWidthDrowDown;
	//**********************************

};

#endif // !defined(AFX_GUIDRAWLAYER_H__0BFB5AA0_94B4_47FC_8F93_84DB87A364F8__INCLUDED_)

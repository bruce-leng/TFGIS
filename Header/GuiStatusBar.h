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





#pragma once


// CGuiStatusBar

#include "GuiLib.h" 

//****************************************************************
class GUILIBDLLEXPORT  CGuiStatusBar : public CStatusBar
{
	DECLARE_DYNAMIC(CGuiStatusBar)

public:
	enum Style{
		   DEVSTUDIO=0,
           OFFICE=1
	};
	//**************************************
	CGuiStatusBar();
	virtual ~CGuiStatusBar();
	//**************************************
public:
	
	void		 Drawpanels(CDC *pDC);
	virtual void PreSubclassWindow();
	void		 DrawLB(CRect *rcLb,CDC* pDC);
	void		 ClearRect(CDC* pDC,CRect rc);
	void		 SetStyle(Style nStyle=CGuiStatusBar::DEVSTUDIO);
protected:
	Style m_Style;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};





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
#include "guitabwnd.h"

#include "GuiLib.h" 
#include "GuiDrawLayer.h" 

class GUILIBDLLEXPORT CGuiTabbed :	public CGuiTabWnd
{
protected:
	GuiTabbedButton m_toolBtn[2];
	CGuiNormalButton m_toolBtnC;
	CImageList   m_img;
	CSize        m_sizeBtn;
	int			 m_nDif;	
public:
	CGuiTabbed(void);
	virtual ~CGuiTabbed(void);
public:
	void	virtual Drawtabs(CDC* dc);
	CSize   GetSizeButton();
	void  OnScrollLeft();
	void  OnScrollRight();
	void  virtual OnScrollClose();
public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSysColorChange( );

};

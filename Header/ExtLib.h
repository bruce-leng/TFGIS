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





//Thanks to Juno Kampstra for this suggestion  


#include "CBaseTab.h"
#include "CoolMenu.h"
#include "GuiBaseTab.h"
#include "GuiButton.h"
#include "GuiCapOutBar.h"
#include "GuicheckBox.h"
#include "GuiColors.h"
#include "GuiComboBoxExt.h"
#include "GuiContainer.h"
#include "GuiControlBar.h"
#include "GuiDocBarExten.h"
#include "GuiDocBarExtenEx.h"
#include "GuiDockBar.h"
#include "GuiDockContext.h"
#include "GuiDockToolBar.h"
#include "GuiDocSpecial.h"
#include "GuiDrawLayer.h"
#include "GuiDropDownEdit.h"
#include "GuiEdit.h"
#include "GuiFDir.h"
#include "GuiFile.h"
#include "GuiFolder.h"
#include "GuiFrameWnd.h"
#include "GuiGroupBox.h"
#include "GuiheaderCtrl.h"
#include "GuiImageLinkButton.h"
#include "GuiLabelButton.h"
#include "GuiLib.h"
#include "GuiLinkButton.h"
#include "GuiListEdit.h"
#include "GuiMDIFrame.h"
#include "GuiMDITabbed.h"
#include "GuiMiniSplitter.h"
#include "GuiMiniTool.h"
#include "GuiNormalButton.h"
#include "GuiOfficeBar.h"
#include "GuiOutLook.h"
#include "GuiOutLookView.h"
#include "GuiPowerPointView.h"
#include "GuiRadioButton.h"
#include "GuiSliderCtrl.h"
#include "GuiSplitterWnd.h"
#include "GuiStatusBar.h"
#include "GuiTabbed.h"
#include "GuiTabPowerPoint.h"
#include "GuiTabWnd.h"
#include "GuiToolBarWnd.h"
#include "GuiToolButton.h"
#include "GuiVisioFolder.h"
#include "GuiWorkPanel.h"
#include "GuiWorkTab.h"
#include "MenuBar.h"
#include "Subclass.h"
//--------------------- new classes  -------------------------
#include "GuiViewWorkTab.h"
#include "GuiView.h"
#include "GuiPanelAccess.h"
#include "GuiMiniFrame.h"
#include "GuiFolderFlat.h"
#include "GuiMiniFrameWnd.h"
#include "GuiSplitter.h"
#include "GuiSysTray.h"
#include "GuiAdodb.h"

#if defined _AFXDLL  

	#if defined( _DEBUG )
		#pragma comment(lib, "guilibd114.lib") 
		#pragma message("Automatically linking with guilibd114.dll")

	#else
		#pragma comment(lib, "guilib114.lib") 
		#pragma message("Automatically linking with guilib114.dll")
	#endif
#elif defined _GUILIB_STATIC_
	#if defined( _DEBUG )
		#pragma comment(lib, "guilibstatic.lib") 
		#pragma message("Automatically linking with guilibstatic.lib")

	#else
		#pragma comment(lib, "guilibstatic.lib") 
		#pragma message("Automatically linking with guilibstatic.lib")
	#endif


#endif

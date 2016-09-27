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






#ifndef __GUILIB_H
#define __GUILIB_H





#if defined _AFXDLL 
	   #define GUILIBDLLEXPORT  AFX_EXT_CLASS
#else 
	#define GUILIBDLLEXPORT
	#pragma message("Static")
#endif


/*
#ifndef BUILD_GUILIB_DLL
#ifdef _AFXDLL
	#ifdef GUILIBDLLEXPORT
		#ifdef _DEBUG
			#pragma comment(lib, "pxed.lib")
		#else
			#pragma comment(lib, "pxe.lib")
		#endif
	#else
		#ifdef _DEBUG
			#pragma comment(lib, "pxld.lib")
		#else
			#pragma comment(lib, "pxl.lib")
		#endif
	#endif
#else
	#ifdef _DEBUG
		#pragma comment(lib, "pxlsd.lib")
	#else
		#pragma comment(lib, "pxls.lib")
	#endif
#endif
#endif // BUILD_PIXIE_DLL

*/

#if defined(_AFXDLL) && !defined(GUILIBDLLEXPORT)
// if using MFC DLL, force-link PixieLib StdPL.obj for precompiled hdr stuff
#pragma comment(linker, "/include:__linkPixieLib")
#endif

//#define GUILIBDLLEXPORT AFX_EXT_CLASS

#endif


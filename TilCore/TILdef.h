#ifndef __TILDEF_2004_2004_1_37998_10_4_7_25509_942649__H__
#define __TILDEF_2004_2004_1_37998_10_4_7_25509_942649__H__

#if _MSC_VER > 1000
	#pragma once
#endif // _MSC_VER > 1000
// Copyright (C) 2003-2004 Leehua Hsu
// .$FILE

//#define MATHDLLEXPORT   __declspec( dllexport )

#ifdef PI
	#undef PI
#endif

#ifdef _AFXDLL
	#define TILEXTCLASS       AFX_CLASS_EXPORT
	#define TILEXTAPI         AFX_API_EXPORT
	#define TILEXTDATA        AFX_DATA_EXPORT
	#define TILEXTDATADEF
#else
	#define TILEXTCLASS       AFX_CLASS_IMPORT
	#define TILEXTAPI         AFX_API_IMPORT
	#define TILEXTDATA        AFX_DATA_IMPORT
	#define TILEXTDATADEF
#endif

#define CHANNAL_RED		0x000001
#define CHANNAL_BLUE	0x000002
#define CHANNAL_GREEN	0x000004
#define CHANNAL_IMAGE	0x000008
#define CHANNAL_GRAPH	0x000010

#define WM_SET_CUR_GRAPH_LAYER	WM_USER+0X1002

#ifndef PI
const double PI =  3.14159265359;
#endif


#endif //__TILDEF_2004_2004_1_37998_10_4_7_25509_942649__H__

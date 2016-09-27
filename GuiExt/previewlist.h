#if !defined(AFX_PREVIEWLIST_H__BA514000_0EFC_4569_A1A6_89CC28BC0657__INCLUDED_)
#define AFX_PREVIEWLIST_H__BA514000_0EFC_4569_A1A6_89CC28BC0657__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreviewList.h : header file
//
#include "GUIExtStd.h"
#include <vector>
#include "GuiDefine.h"
/////////////////////////////////////////////////////////////////////////////
// CPreviewList window
using namespace std;

#define	THUMBNAIL_WIDTH		100
#define	THUMBNAIL_HEIGHT	90

// 图像文件指针
typedef struct IMAGELIST
{
	CString strImageName;	// 图像名
	BYTE* pBuf;				// 图像文件数据指针
	long lSize;				// 数据长度
} IMAGELIST;
typedef vector< IMAGELIST > VIMAGELIST;

// 岩性识别结果(09.06.01)
typedef struct __tagRECOGTEXTURE
{
	CString strDK;			// 掌子面里程
	CString strRockLith;	// 岩性描述,如：花岗岩，中风化
} RECOGTEXTURE, *PRECOGTEXTURE, **PPRECOGTEXTURE;

//##ModelId=3FFE4BD20022
typedef void (*PFUNLOGINFO)(const CString& str);
//##ModelId=3FFE4BD2004E
class TILEXTCLASS CPreviewList : public CListCtrl
{
// Construction
public:
	//##ModelId=3FFE4BD2005E
	CPreviewList();

	//////////////////////////////////////////////////////////////////////////
	// 添加(08.09.10)
	// 加入图像指针
	void AddImagePtr(CString strImageName, BYTE* pBuf, long lSize);
	void DelImagePtrAll();

	// 该变量用于存储图像名及数据文件指针
	VIMAGELIST m_vImageList;
	//////////////////////////////////////////////////////////////////////////

	
// Attributes
public:
	//##ModelId=3FFE4BD2006E
	PFUNLOGINFO m_pfunLog;	
	//##ModelId=3FFE4BD2007D
	BOOL Initialize();
	
	//##ModelId=3FFE4BD2007E
	void SetMessageInfo(HWND hWnd, DWORD dwMessage)
	{m_hRevWnd = hWnd; m_dwRevMessage = dwMessage;};

	//##ModelId=3FFE4BD2008E
	HWND m_hRevWnd;
	//##ModelId=3FFE4BD200DA
	DWORD m_dwRevMessage;
	//##ModelId=3FFE4BD200DB
	bool IsRunning() { return m_bRunning;}
	//##ModelId=3FFE4BD200DC
	void LoadThumbImages();
	
	//##ModelId=3FFE4BD200EA
	void DrawNail();
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPreviewList)
	//}}AFX_VIRTUAL

// Implementation
public:
	//##ModelId=3FFE4BD200EB
	void OpenNewDir();
	//##ModelId=3FFE4BD200EC
	virtual ~CPreviewList();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPreviewList)
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPreviewListAddFile();
	afx_msg void OnOpenDir();
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnImageFileInfo();
	afx_msg void OnRecogTexture();		// 岩性识别
	afx_msg void OnAddToFace();
	afx_msg void OnUpdateAddToFace(CCmdUI* pCmdUI);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	//##ModelId=3FFE4BD2011C
	void SelectDirectory(LPCSTR pstr);
	//##ModelId=3FFE4BD20129
	BOOL TerminateThread();
	//##ModelId=3FFE4BD20138
	bool m_bRunning;			// Flag to whether thread is on running or not
	//##ModelId=3FFE4BD20139
	bool m_bTerminate;			// Flag to Thread to be terminated
	//##ModelId=3FFE4BD20148
	unsigned int m_dwThreadID;	// Thread ID
	HANDLE m_hThread, m_hEvent; // Thread and Event handle
	//##ModelId=3FFE4BD20157
	static unsigned __stdcall	LoadThumbNail(LPVOID lpParam);
	//##ModelId=3FFE4BD20168
	CImageList				m_ImageListThumb;		// image list holding the thumbnails

	//##ModelId=3FFE4BD20187
	vector<CString> m_vFileName;
	//##ModelId=3FFE4BD20196
	void SetPathInfo(const CString& strCurrentDirectory, vector<CString>* vFileName);
	//##ModelId=3FFE4BD201A6
	CString m_strCurrentDirectory;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREVIEWLIST_H__BA514000_0EFC_4569_A1A6_89CC28BC0657__INCLUDED_)

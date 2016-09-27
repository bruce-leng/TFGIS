// TilSys.h : main header file for the TILSYS application
//

#if !defined(AFX_TILSYS_H__D6BDDAEB_39BC_413C_9DA0_DAEE507616E0__INCLUDED_)
#define AFX_TILSYS_H__D6BDDAEB_39BC_413C_9DA0_DAEE507616E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

#include "TilCanvasMgr.h"

CTilCanvasMgr* GetCanvasMgr();
/////////////////////////////////////////////////////////////////////////////
// CTilSysApp:
// See TilSys.cpp for the implementation of this class
//
class CMSDITemplate;

class CTilSysApp : public CWinApp
{
public:
	CTilSysApp();
	
	CTilCanvasMgr* m_pCanvasMgr;
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CMSDITemplate* GetDocTemplate(CRuntimeClass* pViewClass);
	CMSDITemplate* GetDocTemplate(CView* pView);
	CDocument* GetDocument();
	CView* GetView(CRuntimeClass* pViewClass);
	
	virtual CMDIChildWnd* CreateOrActivateFrame(CRuntimeClass* pViewClass, CDocument* pDoc);
	virtual CDocument* OpenDocumentFile(LPCSTR pszPathName);
	virtual BOOL CloseDocument();
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTilSysApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CTilSysApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	afx_msg void OnProjectOpen();
	afx_msg void OnProjectSave();
	afx_msg void OnProjectSaveAs();
	afx_msg void OnFileSave();
	afx_msg void OnProjectNew();
	afx_msg void OnProjectClose();
	afx_msg void OnFileSaveAll();
	afx_msg void OnUpdateFileSaveAll(CCmdUI* pCmdUI);
	afx_msg void OnUpdateProjectSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateProjectSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnUpdateProjectClose(CCmdUI* pCmdUI);
	afx_msg void OnAppHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL PromptForProjName(CString& fileName, UINT nIDSTitle, 
	DWORD dwFlags, BOOL bOpenFileDialog, const CString& strExt = "stu");
	BOOL PromptForFileName(CString& fileName, UINT nIDSTitle, 
	DWORD dwFlags, BOOL bOpenFileDialog, int* pType=NULL);
	int GetIndexFromType(int nDocType, BOOL bOpenFileDialog);
	int GetTypeFromIndex(int nIndex, BOOL bOpenFileDialog);
	CString GetExtFromType(int nDocType);
	CString GetDescFromType(int nDocType);
	CString GetFileTypes(BOOL bOpenFileDialog);
	BOOL GetWritableType(int nDocType);

//private:
//	//
//	void OpenDocumentFiles(const CString& fileNames);
};

struct DocType
{
public:
	int nID;
	BOOL bRead;
	BOOL bWrite;
	const char* description;
	const char* ext;
};

/////////////////////////////////////////////////////////////////////////////
// MultiSingle doc template for MSDI program (only one document in a MDI)

class CMSDITemplate : public CMultiDocTemplate
{
// Constructors
public:
	
  CMSDITemplate(UINT nIDResource, CRuntimeClass* pDocClass,
    CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass) :
  CMultiDocTemplate(nIDResource, pDocClass, pFrameClass, pViewClass) {};

  // this override to reset the document counter to 1
  void SetUntitleCount(UINT Count)
    { m_nUntitledCount = Count; };

  // this overrides to retreive the view and doc runtime class
  CRuntimeClass* GetViewClass()
    { return m_pViewClass; }
	
  CRuntimeClass* GetDocClass()
    { return m_pDocClass; }
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TILSYS_H__D6BDDAEB_39BC_413C_9DA0_DAEE507616E0__INCLUDED_)

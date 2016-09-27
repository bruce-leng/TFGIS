// TilSys.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TilSys.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "TilSysDoc.h"
#include "TilSysView.h"
#include "OpenGlFrm.h"
#include "MeshView.h"
#include "Splash.h"
#include "GuiTabWnd.h"
#include "TilComFunction.h"

#include "ximage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////

DocType doctypes[CMAX_IMAGE_FORMATS] =
{
	{ -1, TRUE, TRUE, "Supported files", "*.bmp;*.gif;*.jpg;*.jpeg;*.png;*.ico;*.tif;*.tiff;*.tga;*.pcx;*.wbmp;*.wmf;*.emf;*.j2k;*.jp2;*.jbg;*.j2c;*.jpc;*.pgx;*.pnm;*.pgm;*.ppm;*.ras" },
#if CXIMAGE_SUPPORT_BMP
	{ CXIMAGE_FORMAT_BMP, TRUE, TRUE, "BMP files", "*.bmp" },
#endif
#if CXIMAGE_SUPPORT_GIF
	{ CXIMAGE_FORMAT_GIF, TRUE, TRUE, "GIF files", "*.gif" },
#endif
#if CXIMAGE_SUPPORT_JPG
	{ CXIMAGE_FORMAT_JPG, TRUE, TRUE, "JPG files", "*.jpg;*.jpeg" },
#endif
#if CXIMAGE_SUPPORT_PNG
	{ CXIMAGE_FORMAT_PNG, TRUE, TRUE, "PNG files", "*.png" },
#endif
#if CXIMAGE_SUPPORT_MNG
	{ CXIMAGE_FORMAT_MNG, TRUE, TRUE, "MNG files", "*.mng;*.jng;*.png" },
#endif
#if CXIMAGE_SUPPORT_ICO
	{ CXIMAGE_FORMAT_ICO, TRUE, TRUE, "ICO CUR files", "*.ico;*.cur" },
#endif
#if CXIMAGE_SUPPORT_TIF
	{ CXIMAGE_FORMAT_TIF, TRUE, TRUE, "TIF files", "*.tif;*.tiff" },
#endif
#if CXIMAGE_SUPPORT_TGA
	{ CXIMAGE_FORMAT_TGA, TRUE, TRUE, "TGA files", "*.tga" },
#endif
#if CXIMAGE_SUPPORT_PCX
	{ CXIMAGE_FORMAT_PCX, TRUE, TRUE, "PCX files", "*.pcx" },
#endif
#if CXIMAGE_SUPPORT_WBMP
	{ CXIMAGE_FORMAT_WBMP, TRUE, TRUE, "WBMP files", "*.wbmp" },
#endif
#if CXIMAGE_SUPPORT_WMF
	{ CXIMAGE_FORMAT_WMF, TRUE, FALSE, "WMF EMF files", "*.wmf;*.emf" },
#endif
#if CXIMAGE_SUPPORT_J2K
	{ CXIMAGE_FORMAT_J2K, TRUE, TRUE, "J2K files", "*.j2k;*.jp2" },
#endif
#if CXIMAGE_SUPPORT_JBG
	{ CXIMAGE_FORMAT_JBG, TRUE, TRUE, "JBG files", "*.jbg" },
#endif
#if CXIMAGE_SUPPORT_JP2
	{ CXIMAGE_FORMAT_JP2, TRUE, TRUE, "JP2 files", "*.j2k;*.jp2" },
#endif
#if CXIMAGE_SUPPORT_JPC
	{ CXIMAGE_FORMAT_JPC, TRUE, TRUE, "JPC files", "*.j2c;*.jpc" },
#endif
#if CXIMAGE_SUPPORT_PGX
	{ CXIMAGE_FORMAT_PGX, TRUE, TRUE, "PGX files", "*.pgx" },
#endif
#if CXIMAGE_SUPPORT_RAS
	{ CXIMAGE_FORMAT_RAS, TRUE, TRUE, "RAS files", "*.ras" },
#endif
#if CXIMAGE_SUPPORT_PNM
	{ CXIMAGE_FORMAT_PNM, TRUE, TRUE, "PNM files", "*.pnm;*.pgm;*.ppm" }
#endif
};
/////////////////////////////////////////////////////////////////////////////

// CTilSysApp

BEGIN_MESSAGE_MAP(CTilSysApp, CWinApp)
//{{AFX_MSG_MAP(CTilSysApp)
ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_PROJECT_OPEN, OnProjectOpen)
	ON_COMMAND(ID_PROJECT_SAVE, OnProjectSave)
	ON_COMMAND(ID_PROJECT_SAVE_AS, OnProjectSaveAs)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_PROJECT_NEW, OnProjectNew)
	ON_COMMAND(ID_PROJECT_CLOSE, OnProjectClose)
	ON_COMMAND(ID_FILE_SAVE_ALL, OnFileSaveAll)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_ALL, OnUpdateFileSaveAll)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_SAVE, OnUpdateProjectSave)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_SAVE_AS, OnUpdateProjectSaveAs)
	ON_UPDATE_COMMAND_UI(ID_PROJECT_CLOSE, OnUpdateProjectClose)
	ON_COMMAND(ID_APP_HELP, OnAppHelp)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTilSysApp construction

CTilSysApp::CTilSysApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_pCanvasMgr = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTilSysApp object

CTilSysApp theApp;
CTilCanvasMgr* GetCanvasMgr()
{
	return theApp.m_pCanvasMgr;
}

/////////////////////////////////////////////////////////////////////////////
// CTilSysApp initialization

BOOL CTilSysApp::InitInstance()
{
	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);

	// 创建画布管理类对象
	m_pCanvasMgr = new CTilCanvasMgr;
	m_pCanvasMgr->New();
	
	AfxEnableControlContainer();
	
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
	
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
	
	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	
	LoadStdProfileSettings();  // Load standard INI file options (including MRU)
	
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.
	
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_TILSYSTYPE,
		RUNTIME_CLASS(CTilSysDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CTilSysView));
	AddDocTemplate(pDocTemplate);

	CMultiDocTemplate* pOpenGLDocTemplate;
	pOpenGLDocTemplate = new CMultiDocTemplate(
		IDR_MENU_OPENGL,
		RUNTIME_CLASS(CTilSysDoc),
		RUNTIME_CLASS(COpenGlFrm),
		RUNTIME_CLASS(CMeshView));
	AddDocTemplate(pOpenGLDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();
	pMainFrame->DragAcceptFiles();
	
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	
	// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//{{AFX_MSG_MAP(CAboutDlg)
// No message handlers
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CTilSysApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

#include "Splash.h"
/////////////////////////////////////////////////////////////////////////////
// CTilSysApp message handlers

BOOL CTilSysApp::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following lines were added by the Splash Screen component.
	if (CSplashWnd::PreTranslateAppMessage(pMsg))
		return TRUE;
	
	return CWinApp::PreTranslateMessage(pMsg);
}

BOOL CTilSysApp::CloseDocument()
{
  if (CWinApp::SaveAllModified())
  {
    // Find first doc template
    POSITION pos = GetFirstDocTemplatePosition();
    ASSERT(pos != NULL);
    CDocTemplate* pTemplate = GetNextDocTemplate(pos);
		
    // get the document
    pos = pTemplate->GetFirstDocPosition();
    if (pos != NULL)
    {
      CDocument* pDoc = pTemplate->GetNextDoc(pos);
      // remove Document (only 1 possible)
      if (pDoc) pDoc->OnCloseDocument();
    }
    // I have override this class to make this correction
    ((CMSDITemplate *)pTemplate)->SetUntitleCount(0);
		
    return TRUE;
  }
  return FALSE;
}

CMSDITemplate* CTilSysApp::GetDocTemplate(CRuntimeClass* pViewClass)
{
  CMSDITemplate* pTemplate;
  POSITION pos = GetFirstDocTemplatePosition();
  while (pos != NULL)
  {
    pTemplate = (CMSDITemplate*)GetNextDocTemplate(pos);
    if (pTemplate->GetViewClass() == pViewClass) return pTemplate;
  }
  return NULL;
}

CMSDITemplate* CTilSysApp::GetDocTemplate(CView* pView)
{
  CMSDITemplate* pTemplate;
  POSITION pos = GetFirstDocTemplatePosition();
  while (pos != NULL)
  {
    pTemplate = (CMSDITemplate*)GetNextDocTemplate(pos);
    if (pView->IsKindOf(pTemplate->GetViewClass())) return pTemplate;
  }
  return NULL;
}

CDocument* CTilSysApp::GetDocument()
{
  CMDIChildWnd* pMDIActive = ((CMDIFrameWnd*)m_pMainWnd)->MDIGetActive();
  if (pMDIActive == NULL) return NULL;
  CDocument* pDoc = pMDIActive->GetActiveDocument();
  return pDoc;
}

CView* CTilSysApp::GetView(CRuntimeClass* pViewClass)
{
  CDocument* pDoc = GetDocument();
  if (pDoc == NULL) return NULL;
  CView* pView;
  POSITION pos = pDoc->GetFirstViewPosition();
  while (pos != NULL)
  {
    pView = pDoc->GetNextView(pos);
    if (pView->IsKindOf(pViewClass)) return pView;
  }
  return NULL;
}

CMDIChildWnd* CTilSysApp::CreateOrActivateFrame(CRuntimeClass* pViewClass, CDocument* pDoc)
{
  // If a view already exists, then activate the MDI child window containing
  // the view. Otherwise, create a new view.
  if (pDoc != NULL) // only for views with document
  {
    CView* pView = GetView(pViewClass);
    if (pView != NULL)
    {
      pView->GetParentFrame()->ActivateFrame();
      return NULL;
    }
  }

  // find the doc template with this view
  CMSDITemplate* pTemplate = GetDocTemplate(pViewClass);
  if (pTemplate == NULL) return NULL;  // not created
  CMDIChildWnd* pNewFrame =
    (CMDIChildWnd*)(pTemplate->CreateNewFrame(pDoc, NULL));
  if (pNewFrame == NULL) return NULL;  // not created
  pTemplate->InitialUpdateFrame(pNewFrame, pDoc);
	
  // use the fist string for window title
  CString strTitle;
  pTemplate->GetDocString(strTitle, CDocTemplate::windowTitle);
  strTitle += "OpenGL";
	
  pNewFrame->SetWindowText(strTitle);
  return pNewFrame;
}

CDocument* CTilSysApp::OpenDocumentFile(LPCSTR pszPathName)
{
	CFileFind finder;
	if (!finder.FindFile(pszPathName))
	{
		AfxMessageBox("您要打开的项目文件不存在!");
		return NULL;
	}
	
	CDocument* pDoc = NULL;
	POSITION pos = GetFirstDocTemplatePosition();
	ASSERT(pos != NULL);
	// the first doctemplate is the main view of the document
	CDocTemplate* pTemplate = GetNextDocTemplate(pos);
	
	if (CloseDocument())
	{		
		if (pszPathName != NULL)
		{   // use CString in place of LPCSTR to convert to memory model
			pTemplate->OpenDocumentFile(CString(pszPathName));
		}
		else
			pTemplate->OpenDocumentFile(NULL);
	}
	pos = pTemplate->GetFirstDocPosition();
	if (pos != NULL) pDoc = pTemplate->GetNextDoc(pos);
	{
		CreateOrActivateFrame(RUNTIME_CLASS(CMeshView), pDoc);
		CreateOrActivateFrame(RUNTIME_CLASS(CTilSysView), pDoc);
		((CMainFrame*)AfxGetMainWnd())->SetCurtab(0);
	}
	return pDoc;
}

void CTilSysApp::OnFileNew() 
{
	// TODO: Add your command handler code here
	CTilSysApp::OpenDocumentFile(NULL);
}

// 保存当前图形、图像文件
void CTilSysApp::OnFileSave() 
{
	// TODO: Add your command handler code here
	CTilCanvas* pCanvas = m_pCanvasMgr->GetCurCanvas();
	if (pCanvas)
	{
		if (pCanvas->GetImage()->IsModified())
			pCanvas->GetImage()->Save();
		if (pCanvas->GetGraph()->IsModified())
			pCanvas->GetGraph()->Save();

		pCanvas->SetModifiedFlag(FALSE);
	}
}

// 保存所有图形、图像文件
void CTilSysApp::OnFileSaveAll() 
{
	// 保存所有图形、图像文件
	for (int i = 0; i < m_pCanvasMgr->Size(); i ++)
	{
		CTilCanvas* pCanvas = m_pCanvasMgr->GetCanvasByIdx(i);
		if (pCanvas->IsModified())
			pCanvas->Save();
	}

	// 保存项目文件
	if (m_pCanvasMgr->IsModified())
		OnProjectSave();
}

void CTilSysApp::OnUpdateFileSaveAll(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	BOOL bModified = FALSE;
	for (int i = 0; i < m_pCanvasMgr->Size(); i ++)
	{
		CTilCanvas* pCanvas = m_pCanvasMgr->GetCanvasByIdx(i);
		if (pCanvas->IsModified())
		{
			bModified = TRUE;
			break;
		}
	}
	pCmdUI->Enable(m_pCanvasMgr->IsModified() || bModified);
}

void CTilSysApp::OnProjectNew() 
{
	if (m_pCanvasMgr->Size() > 0)
	{
		OnProjectSave();
		m_pCanvasMgr->RemoveAll();
	}

	CTilSysApp::OpenDocumentFile(NULL);
}

// 打开项目文件
void CTilSysApp::OnProjectOpen() 
{
  CString newName;
  if (!DoPromptFileName(newName, AFX_IDS_OPENFILE,
    OFN_HIDEREADONLY | OFN_CREATEPROMPT , TRUE, NULL)) return;
  CTilSysApp::OpenDocumentFile(newName);
}

// 保存项目文件
void CTilSysApp::OnProjectSave() 
{
	// 项目存储文件路径及文件名
	CString strFileName;
	
	// 获得以前保存的文件路径,如果没有,则查找画布的路径，根据画布路径自动存储项目文件
	if (m_pCanvasMgr->GetPathName().IsEmpty())
	{
		// 画布不存在的情况已经使按钮无效
		// 如果画布存在,则将项目文件存储在与画布相同的文件夹下
		CTilCanvas* pCanvas = m_pCanvasMgr->GetCanvasByIdx(0);
		strFileName = get_file_path(pCanvas->GetImage()->GetPathName());
		strFileName += "\\project.stu";
		
		m_pCanvasMgr->SaveAs(strFileName);
		((CTilSysDoc*)GetDocument())->UpdateDrawTree();
	}
	else
		m_pCanvasMgr->Save();
}

// 保存项目文件为..
void CTilSysApp::OnProjectSaveAs() 
{
	// 项目存储文件路径及文件名
	CString strFileName;

	// 获得以前保存的文件路径,如果没有,则查找画布的路径，根据画布路径自动存储项目文件
	if (m_pCanvasMgr->GetPathName().IsEmpty())
	{
		// 画布不存在的情况已经使按钮无效
		// 如果画布存在,则将项目文件存储在与画布相同的文件夹下
		CTilCanvas* pCanvas = m_pCanvasMgr->GetCanvasByIdx(0);
		strFileName = get_file_path(pCanvas->GetImage()->GetPathName());
		strFileName += "\\project.stu";
		
		m_pCanvasMgr->SaveAs(strFileName);
		((CTilSysDoc*)GetDocument())->UpdateDrawTree();
	}
	else
		m_pCanvasMgr->Save();
}

// 关闭项目文件
void CTilSysApp::OnProjectClose() 
{
	OnProjectSave();
	m_pCanvasMgr->RemoveAll();
	OpenDocumentFile(NULL);
}

//////////////////////////////////////////////////////////////////////////////
int CTilSysApp::GetIndexFromType(int nDocType, BOOL bOpenFileDialog)
{
	int nCnt = 0;
	for (int i=0;i<CMAX_IMAGE_FORMATS;i++){
		if (bOpenFileDialog ? doctypes[i].bRead : doctypes[i].bWrite){
			if (doctypes[i].nID == nDocType) return nCnt;
			nCnt++;
		}
	}
	return -1;
}
//////////////////////////////////////////////////////////////////////////////
int CTilSysApp::GetTypeFromIndex(int nIndex, BOOL bOpenFileDialog)
{
	int nCnt = 0;
	for (int i=0;i<CMAX_IMAGE_FORMATS;i++){
		if (bOpenFileDialog ? doctypes[i].bRead : doctypes[i].bWrite){
			if (nCnt == nIndex)
//              return i; // PJO - Buglet ?
                return doctypes[i].nID;
			nCnt++;
		}
	}
	ASSERT(FALSE);
	return -1;
}
//////////////////////////////////////////////////////////////////////////////
CString CTilSysApp::GetExtFromType(int nDocType)
{
	for (int i=0;i<CMAX_IMAGE_FORMATS;i++){
		if (doctypes[i].nID == nDocType)
			return doctypes[i].ext;
	}
	return CString("");
}
//////////////////////////////////////////////////////////////////////////////
CString CTilSysApp::GetDescFromType(int nDocType)
{
	for (int i=0;i<CMAX_IMAGE_FORMATS;i++){
		if (doctypes[i].nID == nDocType)
			return doctypes[i].description;
	}
	return CString("");
}
//////////////////////////////////////////////////////////////////////////////
BOOL CTilSysApp::GetWritableType(int nDocType){
	for (int i=0;i<CMAX_IMAGE_FORMATS;i++){
		if (doctypes[i].nID == nDocType)
			return doctypes[i].bWrite;
	}
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////
CString CTilSysApp::GetFileTypes(BOOL bOpenFileDialog)
{
	CString str;
	for (int i=0;i<CMAX_IMAGE_FORMATS;i++){
		if (bOpenFileDialog && doctypes[i].bRead){
			str += doctypes[i].description;
			str += (TCHAR)NULL;
			str += doctypes[i].ext;
			str += (TCHAR)NULL;
		} else if (!bOpenFileDialog && doctypes[i].bWrite) {
			str += doctypes[i].description;
			str += (TCHAR)NULL;
			str += doctypes[i].ext;
			str += (TCHAR)NULL;
		}
	}
	return str;
}

int CTilSysApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_pCanvasMgr)
		delete m_pCanvasMgr;

	return CWinApp::ExitInstance();
}


void CTilSysApp::OnUpdateProjectSave(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_pCanvasMgr->Size() > 0 && m_pCanvasMgr->IsModified());
}

void CTilSysApp::OnUpdateProjectSaveAs(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_pCanvasMgr->Size() > 0 && m_pCanvasMgr->IsModified());
}

void CTilSysApp::OnUpdateProjectClose(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_pCanvasMgr->Size() > 0);
}

// 打开帮助文档
#include <shlobj.h>  //  
#include <shlwapi.h>
#include <HtmlHelp.h>
void CTilSysApp::OnAppHelp() 
{
	// 打开doc帮助文档
	char szPath[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH, szPath);
	CString strPathName = szPath;
#if 0
	strPathName += "\\help.doc";
	{    
	SHELLEXECUTEINFO ShExecInfo;    //定义一个 SHELLEXECUTEINFO结构体  //变量，此结构体用来储存打开操作的一些信息            
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);         
	ShExecInfo.fMask = NULL;          
	ShExecInfo.hwnd = NULL;         
	ShExecInfo.lpVerb =  "open";	//要打开此处定义open，当然还有其他操作具体         
	ShExecInfo.lpFile = strPathName;//欲打开word路径         
	ShExecInfo.lpParameters = NULL;          
	ShExecInfo.lpDirectory = NULL;           
	ShExecInfo.nShow = SW_MAXIMIZE; //打开时最大化窗口          
	ShExecInfo.hInstApp = NULL;             
	ShellExecuteEx(&ShExecInfo);	//配置好后调用ShellExecuteEx（）函数，注意&  //不能少，具体查看ShellExecuteEx（）函数定义。  
	}
#else
	strPathName += "\\help.chm";
	HtmlHelp(NULL, strPathName, HH_DISPLAY_TOPIC, 0);
#endif
}

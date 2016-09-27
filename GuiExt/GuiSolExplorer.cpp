#include "StdAfx.h"
#include "guisolexplorer.h"
#include "resource.h"

#define ID_PROPERTIES 0x332
//#define ID_CLASSVIEW  0x333
#define ID_NEWFOLDER  0x334
#define TOKEN_PRJ 0x01

CGuiSolExplorer::CGuiSolExplorer(void)
{
}

CGuiSolExplorer::~CGuiSolExplorer(void)
{
}

BEGIN_MESSAGE_MAP(CGuiSolExplorer, CGuiControlBar)
	ON_WM_CREATE()
	ON_COMMAND(ID_PROPERTIES,  Onproperties)
	ON_COMMAND(ID_NEWFOLDER,  OnNewFolder)
END_MESSAGE_MAP()

int CGuiSolExplorer::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CGuiControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	//creamos el tab 
	if (!m_TabSolExplorer.Create(WS_VISIBLE|WS_CHILD,CRect(0,0,0,0),this,0x9999))
		return -1;

	const DWORD dwStyle =	WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS ;
	
	m_imgList.Create (IDB_DBPROJECTS, 16, 20, RGB (255, 0, 0));
	CreatContExplorer();
	if (!m_TreeSolExplorer.Create(dwStyle,	CRect(0,0,0,0), &m_ctSolution, 2))
		return -1;
	CreatContClassView();
	if (!m_TreeClassView.Create(dwStyle,	CRect(0,0,0,0), &m_ctClassView, 2))
		return -1;
	
	m_TreeSolExplorer.SetImageList(&m_imgList,TVSIL_NORMAL);
	m_TreeClassView.SetImageList(&m_imgList,TVSIL_NORMAL);
	m_TabSolExplorer.SetImageList(IDB_DBPROJECTS, 16,21, RGB (255, 0, 0));
	m_TabSolExplorer.Addtab(&m_ctSolution,"工程管理",0);
	m_TabSolExplorer.Addtab(&m_ctClassView,"Class View",1);
	FillerSolExplorer();
	FillTreeClassView();
	m_ctSolution.AddComponen(&m_TreeSolExplorer);
	m_ctClassView.AddComponen(&m_TreeClassView);
	return 0;
}

void CGuiSolExplorer::Onproperties()
{
	AfxMessageBox("Properties");
}
void CGuiSolExplorer::OnNewFolder()
{
	AfxMessageBox("New Folder");
}

int CGuiSolExplorer::CreatContClassView()
{
	if (!m_ctClassView.Create(WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),&m_TabSolExplorer,124))
		return -1;
//	m_ctClassView.AddComponen(&m_miClassView);
//	m_miClassView.AlingButtons(CGuiMiniTool::ALIGN_LEFT);
//	m_miClassView.SetImageList(IDB_DBPROJECTS, 16,24, RGB (255, 0, 0));
//	m_miClassView.AddButton(23,ID_NEWFOLDER,NULL,_T(""),"New Folder");
//	m_miClassView.AutoSize(FALSE);
//	m_miClassView.SetColor(GuiDrawLayer::GetRGBColorXP());
	return 1;
}

int CGuiSolExplorer::CreatContExplorer()
{
	if (!m_ctSolution.Create(WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),&m_TabSolExplorer,124))
		return -1;
//	m_ctSolution.AddComponen(&m_miSolution);
//	m_miSolution.AlingButtons(CGuiMiniTool::ALIGN_LEFT);
//	m_miSolution.SetImageList(IDB_DBPROJECTS, 16,22, RGB (255, 0, 0));
//	m_miSolution.AddButton(21,ID_PROPERTIES,NULL,_T(""),"properties");
//	m_miSolution.AutoSize(FALSE);
//	m_miSolution.SetColor(GuiDrawLayer::GetRGBColorXP());
	return 0;
}

void CGuiSolExplorer::FillerSolExplorer()
{
	HTREEITEM hRoot = m_TreeSolExplorer.InsertItem (_T("项目解决方案"), 0, 0);
	HTREEITEM hSrc = m_TreeSolExplorer.InsertItem (_T("未命名"), 11, 12, hRoot);
	m_TreeSolExplorer.SetItemData(hRoot, TOKEN_PRJ); //工程索引
	m_TreeSolExplorer.SetItemState (hRoot, TVIS_BOLD, TVIS_BOLD);
}

void CGuiSolExplorer::FillTreeClassView()
{
	HTREEITEM hRoot =m_TreeClassView.InsertItem (_T("Gui_DevStudio"), 0, 0);
	HTREEITEM hGlobal = m_TreeClassView.InsertItem (_T("Global Functions and Variables"), 16, 16, hRoot);
	m_TreeClassView.InsertItem (_T("indicators"), 18, 18, hGlobal);
	m_TreeClassView.InsertItem (_T("theApp"), 18, 18, hGlobal);
	m_TreeClassView.InsertItem (_T("THIS_FILE"), 18, 18, hGlobal);
	m_TreeClassView.InsertItem (_T("THIS_FILE"), 18, 18, hGlobal);
	m_TreeClassView.InsertItem (_T("THIS_FILE"), 18, 18, hGlobal);
	m_TreeClassView.InsertItem (_T("THIS_FILE"), 18, 18, hGlobal);
	m_TreeClassView.InsertItem (_T("THIS_FILE"), 18, 18, hGlobal);

	HTREEITEM hMacros = m_TreeClassView.InsertItem (_T("Macros and Constants"), 20, 20, hRoot);
	m_TreeClassView.InsertItem (_T("AFX_CHILDFRM_H_AF42DC2C"), 20, 20, hMacros);
	m_TreeClassView.InsertItem (_T("AFX_GUI_DEVSTUDIO_H"), 20, 20, hMacros);
	m_TreeClassView.InsertItem (_T("AFX_GUI_DECSTUDIOVIEW_H"), 20, 20, hMacros);
	m_TreeClassView.InsertItem (_T("AFX_MAINFRM_H"), 20, 20, hMacros);
	m_TreeClassView.InsertItem (_T("_APS_3D_CONTROLS"), 20, 20, hMacros);
	m_TreeClassView.InsertItem (_T("_APS_NEXT_COMMAND_VALUE"), 20, 20, hMacros);
	m_TreeClassView.InsertItem (_T("IDB_DBPROJECTS"), 20, 20, hMacros);
	m_TreeClassView.InsertItem (_T("IDR_MAINFRAME"), 20, 20, hMacros);
	m_TreeClassView.InsertItem (_T("new"), 20, 20, hMacros);

	HTREEITEM hDlg = m_TreeClassView.InsertItem (_T("CAboutDlg"), 15, 15, hRoot);

	m_TreeClassView.InsertItem (_T("Bases and Interfaces"), 15, 15,hDlg);
	m_TreeClassView.InsertItem (_T("Maps"), 15, 15, hDlg);
	m_TreeClassView.InsertItem (_T("_unnamed_d4099230_1"), 15, 15, hDlg);
	m_TreeClassView.InsertItem (_T("CAboutDlg(void)"), 15, 15, hDlg);

	HTREEITEM hChild = m_TreeClassView.InsertItem (_T("CChilFrame"), 15, 15, hRoot);
	HTREEITEM hChild1 = m_TreeClassView.InsertItem (_T("CGui_DevStudioApp"), 15, 15, hRoot);
	HTREEITEM hChild2 = m_TreeClassView.InsertItem (_T("CGui_DevStudioView"), 15, 15, hRoot);
	
	m_TreeClassView.Expand (hRoot, TVE_EXPAND);
	m_TreeClassView.Expand (hGlobal, TVE_EXPAND);
	m_TreeClassView.Expand (hMacros, TVE_EXPAND);
	m_TreeClassView.Expand (hDlg, TVE_EXPAND);
}

void CGuiSolExplorer::InsertFileToSolExplorer(const CString& fileName)
{
	HTREEITEM hRoot = m_TreeSolExplorer.GetRootItem();
	HTREEITEM hPrj = m_TreeSolExplorer.GetChildItem(hRoot);
	if (hRoot != NULL)
	{
		if (hPrj == NULL)
		{
			hPrj = m_TreeSolExplorer.InsertItem(_T("未命名"), 16, 16, hRoot);
		}
		if (hPrj != NULL)
		{
			m_TreeSolExplorer.InsertItem(_T(fileName), 18, 18, hPrj);
		}
	}
}

void CGuiSolExplorer::InsertProjToSolExplorer(const CString& projName)
{
	HTREEITEM hRoot = m_TreeSolExplorer.GetRootItem();
	HTREEITEM hPrj = m_TreeSolExplorer.GetChildItem(hRoot);
	if (hRoot != NULL)
	{
		m_TreeSolExplorer.DeleteItem(hPrj);

		if (projName.IsEmpty())
		{
			return;
		}
		else
		{
			m_TreeSolExplorer.InsertItem(_T(projName), 16, 16, hRoot);
		}
	}

	m_TreeSolExplorer.Expand (hPrj, TVE_EXPAND);
	m_TreeSolExplorer.Expand (hRoot, TVE_EXPAND);
}

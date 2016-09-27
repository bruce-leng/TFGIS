// PreviewList.cpp : implementation file
//

#include "stdafx.h"
#include "PreviewList.h"


#include <PROCESS.H>
#include "resource.h"
//#include "FolderFind.h"
#include "ximage.h"
#include "TilImage.h"
#include "GUIShellManager.h"
#include "Veins.h"
#include "BPnetwork.h"
#include "TilComFunction.h"

#include "extlib.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreviewList

//##ModelId=3FFE4BD2005E
CPreviewList::CPreviewList()
{
	m_bTerminate=false;
	m_bRunning=false;
	// Initialize Thread Handle and Create Event Handle for controlling thread
	m_hThread = NULL;
}

//##ModelId=3FFE4BD200EC
CPreviewList::~CPreviewList()
{
	TerminateThread();
}


BEGIN_MESSAGE_MAP(CPreviewList, CListCtrl)
	//{{AFX_MSG_MAP(CPreviewList)
	ON_COMMAND(ID_PREVIEWLIST_ADD_FILE, OnPreviewListAddFile)
	ON_COMMAND(ID_OPEN_DIR, OnOpenDir)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_COMMAND(ID_IMAGE_INFO, OnImageFileInfo)
	ON_COMMAND(ID_RECOG_TEXTURE, OnRecogTexture)
	ON_COMMAND(ID_ADD_TO_FACE, OnAddToFace)
	ON_UPDATE_COMMAND_UI(ID_ADD_TO_FACE, OnUpdateAddToFace)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreviewList message handlers
//##ModelId=3FFE4BD2007D
BOOL CPreviewList::Initialize()
{
	m_ImageListThumb.Create(THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT, ILC_COLOR24, 0, 1);
	SetImageList(&m_ImageListThumb, LVSIL_NORMAL);
	m_pfunLog = NULL;
	return TRUE;
}

//##ModelId=3FFE4BD20196
void CPreviewList::SetPathInfo(const CString& strCurrentDirectory, vector<CString>* vFileName)
{
//	m_strCurrentDirectory = strCurrentDirectory;
//	m_vFileName = vFileName;
}

//##ModelId=3FFE4BD2011C
void CPreviewList::SelectDirectory(LPCSTR pstr)
{
	m_strCurrentDirectory=pstr;
	if(m_strCurrentDirectory.Right(1) != '\\')
		m_strCurrentDirectory += "\\";

	m_vFileName.clear();

	CFileFind finder;
	CString strWildCard(m_strCurrentDirectory);
	strWildCard+="*.*";

	BOOL bWorking=finder.FindFile(strWildCard);

	while(bWorking)
	{
		bWorking=finder.FindNextFile();

		if(finder.IsDots() || finder.IsDirectory())
			continue;
		else
		{
			CString filePath=finder.GetFileName();

			// Get Image File Name List
			if (get_file_type(filePath) != CXIMAGE_FORMAT_UNKNOWN)
			{
				// Make Lower for Sorting
				filePath.MakeLower();
				m_vFileName.push_back(filePath);
			}
		}
	}
	finder.Close();

	DrawNail();
}

//##ModelId=3FFE4BD200EA
void CPreviewList::DrawNail()
{
	LoadThumbImages();
}

//##ModelId=3FFE4BD20157
unsigned __stdcall CPreviewList::LoadThumbNail(LPVOID lpParam)
{
	CPreviewList* ListCtrl=(CPreviewList*)lpParam;
	CImageList* pImgList=&(ListCtrl->m_ImageListThumb);

	// reset our image list
	for(int i=0; i<pImgList->GetImageCount(); i++)
		pImgList->Remove(i);	

	// remove all items from list view
	ListCtrl->DeleteAllItems();

	pImgList->SetImageCount(ListCtrl->m_vFileName.size());

	char path[MAX_PATH];
	vector<CString>::iterator iter;
	
	// Set redraw to FALSE to avoid flickering during adding new items
	ListCtrl->SetRedraw(FALSE);
	int nIndex=0;
	for(iter=ListCtrl->m_vFileName.begin(); iter!=ListCtrl->m_vFileName.end() && ListCtrl->m_bTerminate!=true; iter++, nIndex++)
	{
		ListCtrl->InsertItem(nIndex, *iter, nIndex);
	}

	ListCtrl->SetRedraw(TRUE);
	ListCtrl->Invalidate();

	// Create Brushes for Border and BackGround
	HBRUSH hBrushBorder=::CreateSolidBrush(RGB(192, 192, 192));
	HBRUSH hBrushBk=::CreateSolidBrush(RGB(255, 255, 255));

	// Border Size
	RECT rcBorder;
	rcBorder.left=rcBorder.top=0;
	rcBorder.right=THUMBNAIL_WIDTH;
	rcBorder.bottom=THUMBNAIL_HEIGHT;

	const float fRatio=(float)THUMBNAIL_HEIGHT/THUMBNAIL_WIDTH;

	int XDest, YDest, nDestWidth, nDestHeight;
	nIndex=0;
//	LogInfo("Reading...");

	if(ListCtrl->m_pfunLog)
		ListCtrl->m_pfunLog("Reading...");
	for(iter=ListCtrl->m_vFileName.begin(); iter!=ListCtrl->m_vFileName.end() && ListCtrl->m_bTerminate!=true; iter++, nIndex++)
	{
		// Load Image File
		sprintf(path, "%s\\%s", ListCtrl->m_strCurrentDirectory, *iter);
//		LogInfo(path);
		if(ListCtrl->m_pfunLog)
			ListCtrl->m_pfunLog(path);

		int nImageType = get_file_type(path);
		if (nImageType == CXIMAGE_FORMAT_UNKNOWN)
			continue;

//		CxImage image((*iter).pBuf, (*iter).lSize, 0);
		CxImage image(path, get_file_type(path));

		if(image.IsValid()==false)
			continue;

		// Calculate Rect to fit to canvas
		const float fImgRatio=(float)image.GetHeight()/image.GetWidth();
		if(fImgRatio > fRatio)
		{
			nDestWidth=THUMBNAIL_HEIGHT/fImgRatio;
			XDest=(THUMBNAIL_WIDTH-nDestWidth)/2;
			YDest=0;
			nDestHeight=THUMBNAIL_HEIGHT;
		}
		else
		{
			XDest=0;
			nDestWidth=THUMBNAIL_WIDTH;
			nDestHeight=THUMBNAIL_WIDTH*fImgRatio;
			YDest=(THUMBNAIL_HEIGHT-nDestHeight)/2;
		}

		CClientDC cdc(ListCtrl);
		HDC hDC=::CreateCompatibleDC(cdc.m_hDC);
		HBITMAP bm = CreateCompatibleBitmap(cdc.m_hDC, THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT);
		HBITMAP pOldBitmapImage = (HBITMAP)SelectObject(hDC,bm);
		// Draw Background
		::FillRect(hDC, &rcBorder, hBrushBk);

		// Draw Image
		image.Stretch(hDC, XDest, YDest, nDestWidth, nDestHeight);

		// Draw Border
		::FrameRect(hDC, &rcBorder, hBrushBorder);

		SelectObject(hDC, pOldBitmapImage);

		// Attach to Bitmap and Replace image in CImageList
		CBitmap bitmap;
		bitmap.Attach(bm);
		pImgList->Replace(nIndex, &bitmap, NULL);

		// Redraw only a current item for removing flickering and fast speed.
		ListCtrl->RedrawItems(nIndex, nIndex);

		// Release used DC and Object
		DeleteDC(hDC);
		DeleteObject(bm);
	}
	DeleteObject(hBrushBorder);
	DeleteObject(hBrushBk);

	ListCtrl->Invalidate();
	ListCtrl->m_bRunning=false;
	ListCtrl->m_bTerminate=false;
//	LogInfo("Done.");
	if(ListCtrl->m_pfunLog)
		ListCtrl->m_pfunLog("Done!");

	_endthreadex( 0 );


	return 0;
}

//##ModelId=3FFE4BD200DC
void CPreviewList::LoadThumbImages()
{
//	CThumbViewerDoc* pDoc = GetDocument();
//	ASSERT_VALID(pDoc);

//	CListCtrl& ListCtrl=GetListCtrl();

	TerminateThread();
//	LoadThumbNail((LPVOID)this);
	m_hThread = (HANDLE)_beginthreadex(NULL,0, LoadThumbNail,(LPVOID)this,0/* CREATE_SUSPENDED*/,&m_dwThreadID); 

	m_bRunning=true;
}

//##ModelId=3FFE4BD200EB
void CPreviewList::OpenNewDir()
{
	DeleteAllItems();

	// reset our image list
	for(int i=0; i<m_ImageListThumb.GetImageCount(); i++)
		m_ImageListThumb.Remove(i);
	m_ImageListThumb.SetImageCount(0);
}
//##ModelId=3FFE4BD200FA
void CPreviewList::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
//	CThumbViewerDoc* pDoc = GetDocument();
//	ASSERT_VALID(pDoc);

//	pDoc->SelectItem(pNMListView->iItem);
	*pResult = 0;
}

//##ModelId=3FFE4BD20129
BOOL CPreviewList::TerminateThread()
{
	if(m_bRunning==false)
		return TRUE;
	m_bTerminate=true;
	HANDLE hTemp = CreateEvent(NULL, FALSE, FALSE, NULL);

	::WaitForSingleObject(m_hThread, 2000);

	CloseHandle(hTemp);
	::CloseHandle(m_hThread);
	return TRUE;
}

//##ModelId=3FFE4BD200FE
void CPreviewList::OnPreviewListAddFile()
{
	if(AfxMessageBox("是否添加到项目组中", MB_ICONQUESTION | MB_YESNO) == IDNO)
		return;
	
	HWND hWnd = ((CFrameWnd*)AfxGetMainWnd())->m_hWnd;//GetActiveView()->m_hWnd;
	
	int nIdx;
	POSITION pos = GetFirstSelectedItemPosition();
	
	while(pos!=NULL)   //CCP ADD LOOP
	{
		if (pos == NULL)
			TRACE0("No items were selected!\n");
		else
		{
			nIdx = GetNextSelectedItem(pos);
		}
		vector<CString>::iterator it;
		int n;
		for(it = m_vFileName.begin(),n=0; it != m_vFileName.end(),n < nIdx; ++it,n++);
		
		::SendMessage(AfxGetMainWnd()->m_hWnd, m_dwRevMessage,WPARAM(&(*it)),LPARAM(&m_strCurrentDirectory));
	}
}

//##ModelId=3FFE4BD2010A
void CPreviewList::OnOpenDir()//CCP ADD
{
//    CFolderFind dlg;
//	if(dlg.DoModal() == IDCANCEL)
//		return;

	CString strInit;
	strInit = AfxGetApp()->GetProfileString("Dir", "Browser", "C:\\");

//	static char BASED_CODE szFilter[] = "supported files|*.jpeg;*.jpg;*.bmp|Jpeg Files (*.jpeg,*.jpg)|*.jpeg;*.jpg|Bmp Files (*.bmp)|*.bmp|All Files (*.*)|*.*||";
//	CFileDialog dlg(TRUE, "*.*", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_FILEMUSTEXIST, szFilter, NULL);

	CGUIShellManager dirmgr;
	if(!dirmgr.BrowseForFolder (strInit,GetParent(),strInit,"浏览目录"))
		return;

	AfxGetApp()->WriteProfileString("Dir", "Browser", strInit);
	SelectDirectory(strInit);
	//SelectDirectory(dlg.m_treeDir.GetSelectedDirectory());
}
///////////////////////////////////////////////////////////////

//##ModelId=3FFE4BD2010C
void CPreviewList::OnRclick(NMHDR* pNMHDR, LRESULT* pResult)
{
		CMenu menu;
		if(GetFirstSelectedItemPosition())
			VERIFY(menu.LoadMenu(IDR_POPUP_PREVIEW_LIST_EDIT_INFO));
		else
			VERIFY(menu.LoadMenu(CG_IDR_POPUP_PREVIEW_LIST));
		
		LPNMITEMACTIVATE lpnmmouse= (LPNMITEMACTIVATE) pNMHDR;
		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		CWnd* pWndPopupOwner = this;
		CPoint point = lpnmmouse->ptAction;
		ClientToScreen(&point);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
			pWndPopupOwner);
}

void CPreviewList::OnImageFileInfo()
{
	AfxMessageBox("图像文件属性");
}

// 加入图像指针
void CPreviewList::AddImagePtr(CString strImageName, BYTE* pBuf, long lSize)
{
	IMAGELIST imageList;
	imageList.strImageName = strImageName;
	imageList.pBuf = pBuf;
	imageList.lSize = lSize;
	m_vImageList.push_back(imageList);
}

void CPreviewList::DelImagePtrAll()
{
	for (int i=0; i<m_vImageList.size(); i++)
	{
		delete [] m_vImageList[i].pBuf;
	}
	m_vImageList.clear();
}

// 岩性识别
/* ==================================================================
 * 函数名称：  CPreviewList::OnRockRecog()
 * 作者：      [Leng]
 * 创建于:     [2009-5-31 23:18:19]
 * 参数列表： 
注释 :	
	识别掌子面岩体岩性			
==================================================================*/
void CPreviewList::OnRecogTexture() 
{
	// TODO: Add your command handler code here
	PRECOGTEXTURE pRecog = NULL;
	pRecog = new RECOGTEXTURE [m_vFileName.size()];

	vector<CString>::iterator iter;
	if(m_pfunLog)
		m_pfunLog("Reading...");

	char path[MAX_PATH];
	int nIndex=0;
	BPnetwork bpNet;
	bpNet.SetPowers();

	for(iter=m_vFileName.begin(); iter!=m_vFileName.end() && m_bTerminate!=true; iter++, nIndex++)
	{
		// Load Image File
		sprintf(path, "%s\\%s", m_strCurrentDirectory, *iter);
		if(m_pfunLog)
			m_pfunLog(path);

		int nImageType = get_file_type(path);
		if (nImageType == CXIMAGE_FORMAT_UNKNOWN)
			continue;

		CImage image(path, get_file_type(path));
		if(image.IsValid()==false)
			continue;

		// 找到当前图像的各种纹理特征参数
		CVeins vein(image);
		double characters[12];

		vein.CreatAccrMatrix();
		characters[0] = sqrt(vein.Vein2Mom());
 		characters[1] = sqrt(vein.Entropy());
 		characters[2] = sqrt(vein.Contrast());
 		characters[3] = sqrt(vein.Homogen(3.0));
		characters[4] = sqrt(vein.RowFrequency(2));
		characters[5] = sqrt(vein.ColFrequency(2));
		characters[6] = sqrt(vein.LDFrequency(2));
		characters[7] = sqrt(vein.RDFrequency(2));
		characters[8] = sqrt(vein.RowFrequency(4));
		characters[9] = sqrt(vein.ColFrequency(4));
		characters[10] = sqrt(vein.LDFrequency(4));
		characters[11] = sqrt(vein.RDFrequency(4));

		int nIdx = bpNet.Recognize(characters,12);

		pRecog[nIndex].strDK = get_file_name(path);
		switch (nIdx)
		{
		case 0:
			pRecog[nIndex].strRockLith = "强风化花岗岩";
			break;
		case 1:
			pRecog[nIndex].strRockLith = "中风化花岗岩";
			break;
		case 2:
			pRecog[nIndex].strRockLith = "微风化花岗岩";
			break;
		case 3:
			pRecog[nIndex].strRockLith = "未风化花岗岩";
			break;
		}
	}

//		HWND hWnd = ::GetActiveWindow();
//		::SendMessage(hWnd, WM_RECOG_TEXTURE, (unsigned int)pRecog, m_vFileName.size());

	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;

	// Get the active MDI child window.
	CMDIChildWnd *pChild = (CMDIChildWnd *) pFrame->GetActiveFrame();

	// Get the active view attached to the active MDI child
	// window.
	CView *pView = (CView *) pChild->GetActiveView();

	pView->SendMessage(WM_RECOG_TEXTURE, (unsigned int)pRecog, m_vFileName.size());

	delete [] pRecog;
}

void CPreviewList::OnAddToFace() 
{
	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild = (CMDIChildWnd *) pFrame->GetActiveFrame();
	CView *pView = (CView *) pChild->GetActiveView();

	int nIdx;
	POSITION pos = GetFirstSelectedItemPosition();
	while(pos!=NULL)   //CCP ADD LOOP
	{
		if (pos == NULL)
			TRACE0("No items were selected!\n");
		else
		{
			nIdx = GetNextSelectedItem(pos);
		}

		// 查找第n个文件路径
		vector<CString>::iterator it;
		int n;
		for(it = m_vFileName.begin(),n=0; it != m_vFileName.end(),n < nIdx; ++it,n++);
		pView->SendMessage(WM_ADD_TO_FACE, WPARAM(&(*it)),LPARAM(&m_strCurrentDirectory));
	}

//	pView->SendMessage(WM_RECOG_TEXTURE, (unsigned int)pRecog, m_vFileName.size());
}

void CPreviewList::OnUpdateAddToFace(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

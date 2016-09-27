#include "StdAfx.h"
#include "guiserverexplorer.h"
#include "AdoConn.h"
#include "ximage.h"
#include "resource.h"

#define ID_REFRESH 0x355
#define ID_CONNECT 0x356
#define ID_CONSERVER 0x367


CGuiServerExplorer::CGuiServerExplorer(void)
{
}

CGuiServerExplorer::~CGuiServerExplorer(void)
{
}
BEGIN_MESSAGE_MAP(CGuiServerExplorer, CGuiControlBar)
	ON_WM_CREATE()
	ON_COMMAND(ID_REFRESH,  OnExplorer)
	ON_COMMAND(ID_CONNECT,  OnConnect)
	ON_COMMAND(ID_CONSERVER,  OnServer)
END_MESSAGE_MAP()

int CGuiServerExplorer::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CGuiControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	const DWORD dwStyle =WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS ;
	if (!m_ctServer.Create(WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,124))
		return -1;

	CreatServerExplorer();

	const DWORD dwStyleSer =WS_CHILD | WS_VISIBLE | LVS_SHOWSELALWAYS|LVS_ALIGNTOP|LVS_ICON|LVS_SINGLESEL|LVS_AUTOARRANGE;
	if (!m_TreeServerExplorer.Create(dwStyleSer,CRect(0,0,0,0), &m_ctServer, 1))
		return -1;
	m_TreeServerExplorer.Initialize();
	m_ctServer.AddComponen(&m_TreeServerExplorer);

	return 0;
}

void CGuiServerExplorer::OnConnect()
{
	/*
	// 暂时没有考虑多线程(这里连接数据库最好用多线程)
	bstr_t vSQL;
	_RecordsetPtr	pRecordSet;
	_variant_t varBLOB;
	_variant_t varImageName;
	
	long nSize;	
	BYTE *pBuffer;
	BYTE* pBuf = NULL;

// 	long lTime, lTime1, lTime2, lTime3;
// 	SYSTEMTIME time;
// 	::GetSystemTime(&time);
// 	lTime = time.wMinute*1000+time.wMilliseconds;
// 	TRACE("Before: Time=%d毫秒\n", lTime);

	ADOConn adoConn;
	adoConn.OnInitADOConn();
	vSQL = "SELECT * FROM Std_Rock where ImageId=1";
	pRecordSet = adoConn.GetRecordSet(vSQL);

// 	::GetSystemTime(&time);
// 	lTime1 = time.wMinute*1000+time.wMilliseconds;
// 	TRACE("ΔDatabase: Time=%d毫秒\n", lTime1-lTime);

	while (!pRecordSet->GetadoEOF())
	{
		nSize = pRecordSet->GetFields()->GetItem("ImageFile")->ActualSize;	
		if(nSize > 0)
		{
			varBLOB = pRecordSet->GetFields()->GetItem("ImageFile")->GetChunk(nSize);
			if(varBLOB.vt == (VT_ARRAY | VT_UI1))
			{
				// 申请必要的存储空间
				if(pBuffer = new BYTE [nSize])		
				{	
					
					SafeArrayAccessData(varBLOB.parray,(void**)&pBuf);
					// 复制数据到缓冲区 pBuffer
					memcpy(pBuffer,pBuf,nSize);				
					SafeArrayUnaccessData(varBLOB.parray);	
					
					//	delete [] pBuf;
				}
			}		
		}

		varImageName = pRecordSet->GetCollect("ImageName");
		CString strImageName =(char *)_bstr_t(varImageName);	

		m_TreeServerExplorer.AddImagePtr(strImageName,pBuffer, nSize);	
		// 		delete [] pBuffer;

		pRecordSet->MoveNext();
	}

// 	::GetSystemTime(&time);
// 	lTime2 = time.wMinute*1000+time.wMilliseconds;
// 	TRACE("ΔExeSQL: Time=%d毫秒\n", lTime2-lTime1);

	m_TreeServerExplorer.LoadThumbImages();

// 	::GetSystemTime(&time);
// 	lTime3 = time.wMinute*1000+time.wMilliseconds;
// 	TRACE("ΔLoadThumbImagesL: Time=%d毫秒\n", lTime3-lTime2);

	m_TreeServerExplorer.DelImagePtrAll();
	*/	
}

void CGuiServerExplorer::OnServer()
{
	AfxMessageBox("Server");
}
void CGuiServerExplorer::OnExplorer()
{
	AfxMessageBox("Refresh");
}
	
int CGuiServerExplorer::CreatServerExplorer()
{
	m_ctServer.AddComponen(&m_miServer);
	m_miServer.AlingButtons(CGuiMiniTool::ALIGN_LEFT);
	m_miServer.SetImageList(IDB_DBPROJECTS, 16,27, RGB (255, 0, 0));
	m_miServer.AddButton(24,ID_REFRESH,NULL,_T(""),"Refresh");
	m_miServer.AddButton(25,ID_CONNECT,NULL,_T(""),"Connect To Database");
	m_miServer.AddButton(26,ID_CONSERVER,NULL,_T(""),"Connect To Server");
	m_miServer.AutoSize(FALSE);
	m_miServer.SetColor(GuiDrawLayer::GetRGBColorXP());
	return 1;
}

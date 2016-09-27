#pragma once
#include "guicontrolbar.h"
#include "GuiContainer.h"
#include "previewlist.h"

class CGuiServerExplorer :	public CGuiControlBar
{
public:

	CPreviewList  m_TreeServerExplorer;
	CGuiContainer m_ctServer;
	CGuiMiniTool  m_miServer;
public:
	void OnConnect();
	void OnServer();
	void OnExplorer();
	CGuiServerExplorer(void);
	virtual ~CGuiServerExplorer(void);
	int CreatServerExplorer();
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

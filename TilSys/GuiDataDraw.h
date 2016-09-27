// GuiDataDraw.h: interface for the CGuiDataDraw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUIDATADRAW_H__2AC4B751_7664_4D59_A107_0A693C2AF3FF__INCLUDED_)
#define AFX_GUIDATADRAW_H__2AC4B751_7664_4D59_A107_0A693C2AF3FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GuiControlBar.h"
#include "CanvasHistogram.h"
#include "CanvasSection.h"
#include "GuiTabWnd.h"

class CGuiDataDraw : public CGuiControlBar  
{
	DECLARE_DYNAMIC(CGuiDataDraw)
protected:
	CGuiTabWnd	m_TabSolExplorer;
	CCanvasHistogram  m_Histogram;		// 灰度直方图
	CCanvasSection		m_Section;		// 地质剖面图
//	CCanvasHistogram	m_Others;
public:
	CGuiDataDraw();
	virtual ~CGuiDataDraw();
	
	void InvalidateHistogram();
	void InvalidateSection();

	void SetCurTab(int nIdx) { m_TabSolExplorer.SetCurtab(nIdx); };

	CCanvasHistogram& GetHistogram() { return m_Histogram; };
	CCanvasSection&		GetSection() { return m_Section; };
//	CCanvasHistogram& GetOthers() { return m_Others;	};

public:
	//{{AFX_MSG(CDataDraw)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_GUIDATADRAW_H__2AC4B751_7664_4D59_A107_0A693C2AF3FF__INCLUDED_)

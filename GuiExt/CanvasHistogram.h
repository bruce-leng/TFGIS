#if !defined(AFX_CANVASHISTOGRAM_H__F60A5A52_8D6E_49DB_8820_B81DDF2365A6__INCLUDED_)
#define AFX_CANVASHISTOGRAM_H__F60A5A52_8D6E_49DB_8820_B81DDF2365A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CanvasHistogram.h : header file
//

#include "GUIExtStd.h"
/////////////////////////////////////////////////////////////////////////////
// CCanvasHistogram window
class CCanvasInfo;

class TILEXTCLASS CCanvasHistogram : public CStatic
{
// Construction
public:
	CCanvasHistogram();

// Attributes
public:
	// 0:红色分量,1:绿色分量,2:蓝色分量,3:灰度直方图
	BOOL m_bShowFlags[4];
	BOOL m_bLogState;
	long m_lHisto[4][256];

	// 直方图的最大、最小值
	double m_dbMax, m_dbMin;
	long m_lMax;

// Operations
public:
	void SetData(long* red, long* green = 0, long* blue = 0, long* gray = 0);
	void Redraw();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCanvasHistogram)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCanvasHistogram();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCanvasHistogram)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnHistogramRed();
	afx_msg void OnHistogramGreen();
	afx_msg void OnHistogramBlue();
	afx_msg void OnHistogramGray();
	afx_msg void OnHistogramProperty();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	void get_max_min();
	void draw_idx(CDC* pDC, int nIdx, COLORREF clr);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CANVASHISTOGRAM_H__F60A5A52_8D6E_49DB_8820_B81DDF2365A6__INCLUDED_)

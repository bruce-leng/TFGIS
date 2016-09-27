#if !defined(AFX_DLGIMAGEGEOMETRYCORRECT_H__C8DA022B_E626_43D4_AF8E_99DD504CAF63__INCLUDED_)
#define AFX_DLGIMAGEGEOMETRYCORRECT_H__C8DA022B_E626_43D4_AF8E_99DD504CAF63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgImageGeometryCorrect.h : header file
//

// 用于图像几何校正的控制点属性
typedef vector<POINT2DF> CTRLPOINTS;
/////////////////////////////////////////////////////////////////////////////
// DlgImageGeometryCorrect dialog

class DlgImageGeometryCorrect : public CDialog
{
// Construction
public:
	DlgImageGeometryCorrect(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgImageGeometryCorrect();
	CTilCanvas *m_pCanvas;
	CTRLPOINTS m_imgPoints;		// 图像上的控制点坐标
	CTRLPOINTS m_ctrlPoints;	// 显示图像控件中的控制点坐标

// Dialog Data
	//{{AFX_DATA(DlgImageGeometryCorrect)
	enum { IDD = IDD_IMAGE_GEOMETRY_CORRECT };
	CStatic	m_ctlImage;
	double	m_dbEdt1NextX;
	double	m_dbEdt2NextX;
	double	m_dbEdt3NextX;
	double	m_dbEdt4NextX;
	double	m_dbEdt5NextX;
	double	m_dbEdt6NextX;
	double	m_dbEdt7NextX;
	double	m_dbEdt8NextX;
	double	m_dbEdt9NextX;

	double	m_dbEdt1NextY;
	double	m_dbEdt2NextY;
	double	m_dbEdt3NextY;
	double	m_dbEdt4NextY;
	double	m_dbEdt5NextY;
	double	m_dbEdt6NextY;
	double	m_dbEdt7NextY;
	double	m_dbEdt8NextY;
	double	m_dbEdt9NextY;

	double	m_dbEdt1PrevX;
	double	m_dbEdt2PrevX;
	double	m_dbEdt3PrevX;
	double	m_dbEdt4PrevX;
	double	m_dbEdt5PrevX;
	double	m_dbEdt6PrevX;
	double	m_dbEdt7PrevX;
	double	m_dbEdt8PrevX;
	double	m_dbEdt9PrevX;

	double	m_dbEdt1PrevY;
	double	m_dbEdt2PrevY;
	double	m_dbEdt3PrevY;
	double	m_dbEdt4PrevY;
	double	m_dbEdt5PrevY;
	double	m_dbEdt6PrevY;
	double	m_dbEdt7PrevY;
	double	m_dbEdt8PrevY;
	double	m_dbEdt9PrevY;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgImageGeometryCorrect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// 自动取得目标控制点
	void ObjPtAutoExtract(CImage* pBinImg, CTRLPOINTS& ctrlPoints);
	// 手动取得目标控制点
//	int ObjPtManualExtract(CImage* pBinImg, POINT2DF imgPt, POINT2DF ctlPt);
	// 手动删除错误目标控制点	
	int ObjPtManualDel(CImage* pBinImg, POINT2DF ctlPt, POINT2DF imgPt);
	// 将控制点在图像中的坐标转换成显示图像控制中的坐标
	void TransferCoord(CTRLPOINTS& srcCtrlPts, CTRLPOINTS& dstCtrlPts);
	// 画出以控制点为中心的红十字形
	void DrawCrossLine(CDC* pDC, CTRLPOINTS& ctrlPoints);
	// 将目标控制点坐标显示在"校正前控制点坐标"组合框中
	void ShowCtrlPointsCoord();

	// Generated message map functions
	//{{AFX_MSG(DlgImageGeometryCorrect)
	afx_msg void OnCtrlPtExtract();
	afx_msg void OnCtrlPtDel();
	afx_msg void OnCtrlPtAdd();
	afx_msg void OnImageCorrect();
	afx_msg void OnOk();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	// 设十字线坐标
//	void set_cross_line(CTRLPOINTS& ctrlPoints);
	BOOL bAdd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGIMAGEGEOMETRYCORRECT_H__C8DA022B_E626_43D4_AF8E_99DD504CAF63__INCLUDED_)

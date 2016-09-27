#if !defined(AFX_DLGGRAPHTUNSIDE_H__E65A11BA_D54A_42EE_9C74_95CC58DDDA35__INCLUDED_)

/************************************
  REVISION LOG ENTRY
  维护人员: Leng
  维护时间 2009-3-26 15:31:31
  说明: 用于设置和生成边墙和拱顶的展开图
 ************************************/

#define AFX_DLGGRAPHTUNSIDE_H__E65A11BA_D54A_42EE_9C74_95CC58DDDA35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGraphTunSide.h : header file
//

//////////////////////////////////////////////////////////////////////////
class DlgGraphTunSide : public CDialog
{
// Construction
public:
	DlgGraphTunSide(CWnd* pParent = NULL);   // standard constructor
	~DlgGraphTunSide();

	float m_fRatio;		// 原始图像在图像框中的缩放率
	float m_fExpand;	// 边界扩展距离
	CTilCanvas* m_pCanvas;
	CImage* m_pImageExpand;	// 地质展开图

	// 将隧道断面边界扩展
	//void ExpandTunArc(float fDist);
	// 将隧道断面边界扩展
	void ExpandTunArc(const float& fDist);
	// 扩展线段
	void OffLine(const POINT2DF& ptA1, const POINT2DF& ptA2,
				const float& fDist, POINT2DF& ptB1, POINT2DF& ptB2);
	// 扩展圆弧
	void OffArc(const POINT2DF& ptA1, const POINT2DF& ptA2, const POINT2DF& ptCenter,
				const float& fDist,	POINT2DF& ptB1, POINT2DF& ptB2);

	// 平移隧道边界
	void MoveTunArc(int nX, int nY);
	// 竖向移动水平裁剪线
	void MoveHLine(int nY);
	// 求得水平裁剪线与断面的交点
	void GetInterPoints(POINT2DF& pt1, POINT2DF& pt2);
// Dialog Data
	//{{AFX_DATA(DlgGraphTunSide)
	enum { IDD = IDD_GRAPH_TUNSIDE };
	CScrollBar	m_ctlMoveV2;
	CScrollBar	m_ctlMoveV;
	CScrollBar	m_ctlMoveH;
	CStatic	m_stcTunSide;
	CSpinButtonCtrl	m_ctlShowWidth;
	float	m_fRealWidth;
	float	m_fShowWidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgGraphTunSide)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgGraphTunSide)
	virtual BOOL OnInitDialog();
	virtual void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnOk();
	afx_msg void OnGeoExpand();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGRAPHTUNSIDE_H__E65A11BA_D54A_42EE_9C74_95CC58DDDA35__INCLUDED_)

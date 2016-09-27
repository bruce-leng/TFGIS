#if !defined(AFX_DLGGRAPHTUNSIDE_H__E65A11BA_D54A_42EE_9C74_95CC58DDDA35__INCLUDED_)

/************************************
  REVISION LOG ENTRY
  ά����Ա: Leng
  ά��ʱ�� 2009-3-26 15:31:31
  ˵��: �������ú����ɱ�ǽ�͹�����չ��ͼ
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

	float m_fRatio;		// ԭʼͼ����ͼ����е�������
	float m_fExpand;	// �߽���չ����
	CTilCanvas* m_pCanvas;
	CImage* m_pImageExpand;	// ����չ��ͼ

	// ���������߽���չ
	//void ExpandTunArc(float fDist);
	// ���������߽���չ
	void ExpandTunArc(const float& fDist);
	// ��չ�߶�
	void OffLine(const POINT2DF& ptA1, const POINT2DF& ptA2,
				const float& fDist, POINT2DF& ptB1, POINT2DF& ptB2);
	// ��չԲ��
	void OffArc(const POINT2DF& ptA1, const POINT2DF& ptA2, const POINT2DF& ptCenter,
				const float& fDist,	POINT2DF& ptB1, POINT2DF& ptB2);

	// ƽ������߽�
	void MoveTunArc(int nX, int nY);
	// �����ƶ�ˮƽ�ü���
	void MoveHLine(int nY);
	// ���ˮƽ�ü��������Ľ���
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

#if !defined(AFX_DLGIMAGEGEOMETRYCORRECT_H__C8DA022B_E626_43D4_AF8E_99DD504CAF63__INCLUDED_)
#define AFX_DLGIMAGEGEOMETRYCORRECT_H__C8DA022B_E626_43D4_AF8E_99DD504CAF63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgImageGeometryCorrect.h : header file
//

// ����ͼ�񼸺�У���Ŀ��Ƶ�����
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
	CTRLPOINTS m_imgPoints;		// ͼ���ϵĿ��Ƶ�����
	CTRLPOINTS m_ctrlPoints;	// ��ʾͼ��ؼ��еĿ��Ƶ�����

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
	// �Զ�ȡ��Ŀ����Ƶ�
	void ObjPtAutoExtract(CImage* pBinImg, CTRLPOINTS& ctrlPoints);
	// �ֶ�ȡ��Ŀ����Ƶ�
//	int ObjPtManualExtract(CImage* pBinImg, POINT2DF imgPt, POINT2DF ctlPt);
	// �ֶ�ɾ������Ŀ����Ƶ�	
	int ObjPtManualDel(CImage* pBinImg, POINT2DF ctlPt, POINT2DF imgPt);
	// �����Ƶ���ͼ���е�����ת������ʾͼ������е�����
	void TransferCoord(CTRLPOINTS& srcCtrlPts, CTRLPOINTS& dstCtrlPts);
	// �����Կ��Ƶ�Ϊ���ĵĺ�ʮ����
	void DrawCrossLine(CDC* pDC, CTRLPOINTS& ctrlPoints);
	// ��Ŀ����Ƶ�������ʾ��"У��ǰ���Ƶ�����"��Ͽ���
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
	// ��ʮ��������
//	void set_cross_line(CTRLPOINTS& ctrlPoints);
	BOOL bAdd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGIMAGEGEOMETRYCORRECT_H__C8DA022B_E626_43D4_AF8E_99DD504CAF63__INCLUDED_)

// DlgImageConcat.h: interface for the DlgImageConcat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined DLGIMAGECONCAT_H
#define DLGIMAGECONCAT_H

typedef vector< vector< POINT > > VVECPOINT;
typedef struct INTER_POINT
{
	POINT pt;	// 交点坐标
	int nLineIdx;	// 点所在多段线的序号
	int nPointIdx;	// 交点在多段线中的位置序号
} INTER_POINT, *PINTER_POINT, **PPINTER_POINT;
typedef vector< INTER_POINT > VINTERPOINT;

#define N 6		// 图像拼接中图像的数量

class CWizard : public CPropertySheet
{
	DECLARE_DYNAMIC(CWizard)
		
		// Construction
public:
	CImage m_Images[N];		// 图像类对象
	POINT2DF m_ImagePts[N];	// 图像中的标记点坐标
	POINT3DF m_GroundPts[N];// 图像中标记点的大地坐标

	CWizard(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CWizard(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	
	// Operations
public:
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWizard)
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	virtual ~CWizard();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CWizard)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
class Step1 : public CPropertyPage
{
	DECLARE_DYNCREATE(Step1)
		
		// Construction
public:
	CImage* m_pImages;		// 图像类对象

	// 自动目标点提取
	int ObjPtAutoExtract(CImage* pBinImg, POINT2DF& point);
	// 画出以控制点为中心的红十字形
	void DrawCrossLine(CStatic* pWnd, POINT2DF& point);

	enum JOB_METHOD{TWO_STEP, THREE_STEP, CRD, CD, TWO_SIDE_PILOT, DOWN_PILOT_ADVANCE_STEP, DOWN_PILOT_ADVANCE_FULL_FACE};

	Step1();
	~Step1();
	
	// Dialog Data
	//{{AFX_DATA(Step1)
	enum { IDD = IDD_IMAGE_CONCACT_STEP1 };
	CString m_strMethods;
	CComboBox	m_cmbxMethods;
	CStatic m_stcImages[N];
	POINT2DF m_ImagePts[N];	// 图像中的标记点坐标
	POINT3DF m_GroundPts[N];// 图像中标记点的大地坐标
	CStatic m_stcJobMethod;
	//}}AFX_DATA
	POINT2DF m_CntrlPts[N];	// 图像坐标在图像显示控件中的坐标
	CRect m_Rects[N];		// 每幅图像显示区域在对话框中的外接矩形
	CImage m_JobMethod;		// 施工方法

	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Step1)
public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnDropdownCmbxMethods();
	afx_msg void OnSelchangeCmbxMethods();
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(Step1)
	afx_msg void OnLoad1();
	afx_msg void OnLoad2();
	afx_msg void OnLoad3();
	afx_msg void OnLoad4();
	afx_msg void OnLoad5();
	afx_msg void OnLoad6();
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRecognition();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//////////////////////////////////////////////////////////////////////////
class Step2 : public CPropertyPage
{
	DECLARE_DYNCREATE(Step2)
		
		// Construction
public:
	CImage* m_pImages;		// 图像类对象

	CRect m_rtGround[N];	// 每幅图像相对于大地坐标的外接矩形坐标区域
	CRect m_rtTotal;		// 几幅图像相对于大地坐标的外接矩形
	CRect m_rtRect[N];		// 以几幅图像总的外接矩形的左上角为原点，向下和向右为正，计算几幅图像新的外接矩形区域

	HBITMAP m_hTotalBitmap;		// 存储几幅图像的拼接图像
	HDC m_hTotalDC;				// 几幅图像的拼接图像在内存中的绘图句柄
	HBITMAP m_hOldTotalBitmap;	// 原来的图像
	float m_fRatio;				// 原始图像在图像显示控件中显示时的缩放率

	VVECPOINT m_vvPoints;	// 用于存储多段线相对于对话框左上角为原点时的坐标
	BOOL m_nDraw;			// 标记画多段线时所处的状态

	Step2();   // standard constructor
	~Step2();

	// Dialog Data
	//{{AFX_DATA(Step2)
	enum { IDD = IDD_IMAGE_CONCACT_STEP2 };
		// NOTE: the ClassWizard will add data members here
	CStatic m_stcImage;
	//}}AFX_DATA
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Step2)
public:
	virtual BOOL OnSetActive();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	// 得到图像相对于大地坐标的外接矩形坐标区域
	CRect get_image_rect(CImage* pImage, POINT2DF ptImage, POINT3DF ptGround);
	// 计算原始图像与多段线的交集图像，将图像作为最终的图像的一部分
//	CRgn* get_inter_section(const CRect& rtImage, VVECPOINT vLines);

	// Generated message map functions
	//{{AFX_MSG(Step2)
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnImageCut();
	afx_msg void OnImageSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//////////////////////////////////////////////////////////////////////////
class Step3 : public CPropertyPage
{
	DECLARE_DYNCREATE(Step3)
		
		// Construction
public:
	Step3();   // standard constructor
	~Step3();
	
	// Dialog Data
	//{{AFX_DATA(Step3)
	enum { IDD = IDD_IMAGE_CONCACT_STEP3 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Step3)
public:
	virtual BOOL OnSetActive();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(Step3)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined DLGIMAGECONCAT_H

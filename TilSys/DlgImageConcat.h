// DlgImageConcat.h: interface for the DlgImageConcat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined DLGIMAGECONCAT_H
#define DLGIMAGECONCAT_H

typedef vector< vector< POINT > > VVECPOINT;
typedef struct INTER_POINT
{
	POINT pt;	// ��������
	int nLineIdx;	// �����ڶ���ߵ����
	int nPointIdx;	// �����ڶ�����е�λ�����
} INTER_POINT, *PINTER_POINT, **PPINTER_POINT;
typedef vector< INTER_POINT > VINTERPOINT;

#define N 6		// ͼ��ƴ����ͼ�������

class CWizard : public CPropertySheet
{
	DECLARE_DYNAMIC(CWizard)
		
		// Construction
public:
	CImage m_Images[N];		// ͼ�������
	POINT2DF m_ImagePts[N];	// ͼ���еı�ǵ�����
	POINT3DF m_GroundPts[N];// ͼ���б�ǵ�Ĵ������

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
	CImage* m_pImages;		// ͼ�������

	// �Զ�Ŀ�����ȡ
	int ObjPtAutoExtract(CImage* pBinImg, POINT2DF& point);
	// �����Կ��Ƶ�Ϊ���ĵĺ�ʮ����
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
	POINT2DF m_ImagePts[N];	// ͼ���еı�ǵ�����
	POINT3DF m_GroundPts[N];// ͼ���б�ǵ�Ĵ������
	CStatic m_stcJobMethod;
	//}}AFX_DATA
	POINT2DF m_CntrlPts[N];	// ͼ��������ͼ����ʾ�ؼ��е�����
	CRect m_Rects[N];		// ÿ��ͼ����ʾ�����ڶԻ����е���Ӿ���
	CImage m_JobMethod;		// ʩ������

	
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
	CImage* m_pImages;		// ͼ�������

	CRect m_rtGround[N];	// ÿ��ͼ������ڴ���������Ӿ�����������
	CRect m_rtTotal;		// ����ͼ������ڴ���������Ӿ���
	CRect m_rtRect[N];		// �Լ���ͼ���ܵ���Ӿ��ε����Ͻ�Ϊԭ�㣬���º�����Ϊ�������㼸��ͼ���µ���Ӿ�������

	HBITMAP m_hTotalBitmap;		// �洢����ͼ���ƴ��ͼ��
	HDC m_hTotalDC;				// ����ͼ���ƴ��ͼ�����ڴ��еĻ�ͼ���
	HBITMAP m_hOldTotalBitmap;	// ԭ����ͼ��
	float m_fRatio;				// ԭʼͼ����ͼ����ʾ�ؼ�����ʾʱ��������

	VVECPOINT m_vvPoints;	// ���ڴ洢���������ڶԻ������Ͻ�Ϊԭ��ʱ������
	BOOL m_nDraw;			// ��ǻ������ʱ������״̬

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
	// �õ�ͼ������ڴ���������Ӿ�����������
	CRect get_image_rect(CImage* pImage, POINT2DF ptImage, POINT3DF ptGround);
	// ����ԭʼͼ�������ߵĽ���ͼ�񣬽�ͼ����Ϊ���յ�ͼ���һ����
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

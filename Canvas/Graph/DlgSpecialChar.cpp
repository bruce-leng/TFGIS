// SpecialDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "CSpecialChar.h"
#include "DlgSpecialChar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSpecialChar dialog


CDlgSpecialChar::CDlgSpecialChar(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSpecialChar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSpecialChar)
	//}}AFX_DATA_INIT
	SelectX=0;
	SelectY=0;
	num=1;
	m_pEdit = (CEdit*)pParent;
}


void CDlgSpecialChar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSpecialChar)
	DDX_Control(pDX, IDC_TAB1, m_tab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSpecialChar, CDialog)
	//{{AFX_MSG_MAP(CDlgSpecialChar)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	ON_COMMAND(ID_INSERT, OnInsert)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSpecialChar message handlers

BOOL CDlgSpecialChar::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	InitList();  
	m_tab.InsertItem( 0, _T("标点符号") );
	m_tab.InsertItem( 1, _T("特殊符号") );
	m_tab.InsertItem( 2, _T("数学符号") );
	m_tab.InsertItem( 3, _T("单位符号") );
	m_tab.InsertItem( 4, _T("数字符号") );
	m_tab.InsertItem( 5, _T("拼音") );
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSpecialChar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	SetTimer(0,10,NULL);  //设置定时器，注释看OnTimer()

	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgSpecialChar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(point.x>20&&point.x<260&&point.y<220&&point.y>80){
		int x=(point.x-20)/20;   /*获得字符的位置*/
		int y=(point.y-80)/20;
		CDC * hdc;
		hdc=GetDC();
		CFont myFont;
		myFont.CreatePointFont(-90,   "宋体");  //设置字体
		hdc->SelectObject(myFont);
		
		if ( FindList(y,x) ){
			/*以下为在上次选择的字符上画一个白框，然后写上黑色的字符
			  在现在选择的字符上画 一个蓝框，并写上白色的字*/
			hdc->FillSolidRect(22+20*SelectX,82+20*SelectY,17,17,RGB(255,255,255));//画白框
			RECT re ;
			re.top =85+20*SelectY;     //下移
			re.left =25+20*SelectX;  //左移
			re.bottom =re.top+15;
			re.right =re.left+15;
			if(FindList(SelectY,SelectX)){
				hdc->DrawText(m_strSelText,&re, DT_CENTER);	 //写原字符				
				hdc->FillSolidRect(22+20*x,82+20*y,17,17,RGB(0,0,255)); //画蓝框
			}
			SelectX=x;  /*记录当前选择*/
			SelectY=y;
			re.top =85+20*SelectY;     //下移
			re.left =25+20*SelectX;  //左移
			re.bottom =re.top+13;
			re.right =re.left+13;			
			if(FindList(SelectY,SelectX)){
				hdc->SetTextColor(RGB(255,255,255));  //设置白色
				hdc->DrawText(m_strSelText,&re, DT_CENTER);	 //写选择的符
				m_select=m_strSelText;      //记录当前选择
				DrawSelect();               //在右边的显示栏，显示选择的字符
			}
		}
	}
	CDialog::OnLButtonDown(nFlags, point);
}

/*OnMouseMove()的语句与OnLButtonDown()基本一样，注释请参看OnLButtonDown()*/
void CDlgSpecialChar::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CDC * hdc;
	hdc=GetDC();
	CFont myFont;
	myFont.CreatePointFont(-90,   "宋体");
	hdc->SelectObject(myFont);
	if(point.x>22&&point.x<258&&point.y<218&&point.y>78){
		int x=(point.x-20)/20;
		int y=(point.y-80)/20;		
		if (FindList(y,x)){
			//FindList(SelectY,SelectX);
			m_select=m_strSelText;
			DrawSelect();
		}
		else{
			if(FindList(SelectY,SelectX)){
				if(m_select!=m_strSelText){
					m_select=m_strSelText;
					DrawSelect();				
				}
			}
		}
		return;
	}
	else{
		if(FindList(SelectY,SelectX)){
			if(m_select!=m_strSelText){
				m_select=m_strSelText;
				DrawSelect();				
			}
		}
		return;
	}
	CDialog::OnMouseMove(nFlags, point);
}

/*InitData()函数的功能是画所有的小格子，同时将当前选择的特殊字符类型全部显示出来*/
void CDlgSpecialChar::InitData()
{
CDC * hdc;
	hdc=GetDC();
    hdc ->Draw3dRect (20,80,243,143,RGB(128,128,128),RGB(255,255,255));
	hdc->Draw3dRect ( 21,81,241,141,RGB(128,128,128),RGB(255,255,255));
    hdc->FillSolidRect(21,81,240,140,RGB(255,255,255));

	hdc ->Draw3dRect (290,120,82,82,RGB(128,128,128),RGB(255,255,255));
	hdc->Draw3dRect ( 291,121,81,81,RGB(128,128,128),RGB(255,255,255));
    hdc->FillSolidRect(291,121,80,80,RGB(255,255,255));
	/*以上代码为画出白色背景和Edit框效果*/

	CPoint point1,point2;
	for(int i=0;i<12;i++){
		point1.x=20+20*(i+1);
		point1.y=81;
		point2.x=20+20*(i+1);
		point2.y=140+80;
		hdc->MoveTo(point1);
		hdc->LineTo(point2);
	}
	for(i=0;i<7;i++){
		point1.x=20;
		point1.y=80+20*(i+1);
		point2.x=240+20;
		point2.y=80+20*(i+1);
		hdc->MoveTo(point1);
		hdc->LineTo(point2);
	}
	/*以上代码画出12*7效果的网格*/

    CFont myFont;
	myFont.CreatePointFont(-90,   "宋体");
	hdc->SelectObject(myFont);
	RECT re ;
	for(i=0;i<7;i++){
		for(int j=0;j<12;j++){
			re.top =5+20*i+80;     //下移
			re.left =5+20*j+20;  //左移
			re.bottom =re.top+15;
			re.right =re.left+15;
            if(FindList(i,j))
				hdc->DrawText(m_strSelText,&re, DT_CENTER);	
		}
	}
	/*以上代码为显示选择字符类型的所有特殊字符*/
   
	/*一下代码是对默认选择的操作*/
    if(!FindList(SelectY,SelectX))
      SelectY=SelectX=0;
	if(FindList(SelectY,SelectX)){
		m_select=m_strSelText;
        hdc->FillSolidRect(22+20*SelectX,82+20*SelectY,17,17,RGB(0,0,255));
		
		RECT re ;
		re.top =85+20*SelectY;     //下移
		re.left =25+20*SelectX;  //左移
		re.bottom =re.top+15;
		re.right =re.left+15;
		hdc->SetTextColor(RGB(255,255,255));
		hdc->DrawText(m_strSelText,&re, DT_CENTER);	
		DrawSelect();
	}
}

/*FindList(int i, int j)函数
根据传入的i和j的值，找到相应的字符，若找到返回true,并将字符赋值到全局变量m_strSelText,否则返回false*/
bool CDlgSpecialChar::FindList(int i, int j)
{
POSITION pos;
	CDC * hdc;
	hdc=GetDC();
	switch(num)
	{
	case 1:
		pos= m_strList1.FindIndex(12*i+j) ;
		if ( pos != NULL ){
			m_strSelText = m_strList1.GetAt(pos) ;
			
		    return true;
		}
		break;
	case 2:
		pos = m_strList2.FindIndex(12*i+j) ;
		if ( pos != NULL ){
			m_strSelText = m_strList2.GetAt(pos) ;
            return true;
		}
		break;
	case 3:
		pos = m_strList3.FindIndex(12*i+j) ;
		if ( pos != NULL ){
			m_strSelText = m_strList3.GetAt(pos) ;
		    return true;
		}
		break;
	case 4:
		pos = m_strList4.FindIndex(12*i+j) ;
		if ( pos != NULL ){
			m_strSelText = m_strList4.GetAt(pos) ;
            return true;
		}
		break;
	case 5:
		pos = m_strList5.FindIndex(12*i+j) ;
		if ( pos != NULL ){
			m_strSelText = m_strList5.GetAt(pos) ;
            return true;
		}
		break;
	case 6:
		pos = m_strList6.FindIndex(12*i+j) ;
		if ( pos != NULL ){
			m_strSelText = m_strList6.GetAt(pos) ;
            return true;
		}
		break;
	}
	return false;
}

/*InitList()函数
初始化m_strList1~m_strList6,大家可根据需要插入自己需要的字符*/
void CDlgSpecialChar::InitList()
{
unsigned char ss[3];
    CString  TempStr;
	int i=162;
	for(int j = 161; j <=161+93 ; j++) //254//数字序号
	{ 
		ss[0] = i;
		ss[1] = j;
		ss[2] = 0;
		TempStr.Format("%s",ss) ;
		if( TempStr.IsEmpty() || TempStr == " " || TempStr == "  "  ) continue;
		if( i == 162 && j >= 171 && j <= 176) continue;
		if( i == 162 && j >= 227 && j <= 228) continue;
		if( i == 162 && j >= 239 && j <= 240) continue;
		if( i == 162 && j >= 253 && j <= 254) continue;
		if( i == 164 && j >= 244 && j <= 254) continue;
		if( i == 165 && j >= 247 && j <= 254) continue;
		if( i == 166 && j >= 185 && j <= 254) continue;
		if( i == 167 && j >= 194 && j <= 254) continue;
		if( i == 168 && j >= 193 && j <= 196) continue;
		if( i == 168 && j >= 234 && j <= 254) continue;
		if( i == 169 && j >= 161 && j <= 163) continue;
		if( i == 169 && j >= 240 && j <= 254) continue;
		if( i > 169 && i < 176 ) continue;
		if( i >= 248 ) continue;
		
		m_strList5.AddTail(TempStr)  ;
	}
	
	i = 161;  //﹏

	for(j = 162; j <=161+30 ; j++) //标点符号
	{ 
		ss[0] = i;
		ss[1] = j;
		ss[2] = 0;
		TempStr.Format("%s",ss) ;
		if( TempStr.IsEmpty() || TempStr == " " || TempStr == "  "  ) continue;
		if( i == 162 && j >= 171 && j <= 176) continue;
		if( i == 162 && j >= 227 && j <= 228) continue;
		if( i == 162 && j >= 239 && j <= 240) continue;
		if( i == 162 && j >= 253 && j <= 254) continue;
		if( i == 164 && j >= 244 && j <= 254) continue;
		if( i == 165 && j >= 247 && j <= 254) continue;
		if( i == 166 && j >= 185 && j <= 254) continue;
		if( i == 167 && j >= 194 && j <= 254) continue;
		if( i == 168 && j >= 193 && j <= 196) continue;
		if( i == 168 && j >= 234 && j <= 254) continue;
		if( i == 169 && j >= 161 && j <= 163) continue;
		if( i == 169 && j >= 240 && j <= 254) continue;
		if( i > 169 && i < 176 ) continue;
		if( i >= 248 ) continue;
		
		m_strList1.AddTail(TempStr)  ;
	}
	TempStr="︵";
    m_strList1.AddTail(TempStr)  ;
	TempStr="︶";
    m_strList1.AddTail(TempStr)  ;
	TempStr="︷";
    m_strList1.AddTail(TempStr)  ;
	TempStr="︸";
    m_strList1.AddTail(TempStr)  ;
	TempStr="′";
    m_strList1.AddTail(TempStr)  ;
	TempStr="‵";
    m_strList1.AddTail(TempStr)  ;
	TempStr="﹏";
    m_strList1.AddTail(TempStr)  ;
	

	for(j = 161+31; j <=161+31+32 ; j++) //数学符号
	{ 
		ss[0] = i;
		ss[1] = j;
		ss[2] = 0;
		TempStr.Format("%s",ss) ;
		if( TempStr.IsEmpty() || TempStr == " " || TempStr == "  "  ) continue;
		if( i == 162 && j >= 171 && j <= 176) continue;
		if( i == 162 && j >= 227 && j <= 228) continue;
		if( i == 162 && j >= 239 && j <= 240) continue;
		if( i == 162 && j >= 253 && j <= 254) continue;
		if( i == 164 && j >= 244 && j <= 254) continue;
		if( i == 165 && j >= 247 && j <= 254) continue;
		if( i == 166 && j >= 185 && j <= 254) continue;
		if( i == 167 && j >= 194 && j <= 254) continue;
		if( i == 168 && j >= 193 && j <= 196) continue;
		if( i == 168 && j >= 234 && j <= 254) continue;
		if( i == 169 && j >= 161 && j <= 163) continue;
		if( i == 169 && j >= 240 && j <= 254) continue;
		if( i > 169 && i < 176 ) continue;
		if( i >= 248 ) continue;
		
		m_strList3.AddTail(TempStr)  ;
	}//  
	TempStr="∟";
    m_strList3.AddTail(TempStr)  ;
	TempStr="⊿";
    m_strList3.AddTail(TempStr)  ;
	TempStr="≒";
    m_strList3.AddTail(TempStr)  ;
	TempStr="﹤";
    m_strList3.AddTail(TempStr)  ;
	TempStr="﹥";
    m_strList3.AddTail(TempStr)  ;
	TempStr="≮";
    m_strList3.AddTail(TempStr)  ;
	TempStr="≯";
    m_strList3.AddTail(TempStr)  ;
	TempStr="≦";
    m_strList3.AddTail(TempStr)  ;
	TempStr="≧";
    m_strList3.AddTail(TempStr)  ;
	for(j = 161+31+35; j <=161+31+35+8 ; j++) //单位符号
	{ 
		ss[0] = i;
		ss[1] = j;
		ss[2] = 0;
		TempStr.Format("%s",ss) ;
		if( TempStr.IsEmpty() || TempStr == " " || TempStr == "  "  ) continue;
		if( i == 162 && j >= 171 && j <= 176) continue;
		if( i == 162 && j >= 227 && j <= 228) continue;
		if( i == 162 && j >= 239 && j <= 240) continue;
		if( i == 162 && j >= 253 && j <= 254) continue;
		if( i == 164 && j >= 244 && j <= 254) continue;
		if( i == 165 && j >= 247 && j <= 254) continue;
		if( i == 166 && j >= 185 && j <= 254) continue;
		if( i == 167 && j >= 194 && j <= 254) continue;
		if( i == 168 && j >= 193 && j <= 196) continue;
		if( i == 168 && j >= 234 && j <= 254) continue;
		if( i == 169 && j >= 161 && j <= 163) continue;
		if( i == 169 && j >= 240 && j <= 254) continue;
		if( i > 169 && i < 176 ) continue;
		if( i >= 248 ) continue;
		
		m_strList4.AddTail(TempStr)  ;
	}
	TempStr="￥";
    m_strList4.AddTail(TempStr)  ;
	TempStr="〒";
    m_strList4.AddTail(TempStr)  ;
	TempStr="℉";
	TempStr="㎡";
    m_strList4.AddTail(TempStr)  ;
	TempStr="﹪";
    m_strList4.AddTail(TempStr)  ;
	TempStr="○";
    m_strList4.AddTail(TempStr)  ;
    
                        //

    for(j = 161+31+33; j <=161+31+33+1;j++) //特殊符号
	{
	    ss[0] = i;
		ss[1] = j;
		ss[2] = 0;
		TempStr.Format("%s",ss); 
		m_strList2.AddTail(TempStr)  ;
	}
	for(j = 161+31+35+9; j <=161+31+35+27 ; j++) //特殊符号
	{ 
		ss[0] = i;
		ss[1] = j;
		ss[2] = 0;
		TempStr.Format("%s",ss) ;
		if( TempStr.IsEmpty() || TempStr == " " || TempStr == "  "  ) continue;
		if( i == 162 && j >= 171 && j <= 176) continue;
		if( i == 162 && j >= 227 && j <= 228) continue;
		if( i == 162 && j >= 239 && j <= 240) continue;
		if( i == 162 && j >= 253 && j <= 254) continue;
		if( i == 164 && j >= 244 && j <= 254) continue;
		if( i == 165 && j >= 247 && j <= 254) continue;
		if( i == 166 && j >= 185 && j <= 254) continue;
		if( i == 167 && j >= 194 && j <= 254) continue;
		if( i == 168 && j >= 193 && j <= 196) continue;
		if( i == 168 && j >= 234 && j <= 254) continue;
		if( i == 169 && j >= 161 && j <= 163) continue;
		if( i == 169 && j >= 240 && j <= 254) continue;
		if( i > 169 && i < 176 ) continue;
		if( i >= 248 ) continue;
		
		m_strList2.AddTail(TempStr)  ;
	}
	
	i=168;
	for(j = 161; j <=161+31 ; j++) //254  //拼音
	{ 
		ss[0] = i;
		ss[1] = j;
		ss[2] = 0;
		TempStr.Format("%s",ss) ;
		if( TempStr.IsEmpty() || TempStr == " " || TempStr == "  "  ) continue;
		if( i == 162 && j >= 171 && j <= 176) continue;
		if( i == 162 && j >= 227 && j <= 228) continue;
		if( i == 162 && j >= 239 && j <= 240) continue;
		if( i == 162 && j >= 253 && j <= 254) continue;
		if( i == 164 && j >= 244 && j <= 254) continue;
		if( i == 165 && j >= 247 && j <= 254) continue;
		if( i == 166 && j >= 185 && j <= 254) continue;
		if( i == 167 && j >= 194 && j <= 254) continue;
		if( i == 168 && j >= 193 && j <= 196) continue;
		if( i == 168 && j >= 234 && j <= 254) continue;
		if( i == 169 && j >= 161 && j <= 163) continue;
		if( i == 169 && j >= 240 && j <= 254) continue;
		if( i > 169 && i < 176 ) continue;
		if( i >= 248 ) continue;
		
		m_strList6.AddTail(TempStr)  ;
	}
}

/*DrawSelect()函数
在右边的显示框显示选择的字符,代码很简单参看左键按下即可*/
void CDlgSpecialChar::DrawSelect()
{
	CDC * hdc;
	hdc=GetDC();
	CFont myFont;
	myFont.CreatePointFont(-300,   "宋体");
	hdc->SelectObject(myFont);
	RECT re ;  //290,120,
	re.top =121+20;     //下移
	re.left =291;  //左移
	re.bottom =re.top+80;
	re.right =re.left+80;
		hdc->SetTextColor(RGB(0,0,255));
	hdc->DrawText(m_select,&re, DT_CENTER);
}

/*OnTimer(UINT nIDEvent)函数，也许大家很奇怪为什么要用。这是一个csdn好友介绍的解决办法:
因为在刷屏时，属性页控件会覆盖InitData()(如果在OnPaint()函数中调用)所画的全部图。
所以在OnPaint()函数中，设置一个很小的定时器。
不过，这也许不是标准的解决办法，希望知道的朋友能和我说一下，谢谢!*/
void CDlgSpecialChar::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	InitData();
	KillTimer(nIDEvent);
	CDialog::OnTimer(nIDEvent);
}

/*OnSelchangeTab1()函数，属性页的相应函数，不用多解释了*/
void CDlgSpecialChar::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int sel = m_tab.GetCurSel();
	switch(sel)
	{
	case 0:
		num=1;
	    InitData();
		break;
	case 1:
		num=2;
		InitData();
		break;
	case 2:
		num=3;
		InitData();
		break;
	case 3:
		num=4;
		InitData();
		break;
	case 4:
		num=5;
		InitData();
		break;
	case 5:
		num=6;
		InitData();
		break;
	}
	*pResult = 0;
}

void CDlgSpecialChar::OnInsert()
{
	m_pEdit->UpdateData(true);
	int   nStart,   nEnd; 
	CString strText;
	
	m_pEdit->GetSel(nStart,   nEnd);  //获取光标的位置
	m_pEdit->GetWindowText(strText);
    strText.Insert(nStart,   m_select);  //插入选择的字符
	m_pEdit->SetWindowText(strText);
	m_pEdit->UpdateData(false);
	
	if(nStart==nEnd)
		m_pEdit->SetSel(nStart+2,   nEnd+2);  //将光标定位于插入的字符后
	m_pEdit->SetFocus();        //获得焦点，即显示光标

	GetDlgItem(IDCANCEL)->SetWindowText("关闭");
	UpdateData(FALSE);
}

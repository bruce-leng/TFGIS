// BPnetwork.cpp: implementation of the BPnetwork class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BPnetwork.h"
#include "matrix_op.h"
#include "math.h"
#include "time.h"
#include <sys/timeb.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


int mark_wb=0;
int errorfilemark=0;//�ļ���������
int dongliangmark=0;//�Ƿ�ʹ�ö������ӵı�־

int r;	// ����ڵ���
int s1;	// ѵ������ά��
int s2;	// �������ά��

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BPnetwork::BPnetwork()
{
	m_ppInput = NULL;
	m_ppOutput = NULL;

	m_W1_init = NULL;
	m_W2_init = NULL;
	m_B1_init = NULL;
	m_B2_init = NULL;
	m_W1_final = NULL;
	m_W2_final = NULL;
	m_B1_final = NULL;
	m_B2_final = NULL;
}

BPnetwork::~BPnetwork()
{
	ClearMem();

	if (m_ppOutput)
	{
		for (int i=0; i<m_nClassNum; i++)
			delete [] m_ppOutput[i];
		delete [] m_ppOutput;
	}

	if (m_ppInput)
	{
		for (int i=0; i<m_nSampleNum; i++)
			delete [] m_ppInput[i];
		delete [] m_ppInput;
	}
}

void BPnetwork::Initial()
{
	m_W1_init = new double [s1*r];
	m_W2_init = new double [s2*s1];
	m_B1_init = new double [s1];
	m_B2_init = new double [s2];
	m_W1_final = new double [s1*r];
	m_W2_final = new double [s2*s1];
	m_B1_final = new double [s1];
	m_B2_final = new double [s2];
}

void BPnetwork::ClearMem()
{
	delete [] m_W1_init;
	delete [] m_W2_init;
	delete [] m_B1_init;
	delete [] m_B2_init;
	delete [] m_W1_final;
	delete [] m_W2_final;
	delete [] m_B1_final;
	delete [] m_B2_final;
}

// ��ȡ��������
void BPnetwork::ReadSample(CString strPathName)
{
	CStdioFile file(strPathName, CFile::modeRead);

	int i,j;

	// ���ݵ�1�ж�ȡ������
	CString str;
	file.ReadString(str);
	m_nSampleNum = atoi(str);

	// ���ݵ�2�ж�ȡ���������
	file.ReadString(str);
	m_nClassNum = atoi(str);

	// ���ݵ�3�ж�ȡ��������ά��
	file.ReadString(str);
	m_nDimIn = atoi(str);

	// ��������������õ��������ά��
	m_nDimOut = 0;
	int nTemp = m_nClassNum-1;
	while (nTemp)
	{
		nTemp /= 2;
		m_nDimOut ++;
	}

	// ����������������ά���õ���������ά��
	m_nDimHide = int(sqrt(m_nDimIn+m_nDimOut)+0.5)+2;

	// ���������������
	m_ppOutput = new double* [m_nClassNum];
	for (i=0; i<m_nClassNum; i++)
	{
		int nC1, nC2;
		nC1 = i;
		m_ppOutput[i] = new double [m_nDimOut];
		for (j=0; j<m_nDimOut; j++)
		{
			nC2 = nC1%2;
			nC1 /= 2;
			m_ppOutput[i][j] = double(nC2);
		}
	}

	// ��ȡ��������
	int nNum = 0;
	m_ppInput = new double* [m_nSampleNum];
	for (i=0; i<m_nSampleNum; i++)
		m_ppInput[i] = new double [m_nDimIn];
	while (file.ReadString(str))
	{
		if (str == "")
			continue;

		int nIdx = 0;
		int nLength = str.GetLength();
		for (j=0; j<str.GetLength(); j++)
		{
			char ch = str.GetAt(j);
			if (str.GetAt(j)==',' && nIdx<m_nDimIn)
				m_ppInput[nNum][nIdx++] = atof(str.Right(nLength-j-1));
		}
		nNum ++;
	}

	r = m_nDimIn;
	s1 = m_nDimHide;
	s2 = m_nDimOut;
}

//��60����֪����������ѵ��BP���硣���У�10�������Ǵ�����δ����ģ�50������������
void BPnetwork::Train() 
{   dongliangmark=0;
	errorfilemark++;
	if(mark_wb==0) prewb();//���������ʼȨֵ��ƫ�ô����init
	int i,current,loops,trainok;
	int* mark = new int [m_nSampleNum];
	//int mark[Ns];//ѵ���ɹ���־
	CString str,str1;
	//numE=(double*)malloc(N_Err*sizeof(double));//������N_Err��ľ������
	/////////////////////////////////////////////////////////////////////
	current=loops=trainok=0;
	for(i=0;i<N_Err;i++) numE[i]=0;
	for(i=0;i<m_nSampleNum;i++) mark[i]=0;
	initwb();//��ʼ��Ȩֵ��ƫ�ã���init��ֵ��final��
	/////////////////////////////////////////////////////////////////////
	double* temp[15];
	CTime startTime = CTime::GetCurrentTime();
	struct _timeb timebuffer;
	int mills1,mills2;
	_ftime(&timebuffer);
	mills1=timebuffer.millitm;

	double *T,*P;
	double *N1,*N2,*A1,*A2,*dW1,*dW2,*dB1,*dB2,*S1,*S2,*E;

step2:
	loadwb();//��final��ֵ���·����ڴ�ռ��W1,W2,B1,B2
	current=loops%m_nSampleNum;//current=loops%Ns;
	P=m_ppInput[current];//P_samples[current];//currentΪ0��9����ѭ��ָ������0��9�ı�׼��������
	T=m_ppOutput[current%m_nClassNum];//T_output[current];

	/******************************�������򴫲���Ϣ********************************/
	N1=MatrixAdd((temp[0]=MatrixMul(m_W1,P,s1,r,1)),m_B1,s1,1);  //N1: s1��1
	A1=f1(N1,s1);//f1���Զ���ĺ������߱���ʸ����������Ĺ���
	N2=MatrixAdd((temp[1]=MatrixMul(m_W2,A1,s2,s1,1)),m_B2,s2,1);//N2: s2��1
	A2=f2(N2,s2);
	/******************************���򴫲����ļ���******************************/
	E=MatrixSub(T,A2,s2,1);//T��A2��EΪs2��1
	numE[loops]=0;	//ע�⣬��ʱ������һ�����ˣ��Ǹ�����
	for(i=0;i<s2;i++)
        numE[loops]=numE[loops]+E[i]*E[i];
	//-------����S2��S1��ע��˳��
	S2=MatrixMulNum((temp[3]=MatrixMul((temp[2]=F2(N2,s2)),E,s2,s2,1)),s2,1,-2.0);
	S1=MatrixMul(temp[6]=(MatrixMul((temp[4]=F1(N1,s1)),(temp[5]=MatrixInver(m_W2,s2,s1)),s1,s1,s2)),S2,s1,s2,1);
	//�����������Ԫ��Ȩֵ��ƫ��
	dW2=MatrixMul(S2,(temp[7]=MatrixInver(A1,s1,1)),s2,1,s1);//dW2: s2��s1
	dW2=MatrixMulNum((temp[8]=dW2),s2,s1,-learnspeed);
	m_W2=MatrixAdd((temp[9]=m_W2),dW2,s2,s1);
	dB2=MatrixMulNum(S2,s2,1,-learnspeed);
	m_B2=MatrixAdd((temp[10]=m_B2),dB2,s2,1);
	//����������ԪȨֵ��ƫ��
	dW1=MatrixMul(S1,(temp[11]=MatrixInver(P,r,1)),s1,1,r);//dW1: s1��r
	dW1=MatrixMulNum((temp[12]=dW1),s1,r,-learnspeed);
	m_W1=MatrixAdd((temp[13]=m_W1),dW1,s1,r);
	dB1=MatrixMulNum(S1,s1,1,-learnspeed);
	m_B1=MatrixAdd((temp[14]=m_B1),dB1,s1,1);
//    dc.DrawText (_T("����ѵ������...���Ե�...."),-1,&rect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	TRACE("%d loops, sample idx: %d, error is: %f\n",loops, current, numE[loops]);
	savewb();//����ʱ�ռ�W��B�е����ݴ���final
	//�ͷŶ�̬������ڴ���
	free(m_W1);free(m_B1);free(N1);free(A1);free(dW1);free(dB1);free(S1);
	free(m_W2);free(m_B2);free(N2);free(A2);free(dW2);free(dB2);free(S2);free(E);
	for(i=0;i<15;i++) free(temp[i]);
	if(loops>N_Err) goto step3;
	if(numE[loops]>error_accept)  //˵����������Ҫ�󡣼�������
	{   mark[current]=0; //��֤��Чλ���¼���
		++loops;
	    goto step2;
	}
	else mark[current]=1;//ִ�е��˲�˵���ڸôε����У�������Ҫ�󣬼ǹ�һ��
	trainok=0;
	for(i=0;i<m_nSampleNum;i++)
		trainok+=mark[i];
	if(trainok>=m_nSampleNum)
	{   //ѵ�����Խ�����;
	    goto step3;
	}
	else {  ++loops; goto step2;}
step3:
	m_learncount=loops-1;//�����ܵ�ѧϰ�����������ȫ�ֱ���
	str1.Format("ѵ����ɣ�\n�ܹ�ѵ���ˣ�%d��\n������%f", m_learncount, numE[loops-1]);
	str=str1;

	//����ѵ������ʱ��
	CTime endTime = CTime::GetCurrentTime();
	CTimeSpan elapsedTime = endTime - startTime;
	_ftime(&timebuffer);
	mills2=timebuffer.millitm;
	long int timeused;
	timeused=elapsedTime.GetTotalSeconds();
	if (mills1<=mills2)
		timeused=timeused*1000+(mills2-mills1);//������ѵ������ʱ�任��ɺ���
	else
		timeused=(timeused-1)*1000+(mills1-mills2);
	str1.Format ("\nѵ����������ʱ�䣺%d ����",timeused);
	str += str1;

    ::AfxMessageBox(str); 
	delete [] mark;
}

//���붯�����ӵ�ѵ������
void BPnetwork::TrainOptimal()
{   dongliangmark=1;
	errorfilemark++;
	if(mark_wb==0) prewb();//���������ʼȨֵ��ƫ�ô����init
	int i,j,current,loops,trainok;
	int* mark = new int [m_nClassNum];
//	int mark[Ns];//ѵ���ɹ���־
	CString str,str1;
	//numE=(double*)malloc(N_Err*sizeof(double));//������N_Err��ľ������
	/////////////////////////////////////////////////////////////////////
	current=loops=trainok=0;
	for(i=0;i<N_Err;i++) numE[i]=0;
	for(i=0;i<m_nSampleNum;i++) mark[i]=0;
	initwb();//��ʼ��Ȩֵ��ƫ�ã���init��ֵ��final��
	/////////////////////////////////////////////////////////////////////
	double* temp[23];
	double* dw1=(double*)malloc(s1*r*sizeof(double));
	double* dw2=(double*)malloc(s2*s1*sizeof(double));
	double* db1=(double*)malloc(s1*1*sizeof(double));
	double* db2=(double*)malloc(s2*1*sizeof(double));
	for(i=0;i<s1;i++)
	{   db1[i]=0.1;
	   for(j=0;j<r;j++)
		  dw1[i*r+j]=0.1;
	}
	for(i=0;i<s2;i++)
	{   db2[i]=0.1;
	   for(j=0;j<s1;j++)
		  dw2[i*s1+j]=0.1;
	}
	CTime startTime = CTime::GetCurrentTime();
	struct _timeb timebuffer;
	int mills1,mills2;
    _ftime(&timebuffer);
	mills1=timebuffer.millitm;

	double *T,*P;
	double *N1,*N2,*A1,*A2,*dW1,*dW2,*dB1,*dB2,*S1,*S2,*E;

step2:
	loadwb();//��final��ֵ���·����ڴ�ռ��W1,W2,B1,B2
	current=loops%m_nSampleNum;
	P=m_ppInput[current];//currentΪ0��9����ѭ��ָ������0��9�ı�׼��������
	T=m_ppOutput[current/m_nClassNum];
	/******************************�������򴫲���Ϣ********************************/
	N1=MatrixAdd((temp[0]=MatrixMul(m_W1,P,s1,r,1)),m_B1,s1,1);  //N1: s1��1
	A1=f1(N1,s1);//f1���Զ���ĺ������߱���ʸ����������Ĺ���
	N2=MatrixAdd((temp[1]=MatrixMul(m_W2,A1,s2,s1,1)),m_B2,s2,1);//N2: s2��1
	A2=f2(N2,s2);
	/******************************���򴫲����ļ���******************************/
	E=MatrixSub(T,A2,s2,1);//T��A2��EΪs2��1
	numE[loops]=0;	//ע�⣬��ʱ������һ�����ˣ��Ǹ�����
	for(i=0;i<s2;i++)
        numE[loops]=numE[loops]+E[i]*E[i];
	//-------����S2��S1��ע��˳��
	S2=MatrixMulNum((temp[3]=MatrixMul((temp[2]=F2(N2,s2)),E,s2,s2,1)),s2,1,-2.0);
	S1=MatrixMul(temp[6]=(MatrixMul((temp[4]=F1(N1,s1)),(temp[5]=MatrixInver(m_W2,s2,s1)),s1,s1,s2)),S2,s1,s2,1);
	//�����������Ԫ��Ȩֵ��ƫ��
	dW2=MatrixMul(S2,(temp[7]=MatrixInver(A1,s1,1)),s2,1,s1);//dW2: s2��s1
	dW2=MatrixMulNum((temp[8]=dW2),s2,s1,(rmobp-1)*learnspeed);
	temp[15]=MatrixMulNum(dw2,s2,s1,rmobp);
	temp[16]=MatrixAdd(temp[15],dW2,s2,s1);
	m_W2=MatrixAdd((temp[9]=m_W2),temp[16],s2,s1);
	dB2=MatrixMulNum(S2,s2,1,(rmobp-1)*learnspeed);
	temp[17]=MatrixMulNum(db2,s2,1,rmobp);
	temp[18]=MatrixAdd(temp[17],dB2,s2,1);
	m_B2=MatrixAdd((temp[10]=m_B2),temp[18],s2,1);
	//����������ԪȨֵ��ƫ��
	dW1=MatrixMul(S1,(temp[11]=MatrixInver(P,r,1)),s1,1,r);//dW1: s1��r
    dW1=MatrixMulNum((temp[12]=dW1),s1,r,(rmobp-1)*learnspeed);
	temp[19]=MatrixMulNum(dw1,s1,r,rmobp);
	temp[20]=MatrixAdd(temp[19],dW1,s1,r);
	m_W1=MatrixAdd((temp[13]=m_W1),temp[20],s1,r);
	dB1=MatrixMulNum(S1,s1,1,(rmobp-1)*learnspeed);
	temp[21]=MatrixMulNum(db1,s1,1,rmobp);
	temp[22]=MatrixAdd(temp[21],dB1,s1,1);
	m_B1=MatrixAdd((temp[14]=m_B1),temp[22],s1,1);
//  dc.DrawText (_T("����ѵ������...���Ե�...."),-1,&rect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	TRACE("%d loops, error is: %f  \n",loops,numE[loops]);
	savewb();//����ʱ�ռ�W��B�е����ݴ���final
	for(i=0;i<s1;i++)
	{   db1[i]=dB1[i];
	   for(j=0;j<r;j++)
		  dw1[i*r+j]=dW1[i*r+j];
	}
	for(i=0;i<s2;i++)
	{   db2[i]=dB2[i];
	   for(j=0;j<s1;j++)
		  dw2[i*s1+j]=dW2[i*s1+j];
	}
	//�ͷŶ�̬������ڴ���
	free(m_W1);free(m_B1);free(N1);free(A1);free(dB1);free(S1);free(dW1);
	free(m_W2);free(m_B2);free(N2);free(A2);free(dB2);free(S2);free(dW2);free(E);
	for(i=0;i<23;i++) free(temp[i]);
	if(loops>N_Err) goto step3;
	if(numE[loops]>error_accept)  //˵����������Ҫ�󡣼�������
	{   mark[current]=0; //��֤��Чλ���¼���
		++loops;
	    goto step2;
	}
	else mark[current]=1;//ִ�е��˲�˵���ڸôε����У�������Ҫ�󣬼ǹ�һ��
	trainok=0;
	for(i=0;i<m_nSampleNum;i++)
		trainok+=mark[i];
	if(trainok>=m_nSampleNum)
	{   //ѵ�����Խ�����;
	    goto step3;
	}
	else {  ++loops; goto step2;}
step3:
	m_learncount=loops-1;//�����ܵ�ѧϰ�����������ȫ�ֱ���
	str1.Format("ѵ����ɣ�\n�ܹ�ѵ���ˣ�%d��\n������%f", m_learncount, numE[loops-1]);

	str=str1;
	//����ѵ������ʱ��
	CTime endTime = CTime::GetCurrentTime();
	CTimeSpan elapsedTime = endTime - startTime;
    _ftime(&timebuffer);
	mills2=timebuffer.millitm;
	long int timeused;
	timeused=elapsedTime.GetTotalSeconds();
	if (mills1<=mills2)
		timeused=timeused*1000+(mills2-mills1);//������ѵ������ʱ�任��ɺ���
	else
		timeused=(timeused-1)*1000+(mills1-mills2);
	str1.Format ("���붯�����Ӻ�ѵ����������ʱ��Ϊ��%d ����    (��ʼȨֵ����������)",timeused);
	str += str1;
	::AfxMessageBox(str); 
	delete [] mark;
}

//���Ѿ�ѵ����ϵ�BP����Ӧ���ڣ���������δ֪���ֵ�ʶ��
void BPnetwork::Verify() 
{
	int targetindex,i=5,sbl=0;
	double *N1,*N2,*A1,*A2,*P;
    double** temp1 = new double* [m_nSampleNum];
	double** temp2 = new double* [m_nSampleNum];
	double** temp3 = new double* [m_nSampleNum];
	double** temp4 = new double* [m_nSampleNum];
	double** temp5 = new double* [m_nSampleNum];
	double** temp6 = new double* [m_nSampleNum];

	int nRatio[4]={0};
	for(i=0;i<m_nSampleNum;i++)//����40������
	{   P=m_ppInput[i];
		temp2[i]=N1=MatrixAdd((temp1[i]=MatrixMul(m_W1_final,P,s1,r,1)),m_B1_final,s1,1);  //N1: s1��1
		temp3[i]=A1=f1(N1,s1);
		temp5[i]=N2=MatrixAdd((temp4[i]=MatrixMul(m_W2_final,A1,s2,s1,1)),m_B2_final,s2,1);//N2: s2��1
		temp6[i]=A2=f2(N2,s2);
		targetindex=TargetTest(A2);
		TRACE("��%dʶ������%d\n", i%m_nClassNum, targetindex);
		if(i%m_nClassNum==targetindex) nRatio[i%m_nClassNum]++;
	}

	CString str, str1;
	for (i=0; i<m_nClassNum; i++)
	{
		str1.Format("�Ե�%d���ʶ���ʴﵽ��%3.2f%%\n", i, 100*(double)nRatio[i]/(m_nSampleNum/m_nClassNum));
		str += str1;
	}
	::AfxMessageBox(str);
    for(i=0;i<m_nSampleNum;i++)
	{ free(temp1[i]);free(temp2[i]);free(temp3[i]);free(temp4[i]);free(temp5[i]);free(temp6[i]);}
	delete [] temp1;
	delete [] temp2;
	delete [] temp3;
	delete [] temp4;
	delete [] temp5;
	delete [] temp6;
}

int BPnetwork::Recognize(double* pSample, int nDim)
{
//	ASSERT(nDim == m_nDimIn);

	int targetindex;
    double *N1,*N2,*A1,*A2,*temp1,*temp2;
	N1=MatrixAdd((temp1=MatrixMul(m_W1_final,pSample,s1,r,1)),m_B1_final,s1,1);  //N1: s1��1
	A1=f1(N1,s1);
	N2=MatrixAdd((temp2=MatrixMul(m_W2_final,A1,s2,s1,1)),m_B2_final,s2,1);//N2: s2��1
	A2=f2(N2,s2);
	targetindex=TargetTest(A2);

	delete [] N1;
	delete [] N2;
	delete [] A1;
	delete [] A2;
	delete [] temp1;
	delete [] temp2;

	return targetindex;
}

//������ó�ʼȨֵ��ƫ�á�����һ��ȡ��1��1֮��������
//��ʼ��Ȩֵ��ƫ�ô����init
void BPnetwork::prewb() 
{ 
	Initial();

	mark_wb=1;//��ʾ�Ѿ������������ʼȨֵ��ƫ��
	//double *W1,*W2,*B1,*B2;
	m_W1=MatrixRand(s1,r);  m_B1=MatrixRand(s1,1);
	m_W2=MatrixRand(s2,s1); m_B2=MatrixRand(s2,1);	
	int i,j;
	for(i=0;i<s1;i++) 
	{   m_B1_init[i]=m_B1[i];
		for(j=0;j<r;j++) 
			m_W1_init[i*r+j]=m_W1[i*r+j];
	}
    for(i=0;i<s2;i++) 
	{   m_B2_init[i]=m_B2[i];
		for(j=0;j<s1;j++) 
			m_W2_init[i*s1+j]=m_W2[i*s1+j];
	}
	free(m_W1);
	free(m_W2);
	free(m_B1);
	free(m_B2);
	CString str;
	::AfxMessageBox(_T("�������������ʼȨֵ��ƫ��.....OK��") );
}

//����ѧϰ�������
void BPnetwork::DrawError(CDC* pDC, const CRect& rect) 
{
	CPen pen(PS_SOLID,3,RGB(105,90,170));
	CPen* pOldPen = pDC->SelectObject(&pen);
	int n,i;
	double numEsum[2000]={0};
	for(n=0;n<rect.Width();n++)
	    numEsum[n]+=numE[n*m_learncount/rect.Width()];
	for(i=0;i<rect.Width();i++)
	{   pDC->MoveTo (30+i ,(int)((float)rect.Height()*0.98));
	    pDC->LineTo (30+i ,(int)((float)rect.Height()*0.98-100*numEsum[i]));
	}
	pDC->SelectObject(pOldPen);
}

//����ǰ��Ȩֵfinal��ֵ��ʼ������Ϊinitֵ������initֵΪ��������ĳ�ʼȨֵ��ƫ��
void BPnetwork::initwb()
{   int i,j;
	for(i=0;i<s1;i++) 
	{   m_B1_final[i]=m_B1_init[i];
		for(j=0;j<r;j++) 
			m_W1_final[i*r+j]=m_W1_init[i*r+j];
	}
    for(i=0;i<s2;i++) 
	{   m_B2_final[i]=m_B2_init[i];
		for(j=0;j<s1;j++) 
			m_W2_final[i*s1+j]=m_W2_init[i*s1+j];
	}
}

//����ǰ��Ȩֵfinal�����һ��ʱ����W1��W2��B1��B2���Ա�����Ժ�����㡣���ﶯ̬�������ڴ�
void BPnetwork::loadwb()
{   int i,j;
    m_W1=(double*)malloc(s1*r*sizeof(double));
    m_W2=(double*)malloc(s2*s1*sizeof(double));
	m_B1=(double*)malloc(s1*1*sizeof(double));
	m_B2=(double*)malloc(s2*1*sizeof(double));
	for(i=0;i<s1;i++) 
	{   m_B1[i]=m_B1_final[i];
		for(j=0;j<r;j++) 
			m_W1[i*r+j]=m_W1_final[i*r+j];
	}
    for(i=0;i<s2;i++) 
	{   m_B2[i]=m_B2_final[i];
		for(j=0;j<s1;j++) 
			m_W2[i*s1+j]=m_W2_final[i*s1+j];
	}
}


//��������ʱ�ڴ�����W1��W2��B1��B2��ֵ�洢�ڵ�ǰȨֵfinal��˲��Ժ�Ϳ���ɾ����ʱ�ڴ������Ż��ڴ�
void BPnetwork::savewb()
{   int i,j;
	for(i=0;i<s1;i++) 
	{   m_B1_final[i]=m_B1[i];
		for(j=0;j<r;j++) 
			m_W1_final[i*r+j]=m_W1[i*r+j];
	}
    for(i=0;i<s2;i++) 
	{   m_B2_final[i]=m_B2[i];
		for(j=0;j<s1;j++) 
			m_W2_final[i*s1+j]=m_W2[i*s1+j];
	}
}

//f1��f2Ϊ�����
double* BPnetwork::f1(double A[],int m)
{   int i;
    double *B=NULL;
	B=(double*)malloc(m*1*sizeof(double));//B��һ��������
	for(i=0;i<m;i++)
		B[i]=(double)1/(1+exp(-A[i]));
	return B;
}

double* BPnetwork::f2(double A[],int m)
{   int i;
    //double c=1.0;
    double *B=NULL;
	B=(double*)malloc(m*1*sizeof(double));
	for(i=0;i<m;i++)
		B[i]=A[i];
	return B;
}

//F1��F2Ϊ��������󵼣�������ʽ��
double* BPnetwork::F1(double N[],int m)
{   int i,j;
    double *B=NULL;
	B=(double*)malloc(m*m*sizeof(double));
	for(i=0;i<m;i++)
		for(j=0;j<m;j++)
		   B[i*m+j]=0;
	for(i=0;i<m;i++)
		B[i*m+i]=exp(-N[i])/((1+exp(-N[i]))*(1+exp(-N[i])));
	return B;
}

double* BPnetwork::F2(double N[],int m)
{   int i,j;
    double *B=NULL;
	B=(double*)malloc(m*m*sizeof(double));
	for(i=0;i<m;i++)
		for(j=0;j<m;j++)
		   B[i*m+j]=0;
	for(i=0;i<m;i++)
		B[i*m+i]=1;
	return B;
}

//����ŷʽ����ƥ��BP����������������ĸ�����
int BPnetwork::TargetTest(double A[])
{   double a,min=100;
    int index=-1,i,j;
	double* Temp;
    for(i=0;i<m_nClassNum;i++)
	{   Temp=MatrixSub(A,m_ppOutput[i],s2,1);
		a=0;
	    for(j=0;j<s2;j++)
		    a=a+Temp[j]*Temp[j];
        if(a<min)
		{
			min=a;
			index=i;
		}
		free(Temp);
	}
	return index;
}

// ��ȡȨֵ
BOOL BPnetwork::ReadPowers(char* filename)
{
	// ���Ȩֵ�Ѿ���ȡ
	mark_wb = 1;

	CStdioFile file;
	if (file.Open(filename, CFile::modeRead))
	{
		int i, nIdx;
		CString str, str1;
		char ch;

		// ��W1_final
		nIdx = 0;
		if (!file.ReadString(str))
			return FALSE;

		for (i=0; i<str.GetLength(); i++)
		{
			ch = str.GetAt(i);
			if (ch!=',')
			{
				str1 += ch;
			}
			else
			{
				m_W1_final[nIdx++] = atof(str1);
				str1.Empty();
			}
		}

		// ��W2_final
		nIdx = 0;
		if (!file.ReadString(str))
			return FALSE;

		for (i=0; i<str.GetLength(); i++)
		{
			ch = str.GetAt(i);
			if (ch!=',')
			{
				str1 += ch;
			}
			else
			{
				m_W2_final[nIdx++] = atof(str1);
				str1.Empty();
			}
		}

		// ��B1_final
		nIdx = 0;
		if (!file.ReadString(str))
			return FALSE;

		for (i=0; i<str.GetLength(); i++)
		{
			ch = str.GetAt(i);
			if (ch!=',')
			{
				str1 += ch;
			}
			else
			{
				m_B1_final[nIdx++] = atof(str1);
				str1.Empty();
			}
		}

		// ��B2_final
		nIdx = 0;
		if (!file.ReadString(str))
			return FALSE;

		for (i=0; i<str.GetLength(); i++)
		{
			ch = str.GetAt(i);
			if (ch!=',')
			{
				str1 += ch;
			}
			else
			{
				m_B2_final[nIdx++] = atof(str1);
				str1.Empty();
			}
		}

		file.Close();

		return TRUE;
	}

	return FALSE;
}

void BPnetwork::SetPowers()
{
	m_nDimIn = r = 12;
	m_nDimHide = s1 = 7;
	m_nDimOut = s2 = 2;
	m_nClassNum = 4;

	Initial();

	memcpy(m_W1_final,W1_final,sizeof(double)*s1*r);
	memcpy(m_W2_final,W2_final,sizeof(double)*s2*s1);
	memcpy(m_B1_final,B1_final,sizeof(double)*s1*1);
	memcpy(m_B2_final,B2_final,sizeof(double)*s2*1);

	// ���������������
	if (!m_ppOutput)
		m_ppOutput = new double* [m_nClassNum];
	for (int i=0; i<m_nClassNum; i++)
	{
		int nC1, nC2;
		nC1 = i;
		m_ppOutput[i] = new double [m_nDimOut];
		for (int j=0; j<m_nDimOut; j++)
		{
			nC2 = nC1%2;
			nC1 /= 2;
			m_ppOutput[i][j] = double(nC2);
		}
	}
}

// ����Ȩֵ
void BPnetwork::SavePowers(char* filename)
{
	CStdioFile file;
	if (file.Open(filename, CFile::modeCreate|CFile::modeReadWrite))
	{
		int i;
		CString str, str1;

		// W1_final
		for (i=0; i<s1*r; i++)
		{
			str1.Format("%f,", m_W1_final[i]);
			str += str1;
		}
		str += '\n';
		file.WriteString(str);
		str.Empty();

		// W2_final
		for (i=0; i<s2*s1; i++)
		{
			str1.Format("%f,", m_W2_final[i]);
			str += str1;
		}
		str += '\n';
		file.WriteString(str);
		str.Empty();

		// B1_final
		for (i=0; i<s1; i++)
		{
			str1.Format("%f,", m_B1_final[i]);
			str += str1;
		}
		str += '\n';
		file.WriteString(str);
		str.Empty();

		// B2_final
		for (i=0; i<s2; i++)
		{
			str1.Format("%f,", m_B2_final[i]);
			str += str1;
		}
		file.WriteString(str);

		file.Close();
	}
}

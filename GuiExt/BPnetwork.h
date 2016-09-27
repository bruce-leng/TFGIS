// BPnetwork.h: interface for the BPnetwork class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BPNETWORK_H__B93FF85F_9681_43C4_AAB3_7EEF49CE9FB6__INCLUDED_)
#define AFX_BPNETWORK_H__B93FF85F_9681_43C4_AAB3_7EEF49CE9FB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


static const double error_accept=0.001; //容许误差值0.02/0.05/0.2
static const double learnspeed=0.01;	//学习速度0.04/0.03/0.3
static const int N_Err=200000;          //最多迭代计算N_Err次均方误差
static const double rmobp=0.8;          //动量因子 must<1
static const int N_Noise=160;			// 噪声样本数量
static double numE[N_Err];				// 误差

static double W1_final[] = {	// 初始权值s1×r
-0.172574,-3.91137,4.732038,-1.270014,1.21898,3.6347,0.086555,2.869567,0.759852,-0.859242,-2.779861,-3.941385,\
0.178655,0.511724,0.673782,0.994561,-0.201484,0.502466,0.817923,0.343387,0.099642,-0.385441,0.390957,0.085569,\
0.02734,2.310803,-3.568491,1.352563,0.445725,-2.045758,-3.125672,0.212971,0.210684,-0.709466,0.37602,4.816821,\
0.79623,-0.450107,-0.839511,-0.707535,-0.35221,-1.022183,0.403938,0.484677,-0.19675,1.012606,0.878839,0.460566,\
0.741261,0.310465,-0.288524,-0.188318,-0.283142,-0.964499,0.024913,-0.917417,0.245884,-0.491691,-0.817854,-0.37681,\
-0.618395,-0.09005,-0.287478,-0.503183,-0.732095,-0.038396,0.079534,-0.172306,-0.07997,0.366265,0.144312,-1.054769,\
-0.152502,7.515487,-0.63602,1.370723,3.312914,-1.386468,2.335245,-1.158167,1.504702,-2.297554,-0.211272,-5.025548\
};
static double W2_final[] = {	// 初始权值s2×s1
-0.374654,0.47072,-0.074213,0.94457,0.720907,-0.794543,-1.552406,1.215972,0.190641,-0.7841,-0.885654,0.686202,-0.010348,0.779407
};
static double B1_final[] = {	// 初始偏移s1×1
-3.05863,0.347594,1.162681,0.767149,0.720864,0.06837,3.747084
};
static double B2_final[] = {	// 初始偏移s2×r
0.123265,0.462353
};

class BPnetwork  
{
public:
	BPnetwork();
	virtual ~BPnetwork();

	void Initial();
	void ClearMem();

	void Train();
	void TrainOptimal();
	void Verify();
	int  Recognize(double* pSample, int nDim);
	void DrawError(CDC* pDC, const CRect& rect);
	void ReadSample(CString strPathName);	// 读取样本数据

	BOOL ReadPowers(char* filename);	// 读取权值
	void SetPowers();	// 直接设置权值
	void SavePowers(char* filename);	// 保存权值
private:
//  enum {r=N_P};//输入数据是8维的
//	enum {s1=N_Layer1};//隐层有7个神经元
//	enum {s2=N_Out};//输出层有4个神经元

	int m_nSampleNum;	// 输入样本数
	int m_nClassNum;	// 输入样本类别数
	int m_nDimIn;		// 输入层维数
	int m_nDimHide;		// 隐层维数
	int m_nDimOut;		// 输出层维数
public:
	void prewb();
	void initwb();

	void savewb();
	void loadwb();
	int  TargetTest(double A[]);

	double** m_ppInput;
	double** m_ppOutput;

	double* f1(double A[],int m);//对列向量进行函数运算，并返回一个列向量
	double* f2(double A[],int m);
	double* F1(double N[],int m);
	double* F2(double N[],int m);
	double* m_W1;
	double* m_W2;
	double* m_B1;
	double* m_B2;
	double* m_W1_init;	// 初始权值s1×r
	double* m_W2_init;	// 初始权值s2×s1
	double* m_B1_init;	// 初始偏移s1×1
	double* m_B2_init;	// 初始偏移s2×r
	double* m_W1_final;	// 最终权值s1×r
	double* m_W2_final;	// 最终权值s2×s1
	double* m_B1_final;	// 最终偏移s1×1
	double* m_B2_final;	// 最终偏移s2×1
	long int m_learncount;   //最终学习次数
};

#endif // !defined(AFX_BPNETWORK_H__B93FF85F_9681_43C4_AAB3_7EEF49CE9FB6__INCLUDED_)

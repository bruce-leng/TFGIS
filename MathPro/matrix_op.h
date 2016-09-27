#include"stdio.h"
#include<time.h>
#include <math.h>

AFX_EXT_API double Surplus(double A[],int m,int n) /*���������ʽ*/ 
{ 
	
	int i,j,k,p,r; 
	double X,temp=1,temp1=1,s=0,s1=0; 
	
	if(n==2) 
	{for(i=0;i<m;i++) 
	for(j=0;j<n;j++) 
		if((i+j)%2) temp1*=A[i*n+j]; 
		else temp*=A[i*n+j]; 
		X=temp-temp1;} 
	else{ 
		for(k=0;k<n;k++) 
		{for(i=0,j=k;i<m,j<n;i++,j++) 
		temp*=A[i*n+j]; 
		if(m-i) 
		{for(p=m-i,r=m-1;p>0;p--,r--) 
		temp*=A[r*n+p-1];} 
		s+=temp; 
		temp=1; 
		} 

		for(k=n-1;k>=0;k--) 
		{for(i=0,j=k;i<m,j>=0;i++,j--) 
		temp1*=A[i*n+j]; 
		if(m-i) 
		{for(p=m-1,r=i;r<m;p--,r++) 
		temp1*=A[r*n+p];} 
		s1+=temp1; 
		temp1=1; 
		} 
		
		X=s-s1;} 
	return X; 
} 
AFX_EXT_API double * MatrixInver(double A[],int m,int n) /*����ת��*/ 
{ 
	int i,j; 
	double *B=NULL; 
	B=(double *)malloc(m*n*sizeof(double)); 
	
	for(i=0;i<n;i++) 
	{
		for(j=0;j<m;j++) 
			B[i*m+j]=A[j*n+i]; 
	}
	return B; 
} 
AFX_EXT_API double * MatrixOpp(double A[],int m,int n) /*�����*/
{ 
	int i,j,x,y,k; 
	double *SP=NULL,*AB=NULL,*B=NULL,X,*C; 
	SP=(double *)malloc(m*n*sizeof(double)); 
	AB=(double *)malloc(m*n*sizeof(double)); 
	B=(double *)malloc(m*n*sizeof(double)); 
	
	X=Surplus(A,m,n); 
	X=1/X; 
	
	for(i=0;i<m;i++) 
	{ 
		for(j=0;j<n;j++) 
		{
			for(k=0;k<m*n;k++) 
				B[k]=A[k]; 
			{
				for(x=0;x<n;x++) 
					B[i*n+x]=0; 
				for(y=0;y<m;y++) 
					B[m*y+j]=0; 
				B[i*n+j]=1; 
				SP[i*n+j]=Surplus(B,m,n); 
				AB[i*n+j]=X*SP[i*n+j]; 
			} 
		}
	}
	C=MatrixInver(AB,m,n); 
	
	return C; 
}

AFX_EXT_API double * MatrixMul(double A[],double B[],int m,int n,int p) /*�������*/ 
// A:m��n  B:n��p  C:m��p  C����A����B
{ 
	int i,j,k; 
	double *C=NULL; 
	C=(double*)malloc(m*p*sizeof(double)); 
	
	for(i=0;i<m;i++) 
		for(j=0;j<p;j++) 
		{   C[i*p+j]=0;
			for(k=0;k<n;k++)
				C[i*p+j]=C[i*p+j]+A[i*n+k]*B[k*p+j]; 
		}

	return C; 
} 

AFX_EXT_API double * MatrixMulNum(double A[],int m,int n,double num) /*��������*/ 
// A:m��n, m��n��
{ 
	int i,j; 
	double *C=NULL; 
	C=(double*)malloc(m*n*sizeof(double)); 	
	for(i=0;i<m;i++) 
		for(j=0;j<n;j++) 
		  C[i*n+j]=num*A[i*n+j];
	return C; 
}

AFX_EXT_API double * MatrixAdd(double A[],double B[],int m,int n) /*�������*/ 
// A:m��n  B:m��n C:m��n  C����A��B
{ 
	int i,j; 
	double *C=NULL; 
	C=(double*)malloc(m*n*sizeof(double)); 	
	for(i=0;i<m;i++) 
		for(j=0;j<n;j++) 
		   C[i*n+j]=A[i*n+j]+B[i*n+j];		
	return C; 
} 

AFX_EXT_API double * MatrixSub(double A[],double B[],int m,int n) /*�������*/ 
// A:m��n  B:m��n C:m��n  C����A��B
{ 
	int i,j; 
	double *C=NULL; 
	C=(double*)malloc(m*n*sizeof(double)); 	
	for(i=0;i<m;i++) 
		for(j=0;j<n;j++) 
		   C[i*n+j]=A[i*n+j]-B[i*n+j];		
	return C; 
} 

AFX_EXT_API double Matrix2Num(double A[])/*��һ��һ�еľ���Aת��Ϊ��*/
{
	return A[0];
}

AFX_EXT_API double * MatrixRand(int m,int n)/*��(-1,1)֮���������������һ��m��n�ľ���*/
{   int i,j;
    double* C=NULL;
	C=(double*)malloc(m*n*sizeof(double));
	srand((unsigned)(time(NULL)));
    for (i=0;i<m;i++) 
	{
		for (j=0;j<n;j++) 
		{
			//rand()�����������������������������Ҫ��֮���ʵ��ı任���䵽-1��1֮��
			C[i*n+j]= (double)(rand())/(32767/2) - 1;
		}
	}
	return C;	
}

AFX_EXT_API double ABS(double a) /*����a�ľ���ֵ*/
{  if(a>=0) return a;
   else return -a;
}

AFX_EXT_API int* Rand2(int a,int b)/*����b��0��a-1�������*/
{   int i;
    int* C=NULL;C=(int*)malloc(b*sizeof(int));
	srand((unsigned)(time(NULL)));
	for (i=0;i<b;i++) 
		{
			//rand()�����������������������������Ҫ��֮���ʵ��ı任���䵽0��a-1֮��
			C[i]=int(a*rand()/32767);
		}
	return C;
}

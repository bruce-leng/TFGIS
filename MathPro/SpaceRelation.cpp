#include "Stdafx.h"
#include "TilPoint.h"
#include "SpaceRelation.h"


#include <math.h>
#include <memory.h>
#include "matrix_op.h"

// �ж��Ƿ���һ���߶�
bool is_a_line(const POINT_3F& pt1, const POINT_3F& pt2)
{
	return pt1 != pt2;
}

// ��������ȡ��һ��ֱ�ߵĲ���
// x = x1+(x2-x1)*t
// y = y1+(y2-y1)*t
// z = z1+(z2-z1)*t
// �����fLine����������
// x=fLine[0]+fLine[1]*t
// y=fLine[2]+fLine[3]*t
// z=fLine[4]+fLine[5]*t
void get_a_line(const POINT_3F& pt1, const POINT_3F& pt2, float* pLine)
{
	pLine[0] = pt1.x;
	pLine[1] = pt2.x-pt1.x;
	pLine[2] = pt1.y;
	pLine[3] = pt2.y-pt1.y;
	pLine[4] = pt1.z;
	pLine[5] = pt2.z-pt1.z;
}

// ����һ�����Ƿ���ĳƽ����
bool is_point_in_face(float* pFace, const POINT_3F& point)
{
	return fabs(pFace[0]*point.x+pFace[1]*point.y+pFace[2]*point.z+pFace[3]) < MAX_DELTA;
}

// �ж�һ�����Ƿ����߶���
int point_in_line(const POINT_3F& pt, const POINT_3F& pt1, const POINT_3F& pt2)
{
	// �����Сֵ
	POINT_3F ptMax, ptMin;
	ptMin.x = min(pt1.x, pt2.x);	ptMax.x = max(pt1.x, pt2.x);
	ptMin.y = min(pt1.y, pt2.y);	ptMax.y = max(pt1.y, pt2.y);
	ptMin.z = min(pt1.z, pt2.z);	ptMax.z = max(pt1.z, pt2.z);

	double deta, deta1;
	// x������ͬ
	if (pt1.x == pt2.x)
	{
		// x,y������ͬ
		if (pt1.y == pt2.y)
		{
			// ��p���x,y������p1,p2��x,y������ͬ
			if (pt.x == pt1.x && pt.y == pt1.y)
			{
				// ��p��z������p1-p2�޶���z������
				if (pt.z >= ptMin.z && pt.z <= ptMax.z)
					return POINT_IN_LINE;
				else
					return POINT_IN_EX_LINE;
			}
			// ��p���x,y������p1,p2��x,y���겻��ͬ
			else
				return POINT_NOT_IN_LINE;
		}
		// y���겻��ͬ
		else
		{
			// x,z������ͬ
			if (pt1.z == pt2.z)
			{
				// ��p��x,z������p1,p2��x,z������ͬ
				if (pt.x == pt1.x && pt.z == pt1.z)
				{
					if (pt.y >= ptMin.y && pt.y <= ptMax.y)
						return POINT_IN_LINE;
					else
						return POINT_IN_EX_LINE;
				}
				// ��p���x,z������p1,p2��x,z���겻��ͬ
				else
					return POINT_NOT_IN_LINE;
			}
			// ��ֻ��x������ͬ,��ʱֱ��p1-p2ƽ����y,z����ȷ����ƽ����
			else
			{
				// ���p��x������p1,p2��x������ͬ
				if (pt.x == pt1.x)
				{
					// ���б����ͬ
					deta = fabs((pt.y-pt1.y)/(pt1.y-pt2.y) - (pt.z-pt1.z)/(pt1.z-pt2.z));
					if (deta < MAX_DELTA)
					{
						// ��p���y,z��p1-p2�޶����߶�����
						if (pt.x >= ptMin.x && pt.x <= ptMax.x)
							return POINT_IN_LINE;
						else
							return POINT_IN_EX_LINE;
					}
					else
						return POINT_NOT_IN_LINE;
				}
				else
					return POINT_NOT_IN_LINE;
			}
		}
	}
	// x���겻��ͬ
	else
	{
		// y������ͬ
		if (pt1.y == pt2.y)
		{
			// y,z������ͬ����ʱp1-p2��x��ƽ��
			if (pt1.z == pt2.z)
			{
				// ��p���y,z������p1,p2��y,z������ͬ
				if (pt.y == pt1.y && pt.z == pt1.z)
				{
					if (pt.x >= ptMin.x && pt.x <= ptMax.x)
						return POINT_IN_LINE;
					else
						return POINT_IN_EX_LINE;
				}
				else
					return POINT_NOT_IN_LINE;
			}
			// ��ֻ��y������ͬ
			else
			{
				// ��б����ͬ
				deta = fabs((pt.x-pt1.x)/(pt1.x-pt2.x) - (pt.z-pt1.z)/(pt1.z-pt2.z));
				if (deta < MAX_DELTA)
				{
					if (pt.x >= ptMin.x && pt.x <= ptMax.x)
						return POINT_IN_LINE;
					else
						return POINT_IN_EX_LINE;
				}
				else
					return POINT_NOT_IN_LINE;
			}
		}
		// x,y���궼����ͬ
		else
		{
			// ��ֻ��z������ͬ
			if (pt1.z == pt2.z)
			{
				// ��б����ͬ
				deta = fabs((pt.x-pt1.x)/(pt1.x-pt2.x) - (pt.y-pt1.y)/(pt1.y-pt2.y));
				if (deta < MAX_DELTA)
				{
					if (pt.x >= ptMin.x && pt.x <= ptMax.x)
						return POINT_IN_LINE;
					else
						return POINT_IN_EX_LINE;
				}
				else
					return POINT_NOT_IN_LINE;
			}
			// ��x,y,z���궼����ͬ
			else
			{
				deta = fabs((pt.x-pt1.x)/(pt1.x-pt2.x) - (pt.y-pt1.y)/(pt1.y-pt2.y));
				deta1 = fabs((pt.x-pt1.x)/(pt1.x-pt2.x) - (pt.z-pt1.z)/(pt1.z-pt2.z));
				if (deta < MAX_DELTA && deta1 < MAX_DELTA)
				{
					if (pt.x >= ptMin.x && pt.x <= ptMax.x)
						return POINT_IN_LINE;
					else
						return POINT_IN_EX_LINE;
				}
				else
					return POINT_NOT_IN_LINE;
			}
		}
	}
}

// ���������Ƿ񹹳�һ��ƽ��
bool is_a_face(const POINT_3F& pt1, const POINT_3F& pt2, const POINT_3F& pt3)
{
	if (pt1 != pt2 && pt1 != pt3)
		return true;
	else
		return false;
}

// ������������ʽ��ƽ��
// | x-x1  y-y1  z-z1|
// |x2-x1 y2-y1 z2-z1| = 0
// |x3-x1 y3-y1 z3-z1|
// ���ƽ�淽��Ϊ: pFace[0]*x+pFace[1]*y+pFace[2]*z+pFace[3]=0
void get_a_face(const POINT_3F& pt1, const POINT_3F& pt2, const POINT_3F& pt3, float* pFace)
{
	POINT_3F temp1, temp2, temp3;

	temp1.x = pt2.x-pt1.x;	temp1.y = pt2.y-pt1.y;	temp1.z = pt2.z-pt1.z;
	temp2.x = pt3.x-pt1.x;	temp2.y = pt3.y-pt1.y;	temp2.z = pt3.z-pt1.z;

	temp3.x = temp1.y*temp2.z - temp1.z*temp2.y;
	temp3.y = temp1.z*temp2.x - temp1.x*temp2.z;
	temp3.z = temp1.x*temp2.y - temp1.y*temp2.x;

	pFace[0] = temp3.x;
	pFace[1] = temp3.y;
	pFace[2] = temp3.z;
	pFace[3] = -(temp3.x*pt1.x + temp3.y*pt1.y + temp3.z*pt1.z);
}

// �õ���������ڵ�ƽ��
void get_a_face(const PPOINT_3F pPolygon, int num, float* pFace)
{
	int i, flag = 0;
	POINT_3F pt1, pt2, pt3;

	// �ҵ�����εĶ����е�������ͬ�Ķ���
	pt1 = pPolygon[0];
	for (i = 1; i < num; i ++)
	{
		if ((pPolygon[i] != pt1) && (flag == 0))
		{
			pt2 = pPolygon[i];
			flag = 1;
		}
		else if ((pPolygon[i] != pt1) && pPolygon[i] != pt2 && flag == 1)
		{
			pt3 = pPolygon[i];
			break;
		}
	}

	get_a_face(pt1, pt2, pt3, pFace);
}

// �����������Ƿ���һ��ƽ����
bool is_a_face(const PPOINT_3F pPolygon, int num)
{
	int i, flag = 0;
	POINT_3F pt1, pt2, pt3;

	// �ҵ�����εĶ����е�������ͬ�Ķ���
	pt1 = pPolygon[0];
	for (i = 1; i < num; i ++)
	{
		if ((pPolygon[i] != pt1) && (flag == 0))
		{
			pt2 = pPolygon[i];
			flag = 1;
		}
		else if ((pPolygon[i] != pt1) && pPolygon[i] != pt2 && flag == 1)
		{
			pt3 = pPolygon[i];
			break;
		}
	}

	// ���������������ͬ�Ķ��㣬��˵������β���ͬһ������
	if (i == num)	return false;

	float fFace[4];
	get_a_face(pt1, pt2, pt3, fFace);

	// �ж϶���ε����ж��㶼��ƽ����
	for (i = 0; i < num; i ++)
	{
		if (!is_point_in_face(fFace, pPolygon[i]))
			return false;
	}

	return true;
}

// ��㵽ƽ��ľ���
float point_line_distance(float* pFace, const POINT_3F& point)
{
	float fTmp1, fTmp2;

	fTmp1 = (float)fabs(pFace[0]*point.x+pFace[1]*point.y+pFace[2]*point.z+pFace[3]);
	fTmp2 = (float)sqrt(pFace[0]*pFace[0]+pFace[1]*pFace[1]+pFace[2]*pFace[2]);

	return fTmp1/fTmp2;
}

// ���߶���ƽ���Ƿ��н��㣬���У�������ý���
// ����ֵ��0: û�н��㡡1: �н��㡡2: �߶���ƽ����
int get_line_face_interpoint(const POINT_3F& pt1, const POINT_3F& pt2, 
							 float* pFace, POINT_3F& point)
{
	if (is_point_in_face(pFace, pt1) && is_point_in_face(pFace, pt2))
		return 2;
	else if (is_point_in_face(pFace, pt1))
	{
		point = pt1;
		return 1;
	}
	else if (is_point_in_face(pFace, pt2))
	{
		point = pt2;
		return 1;
	}

	// ��������ƽ��ľ����������ƽ���ͬ��,��û�н���
	float fDist1, fDist2, tmp1, tmp2;
	fDist1 = point_line_distance(pFace, pt1);
	fDist2 = point_line_distance(pFace, pt2);

	tmp1 = pFace[0]*pt1.x+pFace[1]*pt1.y+pFace[2]*pt1.z+pFace[3];
	tmp2 = pFace[0]*pt2.x+pFace[1]*pt2.y+pFace[2]*pt2.z+pFace[3];

	// ��tmp1��tmp2ͬ�ţ���˵���߶ε������˵���ƽ���ͬ�࣬��û�н���
	if (fDist1 == fDist2 && tmp1*tmp2 > 0)
		return 0;

	// ����߶���ƽ���ཻ,�������
	float fLine[6], t;
	get_a_line(pt1, pt2, fLine);

	// ��ֱ�߷���x=x1+(x2-x1)*t,y=y1+(y2-y1)*t,z=z1+(z2-z1)*t�еĲ���t
	// tmp2*t+tmp1=0,����pt1����ƽ����,��tmp1!=0.��ʱ��tΪ0,��tmp2*t+tmp1=0�����ܳ���
	// ��t������Ϊ0,ͬ����tmp2������Ϊ0����˿�����tmp1/tmp2
	tmp2 = pFace[0]*fLine[1]+pFace[1]*fLine[3]+pFace[2]*fLine[5];
	t = - tmp1/tmp2;

	point.x = fLine[0] + fLine[1]*t;
	point.y = fLine[2] + fLine[3]*t;
	point.z = fLine[4] + fLine[5]*t;

	return 1;
}

// ��һ��ƽ����תһ���ǶȺ��ƽ��,��ת������������б�׼���Ƹ�����ֱ�߶ν���
// pAxisHead: ��ת�������
// pAxisRear: ��ת�����յ�
// ptSrc: ����ת��
// fAngle: ��ת�Ƕ�
// ptDst: ��ת��ĵ�
void rotate_face(const POINT_3F& ptAxisHead, const POINT_3F& ptAxisRear,
				  const POINT_3F& ptSrc, float fAngle, POINT_3F& ptDst)
{
	double p1[3], p2[3], src[3], dst[3];

	p1[0] = ptAxisHead.x;	p1[1] = ptAxisHead.y;	p1[2] = ptAxisHead.z;
	p2[0] = ptAxisRear.x; p2[1] = ptAxisRear.y; p2[2] = ptAxisRear.z;
	src[0] = ptSrc.x; src[1] = ptSrc.y; src[2] = ptSrc.z; 

	rotate_point(p1, p2, src, fAngle, dst);

	ptDst.x = (float)dst[0];
	ptDst.y = (float)dst[1];
	ptDst.z = (float)dst[2];
}

// ��һ��ƽ������һ��ƽ���ϵĶ���εĽ���
// pFace: ��һ����
// pPolygon: �ڶ������ϵĶ����, num: ����ζ�����
// ���: ppPts������, ptNum: ������
void get_faces_intersections(float* pFace, 
							POINT_3F* pPolygon, int num,
							PPPOINT_3F ppPts, int& ptNum)
{
	int i;
	POINT_3F point, pt[100];

	// ������߶���ƽ��Ľ���
	ptNum = 0;
	for (i = 1; i < num; i ++)
	{
		if (is_a_line(pPolygon[i-1], pPolygon[i]))
		{
			// ���߶���ƽ��Ľ���,����1��ʾ�н��㣬���н��������߶���
			int nRet = get_line_face_interpoint(pPolygon[i-1], pPolygon[i], pFace, point);
			if (nRet==1 && point_in_line(point, pPolygon[i-1], pPolygon[i])==POINT_IN_LINE)
				pt[ptNum ++] = point;
		}
	}
	if (is_a_line(pPolygon[0], pPolygon[num-1]))
	{
		// ����1��ʾ�н���
		int nRet = get_line_face_interpoint(pPolygon[0], pPolygon[num-1], pFace, point);
		if (nRet==1 && point_in_line(point, pPolygon[0], pPolygon[num-1])==POINT_IN_LINE)
			pt[ptNum ++] = point;
	}

	*ppPts = new POINT_3F [ptNum];
	for (i = 0; i < ptNum; i ++)
	{
		(*ppPts)[i] = pt[i];
	}
}

// ��������ľ���
float get_distance(const POINT_3F& pt1, const POINT_3F& pt2)
{
	float fDist = 0.0f;

	fDist += (pt1.x-pt2.x)*(pt1.x-pt2.x);
	fDist += (pt1.y-pt2.y)*(pt1.y-pt2.y);
	fDist += (pt1.z-pt2.z)*(pt1.z-pt2.z);

	return fDist;
}

// �Զ���α߽��ϵĵ��ر߽��߽�������(��Щ��ָһ��ƽ����һ����α߽�Ľ���)
void sort_intersections(PPOINT_3F pPolygon, int num, PPOINT_3F pPts, int ptNum)
{
	int i, j, k;
	PPOINT_3F pRetPts = new POINT_3F [ptNum];	// �����ĵ�
	PPOINT_3F pHead = pRetPts;	// ��סָ��ͷ

	int nSize;
	PPOINT_3F pPoint = new POINT_3F [ptNum];
	double* pDist = new double [ptNum];
	int* pIdx = new int [ptNum];

	int nIdx1, nIdx2;
	for (i = 0; i < num; i ++)
	{
		nSize = 0;
		if (i == 0)
		{
			nIdx1 = num-1;
			nIdx2 = 0;
		}
		else
		{
			nIdx1 = i-1;
			nIdx2 = i;
		}

		if (is_a_line(pPolygon[nIdx1], pPolygon[nIdx2]))
		{
			for (j = 0; j < ptNum; j ++)
			{
				if (point_in_line(pPts[j], pPolygon[nIdx1], pPolygon[nIdx2]))
					pPoint[nSize ++] = pPts[j];
			}

			// �������ڵ�ǰ�߶��ϵĵ������Щ�����һ���˵�ľ�������
			for (j = 0; j < nSize; j ++)
				pDist[j] = get_distance(pPolygon[nIdx1], pPoint[j]);

			// ����ð�ݷ�����ͬʱ��ס������
			double dbTmp;
			POINT_3F ptTemp;
			for (j = 0; j < nSize; j ++)
			{
				for (k = j+1; k < nSize; k ++)
				{
					if (pDist[j] > pDist[k])
					{
						dbTmp = pDist[j];
						pDist[j] = pDist[k];
						pDist[k] = dbTmp;

						ptTemp = pPoint[j];
						pPoint[j] = pPoint[k];
						pPoint[k] = ptTemp;

					}
				}
			}

			// �����������ݿ���������������
			memcpy(pRetPts, pPoint, sizeof(POINT_3F) * nSize);
			pRetPts += nSize;
		}
	}

	// �����������ݿ���������������
	memcpy(pPts, pHead, sizeof(POINT_3F) * ptNum);

	delete [] pPoint;
	delete [] pDist;
	delete [] pRetPts;
}

// ���������Ľ������,�������������Ľ���
// ������ش﷨���󽻵�
// ����Ϊtrueʱ��ʾ�н��㣬������㣬��Ϊfalse,��û�н���
bool get_3faces_interpoint(float* fFace1, float* fFace2, float* fFace3, POINT_3F& pt)
{
	double fA, fA1, fA2, fA3;

	fA = 0;
	fA += fFace1[0]*(fFace2[1]*fFace3[2]-fFace2[2]*fFace3[1]);
	fA += fFace2[0]*(fFace1[2]*fFace3[1]-fFace1[1]*fFace3[2]);
	fA += fFace3[0]*(fFace1[1]*fFace2[2]-fFace1[2]*fFace2[1]);

	if (fabs(fA) < 0.000001)
		return false;

	fA1 = fA2 = fA3 = 0;

	fA1 += -fFace1[3]*(fFace2[1]*fFace3[2]-fFace2[2]*fFace3[1]);
	fA1 += -fFace2[3]*(fFace1[2]*fFace3[1]-fFace1[1]*fFace3[2]);
	fA1 += -fFace3[3]*(fFace1[1]*fFace2[2]-fFace1[2]*fFace2[1]);

	fA2 += -fFace1[3]*(fFace2[2]*fFace3[0]-fFace2[0]*fFace3[2]);
	fA2 += -fFace2[3]*(fFace1[0]*fFace3[2]-fFace1[2]*fFace3[0]);
	fA2 += -fFace3[3]*(fFace1[2]*fFace2[0]-fFace1[0]*fFace2[2]);

	fA3 += -fFace1[3]*(fFace2[0]*fFace3[1]-fFace2[1]*fFace3[0]);
	fA3 += -fFace2[3]*(fFace1[1]*fFace3[0]-fFace1[0]*fFace3[1]);
	fA3 += -fFace3[3]*(fFace1[0]*fFace2[1]-fFace1[1]*fFace2[0]);

	pt.x = fA1/fA;
	pt.y = fA2/fA;
	pt.z = fA3/fA;

	return true;
}

//	rotate_point() ����ռ��������������ת�������
//	p1:������㣬p2�������յ㣬rotateP������ת�㣬
//	sita:��ת�Ƕȣ�pAfterRotate:����ת����ת�������
int rotate_point(double *p1,double *p2,double *rotateP,double sita,double *pAftefRotate)
{
	int i;
	double vector[3];  //������p1-p2�����p1ƽ�Ƶ�ԭ��������
	double rotate[3];

	for(i = 0;i < 3; i ++)
	{
		vector[i] = p2[i]-p1[i];
		rotate[i] = rotateP[i]-p1[i];
	}
	// ������vector�ĳ���len
	double len = sqrt(vector[0]*vector[0]+vector[1]*vector[1]+vector[2]*vector[2]);

	if(len == 0)
	{
		return 0;
	}

	// ��λ������vector	
	for(i = 0; i < 3; i ++)
		vector[i]/= len;

	// ��λ����vector��yoz���ϵ�ͶӰ����lyz
	double lyz =  sqrt(vector[1]*vector[1]+vector[2]*vector[2]); 

	// ��vector��yoz����ͶӰΪ0��ʵ���Ͼ�����X����תsita��	
	if(lyz==0)
	{
		if(vector[0]<0.0) sita*= -1.0;
		pAftefRotate[0] = p1[0]+rotate[0];
		pAftefRotate[1] = p1[1]+rotate[1]*cos(sita)-rotate[2]*sin(sita);
		pAftefRotate[2] = p1[2]+rotate[1]*sin(sita)+rotate[2]*cos(sita);
		return 1;
	}

	// ��λ����vector��xoz���ϵ�ͶӰ����lxz
	double lxz=sqrt(vector[0]*vector[0]+vector[2]*vector[2]);
	// ��vector��xoz����ͶӰΪ0��ʵ���Ͼ�����Y����תsita��	
	if(lxz==0)
	{
		if(vector[1]<0.0) sita*= -1.0;
		pAftefRotate[0] = p1[0]+rotate[0]*cos(sita)+rotate[2]*sin(sita);
		pAftefRotate[1] = p1[1]+rotate[1];
		pAftefRotate[2] = p1[2]-rotate[0]*sin(sita)+rotate[2]*cos(sita);
		return 1;
	}

	// ��λ����vector��xoy���ϵ�ͶӰ����lxy
	double lxy=sqrt(vector[0]*vector[0]+vector[1]*vector[1]);
	// ��vector��xoy����ͶӰΪ0��ʵ���Ͼ�����Y����תsita��	
	if(lxy==0)
	{
		if(vector[2]<0.0) sita*= -1.0;
		pAftefRotate[0] = p1[0]+rotate[0]*cos(sita)-rotate[1]*sin(sita);
		pAftefRotate[1] = p1[1]+rotate[0]*sin(sita)+rotate[1]*cos(sita);
		pAftefRotate[2] = p1[2]+rotate[2];
		return 1;
	}
	
	// ��Z����ת����vector��YOZ�����ת����Rz
#define ROW 3
#define COL 3
#define LENGTH 9
	double Rz[LENGTH], Rx[LENGTH], nRz[LENGTH], nRx[LENGTH], R[LENGTH];

	Rz[0*COL+0] = vector[1]/lxy;
	Rz[0*COL+1] = vector[0]/lxy;
	Rz[0*COL+2] = 0.0;
	Rz[1*COL+0] = -1.0*vector[0]/lxy;
	Rz[1*COL+1] = vector[1]/lxy;
	Rz[1*COL+2] = 0.0;
	Rz[2*COL+0] = 0.0;
	Rz[2*COL+1] = 0.0;
	Rz[2*COL+2] = 1.0;

	// ������X����ת����vector��XOZ�����ת����Rx
	Rx[0*COL+0] = 1.0;
	Rx[0*COL+1] = 0.0;
	Rx[0*COL+2] = 0.0;
	Rx[1*COL+0] = 0.0;
	Rx[1*COL+1] = vector[2];
	Rx[1*COL+2] = lxy;
	Rx[2*COL+0] = 0.0;
	Rx[2*COL+1] = -1.0*lxy;
	Rx[2*COL+2] = vector[2];

	// ��P����Z����תsita�Ǻ����ת����R
	R[0*COL+0] = cos(sita);
	R[0*COL+1] = sin(sita);
	R[0*COL+2] = 0.0;
	R[1*COL+0] = -1.0*sin(sita);
	R[1*COL+1] = cos(sita);
	R[1*COL+2] = 0.0;
	R[2*COL+0] = 0.0;
	R[2*COL+1] = 0.0;
	R[2*COL+2] = 1.0;

	// Rx�������
	nRx[0*COL+0] = 1.0;
	nRx[0*COL+1] = 0.0;
	nRx[0*COL+2] = 0.0;
	nRx[1*COL+0] = 0.0;
	nRx[1*COL+1] = vector[2];
	nRx[1*COL+2] = -1.0*lxy;
	nRx[2*COL+0] = 0.0;
	nRx[2*COL+1] = lxy;
	nRx[2*COL+2] = vector[2];

	// Rz�������
	nRz[0*COL+0] = vector[1]/lxy;
	nRz[0*COL+1] = -1.0*vector[0]/lxy;
	nRz[0*COL+2] = 0.0;
	nRz[1*COL+0] = vector[0]/lxy;
	nRz[1*COL+1] = vector[1]/lxy;
	nRz[1*COL+2] = 0.0;
	nRz[2*COL+0] = 0.0;
	nRz[2*COL+1] = 0.0;
	nRz[2*COL+2] = 1.0;

	//	������ת����Rz*Rx*R*nRx*nRz;
	//	R1=Rz*Rx;
	double* R1 = MatrixMul(Rz,Rx,3,3,3);

	//	R2=R1*R=Rz*Rx*R
	double* R2 = MatrixMul(R1,R,3,3,3);

	//	R3=R2*nRx=Rz*Rx*R*nRx
	double* R3 = MatrixMul(R2,nRx,3,3,3);

	//	R4=R3*nRz=Rz*Rx*R*nRx*nRz
	double* R4 = MatrixMul(R3,nRz,3,3,3);

//	cout<<"RotateMatrix2 �� R4���£�"<<endl;
//	for(i=0;i<3;i++)
//	{	for(j=0;j<3;j++)
//			cout<<R4[i][j]<<"  ";
//		cout<<endl;
//	}
//		cout<<endl;

	//	rotateP��ƽ�ƺ�ĵ�λ����vector��ת�������
	double* rp=MatrixMul(rotate,R4,1,3,3);

	// *pAftefRotate���ص�rotateP������P1-P2��ת�������
	for(i=0;i<COL;i++)
		pAftefRotate[i]=rp[i]+p1[i];

	delete [] R1;
	delete [] R2;
	delete [] R3;
	delete [] R4;
	delete [] rp;

	return 1;
}

// ��������Բ�Ľ���
PCROSSPOINT poly_circle_section(const POINT2DF& ptCenter, float fRadius, const POINT2DF* pPolygon, int nVerNum)
{
	PCROSSPOINT pStart, p1, p2;
	double dbA, dbB, dbC;

	POINT2DF* pPoly = new POINT2DF [nVerNum];
	for (int i=0; i<nVerNum; i++)
	{
		pPoly[i].x = pPolygon[i].x-ptCenter.x;
		pPoly[i].y = pPolygon[i].y-ptCenter.y;
	}

	// �ж�delta=B*B-A*C;
	pStart = p1 = p2 = NULL;
	double dbDelta, dbT1, dbT2;
	for (i=0; i<nVerNum; i++)
	{
		dbA = (pPoly[(i+1)%nVerNum].x-pPoly[i].x)*(pPoly[(i+1)%nVerNum].x-pPoly[i].x)
			+(pPoly[(i+1)%nVerNum].y-pPoly[i].y)*(pPoly[(i+1)%nVerNum].y-pPoly[i].y);
		dbB = pPoly[i].x*(pPoly[(i+1)%nVerNum].x-pPoly[i].x)
			+pPoly[i].y*(pPoly[(i+1)%nVerNum].y-pPoly[i].y);
		dbC = pPoly[i].x*pPoly[i].x+pPoly[i].y*pPoly[i].y-fRadius*fRadius;

		dbDelta = dbB*dbB-dbA*dbC;
		if (dbDelta <= 0)
			continue;

		// dbT1<dbT2;
		dbT1 = (-dbB-sqrt(dbDelta))/dbA;
		dbT2 = (-dbB+sqrt(dbDelta))/dbA;

		if (dbT1 < 0)
		{
			if (dbT2 <= 0)
				continue;

			if (dbT2>0 && dbT2<=1)
			{
				p2 = new CROSSPOINT;
				p2->nS = 2;	// Բ�ڲ���
				p2->x  = pPoly[i].x;
				p2->y  = pPoly[i].y;

				if (pStart == NULL)
				{
					pStart = p2;
					p1 = p2;
				}
				else
				{
					p1->next = p2;
					p1 = p2;
				}

				p2 = new CROSSPOINT;
				p2->nS = 1;	// ���߽���
				p2->x  = pPoly[i].x+(pPoly[(i+1)%nVerNum].x-pPoly[i].x)*dbT2;
				p2->y  = pPoly[i].y+(pPoly[(i+1)%nVerNum].y-pPoly[i].y)*dbT2;

				p1->next = p2;
				p1 = p2;
			}
			else // if (dbT2 > 1)
			{
				p2 = new CROSSPOINT;
				p2->nS = 2;	// Բ�ڶ���
				p2->x  = pPoly[i].x;
				p2->y  = pPoly[i].y;

				if (pStart == NULL)
				{
					pStart = p2;
					p1 = p2;
				}
				else
				{
					p1->next = p2;
					p1 = p2;
				}
			}
		}
		else if (dbT1>=0 && dbT1<1)
		{
			if (dbT2>0 && dbT2<=1)
			{
				p2 = new CROSSPOINT;
				p2->nS = 0;	// ��߽���
				p2->x  = pPoly[i].x+(pPoly[(i+1)%nVerNum].x-pPoly[i].x)*dbT1;
				p2->y  = pPoly[i].y+(pPoly[(i+1)%nVerNum].y-pPoly[i].y)*dbT1;
				
				if (pStart == NULL)
				{
					pStart = p2;
					p1 = p2;
				}
				else
				{
					p1->next = p2;
					p1 = p2;
				}

				p2 = new CROSSPOINT;
				p2->nS = 1;	// ���߽���
				p2->x  = pPoly[i].x+(pPoly[(i+1)%nVerNum].x-pPoly[i].x)*dbT2;
				p2->y  = pPoly[i].y+(pPoly[(i+1)%nVerNum].y-pPoly[i].y)*dbT2;

				p1->next = p2;
				p1 = p2;
			}
			else if (dbT2>1)
			{
				p2 = new CROSSPOINT;
				p2->nS = 0;	// ��߽���
				p2->x  = pPoly[i].x+(pPoly[(i+1)%nVerNum].x-pPoly[i].x)*dbT1;
				p2->y  = pPoly[i].y+(pPoly[(i+1)%nVerNum].y-pPoly[i].y)*dbT1;
				
				if (pStart == NULL)
				{
					pStart = p2;
					p1 = p2;
				}
				else
				{
					p1->next = p2;
					p1 = p2;
				}
			}
		}
	}

	// ������ڽ���
	if (pStart != NULL)
		p1->next = NULL;

	p1 = pStart;
	while (p1 != NULL)
	{
		p1->x += ptCenter.x;
		p1->y += ptCenter.y;
		p1 = p1->next;
	}

	delete [] pPoly;
	return pStart;
}

// �жϵ��Ƿ��ڶ������
// ע�⣬���㷨����͹���������
/*
bool pt_in_polygon(const POINT2DF& pt, const POINT2DF* pPolygon, int nVerNum)
{
	int i, nNum=0;
	float fMark;

	nNum = 0;
	for (i=0; i<nVerNum-1; i++)
	{
		nNum += ((pPolygon[i].x==pPolygon[i+1].x) && (pPolygon[i].y==pPolygon[i+1].y));
	}
	if (nNum == nVerNum-1)
		return FALSE;

	nNum = 0;
	for (i=0; i<nVerNum-1; i++)
	{
		fMark = 0;
		fMark += pPolygon[i].x*pPolygon[i+1].y-pPolygon[i].y*pPolygon[i+1].x;
		fMark += pPolygon[i+1].x*pt.y-pPolygon[i+1].y*pt.x;
		fMark += pt.x*pPolygon[i].y-pt.y*pPolygon[i].x;
		nNum += (fMark>=0)?1:-1;
	}

	fMark += pPolygon[i].x*pPolygon[0].y-pPolygon[i].y*pPolygon[0].x;
	fMark += pPolygon[0].x*pt.y-pPolygon[0].y*pt.x;
	fMark += pt.x*pPolygon[i].y-pt.y*pPolygon[i].x;
	nNum += (fMark>=0)?1:-1;

	nNum = (nNum>0)?nNum:-nNum;
	if (nNum == nVerNum)
		return TRUE;
	else
		return FALSE;
}
*/

bool pt_in_polygon(const POINT2DF& pt, const POINT2DF* pPolygon, int nVerNum)
{   
	try
	{   
		long i, j;
		bool bIsInside = false; 
		
		for (i=0, j=nVerNum-1; i<nVerNum; j=i++)     
		{   
			if ((((pPolygon[i].y<=pt.y) && (pt.y<pPolygon[j].y)) || ((pPolygon[j].y<=pt.y) && (pt.y<pPolygon[i].y)))   
				&& (pt.x<(pPolygon[j].x-pPolygon[i].x)*(pt.y-pPolygon[i].y)/(pPolygon[j].y-pPolygon[i].y)+pPolygon[i].x))   
			{   
				bIsInside =  !bIsInside;   
			}   
		}
		return bIsInside;   
	}   
	catch(...)   
	{   
		throw   CString("An   unspecified   error   occurred");   
	}   
}

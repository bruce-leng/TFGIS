// TunArc.cpp: implementation of the CTilTunArc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "canvas.h"
#include "TilTunArc.h"

#include "..\TilCanvas.h"
#include "GraphDefine.h"
#include "MathComput.h"
#include <float.h>
#include "Polygon.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define PI 3.14159265358979323846 

// ��cad�У�����lwpolyline����ʼ��BeginPoint��
// ��ֹ��EndPoint����
void lwpolylineToArc(const POINT2DF& BeginPoint,const POINT2DF& EndPoint,
					 double u,POINT2DF &CenterPoint)
{
	double centerAngle;//����
	centerAngle=4*atan(fabs(u));
	double x1,x2,y1,y2;//Բ����ʼ�����ֹ��
	x1=BeginPoint.x;
	x2=EndPoint.x;
	y1=BeginPoint.y;
	y2=EndPoint.y;
	double L; //�ҳ�
	L=sqrt(pow((x1-x2),2)+pow((y1-y2),2)); 
	
	double R;//Բ���뾶
	R=0.5*L/sin(0.5*centerAngle);
	//��֪Բ������Ͱ뾶����Բ������
	double h;//Բ�ĵ��ҵľ���
	h=sqrt(R*R-L*L/4);
	double xc,yc;//Բ������
	double xa,ya; //��ʼ�����ֹ�����ߵ��е��������
	xa=0.5*(x1+x2);
	ya=0.5*(y1+y2);
	//�ҵķ���ǣ�0-2PI֮��
	double angle;//��㵽�յ����������x������֮�����б��
	angle=acos((x2-x1)/sqrt(pow(x2-x1,2)+pow(y2-y1,2)));
	double amass; //��������X������λ�����Ĳ��
	amass = y2-y1;//�ɣ���(x2-x1)*0+1*(y2-y1)���õ�
	
	if (amass<0)
	{
		angle=-angle;
		angle=2*PI+angle;
	}

	double DirectionAngle;//���е㵽Բ�ĵ�ֱ�������ķ���ǣ�0-2PI֮�䣩
	if ((u>0 && centerAngle<PI)||(u<0 && centerAngle>PI))
		DirectionAngle=angle+PI/2;
	if((u<0 && centerAngle<PI)||(u>0 && centerAngle>PI))
		DirectionAngle=angle-PI/2;
	if (DirectionAngle>2*PI)
		DirectionAngle= DirectionAngle-2*PI;
	double d;//Բ�ĵ��ҵľ���
	d=sqrt(R*R-L*L/4);
	if (fabs(DirectionAngle)<0.0000001)
	{
		xc=xa+d;
		yc=ya;
    }
	else if(fabs(DirectionAngle-PI/2)<0.0000001)
	{
		xc=xa;
		yc=ya+d;
	}
	else if (fabs(DirectionAngle-PI)<0.0000001)
	{
		xc=xa-d;
		yc=xa;
	}
	else if (fabs(DirectionAngle-(PI+PI/2))<0.0000001)
	{
		xc=xa;
		yc=xa-d;
	}
	else
	{
		double nslope,k;//nslope Ϊ�ҵ�б�ʣ�KΪ���д��ߵ�б��
		double nAngle;//�д��ߵ���б�ǣ�
		double X,Y; //Բ������������ĵ������ƫ����
		
		nslope = (y2 - y1) / (x2-x1);
		k = -1 / nslope;
		nAngle = atan(k) ;
		X = cos(nAngle) * d;
		Y = sin(nAngle) * d;
		if (DirectionAngle > PI / 2 && DirectionAngle < PI )
		{
			X = -X;
			Y = -Y;
		}
		if (DirectionAngle > PI && DirectionAngle < (PI + PI / 2) )
		{
			X = -X;
			Y = -Y;
		}
		xc=xa + X;
		yc=ya+ Y;
	}

	CenterPoint.x=xc;
	CenterPoint.y=yc;
//	CenterPoint.z=0.0;
}

// ��͹�ȼнǣ��ԽǶȱ�ʾ
double convexAngle(const POINT2DF& pt1, const POINT2DF &pt2, const POINT2DF &ptCenter, double convex)
{
	double angle;

	// ��p1�ĽǶ�
	POINT2DF p1, p2;
	p1.x = pt1.x-ptCenter.x;
	p1.y = pt1.y-ptCenter.y;
	double dist1 = sqrt(p1.x*p1.x+p1.y*p1.y);
	double angle1 = acos(p1.x/dist1);
	// p1��3��4����ʱ
	if (p1.y<0 && fabs(p1.y)>0.0000001 )
		angle1 += PI;

	p2.x = pt2.x-ptCenter.x;
	p2.y = pt2.y-ptCenter.y;
	double dist2 = sqrt(p2.x*p2.x+p2.y*p2.y);
	double angle2 = acos(p2.x/dist2);
	// p2��3��4����ʱ
	if (p2.y<0 && fabs(p2.y)>0.0000001)
		angle2 += PI;

	// ͹��ֵΪ����ʾ˳ʱ��Բ����͹��ֵΪ����ʾ��ʱ��Բ��
	if (convex<0 && fabs(convex)>0.0000001)
	{
		if (angle1>angle2)
			angle = angle1-angle2;
		else
			angle = angle1+PI-angle2;
	}

	return angle/PI*180;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTilTunArc::CTilTunArc()
{
}

CTilTunArc::~CTilTunArc()
{
	ClearMem();
}

/*
// ������ߴ縴�Ƶ���ǰ�����
void CTilTunArc::Copy(CTilTunArc& tunArc)
{
	m_ptImgCenter = tunArc.m_ptImgCenter;
	vector<PTUNARC_UNIT>* pTunUnits = tunArc.GetTunArcUnits();
	for (int i=0; i<pTunUnits->size(); i++)
		m_vTAUnits.push_back((*pTunUnits)[i]);
}

void CTilTunArc::ReadTunArc(CString strPathName, CTilCanvas* pCanvas)
{
	ASSERT(pCanvas->IsValide());

	CStdioFile file;
	if (!file.Open(strPathName, CFile::modeRead))
	{
		AfxMessageBox("��������ļ��򿪴���!");
		return;
	}

	CString strLine;

	// ��һ��Ϊ�ڵ���
// 	file.ReadString(strLine);
// 	int nNum = atoi(strLine);

	// ��������������÷�Χ����ʱû������һ����
	//////////////////////////////////////////////////////////////////////////
	CPolygon* pTunArc = (CPolygon*)pCanvas->GetGraph()->GetGraphByIdx(GRAPH_LAYER_TUNARC)->GetUnitByIdx(0);

	pTunArc->m_pPointList->RemoveAll();

	POINT2DF pt1, pt2, ptTunCenter, ptCenter;

	// ��ȡ������������ĵ�ƽ������
	file.ReadString(strLine);
	get_coord(strLine, ptTunCenter);
	
	// ��ȡ��2�У�����Ϊ������
	file.ReadString(strLine);
	get_coord(strLine, pt1);
	pTunArc->m_pPointList->Add(pt1);

	while (file.ReadString(strLine))
	{
		// ������(ֱ�߻�����)
		int nType = atoi(strLine);

		file.ReadString(strLine);

		// ��Ϊֱ�ߣ�������һ������
		if (nType == Graph_Line)
		{
			get_coord(strLine, pt2);
			pTunArc->m_pPointList->Add(pt2);
		}
		else // ��Ϊ���ߣ�������ڶ����˵�(nType == Graph_RoundArc)
		{
			// ��ȡԲ�����꼰Բ���Ƕ�
			get_coord(strLine, ptCenter);
			file.ReadString(strLine);
			double dbAngle = atof(strLine)/180*PI;
//			get_arc_point(pt1, ptCenter, dbAngle, pt2);

			// ��뾶�ͻ���
			double dbRadius = get_distance(pt1, ptCenter);

#define AVE_ARC_LEN 1250	// Բ���뾶���ڹ涨ֵʱ��ƽ�������ָ�Բ��
#define RADIUS		5000	// ��ƽ�������ָ�Բ��ʱ����СԲ���뾶
#define AVE_ANGLE	0.25	// Բ���뾶С�ڹ涨ֵʱ��ƽ���Ƕȷָ�Բ��

			// ��Բ���ֶΣ������ζ�����Ϊ����ζ��㱣��
			// ��Բ���ֳɵĶ�����ÿ�εĻ���
			int nSegNum;
			double dbSegAngle;// dbLength
			// Բ���뾶С�ڹ涨ֵʱ��ƽ���Ƕȷָ�Բ��
			if (dbRadius <= RADIUS)
			{
				nSegNum = int(dbAngle/AVE_ANGLE+0.5);
				dbSegAngle = dbAngle/nSegNum;
			}
			else // Բ���뾶���ڹ涨ֵʱ��ƽ�������ָ�Բ��
			{
				double dbLength  = dbRadius*dbAngle;
				nSegNum = int(dbLength/AVE_ARC_LEN+0.5);
				dbSegAngle = dbAngle/nSegNum;
			}

			// ���ϸ��㲢��Ϊ����ζ���ѹ���ջ
			for (int i=0; i<nSegNum-1; i++)
			{
				POINT2DF ptTmp;
				get_arc_point(pt1, ptCenter, dbSegAngle, ptTmp);
				pTunArc->m_pPointList->Add(ptTmp);
				pt1 = ptTmp;
			}

			file.ReadString(strLine);
			get_coord(strLine, pt2);
			pTunArc->m_pPointList->Add(pt2);
		}

		// ��pt1��Ϊ��һ���ߵ���ʼ��
		pt1 = pt2;
	}

	// ���һ�㼴Ϊ��һ�㣬����ɾ��
	pTunArc->m_pPointList->RemoveAt(pTunArc->m_pPointList->GetSize()-1);

	//////////////////////////////////////////////////////////////////////////
	// ��x,y���������С��
	POINT2DF ptTmp = pTunArc->m_pPointList->GetAt(0);
	double dbMinX, dbMinY, dbMaxX, dbMaxY;
	dbMinX = dbMaxX = ptTmp.x;
	dbMinY = dbMaxY = ptTmp.y;
	for (int i=1; i<pTunArc->m_pPointList->GetSize(); i++)
	{
		POINT2DF ptTmp = pTunArc->m_pPointList->GetAt(i);
		if (dbMinX > ptTmp.x)	dbMinX = ptTmp.x;
		if (dbMaxX < ptTmp.x)	dbMaxX = ptTmp.x;

		if (dbMinY > ptTmp.y)	dbMinY = ptTmp.y;
		if (dbMaxY < ptTmp.y)	dbMaxY = ptTmp.y;
	}

	// ��y���귴�򲢰�������С,ͬʱ���Һ�����ƽ��10����
	double dbY = dbMaxY-dbMinY;
	for (i=0; i<pTunArc->m_pPointList->GetSize(); i++)
	{
		POINT2DF ptTmp = pTunArc->m_pPointList->GetAt(i);
		ptTmp.x = (ptTmp.x-dbMinX)*TunArc_Scale+TunArc_Extend;
		ptTmp.y = (dbY-(ptTmp.y-dbMinY))*TunArc_Scale+TunArc_Extend;
		pTunArc->m_pPointList->SetAt(i, ptTmp);
	}

	// �������ĵ�����
	ptTunCenter.x = (ptTunCenter.x-dbMinX)*TunArc_Scale+TunArc_Extend;
	ptTunCenter.y = (dbY-(ptTunCenter.y-dbMinY))*TunArc_Scale+TunArc_Extend;
	pCanvas->SetCenterImgCoord(ptTunCenter);
	//////////////////////////////////////////////////////////////////////////

	file.Close();
}

void CTilTunArc::get_coord(CString strLine, POINT2DF& pt)
{
	int i;
	CString strTmp;

	// ��x����
	strLine.TrimLeft();
	int nLength = strLine.GetLength();
	for (i=0; i<nLength; i++)
	{
		if (strLine.GetAt(i) == ' ')
		{
			strTmp = strLine.Left(i+1);
			pt.x = atof(strTmp);
			strTmp = strLine.Right(nLength-i-1);
			break;
		}
	}

	// ��y����
	strLine = strTmp;
	strLine.TrimLeft();
	pt.y = atof(strTmp);
// 	nLength = strLine.GetLength();
// 	for (i=0; i<nLength; i++)
// 	{
// 		if (strLine.GetAt(i) == ' ')
// 		{
// 			strTmp = strLine.Left(i+1);
// 			pt.y = atof(strTmp);
// 			// strTmp = strLine.Right(nLength-i-1);
// 			break;
// 		}
// 	}
}
*/

// ��ȡ�������ߴ粢�����Ա������
BOOL CTilTunArc::Import(CString strPathTunArc)
{
	/*
	int i;
	CStdioFile file;
	if (!file.Open(strPathTunArc, CFile::modeRead))
	{
		AfxMessageBox("��������ļ��򿪴���!");
		return FALSE;
	}

	m_ptImgCenter.x = m_ptImgCenter.y = 0;
	ClearMem();

	// ��������ļ����Ѿ����������������ߴ���Ϣ
	int nType;
	CString strLine;
	POINT2DF pt, pt1, pt2, ptTunCenter, ptCenter;

	//////////////////////////////////////////////////////////////////////////
	// ��ȡ������������ĵ�ƽ������
	file.ReadString(strLine);
	get_coord(strLine, ptTunCenter);

	// ��ȡ��2�У�����Ϊ������
	file.ReadString(strLine);
	get_coord(strLine, pt1);

	while (file.ReadString(strLine))
	{
		pt.x = pt1.x-ptTunCenter.x;
		pt.y = pt1.y-ptTunCenter.y;

		TUNARC_UNIT tunArcUnit;
		tunArcUnit.pt1.x = pt.x*TunArc_Scale;
		tunArcUnit.pt1.y = pt.y*TunArc_Scale;

		// ������(ֱ�߻�����)
		nType = atoi(strLine);
		tunArcUnit.nType = nType;

		// ��Ϊ���ߣ�������ڶ����˵�(nType == Graph_RoundArc)
		if (nType == Graph_RoundArc)
		{
			// ��ȡԲ������
			file.ReadString(strLine);
			get_coord(strLine, ptCenter);
			pt.x = ptCenter.x-ptTunCenter.x;
			pt.y = ptCenter.y-ptTunCenter.y;
			tunArcUnit.ptCenter.x = pt.x*TunArc_Scale;
			tunArcUnit.ptCenter.y = pt.y*TunArc_Scale;

			// ��ȡ����
			file.ReadString(strLine);
			double dbAngle = atof(strLine)/180*PI;
			tunArcUnit.angle = dbAngle;
		}

		// ��ȡ��2���˵�
		file.ReadString(strLine);
		get_coord(strLine, pt2);
		pt.x = pt2.x-ptTunCenter.x;
		pt.y = pt2.y-ptTunCenter.y;
		tunArcUnit.pt2.x = pt.x*TunArc_Scale;
		tunArcUnit.pt2.y = pt.y*TunArc_Scale;

		m_vTAUnits.push_back(tunArcUnit);

		// ��pt1��Ϊ��һ���ߵ���ʼ��
		pt1 = pt2;
	}
	*/

	if (!ReadDXF(strPathTunArc))
	{
		AfxMessageBox("��������ļ��򿪴���!");
		return FALSE;
	}

//	Test();

	// �Է�Polyline��ͼԪ����˳ʱ������
	if (!clockwisePoly(TRUE))
	{
		AfxMessageBox("��ֻ���������ķ�տ���������cadͼ��!", MB_OK|MB_ICONINFORMATION);
	}
	// ��ͼԪ��Ϊ��ԭ�����ĶԳƵ�ͼԪ
	changeAxisDir(-1,-1);
	// ����������
	SetZoom(TunArc_Scale*2);

	// ���������������½�����ΪͼԪ��ԭ��
	CUnit* pTunArc = new CPolygon;
	Transferto(pTunArc);
	CRect rect = pTunArc->GetPolyRect();
	delete pTunArc;
	POINT2DF ptOrign(rect.left-TunArc_Extend, rect.top-TunArc_Extend);

	// �����ĵ�����Ϊԭ��
	resetOrign(ptOrign);

	//////////////////////////////////////////////////////////////////////////

	//m_ptImgCenter.x = rect.Width()/2+TunArc_Extend;
	//m_ptImgCenter.y = rect.bottom+TunArc_Extend;

	/*
	// �����йؼ�����x�ᷭת180�ȱ���
	for (int i=0; i<m_vTAUnits.size(); i++)
	{
		m_vTAUnits[i]->pt1.x *= -1;
		m_vTAUnits[i]->pt1.y *= -1;

		m_vTAUnits[i]->ptCenter.x *= -1;
		m_vTAUnits[i]->ptCenter.y *= -1;

		m_vTAUnits[i]->pt2.x *= -1;
		m_vTAUnits[i]->pt2.y *= -1;
	}
	*/
	//////////////////////////////////////////////////////////////////////////

	return TRUE;
}

// ��ȡ�ʹ洢�������ߴ�����
BOOL CTilTunArc::Serialize(CArchive& ar)
{
	// ��ȡ�������ߴ�����
	if (ar.IsLoading())
	{
		POINT2DF pt, ptStart;
		int nType;

		// ��������ļ����Ѿ����������������ߴ���Ϣ
		int nIsTunArcSaved;
		ar >> nIsTunArcSaved;
		if (nIsTunArcSaved != Graph_TunArc_Save)
		{
			TRACE("û�б�����������ߴ�!\n");
			return FALSE;
		}

		// �ȶ�ȡ����������ĵ���ͼ���е�����
		ar >> m_ptImgCenter.x >> m_ptImgCenter.y;

		// ��ȡͼԪ��
		int nSize;
		ar >> nSize;

		// ��ȡ�����ϵ�1�����꼰ͼԪ����
		ar >> pt.x >> pt.y;

		ptStart = pt;

		// ���ͼԪ���Ͳ���Graph_Unit_Save�����������ߴ绹δ��ȡ���
		for (int i=0; i<nSize; i++)
		{
			// ��ȡ��2�У�����Ϊ������
			PTUNARC_UNIT pArcUnit = new TUNARC_UNIT;
			pArcUnit->pt1 = pt;

			// ��ȡͼԪ����
			ar >> nType;
			pArcUnit->nType = nType;

			// ��ΪԲ�������ȡԲ�����꼰����
			if (nType == Graph_RoundArc)
			{
				// ��ȡԲ������
				ar >> pArcUnit->ptCenter.x >> pArcUnit->ptCenter.y;

				// ��ȡԲ������
				ar >> pArcUnit->angle;
				// ������ת���ɽǶ�
				//pArcUnit->angle = pArcUnit->angle/PI*180;
			}

			// ��ȡ��һ�˵�����
			ar >> pArcUnit->pt2.x >> pArcUnit->pt2.y;

			m_vTAUnits.push_back(pArcUnit);

			// ��pt2��Ϊ��ͼԪ����ʼ��
			pt = pArcUnit->pt2;
		}

		return TRUE;
	}
	// �����������ߴ�����
	else
	{
		// ���û�ж���ߴ����ݣ��򲻱���
		if (m_vTAUnits.empty())
		{
			ar << Graph_Unit_Save;
			return FALSE;
		}

		// ��������ļ����Ѿ����������������ߴ���Ϣ
		ar << Graph_TunArc_Save;
		ar << m_ptImgCenter.x << m_ptImgCenter.y;

		// ����ͼԪ��
		int nSize = m_vTAUnits.size();
		ar << nSize;

		POINT2DF pt = m_vTAUnits[0]->pt1;
		ar << m_vTAUnits[0]->pt1.x << m_vTAUnits[0]->pt1.y;
		for (int i=0; i<nSize; i++)
		{
			ar << m_vTAUnits[i]->nType;

			if (m_vTAUnits[i]->nType == Graph_RoundArc)
			{
				ar << m_vTAUnits[i]->ptCenter.x << m_vTAUnits[i]->ptCenter.y;
				ar << m_vTAUnits[i]->angle;
			}

			ar << m_vTAUnits[i]->pt2.x << m_vTAUnits[i]->pt2.y;
		}

		return TRUE;
	}
}

/*
// ��ͼ�β��ļ��ж�ȡ������β�����洢�ڳ�Ա������
BOOL CTilTunArc::Read(CFile& file)
{
	POINT2DF pt;
	int nType;

	file.SeekToBegin();
	CArchive ar(&file, CArchive::load);

	// ��������ļ����Ѿ����������������ߴ���Ϣ
	int nIsTunArcSaved;
	ar >> nIsTunArcSaved;
	if (nIsTunArcSaved != Graph_TunArc_Save)
	{
		TRACE("û�б�����������ߴ�!\n");
		return FALSE;
	}

	// �ȶ�ȡ����������ĵ���ͼ���е�����
	ar >> m_ptImgCenter.x >> m_ptImgCenter.y;

	// ��ȡ�����ϵ�1�����꼰ͼԪ����
	ar >> pt.x >> pt.y >> nType;

	// ���ͼԪ���Ͳ���Graph_Unit_Save�����������ߴ绹δ��ȡ���
	while (nType != Graph_Unit_Save)
	{
		// ��ȡ��2�У�����Ϊ������
		TUNARC_UNIT tunArcUnit;
		tunArcUnit.pt1 = pt;
		tunArcUnit.nType = nType;

		// ��ΪԲ�������ȡԲ�����꼰����
		if (nType == Graph_RoundArc)
		{
			// ��ȡԲ������
			ar >> tunArcUnit.ptCenter.x >> tunArcUnit.ptCenter.y;

			// ��ȡԲ������
			ar >> tunArcUnit.angle;
		}

		// ��ȡ��һ�˵�����
		ar >> tunArcUnit.pt2.x >> tunArcUnit.pt2.y;

		m_vTAUnits.push_back(tunArcUnit);

		// ��pt2��Ϊ��ͼԪ����ʼ��
		pt = tunArcUnit.pt2;
	}

	ar.Close();

	return TRUE;
}

// �����������
void CTilTunArc::Save(CFile& file)
{
	// �ӿ�ʼ����ȡ
	file.SeekToBegin();

	CArchive ar(&file, CArchive::store);

	// ��������ļ����Ѿ����������������ߴ���Ϣ
	ar << Graph_TunArc_Save;
	ar << m_ptImgCenter.x << m_ptImgCenter.y;

	ar << m_vTAUnits[0].pt1.x << m_vTAUnits[0].pt1.y;
	for (int i=0; i<m_vTAUnits.size(); i++)
	{
		ar << m_vTAUnits[i].nType;

		if (m_vTAUnits[i].nType == Graph_RoundArc)
		{
			ar << m_vTAUnits[i].ptCenter.x << m_vTAUnits[i].ptCenter.y;
			ar << m_vTAUnits[i].angle;
		}

		ar << m_vTAUnits[i].pt2.x << m_vTAUnits[i].pt2.y;
	}

	ar.Close();
}
*/

/* ==================================================================
 * �������ƣ�  CTilTunArc::TransferPolygon()
 * ���ߣ�      [Leng]
 * ������:     [2009-3-27 11:00:13]
 * ����ֵ��    [BOOL] -
 * �����б� 
               [float fAveArcLenT] - [In] // Բ���뾶���ڹ涨ֵʱ��ƽ�������ָ�Բ��
               [ float fRadiusT] - [In]	  // ��ƽ�������ָ�Բ��ʱ����СԲ���뾶
               [ float fAveAngleT] - [In] // Բ���뾶С�ڹ涨ֵʱ��ƽ�����ȷָ�Բ��
               [ CUnit* pTunArc] - [In]   // ����������ߴ�
ע�� :	
				
==================================================================*/
BOOL CTilTunArc::Transferto(CUnit* pTunArc, float fAveArcLenT,
								 float fRadiusT, float fAveAngleT)
{
	if (m_vTAUnits.empty())
		return FALSE;

	if (pTunArc->m_pPointList!=NULL)
		pTunArc->m_pPointList->RemoveAll();

	POINT2DF pt1, ptCenter;
	double dbRadius;
	float fAngle;

//	TRACE("/---------------------ת���ɶ����---------------------/\n");
	int i, nSize;
	nSize = m_vTAUnits.size();
	for (i=0; i<nSize; i++)
	{
		pt1 = m_vTAUnits[i]->pt1;//+m_ptImgCenter;
		pTunArc->m_pPointList->Add(pt1);
//		TRACE("Point[%f,%f]\n",pt1.x,pt1.y);

		if (m_vTAUnits[i]->nType == Graph_RoundArc)
		{
			// ��뾶�ͻ���
			ptCenter = m_vTAUnits[i]->ptCenter;//+m_ptImgCenter;
			dbRadius = get_distance(pt1, ptCenter);

			// ��Բ���ֶΣ������ζ�����Ϊ����ζ��㱣��
			// ��Բ���ֳɵĶ�����ÿ�εĻ���
			fAngle = m_vTAUnits[i]->angle/180*PI;

			int nSegNum;
			double dbSegAngle/*, dbLength*/;
			// Բ���뾶С�ڹ涨ֵʱ��ƽ���Ƕȷָ�Բ��
			if (dbRadius <= fRadiusT)
			{
				nSegNum = int(fAngle/fAveAngleT+0.5);
				dbSegAngle = fAngle/nSegNum;
			}
			else // Բ���뾶���ڹ涨ֵʱ��ƽ�������ָ�Բ��
			{
				double dbLength  = dbRadius*fAngle;
				nSegNum = int(dbLength/fAveArcLenT+0.5);
				dbSegAngle = fAngle/nSegNum;
			}

			// ���ϸ��㲢��Ϊ����ζ���ѹ���ջ
			for (int i=0; i<nSegNum-1; i++)
			{
				POINT2DF ptTmp;
				get_arc_point(pt1, ptCenter, dbSegAngle, ptTmp);
				pTunArc->m_pPointList->Add(ptTmp);
				pt1 = ptTmp;
//				TRACE("Point[%f,%f]\n",pt1.x,pt1.y);
			}
		}
	}

	return TRUE;
}

void CTilTunArc::operator = (CTilTunArc& tunArc)
{
	int nSize = tunArc.m_vTAUnits.size();
	for (int i=0; i<nSize; i++)
	{
		PTUNARC_UNIT pUnit = new TUNARC_UNIT;
		*pUnit = *(tunArc.m_vTAUnits[i]);
		m_vTAUnits.push_back(pUnit);
	}
}

// �������������ƶ����
// dX: ˮƽ�����ƶ���λ��
// dY: ��ֱ�����ƶ���λ��
void CTilTunArc::Move(double dX, double dY)
{
	// ͼ��Բ������ĵ���
	m_ptImgCenter.x += dX;
	m_ptImgCenter.y += dY;

	int nSize = m_vTAUnits.size();
	for (int i=0; i<nSize; i++)
	{
		// �ı������˵������
		m_vTAUnits[i]->pt1.x += dX;
		m_vTAUnits[i]->pt1.y += dY;
		m_vTAUnits[i]->pt2.x += dX;
		m_vTAUnits[i]->pt2.y += dY;

		if (m_vTAUnits[i]->nType == Graph_RoundArc)
		{
			m_vTAUnits[i]->ptCenter.x += dX;
			m_vTAUnits[i]->ptCenter.y += dY;
		}
	}
}
	
// ��ȡdxf�ļ���ֻ�������еĶ���ߡ�Բ����ֱ�ߵ�ͼԪ
BOOL CTilTunArc::ReadDXF(const CString& filename)
{
	ClearMem();

	CFileException ex;
	if (!m_File.Open(filename, CFile::modeRead,&ex))
	{
		#ifdef _DEBUG
		  afxDump << "File could not be opened " << ex.m_cause << "\n";
		#endif
		AfxMessageBox("Unable to open file for reading");
		return FALSE;
	}

	CString strLine;
	try
	{
		// ����ENTITIES��
		while (m_File.ReadString(strLine))
		{
			if (strLine == "ENTITIES")
				break;
		}

		while (m_File.ReadString(strLine))
		{
			if (strLine == "AcDbPolyline")	// �����
			{
				ReadPolyline();
				m_bIsPoly = TRUE;
			}
			else if (strLine == "AcDbCircle")	// Բ��
			{
				ReadArc();
				m_bIsPoly = FALSE;
			}
			else if (strLine == "AcDbLine")		// �߶�
			{
				ReadLine();
				m_bIsPoly = FALSE;
			}
		}
	}
	catch (...)
	{
		throw  CString("�ļ���ȡ����!"); 
	}

	m_File.Close();

	return TRUE;
}

// ��ȡ�����ߣ���AcDbPolyline����һ�п�ʼ
void CTilTunArc::ReadPolyline()
{
	CString strLine;

	m_File.ReadString(strLine);	// 90
	m_File.ReadString(strLine);	// ������
	int nVerNum = atoi(strLine);

	m_File.ReadString(strLine); // 70
	m_File.ReadString(strLine); // ����߱�־����λ���룩��Ĭ��ֵΪ 0��1 = �رգ�128 = Plinegen
	int nMark=atoi(strLine);

	m_File.ReadString(strLine); // 43
	m_File.ReadString(strLine); // �̶����

	// 38:��ߣ�39:���
	while (m_File.ReadString(strLine))
	{
		if (atoi(strLine)==38 || atoi(strLine)==39)
			m_File.ReadString(strLine);
		else // 10
			break;
	}

	POINT2DF pt1,pt2,ptCenter;// ���㡢�е�����
	double convex; // ͹��
	double angle;  // ͹�ȼн�
	m_File.ReadString(strLine); // x
	pt1.x = atof(strLine);
	m_File.ReadString(strLine); // 20
	m_File.ReadString(strLine); // y
	pt1.y = atof(strLine);

	m_File.ReadString(strLine); // 10
	while (atoi(strLine)==10 || atoi(strLine)==42)
	{
		if (atoi(strLine)==10)
		{
			m_File.ReadString(strLine); // x
			pt2.x = atof(strLine);
			
			m_File.ReadString(strLine); // 20
			m_File.ReadString(strLine); // y
			pt2.y = atof(strLine);
		
			// ����ֱ��ͼԪ
			PTUNARC_UNIT pLine = new TUNARC_UNIT;
			pLine->pt1 = pt1;
			pLine->pt2 = pt2;
			
			pLine->nType = Graph_Line;
			m_vTAUnits.push_back(pLine);
			
			pt1 = pt2;
			m_File.ReadString(strLine);
			continue;
		}
		else if (atoi(strLine)==42)
		{
			// ����͹�ȼ���Բ������
			m_File.ReadString(strLine);
			convex = atof(strLine);

			m_File.ReadString(strLine);	// 10
			m_File.ReadString(strLine); // x
			pt2.x = atof(strLine);
			
			m_File.ReadString(strLine); // 20
			m_File.ReadString(strLine); // y
			pt2.y = atof(strLine);

			lwpolylineToArc(pt1,pt2,convex,ptCenter);
			// ����͹�ȼ���Բ���нǽǶ�
			angle = convexAngle(pt1,pt2,ptCenter,convex);

			// ����Բ��ͼԪ
			PTUNARC_UNIT pArc = new TUNARC_UNIT;
			pArc->pt1 = pt1;
			pArc->pt2 = pt2;
			pArc->ptCenter = ptCenter;
			pArc->angle = angle;

			pArc->nType =Graph_RoundArc;
			m_vTAUnits.push_back(pArc);

			pt1 = pt2;
			m_File.ReadString(strLine);
			continue;
		}
		else
			break;
	}

	// ���nMark==1,��˵���Ƿ������,Ҫ�������һ��
	if(nMark==1)
	{
		// ����ֱ��ͼԪ
		PTUNARC_UNIT pLine = new TUNARC_UNIT;
		pLine->pt1 = pt1;
		pLine->pt2 = m_vTAUnits[0]->pt1;
		
		pLine->nType = Graph_Line;
		m_vTAUnits.push_back(pLine);
	}
}

// ��ȡ�߶Σ���AcDbLine����һ�п�ʼ
void CTilTunArc::ReadLine()
{
	CString strLine;
	PTUNARC_UNIT pLine = new TUNARC_UNIT;

	// ��ȡx����
	while (m_File.ReadString(strLine))
	{
		if (atoi(strLine) != 10)
		{
			m_File.ReadString(strLine);
		}
		else
			break;
	}
	m_File.ReadString(strLine);	// x1����
	pLine->pt1.x = atof(strLine);

	m_File.ReadString(strLine);	//20
	m_File.ReadString(strLine);	// y1����
	pLine->pt1.y = atof(strLine);

	m_File.ReadString(strLine); //30
	m_File.ReadString(strLine);	// z1����
//	pLine->pt1.z = atof(strLine);

	m_File.ReadString(strLine);	//11
	m_File.ReadString(strLine);	// x2����
	pLine->pt2.x = atof(strLine);

	m_File.ReadString(strLine);	//21
	m_File.ReadString(strLine);	// y2����
	pLine->pt2.y = atof(strLine);

	m_File.ReadString(strLine); //31
	m_File.ReadString(strLine);	// z2����
//	pLine->pt2.z = atof(strLine);

	pLine->nType =Graph_Line;
	m_vTAUnits.push_back(pLine);
}

// ��ȡԲ����,��AcDbCircle����һ�п�ʼ
void CTilTunArc::ReadArc()
{
	CString strLine;
	PTUNARC_UNIT pArc = new TUNARC_UNIT;

	// ��ȡ���ĵ�����
	while (m_File.ReadString(strLine))
	{
		if (atoi(strLine) != 10)
		{
			m_File.ReadString(strLine);
		}
		else
			break;
	}
	m_File.ReadString(strLine);	// x����
	pArc->ptCenter.x = atof(strLine);

	m_File.ReadString(strLine);	//20
	m_File.ReadString(strLine);	// y����
	pArc->ptCenter.y = atof(strLine);

	m_File.ReadString(strLine); //30
	m_File.ReadString(strLine);	// z����
//	pArc->ptCenter.z = atof(strLine);

	// �뾶
	m_File.ReadString(strLine); //40
	m_File.ReadString(strLine);	// �뾶
	double radius = atof(strLine);

	m_File.ReadString(strLine);	// 100
	m_File.ReadString(strLine);	// AcDbArc:������

	// ��ʼ����ֹ�Ƕ�,����ʱ�뷽����ת
	m_File.ReadString(strLine);	//50
	m_File.ReadString(strLine);	// ���Ƕ�
	double angle1 = atof(strLine);

	// �������Ƕȵ������ʼ�˵�����
	POINT2DF pt1,pt2;
	pt1.x = pArc->ptCenter.x+radius*cos(angle1/180*PI);
	pt1.y = pArc->ptCenter.y+radius*sin(angle1/180*PI);
	pArc->pt1 = pt1;

	m_File.ReadString(strLine);	//51
	m_File.ReadString(strLine);	// �˵�Ƕ�
	double angle2 = atof(strLine);

	// �����յ�Ƕȵ������ֹ�˵�����
	pt2.x = pArc->ptCenter.x+radius*cos(angle2/180*PI);
	pt2.y = pArc->ptCenter.y+radius*sin(angle2/180*PI);
	pArc->pt2 = pt2;

	// �н�
	if (angle1 < angle2)
		pArc->angle = angle2-angle1;
	else
		pArc->angle = angle2+360-angle1;

	pArc->nType = Graph_RoundArc;
	m_vTAUnits.push_back(pArc);
}

// �Է�Polyline��ͼԪ��������
BOOL CTilTunArc::clockwisePoly(BOOL bClockWise)
{
	int i,j;
	CRect rect(99999999,99999999,-99999999,-99999999);
	int nSize = m_vTAUnits.size();
	if (nSize == 0)
		return FALSE;

	// �������ͼԪ����Ӿ���
	/*
	if (rect.left > m_vTAUnits[0]->pt1.x)	rect.left = m_vTAUnits[0]->pt1.x;
	if (rect.right < m_vTAUnits[0]->pt1.x) rect.right = m_vTAUnits[0]->pt1.x;
	if (rect.top > m_vTAUnits[0]->pt1.y)	rect.top = m_vTAUnits[0]->pt1.y;
	if (rect.bottom < m_vTAUnits[0]->pt1.y)	rect.bottom = m_vTAUnits[0]->pt1.y;
	for (i=0; i<nSize-1; i++)
	{
		if (rect.left > m_vTAUnits[i]->pt2.x)	rect.left = m_vTAUnits[i]->pt2.x;
		if (rect.right < m_vTAUnits[i]->pt2.x) rect.right = m_vTAUnits[i]->pt2.x;
		if (rect.top > m_vTAUnits[i]->pt2.y)	rect.top = m_vTAUnits[i]->pt2.y;
		if (rect.bottom < m_vTAUnits[i]->pt2.y)	rect.bottom = m_vTAUnits[i]->pt2.y;
	}
	*/

	// �Բ���Polyline���͵����ݽ�������
	if (!m_bIsPoly)
	{
		// �Ե�1��ͼԪ��ʼ���β�������˵���ͬ��ͼԪ������
		for (i=0; i<nSize-1; i++)
		{
			PTUNARC_UNIT pTemp = NULL;
			for (j=i+1; j<nSize; j++)
			{
//				PTUNARC_UNIT* pSharpi = m_vTAUnits[i];
//				PTUNARC_UNIT* pSharpj = m_vTAUnits[j];
				if (fabs(m_vTAUnits[i]->pt2.x-m_vTAUnits[j]->pt1.x)<0.0000001
				 && fabs(m_vTAUnits[i]->pt2.y-m_vTAUnits[j]->pt1.y)<0.0000001)
				{
					// ������������ͼԪ�ս�����i��ͼԪ���棬�򲻴���
					if (j>i+1)	
					{
						// ����ͼԪ
						pTemp = m_vTAUnits[i+1];
						m_vTAUnits[i+1] = m_vTAUnits[j];
						m_vTAUnits[j] = pTemp;
					}
					break;
				}
				else if (fabs(m_vTAUnits[i]->pt2.x-m_vTAUnits[j]->pt2.x)<0.0000001
						&& fabs(m_vTAUnits[i]->pt2.y-m_vTAUnits[j]->pt2.y)<0.0000001)
				{
					// �����˵�
					POINT2DF pt = m_vTAUnits[j]->pt1;
					m_vTAUnits[j]->pt1 = m_vTAUnits[j]->pt2;
					m_vTAUnits[j]->pt2 = pt;
					
					// ������������ͼԪ�ս�����i��ͼԪ���棬�򲻴���
					if (j>i+1)	
					{
						// ����ͼԪ
						pTemp = m_vTAUnits[i+1];
						m_vTAUnits[i+1] = m_vTAUnits[j];
						m_vTAUnits[j] = pTemp;
					}

					break;
				}
			}
		}
	}

	// �ж���˳ʱ����������ʱ������(˳ʱ��Ϊ������ʱ��Ϊ��)
	double dbArea = 0.0;
	for (i=0; i<nSize;i++)
	{
		dbArea += m_vTAUnits[i]->pt1.x*m_vTAUnits[i]->pt2.y
				 -m_vTAUnits[i]->pt1.y*m_vTAUnits[i]->pt2.x;
		
	}

	// ����ʱ�����ݷ���(˳ʱ��Ϊ������ʱ��Ϊ��)
	if ((bClockWise && dbArea>0) || (!bClockWise && dbArea<0))
	{
		// �Ƚ�ͼԪ����
		PTUNARC_UNIT pTemp = NULL;
		for (i=0; i<nSize/2; i++)
		{
			pTemp = m_vTAUnits[i];
			m_vTAUnits[i] = m_vTAUnits[nSize-1-i];
			m_vTAUnits[nSize-1-i] = pTemp;
		}

		// �ٽ�ͼԪ�˵����ݷ���
		POINT2DF pt;
		for (i=0; i<nSize; i++)
		{
			pt = m_vTAUnits[i]->pt1;
			m_vTAUnits[i]->pt1 = m_vTAUnits[i]->pt2;
			m_vTAUnits[i]->pt2 = pt;
		}
	}

	// �����1��ͼԪ�ĵ�1�㲻�����һ��ͼԪ�����һ���˵㣬��˵��
	// ����ͼԪ�����һ���Ƿ�տ��������߻�ֻ�з�յĿ���������
	if ((fabs(m_vTAUnits[0]->pt1.x-m_vTAUnits[nSize-1]->pt2.x)>0.001)
		|| (fabs(m_vTAUnits[0]->pt1.y-m_vTAUnits[nSize-1]->pt2.y)>0.001))
		return FALSE;

	return TRUE;
}

void CTilTunArc::ClearMem()
{
	int nSize = m_vTAUnits.size();
	for (int i=0; i<nSize; i++)
	{
		delete m_vTAUnits[i];
	}
	m_vTAUnits.clear();
}

void CTilTunArc::Test()
{
	PTUNARC_UNIT pSharp = NULL;
	int nSize = m_vTAUnits.size();
	for (int i=0; i<nSize; i++)
	{
		pSharp = m_vTAUnits[i];
		TRACE("/------------------------------/");
		if (pSharp->nType == Graph_Line)
		{
			TRACE("�߶Σ�\n");
			TRACE("��һ�㣺%f,%f\n",pSharp->pt1.x,pSharp->pt1.y);
			TRACE("�ڶ��㣺%f,%f\n",pSharp->pt2.x,pSharp->pt2.y);
		}
		else
		{
			TRACE("Բ����\n");
			TRACE("�˵�1��%f,%f\n",pSharp->pt1.x,pSharp->pt1.y);
			TRACE("�˵�2��%f,%f\n",pSharp->pt2.x,pSharp->pt2.y);
			TRACE("Բ�ģ�%f,%f\n",pSharp->ptCenter.x,pSharp->ptCenter.y);
			TRACE("�нǣ�%f��\n",pSharp->angle);
		}
	}
}

// ����������ͼԪy���귴��
void CTilTunArc::changeAxisDir(int x, int y)
{
	PTUNARC_UNIT pArcUnit = NULL;
	int nSize = m_vTAUnits.size();
	for (int i=0; i<nSize; i++)
	{
		pArcUnit = m_vTAUnits[i];
		pArcUnit->pt1.x *= x;
		pArcUnit->pt1.y *= y;

		pArcUnit->pt2.x *= x;
		pArcUnit->pt2.y *= y;

		if (pArcUnit->nType == Graph_RoundArc)
		{
			pArcUnit->ptCenter.x *= x;
			pArcUnit->ptCenter.y *= y;
		}
	}
}

// ����������
void CTilTunArc::tunArcCenter(POINT2DF& ptCenter)
{
	ptCenter.x = ptCenter.y = 0;

	PTUNARC_UNIT pArcUnit = NULL;
	int nSize = m_vTAUnits.size();
	for (int i=0; i<nSize; i++)
	{
		pArcUnit = m_vTAUnits[i];
		ptCenter.x += pArcUnit->pt1.x;
		ptCenter.y += pArcUnit->pt1.y;
	}

	ptCenter.x /= nSize;
	ptCenter.y /= nSize;
}

// �Ը�����Ϊԭ������������������
void CTilTunArc::resetOrign(const POINT2DF& ptOrign)
{
	PTUNARC_UNIT pArcUnit = NULL;
	int nSize = m_vTAUnits.size();
	for (int i=0; i<nSize; i++)
	{
		pArcUnit = m_vTAUnits[i];
		pArcUnit->pt1.x -= ptOrign.x;
		pArcUnit->pt1.y -= ptOrign.y;

		pArcUnit->pt2.x -= ptOrign.x;
		pArcUnit->pt2.y -= ptOrign.y;

		if (pArcUnit->nType == Graph_RoundArc)
		{
			pArcUnit->ptCenter.x -= ptOrign.x;
			pArcUnit->ptCenter.y -= ptOrign.y;
		}
	}
}

// ����ͼԪ����
void CTilTunArc::SetZoom(double scale)
{
	PTUNARC_UNIT pArcUnit = NULL;
	int nSize = m_vTAUnits.size();
	for (int i=0; i<nSize; i++)
	{
		pArcUnit = m_vTAUnits[i];
		pArcUnit->pt1.x *= scale;
		pArcUnit->pt1.y *= scale;

		pArcUnit->pt2.x *= scale;
		pArcUnit->pt2.y *= scale;

		if (pArcUnit->nType == Graph_RoundArc)
		{
			pArcUnit->ptCenter.x *= scale;
			pArcUnit->ptCenter.y *= scale;
		}
	}
}


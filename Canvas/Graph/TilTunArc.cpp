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

// 在cad中，根据lwpolyline的起始点BeginPoint、
// 终止点EndPoint、和
void lwpolylineToArc(const POINT2DF& BeginPoint,const POINT2DF& EndPoint,
					 double u,POINT2DF &CenterPoint)
{
	double centerAngle;//包角
	centerAngle=4*atan(fabs(u));
	double x1,x2,y1,y2;//圆弧起始点和终止点
	x1=BeginPoint.x;
	x2=EndPoint.x;
	y1=BeginPoint.y;
	y2=EndPoint.y;
	double L; //弦长
	L=sqrt(pow((x1-x2),2)+pow((y1-y2),2)); 
	
	double R;//圆弧半径
	R=0.5*L/sin(0.5*centerAngle);
	//已知圆上两点和半径，求圆心坐标
	double h;//圆心到弦的距离
	h=sqrt(R*R-L*L/4);
	double xc,yc;//圆心坐标
	double xa,ya; //起始点和终止点连线的中点横纵坐标
	xa=0.5*(x1+x2);
	ya=0.5*(y1+y2);
	//弦的方向角（0-2PI之）
	double angle;//起点到终点的弦向量与x正方向之间的倾斜角
	angle=acos((x2-x1)/sqrt(pow(x2-x1,2)+pow(y2-y1,2)));
	double amass; //弦向量与X轴正向单位向量的叉积
	amass = y2-y1;//由（由(x2-x1)*0+1*(y2-y1)）得到
	
	if (amass<0)
	{
		angle=-angle;
		angle=2*PI+angle;
	}

	double DirectionAngle;//弦中点到圆心的直线向量的方向角（0-2PI之间）
	if ((u>0 && centerAngle<PI)||(u<0 && centerAngle>PI))
		DirectionAngle=angle+PI/2;
	if((u<0 && centerAngle<PI)||(u>0 && centerAngle>PI))
		DirectionAngle=angle-PI/2;
	if (DirectionAngle>2*PI)
		DirectionAngle= DirectionAngle-2*PI;
	double d;//圆心到弦的距离
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
		double nslope,k;//nslope 为弦的斜率，K为弦中垂线的斜率
		double nAngle;//中垂线的倾斜角；
		double X,Y; //圆心相对于弦中心点的坐标偏移量
		
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

// 求凸度夹角，以角度表示
double convexAngle(const POINT2DF& pt1, const POINT2DF &pt2, const POINT2DF &ptCenter, double convex)
{
	double angle;

	// 求p1的角度
	POINT2DF p1, p2;
	p1.x = pt1.x-ptCenter.x;
	p1.y = pt1.y-ptCenter.y;
	double dist1 = sqrt(p1.x*p1.x+p1.y*p1.y);
	double angle1 = acos(p1.x/dist1);
	// p1在3、4像限时
	if (p1.y<0 && fabs(p1.y)>0.0000001 )
		angle1 += PI;

	p2.x = pt2.x-ptCenter.x;
	p2.y = pt2.y-ptCenter.y;
	double dist2 = sqrt(p2.x*p2.x+p2.y*p2.y);
	double angle2 = acos(p2.x/dist2);
	// p2在3、4像限时
	if (p2.y<0 && fabs(p2.y)>0.0000001)
		angle2 += PI;

	// 凸度值为负表示顺时针圆弧，凸度值为正表示逆时针圆弧
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
// 将隧道尺寸复制到当前类对象
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
		AfxMessageBox("隧道断面文件打开错误!");
		return;
	}

	CString strLine;

	// 第一行为节点数
// 	file.ReadString(strLine);
// 	int nNum = atoi(strLine);

	// 求隧道内轮廓适用范围（暂时没有做这一步）
	//////////////////////////////////////////////////////////////////////////
	CPolygon* pTunArc = (CPolygon*)pCanvas->GetGraph()->GetGraphByIdx(GRAPH_LAYER_TUNARC)->GetUnitByIdx(0);

	pTunArc->m_pPointList->RemoveAll();

	POINT2DF pt1, pt2, ptTunCenter, ptCenter;

	// 读取隧道外轮廓中心点平面坐标
	file.ReadString(strLine);
	get_coord(strLine, ptTunCenter);
	
	// 读取第2行，必须为点坐标
	file.ReadString(strLine);
	get_coord(strLine, pt1);
	pTunArc->m_pPointList->Add(pt1);

	while (file.ReadString(strLine))
	{
		// 求线型(直线或曲线)
		int nType = atoi(strLine);

		file.ReadString(strLine);

		// 若为直线，则求下一点坐标
		if (nType == Graph_Line)
		{
			get_coord(strLine, pt2);
			pTunArc->m_pPointList->Add(pt2);
		}
		else // 若为弧线，则求出第二个端点(nType == Graph_RoundArc)
		{
			// 读取圆心坐标及圆弧角度
			get_coord(strLine, ptCenter);
			file.ReadString(strLine);
			double dbAngle = atof(strLine)/180*PI;
//			get_arc_point(pt1, ptCenter, dbAngle, pt2);

			// 求半径和弧长
			double dbRadius = get_distance(pt1, ptCenter);

#define AVE_ARC_LEN 1250	// 圆弧半径大于规定值时用平均弧长分割圆弧
#define RADIUS		5000	// 用平均弧长分割圆弧时的最小圆弧半径
#define AVE_ANGLE	0.25	// 圆弧半径小于规定值时用平均角度分割圆弧

			// 将圆弧分段，将各段顶点作为多边形顶点保存
			// 求圆弧分成的段数及每段的弧度
			int nSegNum;
			double dbSegAngle;// dbLength
			// 圆弧半径小于规定值时用平均角度分割圆弧
			if (dbRadius <= RADIUS)
			{
				nSegNum = int(dbAngle/AVE_ANGLE+0.5);
				dbSegAngle = dbAngle/nSegNum;
			}
			else // 圆弧半径大于规定值时用平均弧长分割圆弧
			{
				double dbLength  = dbRadius*dbAngle;
				nSegNum = int(dbLength/AVE_ARC_LEN+0.5);
				dbSegAngle = dbAngle/nSegNum;
			}

			// 求弧上各点并作为多边形顶点压入堆栈
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

		// 将pt1作为下一段线的起始点
		pt1 = pt2;
	}

	// 最后一点即为第一点，将其删除
	pTunArc->m_pPointList->RemoveAt(pTunArc->m_pPointList->GetSize()-1);

	//////////////////////////////////////////////////////////////////////////
	// 求x,y坐标最大及最小点
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

	// 将y坐标反向并按比例缩小,同时向右和向下平移10像素
	double dbY = dbMaxY-dbMinY;
	for (i=0; i<pTunArc->m_pPointList->GetSize(); i++)
	{
		POINT2DF ptTmp = pTunArc->m_pPointList->GetAt(i);
		ptTmp.x = (ptTmp.x-dbMinX)*TunArc_Scale+TunArc_Extend;
		ptTmp.y = (dbY-(ptTmp.y-dbMinY))*TunArc_Scale+TunArc_Extend;
		pTunArc->m_pPointList->SetAt(i, ptTmp);
	}

	// 设置中心点坐标
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

	// 求x坐标
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

	// 求y坐标
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

// 读取隧道断面尺寸并存入成员变量中
BOOL CTilTunArc::Import(CString strPathTunArc)
{
	/*
	int i;
	CStdioFile file;
	if (!file.Open(strPathTunArc, CFile::modeRead))
	{
		AfxMessageBox("隧道断面文件打开错误!");
		return FALSE;
	}

	m_ptImgCenter.x = m_ptImgCenter.y = 0;
	ClearMem();

	// 用来标记文件中已经被存入了隧道断面尺寸信息
	int nType;
	CString strLine;
	POINT2DF pt, pt1, pt2, ptTunCenter, ptCenter;

	//////////////////////////////////////////////////////////////////////////
	// 读取隧道外轮廓中心点平面坐标
	file.ReadString(strLine);
	get_coord(strLine, ptTunCenter);

	// 读取第2行，必须为点坐标
	file.ReadString(strLine);
	get_coord(strLine, pt1);

	while (file.ReadString(strLine))
	{
		pt.x = pt1.x-ptTunCenter.x;
		pt.y = pt1.y-ptTunCenter.y;

		TUNARC_UNIT tunArcUnit;
		tunArcUnit.pt1.x = pt.x*TunArc_Scale;
		tunArcUnit.pt1.y = pt.y*TunArc_Scale;

		// 求线型(直线或曲线)
		nType = atoi(strLine);
		tunArcUnit.nType = nType;

		// 若为弧线，则求出第二个端点(nType == Graph_RoundArc)
		if (nType == Graph_RoundArc)
		{
			// 读取圆心坐标
			file.ReadString(strLine);
			get_coord(strLine, ptCenter);
			pt.x = ptCenter.x-ptTunCenter.x;
			pt.y = ptCenter.y-ptTunCenter.y;
			tunArcUnit.ptCenter.x = pt.x*TunArc_Scale;
			tunArcUnit.ptCenter.y = pt.y*TunArc_Scale;

			// 读取弧度
			file.ReadString(strLine);
			double dbAngle = atof(strLine)/180*PI;
			tunArcUnit.angle = dbAngle;
		}

		// 读取第2个端点
		file.ReadString(strLine);
		get_coord(strLine, pt2);
		pt.x = pt2.x-ptTunCenter.x;
		pt.y = pt2.y-ptTunCenter.y;
		tunArcUnit.pt2.x = pt.x*TunArc_Scale;
		tunArcUnit.pt2.y = pt.y*TunArc_Scale;

		m_vTAUnits.push_back(tunArcUnit);

		// 将pt1作为下一段线的起始点
		pt1 = pt2;
	}
	*/

	if (!ReadDXF(strPathTunArc))
	{
		AfxMessageBox("隧道断面文件打开错误!");
		return FALSE;
	}

//	Test();

	// 对非Polyline线图元进行顺时针排序
	if (!clockwisePoly(TRUE))
	{
		AfxMessageBox("请只输入完整的封闭开挖轮廓线cad图形!", MB_OK|MB_ICONINFORMATION);
	}
	// 将图元变为绕原点中心对称的图元
	changeAxisDir(-1,-1);
	// 设置缩放率
	SetZoom(TunArc_Scale*2);

	// 将开挖轮廓线左下角设置为图元的原点
	CUnit* pTunArc = new CPolygon;
	Transferto(pTunArc);
	CRect rect = pTunArc->GetPolyRect();
	delete pTunArc;
	POINT2DF ptOrign(rect.left-TunArc_Extend, rect.top-TunArc_Extend);

	// 将重心点设置为原点
	resetOrign(ptOrign);

	//////////////////////////////////////////////////////////////////////////

	//m_ptImgCenter.x = rect.Width()/2+TunArc_Extend;
	//m_ptImgCenter.y = rect.bottom+TunArc_Extend;

	/*
	// 将所有关键点绕x轴翻转180度保存
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

// 读取和存储隧道断面尺寸数据
BOOL CTilTunArc::Serialize(CArchive& ar)
{
	// 读取隧道断面尺寸数据
	if (ar.IsLoading())
	{
		POINT2DF pt, ptStart;
		int nType;

		// 用来标记文件中已经被存入了隧道断面尺寸信息
		int nIsTunArcSaved;
		ar >> nIsTunArcSaved;
		if (nIsTunArcSaved != Graph_TunArc_Save)
		{
			TRACE("没有保存过隧道断面尺寸!\n");
			return FALSE;
		}

		// 先读取隧道断面中心点在图像中的坐标
		ar >> m_ptImgCenter.x >> m_ptImgCenter.y;

		// 读取图元数
		int nSize;
		ar >> nSize;

		// 读取断面上第1点坐标及图元类型
		ar >> pt.x >> pt.y;

		ptStart = pt;

		// 如果图元类型不是Graph_Unit_Save，则隧道断面尺寸还未读取完成
		for (int i=0; i<nSize; i++)
		{
			// 读取第2行，必须为点坐标
			PTUNARC_UNIT pArcUnit = new TUNARC_UNIT;
			pArcUnit->pt1 = pt;

			// 读取图元类型
			ar >> nType;
			pArcUnit->nType = nType;

			// 若为圆弧，则读取圆心坐标及弧度
			if (nType == Graph_RoundArc)
			{
				// 读取圆心坐标
				ar >> pArcUnit->ptCenter.x >> pArcUnit->ptCenter.y;

				// 读取圆弧弧度
				ar >> pArcUnit->angle;
				// 将弧度转化成角度
				//pArcUnit->angle = pArcUnit->angle/PI*180;
			}

			// 读取另一端点坐标
			ar >> pArcUnit->pt2.x >> pArcUnit->pt2.y;

			m_vTAUnits.push_back(pArcUnit);

			// 将pt2作为下图元的起始点
			pt = pArcUnit->pt2;
		}

		return TRUE;
	}
	// 保存隧道断面尺寸数据
	else
	{
		// 如果没有断面尺寸数据，则不保存
		if (m_vTAUnits.empty())
		{
			ar << Graph_Unit_Save;
			return FALSE;
		}

		// 用来标记文件中已经被存入了隧道断面尺寸信息
		ar << Graph_TunArc_Save;
		ar << m_ptImgCenter.x << m_ptImgCenter.y;

		// 保存图元数
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
// 从图形层文件中读取隧道拱形并将其存储在成员变量中
BOOL CTilTunArc::Read(CFile& file)
{
	POINT2DF pt;
	int nType;

	file.SeekToBegin();
	CArchive ar(&file, CArchive::load);

	// 用来标记文件中已经被存入了隧道断面尺寸信息
	int nIsTunArcSaved;
	ar >> nIsTunArcSaved;
	if (nIsTunArcSaved != Graph_TunArc_Save)
	{
		TRACE("没有保存过隧道断面尺寸!\n");
		return FALSE;
	}

	// 先读取隧道断面中心点在图像中的坐标
	ar >> m_ptImgCenter.x >> m_ptImgCenter.y;

	// 读取断面上第1点坐标及图元类型
	ar >> pt.x >> pt.y >> nType;

	// 如果图元类型不是Graph_Unit_Save，则隧道断面尺寸还未读取完成
	while (nType != Graph_Unit_Save)
	{
		// 读取第2行，必须为点坐标
		TUNARC_UNIT tunArcUnit;
		tunArcUnit.pt1 = pt;
		tunArcUnit.nType = nType;

		// 若为圆弧，则读取圆心坐标及弧度
		if (nType == Graph_RoundArc)
		{
			// 读取圆心坐标
			ar >> tunArcUnit.ptCenter.x >> tunArcUnit.ptCenter.y;

			// 读取圆弧弧度
			ar >> tunArcUnit.angle;
		}

		// 读取另一端点坐标
		ar >> tunArcUnit.pt2.x >> tunArcUnit.pt2.y;

		m_vTAUnits.push_back(tunArcUnit);

		// 将pt2作为下图元的起始点
		pt = tunArcUnit.pt2;
	}

	ar.Close();

	return TRUE;
}

// 保存隧道断面
void CTilTunArc::Save(CFile& file)
{
	// 从开始处读取
	file.SeekToBegin();

	CArchive ar(&file, CArchive::store);

	// 用来标记文件中已经被存入了隧道断面尺寸信息
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
 * 函数名称：  CTilTunArc::TransferPolygon()
 * 作者：      [Leng]
 * 创建于:     [2009-3-27 11:00:13]
 * 返回值：    [BOOL] -
 * 参数列表： 
               [float fAveArcLenT] - [In] // 圆弧半径大于规定值时用平均弧长分割圆弧
               [ float fRadiusT] - [In]	  // 用平均弧长分割圆弧时的最小圆弧半径
               [ float fAveAngleT] - [In] // 圆弧半径小于规定值时用平均弧度分割圆弧
               [ CUnit* pTunArc] - [In]   // 输出隧道断面尺寸
注释 :	
				
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

//	TRACE("/---------------------转化成多段线---------------------/\n");
	int i, nSize;
	nSize = m_vTAUnits.size();
	for (i=0; i<nSize; i++)
	{
		pt1 = m_vTAUnits[i]->pt1;//+m_ptImgCenter;
		pTunArc->m_pPointList->Add(pt1);
//		TRACE("Point[%f,%f]\n",pt1.x,pt1.y);

		if (m_vTAUnits[i]->nType == Graph_RoundArc)
		{
			// 求半径和弧长
			ptCenter = m_vTAUnits[i]->ptCenter;//+m_ptImgCenter;
			dbRadius = get_distance(pt1, ptCenter);

			// 将圆弧分段，将各段顶点作为多边形顶点保存
			// 求圆弧分成的段数及每段的弧度
			fAngle = m_vTAUnits[i]->angle/180*PI;

			int nSegNum;
			double dbSegAngle/*, dbLength*/;
			// 圆弧半径小于规定值时用平均角度分割圆弧
			if (dbRadius <= fRadiusT)
			{
				nSegNum = int(fAngle/fAveAngleT+0.5);
				dbSegAngle = fAngle/nSegNum;
			}
			else // 圆弧半径大于规定值时用平均弧长分割圆弧
			{
				double dbLength  = dbRadius*fAngle;
				nSegNum = int(dbLength/fAveArcLenT+0.5);
				dbSegAngle = fAngle/nSegNum;
			}

			// 求弧上各点并作为多边形顶点压入堆栈
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

// 设置隧道断面的移动情况
// dX: 水平方向移动的位移
// dY: 垂直方向移动的位移
void CTilTunArc::Move(double dX, double dY)
{
	// 图像圆心坐标的调整
	m_ptImgCenter.x += dX;
	m_ptImgCenter.y += dY;

	int nSize = m_vTAUnits.size();
	for (int i=0; i<nSize; i++)
	{
		// 改变两个端点的坐标
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
	
// 读取dxf文件，只解析其中的多段线、圆弧、直线等图元
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
		// 读至ENTITIES块
		while (m_File.ReadString(strLine))
		{
			if (strLine == "ENTITIES")
				break;
		}

		while (m_File.ReadString(strLine))
		{
			if (strLine == "AcDbPolyline")	// 多段线
			{
				ReadPolyline();
				m_bIsPoly = TRUE;
			}
			else if (strLine == "AcDbCircle")	// 圆弧
			{
				ReadArc();
				m_bIsPoly = FALSE;
			}
			else if (strLine == "AcDbLine")		// 线段
			{
				ReadLine();
				m_bIsPoly = FALSE;
			}
		}
	}
	catch (...)
	{
		throw  CString("文件读取错误!"); 
	}

	m_File.Close();

	return TRUE;
}

// 读取多义线，从AcDbPolyline的下一行开始
void CTilTunArc::ReadPolyline()
{
	CString strLine;

	m_File.ReadString(strLine);	// 90
	m_File.ReadString(strLine);	// 顶点数
	int nVerNum = atoi(strLine);

	m_File.ReadString(strLine); // 70
	m_File.ReadString(strLine); // 多段线标志（按位编码）；默认值为 0：1 = 关闭；128 = Plinegen
	int nMark=atoi(strLine);

	m_File.ReadString(strLine); // 43
	m_File.ReadString(strLine); // 固定宽度

	// 38:标高，39:厚度
	while (m_File.ReadString(strLine))
	{
		if (atoi(strLine)==38 || atoi(strLine)==39)
			m_File.ReadString(strLine);
		else // 10
			break;
	}

	POINT2DF pt1,pt2,ptCenter;// 顶点、中点坐标
	double convex; // 凸度
	double angle;  // 凸度夹角
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
		
			// 保存直线图元
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
			// 根据凸度计算圆心坐标
			m_File.ReadString(strLine);
			convex = atof(strLine);

			m_File.ReadString(strLine);	// 10
			m_File.ReadString(strLine); // x
			pt2.x = atof(strLine);
			
			m_File.ReadString(strLine); // 20
			m_File.ReadString(strLine); // y
			pt2.y = atof(strLine);

			lwpolylineToArc(pt1,pt2,convex,ptCenter);
			// 根据凸度计算圆弧夹角角度
			angle = convexAngle(pt1,pt2,ptCenter,convex);

			// 保存圆弧图元
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

	// 如果nMark==1,则说明是封闭曲线,要保留最后一段
	if(nMark==1)
	{
		// 保存直线图元
		PTUNARC_UNIT pLine = new TUNARC_UNIT;
		pLine->pt1 = pt1;
		pLine->pt2 = m_vTAUnits[0]->pt1;
		
		pLine->nType = Graph_Line;
		m_vTAUnits.push_back(pLine);
	}
}

// 读取线段，从AcDbLine的下一行开始
void CTilTunArc::ReadLine()
{
	CString strLine;
	PTUNARC_UNIT pLine = new TUNARC_UNIT;

	// 读取x坐标
	while (m_File.ReadString(strLine))
	{
		if (atoi(strLine) != 10)
		{
			m_File.ReadString(strLine);
		}
		else
			break;
	}
	m_File.ReadString(strLine);	// x1坐标
	pLine->pt1.x = atof(strLine);

	m_File.ReadString(strLine);	//20
	m_File.ReadString(strLine);	// y1坐标
	pLine->pt1.y = atof(strLine);

	m_File.ReadString(strLine); //30
	m_File.ReadString(strLine);	// z1坐标
//	pLine->pt1.z = atof(strLine);

	m_File.ReadString(strLine);	//11
	m_File.ReadString(strLine);	// x2坐标
	pLine->pt2.x = atof(strLine);

	m_File.ReadString(strLine);	//21
	m_File.ReadString(strLine);	// y2坐标
	pLine->pt2.y = atof(strLine);

	m_File.ReadString(strLine); //31
	m_File.ReadString(strLine);	// z2坐标
//	pLine->pt2.z = atof(strLine);

	pLine->nType =Graph_Line;
	m_vTAUnits.push_back(pLine);
}

// 读取圆曲线,从AcDbCircle的下一行开始
void CTilTunArc::ReadArc()
{
	CString strLine;
	PTUNARC_UNIT pArc = new TUNARC_UNIT;

	// 读取中心点坐标
	while (m_File.ReadString(strLine))
	{
		if (atoi(strLine) != 10)
		{
			m_File.ReadString(strLine);
		}
		else
			break;
	}
	m_File.ReadString(strLine);	// x坐标
	pArc->ptCenter.x = atof(strLine);

	m_File.ReadString(strLine);	//20
	m_File.ReadString(strLine);	// y坐标
	pArc->ptCenter.y = atof(strLine);

	m_File.ReadString(strLine); //30
	m_File.ReadString(strLine);	// z坐标
//	pArc->ptCenter.z = atof(strLine);

	// 半径
	m_File.ReadString(strLine); //40
	m_File.ReadString(strLine);	// 半径
	double radius = atof(strLine);

	m_File.ReadString(strLine);	// 100
	m_File.ReadString(strLine);	// AcDbArc:子类标记

	// 起始角终止角度,沿逆时针方向旋转
	m_File.ReadString(strLine);	//50
	m_File.ReadString(strLine);	// 起点角度
	double angle1 = atof(strLine);

	// 根据起点角度等求得起始端点坐标
	POINT2DF pt1,pt2;
	pt1.x = pArc->ptCenter.x+radius*cos(angle1/180*PI);
	pt1.y = pArc->ptCenter.y+radius*sin(angle1/180*PI);
	pArc->pt1 = pt1;

	m_File.ReadString(strLine);	//51
	m_File.ReadString(strLine);	// 端点角度
	double angle2 = atof(strLine);

	// 根据终点角度等求得终止端点坐标
	pt2.x = pArc->ptCenter.x+radius*cos(angle2/180*PI);
	pt2.y = pArc->ptCenter.y+radius*sin(angle2/180*PI);
	pArc->pt2 = pt2;

	// 夹角
	if (angle1 < angle2)
		pArc->angle = angle2-angle1;
	else
		pArc->angle = angle2+360-angle1;

	pArc->nType = Graph_RoundArc;
	m_vTAUnits.push_back(pArc);
}

// 对非Polyline线图元进行排序
BOOL CTilTunArc::clockwisePoly(BOOL bClockWise)
{
	int i,j;
	CRect rect(99999999,99999999,-99999999,-99999999);
	int nSize = m_vTAUnits.size();
	if (nSize == 0)
		return FALSE;

	// 求出所有图元的外接矩形
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

	// 对不是Polyline类型的数据进行排序
	if (!m_bIsPoly)
	{
		// 以第1个图元开始依次查找与其端点相同的图元并加入
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
					// 若满足条件的图元刚紧跟第i个图元后面，则不处理
					if (j>i+1)	
					{
						// 交换图元
						pTemp = m_vTAUnits[i+1];
						m_vTAUnits[i+1] = m_vTAUnits[j];
						m_vTAUnits[j] = pTemp;
					}
					break;
				}
				else if (fabs(m_vTAUnits[i]->pt2.x-m_vTAUnits[j]->pt2.x)<0.0000001
						&& fabs(m_vTAUnits[i]->pt2.y-m_vTAUnits[j]->pt2.y)<0.0000001)
				{
					// 交换端点
					POINT2DF pt = m_vTAUnits[j]->pt1;
					m_vTAUnits[j]->pt1 = m_vTAUnits[j]->pt2;
					m_vTAUnits[j]->pt2 = pt;
					
					// 若满足条件的图元刚紧跟第i个图元后面，则不处理
					if (j>i+1)	
					{
						// 交换图元
						pTemp = m_vTAUnits[i+1];
						m_vTAUnits[i+1] = m_vTAUnits[j];
						m_vTAUnits[j] = pTemp;
					}

					break;
				}
			}
		}
	}

	// 判断是顺时针排序还是逆时针排序(顺时针为负，逆时针为正)
	double dbArea = 0.0;
	for (i=0; i<nSize;i++)
	{
		dbArea += m_vTAUnits[i]->pt1.x*m_vTAUnits[i]->pt2.y
				 -m_vTAUnits[i]->pt1.y*m_vTAUnits[i]->pt2.x;
		
	}

	// 对逆时针数据反向(顺时针为负，逆时针为正)
	if ((bClockWise && dbArea>0) || (!bClockWise && dbArea<0))
	{
		// 先将图元反向
		PTUNARC_UNIT pTemp = NULL;
		for (i=0; i<nSize/2; i++)
		{
			pTemp = m_vTAUnits[i];
			m_vTAUnits[i] = m_vTAUnits[nSize-1-i];
			m_vTAUnits[nSize-1-i] = pTemp;
		}

		// 再将图元端点数据反向
		POINT2DF pt;
		for (i=0; i<nSize; i++)
		{
			pt = m_vTAUnits[i]->pt1;
			m_vTAUnits[i]->pt1 = m_vTAUnits[i]->pt2;
			m_vTAUnits[i]->pt2 = pt;
		}
	}

	// 如果第1个图元的第1点不是最后一个图元的最后一个端点，则说明
	// 所有图元组合在一起不是封闭开挖轮廓线或不只有封闭的开挖轮廓线
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
			TRACE("线段：\n");
			TRACE("第一点：%f,%f\n",pSharp->pt1.x,pSharp->pt1.y);
			TRACE("第二点：%f,%f\n",pSharp->pt2.x,pSharp->pt2.y);
		}
		else
		{
			TRACE("圆弧：\n");
			TRACE("端点1：%f,%f\n",pSharp->pt1.x,pSharp->pt1.y);
			TRACE("端点2：%f,%f\n",pSharp->pt2.x,pSharp->pt2.y);
			TRACE("圆心：%f,%f\n",pSharp->ptCenter.x,pSharp->ptCenter.y);
			TRACE("夹角：%f度\n",pSharp->angle);
		}
	}
}

// 开挖轮廓线图元y坐标反向
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

// 求重心坐标
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

// 以给定点为原点坐标重新设置坐标
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

// 设置图元比例
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


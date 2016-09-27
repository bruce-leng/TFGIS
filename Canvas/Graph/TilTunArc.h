// TunArc.h: interface for the CTilTunArc class.
//
//////////////////////////////////////////////////////////////////////

/************************************
  REVISION LOG ENTRY
  维护人员: leng
  维护时间 2008-9-23 11:06:35
  说明: 读取隧道拱形参数文件
 ************************************/


#if !defined(AFX_TUNARC_H__293DE523_3223_4D12_8EC8_5618D6DCE1B0__INCLUDED_)
#define AFX_TUNARC_H__293DE523_3223_4D12_8EC8_5618D6DCE1B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TilPoint.h"

// 隧道断面上的图元
typedef struct __tagTUNARC_UNIT
{
	int nType;	// 图元类型(10:线段,18:圆弧)
	POINT2DF pt1;	// 第1个端点
	POINT2DF pt2;	// 第2个端点

	// 下面是图元为圆弧的情况用到的参数
	POINT2DF ptCenter;	// 圆弧的圆心
	float angle;		// 圆弧的弧度
} TUNARC_UNIT, *PTUNARC_UNIT, **PPTUNARC_UNIT;

#include <vector>
using namespace::std;

// 将断面尺寸转化成显示图形多边形
#define AVE_ARC_LEN 1250	// 圆弧半径大于规定值时用平均弧长分割圆弧
#define RADIUS		5000	// 用平均弧长分割圆弧时的最小圆弧半径
#define AVE_ANGLE	0.25	// 圆弧半径小于规定值时用平均弧度分割圆弧

class CTilCanvas;
class CGraph;
class CUnit;
class AFX_EXT_CLASS CTilTunArc
{
public:
	CTilTunArc();
	virtual ~CTilTunArc();

	// 判断隧道断面尺寸是否有效
	//BOOL IsValid() {return !m_vTAUnits.empty();};
	// 读取和存储隧道断面尺寸数据
	BOOL Serialize(CArchive& ar);
	// 从图形断面输入文件中读出隧道拱形并将其存储在成员变量中
	BOOL Import(CString strPathTunArc);
	// 将断面尺寸转化成显示图形多边形
	BOOL Transferto(CUnit* pTunArc, float fAveArcLenT=AVE_ARC_LEN,
					float fRadiusT=RADIUS, float fAveAngleT=AVE_ANGLE);

	// 读取图形层文件中的隧道断面尺寸
	// BOOL ReadGraphTunArc(CArchive& ar, CUnit* pTunArc);
	// 设置隧道原点在图像中的坐标
	void SetImgCenter(const POINT2DF& ptImgCenter) {m_ptImgCenter = ptImgCenter;};
	// 取得隧道原点在图像中的坐标
	POINT2DF GetImgCenter(){return m_ptImgCenter;};
	// 取得隧道断面图元
	vector<PTUNARC_UNIT>* GetTunArcUnits(){return &m_vTAUnits;};
	// 设置隧道断面的移动情况
	void Move(double dX, double dY);

//////////////////////////////////////////////////////////////////////////

public:
	void operator = (CTilTunArc& tunArc);
	// 读取文件
	BOOL ReadDXF(const CString& filename);
	// 清除内存
	void ClearMem();
	// 设置图元比例
	void SetZoom(double scale);
	// 测试..........................
	void Test();

protected:
	// 读取多义线
	void ReadPolyline();
	// 读取线段
	void ReadLine();
	// 读取圆曲线
	void ReadArc();

private:
	// 对非Polyline线图元进行排序
	BOOL clockwisePoly(BOOL bClockWise=TRUE);
	// 开挖轮廓线图元y坐标反向
	void changeAxisDir(int x, int y);
	// 求重心坐标
	void tunArcCenter(POINT2DF& ptCenter);
	// 以给定点为原点坐标重新设置坐标
	void resetOrign(const POINT2DF& ptOrign);

	vector< PTUNARC_UNIT > m_vTAUnits;
	POINT2DF m_ptImgCenter;	// 隧道原点在图像中的坐标

	CStdioFile m_File;
	BOOL m_bIsPoly;
};

#endif // !defined(AFX_TUNARC_H__293DE523_3223_4D12_8EC8_5618D6DCE1B0__INCLUDED_)

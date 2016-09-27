// TunArc.h: interface for the CTilTunArc class.
//
//////////////////////////////////////////////////////////////////////

/************************************
  REVISION LOG ENTRY
  ά����Ա: leng
  ά��ʱ�� 2008-9-23 11:06:35
  ˵��: ��ȡ������β����ļ�
 ************************************/


#if !defined(AFX_TUNARC_H__293DE523_3223_4D12_8EC8_5618D6DCE1B0__INCLUDED_)
#define AFX_TUNARC_H__293DE523_3223_4D12_8EC8_5618D6DCE1B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TilPoint.h"

// ��������ϵ�ͼԪ
typedef struct __tagTUNARC_UNIT
{
	int nType;	// ͼԪ����(10:�߶�,18:Բ��)
	POINT2DF pt1;	// ��1���˵�
	POINT2DF pt2;	// ��2���˵�

	// ������ͼԪΪԲ��������õ��Ĳ���
	POINT2DF ptCenter;	// Բ����Բ��
	float angle;		// Բ���Ļ���
} TUNARC_UNIT, *PTUNARC_UNIT, **PPTUNARC_UNIT;

#include <vector>
using namespace::std;

// ������ߴ�ת������ʾͼ�ζ����
#define AVE_ARC_LEN 1250	// Բ���뾶���ڹ涨ֵʱ��ƽ�������ָ�Բ��
#define RADIUS		5000	// ��ƽ�������ָ�Բ��ʱ����СԲ���뾶
#define AVE_ANGLE	0.25	// Բ���뾶С�ڹ涨ֵʱ��ƽ�����ȷָ�Բ��

class CTilCanvas;
class CGraph;
class CUnit;
class AFX_EXT_CLASS CTilTunArc
{
public:
	CTilTunArc();
	virtual ~CTilTunArc();

	// �ж��������ߴ��Ƿ���Ч
	//BOOL IsValid() {return !m_vTAUnits.empty();};
	// ��ȡ�ʹ洢�������ߴ�����
	BOOL Serialize(CArchive& ar);
	// ��ͼ�ζ��������ļ��ж���������β�����洢�ڳ�Ա������
	BOOL Import(CString strPathTunArc);
	// ������ߴ�ת������ʾͼ�ζ����
	BOOL Transferto(CUnit* pTunArc, float fAveArcLenT=AVE_ARC_LEN,
					float fRadiusT=RADIUS, float fAveAngleT=AVE_ANGLE);

	// ��ȡͼ�β��ļ��е��������ߴ�
	// BOOL ReadGraphTunArc(CArchive& ar, CUnit* pTunArc);
	// �������ԭ����ͼ���е�����
	void SetImgCenter(const POINT2DF& ptImgCenter) {m_ptImgCenter = ptImgCenter;};
	// ȡ�����ԭ����ͼ���е�����
	POINT2DF GetImgCenter(){return m_ptImgCenter;};
	// ȡ���������ͼԪ
	vector<PTUNARC_UNIT>* GetTunArcUnits(){return &m_vTAUnits;};
	// �������������ƶ����
	void Move(double dX, double dY);

//////////////////////////////////////////////////////////////////////////

public:
	void operator = (CTilTunArc& tunArc);
	// ��ȡ�ļ�
	BOOL ReadDXF(const CString& filename);
	// ����ڴ�
	void ClearMem();
	// ����ͼԪ����
	void SetZoom(double scale);
	// ����..........................
	void Test();

protected:
	// ��ȡ������
	void ReadPolyline();
	// ��ȡ�߶�
	void ReadLine();
	// ��ȡԲ����
	void ReadArc();

private:
	// �Է�Polyline��ͼԪ��������
	BOOL clockwisePoly(BOOL bClockWise=TRUE);
	// ����������ͼԪy���귴��
	void changeAxisDir(int x, int y);
	// ����������
	void tunArcCenter(POINT2DF& ptCenter);
	// �Ը�����Ϊԭ������������������
	void resetOrign(const POINT2DF& ptOrign);

	vector< PTUNARC_UNIT > m_vTAUnits;
	POINT2DF m_ptImgCenter;	// ���ԭ����ͼ���е�����

	CStdioFile m_File;
	BOOL m_bIsPoly;
};

#endif // !defined(AFX_TUNARC_H__293DE523_3223_4D12_8EC8_5618D6DCE1B0__INCLUDED_)

// StaticCanvas.cpp : implementation file
//

#include "stdafx.h"
#include "StaticCanvas.h"
#include "NewMenu.h"
#include <float.H>
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// CData

CData::CData() : 
m_nDataLen(0), 
m_pDataBuf(NULL), 
m_bDraw(FALSE),
m_clr(0x00FF0000)
{
}

CData::~CData()
{
	ClearMem();
}

BOOL CData::Create(int nLen)
{
	ASSERT(nLen > 0);

	ClearMem();

	m_pDataBuf = new double [nLen];
	if(!m_pDataBuf)
		return FALSE;

//	srand( (unsigned)time( NULL ) );
//
//	Sleep(1500);
//	for(int i = 0; i < nLen; ++i)
//		m_pDataBuf[i] = rand();
	m_nDataLen = nLen;

	return TRUE;
}

void CData::ClearMem()
{
	if(m_pDataBuf)
		delete [] m_pDataBuf;

	m_nDataLen = 0;
}

void CData::MaxMin(double& dbMax, double& dbMin)
{
	dbMax = DBL_MIN;
	dbMin = DBL_MAX;
	double dbTmp;
	for(int i = 0; i < m_nDataLen; ++i)
	{
		dbTmp = m_pDataBuf[i];
		dbMax = dbMax < dbTmp ? dbTmp : dbMax;
		dbMin = dbMin > dbTmp ? dbTmp : dbMin;
	}
}
//////////////////////////////////////////////////////////////////////////
// CCanvasInfo
CCanvasInfo::CCanvasInfo() : m_pHist(NULL), m_bLog(FALSE) 
{
}

CCanvasInfo::~CCanvasInfo()
{
	ClearAllMem();
}

BOOL CCanvasInfo::Create(int nCount, int nLen)
{
	ClearAllMem();

	m_pHist = new CData* [nCount];
	if(!m_pHist)
		return FALSE;

	memset(m_pHist, 0, nCount * sizeof(CData*));

	BOOL bSuc = TRUE;

	for(int i = 0; i < nCount; ++i)
	{
		m_pHist[i] = new CData();
		if(!m_pHist[i])
		{
			bSuc = FALSE;
			break;
		}
		m_pHist[i]->Create(nLen);
	}

	if(!bSuc)
	{
		ClearAllMem();
		return FALSE;
	}

	m_nCount = nCount;
	return TRUE;
}

BOOL CCanvasInfo::ChangeHist(int nIdx, int nLen)
{
	ClearIdxMem(nIdx);
	m_pHist[nIdx] = new CData();
	return m_pHist[nIdx]->Create(nLen);
}

void CCanvasInfo::ClearAllMem()
{
	if(!m_pHist)
		return;

	for(int i = 0; i < m_nCount; ++i)
	{
		if(m_pHist[i])
			delete m_pHist[i];
	}

	delete [] m_pHist;
}

void CCanvasInfo::ClearIdxMem(int nIdx)
{
	ASSERT(nIdx < m_nCount || nIdx >= 0);

	if(!m_pHist || !m_pHist[nIdx])
		return;

	delete m_pHist[nIdx];
	m_pHist[nIdx] = NULL;
}

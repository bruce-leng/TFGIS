// EdgeFind.cpp: implementation of the CEdgeFind class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "canvas.h"
#include "EdgeFind.h"
#include "MathComput.h"
#include "Graph\Polygon.h"
#include "Progress.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define ANGLE_NUM 18

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEdgeFind::CEdgeFind()
{
	m_pCanvas = NULL;
}

CEdgeFind::CEdgeFind(CTilCanvas* pCanvas)
{
	m_pCanvas = pCanvas;
}

CEdgeFind::~CEdgeFind()
{

}

// ���ұ߽磬ע�⣺�˴�ֻ�����ֵϸ��ͼ��
BOOL CEdgeFind::FindEdge(const float& fDsumT, const float& fDminT,
						 const float& fAngleT, const float& fLengthT, int& nPosition)
{
	int i, j, m, n;

	CGraph* pGraph = m_pCanvas->GetGraph()->GetGraphByIdx(GRAPH_LAYER_JOINT);// ���ҵ��ı߽�ֻ�б���ɽ����
	CImage* pImage = m_pCanvas->GetImage()->GetCurImage();

	//  ���ý�����
// 	CProgress proDlg;
// 	proDlg.Create();
// 	proDlg.SetWindowText("�Ҳ�߽���ȡ");
// 	proDlg.m_progress.SetRange(0, 100);
// 	proDlg.m_progress.SetStep(1);
// 	proDlg.m_progress.SetPos(0);
// 	proDlg.OnInitDialog();
// 	proDlg.ShowWindow(SW_SHOWNORMAL);
	nPosition = 0;

	// ��ͼ�����ŵ�ĳ�̶���Χ�ڣ��ٲ���Canny���Ӽ��
	const int nFixWidth = 800;
	int nWidth = pImage->GetWidth();
	int nHeight = pImage->GetHeight();
	int nMax = max(nWidth,nHeight);
	float fRatio;
	if (nMax <= nFixWidth)
		fRatio = 1;
	else
		fRatio = nFixWidth*1.0/nMax;
	
	int nNewWidth = nWidth*fRatio;
	int nNewHeight = nHeight*fRatio;
	CImage* pImage1 = new CImage(*pImage);
	pImage1->Resample(nNewWidth, nNewHeight,2);
	nPosition = 2;
//	proDlg.m_progress.SetPos(2);

	pImage1->EdgeCanny();
	nPosition = 5;
//	proDlg.m_progress.SetPos(5);

	// ͼ��ϸ�����������ڵ�
	pImage1->MorphThining();
	nPosition = 13;
//	proDlg.m_progress.SetPos(13);

	ObjRunLenCode objRLC;
	pImage1->ObjAllRLC(&objRLC);
	nPosition = 15;
//	proDlg.m_progress.SetPos(15);
	delete pImage1;

	vector< vector< LINECAP> > lineArray;
	PPRUNLENGTHCODE_MGR ppRLCMgr = objRLC.GetObjAll(); 
	int nObjNum = objRLC.GetObjNum();
	for (i=0; i<nObjNum; i++)
	{
		CRect rect;
		objRLC.ObjRect(ppRLCMgr[i], rect);

		// ��ȡ�����߽���Ŀ��
		MATRIX* pMatrix = NULL;
		if (!CopyBuffer(ppRLCMgr[i], &pMatrix))
			return FALSE;

		// �ҵ�Ŀ��߽��
		deque<POINT> dPoints;
		FindEdgePoints(pMatrix, dPoints);

		// Ŀ������������
		vector<LINECAP> lineCaps;
		LinesFit(dPoints, 3, lineCaps);

		// ����߳���̫С�����޳�
		float fLength=0;
		for (j=0; j<lineCaps.size(); j++)
		{
			fLength += get_distance(lineCaps[j].pt1, lineCaps[j].pt2);
		}
//		if (fLength < 5)
//		{
//			delete pMatrix;
//			continue;
//		}

		// ��������ƽ�ƺ�ѹ������
		for (j=0; j<lineCaps.size(); j++)
		{
			lineCaps[j].pt1.x += rect.left-1;
			lineCaps[j].pt1.y += rect.top-1;
			lineCaps[j].pt2.x += rect.left-1;
			lineCaps[j].pt2.y += rect.top-1;
		}	
		lineArray.push_back(lineCaps);

		if (pMatrix)
			delete pMatrix;

		nPosition = 15 + 10 * i / nObjNum;
//		proDlg.m_progress.SetPos(15 + 10 * i / nObjNum);
	}

	// ��������Ŀ����бȽϷ���
	float fRelation;
	int nEnd1, nEnd2, nSum, nSum1, nSum2;
	vector< vector< Relation* > > vvRelation;
	nSum = lineArray.size();
	for (i=0; i<nSum; i++)
	{
		vector< Relation* > vRelation;
		vvRelation.push_back(vRelation);
	}

	for (i=0; i<nSum-1; i++)
	{
		vector<LINECAP> lineCaps1 = lineArray[i];
		for (j=i+1; j<nSum; j++)
		{
			vector<LINECAP> lineCaps2 = lineArray[j];
			if (BuildAdjacent(lineCaps1, lineCaps2, fDsumT, fDminT,
						fAngleT, fLengthT, nEnd1, nEnd2, fRelation))
			{
				// ��¼�໥��ϵ
				PRelation pRelation = new Relation;
				pRelation->fRelation = fRelation;
				pRelation->nLine1 = i;
				pRelation->nEnd1 = nEnd1;
				pRelation->nLine2 = j;
				pRelation->nEnd2 = nEnd2;
				vvRelation[i].push_back(pRelation);

				PRelation pRelation1 = new Relation;
				pRelation1->fRelation = fRelation;
				pRelation1->nLine1 = j;
				pRelation1->nEnd1 = nEnd2;
				pRelation1->nLine2 = i;
				pRelation1->nEnd2 = nEnd1;
				vvRelation[j].push_back(pRelation1);
			}
		} 
		nPosition = 25 + 49 * i / nSum;
//		proDlg.m_progress.SetPos(25 + 49 * i / nSum);
	}
	nPosition = 74;
//	proDlg.m_progress.SetPos(74);

	// ���߽��߸�������Ӧ��������͹���
	PRelation pTmp;
	for (i=0; i<nSum; i++)
	{
		nSum1 = vvRelation[i].size();

		// ���������ֵ���Ӵ�С����
		for (m=0; m<nSum1-1; m++)
		{
			for (n=m+1; n<nSum1; n++)
			{
				if (vvRelation[i][m]->fRelation < vvRelation[i][n]->fRelation)
				{
					pTmp			 = vvRelation[i][m];
					vvRelation[i][m] = vvRelation[i][n];
					vvRelation[i][n] = pTmp;
				}
			}
		}
	}
	nPosition = 75;
//	proDlg.m_progress.SetPos(75);

	//////////////////////////////////////////////////////////////////////////
	// ȷ��ͬһ�߽����ϸ������ڱ߽��ߵĶ�Ӧ��ϵ
	BOOL* pMark = new BOOL [nSum];
	memset(pMark, 0, sizeof(BOOL)*nSum);
	vector< deque< POINT > > vdPoints;
	for (i=0; i<nSum; i++)
	{
		// �����ǰ�߽��߱���ǹ���û����֮���Ӧ�ı߽磬�������һ���߽���
		if (pMark[i])
			continue;
		pMark[i] = TRUE;

		deque< POINT > dPoints;
		int nCurLine, nCurEnd, nLine1, nEnd1, nLine2, nEnd2;

		// �����ǰ�߽��������߽粻��������ֱ���˳�
		if (vvRelation[i].size() == 0)
		{
			dPoints.push_back(lineArray[i][0].pt1);
			for (j=0; j<lineArray[i].size(); j++)
			{
				dPoints.push_back(lineArray[i][j].pt2);
			}
			vdPoints.push_back(dPoints);
			continue;
		}

		// ���ҵ�i���߽��ߵ�����һ���˵����Ӧ�ı߽���
 		nCurLine = vvRelation[i][0]->nLine1;
		nCurEnd  = vvRelation[i][0]->nEnd1;
		nLine1 = nCurLine;
		nEnd1  = nCurEnd;

		// �洢��ǰ�߽��ߵĶ˵�
		if (nEnd1 == 0)	// �߽���ͷ
		{
			dPoints.push_front(lineArray[i][0].pt1);
			for (j=0; j<lineArray[i].size(); j++)
			{
				dPoints.push_front(lineArray[i][j].pt2);
			}
		}
		else // �߽���β
		{
			dPoints.push_back(lineArray[i][0].pt1);
			for (j=0; j<lineArray[i].size(); j++)
			{
				dPoints.push_back(lineArray[i][j].pt2);
			}
		}

		// �����뵱ǰ�߽��߶˵�nCurEnd���ڵ���һ�߽��ߣ�ֱ��û�����ڱ߽�
		while (FindNextNeighborEdge(vvRelation, nLine1, nEnd1, nLine2, nEnd2)
			&& !pMark[nLine2])
		{
			pMark[nLine2] = TRUE;

			if (nEnd2 == 0)	// �߽���ͷ
			{
				dPoints.push_back(lineArray[nLine2][0].pt1);
				for (j=0; j<lineArray[nLine2].size(); j++)
				{
					dPoints.push_back(lineArray[nLine2][j].pt2);
				}
			}
			else	// �߽���β
			{
				nSum2 = lineArray[nLine2].size();
				dPoints.push_back(lineArray[nLine2][nSum2-1].pt2);
				for (j=nSum2-1; j>=0; j--)
				{
					dPoints.push_back(lineArray[nLine2][j].pt1);
				}
			}

			nLine1 = nLine2;
			nEnd1  = 1-nEnd2;
		}

		// �����뵱ǰ�߽��߶˵�1-nCurEnd���ڵ���һ�߽��ߣ�ֱ��û�����ڱ߽�
		nLine1 = nCurLine;
		nEnd1  = 1-nCurEnd;
		while (FindNextNeighborEdge(vvRelation, nLine1, nEnd1, nLine2, nEnd2)
			&& !pMark[nLine2])
		{
			pMark[nLine2] = TRUE;
			
			if (nEnd2 == 0)	// �߽���ͷ
			{
				dPoints.push_front(lineArray[nLine2][0].pt1);
				for (j=0; j<lineArray[nLine2].size(); j++)
				{
					dPoints.push_front(lineArray[nLine2][j].pt2);
				}
			}
			else	// �߽���β
			{
				nSum2 = lineArray[nLine2].size();
				dPoints.push_front(lineArray[nLine2][nSum2-1].pt2);
				for (j=nSum2-1; j>=0; j--)
				{
					dPoints.push_front(lineArray[nLine2][j].pt1);
				}
			}

			nLine1 = nLine2;
			nEnd1  = 1-nEnd2;
		}

		vdPoints.push_back(dPoints);

		nPosition = 75 + 15 * i / nSum;
//		proDlg.m_progress.SetPos(75 + 15 * i / nSum);
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// �Ա߽��߽���ɸѡ���ҵ�����Ҫ��ı߽���
	// ���ݱ߽��߳��Ƚ���ɸѡ
	nSum = vdPoints.size();
	float fLength;
	vector< int > vBad;
	for (i=0; i<nSum; i++)
	{
		fLength = 0;
		nSum1 = vdPoints[i].size();
		for (j=0; j<nSum1-1; j++)
		{
			fLength += get_distance(vdPoints[i][j], vdPoints[i][j+1]);
		}

		// ���ݳ�����ֵ�ж��Ƿ����߽���,��¼���豣���߽�����
		if (fLength <= fLengthT)
			vBad.push_back(i);

		nPosition = 90 + 3 * i / nSum;
//		proDlg.m_progress.SetPos(90 + 3 * i / nSum);
	}

	// ɾ������Ҫ�����ı߽���
	nSum = vBad.size();
	for (i=nSum-1; i>=0;  i--)
	{
		vdPoints.erase(vdPoints.begin()+vBad[i]);
	}
	nPosition = 98;
//	proDlg.m_progress.SetPos(98);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// ���߽��ߴ洢��ͼ�β�
	POINT2DF pt;
	vector<vector<LINECAP> > vvLinesFit;

	nSum = vdPoints.size();
	for (i=0; i<nSum; i++)
	{
		CUnit* pUnit = (CUnit*)new CPolygon;
		pUnit->SetPolyClose(FALSE);

		vector<LINECAP> vLines;
		LinesFit(vdPoints[i], 3, vLines);
		vvLinesFit.push_back(vLines);

		pt.x = vLines[0].pt1.x/fRatio;
		pt.y = (nNewHeight-vLines[0].pt1.y-1)/fRatio;
		pUnit->m_pPointList->Add(pt);

		nSum1 = vLines.size();
		for (j=0; j<nSum1; j++)
		{
			pt.x = vLines[j].pt2.x/fRatio;
			pt.y = (nNewHeight-vLines[j].pt2.y-1)/fRatio;
			pUnit->m_pPointList->Add(pt);
		}
		pUnit->m_PenColor = RGB(255,0,0);
		pGraph->PushBackUnit(pUnit);

	}

	//////////////////////////////////////////////////////////////////////////
	// ͳ�ƽǶȷ�Χ
	int nNum[ANGLE_NUM];
	StatAngles(vvLinesFit, nNum);
	vector<int> vSum;
	StatLines(vvLinesFit, vSum);
	int nNumber = vSum.size();
	int* pNum = new int [nNumber];
	for (i=0; i<nNumber; i++)
	{
		pNum[i] = vSum[i];
		// TRACE("\n%d", pNum[i]);
	}
	delete [] pNum;
	nPosition = 99;
//	proDlg.m_progress.SetPos(99);
	//////////////////////////////////////////////////////////////////////////

	// ɾ��ָ��ָ����ڴ�
	for (i=0; i<vvRelation.size(); i++)
	{
		for (j=0; j<vvRelation[i].size(); j++)
		{
			delete vvRelation[i][j];
		}
	}
	delete [] pMark;

	nPosition = 100;
//	proDlg.m_progress.SetPos(100);
//	proDlg.DestroyWindow();

	return TRUE;
}

// ��Ŀ�꿽�����ڴ��У�Ŀ�꿽������չһ���߽�����
BOOL CEdgeFind::CopyBuffer(PRUNLENGTHCODE_MGR pRLCMgr, MATRIX** ppMatrix)
{
	int i, j;

	// ȡ����Ӿ���
	CRect rect;
	rect.left = pRLCMgr->RC[0].xStart;
	rect.right = pRLCMgr->RC[0].xEnd;
	rect.top = pRLCMgr->RC[0].y;
	rect.bottom = pRLCMgr->RC[pRLCMgr->nRunNum-1].y;
	for (i=0; i<pRLCMgr->nRunNum; i++)
	{
		if (rect.left > pRLCMgr->RC[i].xStart)
			rect.left = pRLCMgr->RC[i].xStart;
		if (rect.right < pRLCMgr->RC[i].xEnd)
			rect.right = pRLCMgr->RC[i].xEnd;
	}
	rect.right ++;
	rect.bottom ++;

	// ��չһ���߽粢�����ڴ�
	int nWidth, nHeight;
	nWidth = rect.Width()+2;
	nHeight = rect.Height()+2;
	ppMatrix[0] = (MATRIX*)new BYTE [sizeof(int)*2+sizeof(BYTE)*nWidth*nHeight];
	if (!ppMatrix[0])
		return FALSE;
	ppMatrix[0]->nWidth = nWidth;
	ppMatrix[0]->nHeight = nHeight;
	memset(ppMatrix[0]->elem, 0, sizeof(BYTE)*nWidth*nHeight);

	int xStart, xEnd, y;
	for (i=0; i<pRLCMgr->nRunNum; i++)
	{
		xStart = pRLCMgr->RC[i].xStart;
		xEnd = pRLCMgr->RC[i].xEnd;
		y = pRLCMgr->RC[i].y;

		for (j=xStart; j<=xEnd; j++)
		{
			ppMatrix[0]->elem[(y-rect.top+1)*nWidth+(j-rect.left+1)] = 255;
		}	
	}

	return TRUE;
}

// ����Ŀ��߽����ϵĵ�
void CEdgeFind::FindEdgePoints(MATRIX* pMatrix, deque<POINT>& dPoints)
{
	int i;
	int nWidth = pMatrix->nWidth;
	int nHeight = pMatrix->nHeight;
	BYTE* pElem = pMatrix->elem;

	//////////////////////////////////////////////////////////////////////////
	// ����8�����ƽ������
	CPoint move8[8];
	move8[0].x = 1, move8[0].y = 0;	// ��1��
	move8[1].x = 1, move8[1].y =-1;	// ��2��
	move8[2].x = 0, move8[2].y =-1;	// ��3��
	move8[3].x =-1, move8[3].y =-1;	// ��4��
	move8[4].x =-1, move8[4].y = 0;	// ��5��
	move8[5].x =-1, move8[5].y = 1;	// ��6��
	move8[6].x = 0, move8[6].y = 1;	// ��7��
	move8[7].x = 1, move8[7].y = 1;	// ��8��

	// �����ҵ���ǰ��ʱ�����ҵ�ǰ���8�����4��������ʼ�������·�ʽ����
	int nPrevOri8[] = {7,7,1,1,3,3,5,5};

	CPoint ptEnd, prevPt, curPt, nextPt;
	int nPrevOri, nNextOri;
	BOOL bClosed = IsClosed(pMatrix, ptEnd);

	// ��ǵ�һ��
	prevPt = ptEnd;
	dPoints.push_back(prevPt);

	// ����һ���㼰����
	for (i=5; i<5+8; i++)
	{
		nextPt = prevPt+move8[i%8];
		if (pElem[nextPt.y*nWidth+nextPt.x] == 255)
		{
			nNextOri = i%8;
			dPoints.push_back(nextPt);
			break;
		}
	}

	if (bClosed)
	{
		while (TRUE)
		{
			nPrevOri = nPrevOri8[nNextOri];
			
			// �ӵ�nPrevOri����ʼ����8�����
			for (i=nPrevOri; i<nPrevOri+8; i++)
			{
				nextPt = prevPt+move8[i%8];
				if (pElem[nextPt.y*nWidth+nextPt.x] == 255)
				{
					nNextOri = i%8;
					dPoints.push_back(nextPt);
					break;
				}
			}

			// ����һ��͵�һ����ͬʱ��˵���ѱ���һ��
			if (nextPt == ptEnd)
			{
				dPoints.pop_back();
				break;
			}
			
			// 
			prevPt = nextPt;
		}
	}
	else	// ������Ƿ������
	{
		curPt = nextPt;

		while (TRUE)
		{
			nPrevOri = nPrevOri8[nNextOri];
			
			// �ӵ�nPrevOri����ʼ����8�����
			for (i=nPrevOri; i<nPrevOri+8; i++)
			{
				nextPt = curPt+move8[i%8];
				if (pElem[nextPt.y*nWidth+nextPt.x] == 255)
				{
					nNextOri = i%8;
					dPoints.push_back(nextPt);
					break;
				}
			}
			
			// ����һ��͵�һ����ͬʱ��˵���ѱ�������һ���˵�
			if (nextPt == prevPt)
			{
				dPoints.pop_back();
				break;
			}

			// 
			prevPt = curPt;
			curPt = nextPt;
		}
	}
}

// �ж��Ƿ��Ƿ�ձ߽��ߣ������ǣ���ȡ��һ���߽�˵�
BOOL CEdgeFind::IsClosed(MATRIX* pMatrix, POINT& ptEnd)
{
	int i;
	CPoint prevPt, curPt, nextPt, startPt;
	int nPrevOri, nNextOri;

	int nWidth = pMatrix->nWidth;
	int nHeight = pMatrix->nHeight;
	BYTE* pElem = pMatrix->elem;

	// ����8�����ƽ������
	CPoint move8[8];
	move8[0].x = 1, move8[0].y = 0;	// ��1��
	move8[1].x = 1, move8[1].y =-1;	// ��2��
	move8[2].x = 0, move8[2].y =-1;	// ��3��
	move8[3].x =-1, move8[3].y =-1;	// ��4��
	move8[4].x =-1, move8[4].y = 0;	// ��5��
	move8[5].x =-1, move8[5].y = 1;	// ��6��
	move8[6].x = 0, move8[6].y = 1;	// ��7��
	move8[7].x = 1, move8[7].y = 1;	// ��8��
	
	// �����ҵ���ǰ��ʱ�����ҵ�ǰ���8�����4��������ʼ�������·�ʽ����
	int nPrevOri8[] = {7,7,1,1,3,3,5,5};

	// ��ǲ��ҵ��ĵ�һ��Ŀ�����ص�
	for (i=1; i<nWidth-1; i++)
	{
		if (pElem[nWidth+i] == 255)
		{
			startPt.x = i;
			startPt.y = 1;
			break;
		}	
	}

	// ����1�����긳�����ص������ֵ
	prevPt = startPt;

	// ����1�����8�����Ƿ���1��Ŀ��㣬���ǣ���õ�Ϊ�˵㣬ֱ���˳�
	int nNum = 0;
	for (i=0; i<8; i++)
	{
		CPoint ptTmp = prevPt+move8[i];
		if (pElem[ptTmp.y*nWidth+ptTmp.x] == 255)
			nNum ++;
	}	
	if (nNum == 1)
	{
		ptEnd = startPt;
		return FALSE;
	}

	// �ӵ�4�㿪ʼ���ҵ�point����һ��8�������ɫΪuObjClr�ĵ�
	for (i=5; i<5+8; i++)
	{
		curPt = prevPt+move8[i%8];
		if (pElem[curPt.y*nWidth+curPt.x] == 255)
		{
			nNextOri = i%8;
			break;
		}
	}

	while (TRUE)
	{
		nPrevOri = nPrevOri8[nNextOri];
		
		// �ӵ�nPrevOri����ʼ����8�����
		for (i=nPrevOri; i<nPrevOri+8; i++)
		{
			nextPt = curPt+move8[i%8];
			if (pElem[nextPt.y*nWidth+nextPt.x] == 255)
			{
				nNextOri = i%8;
				break;
			}
		}
		
		// ��ǰһ��ͺ�һ����ͬʱ��˵����ǰ��Ϊ�˵�
		if (nextPt == prevPt)
		{
			ptEnd = curPt;
			return FALSE;
		}
		else if (nextPt == startPt)	// �����һ�����һ����ͬ����Ϊ�������
		{
			ptEnd = nextPt;
			return TRUE;
		}	

		// 
		prevPt = curPt;
		curPt = nextPt;
	}

}

// �����ֱ�߼�н�
float CEdgeFind::GetAngle(const LINECAP& line1, const LINECAP& line2)
{
	float fRetAngle;
	if (fabs(line1.fAngle-line2.fAngle) < 90)
		fRetAngle = fabs(line1.fAngle-line2.fAngle);
	else
		fRetAngle = 180-fabs(line1.fAngle-line2.fAngle);

	return fRetAngle;
}

// �����ֱ�߼�˵㵽��һֱ�ߵľ����
float CEdgeFind::GetDistSum(const LINECAP& line1, const int& nIdx1,
									const LINECAP& line2, const int& nIdx2)
{
	float fD1, fD2;
	if (nIdx1==0)
		fD1 = point_line_dist(line1.pt1, line2.pt1, line2.pt2);
	else
		fD1 = point_line_dist(line1.pt2, line2.pt1, line2.pt2);

	if (nIdx2==0)
		fD2 = point_line_dist(line2.pt1, line1.pt1, line1.pt2);
	else
		fD2 = point_line_dist(line2.pt2, line1.pt1, line1.pt2);

	return (fD1+fD2);
}

// �ж���ֱ�ߵ�����һ���Ķ˵㵽��һ���߶εĴ����Ƿ����߶���
BOOL CEdgeFind::IsVertPtInLine(const LINECAP& line1, const LINECAP& line2)
{
	POINT2DF pt1_1, pt1_2, pt2_1, pt2_2;
	pt1_1.x = line1.pt1.x;	pt1_1.y = line1.pt1.y;
	pt1_2.x = line1.pt2.x;	pt1_2.y = line1.pt2.y;
	pt2_1.x = line2.pt1.x;	pt2_1.y = line2.pt1.y;
	pt2_2.x = line2.pt2.x;	pt2_2.y = line2.pt2.y;
	
	// �ж�L1���˵�a1,a2��L2�ϵĴ����Ƿ���L2�ϼ�L2���˵㵽L1�Ĵ����Ƿ���L1��
	POINT2DF ptVert;
	get_vert_point_in_line(pt1_1, pt2_1, pt2_2, ptVert);
	if (point_in_line(ptVert, pt2_1, pt2_2) == POINT_IN_LINE)
			return TRUE;

	get_vert_point_in_line(pt1_2, pt2_1, pt2_2, ptVert);
	if (point_in_line(ptVert, pt2_1, pt2_2) == POINT_IN_LINE)
		return TRUE;

	get_vert_point_in_line(pt2_1, pt1_1, pt1_2, ptVert);
	if (point_in_line(ptVert, pt1_1, pt1_2) == POINT_IN_LINE)
		return TRUE;

	get_vert_point_in_line(pt2_2, pt1_1, pt1_2, ptVert);
	if (point_in_line(ptVert, pt1_1, pt1_2) == POINT_IN_LINE)
		return TRUE;

	return FALSE;
}

// ������߶ζ˵�����С���룬����¼��С�����
// float& fMinDist: ������С����
// int& nIdx1, int& nIdx2: ��С��������˵�,0��ʾ��1���˵�,1��ʾ��2���˵�
float CEdgeFind::GetMinDist(const LINECAP& line1, const LINECAP& line2,
														float& fMinDist, int& nIdx1, int& nIdx2)
{
	float fD1, fD2, fD3, fD4;
	// ��L1��L2���߶ξ�����������˵��ľ���
	fD1 = get_distance(line1.pt1, line2.pt1);
	fD2 = get_distance(line1.pt1, line2.pt2);
	fD3 = get_distance(line1.pt2, line2.pt1);
	fD4 = get_distance(line1.pt2, line2.pt2);

	fMinDist = fD1;
	nIdx1 = 0;
	nIdx2 = 0;
	if (fMinDist>fD2)
	{
		fMinDist = fD2;
		nIdx1 = 0;
		nIdx2 = 1;
	}
	if (fMinDist>fD3)
	{
		fMinDist = fD3;
		nIdx1 = 1;
		nIdx2 = 0;
	}
	if (fMinDist>fD4)
	{
		fMinDist = fD4;
		nIdx1 = 1;
		nIdx2 = 1;
	}

	return fMinDist;
}

// �����������������߽��ߵ�����������
// const vector<LINECAP>& lineCaps1: �߽���1
// const vector<LINECAP>& lineCaps2: �߽���2
// const float& fDsumT, const float& fDminT: �����ж�������������ֵ
// const float& fAngleT, const float& fLengthT: �����ж�������������ֵ
// int& nEnd1, int&nEnd2: ���ر߽���1��2�Ķ˵����,0Ϊ��1���˵�,1Ϊ���һ���˵�
// float& fRelation: �����
// ���������ڵ�ɹ�����TRUE�����򷵻�FALSE
BOOL CEdgeFind::BuildAdjacent(
	   const vector<LINECAP>& lineCaps1,
	   const vector<LINECAP>& lineCaps2, 
	   const float& fDsumT, const float& fDminT,
	   const float& fAngleT, const float& fLengthT,
	   int& nEnd1, int& nEnd2, float& fRelation)
{
	int i, j, nIdx1, nIdx2;
	int nSegNum1, nSegNum2;
	float fDmin, fLen1, fLen2, fDsum, fAngle;
	nSegNum1 = lineCaps1.size();
	nSegNum2 = lineCaps2.size();

	int nNum1, nNum2;
	nNum1 = (nSegNum1>1)?2:1;
	nNum2 = (nSegNum2>1)?2:1;
	for (i=0; i<nNum1; i++)
	{
		nIdx1 = (i<1)?0:nSegNum1-1;
		LINECAP line1 = lineCaps1[nIdx1];
		for (j=0; j<nNum2; j++)
		{
			nIdx2 = (j<1)?0:nSegNum2-1;
			LINECAP line2 = lineCaps2[nIdx2];

			// �������߶ζ˵��ľ��룬�ҳ���С����Ͷ˵�
			GetMinDist(line1, line2, fDmin, nEnd1, nEnd2);
			fLen1 = get_distance(line1.pt1, line1.pt2);
			fLen2 = get_distance(line2.pt1, line2.pt2);
			// ��fDmin���ڸ�����ֵ�����line1��line2�ĳ���,�򷵻�
			if (fDmin>fDminT || (fDmin>0.75*fLen1 && fDmin>0.75*fLen2))
				return FALSE;

			if (nNum1 > 1)	// ����1������߲�ֹ1��
			{
				if (i == 0)
				{
					if (nNum2 > 1)
					{
						if (j == 0)
						{
							if (nEnd1!=0 || nEnd2!=0)
								continue;
						}
						else
						{
							if (nEnd1!=0 || nEnd2!=1)
								continue;
						}
					}
					else
					{
						if (nEnd1!=0)
							continue;
					}
				}
				else	// i==1
				{
					if (nNum2 > 1)
					{
						if (j == 0)
						{
							if (nEnd1!=1 || nEnd2!=0)
								continue;
						}
						else
						{
							if (nEnd1!=1 || nEnd2!=1)
								continue;
						}
					}
					else
					{
						if (nEnd1!=1)
							continue;
					}
				}
			}
			else	// nNum==1
			{
				if (nNum2 > 1)
				{
					if (j == 0)
					{
						if (nEnd2 != 0)
							continue;
					}
					else
					{
						if (nEnd2 != 1)
							continue;
					}
				}
			}

			// ������ֱ�߼�һ��ֱ�����˵㵽��һ��ֱ�ߵľ���
			fDsum = GetDistSum(line1, nEnd1, line2, nEnd2);
			// ��fDsumС�ڸ�����ֵ�������
			if (fDsum > fDsumT)
				return FALSE;

			// �������߶μ�ļн�
			fAngle = GetAngle(line1, line2);
			// ��fAС�ڸ�����ֵ�������
			if (fAngle > fAngleT)
				return FALSE;

			// ����line1�Ķ˵�nEnd1��line2�˵�nEnd2��������line1��ļн�
#if 0
			POINT pt0, pt1, pt2;
			pt0 = (nEnd1==0)?line1.pt2:line1.pt1;
			pt1 = (nEnd1==0)?line1.pt1:line1.pt2;
			pt2 = (nEnd2==0)?line2.pt1:line2.pt2;
			fAngle1 = get_angle(pt0,pt1,pt2);
			fAngle1 = 180-fAngle1*180/PI;
			if (fAngle1 > 45)
				return FALSE;
#endif
			
			// �ж�line1�Ƿ���line2���ӳ����ϣ�ͬʱline2�Ƿ���line2���ӳ�����
			if (IsVertPtInLine(line1, line2))
				return FALSE;
			
			// �������߽��ߵ�����ԣ�ϵ��ѡ��������д���֤
			fRelation = 2/fDsum+1/fDmin+1/(fAngle+10);
			
			return TRUE;
		}
	}	

	return FALSE;
}

// �����뵱ǰ�߽�����������һ���߽���
// const CArray<LINECAP, LINECAP>& lineCaps : ԭʼ�߽���
// const vector< vector< Relation* > >& vvRelation : �߽��߼���
// const int& nLine1, const int& nEnd1 : �Ѿ��߽��߼��˵�
// int& nLine2, int& nEnd2 : ��������֪�߽��������ı߽��ߺͶ˵�
BOOL CEdgeFind::FindNextNeighborEdge( 
				  const vector< vector< Relation* > >& vvRelation, 
				  const int& nLine1, const int& nEnd1,
				  int& nLine2, int& nEnd2)
{
	int i, nSum1, nSum2, nEndIdx;

	// ���ҵ�1��������nEnd1��������һ���߽���
	nSum1 = vvRelation[nLine1].size();
	for (i=0; i<nSum1; i++)
	{
		if (vvRelation[nLine1][i]->nEnd1 == nEnd1)
		{
			nLine2 = vvRelation[nLine1][i]->nLine2;
			nEnd2  = vvRelation[nLine1][i]->nEnd2;
			break;
		}
	}
	if (i == nSum1)
		return FALSE;

	// ����nLine2�ĵ�һ���˵��Ƿ���nLine1����
	nEndIdx = vvRelation[nLine2][0]->nEnd2;
	nSum2 = vvRelation[nLine2].size();
	if ((vvRelation[nLine2][0]->nLine2==nLine1)	&& (nEndIdx==nEnd1))
	{
		return TRUE;
	}
	else
	{
		// �����һ����¼������nLine1����һ�£��п�������ΪnLine2�ĵ�һ��
		// ��¼�洢������һ���˵��������߽��������ӵ���Ϣ���ʲ��Һ������
		// ��һ���˵㲻ͬ�ļ�¼�����Ƿ�����һ�����߽������ӵ���Ϣ
		nEndIdx = vvRelation[nLine2][0]->nEnd1;

		for (i=1; i<nSum2; i++)
		{
			// ��������¼�ĵ�nLine2�Ķ˵����1����¼�Ķ˵���ͬ�������
			if (vvRelation[nLine2][i]->nEnd1 == nEndIdx)
				continue;
			
			if ((vvRelation[nLine2][i]->nLine2==nLine1)
				&& (vvRelation[nLine2][i]->nEnd2==nEnd1))
			{
				return TRUE;
			}
			else // ������Ӧ��һ���߽磬������ѭ��
			{
				break;
			}
		}
		// �������һ���߽���������������ѭ��
		return FALSE;
	}
}

// ������С���˷��Զ���߽����������
LINECAP CEdgeFind::LineFit(vector<LINECAP>& vLine)
{
	int i, nSum;
	float f_xSum, f_ySum, f_xxSum, f_xySum;
	LINECAP line;

	// �ṹ�����������������
	nSum = vLine.size();
	f_xSum = f_ySum = f_xySum = f_xxSum = 0.0;
	f_xSum = vLine[0].pt1.x;
	f_ySum = vLine[0].pt1.y;
	f_xxSum = vLine[0].pt1.x*vLine[0].pt1.x;
	f_xySum = vLine[0].pt1.x*vLine[0].pt1.y;
	for (i=0; i<nSum; i++)
	{
		f_xSum += vLine[i].pt2.x;
		f_ySum += vLine[i].pt2.y;
		f_xySum += vLine[i].pt2.x*vLine[i].pt2.y;
		f_xxSum += vLine[i].pt2.x*vLine[i].pt2.x;
	}

	if (abs(f_xSum*f_xSum - nSum*f_xxSum) > 0.000001)
	{
		// �ṹ����������Է���б��
		float fK = (f_xSum*f_ySum - nSum*f_xySum)/(f_xSum*f_xSum - nSum*f_xxSum);

		// �ṹ�����			
		line.fAngle = atan(fK)*180/PI;
	}
	// �ṹ�����
	else
		line.fAngle = 90;

	// ע�⣺�˴�ֱ��ȡ�����˵���Ϊ������Ϻ�Ķ˵�
	line.pt1.x = vLine[0].pt1.x;
	line.pt1.y = vLine[0].pt1.y;
	line.pt2.x = vLine[nSum-1].pt2.x;
	line.pt2.y = vLine[nSum-1].pt2.y;

	return line;
}

// �Զ����ɵĶ�ν����������
void CEdgeFind::LinesFit(const deque<POINT>& dPoints, const float& fDist, vector<LINECAP>& vLines)
{
	int nIdx1, nIdx2;
	POINT pt, pt1, pt2;
	LINECAP line;
	int nSum = dPoints.size();
	nIdx1 = 0;
	nIdx2 = 1;
	pt1 = dPoints[0];
	line.pt1 = pt1;
	while (nIdx2<nSum)
	{
		pt2 = dPoints[nIdx2];

		for (int i=nIdx1+1; i<=nIdx2-1; i++)
		{
			pt = dPoints[i];
			if (point_line_dist(pt, pt1, pt2) > fDist)
			{
				line.pt2 = dPoints[nIdx2-1];
				line.fAngle = get_angle(line.pt1, line.pt2);
				vLines.push_back(line);

				line.pt1 = line.pt2;
				pt1 = line.pt1;
				nIdx1 = nIdx2-1;
				break;
			}
		}

		nIdx2++;
	}

	// ���һ��
	line.pt2 = dPoints[nIdx2-1];
	line.fAngle = get_angle(line.pt1, line.pt2);
	vLines.push_back(line);
}

// ͳ�ƽǶȷ�Χֵ
// const vector<vector<LINECAP>>& vvLines: �߽���
// int* pNum: ���ֵ����18��Ԫ�أ���Ϊ��180��ƽ��Ϊ18�ݣ�ÿ��10��
// ͳ��ÿ10�ȷ�Χ�ڵı߽�������
void CEdgeFind::StatAngles(const vector<vector<LINECAP> >& vvLines, int* pNum)
{
	memset(pNum, 0, sizeof(int)*ANGLE_NUM);

	int i, j, nSum, nSum1;
	nSum = vvLines.size();

#define CLOSE 0
	// ȡ��������ζ����
#if CLOSE
	CRgn rgn;
	CGraph* pGraph = m_pCanvas->GetGraph()->GetGraphByIdx(GRAPH_LAYER_TUNARC);
	CUnit* pUnit = pGraph->GetUnitByIdx(0);
	int nNum = pUnit->m_pPointList->GetSize();
	POINT* pPoints = new POINT[nNum];
	for (i=0; i<nNum; i++)
	{
		POINT2DF pt = pUnit->m_pPointList->ElementAt(i);
		pPoints[i].x = pt.x;
		pPoints[i].y = pt.y;
	}
	rgn.CreatePolygonRgn(pPoints, nNum, WINDING);
#endif

	for (i=0; i<nSum; i++)
	{
		nSum1 = vvLines[i].size();
		for (j=0; j<nSum1; j++)
		{
			LINECAP line = vvLines[i][j];
#ifdef CLOSE
//			if (rgn.PtInRegion(line.pt1) && rgn.PtInRegion(line.pt2))
#endif
				pNum[int(line.fAngle/(180/ANGLE_NUM))] ++;
		}
	}
}

// ͳ�Ʊ߽���
void CEdgeFind::StatLines(const vector<vector<LINECAP> >& vvLines, vector<int>& vSum)
{
	int i, j, k;
	int nLines, nLinePts;
	float fLength;

	nLines = vvLines.size();
	for (i=0; i<nLines; i++)
	{
		fLength = 0;
		nLinePts = vvLines[i].size();
		for (j=0; j<nLinePts; j++)
		{
			fLength += get_distance(vvLines[i][j].pt1, vvLines[i][j].pt2);
		}

		// �ж��Ƿ�û�е�ǰfLength������ͳ��ֵ
		k = fLength/10;
		if (vSum.size()==k)
		{
			vSum.push_back(1);
		}
		else if (vSum.size()<k)
		{
			for (int m=vSum.size(); m<k; m++)
				vSum.push_back(0);
			vSum.push_back(1);
		}
		else
			vSum[k] ++;
	}
}

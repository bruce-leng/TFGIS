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

// 查找边界，注意：此处只处理二值细化图像
BOOL CEdgeFind::FindEdge(const float& fDsumT, const float& fDminT,
						 const float& fAngleT, const float& fLengthT, int& nPosition)
{
	int i, j, m, n;

	CGraph* pGraph = m_pCanvas->GetGraph()->GetGraphByIdx(GRAPH_LAYER_JOINT);// 查找到的边界只有保存成节理层
	CImage* pImage = m_pCanvas->GetImage()->GetCurImage();

	//  设置进度条
// 	CProgress proDlg;
// 	proDlg.Create();
// 	proDlg.SetWindowText("岩层边界提取");
// 	proDlg.m_progress.SetRange(0, 100);
// 	proDlg.m_progress.SetStep(1);
// 	proDlg.m_progress.SetPos(0);
// 	proDlg.OnInitDialog();
// 	proDlg.ShowWindow(SW_SHOWNORMAL);
	nPosition = 0;

	// 将图像缩放到某固定范围内，再采用Canny算子检测
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

	// 图像细化并清除交叉节点
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

		// 提取单个边界线目标
		MATRIX* pMatrix = NULL;
		if (!CopyBuffer(ppRLCMgr[i], &pMatrix))
			return FALSE;

		// 找到目标边界点
		deque<POINT> dPoints;
		FindEdgePoints(pMatrix, dPoints);

		// 目标进行线性拟合
		vector<LINECAP> lineCaps;
		LinesFit(dPoints, 3, lineCaps);

		// 如果线长度太小，则剔除
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

		// 加上坐标平移后压入序列
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

	// 对所有线目标进行比较分析
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
				// 记录相互关系
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

	// 将边界线根据其相应属性排序和归类
	PRelation pTmp;
	for (i=0; i<nSum; i++)
	{
		nSum1 = vvRelation[i].size();

		// 根据相关性值按从大到小排序
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
	// 确立同一边界线上各段相邻边界线的对应关系
	BOOL* pMark = new BOOL [nSum];
	memset(pMark, 0, sizeof(BOOL)*nSum);
	vector< deque< POINT > > vdPoints;
	for (i=0; i<nSum; i++)
	{
		// 如果当前边界线被标记过或没有与之相对应的边界，则分析下一条边界线
		if (pMark[i])
			continue;
		pMark[i] = TRUE;

		deque< POINT > dPoints;
		int nCurLine, nCurEnd, nLine1, nEnd1, nLine2, nEnd2;

		// 如果当前边界与其它边界不相连，则直接退出
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

		// 查找第i条边界线的其中一个端点相对应的边界线
 		nCurLine = vvRelation[i][0]->nLine1;
		nCurEnd  = vvRelation[i][0]->nEnd1;
		nLine1 = nCurLine;
		nEnd1  = nCurEnd;

		// 存储当前边界线的端点
		if (nEnd1 == 0)	// 边界线头
		{
			dPoints.push_front(lineArray[i][0].pt1);
			for (j=0; j<lineArray[i].size(); j++)
			{
				dPoints.push_front(lineArray[i][j].pt2);
			}
		}
		else // 边界线尾
		{
			dPoints.push_back(lineArray[i][0].pt1);
			for (j=0; j<lineArray[i].size(); j++)
			{
				dPoints.push_back(lineArray[i][j].pt2);
			}
		}

		// 查找与当前边界线端点nCurEnd相邻的下一边界线，直至没有相邻边界
		while (FindNextNeighborEdge(vvRelation, nLine1, nEnd1, nLine2, nEnd2)
			&& !pMark[nLine2])
		{
			pMark[nLine2] = TRUE;

			if (nEnd2 == 0)	// 边界线头
			{
				dPoints.push_back(lineArray[nLine2][0].pt1);
				for (j=0; j<lineArray[nLine2].size(); j++)
				{
					dPoints.push_back(lineArray[nLine2][j].pt2);
				}
			}
			else	// 边界线尾
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

		// 查找与当前边界线端点1-nCurEnd相邻的下一边界线，直至没有相邻边界
		nLine1 = nCurLine;
		nEnd1  = 1-nCurEnd;
		while (FindNextNeighborEdge(vvRelation, nLine1, nEnd1, nLine2, nEnd2)
			&& !pMark[nLine2])
		{
			pMark[nLine2] = TRUE;
			
			if (nEnd2 == 0)	// 边界线头
			{
				dPoints.push_front(lineArray[nLine2][0].pt1);
				for (j=0; j<lineArray[nLine2].size(); j++)
				{
					dPoints.push_front(lineArray[nLine2][j].pt2);
				}
			}
			else	// 边界线尾
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
	// 对边界线进行筛选，找到符合要求的边界线
	// 根据边界线长度进行筛选
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

		// 根据长度阈值判断是否保留边界线,记录不予保留边界的序号
		if (fLength <= fLengthT)
			vBad.push_back(i);

		nPosition = 90 + 3 * i / nSum;
//		proDlg.m_progress.SetPos(90 + 3 * i / nSum);
	}

	// 删除不需要保留的边界线
	nSum = vBad.size();
	for (i=nSum-1; i>=0;  i--)
	{
		vdPoints.erase(vdPoints.begin()+vBad[i]);
	}
	nPosition = 98;
//	proDlg.m_progress.SetPos(98);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 将边界线存储至图形层
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
	// 统计角度范围
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

	// 删除指针指向的内存
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

// 将目标拷贝至内存中，目标拷贝后扩展一个边界像素
BOOL CEdgeFind::CopyBuffer(PRUNLENGTHCODE_MGR pRLCMgr, MATRIX** ppMatrix)
{
	int i, j;

	// 取得外接矩形
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

	// 扩展一个边界并分配内存
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

// 查找目标边界线上的点
void CEdgeFind::FindEdgePoints(MATRIX* pMatrix, deque<POINT>& dPoints)
{
	int i;
	int nWidth = pMatrix->nWidth;
	int nHeight = pMatrix->nHeight;
	BYTE* pElem = pMatrix->elem;

	//////////////////////////////////////////////////////////////////////////
	// 定义8邻域的平移坐标
	CPoint move8[8];
	move8[0].x = 1, move8[0].y = 0;	// 第1点
	move8[1].x = 1, move8[1].y =-1;	// 第2点
	move8[2].x = 0, move8[2].y =-1;	// 第3点
	move8[3].x =-1, move8[3].y =-1;	// 第4点
	move8[4].x =-1, move8[4].y = 0;	// 第5点
	move8[5].x =-1, move8[5].y = 1;	// 第6点
	move8[6].x = 0, move8[6].y = 1;	// 第7点
	move8[7].x = 1, move8[7].y = 1;	// 第8点

	// 当查找到当前点时，查找当前点的8邻域和4邻域点的起始方向按如下方式进行
	int nPrevOri8[] = {7,7,1,1,3,3,5,5};

	CPoint ptEnd, prevPt, curPt, nextPt;
	int nPrevOri, nNextOri;
	BOOL bClosed = IsClosed(pMatrix, ptEnd);

	// 标记第一点
	prevPt = ptEnd;
	dPoints.push_back(prevPt);

	// 找下一个点及方向
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
			
			// 从第nPrevOri方向开始查找8邻域点
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

			// 当下一点和第一点相同时，说明已遍历一周
			if (nextPt == ptEnd)
			{
				dPoints.pop_back();
				break;
			}
			
			// 
			prevPt = nextPt;
		}
	}
	else	// 如果不是封闭曲线
	{
		curPt = nextPt;

		while (TRUE)
		{
			nPrevOri = nPrevOri8[nNextOri];
			
			// 从第nPrevOri方向开始查找8邻域点
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
			
			// 当下一点和第一点相同时，说明已遍历至后一个端点
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

// 判断是否是封闭边界线，若不是，则取得一个边界端点
BOOL CEdgeFind::IsClosed(MATRIX* pMatrix, POINT& ptEnd)
{
	int i;
	CPoint prevPt, curPt, nextPt, startPt;
	int nPrevOri, nNextOri;

	int nWidth = pMatrix->nWidth;
	int nHeight = pMatrix->nHeight;
	BYTE* pElem = pMatrix->elem;

	// 定义8邻域的平移坐标
	CPoint move8[8];
	move8[0].x = 1, move8[0].y = 0;	// 第1点
	move8[1].x = 1, move8[1].y =-1;	// 第2点
	move8[2].x = 0, move8[2].y =-1;	// 第3点
	move8[3].x =-1, move8[3].y =-1;	// 第4点
	move8[4].x =-1, move8[4].y = 0;	// 第5点
	move8[5].x =-1, move8[5].y = 1;	// 第6点
	move8[6].x = 0, move8[6].y = 1;	// 第7点
	move8[7].x = 1, move8[7].y = 1;	// 第8点
	
	// 当查找到当前点时，查找当前点的8邻域和4邻域点的起始方向按如下方式进行
	int nPrevOri8[] = {7,7,1,1,3,3,5,5};

	// 标记查找到的第一个目标像素点
	for (i=1; i<nWidth-1; i++)
	{
		if (pElem[nWidth+i] == 255)
		{
			startPt.x = i;
			startPt.y = 1;
			break;
		}	
	}

	// 将第1点坐标赋给返回点的坐标值
	prevPt = startPt;

	// 看第1个点的8邻域是否有1个目标点，若是，则该点为端点，直接退出
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

	// 从第4点开始查找点point的下一个8邻域的颜色为uObjClr的点
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
		
		// 从第nPrevOri方向开始查找8邻域点
		for (i=nPrevOri; i<nPrevOri+8; i++)
		{
			nextPt = curPt+move8[i%8];
			if (pElem[nextPt.y*nWidth+nextPt.x] == 255)
			{
				nNextOri = i%8;
				break;
			}
		}
		
		// 当前一点和后一点相同时，说明当前点为端点
		if (nextPt == prevPt)
		{
			ptEnd = curPt;
			return FALSE;
		}
		else if (nextPt == startPt)	// 如果下一点与第一点相同，则为封闭曲线
		{
			ptEnd = nextPt;
			return TRUE;
		}	

		// 
		prevPt = curPt;
		curPt = nextPt;
	}

}

// 获得两直线间夹角
float CEdgeFind::GetAngle(const LINECAP& line1, const LINECAP& line2)
{
	float fRetAngle;
	if (fabs(line1.fAngle-line2.fAngle) < 90)
		fRetAngle = fabs(line1.fAngle-line2.fAngle);
	else
		fRetAngle = 180-fabs(line1.fAngle-line2.fAngle);

	return fRetAngle;
}

// 获得两直线间端点到另一直线的距离和
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

// 判断两直线的其中一条的端点到另一条线段的垂足是否在线段上
BOOL CEdgeFind::IsVertPtInLine(const LINECAP& line1, const LINECAP& line2)
{
	POINT2DF pt1_1, pt1_2, pt2_1, pt2_2;
	pt1_1.x = line1.pt1.x;	pt1_1.y = line1.pt1.y;
	pt1_2.x = line1.pt2.x;	pt1_2.y = line1.pt2.y;
	pt2_1.x = line2.pt1.x;	pt2_1.y = line2.pt1.y;
	pt2_2.x = line2.pt2.x;	pt2_2.y = line2.pt2.y;
	
	// 判断L1两端点a1,a2到L2上的垂足是否在L2上及L2两端点到L1的垂足是否在L1上
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

// 获得两线段端点间的最小距离，并记录最小距离点
// float& fMinDist: 返回最小距离
// int& nIdx1, int& nIdx2: 最小距离的两端点,0表示第1个端点,1表示第2个端点
float CEdgeFind::GetMinDist(const LINECAP& line1, const LINECAP& line2,
														float& fMinDist, int& nIdx1, int& nIdx2)
{
	float fD1, fD2, fD3, fD4;
	// 求L1与L2两线段距离最近的两端点间的距离
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

// 查找满足条件的两边界线的相连的两点
// const vector<LINECAP>& lineCaps1: 边界线1
// const vector<LINECAP>& lineCaps2: 边界线2
// const float& fDsumT, const float& fDminT: 用于判定符合条件的阈值
// const float& fAngleT, const float& fLengthT: 用于判定符合条件的阈值
// int& nEnd1, int&nEnd2: 返回边界线1和2的端点序号,0为第1个端点,1为最后一个端点
// float& fRelation: 相关性
// 若连接相邻点成功返回TRUE，否则返回FALSE
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

			// 计算两线段端点间的距离，找出最小距离和端点
			GetMinDist(line1, line2, fDmin, nEnd1, nEnd2);
			fLen1 = get_distance(line1.pt1, line1.pt2);
			fLen2 = get_distance(line2.pt1, line2.pt2);
			// 若fDmin大于给定阈值或大于line1和line2的长度,则返回
			if (fDmin>fDminT || (fDmin>0.75*fLen1 && fDmin>0.75*fLen2))
				return FALSE;

			if (nNum1 > 1)	// 若第1个多段线不止1段
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

			// 计算两直线间一条直线两端点到另一条直线的距离
			fDsum = GetDistSum(line1, nEnd1, line2, nEnd2);
			// 若fDsum小于给定阈值，则继续
			if (fDsum > fDsumT)
				return FALSE;

			// 计算两线段间的夹角
			fAngle = GetAngle(line1, line2);
			// 若fA小于给定阈值，则继续
			if (fAngle > fAngleT)
				return FALSE;

			// 计算line1的端点nEnd1与line2端点nEnd2的连线与line1间的夹角
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
			
			// 判断line1是否在line2的延长线上，同时line2是否在line2的延长线上
			if (IsVertPtInLine(line1, line2))
				return FALSE;
			
			// 计算两边界线的相关性，系数选择合理性有待验证
			fRelation = 2/fDsum+1/fDmin+1/(fAngle+10);
			
			return TRUE;
		}
	}	

	return FALSE;
}

// 查找与当前边界线相连的下一条边界线
// const CArray<LINECAP, LINECAP>& lineCaps : 原始边界线
// const vector< vector< Relation* > >& vvRelation : 边界线集合
// const int& nLine1, const int& nEnd1 : 已经边界线及端点
// int& nLine2, int& nEnd2 : 待求与已知边界线相连的边界线和端点
BOOL CEdgeFind::FindNextNeighborEdge( 
				  const vector< vector< Relation* > >& vvRelation, 
				  const int& nLine1, const int& nEnd1,
				  int& nLine2, int& nEnd2)
{
	int i, nSum1, nSum2, nEndIdx;

	// 查找第1条可能与nEnd1相连的下一条边界线
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

	// 查找nLine2的第一个端点是否与nLine1相连
	nEndIdx = vvRelation[nLine2][0]->nEnd2;
	nSum2 = vvRelation[nLine2].size();
	if ((vvRelation[nLine2][0]->nLine2==nLine1)	&& (nEndIdx==nEnd1))
	{
		return TRUE;
	}
	else
	{
		// 如果第一条记录不能与nLine1的相一致，有可能是因为nLine2的第一条
		// 记录存储的是另一个端点与其它边界线相连接的信息，故查找后面的与
		// 第一个端点不同的记录，看是否有与一第条边界相连接的信息
		nEndIdx = vvRelation[nLine2][0]->nEnd1;

		for (i=1; i<nSum2; i++)
		{
			// 如果后面记录的第nLine2的端点与第1条记录的端点相同，则继续
			if (vvRelation[nLine2][i]->nEnd1 == nEndIdx)
				continue;
			
			if ((vvRelation[nLine2][i]->nLine2==nLine1)
				&& (vvRelation[nLine2][i]->nEnd2==nEnd1))
			{
				return TRUE;
			}
			else // 若不对应第一条边界，则跳出循环
			{
				break;
			}
		}
		// 若不与第一条边界线相连，则跳出循环
		return FALSE;
	}
}

// 利用最小二乘法对多段线进行线性拟合
LINECAP CEdgeFind::LineFit(vector<LINECAP>& vLine)
{
	int i, nSum;
	float f_xSum, f_ySum, f_xxSum, f_xySum;
	LINECAP line;

	// 结构面线性拟合特征数据
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
		// 结构面所拟合线性方程斜率
		float fK = (f_xSum*f_ySum - nSum*f_xySum)/(f_xSum*f_xSum - nSum*f_xxSum);

		// 结构面倾角			
		line.fAngle = atan(fK)*180/PI;
	}
	// 结构面倾角
	else
		line.fAngle = 90;

	// 注意：此处直接取两个端点作为线性拟合后的端点
	line.pt1.x = vLine[0].pt1.x;
	line.pt1.y = vLine[0].pt1.y;
	line.pt2.x = vLine[nSum-1].pt2.x;
	line.pt2.y = vLine[nSum-1].pt2.y;

	return line;
}

// 对多点组成的多段进行线性拟合
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

	// 最后一段
	line.pt2 = dPoints[nIdx2-1];
	line.fAngle = get_angle(line.pt1, line.pt2);
	vLines.push_back(line);
}

// 统计角度范围值
// const vector<vector<LINECAP>>& vvLines: 边界线
// int* pNum: 输出值，共18个元素，意为将180度平分为18份，每份10度
// 统计每10度范围内的边界线条数
void CEdgeFind::StatAngles(const vector<vector<LINECAP> >& vvLines, int* pNum)
{
	memset(pNum, 0, sizeof(int)*ANGLE_NUM);

	int i, j, nSum, nSum1;
	nSum = vvLines.size();

#define CLOSE 0
	// 取得隧道拱形多边形
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

// 统计边界线
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

		// 判断是否没有当前fLength的数量统计值
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

// TilComFunction.cpp: implementation of the CTilComFunction class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TilComFunction.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 由路径及文件名得到文件路径
CString get_file_path(LPCTSTR lpszPathName)
{
	CString strFileName = get_file_name_ext(lpszPathName);
	CString strPathname = lpszPathName;
	return strPathname.Left(strPathname.GetLength()-strFileName.GetLength()-1);
}
CString get_file_name_ext(LPCTSTR lpszPathName)
{
	CString strPathName = lpszPathName;
	int nIdx, nSum;

	nSum = strPathName.GetLength();
	for (int i = nSum-1; i >= 0; i --)
	{
		if (strPathName.GetAt(i) == '\\')
		{
			nIdx = i;
			break;
		}
	}

	return strPathName.Right(nSum-nIdx-1);
}

CString get_file_name(LPCTSTR lpszPathName)
{
	CString strPathName = lpszPathName;
	int nIdx, nSum;

	nSum = strPathName.GetLength();
	for (int i = nSum-1; i >= 0; i --)
	{
		if (strPathName.GetAt(i) == '\\')
		{
			nIdx = i;
			break;
		}
	}

	return strPathName.Mid(nIdx+1, nSum-5-nIdx);
}

CString get_file_ext(LPCTSTR lpszPathName)
{
	CString strPathName = lpszPathName;
	int nIdx, nSum;

	nSum = strPathName.GetLength();
	for (int i = nSum-1; i >= 0; i --)
	{
		if (strPathName.GetAt(i) == '.')
		{
			nIdx = i;
			break;
		}
	}

	return strPathName.Right(nSum-nIdx-1);
}

// 判断是否是一个文件路径
// 返回0表示不是路径,1为相对路径，2为绝对路径
int is_path_name(const CString& strPathName)
{
	if (strPathName.GetAt(strPathName.GetLength()-4) != '.')
		return 0;

	int i, nIdx;
	for (i = 0; i < strPathName.GetLength(); i ++)
	{
		if (strPathName.GetAt(i) == '\\')
		{
			nIdx = i;
			break;
		}
	}
	// 如果没找到要目录中的'\'则不是路径
	if (i == strPathName.GetLength())
		return 0;

	// 如果要目录不是形如D:或..的含有两个字符的情况，则不是路径
	if (nIdx > 2)		return 0;

	if (strPathName.GetAt(1) == ':')
	{
		char ch = strPathName.GetAt(0);
		if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
			return 2;
	}
	else if (strPathName.Left(2) == "..")
		return 1;

	return 0;
}

// 检测是否是标准里程
BOOL is_std_mileage(const CString& strSrcPathName)
{
	CString strFileName = get_file_name(strSrcPathName);
	strFileName.MakeLower();

	int i;
	int nIdx0 = strFileName.Find('k');
	int nIdx = strFileName.Find('+');
	// 如果k与+之间间隔不大于1,说明k与+之间没有其它字符,格式错误
	if (nIdx-nIdx0 <= 1)
		return FALSE;
	// 判断k和'+'之间是否是数字
	for (i = nIdx0+1; i < nIdx; i ++)
	{
		char ch = strFileName.GetAt(i);
		if (ch < '0' || ch > '9')
			return FALSE;
	}
	// 判断'+'后面的是否是数字
	for (i = nIdx+1; i < strFileName.GetLength(); i ++)
	{
		char ch = strFileName.GetAt(i);
		if ((ch < '0' || ch > '9') && (ch != '.'))
			return FALSE;
	}
	// 判断'+'后面的数是否小于999
	long tmp = atoi(strFileName.Right(strFileName.GetLength()-nIdx-1));
	if (tmp > 999)
		return FALSE;

	return TRUE;
}

// 由路径得到文件里程前缀
CString get_std_dk(const CString& strSrcPathName)
{
	CString strFileName = get_file_name(strSrcPathName);
	CString strTemp = strFileName;
	strTemp.MakeLower();
	int nIdx = strTemp.Find('k');
	return strFileName.Left(nIdx+1);

}

// 根据文件名得到切片拍摄的里程
float get_std_mileage(const CString& strSrcPathName)
{
	CString strFileName = get_file_name(strSrcPathName);
	int nIdx1 = strFileName.Find('+');

	strFileName.MakeLower();
	int nIdx0 = strFileName.Find('k');
	float fDK = atoi(strFileName.Mid(nIdx0+1, nIdx1-nIdx0-1));
	fDK *= 1000;
	fDK += atof(strFileName.Right(strFileName.GetLength()-nIdx1-1));
	return fDK;
}

// 由原始文件路径得到图像路径和文件名
CString AFX_EXT_API get_path_image(const CString& strSrcPathName)
{
	CString strPathImage;

	strPathImage = strSrcPathName.Left(strSrcPathName.GetLength()-3);
	strPathImage += EXT_IMAGE;

	return strPathImage;
}

// 由原始文件路径得到图形路径和文件名
CString AFX_EXT_API get_path_graph(const CString& strSrcPathName)
{
	CString strPathGraph;

	strPathGraph = strSrcPathName.Left(strSrcPathName.GetLength()-3);
	strPathGraph += EXT_GRAPH;

	return strPathGraph;
}

// 由原始文件路径得到掌子面信息路径和文件名
CString AFX_EXT_API get_path_info(const CString& strSrcPathName)
{
	CString strPathGraph;
	
	strPathGraph = strSrcPathName.Left(strSrcPathName.GetLength()-3);
	strPathGraph += EXT_INFO;
	
	return strPathGraph;
}

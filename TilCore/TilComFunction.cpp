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
// ��·�����ļ����õ��ļ�·��
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

// �ж��Ƿ���һ���ļ�·��
// ����0��ʾ����·��,1Ϊ���·����2Ϊ����·��
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
	// ���û�ҵ�ҪĿ¼�е�'\'����·��
	if (i == strPathName.GetLength())
		return 0;

	// ���ҪĿ¼��������D:��..�ĺ��������ַ������������·��
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

// ����Ƿ��Ǳ�׼���
BOOL is_std_mileage(const CString& strSrcPathName)
{
	CString strFileName = get_file_name(strSrcPathName);
	strFileName.MakeLower();

	int i;
	int nIdx0 = strFileName.Find('k');
	int nIdx = strFileName.Find('+');
	// ���k��+֮����������1,˵��k��+֮��û�������ַ�,��ʽ����
	if (nIdx-nIdx0 <= 1)
		return FALSE;
	// �ж�k��'+'֮���Ƿ�������
	for (i = nIdx0+1; i < nIdx; i ++)
	{
		char ch = strFileName.GetAt(i);
		if (ch < '0' || ch > '9')
			return FALSE;
	}
	// �ж�'+'������Ƿ�������
	for (i = nIdx+1; i < strFileName.GetLength(); i ++)
	{
		char ch = strFileName.GetAt(i);
		if ((ch < '0' || ch > '9') && (ch != '.'))
			return FALSE;
	}
	// �ж�'+'��������Ƿ�С��999
	long tmp = atoi(strFileName.Right(strFileName.GetLength()-nIdx-1));
	if (tmp > 999)
		return FALSE;

	return TRUE;
}

// ��·���õ��ļ����ǰ׺
CString get_std_dk(const CString& strSrcPathName)
{
	CString strFileName = get_file_name(strSrcPathName);
	CString strTemp = strFileName;
	strTemp.MakeLower();
	int nIdx = strTemp.Find('k');
	return strFileName.Left(nIdx+1);

}

// �����ļ����õ���Ƭ��������
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

// ��ԭʼ�ļ�·���õ�ͼ��·�����ļ���
CString AFX_EXT_API get_path_image(const CString& strSrcPathName)
{
	CString strPathImage;

	strPathImage = strSrcPathName.Left(strSrcPathName.GetLength()-3);
	strPathImage += EXT_IMAGE;

	return strPathImage;
}

// ��ԭʼ�ļ�·���õ�ͼ��·�����ļ���
CString AFX_EXT_API get_path_graph(const CString& strSrcPathName)
{
	CString strPathGraph;

	strPathGraph = strSrcPathName.Left(strSrcPathName.GetLength()-3);
	strPathGraph += EXT_GRAPH;

	return strPathGraph;
}

// ��ԭʼ�ļ�·���õ���������Ϣ·�����ļ���
CString AFX_EXT_API get_path_info(const CString& strSrcPathName)
{
	CString strPathGraph;
	
	strPathGraph = strSrcPathName.Left(strSrcPathName.GetLength()-3);
	strPathGraph += EXT_INFO;
	
	return strPathGraph;
}

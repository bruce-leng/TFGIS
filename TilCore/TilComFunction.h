// TilComFunction.h: interface for the CTilComFunction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TILCOMFUNCTION_H__26B46DD9_44AC_47E8_9593_2878545C1C28__INCLUDED_)
#define AFX_TILCOMFUNCTION_H__26B46DD9_44AC_47E8_9593_2878545C1C28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define OBJ_MAX_NUM 20000
#define OBJ_EDGE_MAX_NUM 250


#define EXT_IMAGE "img"		// ͼ���ļ���չ��
#define EXT_GRAPH "gph"		// ͼ���ļ���չ��
#define EXT_INFO  "ifo"		// ������Ϣ�ļ���չ��

// ��·�����ļ����õ��ļ�·��
AFX_EXT_API CString get_file_path(LPCTSTR lpszPathName);
// ��·���õ��ļ�������չ��
AFX_EXT_API CString get_file_name_ext(LPCTSTR lpszPathName);
// ��·���õ��ļ���
AFX_EXT_API CString get_file_name(LPCTSTR lpszPathName);
// ��·���õ��ļ���չ��
AFX_EXT_API CString get_file_ext(LPCTSTR lpszPathName);
// �ж��Ƿ���һ���ļ�·��
AFX_EXT_API int is_path_name(const CString& strPathName);
// �ж��ļ����Ƿ��Ǳ�׼�����
AFX_EXT_API BOOL is_std_mileage(const CString& strSrcPathName);
// ��·���õ��ļ����ǰ׺
AFX_EXT_API CString get_std_dk(const CString& strSrcPathName);
// �����ļ����õ���Ƭ��������
AFX_EXT_API float get_std_mileage(const CString& strSrcPathName);
// ��ԭʼ�ļ�·���õ�ͼ��·�����ļ���
AFX_EXT_API CString get_path_image(const CString& strSrcPathName);
// ��ԭʼ�ļ�·���õ�ͼ��·�����ļ���
AFX_EXT_API CString get_path_graph(const CString& strSrcPathName);
// ��ԭʼ�ļ�·���õ���������Ϣ·�����ļ���
AFX_EXT_API CString get_path_info(const CString& strSrcPathName);


#endif // !defined(AFX_TILCOMFUNCTION_H__26B46DD9_44AC_47E8_9593_2878545C1C28__INCLUDED_)

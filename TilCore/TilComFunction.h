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


#define EXT_IMAGE "img"		// 图像文件扩展名
#define EXT_GRAPH "gph"		// 图形文件扩展名
#define EXT_INFO  "ifo"		// 素描信息文件扩展名

// 由路径及文件名得到文件路径
AFX_EXT_API CString get_file_path(LPCTSTR lpszPathName);
// 由路径得到文件名和扩展名
AFX_EXT_API CString get_file_name_ext(LPCTSTR lpszPathName);
// 由路径得到文件名
AFX_EXT_API CString get_file_name(LPCTSTR lpszPathName);
// 由路径得到文件扩展名
AFX_EXT_API CString get_file_ext(LPCTSTR lpszPathName);
// 判断是否是一个文件路径
AFX_EXT_API int is_path_name(const CString& strPathName);
// 判断文件名是否是标准的里程
AFX_EXT_API BOOL is_std_mileage(const CString& strSrcPathName);
// 由路径得到文件里程前缀
AFX_EXT_API CString get_std_dk(const CString& strSrcPathName);
// 根据文件名得到切片拍摄的里程
AFX_EXT_API float get_std_mileage(const CString& strSrcPathName);
// 由原始文件路径得到图像路径和文件名
AFX_EXT_API CString get_path_image(const CString& strSrcPathName);
// 由原始文件路径得到图形路径和文件名
AFX_EXT_API CString get_path_graph(const CString& strSrcPathName);
// 由原始文件路径得到掌子面信息路径和文件名
AFX_EXT_API CString get_path_info(const CString& strSrcPathName);


#endif // !defined(AFX_TILCOMFUNCTION_H__26B46DD9_44AC_47E8_9593_2878545C1C28__INCLUDED_)

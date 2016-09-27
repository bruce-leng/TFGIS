#pragma once
#include "GUIExtStd.h"
#include "guicontrolbar.h"
#include "GuiTabWnd.h"
#include "LogEdit.h"
#include "ComboListCtrl.h"

#include <vector>
#include "FaceCaption.h"
#include "previewlist.h"

using namespace::std;

void LogInfo(const CString& str);
void ImageInfo(int nWidth, int nHeight, int nDepth, const CString& strFileName);
void ObjectInfo(int nLayNo, int nObjNo,  float fRealArea,
								float fCenX,	 float fCenY, float fLeft, float fRight,
								float fTop,		 float fBottom,float fGraySum,
								float fGrayAve,float fGrayMax, float fGrayMin);
void FaceCaptionInfo(deque<CFaceCaption*>& dFaceCaption);

class TILEXTCLASS CGuiOutPut :	public CGuiControlBar
{
protected:
	CGuiTabWnd	m_TabSolExplorer;
	CEdit		m_EditFind;
	CPreviewList m_Pattern;	// ͼԪģʽ

	// ��״̬��Ϣ����ҳ����
	static CLogInfo	m_EditOutput;
	// ��ͼ�����ԡ���ҳ����
	static CListCtrl m_ImgCaption;
	// ��������������ҳ����
	static CComboListCtrl m_LayerInfo;

public:
	CGuiOutPut(void);
	virtual ~CGuiOutPut(void);

	friend void LogInfo(const CString& str);
	friend void ImageInfo(int nWidth, int nHeight, int nDepth, const CString& strFileName);
	friend void ObjectInfo(int nLayNo, int nObjNo,  float fRealArea,
						float fCenX, float fCenY, float fLeft, float fRight,
						float fTop,	float fBottom, float fGraySum,
						float fGrayAve, float fGrayMax, float fGrayMin);
	friend void FaceCaptionInfo(deque<CFaceCaption*>& dFaceCaption);
	// ɾ������������������Ϣ
	friend void FaceCaptionInfoClear(deque<CFaceCaption*>& dFaceCaption);

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

protected:
	void set_tab_sol_explorer_info(void);
	void set_image_caption_info(void);
	void set_layer_info(void);
};

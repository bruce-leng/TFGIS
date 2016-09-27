#ifndef __SCENEGRAPH3D_2004_2004_1_37998_17_52_55_937439_358056__H__
#define __SCENEGRAPH3D_2004_2004_1_37998_17_52_55_937439_358056__H__

#if _MSC_VER > 1000
	#pragma once
#endif // _MSC_VER > 1000
//********************************************
// SceneGraph.h
// class CSceneGraph
#include "Object3d.h"
#include "Texture.h"
#include "Material.h"
#include "Array3d.h"

#include <VECTOR>
#include <gl\gl.h>

using namespace std;

class AFX_EXT_CLASS CSceneGraph3d
{

private :

	// Datas
	CArray3d<CObject3d> m_ArrayObject3d;
	CArray3d<CTexture>  m_ArrayTexture;
	CArray3d<CMaterial> m_ArrayMaterial;
	int		m_nTextureBufLen;
	GLuint *m_pIndexTextureBinding;
	unsigned int m_ListDone;

public :
	CString m_strName;

	// Constructor
	CSceneGraph3d();
	~CSceneGraph3d();

	// Datas
	void Add(CObject3d *pObject3d);
	void RemoveAt(int index);
	CObject3d *GetAt(int index);
	CObject3d *operator[](int index);
	int NbObject(void) { return m_ArrayObject3d.GetSize(); }
	void Free(void);

	// I/O
	int SaveFile(char *name);
	int SaveFileRaw(char *name);
	int WriteHeader(CStdioFile &file,char *name);

	// Misc
	int BuildAdjacency();
	int CalculateNormalPerVertex();
	int CalculateNormalPerFace();

	// Binding
	void SetNormalBinding(int type);
	void SetColorBinding(int type);

	// OpenGL
	virtual int glBuildList();
	void glDraw();
	void glDraw(int type);
	void SetModified(BOOL bModified){m_ListDone = bModified;};	//*********
	void RebuildTexture();
//	void DrawTexture(BOOL bDraw);

	int m_nShowMesh;
	void DrawObject(int Start, int nNum = 3);
	int GetCurShowIdx() {return m_nShowMesh;};
	
	int m_nSideFaceNum;
	int GetSideFaceNum() {return m_nSideFaceNum;};
	// Textures
	
	void SetTextureNum(int nNum);
	CArray3d<CTexture> *GetArrayTexture() { return &m_ArrayTexture; }
	int HasTexture(char *name,int *index);
	void SetTextureIdxAt(int nIdx, CTexture * pTexure);
	int NbTexture() { return m_ArrayTexture.GetSize(); }
	int AddTexture(CTexture *pTexture) { return m_ArrayTexture.Add(pTexture); }
	void RemoveAllTexture() { m_ArrayTexture.RemoveAll(); }
	void RemoveTexture(int index) { m_ArrayTexture.RemoveAt(index); }
	CTexture *GetTexture(int index) { return m_ArrayTexture[index]; }
	void ExchangeTexture(int nIdx1, int nIdx2);
	void MoveTextureIdxToNewPos(int nOldPos, int nNewPos);

	void FreeVertext();
};

#endif //__SCENEGRAPH3D_2004_2004_1_37998_17_52_55_937439_358056__H__
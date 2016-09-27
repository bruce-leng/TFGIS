//********************************************
// SceneGraph.cpp
//********************************************
// class CSceneGraph
//********************************************
// pierre.alliez@cnet.francetelecom.fr
// Created : 15/01/98
// Modified : 15/01/98
//********************************************

#include "stdafx.h"
#include "Base3d.h"
#include "SceneGraph3d.h"

//////////////////////////////////////////////
// CONSTRUCTORS
//////////////////////////////////////////////

//********************************************
// Constructor
//********************************************
CSceneGraph3d::CSceneGraph3d()
{
	m_pIndexTextureBinding = NULL;
	m_ListDone = 0;
	m_nShowMesh = 0;
	m_nTextureBufLen = 0;
}

//********************************************
// Destructor
//********************************************
CSceneGraph3d::~CSceneGraph3d()
{
	Free();
}

//********************************************
// Destructor
//********************************************
void CSceneGraph3d::Free(void)
{
	// Textures
	if(m_ArrayTexture.GetSize())
	{
		::glDeleteTextures(m_ArrayTexture.GetSize(),m_pIndexTextureBinding);
		if(m_pIndexTextureBinding != NULL)
		{
			delete [] m_pIndexTextureBinding;
			m_pIndexTextureBinding = NULL;
		}
	}
	m_ArrayTexture.Free();
	// Objects
	m_ArrayObject3d.Free();
}

void CSceneGraph3d::FreeVertext()
{
	m_ArrayObject3d.Free();
}

//////////////////////////////////////////////
// DATAS
//////////////////////////////////////////////

//********************************************
// Add
//********************************************
void CSceneGraph3d::Add(CObject3d *pObject3d)
{
	m_ArrayObject3d.Add(pObject3d);
}

//********************************************
// Add
//********************************************
void CSceneGraph3d::RemoveAt(int index)
{
	m_ArrayObject3d.RemoveAt(index);
}

//********************************************
// GetAt
//********************************************
CObject3d *CSceneGraph3d::GetAt(int index)
{
	ASSERT(index < m_ArrayObject3d.GetSize());
	return m_ArrayObject3d[index];
}
	
//********************************************
// Operator []
//********************************************
CObject3d *CSceneGraph3d::operator[](int index)
{
	ASSERT(index < m_ArrayObject3d.GetSize());
	return m_ArrayObject3d[index];
}

//////////////////////////////////////////////
// OPENGL
//////////////////////////////////////////////

//********************************************
// BuildList
//********************************************
int CSceneGraph3d::glBuildList()
{
	m_strName = "Object";
	glInitNames();
	glPushName(0);

	int nObjIdx = 0;
	// Meshes
	//***********************************
	unsigned int size = m_ArrayObject3d.GetSize();
	for(unsigned int i=0; i<size; i++)
	{
		CObject3d *pObject3d = m_ArrayObject3d.GetAt(i);
		if(pObject3d != NULL)
		{
			if (pObject3d->GetType() == TYPE_MESH3D)
			
			if (((CMesh3d*)pObject3d)->GetMeshType() == SIDE_FACE)
			{
				glLoadName(++nObjIdx);
			}

			pObject3d->glBuildList();
		}

	}

	// Textures
	//***********************************
	unsigned int NbTexture = m_ArrayTexture.GetSize();
	if(NbTexture)
	{
		// Cleanup

		if(m_pIndexTextureBinding != NULL)
		{
			::glDeleteTextures(NbTexture,m_pIndexTextureBinding);
			delete [] m_pIndexTextureBinding;
		}
		m_pIndexTextureBinding = new unsigned int[NbTexture];
		memset(m_pIndexTextureBinding, 0xFF, NbTexture * sizeof(GLuint));
		glGenTextures(NbTexture, m_pIndexTextureBinding);
//		m_nTextureBufLen = NbTexture;

		int error = glGetError();
		ASSERT(error !=  GL_INVALID_VALUE);
		ASSERT(error !=  GL_INVALID_OPERATION);

		TRACE("\n glBuildList() = %d", NbTexture);
		
		for(i=0;i<NbTexture;i++)
		{
			while (GL_NO_ERROR != glGetError() ) {}
			// Read datas
			CTexture *pTexture = m_ArrayTexture[i];

			// Bind texture
			glBindTexture(GL_TEXTURE_2D,m_pIndexTextureBinding[i]);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

			int error = glGetError();
			ASSERT(error !=  GL_INVALID_ENUM);
			ASSERT(error !=  GL_INVALID_OPERATION);

			ASSERT(pTexture->GetData() != NULL);
			
			::glTexImage2D(GL_TEXTURE_2D,0,3,
										 pTexture->GetWidth(),pTexture->GetHeight(),0,			
										 GL_BGR_EXT,GL_UNSIGNED_BYTE,pTexture->GetData());
			error = glGetError();
			ASSERT(error !=  GL_INVALID_ENUM);
			ASSERT(error !=  GL_INVALID_OPERATION);
			ASSERT(error !=  GL_INVALID_VALUE);

		}
	}
	
	m_ListDone = 1;

	return 1;
}

void CSceneGraph3d::RebuildTexture()
{
	unsigned int NbTexture = m_ArrayTexture.GetSize();
	if(NbTexture)
	{
		// Cleanup

		if(!m_pIndexTextureBinding)
		{
			m_pIndexTextureBinding = new unsigned int[NbTexture];
			memset(m_pIndexTextureBinding, 0xFF, NbTexture * sizeof(GLuint));
			glGenTextures(NbTexture, m_pIndexTextureBinding);
		}

		TRACE("\n RebuildTexture() = %d", NbTexture);
		for(int i=0;i<NbTexture;i++)
		{
			while (GL_NO_ERROR != glGetError() ) {}
			// Read datas
			CTexture *pTexture = m_ArrayTexture[i];

			// Bind texture
			glBindTexture(GL_TEXTURE_2D,m_pIndexTextureBinding[i]);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

			ASSERT(pTexture->GetData() != NULL);
			int error = glGetError();
			ASSERT(error !=  GL_INVALID_ENUM);
			ASSERT(error !=  GL_INVALID_OPERATION);

			
			::glTexImage2D(GL_TEXTURE_2D,0,3,
										 pTexture->GetWidth(),pTexture->GetHeight(),0,			
										 GL_BGR_EXT,GL_UNSIGNED_BYTE,pTexture->GetData());
			error = glGetError();
			ASSERT(error !=  GL_INVALID_ENUM);
			ASSERT(error !=  GL_INVALID_OPERATION);
			ASSERT(error !=  GL_INVALID_VALUE);

		}
	}	
	m_ListDone = 1;
}

//********************************************
// glDraw
//********************************************

void CSceneGraph3d::glDraw()
{
	if(!m_ListDone)
	{
		glBuildList();
		RebuildTexture();
	}

	unsigned int size = m_ArrayObject3d.GetSize();

	int nStart = m_nShowMesh;
	BOOL bEndDraw = FALSE;

	for(unsigned int i=0; i<size; i++)
	{
		CObject3d *pObject3d = m_ArrayObject3d[i];
		
		// Texture
		if(pObject3d->GetType() == TYPE_MESH3D)
		{
			CMesh3d *pMesh = (CMesh3d *)pObject3d;
						
			int IndexTexture = pMesh->GetTextureIndex();
			if(IndexTexture >= 0 && m_pIndexTextureBinding != NULL)// && CObject3d::g_bDrawText
			{
				//ASSERT(glIsTexture(m_pIndexTextureBinding[IndexTexture]));
				if (glIsTexture(m_pIndexTextureBinding[IndexTexture]))
				{
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/////////////////////// 纹理定义
					glBindTexture(GL_TEXTURE_2D,m_pIndexTextureBinding[IndexTexture]);
				}
			}

			// 设置正、反面为边缘绘制或填充方式
			if (pMesh->GetMeshType() == SIDE_FACE || pMesh->GetMeshType() == ARC_FACE)
				::glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			else
				::glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			
			//之所以如此复杂，是由于在显示一个地质层目标的时候，需要显示该层
			//所包含的侧面图和各切面的纹理贴图
//			if(pMesh->GetMeshType() == SIDE_FACE)
			{   
				if (bEndDraw)
					break;
				
				if (nStart == -1)
				{
					pObject3d->glDraw();
				}
				else
				{
					if (!nStart)
					{
						pObject3d->glDraw();
						bEndDraw = TRUE;
					}
					--nStart;
				}
			}
			
			if (nStart < 0)
				pObject3d->glDraw();

			if (pMesh->GetMeshType() == ARC_FACE)
				pObject3d->glDraw();
			
		}
		else
			pObject3d->glDraw();

	}

	if (bEndDraw == FALSE && nStart == 0)
		m_nShowMesh = -1;
}

void CSceneGraph3d::DrawObject(int nStart, int nNum)
{
	if (nStart < -1 || nStart >= m_ArrayObject3d.GetSize())
		nStart = -1;

	m_nShowMesh = nStart;
}

//********************************************
// glDraw
// draw only type
//********************************************
void CSceneGraph3d::glDraw(int type)
{
	if(!m_ListDone)
	{
		glBuildList();
		RebuildTexture();
	}

	unsigned int size = m_ArrayObject3d.GetSize();
	for(unsigned int i=0; i<size; i++)
	{
		CObject3d *pObject3d = m_ArrayObject3d[i];
		if(pObject3d->GetType() == type)
		{
			if (pObject3d->GetType() == TYPE_MESH3D)
			{
				pObject3d->glDraw();
			}
		}
	}
}

//////////////////////////////////////////////
// MISC
//////////////////////////////////////////////

//********************************************
// BuildAdjacency
// For each mesh
//********************************************
int CSceneGraph3d::BuildAdjacency()
{
	int size = m_ArrayObject3d.GetSize();
	if(size ==0)
		return 0;
	
	for(int i=0;i<size;i++)
	{
		CObject3d *pObject3d = m_ArrayObject3d[i];
		if(pObject3d->GetType() != TYPE_MESH3D)
			continue;
		CMesh3d *pMesh = (CMesh3d *)m_ArrayObject3d[i];
		pMesh->BuildAdjacency();
	}

	return 1;
}

//********************************************
// CalculateNormalPerVertex
// For each mesh
//********************************************
int CSceneGraph3d::CalculateNormalPerVertex()
{
	int size = m_ArrayObject3d.GetSize();
	if(size ==0)
		return 0;

	for(int i=0;i<size;i++)
	{
		CObject3d *pObject3d = m_ArrayObject3d[i];
		if(pObject3d->GetType() != TYPE_MESH3D)
			continue;
		CMesh3d *pMesh = (CMesh3d *)m_ArrayObject3d[i];
		pMesh->CalculateNormalPerVertex();
	}

	return 1;
}

//********************************************
// CalculateNormalPerFace
// For each mesh
//********************************************
int CSceneGraph3d::CalculateNormalPerFace()
{
	int size = m_ArrayObject3d.GetSize();
	if(size ==0)
		return 0;

	for(int i=0;i<size;i++)
	{
		CObject3d *pObject3d = m_ArrayObject3d[i];
		if(pObject3d->GetType() != TYPE_MESH3D)
			continue;
		CMesh3d *pMesh = (CMesh3d *)m_ArrayObject3d[i];
		pMesh->CalculateNormalPerFace();
	}

	return 1;
}

//********************************************
// SetNormalBinding
//********************************************
void CSceneGraph3d::SetNormalBinding(int type)
{
	int size = m_ArrayObject3d.GetSize();
	for(int i=0;i<size;i++)
	{
		CObject3d *pObject3d = m_ArrayObject3d[i];
		if(pObject3d->GetType() != TYPE_MESH3D)
			continue;
		CMesh3d *pMesh = (CMesh3d *)m_ArrayObject3d[i];
		pMesh->SetNormalBinding(type);
	}
}

//********************************************
// SetColorBinding
//********************************************
void CSceneGraph3d::SetColorBinding(int type)
{
	int size = m_ArrayObject3d.GetSize();
	for(int i=0;i<size;i++)
	{
		CObject3d *pObject3d = m_ArrayObject3d[i];
		if(pObject3d->GetType() != TYPE_MESH3D)
			continue;
		CMesh3d *pMesh = (CMesh3d *)m_ArrayObject3d[i];
		//NORMAL_PER_VERTEX
		pMesh->SetColorBinding(type);
//		pMesh->SetColorBinding(NORMAL_PER_VERTEX);
	}
}

//////////////////////////////////////////////
// TEXTURES
//////////////////////////////////////////////

//********************************************
// HasTexture
//********************************************
int CSceneGraph3d::HasTexture(char *name,
							  int *index)
{
	for(int i=0;i<m_ArrayTexture.GetSize();i++)
		if(m_ArrayTexture[i]->GetFileName() == name)
		{
			*index = i;
			return 1;
		}
	return 0;
}

void CSceneGraph3d::ExchangeTexture(int nIdx1, int nIdx2)
{
	CTexture* pTextureTmp;

	if (nIdx1 < 0 || nIdx2 < 0 || nIdx1 >= m_ArrayTexture.GetSize() || nIdx2 >= m_ArrayTexture.GetSize())
		return;
	
	pTextureTmp = m_ArrayTexture.GetAt(nIdx1);
	m_ArrayTexture.SetAt(nIdx1, m_ArrayTexture.GetAt(nIdx2));
	m_ArrayTexture.SetAt(nIdx1, pTextureTmp);
}

void CSceneGraph3d::MoveTextureIdxToNewPos(int nOldPos, int nNewPos)
{
//	CTexture* pTextureTmp;
	
	if (nOldPos < 0 || nNewPos < 0 || nOldPos >= m_ArrayTexture.GetSize() || nNewPos >= m_ArrayTexture.GetSize())
		return;
	
	GLuint TetureIdx = m_pIndexTextureBinding[nOldPos];
	if (nOldPos < nNewPos)
	{
		for (int i = nOldPos; i <= nNewPos; ++i)
			m_pIndexTextureBinding[i] = m_pIndexTextureBinding[i + 1];

	}
	else if (nOldPos > nNewPos)
	{
		for (int i = nOldPos; i > nNewPos; --i)
			m_pIndexTextureBinding[i] = m_pIndexTextureBinding[i - 1];
	}

	m_pIndexTextureBinding[nNewPos + 1] = TetureIdx;
}

//////////////////////////////////////////////
// I/O
//////////////////////////////////////////////

//********************************************
// SaveFile
//********************************************
int CSceneGraph3d::SaveFile(char *name)
{
	// Check
	if(NbObject() == 0)
	{
		AfxMessageBox("This scene does not contain meshes");
		return 0;
	}

	// Check for valid file
	CStdioFile file;
	CFileException ex;
	
	// Write header
	if(!WriteHeader(file,name))
	{
		AfxMessageBox("Error during writing header");
		return 0;
	}

	// Meshes
	for(int i=0;i<NbObject();i++)
	{
		CObject3d *pObject = m_ArrayObject3d[i];
		if(pObject->GetType() == TYPE_MESH3D)
			((CMesh3d *)pObject)->WriteFile(file);
	}

	// Close file
  file.Close();

	return 1;
}

//********************************************
// SaveFileRaw
//********************************************
int CSceneGraph3d::SaveFileRaw(char *name)
{
	// Check
	if(NbObject() == 0)
	{
		AfxMessageBox("This scene does not contain meshes");
		return 0;
	}

	// Check for valid file
	CFile file;
	CFileException ex;

	// Try to open file (text mode)
	if(!file.Open(name,CFile::modeCreate | CFile::modeWrite | CFile::typeBinary,&ex))
	{
		#ifdef _DEBUG
		  afxDump << "File could not be opened " << ex.m_cause << "\n";
		#endif
		AfxMessageBox("Unable to open file for writing");
		return 0;
	}
	
	// Meshes
	unsigned int NbMesh = NbObject();
	file.Write(&NbMesh,sizeof(unsigned int));
	for(unsigned int i=0;i<NbMesh;i++)
	{
		CObject3d *pObject = m_ArrayObject3d[i];
		if(pObject->GetType() == TYPE_MESH3D)
			((CMesh3d *)pObject)->WriteFileRaw(file);
	}

	// Close file
  file.Close();

	return 1;
}

//**********************************************
// WriteHeader
// Do not close file
//**********************************************
int CSceneGraph3d::WriteHeader(CStdioFile &file,
							   char *name)
{
	CFileException ex;
	
	// Try to open file (text mode)
	if(!file.Open(name,CFile::modeCreate | CFile::modeWrite | CFile::typeText,&ex))
	{
		#ifdef _DEBUG
		  afxDump << "File could not be opened " << ex.m_cause << "\n";
		#endif
		AfxMessageBox("Unable to open file for writing");
		return 0;
	}

	// ** Header *******************************
	
	TRY
	{
		file.WriteString("#VRML V2.0 utf8\n\n");
		file.WriteString("# Produced by 3d Toolbox 1.0 (Pierre Alliez, CNET / DIH / HDM)\n\n");
	}
	CATCH(CFileException, e)
	{
		#ifdef _DEBUG
				afxDump << "Error during writing " << e->m_cause << "\n";
		#endif
		AfxMessageBox("Error during writing file header");
		file.Close();
		return 0;
	}
	END_CATCH

	// do not close file
	return 1;
}

// ** EOF **

void CSceneGraph3d::SetTextureNum(int nNum) 
{
	m_ArrayTexture.SetSize(nNum);
}

void CSceneGraph3d::SetTextureIdxAt(int nIdx, CTexture * pTexure)
{
	m_ArrayTexture.SetAtGrow(nIdx, pTexure);
}

//void CSceneGraph3d::DrawTexture(BOOL bDraw)
//{
//	for (int i = 0; i < m_ArrayObject3d.GetSize(); ++i)
//	{
//		CObject3d* pObj = m_ArrayObject3d.GetAt(i);
//
//		if (pObj->GetType() == TYPE_MESH3D)
//			((CMesh3d*)pObj)->DrawTexture(bDraw);
//	}
//
//	m_ListDone = 0;
//}
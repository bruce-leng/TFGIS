//********************************************
// Mesh3d.h
// class CMesh3d
//********************************************
// A mesh : simplicial complex
// + Array of vertex
// + Array of faces
// + Binding infos
//********************************************
// pierre.alliez@cnet.francetelecom.fr
// Created : 15/01/98
// Modified : 07/07/98
//********************************************

#ifndef _MESH_3D_
#define _MESH_3D_

#include "Array3d.h"
#include "Vertex3d.h"
#include "Face3d.h"
#include "Vector3d.h"
#include "Material.h"
#include "Transform.h"

class CColorRamp;
class CTexture;

typedef enum 
{
	MESH_ARC,		// 边墙及拱部
	MESH_STRUCT,	// 结构面
	MESH_DRILL,		// 地质钻孔
	MESH_FACE,		// 隧道掌子面
	MESH_ROCK,		// 岩体

	SIDE_FACE,
	TOP_FACE,
	BOTTOM_FACE,
	ARC_FACE,
	STRUCT_FACE
} MESH_TYPE;

#include "TilPoint.h"
// 查找边墙纹理贴图的相对纹理坐标
void FindTexCoord(CFace3d* pFace, PPOINT2DF pCoords);
void FindTexCoordRock(CFace3d* pFace, PPOINT2DF pCoords);

class AFX_EXT_CLASS CMesh3d : public CObject3d
{
friend class CParserVrml;
friend class CParseChannelData;
private :

	// Datas
	CArray3d<CVertex3d> m_ArrayVertex;
	CArray3d<CFace3d>   m_ArrayFace;

	CTransform m_Transform;
	CString m_Name;
	
	MESH_TYPE m_nMeshType; //enum
	// Apparence & texture
	CMaterial m_Material; // material
	int m_NormalBinding;  // normals defined at vertices or faces ? (Gouraud)
	int m_ColorBinding;   // colors defined at vertices or faces ? (Gouraud)

	// Texture
	int m_IndexTexture;   // which texture in SceneGraph ? (-1 : no texture)
//	float *m_pTextureCoordinate;
//	int *m_pTextureCoordinateIndex;

	// OpenGL-specific
	unsigned int m_ListOpenGL;
	unsigned int m_ListDone;
	int m_Modified;
	int m_Show;

	long	m_lIdHigh;
	long	m_lIdLow;

	float	m_fLockOffset[2]; //锚点偏移修正[0] = x; [1] = y;
public :
	void SetLockOffset(float* fLockOffset) {m_fLockOffset[0] = fLockOffset[0]; m_fLockOffset[1] = fLockOffset[1];};
	void GetLockOffset(float* fLockOffset) {fLockOffset[0] = m_fLockOffset[0]; fLockOffset[1] = m_fLockOffset[1];};

	void SetLockOffset(float fOffsetX, float fOffsetY) {m_fLockOffset[0] = fOffsetX; m_fLockOffset[1] = fOffsetY;};
	void SetLockOffset(float& fOffsetX, float& fOffsetY) {fOffsetX = m_fLockOffset[0]; fOffsetY = m_fLockOffset[1];};
	
	float LockOffsetX() {return m_fLockOffset[0];};
	float LockOffsetY() {return m_fLockOffset[1];};

	void SetMeshID(long lIDLow, long lIdHigh){m_lIdHigh = lIdHigh; m_lIdLow = lIDLow;};
	void GetMeshID(long &lIDLow, long &lIdHigh){lIdHigh = m_lIdHigh; lIDLow = m_lIdLow;};

	// Constructor
	CMesh3d();
	virtual ~CMesh3d();

	MESH_TYPE GetMeshType(){return m_nMeshType;};
	void SetMeshType(MESH_TYPE nMeshType) {m_nMeshType = nMeshType;};
	// Datas
	void Free();
	virtual int GetType();
	int IsValid();
	void Copy(CMesh3d *pMesh);

	// Faces
	int NbFace() { return m_ArrayFace.GetSize(); }
	CFace3d *GetFace(int index) { return m_ArrayFace[index]; }
	void AddFace(CFace3d *pFace) { m_ArrayFace.Add(pFace); }
	void SetNbFace(int NbFace) { m_ArrayFace.SetSize(NbFace); }
	CArray3d<CFace3d> *GetArrayFace() { return &m_ArrayFace; }
	int Has(CFace3d *pFace) { return m_ArrayFace.Has(pFace); }
	int FindFace(CFace3d *pFace); // even in neighborign faces

	// Flags
	void SetFlagOnFaces(int flag);
	void SetFlagOnVertices(int flag);
	CVertex3d *GetFirstVertexWithFlag(int flag);
	int GetMaxFlagOnFaces();
	int FindSmallestFlagOnVerticesDiffThan(int flag,int *pFounded);
	int FindFlagOnVerticesDiffThan(int flag);
	CVertex3d *GetAnyVertexWithFlagRootPrefered(int flag);


	// Vertices
	int NbVertex() { return m_ArrayVertex.GetSize(); }
	void SetNbVertex(int NbVertex)  { m_ArrayVertex.SetSize(NbVertex); }
	CArray3d<CVertex3d> *GetArrayVertex() { return &m_ArrayVertex; }
	void AddVertex(CVertex3d *pVertex) { m_ArrayVertex.Add(pVertex); }
	int DeleteVertex(CVertex3d *pVertex);
	int DeleteVertex(int index);
	int DeleteFaceNbNeighbors(int NbNeighbor);
	CVertex3d *GetVertex(int index) {return m_ArrayVertex[index];}
	int Has(CVertex3d *pVertex) { return m_ArrayVertex.Has(pVertex); }

	// Edges
	float GetMeanLengthEdge();

	// Name
	CString GetName(void) { return m_Name; } 
	void SetName(CString &string) { m_Name = string; } 

	// I/O
	int WriteFile(CStdioFile &file);
	int WriteFileRaw(CFile &file);

	// Transform
	void SetTransform(CTransform &transform) { m_Transform.Copy(transform); }
	CTransform *GetTransform(void) { return &m_Transform; }

	// Range
	void Range(int coord,float *min,float *max); 
	void Range(int coord,float min,float max); 
	void Offset(int coord,float offset);
	void Scale(int coord,float scale);
	void Move(float dx,float dy,float dz);

	// Predefined
	int GenerateMap(int line,int col,float min,float max);
	int GenerateBox(float dx,float dy,float dz);
	int GenerateMap(CTexture *pTexture,int width,int height,int FlagColor = 1);

	// OpenGL
	virtual int glBuildList();
	virtual int glDraw();
	void Show(int flag) { m_Show = flag; }

	// Debug
	void Trace();

	// Adjacency 
	int BuildAdjacency();

	// Processing
	int IndexFrom(CFace3d *pFace);
	int IndexFrom(CVertex3d *pVertex);

	// Modif
	void SetModified(int flag = 1) { m_Modified = flag; }
	int GetModified() { return m_Modified; }


	// Vertex removal
	int VertexRemoval(CVertex3d *pV);
	int VertexRemoval();

	// Search
	int FindVertex(CVertex3d *pVertex);

	// Normals
	int CalculateNormalPerVertex(void);
	int CalculateNormalPerFace(void);
	void SetNormalBinding(int type);
	int GetNormalBinding(void);
	CVectorSet3d *GetVectorSetNormalPerFace(void); // alloc on the global heap

	// Adjacency
	void Rebuild();

	// Distance
	double SquareDistanceToVertex(CVertex3d *pVertex,CVertex3d **pVertexRef);
	double SquareDistanceToFace(CVertex3d *pVertex,CVertex3d *pVertexRef,CFace3d **ppFaceRef);

	// Sharp edges
	int ColorSharpEdge(float threshold,CColor &color);
	CVectorSet3d *GetSharpEdges(float threshold);

	// Strings
	CVectorSet3d *GetStringBetweenVertices();

	// Color
	void SetColorBinding(int type);
	void SetColor(CColor &color);
	int GetColorBinding(void);

	// Material
	CMaterial *GetMaterial() { return &m_Material; }
	void SetMaterial(CMaterial *pMaterial) { m_Material.Copy(pMaterial); }

	// Texture
//	BOOL m_bDrawTexture;
//	void DrawTexture(BOOL bDrawTexture) {m_bDrawTexture = bDrawTexture;};
//	BOOL IsDrawTexture() {return m_bDrawTexture;};
	int GetTextureIndex() { return m_IndexTexture; }
	void SetTextureIndex(int IndexTexture) {m_IndexTexture = IndexTexture;};
	// Subdivision
	int SubdivisionLoop(int MoveOnBundary = 1); // Charles Loop (1987)
	int Subdivision(void); 
	float Alpha(int n);

	// Smoothing
	int Smooth(int MoveOnBundary = 1);

	// Coloring (curvature, compacity, etc...)
	void ColorCurvature(CColorRamp *pRamp);
	void ColorNormalSpace(CColorRamp *pRamp);
	void ColorCompacity(CColorRamp *pRamp);
	void ColorHeight(CColorRamp *pRamp);
	void ColorFacesFromFlagPastels();
	void ColorFacesFromFlagGrey();

	// Intersection
	int NearestIntersectionWithLine(CVertex3d *pV0,CVertex3d *pV1,CVertex3d *pVertexResult,
		                              CFace3d **ppFaceResult,int *pNbFaceVisited);
	// Area
	double GetMinArea(CFace3d **ppFace = NULL);
	double GetMeanArea();

};


#endif // _MESH_3D_

//********************************************
// Object3d.h
//********************************************
// class CObject3d, the base 3d object
//********************************************
// pierre.alliez@cnet.francetelecom.fr
// Created : 17/12/97
// Modified : 19/12/97
//********************************************

#ifndef _OBJECT3D_
#define _OBJECT3D_

#undef CREATE_TEXTURE_BYTHREAD
#define CREATE_TEXTURE_BYTHREAD

class AFX_EXT_CLASS CObject3d
{
private :

public :
	static BOOL g_bDrawText;
	CObject3d() {}
	virtual ~CObject3d() {}

	// Datas
	virtual int GetType();
	virtual int glBuildList();
	virtual int glDraw();

};

#endif // _OBJECT3D_

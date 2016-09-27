//********************************************
// Transform.cpp
//********************************************
// class CTransform
//********************************************
// pierre.alliez@cnet.francetelecom.fr
// Created : 23/02/98
// Modified : 23/02/98
//********************************************

#include "stdafx.h"
#include "Transform.h"


//********************************************
// Constructor
//********************************************
CTransform::CTransform()
{
	SetScale(CVector3d(1.0f,1.0f,1.0f));
	SetTranslation(CVector3d(0.0f,0.0f,0.0f));
	SetValueRotation(0.0f);
}

//********************************************
// Constructor
//********************************************
void CTransform::Clear(void)
{
	SetScale(CVector3d(1.0f,1.0f,1.0f));
	SetTranslation(CVector3d(0.0f,0.0f,0.0f));
	SetValueRotation(0.0f);
}

//********************************************
// Copy
//********************************************
void CTransform::Copy(CTransform &transform)
{
	SetScale(transform.GetScale());
	SetTranslation(transform.GetTranslation());
	SetRotation(transform.GetRotation());
	SetValueRotation(transform.GetValueRotation());
}

//********************************************
// Copy
//********************************************
void CTransform::Copy(CTransform *pTransform)
{
	SetScale(pTransform->GetScale());
	SetTranslation(pTransform->GetTranslation());
	SetRotation(pTransform->GetRotation());
	SetValueRotation(pTransform->GetValueRotation());
}




//////////////////////////////////////////////
// DATAS
//////////////////////////////////////////////



// ** EOF **

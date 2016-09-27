//********************************************
// Vector3d.h
//********************************************
// pierre.alliez@cnet.francetelecom.fr
// Created : 10/12/97
// Modified : 19/01/98
//********************************************

#ifndef _VECTOR_3D_
#define _VECTOR_3D_

#include "Object3d.h"
#include <math.h>

class CVertex3d;

class CVector3d : public CObject3d
{

private :

	// Geometry
	float m_x;
	float m_y;
	float m_z;

public :

	CVector3d() { m_x = m_y = m_z = 0.0f; } 
	virtual ~CVector3d(){};
	
	// Constructors
	CVector3d(const float x,const float y,const float z);
	CVector3d( CVector3d &vc){Set(&vc);};
//	CVector3d(const CVector3d vc){m_x = vc.x; m_y = vc.y; m_z = vc.z; };
	CVector3d(CVector3d *pVector);
	CVector3d(CVertex3d *pVertex1,CVertex3d *pVertex2);
	CVector3d(CVertex3d &vertex1,CVertex3d &vertex2);

	// Debug
	void Trace();

	// Data setting
	virtual int GetType();
	void Clear(void);
	void Set(CVector3d *pVector);
	void Set(CVector3d &vc);
	void Set(CVertex3d *pVertex1, CVertex3d *pVertex2);
	void Set(const float x,const float y,const float z);
	void Copy(CVector3d &vc);
	void Copy(CVector3d *pVector);

	// Per coordinate (explicit inline functions)
	void x(float x) {	m_x = x; }
	void y(float y) {	m_y = y; }
	void z(float z) {	m_z = z; } 

      float       operator []( int i) const {  switch(i) {
      case 0: 	return m_x;
      case 1: 	return m_y;
      case 2: 	return m_z;
      default:  return 0;
      }}
      float&      operator []( int i)  {  switch(i) {
      case 0: 	return m_x;
      case 1: 	return m_y;
      case 2: 	return m_z;
      }}

	// Data access (explicit inline functions)
	float x(void) { return m_x; }
	float y(void) { return m_y; }
	float z(void) { return m_z; }

	// Operators
	CVector3d operator=(CVector3d& vc); // Setting
	void operator+=(CVector3d* pVector);
	void operator/=(float factor);
	void operator*=(float factor);

	void operator-=(CVector3d& pVector);

	void Inner(CVector3d& vc);
	static CVector3d Inner(CVector3d* u,CVector3d* v); // Stupid c++

      float       Dot(const CVector3d& v)        const    {  return (m_x*v.m_x+m_y*v.m_y+m_z*v.m_z);   }
      void        Vector(const CVector3d& b,
                               CVector3d &c)     const    {  c.m_x= m_y * b.m_z - m_z * b.m_y;
	                                                         c.m_y= m_z * b.m_x - m_x * b.m_z;
	                                                         c.m_z= m_x * b.m_y - m_y * b.m_x; }
      float       SquaredLenght()               const    {  return Dot(*this);   }
      float       Lenght()                      const    {  return (float)sqrt(Dot(*this));   }

	// Misc
	void NormalizeL2(void);
	void NormalizeL2(float value);
	double GetNormL2(void);
	double GetNormL2Square(void);
	int Collinear(CVector3d *pVector);
	int Collinear(CVector3d &vc);

	void RotateXZ(float alpha);

};

// External operators
CVector3d operator+(CVector3d& u,CVector3d& v); // Add
CVector3d operator-(CVector3d& u,CVector3d& v); // Subtract
CVector3d operator^(CVector3d& u,CVector3d& v); // Inner

#endif // _VECTOR_3D_

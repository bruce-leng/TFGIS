#ifndef __TILCORE_2004_2004_1_37999_12_47_25_499949_698849__H__
#define  __TILCORE_2004_2004_1_37999_12_47_25_499949_698849__H__


#if _MSC_VER > 1000
	#pragma once
#endif // _MSC_VER > 1000

#include "TILdef.h"

#ifdef PI
	#undef PI 
#endif

#ifdef PID
	#undef PID
#endif

#ifdef PIP2
	#undef PIP2
#endif

#ifdef E
	#undef E
#endif

#ifdef Degree
	#undef Degree
#endif

#ifdef DegreeD
	#undef DegreeD
#endif

#ifdef GoldenRatio
	#undef GoldenRatio
#endif

#ifdef EulerGamma
	#undef EulerGamma
#endif

#ifdef Catalan
	#undef Catalan
#endif

namespace CONST_NUM
{
	//##ModelId=3FFE4BE8010A
	const double   PI  = 3.1415926535897932384626433832795;
	 // 1 / Pi = 0.318309
	//##ModelId=3FFE4BE80119
	const double   PID = 0.31830988618379067153776752674503;
	// 2 * PI = 6.2831853
	//##ModelId=3FFE4BE8011A
	const double   PIP2 = 6.2831853071795864769252867665590;
	// Pi / 2 = 1.5707963 
	//##ModelId=3FFE4BE80128
	const double   PID2 = 1.5707963267948966192313216916398;
	 // E is the exponential staticant = 2.7182818
	//##ModelId=3FFE4BE80138
	const double   E = 2.7182818284590452353602874713527;
	 // Degree gives the number of radians in one degree. It has a numerical value of Pi / 180. = 0.0174532
	//##ModelId=3FFE4BE80139
	const double   Degree = 0.017453292519943295769236907684886;
	// 1 / Degree
	//##ModelId=3FFE4BE80148
	const double   DegreeD = 57.295779513082320876798154814105;
	// GoldenRatio is the golden ratio ((1+sqrt(2)) / 2)
	//##ModelId=3FFE4BE80157
	const double   GoldenRatio = 1.6180339887498948482045868343656;
	 //EulerGamma is Euler's staticant Gamma,  = 0.5772156
	//##ModelId=3FFE4BE80158
	const double   EulerGamma = 0.57721566490153286060651209008241;
	 // Catalan's staticant is given by the sum Sum[power(-1, k)power(2 k +1, -2)]; = 0.9159655
	//##ModelId=3FFE4BE80167
	const double   Catalan = 0.91596559417721901505460351493238;
};



#endif //__TILCORE_2004_2004_1_37999_12_47_25_499949_698849__H__
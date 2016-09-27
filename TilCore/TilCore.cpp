// TilCore.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#include "TilCore.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static AFX_EXTENSION_MODULE TilCoreDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("TILCORE.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(TilCoreDLL, hInstance))
			return 0;

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		new CDynLinkLibrary(TilCoreDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("TILCORE.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(TilCoreDLL);
	}
	return 1;   // ok
}

//double CONST_NUM::PI = 3.1415926535897932384626433832795; // Pi

//double CONST_NUM::PID = 0.31830988618379067153776752674503; // 1 / Pi

//double CONST_NUM::PIP2 = 6.2831853071795864769252867665590; // 2 * PI

//double CONST_NUM::PID2 = 1.5707963267948966192313216916398; // Pi / 2

//double CONST_NUM::E = 2.7182818284590452353602874713527; // E is the exponential ant

//double CONST_NUM::Degree = 0.017453292519943295769236907684886; // Degree gives the number of radians in one degree. It has a numerical value of Pi / 180.

//double CONST_NUM::DegreeD = 57.295779513082320876798154814105;// 1 / Degree

//double CONST_NUM::GoldenRatio = 1.6180339887498948482045868343656; // GoldenRatio is the golden ratio ((1+sqrt(2)) / 2), with numerical value 

//double CONST_NUM::EulerGamma = 0.57721566490153286060651209008241; //EulerGamma is Euler's ant Gamma,  

//double CONST_NUM::Catalan = 0.91596559417721901505460351493238; // Catalan's ant is given by the sum Sum[power(-1, k)power(2 k +1, -2)];

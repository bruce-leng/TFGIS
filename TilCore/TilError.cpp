 ///////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISERROR
//
// @module TilError.cpp |
//
// Implementation of the error objects member functions.
//
// <nl>
//
// Copyright © 1996-2000 Microsoft Corporation, All Rights Reserved
// <nl>
//
// @xref <l TilError\.h>
//
// <nl>
// @index | VISERROR
//
///////////////////////////////////////////////////////////////////////////


// This should be the first preprocessor statement in the file so that
// automatic precompiled headers work correctly.
//#include "TilCorePch.h"
//#include "TilCoreProj.h"

#include "stdafx.h"
#include "TilCore.h"

#include <stdarg.h>
#include "tilbase.h"
#include "TilError.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG

// @func Look through the error code array and return an error message from
//        the message array that has the same index. If the code in not
//        found, return a message "Unknown error".
// @rdesc An error message corresponding to an error code.
TILICEXPORT const char * TilErrorCodeToMessage(
      long lErrorCode,					// @parm An error code.
	  const char *rgszErrorMessage[],	// @parm An array of error messages
	  long rglErrorCode[])				// @parm A dictionary of error codes.
{
	for ( int i = 0; rgszErrorMessage[i]; i++ )
	{
		if ( lErrorCode == rglErrorCode[i] )
			return rgszErrorMessage[i];
	}

    return "Unknown error";
}
// @func Look through the message array and return a code from code array
//        with the same index. If the message is not in the message array,
//        the code for UNKNOWN error is returned.
// @rdesc An error code (defined in TilError.h) that corresponds to this particular error.
TILICEXPORT long TilErrorMessageToCode(
								  const char *szMessage,			// @parm Error message that corresponds to this particular error.
								  const char *rgszErrorMessage[],	// @parm An array of error messages
								  long rglErrorCode[])				// @parm A dictionary of error codes
{
	for ( int i = 0; rgszErrorMessage[i]; i++ )
	{
		if ( strstr(szMessage, rgszErrorMessage[i]) != NULL )
			return rglErrorCode[i];
	}
	
	return CTilError::ETilerrorUnknown;
}

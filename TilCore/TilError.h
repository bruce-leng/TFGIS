////////////////////////////////////////////////////////////////////////////
//
// @doc INTERNAL EXTERNAL VISERROR
//
// @module TilError.h |
//
// This file defines error objects used for the exception handling 
// in MSTilSDK. The basic object, CTilError, is a base class that provides
// some information about the exception (file name, function name, etc.)
// CTilFileIOError and CTilMemoryError are derived from CTilError class.
// They contain more specific information about the nature of the exception.
//
// <nl>
//
// @devnote:(INTERNAL)
//  
// Due to a bug in VC++, the _LINE_ macro doesn't return the right 
// line number. This causes an exception object to be created with a 
// wrong line number. Once this bug is fixed, line number should be 
// reported correctly by all the error objects.
//  
// FullMessage member function returns an error message with file name,
// function name and the error message specific to a certain error object.
// Each piece of this information can be obtained separately by using appropriate
// member functions.
//
// Protected member function ComposeMessages takes a number (n) of strings 
// and concatenates them together into one message.
//
// @todo:(INTERNAL)
// We should have macros to throw the common exceptions.
//
// We might want an exception for type mismatches.
//
////////////////////////////////////////////////////////////////////////////


#ifndef __VIS_CORE_ERROR_H__
#define __VIS_CORE_ERROR_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//#include <stdlib.h>
//#include <string>
#include "TILdef.h"
#include "Tilbase.h"
// Helper macro
// LATER:  Add more helper macros?
#define VIS_THROW_OOM(szFunctionName) \
throw CTilMemoryError(szFunctionName, __FILE__, __LINE__)


////////////////////////////////////////////////////////////////////////////
//
// @enum ETilError |
// Error Codes for various errors that might occur (mostly file I/O).
//
// @devnote:(INTERNAL) LATER:  May want more specific memory errors.
//
// @xref <c CTilError>
//
////////////////////////////////////////////////////////////////////////////
//##ModelId=3FDF232C02AF

// UNDONE (in Rel1):  We shouldn't use std::string in these classes, since
// std::string may need to allocate memory.  Instead, we should
// only use the static strings passed to the ctors.


//错误处理类
//##ModelId=3FFE4BE80011
class AFX_EXT_CLASS CTilError : public CUserException
{
	// @access Public members
public:
	enum 
	{
		// Unknown error type.
		//##ModelId=3FDF232C02CE
		ETilerrorUnknown			=  0,
		
		// A feature was not implemented when the library was built.
		//##ModelId=3FDF232C02CF
		ETilerrorNotImplemented		=  1,
		
		// Type mismatch.
		//##ModelId=3FDF232C02DE
		ETilerrorTypeMismatch		=  2,
		
		// Image shape mismatch.
		//##ModelId=3FDF232C02EE
		ETilerrorImageShapeMismatch	=  3,
		
		// Out of memory error.
		//##ModelId=3FDF232C02FD
		ETilerrorMemory				= 10,
		
		// File name expected, but no file name given.
		//##ModelId=3FDF232C030D
		ETilerrorNoFilename			= 20,
		
		// File open failed.
		//##ModelId=3FDF232C031C
		ETilerrorOpen				= 21,
		
		// File read failed.
		//##ModelId=3FDF232C032C
		ETilerrorRead				= 22,
		
		// File write failed.
		//##ModelId=3FDF232C032D
		ETilerrorWrite				= 23,
		
		// Bad file format.
		//##ModelId=3FDF232C033C
		ETilerrorFormat				= 24,
		
		// Bad pixel type.
		//##ModelId=3FDF232C034B
		ETilerrorPixFmt				= 25,
		
		// Bad image shape.
		//##ModelId=3FDF232C035B
		ETilerrorShape				= 26,
		
		// Corrupt file.
		//##ModelId=3FDF232C036B
		ETilerrorCorrupt			= 27,
		
		// An operation failed unexpectedly.  For example, a matrix operation
		// might encounter a singluar matrix or a required image operation may
		// not be supported.
		//##ModelId=3FDF232C037A
		ETilerrorOpFailed			= 28,
		
		// Miscellaneous error using the ImageMagisk file handler.
		//##ModelId=3FDF232C037B
		ETilerrorMagickMisc			= 29,
		
		// Unexpected.
		//##ModelId=3FDF232C038A
		ETilerrorUnexpected			= 100
	};
	//构造函数
	//##ModelId=3FFE4BE80020
	CTilError(const char *szMessage, long lErrorCode,   
		const char *szFunctionName = 0, 
		const char *szSourceFile = 0, 
		int iLine = 0) throw();
	
	// @access:(IMPL) Protected members
protected:
	// Function name that the error occurred in.
	//##ModelId=3FFE4BE80032
	const char *m_szFunctionName;
	
	// Error message.
	//##ModelId=3FFE4BE8003E
	const char *m_szMessage;
	
	// Source file that the error occurred in.
	//##ModelId=3FFE4BE8004E
	const char *m_szSourceFile;
	
	// Line number that the error occurred on.
	//##ModelId=3FFE4BE8004F
	int  m_line;
	
	// Error code.
	//##ModelId=3FFE4BE8005D
	long m_code;
};

//文件I/O错误处理处理类
//##ModelId=3FFE4BE8006D
class AFX_EXT_CLASS CTilFileIOError : public CFileException
{
	// @access Public members
public:
	
	
private:
};



////////////////////////////////////////////////////////////////////////////
//
// @class
//
// Error Objects for Memory Allocation Failure.
// It is not clear if this class is needed because 
// Windows will generate it's own exception for memory
// allocation problems.
//
// @base public | CTilError
//
// @xref <c CTilError>
//
////////////////////////////////////////////////////////////////////////////
//##ModelId=3FFE4BE8007D
class AFX_EXT_CLASS CTilMemoryError :public CMemoryException
{
	// @access Public members
public:
};

////////////////////////////////////////////////////////////////////////////
//
//	Translation between codes and messages
//
////////////////////////////////////////////////////////////////////////////

TILICEXPORT long TilErrorMessageToCode(const char *szMessage,
																			 const char *rgszErrorMessage[],
																			 long rglErrorCode[]);

TILICEXPORT const char * TilErrorCodeToMessage(long lErrorCode,
																							 const char *rgszErrorMessage[],
																							 long rglErrorCode[]);


#include "TilError.inl"


#endif // __VIS_CORE_ERROR_H__


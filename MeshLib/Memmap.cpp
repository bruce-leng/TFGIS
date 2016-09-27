/*
Module : MEMMAP.CPP
Purpose: Implementation for an MFC class to wrap memory mapped files
Created: PJN / MEMMAP/1 / 19-03-1997
History: PJN / 31-3-1998  1) Class now avoids trying to lock the mutex if only read access is required
                          2) User now has the option of specifying whether a file should be mapped with 
                             A Null terminator at the end. Can prove helpful when you want to use some
                             of the "C" runtime functions on the pointer returned.
         PJN / 20-4-1998  1) Now uses GetFileSize SDK call instead of GetFileInformationByHandle as a
                             more "reliable" way to determine file length.
                          2) Included TRACE statements to call GetLastError in all places where 
                             SDK functions fail
         PJN / 29-5-1998  1) Mapping a file now has the option of making it named or not.
         PJN / 22-10-1998 1) Fixed a bug in a number of calls to CreateMappingName when the classes were 
                             being used to share memory.
                          2) Tidy up of the demo app including:
                             a) Made the amount of text being shared a constant of MAX_EDIT_TEXT instead
                             of hardcoding it to 20 everywhere in the sample.
                             b) Changed where the timer is being created to OnInitDialog
                             c) Tidied up the initialisation sequence in OnInitDialog
                             d) Now using _tcscpy instead of _tcsncpy to ensure array is null terminated
                             e) Fixed resource.h which was causing the resources to fail to compile
                             f) Removed unnecessary symbols from resource.h
                             g) Optimized the way the OnTimer code works to only update the text when it
                             has changed in the MMF. This means that you can type continuously into the
                             edit control.
                          3) New documentation in the form of a HTML file.
                          4) Sample now ships as standard with VC 5 workspace files
         PJN / 30-3-1999  1) Code is now unicode compliant
                          2) Code now supports growable MMF's
                          3) Addition of accessor functions for file handle and file mapping handle
         PJN / 21-4-1999  1) Works around a Window bug where you try to memory map a zero length file on
                             Windows 95 or 98.




Copyright (c) 1997 - 1998 by PJ Naughter.  
All rights reserved.

*/

/////////////////////////////////  Includes  / Defines ////////////////////////
#include "stdafx.h"
#include "memmap.h"
#include <winioctl.h>

//Define taken from the Windows 2000 Beta platform SDK, means that CMemMapFile 
//will compile correctly even on systems which do have have the platform SDK
//installed
#ifndef FSCTL_SET_SPARSE
#define FSCTL_SET_SPARSE                CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 49, METHOD_BUFFERED, FILE_WRITE_DATA)
#endif


///////////////////////////////// Implementation //////////////////////////////



IMPLEMENT_DYNAMIC(CMemMapFile, CObject)


CMemMapFile::CMemMapFile()
{
  //Initialise variables to sane values
  m_hFile = INVALID_HANDLE_VALUE;
  m_hMapping = NULL;
  m_bReadOnly = TRUE;
  m_bAppendNull = FALSE;
  m_lpData = NULL;
  m_bOpen = FALSE;
  m_hMutex = NULL;
  m_dwLength = 0;
}

CMemMapFile::~CMemMapFile()
{
  UnMap();
}

BOOL CMemMapFile::MapFile(const CString& sFilename, BOOL bReadOnly, DWORD dwShareMode, BOOL bAppendNull, BOOL bNamed, BOOL bGrowable)
{
  //Work out the file access flags
  m_bReadOnly = bReadOnly;
	DWORD dwDesiredFileAccess = GENERIC_READ;
	if (!m_bReadOnly)
		dwDesiredFileAccess |= GENERIC_WRITE;

  //store away the append Null flag
  m_bAppendNull = bAppendNull;

  //Open the real file on the file system
  m_hFile = CreateFile(sFilename, dwDesiredFileAccess, dwShareMode, NULL, 
                       OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (m_hFile == INVALID_HANDLE_VALUE)
  {
    TRACE(_T("Failed in call to CreateFile, GetLastError returned %d\n"), GetLastError());
    UnMap();
    return FALSE;
  }

  // Make the file sparse, if requested to make the memory mapped file growable
  if (bGrowable)
  {
    DWORD dw;
    if (!::DeviceIoControl(m_hFile, FSCTL_SET_SPARSE, NULL, 0, NULL, 0, &dw, NULL))
    {
      TRACE(_T("Failed in call to make file sparse, You need Windows 2000 and an NTFS 5 volume for this !!, GetLastError returned %d\n"), GetLastError());
      UnMap();
      return FALSE;
    }
  }

	// Get the size of the file we are mapping
  DWORD dwFileSizeHigh=0;
  m_dwLength = GetFileSize(m_hFile, &dwFileSizeHigh);
  if (m_dwLength == 0xFFFFFFFF)
  {
    //There was an error calling GetFileSize
    TRACE(_T("Failed in call to GetFileSize, GetLastError returned %d\n"), GetLastError());
    UnMap();
    return FALSE;
  }

  //Fail if file is greater than 4GB in size
  if (dwFileSizeHigh)
  {
    //There was an error calling GetFileSize
    TRACE(_T("File size is greater than 4GB, Memory mapping this file size will not work until CMemMapFile supports Win64 !!\n"));
    UnMap();
    return FALSE;
  }

  //Fail if file is 0 length in size, calling CreateFileMapping on a 
  //zero length file on 95/98 can cause problems
  if (dwFileSizeHigh == 0 && m_dwLength == 0)
  {
    TRACE("File size is 0, not attempting to memory map the file\n");
    UnMap();
    return FALSE;
  }

  //Do the actual mapping
  m_sMappingName = CreateMappingName(sFilename, bNamed);
  return MapHandle(m_hFile);
}

BOOL CMemMapFile::MapMemory(const CString& sName, DWORD dwBytes, BOOL bReadOnly)
{
  m_dwLength = dwBytes;
  m_bReadOnly = bReadOnly;
  m_bAppendNull = FALSE;
  m_sMappingName = CreateMappingName(sName, TRUE);
  return MapHandle((HANDLE)0xFFFFFFFF);
}

BOOL CMemMapFile::MapExistingMemory(const CString& sName, DWORD dwBytes, BOOL bReadOnly)
{
  m_dwLength = dwBytes;
  m_bReadOnly = bReadOnly;

  DWORD dwDesiredAccess = (!bReadOnly) ? FILE_MAP_WRITE : FILE_MAP_READ;
  m_sMappingName = CreateMappingName(sName, TRUE);
  m_hMapping = OpenFileMapping(dwDesiredAccess,	0, m_sMappingName);
  if (m_hMapping == NULL)
  {
    TRACE(_T("Failed in call to OpenFileMapping, GetLastError returned %d\n"), GetLastError());
    UnMap();
    return FALSE;
  }

  //Map the view
  m_lpData = MapViewOfFile(m_hMapping, dwDesiredAccess, 0, 0, m_dwLength);

  //Create the mutex to sync access
  m_hMutex = CreateMutex(NULL, FALSE, CreateMutexName());

  return (m_lpData != NULL);
}

BOOL CMemMapFile::MapHandle(HANDLE hHandle)
{
  //Create the file mapping object
  DWORD flProtect = (!m_bReadOnly) ? PAGE_READWRITE : PAGE_READONLY;

  //work out the length of the file mapping to create
  DWORD dwLength = m_dwLength;
  if (m_bAppendNull)
    dwLength += 2;

  m_hMapping = ::CreateFileMapping(hHandle, NULL, flProtect, 0, dwLength, m_sMappingName);
  if (m_hMapping == NULL)
  {
    TRACE(_T("Failed in call to CreateFileMapping, GetLastError returned %d\n"), GetLastError());
    UnMap();
    return FALSE;
  }

  //Map the view
	DWORD dwDesiredAccess = (!m_bReadOnly) ? FILE_MAP_WRITE : FILE_MAP_READ;
  m_lpData = MapViewOfFile(m_hMapping, dwDesiredAccess, 0, 0, 0);

  //null terminare if asked to do so
  if (m_bAppendNull && m_lpData)
  {
    //use two Nulls just incase the data as viewed as an array of wide characters
    BYTE* lpData = (BYTE*) m_lpData;
    lpData[m_dwLength] = 0;
    lpData[m_dwLength+1] = 0;
  }

  //Create the mutex to sync access
  m_hMutex = CreateMutex(NULL, FALSE, CreateMutexName());

  return (m_lpData != NULL);
}

LPVOID CMemMapFile::Open(DWORD dwTimeout)
{
  if (m_lpData == NULL)
    return NULL;

  if (m_bReadOnly)
  {
    m_bOpen = TRUE;
	  return m_lpData;
  }
  else
  {
    //Synchronise access to the MMF using the named mutex
	  DWORD dwResult = WaitForSingleObject(m_hMutex, dwTimeout);

//	  if (dwResult == WAIT_OBJECT_0)
//	  {
//		  m_bOpen = TRUE;
//		  return m_lpData;
//	  }
		  return m_lpData;

    return NULL;
  }
}

BOOL CMemMapFile::Close()
{
  //Release our interest in this MMF
  if (!m_bOpen)
    return FALSE;

  m_bOpen = FALSE;
  ReleaseMutex(m_hMutex);
  
  return TRUE;
}

BOOL CMemMapFile::Flush()
{
  if (m_lpData == NULL)
		return FALSE;

  return FlushViewOfFile(m_lpData, 0);
}

CString CMemMapFile::CreateMutexName() const
{
  return m_sMappingName + _T("MUTEX");
}

void CMemMapFile::UnMap()
{
  //Close any views which may be open
	Close();

  //unmap the view
	if (m_lpData != NULL)
	{
		FlushViewOfFile(m_lpData, 0);
		UnmapViewOfFile(m_lpData);
		m_lpData = NULL;
	}

  //remove the file mapping
	if (m_hMapping != NULL)
	{
		CloseHandle(m_hMapping);
		m_hMapping = NULL;
	}

  //close the file system file if its open
	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}

  //Close the mutex we have been using
	if (m_hMutex != NULL)
	{
		CloseHandle(m_hMutex);
		m_hMutex = NULL;
	}

  //Reset the remaining member variables
  m_bReadOnly = TRUE;
  m_sMappingName = _T("");
  m_dwLength = 0;
}

CString CMemMapFile::CreateMappingName(const CString& sName, BOOL bNamed)
{
  CString rVal;
  if (bNamed)
  {
    rVal = sName;

    //Replace all '\' by '_'
    for (int i=0; i<sName.GetLength(); i++)
    {
      if (rVal.GetAt(i) == _T('\\'))
        rVal.SetAt(i, _T('_'));
    }

    rVal.MakeUpper();
  }

  return rVal;
}

#ifdef _DEBUG
void CMemMapFile::AssertValid() const
{
  CObject::AssertValid();
}
#endif

#ifdef _DEBUG
void CMemMapFile::Dump(CDumpContext& dc) const
{
  CObject::Dump(dc);

  CString sText;
  sText.Format(_T("m_hFile=0x%08x\n"), m_hFile);
  dc << sText;
  sText.Format(_T("m_hMapping=0x%08x\n"), m_hMapping);
  dc << sText;
  sText.Format(_T("m_bReadOnly=%d\n"), m_bReadOnly);
  dc << sText;
  sText.Format(_T("m_lpData=0x%08x\n"), m_lpData);
  dc << sText;
  sText.Format(_T("m_sMappingName=%s\n"), m_sMappingName);
  dc << sText;
  sText.Format(_T("m_bOpen=%d\n"), m_bOpen);
  dc << sText;
  sText.Format(_T("m_hMutex=0x%08x\n"), m_hMutex);
  dc << sText;
  sText.Format(_T("m_dwLength=%d\n\n"), m_dwLength);
  dc << sText;
}
#endif


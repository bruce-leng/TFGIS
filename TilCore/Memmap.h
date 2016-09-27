/*
Module : MEMMAP.H
Purpose: Interface for an MFC class to wrap memory mapped files
Created: PJN / 30-07-1997


Copyright (c) 1997 - 1998 by PJ Naughter.  
All rights reserved.

*/


////////////////////////////////// Macros ///////////////////////////

#ifndef __MEMMAP_H__
#define __MEMMAP_H__ 




/////////////////////////// Classes /////////////////////////////////


class AFX_EXT_CLASS CMemMapFile : public CObject
{
public:
//Constructors / Destructors
  CMemMapFile();
  ~CMemMapFile();

//Methods
  BOOL    MapFile(const CString& sFilename, BOOL bReadOnly = FALSE, DWORD dwShareMode = 0, 
                  BOOL bAppendNull = FALSE, BOOL bNamed = FALSE, BOOL bGrowable = FALSE);
  BOOL    MapMemory(const CString& sName, DWORD dwBytes, BOOL bReadOnly = FALSE);
  BOOL	  MapExistingMemory(const CString& sName, DWORD dwBytes, BOOL bReadOnly = FALSE);
  void    UnMap();
  LPVOID  Open(DWORD dwTimeout = INFINITE);
  BOOL	  Close();
  BOOL	  Flush();

//Accessors
  CString GetMappingName() const { return m_sMappingName; }   ;
  HANDLE  GetFileHandle() const { return m_hFile; };
  HANDLE  GetFileMappingHandle() const { return m_hMapping; };
  BOOL    IsOpen() const { return m_bOpen; };
  DWORD   GetLength() const { return m_dwLength; };

//Diagnostics / Debug
  #ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
  #endif

protected:
  DECLARE_DYNAMIC(CMemMapFile)

  BOOL MapHandle(HANDLE hHandle);
  CString CreateMappingName(const CString& sName, BOOL bNamed);
  CString CreateMutexName() const;

  HANDLE  m_hFile;
  HANDLE  m_hMapping;
  BOOL    m_bReadOnly;
  BOOL    m_bAppendNull;
  LPVOID  m_lpData;
  CString m_sMappingName;
  BOOL    m_bOpen;
  HANDLE  m_hMutex;
  DWORD   m_dwLength;
};


#endif //__MEMMAP_H__
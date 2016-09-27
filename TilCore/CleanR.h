//////////////////////////////////////////////////////////////////////
/*
Header
Source code to the CleanR 
*/
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLEANR1_H__37A96329_ACF6_40E2_8C57_63C5B7253819__INCLUDED_)
#define AFX_CLEANR1_H__37A96329_ACF6_40E2_8C57_63C5B7253819__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//////////////////////////////////////////////////////////////////////

//Structure to interface with corresponding BOOLs in CCleanR, will replace with <bitset> soon
struct CCleanRboolSet{
	BOOL bStrict,bCheckOnlyWords,bLastWordMayNotHavePunct,bCaseSensetive,bOverWriteFile,bReplaceLFWithCRLF,bReplaceCRLFWithLF;
};


class AFX_EXT_CLASS CCleanR  
{
public:
	DWORD Process(); //Initiates start of processing, and returns the number of matches found
	BOOL SetReplacementString(const char *szReplacementString);
	BOOL SetSearchString(const char *szSearchString);
	BOOL SetFileName(const char *szFileName);
	CCleanR();
	CCleanR::CCleanR(CCleanRboolSet boolSet);
	virtual ~CCleanR();

private:
	BOOL IsCharBelongingToSet(char cCharToTest,char *szSetValues);
	BOOL m_bReplaceCRLFWithLF;
	BOOL m_bReplaceLFWithCRLF;
	BOOL m_bLastWordMayNotHavePunct;
	BOOL m_bCheckOnlyWords;
	BOOL m_bStrict;
	BOOL m_bStrainFound;
	BOOL m_bSearchIsFinished;
	BOOL m_bCaseSensetive;
	BOOL m_bOverWriteFile;
	CString m_sSearchString;
	CString m_sReplacementString;
	CString m_sTempSearchString;
	CString m_sInputFileName;
	CString m_sOutputFileName;
};

#endif // !defined(AFX_CLEANR1_H__37A96329_ACF6_40E2_8C57_63C5B7253819__INCLUDED_)

/*
CleanR1.cpp: implementation of the CCleanR class.
This implementation to be used in MFC capable projects
Exception handling pending..

Limitations:
------------
In a ASCII text file, a newline is stored as just a SINGLE newline ['\n' or integer '10'], but Microsoft's CString transforms them into a "\r\n" ie a carriage return - newline pair, and thus Find() takes them as two differnet strings !
In a nutshell -> Any '\n' [int 10] is converted in to a "\r\n" [int 13,10] pair by CString.

Hence I am converting all text into CString compatible form.
  .. and any suggestions will be greatfully appreciated.

Last Updated:
-------------
12:35 PM 6/23/2003

  A sequence of alphabets 
	(1) preceded and followed by ' ', OR preceded by NULL but followed by ' '(incase of the first word) OR preceded by a newline.but followed by ' ' 
	(2) OR preceded by ' ' and followed by standard punctutaion mark -> :;",.?!` and 
	 (2.1) if bStrict is TRUE, the next character after the punctuation HAS to be a ' ', else if bStrict is FALSE it can be anything
	
*/
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CleanR.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
//The followings are customizable.Change/add to suit your needs
char szPrecedeChars[]=" ,.;:!?\r\n"; //This string defines what characters constitute a valid preceding character(the one preceding a word)
char szFollowingChars[]=" ,.;:!?\r\n"; //This string defines what can constitutes a valid set of chracters which can follow a word. This is the string defining a 'lax'(NOT Strict) grammer document
char szStrictPrecedeChars[]=" \r\n";
char szValidPunctuation[]=",.:;!?\"\r\n"; //Did I miss anything
//////////////////////////////////////////////////////////////////////

CCleanR::CCleanR()
{
	//Initialize things to default values
	m_bStrict=FALSE;
	m_bCheckOnlyWords=FALSE;
	m_bLastWordMayNotHavePunct=FALSE; 
	m_sSearchString=_T("AppWizard"); //Just a default parameter
	m_sReplacementString=_T("");
	m_sTempSearchString=_T("");//To be filled up incase of case insensetive searches
	m_sInputFileName=_T("Test.TXT");//Just a default parameter
	m_sOutputFileName=_T("TestOut.TXT");//Just a default parameter
	m_bStrainFound=FALSE;
	m_bSearchIsFinished=TRUE;
	m_bCaseSensetive=TRUE;
	m_bOverWriteFile=TRUE;
	m_bReplaceLFWithCRLF=FALSE;
}

CCleanR::CCleanR(CCleanRboolSet boolSet)
{
	//Initialize options with 'boolSet' values
	m_bStrict=boolSet.bStrict;
	if(m_bStrict) m_bCheckOnlyWords=TRUE;//If bStrict is TRUE, bCheckOnlyWords is automatically to be TRUE(guess why )
	else m_bCheckOnlyWords=boolSet.bCheckOnlyWords;
	m_bLastWordMayNotHavePunct=boolSet.bLastWordMayNotHavePunct; 
	m_sSearchString=_T("AppWizard"); //Just a default parameter
	m_sReplacementString=_T("");
	m_sTempSearchString=_T("");//To be filled up incase of case insensetive searches
	m_sInputFileName=_T("Test.TXT");//Just a default parameter
	m_sOutputFileName=_T("TestOut.TXT");//Just a default parameter
	m_bStrainFound=FALSE;
	m_bSearchIsFinished=TRUE;
	m_bCaseSensetive=boolSet.bCaseSensetive;
	m_bOverWriteFile=boolSet.bOverWriteFile;
	m_bReplaceLFWithCRLF=boolSet.bReplaceLFWithCRLF;
	m_bReplaceCRLFWithLF=boolSet.bReplaceCRLFWithLF;
}

CCleanR::~CCleanR()
{

}

BOOL CCleanR::IsCharBelongingToSet(char cCharToTest, char *szSetValues)
{
	/*
	Function to test of cCharToTest exists in szSetValues.
	This INCLUDES the terminating NULL string, since a word can ALSO be followed/preceded by a NULL
	*/
	for(unsigned int i=0;i<=strlen(szSetValues);i++) //We want to compare the NULL too.
		if(szSetValues[i]==cCharToTest) return TRUE;
	return FALSE;
}

BOOL CCleanR::SetFileName(const char *szFileName)
{
	//Sets the m_sFileName value
	if(lstrcmp(szFileName,"")==0) return false;//AfxMessageBox(("No filename was selected! Will use default filename "+m_sInputFileName+"."));
	else m_sInputFileName=szFileName;
	m_sOutputFileName = szFileName;
	return TRUE;
}

BOOL CCleanR::SetSearchString(const char *szSearchString)
{
	//Sets the m_sSearchString value
	if(lstrcmp(szSearchString,"")==0) return false;//AfxMessageBox(("Search string was empty!\nWill be using default search string \""+m_sSearchString+"\".")); //perhaps a 'throw' would have been better ?
	else m_sSearchString=szSearchString;
	return TRUE;
}

BOOL CCleanR::SetReplacementString(const char *szReplacementString)
{
	//Sets the m_sReplacementString value
	m_sReplacementString=szReplacementString;
	return TRUE;
}

DWORD CCleanR::Process()
{
	//Does the processing, and returns the number of matches found
	CString m_sReadFromFile,sWriteToFile,m_sTempSearchString,m_sTempReadFromFile;
	HANDLE hFile=CreateFile((LPCTSTR)m_sInputFileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE) 
	{
		//AfxMessageBox(("File open error! Skipping file.\n["+m_sInputFileName+"]") ,MB_OK|MB_ICONINFORMATION);
		return 0;
	}	
	DWORD dwBufferSize=GetFileSize(hFile,NULL),dwBytesRead=0;
	
	if(dwBufferSize==-1) return 0;//Could not get file size
	//Let's get a buffer dwBufferSize bytes long
	BOOL bResult=ReadFile(hFile,m_sReadFromFile.GetBuffer(dwBufferSize),dwBufferSize,&dwBytesRead,NULL); //Debug Alert !! CString::GetBuffer() called, now NOTHING but ReleaseBuffer() to be called.
	m_sReadFromFile.ReleaseBuffer(); //Done CString::ReleaseBuffer(). Object's now usable again
	CloseHandle(hFile);
	//Check upon above operation
	if((!bResult)||(dwBufferSize!=dwBytesRead))
	{
		//AfxMessageBox(("File read error! Skipping file.\n["+m_sInputFileName+"]") ,MB_OK|MB_ICONINFORMATION);
		return 0;
	}
	if(m_bReplaceLFWithCRLF) m_sReadFromFile.Replace("\n","\r\n"); //Replaces ALL LF from current file to CR\LF. This will also make the file properly laid out and readble in simple text editors like Notepad.
	if(m_bReplaceCRLFWithLF) m_sSearchString.Replace("\r\n","\n"); //Replaces ALL CR\LF to LF. This lets the user search files which represents a new line with a single 0x0A rather than the correct 0x0A & 0X0D pair, without changing the filesize of the processed file due to lone LFs being changed to CR/LF as done in above option.
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
	Well, sReadFromFile now contains the WHOLE file contents !
	This eats up RAM , but drastically improves program performance.
	Any suggestions ?(If you are thinking of CreateFileMapping,MapViewOfFile.. etc, please send it in code ;) - They are a real pain-in-the-you-know-what !
	*/

	/*
	Currently, when we are making case insensetive searches, we are copying the WHOLE file which was in m_sReadFromFile to m_sTempReadFromFile, but converting 
	it fully to uppercase. The same for sSearchString.

	This actually means that during case insensetive searches, we are effectively using DOUBLE the memory than the same, but case sensetive search
	*/
	DWORD dwMatches=0,dwCleanStrainPos=0,dwReadFileLen=m_sReadFromFile.GetLength(),dwStrainLen=m_sSearchString.GetLength();
	
	if(!m_bCaseSensetive) 
	{
		//We convert both 'Temp' strings to uppercase if it's a case insensetive search
		m_sTempSearchString=m_sSearchString;
		m_sTempReadFromFile=m_sReadFromFile;
		m_sTempSearchString.MakeUpper();
		m_sTempReadFromFile.MakeUpper();
		dwCleanStrainPos=m_sTempReadFromFile.Find(m_sTempSearchString);		
	}
	else dwCleanStrainPos=m_sReadFromFile.Find(m_sSearchString);
	if(dwCleanStrainPos!=-1) {m_bStrainFound=TRUE;m_bSearchIsFinished=!m_bStrainFound;}
	else m_bStrainFound=FALSE;

	if(m_bStrainFound)
	{
		//We will create the output file only iff the search string was found - no point in creating 0 byte files!
		if(m_bOverWriteFile) m_sOutputFileName=m_sInputFileName; //if OverWriteOption was chosen ..
		///Let's create the output file
		hFile=CreateFile((LPCTSTR)m_sOutputFileName,GENERIC_WRITE,FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		if(hFile==INVALID_HANDLE_VALUE) 
		{
			//AfxMessageBox(("Error writing output file!\nAborting.\n["+m_sOutputFileName+"]") ,MB_OK|MB_ICONSTOP);
			return 0;
		}
	}
	/*
	A word is identified as: [details moved to article]
	*/
	while(m_bStrainFound&&(!m_bSearchIsFinished))
	{
		//Search string found.Now check if user asked us to change substrings or only words
		dwReadFileLen=m_sReadFromFile.GetLength();
		dwMatches++;
		///////////////////////////////////////////
		sWriteToFile=m_sReadFromFile.Left(dwCleanStrainPos);
		if(m_bCheckOnlyWords)
		{
			DWORD dwFollowPos=(sWriteToFile.GetLength()+m_sSearchString.GetLength());

			char cFollow=NULL;
			if(dwFollowPos<m_sReadFromFile.GetLength()) cFollow=m_sReadFromFile.GetAt(dwFollowPos); //if the check within if() was NOT there, we could end up doing a out of bounds operation !
			char cPrecededBy=m_sReadFromFile.GetAt(dwCleanStrainPos-1);
			if(!m_bStrict)
			{
				//If we are NOT to be strict with grammer
				if(IsCharBelongingToSet(cPrecededBy,szPrecedeChars))
				{
					if(!IsCharBelongingToSet(cFollow,szFollowingChars)) sWriteToFile+=m_sReadFromFile.Mid(dwCleanStrainPos,m_sSearchString.GetLength());
					else sWriteToFile+=m_sReplacementString;// Match found, let's put in the replacement string
					//We will leave out m_sSearchString ONLY if cFollow was NOT among szFollowingChars,thus NOT qualifying it as  a word
					//Hey, we where ONLY asked to replace WORDS, not substrings, so give him the data back!
				}
				else sWriteToFile+=m_sReadFromFile.Mid(dwCleanStrainPos,m_sSearchString.GetLength());
			}
			else
			{
				//Grammer layout is strict - those strings NOT in confirmation will be excluded/ignored
				if(IsCharBelongingToSet(cPrecededBy,szStrictPrecedeChars))
				{
					if(IsCharBelongingToSet(cFollow,szFollowingChars))
					{
						//So far, so good, now if cFollow was a punctuation but NOT a NULL(which makes m_sSearchString here the LAST word) OR a blank/space, let's see if a space or a newline or a NULL follows
						if(((dwFollowPos+1)<m_sReadFromFile.GetLength())&&(cFollow!=' ')) cFollow=m_sReadFromFile.GetAt(dwFollowPos+1);//Let's get the next character into cFollow if it was not a space (all punctuations EXCEPT the space should be followed by a space)
						if(IsCharBelongingToSet(cFollow,szValidPunctuation)&&(cFollow!=' ')) 
						{
							if((!m_bLastWordMayNotHavePunct)&&(cFollow==NULL)) sWriteToFile+=m_sReadFromFile.Mid(dwCleanStrainPos,m_sSearchString.GetLength());//Match was found,but with the last word lacking a punctuation(probably a period), and the user wanted us to ignore this, so we don't put in the replacement string.
							else sWriteToFile+=m_sReplacementString; //Correctly formatted word found, replace it
						}
						else 
						{
							if(cFollow==' ') sWriteToFile+=m_sReplacementString;//Correctly formatted word found, replace it
							else sWriteToFile+=m_sReadFromFile.Mid(dwCleanStrainPos,m_sSearchString.GetLength());//cFollow was not in szValidPunctuation
						}
					}
					else sWriteToFile+=m_sReadFromFile.Mid(dwCleanStrainPos,m_sSearchString.GetLength());//cFollow was not in szFollowingChars
				}
				else sWriteToFile+=m_sReadFromFile.Mid(dwCleanStrainPos,m_sSearchString.GetLength());//cPrecededBy was not in szStrictPrecedeChars
			}
		}
		else sWriteToFile+=m_sReplacementString; //We are checking not only words but substrings too, so we replace every match
		dwCleanStrainPos=dwReadFileLen-(dwCleanStrainPos+dwStrainLen);
		m_sReadFromFile=m_sReadFromFile.Right(dwCleanStrainPos);//Dynamically reducing the memory usage.Writing 'Clean' stream to disk and removing from RAM the portion written to disc.
		if(!m_bCaseSensetive) m_sTempReadFromFile=m_sTempReadFromFile.Right(dwCleanStrainPos);//Dynamically reducing the memory usage.Writing 'Clean' stream to disk and removing from RAM the portion written to disc.
		if(m_bStrainFound) 
		{
			dwBufferSize=sWriteToFile.GetLength();
			bResult=WriteFile(hFile,(LPCTSTR)sWriteToFile,dwBufferSize,&dwBytesRead,NULL);
			if((!bResult)||(dwBufferSize!=dwBytesRead)) //Read dwBytesRead as dwBytesWritten
			{
				//AfxMessageBox(("File write error! Skipping file.\n["+m_sOutputFileName+"]") ,MB_OK|MB_ICONSTOP);
				return 0;
			}
		}
		//Check for more occurences
		if(((!m_bCaseSensetive)&&((dwCleanStrainPos=m_sTempReadFromFile.Find(m_sTempSearchString))==-1))||((m_bCaseSensetive)&&((dwCleanStrainPos=m_sReadFromFile.Find(m_sSearchString))==-1))) m_bSearchIsFinished=TRUE; //No more occurences found [Glad someone was patient enough to go till here from the if.. :) ]
	}//End of while(m_bStrainFound)
	/////////Write out rest of file which did not contain m_sSearchString at all
	if(m_bStrainFound) dwCleanStrainPos=dwReadFileLen-(dwCleanStrainPos+dwStrainLen);
	else 
	{
		//AfxMessageBox("Sorry, the supplied search string \""+m_sSearchString+"\" was not found.");
		dwCleanStrainPos=0;
	}
	/*
	If the file did not have any matching strings, we write nothing to our output file, 
	else we write the last dwCleanStrainPos bytes which are the bytes immediately following the LAST occurence of m_sSearchString
	*/
	if(!m_sReadFromFile.IsEmpty())
	{
		sWriteToFile=m_sReadFromFile.Right(dwCleanStrainPos);
		if(m_bStrainFound) 
		{
			dwBufferSize=sWriteToFile.GetLength();
			bResult=WriteFile(hFile,(LPCTSTR)sWriteToFile,dwBufferSize,&dwBytesRead,NULL);
			if((!bResult)||(dwBufferSize!=dwBytesRead)) //Read dwBytesRead as dwBytesWritten
			{
				//AfxMessageBox(("File write error! Skipping file.\n["+m_sOutputFileName+"]") ,MB_OK|MB_ICONSTOP);
				return 0;
			}
		}
	}
	if(m_bStrainFound) CloseHandle(hFile);
	return dwMatches;
}

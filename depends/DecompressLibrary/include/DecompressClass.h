// DecompressLibrary.h  Version 1.0
//
// Authors:      Marius Ciorecan (cmaryus@yahoo.com)
//
//
// Version 1.0   - Using XUnzip and gzip classes to make a general class that could decompress a zip, gz, tar.gz
// //
///////////////////////////////////////////////////////////////////////////////
//
// Marius Ciorecan comments:
// --------------------------
// The class uses the code from :
//		- XUnzip library written by Hans Dietrich (hdietrich2@hotmail.com) for decompression of zip files.
//		- CGzip library written by Jonathan de Halleux (dehalleux@pelikhan.com)
//	All other source files from this library contain the original source code, see there the copyright
//	The class uses also the specifications for the TAR format, in order to extract data from a tar file.

#pragma once

#include <atlstr.h>
#include <Windows.h>
#include "XUnzip.h"
#include "GnuTar.h"
#include "gzip.h"

using namespace zlib;

enum EArchiveType
{
	UNKNOWN_ARCHIVE = 0,
	ZIP_ARCHIVE,
	GZIP_ARCHIVE,
	TAR_GZIP_ARCHIVE
};

class CDecompressClass
{
public:
	CDecompressClass(void);
	~CDecompressClass(void);

	/***********************************************************************
	Open a compressed file.
	Parameters
		pszCompressedFile - name of the file
	Return values:
		TRUE - file opened
		FALSE - file could not be opened (use GetErrorText() to find the errror)\
	Note:
		The archive file must have one of these extensions:
		- .zip
		- .gz
		- .tar.gz
		- .tgz
	***********************************************************************/
	BOOL	OpenArchive(IN const wchar_t* pszArchiveFile);

	/***********************************************************************
	Closes the archive, destroys all the internal objects
	***********************************************************************/
	void	CloseArchive();

	/***********************************************************************
	Get number of files in the archive
	Return values:
		Number of files in the archive.
		-1 if there are errors.
	***********************************************************************/
	int		GetCompressedFilesCount();

	/***********************************************************************
	Get a buffer that contain the file in archive at a specific position
	Params:
		nFileIndex - file position in archive
		pszFileBuffer - buffer containg the file data
		nFileSize - file size
		fIsFile - TRUE if this is a regular file, FALSE otherwise (directory, link, etc)
		szFileName - name of the file
	Return values:
		TRUE - file found and buffer filled correctly
		FALSE - some error (use GetErrorText() to find the error)
	Note:
		All data buffers will be allocated/deleted internally in this class,
		they should not be changed from outside !
	***********************************************************************/
	BOOL	GetArchiveFile(	IN int nFileIndex, 
							OUT char** pszFileBuffer, 
							OUT int& nFileSize,
							OUT BOOL& fIsFile,
							OUT CString& szFileName);


	/***********************************************************************
	Get last error if there were any problems
	***********************************************************************/
	CString	GetErrorText();

private:

	// gzip object
	// we will use this if it's a gzip file
	CGZip			m_GZIP;
	
	// (for .zip we will use the unzip API's from Mark Adler library)
	HZIP			m_hZIP;
	
	// name of the archive
	CString			m_szArchiveName;

	// buffer with all data from the archive file
	char*			m_pFileBuffer;
	size_t			m_nBufferLen;

	// last error
	CString			m_szLastError;

public:
	// type of the archive
	EArchiveType	m_nArchiveType;
};

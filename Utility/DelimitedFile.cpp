#include "stdafx.h"
#include "DelimitedFile.h"
#include "GMMemory.h"

// TODO: CDelimitedFile::Write 계열 제작

CDelimitedFile::CDelimitedFile(const char* pszDelimiter)
:	m_fpFile(0), m_dwColumn(0), m_dwColumnCount(0)
{
	m_ColumnNames.reserve(DEFAULT_COLUMN_NUM);
	strncpy(m_szDelimiter, pszDelimiter, MAX_DELIMITER_NUM);
}

CDelimitedFile::~CDelimitedFile()
{
	Close();	
}

BOOL CDelimitedFile::Open( LPCSTR szFilename, int nHeadLine, UINT nOpenFlags )
{
	m_fpFile = ::fopen( szFilename, (nOpenFlags & modeCreate) ? "wt" : "rt" );
	if (NULL == m_fpFile)
	{
        #ifdef ERRLOG2
		DWORD dwError = ::GetLastError();
		ERRLOG2(g_Log, "파일 %s를 열 수 없습니다. Error Code : %d", szFilename, dwError);
		#endif

		return FALSE;
	}

	if (nHeadLine > -1) 
	{
		for (int i=0; i<nHeadLine; ++i)
		{
			ReadLine();
		}

		for(m_dwColumnCount = 0; true; ++m_dwColumnCount)
		{
			char* szColumnName = new char[MAX_COLUMN_LENGHT];
			
			if(0 == szColumnName) 
			{ 
				return FALSE; 
			}

			if(false == ReadString(szColumnName, MAX_COLUMN_LENGHT - 1)) 
			{
				delete szColumnName;
				break;
			}

			size_t nLength = strlen(szColumnName);
			while(nLength-- > 0)
			{
				if(szColumnName[nLength] == '\r' || szColumnName[nLength] == '\n')
				{
					szColumnName[nLength] = 0;
				}
			}

			szColumnName[MAX_COLUMN_LENGHT - 1] = 0;
			m_ColumnNames.push_back(szColumnName);
		}
	}
	return TRUE;
}

void CDelimitedFile::Close( void )
{
	if( m_fpFile )
	{
		::fclose( m_fpFile );
		m_fpFile = 0;
	}

	ColumnArray::size_type nTotalSize = m_ColumnNames.size();
	for(ColumnArray::size_type nCount = 0; nCount < nTotalSize; ++nCount) 
	{ 
		delete [] m_ColumnNames[nCount];
	}

	m_ColumnNames.clear();
}

BOOL CDelimitedFile::ReadLine( void )
{
	while( ::fgets( m_szLine, MAX_LINE_LENGTH, m_fpFile ) ) {
		if( (m_szLine[0]) && (m_szLine[0] != ';') ) {
			m_dwColumn = 0;
			memcpy(m_szBackupLine, m_szLine, MAX_LINE_LENGTH);
			return TRUE;
		}
	}
	return FALSE;
}

// 빈줄 및 C++ 스타일 주석 지원
bool CDelimitedFile::ReadLine2nd(void)
{
	while (NULL != ::fgets(m_szLine, MAX_LINE_LENGTH, m_fpFile))
	{
		if (0 == strcmp("\n", m_szLine))
		{
			continue;
		}

		if ('/' != m_szLine[0] && '/' != m_szLine[1])
		{
			m_dwColumn = 0;
			memcpy(m_szBackupLine, m_szLine, MAX_LINE_LENGTH);
			return true;
		}
	}

	return false;
}

BOOL CDelimitedFile::ReadData( bool &bNumber )
{
	CHAR *pszData = ::strtok( (m_dwColumn++) ? NULL : m_szLine, m_szDelimiter );

	if( !pszData || !(*pszData) )
		return FALSE;
		
	bNumber = false;
	if (atoi(pszData) == 1)
		bNumber = true;

	char szString[2];
	strncpy(szString, pszData, 1);
	if (NULL == ::strcmp(szString, "O"))
		bNumber = true;

	return TRUE;
}

BOOL CDelimitedFile::ReadData( double &dNumber )
{
	CHAR *pszData = ::strtok( (m_dwColumn++) ? NULL : m_szLine, m_szDelimiter );

	if( !pszData || !(*pszData) )
		return FALSE;
	dNumber = atof( pszData );
	return TRUE;
}

BOOL CDelimitedFile::ReadData( float &fNumber )
{
	CHAR *pszData = ::strtok( (m_dwColumn++) ? NULL : m_szLine, m_szDelimiter );

	if( !pszData || !(*pszData) )
		return FALSE;
	fNumber = (float) atof( pszData );
	return TRUE;
}

BOOL CDelimitedFile::ReadData( long &lNumber )
{
	CHAR *pszData = ::strtok( (m_dwColumn++) ? NULL : m_szLine, m_szDelimiter );

	if( !pszData || !(*pszData) )
		return FALSE;
	lNumber = (long) atol( pszData );
	return TRUE;
}

BOOL CDelimitedFile::ReadData( unsigned long &fNumber )
{
	CHAR *pszData = ::strtok( (m_dwColumn++) ? NULL : m_szLine, m_szDelimiter );

	if( !pszData || !(*pszData) )
		return FALSE;
	fNumber = (unsigned long) atol( pszData );
	return TRUE;
}

BOOL CDelimitedFile::ReadData( int &iNumber )
{
	CHAR *pszData = ::strtok( (m_dwColumn++) ? NULL : m_szLine, m_szDelimiter );

	if( !pszData || !(*pszData) )
		return FALSE;
	iNumber = atoi( pszData );
	return TRUE;
}

BOOL CDelimitedFile::ReadData( unsigned short &iNumber )
{
	CHAR *pszData = ::strtok( (m_dwColumn++) ? NULL : m_szLine, m_szDelimiter );

	if( !pszData || !(*pszData) )
		return FALSE;
	iNumber = (unsigned short)atoi( pszData );
	return TRUE;
}

BOOL CDelimitedFile::ReadData( short &iNumber )
{
	CHAR *pszData = ::strtok( (m_dwColumn++) ? NULL : m_szLine, m_szDelimiter );

	if( !pszData || !(*pszData) )
		return FALSE;
	iNumber = (short)atoi( pszData );
	return TRUE;
}

BOOL CDelimitedFile::ReadData( unsigned char &iNumber )
{
	CHAR *pszData = ::strtok( (m_dwColumn++) ? NULL : m_szLine, m_szDelimiter );

	if( !pszData || !(*pszData) )
		return FALSE;
	iNumber = (unsigned char)atoi( pszData );
	return TRUE;
}

BOOL CDelimitedFile::ReadData( char &iNumber )
{
	CHAR *pszData = ::strtok( (m_dwColumn++) ? NULL : m_szLine, m_szDelimiter );

	if( !pszData || !(*pszData) )
		return FALSE;
	iNumber = (char)atoi( pszData );
	return TRUE;
}

BOOL CDelimitedFile::ReadData( __int64 &i64Number )
{
	CHAR *pszData = ::strtok( (m_dwColumn++) ? NULL : m_szLine, m_szDelimiter );

	if( !pszData || !(*pszData) )
		return FALSE;
	i64Number = _atoi64( pszData );
	return TRUE;
}

BOOL CDelimitedFile::ReadData( DWORD64 &i64Number )
{
	CHAR *pszData = ::strtok( (m_dwColumn++) ? NULL : m_szLine, m_szDelimiter );

	if( !pszData || !(*pszData) )
		return FALSE;
	i64Number = (DWORD)_atoi64( pszData );
	return TRUE;
}

BOOL CDelimitedFile::ReadString( char *szString, DWORD dwSize )
{
	CHAR *pszData = ::strtok( (m_dwColumn++) ? NULL : m_szLine, m_szDelimiter );

	if( !pszData || !(*pszData) )
		return FALSE;
	strncpy(szString, pszData, dwSize);
	return TRUE;
}

BOOL CDelimitedFile::ReadData(const  char *szField, double &dNumber )
{
	int nSearch;

	if( ((nSearch = FindColumn( szField )) == -1) )
		return FALSE;
	GotoColumn( nSearch );
	return ReadData( dNumber );
}

BOOL CDelimitedFile::ReadData(const  char *szField, float &fNumber )
{
	int nSearch;

	if( ((nSearch = FindColumn( szField )) == -1) )
		return FALSE;
	GotoColumn( nSearch );
	return ReadData( fNumber );
}

BOOL CDelimitedFile::ReadData(const  char *szField, unsigned long &fNumber )
{
	int nSearch;

	if( ((nSearch = FindColumn( szField )) == -1) )
		return FALSE;
	GotoColumn( nSearch );
	return ReadData( fNumber );
}

BOOL CDelimitedFile::ReadData(const  char *szField, long &lNumber )
{
	int nSearch;

	if( ((nSearch = FindColumn( szField )) == -1) )
		return FALSE;
	GotoColumn( nSearch );
	return ReadData( lNumber );
}

BOOL CDelimitedFile::ReadData(const  char *szField, int &iNumber )
{
	int nSearch;

	if( ((nSearch = FindColumn( szField )) == -1) )
		return FALSE;
	GotoColumn( nSearch );
	return ReadData( iNumber );
}

BOOL CDelimitedFile::ReadData(const  char *szField, unsigned short &iNumber )
{
	int nSearch;

	if( ((nSearch = FindColumn( szField )) == -1) )
		return FALSE;
	GotoColumn( nSearch );
	return ReadData( iNumber );
}

BOOL CDelimitedFile::ReadData(const  char *szField, short &iNumber )
{
	int nSearch;

	if( ((nSearch = FindColumn( szField )) == -1) )
		return FALSE;
	GotoColumn( nSearch );
	return ReadData( iNumber );
}

BOOL CDelimitedFile::ReadData(const  char *szField, unsigned char &iNumber )
{
	int nSearch;

	if( ((nSearch = FindColumn( szField )) == -1) )
		return FALSE;
	GotoColumn( nSearch );
	return ReadData( iNumber );
}

BOOL CDelimitedFile::ReadData(const  char *szField, char &iNumber )
{
	int nSearch;

	if( ((nSearch = FindColumn( szField )) == -1) )
		return FALSE;
	GotoColumn( nSearch );
	return ReadData( iNumber );
}

BOOL CDelimitedFile::ReadData(const  char *szField, __int64 &i64Number )
{
	int nSearch;

	if( ((nSearch = FindColumn( szField )) == -1) )
		return FALSE;
	GotoColumn( nSearch );
	return ReadData( i64Number );
}

BOOL CDelimitedFile::ReadData(const  char *szField, DWORD64 &i64Number )
{
	int nSearch;

	if( ((nSearch = FindColumn( szField )) == -1) )
		return FALSE;
	GotoColumn( nSearch );
	return ReadData( i64Number );
}

BOOL CDelimitedFile::ReadString(const  char *szField, char *szString, DWORD dwSize )
{
	int nSearch;

	if( ((nSearch = FindColumn( szField )) == -1) )
		return FALSE;
	GotoColumn( nSearch );
	return ReadString( szString, dwSize );
}

BOOL CDelimitedFile::GotoColumn( int nColumn )
{
	CHAR* pszPos = 0;

	if(nColumn < 0 || (DWORD)nColumn > m_dwColumnCount) {
		return FALSE;
	}

	if((m_dwColumn == (DWORD) nColumn)) {
		return TRUE;
	}

	if( nColumn == 0 ) {
		m_dwColumn = 0;
		strtok(m_szLine, m_szDelimiter);
	} 
	else {
		for(;m_dwColumn < m_dwColumnCount && nColumn != m_dwColumn; ++m_dwColumn )
			pszPos = strtok(0, m_szDelimiter);

		if(m_dwColumn == m_dwColumnCount) {
			memcpy(m_szLine, m_szBackupLine, MAX_LINE_LENGTH);
			pszPos = strtok(m_szLine, m_szDelimiter);
			for(m_dwColumn = 1; m_dwColumn < (DWORD)nColumn && nColumn != m_dwColumn; ++m_dwColumn ) {
				pszPos = strtok(NULL, m_szDelimiter );
			}
		}
	}

	return TRUE;
}

int CDelimitedFile::FindColumn(const char *szField)
{	
	for(ColumnArray::size_type nCount = 0; nCount < m_ColumnNames.size(); ++nCount)
	{
		const char* szColumnName = m_ColumnNames[nCount];
		if(0 == strcmp(szField, szColumnName))
		{
            return static_cast<int>(nCount);
		}
	}

	return -1;
}

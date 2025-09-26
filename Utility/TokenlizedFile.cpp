#include "stdafx.h"
#include "TokenlizedFile.h"

#include <algorithm>

#include <Utility/Math/Math.h>
#include "GMMemory.h"

using namespace std;


char* trim_string(char* szString)
{
	while(*szString == '\r'|| *szString == '\n' || *szString == ' ')
	{
		++szString;
	}

	char* szTail = szString + strlen(szString);

	while(szString != szTail
		&& (*szTail == '\r'|| *szTail == '\n' || *szTail == ' ' || *szTail == '\0'))
	{
		*szTail = 0;
		--szTail;
	}

	return szString;
}


CTokenlizedFile::ColumnInfo::ColumnInfo(const char* szColumnName, unsigned int dwIndex)
:   m_dwHashKey(Math::HashFunc::sdbmHash(reinterpret_cast<const unsigned char*>(szColumnName))),
    m_szColumnName(szColumnName), m_dwIndex(dwIndex)
{

}

CTokenlizedFile::CTokenlizedFile(const char* lpszDelimiter)
:   m_lpFile(0), m_nLine(0)
{
    strncpy(m_szDelimiter, lpszDelimiter, MAX_DELIMITER_NUM - 1);
    m_szDelimiter[MAX_DELIMITER_NUM - 1] = '\0';

    m_ColumnInfo.reserve(DEFAULT_COLUMN_NUM);
    m_ColumnValues.reserve(DEFAULT_COLUMN_NUM);
}

CTokenlizedFile::~CTokenlizedFile()
{

}

void CTokenlizedFile::Close(void)
{
    if (0 != m_lpFile)
    {
        fclose(m_lpFile);
    }

    m_ColumnInfo.clear();
    m_ColumnValues.clear();
}


bool CTokenlizedFile::Open(const char* szFilename, const char* szOpenMode)
{
    Close();

    m_nLine = 0;
    m_lpFile = fopen(szFilename, szOpenMode);
    return (0 != m_lpFile) ? true : false;
}


bool CTokenlizedFile::ReadColumn(void)
{
    m_ColumnInfo.clear();

    if (0 != fgets(m_szColumn, MAX_LINE_BUFFER, m_lpFile))
    {
        char*           szToken = strtok(m_szColumn, m_szDelimiter);
        unsigned int    dwIndex = 0;

        do                
        {
            m_ColumnInfo.push_back(ColumnInfo(trim_string(szToken), dwIndex));
            ++dwIndex;

        } while (0 != (szToken = strtok(0, m_szDelimiter)));

        ++m_nLine;
		std::sort(m_ColumnInfo.begin(), m_ColumnInfo.end());
        return true;
    }

    return false;   
}


bool CTokenlizedFile::ReadLine(void)
{    
    m_ColumnValues.clear();

    if (0 != fgets(m_szLine, MAX_LINE_BUFFER, m_lpFile))
    {   
		if (0 == strcmp("\n", m_szLine))
		{
			return false;
		}

        char* szToken = strtok(m_szLine, m_szDelimiter);

        do
        {   
            m_ColumnValues.push_back(trim_string(szToken));

        } while (0 != (szToken = strtok(0, m_szDelimiter)));

        ++m_nLine;
        return true;
    } 

    return false;   
}


const char* CTokenlizedFile::GetStringValue(const char* szColumnName)
{    
    ColumnInfo Info(szColumnName, 0);

    pair<ColumnArray::iterator, ColumnArray::iterator> pair = 
		std::equal_range(m_ColumnInfo.begin(), m_ColumnInfo.end(), Info);

    for(;pair.first != pair.second; ++pair.first)
    {
        ColumnInfo& Index = *pair.first;

        if (0 == strcmp(Info.m_szColumnName, Index.m_szColumnName))
        {
            return GetStringValue(Index.m_dwIndex);
        }
	}

	return 0;
}


const char* CTokenlizedFile::GetColumnName(size_t nIndex)
{
	return (nIndex < m_ColumnInfo.size()) ? 
		m_ColumnInfo[nIndex].m_szColumnName : 0;
}

const char* CTokenlizedFile::GetStringValue(size_t nIndex)
{
	return (nIndex < m_ColumnValues.size()) ? 
		m_ColumnValues[nIndex] : 0;
}

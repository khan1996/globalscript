#ifndef _CUSTOM_TOKENLIZED_FILE_H_
#define _CUSTOM_TOKENLIZED_FILE_H_

#include <cstdio>
#include <vector>

class CTokenlizedFile
{
public:

    CTokenlizedFile(const char* lpszDelimiter = "\t");
    ~CTokenlizedFile();

    enum 
    {
		MAX_LINE_BUFFER     = 16384,
        MAX_DELIMITER_NUM   = 16,        
        DEFAULT_COLUMN_NUM  = 64
    };

    bool Open(const char* szFilename, const char* szOpenMode = "rt");
    void Close(void);
    
    bool ReadColumn(void);      // 컬럼 이름을 읽는다.
    bool ReadLine(void);        // 한 라인을 읽는다.

    bool IsEOF(void) { return (NULL != m_lpFile) ? (0 != feof(m_lpFile)) : false; }

    size_t GetColumnNum(void)   { return m_ColumnInfo.size();   }
    size_t GetCurrentLine(void) { return m_nLine;               }
    size_t GetValueNum(void)    { return m_ColumnValues.size(); }

    const char* GetColumnName(size_t nIndex);
    const char* GetStringValue(size_t nIndex);
    const char* GetStringValue(const char* szColumnName);

private:

    struct ColumnInfo
    {
        unsigned long   m_dwHashKey;
        unsigned int    m_dwIndex;
        const char*		m_szColumnName;

        ColumnInfo() : m_dwHashKey(0), m_dwIndex(0), m_szColumnName(0) { }
        ColumnInfo(const char* szColumnName, unsigned int dwIndex);

        bool operator < (const ColumnInfo& rhsInfo) const { return m_dwHashKey < rhsInfo.m_dwHashKey; }
    };

    FILE*   m_lpFile;
    size_t  m_nLine;

	char    m_szDelimiter[MAX_DELIMITER_NUM];
	char	m_szColumn[MAX_LINE_BUFFER];	
	char	m_szLine[MAX_LINE_BUFFER];    

    typedef std::vector<ColumnInfo>		ColumnArray;
    typedef std::vector<char*>			ValueArray;

    ColumnArray     m_ColumnInfo;
    ValueArray      m_ColumnValues;
};

#endif
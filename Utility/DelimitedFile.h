#ifndef _DELIMITED_FILE_H_
#define _DELIMITED_FILE_H_

#pragma once

#include <winsock2.h>
#include <windows.h>
#include <vector>

class CDelimitedFile
{

public:
	
	// MAX Delimiter num is 32;
	CDelimitedFile(const char* pszDelimiter = "\t");
	~CDelimitedFile();
	
	enum { 
		MAX_LINE_LENGTH	= 8192,
		MAX_DELIMITER_NUM = 32,
		MAX_COLUMN_LENGHT = 64,
		DEFAULT_COLUMN_NUM = 32
	};

	enum OpenFlags {
		modeRead =    (int) 0x00000,
		modeCreate =  (int) 0x01000
	};

private:
	
	FILE *m_fpFile;
	DWORD m_dwColumn;
	DWORD m_dwColumnCount;
	CHAR m_szLine[MAX_LINE_LENGTH];
	CHAR m_szBackupLine[MAX_LINE_LENGTH];

	char m_szDelimiter[MAX_DELIMITER_NUM];

	typedef std::vector<char*>	ColumnArray;
    ColumnArray	m_ColumnNames;

private:

	int  FindColumn(const char *szField );
	BOOL GotoColumn( int nColumn );

public:
	BOOL Open(LPCSTR szFilename, int nHeadLine = -1, UINT nOpenFlags = modeRead );
	BOOL ReadLine();
	bool ReadLine2nd(void);	// 빈줄 및 C++ 스타일 주석 지원

	BOOL ReadData(bool &bNumber);

	BOOL ReadData(double &dNumber);
	BOOL ReadData(float &fNumber);

	BOOL ReadData(long &lNumber);
	BOOL ReadData(unsigned long &fNumber);

	BOOL ReadData(int &iNumber);	
	BOOL ReadData(unsigned short &iNumber);
	BOOL ReadData(short &iNumber);
	BOOL ReadData(unsigned char &iNumber);
	BOOL ReadData(char &iNumber);
	
	BOOL ReadData(__int64 &i64Number);
	BOOL ReadData(DWORD64 &i64Number);

	BOOL ReadString(char *szString, DWORD dwSize);


	BOOL ReadData(const char *szField, double &dNumber);
	BOOL ReadData(const char *szField, float &fNumber);

	BOOL ReadData(const char *szField, long &lNumber );
	BOOL ReadData(const char *szField, unsigned long &fNumber);

	BOOL ReadData(const char *szField, int &iNumber);	
	BOOL ReadData(const char *szField, unsigned short &iNumber);
	BOOL ReadData(const char *szField, short &iNumber);
	BOOL ReadData(const char *szField, unsigned char &iNumber);
	BOOL ReadData(const char *szField, char &iNumber);
	
	BOOL ReadData(const char *szField, __int64 &i64Number);
	BOOL ReadData(const char *szField, DWORD64 &i64Number);

	BOOL ReadString(const char *szField, char *szString, DWORD dwSize);

	void Close();
};


#endif
// XORCrypt.cpp: implementation of the CXORCrypt class.
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XORCrypt.h"

#include <ctime>
#include <cstdlib>
#include "GMMemory.h"

CXORCrypt CXORCrypt::ms_this;

CXORCrypt::CXORCrypt()
{
	InitCodePage();

	for(int iVer = 2; iVer < 4; ++iVer)
	{
		for(int iPage = 0; iPage < CODE_PAGE; ++iPage)
		{
			for( int iBit = 0; iBit < BIT_COUNT; ++iBit)
			{
				BitFields[iVer][iPage][iBit] = BitFields[iVer][iPage][iBit]^BitFields[0][iPage][iBit];
			}
		}
	}

	return;
}


CXORCrypt::~CXORCrypt()
{

}


//Interface////////////////////////////////////////////////////////////////////////////////////
// XORF [ public ] - XOR 연산. # 순방향
// Parameter : 1st : 시작 포인터, 2st : 길이, 3st : 코드 페이지
// Return:
void CXORCrypt::XORF(char *Start_In, int Length_In, unsigned short PageVer, 
                     unsigned char PageNum1, unsigned char PageNum2)
{
	for (int Counter = 0; Counter < Length_In; ++Counter) 
    {
		*Start_In = *Start_In ^ BitFields[PageVer][PageNum1][Counter % BIT_COUNT];
		*Start_In = *Start_In ^ BitFields[PageVer][PageNum2][Counter % BIT_COUNT];
		++Start_In;
	}
}

//Interface////////////////////////////////////////////////////////////////////////////////////
// XORB [ public ] - XOR 연산. # 역방향
// Parameter : 1st : 시작 포인터, 2st : 길이, 3st : 코드 페이지
// Return:
void CXORCrypt::XORB(char *Start_In, int Length_In, unsigned short PageVer, 
                     unsigned char PageNum1, unsigned char PageNum2)
{
	for (int Counter = 0; Counter < Length_In; ++Counter) 
    {
		*Start_In = *Start_In ^ BitFields[PageVer][PageNum2][Counter % BIT_COUNT];
		*Start_In = *Start_In ^ BitFields[PageVer][PageNum1][Counter % BIT_COUNT];
		++Start_In;
	}
}

//Interface////////////////////////////////////////////////////////////////////////////////////
// InitCODE_PAGE [ public ] - 암호 페이지를 초기화 한다
// Parameter :
// Return:
void CXORCrypt::InitCodePage(void)
{
	int CODE_PAGENums = CODE_PAGE * 10;	

	srand((unsigned)time(NULL));
	for (int Counter = 0; Counter < CODE_PAGENums; ++Counter) 
    {
		m_CodePage[Counter] = rand() % CODE_PAGE;

		if(Counter > 1)
		{
			if (m_CodePage[Counter] == m_CodePage[Counter - 1])
            {
				--Counter;
            }
		}
	}

	m_CodePageCount = rand() % (CODE_PAGE * 10);
}

//Interface////////////////////////////////////////////////////////////////////////////////////
// GetCODE_PAGE [ public ] - 암호 페이지를 얻는다
// Parameter :
// Return:
unsigned long CXORCrypt::GetCodePage(void)
{
	if(0xffffff00 < m_CodePageCount)
    {
		m_CodePageCount = 0;
    }

	m_CodePageCount = (m_CodePageCount + 2) % (CODE_PAGE * 10);

	return (((unsigned long)PAGE_VERSION << 16) & 0xffff0000) | 
		(((unsigned long)m_CodePage[m_CodePageCount] << 8) & 0x0000ff00) | 
		((unsigned long)m_CodePage[m_CodePageCount - 1] & 0x000000ff);
}

unsigned long CXORCrypt::GetCodePage(unsigned short PageVer_In)
{
	if(0xffffff00 < m_CodePageCount)
    {
		m_CodePageCount = 0;
    }

	m_CodePageCount = (m_CodePageCount + 2) % (CODE_PAGE * 10);

	return (((unsigned long)PageVer_In << 16) & 0xffff0000) |
		(((unsigned long)m_CodePage[m_CodePageCount] << 8) & 0x0000ff00) |
		((unsigned long)m_CodePage[m_CodePageCount - 1] & 0x000000ff);
}


//Interface////////////////////////////////////////////////////////////////////////////////////
// InnerEncodeS [ public ] - 데이터 암호화 : 데이터를 XOR 연산 한다. # 클라이언트 용
// Parameter : 1st : 시작 포인터, 2st : 길이, 3st : 코드 페이지
// Return:
bool CXORCrypt::EncodePacket(char *Start_In, int Length_In, unsigned long CodePage_In)
{
	unsigned short  PageVer = 0;
	unsigned char   PageNum1 = 0, PageNum2 = 0;

	PageVer = (unsigned short)(((CodePage_In & 0xffff0000) >> 16) & 0x0000ffff);
	PageNum1 = (unsigned char)(((CodePage_In & 0xff00) >> 8) & 0x00ff);
	PageNum2 = (unsigned char)(CodePage_In & 0x00ff);

	if(PageVer < 0 || PageVer > 4)
		return false;

	if(PageNum1 < 0 || PageNum1 >= CODE_PAGE || PageNum2 < 0 || PageNum2 >= CODE_PAGE)
		return false;

	XORF(Start_In, Length_In, PageVer, PageNum1, PageNum2);

	return true;
}


//Interface////////////////////////////////////////////////////////////////////////////////////
// InnerEncodeS [ public ] - 데이터 암호화 : 데이터를 XOR 연산 한다. # 서버용
// Parameter : 1st : 시작 포인터, 2st : 길이, 3st : 코드 페이지
// Return:
bool CXORCrypt::DecodePacket(char *Start_In, int Length_In, unsigned long CodePage_In)
{
	unsigned short PageVer = 0;
	unsigned char PageNum1 = 0, PageNum2 = 0;

	PageVer = (unsigned short)(((CodePage_In & 0xffff0000) >> 16) & 0x0000ffff);
	PageNum1 = (unsigned char)(((CodePage_In & 0xff00) >> 8) & 0x00ff);
	PageNum2 = (unsigned char)(CodePage_In & 0x00ff);

	if(PageVer < 0 || PageVer > 4)
		return false;

	if(PageNum1 < 0 || PageNum1 >= CODE_PAGE || PageNum2 < 0 || PageNum2 >= CODE_PAGE)
		return false;

	XORB(Start_In, Length_In, PageVer, PageNum1, PageNum2);

	return true;
}


//Interface////////////////////////////////////////////////////////////////////////////////////
// EncodeHeader [ public ] - 데이터 암호화 : 데이터를 XOR 연산 한다.
// Parameter : 1st : 시작 포인터, 2st : 길이
// Return:
void CXORCrypt::EncodeHeader(char *Start_In, int Length_In, unsigned short PageVer_In, unsigned char PageNum_In)
{
	for(int Counter = 0; Counter < Length_In; ++Counter)
	{
		*Start_In = *Start_In ^ BitFields[PageVer_In][PageNum_In][Counter % BIT_COUNT];
		*Start_In = (((*Start_In & 0x80) >> 7) & 0x01) | ((*Start_In << 1) & 0xFE);	
		++Start_In;
	}
}


//Interface////////////////////////////////////////////////////////////////////////////////////
// DecodeHeader [ public ] - 데이터 복호화 : 데이터를 XOR 연산 한다. # 서버용
// Parameter : 1st : 시작 포인터, 2st : 길이
// Return:
void CXORCrypt::DecodeHeader(char *Start_In, int Length_In, unsigned short PageVer_In, unsigned char PageNum_In)
{
	for(int Counter = 0; Counter < Length_In; ++Counter)
	{
		*Start_In = (((*Start_In & 0x01) << 7) & 0x80) | ((*Start_In >> 1) & 0x7F);
		*Start_In = *Start_In ^ BitFields[PageVer_In][PageNum_In][Counter % BIT_COUNT];		
		++Start_In;
	}
}


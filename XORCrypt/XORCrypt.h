#ifndef _CXOR_CRYPT_H_
#define _CXOR_CRYPT_H_

#pragma once

#include <Pattern/Singleton.h>

class CXORCrypt : public CSingleton<CXORCrypt>
{
public:

    enum
    {
		// edith 2009.03.14 매번 바뀌는 패킷암호화 키
        PAGE_VERSION	= 2,
        BIT_COUNT		= 40,
	    CODE_PAGE		= 10,
    };

private:

	static unsigned char BitFields[][CODE_PAGE][BIT_COUNT];

    unsigned char	m_CodePage[CODE_PAGE * 10];	
    unsigned long	m_CodePageCount;
	
	void	InitCodePage(void);

	void	XORF(char *Start_In, int Length_In, unsigned short PageVer, unsigned char PageNum1, unsigned char PageNum2);
	void	XORB(char *Start_In, int Length_In, unsigned short PageVer, unsigned char PageNum1, unsigned char PageNum2);

    // To make Singleton Class.
	CXORCrypt();
	
    static CXORCrypt ms_this;

public:

    ~CXORCrypt();

	// interface of encoder, decoder
	unsigned long	GetCodePage(void);
	unsigned long	GetCodePage(unsigned short PageVer_In);
	inline unsigned short	GetPageVer(unsigned long CodePage_In);

	bool	EncodePacket(char *Start_In, int Length_In, unsigned long CodePage_In);
	bool	DecodePacket(char *Start_In, int Length_In, unsigned long CodePage_In);

	void	EncodeHeader(char *Start_In, int HeaderLen_In, unsigned short PageVer_In, unsigned char PageNum_In);
	void	DecodeHeader(char *Start_In, int HeaderLen_In, unsigned short PageVer_In, unsigned char PageNum_In);
};

inline unsigned short CXORCrypt::GetPageVer(unsigned long CodePage_In)
{
	return (unsigned short)(((CodePage_In & 0xffff0000) >> 16) & 0x0000ffff);
}


#endif


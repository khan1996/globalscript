// FastMath.h: interface for the CFastMath class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _CUSTOM_MATH_FUNCTIONS_H_
#define _CUSTOM_MATH_FUNCTIONS_H_

#include <winsock2.h>
#include <windows.h>
#include <cmath>


namespace Math
{
    namespace Convert
    {
        extern const unsigned short m_FastHeToBi[0x100];

	    // SPX, IPX 주소 계산을 위해 사용하는 함수
	    inline unsigned char	BiToHe(char cBin);	
	    inline void			    AcToHe(char *szDst, char *szSrc, int iCount);

	    // String을 값으로 바꿈(String은 0x가 붙은 Hex string이다.)
	    inline BYTE	    StrToHex08(const char *szSrc);
	    inline WORD     StrToHex16(const char *szSrc);
	    inline DWORD    StrToHex32(const char *szSrc);
	    inline DWORD64  StrToHex64(const char *szSrc);

	    // String을 값으로 바꿈(0x가 붙던지, 붙지 않던지간에 전부 바꿀 수 있다)
	    inline BYTE 	Atoc(const char *szSrc);
	    inline WORD     Atos(const char *szSrc);
	    inline DWORD    Atoi(const char *szSrc);
	    inline DWORD64  Atol64(const char *szSrc);

	    // 값을 Hex String으로 바꿈
	    inline void     Hex08ToStr(char *szDest, BYTE hex);
	    inline void     Hex16ToStr(char *szDest, WORD hex);
	    inline void     Hex32ToStr(char *szDest, DWORD hex);
	    inline void     Hex64ToStr(char *szDest, DWORD64 hex);
    };

    namespace Random
    {
        // 리턴값은 nBeginExtent ~ nEndExtent - 1의 범위를 갖는다.
	    unsigned long ComplexRandom(int nEndExtent, int nBeginExtent = 0);	// 비교적 분포가 좋다.
		int SimpleRandom(unsigned long dwSeed, int nEndExtent, int nBeginExtent = 0);
    };

    namespace HashFunc
    {
	    // -------------------------------------------------------------------------------------------------
	    // String-to-Hash 함수들 : http://www.cs.yorku.ca/~oz/hash.html 에서 참고

	    inline unsigned long djb2Hash(const unsigned char *str);	// first reported by dan bernstein 
	    inline unsigned long sdbmHash(const unsigned char *str);	// this is one of the algorithms used in berkeley db        
	    inline unsigned long looseHash(const unsigned char *str);	// 가장 간단한 해쉬. 그냥 모두 더함.
    };

	namespace Const
	{
		static float PI = 3.14159f;
	}
};

#include "Convert.inl"



inline unsigned long Math::HashFunc::djb2Hash(const unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

	while (c = *str++) { hash = ((hash << 5) + hash) + c; /* hash * 33 + c */ }
    return hash;
}


inline unsigned long Math::HashFunc::sdbmHash(const unsigned char *str)
{
    unsigned long hash = 0;
    int c;

	while (c = *str++) { hash = c + (hash << 6) + (hash << 16) - hash; }
    return hash;
}


inline unsigned long Math::HashFunc::looseHash(const unsigned char *str)
{
	unsigned int hash = 0;
	int c;

	while (c = *str++) { hash += c; }
	return hash;
}


#endif // !defined(AFX_FASTMATH_H__ED69578B_18C1_42EA_9C5E_888DC38101C2__INCLUDED_)

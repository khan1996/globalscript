#ifndef _CHAR_CONFIG_PACKET_STRUCT_H_
#define _CHAR_CONFIG_PACKET_STRUCT_H_

#pragma pack(1)

#include <winsock2.h>
#include <windows.h>

// 전투/반전 모드 정보 (주의 : 구조체 크기가 변경되면 애로사항 꽃핌)
typedef struct PeaceModeInfo* LPPeaceModeInfo;
struct PeaceModeInfo
{
	bool		m_bPeace;
	FILETIME	m_FileTime;

	PeaceModeInfo() : m_bPeace(false)
	{
		m_FileTime.dwHighDateTime = 0;
		m_FileTime.dwLowDateTime = 0;
	}
};

// 거부 옵션 (주의 : 구조체 크기가 변경되면 애로사항 꽃핌)
typedef struct RejectOption* LPRejectOption;
struct RejectOption
{
	union 
	{
		struct RejectStruct
		{
			unsigned short	m_Party		: 1;    // 파티
			unsigned short	m_Exchange	: 1;    // 교환 거부
			unsigned short	m_Duel		: 1;    // 일대일 거부
			unsigned short	m_Whisper	: 1;    // 귓속말 거부
			unsigned short  m_Friend    : 1;    // 친구 거부
			unsigned short	m_Shout		: 1;    // 외치기 거부            
			unsigned short	m_Recall	: 1;    // 리콜 거부            
			unsigned short	m_Remainder	: 9;
		};

		RejectStruct    Reject;
		unsigned short	m_wReject;
	};

	RejectOption() : m_wReject(0) { }
	RejectOption(unsigned short wReject) : m_wReject(wReject) { }
};


// 언어필터 옵션
typedef struct LanguageOption* LPLanguageOption;
struct LanguageOption
{
	union 
	{
		// 필터링할 언어의 설정
		struct LangStruct
		{
			unsigned short	m_ENG		: 1;
			unsigned short	m_CHS		: 1;
			unsigned short	m_JPN		: 1;
			unsigned short	m_KOR		: 1;
			unsigned short	m_PRT		: 1;
			unsigned short	m_SPN		: 1;
			unsigned short	m_POL		: 1;
			unsigned short	m_Remainder	: 9;
		};

		LangStruct	    Lang;
		unsigned short	m_usLang;
	};

	LanguageOption() : m_usLang(0) { }
	LanguageOption(unsigned short usLang) : m_usLang(usLang) { }
};

#pragma pack()

#endif
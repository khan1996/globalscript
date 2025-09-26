#ifndef _CHAR_COMMUNITY_PACKET_STRUCT_H_
#define _CHAR_COMMUNITY_PACKET_STRUCT_H_

#pragma pack(1)

#include <DB/DBDefine.h>

// 파티 찾기 구조체
typedef struct MemberFindParty* LPMemberFindParty;
struct MemberFindParty 
{
	char m_strName[CHAR_INFOST::MAX_NAME_LEN];       // 이름
	unsigned char m_cLevel;         // 레벨    
	unsigned char m_cClass;         // 클래스
};

// 파티원 찾기 구조체
typedef struct PartyFindMember* LPPartyFindMember;
struct PartyFindMember 
{
	char m_strLeaderName[CHAR_INFOST::MAX_NAME_LEN];     // 리더 이름

	unsigned char m_cAverageLevel;      // 평균 레벨
	unsigned char m_cMemberNum;         // 멤버 수
};

#pragma pack()

#endif
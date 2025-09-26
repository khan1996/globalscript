#ifndef _PARTY_INFO_PACKET_H_
#define _PARTY_INFO_PACKET_H_

#include "DataPacket.h"
#include <DB/DBdefine.h>

#pragma pack(1)

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 파티 정보
//
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct PktPID* LPPktPID;
struct PktPID : public PktDD 
{
	unsigned long		m_dwCharID;	
	unsigned long		m_dwPartyID;

	unsigned long		m_dwServerID;
	unsigned long		m_dwGID;
	unsigned short		m_wClass;
	char				m_cLevel;
};

typedef struct PktPIDAck* LPPktPIDAck;
struct PktPIDAck : public PktDD 
{
	PARTY				m_Party;

	unsigned long		m_dwCharID;

	unsigned long		m_dwServerID;
	unsigned long		m_dwGID;
	unsigned short		m_wClass;
	char				m_cLevel;
};


////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 파티 생성
//
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct PktCPD* LPPktCPD;
struct PktCPD : public PktDD 
{
	unsigned long				m_dwLeaderID;
	unsigned long				m_dwMemberID;

	unsigned long				m_dwLeaderGID;
	unsigned short				m_wLeaderClass;
	char						m_cLeaderLevel;

	unsigned long				m_dwMemberGID;
	unsigned short				m_wMemberClass;
	char						m_cMemberLevel;	
};

typedef struct PktCPDAck* LPPktCPDAck;
struct PktCPDAck : public PktDD
{
	PARTY						m_Party;

	unsigned long				m_dwLeaderID;
	unsigned long				m_dwMemberID;

	unsigned long				m_dwLeaderGID;
	unsigned short				m_wLeaderClass;
	char						m_cLeaderLevel;

	unsigned long				m_dwMemberGID;
	unsigned short				m_wMemberClass;
	char						m_cMemberLevel;

	unsigned char				m_cZone;
};


////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 파티 삭제
//
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct PktDPD* LPPktDPD;
struct PktDPD : public PktDD 
{
	unsigned long		m_dwPartyID;	
};


////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 파티 멤버 정보
//
//							
//	멤버 로그인		:		CID/CID					0/SID
//	멤버 로그아웃	:		CID/CID					0/리더 아이디(0 일경우 리더 양도 없음)
//	멤버 가입		:		CID/CID					CID/CID
//	멤버 삭제		:		CID/CID					CID/리더 아이디(0 일 경우 리더 양도 없음)
//
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct PktPMD* LPPktPMD;
struct PktPMD : public PktDD 
{
	enum PktPMDErr
	{
		NO_SERVER_ERR		= 0,	
		SERVER_ERROR		= 1,
		FAIL_WRONG_PARTY	= 2
	};

	unsigned long		m_dwPartyID;	
	unsigned long		m_dwSenderID;
	
	unsigned long		m_dwGID;				// 길드 아이디(이전 Reference 도 사용).
	unsigned long		m_dwServerID;			// 서버 아이디.
	unsigned short		m_wClass;				// 클래스.
	char				m_cLevel;				// 레벨.

	char	            m_strSenderName[CHAR_INFOST::MAX_NAME_LEN];	
};


struct PartyMemberData
{
	// 인챈트 정보의 경우 CreatureStructure.h의 EnchantInfo에 변화가 생기면 수정이 필요합니다.

    enum ChangedData
    {
        CHANGED_XPOS        	= ( 1 << 0 ),
        CHANGED_YPOS        	= ( 1 << 1 ),
        CHANGED_ZPOS        	= ( 1 << 2 ),
        CHANGED_MAX_HP      	= ( 1 << 3 ),
        CHANGED_MAX_MP      	= ( 1 << 4 ),
        CHANGED_CUR_HP      	= ( 1 << 5 ),
        CHANGED_CUR_MP      	= ( 1 << 6 ),
        CHANGED_LEVEL       	= ( 1 << 7 ),
        CHANGED_CLASS       	= ( 1 << 8 ),

		CHANGED_ENCHANT_INFO_0	= ( 1 << 9 ),
		CHANGED_ENCHANT_INFO_1	= ( 1 << 10 ),
		CHANGED_ENCHANT_INFO_2	= ( 1 << 11 ),
		CHANGED_ENCHANT_INFO_3	= ( 1 << 12 )
    };

	enum Const
	{
		MAX_ENCHANT_INFO_NUM	= 4
	};

    unsigned long   m_dwCID;            // 캐릭터 CID

    float           m_fXPos;            // X위치
    float           m_fYPos;            // Y위치
    float           m_fZPos;            // Z위치

    unsigned short  m_usMaxHP;          // 최대 HP
    unsigned short  m_usMaxMP;          // 최대 MP
    unsigned short  m_usCurHP;          // 현재 HP
    unsigned short  m_usCurMP;          // 현재 MP

    unsigned char   m_cLevel;           // 캐릭터 레벨
    unsigned char   m_cClass;           // 캐릭터 클래스

	unsigned long	m_dwEnchantInfo[MAX_ENCHANT_INFO_NUM];	// 인챈트 정보

    unsigned short  m_usDataChanged;    // 데이터 변경 여부
};

#pragma pack()


#endif
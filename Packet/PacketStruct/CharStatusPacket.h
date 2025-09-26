#ifndef _CHAR_STATUS_PACKET_H_
#define _CHAR_STATUS_PACKET_H_

// CharStatusPacket.h
#include <Network/Packet/PacketBase.h>
#include <DB/DBdefine.h>
#include <Item/ItemStructure.h>

#include "CharStatusPacketStruct.h"

#pragma pack(1)

// 캐릭터 자살
typedef struct PktSC* LPPktSC;
struct PktSC : public PktBase
{
	unsigned long	m_dwCharID;	        // 캐릭터 아이디
};

// 캐릭터 자살 Ack
typedef struct PktSCAck PktSCAck, *LPPktSCAck;
struct PktSCAck : public PktBase 
{
    POS				m_Pos;
	unsigned long	m_dwCharID;		    // 캐릭터 아이디
};

// 바인드 포지션
typedef struct PktBP* LPPktBP;
struct PktBP : public PktBase 
{
	enum PktBPCmd
	{
		BP_SAVE_POSITION	= 0,
		BP_WARP				= 1,
		BP_RECALL			= 2
	};

  	POS				m_Pos;              // 바인드할 위치
	char			m_cZone;			// 바인드할 존

	unsigned long	m_dwNPCID;          // 바인드할 NPC ID
	unsigned char	m_cCmd;             // 포지션 명령		(0 = 세이브 위치, 1 = 위치 이동)
};

// 레벨업
typedef struct PktLU* LPPktLU;
struct PktLU : public PktBase 
{
	ChState			m_State;
	unsigned long	m_dwCharID;         // 캐릭터 아이디
	unsigned char	m_cLevel;	        // 레벨
};

// 레벨업 정보
typedef struct PktLUInfo* LPPktLUInfo;
struct PktLUInfo : public PktBase
{
	unsigned long	m_dwCharID;         // 캐릭터 아이디
	unsigned char	m_cLevel;	        // 캐릭터 레벨
};

// 스탯 초기화
typedef struct PktSR* LPPktSR;
struct PktSR : public PktBase
{
	ChState			m_State;
};

// 스탯 초기화
typedef struct PktSRAck* LPPktSRAck;
struct PktSRAck : public PktBase
{
	enum PktSRAckErr
	{
		FAIL_NO_CHANCE	= 2
	};

	ChState			m_State;
	SKILL			m_Skill;
};

// 스테이터스 재분배
typedef struct PktSRT* LPPktSRT;
struct PktSRT : public PktBase
{
	ChState			m_State;
	Item::ItemPos   m_ItemPos;          // 망각의 돌 위치
};

// 스테이터스 재분배 Ack
typedef struct PktSRTAck* LPPktSRTAck;
struct PktSRTAck : public PktBase
{
	enum PktSRTAckErr
	{
		FAIL_INVALID_IP			= 2,	// 재분배 할 수 있는 포인트를 초과하여 재분배하려 한 경우
		FAIL_NO_ITEM			= 3,	// 요청한 위치에 아이템(망각의 돌)이 없음 (혹은 갯수가 이상하거나 서버에서 아이템 제거에 실패한 경우)
		FAIL_INVALID_ITEM		= 4,	// 요청한 위치에 있는 아이템이 올바른 아이템(망각의 돌)이 아닌 경우
		FAIL_NOT_ENOUGH_GOLD	= 5,	// 금액 부족
		FAIL_INVALID_STATUS		= 6		// 재분배된 스탯에 문제가 있는 경우 (총 IP가 맞지 않거나 클래스 최소 스탯에 못 미치는 경우)
	};

	enum Const
	{
		MAX_STATUS_RETRAIN	= 800
	};

	ChState			m_State;
	SKILL			m_Skill;
	Item::ItemPos   m_ItemPos;          // 망각의 돌 위치
	unsigned long	m_dwGold;           // 소비 금액
};

// 능력치 증가
typedef struct PktIP* LPPktIP;	
struct PktIP : public PktBase 
{
	unsigned long	m_dwCharID;         // 캐릭터 ID
	unsigned char	m_cStateType;       // 능력치 타입
};

// 능력치 증가 Ack
typedef struct PktIPAck* LPPktIPAck;	
struct PktIPAck : public PktBase 
{
    ChState			m_State;
	unsigned long	m_dwCharID;	        // 캐릭터 ID
};

// 클래스 업그레이드
typedef struct PktCU* LPPktCU;
struct PktCU : public PktBase
{
	unsigned long	m_dwCharID;         // 캐릭터 ID
	unsigned char	m_cClass;           // 클래스 번호
};

// 클래스 업그레이드 Ack
typedef struct PktCUAck* LPPktCUAck;
struct PktCUAck : public PktBase 
{
	ChState			m_State;            // 캐릭터 상태
	unsigned long	m_dwCharID;         // 캐릭터 ID
	unsigned long	m_dwFame;			// 명성
	unsigned char	m_cLevel;			// 레벨
	unsigned char	m_cClass;	        // 클래스 번호
};

// 액스트라 이벤트
typedef struct PktExtraEvent* LPPktExtraEvent;
struct PktExtraEvent : public PktBase 
{
	enum PktExtraEventErr
	{
		EVENT_NONE					= 2,	// 처리되지 않는 이벤트 
		EVENT_CLEAR					= 3,	// 클리어했다.
		FAIL_EVENT					= 4,	// 이벤트 실패
		FAIL_REWARD					= 5,	// 이벤트 보상 실패
	};

	unsigned long	m_dwUserID;         // 유저 ID
	unsigned long	m_dwCharID;	        // 캐릭터 ID
	unsigned long	m_dwEventID;		// 이벤트 ID
	unsigned long	m_dwValue1;			// 추가값
	unsigned long	m_dwValue2;			// 추가값
};

// 스킬 (생성/지우기/락/락해제)
typedef struct PktSk* LPPktSk;
struct PktSk : public PktBase 
{
	enum PktSkError
	{	
		// TODO : 겹치는 상수들은 클라이언트와 상의하여 유니크하게 수정합시다.

		// 스킬 관련 공통 에러
		FAIL_NOT_SKILL				= 2,									
		FAIL_ERASE_CLASS_SKILL		= 5,	// 클래스 스킬 삭제
		FAIL_NO_ITEM				= 6,	// 요청한 위치에 아이템(망각의 돌)이 없음 (혹은 갯수가 이상하거나 서버에서 아이템 제거에 실패한 경우)
		FAIL_INVALID_ITEM			= 7,	// 요청한 위치에 있는 아이템이 올바른 아이템(망각의 돌)이 아닌 경우

		// 생성 에러 
		FAIL_MAX_LEVEL				= 2,	// 최대 레벨임 (락해야함)
		FAIL_NOT_CURRENT_LOCK_COUNT	= 3,	// 익힐 단계가 아닌 스킬
		FAIL_NOT_ENOUGH_SLOT		= 4,	// 슬롯 부족
		FAIL_NOT_CURRENT_CLASS		= 5,	// 해당 클래스가 아님
		FAIL_NOT_ENOUGH_STATUS		= 6,	// 스탯이 모자람
		FAIL_NOT_ENOUGH_SKILL_POINT	= 7,	// 스킬 포인트 부족

		// 지우기 에러
		FAIL_NON_LEVEL				= 3,	// 레벨이 없음
		FAIL_ALREADY_LOCK			= 4,	// 이미 락한 스킬

		// 락 에러
		FAIL_FULL_LOCK				= 3,	// 락을 모두한 스킬
		FAIL_NOT_ENOUGH_LEVEL		= 4,	// 레벨이 다 차지않은 스킬

		// 락 해제 에러
		FAIL_NON_LOCK				= 3,	// 락하지 않은 스킬
		FAIL_FIFTHSKILL_UNLOCK		= 4		// 5단계 스킬은 지우기로 4단계로 내릴수 없다.
	};

	unsigned long	m_dwCharID;         // 캐릭터 ID
	unsigned short	m_wSkill;           // 스킬 번호
	unsigned char	m_cIndex;           // 스킬 위치
	unsigned char	m_cSkillLevel;		// 스킬 레벨

	Item::ItemPos   m_ItemPos;          // 망각의 돌 위치
};
/*
// 스킬 락 해제
typedef struct PktSkUL* LPPktSkUL;
struct PktSkUL : public PktSk 
{
    Item::ItemPos   m_itemPos;          // 망각의 돌 위치 PktSk에 정의
};
*/
// 스킬 락 해제 Ack
typedef struct PktSkULAck* LPPktSkULAck;
struct PktSkULAck : public PktSk 
{
    unsigned long	m_dwGold;           // 소비 금액
//    Item::ItemPos   m_itemPos;          // 망각의 돌 위치	PktSk에 정의
};

// HP 리젠 패킷	( Char HP Regen )
typedef struct PktHP* LPPktHP;
struct PktHP : public PktBase 
{
	unsigned long	m_dwCharID;         // 캐릭터 아이디	
	unsigned short	m_sHP;              // 현재 hp
	unsigned short	m_sMP;              // 현재 mp
};

// 정보 요청	( Require Info )
//	- P2P로 다른 클라이언트에게 직접 보낼 시에는 주소내용이 추가 되며,
//		해당 클라이언트 아이디는 보내는 이의 캐릭터 아이디가 되겠다.
typedef struct PktRI* LPPktRI;
struct PktRI : public PktBase
{
	SOCKADDR_IN		m_PublicAddress;
	SOCKADDR_IN		m_PrivateAddress;

	unsigned long	m_dwCharID;         // 해당 캐릭터 아이디
	unsigned char   m_cCmd;             // 커맨드 (0 : HP, MP 정보, 1 : UDP 주소, 2 : 모양)
};

// 캐릭터 겉모습 정보
typedef struct PktSI* LPPktSI;
struct PktSI : public PktBase
{
	enum { MAX_STALL_NAME_LEN = 32 };

	enum WEAPON_HAND
    { 
		WEAPON_HAND1 = 1,
        WEAPON_HAND2 = 2 
	};

	CHAR_VIEW		m_CharView;         // 캐릭터 겉모습
	unsigned long	m_dwStatusFlag;     // 캐릭터에 걸려 있는 chant, enchant Flag
	bool			m_bPeace;			// 반전 모드
	unsigned char	m_cSelectedHands;   // 현재 사용중인 손 (1 = 무기1 & 방패1, 2 = 무기2 & 방패2)

	char			m_StallName[MAX_STALL_NAME_LEN];	// 노점상 이름
};

// 엘리트 보너스
typedef struct PktEB* LPPktEB;
struct PktEB : public PktBase 
{
	char			m_cEliteBonus;		// 엘리트 보너스
};

#pragma pack()

#endif
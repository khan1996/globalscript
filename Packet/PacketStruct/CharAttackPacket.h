#ifndef _CHAR_ATTACK_PACKET_H_
#define _CHAR_ATTACK_PACKET_H_

// CharAttackPacket.h
#include <DB/DBDefine.h>
#include <Network/Packet/PacketBase.h>
#include <Network/Packet/PacketStruct/DataPacket.h>

#include "CharAttackPacketStruct.h"
#include <Community/Guild/GuildConstants.h>

#pragma pack(1)


// 캐릭터 공격 패킷
typedef struct PktAt* LPPktAt;
struct PktAt : public PktBase
{
	POS				m_Postion;      // 공격자 캐릭터 위치
	float			m_fDir;         // 공격자 방향

	unsigned long	m_dwCharID;	    // 공격자 캐릭터 아이디

	AtType			m_AtType;       // 공격 타입
	AtNode			m_AtNode;       // 방어자 정보
};

// 캐릭터 공격 패킷 Ack (가변 길이) + 방어자 노드
typedef struct PktAtAck* LPPktAtAck;
struct PktAtAck : public PktBase 
{
    enum PktAtAckErr
	{
		FAIL_ALREADY_DEAD		= 2,	// 이미 죽은 크리쳐에게 공격
		FAIL_TOO_FAR			= 3,	// 거리가 멀어져서 스킬 사용 불가
		FAIL_SUICIDE			= 4,	// 스킬을 사용하면 자살하게 된다.
		FAIL_PEACE_TO_WAR		= 5,	// 반전 모드 캐릭터가 전투 모드 캐릭터에게 긍정적인 인챈트 부여 시도
		FAIL_SPELL				= 6,	// 스펠 사용 실패
		FAIL_TO_SIEGE_OBJECT	= 7,	// 공성 오브젝트에게 할수 없는 공격 시도
		FAIL_TO_MONSTER			= 8,	// 몬스터에게 할수 없는 공격 시도
		FAIL_FRIENDLY_ATTACK	= 9,	// 아군에게 공격할수 없다
		FAIL_NOT_SIEGE_ATTACK	= 10,	// 공성병기의 공격이 잘못되었다.
		FAIL_NOT_STEALTH		= 11,	// 스텔스를 사용할 수 없다.
	};

	unsigned long	m_dwCharID;     // 공격자 캐릭터 아이디
	AtType			m_AtType;       // 공격 타입

	unsigned short	m_wHP;			// 공격자 HP
	unsigned short	m_wMP;			// 공격자 MP
	unsigned short	m_wMPHeal;		// 공격자 MP 회복량

	unsigned char	m_cJudge;       // 플래그
	unsigned char	m_cDefenserNum; // 방어자 수
};


// 캐릭터 공격 정보 (가변 길이) + 방어자 노드
typedef struct PktAtInfo* LPPktAtInfo;
struct PktAtInfo : public PktBase 
{
	unsigned long	m_dwCharID;     // 캐릭터 아이디
	AtType			m_AtType;       // 공격 타입
	unsigned char	m_cDefenserNum; // 방어자 수
};


// 캐릭터 공격 받음 패킷
typedef struct PktAted* LPPktAted;
struct PktAted : public PktBase
{
	POS				m_Postion;      // 공격자 위치
	float			m_fDir;         // 공격자 방향

	unsigned long	m_dwCharID;     // 공격자 아이디
	unsigned long	m_dwMyID;		// 방어자 아이디

	AtType			m_AtType;       // 공격 타입

	unsigned short	m_wMyHP;		// 방어자 HP
	unsigned short	m_wMyMP;		// 방어자 MP
	unsigned short	m_wMyMPHeal;	// 방어자 MP 회복량

   	unsigned char	m_cJudge;       // 판정 값
};


// 캐릭터 장비 내구도
typedef struct PktEquipDurability* LPPktEquipDurability;
struct PktEquipDurability : public PktBase
{
	unsigned long	m_dwCharID;		// 캐릭터 아이디

	unsigned char	m_cIndex;		// 장비 위치
	unsigned char	m_cValue;		// 남은 내구도
	unsigned char	m_cValueMax;	// 남은 내구도
};


// 듀얼 명령
//
//  명령 :
//	0 = 제안,	1 = 수락,	2 = 거절,	3 = 로그아웃,	4 = 사망
//
//  에러 :
//	0 = 성공,	1 = 서버 에러,	2 = 이미 듀얼중인 캐릭터에게 듀얼 신청,
//	3 = 거부 옵션이 켜진 상태
typedef struct PktDuC* LPPktDuC;
struct PktDuC : public PktBase 
{
	enum DuelCmd 
	{ 
		DUC_PROPOSE		= 0,	DUC_READY		= 1,			DUC_REFUSE			= 2, 
		DUC_LOGOUT		= 3,	DUC_DEAD		= 4,			DUC_PARTY_PROPOSE	= 5,
		DUC_PARTY_READY	= 6,	DUC_REQUEST_PARTY_INFO	= 7,	DUC_CANCEL			= 8
	};

	enum PktDuCError
	{
		NO_SERVER_ERR	= 0,	SERVER_ERROR	= 1,	FAIL_DUELING_CHAR	= 2,
		FAIL_REJECT		= 3,	FAIL_NOT_LEADER	= 4,	FAIL_NOT_PARTY		= 5
	};

	unsigned long	m_dwSenderID;	// 보낸이 아이디
	unsigned long	m_dwRecverID;	// 받는이 아이디
   	unsigned char   m_cCmd;         // 명령
};


// 팀배틀 정보
typedef struct PktTBInfo* LPPktTBInfo;
struct PktTBInfo : public PktBase
{
	enum DuelCmd 
	{ 
		DUC_PROPOSE		= 0,	DUC_READY		= 1,	DUC_REFUSE			= 2, 
		DUC_LOGOUT		= 3,	DUC_DEAD		= 4,	DUC_PARTY_PROPOSE	= 5,
		DUC_PARTY_READY	= 6,	DUC_REQUEST_PARTY_INFO	= 7
	};

	unsigned long m_dwCharID;
	char m_strChallengerName[CHAR_INFOST::MAX_NAME_LEN];
    unsigned char m_cCmd;

	unsigned char m_cMemberNum;
};


// 캐릭터 챈트&인챈트 정보
typedef struct PktSpInfo* LPPktSpInfo;
struct PktSpInfo : public PktBase 
{
	unsigned long	m_dwCharID;         // 캐릭터 아이디
	unsigned short	m_nEnchantLevel;    // 주문 레벨
	unsigned char	m_cSpellType;       // 주문 타입 ( namespace Skill::SpellType참조 )
	unsigned long	m_dwEnchantTime;    // 주문 유지시간
	bool			m_bOnOff;           // 켜짐/꺼짐	
};


// 캐릭터 리스폰
typedef struct PktRs* LPPktRs;
struct PktRs : public PktBase
{
	enum RespawnCmd
	{
		RS_NORMAL				= 0,		// 일반적인 방식 (기본 리스폰 장소에서 리스폰)
		RS_SELECT				= 1,		// 리스폰 장소 선택 확정
		RS_FORMER_PLACE			= 2,		// 제자리 리스폰 (스킬에 의해)

		RS_BG_HUMAN1			= 3,		// 휴먼 포인트1 (배틀그라운드)
		RS_BG_HUMAN2			= 4,		// 휴먼 포인트2 (배틀그라운드)
		RS_BG_HUMAN3			= 5,		// 휴먼 포인트3 (배틀그라운드)
		RS_BG_AKHAN1			= 6,		// 아칸 포인트1 (배틀그라운드)
		RS_BG_AKHAN2			= 7,		// 아칸 포인트2 (배틀그라운드)
		RS_BG_AKHAN3			= 8,		// 아칸 포인트3 (배틀그라운드)

		RS_BGSERVER_RANDOM		= 9,		// 배틀 그라운드 서버군
		
		RS_BG_EXIT				= 10		// 배틀그라운드 탈출
	};

	unsigned long	m_dwCharID;
	unsigned char	m_cCmd;
	unsigned long	m_dwTownID;
	POS				m_Position;
};

// 캐릭터 리스폰 Ack
typedef struct PktRsAck* LPPktRsAck;
struct PktRsAck : public PktBase 
{
	enum Error
	{
		ERR_NOT_EXIST_CAMP	= 2,	// 존재하지 않는 요새
	};

	POS				m_Position;     // 현재 위치
    __int64			m_dlExp;        // 현재 경험점
	unsigned long	m_dwGold;		// 현재 보유 금액

	unsigned long	m_dwCharID;     // 캐릭터 아이디
	unsigned short	m_wHP;          // HP
	unsigned short	m_wMP;          // MP
};

// 리스폰 Ack (배틀그라운드 대기중)
typedef struct PktBGRsAck* LPPktBGRsAck;
struct PktBGRsAck : public PktBase
{
	enum PktBGRsAckErr
	{
		FAIL_PERSONNEL_OVER	= 2
	};

	unsigned long	m_dwCharID;		// 캐릭터 아이디

	unsigned short	m_wTurn;		// 캐릭터 순서
	unsigned short	m_wWaitNum;		// 전체 대기인 수

	unsigned short	m_wLeftTime;	// 남은 시간

	unsigned short	m_wHumanNum;	// 전체 휴먼 수
	unsigned short	m_wAkhanNum;	// 전체 아칸 수
};

// 리스폰 큐 대기인 정보 (배틀그라운드용)
typedef struct PktRsWaitQueue* LPPktRsWaitQueue;
struct PktRsWaitQueue : public PktBase
{
	unsigned long	m_dwCharID;		// 캐릭터 아이디
};

// 리스폰 큐 대기인 정보 Ack (배틀그라운드용)
typedef struct PktRsWaitQueueAck* LPPktRsWaitQueueAck;
struct PktRsWaitQueueAck : public PktBase
{
	enum { MAX_POINT_NUM_PER_RACE = 3};

	unsigned long	m_dwCharID;								// 캐릭터 아이디
	unsigned short	m_wWaitNum[MAX_POINT_NUM_PER_RACE];		// 각 리스폰 위치별 대기인 수
};

// 캐릭터 리스폰 정보 (리스폰 위치들)
// 추가 정보가 뒤에 따라 붙어서 간다.
typedef struct PktRsInfo* LPPktRsInfo;
struct PktRsInfo : public PktBase
{
	unsigned long m_dwCharID;
};

// 캐릭터 리스폰 정보 Ack (리스폰 위치들)
// 추가 정보가 뒤에 따라 붙어서 간다.
typedef struct PktRsInfoAck* LPPktRsInfoAck;
struct PktRsInfoAck : public PktBase
{
	enum Const
	{
		MAX_RESPAWN_POINT	= 50
	};

	unsigned long	m_dwCharID;
	unsigned char	m_cRsAreaNum;
	unsigned short	m_wSize;
};

struct RespawnArea
{
	unsigned long	m_dwTownID;
	POS				m_RespawnPos;
};

// 캐릭터 리스폰 지역 세부 정보
typedef struct PktRsAreaInfo* LPPktRsAreaInfo;
struct PktRsAreaInfo : public PktBase
{
	unsigned long m_dwCharID;
	unsigned long m_dwTownID;	// 마을, 길드 요새, 성 ID
};

// 캐릭터 리스폰 지역 세부 정보 Ack
typedef struct PktRsAreaInfoAck* LPPktRsAreaInfoAck;
struct PktRsAreaInfoAck : public PktBase
{
	unsigned long	m_dwCharID;
	unsigned long	m_dwTownID;
	unsigned short	m_nWaitOrder;	// 대기순위
	unsigned char	m_cRemainHour;	// 남은 시간 (시)
	unsigned char	m_cRemainMin;	// 남은 시간 (분)
	unsigned char	m_cRemainSec;	// 남은 시간 (초)
	bool			m_bCount;		// 시간 카운팅 시작여부
};

// 어워드 처리
typedef struct PktAw* LPPktAw;
struct PktAw : public PktBase
{
	unsigned long	m_dwCharID;     // 캐릭터 아이디
	unsigned long	m_dwExp;        // 경험치
};


// 반전/전쟁모드 전환
typedef struct PktPeace* LPPktPeace;
struct PktPeace : public PktBase
{
	enum PktPeaceErr
	{
		FAIL_WRONG_SYNC		= 2,	// 클라이언트/서버 간의 싱크가 맞지 않음
		FAIL_BATTLE_GROUND	= 3,	// 배틀그라운드에서는 모드 변경 불가능
		FAIL_GUILD_MEMBER	= 4		// 길드원은 모드 변경 불가능
	};

	unsigned long	m_dwCharID;		// 캐릭터 아이디
	unsigned char	m_cLeftTime;	// 남은 시간
	bool			m_bPeace;		// 반전 모드

};


// 소환수 명령
typedef struct PktSummonCmd* LPPktSummonCmd;
struct PktSummonCmd : public PktBase
{
	enum SummonCmd
	{
		SUC_ATTACK		= 0,
		SUC_RETURN		= 1,
		SUC_DIE			= 2,
		SUC_GUARD_ON	= 3,
		SUC_GUARD_OFF	= 4
	};

	unsigned long	m_dwCharID;		// 캐릭터 아이디
	unsigned long	m_dwTargetID;	// 대상 아이디
	unsigned char	m_cCmd;			// 명령
};


// 석상 정보
typedef struct PktStatueInfo* LPPktStatueInfo;
struct PktStatueInfo : public PktBase
{
	unsigned long	m_dwMileage;	// 보너스가 추가된 공헌메달 (0이면 보너스 없음)

	unsigned short	m_wHumanNum;	// 전체 휴먼 수
	unsigned short	m_wAkhanNum;	// 전체 아칸 수

	unsigned short	m_wNowHP;		// 석상의 남은 HP
	unsigned short	m_wKID;			// 석상 종류 아이디
	unsigned char	m_cState;		// 석상 상태 (몬스터 State와 동일)
	unsigned char	m_cIndex;		// 석상 인덱스
};


// 카메라 스크립트
typedef struct PktCameraScript* LPPktCameraScript;
struct PktCameraScript : public PktBase
{
	unsigned long	m_dwDeleteObject;	// 사라지는 오브젝트 아이디
	unsigned long	m_dwNewObject;		// 생성되는 오브젝트 아이디
	POS				m_NewPos;			// 생성되는 오브젝트 위치
};

// 사망 및 부활 정보
struct PktDeadInfo : public PktDD 
{
    enum TypeCode
    {
        DEAD        = 0,
        RESPAWN     = 1,
        RESURRECT   = 2
    };

    /*
    typecode:00 HP/MP 죽은넘CID / 죽은넘이름 / 살해자CID / 살해자이름 / 살해자길드명
    typecode:00 HP/MP 죽은넘CID / 죽은넘이름 / 살해자CID / 살해자이름 /

    'XXXXX님이 몬스터 YYYYY에 의해 사망하셨습니다.'
    'XXXXX님이 YYYYY길드의 ZZZZZ님에 의해 사망하셨습니다.'		
    'XXXXX님이 ZZZZZ님에 의해 사망하셨습니다.'

    typecode:01 HP/MP 죽은넘CID / 죽은넘이름 /
    'XXXXX님이 일반 부활을 선택하셨습니다. 마을이나 요새에서 리스폰하게 됩니다.'

    typecode:02 HP/MP 죽은넘CID / 죽은넘이름 / 살린넘CID / 살린넘이름 / 살린넘길드명
    typecode:02 HP/MP 죽은넘CID / 죽은넘이름 / 
    'XXXXX님에 의해 YYYYY님이 부활 하셨습니다.'
    '누군가에 의해 YYYYY님이 부활 하셨습니다.'
    */

    unsigned char       m_cTypeCode;            // 타입코드
    unsigned short      m_usDeadMemCurrHP;      // 죽은넘 HP
    unsigned short      m_usDeadMemCurrMP;      // 죽은넘 MP

    unsigned long       m_dwDeadCID;            // 죽은넘 CID
    unsigned long       m_dwDeadPID;            // 죽은넘 PID
    unsigned long       m_dwReferenceID;        // 죽이거나 살린넘 CID

	unsigned char		m_cDeadNation;			// 죽은넘 국적
	unsigned char		m_cDeadClass;			// 죽은넘 클래스
	unsigned char		m_cRefNation;			// 죽이거나 살린넘 국적
	unsigned char		m_cRefClass;			// 죽이거나 살린넘 클래스

    char                m_szDeadName[CHAR_INFOST::MAX_NAME_LEN];
    char                m_szRefName[CHAR_INFOST::MAX_NAME_LEN];
    char                m_szRefGuildName[Guild::MAX_GUILD_NAME_LEN];
};


#pragma pack()

#endif
#ifndef _BATTLEGROUND_SERVER_PACKET_H_
#define _BATTLEGROUND_SERVER_PACKET_H_

#include <Network/Packet/PacketBase.h>
#include <Creature/Character/CharacterClass.h>

#pragma pack(1)


// 방 리스트 (가변 길이) + 방 정보
typedef struct PktBGServerMapList* LPPktBGServerMapList;
struct PktBGServerMapList : public PktBase
{
	enum Const
	{
		MAX_MAP_NUM = 10
	};

	bool			m_bAll;				// true 이면 모든 맵 리스트, false 이면 자신의 맵 정보
	unsigned char	m_cMapInfoNodeNum;	// 노드 수
};


// 방 정보
struct BGServerMapInfoNode
{
	enum MapType
	{
		FRAG			= 0,		// 스코어 경쟁 게임 방
		STATUE			= 1,		// 석상 점령 게임 방

		MAX_MAP_TYPE
	};

	// 배틀 그라운드 하나의 가상 맵이 가져야 하는 변수
	bool			m_bPlaying;								// 게임 중인지 여부
    unsigned char	m_cMapType;								// 게임 방식
	unsigned char	m_cMaxCharNumOfNation;					// 한 진영 최대 인원
	unsigned char	m_cRemainMin;							// 남은 시간 (경기시간 혹은 쉬는시간) (분)
	unsigned short	m_wTargetScore;							// 목표 점수
	unsigned char	m_cCurrentCharNum[CClass::MAX_RACE];	// 현재 인원
	unsigned short	m_wScore[CClass::MAX_RACE];				// 점수
};


// 방 결과 리스트 (가변 길이) + 방 결과 정보
typedef struct PktBGServerResultList* LPPktBGServerResultList;
struct PktBGServerResultList : public PktBase
{
	enum Const
	{
		MAX_MAP_NUM = 10
	};

	bool			m_bAll;					// true 이면 모든 방 리스트, false 이면 자신의 방 정보
	unsigned char	m_cResultInfoNodeNum;	// 노드 수
};


// 방 결과 정보
struct BGServerResultInfoNode
{
	unsigned char	m_cWinRace;						// 승자 (CClass::MAX_RACE 이면 무승부)
	unsigned char	m_cPlayMin;						// 실제 플레이한 시간
	unsigned char	m_cKill;						// 상대방을 죽인 수 
	unsigned char	m_cKilled;						// 죽은 수
	unsigned short	m_wAward;						// 보상으로 받은 마일리지
	unsigned short	m_wScore[CClass::MAX_RACE];	// 점수
};


// BATTLEGROUND_SERVER 존 이동
typedef struct PktBGServerMoveZone* LPPktBGServerMoveZone;
struct PktBGServerMoveZone : public PktBase
{
	enum MoveType
	{
		TYPE_PLAYER		= 0,	// 게임 참전자
		TYPE_SPECTATOR	= 1,	// 게임 관전자

		MAX_TYPE_NUM
	};

	enum Error
	{
		FAIL_FULL_MAP	= 2,	// 방 정원 초과
		FAIL_FIX_RATE	= 3		// 각 팀의 균형을 맞추기 위한 비율 제한
	};

	unsigned short	m_wMapNumber;	// 해당 방 번호 (0은 대기실)
	unsigned char	m_cMoveType;	// 방에 들어가는 타입
	unsigned char	m_cZone;		// 해당 방의 존 번호 (Ack)
};


// 정섭 캐릭터 정보
typedef struct PktBGServerCharSlot* LPPktBGServerCharSlot;
struct PktBGServerCharSlot : public PktBase
{
	unsigned long	m_dwCID;		// 캐릭터 아이디 (Client <-> Game)
	unsigned long	m_dwUID;		// 유저 아이디   (Game <-> DBAgent)
	unsigned char	m_cGroup;		// 서버 그룹
};

// 정섭 캐릭터 정보 Ack
typedef struct PktBGServerCharSlotAck* LPPktBGServerCharSlotAck;
struct PktBGServerCharSlotAck : public PktBase
{
    enum PktCharSlotAckErr
    {
        FAIL_NOT_USER	= 2,		// DB에서 유저 정보 얻기 실패
        FAIL_NOT_CHAR	= 3			// DB에서 캐릭터 정보 얻기 실패
    };

	unsigned long	m_dwCID;		// 캐릭터 아이디
	unsigned char	m_cGroup;		// 서버 그룹

    unsigned long	m_dwSlotCID[USER_INFO::MAX_CHAR_NUM];								// 정섭 캐릭터 ID
	char            m_szSlotName[USER_INFO::MAX_CHAR_NUM][CHAR_INFOST::MAX_NAME_LEN];	// 정섭 캐릭터 이름
};


// 환전소 이용
typedef struct PktBGServerMileageChange* LPPktBGServerMileageChange;
struct PktBGServerMileageChange : public PktBase
{
	enum PktBGSMCCmd
	{
		MC_REQUEST		= 0,	// 정보 요청 (선택한 정섭의...)
		MC_INFO			= 1,	// 정보 Ack
		MC_CHANGE		= 2,	// 교환 요청 (BG의 공헌메달 -> 정섭의 돈)
		MC_RESULT		= 3		// 교환 Ack
	};

    enum PktErr
    {
        SERVER_ERROR        = 1,
        NOT_USED_STORE      = 2,    // 창고 사용한적이 없는 경우 - MC_RESULT (메시지 처리 필요)
        NOT_ENOUGH_MEDAL    = 3,    // 공헌 메달 부족 - MC_RESULT (메시지 처리 필요)
        MONEY_OVERFLOW      = 4,    // 돈 오버플로우 발생(입금 불가) - MC_RESULT ( 메시지 처리 필요)
        DATA_REQUESTING     = 5,    // 서버 데이터 요청중임 - MC_RESULT (메시지 처리 필요 없음)
        NONE_CHARACTER      = 6     // 캐릭터가 없는 경우 - MC_INFO (메시지 처리 필요)
    };

	unsigned long	m_dwCID;		// 캐릭터 ID
	unsigned char	m_cGroup;		// 서버 그룹

	unsigned char	m_cCmd;			// 명령

	unsigned long	m_dwGold;		// 돈
	unsigned long	m_dwMileage;	// 공헌 메달
};

#pragma pack()


#endif
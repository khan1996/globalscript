#ifndef _CASTLE_PACKET_H_
#define _CASTLE_PACKET_H_

#include <algorithm>

#include <Network/Packet/PacketBase.h>
#include <Network/Packet/PacketStruct/CharMovePacket.h>

#include <DB/DBDefine.h>
#include <Item/ItemStructure.h>
#include <Castle/CastleConstants.h>
#include <Creature/Siege/SiegeConstants.h>
#include <Community/Guild/GuildConstants.h>

#pragma pack(1)

using namespace Castle;
using namespace Siege;

// 성 관리 권한
struct CastleRight
{
	enum Index
	{
		CONTROL_CASTLE_EMBLEM	= 0,		// 성 상징물 관리
		CONTROL_CASTLE_GATE		= 1,		// 성문 관리
		USE_CASTLE_GATE			= 2,		// 성문 이용
		CONTROL_CASTLE_ARMS		= 3,		// 병기 관리
		USE_CASTLE_ARMS			= 4,		// 병기 이용

		MAX_RIGHT_NUM			= 10
	};

	unsigned char	m_aryCastleRight[MAX_RIGHT_NUM];

	CastleRight()
	{
		std::fill_n(&m_aryCastleRight[0], int(MAX_RIGHT_NUM), Guild::COMMON);
	};

	bool Check(unsigned char cRightType, unsigned char cTitle)
	{
		return (m_aryCastleRight[cRightType] >= cTitle);
	}
};

// 길드 요새 관리 권한
struct CampRight
{
	enum Index
	{
		CAMP_CONTROL		= 0,	// 길드 요새 관리 권한

		MAX_RIGHT_NUM		= 10
	};

	unsigned char	m_aryCampRight[MAX_RIGHT_NUM];

	CampRight()
	{
		std::fill_n(&m_aryCampRight[0], int(MAX_RIGHT_NUM), Guild::MASTER);
	}

	bool Check(unsigned char cRightType, unsigned char cTitle)
	{
        return (m_aryCampRight[cRightType] >= cTitle);
	}
};


// 성 정보 DB 데이터
struct CastleInfoDB
{
	unsigned long	m_dwCastleID;							// Castle ID
	unsigned char	m_cNation;								// 소유 국가
	unsigned char	m_cZone;								// 존 번호
	unsigned char	m_cChannel;								// 채널 번호
	unsigned char	m_cNameID;								// 성 이름 ID
	unsigned char	m_cSiegeCount;							// 공성 횟수 (임시 세금 -> 누적 세금으로 옮길때 사용)
	unsigned char	m_cInvincibleCount;						// 무적 횟수
	unsigned short	m_wTotalSiegeCount;						// 누적 공성 횟수
	unsigned long	m_dwTotalTaxMoney;						// 누적 세금 회수량
	char			m_szRight[CastleRight::MAX_RIGHT_NUM];	// 길드원들의 성 관리 권한
	POS				m_InPos;								// 뒷문 사용시 성안으로 이동될 위치
	POS				m_OutPos;								// 뒷문 사용시 성밖으로 이동될 위치
	unsigned short	m_wItemID;								// 업그레이드에 사용할 보석 아이템 아이디
	unsigned char	m_cItemNum;								// 업그레이드에 사용할 보석 아이템 갯수
};

// 성의 세율 및 세금 DB 데이터
struct CastleTaxInfoDB
{
	unsigned long	m_dwCastleID;							// Castle ID
	unsigned char	m_cTaxType;								// 종류 (구매/판매, 수리, 재련, 요새 Gold, 요새 광물)
	unsigned char	m_cTax;									// 세율
	unsigned long	m_dwTempTaxMoney;						// 임시 세금
	unsigned long	m_dwTaxMoney;							// 세금
	unsigned char	m_cTaxChangable;						// 세율 변경 가능 여부
};

// 성의 광물 세금 데이터
struct CastleMineralInfoDB
{
	unsigned long	m_dwCastleID;							// Castle ID
	unsigned short	m_wMineralID;							// 광물 Item ID
	unsigned short	m_wAmount;								// 양
	unsigned char	m_cFlag;								// 누적 or 임시 체크 플래그
};

// 성 관련 오브젝트 DB 데이터
struct CastleObjectInfoDB
{
	unsigned long	m_dwCID;
	unsigned long	m_dwCastleID;							// CastleID : 성 상징물, 성문, 뒷문, 가드, 근거리/원거리 수성병기
	unsigned long	m_dwOwnerID;							// 근거리/원거리 수성병기의 소유자 CID
	unsigned long	m_dwHP;									// HP
	unsigned short	m_wObjectType;							// 성 오브젝트 타입 ( 성 상징물, 성문, 뒷문, 가드, 근거리/원거리 수성병기)
	float			m_fDefaultDir;							// 성 오브젝트의 기본 방향
	unsigned char	m_cState;								// 현재 오브젝트의 상태
	unsigned char	m_cSubState;							// 현재 오브젝트의 서브 상태		(Use EMBLEM & GATE)
	unsigned char	m_cUpgradeStep;							// 현재 오브젝트의 업그레이드 단계	(Except GUARD)
	unsigned char	m_cUpgradeType;							// 성 상징물의 업그레이드 타입		(Only EMBLEM)
	TIME			m_RemainUpTime;							// 각종 남은 시간 저장 변수
	TIME			m_LastUseTime;							// 마지막 사용 시간
	POS				m_Pos;									// 위치
};


// 요새 오브젝트 DB 데이터
struct CampInfoDB
{
	unsigned long	m_dwCampID;								// CampID
	unsigned long	m_dwGID;								// 소유 Guild ID
	unsigned long	m_dwHP;									// HP
	unsigned char	m_cZone;								// 존
	unsigned char	m_cChannel;								// 채널 정보
	unsigned short	m_wObjectType;							// 요새 오브젝트 타입 ( 요새, 채굴기, 상점, 월드웨폰 )
	unsigned char	m_cState;								// 길드 요새 상태
	unsigned char	m_cSubState;							// 길드 요새 서브 상태 ( 채굴기의 On/Off/Ready 상태 )
	unsigned char	m_cUpgradeStep;							// 업그레이드 단계
	TIME			m_TimeValue;							// 구축시까지 남은 시간
	TIME			m_LastUseTime;							// 마지막으로 사용한 시간
	char			m_szRight[CampRight::MAX_RIGHT_NUM];	// 길드원들의 길드 요새 관리 권한
	POS				m_Pos;									// 위치
	unsigned char	m_cMaterial;							// 공성용 자재 갯수
	unsigned char	m_cSiegeCount;							// 공성이 지나간 횟수
};

// 채굴기의 광물 정보 DB 데이터
struct CampMineralInfoDB
{
	unsigned long	m_dwCampID;								// CampID
	unsigned short	m_wMineralID;							// 광물 Item ID
	unsigned short	m_wAmount;								// 양
	unsigned char	m_cFlag;								// 누적 or 임시 체크 플래그
};

// 지력 정보 DB 데이터
struct FertilityInfoDB
{
	unsigned long	m_dwVeinColor;							// 광맥 색상
	unsigned long	m_dwFertility;							// 현재 지력
	unsigned char	m_cZone;								// 존 번호
	unsigned char	m_cChannel;								// 채널 번호
};

// 게임 시간 DB 데이터
struct GameTimeDB
{
	unsigned char	m_cGuildTime;							// 길드 전쟁 시간 정보
	unsigned char	m_cRealmTime;							// 국가 전쟁 시간 정보
	unsigned char	m_cSiegeTime;							// 공성전 시간 정보
	unsigned char	m_cMiningTime;							// 임시 채굴기 시간 정보
	unsigned char	m_cEnterTime;							// 테섭용 신규존 진입제한 플래그
};

// 길드 요새 상점 정보 DB 테이터
struct CampShopInfoDB
{
	enum
	{
		MAX_CONTAINER_SIZE		= 7995						// 	m_dwTotalSize + m_cItemNum + m_aryItemContainer = 8000
	};

	unsigned long	m_dwCampID;								// 길드 요새 ID
	unsigned long	m_dwTotalSize;							// 아이템 사이즈 (아이템 버퍼 + 가격)
	unsigned char	m_cItemNum;								// 아이템 갯수
	unsigned char	m_aryItemContainer[MAX_CONTAINER_SIZE];	// 판매중인 아이템 목록
	unsigned long	m_dwTempSafe;							// 임시 금고
	unsigned char	m_cTax;									// 세율
};

// 월드 웨폰 정보 DB 데이터
struct WorldWeaponInfoDB
{
	unsigned char	m_cZone;								// 존 번호
	unsigned char	m_cChannel;								// 채널 번호
	unsigned char	m_cKarRemainSiegeTime;					// 카르테란트 월드 웨폰이 부서지고난 후 지나야할 공성 시간
	unsigned char	m_cMerRemainSiegeTime;					// 메르카디아 월드 웨폰이 부서지고난 후 지나야할 공성 시간
};

// 국가 전쟁 석상 정보 DB 데이터
struct StatueInfoDB
{
	unsigned long	m_dwIndex;								// 석상 인덱스 (존에 배치한 숫자만큼 존재)
	unsigned char	m_cChannel;								// 채널
	unsigned short	m_wKID;									// 현재 석상의 상태 (KID 로 상태 판별)
	unsigned long	m_dwLostHP;								// 석상의 깍인 HP 정보
};


// 성 정보 (DBAgentServer -> GameServer)
// 가변 정보 : + CastleInfoDB
//			   + CastleTaxInfo * Castle::MAX_TAX_TYPE
//			   + CastleMineralInfo + CastleMineral * ( m_cAccumulatedNum + m_cTemporaryNum )
//			   + CastleObjectInfo * m_cCastleObjectNum
struct PktCastleInfo : public PktBase
{
	bool			m_bStartFlag;							// 처음 정보인가?
	unsigned char	m_cCastleObjectNum;						// 성 관련 오브젝트 수
	unsigned short	m_wSize;								// 성 정보 크기
};

struct CastleTaxInfo
{
	unsigned char	m_cTaxType;								// 종류 (구매/판매, 수리, 재련, 요새 Gold, 요새 광물)
	unsigned char	m_cTax;									// 세율
	unsigned long	m_dwTempTaxMoney;						// 임시 세금
	unsigned long	m_dwTaxMoney;							// 세금
	unsigned char	m_cTaxChangable;						// 세율 변경 가능 여부
};

struct CastleMineral
{
	unsigned short	m_wMineralID;							// 광물 Item ID
	unsigned short	m_wAmount;								// 광물 갯수
};

struct CastleMineralInfo
{
	unsigned char	m_cAccumulatedNum;						// 누적 보관된 광물의 갯수
	unsigned char	m_cTemporaryNum;						// 임시 보관된 광물의 갯수
	unsigned short	m_wSize;								// 광물 정보 크기
};

struct CastleObjectInfo
{
	unsigned long	m_dwCID;								// CID
	unsigned long	m_dwCastleID;							// CastleID : 성 상징물, 성문, 뒷문, 가드, 근거리/원거리 수성병기
	unsigned long	m_dwOwnerID;							// 근거리/원거리 수성 병기의 소유자 CID
	unsigned char	m_cNation;								// 국가
	unsigned long	m_dwHP;									// HP
	unsigned short	m_wObjectType;							// 성 오브젝트 타입 ( 성 상징물, 성문, 뒷문, 가드, 근거리/원거리 수성병기)
	float			m_fDefaultDir;							// 성 오브젝트의 기본 방향
	unsigned char	m_cState;								// 현재 오브젝트의 상태
	unsigned char	m_cSubState;							// 현재 오브젝트의 서브 상태		(Use EMBLEM & GATE)
	unsigned char	m_cUpgradeStep;							// 현재 오브젝트의 업그레이드 단계	(Except GUARD)
	unsigned char	m_cUpgradeType;							// 성 상징물의 업그레이드 타입		(Only EMBLEM)
	POS				m_Pos;									// 위치
};

// 세금 이동 정보 패킷
// DBAgentServer -> GameServer -> Client	: 임시 세금 -> 누적 세금으로 이동
// GameServer -> DBAgentServer -> DB		: 임시 세금 정보 업데이트
typedef struct PktCastleTaxMove* LPPktCastleTaxMove;
struct PktCastleTaxMove : public PktBase
{
	unsigned long	m_dwCastleID;								// 성 ID
	unsigned long	m_dwMoveTempMoney[ Castle::MAX_TAX_TYPE ];	// 누적 세금으로 이동한 임시 세금 액수, 업데이트 될 임시 세금 정보
	unsigned long	m_dwResultTaxMoney[ Castle::MAX_TAX_TYPE ];	// 임시 세금이 이동된 후의 누적 세금 액수
};

// 성의 광물 세금 정보 패킷
// 가변 정보  : + CastleMineral * m_cNum
typedef struct PktCastleMineralInfo* LPPktCastleMineralInfo;
struct PktCastleMineralInfo : public PktBase
{
	unsigned long	m_dwCastleID;							// 성 ID
	unsigned char	m_cMineralType;							// 광물 타입 (누적, 임시)
	unsigned char	m_cNum;									// 광물 수
	unsigned short	m_wSize;								// 광물 정보 크기
};

// 성의 공성 횟수 정보 패킷
typedef struct PktCastleSiegeCount* LPPktCastleSiegeCount;
struct PktCastleSiegeCount : public PktBase
{
	unsigned long	m_dwCastleID;							// 성 ID
	unsigned char	m_cSiegeCount;							// 공성 횟수 (임시 세금 -> 누적 세금)
	unsigned char	m_cInvincibleCount;						// 남은 무적 공성 횟수
	unsigned short	m_wTotalSiegeCount;						// 누적 공성 횟수
};


// 국가 전쟁 석상 정보 구조체
struct StatueInfo
{
	unsigned long	m_dwIndex;
	unsigned short	m_wKID;
	unsigned long	m_dwLostHP;
};

// 국가 전쟁 석상 디스플레이 정보 구조체
struct StatueDisplayInfo
{
	unsigned long	m_dwIndex;
	unsigned long	m_dwKID;
	unsigned char	m_cNation;
	bool			m_bSummoning;
	bool			m_bAttacked;
	POS				m_Pos;
};

// 국가 전쟁 석상 정보 패킷 (DBAgentServer <-> GameServer / GameServer -> Client)
// 가변 정보 :
//		wSubCmd : CMD_INFO				+ StatueInfo * 석상 수
//				: CMD_UPDATE_LOST_HP	+ StatueInfo
//				: CMD_UPDATE_KID		+ StatueInfo
//				: CMD_DISPLAY_INFO		+ StatueDisplayInfo * 석상 수	(클라이언트 처음 로그인시)
//										+ StatueDisplayInfo				(석상 정보 변경시)

typedef struct PktStatueCmd* LPPktStatueCmd;
struct PktStatueCmd : public PktBase
{
	enum SubCmd
	{
		CMD_INFO			= 0,	// DBAgentServer	->	GameServer
		CMD_UPDATE_LOST_HP	= 1,	// GameServer		->	DBAgentServer
		CMD_UPDATE_KID		= 2,	// GameServer		->	DBAgentServer
		CMD_DISPLAY_INFO	= 3		// GameServer		->	Client
	};

	enum Const
	{
		MAX_STATUE_NUM		= 16
	};

	unsigned short	m_wSubCmd;
	unsigned short	m_wSize;		// INFO : 가변 버퍼 사이즈  /  LOST_HP : sizeof(StatueInfo)	/  KID : sizeof(StatueInfo)	/	DISPLAY_INFO : 가변 버퍼 사이즈
	unsigned long	m_dwValue;		// INFO : 석상 수			/  LOST_HP : Channel			/  KID : Channel			/	DISPLAY_INFO : 석상 수
};


// 길드 요새 정보 (DBAgentServer -> GameServer)
// 가변 정보 : 채굴기   - CampInfoDB + MiningCampInfo + CampMineralInfo * (m_cAccumulatedNum + m_cTemporaryNum)
//             상점     - CampInfoDB + CampShopInfo + 아이템 버퍼
//             월드웨폰 -
struct PktCampInfo : public PktBase
{
	bool			m_bStartFlag;							// 처음 정보인가?
	unsigned long	m_dwCID;								// 요새의 CID
	unsigned short	m_wSize;								// 요새 정보 크기
};

struct MiningCampInfo
{
	unsigned char	m_cAccumulatedNum;						// 누적 보관된 광물의 갯수
	unsigned char	m_cTemporaryNum;						// 임시 보관된 광물의 갯수
	unsigned short	m_wSize;								// 광물 정보 크기
};

struct CampMineralInfo
{
	unsigned short	m_wMineralID;							// 광물 Item ID
	unsigned short	m_wAmount;								// 광물 갯수
};

struct CampShopInfo
{
	unsigned long	m_dwTempSafe;							// 임시 금고
	unsigned char	m_cTax;									// 세율
	unsigned long	m_dwBufferSize;							// 아이템 버퍼 사이즈
	unsigned char	m_cItemNum;								// 아이템 갯수
};

// 월드 웨폰 정보 전송 (DBAgentServer -> GameServer)
struct PktWorldWeaponInfo : public PktBase
{
	unsigned char	m_cKarRemainSiegeTime;					// 카르테란트 국가의 월드 웨폰이 부서진후 지나야할 공성 시간
	unsigned char	m_cMerRemainSiegeTime;					// 메르카디아 국가의 월드 웨폰이 부서진후 지나야할 공성 시간
};

// 지력 정보
// 가변 정보 (DBAgentServer -> GameServer) : + (dwVeinColor + dwFertility) * m_wNum
// 가변 정보 (GameServer -> DBAgentServer) : + ZoneChannelInfo + (dwVeinColor + dwFertility) * m_wNum
struct PktFertilityInfo : public PktBase
{
	unsigned short	m_wNum;									// 광맥 갯수
	unsigned short	m_wSize;								// 지력 정보 크기
};

// 길드 요새 상점 정보 (+ 아이템 버퍼)
struct PktCampShopInfo : public PktBase
{
	unsigned long	m_dwCampID;								// 길드 요새 ID
	CampShopInfo	m_CampShopInfo;							// 길드 요새 상점 정보
};

struct ZoneChannelInfo
{
	unsigned char	m_cZone;
	unsigned char	m_cChannel;
};

// 현재 누적된 채굴기 광물량 확인 패킷
// 가변 정보  : + CampMineralInfo * m_cNum
typedef struct PktMiningCampMineralInfo* LPPktMiningCampMineralInfo;
struct PktMiningCampMineralInfo : public PktBase
{
	unsigned long	m_dwCampID;								// 채굴기 요새 ID
	unsigned long	m_dwRemainTime;							// 남은 시간 (단위: 일)
	unsigned char	m_cState;								// 현재 상태
	unsigned char	m_cMineralType;							// 광물 타입 (누적, 임시)
	unsigned char	m_cNum;									// 광물 수
	unsigned short	m_wSize;								// 광물 정보 크기
};

// 채굴기 프로세스 실행 패킷
typedef struct PktProcessMining* LPPktProcessMining;
struct PktProcessMining : public PktBase
{
	enum
	{
		TEMPORARY_PROCESS	= 1,
		ACCUMULATED_PROCESS	= 2
	};

	unsigned long	m_dwProcessType;						// 임시 채굴 프로세스인지, 누적 채굴 프로세스인지
};


// 성 생성 (GameServer -> Client)
// 성의 광물 정보는 필요할때 서버에서 보내준다.
typedef struct PktCreateCastle*	LPPktCreateCastle;
struct PktCreateCastle : public PktBase
{
	unsigned long	m_dwCastleID;							// CastleID
	unsigned char	m_cNation;								// 소유 국가
	unsigned char	m_cZone;								// 존 번호
	unsigned char	m_cNameID;								// 성 이름 ID
	unsigned char	m_cUpgradeStep;							// 성 상징물 업그레이드 단계
	unsigned char	m_cInvincibleCount;						// 무적 횟수
    unsigned short	m_wTotalSiegeCount;						// 누적 공성 횟수
	unsigned long	m_dwTotalTaxMoney;						// 누적 세금 회수량
	float			m_fPosX ;								// 성의 위치
	float			m_fPosY ;
	float			m_fPosZ ;
	unsigned short	m_wItemID;								// 업그레이드에 사용할 보석 아이템 ID
	unsigned char	m_cItemNum[Castle::EMBLEM_UPGRADE_JEWEL_POS_NUM];	// 업그레이드에 사용할 보석 아이템 갯수
	CastleRight		m_CastleRight;							// 길드원들의 성 관리 권한
	CastleTaxInfo	m_CastleTax[Castle::MAX_TAX_TYPE];		// 성 세율/세금 정보
};


// 길드 요새 생성 (GameServer -> Client, GameServer -> DBAgentServer)
typedef struct PktCreateCamp*	LPPktCreateCamp;
struct PktCreateCamp : public PktBase
{
	enum Error
	{
		ERROR_EXIST_ANOTHER_WEAPON	= 2,						// 이미 월드 웨폰이 존재할 경우
	};

	unsigned long	m_dwActorCID;							// 요새 생성을 요청한 캐릭터의 CID
	unsigned long	m_dwCID;								// CID
	unsigned long	m_dwCampID;								// CampID
	unsigned long	m_dwGID;								// 소유 Guild ID
	unsigned long	m_dwHP;									// HP
	unsigned short	m_wObjectType;							// 길드 요새 타입
	unsigned char	m_cZone;								// 존 번호 (GameServer -> DBAgentServer 에서 사용)
	unsigned char	m_cChannel;								// 채널 번호 (GameServer -> DBAgentServer 에서 사용)
	unsigned char	m_cState;								// 길드 요새 상태
	unsigned char	m_cSubState;							// 길드 요새 서브 상태
	unsigned char	m_cUpgradeStep;							// 업그레이드 단계
	unsigned char	m_cNation;								// 캠프 국적 (길드 국적 : 클라이언트에 길드 정보가 없을때를 위해서)
	CampRight		m_CampRight;							// 길드 요새 관리 권한
	POS				m_Pos;									// 위치
	unsigned char	m_cMaterial;							// 공성용 자재 갯수
};


// 공성 병기 생성 (GameServer -> Client, GameServer -> DBAgentServer)
typedef struct PktCreateSiegeArms*	LPPktCreateSiegeArms;
struct PktCreateSiegeArms : public PktBase
{
	unsigned long	m_dwCID;								// CID (공성 병기 ID)
	unsigned long	m_dwOwnerID;							// 공성 병기 주인 CID
	unsigned short	m_wObjectType;							// 공성 병기 타입
	unsigned char	m_cNation;								// 공성 병기 소유주의 국가
	unsigned long	m_dwHP;									// HP
	unsigned char	m_cZone;								// 존 번호 (GameServer -> DBAgentServer 에서 사용)
	unsigned char	m_cChannel;								// 채널 번호 (GameServer -> DBAgentServer 에서 사용)
	unsigned char	m_cState;								// 공성 병기 상태
	unsigned char	m_cUpgradeStep;							// 공성 병기 업그레이드 단계
	POS				m_Pos;									// 위치
};


// 길드 요새 관련 명령 패킷 ( Client <-> GameServer <-> DBAgentServer )
typedef struct PktCampCmd*	LPPktCampCmd;
struct PktCampCmd : public PktBase
{
	enum SubCmd
	{
		CAMP_NO_CMD					= 0,

		// 길드 요새 기본 명령
		CAMP_BUILD					= 1,	// 길드 요새 구축 (사용 안함, 스타트킷 아이템으로 사용)
		CAMP_BUILD_COMPLETE			= 2,	// 길드 요새 구축 완료
		CAMP_CANCEL					= 3,	// 길드 요새 구축 해제
		CAMP_CANCEL_COMPLETE		= 4,	// 길드 요새 구축 해제 완료
		CAMP_UPGRADE				= 5,	// 길드 요새 업그레이드
		CAMP_UPGRADE_COMPLETE		= 6,	// 길드 요새 업그레이드 완료
		CAMP_REPAIR					= 7,	// 길드 요새 수리 
		CAMP_REPAIR_COMPLETE		= 8,	// 지지 수리 완료
		CAMP_TO_STARTKIT			= 9,	// 스타트킷으로
		CAMP_TO_STARTKIT_COMPLETE	= 10,	// 스타트킷으로 완료
		CAMP_CHANGE_TYPE			= 11,	// 길드 요새 타입 변형
		CAMP_CHANGE_COMPLETE		= 12,	// 길드 요새 타입 변형 완료
		CAMP_DESTROY				= 13,	// 길드 요새 파괴시 / 7일간 캠프 미사용시 파괴
		CAMP_CARE					= 14,	// 손질하기

		CAMP_UPDATE_HP				= 15,	// HP 정보를 업데이트 (게임 서버의 HP 정보로 업데이트)

		// 채굴기 전용 명령
		MINING_CAMP_SHOW_MINERAL	= 16,	// 누적 광물 확인
		MINING_CAMP_ON_OFF			= 17,	// 채굴기 켜기 / 끄기	( dwValue1 : On / Off )
		MINING_CAMP_GAIN_MINERAL	= 18,	// 광물 획득			( dwValue1 : 광물 Index, dwValue2 : 획득량 )

		// 상점 전용 명령
		CAMP_SHOP_CHANGE_TAX		= 19,	// 세율 변경
		CAMP_SHOP_GAIN_TAX			= 20,	// 수익금 획득

		// 월드웨폰 전용 명령
		WORLDWEAPON_CHARGE			= 21,	// 무기 장전
		WORLDWEAPON_CHARGE_COMPLETE	= 22,	// 무기 장전 완료
		WORLDWEAPON_FIRE			= 23,	// 무기 발사
		WORLDWEAPON_FIRE_COMPLETE	= 24,	// 무기 발사 완료

		CAMP_NOT_REBUILDTIME		= 25	// 길드 요새 파괴 후 다시 구축할 수 있는 시간이 지나지 않았을 경우.
	};

	enum Error
	{
		FAIL_NOT_EXIST_GUILD		= 2,		// 존재 하지 않는 길드
		FAIL_NOT_EXIST_CAMP			= 3,		// 존재 하지 않는 성
		FAIL_NOT_CAMP				= 4,		// 진지가 아님
		FAIL_CAMP_STATE				= 5,		// 길드 요새 상태가 이상함
		FAIL_DIFFERENT_GUILD		= 6,		// 길드 아이디가 다름
		FAIL_NOT_MASTER				= 7,		// 길드 마스터가 아님
		FAIL_CAMP_RIGHT				= 8,		// 길드 요새 권한 없음
		FAIL_NOT_ENOUGH_MATERIAL	= 9,		// 자재 부족
		FAIL_DIFFERENT_GOLD			= 10,		// 클라이언트와 서버의 돈이 다름
		FAIL_NOT_ENOUGH_GOLD		= 11,		// 길드에 돈이 부족
		FAIL_NOT_MINING_CAMP		= 12,		// 채굴기가 아님 (채굴기로 캐스팅 실패)
		FAIL_NOT_COMPLETE_STATE		= 13,		// 완료상태(보통상태)가 아닙니다.
		FAIL_NOW_MINING				= 14,		// 채굴중입니다.
		FAIL_UPGRADE_STEP			= 15,		// 업그레이드 단계가 부족합니다.
		FAIL_NOT_ENOUGH_MINERAL		= 16,		// 광물수가 부족합니다.
		FAIL_NOT_CAMP_SHOP			= 17,		// 길드 요새 상점이 아닙니다.
		FAIL_NOT_ZERO_CUSTOMER		= 18,		// 입장객이 있을 때는 세율을 변경할 수 없습니다.
		FAIL_TOO_MANY_TAX			= 19,		// 세율이 최대치를 초과하였습니다.
		FAIL_NOT_WORLDWEAPON		= 20,		// 월드 웨폰이 아닙니다.
		FAIL_NOT_EMPTY_WEAPON		= 21,		// 월드 웨폰의 무기가 비어 있지 않습니다. (장전중, 장전됨, 발사중)
		FAIL_NOT_READY_WEAPON		= 22,		// 월드 웨폰의 무기가 준비 되어 있지 않습니다. (비어있음, 장전중, 발사중)
		FAIL_NOW_CHARGING			= 23,		// 월드 웨폰 무기 장전중입니다.
		FAIL_NOW_FIRING				= 24,		// 월드 웨폰 무기 발사중입니다.
		FAIL_MAX_UPGRADE			= 25,		// 업그레이드 Max 입니다.
		FAIL_HAS_MINERAL			= 26,		// 채굴기 타입을 변형하려 할때 광물이 남아 있을 경우
		FAIL_HAS_ITEM				= 27		// 상점 타입을 변형하려 할때 아이템이 남아 있을 경우
	};

	unsigned long	m_dwCID;			// Client -> GameServer : 보낸 사람 CID		GameServer -> DBAgentServer : 보낸 사람 CID 혹은 0
										// Client <- GameServer : 길드 요새 CID		GameServer <- DBAgentServer : 길드 요새 CID
	unsigned long	m_dwCampID;
	unsigned char	m_cState;

	unsigned long	m_dwValue1;
	unsigned long	m_dwValue2;

	unsigned char	m_cSubCmd;
};



// 성 관련 명령 패킷  ( Client <-> GameServer <-> DBAgentServer )
typedef struct PktCastleCmd*	LPPktCastleCmd;
struct PktCastleCmd : public PktBase
{
	enum SubCmd
	{
		CASTLE_NO_CMD					=  0,
		
		// 상징물 관련 명령																																																			
		CASTLE_SET_TAX					=  1,		// 세율 조정
//		CASTLE_SET_RIGHT				=  2,		// 권한 설정 (따로 패킷 만들어서 보냄)
		CASTLE_UPGRADE_EMBLEM			=  3,		// 상징물 업그레이드
		CASTLE_UPGRADE_EMBLEM_COMPLETE	=  4,		// 상징물 업그레이드 완료
		CASTLE_UPGRADE_ITEM_INFO		=  5,		// 성 상징물 업그레이드 보석 정보	(GameServer --> DBAgentServer)
		CASTLE_UPDATE_EMBLEM			=  6,		// 상징물 부서짐
		CASTLE_SUMMON_EMBLEM_COMPLETE	=  7,		// 상징물 소환 완료
		CASTLE_CHANGE_MASTER			=  8,		// 성주 변경
		CASTLE_TAKE_TAXMONEY			=  9,		// 세금 회수
		CASTLE_LOSE_OWNERSHIP			= 10,		// 길드 파괴등으로 성주 없어짐
		CASTLE_SHOW_MINERAL				= 11,		// 누적 광물 세금 정보 요청
		CASTLE_GAIN_MINERAL				= 12,		// 광물 획득 ( dwValue1 : 광물ID, dwValue2 : 획득량 )
																																																											
		// 성문/뒷문 관련 명령																																																		
		CASTLE_GATE_OPEN				= 13,		// 성문 열기
		CASTLE_GATE_CLOSE				= 14,		// 성문 닫기
		CASTLE_GATE_PROTECT				= 15,		// 성문 막기
		CASTLE_GATE_PROTECT_CANCEL		= 16,		// 성문 막기 해제																																								
		CASTLE_GATE_PROTECT_INFO		= 17,		// 성문 막기 정보
		CASTLE_UPGRADE_GATE				= 18,		// 성문 업그레이드
		CASTLE_UPGRADE_GATE_COMPLETE	= 19,		// 성문 업그레이드 완료
		CASTLE_REPAIR_GATE				= 20,		// 성문 수리
		CASTLE_REPAIR_GATE_COMPLETE		= 21,		// 성문 수리 완료
		CASTLE_RESTORE_GATE				= 22,		// 성문 복구
		CASTLE_RESTORE_GATE_COMPLETE	= 23,		// 성문 복구 완료
		CASTLE_DESTROY_GATE				= 24,		// 성문 파괴
		CASTLE_BACKDOOR					= 25,		// 뒷문 사용

		// 수성 병기 관련 명령																																																	
		CASTLE_CREATE_ARMS				= 26,		// 수성 병기 개발
		CASTLE_CREATE_ARMS_COMPLETE		= 27,		// 수성 병기 개발 완료
		CASTLE_RIDE_ARMS				= 28,		// 수성 병기 탑승
		CASTLE_GETOFF_ARMS				= 29,		// 수성 병기 내림
		CASTLE_UPGRADE_ARMS				= 30,		// 수성 병기 업그레이드
		CASTLE_UPGRADE_ARMS_COMPLETE	= 31,		// 수성 병기 업그레이드 완료
		CASTLE_REPAIR_ARMS				= 32,		// 수성 병기 수리
		CASTLE_REPAIR_ARMS_COMPLETE		= 33,		// 수성 병기 수리 완료
		CASTLE_DESTROY_ARMS				= 34,		// 수성 병기 파괴
		CASTLE_ARMS_CHANGE_MASTER		= 35,		// 수성 병기 성주 변경

		// HP 업데이트
		CASTLE_UPDATE_HP				= 36,		// HP 를 게임서버의 값으로 업데이트 (GameServer --> DBAgentServer)

		// 성문 추가 기능 ( DBAgentServer -> GameServer )
		CASTLE_GATE_FORCE_OPEN			= 37,		// 중계 서버가 성문을 강제로 열린 상태로 만든다.
		CASTLE_GATE_FORCE_CLOSE			= 38,		// 중계 서버가 성문을 강제로 닫힌 상태로 만든다.

		// 성 상징물 공격 받고 있다는 메세지 명령
		CASTLE_EMBLEM_ATTACKED			= 39,		// 성 상징물 공격 받음
	};

	enum Const
	{
		DESTROY		= 0,		// 수성 병기 파괴
		TO_NPC		= 1,		// 수성 병기를 병기 관리 NPC 로 ( 생산 비용 절반을 돌려줌 )
	};

	enum Error
	{
		FAIL_NOT_GUILD_MEMBER		= 2,	// 길드에 가입하지 않았습니다.
		FAIL_NOT_EXIST_CASTLE		= 3,	// 존재 하지 않는 성
		FAIL_DIFFERENT_GUILD		= 4,	// 길드 아이디가 다름
		FAIL_NOT_EXIST_GUILD		= 5,	// 존재 하지 않는 길드
		FAIL_CASTLE_RIGHT			= 6,	// 성 관련 권한이 없습니다.
		FAIL_MAX_UPGRADE			= 7,	// 최대 업그레이드 상태임
		FAIL_NOT_ENOUGH_JEWEL		= 8,	// 보석 갯수 부족
		FAIL_JEWEL_KIND				= 9,	// 적절하지 않은 보석 종류
		FAIL_CASTLE_OBJECT_STATE	= 10,	// 성 관련 오브젝트의 상태가 적절치 못할 경우
		FAIL_DIFFERENT_GOLD			= 11,	// 클라이언트와 서버의 돈이 다름
		FAIL_NOT_ENOUGH_GOLD		= 12,	// 금액이 부족할 경우
		FAIL_ALREADY_RIDE			= 13,	// 병기에 탑승해 있음
		FAIL_NOT_RIDE				= 14,	// 병기를 타고 있지 않음
		FAIL_CHANGE_TAX				= 15,	// 세율을 변경할수 없음
		FAIL_NOT_GUILD_MASTER		= 16,	// 길드 마스터가 아니다.
		FAIL_REQUEST_TOO_MUCH_GOLD	= 17,	// 너무 많은 돈을 요청했습니다.
		FAIL_NOT_ENOUGH_MINERAL		= 18,	// 너무 많은 수의 광물을 요청했습니다.
		FAIL_HAS_ANOTHER_CASTLE_ARMS = 19,	// 이미 다른 수성 병기를 가지고 있습니다.
		FAIL_NOT_MY_CASTLE_ARMS		= 20,	// 자신의 수성 병기가 아닙니다.
		FAIL_NOT_SIEGE_TIME			= 21,	// 수성 병기를 만들수 있는 시간이 아닙니다.
		FAIL_HAS_ANOTHER_SIEGE_ARMS	= 22,	// 이미 다른 공성 병기를 가지고 있습니다.
		NOT_LIMIT_LEVEL				= 23,	// 80 레벨 미만일 경우.
	};

	unsigned long	m_dwCID;						// 보낸 사람 CID
	unsigned long	m_dwCastleID;					// 성 ID
	unsigned long	m_dwCastleObjectID;				// 성 오브젝트 ID

	unsigned long	m_dwValue1;
	unsigned long	m_dwValue2;
	unsigned char	m_cState;

	unsigned char	m_cSubCmd;
};

// 성 관리 권한
typedef struct PktCastleRight* LPPktCastleRight;
struct PktCastleRight : public PktBase
{
	enum eError
	{
		FAIL_NOT_EXIST_GUILD		= 2,	// 존재 하지 않는 길드
		FAIL_NOT_EXIST_CASTLE		= 3,	// 존재 하지 않는 성
		FAIL_DIFFERENT_GUILD		= 4,	// 길드 아이디가 다름
		FAIL_NOT_MASTER				= 5		// 길드 마스터가 아님
	};

	unsigned long	m_dwCID;			// 아이디 (C -> S : CID)
	unsigned long	m_dwCastleID;
	CastleRight		m_CastleRight;		// 성 관리 권한
};


// 길드 요새 관리 권한
typedef struct PktCampRight* LPPktCampRight;
struct PktCampRight : public PktBase
{
	enum eError
	{
		FAIL_NOT_EXIST_GUILD		= 2,	// 존재 하지 않는 길드
		FAIL_NOT_EXIST_CAMP			= 3,	// 존재 하지 않는 길드 요새
		FAIL_NOT_CAMP				= 4,	// 캠프가 아님
		FAIL_DIFFERENT_GUILD		= 5,	// 길드 아이디가 다름
		FAIL_CAMP_STATE				= 6,	// 길드 요새 상태가 이상함
		FAIL_NOT_MASTER				= 7		// 길드 마스터가 아님
	};

	unsigned long	m_dwCID;			// 아이디 (C -> S : CID)
	unsigned long	m_dwCampID;
	CampRight		m_CampRight;		// 길드 요새 관리 권한
};

// 공성 오브젝트 자재 이동 패킷
typedef struct PktTakeMaterial* LPPktTakeMaterial;
struct PktTakeMaterial : public PktBase
{
	enum SubCmd
	{
		SC_TAKE_IN	= 1,				// 길드 요새에 자재 넣기
		SC_TAKE_OUT	= 2					// 길드 요새에서 자재 꺼내기
	};

	enum eError
	{
		FAIL_ITEM_NUMBER		= 2,	// 갯수가 이상함
		FAIL_NOT_EXIST_ITEMINFO	= 3,	// 존재하지 않는 아이템 정보 (자재 아이템 아이디 이상)
		FAIL_OVER_STACK			= 4,	// 스택 갯수 초과
		FAIL_NOT_EXIST_CAMP		= 5,	// 해당 요새가 존재 하지 않음
		FAIL_CREATE_ITEM		= 6,	// 공성용 자재 생성 실패
		FAIL_NOT_EXIST_ITEM		= 7,	// 공성용 자재가 없음
		FAIL_CAMP_RIGHT			= 8		// 권한 없음
	};

	unsigned long	m_dwCID;			// 행위자
	unsigned long	m_dwCampID;			// 요새 아이디
	unsigned char	m_cSubCmd;			// 넣기/빼기 커맨드
	unsigned char	m_cNum;				// 갯수
	Item::ItemPos	m_Pos;				// 아이템의 위치
};

// 성 업그레이드 보석 아이템 이동 패킷
typedef struct PktTakeCastleJewel* LPPktTakeCastleJewel;
struct PktTakeCastleJewel : public PktBase
{
	enum SubCmd
	{
		SC_TAKE_IN	= 1,				// 성 상징물 업그레이드에 보석 넣기
		SC_TAKE_OUT	= 2					// 성 상징물 업그레이드에서 보석 꺼내기
	};

	enum eError
	{
		FAIL_NOT_EXIST_CASTLE			= 2,	// 존재 하지 않는 성
		FAIL_NOT_EXIST_EMBLEM			= 3,	// 존재 하지 않는 성 상징물
		FAIL_WRONG_JEWEL				= 4,	// 적절하지 못한 보석
		FAIL_DIFFERENT_JEWEL			= 5,	// 다른 종류의 보석
		FAIL_NOT_EXIST_ITEM_PROTOTYPE	= 6,	// 아이템 프로토 타입이 존재하지 않음
		FAIL_OVER_STACK					= 7,	// 스택 오버 플러우
		FAIL_NOT_EXIST_ITEM				= 8,	// 해당 위치에 보석 아이템이 존재하지 않음
		FAIL_ITEM_NUMBER				= 9,	// 아이템 갯수가 이상함
		FAIL_CREATE_ITEM				= 10,	// 보석 아이템 생성 실패
	};

	unsigned long	m_dwCID;			// 행위자
	unsigned long	m_dwCastleID;		// 성 아이디
	unsigned char	m_cSubCmd;			// 넣기/빼기 커맨드
	unsigned short	m_wItemID;			// 보석 아이디
	unsigned char	m_cIndex;			// 보석을 넣은 UI 상의 위치 (0 ~ 9)
	unsigned char	m_cNum;				// 갯수
	Item::ItemPos	m_Pos;				// 아이템의 위치
};

// 공성 병기 관련 명령 패킷 ( Client <-> GameServer <-> DBAgentServer )
typedef struct PktSiegeArmsCmd*	LPPktSiegeArmsCmd;
struct PktSiegeArmsCmd : public PktBase
{
	enum SubCmd
	{
		SIEGE_NO_CMD				= 0,

		SIEGE_CREATE_ARMS			= 1,		// 공성 병기 생성 (사용 안함, 스타트킷 아이템으로 사용)
		SIEGE_CREATE_ARMS_COMPLETE	= 2,		// 공성 병기 생성 완료
		SIEGE_CANCEL				= 3,		// 공성 병기 생성 취소 (바로됨)
		SIEGE_RIDE_ARMS				= 4,		// 공성 병기 탑승
		SIEGE_GETOFF_ARMS			= 5,		// 공성 병기 내림
		SIEGE_REPAIR_ARMS			= 6,		// 공성 병기 수리
		SIEGE_REPAIR_ARMS_COMPLETE	= 7,		// 공성 병기 수리 완료
		SIEGE_TO_STARTKIT			= 8,		// 공성 병기를 스타트킷으로
		SIEGE_TO_STARTKIT_COMPLETE	= 9,		// 공성 병기를 스타트킷으로 완료
		SIEGE_DESTROY_ARMS			= 10,		// 공성 병기 파괴

		// HP 업데이트
		SIEGE_UPDATE_HP				= 12,		// HP 를 게임서버의 값으로 업데이트 (GameServer --> DBAgentServer)
	};

	enum Error
	{
		FAIL_NOT_EXIST_GUILD		= 2,		// 존재 하지 않는 길드
		FAIL_NOT_EXIST_SIEGE_ARMS	= 3,		// 존재 하지 않는 공성 병기
		FAIL_NOT_SIEGE_ARMS			= 4,		// 공성 병기가 아님
		FAIL_NOT_OWNER				= 5,		// 병기 소유자가 아님
		FAIL_SIEGE_ARMS_STATE		= 6,		// 공성 병기의 상태가 적절치 않음
		FAIL_ALREADY_RIDE			= 7,		// 병기에 탑승해 있음
		FAIL_NOT_RIDE				= 8,		// 병기를 타고 있지 않음
		FAIL_RIDE_SIEGE_ARMS		= 9,		// 병기 탑승에 실패
		FAIL_GET_OFF_SIEGE_ARMS		= 10,		// 병기 내리기 실패
		FAIL_NOT_ENOUGH_MATERIAL	= 11		// 자재 부족
	};

	unsigned long	m_dwCID;					// OwnerID 라고 보면된다.
	unsigned long	m_dwArmsID;

	unsigned char	m_cState;
	unsigned long	m_dwValue;					// 탑승한 캐릭터 CID, 내린 캐릭터 CID, 수리시 목표 HP, 자재를 모두 돌려주는가?
	
	unsigned char	m_cSubCmd;
};


// 공성 오브젝트 브로드 캐스트
typedef struct PktSiegeBroadCast* LPPktSiegeBroadCast;
struct PktSiegeBroadCast : public PktBase
{
// CASTLE_TODO : 사용하지 않는 옛날 공성 오브젝트 브로드 캐스트 패킷

	unsigned long	m_dwCID;
	unsigned long	m_dwOwnerID;	// 성, 길드 요새, 소유캐릭터 ID
	unsigned long	m_dwGID;		// 길드 ID
	unsigned long	m_dwNowHP;
	unsigned long	m_dwMaxHP;
	unsigned long	m_dwRiderID[Siege::AIRSHIP_RIDER_NUM];
	unsigned short	m_wObjectType;
	unsigned char	m_cState;
	unsigned char	m_cSubState;
	unsigned char	m_cUpgradeStep;
	unsigned char	m_cUpgradeType;
	float			m_fDefaultDir;
	CNetworkPos		m_NetworkPos;
};

typedef struct PktSiegeBroadCast2nd* LPPktSiegeBroadCast2nd;
struct PktSiegeBroadCast2nd : public PktBase
{
// CASTLE_TODO : 사용하지 않는 옛날 공성 오브젝트 브로드 캐스트 패킷
};

typedef struct PktNewSiegeBroadCast* LPPktNewSiegeBroadCast;
struct PktNewSiegeBroadCast : public PktBase
{
	unsigned long	m_dwCID;
	unsigned char	m_cType;
};

// 게임 시간 패킷
struct TimeInfo
{
	unsigned char	m_cMonth;
	unsigned char	m_cDay;
	unsigned char	m_cHour;
	unsigned char	m_cMinute;
	unsigned long	m_dwTime;	// DBAgentServer -> GameServer	: Duration Minutes
								// GameServer -> Client			: Remain Seconds
};

struct GuildTimeInfo
{
	bool			m_bUpdated;
	unsigned char	m_cNowGuildTime;
	TimeInfo		m_TimeInfo;
};

struct RealmTimeInfo
{
	bool			m_bUpdated;
	unsigned char	m_cNowRealmTime;
	TimeInfo		m_TimeInfo;
};

struct SiegeTimeInfo
{
	bool			m_bUpdated;
	unsigned char	m_cNowSiegeTime;
	TimeInfo		m_TimeInfo;
};

struct EnterTimeInfo
{
	bool			m_bUpdated;
	unsigned char	m_cNowEnterTime;
	TimeInfo		m_TimeInfo;
};

struct EventTimeInfo
{
	bool			m_bUpdated;
	unsigned char	m_cNowEventTime;
	TimeInfo		m_TimeInfo;
};

struct CastleSiegeInfo
{
	unsigned long	m_dwCastleID;
	bool			m_bEnableSiege;				// 현재 공성이 가능한가?
};

// 가변정보 : + CastleSiegeInfo * SiegeTimeInfo::m_cCastleNum
typedef struct PktGameTimeInfo* LPPktGameTimeInfo;
struct PktGameTimeInfo : public PktBase
{
	unsigned char	m_cType;
	unsigned char	m_cCastleNum;
	unsigned short	m_wSize;

	GuildTimeInfo	m_GuildTimeInfo;
	RealmTimeInfo	m_RealmTimeInfo;
	SiegeTimeInfo	m_SiegeTimeInfo;
	EnterTimeInfo	m_EnterTimeInfo;
	EventTimeInfo	m_EventTimeInfo;
};

// 스텔스 사용 가능 여부 패킷
typedef struct PktStealthInfo* LPPktStealthInfo;
struct PktStealthInfo : public PktBase
{
	bool	m_bUseStealth;			// 스텔스를 사용할수 있는가?
};


// 성 정보 업데이트 ( DBAgentServer --> GameServer --> Client )
typedef struct PktCastleUpdate* LPPktCastleUpdate;
struct PktCastleUpdate : public PktBase
{
	unsigned long	m_dwCastleID;							// CastleID
	unsigned long	m_dwGID;								// 소유 Guild ID
	unsigned short	m_wTax;									// 세율
	unsigned long	m_dwTaxMoney;							// 세금 수입금
	unsigned char	m_cInvincibleDay;						// 남은 무적 횟수
	unsigned char	m_cTaxMoneyRemainDay;					// 수입금을 받기까지 남은 날 (GameServer -> Client)

	CastleRight		m_CastleRight;							// 길드원들의 성 관리 권한
//	char			m_szCastleName[MAX_CASTLE_NAME_LEN];	// 성 이름
};

// 길드 요새 관련 메세지 패킷
typedef struct PktCampMessage* LPPktCampMessage;
struct PktCampMessage : public PktBase
{
	enum { MAX_NAME_LEN	= 16 };

	enum
	{
		MSGCMD_ENCAMP_START					= 0,	// 길드 요새 구축 시작
		MSGCMD_ENCAMPING,							// 길드 요새 구축 중
		MSGCMD_ENCAMP_END,							// 길드 요새 구축 완료
		
		MSGCMD_CANCEL_START,						// 길드 요새 구축 취소
		MSGCMD_CANCEL_END,							// 길드 요새 구축 취소 완료
		
		MSGCMD_REPAIR_START,						// 길드 요새 수리
		MSGCMD_REPAIR_END,							// 길드 요새 수리 완료
		
		MSGCMD_UPGRADE_START,						// 길드 요새 업그레이드
		MSGCMD_UPGRADE_END,							// 길드 요새 업그레이드 완료

		MSGCMD_SET_RIGHT,							// 길드 요새 관리 권한 변경

		MSGCMD_SELF_DESTROY_START,					// 길드 요새 스스로 파괴 시작
		MSGCMD_SELF_DESTROY_END,					// 길드 요새 스스로 파괴 완료

		MSGCMD_CHANGE_TYPE_START,					// 길드 요새 타입 변형 시작
		MSGCMD_CHANGE_TYPE_END,						// 길드 요새 타입 변형 완료

		MSGCMD_ATTACKED,							// 길드 요새 공격 당함
		MSGCMD_DESTROYED,							// 길드 요새 파괴됨 (파괴 당함)
		MSGCMD_DESTROY,								// 길드 요새 파괴함 (파괴 함)

		MSGCMD_MINING_END,							// 채굴기 채굴 완료

		MSGCMD_SHOP_GAIN_TAX,						// 길드 요새 상점 세금 수익금 획득

		MSGCMD_WEAPON_ENCAMP_START,					// 월드 웨폰 구축 시작
		MSGCMD_WEAPON_ENCAMPING,					// 월드 웨폰 구축 중
        MSGCMD_WEAPON_ENCAMP_END,					// 월드 웨폰 구축 완료

		MSGCMD_WEAPON_CANCEL_START,					// 월드 웨폰 구축 취소
		MSGCMD_WEAPON_CANCEL_END,					// 월드 웨폰 구축 취소 완료

		MSGCMD_WEAPON_REPAIR_START,					// 월드 웨폰 수리
		MSGCMD_WEAPON_REPAIR_END,					// 월드 웨폰 수리 완료

		MSGCMD_WEAPON_UPGRADE_START,				// 월드 웨폰 업그레이드
		MSGCMD_WEAPON_UPGRADE_END,					// 월드 웨폰 업그레이드 완료

		MSGCMD_WEAPON_SET_RIGHT,					// 월드 웨폰 관리 권한 변경
		
		MSGCMD_WEAPON_SELF_DESTROY_START,			// 월드 웨폰 스스로 파괴 시작
		MSGCMD_WEAPON_SELF_DESTROY_END,				// 월드 웨폰 스스로 파괴 완료

		MSGCMD_WEAPON_CHARGE_START,					// 월드 웨폰 무기 장전 시작
		MSGCMD_WEAPON_CHARGE_END,					// 월드 웨폰 무기 장전 완료

		MSGCMD_WEAPON_ATTACKED,						// 월드 웨폰 공격 당함
		MSGCMD_WEAPON_DESTROYED,					// 월드 웨폰 파괴됨

		MSGCMD_WEAPON_FIRE_WARNING,					// 월드 웨폰 발사 경고

		MSGCMD_TAKE_IN_MATERIAL,					// 길드 요새에 자재 넣기
		MSGCMD_TAKE_OUT_MATERIAL,					// 길드 요새에서 자재 빼기

		MSGCMD_GUILD_TIME_COMPLETE,					// 길드 재정비 시간 완료.

		MSGCMD_MAX_CMD
	};

	struct sRight
	{
		unsigned char m_cCampControl;
	};

	char			m_szMasterName[MAX_NAME_LEN];
	char			m_szPCName[MAX_NAME_LEN];
	char			m_szGuildName[Guild::MAX_GUILD_NAME_LEN];
	unsigned long	m_dwCampID;
	unsigned long	m_dwGID;
	unsigned char	m_cZone;
	unsigned char	m_cMsgCmd;
	unsigned char	m_cRemainTime;
	unsigned char	m_cNation;
	sRight			m_Right;
	bool			m_bNotify;
	int				m_nValue1;	// 명성치, 변형된 타입, 월드 웨폰 발사 X좌표, 월드웨폰을 파괴당한 국가, 공성 자재 이동 갯수
	int				m_nValue2;	// 기존 타입, 월드 웨폰 발사 Z좌표, 공성 자재 최종 갯수

	unsigned char	m_cTimeChk;		// 최대 256 분.
};

struct PktWarOnOff : public PktBase
{
	enum Error
	{
		E_WAR_OFF		= 2,	// 전쟁 플래그를 Off 시킬수 없음
		E_WAR_ON		= 3,	// 전쟁 플래그를 On 시킬수 없음
		E_WAR_INSTANCE	= 4,	// 전쟁 플래그를 Instance 시킬수 없음
		E_INVALID_TYPE	= 5		// 존재하지 않는 전쟁 타입
	};

	unsigned long	m_dwCID;	
	unsigned char	m_cType;	// GameTime::eTimeType
	unsigned char	m_cFlag;	// Creature::WarFlag
};

// 국가전쟁 공헌훈장 포인트.
struct PktRealmPoint : public PktBase
{
	enum
	{
		REALM_JOIN		= 1,				// 국가 전쟁에 참여 했을 경우.
		REALM_OUT		= 2					// 국가 전쟁에 미참여 했을 경우.
	};

	unsigned char	m_cRealmPoint;			// 공헌훈장 포인트.
	unsigned long	m_dwCID;				// 캐릭터 CID.
	unsigned char	m_cType;				// 참여, 미참여.
};

#pragma pack()

#endif	// _CASTLE_PACKET_H_
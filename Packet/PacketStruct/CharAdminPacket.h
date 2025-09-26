#ifndef _CHAR_ADMIN_PACKET_H_
#define _CHAR_ADMIN_PACKET_H_

// CharAdminPacket.h
#include <winsock2.h>
#include <windows.h>
#include <Network/Packet/PacketBase.h>
#include <DB/DBdefine.h>
#include <Item/ItemStructure.h>

#pragma pack(1)

typedef struct PktAdmin* LPPktAdmin;
struct PktAdmin : public PktBase 
{
	enum {	MAX_NAME_LEN = 16 };
	enum CMD 
	{	
		NONE					= 0,	// 운영자 명령

		MOVE_ZONE_CHAR			= 1,	// 캐릭터 존 이동
		MOVE_ZONE_PARTY			= 2,	// 파티 존 이동
		MOVE_CHAR				= 3,	// 캐릭터 이동
		MOVE_PARTY				= 4,	// 파티 이동

		RESPAWN_CHAR			= 5,	// 캐릭터 리스폰
		RESPAWN_PARTY			= 6,	// 파티 리스폰
		KILL_CHAR				= 7,	// 캐릭터 킬
		KILL_PARTY				= 8,	// 파티 킬

		DUEL_INIT				= 9,	// 듀얼 초기화

		CREATE_ITEM				= 10,	// 아이템 드롭
		CREATE_MON				= 11,	// 몬스터 생성

		INFO_CHAR				= 12,	// 캐릭터 정보

		GET_EXP					= 13,	// 경험치 얻기
		GET_GOLD				= 14,	// 돈 얻기
		LEVEL_UP				= 15,	// 레벨업

		PEACE_MODE				= 16,	// 반전단체 가입
		WAR_MODE				= 17,	// 반전단체 탈퇴

		GET_FAME				= 18,	// 명성 얻기
		GET_MILEAGE				= 19,	// 마일리지 얻기

		TRACE_CHAR				= 20,	// 캐릭터 추적
		HIDE_CHAR				= 21,	// 캐릭터 투명
		SHOW_CHAR				= 22,	// 캐릭터 투명 해제
		INVINCIBLE_CHAR			= 23,	// 캐릭터 무적
		NORMAL_CHAR				= 24,	// 캐릭터 무적 해제
		LOGOUT_CHAR				= 25,	// 캐릭터 강제 로그아웃

		APOCALYPSE				= 26,	// 주위 생물체 모두 킬

		CREATE_EQUIP			= 27,	// 랜덤 옵션이 붙는 장비 드롭

		ELITE_BONUS				= 28,	// 엘리트 보너스 조절

		STATUE_INIT				= 29,	// 명예의 석상 초기화
		RESPAWN_ALL				= 30,	// 모든 캐릭터 리스폰

		CHANGE_CLASS			= 31,	// 전직 하기
		LEARN_SKILL				= 32,	// 해당 스킬 배우기
		ERASE_SKILL				= 33,	// 해당 스킬 지우기
		LEVEL_DOWN				= 34,	// 레벨 다운

		GET_QUEST				= 35,	// 퀘스트 받음
		ADD_QUEST				= 36,	// 완료 퀘스트에 추가
		DELETE_QUEST			= 37,	// 수행/완료 퀘스트를 삭제

										// BATTLEGROUND_SERVER 운영자 명령
		FINISH_GAME				= 38,	// 해당 방의 경기 종료 (승패 결정)
		DRAW_GAME				= 39,	// 해당 방의 경기 종료 (무승부)
		IGNORE_GAME				= 40,	// 해당 방의 경기 종료 (승패 무시)
		SET_MAX					= 41,	// 해당 방의 최대 인원수 설정
		SET_SCORE				= 42,	// 해당 방의 목표 점수 설정
		SET_LIMIT_TIME			= 43,	// 해당 방의 시간 제한 설정
		SET_REST_TIME			= 44,	// 해당 방의 쉬는 시간 설정

		SET_ITEM_TEMP_INVEN		= 45,	// 임시 인벤토리에 아이템 넣기

		ADD_SPELL				= 46,	// 해당 스펠을 부여
		DELETE_SPELL			= 47,	// 부여된 해당 스펠을 파괴

		STOP					= 48,	// 해당 캐릭터 묶어두기
		STOP_OFF				= 49,	// 묶어둔 해당 캐릭터를 풀어주기

		CHAT_BAN				= 50,	// 채팅 금지
		CHAT_OK					= 51,	// 채금 해제

		NEO						= 52,	// 네오
		SMITH					= 53,	// 스미스

		RELATION				= 55,	// 적아군 식별.

		GM_FORM					= 56,	// PC 에서 GM 으로.
		PC_FORM					= 57,	// GM 에서 PC 로.

		REALM_POINT				= 58,	// 국가 전쟁 공헌 훈장.
		DBAGENT_KILL			= 59,	// 중계 서버 강제 종료

		REQUEST_TRACE_POS		= 100	// DBAgent 가 게임 서버에게 추적할 캐릭터의 위치를 요청하는 명령
										// 해당 게임 서버가 DBAgent 에게 추적한 캐릭터의 좌표를 넘겨줄때의 명령
	};

	enum eError
	{
		E_LOGOUT_CHAR			= 2,	// 로그아웃한 캐릭터 입니다.
	};
	
	unsigned short	m_usCmd;					// 운영자 명령

	char			m_stName[MAX_NAME_LEN];		// 이름

	union
	{
		struct ZoneInfo
		{
			char		m_cZone;				// 존 번호
			char		m_cChannel;				// 채널 번호
		} m_ZoneInfo;

		struct EliteBonusInfo
		{
			char		m_cNation;				// 국가
			char		m_cLevel;				// 레벨
		} m_EliteBonusInfo;

		unsigned short	m_usProtoTypeID;		// 타입 아이디
	};
	
	POS				m_Position;					// 위치
	
	unsigned long	m_dwAmount;					// 양
	unsigned long	m_dwAdminCID;				// Action 의 주체가 되는 Admin 캐릭터의 CID
	unsigned long	m_dwAmount2;				// 양2
	unsigned long	m_dwAmount3;				// 양3
};


// 어드민 툴에 데이터 업데이트하기.
struct PktAdminToolGetData : public PktBase
{
    enum GetDataType
    {
        // DB중계 -> 운영서버
        CHAR_BASIC_DATA		= 10,			// 캐릭터 정보. 다음 정보들이 들어감. 

        // CHAR_INFOST	캐릭터 기본 정보
        // CHAR_POS		캐릭터 위치 정보
        // SKILL		캐릭터 스킬 정보
        // QUICK		캐릭터 퀵 슬롯
        // EQUIP		캐릭터 장비
        // INVEN		캐릭터 인벤
        // EXTRA		캐릭터 여분
        // EXCHANGE		캐릭터 교환

        CHAR_EXTRA_DATA		= 11,			// 캐릭터 추가 정보들. 다음 정보들이 들어감.

        // CHAR_INFOEX	캐릭터 추가 정보
        // QUEST		퀘스트
        // HISTORY		히스토리	
        // CONFIG		설정
        // STORE_INFO	창고 정보

        CHAR_FRIEND_DATA	= 12,	// 캐릭터 친구 정보
        CHAR_BAN_DATA		= 13,	// 캐릭터 거부 정보

        STORE_12_DATA		= 14,	// 창고 1, 2탭 데이터
        STORE_34_DATA		= 15,	// 창고 3, 4탭 데이터

        FINISH_GET_DATA		= 19,	// DB중계에서 운영서버로 데이터를 전부 보냈음.

        // 운영서버 -> DB중계
        GET_CHAR_DATA		= 20,	// 캐릭터 데이터 요청 패킷

        CHANGE_CHAR_NAME    = 30    // 캐릭터 이름 바꾸기
    };

    enum GetError
    {
        UNKNOWN_ERROR				= 1,	// 알 수 없는 에러
        INVALID_REQUEST				= 2,	// 잘못된 데이터 패킷

        GET_UIDCID_QUERY_FAILED		= 3,	// DB에서 이름으로 UID, CID를 얻어오는 데 실패했다.
        INVALID_UID_CID_DATA		= 4,	// DB에서 이름으로 얻어온 UID, CID데이터가 이상하다.

        GET_STORE_QUERY_FAILED		= 5,	// DB에서 창고 데이터를 얻어오는 데 실패했다.
        GET_CHAR_DATA_QUERY_FAILED	= 6		// DB에서 캐릭터 데이터를 얻어오는 데 실패했다.
    };

    char            m_szName[CHAR_INFOST::MAX_NAME_LEN];
    unsigned long   m_dwRequestKey;
	unsigned long   m_dwUID;
    unsigned long   m_dwCID;
    unsigned char   m_cType;    
	unsigned char	m_cOldServerGroupID;
};

struct PktAdminToolGetDataAck : public PktBase
{
    unsigned long   m_dwRequestKey;
    unsigned long   m_dwUID;
    unsigned long   m_dwCID;
    unsigned char   m_cType;
	unsigned char	m_cOldServerGroupID;
};


// 어드민 툴에서 데이터 업데이트하기. (뒤에 데이터를 넣음)
struct PktAdminToolSetData : public PktBase
{
    enum SetDataType
    {
        CHAR_BASIC_DATA		= 10,			// 캐릭터 정보. 다음 정보들이 들어감. 

        // CHAR_INFOST	캐릭터 기본 정보
        // CHAR_POS		캐릭터 위치 정보
        // SKILL		캐릭터 스킬 정보
        // QUICK		캐릭터 퀵 슬롯
        // EQUIP		캐릭터 장비
        // INVEN		캐릭터 인벤
        // EXTRA		캐릭터 여분
        // EXCHANGE		캐릭터 교환

        CHAR_EXTRA_DATA		= 11,			// 캐릭터 추가 정보들. 다음 정보들이 들어감.

        // CHAR_INFOEX	캐릭터 추가 정보
        // QUEST		퀘스트
        // HISTORY		히스토리	
        // CONFIG		설정
        // STORE_INFO	창고 정보

        CHAR_FRIEND_DATA	= 12,	// 캐릭터 친구 정보
        CHAR_BAN_DATA		= 13,	// 캐릭터 거부 정보

        STORE_12_DATA		= 14,	// 창고 1, 2탭 데이터
        STORE_34_DATA		= 15,	// 창고 3, 4탭 데이터

        OPEN_UPDATE_DATA	= 20,	// 세션을 열고 DB에 업데이트하려 시도한다.
        FINISH_UPDATE_DATA	= 21,	// 데이터를 DB에 업데이트하고 세션을 닫는다.		

        RELOAD_ADMINLIST    = 30
    };

    enum SetError
    {
        UNKNOWN_ERROR				= 1,	// 알 수 없는 에러.
        INVALID_UID_CID_ERROR		= 2,	// 알 수 없는 UID/CID
        LOGIN_ANOTHER_GAMESERVER	= 3,	// 다른 게임 서버에 로그인 해 있어서 업데이트 불가.

        SESSION_OPEN_FAILED					= 4,	// 세션 열기 실패
        ALREADY_LOGIN_USER_SESSION_OPEN		= 5,	// 이미 로그인 되어 있는 유저 세션을 열려고 했음
        ANOTHER_CHAR_LOAD_FAILED			= 6,	// 캐릭터 정보를 DB에서 읽어오는 데 실패
        SETTING_DATA_FAILED					= 7,	// 데이터 세팅 실패 (데이터 길이등이 맞지 않음)
        SESSION_START_FAILED				= 8		// 세션 시작 실패.
    };

    unsigned __int64    m_dwSerial;
    unsigned long       m_dwRequestKey;
    unsigned long       m_dwUID;
    unsigned long       m_dwCID;
    unsigned char       m_cType;
	unsigned char		m_cOldServerGroupID;
};

struct PktAdminToolSetDataAck : public PktBase
{
    unsigned long       m_dwRequestKey;
    unsigned char       m_cType;
};


struct PktNewAdminToolSetDataAck : public PktBase
{
    unsigned long   m_dwRequestKey;
    unsigned long   m_dwUID;
    unsigned long   m_dwCID;
    unsigned char   m_cType;
};


struct GiveItemInfo
{
    enum GiveItemType
    {
        MIN_DAMAGE      = 0,
        MAX_DAMAGE      = 1,
        ARMOR           = 2,
        HIT_RATE        = 3,
        EVADE           = 4,
        MAX_HP          = 5,
        HP_REGEN        = 6,
        MAX_MP          = 7,
        MP_REGEN        = 8,
        CRITICAL        = 9,
        BLOCK           = 10,
        SPEED           = 11,
        MAGIC_POWER     = 12,
        MAGIC_RESIST    = 13,
        MAX_GIVEITEM_ATTRIBUTE = 14
    };

    unsigned long       m_dwCreateID;
    unsigned long       m_dwUID;
    unsigned long       m_dwCID;
    unsigned short      m_usPrototypeID;
    unsigned char       m_cAmount;
    unsigned char       m_cAttribute[MAX_GIVEITEM_ATTRIBUTE];
    unsigned __int64    m_dwItemUID;
};

struct UpdateItemUID
{
    unsigned __int64    m_dwItemUID;
    unsigned long       m_dwCreateID;
};

struct PktGiveItemToTempInven : public PktBase
{
    enum Const
    {
        MAX_GIVE_ITEM = 10
    };

    GiveItemInfo        m_GiveItemInfo[MAX_GIVE_ITEM];
    unsigned char       m_cGiveItemNum;
};

struct PktCharNameChange : public PktBase
{
    enum Error
    {
        ERR_SERVER                  = 1,    // 서버 에러
        ERR_CANNOT_FIND_CHARACTER   = 2,    // 캐릭터 찾을 수 없음(로그인된 캐릭터가 아님)
        ERR_NAMECHANGE_ITEM         = 3,    // 이름 변경 
        ERR_INVALID_NAME            = 4,    // 만들 수 없는 이름(특수문자등이 들어간 이상한 이름)
        ERR_CANNOT_USE_NAME         = 5,    // 사용할 수 없는 이름
        ERR_ALREADY_USE_NAME        = 6     // 이미 사용하고 있는 이름
       
    };

    char                m_szCharName[CHAR_INFOST::MAX_NAME_LEN];    // 바꿀 이름
    unsigned long       m_dwUID;                                    // 자기 캐릭터 UID
    unsigned long       m_dwCID;                                    // 자기 캐릭터 CID    
    unsigned char       m_cNameChangeCount;                         // 갱신된 이름 변경 카운트 숫자.
	Item::ItemPos		m_ItemPos;		// 계명허가서 (추가작업됨)
};

struct PktAdminCommandLog : public PktBase 
{
	enum Const
	{
		MAX_COMMAND_SIZE	= 128
	};

	unsigned long		m_dwAdminCID;					// 해당 GM UID
	unsigned long		m_dwCID;						// 해당 캐릭터 CID.
	char				m_szCommand[MAX_COMMAND_SIZE];	// 해당 명령어 텍스트.
};

#pragma pack()

#endif
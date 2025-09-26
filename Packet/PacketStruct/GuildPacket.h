#ifndef _GUILD_PACKET_H_
#define _GUILD_PACKET_H_

#include <Network/Packet/PacketBase.h>
#include <Creature/Character/CharacterClass.h>
#include <Creature/CreatureStructure.h>

#include <Community/Guild/GuildConstants.h>
#include <Community/Guild/GuildStructure.h>

#pragma pack(1)

using namespace Guild;

// 길드 생성
struct PktCreateGuild : public PktBase
{
	enum PktCreateGuildErr
	{
		FAIL_ALREADY_NAME	= 2,	// 이미 존재하는 이름
		FAIL_DISSOLVE_GUILD	= 3,    // 길드 해체
        FAIL_INVALID_NAME   = 4     // 부적절한 이름
	};

    enum DissolveReason
    {
        NONE_LOGIN                                      = 1,    // 오랫동안 아무도 로그인하지 않음
        MIN_MEMBER_TIMEOUT                              = 2,    // 길드원이 최소 인원 미만인 채 일정 시간 경과
        NONE_NEXT_GUILDMASTER_BY_NONE_LOGIN             = 3,    // 일정기간 길드마스터가 로그인하지 않아, 권한 이양하려 할 시에 길마가 없을 경우
        NONE_NEXT_GUILDMASTER_BY_GUILDMASTER_OUT        = 4,    // 길드마스터 탈퇴시에 권한 이양하려 할 시에 길마가 없을 경우
        NONE_NEXT_GUILDMASTER_BY_GUILDMASTER_DELETED    = 5     // 길드마스터 삭제시에 권한 이양하려 할 시에 길마가 없을 경우
    };

	unsigned long	m_dwCID;
	unsigned long	m_dwGID;
	unsigned char	m_cInclination;
	char			m_szGuildName[MAX_GUILD_NAME_LEN];
};

// 길드 리스트 (가변 길이) + 길드 정보 
struct PktGuildList : public PktBase
{
	enum { MAX_NUM_PER_PAGE = 10 };

	unsigned long	m_dwCID;			// 캐릭터 아이디

	unsigned char	m_cSortCmd;			// 명령
	unsigned char	m_cPage;			// 페이지

	unsigned char	m_cSmallNodeNum;	// 노드 수 (C -> S : 체크섬 노드, S -> C : 간략 노드)
	unsigned char	m_cLargeNodeNum;	// 노드 수 (전체 정보 노드)
};

// 길드 정보 (체크섬)
struct GuildCheckSumNode
{
	unsigned long	m_dwGID;			// 길드 아이디
	unsigned long	m_dwCheckSum;		// 체크섬
};

// 길드 정보 (종종 변하지만 간혹 요청되는 자잘한 것들)
struct GuildSmallInfoNode
{
	// TODO : 국가, 레벨, 길드 이름은 GuildLargeInfoNode로 빼면 될 듯 합니다.
	//	(국가와 길드 이름은 변경되지 않습니다. 레벨은 마크나 관계처럼 변경시마다 브로드캐스팅하면 됩니다.)
	//	더불어 길드 생성시 생성되는 길드의 GuildLargeInfoNode를 브로드캐스팅해야 합니다.
	//	(이 때 주의할 점은 길드 생성 패킷 전송 당시엔 마스터가 가입하지 않은 상태이므로 마스터 이름을 별도로 넣어줘야 합니다.)

	unsigned long	m_dwGID;				// 길드 아이디
	unsigned char	m_cIndex;				// 리스트상의 인덱스
	unsigned char	m_cInclination;			// 길드 성향

	unsigned short	m_wRank;				// 순위
	unsigned long	m_dwFame;				// 명성
	unsigned char	m_cLevel;				// 레벨
	unsigned char	m_cCurrentMemberNum;	// 인원수

	char	m_szName[MAX_GUILD_NAME_LEN];			// 길드 이름
	char	m_szMasterName[MAX_MEMBER_NAME_LEN];	// 마스터 이름

	GuildSmallInfoNode()
		:	m_dwGID(0), m_cIndex(0), m_cInclination(0), m_wRank(0), m_dwFame(0), m_cLevel(0), m_cCurrentMemberNum(0)
	{
		std::fill_n(m_szName, int(MAX_GUILD_NAME_LEN), 0);
		std::fill_n(m_szMasterName, int(MAX_MEMBER_NAME_LEN), 0);
	}

	GuildSmallInfoNode(unsigned long dwGID, unsigned char cIndex, unsigned char cInclination, 
		unsigned short wRank, unsigned long dwFame, unsigned char cLevel, 
		unsigned char cCurrentMemberNum, const char* szGuildName, const char* szMasterName)
		:	m_dwGID(dwGID), m_cIndex(cIndex), m_cInclination(cInclination), 
			m_wRank(wRank), m_dwFame(dwFame), m_cLevel(cLevel), m_cCurrentMemberNum(cCurrentMemberNum)
	{
		if (NULL != szGuildName)
		{
			strncpy(m_szName, szGuildName, MAX_GUILD_NAME_LEN);
		}

		if (NULL != szMasterName)
		{
			strncpy(m_szMasterName, szMasterName, MAX_MEMBER_NAME_LEN);
		}
	}
};

// 길드 정보 (모두)
struct GuildLargeInfoNode : public GuildSmallInfoNode
{
	char			m_szMark[MAX_MARK_SIZE];		// 마크
	unsigned char	m_cRelationByMine;				// 클라이언트(유저)가 이 길드에 맺은 길드 관계
	unsigned char	m_cRelationByTarget;			// 이 길드가 클라이언트(유저)에게 맺은 길드 관계

	GuildLargeInfoNode()
		:	GuildSmallInfoNode(), m_cRelationByMine(0), m_cRelationByTarget(0)
	{
		std::fill_n(m_szMark, int(MAX_MARK_SIZE), 0);
	}

	GuildLargeInfoNode(unsigned long dwGID, unsigned char cIndex, unsigned char cInclination, 
		unsigned short wRank, unsigned long dwFame, unsigned char cLevel, 
		unsigned char cCurrentMemberNum, const char* szMasterName, const char* szGuildName, 
		const char* szMark, unsigned char cRelationByMine, unsigned char cRelationByTarget)
		:	GuildSmallInfoNode(dwGID, cIndex, cInclination, wRank, dwFame, cLevel, cCurrentMemberNum, szGuildName, szMasterName), 
			m_cRelationByMine(cRelationByMine), m_cRelationByTarget(cRelationByTarget)
	{
		if (NULL != szMark)
		{
			memcpy(m_szMark, szMark, MAX_MARK_SIZE);
		}
	}
};

// 길드 멤버 관련 명령
struct PktGuildCmd : public PktBase
{
	enum GuildCmd
	{ 
		GC_INVITE			= 0,			// 초대
		GC_JOIN				= 1,			// 가입 (초대에 의한)
		GC_REFUSE			= 2,			// 거절 (초대에 대한)
		GC_ASK_ENTER		= 3,			// 가입 신청
		GC_ASK_LEAVE		= 4,			// 탈퇴 신청
		GC_KICK				= 5,			// 강제 탈퇴
		GC_SETUP_TITLE		= 6,			// 직위 변경
		GC_LOGINOUT			= 7,			// 로그인/아웃
		GC_RANK_UP			= 8,			// 서열 위로
		GC_RANK_DOWN		= 9,			// 서열 아래로
		GC_TACTICS			= 10,			// 용병 가입 신청.
		GC_TACTICS_JOIN		= 11,			// 용병 가입 초대.
		GC_TACTICS_TITLE	= 12,			// 용병 허가.
		GC_TACTICS_KICK		= 13,			// 용병 삭제.
		GC_TACTICS_LEAVE	= 14,			// 용병 탈퇴.
		GC_TACTICS_REQUEST	= 15,			// 길드 전쟁 중 용병 허가 시 캐릭터 확인 요청.
	};

	enum PktGuildCmdErr
	{
		FAIL_NOT_MEMBER			= 2,	// 길드원이 아님
		FAIL_NOT_RIGHT			= 3,	// 권한이 없음
		FAIL_ALREADY_MEMBER		= 4,	// 이미 길드원임
		FAIL_OVER_MEMBER		= 5,	// 길드 정원 초과
		FAIL_NATION				= 6,	// 다른 국가의 길드

		FAIL_DIFFERENCE_TITLE	= 7,	// 다른 직위로의 서열 변경
		FAIL_OVER_MEMBER_NUM	= 8,	// 길드원 수 범위를 초과하는 서열 변경

		FAIL_GUILDWAR_TIME_KICK	= 9,	// 길드전쟁 시간에 탈퇴를 시도하는 경우

		FAIL_TACTICS_LIMITLEVEL			= 10,	// 용병 신청 레벨이 안되는 캐릭터.
		FAIL_TACTICS_NATION				= 11,	// 국적이 다른 캐릭터.
		FAIL_TACTICS_GUILDJOIN			= 12,	// 길드나 용병에 가입되어있거나 신청중인 캐릭터.
		FAIL_TACTICS_OVER_MEMBER		= 13,	// 길드 정원 초과.

		FAIL_TACTICS_LOGOUT				= 14	// 용병 허가 시 로그아웃 상태일때.
	};

	unsigned long	m_dwGID;			// 길드 아이디

	unsigned long	m_dwSenderID;	    // 캐릭터 아이디 (주체)
	unsigned long	m_dwReferenceID;	// 참조 아이디 (대상 캐릭터/서버 아이디/권한 비트)

	char			m_szGuildName[MAX_GUILD_NAME_LEN];		// 길드 이름
	char			m_szSenderName[MAX_MEMBER_NAME_LEN];	// 캐릭터 이름 (주체)

	unsigned short	m_wCmd;             // 명령
};

// 멤버 리스트 (가변 길이) + 멤버 정보 
struct PktGuildMemberList : public PktBase
{
	enum { MAX_NUM_PER_PAGE = 10 };

	unsigned long	m_dwCID;			// 캐릭터 아이디

	unsigned char	m_cMemberType;		// 멤버리스트, 용병리스트
	unsigned char	m_cSortCmd;			// 명령 (Guild::MemberSort)
	unsigned char	m_cPage;			// 페이지

	unsigned char	m_cTotalMemberNum;	// 총 멤버 수
	unsigned char	m_cNodeNum;			// 노드 수 (멤버 노드)
};

// 멤버 정보
struct GuildMemberInfoNode
{
	unsigned long	m_dwCID;

	char			m_szName[MAX_MEMBER_NAME_LEN];
	unsigned char	m_cTactics;

	unsigned long	m_dwFame;
	unsigned long	m_dwGold;
	unsigned long	m_dwServerID;

	unsigned char	m_cRank;
    unsigned char	m_cTitle;
	unsigned char	m_cLevel;
	unsigned char	m_cClass;
	unsigned char	m_cGuildWarFlag;
};

// 멤버 정보 업데이트 (Game <-> DBAgent)
struct PktGuildMemberInfoUpdate : public PktBase
{
	unsigned long		m_dwGID;
	unsigned long		m_dwCID;

	MemberListInfo		m_MemberListInfo;
	MemberDetailInfo	m_MemberDetailInfo;
};

// 멤버 명성 업데이트
struct PktGuildMemberFameUpdate : public PktBase
{
	unsigned long		m_dwOurGID;
	unsigned long		m_dwEnemyGID;
	unsigned long		m_dwCID;
	unsigned char		m_cType;
	int					m_nAddFame;
};

// 멤버 돈 업데이트
struct PktGuildMemberGoldUpdate : public PktBase
{
	unsigned long		m_dwGID;
	unsigned long		m_dwCID;
	unsigned long		m_dwGold;
};

// 길드 마크
struct PktGuildMark : public PktBase
{
	enum eError
	{
		FAIL_INVALID_GUILD		= 2,		// 존재하지않는 길드
		FAIL_INVALID_MARK		= 3,		// 변경할 마크가 없음
		FAIL_INVALID_CHARACTER	= 4,		// 존재하지않는 캐릭터
		FAIL_NOT_MASTER			= 5,		// 마스터가 아님
		FAIL_NOT_ENOUGH_GOLD	= 6,		// 금액 부족
	};

	unsigned long	m_dwCID;					// 캐릭터 아이디
	unsigned long	m_dwGID;					// 길드 아이디

	char			m_szMark[MAX_MARK_SIZE];	// 마크
	unsigned long	m_dwGold;					// 금고 잔액
};

// 길드 레벨
struct PktGuildLevel : public PktBase
{
	unsigned long	m_dwUID;			// 아이디 (C -> S : 캐릭터 아이디, S -> C : 길드 아이디)
	unsigned char	m_cLevel;			// 레벨
	unsigned long	m_dwGold;			// 금고 잔액
};

// 길드 관계
struct PktGuildRelation : public PktBase
{
	enum eSubCmd
	{
		GR_HOSTILITY			= 0,	// 적대 선언
		GR_PEACE				= 1,	// 평화 선언
		GR_ALERT				= 2,	// 경계 적대 선언
		GR_COUNTER				= 3,	// 카운터 적대 선언
		GR_REQUEST				= 4,	// 이미 적대인 길드와 평화 선언후 대상 길드와 적대 선언
										// 카운터 적대 선언을 할지 물어보는 대화창
		GR_HOSTILITYDEL			= 5		// A->B로 적대선언을 한후 A->B에게 평화선언을 해도 B의 Target에 존재하는
										// 적대선언 리스트는 사라지지 않는다. B가 직접 5번 커맨드로 취소해줘야만 
										// 취소가 된다.
	};

	enum eError
	{
		FAIL_NOT_MEMBER					= 2,	// 해당 길드의 멤버가 아님
		FAIL_NOT_HOSTILITY_TIME			= 3,	// 적대 선언을 할수 없는 시간
		FAIL_NOT_ALERT_TIME				= 4,	// 경계 적대 선언을 할수 없는 시간
		FAIL_NOT_COUNTER_TIME			= 5,	// 카운터 적대 선언을 할수 없는 시간
		FAIL_NOT_MASTER					= 6,	// 길드의 마스터가 아님
        FAIL_NOT_EXIST_GUILD			= 7,	// 길드 ID가 0 이거나 길드가 존재하지 않음
		FAIL_SELF_GUILD					= 8,	// 자신의 길드에게 적대/평화/경계/카운터 선언
		FAIL_ALREADY_HOSTILITY			= 9,	// 대상 길드와 이미 적대 관계입니다.
		FAIL_ALREADY_COUNTER_HOSTILITY	= 10,	// 대상 길드와 이미 카운터 적대 관계입니다.
		FAIL_ALREADY_ALERT_HOSTILITY	= 11,	// 데상 길드와 이미 경계 적대 관계입니다.
		FAIL_ALREADY_NEUTRALITY			= 12,	// 대상 길드와 이미 중립 관계입니다.
		FAIL_MAX_ALERT_NUM				= 13,	// 경계 적대 선언권 초과
		FAIL_NOT_HAVE_DEVELOPING_CAMP	= 14,	// 구축중인 길드 요새가 없음
		FAIL_NOT_HAVE_ALERT				= 15,	// 경계 적대 신청을 받지 않고 카운터 적대 신청을 하려함
		FAIL_TARGET_HOSTILITY_FULL		= 16,	// 적대 선언 받을 수 있는 한계치 초과

		FAIL_REQUEST_WINDOW				= 100,	// 적대 선언수 초과로 인해 평화 선언후 적대 선언을 하도록 인증
		FAIL_REQUEST_COUNTER			= 101,	// 카운터 적대 선언을 하도록 인증
	};

	unsigned long	m_dwCID;			// 캐릭터 아이디
	unsigned long	m_dwGID;			// 자신의 GID
	unsigned long	m_dwTargetGID;		// 상대 길드 아이디
	unsigned long	m_dwValue;			// 적대 선언중인 GID ( GR_REQUEST 일때만... )
    char			m_cSubCmd;			// 길드 관계 변경 커맨드

	char			m_szGuildName[MAX_GUILD_NAME_LEN];			// 대상 길드명 / 선포 길드명
	char			m_szTempGuildName[MAX_GUILD_NAME_LEN];		// 이미 적대 선언중인 길드 이름
};

// 길드 성향
struct PktGuildInclination : public PktBase
{
	enum PktGIErr
	{
		FAIL_SO_FAST				= 2,	// 일정 시간동안을 변경할 수 없다.
		FAIL_WRONG_NATION_KARTERANT	= 3,	// 카르테란트 길드에는 인간만 속할 수 있다.
		FAIL_WRONG_NATION_MERKADIA	= 4		// 메르카디아 길드에는 아칸만 속할 수 있다.
	};

	unsigned long	m_dwUID;			// 아이디 (C -> S : 캐릭터 아이디, S -> C : 길드 아이디)
	unsigned char	m_cInclination;		// 성향 (Guild::Inclination 참고)
};

// 길드 권한 정보
struct GuildRight
{
	enum Index
	{
		PUT_STOREHOUSE		= 0,
		GET_STOREHOUSE		= 1,
		USE_SAFE			= 2,
		INVITE_MEMBER		= 3,
		PERMIT_JOIN			= 4,
		CHANGE_PASSWORD		= 5,

		REGULATE_TEX		= 6,
		KICK_MEMBER			= 7,
		BOARD_ADMIN			= 8,

		SETUP_MARK_N_LEVEL	= 9,
		SETUP_MIDDLE		= 10,
		DIVIDE_GOLD			= 11,
		SETUP_POLICY		= 12,

		MAX_USING_RIGHT		= 13
	};

	unsigned char m_aryRight[MAX_RIGHT_SIZE];

	GuildRight()
	{
		Init(false);
	}

	GuildRight(bool bMaxRight)
	{ 
		Init(bMaxRight);
	}

	void Init(bool bMaxRight)
	{
		std::fill_n(m_aryRight, int(MAX_RIGHT_SIZE), Guild::NONE);

		m_aryRight[CHANGE_PASSWORD] = (true == bMaxRight) ? Guild::COMMON : Guild::MASTER;
		m_aryRight[PUT_STOREHOUSE] = (true == bMaxRight) ? Guild::COMMON : Guild::MASTER;		
		m_aryRight[GET_STOREHOUSE] = (true == bMaxRight) ? Guild::COMMON : Guild::MASTER;
		m_aryRight[USE_SAFE] = (true == bMaxRight) ? Guild::COMMON : Guild::MASTER;
		m_aryRight[INVITE_MEMBER] = (true == bMaxRight) ? Guild::COMMON : Guild::MASTER;	
		m_aryRight[PERMIT_JOIN] = (true == bMaxRight) ? Guild::COMMON : Guild::MASTER;

		m_aryRight[REGULATE_TEX] = (true == bMaxRight) ? Guild::MIDDLE_ADMIN : Guild::MASTER;	
		m_aryRight[KICK_MEMBER] = (true == bMaxRight) ? Guild::MIDDLE_ADMIN : Guild::MASTER;
		m_aryRight[BOARD_ADMIN] = (true == bMaxRight) ? Guild::MIDDLE_ADMIN : Guild::MASTER;

		m_aryRight[SETUP_MARK_N_LEVEL] = (true == bMaxRight) ? Guild::MASTER : Guild::MASTER;
		m_aryRight[SETUP_MIDDLE] = (true == bMaxRight) ? Guild::MASTER : Guild::MASTER;
		m_aryRight[DIVIDE_GOLD] = (true == bMaxRight) ? Guild::MASTER : Guild::MASTER;
		m_aryRight[SETUP_POLICY] = (true == bMaxRight) ? Guild::MASTER : Guild::MASTER;
	}

	int IsValid(void)
	{
		GuildRight MaxRight(true);

		int nIndex = 0;
		for (; nIndex < MAX_RIGHT_SIZE; ++nIndex)
		{
			if (m_aryRight[nIndex] > MaxRight.m_aryRight[nIndex])
			{
				return nIndex;
			}
		}

		return nIndex;
	}
};

// 길드 권한
struct PktGuildRight : public PktBase
{
	unsigned long	m_dwUID;			// 아이디 (C -> S : 캐릭터 아이디, S -> C : 길드 아이디)
	GuildRight		m_GuildRight;		// 길드 권한
};

// 자기 길드 정보
struct PktMyGuildInfo : public PktBase
{
	unsigned long	m_dwGold;			// 길드 금고

	GuildRight		m_GuildRight;		// 길드 권한
	unsigned char	m_cTitle;			// 직위
};

// 적대 현황 리스트 (가변 길이) + 적대 현황 정보 
struct PktGuildHostilityList : public PktBase
{
	enum { MAX_NUM_PER_PAGE = 5 };

	enum PageState
	{
		PAGE_CURRENT	= 0,
		PAGE_PREV		= 1,
		PAGE_NEXT		= 2
	};

	unsigned long	m_dwCID;			// 캐릭터 아이디

	unsigned char	m_cCurrentPage;		// 현재 페이지
	unsigned char	m_cPageState;		// 요청한 페이지의 상태
	unsigned char	m_cNodeNum;			// 가변 노드수
	unsigned short	m_wTotalItemNum;	// 적대 현황의 총 리스트 수

	char	m_szHostility[MAX_GUILD_NAME_LEN];	// 적대 선언 중인 길드명
	char	m_szAlert[MAX_GUILD_NAME_LEN];		// 경계 적대 선언중인 길드명
};

// 길드 적대 현황 정보
struct GuildHostilityInfoNode
{
	char	m_szName[MAX_GUILD_NAME_LEN];	// 적대 관계 길드 이름
	unsigned char m_cRelationType;			// 적대 관계 타입 (Guild::Relation)
};

// 자신의 길드의 관계 리스트 요청 패킷 (가변 길이) + 관계 정보
struct PktGuildRelationInfo : public PktBase
{
	unsigned long	m_dwCID;			// 요청 캐릭터
	unsigned long	m_dwGID;			// 자신의 길드 ID
	unsigned short	m_wNodeNum;			// 가변 노드수
};

// 길드 관계 정보
struct GuildRelationInfoNode
{
	// GuildLargeInfo 의 내용과 동일
	unsigned long	m_dwGID;
	unsigned char	m_cRelationByMine;
	unsigned char	m_cRelationByTarget;
};

// 길드 금고
struct PktGuildSafe : public PktBase
{
	enum PktGuildSafeErr
	{
		FAIL_NOT_ENOUGH_MONEY	= 2,		// 금고에 돈이 충분치 않음
		FAIL_NOT_CHAR_MONEY     = 3,		// 캐릭터에 돈이 충분치 않음
		FAIL_NOT_GUILD_DEPOSIT  = 4,		// 금고 입금하는데 실패했습니다.
		FAIL_NOT_GUILD_UPDATE	= 5,		// 금고 정보를 업데이트 하는데 실패했습니다.
		FAIL_REQUEST_DATA	    = 6,		// 정보를 요청중입니다.
	};

	enum SafeCmd
	{
		SC_WITHDRAWAL	= 0,			// 출금
		SC_DEPOSIT		= 1,			// 입금
		SC_RELEASE		= 2,			// 길드원에게 방출
		SC_REFER		= 3,			// 사용 내역 조회
		SC_CAMPSHOP		= 4,			// 길드 요새 상점의 수익금
		SC_BONUS		= 5,			// 길드 보너스
	};

	unsigned long	m_dwCID;			// 캐릭터 아이디
	unsigned long	m_dwGold;			// 입금/출금/방출하는 금액
	unsigned char	m_cCmd;				// 명령
};

// 길드 금고 Ack
struct PktGuildSafeAck : public PktBase
{
	unsigned long	m_dwCID;			// 캐릭터 아이디
	unsigned long	m_dwGID;			// 길드 아이디

	unsigned long	m_dwSafeGold;		// 금고 잔액
	unsigned long	m_dwCharGold;		// 캐릭터 잔액 (C -> S : 입금/출금/방출하는 금액)

	unsigned char	m_cCmd;				// 명령

	char			m_szCharName[Guild::MAX_MEMBER_NAME_LEN];		// 캐릭터 이름
};

// 길드 DB (가변 길이) + 길드 정보
struct PktGuildDB : public PktBase
{
	bool			m_bStartFlag;			// 시작 패킷인가? (PktGuildDB는 길드 수만큼 보내므로 첫번째 길드를 받았을 때 리스트를 초기화 해주어야 한다.)

	unsigned long	m_dwGID;				// 길드 아이디

	unsigned char	m_cTotalMemberNum;		// 길드원 수
	unsigned char	m_cRelationByMineNum;	// 내가 맺은 길드 관계수
	unsigned char	m_cRelationByTargetNum;	// 다른 길드가 맺은 길드 관계수

	unsigned short	m_wSize;				// 길드 정보 크기
};

// 길드 정보 (DB)
struct GuildInfoDB
{
	unsigned long	m_dwGID;
	char			m_strName[MAX_GUILD_NAME_FOR_DB];
	unsigned char	m_cLevel;
	unsigned long	m_dwFame;
	unsigned long	m_dwGold;
	TIME			m_tmLastLogout;
	TIME			m_tmCheckMember;
	TIME			m_tmGMLastLogout;
	char			m_szMark[MAX_MARK_SIZE];
	char			m_szRight[MAX_RIGHT_SIZE];
	unsigned char	m_cInclination;
	TIME			m_tmChangeInclination;
};

// 길드 멤버 (DB)
struct GuildMemberDB
{
	unsigned long	m_dwGID;
	unsigned long	m_dwCID;
	unsigned long	m_dwRank;
	char			m_strName[MAX_MEMBER_NAME_LEN];
	unsigned long	m_dwTitle;
	unsigned char	m_cLevel;
	unsigned short	m_wClass;
	unsigned long	m_dwFame;
	unsigned long	m_dwGold;
	unsigned char	m_cGuildWarFlag;
	TIME			m_LeaveGuildTime;

	unsigned char	m_cTactics;
};

// 길드 관계 (DB)
struct GuildRelationDB
{
	unsigned long	m_dwGID;
	unsigned long	m_dwTargetGID;
	unsigned char	m_cRelation;
};

// 길드 포지션 설정 패킷.
struct PktGuildPosition : public PktBase
{
	enum PktErr
	{
		FAIL_GUILD			= 1,		// 길드가 없을때.
		FAIL_CONNECT_CHAR	= 2,		// 캐릭터가 접속 중일때.
		FAIL_DB_UPDATE		= 3,		// DB 업데이트 실패.
		FAIL_MEMBER_DELETE	= 4			// 길드 멤버 삭제 실패.
	};

	enum Const
	{
		TYPE_POSITION		= 0,		// 포지션 변경시.
		TYPE_MEMBERDELETE	= 1			// 길드 멤버 삭제시.
	};

	unsigned long	m_dwGID;
	unsigned long	m_dwCID;
	unsigned char	m_cPosition;
	unsigned char	m_cType;
};

// 위치가 없어서 임시로(조이스틱 키 설정 관련).
struct PktKeyInfo : public PktBase
{	
	enum Const
	{
		MAX_KEY_INFO	= 34	
	};

	unsigned long	m_dwCID;
	unsigned long	m_dwKeyInfo[MAX_KEY_INFO];
};

#pragma pack()

#endif
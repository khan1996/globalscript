#ifndef _CREATURE_STRUCTURE_H_
#define _CREATURE_STRUCTURE_H_

#include <winsock2.h>
#include <windows.h>
#include <cmath>

#include <DB/DBDefine.h>

// 전방 참조
struct CreatureStatus;
struct CharacterStatus;
struct SKILL;

namespace Item
{
	class CItem;
	class CEquipment;

	namespace EquipType
	{
		enum DoubleSwordType;
	}
}

namespace BattleInclination
{
	enum Const
	{
		IS_PEACEMODE        = ( 1 << 0 ),   // 평화모드인 경우 세팅
		IS_SAFETY_ZONE      = ( 1 << 1 ),   // 세이프티 존 내부에 있는 경우 세팅
		HAS_CAMP            = ( 1 << 2 ),   // 길드 요새를 가진 길드에 속해 있을 경우 세팅
		HAS_DEVELOPING_CAMP	= ( 1 << 3 ),   // 건설중인 길드 요새를 가진 길드에 속해 있을 경우 세팅
		IS_CASTLE_OBJECT    = ( 1 << 4 ),   // 공성 Object이면서 수성병기인 경우만 세팅
		IS_CASTLE_NPC		= ( 1 << 5 ),	// 공성 Object이면서 수성 병기의 병기 관리 NPC 와 뒷문 NPC
		IS_GOD_MODE			= ( 1 << 6 ),	// G모드인 경우 세팅 
		IS_TEAM_BATTLE		= ( 1 << 7 ),	// 팀배틀 중인 경우 세팅
	};

	enum VSType
	{
		VS_CHARACTER		= ( 1 << 0 ),	// PvP
		VS_PARTY			= ( 1 << 1 ),	// 파티간 배틀
		VS_GUILD			= ( 1 << 2 ),	// 길드간 배틀
	};

	// 피아식별시 주체가 되는 캐릭터 정보
	// 여기서 데이터 추가하거나 삭제하는 경우, 
	// 브로드캐스팅 시, 채팅서버로 데이터 전송하는 부분도 수정해야한다.
	struct CharData
	{
		bool IsPeaceMode(void) const		{ return 0 != (m_cFlags & IS_PEACEMODE);   		}	// 평화모드인 경우
		bool IsSafetyZone(void) const		{ return 0 != (m_cFlags & IS_SAFETY_ZONE); 		}	// 내가 Safety존 내부에 있는지
		bool HasCamp(void) const			{ return 0 != (m_cFlags & HAS_CAMP);       		}	// 속한 길드가 길드 요새를 가지고 있는지...
		bool HasDevelopingCamp(void) const	{ return 0 != (m_cFlags & HAS_DEVELOPING_CAMP); }	// 속한 길드가 길드 요새를 건설중인지...
		bool IsCastleObject(void) const		{ return 0 != (m_cFlags & IS_CASTLE_OBJECT);	}	// 공성병기인 경우, 수성병기인지...
		bool IsCastleNPC(void) const		{ return 0 != (m_cFlags & IS_CASTLE_NPC);		}	// 수성 NPC 인 경우
		bool IsGODMode(void) const			{ return 0 != (m_cFlags & IS_GOD_MODE);			}	// G모드인 경우
		bool IsTeamBattle(void) const		{ return 0 != (m_cFlags & IS_TEAM_BATTLE);		}	// 팀배틀 중인 경우

		void SetPeaceMode(bool bPeaceMode)					{ m_cFlags |= bPeaceMode ? IS_PEACEMODE : 0;				}
		void SetSafetyZone(bool bInSafetyZone)				{ m_cFlags |= bInSafetyZone ? IS_SAFETY_ZONE : 0;			}
		void SetHasCamp(bool bHasCamp)						{ m_cFlags |= bHasCamp ? HAS_CAMP : 0;						}
		void SetHasDevelopingCamp(bool bHasDevelopingCamp)	{ m_cFlags |= bHasDevelopingCamp ? HAS_DEVELOPING_CAMP : 0;	}
		void SetCastleObject(bool bCastleObject)			{ m_cFlags |= bCastleObject ? IS_CASTLE_OBJECT : 0; 		}
		void SetCastleNPC(bool bCastleNPC)					{ m_cFlags |= bCastleNPC ? IS_CASTLE_NPC : 0;				}
		void SetGODMode(bool bGODMode)						{ m_cFlags |= bGODMode ? IS_GOD_MODE : 0;					}
		void SetTeamBattle(bool bTeamBattle)				{ m_cFlags |= bTeamBattle ? IS_TEAM_BATTLE : 0;				}

		unsigned long 	m_dwCID;          	// 캐릭터 CID
		unsigned long 	m_dwGID;          	// 캐릭터 GID        
		unsigned long 	m_dwCastleID;     	// 공성 병기인 경우, 속한 성의 ID를 넣는다.

		unsigned char 	m_cSiegeState;    	// 공성 병기인 경우, 공성 병기 상태를 넣는다.
		unsigned char 	m_cNation;        	// 캐릭터 국적
		unsigned char 	m_cAdminLevel;    	// 운영자 레벨
		unsigned char 	m_cFlags;         	// 기타 정보

		unsigned long	m_dwPID;			// 캐릭터에 파티 정보
		unsigned char	m_cTacticsFlag;		// 캐릭터에 용병 정보

		unsigned char	m_cRealmWarFlag;
		unsigned char	m_cGuildWarFlag;

		unsigned char	m_cCreatureType;	// 크리쳐 타입		
		unsigned short	m_wObjectType;		// 스트럭쳐 정보
		unsigned short	m_wKind;			// 스트럭쳐 종류

		unsigned char	m_cRide;			// 병기에 탑승 여부.
	};    

	// 피아식별시 쓰이는 캐릭터간의 관계 (일단은 채팅 서버까진 갈 필요 없는 정보로 구성)
	struct RelationData
	{
		unsigned char	m_cVSFlag;			// PvP, 팀배틀 정보
		bool			m_bHostileGuild;	// 적대 길드인가?
	};
};

namespace Creature
{
	enum Const
	{
		PC_MAX_LEVEL		= 100,		// 캐릭터 최대 레벨
		MONSTER_MAX_LEVEL	= 150,		// 몬스터 최대 레벨

		// edith 2008.02.27 길드 가입 제한제외
		LEVEL_ABLE_WAR		= 1		// 전쟁이 가능한 최소 레벨 (길드 가입/전쟁 모드 제한)
	};

	enum IdentifyCIDBit 
	{
		MONSTER_KIND_BIT	= 0x0000FFFF,

		MONSTER_BIT			= 0x80000000,
		NPC_BIT				= 0x40000000,
		SIEGE_OBJECT_BIT	= 0x10000000,
		SUMMON_MONSTER_BIT	= 0xA0000000,	// MONSTER_BIT (0x80000000) + 0x20000000
		NPC_LINK_BIT		= 0x50000000,	// 소환 완료된 석상의 경우 이 비트가 세트된 NPC와 링크되어 메달상점의 역할을 한다.
											// NPC_BIT (0x40000000) + 0x10000000

		// 석상 KID (MonsterStructure.h 에도 정의되어 있음)
		MIN_STATUE_KID		= 4001,
		MAX_STATUE_KID		= 4070
	};

	enum CreatureType
	{
		CT_PC				= 0,
		CT_NPC				= 1,
		CT_MONSTER			= 2,
		CT_SUMMON			= 3,
		CT_STRUCT			= 4,
		CT_SIEGE_OBJECT		= 5,
		CT_NONE_TYPE		= 99
	};

	enum Nation		
	{ 
		STATELESS		= 0,	// 무국적 (몬스터, NPC)
		KARTERANT		= 1,	// 카르테란트
		MERKADIA		= 2,	// 메르카디아
		ALMIGHTY_PIRATE	= 3,	// 신의해적단

		MAX_NATION		= 4
	};			  

	enum WarFlag
	{
		WAR_OFF			= 0,	// 전쟁 참여 안함
		WAR_ON			= 1,	// 전쟁 참여
		WAR_INSTANCE	= 2		// 인스턴스 전쟁 참여
	};

	enum StatusType
	{
		RIGHT_PASSIVE_TYPE		= 0, 	// 패시브 스킬이 적용된 오른손 장비 스탯 (기본값으로 CreatureStatus에 바로 사용된다.)
		LEFT_PASSIVE_TYPE		= 1,	// 패시브 스킬이 적용된 왼손 장비 스탯 (이도류)
		RIGHT_NON_PASSIVE_TYPE	= 2,	// 패시브 스킬이 적용되지 않은 오른손 장비 스탯 (오른손 무기를 사용하는 스킬)
		LEFT_NON_PASSIVE_TYPE	= 3,	// 패시브 스킬이 적용되지 않은 왼손 장비 스탯 (왼손 무기(스킬암)를 사용하는 스킬)

		MAX_STATUS_TYPE			= 4
	};

	// 서버에서 사용하는 Creature 판별 함수들
	// 클라이언트에서는 CRYLGameData::GetClientIdentity() 함수 사용!!
	static bool	IsSummonMonster(unsigned long dwCID)
	{
		return (Creature::SUMMON_MONSTER_BIT == (dwCID & Creature::SUMMON_MONSTER_BIT));
	}

	static bool	IsStruct(unsigned long dwCID)
	{
		unsigned short wKID = static_cast<unsigned short>(dwCID & Creature::MONSTER_KIND_BIT);
		return (wKID >= MIN_STATUE_KID && wKID <= MAX_STATUE_KID);
	}

	static bool	IsLinkedNPC(unsigned long dwCID)
	{
		return (Creature::NPC_LINK_BIT == (dwCID & Creature::NPC_LINK_BIT));
	}

	static CreatureType GetCreatureType(unsigned long dwCID)
	{
		if (dwCID & Creature::MONSTER_BIT)
		{
			if (IsSummonMonster(dwCID)) return Creature::CT_SUMMON;
			else if (IsStruct(dwCID)) return Creature::CT_STRUCT;

			return Creature::CT_MONSTER;
		}
		else if (dwCID & Creature::NPC_BIT)	return Creature::CT_NPC;
		else if (dwCID & Creature::SIEGE_OBJECT_BIT) return Creature::CT_SIEGE_OBJECT;

		return Creature::CT_PC;
	}

    inline static const char* GetNationName(unsigned char cNation)
    {
        switch(cNation)
        {
        case KARTERANT:         return "KARTERANT";
        case MERKADIA:          return "MERKADIA";
        case ALMIGHTY_PIRATE:   return "ALMIGHTY_PIRATE";
        }

        return "STATELESS";
    }

    inline static const char* GetShortNationName(unsigned char cNation)
    {
        switch(cNation)
        {
        case KARTERANT:         return "KR";
        case MERKADIA:          return "ME";
        case ALMIGHTY_PIRATE:   return "AP";
        }

        return "ST";
    }
};


// 엘리트 보너스 정보
namespace EliteBonus
{
	enum Const
	{ 
		MAX_BONUS_LEVEL		= 10,
		MAX_BONUS_DROP_ITEM	= 3
	};

	// 엘리트 보너스에 의한 경험치/드랍률 보너스가 제거되었습니다. (2004-09-17 by 로딘)
	static short usBonusExp[MAX_BONUS_LEVEL] = { 
		100 + 0, 
		100 + 0, 
		100 + 0, 
		100 + 0, 
		100 + 0,
		100 + 0,
		100 + 0, 
		100 + 0,
		100 + 0,
		100 + 0
	};

	static unsigned char usBonusItemDropRate[MAX_BONUS_LEVEL][MAX_BONUS_DROP_ITEM] = { 
		{ 0, 0, 0 }, 
		{ 0, 0, 0 }, 
		{ 0, 0, 0 }, 
		{ 0, 0, 0 }, 
		{ 0, 0, 0 }, 
		{ 0, 0, 0 }, 
		{ 0, 0, 0 }, 
		{ 0, 0, 0 }, 
		{ 0, 0, 0 }, 
		{ 0, 0, 0 }
	};

	struct EliteBonusData
	{
		char m_cNation;
		unsigned char m_cLevel;

		EliteBonusData();
		EliteBonusData(char cNation, unsigned char cLevel);
	};
}


// 맵 이동 정보
struct Position
{
	float m_fPointX;				// Point X 좌표
	float m_fPointY;				// Point Y 좌표
	float m_fPointZ;				// Point Z 좌표

	inline Position();
	inline Position(const POS& Pos);
	inline Position(const Position& Pos);
	inline Position(float fPointX, float fPointY, float fPointZ);	

    inline POS PositionToPOS() const;

	inline unsigned long GetDistance(const Position& rhs) const
	{ 
		return static_cast<unsigned long>(sqrt(
			(m_fPointX - rhs.m_fPointX) * (m_fPointX - rhs.m_fPointX) + 
			(m_fPointY - rhs.m_fPointY) * (m_fPointY - rhs.m_fPointY) +
			(m_fPointZ - rhs.m_fPointZ) * (m_fPointZ - rhs.m_fPointZ)));
	}

    inline unsigned long GetSquaredDistance(const Position& rhs) const
    { 
        return static_cast<unsigned long>(
            (m_fPointX - rhs.m_fPointX) * (m_fPointX - rhs.m_fPointX) + 
            (m_fPointY - rhs.m_fPointY) * (m_fPointY - rhs.m_fPointY) +
            (m_fPointZ - rhs.m_fPointZ) * (m_fPointZ - rhs.m_fPointZ));
    }

	inline bool operator == (const Position& rhs) const
	{
		if ( m_fPointX == rhs.m_fPointX && m_fPointY == rhs.m_fPointY && m_fPointZ == rhs.m_fPointZ )
		{
			return true;
		}

		return false;
	}

	inline bool operator != (const Position& rhs) const
	{
		if ( m_fPointX != rhs.m_fPointX || m_fPointY != rhs.m_fPointY || m_fPointZ != rhs.m_fPointZ )
		{
			return true;
		}

		return false;
	}

	inline void operator () (const Position& rhs)
	{
		m_fPointX	= rhs.m_fPointX;
		m_fPointY	= rhs.m_fPointY;
		m_fPointZ	= rhs.m_fPointZ;
	}
};

inline Position::Position()
:	m_fPointX(0.0f), m_fPointY(0.0f), m_fPointZ(0.0f) { }

inline Position::Position(const POS& Pos)
:	m_fPointX(Pos.fPointX), m_fPointY(Pos.fPointY), m_fPointZ(Pos.fPointZ) { }

inline Position::Position(const Position& Pos)
:	m_fPointX(Pos.m_fPointX), m_fPointY(Pos.m_fPointY), m_fPointZ(Pos.m_fPointZ) { }

inline Position::Position(float fPointX, float fPointY, float fPointZ)
:	m_fPointX(fPointX), m_fPointY(fPointY), m_fPointZ(fPointZ) { }

inline POS Position::PositionToPOS() const
{
    POS stPos;
    stPos.fPointX = m_fPointX;
    stPos.fPointY = m_fPointY;
    stPos.fPointZ = m_fPointZ;

    return stPos;
}


// 동작 정보
struct MotionInfo
{
	float			m_fDirection;	// 바라보는 방향
	float			m_fVelocity;	// 속도
	unsigned short	m_wAction;		// 취하는 행동
	unsigned long	m_dwFrame;		// 모션 프레임 ( 몬스터, NPC가 사용 )

	MotionInfo();
};


// 인챈트 정보
struct EnchantInfo
{
	enum 
	{
		ULONG_BIT		= 32,	// unsigned long (32바이트)
		MAX_ARRAY		= 4		// 스펠의 종류는 UCHAR_BIT * MAX_ARRAY
	};

	unsigned long	m_dwStatusFlag[MAX_ARRAY];	// 챈트/인챈트 플래그

	EnchantInfo();
    EnchantInfo( const EnchantInfo& Info )      // 대입을 위한 복사 생성자
    {
        for( int i = 0 ; i < MAX_ARRAY ; ++i )
        {
            m_dwStatusFlag[ i ] = Info.m_dwStatusFlag[ i ];
        }
    }

	void SetFlag(unsigned char cSpellKind);
	void ResetFlag(unsigned char cSpellKind);
	void ClearFlag(void);

	bool GetFlag(unsigned char cSpellKind) const;
};


// 데미지 계산시의 추가 효과 정보
struct CalculateDamageInfo
{
	bool	m_bForceDRC;			// '참'이면 기본 DRC를 무시하고 추가 DRC로 대체

	float	m_fDRC;					// 추가 DRC
    short	m_nOffenceRevision;		// 추가 공격보정
	short	m_nMinDamage;			// 추가 최소데미지
	short	m_nMaxDamage;			// 추가 최대데미지

	CalculateDamageInfo()
	:	m_bForceDRC(false), m_fDRC(0.0f), m_nOffenceRevision(0), m_nMinDamage(0), m_nMaxDamage(0) 
	{ }

	CalculateDamageInfo(bool bForceDRC, float fDRC, short nOffenceRevision, short nMinDamage, short nMaxDamage)
	:	m_bForceDRC(bForceDRC), m_fDRC(fDRC), 
		m_nOffenceRevision(nOffenceRevision), m_nMinDamage(nMinDamage), m_nMaxDamage(nMaxDamage) 
	{ }
};
typedef CalculateDamageInfo* LPCalculateDamageInfo;


// 캐릭터에 대한 속성
struct CharacterStatus
{
	short	m_nSTR;		// 캐릭터 STR
	short	m_nDEX;		// 캐릭터 DEX
	short	m_nCON;		// 캐릭터 CON
	short	m_nINT;		// 캐릭터 INT
	short	m_nWIS;		// 캐릭터 WIS

	CharacterStatus();
	CharacterStatus(CHAR_INFOST& characterDBData);
	void Init(CHAR_INFOST& characterDBData);
};
typedef CharacterStatus*	LPCharacterStatus;

struct FightStatus
{
	// 계산 상태 (중복 계산 방지)
	enum CalculateState
	{
		CS_NONE				= 0,				// 초기화
		CS_BASE_INFO		= 1,				// 기본 스탯으로 계산을 완료한 상태
		CS_EQUIP_INFO		= 2,				// 장비 효과를 추가 계산한 상태
		CS_ENCHANT_INFO		= 3					// 인챈트 효과를 받은 상태
	};

	enum Const
	{
		MAX_INCHANT_HP		= 15000,
		MAX_INCHANT_MP		= 15000
	};

	long			m_lMinDamage;				// 최소데미지
	long			m_lMaxDamage;				// 최대데미지
	short			m_wArmor;					// 방어력
	short			m_wHitRate;					// 명중
	short			m_wEvade;					// 회피

	unsigned short	m_nMaxHP;					// HP 최대값
	short			m_wHPRegen;					// HP 회복
	unsigned short	m_nMaxMP;					// MP 최대값
	short			m_wMPRegen;					// MP 회복

	short			m_wCritical;				// 크리티컬
	short			m_wBlock;					// 블록
	short			m_wSpeed;					// 속도
	short			m_wCoolDownRevision;		// 쿨다운보정
	short			m_wSkillPoint;				// 스킬포인트

	short			m_wMagicPower;				// 마법력
	short			m_wMagicResist;				// 마법저항

	int				m_wLuck;					// 행운값

	//--//	start..
	short			m_wFrost;					// 냉기/정지 속성
	short			m_wFire;					// 불/독 속성
	short			m_wElectro;					// 전지/충격 속성
	short			m_wDarkness;				// 암흑/빛 속성
	//--//	end..

	float			m_fDefenceRate;				// 방어%
	float			m_fCriticalMagnification;	// 크리티컬배율
	float			m_fCriticalRate;			// 크리티컬%
	float			m_fBlockRate;				// 블럭률%
	float			m_fSpeedRate;				// 속도%
	float			m_fCoolDownRevisionRate;	// 쿨다운보정%
	float			m_fMagicResistRate;			// 마법저항%

	float			m_fLuckResistRate;			// 럭키찬스%

	float			m_fLevelExplosion;			// 레벨 익스플로젼

	unsigned char	m_cCriticalType;			// 크리티컬 타입
	unsigned char	m_cComboCount;				// 공격 콤보 횟수

	unsigned char	m_cCalculateState;			// 계산 상태


	FightStatus();

	bool CalculateBaseInfo(const CharacterStatus& characterStatus, const unsigned short wLevel, const unsigned char cClass);
		
	bool CalculateEquipInfo(const Item::CEquipment** ppEquip, const bool bFirstWeaponSlot, const bool bRightHand,  const bool bRideFlag,
		const SKILL& skill, const unsigned char cLevel, const unsigned short wRiderAbility, const unsigned short wLuckAbility, const unsigned short wLevelAbility,
		const CharacterStatus& characterStatus, const unsigned char cClass, FightStatus& resultStatus, int& resultLuck) const;

	bool CalculateEnchantInfo(const unsigned short* aryEnchantLevel, FightStatus& resultStatus) const;
		

	void CalculateSubStatus(void);

	static bool CheckEquipable(const Item::CEquipment* lpEquip, const CharacterStatus& characterStatus,
		const unsigned char cClass, const unsigned char cLevel, const unsigned short wLevelAbility);	//--//
};

#pragma pack(8)

//------------------------------------------------------------------------------------
// TODO : memory alignment 주의할 것.
//		: 4byte단위로 alignment 가 되도록 할 것.
//		: 메모리 부족시에는 크기가 많이 필요 없는 멤버는 short등으로 줄일 것.

// 무기나 도구에 의해서 영향을 받은 능력치를 저장 (게임상에서 모든 계산은 이것으로 함)
struct CreatureStatus
{
	__int64			m_nExp;			// 사람인 경우는 자신의 경험점, 몬스터인 경우는 나누어 줄 경험점
	int				m_nLevel;		// 레벨
	unsigned short	m_nNowHP;		// 현재 HP
	unsigned short	m_nNowMP;		// 현재 MP (몬스터의 경우는 무조건 최대값으로 초기화)

	FightStatus		m_StatusInfo;	// 대부분의 정보가 이곳에 있음

	CreatureStatus();
	CreatureStatus(CHAR_INFOST& characterDBData);
	void Init(CHAR_INFOST& characterDBData);
};
typedef CreatureStatus* LPCreatureStatus;

#pragma pack()

#endif

#ifndef _MONSTER_STRUCTURE_H_
#define _MONSTER_STRUCTURE_H_

#include <Creature/CreatureStructure.h>

#pragma pack(8)

struct MonsterInfo
{
	enum MaxNumber
	{
		MAX_MOTION_NUM				= 4,		// 최대 모션 개수
		MAX_ORIGINAL_ITEM_NUM		= 5,		// 몬스터 보유 잡템 최대수

		MAX_NAME_LEN				= 64,		// 몬스터 이름 길이
		MAX_MODELING_FLAG_LENGTH	= 32		// 모델링 데이터 이름 길이
	};

	enum MonsterPattern
	{
		PATTERN_COMMON		= 0,	
		PATTERN_WARRIOR		= 1,				// 워리어
		PATTERN_DEFENDER	= 2,				// 디펜더
		PATTERN_MAGE		= 3,				// 메이지
		PATTERN_ACOLYTE		= 4,				// 어콜라이트
        PATTERN_BOSS		= 5,				// 보스
		PATTERN_BG			= 6,				// 배경(?)
		PATTERN_SUMMON		= 7,				// 소환수
		PATTERN_STRUCTURE	= 8,				// 구조물 (석상 종류)
		PATTERN_NAMED		= 9,				// 네임드
		PATTERN_CHIEF		= 10,				// 치프
		PATTERN_OBJECT		= 11,				// 오브젝트 (꽃, 버섯 등등..)
		PATTERN_GUARD		= 12,				// 경비병
		PATTERN_GATHER		= 13,				// 수집 타입
		MAX_PATTERN			= 14
	};

	enum SkillPattern
	{
		ATTACK_SKILL		= 0,				// 공격열 스킬
		RECOVER_SKILL		= 1,				// 회복열 스킬
		ENCHANT_SKILL		= 2,				// 부여열 스킬
		EXTENT_SKILL		= 3,				// 범위열 스킬
		BOSS_CHANT_SKILL	= 4,				// 보스의 챈트 스킬

		MAX_SKILL_PATTERN	= 5	
	};

	enum HitBox
	{
		X_PLUS				= 0, 
		X_MINUS				= 1, 
		Y_PLUS				= 2, 
		Y_MINUS				= 3,

		MAX_HITBOX_NUM		= 4
	};

	enum Z3D_CHR_ACTION
	{
		Z3D_CA_RUN			= 1,				// 달리기
		Z3D_CA_ATTACK		= 2,				// 공격
		Z3D_CA_WALK			= 8,				// 걷기
		Z3D_CA_CASTING		= 24				// 캐스팅
	};

	enum AwardKind 
	{ 
		ORIGINAL1			= 0,				// 잡템1
		ORIGINAL2			= 1,				// 잡템2
		ORIGINAL3			= 2,				// 잡템3
		ORIGINAL4			= 3,				// 잡템4
		ORIGINAL5			= 4,				// 잡템5

		COIN				= 5,				// 돈
		GEM					= 6,				// 보석
		METAL				= 7,				// 광물
		POTION				= 8,				// 물약
		SKILL				= 9,				// 스킬북

		CASH_ITEM			= 10,				// 캐쉬템
		LOTTERY				= 11,				// 복권

		F_STANDARD_EQUIP	= 12,				// Standard-Option Item (Grade F)
		D_STANDARD_EQUIP	= 13,				// Standard-Option Item (Grade D)
		C_STANDARD_EQUIP	= 14,				// Standard-Option Item (Grade C)
		B_STANDARD_EQUIP	= 15,				// Standard-Option Item (Grade B)
		A_STANDARD_EQUIP	= 16,				// Standard-Option Item (Grade A)

		F_OVER_EQUIP		= 17,				// Over-Option Item (Grade F)
		D_OVER_EQUIP		= 18,				// Over-Option Item (Grade D)
		C_OVER_EQUIP		= 19,				// Over-Option Item (Grade C)
		B_OVER_EQUIP		= 20,				// Over-Option Item (Grade B)
		A_OVER_EQUIP		= 21,				// Over-Option Item (Grade A)

		NO_EQUIP			= 22,				// No-Option Item

		//--//	start..
		F_RING				= 23,				// 반지 (Grade F)
		D_RING				= 24,				// 반지 (Grade D)
		C_RING				= 25,				// 반지 (Grade C)
		B_RING				= 26,				// 반지 (Grade B)
		A_RING				= 27,				// 반지 (Grade A)

		F_NECKLACE			= 28,				// 목걸이 (Grade F)
		D_NECKLACE			= 29,				// 목걸이 (Grade D)
		C_NECKLACE			= 30,				// 목걸이 (Grade C)
		B_NECKLACE			= 31,				// 목걸이 (Grade B)
		A_NECKLACE			= 32,				// 목걸이 (Grade A)
		
		F_RUNE				= 33,				// 룬 (Grade F)
		D_RUNE				= 34,				// 룬 (Grade D)
		C_RUNE				= 35,				// 룬 (Grade C)
		B_RUNE				= 36,				// 룬 (Grade B)
		A_RUNE				= 37,				// 룬 (Grade A)
		DESTRUCTION_RUNE	= 38,				// 소멸의 룬

		MAX_AWARD_KIND		= 39
		//--//	end..
	};

	enum SpeacialKindID
	{
		// 소환수
		SUMMON_KINDLING			= 3500, 
		SUMMON_FLAMEWALKER		= 3506,
		SUMMON_BURNINGSHIELD	= 3513,
		SUMMON_FIREWING			= 3520,
		SUMMON_DEATHBURN		= 3527,

		// 1레벨 석상
		// 명예의 석상
		STATUE_NEUTRALITY1		= 4001,
		STATUE_HUMAN_LOADING1	= 4002,
		STATUE_HUMAN_COMPLETE1	= 4003,
		STATUE_AKHAN_LOADING1	= 4004,
		STATUE_AKHAN_COMPLETE1	= 4005,

		// BattleGround Server 명예의 석상
		BG_STATUE_NEUTRALITY1			= 4006,
		BG_STATUE_HUMAN_LOADING1		= 4007,
		BG_STATUE_HUMAN_COMPLETE1		= 4008,
		BG_STATUE_AKHAN_LOADING1		= 4009,
		BG_STATUE_AKHAN_COMPLETE1		= 4010,

		// 다크 카나번 석상
		POWER_STATUE_NEUTRALITY1		= 4011,		// 힘의 석상
		POWER_STATUE_HUMAN_LOADING1		= 4012,
		POWER_STATUE_HUMAN_COMPLETE1	= 4013,
		POWER_STATUE_AKHAN_LOADING1		= 4014,
		POWER_STATUE_AKHAN_COMPLETE1	= 4015,

		INT_STATUE_NEUTRALITY1			= 4016,		// 지능의 석상
		INT_STATUE_HUMAN_LOADING1		= 4017,
		INT_STATUE_HUMAN_COMPLETE1		= 4018,
		INT_STATUE_AKHAN_LOADING1		= 4019,
		INT_STATUE_AKHAN_COMPLETE1		= 4020,

		EXP_STATUE_NEUTRALITY1			= 4021,		// 경험의 석상
		EXP_STATUE_HUMAN_LOADING1		= 4022,
		EXP_STATUE_HUMAN_COMPLETE1		= 4023,
		EXP_STATUE_AKHAN_LOADING1		= 4024,
		EXP_STATUE_AKHAN_COMPLETE1		= 4025,

		WEALTH_STATUE_NEUTRALITY1		= 4026,		// 부의 석상
		WEALTH_STATUE_HUMAN_LOADING1	= 4027,
		WEALTH_STATUE_HUMAN_COMPLETE1	= 4028,
		WEALTH_STATUE_AKHAN_LOADING1	= 4029,
		WEALTH_STATUE_AKHAN_COMPLETE1	= 4030,

		LIFE_EXTRACT_NEUTRALITY1		= 4031,		// 생명 축출기
		LIFE_EXTRACT_HUMAN_LOADING1		= 4032,
		LIFE_EXTRACT_HUMAN_COMPLETE1	= 4033,
		LIFE_EXTRACT_AKHAN_LOADING1		= 4034,
		LIFE_EXTRACT_AKHAN_COMPLETE1	= 4035,


		// 2레벨 석상
		// 명예의 석상
		STATUE_NEUTRALITY2		= 4036,
		STATUE_HUMAN_LOADING2	= 4037,
		STATUE_HUMAN_COMPLETE2	= 4038,
		STATUE_AKHAN_LOADING2	= 4039,
		STATUE_AKHAN_COMPLETE2	= 4040,

		// BattleGround Server 명예의 석상
		BG_STATUE_NEUTRALITY2			= 4041,
		BG_STATUE_HUMAN_LOADING2		= 4042,
		BG_STATUE_HUMAN_COMPLETE2		= 4043,
		BG_STATUE_AKHAN_LOADING2		= 4044,
		BG_STATUE_AKHAN_COMPLETE2		= 4045,

		// 다크 카나번 석상
		POWER_STATUE_NEUTRALITY2		= 4046,		// 힘의 석상
		POWER_STATUE_HUMAN_LOADING2		= 4047,
		POWER_STATUE_HUMAN_COMPLETE2	= 4048,
		POWER_STATUE_AKHAN_LOADING2		= 4049,
		POWER_STATUE_AKHAN_COMPLETE2	= 4050,

		INT_STATUE_NEUTRALITY2			= 4051,		// 지능의 석상
		INT_STATUE_HUMAN_LOADING2		= 4052,
		INT_STATUE_HUMAN_COMPLETE2		= 4053,
		INT_STATUE_AKHAN_LOADING2		= 4054,
		INT_STATUE_AKHAN_COMPLETE2		= 4055,

		EXP_STATUE_NEUTRALITY2			= 4056,		// 경험의 석상
		EXP_STATUE_HUMAN_LOADING2		= 4057,
		EXP_STATUE_HUMAN_COMPLETE2		= 4058,
		EXP_STATUE_AKHAN_LOADING2		= 4059,
		EXP_STATUE_AKHAN_COMPLETE2		= 4060,

		WEALTH_STATUE_NEUTRALITY2		= 4061,		// 부의 석상
		WEALTH_STATUE_HUMAN_LOADING2	= 4062,
		WEALTH_STATUE_HUMAN_COMPLETE2	= 4063,
		WEALTH_STATUE_AKHAN_LOADING2	= 4064,
		WEALTH_STATUE_AKHAN_COMPLETE2	= 4065,

		LIFE_EXTRACT_NEUTRALITY2		= 4066,		// 생명 축출기
		LIFE_EXTRACT_HUMAN_LOADING2		= 4067,
		LIFE_EXTRACT_HUMAN_COMPLETE2	= 4068,
		LIFE_EXTRACT_AKHAN_LOADING2		= 4069,
		LIFE_EXTRACT_AKHAN_COMPLETE2	= 4070,

		// 석상 KID (CreatureStructure.h 에도 정의되어 있음)
		MIN_STATUE_KID			= 4001,
		MAX_STATUE_KID			= 4070
	};

	enum StatueEffectType
	{
		SE_NONE				= 0,
		SE_HP				= 1,		// Max HP 상승 효과
		SE_MP				= 2,		// Max MP 상승 효과
		SE_EXP				= 3,		// 획득 Exp 상승 효과
		SE_DROPRATE			= 4,		// 획득 아이템 DropRate 상승 효과

		MAX_EFFECT_TYPE_NUM	= 5
	};


	MotionInfo	m_MonsterMotions[MAX_MOTION_NUM];				// 몬스터의 행동 정보 (걷기, 달리기, 공격, 캐스팅)
	char		m_strName[MAX_NAME_LEN];						// 이름
	char		m_strModelingFlag[MAX_MODELING_FLAG_LENGTH];	// 모델링 형태 플래그 
	float		m_fHitBox[MAX_HITBOX_NUM];						// 타격박스

	unsigned short	m_aryAwardItem[MAX_ORIGINAL_ITEM_NUM];		// 몬스터가 가지고 있는 오리지날 아이템들
	unsigned long	m_aryDropRate[MAX_AWARD_KIND];				// 아이템 종류별 드랍률
	unsigned char	m_cStandardBaseNum;							// Standard-Option 장비의 최소 + 갯수
	unsigned char	m_cOverBaseNum;								// Over-Option 장비의 최소 + 갯수

	unsigned long	m_dwKID;									// 종류 ID
	unsigned long	m_dwRespawnTime;							// 리스폰 타임

	float	m_fSize;											// 사이즈
	float	m_fAttackAngle;										// 공격 각도
	short	m_wAttackRange;										// 공격 거리

	unsigned char	m_cSkillPattern;							// 스킬 패턴
    unsigned char	m_cSkillLevel;								// 스킬 레벨

	unsigned char	m_cFixLevelGap;								// 고정 레벨갭
	bool			m_bFixLevelGap;								// 고정 레벨갭 적용 여부
		
	char	m_cNation;											// 국가

	unsigned short	m_wSkillUseRate;							// 스킬 사용 확률
	unsigned short	m_wSkillID[MAX_SKILL_PATTERN];				// 몬스터 스킬 ID
	unsigned char	m_cEnchantSpellType;						// 부여 스킬에 의한 스펠타입 ( Skill::SpellID::Type )
	unsigned char	m_cChantSpellType;							// 파티 챈트 스킬에 의한 스펠타입 ( Skill::SpellID::Type )
	float			m_fSkillEffectSize;							// 스킬 이펙트 사이즈 (인간: 1.0)
	
	bool	m_bIgnoreEnchant;									// 챈트, 인챈트 무시 여부
	bool	m_bCollision;										// 충돌 체크 여부
	bool	m_bFirstAttack;										// 선제공격 여부
	bool	m_bReturnPosition;									// 타겟이 도망가면 제자리로 돌아가는지 여부
	bool	m_bUseLOD;											// LOD 처리 유무

	// 공성 오브젝트 관련
	unsigned long	m_dwDevelopGold;							// 개발 비용
	unsigned char	m_cDevelopSpeed;							// 개발 속도 (단위 : 분)
	unsigned long	m_dwUpgradeGold;							// 업그레이드 비용
	unsigned char	m_cUpgradeSpeed;							// 업그레이드 속도 (단위 : 분)
	
	float	m_fDevelopGoldDown;									// 개발 비용 절감 수치 (%)
	float	m_fDevelopSpeedUp;									// 개발 속도 향상 수치 (%)
	float	m_fUpgradeGoldDown;									// 업그레이드 비용 절감 수치 (%)
	float	m_fUpgradeSpeedUp;									// 업그레이드 속도 향상 수치 (%)
	float	m_fDefenseUp;										// 방어력 향상 수치 (%)
	float	m_fOffenseUp;										// 공격력 향상 수치 (%)
	float	m_fHPUp;											// HP 향상 수치 (%)
	float	m_fBonusRate;										// 총 수입 보너스 (%)
	float	m_fRespawnSpeedUp;									// PC 리스폰 속도 향상 수치 (%)

	// 국가전 석상 효과 관련
	unsigned char	m_cStatueEffectType;						// 석상 효과 타입
	unsigned char	m_cStatueEffectPercent;						// 석상 효과 퍼센트
	bool			m_bStatueRespawnEnable;						// 석상 리스폰 가능 여부
		
	MonsterInfo();
    static MonsterPattern GetMonsterPattern(const char* szMonsterType);
	static StatueEffectType	GetStatueEffectType(const char* szStatueEffectType);

	static unsigned short	GetCompleteStatueKID(unsigned short wNowKID);
	static unsigned short	GetLoadingStatueKID(unsigned char cRace, unsigned short wNowKID);
	static unsigned short	GetDefaultStatueKID(unsigned short wNowKID);
	static bool				IsLoadingStatueKID(unsigned short wNowKID);
	static bool				IsRaceCompleteStatueKID(unsigned short wNowKID);
};


#pragma pack()

#endif

#ifndef	_ENEMYCHECK_H_
#define	_ENEMYCHECK_H_

#include <Pattern/Singleton.h>

#include "CreatureStructure.h"

// 적아군 식별 관련 (서버, 클라이언트 공용으로 사용)
namespace EnemyCheck
{
	enum EnemyType
	{
		// 리턴 타입
		EC_ENEMY			= 0,	// 적군
		EC_NEUTRAL			= 1,	// 중립
		EC_FRIEND			= 2,	// 아군

		EC_ERROR			= 3		// 에러
	};

	enum EnemyCheckType
	{
		// 체크 타입
		EC_CHECK_ENEMY				= 0,	// 적식별
		EC_CHECK_NEUTRAL_EXCETION	= 1,	// 중립 예외 처리
		EC_CHECK_FRIEND				= 2,	// 아군 식별
		EC_CHECK_FRIEND_2ND			= 3,	// 2차 아군 식별

		MAX_CHECK_TYPE				= 4		// 식별룰 체크 루틴 수
	};

	enum EnemyCheckRule
	{
		RULE_HUNTING		= 0,	// 헌팅룰
		RULE_GUILD_WAR		= 1,	// 길드전룰
		RULE_REALM_WAR		= 2,	// 렐름전룰
		RULE_STATUE_WAR		= 3,	// 석상전룰
		RULE_SIEGE_WAR		= 4,	// 공성전룰

		MAX_RULE_NUM		= 5
	};

	// -------------------------------------------------------------------------
	// Siege 관련 함수

	bool	IsCamp(unsigned short wObjectType);						// 길드 요새
	bool	IsCastleArms(unsigned short wObjectType);				// 수성 병기
	bool	IsSiegeArms(unsigned short wObjectType);				// 공성 병기
	bool	IsStruct(unsigned short wKind);							// 스트럭쳐

	bool	IsGate(unsigned short wObjectType);						// 성문
	bool	IsEmblem(unsigned short wObjectType);					// 성 상징물
	bool	IsCastleNPC(unsigned short wObjectType);				// 병기 관리 NPC


	// -------------------------------------------------------------------------
	// 양국 체제 이후 추가된 내용

	typedef bool (*FnProcess)(BattleInclination::CharData& ownerInfo, BattleInclination::CharData& targetInfo, 
		BattleInclination::RelationData& relationInfo, unsigned char& cResult);

	typedef bool (*FnCheck)(BattleInclination::CharData& ownerInfo, BattleInclination::CharData& targetInfo,
		EnemyCheck::EnemyCheckRule eRule);

	class CCheckTable : public CSingleton<CCheckTable>
	{
	public:     

		bool Initialize(void);

		void SetEnemyCheckRule(EnemyCheckRule eRule)	{ m_eRule = eRule;	}
		EnemyCheckRule GetEnemyCheckRule(void)			{ return m_eRule;	}

		unsigned long IsEnemyFromStruct(BattleInclination::CharData& ownerInfo, BattleInclination::CharData& targetInfo, 
			BattleInclination::RelationData& relationInfo, unsigned char& cResult);

	private:

		CCheckTable();
		~CCheckTable();

		unsigned long	GetEnemyType(unsigned char cEnemyCheckType) const;	// 체크 타입에 해당하는 적식별 결과 리턴
		
		bool				m_bInit;										// 초기화 여부

		EnemyCheckRule		m_eRule;										// 현재 적용 중인 피아식별 룰
		FnProcess			m_fnIsEnemy[MAX_CHECK_TYPE][MAX_RULE_NUM];		// 각 상황에 따른 피아식별 함수들의 테이블
		FnCheck				m_fnCheckSkip[MAX_CHECK_TYPE];					// 각 상황에 따른 피아식별을 스킵해야 하는지 체크하는 함수들의 테이블
		
		static CCheckTable	ms_this;
	};

	class CFunctions
	{
	private:

		// 결과값에 의미를 둔다기보단, 숫자가 겹치는 걸 방지하기 위해 enum으로 설정
		enum ResultType
		{
			// EC_CHECK_ENEMY 의 결과값
			EN_COMMON_PVP				= 1,				// 명시적 PvP로 선언된 적 (듀얼)
			EN_COMMON_STATELESS_MONSTER,					// 무국적(제3국) 몬스터
			EN_HUNTING_VSPARTY_VSGUILD,						// 명시적 선언으로 선언된 그룹 덩어리 (파티/길드 듀얼)
			EN_GUILDWAR_HOSTILITY_GUILD,					// 자신과 대상이 길드전에 참여중이며 적대 관계 일때
			EN_REALMWAR_ENEMY_PC_MONSTER_NPC,				// 상대 국적의 PC, 몬스터, NPC
			EN_STATUEWAR_ENEMY_PC_MONSTER_STRUCT,			// 상대 국적의 PC, 몬스터, 스트럭처
			EN_STATUEWAR_STATELESS_STRUCT,					// 제 3국적 스트럭처
			EN_SIEGE_ENEMY_PC_MONSTER,						// 상대 국적의 PC, 몬스터
			EN_SIEGE_ENEMY_GATE_EMBLEM_CASTLE_ARMS,			// 상대 국적의 성문, 성 상징물, 수성 병기
			EN_SIEGE_ENEMY_SIEGE_ARMS,						// 상대 국적이 소환한 공성 병기.



			// EC_CHECK_NEUTRAL_EXCETION 의 결과값
			NE_COMMON_GMODE				= 21,				// 상대가 본인이 아니며 G모드일 경우 중립 처리
			NE_COMMON_ENEMY_NPC,							// 상대 국적의 NPC
			
			NE_HUNTING_NON_BATTLER,							// 전투에 참여하지 않은 모든 인원
			NE_HUNTING_OTHER_BATTLER,						// 자신과 다른 팀배틀에 참여중인 모든 인원
			NE_HUNTING_BATTLER,								// 팀배틀에 참여중인 모든 인원
			
			NE_GUILDWAR_IN_SAFETYZONE,						// 내가 세이프티 존에 있을 경우 나와 적대 관계인 모든 인원
			NE_GUILDWAR_NON_BATTLER,						// 길드전 참여중이지 않은 모든 인원
			NE_GUILDWAR_OTHER_BATTLER,						// 자신의 길드가 아닌 길드전쟁 참여자
			NE_GUILDWAR_BATTLER,							// 길드전 참여중인 모든 인원
			NE_GUILDWAR_MYGUILD,							// 자신의 길드원 전원
			
			NE_REALMWAR_ENEMY_STRUCT,						// 자신과 다른 국적의 석상 스트럭쳐
			NE_REALMWAR_ENEMY_GATE_EMBLEM_SIEGEOBJECT,		// 상대 국적의 성내 소환물, 성의 대문, 소환한 공성 병기

			NE_STATUEWAR_ENEMY_GATE_EMBLEM_SIEGEOBJECT,		// 상대 국적의 성내 소환물, 성의 대문, 소환한 공성 병기

			NE_SIEGE_ENEMY_STRUCT,							// 자신과 다른 국적의 석상 스트럭쳐

			// EC_CHECK_FRIEND 의 결과값
			FR_COMMON_MYPARTY			= 41,				// 명시적으로 선언된 같은 파티
			FR_COMMON_MYGUILD,								// 명시적으로 선언된 같은 길드
			FR_COMMON_MYSELF,								// 자기 자신
			FR_GUILDWAR_MYGUILD,							// 내가 참여한 길드 그룹
			FR_REALMWAR_MYNATION_PC_NPC_MONSTER_STRUCT,		// 자국 국적의 PC, NPC, 몬스터, 석상
			FR_REALMWAR_MYNATION_GATE_EMBLEM_SIEGEOBJECT,	// 자국 국적의 소환물, 성의 대문, 공성 병기.
			FR_STATUEWAR_MYNATION_PC_NPC_MONSTER_STRUCT,	// 자국 국적의 PC, NPC, 몬스터, 석상
			FR_STATUEWAR_MYNATION_GATE_EMBLEM_SIEGEOBJECT,	// 자국 국적의 소환물, 성의 대문, 공성 병기.
			FR_SIEGEWAR_MYNATION_PC_NPC_MONSTER_STRUCT,		// 자국 국적의 PC, NPC, 몬스터, 석상
			FR_SIEGEWAR_MYNATION_SIEGEOBJECT,				// 자국 국적의 소환물, 성의 대문, 수성병기, 공성 병기.
			

			// EC_CHECK_FRIEND_2ND 의 결과값
			FR2_COMMON_MYNATION_PC_NPC	= 61,				// 자국 PC, NPC
			FR2_COMMON_STATELESS_NPC,						// 무국적 NPC
		};


		// 공격 가능 여부 체크를 위한 크리쳐 타입
		enum AttackableCreatureType
		{
			ACT_PC							= 0,	// PC
			ACT_MONSTER						= 1,	// 몬스터
			ACT_EMBLEM						= 2,	// 성 상징물
			ACT_GATE						= 3,	// 성문
			ACT_CAMP						= 4,	// 길드 요새
			ACT_GUARD						= 5,	// 가드
			ACT_SHORT_RANGE_CASTLE_ARMS		= 6,	// 근거리 수성 병기
			ACT_LONG_RANGE_CASTLE_ARMS		= 7,	// 원거리 수성 병기
			ACT_SHORT_RANGE_SIEGE_ARMS		= 8,	// 근거리 공성 병기
			ACT_LONG_RANGE_SIEGE_ARMS		= 9,	// 원거리 공성 병기
			ACT_AIRSHIP						= 10,	// 수송선
			ACT_NONE						= 11,	// 뒷문, NPC, 병기관리NPC

			MAX_ATTACKABLE_CREATURE			= 12,	// 공성 시간 및 일반 시간에 공격가능한 대상 수
		};

		// 공격 가능 여부 리턴값
		enum ReturnACT
		{
			RET_NONE		= 0,					// 공격 불가능
			RET_ALLWAYS		= 1,					// 항상 공격 가능
			RET_DEVELOPING	= 2,					// 개발 중일때만 공격 가능
		};

		const static unsigned char	ms_AttackableTable[MAX_ATTACKABLE_CREATURE][MAX_ATTACKABLE_CREATURE];	// 각 크리쳐 타입에 따른 공격 가능 여부 테이블


		friend class CCheckTable;

		// 피아식별 함수 정의 매크로
		#define DECLARE_ENEMY_CKECK(CheckRule) \
			static bool CheckRule(BattleInclination::CharData& ownerInfo, BattleInclination::CharData& targetInfo, \
			BattleInclination::RelationData& relationInfo, unsigned char& cResult)

		#define DEFINE_ENEMY_CKECK(CheckRule) \
			bool EnemyCheck::CFunctions::##CheckRule( \
			BattleInclination::CharData& ownerInfo, BattleInclination::CharData& targetInfo, \
			BattleInclination::RelationData& relationInfo, unsigned char& cResult)

		#define USE_ENEMY_CKECK(CheckRule) CheckRule(ownerInfo, targetInfo, relationInfo, cResult)


		// 피아식별 루틴 스킵 체크 함수 정의 매크로
		#define DECLARE_SKIP_CKECK(CheckType) \
			static bool CheckType(BattleInclination::CharData& ownerInfo, BattleInclination::CharData& targetInfo, \
			EnemyCheck::EnemyCheckRule eRule)

		#define DEFINE_SKIP_CHECK(CheckType) \
			bool EnemyCheck::CFunctions::##CheckType( \
			BattleInclination::CharData& ownerInfo, BattleInclination::CharData& targetInfo, \
			EnemyCheck::EnemyCheckRule eRule)

		
		// 공격 가능 여부 체크 함수 정의 매크로
		#define DECLARE_ATTACKABLE_CHECK(FuncName) \
			static bool FuncName(BattleInclination::CharData& ownerInfo, BattleInclination::CharData& targetInfo)

		#define DEFINE_ATTACKABLE_CHECK(FuncName) \
			bool EnemyCheck::CFunctions::##FuncName( \
			BattleInclination::CharData& ownerInfo, BattleInclination::CharData& targetInfo)


		// 공통
		DECLARE_ENEMY_CKECK(IsEnemyCommon);
		DECLARE_ENEMY_CKECK(IsNeutralExceptionCommon);
		DECLARE_ENEMY_CKECK(IsFriendCommon);
		DECLARE_ENEMY_CKECK(IsFriend2ndCommon);

		// 헌팅룰
		DECLARE_ENEMY_CKECK(IsEnemyForHunting);
		DECLARE_ENEMY_CKECK(IsNeutralExceptionForHunting);
		DECLARE_ENEMY_CKECK(IsFriendForHunting);
		DECLARE_ENEMY_CKECK(IsFriend2ndForHunting);

		// 길드전룰
		DECLARE_ENEMY_CKECK(IsEnemyForGuildWar);
		DECLARE_ENEMY_CKECK(IsNeutralExceptionForGuildWar);
		DECLARE_ENEMY_CKECK(IsFriendForGuildWar);
		DECLARE_ENEMY_CKECK(IsFriend2ndForGuildWar);

		// 렐름전룰
		DECLARE_ENEMY_CKECK(IsEnemyForRealmWar);
		DECLARE_ENEMY_CKECK(IsNeutralExceptionForRealmWar);
		DECLARE_ENEMY_CKECK(IsFriendForRealmWar);
		DECLARE_ENEMY_CKECK(IsFriend2ndForRealmWar);

		// 석상전룰
		DECLARE_ENEMY_CKECK(IsEnemyForStatueWar);
		DECLARE_ENEMY_CKECK(IsNeutralExceptionForStatueWar);
		DECLARE_ENEMY_CKECK(IsFriendForStatueWar);
		DECLARE_ENEMY_CKECK(IsFriend2ndForStatueWar);

		// 공성전룰
		DECLARE_ENEMY_CKECK(IsEnemyForSiegeWar);
		DECLARE_ENEMY_CKECK(IsNeutralExceptionForSiegeWar);
		DECLARE_ENEMY_CKECK(IsFriendForSiegeWar);
		DECLARE_ENEMY_CKECK(IsFriend2ndForSiegeWar);

		// 피아식별 스킵 체크
		DECLARE_SKIP_CKECK(IsEnemySkip);
		DECLARE_SKIP_CKECK(IsNeutralExceptionSkip);
		DECLARE_SKIP_CKECK(IsFriendSkip);
		DECLARE_SKIP_CKECK(IsFriend2ndSkip);

		// 공격 가능 여부 체크 함수
		DECLARE_ATTACKABLE_CHECK(IsAttackable);

		static unsigned char	FindAttackableCreatureType(BattleInclination::CharData& charInfo);
	};
}

#endif
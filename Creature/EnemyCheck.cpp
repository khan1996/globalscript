
#include <Log/ServerLog.h>

#include <Creature/Siege/SiegeConstants.h>
#include <Creature/Monster/MonsterStructure.h>

#include "EnemyCheck.h"
#include "GMMemory.h"

using namespace EnemyCheck;
using namespace Creature;
using namespace Siege;

CCheckTable   CCheckTable::ms_this;

const unsigned char EnemyCheck::CFunctions::ms_AttackableTable[EnemyCheck::CFunctions::MAX_ATTACKABLE_CREATURE][EnemyCheck::CFunctions::MAX_ATTACKABLE_CREATURE] =
{
//    아래 숫자는 EnemyCheck::CFunctions::ReturnACT 상수 참고!!
//	  PC,	몬스터,	성 상징물,	성문,	길드 요새,	가드,	근방,	원방,	근공,	원공,	수송선,	NONE
	{ 1,	1,		1,			1,		1,			1,		1,		1,		1,		1,		1,		0},		// PC
	{ 1,	1,		0,			0,		0,			0,		0,		0,		0,		0,		0,		0},		// 몬스터
	{ 1,	0,		0,			0,		0,			0,		0,		0,		0,		0,		0,		0},		// 성 상징물
	{ 0,	0,		0,			0,		0,			0,		0,		0,		0,		0,		0,		0},		// 성문
	{ 1,	0,		0,			0,		0,			0,		0,		0,		0,		0,		0,		0},		// 길드 요새
	{ 1,	0,		0,			0,		0,			0,		0,		0,		1,		1,		0,		0},		// 가드
	{ 0,	0,		0,			0,		0,			0,		0,		0,		1,		1,		1,		0},		// 근거리 수성 병기
	{ 1,	0,		0,			0,		0,			0,		0,		0,		1,		1,		1,		0},		// 원거리 수성 병기 (비공정이 바닥에 있을때만 처리는 이후에 추가한다.!!)
	{ 0,	0,		0,			1,		0,			0,		0,		0,		0,		0,		0,		0},		// 근거리 공성 병기 (비공정이 바닥에 있을때만 처리는 이후에 추가한다.!!)
	{ 1,	0,		0,			1,		0,			1,		1,		1,		1,		1,		1,		0},		// 원거리 공성 병기 (비공정이 바닥에 있을때만 처리는 이후에 추가한다.!!)
	{ 0,	0,		0,			0,		0,			0,		0,		0,		0,		0,		0,		0},		// 수송선
	{ 0,	0,		0,			0,		0,			0,		0,		0,		0,		0,		0,		0}		// 뒷문, NPC, 병기관리NPC
} ;

bool EnemyCheck::IsCamp(unsigned short wObjectType)
{
	switch ( wObjectType )
	{
		case Siege::CAMP:
		case Siege::CAMP_SHOP:
		case Siege::MINING_CAMP:
		case Siege::KARTERANT_WEAPON:
		case Siege::MERKADIA_WEAPON:
			return true;
	}

	return false;
}

bool EnemyCheck::IsCastleArms(unsigned short wObjectType)
{
	switch ( wObjectType )
	{
		case Siege::GUARD :	
		case Siege::SHORT_RANGE_CASTLE_ARMS :
		case Siege::LONG_RANGE_CASTLE_ARMS :
		case Siege::EMBLEM:
		case Siege::GATE:	
			return true;
	}

	return false;
}

bool EnemyCheck::IsCastleNPC(unsigned short wObjectType)
{
	if ( Siege::CASTLE_ARMS_NPC == wObjectType )
	{
		return true;
	}

	return false;
}

bool EnemyCheck::IsSiegeArms(unsigned short wObjectType)
{
	switch ( wObjectType )
	{
		case Siege::SHORT_RANGE_SIEGE_ARMS :
		case Siege::LONG_RANGE_SIEGE_ARMS :
		case Siege::AIRSHIP :
			return true;
	}

	return false;
}

bool EnemyCheck::IsEmblem(unsigned short wObjectType)
{
	if(wObjectType==Siege::EMBLEM)
	{
		return true;
	}
	return false;
}

bool EnemyCheck::IsGate(unsigned short wObjectType)
{
	if(wObjectType==Siege::GATE)
	{
		return true;
	}
	return false;
}

bool EnemyCheck::IsStruct(unsigned short wKind)
{
	if (MonsterInfo::MIN_STATUE_KID <= wKind &&
		MonsterInfo::MAX_STATUE_KID >= wKind)
	{
		return true;
	}

	return false;
}


bool EnemyCheck::CCheckTable::Initialize(void)
{
	m_fnIsEnemy[EC_CHECK_ENEMY][RULE_HUNTING]				= CFunctions::IsEnemyForHunting;
	m_fnIsEnemy[EC_CHECK_NEUTRAL_EXCETION][RULE_HUNTING]	= CFunctions::IsNeutralExceptionForHunting;
	m_fnIsEnemy[EC_CHECK_FRIEND][RULE_HUNTING]				= CFunctions::IsFriendForHunting;
	m_fnIsEnemy[EC_CHECK_FRIEND_2ND][RULE_HUNTING]			= CFunctions::IsFriend2ndForHunting;

	m_fnIsEnemy[EC_CHECK_ENEMY][RULE_GUILD_WAR]				= CFunctions::IsEnemyForGuildWar;
	m_fnIsEnemy[EC_CHECK_NEUTRAL_EXCETION][RULE_GUILD_WAR]	= CFunctions::IsNeutralExceptionForGuildWar;
	m_fnIsEnemy[EC_CHECK_FRIEND][RULE_GUILD_WAR]			= CFunctions::IsFriendForGuildWar;
	m_fnIsEnemy[EC_CHECK_FRIEND_2ND][RULE_GUILD_WAR]		= CFunctions::IsFriend2ndForGuildWar;

	m_fnIsEnemy[EC_CHECK_ENEMY][RULE_REALM_WAR]				= CFunctions::IsEnemyForRealmWar;
	m_fnIsEnemy[EC_CHECK_NEUTRAL_EXCETION][RULE_REALM_WAR]	= CFunctions::IsNeutralExceptionForRealmWar;
	m_fnIsEnemy[EC_CHECK_FRIEND][RULE_REALM_WAR]			= CFunctions::IsFriendForRealmWar;
	m_fnIsEnemy[EC_CHECK_FRIEND_2ND][RULE_REALM_WAR]		= CFunctions::IsFriend2ndForRealmWar;

	m_fnIsEnemy[EC_CHECK_ENEMY][RULE_STATUE_WAR]			= CFunctions::IsEnemyForStatueWar;
	m_fnIsEnemy[EC_CHECK_NEUTRAL_EXCETION][RULE_STATUE_WAR]	= CFunctions::IsNeutralExceptionForStatueWar;
	m_fnIsEnemy[EC_CHECK_FRIEND][RULE_STATUE_WAR]			= CFunctions::IsFriendForStatueWar;
	m_fnIsEnemy[EC_CHECK_FRIEND_2ND][RULE_STATUE_WAR]		= CFunctions::IsFriend2ndForStatueWar;

	m_fnIsEnemy[EC_CHECK_ENEMY][RULE_SIEGE_WAR]				= CFunctions::IsEnemyForSiegeWar;
	m_fnIsEnemy[EC_CHECK_NEUTRAL_EXCETION][RULE_SIEGE_WAR]	= CFunctions::IsNeutralExceptionForSiegeWar;
	m_fnIsEnemy[EC_CHECK_FRIEND][RULE_SIEGE_WAR]			= CFunctions::IsFriendForSiegeWar;
	m_fnIsEnemy[EC_CHECK_FRIEND_2ND][RULE_SIEGE_WAR]		= CFunctions::IsFriend2ndForSiegeWar;

	m_fnCheckSkip[EC_CHECK_ENEMY]				= CFunctions::IsEnemySkip;
	m_fnCheckSkip[EC_CHECK_NEUTRAL_EXCETION]	= CFunctions::IsNeutralExceptionSkip;
	m_fnCheckSkip[EC_CHECK_FRIEND]				= CFunctions::IsFriendSkip;
	m_fnCheckSkip[EC_CHECK_FRIEND_2ND]			= CFunctions::IsFriend2ndSkip;

	m_bInit = true;

	return true;
}

unsigned long EnemyCheck::CCheckTable::IsEnemyFromStruct(BattleInclination::CharData& ownerInfo, 
														 BattleInclination::CharData& targetInfo, 
														 BattleInclination::RelationData& relationInfo, 
														 unsigned char& cResult)
{
	if (false == m_bInit)
	{
		ERRLOG0(g_Log, "피아식별 함수 테이블이 초기화되지 않은 상태에서 피아식별을 시도하였습니다.");
		return EC_ERROR;
	}

	if (MAX_RULE_NUM == m_eRule)
	{
		ERRLOG0(g_Log, "피아식별 룰이 결정되지 않은 상태에서 피아식별을 시도하였습니다.");
		return EC_ERROR;
	}

	// 순차적으로 적용된다.
	for (unsigned char cIndex = 0; cIndex < MAX_CHECK_TYPE; ++cIndex)
	{
		if (NULL != m_fnIsEnemy[cIndex][m_eRule])
		{
			if (NULL != m_fnCheckSkip[cIndex] && true == m_fnCheckSkip[cIndex](ownerInfo, targetInfo, m_eRule))
			{
				continue;
			}

			if (true == m_fnIsEnemy[cIndex][m_eRule](ownerInfo, targetInfo, relationInfo, cResult))
			{
				return GetEnemyType(cIndex);
			}
		}
	}

	// 중립 처리
	return EC_NEUTRAL;
}

unsigned long EnemyCheck::CCheckTable::GetEnemyType(unsigned char cEnemyCheckType) const
{
	switch (cEnemyCheckType)
	{
		case EC_CHECK_ENEMY:				return EC_ENEMY;
		case EC_CHECK_NEUTRAL_EXCETION:		return EC_NEUTRAL;
		case EC_CHECK_FRIEND:				return EC_FRIEND;
		case EC_CHECK_FRIEND_2ND:			return EC_FRIEND;

		default:							return EC_NEUTRAL;
	}

	return EC_NEUTRAL;
}


EnemyCheck::CCheckTable::CCheckTable()
:	m_bInit(false), m_eRule(MAX_RULE_NUM)
{
}

EnemyCheck::CCheckTable::~CCheckTable()
{
}

// Rodin : 길드전룰 중 임시 탈퇴원에 대한 처리 필요

// ----------------------------------------------------------------------
// 공통

DEFINE_ENEMY_CKECK(IsEnemyCommon)
{
	if (relationInfo.m_cVSFlag & BattleInclination::VS_CHARACTER)
	{
		// 명시적 PvP로 선언된 적 (듀얼)
		cResult = EN_COMMON_PVP;
		return true;
	}

	if (Creature::STATELESS == targetInfo.m_cNation)
	{
		if (CT_MONSTER == targetInfo.m_cCreatureType)
		{
			// 무국적(제3국) 몬스터
			cResult = EN_COMMON_STATELESS_MONSTER;
			return true;
		}
	}

	return false;
}

DEFINE_ENEMY_CKECK(IsNeutralExceptionCommon)
{
	if (ownerInfo.m_dwCID != targetInfo.m_dwCID && true == targetInfo.IsGODMode())
	{
		// 상대가 본인이 아니며 G모드일 경우 중립 처리
		cResult = NE_COMMON_GMODE;
		return true;
	}

	if (Creature::STATELESS != targetInfo.m_cNation &&
		ownerInfo.m_cNation != targetInfo.m_cNation && 
		(true == ownerInfo.IsPeaceMode() || true == targetInfo.IsPeaceMode()))
	{
		// 상대가 본인이 아니며 G모드일 경우 중립 처리
		cResult = NE_COMMON_GMODE;
		return true;
	}

	if (ownerInfo.m_cNation != targetInfo.m_cNation &&
		Creature::STATELESS != targetInfo.m_cNation &&
		CT_NPC == targetInfo.m_cCreatureType)
	{
		// 상대 국적의 NPC
		cResult = NE_COMMON_ENEMY_NPC;
		return true;
	}

	return false;
}

DEFINE_ENEMY_CKECK(IsFriendCommon)
{
	if (0 != ownerInfo.m_dwPID && 0 != targetInfo.m_dwPID)
	{
		if (ownerInfo.m_dwPID == targetInfo.m_dwPID)
		{
			// 명시적으로 선언된 같은 파티
			cResult = FR_COMMON_MYPARTY;
			return true;
		}
	}

	if (0 != ownerInfo.m_dwGID && 0 != targetInfo.m_dwGID)
	{
		if (ownerInfo.m_dwGID == targetInfo.m_dwGID)
		{
			// 명시적으로 선언된 같은 길드
			cResult = FR_COMMON_MYGUILD;
			return true;
		}
	}

	if (ownerInfo.m_dwCID == targetInfo.m_dwCID)
	{
		// 자기 자신
		cResult = FR_COMMON_MYSELF;
		return true;
	}

	return false;
}

DEFINE_ENEMY_CKECK(IsFriend2ndCommon)
{
	if (ownerInfo.m_cNation == targetInfo.m_cNation)
	{
		if (CT_PC == targetInfo.m_cCreatureType || 
			CT_NPC == targetInfo.m_cCreatureType)
		{
			// 자국 PC, NPC
			cResult = FR2_COMMON_MYNATION_PC_NPC;
			return true;
		}
	}

	if (Creature::STATELESS == targetInfo.m_cNation)
	{
		if (CT_NPC == targetInfo.m_cCreatureType)
		{
			// 무국적 NPC
			cResult = FR2_COMMON_STATELESS_NPC;
			return true;
		}
	}

	return false;
}

// ----------------------------------------------------------------------
// 헌팅룰

DEFINE_ENEMY_CKECK(IsEnemyForHunting)
{
	if (true == USE_ENEMY_CKECK(IsEnemyCommon))
	{
		// 공통
		return true;
	}

	if(CT_SIEGE_OBJECT == ownerInfo.m_cCreatureType ||
		CT_SIEGE_OBJECT == targetInfo.m_cCreatureType)
		return false;

	if (relationInfo.m_cVSFlag & BattleInclination::VS_PARTY || 
		relationInfo.m_cVSFlag & BattleInclination::VS_GUILD)
	{
		// 명시적 선언으로 선언된 그룹 덩어리 (파티/길드 듀얼)
		cResult = EN_HUNTING_VSPARTY_VSGUILD;
		return true;
	}

	if ( !ownerInfo.IsSafetyZone() && !targetInfo.IsSafetyZone())
	{
		// 내가 적대를 선언했거나 상대로부터 적대를 선언 받은 경우
		if (true == relationInfo.m_bHostileGuild)
		{
			// 자신과 대상이 길드전에 참여중이며 적대 관계 일때
			cResult = EN_GUILDWAR_HOSTILITY_GUILD;
			return true;
		}
	}

	return false;
}

DEFINE_ENEMY_CKECK(IsNeutralExceptionForHunting)
{
	if (true == USE_ENEMY_CKECK(IsNeutralExceptionCommon))
	{
		// 공통
		return true;
	}

	// 팀배틀에 참여중인 PC
	if (true == ownerInfo.IsTeamBattle())
	{
		if (false == targetInfo.IsTeamBattle())
		{
			// 전투에 참여하지 않은 모든 인원
			cResult = NE_HUNTING_NON_BATTLER;
			return true;
		}

		if (!(relationInfo.m_cVSFlag & BattleInclination::VS_PARTY || 
			  relationInfo.m_cVSFlag & BattleInclination::VS_GUILD))
		{
			// 자신과 다른 팀배틀에 참여중인 모든 인원
			cResult = NE_HUNTING_OTHER_BATTLER;
			return true;
		}

	}
	// 팀배틀에 참여중이지 않은 PC
	else
	{
		if (true == targetInfo.IsTeamBattle())
		{
			// 팀배틀에 참여중인 모든 인원
			cResult = NE_HUNTING_BATTLER;
			return true;
		}
	}

	return false;
}

DEFINE_ENEMY_CKECK(IsFriendForHunting)
{
	if (true == USE_ENEMY_CKECK(IsFriendCommon))
	{
		// 공통
		return true;
	}

	return false;
}

DEFINE_ENEMY_CKECK(IsFriend2ndForHunting)
{
	if (true == USE_ENEMY_CKECK(IsFriend2ndCommon))
	{
		// 공통
		return true;
	}

	return false;
}

// ----------------------------------------------------------------------
// 길드전룰

DEFINE_ENEMY_CKECK(IsEnemyForGuildWar)
{
	if (true == USE_ENEMY_CKECK(IsEnemyCommon))
	{
		// 공통
		return true;
	}

	// 길드전일때 길드 오브젝트인데 종족이 다르면 무조건 적대관계이다.
	if(CT_SIEGE_OBJECT == ownerInfo.m_cCreatureType || CT_SIEGE_OBJECT == targetInfo.m_cCreatureType)
	{
//		if (ownerInfo.m_cNation != targetInfo.m_cNation && Creature::STATELESS != targetInfo.m_cNation)
		if(ownerInfo.m_dwGID != targetInfo.m_dwGID)
		{
			// 상대 국적의 PC, 몬스터, NPC
			cResult = EN_REALMWAR_ENEMY_PC_MONSTER_NPC;
			return true;
		}
	}
/*
	// 자신이 길드전 참여자
	if ( !ownerInfo.IsSafetyZone() &&
		 (Creature::WAR_ON == ownerInfo.m_cGuildWarFlag ||
		  Creature::WAR_INSTANCE == ownerInfo.m_cGuildWarFlag) )
		 
	{
		// 대상이 길드전 참여중일때
		if ( !targetInfo.IsSafetyZone() && 
			 (Creature::WAR_ON == targetInfo.m_cGuildWarFlag ||
			  Creature::WAR_INSTANCE == targetInfo.m_cGuildWarFlag) )
		{
			// 내가 적대를 선언했거나 상대로부터 적대를 선언 받은 경우
			if (true == relationInfo.m_bHostileGuild)
			{
				// 자신과 대상이 길드전에 참여중이며 적대 관계 일때
				cResult = EN_GUILDWAR_HOSTILITY_GUILD;
				return true;
			}
		}
		
		if(!targetInfo.IsSafetyZone())
		{
			// edith 2009.06.13 길드전에서는 같은 길드를 제외하고 모두 적
			if(targetInfo.m_cCreatureType == CT_PC)
			{
				if(ownerInfo.m_dwGID == targetInfo.m_dwGID)
					return USE_ENEMY_CKECK(IsEnemyForRealmWar);

				// 우호길드가아닐경우 무조건 적 
				cResult = EN_GUILDWAR_HOSTILITY_GUILD;
				return true;
			}

			// 대상이 길드전에 참여중이지 않을때
			return USE_ENEMY_CKECK(IsEnemyForRealmWar);
		}
	}
	// 자신이 길드전 미참여자
	else if (Creature::WAR_OFF == ownerInfo.m_cGuildWarFlag)
	{
		if(!targetInfo.IsSafetyZone())
		{
			// edith 2009.06.13 길드전에서는 같은 길드를 제외하고 모두 적
			if(targetInfo.m_cCreatureType == CT_PC)
			{
				if(ownerInfo.m_dwGID == targetInfo.m_dwGID)
					return USE_ENEMY_CKECK(IsEnemyForRealmWar);

				// 우호길드가아닐경우 무조건 적 
				cResult = EN_GUILDWAR_HOSTILITY_GUILD;
				return true;
			}
		}

		// 헌팅룰 적용
		return USE_ENEMY_CKECK(IsEnemyForRealmWar);
	}
*/

	// 자신이 길드전 참여자
	if ( !ownerInfo.IsSafetyZone() )		 
	{
		// 대상이 길드전 참여중일때
		if ( !targetInfo.IsSafetyZone() )
		{
			// 내가 적대를 선언했거나 상대로부터 적대를 선언 받은 경우
			if (true == relationInfo.m_bHostileGuild)
			{
				// 자신과 대상이 길드전에 참여중이며 적대 관계 일때
				cResult = EN_GUILDWAR_HOSTILITY_GUILD;
				return true;
			}

			// edith 2009.06.13 길드전에서는 같은 길드를 제외하고 모두 적
			if(targetInfo.m_cCreatureType == CT_PC)
			{
				if(ownerInfo.m_dwGID == targetInfo.m_dwGID)
					return USE_ENEMY_CKECK(IsEnemyForRealmWar);

				// 우호길드가아닐경우 무조건 적 
				cResult = EN_GUILDWAR_HOSTILITY_GUILD;
				return true;
			}

			// 대상이 길드전에 참여중이지 않을때
			return USE_ENEMY_CKECK(IsEnemyForRealmWar);
		}
	}
	return false;
}

DEFINE_ENEMY_CKECK(IsNeutralExceptionForGuildWar)
{
	if (true == USE_ENEMY_CKECK(IsNeutralExceptionCommon))
	{
		// 공통
		return true;
	}

	// 자신이 길드전 참여자
	if (Creature::WAR_ON == ownerInfo.m_cGuildWarFlag ||
		Creature::WAR_INSTANCE == ownerInfo.m_cGuildWarFlag)
	{
		if ( ownerInfo.IsSafetyZone() )
		{
			// 내가 적대를 선언했거나 상대로부터 적대를 선언 받은 경우
			if (true == relationInfo.m_bHostileGuild)
			{
				// 내가 세이프티 존에 있을 경우 나와 적대 관계인 모든 인원
				cResult = NE_GUILDWAR_IN_SAFETYZONE;
				return true;
			}
		}

		if (Creature::WAR_OFF == targetInfo.m_cGuildWarFlag)
		{
			// 길드전 참여중이지 않은 모든 인원
			cResult = NE_GUILDWAR_NON_BATTLER;
			return true;
		}

		if ( ownerInfo.m_dwGID != targetInfo.m_dwGID &&
			 (Creature::WAR_ON == targetInfo.m_cGuildWarFlag || Creature::WAR_INSTANCE == targetInfo.m_cGuildWarFlag) )
		{
			// 자신의 길드가 아닌 길드전쟁 참여자
			cResult = NE_GUILDWAR_OTHER_BATTLER;
			return true;
		}
	}
	// 자신이 길드전 미참여자
	else if (Creature::WAR_OFF == ownerInfo.m_cGuildWarFlag)
	{
		if (Creature::WAR_ON == targetInfo.m_cGuildWarFlag ||
			Creature::WAR_INSTANCE == targetInfo.m_cGuildWarFlag)
		{
			// 길드전 참여중인 모든 인원
			cResult = NE_GUILDWAR_BATTLER;
			return true;
		}

		// 헌팅룰 적용
		return USE_ENEMY_CKECK(IsNeutralExceptionForRealmWar);
	}

	return false;
}

DEFINE_ENEMY_CKECK(IsFriendForGuildWar)
{
	if (true == USE_ENEMY_CKECK(IsFriendCommon))
	{
		// 공통
		return true;
	}

	// 자신이 길드전 참여자
	if (Creature::WAR_ON == ownerInfo.m_cGuildWarFlag ||
		Creature::WAR_INSTANCE == ownerInfo.m_cGuildWarFlag)
	{
		if (ownerInfo.m_dwGID == targetInfo.m_dwGID)
		{
			// 내가 참여한 길드 그룹
			cResult = FR_GUILDWAR_MYGUILD;
			return true;
		}
	}
	// 자신이 길드전 미참여자
	else if (Creature::WAR_OFF == ownerInfo.m_cGuildWarFlag)
	{
		// 헌팅룰 적용
		return USE_ENEMY_CKECK(IsFriendForRealmWar);
	}

	return false;
}

DEFINE_ENEMY_CKECK(IsFriend2ndForGuildWar)
{
	if (true == USE_ENEMY_CKECK(IsFriend2ndCommon))
	{
		// 공통
		return true;
	}

	return false;
}

// ----------------------------------------------------------------------
// 렐름전룰

DEFINE_ENEMY_CKECK(IsEnemyForRealmWar)
{
	if (true == USE_ENEMY_CKECK(IsEnemyCommon))
	{
		// 공통
		return true;
	}

	if(CT_SIEGE_OBJECT == ownerInfo.m_cCreatureType ||
		CT_SIEGE_OBJECT == targetInfo.m_cCreatureType)
		return false;
/*
	if (ownerInfo.m_cNation != targetInfo.m_cNation && Creature::STATELESS != targetInfo.m_cNation)
	{
		if (CT_PC == targetInfo.m_cCreatureType || 
			CT_MONSTER == targetInfo.m_cCreatureType || 
			CT_NPC == targetInfo.m_cCreatureType)
		{
			// 상대 국적의 PC, 몬스터, NPC
			cResult = EN_REALMWAR_ENEMY_PC_MONSTER_NPC;
			return true;
		}
	}
*/

	if ( !ownerInfo.IsSafetyZone() && !targetInfo.IsSafetyZone())
	{
		if (ownerInfo.m_cNation != targetInfo.m_cNation && Creature::STATELESS != targetInfo.m_cNation)
		{
			if (CT_PC == targetInfo.m_cCreatureType || 
				CT_MONSTER == targetInfo.m_cCreatureType || 
				CT_NPC == targetInfo.m_cCreatureType)
			{
				// 상대 국적의 PC, 몬스터, NPC
				cResult = EN_REALMWAR_ENEMY_PC_MONSTER_NPC;
				return true;
			}
		}

		// 내가 적대를 선언했거나 상대로부터 적대를 선언 받은 경우
		if (true == relationInfo.m_bHostileGuild)
		{
			// 자신과 대상이 길드전에 참여중이며 적대 관계 일때
			cResult = EN_GUILDWAR_HOSTILITY_GUILD;
			return true;
		}
	}
	
	return false;
}

DEFINE_ENEMY_CKECK(IsNeutralExceptionForRealmWar)
{
	if (true == USE_ENEMY_CKECK(IsNeutralExceptionCommon))
	{
		// 공통
		return true;
	}

	if (ownerInfo.m_cNation != targetInfo.m_cNation)
	{
		if (CT_STRUCT == targetInfo.m_cCreatureType)
		{
			// 자신과 다른 국적의 석상 스트럭쳐
			cResult = NE_REALMWAR_ENEMY_STRUCT;
			return true;
		}

		if (IsGate(targetInfo.m_wObjectType) ||
			IsEmblem(targetInfo.m_wObjectType) ||
			IsSiegeArms(targetInfo.m_wObjectType))
		{
			// 상대 국적의 성내 소환물, 성의 대문, 소환한 공성 병기.
			cResult = NE_REALMWAR_ENEMY_GATE_EMBLEM_SIEGEOBJECT;
			return true;
		}		
	}



	return false;
}

DEFINE_ENEMY_CKECK(IsFriendForRealmWar)
{
	if (true == USE_ENEMY_CKECK(IsFriendCommon))
	{
		// 공통
		return true;
	}

	if (ownerInfo.m_cNation == targetInfo.m_cNation)
	{
		if (CT_PC == targetInfo.m_cCreatureType || 
			CT_NPC == targetInfo.m_cCreatureType || 
			CT_MONSTER == targetInfo.m_cCreatureType ||
			CT_STRUCT == targetInfo.m_cCreatureType)
		{
			// 자국 국적의 PC, NPC, 몬스터, 석상
			cResult = FR_REALMWAR_MYNATION_PC_NPC_MONSTER_STRUCT;
			return true;
		}

		if (IsGate(targetInfo.m_wObjectType) ||
			IsEmblem(targetInfo.m_wObjectType) ||
			IsSiegeArms(targetInfo.m_wObjectType))
		{
			// 자국 국적의 소환물, 성의 대문, 공성 병기.
			cResult = FR_REALMWAR_MYNATION_GATE_EMBLEM_SIEGEOBJECT;
			return true;
		}		
	}

	return false;
}

DEFINE_ENEMY_CKECK(IsFriend2ndForRealmWar)
{
	if (true == USE_ENEMY_CKECK(IsFriend2ndCommon))
	{
		// 공통
		return true;
	}

	return false;
}

// ----------------------------------------------------------------------
// 석상전룰

DEFINE_ENEMY_CKECK(IsEnemyForStatueWar)
{
	if (true == USE_ENEMY_CKECK(IsEnemyCommon))
	{
		// 공통
		return true;
	}

	if (ownerInfo.m_cNation != targetInfo.m_cNation)
	{
		if (Creature::STATELESS != targetInfo.m_cNation)
		{
			if (CT_PC == targetInfo.m_cCreatureType || 
				CT_MONSTER == targetInfo.m_cCreatureType || 
				CT_STRUCT == targetInfo.m_cCreatureType)
			{
				// 상대 국적의 PC, 몬스터, 스트럭처
				cResult = EN_STATUEWAR_ENEMY_PC_MONSTER_STRUCT;
				return true;
			}
		}
		else
		{
			if (CT_STRUCT == targetInfo.m_cCreatureType)
			{
				// 제 3국적 스트럭처
				cResult = EN_STATUEWAR_STATELESS_STRUCT;
				return true;
			}
		}
	}

	return false;
}

DEFINE_ENEMY_CKECK(IsNeutralExceptionForStatueWar)
{
	if (true == USE_ENEMY_CKECK(IsNeutralExceptionCommon))
	{
		// 공통
		return true;
	}

	if (ownerInfo.m_cNation != targetInfo.m_cNation)
	{
		if (IsGate(targetInfo.m_wObjectType) ||
			IsEmblem(targetInfo.m_wObjectType) ||
			IsSiegeArms(targetInfo.m_wObjectType))
		{
			// 상대 국적의 성내 소환물, 성의 대문, 소환한 공성 병기.
			cResult = NE_STATUEWAR_ENEMY_GATE_EMBLEM_SIEGEOBJECT;
			return true;
		}
	}	

	return false;
}

DEFINE_ENEMY_CKECK(IsFriendForStatueWar)
{
	if (true == USE_ENEMY_CKECK(IsFriendCommon))
	{
		// 공통
		return true;
	}

	if (ownerInfo.m_cNation == targetInfo.m_cNation)
	{
		if (CT_PC == targetInfo.m_cCreatureType || 
			CT_NPC == targetInfo.m_cCreatureType || 
			CT_MONSTER == targetInfo.m_cCreatureType || 
			CT_STRUCT == targetInfo.m_cCreatureType)
		{
			// 자국 국적의 PC, NPC, 몬스터, 석상
			cResult = FR_STATUEWAR_MYNATION_PC_NPC_MONSTER_STRUCT;
			return true;
		}

		if (IsGate(targetInfo.m_wObjectType) ||
			IsEmblem(targetInfo.m_wObjectType) ||
			IsSiegeArms(targetInfo.m_wObjectType))
		{
			// 자국 국적의 소환물, 성의 대문, 공성 병기.
			cResult = FR_STATUEWAR_MYNATION_GATE_EMBLEM_SIEGEOBJECT;
			return true;
		}		
	}

	return false;
}

DEFINE_ENEMY_CKECK(IsFriend2ndForStatueWar)
{
	if (true == USE_ENEMY_CKECK(IsFriend2ndCommon))
	{
		// 공통
		return true;
	}

	return false;
}

// ----------------------------------------------------------------------
// 공성전룰

DEFINE_ENEMY_CKECK(IsEnemyForSiegeWar)
{
	if (true == USE_ENEMY_CKECK(IsEnemyCommon))
	{
		// 공통
		return true;
	}

	if (ownerInfo.m_cNation != targetInfo.m_cNation)
	{
		if (CT_PC == targetInfo.m_cCreatureType || CT_MONSTER == targetInfo.m_cCreatureType)
		{
			// 상대 국적의 PC, 몬스터.
			cResult = EN_SIEGE_ENEMY_PC_MONSTER;					 
			return true;
		}
		
		if (IsCastleArms(targetInfo.m_wObjectType))
		{
			// 상대 국적의 성문, 성 상징물, 수성 병기
			cResult = EN_SIEGE_ENEMY_GATE_EMBLEM_CASTLE_ARMS;
			return true;
		}

		if(IsSiegeArms(targetInfo.m_wObjectType))
		{
			// 상대 국적이 소환한 공성 병기.
			cResult = EN_SIEGE_ENEMY_SIEGE_ARMS;
			return true;
		}
	}


	return false;
}

DEFINE_ENEMY_CKECK(IsNeutralExceptionForSiegeWar)
{
	if (true == USE_ENEMY_CKECK(IsNeutralExceptionCommon))
	{
		// 공통
		return true;
	}

	if (ownerInfo.m_cNation != targetInfo.m_cNation)
	{
		if (CT_STRUCT == targetInfo.m_cCreatureType)
		{
			// 자신과 다른 국적의 석상 스트럭쳐
			cResult = NE_SIEGE_ENEMY_STRUCT;
			return true;
		}
	}

	return false;
}

DEFINE_ENEMY_CKECK(IsFriendForSiegeWar)
{
	if (true == USE_ENEMY_CKECK(IsFriendCommon))
	{
		// 공통
		return true;
	}

	if (ownerInfo.m_cNation == targetInfo.m_cNation)
	{
		if (CT_PC == targetInfo.m_cCreatureType || 
			CT_NPC == targetInfo.m_cCreatureType || 
			CT_MONSTER == targetInfo.m_cCreatureType ||
			CT_STRUCT == targetInfo.m_cCreatureType)
		{
			// 자국 국적의 PC, NPC, 몬스터, 석상
			cResult = FR_SIEGEWAR_MYNATION_PC_NPC_MONSTER_STRUCT;
			return true;
		}

		if (IsSiegeArms(targetInfo.m_wObjectType) ||
			IsCastleArms(targetInfo.m_wObjectType) ||
			IsCastleNPC(targetInfo.m_wObjectType))
		{
			// 자국 국적의 소환물, 성의 대문, 수성 병기, 공성 병기.
			cResult = FR_SIEGEWAR_MYNATION_SIEGEOBJECT;
			return true;
		}		
	}

	return false;
}

DEFINE_ENEMY_CKECK(IsFriend2ndForSiegeWar)
{
	if (true == USE_ENEMY_CKECK(IsFriend2ndCommon))
	{
		// 공통
		return true;
	}

	return false;
}

// ----------------------------------------------------------------------
// 피아식별 스킵 체크
DEFINE_SKIP_CHECK(IsEnemySkip)
{
	if (true == targetInfo.IsGODMode())
	{
		// 상대가 G모드인 경우 적식별 스킵
		return true;
	}

	if (CT_PC == targetInfo.m_cCreatureType && (true == ownerInfo.IsPeaceMode() || true == targetInfo.IsPeaceMode()))
	{
		// 상대가 평화모드일 경우 적식별 스킵
		return true;
	}

	if (false == IsAttackable(ownerInfo, targetInfo))
	{
		// 상대가 공격할 수 없는 크리쳐라면 적식별 스킵
		return true;
	}

	if (RULE_REALM_WAR == eRule || RULE_STATUE_WAR == eRule)
	{
		// 자신이 공성용 오브젝트에 탑승한 경우 적식별 스킵
		if (ownerInfo.m_cRide)
		{
			if (ownerInfo.IsCastleObject() || IsSiegeArms(ownerInfo.m_wObjectType)) 
			{
				return true;
			}
		}
	}
	
	return false;
}

DEFINE_SKIP_CHECK(IsNeutralExceptionSkip)
{
	if (ownerInfo.m_cNation == targetInfo.m_cNation &&
		Creature::STATELESS != targetInfo.m_cNation &&
		CT_NPC == targetInfo.m_cCreatureType)
	{
		// 자국 NPC 의 경우는 중립 예외처리 스킵
		return true;
	}

	if (RULE_REALM_WAR == eRule || RULE_STATUE_WAR == eRule)
	{
		// 자신이 공성용 오브젝트에 탑승한 경우 중립 예외처리 스킵
		if (ownerInfo.m_cRide)
		{
			if (ownerInfo.IsCastleObject() || IsSiegeArms(ownerInfo.m_wObjectType)) 
			{
				return true;
			}
		}
	}

	return false;
}

DEFINE_SKIP_CHECK(IsFriendSkip)
{
	return false;
}

DEFINE_SKIP_CHECK(IsFriend2ndSkip)
{
	return false;
}

// ----------------------------------------------------------------------
// 피아식별 스킵 체크
DEFINE_ATTACKABLE_CHECK(IsAttackable)
{
	unsigned char cOwnerType = FindAttackableCreatureType(ownerInfo);
	unsigned char cTargetType = FindAttackableCreatureType(targetInfo);

	if (RET_ALLWAYS == ms_AttackableTable[cOwnerType][cTargetType])
	{
		return true;
	}
	else if (RET_DEVELOPING == ms_AttackableTable[cOwnerType][cTargetType] && Siege::DEVELOPING == targetInfo.m_cSiegeState)
	{
		return true;
	}

	return false;
}

unsigned char	EnemyCheck::CFunctions::FindAttackableCreatureType(BattleInclination::CharData& charInfo)
{
	unsigned char cType = ACT_NONE;

	switch (charInfo.m_cCreatureType)
	{
		case CT_PC:
			cType = ACT_PC;
			break;

		case CT_NPC:
			cType = ACT_NONE;
			break;

		case CT_MONSTER:
		case CT_SUMMON:
		case CT_STRUCT:
			cType = ACT_MONSTER;
			break;

		case CT_SIEGE_OBJECT:
		{
			switch (charInfo.m_wObjectType)
			{
				case Siege::EMBLEM:
					cType = ACT_EMBLEM;
					break;

				case Siege::GATE:
					cType = ACT_GATE;
					break;

				case Siege::CAMP:
				case Siege::MINING_CAMP:
				case Siege::CAMP_SHOP:
				case Siege::KARTERANT_WEAPON:
				case Siege::MERKADIA_WEAPON:
					cType = ACT_CAMP;
					break;

				case Siege::BACKDOOR:
				case Siege::CASTLE_ARMS_NPC:
					cType = ACT_NONE;
					break;

				case Siege::GUARD:
					cType = ACT_GUARD;
					break;

				case Siege::SHORT_RANGE_CASTLE_ARMS:
					cType = ACT_SHORT_RANGE_CASTLE_ARMS;
					break;

				case Siege::LONG_RANGE_CASTLE_ARMS:
					cType = ACT_LONG_RANGE_CASTLE_ARMS;
					break;

				case Siege::SHORT_RANGE_SIEGE_ARMS:
					cType = ACT_SHORT_RANGE_SIEGE_ARMS;
					break;

				case Siege::LONG_RANGE_SIEGE_ARMS:
					cType = ACT_LONG_RANGE_SIEGE_ARMS;
					break;

				case Siege::AIRSHIP:
					cType = ACT_AIRSHIP;
					break;
			}

			break;
		}
	}

	return cType;
}

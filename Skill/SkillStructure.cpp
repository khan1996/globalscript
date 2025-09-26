#include "stdafx.h"
#include <algorithm>
#include "SkillStructure.h"
#include "GMMemory.h"


const CTypeName Skill::Type::SkillTypes[MAX_SKILL_TYPE] =
{
	CTypeName(Skill::Type::NONE,		"NONE"),
	CTypeName(Skill::Type::PASSIVE,	    "PASSIVE"),
	CTypeName(Skill::Type::INSTANCE,	"INSTANCE"),
	CTypeName(Skill::Type::CAST,		"CAST"),
	CTypeName(Skill::Type::CHANT,	    "CHANT"),
	CTypeName(Skill::Type::ENCHANT,	    "ENCHANT"),
	CTypeName(Skill::Type::ITEM,		"ITEM"),
	CTypeName(Skill::Type::SET,			"SET"), 
	CTypeName(Skill::Type::ACTION,		"ACTION"),
	CTypeName(Skill::Type::ABILITY,		"ABILITY"),
	CTypeName(Skill::Type::GATHER,		"GATHER")
};


const CTypeName Skill::UseLimit::UseLimits[MAX_USE_LIMIT] = 
{
	CTypeName(Skill::UseLimit::NONE,					"NONE"),
	CTypeName(Skill::UseLimit::FOR_FIGHTER,				"FORFIGHTER"),
	CTypeName(Skill::UseLimit::WITH_SHIELD,				"WITHSHIELD"),
	CTypeName(Skill::UseLimit::FOR_FIGHTER_TWOHANDED,	"FORFIGHTERTWOHAND"),
	CTypeName(Skill::UseLimit::FOR_ALL_ONEHAND,			"FORALLONEHAND"),
	CTypeName(Skill::UseLimit::WITH_DAGGER,				"WITHDAGGER"),
	CTypeName(Skill::UseLimit::WITH_THROWING_DAGGER,	"WITHTHROWINGDAGGER"),
	CTypeName(Skill::UseLimit::RANGED,					"RANGED"),
	CTypeName(Skill::UseLimit::LEFT_ARM,				"LEFTARM"),
	CTypeName(Skill::UseLimit::GUARD_ARM,				"GUARDARM"),
	CTypeName(Skill::UseLimit::WITH_WEAPON,				"WITHWEAPON"),
	CTypeName(Skill::UseLimit::ATTACK_ARM,				"ATTACKARM"),
	CTypeName(Skill::UseLimit::GUN_ARM,					"GUNARM"),
	CTypeName(Skill::UseLimit::KNIFE_ARM,				"KNIFEARM"),
	CTypeName(Skill::UseLimit::WITH_CLOW,				"WITHCLOW")
};
						

const CTypeName Skill::StatusLimit::StatusLimits[MAX_STATUS_LIMIT] = 
{
	CTypeName(Skill::StatusLimit::NONE,		"NONE"),
	CTypeName(Skill::StatusLimit::STR,		"STR"),	
	CTypeName(Skill::StatusLimit::DEX,		"DEX"),	
	CTypeName(Skill::StatusLimit::CON,		"CON"),	
	CTypeName(Skill::StatusLimit::INT,		"INT"),	
	CTypeName(Skill::StatusLimit::WIS,		"WIS"),
	CTypeName(Skill::StatusLimit::LEVEL,	"LEVEL")	//--//
};


const CTypeName Skill::Target::TargetTypes[MAX_TARGET_TYPE] = 
{
	CTypeName(Skill::Target::NONE,		    		"NONE"),
	CTypeName(Skill::Target::MELEE,	    			"MELEE"),
	CTypeName(Skill::Target::FRIEND,				"FRIEND"),
	CTypeName(Skill::Target::ENEMY,			    	"ENEMY"),
	CTypeName(Skill::Target::DEAD_FRIEND,		    "FRDEAD"),
	CTypeName(Skill::Target::DEAD_ENEMY,			"ENDEAD"),
	CTypeName(Skill::Target::FRIEND_OBJECT,		    "FROBJ"),
	CTypeName(Skill::Target::ENEMY_OBJECT,		    "ENOBJ"),
	CTypeName(Skill::Target::PARTY,				    "PARTY"),
	CTypeName(Skill::Target::FRIEND_EXCEPT_SELF,	"FREXME"),
	CTypeName(Skill::Target::LINE_ENEMY,			"ENLINE"),
	CTypeName(Skill::Target::SUMMON,				"SUMMON"),
};


Skill::ProtoType::ProtoType() {	Initialize(); }

void Skill::ProtoType::Initialize()
{
    m_eSkillType    = Type::NONE;
    m_eUseLimit     = UseLimit::NONE;
    m_eTargetType   = Target::NONE;

    m_fMinRange = m_fMaxRange = m_fEffectExtent = 0.0f;

    m_bCounter = m_bInterrupt = m_bProtection = m_bGauge = m_bIsClassSkill = false;
    m_cStrikeNum = m_cEndScript = m_cEndCoolDown = 0;
    m_usSkill_ID = m_StartMP = m_LevelMP = m_LockMP = m_StartTick = m_LevelTick = m_usParentSkill = m_usChildSkill = 0;
	m_dwCoolDownTime = 0;

    m_SpriteInfo.Initialize();

	std::fill_n(&m_StatusLimitType[0], int(MAX_LIMIT_NUM), 0);
	std::fill_n(&m_StatusLimitValue[0], int(MAX_LIMIT_NUM), 0);
		
	std::fill_n(&m_szEffectFileName[0], int(MAX_FILE_NAME), 0);
	std::fill_n(&m_szHitFileName[0], int(MAX_FILE_NAME), 0);
	std::fill_n(&m_szCastingFileName[0], int(MAX_FILE_NAME), 0);

	std::fill_n(&m_szSkillDescribe[0], int(MAX_SKILL_DESCRIBE), 0);
}

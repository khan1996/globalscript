
#include "stdafx.h"

#include <algorithm>

#include <Creature/Character/CharacterClass.h>

#include "MonsterStructure.h"
#include "GMMemory.h"

MonsterInfo::MonsterInfo()
:	m_dwKID(0), m_dwRespawnTime(0), m_fSize(0), m_fAttackAngle(0), m_cSkillPattern(0), m_cSkillLevel(0), 
	m_cNation(Creature::STATELESS), m_cFixLevelGap(0), m_bFixLevelGap(false), m_bCollision(false),
	m_bFirstAttack(false), m_bReturnPosition(false), 
	m_cEnchantSpellType(0), m_cChantSpellType(0), m_wSkillUseRate(0), m_fSkillEffectSize(1.0f),
	m_cStatueEffectType(SE_NONE), m_cStatueEffectPercent(0), m_bStatueRespawnEnable(false)
{
	std::fill_n(m_aryAwardItem, int(MAX_ORIGINAL_ITEM_NUM), 0);
	std::fill_n(m_aryDropRate, int(MAX_AWARD_KIND), 0);
	std::fill_n(m_strName, int(MAX_NAME_LEN), 0);
	std::fill_n(m_strModelingFlag, int(MAX_MODELING_FLAG_LENGTH), 0);	
	std::fill_n(m_fHitBox, int(MAX_HITBOX_NUM), 0.0f);
	std::fill_n(m_wSkillID, int(MAX_SKILL_PATTERN), 0);
}


MonsterInfo::MonsterPattern  MonsterInfo::GetMonsterPattern(const char* szMonsterType)
{
    struct TypeAndName
    {
        const char*             m_szName;
        const MonsterPattern    m_MonsterPattern;

        TypeAndName(const char* szName, const MonsterPattern ePattern) 
            :   m_szName(szName), m_MonsterPattern(ePattern) { }
    };

    static TypeAndName  monsterTypeName[MAX_PATTERN] = 
    {
        TypeAndName("Common",  		PATTERN_COMMON),
        TypeAndName("Warrior",  	PATTERN_WARRIOR),
        TypeAndName("Defender", 	PATTERN_DEFENDER),
        TypeAndName("Mage",     	PATTERN_MAGE),
        TypeAndName("Acolyte",  	PATTERN_ACOLYTE),
        TypeAndName("Boss",     	PATTERN_BOSS),
        TypeAndName("BG",       	PATTERN_BG), 
        TypeAndName("Summon",   	PATTERN_SUMMON), 
        TypeAndName("Structure",	PATTERN_STRUCTURE),
		TypeAndName("Named",		PATTERN_NAMED),
		TypeAndName("Chief",		PATTERN_CHIEF),
		TypeAndName("Object",		PATTERN_OBJECT),
		TypeAndName("Guard",		PATTERN_GUARD),
		TypeAndName("Gather",		PATTERN_GATHER)		
    };

    TypeAndName* lpTypeNamePastEnd = monsterTypeName + MAX_PATTERN;
    for (TypeAndName* lpTypeName = monsterTypeName; lpTypeName != lpTypeNamePastEnd; ++lpTypeName)        
    {
        if (0 == strcmp(szMonsterType, lpTypeName->m_szName))
        {
            return lpTypeName->m_MonsterPattern;
        }
    }

    return PATTERN_COMMON;
}

MonsterInfo::StatueEffectType	MonsterInfo::GetStatueEffectType(const char* szStatueEffectType)
{
	struct TypeAndName
	{
		const char*				m_szName;
		const StatueEffectType	m_StatueEffectType;

		TypeAndName(const char* szName, const StatueEffectType eEffectType)
			:   m_szName(szName), m_StatueEffectType(eEffectType) { }
	};

	static TypeAndName  effectTypeName[MAX_EFFECT_TYPE_NUM] = 
	{
		TypeAndName("None",			SE_NONE),
		TypeAndName("HP",			SE_HP),
		TypeAndName("MP",			SE_MP),
		TypeAndName("EXP",			SE_EXP),
		TypeAndName("DropRate",		SE_DROPRATE)
	};

	TypeAndName* lpTypeNamePastEnd = effectTypeName + MAX_EFFECT_TYPE_NUM;
	for (TypeAndName* lpTypeName = effectTypeName; lpTypeName != lpTypeNamePastEnd; ++lpTypeName)        
	{
		if (0 == strcmp(szStatueEffectType, lpTypeName->m_szName))
		{
			return lpTypeName->m_StatueEffectType;
		}
	}

	return SE_NONE;
}

unsigned short	MonsterInfo::GetCompleteStatueKID(unsigned short wNowKID)
{
	switch (wNowKID)
	{
		// 1레벨
		case MonsterInfo::STATUE_HUMAN_LOADING1:			return MonsterInfo::STATUE_HUMAN_COMPLETE1;			break;
		case MonsterInfo::STATUE_AKHAN_LOADING1:			return MonsterInfo::STATUE_AKHAN_COMPLETE1;			break;
		case MonsterInfo::BG_STATUE_HUMAN_LOADING1:			return MonsterInfo::BG_STATUE_HUMAN_COMPLETE1;		break;
		case MonsterInfo::BG_STATUE_AKHAN_LOADING1:			return MonsterInfo::BG_STATUE_AKHAN_COMPLETE1;		break;
		case MonsterInfo::POWER_STATUE_HUMAN_LOADING1:		return MonsterInfo::POWER_STATUE_HUMAN_COMPLETE1;	break;
		case MonsterInfo::POWER_STATUE_AKHAN_LOADING1:		return MonsterInfo::POWER_STATUE_AKHAN_COMPLETE1;	break;
		case MonsterInfo::INT_STATUE_HUMAN_LOADING1:		return MonsterInfo::INT_STATUE_HUMAN_COMPLETE1;		break;
		case MonsterInfo::INT_STATUE_AKHAN_LOADING1:		return MonsterInfo::INT_STATUE_AKHAN_COMPLETE1;		break;
		case MonsterInfo::EXP_STATUE_HUMAN_LOADING1:		return MonsterInfo::EXP_STATUE_HUMAN_COMPLETE1;		break;
		case MonsterInfo::EXP_STATUE_AKHAN_LOADING1:		return MonsterInfo::EXP_STATUE_AKHAN_COMPLETE1;		break;
		case MonsterInfo::WEALTH_STATUE_HUMAN_LOADING1:		return MonsterInfo::WEALTH_STATUE_HUMAN_COMPLETE1;	break;
		case MonsterInfo::WEALTH_STATUE_AKHAN_LOADING1:		return MonsterInfo::WEALTH_STATUE_AKHAN_COMPLETE1;	break;
		case MonsterInfo::LIFE_EXTRACT_HUMAN_LOADING1:		return MonsterInfo::LIFE_EXTRACT_HUMAN_COMPLETE1;	break;
		case MonsterInfo::LIFE_EXTRACT_AKHAN_LOADING1:		return MonsterInfo::LIFE_EXTRACT_AKHAN_COMPLETE1;	break;
		// 2레벨
		case MonsterInfo::STATUE_HUMAN_LOADING2:			return MonsterInfo::STATUE_HUMAN_COMPLETE2;			break;
		case MonsterInfo::STATUE_AKHAN_LOADING2:			return MonsterInfo::STATUE_AKHAN_COMPLETE2;			break;
		case MonsterInfo::BG_STATUE_HUMAN_LOADING2:			return MonsterInfo::BG_STATUE_HUMAN_COMPLETE2;		break;
		case MonsterInfo::BG_STATUE_AKHAN_LOADING2:			return MonsterInfo::BG_STATUE_AKHAN_COMPLETE2;		break;
		case MonsterInfo::POWER_STATUE_HUMAN_LOADING2:		return MonsterInfo::POWER_STATUE_HUMAN_COMPLETE2;	break;
		case MonsterInfo::POWER_STATUE_AKHAN_LOADING2:		return MonsterInfo::POWER_STATUE_AKHAN_COMPLETE2;	break;
		case MonsterInfo::INT_STATUE_HUMAN_LOADING2:		return MonsterInfo::INT_STATUE_HUMAN_COMPLETE2;		break;
		case MonsterInfo::INT_STATUE_AKHAN_LOADING2:		return MonsterInfo::INT_STATUE_AKHAN_COMPLETE2;		break;
		case MonsterInfo::EXP_STATUE_HUMAN_LOADING2:		return MonsterInfo::EXP_STATUE_HUMAN_COMPLETE2;		break;
		case MonsterInfo::EXP_STATUE_AKHAN_LOADING2:		return MonsterInfo::EXP_STATUE_AKHAN_COMPLETE2;		break;
		case MonsterInfo::WEALTH_STATUE_HUMAN_LOADING2:		return MonsterInfo::WEALTH_STATUE_HUMAN_COMPLETE2;	break;
		case MonsterInfo::WEALTH_STATUE_AKHAN_LOADING2:		return MonsterInfo::WEALTH_STATUE_AKHAN_COMPLETE2;	break;
		case MonsterInfo::LIFE_EXTRACT_HUMAN_LOADING2:		return MonsterInfo::LIFE_EXTRACT_HUMAN_COMPLETE2;	break;
		case MonsterInfo::LIFE_EXTRACT_AKHAN_LOADING2:		return MonsterInfo::LIFE_EXTRACT_AKHAN_COMPLETE2;	break;
	}

	return wNowKID;
}

unsigned short	MonsterInfo::GetLoadingStatueKID(unsigned char cRace, unsigned short wNowKID)
{
	switch (cRace)
	{
		case CClass::HUMAN:
		{
			// 1레벨
			if (MonsterInfo::STATUE_NEUTRALITY1 <= wNowKID && MonsterInfo::STATUE_AKHAN_COMPLETE1 >= wNowKID)
			{
				return MonsterInfo::STATUE_HUMAN_LOADING1;
			}
			else if (MonsterInfo::BG_STATUE_NEUTRALITY1 <= wNowKID && MonsterInfo::BG_STATUE_AKHAN_COMPLETE1 >= wNowKID)
			{
				return MonsterInfo::BG_STATUE_HUMAN_LOADING1;
			}
			else if (MonsterInfo::POWER_STATUE_NEUTRALITY1 <= wNowKID && MonsterInfo::POWER_STATUE_AKHAN_COMPLETE1 >= wNowKID)
			{
				return MonsterInfo::POWER_STATUE_HUMAN_LOADING1;
			}
			else if (MonsterInfo::INT_STATUE_NEUTRALITY1 <= wNowKID && MonsterInfo::INT_STATUE_AKHAN_COMPLETE1 >= wNowKID)
			{
				return MonsterInfo::INT_STATUE_HUMAN_LOADING1;
			}
			else if (MonsterInfo::EXP_STATUE_NEUTRALITY1 <= wNowKID && MonsterInfo::EXP_STATUE_AKHAN_COMPLETE1 >= wNowKID)
			{
				return MonsterInfo::EXP_STATUE_HUMAN_LOADING1;
			}
			else if (MonsterInfo::WEALTH_STATUE_NEUTRALITY1 <= wNowKID && MonsterInfo::WEALTH_STATUE_AKHAN_COMPLETE1 >= wNowKID)
			{
				return MonsterInfo::WEALTH_STATUE_HUMAN_LOADING1;
			}
			else if (MonsterInfo::LIFE_EXTRACT_NEUTRALITY1 <= wNowKID && MonsterInfo::LIFE_EXTRACT_AKHAN_COMPLETE1 >= wNowKID)
			{
				return MonsterInfo::LIFE_EXTRACT_HUMAN_LOADING1;
			}
			//2레벨
			else if (MonsterInfo::STATUE_NEUTRALITY2 <= wNowKID && MonsterInfo::STATUE_AKHAN_COMPLETE2 >= wNowKID)
			{
				return MonsterInfo::STATUE_HUMAN_LOADING2;
			}
			else if (MonsterInfo::BG_STATUE_NEUTRALITY2 <= wNowKID && MonsterInfo::BG_STATUE_AKHAN_COMPLETE2 >= wNowKID)
			{
				return MonsterInfo::BG_STATUE_HUMAN_LOADING2;
			}
			else if (MonsterInfo::POWER_STATUE_NEUTRALITY2 <= wNowKID && MonsterInfo::POWER_STATUE_AKHAN_COMPLETE2 >= wNowKID)
			{
				return MonsterInfo::POWER_STATUE_HUMAN_LOADING2;
			}
			else if (MonsterInfo::INT_STATUE_NEUTRALITY2 <= wNowKID && MonsterInfo::INT_STATUE_AKHAN_COMPLETE2 >= wNowKID)
			{
				return MonsterInfo::INT_STATUE_HUMAN_LOADING2;
			}
			else if (MonsterInfo::EXP_STATUE_NEUTRALITY2 <= wNowKID && MonsterInfo::EXP_STATUE_AKHAN_COMPLETE2 >= wNowKID)
			{
				return MonsterInfo::EXP_STATUE_HUMAN_LOADING2;
			}
			else if (MonsterInfo::WEALTH_STATUE_NEUTRALITY2 <= wNowKID && MonsterInfo::WEALTH_STATUE_AKHAN_COMPLETE2 >= wNowKID)
			{
				return MonsterInfo::WEALTH_STATUE_HUMAN_LOADING2;
			}
			else if (MonsterInfo::LIFE_EXTRACT_NEUTRALITY2 <= wNowKID && MonsterInfo::LIFE_EXTRACT_AKHAN_COMPLETE2 >= wNowKID)
			{
				return MonsterInfo::LIFE_EXTRACT_HUMAN_LOADING2;
			}
		}
		break;

		case CClass::AKHAN:
		{
			// 1레벨
			if (MonsterInfo::STATUE_NEUTRALITY1 <= wNowKID && MonsterInfo::STATUE_AKHAN_COMPLETE1 >= wNowKID)
			{
				return MonsterInfo::STATUE_AKHAN_LOADING1;
			}
			else if (MonsterInfo::BG_STATUE_NEUTRALITY1 <= wNowKID && MonsterInfo::BG_STATUE_AKHAN_COMPLETE1 >= wNowKID)
			{
				return MonsterInfo::BG_STATUE_AKHAN_LOADING1;
			}
			else if (MonsterInfo::POWER_STATUE_NEUTRALITY1 <= wNowKID && MonsterInfo::POWER_STATUE_AKHAN_COMPLETE1 >= wNowKID)
			{
				return MonsterInfo::POWER_STATUE_AKHAN_LOADING1;
			}
			else if (MonsterInfo::INT_STATUE_NEUTRALITY1 <= wNowKID && MonsterInfo::INT_STATUE_AKHAN_COMPLETE1 >= wNowKID)
			{
				return MonsterInfo::INT_STATUE_AKHAN_LOADING1;
			}
			else if (MonsterInfo::EXP_STATUE_NEUTRALITY1 <= wNowKID && MonsterInfo::EXP_STATUE_AKHAN_COMPLETE1 >= wNowKID)
			{
				return MonsterInfo::EXP_STATUE_AKHAN_LOADING1;
			}
			else if (MonsterInfo::WEALTH_STATUE_NEUTRALITY1 <= wNowKID && MonsterInfo::WEALTH_STATUE_AKHAN_COMPLETE1 >= wNowKID)
			{
				return MonsterInfo::WEALTH_STATUE_AKHAN_LOADING1;
			}
			else if (MonsterInfo::LIFE_EXTRACT_NEUTRALITY1 <= wNowKID && MonsterInfo::LIFE_EXTRACT_AKHAN_COMPLETE1 >= wNowKID)
			{
				return MonsterInfo::LIFE_EXTRACT_AKHAN_LOADING1;
			}
			// 2레벨
			else if (MonsterInfo::STATUE_NEUTRALITY2 <= wNowKID && MonsterInfo::STATUE_AKHAN_COMPLETE2 >= wNowKID)
			{
				return MonsterInfo::STATUE_AKHAN_LOADING2;
			}
			else if (MonsterInfo::BG_STATUE_NEUTRALITY2 <= wNowKID && MonsterInfo::BG_STATUE_AKHAN_COMPLETE2 >= wNowKID)
			{
				return MonsterInfo::BG_STATUE_AKHAN_LOADING2;
			}
			else if (MonsterInfo::POWER_STATUE_NEUTRALITY2 <= wNowKID && MonsterInfo::POWER_STATUE_AKHAN_COMPLETE2 >= wNowKID)
			{
				return MonsterInfo::POWER_STATUE_AKHAN_LOADING2;
			}
			else if (MonsterInfo::INT_STATUE_NEUTRALITY2 <= wNowKID && MonsterInfo::INT_STATUE_AKHAN_COMPLETE2 >= wNowKID)
			{
				return MonsterInfo::INT_STATUE_AKHAN_LOADING2;
			}
			else if (MonsterInfo::EXP_STATUE_NEUTRALITY2 <= wNowKID && MonsterInfo::EXP_STATUE_AKHAN_COMPLETE2 >= wNowKID)
			{
				return MonsterInfo::EXP_STATUE_AKHAN_LOADING2;
			}
			else if (MonsterInfo::WEALTH_STATUE_NEUTRALITY2 <= wNowKID && MonsterInfo::WEALTH_STATUE_AKHAN_COMPLETE2 >= wNowKID)
			{
				return MonsterInfo::WEALTH_STATUE_AKHAN_LOADING2;
			}
			else if (MonsterInfo::LIFE_EXTRACT_NEUTRALITY2 <= wNowKID && MonsterInfo::LIFE_EXTRACT_AKHAN_COMPLETE2 >= wNowKID)
			{
				return MonsterInfo::LIFE_EXTRACT_AKHAN_LOADING2;
			}

		}
		break;
	}

	return GetDefaultStatueKID(wNowKID);
}

unsigned short	MonsterInfo::GetDefaultStatueKID(unsigned short wNowKID)
{
	// 1레벨
	if (MonsterInfo::STATUE_NEUTRALITY1 <= wNowKID && MonsterInfo::STATUE_AKHAN_COMPLETE1 >= wNowKID)
	{
		return MonsterInfo::STATUE_NEUTRALITY1;
	}
	else if (MonsterInfo::BG_STATUE_NEUTRALITY1 <= wNowKID && MonsterInfo::BG_STATUE_AKHAN_COMPLETE1 >= wNowKID)
	{
		return MonsterInfo::BG_STATUE_NEUTRALITY1;
	}
	else if (MonsterInfo::POWER_STATUE_NEUTRALITY1 <= wNowKID && MonsterInfo::POWER_STATUE_AKHAN_COMPLETE1 >= wNowKID)
	{
		return MonsterInfo::POWER_STATUE_NEUTRALITY1;
	}
	else if (MonsterInfo::INT_STATUE_NEUTRALITY1 <= wNowKID && MonsterInfo::INT_STATUE_AKHAN_COMPLETE1 >= wNowKID)
	{
		return MonsterInfo::INT_STATUE_NEUTRALITY1;
	}
	else if (MonsterInfo::EXP_STATUE_NEUTRALITY1 <= wNowKID && MonsterInfo::EXP_STATUE_AKHAN_COMPLETE1 >= wNowKID)
	{
		return MonsterInfo::EXP_STATUE_NEUTRALITY1;
	}
	else if (MonsterInfo::WEALTH_STATUE_NEUTRALITY1 <= wNowKID && MonsterInfo::WEALTH_STATUE_AKHAN_COMPLETE1 >= wNowKID)
	{
		return MonsterInfo::WEALTH_STATUE_NEUTRALITY1;
	}
	else if (MonsterInfo::LIFE_EXTRACT_NEUTRALITY1 <= wNowKID && MonsterInfo::LIFE_EXTRACT_AKHAN_COMPLETE1 >= wNowKID)
	{
		return MonsterInfo::LIFE_EXTRACT_NEUTRALITY1;
	}
	// 2레벨
	else if (MonsterInfo::STATUE_NEUTRALITY2 <= wNowKID && MonsterInfo::STATUE_AKHAN_COMPLETE2 >= wNowKID)
	{
		return MonsterInfo::STATUE_NEUTRALITY2;
	}
	else if (MonsterInfo::BG_STATUE_NEUTRALITY2 <= wNowKID && MonsterInfo::BG_STATUE_AKHAN_COMPLETE2 >= wNowKID)
	{
		return MonsterInfo::BG_STATUE_NEUTRALITY2;
	}
	else if (MonsterInfo::POWER_STATUE_NEUTRALITY2 <= wNowKID && MonsterInfo::POWER_STATUE_AKHAN_COMPLETE2 >= wNowKID)
	{
		return MonsterInfo::POWER_STATUE_NEUTRALITY2;
	}
	else if (MonsterInfo::INT_STATUE_NEUTRALITY2 <= wNowKID && MonsterInfo::INT_STATUE_AKHAN_COMPLETE2 >= wNowKID)
	{
		return MonsterInfo::INT_STATUE_NEUTRALITY2;
	}
	else if (MonsterInfo::EXP_STATUE_NEUTRALITY2 <= wNowKID && MonsterInfo::EXP_STATUE_AKHAN_COMPLETE2 >= wNowKID)
	{
		return MonsterInfo::EXP_STATUE_NEUTRALITY2;
	}
	else if (MonsterInfo::WEALTH_STATUE_NEUTRALITY2 <= wNowKID && MonsterInfo::WEALTH_STATUE_AKHAN_COMPLETE2 >= wNowKID)
	{
		return MonsterInfo::WEALTH_STATUE_NEUTRALITY2;
	}
	else if (MonsterInfo::LIFE_EXTRACT_NEUTRALITY2 <= wNowKID && MonsterInfo::LIFE_EXTRACT_AKHAN_COMPLETE2 >= wNowKID)
	{
		return MonsterInfo::LIFE_EXTRACT_NEUTRALITY2;
	}



	return wNowKID;
}

bool	MonsterInfo::IsLoadingStatueKID(unsigned short wNowKID)
{
	switch (wNowKID)
	{
		// 1레벨
		case MonsterInfo::STATUE_HUMAN_LOADING1:
		case MonsterInfo::STATUE_AKHAN_LOADING1:
		case MonsterInfo::BG_STATUE_HUMAN_LOADING1:
		case MonsterInfo::BG_STATUE_AKHAN_LOADING1:
		case MonsterInfo::POWER_STATUE_HUMAN_LOADING1:
		case MonsterInfo::POWER_STATUE_AKHAN_LOADING1:
		case MonsterInfo::INT_STATUE_HUMAN_LOADING1:
		case MonsterInfo::INT_STATUE_AKHAN_LOADING1:
		case MonsterInfo::EXP_STATUE_HUMAN_LOADING1:
		case MonsterInfo::EXP_STATUE_AKHAN_LOADING1:
		case MonsterInfo::WEALTH_STATUE_HUMAN_LOADING1:
		case MonsterInfo::WEALTH_STATUE_AKHAN_LOADING1:
		case MonsterInfo::LIFE_EXTRACT_HUMAN_LOADING1:
		case MonsterInfo::LIFE_EXTRACT_AKHAN_LOADING1:
		// 2레벨
		case MonsterInfo::STATUE_HUMAN_LOADING2:
		case MonsterInfo::STATUE_AKHAN_LOADING2:
		case MonsterInfo::BG_STATUE_HUMAN_LOADING2:
		case MonsterInfo::BG_STATUE_AKHAN_LOADING2:
		case MonsterInfo::POWER_STATUE_HUMAN_LOADING2:
		case MonsterInfo::POWER_STATUE_AKHAN_LOADING2:
		case MonsterInfo::INT_STATUE_HUMAN_LOADING2:
		case MonsterInfo::INT_STATUE_AKHAN_LOADING2:
		case MonsterInfo::EXP_STATUE_HUMAN_LOADING2:
		case MonsterInfo::EXP_STATUE_AKHAN_LOADING2:
		case MonsterInfo::WEALTH_STATUE_HUMAN_LOADING2:
		case MonsterInfo::WEALTH_STATUE_AKHAN_LOADING2:
		case MonsterInfo::LIFE_EXTRACT_HUMAN_LOADING2:
		case MonsterInfo::LIFE_EXTRACT_AKHAN_LOADING2:
			return true;

		default:
			return false;
	}

	return false;
}

bool	MonsterInfo::IsRaceCompleteStatueKID(unsigned short wNowKID)
{
	switch (wNowKID)
	{
		// 1레벨
		case MonsterInfo::STATUE_HUMAN_COMPLETE1:
		case MonsterInfo::STATUE_AKHAN_COMPLETE1:
		case MonsterInfo::BG_STATUE_HUMAN_COMPLETE1:
		case MonsterInfo::BG_STATUE_AKHAN_COMPLETE1:
		case MonsterInfo::POWER_STATUE_HUMAN_COMPLETE1:
		case MonsterInfo::POWER_STATUE_AKHAN_COMPLETE1:
		case MonsterInfo::INT_STATUE_HUMAN_COMPLETE1:
		case MonsterInfo::INT_STATUE_AKHAN_COMPLETE1:
		case MonsterInfo::EXP_STATUE_HUMAN_COMPLETE1:
		case MonsterInfo::EXP_STATUE_AKHAN_COMPLETE1:
		case MonsterInfo::WEALTH_STATUE_HUMAN_COMPLETE1:
		case MonsterInfo::WEALTH_STATUE_AKHAN_COMPLETE1:
		case MonsterInfo::LIFE_EXTRACT_HUMAN_COMPLETE1:
		case MonsterInfo::LIFE_EXTRACT_AKHAN_COMPLETE1:
		// 2레벨
		case MonsterInfo::STATUE_HUMAN_COMPLETE2:
		case MonsterInfo::STATUE_AKHAN_COMPLETE2:
		case MonsterInfo::BG_STATUE_HUMAN_COMPLETE2:
		case MonsterInfo::BG_STATUE_AKHAN_COMPLETE2:
		case MonsterInfo::POWER_STATUE_HUMAN_COMPLETE2:
		case MonsterInfo::POWER_STATUE_AKHAN_COMPLETE2:
		case MonsterInfo::INT_STATUE_HUMAN_COMPLETE2:
		case MonsterInfo::INT_STATUE_AKHAN_COMPLETE2:
		case MonsterInfo::EXP_STATUE_HUMAN_COMPLETE2:
		case MonsterInfo::EXP_STATUE_AKHAN_COMPLETE2:
		case MonsterInfo::WEALTH_STATUE_HUMAN_COMPLETE2:
		case MonsterInfo::WEALTH_STATUE_AKHAN_COMPLETE2:
		case MonsterInfo::LIFE_EXTRACT_HUMAN_COMPLETE2:
		case MonsterInfo::LIFE_EXTRACT_AKHAN_COMPLETE2:
			return true;

		default:
			return false;
	}

	return false;
}
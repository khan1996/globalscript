
#include "stdafx.h"

#include <Creature/Character/CharacterStructure.h>
#include <Network/Packet/PacketStruct/CharStatusPacketStruct.h>

#include "CharacterClass.h"
#include "GMMemory.h"


// 홀수 레벨일 때 증가량을 기준으로 작성.
CClass ClassTable[CClass::MAX_CLASS] = 
{
	CClass(),
	CClass(CClass::Fighter,		CClass::Fighter,	CClass::DEFAULT_CLASS,	CClass::HUMAN,	CClass::STR,  1, CClass::CON,	1, false),
	CClass(CClass::Rogue,		CClass::Rogue,		CClass::DEFAULT_CLASS,	CClass::HUMAN,	CClass::DEX,  1, CClass::STR,	1, false),
	CClass(CClass::Mage,		CClass::Mage,		CClass::DEFAULT_CLASS,	CClass::HUMAN,	CClass::INT,  1, CClass::DEX,	1, false),
	CClass(CClass::Acolyte,		CClass::Acolyte,	CClass::DEFAULT_CLASS,	CClass::HUMAN,	CClass::WIS,  1, CClass::CON,	1, false),
	CClass(CClass::Defender,	CClass::Fighter,	CClass::JOB_CHANGE_1ST,	CClass::HUMAN,	CClass::STR,  2, CClass::CON,	1, true),
	CClass(CClass::Warrior,		CClass::Fighter,	CClass::JOB_CHANGE_1ST,	CClass::HUMAN,	CClass::STR,  2, CClass::CON,	1, false),
    CClass(CClass::Assassin,	CClass::Rogue,		CClass::JOB_CHANGE_1ST,	CClass::HUMAN,	CClass::DEX,  2, CClass::STR,	1, false),
	CClass(CClass::Archer,		CClass::Rogue,		CClass::JOB_CHANGE_1ST,	CClass::HUMAN,	CClass::DEX,  2, CClass::STR,	1, true),
	CClass(CClass::Sorcerer,	CClass::Mage,		CClass::JOB_CHANGE_1ST,	CClass::HUMAN,	CClass::INT,  2, CClass::DEX,	1, false),
	CClass(CClass::Enchanter,	CClass::Mage,		CClass::JOB_CHANGE_1ST,	CClass::HUMAN,	CClass::INT,  2, CClass::DEX,	1, true),
	CClass(CClass::Priest,		CClass::Acolyte,	CClass::JOB_CHANGE_1ST,	CClass::HUMAN,	CClass::WIS,  2, CClass::CON,	1, true),
	CClass(CClass::Cleric,		CClass::Acolyte,	CClass::JOB_CHANGE_1ST,	CClass::HUMAN,	CClass::WIS,  2, CClass::CON,	1, false),
	CClass(),																		
	CClass(),																		
	CClass(),																		
	CClass(),																		
	CClass(CClass::Combatant,	CClass::Combatant,	CClass::DEFAULT_CLASS,	CClass::AKHAN,	CClass::STR,  1, CClass::NONE_STAT, 0, false),
	CClass(CClass::Officiator,	CClass::Officiator,	CClass::DEFAULT_CLASS,	CClass::AKHAN,	CClass::DEX,  1, CClass::NONE_STAT, 0, false),
	CClass(CClass::Templar,		CClass::Combatant,	CClass::JOB_CHANGE_1ST,	CClass::AKHAN,	CClass::STR,  2, CClass::CON,	1, true),
	CClass(CClass::Attacker,	CClass::Combatant,	CClass::JOB_CHANGE_1ST,	CClass::AKHAN,	CClass::STR,  2, CClass::CON,	1, false),
	CClass(CClass::Gunner,		CClass::Combatant,	CClass::JOB_CHANGE_1ST,	CClass::AKHAN,	CClass::DEX,  2, CClass::STR,	1, true),
	CClass(CClass::RuneOff,		CClass::Officiator,	CClass::JOB_CHANGE_1ST,	CClass::AKHAN,	CClass::INT,  2, CClass::DEX,	1, false),
	CClass(CClass::LifeOff,		CClass::Officiator,	CClass::JOB_CHANGE_1ST,	CClass::AKHAN,	CClass::WIS,  2, CClass::DEX,	1, false),
	CClass(CClass::ShadowOff,	CClass::Officiator,	CClass::JOB_CHANGE_1ST,	CClass::AKHAN,	CClass::DEX,  2, CClass::STR,	1, false)
};		


CClass::CClass(const JobType eJobType, const JobType ePrevJobType, const JobLevel eJobLevel, const RaceType eRace,	
			   const CClass::StatusType eType1, const unsigned char cIncrement1,
			   const CClass::StatusType eType2, const unsigned char cIncrement2,
			   const bool bLevelSwap)
			   :	m_eJobType(eJobType), m_ePrevJobType(ePrevJobType), m_eJobLevel(eJobLevel), 
					m_eRace(eRace), m_bLevelSwap(bLevelSwap)
{	
	m_eIncrementType[0]		= eType1;		m_eIncrementType[1]		= eType2;
	m_cIncrementValue[0]	= cIncrement1;	m_cIncrementValue[1]	= cIncrement2; 			
}

CClass::CClass()
:	m_eJobType(NONE_JOB), m_ePrevJobType(NONE_JOB), 
	m_eJobLevel(DEFAULT_CLASS), m_eRace(MAX_RACE), m_bLevelSwap(false)
{	
	m_eIncrementType[0]		= NONE_STAT;	m_eIncrementType[1]		= NONE_STAT;
	m_cIncrementValue[0]	= 0;			m_cIncrementValue[1]	= 0; 			
}

void CClass::LevelUp(CHAR_INFOST* InfoSt)
{
	unsigned char cIndex1 = m_bLevelSwap ? ((1 == InfoSt->Level % 2) ? 0 : 1) : 0;
	unsigned char cIndex2 = m_bLevelSwap ? ((1 == InfoSt->Level % 2) ? 1 : 0) : 1;
	IncrementByType(InfoSt, 0, cIndex1);
	IncrementByType(InfoSt, 1, cIndex2);

	if (AKHAN == GetRace(static_cast<unsigned char>(InfoSt->Class)))
	{
		if (JobLevel::JOB_CHANGE_1ST == GetJobLevel(static_cast<unsigned char>(InfoSt->Class)))
		{
			InfoSt->IP += 2;
		}
	}
	else
	{
		InfoSt->IP += 2;
	}
}

unsigned char CClass::GetRequiredIP(unsigned short usClass, unsigned char cType)
{
	if (usClass >= MAX_CLASS || cType >= StatusType::MAX_TYPE) { return 0xFF; }

	if (ClassTable[usClass].m_eJobLevel == JOB_CHANGE_1ST)
	{
		return (ClassTable[usClass].m_eIncrementType[0] == cType ||
			ClassTable[usClass].m_eIncrementType[1] == cType) ? 2 : 1;
	}

	for (int nClassIndex = 0; nClassIndex < MAX_CLASS; nClassIndex++)
	{
		if (ClassTable[nClassIndex].m_eJobType == usClass)
		{
			if (ClassTable[nClassIndex].m_eIncrementType[0] == cType || 
				ClassTable[nClassIndex].m_eIncrementType[1] == cType)
			{
				return 2;
			}
		}
	}

	return 1;
}

bool CClass::JobChange(CharacterDBData* DBData, unsigned char cClassType)
{	
	if (DBData->m_Info.Level < 10) { return false; }

	switch (m_eJobLevel)
	{
		case DEFAULT_CLASS:		return DowngradeClass(DBData, cClassType);
		case JOB_CHANGE_1ST:	return UpgradeClass(DBData, cClassType);
	}

	return false;
}

bool CClass::UpgradeClass(CharacterDBData* DBData, unsigned char cClassType)
{
	if (DBData->m_Info.Class != ClassTable[cClassType].m_ePrevJobType || 
		ClassTable[DBData->m_Info.Class].m_eJobLevel != DEFAULT_CLASS) 
	{
		return false;
	}

	// 환원되는 IP 계산을 위한 임시 변수들
	unsigned short StatusA = 0;
	unsigned short StatusB = 0;
	unsigned short StatusC = 0;

	switch (cClassType) 
	{	
		// 인간
		case Defender:
			DBData->m_Info.STR += static_cast<int>((DBData->m_Info.Level - 1) / 2.0f);
			DBData->m_Info.CON += static_cast<int>((DBData->m_Info.Level - 1) / 2.0f + 0.5f);		

			StatusA = DBData->m_Info.STR - (20 + static_cast<int>((DBData->m_Info.Level - 1) * 1.5f));
			StatusB = DBData->m_Info.CON - (20 + static_cast<int>((DBData->m_Info.Level - 1) * 1.5f + 0.5f));
			StatusC = DBData->m_Info.DEX + DBData->m_Info.INT + DBData->m_Info.WIS - 60;

			DBData->m_Info.Class = Defender;
			break;

		case Warrior:
			DBData->m_Info.STR += DBData->m_Info.Level - 1;	

			StatusA = DBData->m_Info.STR - (20 + (DBData->m_Info.Level - 1) * 2);
			StatusB = DBData->m_Info.CON - (20 + DBData->m_Info.Level - 1);
			StatusC = DBData->m_Info.DEX + DBData->m_Info.INT + DBData->m_Info.WIS - 60;

			DBData->m_Info.Class = Warrior;
			break;

		case Assassin:
			DBData->m_Info.DEX += DBData->m_Info.Level - 1;

			StatusA = DBData->m_Info.DEX - (20 + (DBData->m_Info.Level - 1) * 2);
			StatusB = DBData->m_Info.STR - (20 + DBData->m_Info.Level - 1);
			StatusC = DBData->m_Info.CON + DBData->m_Info.INT + DBData->m_Info.WIS - 60;

			DBData->m_Info.Class = Assassin;
			break;

		case Archer:		
			DBData->m_Info.DEX += static_cast<int>((DBData->m_Info.Level - 1) / 2.0f);
			DBData->m_Info.STR += static_cast<int>((DBData->m_Info.Level - 1) / 2.0f + 0.5f);

			StatusA = DBData->m_Info.DEX - (20 + static_cast<int>((DBData->m_Info.Level - 1) * 1.5f));
			StatusB = DBData->m_Info.STR - (20 + static_cast<int>((DBData->m_Info.Level - 1) * 1.5f + 0.5f));
			StatusC = DBData->m_Info.CON + DBData->m_Info.INT + DBData->m_Info.WIS - 60;

			DBData->m_Info.Class = Archer;
			break;

		case Sorcerer:
			DBData->m_Info.INT += DBData->m_Info.Level - 1;

			StatusA = DBData->m_Info.INT - (20 + (DBData->m_Info.Level - 1) * 2);
			StatusB = DBData->m_Info.DEX - (20 + DBData->m_Info.Level - 1);
			StatusC = DBData->m_Info.STR + DBData->m_Info.CON + DBData->m_Info.WIS - 60;

			DBData->m_Info.Class = Sorcerer;
			break;

		case Enchanter:
			DBData->m_Info.INT += static_cast<int>((DBData->m_Info.Level - 1) / 2.0f);
			DBData->m_Info.DEX += static_cast<int>((DBData->m_Info.Level - 1) / 2.0f + 0.5f);

			StatusA = DBData->m_Info.INT - (20 + static_cast<int>((DBData->m_Info.Level - 1) * 1.5f));
			StatusB = DBData->m_Info.DEX - (20 + static_cast<int>((DBData->m_Info.Level - 1) * 1.5f + 0.5f));
			StatusC = DBData->m_Info.STR + DBData->m_Info.CON + DBData->m_Info.WIS - 60;

			DBData->m_Info.Class = Enchanter;
			break;

		case Priest:
			DBData->m_Info.WIS += static_cast<int>((DBData->m_Info.Level - 1) / 2.0f);
			DBData->m_Info.CON += static_cast<int>((DBData->m_Info.Level - 1) / 2.0f + 0.5f);

			StatusA = DBData->m_Info.WIS - (20 + static_cast<int>((DBData->m_Info.Level - 1) * 1.5f));
			StatusB = DBData->m_Info.CON - (20 + static_cast<int>((DBData->m_Info.Level - 1) * 1.5f + 0.5f));
			StatusC = DBData->m_Info.STR + DBData->m_Info.DEX + DBData->m_Info.INT - 60;

			DBData->m_Info.Class = Priest;
			break;

		case Cleric:
			DBData->m_Info.WIS += DBData->m_Info.Level - 1;

			StatusA = DBData->m_Info.WIS - (20 + (DBData->m_Info.Level - 1) * 2);
			StatusB = DBData->m_Info.CON - (20 + DBData->m_Info.Level - 1);
			StatusC = DBData->m_Info.STR + DBData->m_Info.DEX + DBData->m_Info.INT - 60;

			DBData->m_Info.Class = Cleric;
			break;

		// 아칸
		case Templar:
			DBData->m_Info.STR += static_cast<int>((DBData->m_Info.Level - 1) / 2.0f + 0.5f);
			DBData->m_Info.CON += static_cast<int>((DBData->m_Info.Level - 1) * 1.5f);
			DBData->m_Info.IP += (DBData->m_Info.Level - 1) * 2;

			StatusA = DBData->m_Info.STR - (20 + static_cast<int>((DBData->m_Info.Level - 1) * 1.5f));
			StatusB = DBData->m_Info.CON - (20 + static_cast<int>((DBData->m_Info.Level - 1) * 1.5f + 0.5f));
			StatusC = DBData->m_Info.DEX + DBData->m_Info.INT + DBData->m_Info.WIS - 60;

			DBData->m_Info.Class = Templar;
			break;

		case Attacker:
			DBData->m_Info.STR += DBData->m_Info.Level - 1;	
			DBData->m_Info.CON += DBData->m_Info.Level - 1;	
			DBData->m_Info.IP += (DBData->m_Info.Level - 1) * 2;

			StatusA = DBData->m_Info.STR - (20 + (DBData->m_Info.Level - 1) * 2);
			StatusB = DBData->m_Info.CON - (20 + DBData->m_Info.Level - 1);
			StatusC = DBData->m_Info.DEX + DBData->m_Info.INT + DBData->m_Info.WIS - 60;

			DBData->m_Info.Class = Attacker;
			break;

		case Gunner:
			DBData->m_Info.DEX += static_cast<int>((DBData->m_Info.Level - 1) * 1.5f);
			DBData->m_Info.STR += static_cast<int>((DBData->m_Info.Level - 1) / 2.0f + 0.5f);
			DBData->m_Info.IP += (DBData->m_Info.Level - 1) * 2;

			StatusA = DBData->m_Info.DEX - (20 + static_cast<int>((DBData->m_Info.Level - 1) * 1.5f));
			StatusB = DBData->m_Info.STR - (20 + static_cast<int>((DBData->m_Info.Level - 1) * 1.5f + 0.5f));
			StatusC = DBData->m_Info.CON + DBData->m_Info.INT + DBData->m_Info.WIS - 60;

			DBData->m_Info.Class = Gunner;
			break;

		case RuneOff:
			DBData->m_Info.INT += (DBData->m_Info.Level - 1) * 2;
			DBData->m_Info.IP += (DBData->m_Info.Level - 1) * 2;

			StatusA = DBData->m_Info.INT - (20 + (DBData->m_Info.Level - 1) * 2);
			StatusB = DBData->m_Info.DEX - (20 + DBData->m_Info.Level - 1);
			StatusC = DBData->m_Info.STR + DBData->m_Info.CON + DBData->m_Info.WIS - 60;

			DBData->m_Info.Class = RuneOff;
			break;

		case LifeOff:
			DBData->m_Info.WIS += (DBData->m_Info.Level - 1) * 2;
			DBData->m_Info.IP += (DBData->m_Info.Level - 1) * 2;

			StatusA = DBData->m_Info.WIS - (20 + (DBData->m_Info.Level - 1) * 2);
			StatusB = DBData->m_Info.DEX - (20 + DBData->m_Info.Level - 1);
			StatusC = DBData->m_Info.STR + DBData->m_Info.CON + DBData->m_Info.INT - 60;

			DBData->m_Info.Class = LifeOff;
			break;

		case ShadowOff:
			DBData->m_Info.STR += DBData->m_Info.Level - 1;
			DBData->m_Info.DEX += DBData->m_Info.Level - 1;

			StatusA = DBData->m_Info.DEX - (20 + (DBData->m_Info.Level - 1) * 2);
			StatusB = DBData->m_Info.STR - (20 + DBData->m_Info.Level - 1);
			StatusC = DBData->m_Info.CON + DBData->m_Info.INT + DBData->m_Info.WIS - 60;

/*
			DBData->m_Info.STR += static_cast<int>((DBData->m_Info.Level - 1) * 1.5f + 0.5f);
			DBData->m_Info.DEX += static_cast<int>((DBData->m_Info.Level - 1) / 2.0f);
			DBData->m_Info.IP += (DBData->m_Info.Level - 1) * 2;

			StatusA = DBData->m_Info.STR - (20 + static_cast<int>((DBData->m_Info.Level - 1) * 1.5f));
			StatusB = DBData->m_Info.DEX - (20 + static_cast<int>((DBData->m_Info.Level - 1) * 1.5f + 0.5f));
			StatusC = DBData->m_Info.CON + DBData->m_Info.INT + DBData->m_Info.WIS - 60;
*/
			DBData->m_Info.Class = ShadowOff;
			break;

		default:	
			return false;
	}

	DBData->m_Info.IP += 10 + (DBData->m_Info.Level - 1) * 2 - ((StatusA + StatusB) * 2 + StatusC) - DBData->m_Info.IP;

	return true;
}

bool CClass::DowngradeClass(CharacterDBData* DBData, unsigned char cClassType)
{
	// 현재는 거너만 가능하다.
	if (Gunner != DBData->m_Info.Class) { return false; }

	if (cClassType != ClassTable[DBData->m_Info.Class].m_ePrevJobType || 
		ClassTable[DBData->m_Info.Class].m_eJobLevel != JOB_CHANGE_1ST) 
	{
		return false;
	}

	// 레벨이 10% 감소한다.
	DBData->m_Info.Level -= static_cast<char>(DBData->m_Info.Level * 0.1f);
	DBData->m_Info.Level = max(DBData->m_Info.Level, 10);
	DBData->m_Info.Exp = 0;

	// 명성치가 0이 된다.
	DBData->m_Info.Fame = 0;

	// 스탯이 초기화된다.
	switch (cClassType) 
	{
		case Combatant:
			DBData->m_Info.STR = 20 + DBData->m_Info.Level - 1;
			DBData->m_Info.DEX = 20;
			DBData->m_Info.CON = 20;
			DBData->m_Info.INT = 20;
			DBData->m_Info.WIS = 20;

			DBData->m_Info.Class = Combatant;
			DBData->m_Info.IP = 10;
			break;

		default:	return false;
	}

	// 스킬, 퀵슬롯 초기화
	DBData->m_Skill = SKILL::SKILL();
	DBData->m_Quick = QUICK::QUICK();

	return true;
}


bool CClass::InitializeClass(CharacterDBData* DBData, unsigned char cClassType)
{
	if (ClassTable[cClassType].m_eJobLevel != DEFAULT_CLASS) 
	{
		return false;
	}

	// 레벨이 10% 감소한다.
	DBData->m_Info.Level = 1;
	DBData->m_Info.Exp = 0;

	// 명성치가 0이 된다.
	DBData->m_Info.Fame = 0;

	// 스탯이 초기화된다.
	DBData->m_Info.STR = 20;
	DBData->m_Info.DEX = 20;
	DBData->m_Info.CON = 20;
	DBData->m_Info.INT = 20;
	DBData->m_Info.WIS = 20;
	DBData->m_Info.IP = 10;
	DBData->m_Info.Class = cClassType;

	// 스킬, 퀵슬롯 초기화
	DBData->m_Skill = SKILL::SKILL();
	DBData->m_Quick = QUICK::QUICK();

	return true;
}

bool CClass::CheckState(ChState& State, unsigned char cLevel)
{
	unsigned short wTIP = State.m_wIP;

	switch (m_eJobType)
	{
		case Fighter:
			wTIP += (State.m_wSTR - 20 - (cLevel - 1)) * 2;
			wTIP += (State.m_wDEX - 20);
			wTIP += (State.m_wCON - 20 - (cLevel - 1)) * 2;
			wTIP += (State.m_wINT - 20);
			wTIP += (State.m_wWIS - 20);

			if (wTIP > (10 + (cLevel - 1) * 2)) { return false; }
			break;

		case Rogue:
			wTIP += (State.m_wSTR - 20 - (cLevel - 1)) * 2;
			wTIP += (State.m_wDEX - 20 - (cLevel - 1)) * 2;
			wTIP += (State.m_wCON - 20);
			wTIP += (State.m_wINT - 20);
			wTIP += (State.m_wWIS - 20);

			if (wTIP > (10 + (cLevel - 1) * 2)) { return false; }
			break;

		case Mage:
			wTIP += (State.m_wSTR - 20);
			wTIP += (State.m_wDEX - 20 - (cLevel - 1)) * 2;
			wTIP += (State.m_wCON - 20);
			wTIP += (State.m_wINT - 20 - (cLevel - 1)) * 2;
			wTIP += (State.m_wWIS - 20);

			if (wTIP > (10 + (cLevel - 1) * 2)) { return false; }
			break;

		case Acolyte:				
			wTIP += (State.m_wSTR - 20);
			wTIP += (State.m_wDEX - 20);
			wTIP += (State.m_wCON - 20 - (cLevel - 1)) * 2;
			wTIP += (State.m_wINT - 20);
			wTIP += (State.m_wWIS - 20 - (cLevel - 1)) * 2;

			if (wTIP > (10 + (cLevel - 1) * 2)) { return false; }
			break;

		case Defender:
			wTIP += (State.m_wSTR - 20 - static_cast<unsigned short>((cLevel - 1) * 1.5f)) * 2;
			wTIP += (State.m_wDEX - 20);
			wTIP += (State.m_wCON - 20 - static_cast<unsigned short>((cLevel - 1) * 1.5f + 0.5f)) * 2;
			wTIP += (State.m_wINT - 20);
			wTIP += (State.m_wWIS - 20);

			if (wTIP > (10 + (cLevel - 1) * 2)) { return false; }
			break;

		case Warrior:	
			wTIP += (State.m_wSTR - 20 - (cLevel - 1) * 2) * 2;
			wTIP += (State.m_wDEX - 20);
			wTIP += (State.m_wCON - 20 - (cLevel - 1) * 1) * 2;
			wTIP += (State.m_wINT - 20);
			wTIP += (State.m_wWIS - 20);

			if (wTIP > (10 + (cLevel - 1) * 2)) { return false; }
			break;

		case Assassin:
			wTIP += (State.m_wSTR - 20 - (cLevel - 1) * 1) * 2;
			wTIP += (State.m_wDEX - 20 - (cLevel - 1) * 2) * 2;
			wTIP += (State.m_wCON - 20);
			wTIP += (State.m_wINT - 20);
			wTIP += (State.m_wWIS - 20);

			if (wTIP > (10 + (cLevel - 1) * 2)) { return false; }
			break;

		case Archer:
			wTIP += (State.m_wSTR - 20 - static_cast<unsigned short>((cLevel - 1) * 1.5f + 0.5f)) * 2;
			wTIP += (State.m_wDEX - 20 - static_cast<unsigned short>((cLevel - 1) * 1.5f)) * 2;
			wTIP += (State.m_wCON - 20);
			wTIP += (State.m_wINT - 20);
			wTIP += (State.m_wWIS - 20);

			if (wTIP > (10 + (cLevel - 1) * 2)) { return false; }
			break;

		case Sorcerer:
			wTIP += (State.m_wSTR - 20);
			wTIP += (State.m_wDEX - 20 - (cLevel - 1) * 1) * 2;
			wTIP += (State.m_wCON - 20);
			wTIP += (State.m_wINT - 20 - (cLevel - 1) * 2) * 2;
			wTIP += (State.m_wWIS - 20);

			if (wTIP > (10 + (cLevel - 1) * 2)) { return false; }
			break;

		case Enchanter:
			wTIP += (State.m_wSTR - 20);
			wTIP += (State.m_wDEX - 20 - static_cast<unsigned short>((cLevel - 1) * 1.5f + 0.5f)) * 2;
			wTIP += (State.m_wCON - 20);
			wTIP += (State.m_wINT - 20 - static_cast<unsigned short>((cLevel - 1) * 1.5f)) * 2;
			wTIP += (State.m_wWIS - 20);

			if (wTIP > (10 + (cLevel - 1) * 2)) { return false; }
			break;

		case Priest:
			wTIP += (State.m_wSTR - 20);
			wTIP += (State.m_wDEX - 20);
			wTIP += (State.m_wCON - 20 - static_cast<unsigned short>((cLevel - 1) * 1.5f + 0.5f)) * 2;
			wTIP += (State.m_wINT - 20);
			wTIP += (State.m_wWIS - 20 - static_cast<unsigned short>((cLevel - 1) * 1.5f)) * 2;

			if (wTIP > (10 + (cLevel - 1) * 2)) { return false; }
			break;

		case Cleric:
			wTIP += (State.m_wSTR - 20);
			wTIP += (State.m_wDEX - 20);
			wTIP += (State.m_wCON - 20 - (cLevel - 1) * 1) * 2;
			wTIP += (State.m_wINT - 20);
			wTIP += (State.m_wWIS - 20 - (cLevel - 1) * 2) * 2;

			if (wTIP > (10 + (cLevel - 1) * 2)) { return false; }
			break;

		case Combatant:
			wTIP += (State.m_wSTR - 20 - (cLevel - 1)) * 2;
			wTIP += (State.m_wDEX - 20);
			wTIP += (State.m_wCON - 20);
			wTIP += (State.m_wINT - 20);
			wTIP += (State.m_wWIS - 20);

			if (wTIP > 10) { return false; }
			break;

		case Officiator:
			wTIP += (State.m_wSTR - 20);
			wTIP += (State.m_wDEX - 20 - (cLevel - 1)) * 2;
			wTIP += (State.m_wCON - 20);
			wTIP += (State.m_wINT - 20);
			wTIP += (State.m_wWIS - 20);

			if (wTIP > 10) { return false; }
			break;

		case Templar:
			wTIP += (State.m_wSTR - 20 - static_cast<unsigned short>((cLevel - 1) * 1.5f)) * 2;
			wTIP += (State.m_wDEX - 20);
			wTIP += (State.m_wCON - 20 - static_cast<unsigned short>((cLevel - 1) * 1.5f + 0.5f)) * 2;
			wTIP += (State.m_wINT - 20);
			wTIP += (State.m_wWIS - 20);

			if (wTIP > (10 + (cLevel - 1) * 2)) { return false; }
			break;

		case Attacker:
			wTIP += (State.m_wSTR - 20 - (cLevel - 1) * 2) * 2;
			wTIP += (State.m_wDEX - 20);
			wTIP += (State.m_wCON - 20 - (cLevel - 1) * 1) * 2;
			wTIP += (State.m_wINT - 20);
			wTIP += (State.m_wWIS - 20);

			if (wTIP > (10 + (cLevel - 1) * 2)) { return false; }
			break;

		case Gunner:
			wTIP += (State.m_wSTR - 20 - static_cast<unsigned short>((cLevel - 1) * 1.5f + 0.5f)) * 2;
			wTIP += (State.m_wDEX - 20 - static_cast<unsigned short>((cLevel - 1) * 1.5f)) * 2;
			wTIP += (State.m_wCON - 20);
			wTIP += (State.m_wINT - 20);
			wTIP += (State.m_wWIS - 20);

			if (wTIP > (10 + (cLevel - 1) * 2)) { return false; }
			break;

		case RuneOff:
			wTIP += (State.m_wSTR - 20);
			wTIP += (State.m_wDEX - 20 - (cLevel - 1) * 1) * 2;
			wTIP += (State.m_wCON - 20);
			wTIP += (State.m_wINT - 20 - (cLevel - 1) * 2) * 2;
			wTIP += (State.m_wWIS - 20);

			if (wTIP > (10 + (cLevel - 1) * 2)) { return false; }
			break;

		case LifeOff:
			wTIP += (State.m_wSTR - 20);
			wTIP += (State.m_wDEX - 20 - (cLevel - 1) * 1) * 2;
			wTIP += (State.m_wCON - 20);
			wTIP += (State.m_wINT - 20);
			wTIP += (State.m_wWIS - 20 - (cLevel - 1) * 2) * 2;

			if (wTIP > (10 + (cLevel - 1) * 2)) { return false; }
			break;

		case ShadowOff:
			wTIP += (State.m_wSTR - 20 - (cLevel - 1) * 1) * 2;
			wTIP += (State.m_wDEX - 20 - (cLevel - 1) * 2) * 2;
			wTIP += (State.m_wCON - 20);
			wTIP += (State.m_wINT - 20);
			wTIP += (State.m_wWIS - 20);

			if (wTIP > (10 + (cLevel - 1) * 2)) { return false; }

/*
			wTIP += (State.m_wSTR - 20 - static_cast<unsigned short>((cLevel - 1) * 1.5f + 0.5f)) * 2;
			wTIP += (State.m_wDEX - 20 - static_cast<unsigned short>((cLevel - 1) * 1.5f)) * 2;
			wTIP += (State.m_wCON - 20);
			wTIP += (State.m_wINT - 20);
			wTIP += (State.m_wWIS - 20);

			if (wTIP > (10 + (cLevel - 1) * 2)) { return false; }
*/
			break;
	}

	return true;
}

bool CClass::CheckMinState(ChState& State, unsigned char cLevel)
{
	if (State.m_wSTR < GetMinState(StatusType::STR, cLevel))	{ return false; }
	if (State.m_wDEX < GetMinState(StatusType::DEX, cLevel))	{ return false; }
	if (State.m_wCON < GetMinState(StatusType::CON, cLevel))	{ return false; }
	if (State.m_wINT < GetMinState(StatusType::INT, cLevel))	{ return false; }
	if (State.m_wWIS < GetMinState(StatusType::WIS, cLevel))	{ return false; }

	return true;
}

unsigned short CClass::GetMinState(StatusType eType, unsigned char cLevel)
{
	// 스탯 기본값
	unsigned short wResult = 20;

	cLevel = cLevel - 1;

	if (eType == m_eIncrementType[0] || eType == m_eIncrementType[1])
	{
		// 레벨에 관계없이 똑같이 성장하는 경우
		if (false == m_bLevelSwap)
		{
			if (eType == m_eIncrementType[0])
			{
				wResult += cLevel * m_cIncrementValue[0];
			}

			if (eType == m_eIncrementType[1])
			{
				wResult += cLevel * m_cIncrementValue[1];
			}
		}
		// 레벨에 따라 스탯이 번갈아가며 성장하는 경우
		else
		{
			if (eType == m_eIncrementType[0])
			{
				wResult += static_cast<unsigned short>(cLevel * 1.5f);
			}

			if (eType == m_eIncrementType[1])
			{
				wResult += static_cast<unsigned short>(cLevel * 1.5f + 0.5f);
			}
		}
	}

	return wResult;
}

CClass::JobLevel CClass::GetJobLevel(unsigned char cClass) 
{
	if (cClass >= MAX_CLASS) { return DEFAULT_CLASS; }
	return ClassTable[cClass].m_eJobLevel;
}

unsigned char CClass::GetPreviousJob(unsigned char cClass) 
{
	if (cClass >= MAX_CLASS) { return NONE_JOB; }
	return ClassTable[cClass].m_ePrevJobType;
}

CClass::RaceType CClass::GetRace(unsigned char cClass)
{
	if (cClass >= MAX_CLASS) { return MAX_RACE; }
	return ClassTable[cClass].m_eRace;
}


#include "stdafx.h"
#include <algorithm>

#include <Network/Packet/PacketStruct/ServerInfo.h>
#include "RYLNetworkData.h"

#include <Creature/Character/CharacterClass.h>
#include <Creature/Character/CharacterStructure.h>

#include <Skill/Spell/SpellKind.h>
#include <Skill/SkillStructure.h>
#include <Skill/SkillMgr.h>

#include <Item/Item.h>

#include "CreatureStructure.h"

#include <GameTime/GameTimeConstants.h>
#include "GMMemory.h"

EliteBonus::EliteBonusData::EliteBonusData() 
:	m_cNation(CClass::MAX_RACE), m_cLevel(0) 
{ 
}

EliteBonus::EliteBonusData::EliteBonusData(char cNation, unsigned char cLevel)
:	m_cNation(cNation), m_cLevel(cLevel)
{ 
}

MotionInfo::MotionInfo()
:	m_fDirection(0.0f), m_fVelocity(0.0f), m_wAction(0), m_dwFrame(0) 
{ 
}

CharacterStatus::CharacterStatus()
:	m_nSTR(0), m_nDEX(0), m_nCON(0), m_nINT(0), m_nWIS(0)
{
}

CharacterStatus::CharacterStatus(CHAR_INFOST& characterDBData)
:	m_nSTR(characterDBData.STR), m_nDEX(characterDBData.DEX), m_nCON(characterDBData.CON),
	m_nINT(characterDBData.INT), m_nWIS(characterDBData.WIS)
{
}

EnchantInfo::EnchantInfo()
{
	ClearFlag();
}

void EnchantInfo::SetFlag(unsigned char cSpellKind)
{
	m_dwStatusFlag[cSpellKind / ULONG_BIT] |= (1 << cSpellKind % ULONG_BIT);
}

void EnchantInfo::ResetFlag(unsigned char cSpellKind)
{
	m_dwStatusFlag[cSpellKind / ULONG_BIT] &= ~(1 << cSpellKind % ULONG_BIT);
}

void EnchantInfo::ClearFlag(void)
{
	std::fill_n(m_dwStatusFlag, int(MAX_ARRAY), 0);
}

bool EnchantInfo::GetFlag(unsigned char cSpellKind) const
{
	int ret = m_dwStatusFlag[cSpellKind / ULONG_BIT] & (1 << cSpellKind % ULONG_BIT);
	return ret > 0;
}

void CharacterStatus::Init(CHAR_INFOST& characterDBData)
{
	m_nSTR = characterDBData.STR;
	m_nDEX = characterDBData.DEX;
	m_nCON = characterDBData.CON;
	m_nINT = characterDBData.INT;
	m_nWIS = characterDBData.WIS;
}


CreatureStatus::CreatureStatus()
:	m_nLevel(0), m_nExp(0), m_nNowHP(0), m_nNowMP(0)
{

}

CreatureStatus::CreatureStatus(CHAR_INFOST& characterDBData)
:	m_nLevel(characterDBData.Level), m_nExp(characterDBData.Exp), 
	m_nNowHP(characterDBData.HP), m_nNowMP(characterDBData.MP)
{

}

void CreatureStatus::Init(CHAR_INFOST& characterDBData)
{
	m_nLevel	= characterDBData.Level;
	m_nExp		= characterDBData.Exp;
	m_nNowHP	= characterDBData.HP;
	m_nNowMP	= characterDBData.MP;
}


FightStatus::FightStatus()
:	m_lMinDamage(0), m_lMaxDamage(0), m_wArmor(0), m_wHitRate(0), m_wEvade(0), 					
	m_nMaxHP(0), m_wHPRegen(0), m_nMaxMP(0), m_wMPRegen(0), 					
	m_wCritical(0), m_wBlock(0), m_wSpeed(0), m_wCoolDownRevision(0), m_wSkillPoint(0), 				
	m_wMagicPower(0), m_wMagicResist(0), m_wLuck(0),
	m_wFrost(0), m_wFire(0), m_wElectro(0), m_wDarkness(0),
	m_fDefenceRate(0), m_fCriticalMagnification(0), m_fCriticalRate(0), 			
	m_fBlockRate(0), m_fSpeedRate(0), m_fCoolDownRevisionRate(0), m_fMagicResistRate(0), m_fLevelExplosion(0), m_fLuckResistRate(0),
	m_cCriticalType(0), m_cComboCount(0), m_cCalculateState(CS_NONE)
{
}

using namespace Item;
/*
// edith 2008.02.4 디펜더 버그수정
// 소스가 가로로 너무 길어져서 마법수를 사용합니다.
// (STR : 1, DEX : 2, CON : 3, INT : 4, WIS : 5)
const unsigned char aryClassBonusStat[CClass::MAX_CLASS][Attribute::MAX_ATTRIBUTE_NUM] = {
	//			MinDam.	MaxDam.	Armor	HitRate	Evade	MaxHP	HPRegen	MaxMP	MPRegen	Cri.	Block	Speed	M.Power	M.Res.	Rune	CoolDw.	SkillPt	Frost	Fire	Electro	Dark
	{	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},			 
																																
	{	0,		1,		1,		3,		1,		1,		3,		3,		5,		5,		0,		0,		2,		5,		5,		0,		0,		0,		0,		0,		0,		0,		},	// 파이터		(STR 1,CON 1)
	{	0,		1,		1,		3,		2,		2,		3,		3,		5,		5,		0,		0,		2,		5,		5,		0,		0,		0,		0,		0,		0,		0,		},	// 로그			(DEX 1,STR 1)
	{	0,		2,		2,		3,		2,		2,		3,		3,		5,		5,		0,		0,		2,		5,		5,		0,		0,		0,		0,		0,		0,		0,		},	// 메이지		(INT 1,DEX 1)
	{	0,		3,		3,		3,		5,		5,		3,		3,		5,		5,		0,		0,		2,		5,		5,		0,		0,		0,		0,		0,		0,		0,		},	// 어콜라이트	(WIS 1,CON 1)
																																
	{	0,		1,		1,		3,		1,		3,		3,		3,		5,		5,		0,		0,		2,		5,		5,		0,		0,		0,		0,		0,		0,		0,		},	// 디펜더		(STR 2,CON 1, C) 
	{	0,		1,		1,		3,		1,		1,		3,		3,		5,		5,		0,		0,		2,		5,		5,		0,		0,		0,		0,		0,		0,		0,		},	// 워리어		(STR 2,CON 1)
	{	0,		1,		1,		3,		2,		2,		3,		3,		5,		5,		0,		0,		2,		5,		5,		0,		0,		0,		0,		0,		0,		0,		},	// 어쌔신		(DEX 2,STR 1)
	{	0,		1,		1,		3,		2,		2,		3,		3,		5,		5,		0,		0,		2,		5,		5,		0,		0,		0,		0,		0,		0,		0,		},	// 아처			(DEX 2,STR 1, C)
	{	0,		2,		2,		3,		4,		4,		3,		3,		4,		4,		0,		0,		2,		4,		4,		0,		0,		0,		0,		0,		0,		0,		},	// 소서러		(INT 2,DEX 1)
	{	0,		2,		2,		3,		4,		4,		3,		3,		4,		4,		0,		0,		2,		4,		4,		0,		0,		0,		0,		0,		0,		0,		},	// 인챈터		(INT 2,DEX 1, C)
	{	0,		3,		3,		3,		5,		5,		3,		3,		5,		5,		0,		0,		2,		5,		5,		0,		0,		0,		0,		0,		0,		0,		},	// 프리스트     (WIS 2,CON 1, C)
	{	0,		3,		3,		3,		5,		5,		3,		3,		5,		5,		0,		0,		2,		5,		5,		0,		0,		0,		0,		0,		0,		0,		},	// 클레릭		(WIS 2,CON 1)
																																
	{	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},			 
	{	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},			 
	{	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},			 
	{	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},			 
	
	{	0,		1,		1,		3,		1,		1,		3,		3,		5,		5,		0,		0,		2,		5,		5,		0,		0,		0,		0,		0,		0,		0,		},	// 컴배턴트		(STR 1)
	{	0,		2,		2,		3,		2,		2,		3,		3,		5,		5,		0,		0,		2,		5,		5,		0,		0,		0,		0,		0,		0,		0,		},  // 오피세이터	(DEX 1)

	{	0,		1,		1,		3,		1,		3,		3,		3,		5,		5,		0,		0,		2,		5,		5,		0,		0,		0,		0,		0,		0,		0,		},	// 템플러		(STR 2, CON 1, C)
	{	0,		1,		1,		3,		1,		1,		3,		3,		5,		5,		0,		0,		2,		5,		5,		0,		0,		0,		0,		0,		0,		0,		},	// 어태커		(STR 2, CON 1)
	{	0,		1,		1,		3,		2,		2,		3,		3,		5,		5,		0,		0,		2,		5,		5,		0,		0,		0,		0,		0,		0,		0,		},	// 거너			(DEX 2, STR 1, C)
	{	0,		2,		2,		3,		4,		4,		3,		3,		4,		4,		0,		0,		2,		4,		4,		0,		0,		0,		0,		0,		0,		0,		},	// 룬오프		(INT 2, DEX 1)
	{	0,		2,		2,		3,		5,		5,		3,		3,		5,		5,		0,		0,		2,		5,		5,		0,		0,		0,		0,		0,		0,		0,		},	// 라이프오프	(WIS 2, DEX 1)
	{	0,		1,		1,		3,		2,		2,		3,		3,		5,		5,		0,		0,		2,		5,		5,		0,		0,		0,		0,		0,		0,		0,		}	// 쉐도우오프	(DEX 2, STR 1)
};

// edith 2008.02.04 디펜더 보너스 Rate 수정
const float aryClassBonusRate[CClass::MAX_CLASS][Attribute::MAX_ATTRIBUTE_NUM] = {
	//			MinDam.	MaxDam.	Armor	HitRate	Evade	MaxHP	HPRegen	MaxMP	MPRegen	Cri.	Block	Speed	M.Power	M.Res.	Rune	CoolDw.	SkillPt	Frost	Fire	Electro	Dark
	{	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},			 

	{	0,		0,		0,		0,	    1,	 0.4f,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},	// 파이터		 
	{	0,		0,		0,		0,	    1,	 0.4f,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},	// 로그
	{	0,		0,		0,		0,	    1,	 0.4f,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},	// 메이지	 
	{	0,		0,		0,		0,	    1,	 0.4f,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},	// 어콜라이트

	{	0,		0,	 0.1f,	 0.5f,	    2, 	 0.6f,	 0.2f,  0.01f,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},	// 디펜더		 
	{	0,	 0.2f,	 0.3f,		0,	    3,	 0.2f,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},	// 워리어
	{	0,		0,	 0.1f,		0,	    1,	 0.4f,		0,	0.01f,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},	// 어쌔신
	{	0,		0,	 0.1f,		0,	    1,	 0.4f,	 0.1f,      0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},	// 아처
	{	0, 	 0.4f,	 0.5f,		0,	    1,	 0.2f,		0,		0,	 0.1f,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},	// 소서러	 
	{	0,	 0.4f,   0.5f,		0,	    1,	 0.2f,		0,		0,		0, 	0.01f,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},	// 인챈터
	{	0,	 0.2f,	 0.3f,		0,	    1,	 0.2f,	 0.1f,	0.01f,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},	// 프리스트
	{	0,	 0.5f,	 0.7f,		0, 	    1,	 0.2f,		0,		0,	0.03f, 0.003f,		0,		0,		0,		0,	 0.1f,		0,		0,		0,		0,		0,		0,		0,		},	// 클레릭

	{	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},			 
	{	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},			 
	{	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},			 
	{	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},			 

	{	0,		0,		0,		0,	    1,	 0.4f,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},	// 컴배턴트 = 파이터
	{	0,		0,		0,		0,	    1,	 0.4f,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},	// 오피세이터 = 메이지

	{	0,		0,	 0.1f,	 0.5f,	    2, 	 0.6f,	 0.2f,  0.01f,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},	// 템플러 = 디펜더
	{	0,	 0.2f,	 0.3f,		0,	    3,	 0.2f,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},	// 어태커 = 워리어
	{	0,		0,	 0.1f,		0,	    1,	 0.4f,	 0.1f,      0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},	// 거너   = 아처
	{	0, 	 0.4f,	 0.5f,		0,	    1,	 0.2f,		0,		0,	 0.1f,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},	// 룬오프 = 소서러
	{	0,	 0.5f,	 0.7f,		0, 	    1,	 0.2f,		0,		0,	0.03f, 0.003f,		0,		0,		0,		0,	 0.1f,		0,		0,		0,		0,		0,		0,		0,		},	// 라이프오프 = 클레릭
	{	0,		0,	 0.1f,		0,	    1,	 0.4f,		0,	0.01f,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		}	// 쉐도우오프 = 어쌔신
};
*/
/*
const unsigned char aryClassBonusStat[CClass::MAX_CLASS][Attribute::MAX_ATTRIBUTE_NUM] = {
	//			MinDam.	MaxDam.	Armor	HitRate	Evade	MaxHP	HPRegen	MaxMP	MPRegen	Cri.	Block	Speed	M.Power	M.Res.	Rune	CoolDw.	SkillPt	Frost	Fire	Electro	Dark
	{	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},			 
																																
	{	0,		1,		1,		3,		1,		1,		3,		3,		1,		1,		0,		0,		1,		1,		1,		0,		0,		0,		0,		0,		0,		0,		},	// 파이터		 
	{	0,		1,		1,		3,		2,		2,		1,		1,		1,		1,		0,		0,		2,		2,		1,		0,		0,		0,		0,		0,		0,		0,		},	// 로그
	{	0,		2,		2,		3,		2,		2,		2,		2,		4,		4,		0,		0,		4,		4,		4,		0,		0,		0,		0,		0,		0,		0,		},	// 메이지	 
	{	0,		3,		3,		3,		3,		3,		3,		3,		5,		5,		0,		0,		5,		5,		5,		0,		0,		0,		0,		0,		0,		0,		},	// 어콜라이트
																																
	{	0,		1,		1,		3,		1,		1,		3,		3,		1,		1,		0,		0,		1,		1,		1,		0,		0,		0,		0,		0,		0,		0,		},	// 디펜더		 
	{	0,		1,		1,		1,		1,		1,		3,		3,		1,		1,		0,		0,		1,		1,		1,		0,		0,		0,		0,		0,		0,		0,		},	// 워리어
	{	0,		1,		1,		3,		2,		2,		1,		1,		1,		1,		0,		0,		2,		2,		1,		0,		0,		0,		0,		0,		0,		0,		},	// 어쌔신
	{	0,		1,		1,		3,		2,		2,		1,		1,		1,		1,		0,		0,		2,		2,		1,		0,		0,		0,		0,		0,		0,		0,		},	// 아처
	{	0,		2,		2,		3,		2,		2,		2,		2,		4,		4,		0,		0,		4,		4,		4,		0,		0,		0,		0,		0,		0,		0,		},	// 소서러	 
	{	0,		2,		2,		3,		2,		2,		2,		2,		4,		4,		0,		0,		4,		4,		4,		0,		0,		0,		0,		0,		0,		0,		},	// 인챈터
	{	0,		3,		3,		3,		3,		3,		3,		3,		5,		5,		0,		0,		5,		5,		5,		0,		0,		0,		0,		0,		0,		0,		},	// 프리스트
	{	0,		3,		3,		3,		3,		3,		3,		3,		5,		5,		0,		0,		5,		5,		5,		0,		0,		0,		0,		0,		0,		0,		},	// 클레릭
																																
	{	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},			 
	{	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},			 
	{	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},			 
	{	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},			 
																																
	{	0,		1,		1,		3,		3,		3,		3,		3,		1,		1,		0,		0,		1,		1,		1,		0,		0,		0,		0,		0,		0,		0,		},	// 컴배턴트
	{	0,		1,		1,		3,		2,		2,		3,		3,		1,		1,		0,		0,		2,		4,		5,		0,		0,		0,		0,		0,		0,		0,		},	// 오피세이터

	{	0,		1,		1,		3,		1,		1,		3,		3,		1,		1,		0,		0,		1,		1,		1,		0,		0,		0,		0,		0,		0,		0,		},	// 템플러
	{	0,		1,		1,		1,		1,		1,		3,		3,		1,		1,		0,		0,		1,		1,		1,		0,		0,		0,		0,		0,		0,		0,		},	// 어태커
	{	0,		1,		1,		3,		2,		2,		1,		1,		1,		1,		0,		0,		2,		2,		1,		0,		0,		0,		0,		0,		0,		0,		},	// 거너
	{	0,		2,		2,		3,		2,		2,		2,		2,		4,		4,		0,		0,		4,		4,		4,		0,		0,		0,		0,		0,		0,		0,		},	// 룬오프	 
	{	0,		2,		2,		2,		2,		2,		2,		2,		5,		5,		0,		0,		5,		5,		5,		0,		0,		0,		0,		0,		0,		0,		},  // 라이프오프
	{	0,		1,		1,		3,		2,		2,		1,		1,		1,		1,		0,		0,		2,		2,		1,		0,		0,		0,		0,		0,		0,		0,		}	// 쉐도우오프
};

const float aryClassBonusRate[CClass::MAX_CLASS][Attribute::MAX_ATTRIBUTE_NUM] = {
	//			MinDam.	MaxDam.	Armor	HitRate	Evade	MaxHP	HPRegen	MaxMP	MPRegen	Cri.	Block	Speed	M.Power	M.Res.	Rune	CoolDw.	SkillPt	Frost	Fire	Electro	Dark
	{	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},			 

	{	0,		0.4f,	0.5f,	0,		2,		0.2f,	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},	// 파이터		 
	{	0,		0.4f,	0.5f,	0,		2,		0.2f,	0,		0,		0,		0,		0,		0,		0.1f,	0,		0,		0,		0,		0,		0,		0,		0,		0,		},	// 로그
	{	0,		0.4f,	0.5f,	0,		2,		0.2f,	0,		0,		0,		0,		0,		0,		0,		0.5f,	0.6f,	0,		0,		0,		0,		0,		0,		0,		},	// 메이지	 
	{	0,		0.4f,	0.5f,	0,		2,		0.2f,	0,		0,		0,		0,		0,		0,		0,		0.5f,	0.6f,	0,		0,		0,		0,		0,		0,		0,		},	// 어콜라이트

	{	0,		0.55f,	0.65f,	0.6f,	6,		0.4f,	0.3f,	0.03f,	0.05f,	0.05f,	0,		0,		0,		0,		0.2f,	0,		0,		0,		0,		0,		0,		0,		},	// 디펜더		 
	{	0,		0.65f,	0.75f,	0.1f,	4,		0.4f,	0.15f,	0.015f,	0.05f,	0,		0,		0,		0.2f,	0,		0.2f,	0,		0,		0,		0,		0,		0,		0,		},	// 워리어
	{	0,		0.55f,	0.65f,	0,		5,		0.6f,	0.08f,	0.005f,	0.2f,	0.02f,	0,		0,		0.1f,	0,		0.2f,	0,		0,		0,		0,		0,		0,		0,		},	// 어쌔신
	{	0,		0.4f,	0.5f,	0,		4,		0.6f,	0.1f,	0.01f,	0.15f,	0.01f,	0,		0,		0.1f,	0,		0.2f,	0,		0,		0,		0,		0,		0,		0,		},	// 아처
	{	0,		0.15f,	0.25f,	0,		2,		0.4f,	0.1f,	0.02f,	0.4f,	0.04f,	0,		0,		0,		1,		0.8f,	0,		0,		0,		0,		0,		0,		0,		},	// 소서러	 
	{	0,		0.15f,	0.25f,	0,		2,		0.4f,	0.05f,	0.005f,	0.3f,	0.03f,	0,		0,		0,		1,		0.8f,	0,		0,		0,		0,		0,		0,		0,		},	// 인챈터
	{	0,		0.15f,	0.25f,	0.3f,	3,		0.2f,	0.15f,	0.015f,	0.2f,	0.02f,	0,		0,		0,		1,		0.6f,	0,		0,		0,		0,		0,		0,		0,		},	// 프리스트
	{	0,		0.15f,	0.25f,	0.3f,	3,		0.2f,	0.1f,	0.01f,	0.3f,	0.03f,	0,		0,		0,		1,		0.6f,	0,		0,		0,		0,		0,		0,		0,		},	// 클레릭

	{	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},			 
	{	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},			 
	{	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},			 
	{	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},			 

	{	0,		0.65f,	0.75f,	0,		4,		0.8f,	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},	// 컴배턴트
	{	0,		0.65f,	0.75f,	0,		4,		0.8f,	0,		0,		0,		0,		0,		0,		0,		1,		1.2f,	0,		0,		0,		0,		0,		0,		0,		},	// 오피세이터

	{	0,		0.55f,	0.65f,	0.6f,	6,		0.4f,	0.3f,	0.03f,	0.05f,	0.05f,	0,		0,		0,		0,		0.2f,	0,		0,		0,		0,		0,		0,		0,		},	// 템플러
	{	0,		0.65f,	0.75f,	0.1f,	4,		0.4f,	0.15f,	0.015f,	0.05f,	0,		0,		0,		0.2f,	0,		0.2f,	0,		0,		0,		0,		0,		0,		0,		},	// 어태커
	{	0,		0.41f,	0.51f,	0,		4,		0.6f,	0.1f,	0.01f,	0.15f,	0.01f,	0,		0,		0.1f,	0,		0.2f,	0,		0,		0,		0,		0,		0,		0,		},	// 거너
	{	0,		0.15f,	0.25f,	0,		2,		0.4f,	0.3f,	0.02f,	0.45f,	0.04f,	0,		0,		0,		1.1f,	0.8f,	0,		0,		0,		0,		0,		0,		0,		},  // 룬오프	 
	{	0,		0.15f,	0.25f,	0.3f,	3,		0.2f,	0.125f,	0.0125f,0.25f,	0.025f,	0,		0,		0,		1,		0.6f,	0,		0,		0,		0,		0,		0,		0,		},	// 라이프오프
	{	0,		0.55f,	0.65f,	0,		5,		0.6f,	0.08f,	0.005f,	0.2f,	0.02f,	0,		0,		0.1f,	0,		0.2f,	0,		0,		0,		0,		0,		0,		0,		}	// 쉐도우오프
};
*/
const unsigned char aryClassBonusStat[CClass::MAX_CLASS][Attribute::MAX_ATTRIBUTE_NUM] = {
	//			MinDam.	MaxDam.	Armor	HitRate	Evade	MaxHP	HPRegen	MaxMP	MPRegen	Cri.	Block	Speed	M.Power	M.Res.	Rune	CoolDw.	SkillPt	Frost	Fire	Electro	Dark
	{	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},			 
																																
	{	0,		1,		1,		3,		1,		1,		3,		3,		1,		1,		0,		0,		1,		1,		1,		0,		0,		0,		0,		0,		0,		0,		},	// 파이터		 
	{	0,		1,		1,		3,		1,		1,		3,		3,		1,		1,		0,		0,		2,		1,		1,		0,		0,		0,		0,		0,		0,		0,		},	// 로그
	{	0,		2,		2,		3,		2,		2,		1,		1,		1,		2,		0,		0,		4,		4,		4,		0,		0,		0,		0,		0,		0,		0,		},	// 메이지	 
	{	0,		3,		3,		3,		3,		3,		1,		1,		1,		2,		0,		0,		5,		5,		5,		0,		0,		0,		0,		0,		0,		0,		},	// 어콜라이트
																																
	{	0,		1,		1,		3,		1,		1,		3,		3,		1,		1,		0,		0,		1,		1,		1,		0,		0,		0,		0,		0,		0,		0,		},	// 디펜더		 
	{	0,		1,		1,		1,		1,		1,		3,		3,		1,		1,		0,		0,		1,		1,		1,		0,		0,		0,		0,		0,		0,		0,		},	// 워리어
	{	0,		1,		1,		3,		2,		2,		1,		1,		1,		1,		2,		0,		2,		2,		1,		0,		0,		0,		0,		0,		0,		0,		},	// 어쌔신
	{	0,		1,		1,		3,		2,		2,		1,		1,		1,		1,		0,		0,		2,		2,		1,		0,		0,		0,		0,		0,		0,		0,		},	// 아처
	{	0,		2,		2,		3,		2,		2,		2,		2,		4,		4,		0,		0,		4,		4,		4,		0,		0,		0,		0,		0,		0,		0,		},	// 소서러	 
	{	0,		2,		2,		3,		2,		2,		2,		2,		4,		4,		0,		0,		4,		4,		4,		0,		0,		0,		0,		0,		0,		0,		},	// 인챈터
	{	0,		3,		3,		3,		3,		3,		3,		3,		5,		5,		0,		0,		5,		5,		5,		0,		0,		0,		0,		0,		0,		0,		},	// 프리스트
	{	0,		3,		3,		3,		3,		3,		3,		3,		5,		5,		0,		0,		5,		5,		5,		0,		0,		0,		0,		0,		0,		0,		},	// 클레릭
																																
	{	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},			 
	{	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},			 
	{	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},			 
	{	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},			 
																																
	{	0,		1,		1,		1,		1,		1,		1,		1,		1,		1,		0,		0,		1,		1,		1,		0,		0,		0,		0,		0,		0,		0,		},	// 컴배턴트
	{	0,		2,		2,		2,		2,		2,		2,		2,		2,		2,		0,		0,		2,		2,		2,		0,		0,		0,		0,		0,		0,		0,		},	// 오피세이터

	{	0,		1,		1,		3,		1,		1,		3,		3,		1,		1,		0,		0,		1,		1,		1,		0,		0,		0,		0,		0,		0,		0,		},	// 템플러
	{	0,		1,		1,		1,		1,		1,		3,		3,		1,		1,		0,		0,		1,		1,		1,		0,		0,		0,		0,		0,		0,		0,		},	// 어태커
	{	0,		1,		1,		3,		2,		2,		1,		1,		1,		1,		0,		0,		2,		2,		1,		0,		0,		0,		0,		0,		0,		0,		},	// 거너
	{	0,		2,		2,		3,		2,		2,		2,		2,		4,		4,		0,		0,		4,		4,		4,		0,		0,		0,		0,		0,		0,		0,		},	// 룬오프	 
	{	0,		2,		2,		2,		2,		2,		2,		2,		5,		5,		0,		0,		5,		5,		5,		0,		0,		0,		0,		0,		0,		0,		},  // 라이프오프
	{	0,		1,		1,		3,		2,		2,		1,		1,		1,		1,		2,		0,		2,		2,		1,		0,		0,		0,		0,		0,		0,		0,		}	// 쉐도우오프
};

const float aryClassBonusRate[CClass::MAX_CLASS][Attribute::MAX_ATTRIBUTE_NUM] = {
	//			MinDam.	MaxDam.	Armor	HitRate	Evade	MaxHP	HPRegen	MaxMP	MPRegen	Cri.	Block	Speed	M.Power	M.Res.	Rune	CoolDw.	SkillPt	Frost	Fire	Electro	Dark
	{	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},			 

	{	0,		0.4f,	0.5f,	0,		2,		0.2f,	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},	// 파이터		 
	{	0,		0.4f,	0.5f,	0,		2,		0.2f,	0,		0,		0,		0,		0,		0,		0.1f,	0,		0,		0,		0,		0,		0,		0,		0,		0,		},	// 로그
	{	0,		0.4f,	0.5f,	0,		2,		0.2f,	0,		0,		0,		0,		0,		0,		0,		0.5f,	0.6f,	0,		0,		0,		0,		0,		0,		0,		},	// 메이지	 
	{	0,		0.4f,	0.5f,	0,		2,		0.2f,	0,		0,		0,		0,		0,		0,		0,		0.5f,	0.6f,	0,		0,		0,		0,		0,		0,		0,		},	// 어콜라이트

	{	0,		0.55f,	0.65f,	0.6f,	6,		0.4f,	0.6f,	0.03f,	0.05f,	0.05f,	0,		0,		0,		0,		0.2f,	0,		0,		0,		0,		0,		0,		0,		},	// 디펜더		 
	{	0,		0.65f,	0.75f,	0.1f,	4,		0.4f,	0.4f,	0.015f,	0.05f,	0,		0,		0,		0.2f,	0,		0.2f,	0,		0,		0,		0,		0,		0,		0,		},	// 워리어
	{	0,		0.55f,	0.65f,	0,		5,		0.6f,	0.08f,	0.005f,	0.2f,	0.02f,	0.5f,	0,		0.1f,	0,		0.2f,	0,		0,		0,		0,		0,		0,		0,		},	// 어쌔신
	{	0,		0.4f,	0.5f,	0,		4,		0.6f,	0.1f,	0.01f,	0.15f,	0.01f,	0,		0,		0.1f,	0,		0.2f,	0,		0,		0,		0,		0,		0,		0,		},	// 아처
	{	0,		0.15f,	0.25f,	0,		2,		0.4f,	0.3f,	0.02f,	0.4f,	0.04f,	0,		0,		0,		1.1f,	0.8f,	0,		0,		0,		0,		0,		0,		0,		},	// 소서러	 
	{	0,		0.15f,	0.25f,	0,		2,		0.4f,	0.2f,	0.02f,	0.4f,	0.04f,	0,		0,		0,		1.1f,	0.8f,	0,		0,		0,		0,		0,		0,		0,		},	// 인챈터
    {	0,		0.75f,	0.85f,	0.3f,	3,		0.2f,	0.15f,	0.015f,	0.2f,	0.02f,	0,		0,		0,		1,		0.8f,	0,		0,		0,		0,		0,		0,		0,		},	// 프리스트
	{	0,		0.7f,	0.8f,	0.3f,	3,		0.2f,	0.1f,	0.01f,	0.3f,	0.03f,	0,		0,		0,		1,		0.8f,	0,		0,		0,		0,		0,		0,		0,		},	// 클레릭

	{	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},			 
	{	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},			 
	{	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},			 
	{	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},			 

	{	0,		0.65f,	0.75f,	0,		4,		0.8f,	0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		},	// 컴배턴트
	{	0,		0.65f,	0.75f,	0,		4,		0.8f,	0,		0,		0,		0,		0,		0,		0,		1,		1.2f,	0,		0,		0,		0,		0,		0,		0,		},	// 오피세이터

	{	0,		0.55f,	0.65f,	0.6f,	6,		0.4f,	0.6f,	0.03f,	0.05f,	0.05f,	0,		0,		0,		0,		0.2f,	0,		0,		0,		0,		0,		0,		0,		},	// 템플러
	{	0,		0.65f,	0.75f,	0.1f,	4,		0.4f,	0.4f,	0.015f,	0.05f,	0,		0,		0,		0.2f,	0,		0.2f,	0,		0,		0,		0,		0,		0,		0,		},	// 어태커
	{	0,		0.41f,	0.51f,	0,		4,		0.6f,	0.1f,	0.01f,	0.15f,	0.01f,	0,		0,		0.1f,	0,		0.2f,	0,		0,		0,		0,		0,		0,		0,		},	// 거너
	{	0,		0.15f,	0.25f,	0,		2,		0.4f,	0.3f,	0.02f,	0.45f,	0.04f,	0,		0,		0,		1.1f,	0.8f,	0,		0,		0,		0,		0,		0,		0,		},  // 룬오프	 
	{   0,		0.7f,	0.8f,	0.3f,	3,		0.2f,	0.15f,	0.015f,	0.25f,	0.025f,	0,		0,		0,		1,		0.8f,	0,		0,		0,		0,		0,		0,		0,		}, 	// 라이프오프
	{	0,		0.55f,	0.65f,	0,		5,		0.6f,	0.08f,	0.005f,	0.2f,	0.02f,	0.5f,	0,		0.1f,	0,		0.2f,	0,		0,		0,		0,		0,		0,		0,		}	// 쉐도우오프
};

using namespace Skill::SpellID;

bool FightStatus::CalculateBaseInfo(const CharacterStatus& characterStatus, 
									const unsigned short wLevel, const unsigned char cClass)
{
	if (CS_NONE != m_cCalculateState)
	{
		return false;
	}

	short aryStatus[CClass::MAX_TYPE] = { 0, };

	// 실제 연산에선 기본값 20을 뺀 값을 사용합니다. 
	aryStatus[CClass::STR] = characterStatus.m_nSTR - 20;
	aryStatus[CClass::DEX] = characterStatus.m_nDEX - 20;
	aryStatus[CClass::CON] = characterStatus.m_nCON - 20;
	aryStatus[CClass::INT] = characterStatus.m_nINT - 20;
	aryStatus[CClass::WIS] = characterStatus.m_nWIS - 20;

	// 전 클래스 공통
	m_lMinDamage = static_cast<long>(aryStatus[CClass::STR]);
	m_lMaxDamage = static_cast<long>(aryStatus[CClass::STR]);
	m_wArmor = 0;
	m_wHitRate = static_cast<short>(1 + (aryStatus[CClass::STR]+aryStatus[CClass::DEX])*0.5f);
	m_wEvade = static_cast<short>((aryStatus[CClass::STR]+aryStatus[CClass::DEX])*0.1f);

	m_nMaxHP = static_cast<short>(150 + 25 * wLevel + (aryStatus[CClass::CON] * 0.2f * wLevel));
	m_nMaxMP = static_cast<short>(150 + 12 * wLevel + (aryStatus[CClass::INT] * 0.4f * wLevel) + (aryStatus[CClass::WIS] * 0.4f * wLevel));

	m_wHPRegen = static_cast<short>(4 + (wLevel + aryStatus[CClass::CON])/2);
	m_wMPRegen = static_cast<short>(4 + (wLevel + aryStatus[CClass::INT] + aryStatus[CClass::WIS])/2);

	m_wCritical = static_cast<short>(6 + (aryStatus[CClass::DEX] * 0.4f));
	m_wBlock = 0;

	m_wSpeed = 34 + static_cast<short>(aryStatus[CClass::DEX] * 0.75f);

	m_wCoolDownRevision = aryStatus[CClass::DEX];
	
	m_wSkillPoint = wLevel;//static_cast<short>(wLevel + (aryStatus[CClass::INT] * 0.1f));
//	m_wSkillPoint = static_cast<short>(wLevel + (aryStatus[CClass::INT] * 0.1f));

	m_wMagicPower = static_cast<short>(aryStatus[CClass::INT] + aryStatus[CClass::WIS]) * 2;
	m_wMagicResist = static_cast<short>((aryStatus[CClass::INT] + aryStatus[CClass::WIS]) * 0.8f);

	m_fLevelExplosion = 1 + 0.02f * wLevel;

	// 클래스별 보너스 레이팅
	m_lMinDamage += static_cast<long>(aryStatus[aryClassBonusStat[cClass][Attribute::MIN_DAMAGE]] * aryClassBonusRate[cClass][Attribute::MIN_DAMAGE]);
	m_lMaxDamage += static_cast<long>(aryStatus[aryClassBonusStat[cClass][Attribute::MAX_DAMAGE]] * aryClassBonusRate[cClass][Attribute::MAX_DAMAGE]);
	m_wArmor += static_cast<short>(aryStatus[aryClassBonusStat[cClass][Attribute::ARMOR]] * aryClassBonusRate[cClass][Attribute::ARMOR]);
	m_wHitRate += static_cast<short>(aryStatus[aryClassBonusStat[cClass][Attribute::HIT_RATE]] * aryClassBonusRate[cClass][Attribute::HIT_RATE]);
	m_wEvade += static_cast<short>(aryStatus[aryClassBonusStat[cClass][Attribute::EVADE]] * aryClassBonusRate[cClass][Attribute::EVADE]);

	const unsigned short wAddMaxHP = 
		static_cast<unsigned short>((aryStatus[aryClassBonusStat[cClass][Attribute::MAX_HP]] * aryClassBonusRate[cClass][Attribute::MAX_HP] * wLevel) + 0.1f);
	m_nMaxHP = ((USHRT_MAX - m_nMaxHP) <= wAddMaxHP) ? USHRT_MAX : m_nMaxHP + wAddMaxHP;

	const unsigned short wAddMaxMP = 
		static_cast<unsigned short>((aryStatus[aryClassBonusStat[cClass][Attribute::MAX_MP]] * aryClassBonusRate[cClass][Attribute::MAX_MP] * wLevel) + 0.1f);
	m_nMaxMP = ((USHRT_MAX - m_nMaxMP) <= wAddMaxMP) ? USHRT_MAX : m_nMaxMP + wAddMaxMP;

	m_wHPRegen += static_cast<short>(aryStatus[aryClassBonusStat[cClass][Attribute::HP_REGEN]] * aryClassBonusRate[cClass][Attribute::HP_REGEN]);
	m_wMPRegen += static_cast<short>(aryStatus[aryClassBonusStat[cClass][Attribute::MP_REGEN]] * aryClassBonusRate[cClass][Attribute::MP_REGEN]);

	m_wCritical += static_cast<short>(aryStatus[aryClassBonusStat[cClass][Attribute::CRITICAL]] * aryClassBonusRate[cClass][Attribute::CRITICAL]);
	m_wBlock += static_cast<short>(aryStatus[aryClassBonusStat[cClass][Attribute::BLOCK]] * aryClassBonusRate[cClass][Attribute::BLOCK]);
	m_wSpeed += static_cast<short>(aryStatus[aryClassBonusStat[cClass][Attribute::SPEED]] * aryClassBonusRate[cClass][Attribute::SPEED]);

	m_wMagicPower += static_cast<short>(aryStatus[aryClassBonusStat[cClass][Attribute::MAGIC_POWER]] * aryClassBonusRate[cClass][Attribute::MAGIC_POWER]);
	m_wMagicResist += static_cast<short>(aryStatus[aryClassBonusStat[cClass][Attribute::MAGIC_RESIST]] * aryClassBonusRate[cClass][Attribute::MAGIC_RESIST]);

	// 럭키 찬스는 기본 10
	m_wLuck = 10;

	m_wCoolDownRevision += 0;
	m_wSkillPoint += 0;

	CalculateSubStatus();

	m_cComboCount = 4;
	m_cCalculateState = CS_BASE_INFO;
	return true;
}

bool FightStatus::CalculateEquipInfo(const Item::CEquipment** ppEquip, const bool bFirstWeaponSlot, const bool bRightHand, const bool bRideFlag,
									 const SKILL& skill, const unsigned char cLevel, const unsigned short wRiderAbility, const unsigned short wLuckAbility, const unsigned short wLevelAbility,
									 const CharacterStatus& characterStatus, const unsigned char cClass, 
									 FightStatus& resultStatus, int& resultLuck) const
{
	// edith 2009.06.20 럭키찬스는 기본값 10이다.
	resultLuck = 10;

	if (CS_BASE_INFO != m_cCalculateState)
	{
		return false;
	}

	// edith 2010.01.02 장비보정 추가
	// 클라이언튼는 보정이 없다.
	float fEquipCorr = 1.0f;
//	if(SERVER_ID::STONE_WAR1 == CRYLNetworkData::Instance()->m_dwPresentZone)
//		fEquipCorr = 0.5f;
	
	resultStatus = *this;

	short wRideSpeed = 0;

	// 장비 효과
	for (unsigned char cEquipIndex = 0; cEquipIndex < EquipmentPos::MAX_EQUPMENT_POS; ++cEquipIndex)
	{
		if(bRideFlag == false && cEquipIndex == EquipmentPos::AVATA)
			continue;

		// 휴먼의 경우 무기/방패 슬롯이 2개씩 있다.
		if (true == bFirstWeaponSlot)
		{
			if (cEquipIndex == EquipmentPos::WEAPON_HAND2 || 
				cEquipIndex == EquipmentPos::SHIELD_HAND2)
			{
				continue;
			}
		}
		else
		{
			if (cEquipIndex == EquipmentPos::WEAPON_HAND1 || 
				cEquipIndex == EquipmentPos::SHIELD_HAND1)
			{
				continue;
			}
		}

		// 왼손/오른손의 구분 (몇몇 스탯은 무기로 사용될 때만 효능을 발휘한다.)
		bool bUseWeapon = true;
		if (EquipmentPos::RINGL != cEquipIndex && 
			EquipmentPos::RINGR != cEquipIndex && 
			EquipmentPos::NECKLACE != cEquipIndex)
		{
			if (true == bRightHand)
			{
				if (cEquipIndex == EquipmentPos::SHIELD_HAND1 || 
					cEquipIndex == EquipmentPos::SHIELD_HAND2)
				{
					bUseWeapon = false;
				}
			}
			else
			{
				if (cEquipIndex == EquipmentPos::WEAPON_HAND1 || 
					cEquipIndex == EquipmentPos::WEAPON_HAND2)
				{
					bUseWeapon = false;
				}
			}
		}

		//dedi statment
		
		bool isNewItemType = false;
		if(EquipmentPos::BACKSIDE == cEquipIndex ||
			EquipmentPos::COSTUME == cEquipIndex ||
			EquipmentPos::BADGE == cEquipIndex ||
			EquipmentPos::COSTUME_RIGHT == cEquipIndex ||
			EquipmentPos::COSTUME_LEFT == cEquipIndex ||
			EquipmentPos::COSTUME_HELM == cEquipIndex)
		{
			isNewItemType = true;
		}
		const Item::CEquipment* lpEquipment = *(ppEquip + cEquipIndex);
		if (NULL != lpEquipment)
		{
			// 장착이 가능한 장비인지 체크한다.
			if (false == CheckEquipable(lpEquipment, characterStatus, cClass, cLevel, wLevelAbility)) { continue; }	

			short aryAttribute[Attribute::MAX_ATTRIBUTE_NUM];
			const_cast<Item::CEquipment*>(lpEquipment)->GetAttribute(aryAttribute, Attribute::MAX_ATTRIBUTE_NUM);

			resultStatus.m_wArmor += (short)(aryAttribute[Attribute::ARMOR] * fEquipCorr);
			resultStatus.m_wEvade += (short)(aryAttribute[Attribute::EVADE] * fEquipCorr);

			resultStatus.m_wMagicPower += (short)(aryAttribute[Attribute::MAGIC_POWER] * fEquipCorr);
			resultStatus.m_wMagicResist += (short)(aryAttribute[Attribute::MAGIC_RESIST] * fEquipCorr);

			const unsigned short wAddMaxHP = aryAttribute[Attribute::MAX_HP] * 10;
			resultStatus.m_nMaxHP = ((USHRT_MAX - resultStatus.m_nMaxHP) <= wAddMaxHP) ? USHRT_MAX : resultStatus.m_nMaxHP + wAddMaxHP;

			const unsigned short wAddMaxMP = aryAttribute[Attribute::MAX_MP] * 10;
			resultStatus.m_nMaxMP = ((USHRT_MAX - resultStatus.m_nMaxMP) <= wAddMaxMP) ? USHRT_MAX : resultStatus.m_nMaxMP + wAddMaxMP;

			resultStatus.m_wHPRegen += (short)(aryAttribute[Attribute::HP_REGEN] * fEquipCorr);
			resultStatus.m_wMPRegen += (short)(aryAttribute[Attribute::MP_REGEN] * fEquipCorr);

			resultStatus.m_wBlock += (short)(aryAttribute[Attribute::BLOCK] * fEquipCorr);

			// 라이더를 탔으면 스피드를 무시한다.
			if(cEquipIndex == EquipmentPos::AVATA)
				wRideSpeed = aryAttribute[Attribute::SPEED]+wRiderAbility;
			else
				resultStatus.m_wSpeed += aryAttribute[Attribute::SPEED];

			// 럭키찬스 저장
			resultLuck += aryAttribute[Attribute::LUCKCHANCE];

			resultStatus.m_wCoolDownRevision += aryAttribute[Attribute::COOLDOWN];
			resultStatus.m_wSkillPoint += aryAttribute[Attribute::SKILL_POINT];

			resultStatus.m_wFrost += (short)(aryAttribute[Attribute::FROST] * fEquipCorr);
			resultStatus.m_wFire += (short)(aryAttribute[Attribute::FIRE] * fEquipCorr);
			resultStatus.m_wElectro += (short)(aryAttribute[Attribute::ELECTRO] * fEquipCorr);
			resultStatus.m_wDarkness += (short)(aryAttribute[Attribute::DARKNESS] * fEquipCorr);
			//dedi add effect awakened
			const Item::CItemType::ArrayType eEquipType = CItemType::GetEquipType(lpEquipment->GetItemInfo().m_DetailData.m_dwFlags);
			//armor type
			if(eEquipType == CItemType::ARMOUR_TYPE)  //|| 
				//eEquipType == CItemType::HELM_TYPE)
			{
				if( 3 == ((CEquipment*)lpEquipment)->GetSeasonRecord())
				{
					short levelawakened = (lpEquipment->GetCoreLevel());
					if(levelawakened >= 4 )
					{
						resultStatus.m_wArmor += 100;
					}
					if(levelawakened >= 8 )
					{
						resultStatus.m_wMagicResist += 100;
					}
					if(levelawakened >= 10 )
					{
						resultStatus.m_wBlock += 100;
					}
					
				}
			}
			//shield type
			if(eEquipType == CItemType::SHIELD_TYPE  )
			{
				if( 3 == ((CEquipment*)lpEquipment)->GetSeasonRecord())
				{
					short levelawakened = (lpEquipment->GetCoreLevel());
					if(levelawakened >= 4 )
					{
						resultStatus.m_wHPRegen += 300;
					}
					if(levelawakened >= 8 )
					{
						resultStatus.m_wSpeed += 30;
					}
					if(levelawakened >= 10 )
					{
						resultStatus.m_wArmor += 200;
					}
					
				}
			}
			//dedi end
			if (true == bUseWeapon)
			{
				resultStatus.m_lMinDamage += static_cast<long>(aryAttribute[Attribute::MIN_DAMAGE] * fEquipCorr);
				resultStatus.m_lMaxDamage += static_cast<long>(aryAttribute[Attribute::MAX_DAMAGE] * fEquipCorr);
				resultStatus.m_wHitRate += (short)(aryAttribute[Attribute::HIT_RATE] * fEquipCorr);

				resultStatus.m_wCritical += (short)(aryAttribute[Attribute::CRITICAL] * fEquipCorr);

				if(isNewItemType == false )
				{
					resultStatus.m_cCriticalType = lpEquipment->GetItemInfo().m_DetailData.m_cCriticalType;
				}
				//resultStatus.m_cCriticalType = lpEquipment->GetItemInfo().m_DetailData.m_cCriticalType;

				//dedi add awakened weapon type
				const Item::CItemType::ArrayType eEquipType = CItemType::GetEquipType(lpEquipment->GetItemInfo().m_DetailData.m_dwFlags);
				if(eEquipType == CItemType::DAGGER_TYPE  ||
					eEquipType == CItemType::ONEHANDED_TYPE ||
					eEquipType == CItemType::LONGRANGE_TYPE || 
					eEquipType == CItemType::TWOHANDED_TYPE ||
					eEquipType == CItemType::SKILL_ARM_TYPE)
				{
					if( 3 == ((CEquipment*)lpEquipment)->GetSeasonRecord())
					{
						short levelawakened = (lpEquipment->GetCoreLevel());
						if(levelawakened >= 4 )
						{
							resultStatus.m_lMinDamage += 30;
							resultStatus.m_lMaxDamage += 30;
						}
						if(levelawakened >= 8 )
						{
							resultStatus.m_wHitRate += 300;
						}
						if(levelawakened >= 10 )
						{
							resultStatus.m_wCritical += 30;
						}
					
					}
				}
				if(eEquipType == CItemType::STAFF_TYPE)
				{
					if( 3 == ((CEquipment*)lpEquipment)->GetSeasonRecord())
					{
						short levelawakened = (lpEquipment->GetCoreLevel());
						if(levelawakened >= 4 )
						{
							resultStatus.m_wMagicPower += 200;
						}
						if(levelawakened >= 8 )
						{
							resultStatus.m_wCoolDownRevision += 100;
						}
						if(levelawakened >= 10 )
						{
							resultStatus.m_wSpeed += 30;
						}
					
					}
				}
				//dedi end
			}
		}
	}
	
	if(true == bRideFlag && wRideSpeed > resultStatus.m_wSpeed)
		resultStatus.m_wSpeed = wRideSpeed;

	if(wLuckAbility)
	{
		resultLuck += wLuckAbility;
		resultStatus.m_wLuck = resultLuck;
	}

	if (true == bRightHand)
	{
		const Item::CEquipment* lpEquipment = (true == bFirstWeaponSlot) ? 
			*(ppEquip + EquipmentPos::WEAPON_HAND1) : *(ppEquip + EquipmentPos::WEAPON_HAND2);

		// 스태프, 사이드의 최대 콤보수 제한
		if (NULL != lpEquipment)
		{
			if (ItemType::STAFF == lpEquipment->GetItemInfo().m_DetailData.m_cItemType || 
				ItemType::OPP_SYTHE == lpEquipment->GetItemInfo().m_DetailData.m_cItemType)
			{
				resultStatus.m_cComboCount = 2;
			}
		}
		// 맨손일 때 최대 콤보수 제한
		else
		{
			resultStatus.m_cComboCount = 0;
		}
	}

	// 데미지에 레벨 익스플로젼 효과 적용
	resultStatus.m_lMinDamage = static_cast<long>(resultStatus.m_lMinDamage * m_fLevelExplosion);
	resultStatus.m_lMaxDamage = static_cast<long>(resultStatus.m_lMaxDamage * m_fLevelExplosion);

	// 패시브 스킬 효과
	for (unsigned char cSlotIndex = 0; cSlotIndex < skill.wSlotNum; ++cSlotIndex) 
	{
		const SKILLSLOT& SkillSlot = skill.SSlot[cSlotIndex];

		unsigned short wSkillID = SkillSlot.SKILLINFO.wSkill;

		// 앨터너티브 스킬의 경우
		const Skill::ProtoType* lpProtoType = CSkillMgr::GetInstance().GetSkillProtoType(wSkillID);
		if (NULL != lpProtoType)
		{
			if (0 != lpProtoType->m_usChildSkill)
			{
				wSkillID = lpProtoType->m_usChildSkill;
			}
		}

#define MasteryAffect(SkillLevel, SkillLockCount, Level) \
	resultStatus.m_lMinDamage += static_cast<long>((3 * SkillLevel + SkillLockCount * 18) * (1 + Level * 0.02f)); \
	resultStatus.m_lMaxDamage += static_cast<long>((3 * SkillLevel + SkillLockCount * 18) * (1 + Level * 0.02f)); \
	resultStatus.m_wHitRate += 3 * SkillLevel + SkillLockCount * 18;

		switch (wSkillID)
		{
			// 소드 마스터리
			case 0x8102:
			{
				const Item::CEquipment* lpRight = (true == bFirstWeaponSlot) ? 
					*(ppEquip + EquipmentPos::WEAPON_HAND1) : *(ppEquip + EquipmentPos::WEAPON_HAND2);
				const Item::CEquipment* lpLeft = (true == bFirstWeaponSlot) ? 
					*(ppEquip + EquipmentPos::SHIELD_HAND1) : *(ppEquip + EquipmentPos::SHIELD_HAND2);

				if (NULL != lpRight)
				{
					if (ItemType::ONEHANDED_SWORD == lpRight->GetItemInfo().m_DetailData.m_cItemType || 
						ItemType::TWOHANDED_SWORD == lpRight->GetItemInfo().m_DetailData.m_cItemType)
					{
						if (true == bRightHand)
						{
							MasteryAffect(SkillSlot.SKILLINFO.cSkillLevel, SkillSlot.SKILLINFO.cLockCount, cLevel);
						}

						if (NULL == lpLeft || 
							(ItemType::ONEHANDED_SWORD != lpLeft->GetItemInfo().m_DetailData.m_cItemType && 
							ItemType::ONEHANDED_AXE != lpLeft->GetItemInfo().m_DetailData.m_cItemType &&
							ItemType::ONEHANDED_BLUNT != lpLeft->GetItemInfo().m_DetailData.m_cItemType))
						{
							if (true == bRightHand)
							{
								resultStatus.m_cComboCount += SkillSlot.SKILLINFO.cLockCount;
							}
						}
						else
						{
							if (false == bRightHand)
							{
								MasteryAffect(SkillSlot.SKILLINFO.cSkillLevel, SkillSlot.SKILLINFO.cLockCount, cLevel);
							}
						}
					}
				}
				break;
			}

			// 액스 마스터리
			case 0x8103:
			{
				const Item::CEquipment* lpRight = (true == bFirstWeaponSlot) ? 
					*(ppEquip + EquipmentPos::WEAPON_HAND1) : *(ppEquip + EquipmentPos::WEAPON_HAND2);
				const Item::CEquipment* lpLeft = (true == bFirstWeaponSlot) ? 
					*(ppEquip + EquipmentPos::SHIELD_HAND1) : *(ppEquip + EquipmentPos::SHIELD_HAND2);

				if (NULL != lpRight)
				{
					if (ItemType::ONEHANDED_AXE == lpRight->GetItemInfo().m_DetailData.m_cItemType || 
						ItemType::TWOHANDED_AXE == lpRight->GetItemInfo().m_DetailData.m_cItemType)
					{
						if (true == bRightHand)
						{
							MasteryAffect(SkillSlot.SKILLINFO.cSkillLevel, SkillSlot.SKILLINFO.cLockCount, cLevel);
						}

						if (NULL == lpLeft || 
							(ItemType::ONEHANDED_SWORD != lpLeft->GetItemInfo().m_DetailData.m_cItemType && 
							ItemType::ONEHANDED_AXE != lpLeft->GetItemInfo().m_DetailData.m_cItemType &&
							ItemType::ONEHANDED_BLUNT != lpLeft->GetItemInfo().m_DetailData.m_cItemType))
						{
							if (true == bRightHand)
							{
								resultStatus.m_cComboCount += SkillSlot.SKILLINFO.cLockCount;
							}
						}
						else
						{
							if (false == bRightHand)
							{
								MasteryAffect(SkillSlot.SKILLINFO.cSkillLevel, SkillSlot.SKILLINFO.cLockCount, cLevel);
							}
						}
					}
				}
				break;
			}

			// edith 2008.05.29 블런트 마스터리 추가 
			// 블런트 마스터리
			case 0x8105:
			{
				const Item::CEquipment* lpRight = (true == bFirstWeaponSlot) ? 
					*(ppEquip + EquipmentPos::WEAPON_HAND1) : *(ppEquip + EquipmentPos::WEAPON_HAND2);
				const Item::CEquipment* lpLeft = (true == bFirstWeaponSlot) ? 
					*(ppEquip + EquipmentPos::SHIELD_HAND1) : *(ppEquip + EquipmentPos::SHIELD_HAND2);

				if (NULL != lpRight)
				{
					if (ItemType::ONEHANDED_BLUNT == lpRight->GetItemInfo().m_DetailData.m_cItemType || 
						ItemType::TWOHANDED_BLUNT == lpRight->GetItemInfo().m_DetailData.m_cItemType)
					{
						if (true == bRightHand)
						{
							MasteryAffect(SkillSlot.SKILLINFO.cSkillLevel, SkillSlot.SKILLINFO.cLockCount, cLevel);
						}

						if (NULL == lpLeft || 
							(ItemType::ONEHANDED_SWORD != lpLeft->GetItemInfo().m_DetailData.m_cItemType && 
							ItemType::ONEHANDED_AXE != lpLeft->GetItemInfo().m_DetailData.m_cItemType &&
							ItemType::ONEHANDED_BLUNT != lpLeft->GetItemInfo().m_DetailData.m_cItemType))
						{
							if (true == bRightHand)
							{
								resultStatus.m_cComboCount += SkillSlot.SKILLINFO.cLockCount;
							}
						}
						else
						{
							if (false == bRightHand)
							{
								MasteryAffect(SkillSlot.SKILLINFO.cSkillLevel, SkillSlot.SKILLINFO.cLockCount, cLevel);
							}
						}
					}
				}
				break;
			}

			// 대거 마스터리
			case 0x8705:
			{
				const Item::CEquipment* lpRight = (true == bFirstWeaponSlot) ? 
					*(ppEquip + EquipmentPos::WEAPON_HAND1) : *(ppEquip + EquipmentPos::WEAPON_HAND2);
				const Item::CEquipment* lpLeft = (true == bFirstWeaponSlot) ? 
					*(ppEquip + EquipmentPos::SHIELD_HAND1) : *(ppEquip + EquipmentPos::SHIELD_HAND2);

				if (NULL != lpRight)
				{
					if (ItemType::DAGGER == lpRight->GetItemInfo().m_DetailData.m_cItemType)
					{
						if (true == bRightHand)
						{
							MasteryAffect(SkillSlot.SKILLINFO.cSkillLevel, SkillSlot.SKILLINFO.cLockCount, cLevel);
						}

						if (NULL == lpLeft || 
							ItemType::DAGGER != lpLeft->GetItemInfo().m_DetailData.m_cItemType)
						{
							if (true == bRightHand)
							{
								resultStatus.m_cComboCount += SkillSlot.SKILLINFO.cLockCount;
							}
						}
						else
						{
							if (false == bRightHand)
							{
								MasteryAffect(SkillSlot.SKILLINFO.cSkillLevel, SkillSlot.SKILLINFO.cLockCount, cLevel);
							}
						}
					}
				}
				break;
			}

			// 크러쉬 웨폰
			case 0x9102:
			{
				const Item::CEquipment* lpRight = (true == bFirstWeaponSlot) ? 
					*(ppEquip + EquipmentPos::WEAPON_HAND1) : *(ppEquip + EquipmentPos::WEAPON_HAND2);
				const Item::CEquipment* lpLeft = (true == bFirstWeaponSlot) ? 
					*(ppEquip + EquipmentPos::SHIELD_HAND1) : *(ppEquip + EquipmentPos::SHIELD_HAND2);

				if (NULL != lpRight)
				{
					if (ItemType::COM_BLUNT == lpRight->GetItemInfo().m_DetailData.m_cItemType || 
						ItemType::OPP_HAMMER == lpRight->GetItemInfo().m_DetailData.m_cItemType ||
						ItemType::OPP_AXE == lpRight->GetItemInfo().m_DetailData.m_cItemType)
					{
						if (true == bRightHand)
						{
							MasteryAffect(SkillSlot.SKILLINFO.cSkillLevel, SkillSlot.SKILLINFO.cLockCount, cLevel);
						}

						if (NULL == lpLeft || 
							ItemType::SKILL_A_ATTACK != lpLeft->GetItemInfo().m_DetailData.m_cItemType)
						{
							if (true == bRightHand)
							{
								resultStatus.m_cComboCount += SkillSlot.SKILLINFO.cLockCount;
							}
						}
						else
						{
							if (false == bRightHand)
							{
								MasteryAffect(SkillSlot.SKILLINFO.cSkillLevel, SkillSlot.SKILLINFO.cLockCount, cLevel);
							}
						}
					}
				}
				break;
			}

			// 블레이드
			case 0x9105:
			{
				const Item::CEquipment* lpRight = (true == bFirstWeaponSlot) ? 
					*(ppEquip + EquipmentPos::WEAPON_HAND1) : *(ppEquip + EquipmentPos::WEAPON_HAND2);
				const Item::CEquipment* lpLeft = (true == bFirstWeaponSlot) ? 
					*(ppEquip + EquipmentPos::SHIELD_HAND1) : *(ppEquip + EquipmentPos::SHIELD_HAND2);

				if (NULL != lpRight)
				{
					if (ItemType::COM_SWORD == lpRight->GetItemInfo().m_DetailData.m_cItemType || 
						ItemType::OPP_SLUSHER == lpRight->GetItemInfo().m_DetailData.m_cItemType)
					{
						if (true == bRightHand)
						{
							MasteryAffect(SkillSlot.SKILLINFO.cSkillLevel, SkillSlot.SKILLINFO.cLockCount, cLevel);
						}

						if (NULL == lpLeft || 
							ItemType::SKILL_A_ATTACK != lpLeft->GetItemInfo().m_DetailData.m_cItemType)
						{
							if (true == bRightHand)
							{
								resultStatus.m_cComboCount += SkillSlot.SKILLINFO.cLockCount;
							}
						}
						else
						{
							if (false == bRightHand)
							{
								MasteryAffect(SkillSlot.SKILLINFO.cSkillLevel, SkillSlot.SKILLINFO.cLockCount, cLevel);
							}
						}
					}
				}
				break;
			}

			// 클로우 마스터리
			case 0x9207:
			{
				const Item::CEquipment* lpRight = (true == bFirstWeaponSlot) ? 
					*(ppEquip + EquipmentPos::WEAPON_HAND1) : *(ppEquip + EquipmentPos::WEAPON_HAND2);
				const Item::CEquipment* lpLeft = (true == bFirstWeaponSlot) ? 
					*(ppEquip + EquipmentPos::SHIELD_HAND1) : *(ppEquip + EquipmentPos::SHIELD_HAND2);

				if (NULL != lpRight)
				{
					if (ItemType::OPP_TALON == lpRight->GetItemInfo().m_DetailData.m_cItemType)
					{
						if (true == bRightHand)
						{
							MasteryAffect(SkillSlot.SKILLINFO.cSkillLevel, SkillSlot.SKILLINFO.cLockCount, cLevel);
						}

						if (NULL == lpLeft || 
							ItemType::SKILL_A_KNIFE != lpLeft->GetItemInfo().m_DetailData.m_cItemType)
						{
							if (true == bRightHand)
							{
								resultStatus.m_cComboCount += SkillSlot.SKILLINFO.cLockCount;
							}
						}
						else
						{
							if (false == bRightHand)
							{
								MasteryAffect(SkillSlot.SKILLINFO.cSkillLevel, SkillSlot.SKILLINFO.cLockCount, cLevel);
							}
						}
					}
				}
				break;
			}

			// 엑스트라 라이프
			case 0x9107:
			{
				const unsigned short wAddMaxHP = cLevel * (SkillSlot.SKILLINFO.cSkillLevel * 2 + SkillSlot.SKILLINFO.cLockCount * 12);
				resultStatus.m_nMaxHP = ((USHRT_MAX - resultStatus.m_nMaxHP) <= wAddMaxHP) ? USHRT_MAX : resultStatus.m_nMaxHP + wAddMaxHP;
				break;
			}

			// 듀얼 윌드 마스터리
			case 0x8604:
			case 0x8706:
			{
				const Item::CEquipment* lpRight = (true == bFirstWeaponSlot) ? 
					*(ppEquip + EquipmentPos::WEAPON_HAND1) : *(ppEquip + EquipmentPos::WEAPON_HAND2);
				const Item::CEquipment* lpLeft = (true == bFirstWeaponSlot) ? 
					*(ppEquip + EquipmentPos::SHIELD_HAND1) : *(ppEquip + EquipmentPos::SHIELD_HAND2);

				if (NULL != lpRight && NULL != lpLeft)
				{
					if(((ItemType::ONEHANDED_SWORD == lpRight->GetItemInfo().m_DetailData.m_cItemType || 
						ItemType::ONEHANDED_AXE == lpRight->GetItemInfo().m_DetailData.m_cItemType || 
						ItemType::DAGGER == lpRight->GetItemInfo().m_DetailData.m_cItemType) && 
						(ItemType::ONEHANDED_SWORD == lpLeft->GetItemInfo().m_DetailData.m_cItemType || 
						ItemType::ONEHANDED_AXE == lpLeft->GetItemInfo().m_DetailData.m_cItemType || 
						ItemType::DAGGER == lpLeft->GetItemInfo().m_DetailData.m_cItemType)))
					{
						MasteryAffect(SkillSlot.SKILLINFO.cSkillLevel, SkillSlot.SKILLINFO.cLockCount, cLevel);

						resultStatus.m_cComboCount += SkillSlot.SKILLINFO.cLockCount;
					}
				}
				break;
			}

			// 스킬암 파이트
			case 0x9405:
			case 0x9803:
			{
				const Item::CEquipment* lpRight = (true == bFirstWeaponSlot) ? 
					*(ppEquip + EquipmentPos::WEAPON_HAND1) : *(ppEquip + EquipmentPos::WEAPON_HAND2);
				const Item::CEquipment* lpLeft = (true == bFirstWeaponSlot) ? 
					*(ppEquip + EquipmentPos::SHIELD_HAND1) : *(ppEquip + EquipmentPos::SHIELD_HAND2);

				if (NULL != lpRight && NULL != lpLeft)
				{
					if (((ItemType::COM_BLUNT == lpRight->GetItemInfo().m_DetailData.m_cItemType || 
						ItemType::COM_SWORD == lpRight->GetItemInfo().m_DetailData.m_cItemType) && 
						ItemType::SKILL_A_ATTACK == lpLeft->GetItemInfo().m_DetailData.m_cItemType) || 
						(ItemType::OPP_TALON == lpRight->GetItemInfo().m_DetailData.m_cItemType && 
						ItemType::SKILL_A_KNIFE == lpLeft->GetItemInfo().m_DetailData.m_cItemType))
					{
						MasteryAffect(SkillSlot.SKILLINFO.cSkillLevel, SkillSlot.SKILLINFO.cLockCount, cLevel);

						resultStatus.m_cComboCount += SkillSlot.SKILLINFO.cLockCount;
					}
				}
				break;
			}

			// 애큐러시
			case 0x8806:
			{
				if (true == bRightHand)
				{
					const Item::CEquipment* lpRight = (true == bFirstWeaponSlot) ? 
						*(ppEquip + EquipmentPos::WEAPON_HAND1) : *(ppEquip + EquipmentPos::WEAPON_HAND2);

					if (NULL != lpRight)
					{
						if (ItemType::BOW == lpRight->GetItemInfo().m_DetailData.m_cItemType || 
							ItemType::CROSSBOW == lpRight->GetItemInfo().m_DetailData.m_cItemType)
						{
							// 타 마스터리와는 수식이 다르다.
							resultStatus.m_lMinDamage += static_cast<long>(
								(2 * SkillSlot.SKILLINFO.cSkillLevel + SkillSlot.SKILLINFO.cLockCount * 12) * (1 + cLevel * 0.02f));
							resultStatus.m_lMaxDamage += static_cast<long>(
								(2 * SkillSlot.SKILLINFO.cSkillLevel + SkillSlot.SKILLINFO.cLockCount * 12) * (1 + cLevel * 0.02f));

							resultStatus.m_wHitRate += 2 * SkillSlot.SKILLINFO.cSkillLevel + SkillSlot.SKILLINFO.cLockCount * 12;
						}
					}
				}
				break;
			}

			// 터프니스
			case 0x8107:
			case 0x9110:
			{
				resultStatus.m_wArmor += static_cast<unsigned short>(
					(3 * SkillSlot.SKILLINFO.cSkillLevel + SkillSlot.SKILLINFO.cLockCount * 18) * (1 + cLevel * 0.02f));
				break;
			}

			// 윌파워
			case 0x8106:
			case 0x9109:
			{
				resultStatus.m_wMPRegen += static_cast<unsigned short>(
					(2 * SkillSlot.SKILLINFO.cSkillLevel + SkillSlot.SKILLINFO.cLockCount * 12) * (1 + cLevel * 0.02f));

				// edith 2009.06.13 윌파워 밸런스 수정
//				resultStatus.m_wMagicResist += 2 * SkillSlot.SKILLINFO.cSkillLevel + SkillSlot.SKILLINFO.cLockCount * 12;
				resultStatus.m_wMagicResist += static_cast<unsigned short>(
					(3 * SkillSlot.SKILLINFO.cSkillLevel + SkillSlot.SKILLINFO.cLockCount * 18) * (1 + cLevel * 0.02f) );
				break;
			}

			// 이베이젼
			case 0x8203:
			case 0x9213:
			{
				resultStatus.m_wEvade += static_cast<unsigned short>(
					(3 * SkillSlot.SKILLINFO.cSkillLevel + SkillSlot.SKILLINFO.cLockCount * 18) * (1 + cLevel * 0.02f));
				break;
			}

			// 임프루브드 크리티컬
			case 0x8202:
			case 0x9108:
			{
				resultStatus.m_wCritical += static_cast<unsigned short>(
					(2 * SkillSlot.SKILLINFO.cSkillLevel + SkillSlot.SKILLINFO.cLockCount * 12) * (1 + cLevel * 0.02f));
				break;
			}

			// 서레너티
			case 0x8306:
			case 0x9212:
			{
				const unsigned short wAddMaxMP = cLevel * (SkillSlot.SKILLINFO.cSkillLevel * 2 + SkillSlot.SKILLINFO.cLockCount * 12);
				resultStatus.m_nMaxMP = ((USHRT_MAX - resultStatus.m_nMaxMP) <= wAddMaxMP) ? USHRT_MAX : resultStatus.m_nMaxMP + wAddMaxMP;

				resultStatus.m_wCoolDownRevision += 2 * SkillSlot.SKILLINFO.cSkillLevel + SkillSlot.SKILLINFO.cLockCount * 12;
				break;
			}

			// 페이스
			case 0x8402:
			{
				resultStatus.m_wMPRegen += static_cast<unsigned short>(
					(1 * SkillSlot.SKILLINFO.cSkillLevel + SkillSlot.SKILLINFO.cLockCount * 6) * (1 + cLevel * 0.02f));
				resultStatus.m_wMagicPower += static_cast<unsigned short>(
					(2 * SkillSlot.SKILLINFO.cSkillLevel + SkillSlot.SKILLINFO.cLockCount * 12) * (1 + cLevel * 0.02f));
				break;
			}

			// 이너사이트
			case 0x9214:
			{
				resultStatus.m_wHPRegen += static_cast<unsigned short>(
					(1 * SkillSlot.SKILLINFO.cSkillLevel + SkillSlot.SKILLINFO.cLockCount * 6) * (1 + cLevel * 0.02f));
				resultStatus.m_wMPRegen += static_cast<unsigned short>(
					(1 * SkillSlot.SKILLINFO.cSkillLevel + SkillSlot.SKILLINFO.cLockCount * 6) * (1 + cLevel * 0.02f));
				break;
			}
		}
	}

    if( resultStatus.m_cComboCount > 6 )
        resultStatus.m_cComboCount = 6;

	resultStatus.CalculateSubStatus();

	resultStatus.m_cCalculateState = CS_EQUIP_INFO;
	return true;
}

using namespace Skill::SpellID;
bool FightStatus::CalculateEnchantInfo(const unsigned short* aryEnchantLevel, FightStatus& resultStatus) const									   
{
	if (CS_EQUIP_INFO != m_cCalculateState)
	{
		return false;
	}

	resultStatus = *this;

	float fHPPercent	= 0.0f;
	float fMPPercent	= 0.0f;

	float fHPSubPercent	= 0.0f;
	float fMPSubPercent	= 0.0f;

	for (unsigned char cSpellIndex = 0; cSpellIndex < MAX_SPELL_TYPE; ++cSpellIndex)
	{
		unsigned short wEnchantLevel = *(aryEnchantLevel + cSpellIndex);
		if (0 != wEnchantLevel)
		{
			switch (cSpellIndex)
			{
				case BattleSong:
					resultStatus.m_lMinDamage += wEnchantLevel * 2;
					resultStatus.m_lMaxDamage += wEnchantLevel * 2;
					resultStatus.m_wHitRate += wEnchantLevel;
					break;

				case MaintenanceChant:	
					resultStatus.m_wArmor += wEnchantLevel;
					resultStatus.m_wEvade += static_cast<short>(wEnchantLevel * 0.1f);
					resultStatus.m_wHPRegen += static_cast<short>(wEnchantLevel * 0.2f);
					break;

				case AccelerationChant:
					resultStatus.m_lMinDamage += wEnchantLevel * 2;
					resultStatus.m_lMaxDamage += wEnchantLevel * 2;
					resultStatus.m_wHitRate += wEnchantLevel;
					resultStatus.m_wMPRegen += static_cast<short>(wEnchantLevel * 0.2f);
					break;

				case LifeAura:	     
					resultStatus.m_wHPRegen += static_cast<short>(wEnchantLevel * 0.4f);
					resultStatus.m_wMPRegen += static_cast<short>(wEnchantLevel * 0.4f);
					break;

				case SpeedBuff:
					resultStatus.m_wSpeed += wEnchantLevel * 2;
					break;

				case ExpOrb:				break;

				case Regeneration:	   
					resultStatus.m_wHPRegen += static_cast<short>(wEnchantLevel * 0.4f);
					break;

				case DemagePotion:		    
					{
						if(wEnchantLevel > 50)
						{
							resultStatus.m_lMinDamage += wEnchantLevel;
							resultStatus.m_lMaxDamage += wEnchantLevel;
							break;
						}

						unsigned short add = static_cast<unsigned short>((float)m_lMinDamage*((float)wEnchantLevel/100.0f));
						resultStatus.m_lMinDamage += add;
						resultStatus.m_lMaxDamage += add;
					}
					break;

				case ArmorPotion:
					{
						if(wEnchantLevel > 50)
						{
							resultStatus.m_wArmor += wEnchantLevel;
							break;
						}

						unsigned short add = static_cast<unsigned short>((float)m_wArmor*((float)wEnchantLevel/100.0f));
						resultStatus.m_wArmor += add;
					}
					break;

				case HitRatePotion:		
					{
						if(wEnchantLevel > 50)
						{
							resultStatus.m_wHitRate += wEnchantLevel;
							break;
						}

						unsigned short add = static_cast<unsigned short>((float)m_wHitRate*((float)wEnchantLevel/100.0f));
						resultStatus.m_wHitRate += add;
					}
					break;

				case EvadePotion:								
					{
						if(wEnchantLevel > 50)
						{
							resultStatus.m_wEvade += wEnchantLevel;
							break;
						}

						unsigned short add = static_cast<unsigned short>((float)m_wEvade*((float)wEnchantLevel/100.0f));
						resultStatus.m_wEvade += add;
					}
					break;

				case MaxHPPotion:
					{
						if(wEnchantLevel > 50)
						{
							resultStatus.m_nMaxHP += wEnchantLevel;
							break;
						}

						unsigned short add = static_cast<unsigned short>((float)m_nMaxHP*((float)wEnchantLevel/100.0f));
						resultStatus.m_nMaxHP += add;
					}
					break;

				case MaxMPPotion:
					{
						if(wEnchantLevel > 50)
						{
							resultStatus.m_nMaxMP += wEnchantLevel;
							break;
						}

						unsigned short add = static_cast<unsigned short>((float)m_nMaxMP*((float)wEnchantLevel/100.0f));
						resultStatus.m_nMaxMP += add;
					}
					break;

				case HPRegenPotion:
					{
						if(wEnchantLevel > 50)
						{
							resultStatus.m_wHPRegen += wEnchantLevel;
							break;
						}

						unsigned short add = static_cast<unsigned short>((float)m_wHPRegen*((float)wEnchantLevel/100.0f));
						resultStatus.m_wHPRegen += add;
					}
					break;

				case MPRegenPotion:
					{
						if(wEnchantLevel > 50)
						{
							resultStatus.m_wMPRegen += wEnchantLevel;
							break;
						}

						unsigned short add = static_cast<unsigned short>((float)m_wMPRegen*((float)wEnchantLevel/100.0f));
						resultStatus.m_wMPRegen += add;
					}
					break;

				case CriticalPotion:
					{
						if(wEnchantLevel > 50)
						{
							resultStatus.m_wCritical += wEnchantLevel;
							break;
						}

						unsigned short add = static_cast<unsigned short>((float)m_wCritical*((float)wEnchantLevel/100.0f));
						resultStatus.m_wCritical += add;
					}
					break;

				case BlockPotion:
					{
						if(wEnchantLevel > 50)
						{
							resultStatus.m_wBlock += wEnchantLevel;
							break;
						}

						unsigned short add = static_cast<unsigned short>((float)m_wBlock*((float)wEnchantLevel/100.0f));
						resultStatus.m_wBlock += add;
					}
					break;
					
				case SpeedPotion:
					{
						if(wEnchantLevel > 50)
						{
							resultStatus.m_wSpeed += wEnchantLevel;
							break;
						}

						unsigned short add = static_cast<unsigned short>((float)m_wSpeed*((float)wEnchantLevel/100.0f));
						resultStatus.m_wSpeed += add;
					}
					break;

				case DeCoolDownPotion:
					{
						if(wEnchantLevel > 50)
						{
							resultStatus.m_wCoolDownRevision += wEnchantLevel;
							break;
						}

						unsigned short add = static_cast<unsigned short>((float)m_wCoolDownRevision*((float)wEnchantLevel/100.0f));
						resultStatus.m_wCoolDownRevision += add;
					}
					break;

				case MagicPowerPotion:
					{
						if(wEnchantLevel > 50)
						{
							resultStatus.m_wMagicPower += wEnchantLevel;
							break;
						}

						unsigned short add = static_cast<unsigned short>((float)m_wMagicPower*((float)wEnchantLevel/100.0f));
						resultStatus.m_wMagicPower += add;
					}
					break;

				case MagicResistPotion:
					{
						if(wEnchantLevel > 50)
						{
							resultStatus.m_wMagicResist += wEnchantLevel;
							break;
						}

						unsigned short add = static_cast<unsigned short>((float)m_wMagicResist*((float)wEnchantLevel/100.0f));
						resultStatus.m_wMagicResist += add;
					}
					break;

				case Slow:			    
					resultStatus.m_wSpeed = 13;
					break;

				case ArmorBroken:		    
					resultStatus.m_wArmor = 
						(resultStatus.m_wArmor > wEnchantLevel) ? resultStatus.m_wArmor - wEnchantLevel : 0;
					break;

				case Blaze:			    
					resultStatus.m_wSpeed += 50;
					resultStatus.m_wHitRate += static_cast<short>(wEnchantLevel * 0.5f);
					resultStatus.m_wEvade = (resultStatus.m_wEvade > static_cast<short>(wEnchantLevel * 0.1f)) ? 
						resultStatus.m_wEvade - static_cast<short>(wEnchantLevel * 0.1f) : 0;
					resultStatus.m_wMagicResist += static_cast<short>(wEnchantLevel * 0.1f);
					break;

				case Charging:		    
					resultStatus.m_wSpeed += 50;
					resultStatus.m_wArmor += static_cast<short>(wEnchantLevel * 0.5f);
					resultStatus.m_wMagicResist += static_cast<short>(wEnchantLevel * 0.1f);
					break;

				case Stealth:		
					resultStatus.m_wSpeed = wEnchantLevel * 10;
					break;

				case ManaShell:				break;

				case Encourage:		  
					resultStatus.m_lMinDamage += wEnchantLevel;
					resultStatus.m_lMaxDamage += wEnchantLevel;
					resultStatus.m_wHitRate += wEnchantLevel;
					resultStatus.m_wArmor += static_cast<short>(wEnchantLevel * 0.5f);
					resultStatus.m_wEvade += static_cast<short>(wEnchantLevel * 0.25f);
					break;

				case EnchantWeapon:	    
					resultStatus.m_lMinDamage += wEnchantLevel * 4;
					resultStatus.m_lMaxDamage += wEnchantLevel * 4;
					resultStatus.m_wHitRate += wEnchantLevel;
					break;

				case BrightArmor:	
					resultStatus.m_wArmor += static_cast<short>(wEnchantLevel * 2.5f);
					break;

				case HardenSkin:		    
					resultStatus.m_wArmor += static_cast<short>(wEnchantLevel * 2.5f);
					break;

				case Flexibility:		    break;

				case Guard:			
					resultStatus.m_wBlock += wEnchantLevel;
					break;

				case Hold:					break;
				case Stun:					break;

				case Frozen:		    
					resultStatus.m_wSpeed = 13;
					resultStatus.m_wCoolDownRevision = 0;
					break;

				case Poisoned:
					resultStatus.m_lMinDamage = 
						(resultStatus.m_lMinDamage > (wEnchantLevel * 4)) ? resultStatus.m_lMinDamage - (wEnchantLevel * 4) : 0;
					resultStatus.m_lMaxDamage = 
						(resultStatus.m_lMaxDamage > (wEnchantLevel * 4)) ? resultStatus.m_lMaxDamage - (wEnchantLevel * 4) : 0;
					resultStatus.m_wHitRate = 
						(resultStatus.m_wHitRate > wEnchantLevel) ? resultStatus.m_wHitRate - wEnchantLevel : 0;
					break;

				case LowerStrength:	 
					resultStatus.m_lMinDamage = 
						(resultStatus.m_lMinDamage > (wEnchantLevel * 4)) ? resultStatus.m_lMinDamage - (wEnchantLevel * 4) : 0;
					resultStatus.m_lMaxDamage = 
						(resultStatus.m_lMaxDamage > (wEnchantLevel * 4)) ? resultStatus.m_lMaxDamage - (wEnchantLevel * 4) : 0;
					resultStatus.m_wHitRate = 
						(resultStatus.m_wHitRate > wEnchantLevel) ? resultStatus.m_wHitRate - wEnchantLevel : 0;
					resultStatus.m_wArmor = 
						(resultStatus.m_wArmor > wEnchantLevel) ? resultStatus.m_wArmor - wEnchantLevel : 0;
					resultStatus.m_wEvade = (resultStatus.m_wEvade > static_cast<short>(wEnchantLevel * 0.25f)) ? 
						resultStatus.m_wEvade - static_cast<short>(wEnchantLevel * 0.25f) : 0;
					break;

				case Invincible:			break;	    
				case Hide:             		break;
				case LuckyOrb:				break;	    

				case StoneForm:		   		
					resultStatus.m_wEvade = 0;
					resultStatus.m_wBlock = 0;
					resultStatus.m_wArmor = SHRT_MAX;
					resultStatus.m_wMagicResist = SHRT_MAX;
					break;

				case CounterAttack:			break;	    
				case Envenom:		   		break;
				case BombSet:				break;	    

				case KarterantWorldBuff:
				case MerkadiaWorldBuff:
					fHPPercent += 0.05f;
					fMPPercent += 0.05f;					
					break;

				case KarterantWorldDeBuff:
				case MerkadiaWorldDeBuff:
					fHPSubPercent += 0.05f;
					fMPSubPercent += 0.05f;					
					break;
				
				case PowerStatue:		// 힘의 석상
					fHPPercent += wEnchantLevel * 0.01f;
					break;

				case IntStatue:			// 경험의 석상
					fMPPercent += wEnchantLevel * 0.01f;
					break;

				case ExpStatue:			// 경험의 석상
					break;

				case WealthStatue:		// 부의 석상
					break;

				case RealmHP:			// 렐름 HP
					fHPPercent += wEnchantLevel * 0.05f;
					break;

				case RealmMP:			// 렐름 MP
					fMPPercent += wEnchantLevel * 0.05f;
					break;

				case Concentration:		// 
					break;

				case TasteWine:		
					resultStatus.m_wArmor += 30;
					resultStatus.m_wEvade += 30;
					break;

				case WifeCracker:			break;			
/*
				case StaminaPotion:		// 체력의 마법 시약.
					switch(wEnchantLevel)
					{
					case 1:
						fHPPercent += 0.2f;					
						fMPPercent += 0.1f;
						break;
					case 2:
						fHPPercent += 0.3f;					
						fMPPercent += 0.2f;
						break;
					case 3:
						fHPPercent += 0.5f;					
						fMPPercent += 0.3f;
						resultStatus.m_wArmor += 150;
						resultStatus.m_wMagicPower += 250;
						resultStatus.m_lMinDamage += 400;
						resultStatus.m_lMaxDamage += 400;
						break;
					}
					
					break;
*/
				case Fired:
					resultStatus.m_lMinDamage = 
						(resultStatus.m_lMinDamage > (wEnchantLevel * 4)) ? resultStatus.m_lMinDamage - (wEnchantLevel * 4) : 0;
					resultStatus.m_lMaxDamage = 
						(resultStatus.m_lMaxDamage > (wEnchantLevel * 4)) ? resultStatus.m_lMaxDamage - (wEnchantLevel * 4) : 0;
					resultStatus.m_wHitRate = 
						(resultStatus.m_wHitRate > wEnchantLevel) ? resultStatus.m_wHitRate - wEnchantLevel : 0;
					break;

				case CurseOfBlind:
					resultStatus.m_lMinDamage = 
						(resultStatus.m_lMinDamage > (wEnchantLevel * 4)) ? resultStatus.m_lMinDamage - (wEnchantLevel * 4) : 0;
					resultStatus.m_lMaxDamage = 
						(resultStatus.m_lMaxDamage > (wEnchantLevel * 4)) ? resultStatus.m_lMaxDamage - (wEnchantLevel * 4) : 0;
					resultStatus.m_wHitRate = 
						(resultStatus.m_wHitRate > wEnchantLevel) ? resultStatus.m_wHitRate - wEnchantLevel : 0;
					resultStatus.m_wArmor = 
						(resultStatus.m_wArmor > wEnchantLevel) ? resultStatus.m_wArmor - wEnchantLevel : 0;
					resultStatus.m_wEvade = (resultStatus.m_wEvade > static_cast<short>(wEnchantLevel * 0.25f)) ? 
						resultStatus.m_wEvade - static_cast<short>(wEnchantLevel * 0.25f) : 0;
					break;

				case HundredLevel:	break;

				case Drunk:			
					break;

				case MichaelBless:			
					break;

				case DeCoolDown:
					resultStatus.m_wCoolDownRevision += wEnchantLevel * 2;
					break;

				case StealHand:
					break;

				case CD_8D12:
				case CD_8D14:
				case CD_8D16:
				case CD_99A1:
				case CD_99A2:
				case CD_99A3:
					break;

			}					   		
		}
	}

	// ---------------------------------------------------------
	// 월드 웨폰, 석상 효과 등의 처리

	unsigned short wOriginalMaxHP = resultStatus.m_nMaxHP;
	unsigned short wOriginalMaxMP = resultStatus.m_nMaxMP;

	resultStatus.m_nMaxHP -= static_cast<unsigned short>( wOriginalMaxHP * fHPSubPercent );
	resultStatus.m_nMaxMP -= static_cast<unsigned short>( wOriginalMaxMP * fMPSubPercent );

	unsigned short wAddMaxHP = static_cast<unsigned short>( wOriginalMaxHP * fHPPercent );

	if(wAddMaxHP>MAX_INCHANT_HP)
		wAddMaxHP = MAX_INCHANT_HP;

	resultStatus.m_nMaxHP = ((USHRT_MAX - resultStatus.m_nMaxHP) <= wAddMaxHP) ? USHRT_MAX : resultStatus.m_nMaxHP + wAddMaxHP;

	unsigned short wAddMaxMP = static_cast<unsigned short>( wOriginalMaxMP * fMPPercent );

	if(wAddMaxHP>MAX_INCHANT_MP)
		wAddMaxHP = MAX_INCHANT_MP;

	resultStatus.m_nMaxMP = ((USHRT_MAX - resultStatus.m_nMaxMP) <= wAddMaxMP) ? USHRT_MAX : resultStatus.m_nMaxMP + wAddMaxMP;

	// ---------------------------------------------------------

	resultStatus.CalculateSubStatus();

	resultStatus.m_cCalculateState = CS_ENCHANT_INFO;
	return true;
}

void FightStatus::CalculateSubStatus(void)
{
	m_fDefenceRate = static_cast<long>((99 * m_wArmor / 4.0f) / (m_wArmor / 4.0f + 50) * 100) / 100.0f;
	m_fCriticalMagnification = 1.4f + m_wCritical / 600.0f;
	m_fCriticalRate = static_cast<long>((99 * m_wCritical / 2.0f) / static_cast<float>(m_wCritical / 2.0f + 50) * 100) / 100.0f;
	m_fBlockRate = static_cast<long>((99 * m_wBlock) / (m_wBlock + 50) * 50) / 100.0f;
	m_fSpeedRate = static_cast<long>((99 * m_wSpeed) / static_cast<float>(m_wSpeed + 50) * 250) / 100.0f;
	m_fCoolDownRevisionRate = 100 - 
		static_cast<long>((99 * m_wCoolDownRevision) / static_cast<float>(m_wCoolDownRevision + 50) * 50) / 100.0f;
	m_fMagicResistRate = static_cast<long>((99 * (static_cast<float>(m_wMagicResist) * 0.85f)) / ((static_cast<float>(m_wMagicResist) * 0.85f) + 50) * 100) / 100.0f - 2.0f;
	if ( m_fMagicResistRate <= 0.0f ) m_fMagicResistRate = 0.0f ;

	// 럭키찬스 레이트
	m_fLuckResistRate = (float)m_wLuck / 3.0f;
}

bool FightStatus::CheckEquipable(const Item::CEquipment* lpEquip, const CharacterStatus& characterStatus,
								 const unsigned char cClass, const unsigned char cLevel, const unsigned short wLevelAbility)	
{
	using namespace Item;

	if (NULL == lpEquip) { return false; }

	// 내구도 제한
	if (0 == lpEquip->GetNumOrDurability()) { return false; }

	int iCurLevel = cLevel;
	if(3 == ((CEquipment*)lpEquip)->GetSeasonRecord())	// 시즌레코드 3이면 레벨제한을 5 다운시킨다)
	{
		iCurLevel = cLevel+((CEquipment*)lpEquip)->GetCoreLevel();
	}

	// 어빌리티 보너스
	iCurLevel += wLevelAbility;

	// 2009.08.18 장착레벨 제한 (LimitStatus 제한)
	unsigned short wValue = lpEquip->GetItemInfo().m_UseLimit.m_wLimitValue;
	switch (lpEquip->GetItemInfo().m_UseLimit.m_cLimitStatus)
	{
		case StatusLimit::STR:		if (characterStatus.m_nSTR < wValue) { return false; }		break;
		case StatusLimit::DEX:		if (characterStatus.m_nDEX < wValue) { return false; }		break;
		case StatusLimit::CON:		if (characterStatus.m_nCON < wValue) { return false; }		break;
		case StatusLimit::INT:		if (characterStatus.m_nINT < wValue) { return false; }		break;
		case StatusLimit::WIS:		if (characterStatus.m_nWIS < wValue) { return false; }		break;
		case StatusLimit::LEVEL:	if (iCurLevel < wValue)		 { return false; }		break;	
	}

	// 클래스 제한
	if (0 == (lpEquip->GetItemInfo().m_UseLimit.m_dwClassLimit & (0x00000001 << (cClass - 1)))) 
	{
		return false;
	}

	// 장착 레벨 제한
	// 2009.08.18 장착레벨 제한
	if(lpEquip->GetItemInfo().m_DetailData.m_wLowLimitLevel || lpEquip->GetItemInfo().m_DetailData.m_wHighLimitLevel)
	{
		if(iCurLevel < lpEquip->GetItemInfo().m_DetailData.m_wLowLimitLevel || iCurLevel > lpEquip->GetItemInfo().m_DetailData.m_wHighLimitLevel)
		{
			return false;
		}
	}	

	// 룬이 장착되어있으면 레벨제한 장착제한 검사를 한다.
	if(cLevel<lpEquip->GetRuneEquipLimitLevel())
	{
		return false;
	}

	return true;
}


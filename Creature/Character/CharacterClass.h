#ifndef _CHARACTER_CLASS_H_
#define _CHARACTER_CLASS_H_

#include <DB/DBDefine.h>


// 전방 참조
struct CharacterDBData;
struct ChState;

class CClass 
{
public:

	enum JobType
    {
		NONE_JOB = 0, 

		// 인간 기본클래스
		Fighter		= 1,	Rogue		= 2,
		Mage		= 3,	Acolyte		= 4,

		// 인간 1st클래스
		Defender	= 5,	Warrior		= 6,
        Assassin	= 7,	Archer		= 8,
		Sorcerer	= 9,	Enchanter	= 10,
        Priest		= 11,	Cleric		= 12,
	
		// 아칸 기본클래스
		Combatant	= 17,	Officiator	= 18,

		// 아칸 1st클래스
		Templar		= 19, 	Attacker	= 20,
		Gunner		= 21,	RuneOff		= 22,
		LifeOff		= 23,	ShadowOff	= 24,

		MAX_CLASS	= 25
	};

	enum StatusType 
	{ 
		NONE_STAT	= 0, 
	
		STR 		= 1, 
		DEX 		= 2, 
		CON 		= 3, 
		INT 		= 4, 
		WIS 		= 5, 

		MAX_TYPE	= 6 
	};

	enum JobLevel	
	{ 
		NONE_CLASS		= 0, 
		DEFAULT_CLASS	= 1, 
		JOB_CHANGE_1ST	= 2 
	};

	enum RaceType
	{ 
		HUMAN		= 0, 
		AKHAN		= 1, 
		MAX_RACE	= 2 
	};

	CClass(const JobType eJobType, const JobType ePrevJobType, const JobLevel eJobLevel, const RaceType eRace,		
		const StatusType eType1, const unsigned char cIncrement1,
		const StatusType eType2, const unsigned char cIncrement2,
		const bool bLevelSwap);

	CClass();
	
	void LevelUp(CHAR_INFOST* InfoSt);
	bool JobChange(CharacterDBData* DBData, unsigned char cClassType);	
	bool UpgradeClass(CharacterDBData* DBData, unsigned char cClassType);	
	bool DowngradeClass(CharacterDBData* DBData, unsigned char cClassType);	
	bool InitializeClass(CharacterDBData* DBData, unsigned char cClassType);	// 레벨 1의 기본 클래스로 만든다.

	bool CheckState(ChState& State, unsigned char cLevel);
	bool CheckMinState(ChState& State, unsigned char cLevel);
	unsigned short GetMinState(StatusType eType, unsigned char cLevel);

	static JobLevel GetJobLevel(unsigned char cClass);
	static unsigned char GetPreviousJob(unsigned char cClass);
	static unsigned char GetRequiredIP(unsigned short usClass, unsigned char cType);
	static RaceType GetRace(unsigned char cClass);
	
protected:

	StatusType		m_eIncrementType[2];	// 성장시 추가 능력치 타입
	unsigned char	m_cIncrementValue[2];	// 성장시 추가 능력치 값

	JobType			m_eJobType;			// 현재 직업
	JobType			m_ePrevJobType;		// 바로 하위 클래스의 직업
	JobLevel		m_eJobLevel;		// 현재 직업의 레벨
	RaceType		m_eRace;			// 소속 종족
		
	bool			m_bLevelSwap;		// 레벨에 따라 스탯이 번갈아가면서 성장함 (짝수 2/1 성장이면 홀수 1/2 성장)

	inline void IncrementByType(CHAR_INFOST* InfoSt, 
		unsigned char cTypeIndex, unsigned char cIncrementIndex);
};

extern CClass ClassTable[CClass::MAX_CLASS];


inline void CClass::IncrementByType(CHAR_INFOST* InfoSt, 
									unsigned char cTypeIndex, unsigned char cIncrementIndex)
{
	switch(m_eIncrementType[cTypeIndex]) {
		case StatusType::STR:	InfoSt->STR	+= m_cIncrementValue[cIncrementIndex];	break;
		case StatusType::DEX:	InfoSt->DEX	+= m_cIncrementValue[cIncrementIndex];	break;
		case StatusType::CON:	InfoSt->CON	+= m_cIncrementValue[cIncrementIndex];	break;
		case StatusType::INT:	InfoSt->INT	+= m_cIncrementValue[cIncrementIndex];	break;
		case StatusType::WIS:	InfoSt->WIS	+= m_cIncrementValue[cIncrementIndex];	break;
	}
}

#endif
#ifndef _CHARACTER_STRUCTURE_H_
#define _CHARACTER_STRUCTURE_H_

#include <Creature/CreatureStructure.h>


// 전방 참조
class CCharacter;

// -----------------------------------------------------------------------
// DB 에서 처음에 읽어 오는 데이터들. 중간중간 저장해서 DB에 갱신함.
// 되도록이면 게임 처리중에는 건드리지 않는다. ( 레벨업 등등을 제외하고 ... )

struct CharacterDBData
{
	CHAR_INFOST		m_Info;			// 외모, 이름, 능력치, hp,mp 등 기본 정보
	CHAR_POS		m_Pos;			// 종료시 위치 정보,  저장된 위치 정보 ( Respawn 장소 )
	SKILL			m_Skill;
	QUICK			m_Quick;
	SPELL			m_Spell;
	unsigned char   m_cAdminLevel;
};
typedef CharacterDBData* LPCharacterDBData;


// 유저 접속 정보. 유저의 IP를 넣음.
struct ConnectInfo
{
	SOCKADDR_IN	m_siAgentHost;		// agent UDP address of host
	SOCKADDR_IN	m_siPrivateHost;	// private UDP address of host
	SOCKADDR_IN	m_siPublicHost;		// public UDP address of host
};
typedef ConnectInfo* LPConnectInfo;


// 캐릭터끼리의 전투 정보 (듀얼, 전투)
struct CharacterFightInfo
{
	CCharacter*		m_pDuelOpponent;

	POS				m_Pos;
	unsigned long	m_dwCellID;

	__int64			m_nRestoreExp;	// 스킬 리저렉션을 통해 부활할 경우 일정량의 경험치가 복구될 수 있다.

	CharacterFightInfo();
};


#endif
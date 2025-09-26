#ifndef _CHAR_ATTACK_PACKET_STRUCT_H_
#define _CHAR_ATTACK_PACKET_STRUCT_H_

#include <DB/DBDefine.h>

#pragma pack(1)

// 방어자 정보	( 차후 서버에서 처리 시 없어질 구조체 )
typedef struct AtNode *LPAtNode;
struct AtNode
{
	enum 
	{ 
		MAX_MONSTER_DEFENDER_NUM	= 5,
		MAX_DEFENDER_NUM			= 10 
	};

	unsigned char	m_wDefenserNum;                     // 방어자 수
	unsigned long	m_dwDefenser[MAX_DEFENDER_NUM];     // 방어자 아이디
	unsigned char	m_cDefenserJudge[MAX_DEFENDER_NUM]; // 방어자 판정
};

// 공격 정보
typedef struct AtType* LPAtType;
struct AtType 
{
	enum 
	{ 
		SKILL_BIT	= 0x8000 
	};

	enum AttackType
	{
		RIGHT_MELEE	= 0, 
		LEFT_MELEE	= 1,

		BOW         = 3,
		CROSSBOW	= 4,

		DEFENCE		= 5,
		GUARD		= 6
	};

	unsigned short	m_wType;                    // 공격 타입 (또는 스킬 ID)

	unsigned char	m_cCasting			: 1;	// 캐스팅 (공격 타입을 스킬 ID로 사용하므로 별도의 공간을 둡니다.)
	unsigned char	m_cAtCount			: 3;    // 공격 카운트 (0~7)
	unsigned char	m_cSkillLockCount	: 3;    // 스킬 락카운트 (0~7)
	unsigned char	m_cMissileAttack	: 1;	// 포탄이 떨어지는 위치에 의한 공격 (특정 타켓이 아님)

	char			m_cSkillLevel;			    // 스킬 레벨 (-127 ~ 128)

	POS				m_DstPos;					// 공성 병기의 포탄이 떨어진 위치
};

// 방어자 노드
typedef struct DefenserNode* LPDefenserNode;
struct DefenserNode 
{
	unsigned long	m_dwCharID;     // 방어자 캐릭터 아이디
	
	unsigned short	m_wMaxHP;	    // 최대 HP
	unsigned short	m_sCurrHP;		// 현재 HP
	unsigned short	m_wMaxMP;		// 최대 MP
	unsigned short	m_sCurrMP;      // 현재 MP
	unsigned short	m_wMPHeal;		// MP 회복량

    unsigned short	m_wDamage;      // 데미지 값
    unsigned char	m_cJudge;       // 판정 값
};

#pragma pack()

#endif
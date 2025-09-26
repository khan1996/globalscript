#ifndef _SKILL_STRUCTURE_H_
#define _SKILL_STRUCTURE_H_

#pragma once

#include <Utility/TypeArray.h>
#include <Item/ItemStructure.h>

#pragma pack(8)

namespace Skill
{
    enum Const
	{ 
		SKILL_MASK			= 0x8000,
	
		ERROR_OF_DISTANCE	= 10		// 거리 예상 오차 (스킬 타겟 거리 계산시 사용)
	};

	namespace Type 
	{
		enum AbilityType
		{
			AB_ANIMAL_TALK		= 0,	// 동물NPC와 대화
			AB_SOCIALEX			= 1,	// 확장 소셜모션 사용가능
			AB_NOFIRSTATK		= 2,	// 선공몹 공격 안당함
			AB_MOUNT_SPEEDUP	= 3,	// 탈것 이속 증가
			AB_TEMPINVEN_TIMEUP = 4,	// 임시인벤토리 시간증가
			AB_GETUP_GOLD		= 5,	// 골드 획득량 증가
			AB_RESPAWN_EX		= 6,	// 확장 리스폰 사용 
			AB_FOOD_UP			= 7,	// 음식류 능력치 증가
			AB_SHOP_MEMBERSHIP	= 8,	// 상점 멤버십(구매가격 하락, 판매가격 상승)
			AB_CHEMICAL			= 9,	// 잡템을 회복템으로 변경(캐미컬)
			AB_ENDUR_SHILD		= 10,	// 내구도 하락 감소
			AB_RECOVERY_UP		= 11,	// HP,MP 회복도 증가
			AB_GATHER_FIND		= 12,	// 미니맵에 게더를 보여준다
			AB_MYEQUIP_UP		= 13,	// 자신의 장비 획득율 증가
			AB_LUCK_UP			= 14,	// 럭찬 증가
			AB_EQUIP_LEVELDOWN	= 15,	// 장비의 제한 레벨을 다운시킨다.
			AB_HERBS			= 16,	// 약초 채집
			AB_MINING			= 17,	// 채광
			MAX_ABILITY_TYPE	= 18
		};

        enum SkillType
        {
		    NONE			= 0,
		    PASSIVE			= 1,
		    INSTANCE		= 2,
		    CAST			= 3,
		    CHANT			= 4,
		    ENCHANT			= 5,
			ITEM			= 6,
			SET				= 7,
			ACTION			= 8, 
			ABILITY			= 9,
			GATHER			= 10,

		    MAX_SKILL_TYPE	= 11
        };

		extern const CTypeName SkillTypes[MAX_SKILL_TYPE];

		enum DRCType
		{
			MAGIC			= 0,
			RIGHT_WEAPON	= 1,
			LEFT_WEAPON		= 2
		};
	};

	namespace UseLimit 
	{
        enum Type
        {
		    NONE					= 0,

		    // 인간
		    FOR_FIGHTER				= 1, //  스탶 대거 맨손 활 석궁 사용불가
		    WITH_SHIELD				= 2, //  방패착용				: 방패가 없으면 사용할 수 없다.
		    FOR_FIGHTER_TWOHANDED	= 3, //  양손무기착용(스탶제외)	: 양손무기 착용시에만 사용이 가능하다. 스태프일 경우에는 사용이 불가능하다.
		    FOR_ALL_ONEHAND			= 4, //  한손무기착용			: 양손무기나 맨손으로는 사용이 불가능하다. 대거 제외
		    WITH_DAGGER				= 5, //  대거착용				: 대거 착용시에만 사용이 가능하다.
		    WITH_THROWING_DAGGER	= 6, //  쓰로잉대거소모			: 인벤토리에 쓰로잉 대거가 남아있을 때에만 사용이 가능하다. (손에 장착되어 있을 필요는 없다.)
		    RANGED					= 7, //  활/석궁장착,화살소모	: 활이나 석궁이 장착되어 있고 인벤토리에 화살이 남아있을 때에만 사용이 가능하다.
    		
		    // 아칸		
		    LEFT_ARM	            = 8,  // 왼팔장착			: 왼팔을 장착하고 있어야지만 사용이 가능하다.
		    GUARD_ARM	            = 9,  // 가드암장착			: 방어용의 암을 장착하고 있어야지만 사용이 가능하다.
		    WITH_WEAPON	            = 10, // 무기장착			: 오른손이 맨손일 때에는 사용이 불가능하다.
		    ATTACK_ARM	            = 11, // 어택암장착			: 공격용의 암을 장착하고 있어야지만 사용이 가능하다.
		    GUN_ARM		            = 12, // 건암장착 탄약소모	: 장거리용의 암을 장착하고 탄약을 가지고 있어야지만 사용이 가능하다.
		    KNIFE_ARM	            = 13, // 나이프암장착		: 나이프형의 암을 장착하고 있어야지만 사용이 가능하다.
		    WITH_CLOW	            = 14, // 클로우장착			: 클로우 착용시에만 사용이 가능하다.

		    MAX_USE_LIMIT			= 15
        };

		extern const CTypeName UseLimits[MAX_USE_LIMIT];
	};

	// 스킬 습득 제한
	namespace StatusLimit	
	{	
        enum Type
        {
		    NONE	            = 0,	// 제한 없음
		    STR		            = 1,	// STR이 일정 이상 필요
		    DEX		            = 2,	// DEX가 일정 이상 필요
		    CON		            = 3,	// CON이 일정 이상 필요
		    INT		            = 4,	// INT가 일정 이상 필요
		    WIS		            = 5, 	// WIS가 일정 이상 필요
			LEVEL				= 6,	// LEVEL이 일정 이상 필요	//--//

            MAX_STATUS_LIMIT    = 7		//--//
        };
		
		extern const CTypeName StatusLimits[MAX_STATUS_LIMIT];
	};

	namespace Target
	{ 
        enum Type
        {
		    NONE				= 0,
		    MELEE				= 1,
		    FRIEND				= 2,
		    ENEMY				= 3,
		    DEAD_FRIEND			= 4,
		    DEAD_ENEMY			= 5,
		    FRIEND_OBJECT		= 6,
		    ENEMY_OBJECT		= 7,
		    PARTY				= 8,
		    FRIEND_EXCEPT_SELF	= 9,
		    LINE_ENEMY			= 10,
			SUMMON				= 11,
		    MAX_TARGET_TYPE		= 12
        };

		extern const CTypeName TargetTypes[MAX_TARGET_TYPE];
	};


    struct ProtoType
	{
		enum 
		{ 
			MAX_LIMIT_NUM		= 2, 
			MAX_FILE_NAME		= 32, 
			MAX_SKILL_NAME		= 32, 
			MAX_SKILL_DESCRIBE	= 256 
		};


		char			    m_szEffectFileName[MAX_FILE_NAME];		// 이펙트 파일명
		char			    m_szHitFileName[MAX_FILE_NAME];			// 히트 파일명
		char			    m_szCastingFileName[MAX_FILE_NAME];		// 캐스팅 파일명
        Item::SpriteData    m_SpriteInfo;							// 스프라이트 정보
		char				m_szSkillDescribe[MAX_SKILL_DESCRIBE];	// 스킬 설명
		
		Type::SkillType     m_eSkillType;		// See Namespace Type
        Target::Type	    m_eTargetType;		// See Namespace Target
        UseLimit::Type      m_eUseLimit;		// See Namespace UseLimit

        unsigned char	    m_StatusLimitType[MAX_LIMIT_NUM];		// 습득 제한 스탯 종류
		char			    m_StatusLimitValue[MAX_LIMIT_NUM];		// 습득 제한 스탯 값

		unsigned short	    m_usSkill_ID;		// 스킬 아이디

		unsigned short	    m_StartMP;			// 시작 소모 MP
        unsigned short	    m_LevelMP;			// 레벨당 소모 MP
		unsigned short		m_LockMP;			// 락카운트당 소모 MP

		unsigned short	    m_StartTick;		// 시작 틱카운트
        unsigned short	    m_LevelTick;		// 레벨당 틱카운트

		unsigned short	    m_usParentSkill;	// 부모 스킬 아이디
		unsigned short		m_usChildSkill;		// 자식 스킬 아이디
		
		unsigned long		m_dwCoolDownTime;	// 쿨다운 시간
		
		float				m_fMinRange;		// 최소 사정 거리
		float			    m_fMaxRange;		// 최대 사정 거리
		float			    m_fEffectExtent;	// 효과 범위

		bool			    m_bProtection;		// 보호 스킬에 의해 막히는가?
		bool			    m_bGauge;			// 연속 사용이 가능한 스킬인가? (ex. 패스트히드)
        bool			    m_bIsClassSkill;	// 클래스 스킬인가?
        bool			    m_bInterrupt;		// 스턴을 깨는 스킬인가?
		bool				m_bCounter;			// 카운터 당하는 스킬인가?

		unsigned char	    m_cEndScript;		// EndScript를 사용하는가?
        unsigned char	    m_cEndCoolDown;		// 스킬 효과가 없어지면 쿨다운이 돌아간다. (ex. 스텔스)
        unsigned char	    m_cStrikeNum;		// 다단 히트 횟수

		unsigned char	    m_cPadding[1];

		ProtoType();
        void Initialize();
	};
}

#pragma pack()

#endif
#ifndef _ITEM_CONSTANTS_H_
#define _ITEM_CONSTANTS_H_

#include <vector>
#include <Utility/TypeArray.h>

namespace Item
{
    // 전방 참조
    struct ItemInfo;
    struct ItemAttribute;

	class CItem;

	enum Const
	{
		MAX_GEM_KIND				= /*14*/ 5,		// 보석의 종류
		MAX_LEVEL_OPTION_UPGRADE	= 5,		// 제련시 옵션이 증가하는 최대 레벨 (이후 레벨에선 소켓이 증가한다.)
		MAX_OPTION_BASE_NUM			= 6,		// 한 아이템에 붙은 수 있는 +의 최대 갯수

		GEMETYPE_RUNE				= 5,		// 5레벨이 룬타입의 젬이다
		GEMETYPE_RUNE_NUM			= 12,		// 젬타입의 룬은 총 12개이다 현재 3개 사용

		MAX_TEMP_INVEN_ITEM_NUM		= 20,		// 임시 인벤토리에 넣을 수 있는 최대 아이템 갯수

		PRICE_AGGRAVATION			= 7,		// 리밸런싱 전 아이템에 대한 가격 가중치

		GM_MODEL_ITEM				= 49999,	// GM 모델로 쓰는 아이템 아이디.

		MAX_UPGRADE_COUNT			= 1,		// 고대의 돌 연속 업그래이드 수.
	};

    namespace ItemType
    {
        // 아이템 타입 ( 8bit )
        enum Type
        {
			NONE					= 0, 

			// 비기능성 장비
			SHIRT					= 1,	TUNIC					= 2,

			// 인간 방어구
			CON_ARMOUR				= 3,	CON_HELM				= 4,
			CON_GLOVE				= 5,	CON_BOOTS				= 6,
			DEX_ARMOUR				= 7,	DEX_HELM				= 8, 
			DEX_GLOVE				= 9,	DEX_BOOTS				= 10, 

			// 인간 무기
			ONEHANDED_SWORD			= 11,	TWOHANDED_SWORD			= 12, 
			ONEHANDED_AXE			= 13,	TWOHANDED_AXE			= 14,
			ONEHANDED_BLUNT			= 15,	TWOHANDED_BLUNT			= 16,
			BOW						= 17,	CROSSBOW				= 18,
			STAFF					= 19,	DAGGER					= 20,
			SHIELD					= 21, 

			// 아칸 방어구
			CON_BODY				= 22,	CON_HEAD				= 23, 
			CON_PELVIS				= 24, 	CON_PROTECT_A			= 25,
			DEX_BODY				= 26,	DEX_HEAD				= 27, 
			DEX_PELVIS				= 28,	DEX_PROTECT_A			= 29, 

			// 아칸 무기
			COM_BLUNT				= 30,	COM_SWORD				= 31,
			OPP_HAMMER				= 32, 	OPP_AXE					= 33, 
			OPP_SLUSHER				= 34,	OPP_TALON				= 35, 
			OPP_SYTHE				= 36,	

			// 스킬암
			SKILL_A_GUARD			= 37,	SKILL_A_ATTACK			= 38,
			SKILL_A_GUN				= 39,	SKILL_A_KNIFE			= 40, 

			// 탄약류
			ARROW					= 41,	BOLT					= 42, 
			AMMO					= 43, 

			// 사용 아이템
			POTION					= 44,	EXP_BOOK				= 45,
			CASH_BOOK				= 46,	EVENT_LOTTERY			= 47, 
			PORTAL					= 48,	FIRE_CRACKER			= 49, 
			SKILL_BOOK				= 50,	FIXED_PORTAL			= 51,	
			SKILL_ITEM				= 52,	CASH_ITEM				= 53, 

			// 사용 불가 아이템
			GEM						= 54,	ORE						= 55,
			ETC						= 56, 	

			// 공성용 스타터킷
			CAMP_KIT				= 57,	SHORT_RANGE_ARMS_KIT	= 58, 
			LONG_RANGE_ARMS_KIT		= 59, 	AIRSHIP_KIT				= 60,
			WORLDWEAPON_K_KIT		= 61,	WORLDWEAPON_M_KIT		= 62,
			MINERAL_KIT				= 63,

			// 아이템 퀘스트용 아이템
			QUEST_ITEM				= 64,

			// 악세사리 아이템
			NECKLACE				= 65,	RING					= 66,
			RUNE					= 67,

			// 판매 전용 추가 아이템(구매/판매가 동일. 2006-06-19 by hackermz)
			GEM_SELL				= 68,

			RIDE					= 69,	TRANSFORM				= 70,

			//dedi start here
			COSTUME					= 71,
			BACKSIDE				= 72,
			BADGE					= 73,
			COSTUME_WEAPON			= 74,
			COSTUME_ARM				= 75,
			COSTUME_SHIELD			= 76,
			COSTUME_HELM			= 77,

			MAX_ITEM_TYPE			= 78,
        };
    };

	// 아이템 장착 제한
	namespace StatusLimit 
	{        
		enum Type
		{
			NONE	            = 0,	
			STR		            = 1,	
			DEX		            = 2,	
			CON		            = 3,	
			INT		            = 4,	
			WIS		            = 5, 
			LEVEL				= 6,

			MAX_STATUS_LIMIT    = 7
		};
	};

    namespace Attribute
    {
		enum Type
		{
			// ---------------------------------------------------------------------------
			// 이 곳의 속성은 DB에 저장되는 값으로 수정시에 DB 컨버팅이 필요합니다.
			NONE					= 0,

			MIN_DAMAGE				= 1,
			MAX_DAMAGE				= 2,
			ARMOR					= 3,
			HIT_RATE				= 4,
			EVADE					= 5,

			MAX_HP					= 6,
			HP_REGEN				= 7,
			MAX_MP					= 8,
			MP_REGEN				= 9,

			CRITICAL				= 10,
			BLOCK					= 11,
			SPEED					= 12,

			MAGIC_POWER				= 13,
			MAGIC_RESIST			= 14,

			LUCKCHANCE				= 15,	

			MAX_DB_ATTRIBUTE_NUM	= 16, 
			// ---------------------------------------------------------------------------

			// ---------------------------------------------------------------------------
			// 이후 추가 속성은 DB에 저장되지 않습니다. (랜덤 옵션이 붙을 수 없습니다.)
			COOLDOWN				= 16,
			SKILL_POINT				= 17,

			FROST					= 18,
			FIRE					= 19,
			ELECTRO					= 20,
			DARKNESS				= 21,

			RUNTYPE					= 22,
			MAX_ATTRIBUTE_NUM		= 22	
			// ---------------------------------------------------------------------------
		};

		enum Trend
		{
			Aggressive	= (1 << 0),
			Defensive	= (1 << 1)
		};

		enum CriticalType
		{
			NONE_TYPE	= 0,	
			
			SWORD_TYPE	= 1,	
			AXE_TYPE	= 2,	
			BLUNT_TYPE	= 3
		};

		extern const CTypeName   Attributes[MAX_ATTRIBUTE_NUM];
    };

	namespace EquipType 
	{ 
		enum Type
		{
			ARMOR_TYPE				= 0,
			DAGGER_TYPE				= 1,
			ONEHANDED_WEAPON_TYPE	= 2,
			LONGRANGE_WEAPON_TYPE	= 3,
			TWOHANDED_WEAPON_TYPE	= 4,
			STAFF_TYPE				= 5,
			SHIELD_TYPE				= 6,

			MAX_TYPE				= 7
		};

		enum Grade
		{
			S_GRADE			= 0,
			AAA_GRADE		= 1,
			AA_GRADE		= 2,
			A_GRADE			= 3,
			B_GRADE			= 4,
			C_GRADE			= 5,
			D_GRADE			= 6,
			F_GRADE			= 7,
			X_GRADE			= 8,
			
			MAX_GRADE		= 9
		};

		enum OptionType
		{
			NONE					= 0,
			STANDARD_OPTION			= 1,
			OVER_OPTION				= 2,
			NO_OPTION				= 3,

			MAX_OPTION_TYPE			= 4
		};
	};

	namespace EquipmentPos 
    {
        // 장비 위치 ( 8bit )
        enum Type
        {
            // 인간 장비 위치
		    HELM				= 0,		    SHIRT				= 1,
		    TUNIC				= 2,		    ARMOUR				= 3,
		    GLOVE				= 4,		    BOOTS				= 5,
		    NECKLACE			= 6,		    RINGR				= 7,
		    RINGL				= 8,		    SHIELD_HAND1		= 9,
		    SHIELD_HAND2		= 10,		    WEAPON_HAND1		= 11,
		    WEAPON_HAND2		= 12,		    AVATA				= 13,	

            // 아칸 장비 위치
		    HEAD				= 0,		    BODY				= 3,
		    PROTECT_ARM			= 4,		    PELVIS				= 5,
			SKILL_ARM			= 9,		    WEAPON				= 11,

			BACKSIDE			= 14,
			COSTUME				= 15,
			BADGE				= 16,
			COSTUME_RIGHT		= 17,
			COSTUME_LEFT		= 18,
			COSTUME_HELM		= 19,
		    MAX_EQUPMENT_POS	= 20,

		    TEMP_ITEM_INDEX		= 128
        };
	};

	namespace ExtraSpacePos
	{
        enum Type
        {
			// --------------------------------------------------------------------------
			// 2004-07-14 by 로딘
			//
			// Extra의 DB 크기는 현재 사용량은 최대 494 / 508 bytes 
			// ( DB 크기를 초과하지 않도록 주의합시다. )
			//
			// 참고 자료 :	잡템의 크기는 14 bytes, 
			//				장비의 최대 크기는 52 bytes 
			//				( 풀옵션일때... 14(기본) + 6(장비) + 8(보석) + 24(옵션) )
			// --------------------------------------------------------------------------
			
		    UPGRADE_EQUIPMENT_POS	= 0,	// 제련 장비
		    UPGRADE_MATERIAL_POS	= 1,	// 제련 광물

			HOLDITEM_POS			= 2,	// 들고 있는 아이템 

			KIT_MATERIAL_POS		= 3,	// 공성병기 제작 자재
			CAMP_MATERIAL_POS		= 4,	// 길드 요새 제작 자재 (사용 안함)

			EMBLEM_JEWEL_POS1		= 5,	// 상징물 업그레이드 보석 (사용 안함)
			EMBLEM_JEWEL_POS2		= 6,	// (사용 안함)	
			EMBLEM_JEWEL_POS3		= 7,	// (사용 안함)	
			EMBLEM_JEWEL_POS4		= 8,	// (사용 안함)
			EMBLEM_JEWEL_POS5		= 9,	// (사용 안함)	
			EMBLEM_JEWEL_POS6		= 10,	// (사용 안함)	
			EMBLEM_JEWEL_POS7		= 11,	// (사용 안함)	
			EMBLEM_JEWEL_POS8		= 12,	// (사용 안함)	
			EMBLEM_JEWEL_POS9		= 13,	// (사용 안함)	

			GRAFT_ORIGINAL_POS		= 14,	// 옵션 이식 원본 장비
			GRAFT_SACRIFICE_POS		= 15,	// 옵션 이식 제물 장비
			GRAFT_RESULT_POS		= 16,	// 옵션 이식 결과 장비

			COMPENSATION_POS		= 17,	// 보상 판매 대상
		    
			MAX_EXTRA_SPACE_NUM		= 18,

			EMBLEM_JEWEL_POS_NUM	= 9
        };
	};

	namespace GiveItem 
	{
		enum ExceptionUID 
		{
			// 쿠폰으로 아이템 지급시 임시 인벤토리가 꽉찬 상태라면 지급을 펜딩한다. 이 때, 사용되는 예외 UID
			PENDING_ITEM_UID		= 0xFFFFFFFF
		};
	};

    namespace EtcItemID
    {
        enum Type
        {
            // 보석 아이템 ID
            GEM_START_ID            = /*1901*/ 15001,
            GEM_END_ID              = /*1942*/ 15015,

            GEM_PERFECTEND_ID       = /*1956*/ 15020,
			GEM_RUNE_START_ID		= /*1957*/ 15021,		// 보석속성인데 매찬올려주는 아이템(룬보석이라고 불림)
			GEM_RUNE_END_ID			= /*1968*/ 15032,
			GEM_UNSOCKET_ID			= 1970,
			GEM_MAXSOCKET_ID		= 1971,

			// 룬 아이템 ID
			RUNE_START_ID			= 40000,	// 시작 아이디는 아이템에 추가될 수 없음 (고로 소멸의 룬 ID와 같은 것이 베스트!)
			RUNE_END_ID				= 44096,

			// 소멸의 룬 ID
			DESTRUCTION_RUNE		= 40000,

            // 광물 아이템 ID
            MINERAL_START_ID        = 2001,
            MINERAL_END_ID          = 2003,

			SILVIN					= 2001,
			MITHRIL					= 2002,
			ITERNIUM				= 2003,
			SOULMETAL				= 2004,
			ANTONIUM				= 2005,
			ANCIENTMETAL			= 2006,
			ENDURANCESTONE			= 2007,	// 내구도 복구 아이템
			AWAKENCORE				= 2008,	// 각성의 코어 (시즌레코드3으로 변경되고 장비착용시 착용레벨-5 효과)

			// 고철 ID
			SCRAPS                  = 2101,

			// 회복약 아이템 ID
			BREAD					= 3020,
			RARE_STEAK				= 3021, 
			WELLDONE_STEAK			= 3022, 
			BACON					= 3023, 
			MANNA1					= 3024,
			MANNA2					= 3025,
			MANNA3					= 3026,
			MANNA4					= 3027,

			// 스킬북 쿠폰 ID
			SKILL_COUPON_3			= 3401,
			SKILL_COUPON_4			= 3402,

			// 공성 관련 스타트킷 ID
			CAMP_KIT_ID				= 7100,
			SHORT_RANGE_ARMS_KIT_ID	= 7101,
			LONG_RANGE_ARMS_KIT_ID	= 7102,
			AIRSHIP_KIT_ID			= 7103,
			WORLDWEAPON_K_KIT		= 7105,
			WORLDWEAPON_M_KIT		= 7106,
			MINERAL_KIT				= 7107,

			// 공성 관련 자재 ID
			SIEGE_MATERIAL_ID		= 7104,

			// 망각의돌 ID
			OBLIVION_STONE			= 9914,
			// 부활의돌 ID
			REBIRTH_STONE			= 9201,
			// 계명허가서 ID
			RENAME_WARRANT			= 9200,

        };
    };

	namespace Chemical
	{
		// 아이템 합성 결과
		enum Result
		{
			S_SUCCESS			= 0,	// 성공

			F_WRONG_RECIPE		= 2,	// 올바른 합성법이 아닙니다.
			F_NOT_ENOUGH_ITEM	= 3,	// 합성 아이템의 수가 최소량에 미치지 못합니다.
			F_WRONG_RATE		= 4,	// 대상 아이템의 숫자에 나머지 값이 발생합니다. 합성 비율을 맞춰주시기 바랍니다.
			F_NOT_QUEST			= 5,	// 필요퀘스트를 하지 못했다.
			F_NOT_ABILITY		= 6,	// 해당 어빌리티가 충족하지 않는다.
		};
	};

	namespace Compensation
	{
		// 클래스별 특수 보상 판매
		enum SpeacialCase
		{
			CASE1		= ItemType::MAX_ITEM_TYPE, 
			CASE2		= ItemType::MAX_ITEM_TYPE + 1, 
			CASE3		= ItemType::MAX_ITEM_TYPE + 2, 
			CASE4		= ItemType::MAX_ITEM_TYPE + 3, 
			CASE5		= ItemType::MAX_ITEM_TYPE + 4, 
			CASE6		= ItemType::MAX_ITEM_TYPE + 5, 
			CASE7		= ItemType::MAX_ITEM_TYPE + 6, 
			CASE8		= ItemType::MAX_ITEM_TYPE + 7, 
			CASE9		= ItemType::MAX_ITEM_TYPE + 8, 

			MAX_CASE	= ItemType::MAX_ITEM_TYPE + 9
		};
	};

	class CItemType
    {
    public:

        enum ArrayType
        {
            EQUIP_TYPE			= 0,
			USE_ITEM_TYPE		= 1,

			WEAPON_TYPE			= 2,    
			SKILL_ARM_TYPE		= 3,    

			HELM_TYPE			= 4,
			ARMOUR_TYPE			= 5,
			DAGGER_TYPE			= 6,
			ONEHANDED_TYPE		= 7,
			LONGRANGE_TYPE		= 8,
			TWOHANDED_TYPE		= 9,
			STAFF_TYPE			= 10,
			SHIELD_TYPE			= 11,

			STANDARD_TYPE		= 12,
			OVER_TYPE			= 13,
			NO_TYPE				= 14,

			ACCESSORY_TYPE		= 15,

			AVATA_TYPE			= 16,

			MAX_ARRAY_TYPE		= 17
        };

        typedef std::vector<ItemAttribute>    ItemAttributeArray;

        static CItemType& GetInstance();

        // 정적 멤버 함수        
		static ArrayType GetEquipType(unsigned long dwItemFlag);

		// Rodin : 이 함수들은 CEquipment의 멤버 함수가 되는게 좋을 듯? (그러면서 CEquipment의 friend도 제거!)
		static void SetInstallGemAttribute(CItem* lpItem);
		static void SetUpgradeItemAttribute(CItem* lpItem);       

		// 멤버 함수
        bool IsCorrectItemType(ArrayType itemType, unsigned char cEquipType);
		bool IsCorrectOptionType(EquipType::OptionType eOptionType, ItemType::Type eItemType);

		EquipType::OptionType GetOptionType(ItemType::Type eItemType);

        void SetEquipTypeFlags(ItemInfo& itemInfo);
		void SetAccessoryTypeFlags(ItemInfo& itemInfo);
		void SetAvataTypeFlags(ItemInfo& itemInfo);
        void SetUseItemTypeFlags(ItemInfo& itemInfo);
        void SetEtcItemTypeFlags(ItemInfo& itemInfo);

        CTypeName* GetItemTypeName(void)	{ return m_ItemTypeNames;	}
		CTypeName* GetItemLimitName(void)	{ return m_ItemLimitNames;	}
		CTypeName* GetItemGradeName(void)	{ return m_ItemGradeNames;	}

    private:

        CItemType();

        typedef std::vector<ItemType::Type> ItemTypeArray;
        ItemTypeArray                       m_ItemTypes[MAX_ARRAY_TYPE];
        CTypeName                           m_ItemTypeNames[ItemType::MAX_ITEM_TYPE];
		CTypeName							m_ItemLimitNames[StatusLimit::MAX_STATUS_LIMIT];
		CTypeName							m_ItemGradeNames[EquipType::MAX_GRADE];
    };
};


#endif
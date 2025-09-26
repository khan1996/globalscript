#ifndef _ITEM_STRUCTURE_H_
#define _ITEM_STRUCTURE_H_

#include <Creature/CreatureStructure.h>
#include "ItemConstants.h"


namespace Item
{
	class CItem;

    #pragma pack(1)

    // ------------------------------------------------------------------------------------------
    // UseLimit, ItemInfo 는 스크립트에만 넣는다.

    struct ItemPos
    {
        unsigned short  m_cPos      : 4;    //  아이템 포지션 (Equip, Inven...) (0~15)
        unsigned short  m_cIndex    : 12;   //  아이템 인덱스 (0~4095)

        ItemPos() : m_cPos(0), m_cIndex(0) { }
        ItemPos(unsigned char cPos, unsigned short usIndex) : m_cPos(cPos), m_cIndex(usIndex) { }

        inline void SetPos(unsigned short wIndex) { m_cIndex = wIndex; }
        inline void SetPos(unsigned char cXIndex, unsigned char cYIndex, unsigned char cZIndex);        
        inline void GetPos(unsigned char& cXIndex, unsigned char& cYIndex, unsigned char& cZIndex) const;

        inline unsigned char GetXIndex(void) const { return (m_cIndex & 0x00F); }
        inline unsigned char GetYIndex(void) const { return (m_cIndex & 0x0F0) >> 4; }
        inline unsigned char GetZIndex(void) const { return (m_cIndex & 0xF00) >> 8; }

        inline void SetXIndex(unsigned char cXIndex) { m_cIndex = (m_cIndex & 0xFF0) + cXIndex; }
        inline void SetYIndex(unsigned char cYIndex) { m_cIndex = (m_cIndex & 0xF0F) + (cYIndex << 4); }
        inline void SetZIndex(unsigned char cZIndex) { m_cIndex = (m_cIndex & 0x0FF) + (cZIndex << 8); }

        inline bool operator == (const ItemPos& rhs) const { return (m_cPos == rhs.m_cPos && m_cIndex == rhs.m_cIndex); }
    };

    inline void ItemPos::SetPos(unsigned char cXIndex, unsigned char cYIndex, unsigned char cZIndex)
    {
        m_cIndex = (cZIndex << 8) + (cYIndex << 4) + cXIndex;
    }

    inline void ItemPos::GetPos(unsigned char& cXIndex, unsigned char& cYIndex, unsigned char& cZIndex) const
    {
        cXIndex = (m_cIndex & 0x00F);
        cYIndex = (m_cIndex & 0x0F0) >> 4;
        cZIndex = (m_cIndex & 0xF00) >> 8;
    }


    struct ItemData
    {
        unsigned __int64    m_dwUID;                //  아이템 UID
        unsigned short      m_usProtoTypeID;        //  아이템 종류 ID
        ItemPos             m_ItemPos;              //  아이템 위치
        unsigned char       m_cItemSize;            //  아이템 구조체 크기
        unsigned char       m_cNumOrDurability;     //  개수 혹은 내구도

        ItemData() { m_dwUID = m_usProtoTypeID = m_cNumOrDurability = 0; m_cItemSize = sizeof(ItemData); }
        void DumpInfo(unsigned long dwCID, const char* szExtraString) const;
    };

    struct ItemAttribute
    {
        unsigned short  m_cType     : 4;    //  속성 개수 4bit (16가지)
        unsigned short  m_usValue   : 12;   //  속성 값  12bit (0 ~ 4096)

        ItemAttribute() : m_cType(0), m_usValue(0) { }
        ItemAttribute(unsigned char cType, unsigned short usValue) : m_cType(cType), m_usValue(usValue) { }
    };

    // 뒤에 Socket내용물이 m_cSocketNum만큼, Attribute가 m_cDiffAttributeNum만큼 붙는다.    
    struct EquipmentInfo
    {
        enum
        {
            MAX_UPGRADE_LEVEL				= 15,   // 최대 제련 레벨
			MAX_CORE_LEVEL					= 10,	// 최대 코어 레벌 (코어란 코어석을 발라 특별한 능력을 부여하는 시스템)

			MAX_ATTRIBUTE_PER_GEM			= 2,	// 보석 하나당 강화되는 최대 속성 수
			MAX_ATTRIBUTE_PER_UPGRADE_LEVEL	= 2,	// 한 단계에 강화되는 최대 속성 수
            MAX_UPGRADE_ATTRIBUTE       	= 6,    // 제련으로 인해서 얻을 수 있는 최대 속성 수

            MAX_MINSIZE_SOCKET_NUM      	= 6,    // 크기가 1by1인 아이템의 최대 소켓 수
            MAX_MINSIZE_ATTRIBUTE_NUM   	= 6,    // 크기가 1by1인 아이템의 최대 속성 수

            MAX_SOCKET_NUM              	= 8,    // 일반 아이템의 최대 소켓 수
            MAX_SOCKET_ATTRIBUTE_NUM    	= 16,   // 소켓 하나당 최대 속성이 두개까지 달릴 수 있음
			// edith 2009.09.16 소켓 개수 수정
//          MAX_ATTRIBUTE_NUM           	= 22,   // 일반 아이템의 최대 속성 수
            MAX_ATTRIBUTE_NUM           	= 15,   // 일반 아이템의 최대 속성 수

			MAX_RUNE_SOCKET_NUM				= 2,	// 장착할수 있는 룬의 최대 수

            MAX_SOCKET_AND_ATTRIBUTE_MINISIZE	= (MAX_MINSIZE_SOCKET_NUM * sizeof(char) + MAX_MINSIZE_ATTRIBUTE_NUM * sizeof(ItemAttribute)),
            MAX_SOCKET_AND_ATTRIBUTE_SIZE		= (MAX_SOCKET_NUM * sizeof(char) + MAX_ATTRIBUTE_NUM * sizeof(ItemAttribute))
        };
        
		unsigned short	m_cUpgradeCnt			:	2;	// 재련시 고대의 돌을 연속적 사용한 카운트.
		unsigned short  m_cReserved1			:   9;	// 예약 데이터... (0~511)
		short           m_cUpgradeLevel			:   5;  // 업그레이드 단계 (-16 ~ 15)

		char            m_cDiffMaxSocket        :   5;  // 상점 아이템 소켓 수 + m_cDiffMaxSocket = 이 아이템의 최대 소켓 수 (-16 ~ 15)
		unsigned char   m_cSeasonRecord			:	3;  // 아이템 생성 시기 (0~7) (리밸런싱 전 : 0, 후 : 1, 매찬으로 능력증가 : 2)

		unsigned char   m_cSocketNum            :   4;  // 이 아이템의 현재 소켓 수 (0 ~ 15)
		unsigned char   m_cAttributeNum         :   4;  // 상점 아이템에 없거나, 수치가 다른 속성 수 (0 ~ 15개)

		char            m_cDiffMaxDurability;           // 상점 아이템과의 최대 내구도 차이.
		unsigned char   m_cCoreLevel			:	4;  // 코어의 레벨 (0 ~ 15)
		unsigned char   m_cReserved2			:	4;	// 예약 데이터...(0~124)

		EquipmentInfo()
		{
			m_cUpgradeCnt = m_cReserved1 = m_cUpgradeLevel = m_cDiffMaxSocket = m_cDiffMaxDurability = 
				m_cSocketNum = m_cAttributeNum = m_cSeasonRecord = m_cCoreLevel = m_cReserved2 = 0;
		}
    };

    #pragma pack()

    const unsigned char MAX_ITEM_SIZE = sizeof(ItemData) + 
        sizeof(EquipmentInfo) + EquipmentInfo::MAX_SOCKET_AND_ATTRIBUTE_SIZE;


    struct SpriteData
    {
	    enum { MAX_NAME = 64, MAX_SPRITE_NAME = 32 };

	    char    m_szName[MAX_NAME];
	    char    m_szSpriteName[MAX_SPRITE_NAME];

	    unsigned short	m_nSpriteMinX;
	    unsigned short	m_nSpriteMinY;
	    unsigned short	m_nSpriteMaxX;
	    unsigned short	m_nSpriteMaxY;

        SpriteData();
        void Initialize();
    };

    
    struct StringData
    {
	    enum
        {
            MAX_FIELD_MODEL_NAME = 32,  MAX_EQUIP_MODEL_NAME = 32,
            MAX_EFFECT_SOUND_NAME = 32, MAX_TYPE_NAME = 32, 
			MAX_ITEM_DESCRIBE = 256
	    };

	    char	m_szFieldModelName[MAX_FIELD_MODEL_NAME];
	    char	m_szEquipModelName[MAX_EQUIP_MODEL_NAME];
	    char	m_szEffectSoundName[MAX_EFFECT_SOUND_NAME];
		char	m_szTypeName[MAX_TYPE_NAME];
	    char	m_szItemDescribe[MAX_ITEM_DESCRIBE];

        StringData();
        void Initialize();
    };

	
    struct DetailData
    {
        enum Flags
        {
            EQUIP           = (1 << 0),  	// 장비할수 있는지 여부
            USE_ITEM        = (1 << 1),  	// 사용할 수 있는지 여부            
            QUICKSLOT_IN    = (1 << 2),  	// 퀵슬롯에 들어갈 수 있는지 여부
            STACKABLE       = (1 << 3),  	// 스택할 수 있는지 여부

			HELM			= (1 << 4),		// 투구류
			ARMOR			= (1 << 5),  	// 갑옷류
            DAGGER          = (1 << 6),  	// 단검류
            ONEHANDED       = (1 << 7),  	// 한손 무기류
			LONGRANGE		= (1 << 8),		// 장거리 무기류 
            TWOHANDED       = (1 << 9),  	// 양손 무기류
			STAFF			= (1 << 10),	// 지팡이류
			SHIELD			= (1 << 11),	// 방패류

			//--//	start..
			NECKLACE		= (1 << 12),	// 목걸이
			RING			= (1 << 13),	// 반지
			RUNE			= (1 << 14),	// 룬
			AVATA			= (1 << 15),	// 룬
			ACCECORIES		= (1 << 16),
			//--//	end..
        };

		unsigned long   m_dwPrice;			// 기본 가격
		unsigned long	m_dwBlackPrice;		// 암시장용 판매 가격
		unsigned long	m_dwMedalPrice;		// 메달상점용 판매 가격

		unsigned short	m_wDropOption;		// 가치 책정을 위한 값
		unsigned char	m_cGrade;			// 가치 책정값이 0 일때 적용될 등급	//--//
		unsigned char	m_cCriticalType;	// 크리티컬 타입
		unsigned char	m_cAttackRange;		// 공격 거리
                
		unsigned char	m_cItemType;		// 아이템 종류( namespace Type 참고 )
		unsigned char	m_cXSize;       	// 아이템 X크기
		unsigned char	m_cYSize;       	// 아이템 Y크기

		unsigned char   m_cMaxSocketNum;				// 최대 소켓수
		unsigned char   m_cDefaultDurabilityOrStack;	// 기본 스택/내구도
        unsigned char   m_cMaxDurabilityOrStack;		// 최대 스택/내구도

		unsigned char	m_cMinDropLevel;	// 드랍하는 몬스터의 최소 레벨
		unsigned char	m_cMaxDropLevel;	// 드랍하는 몬스터의 최대 레벨

		unsigned short	m_wLowLimitLevel;	// 장착 하한선.
		unsigned short	m_wHighLimitLevel;	// 장착 상한선.

		bool			m_bExchangeAndDrop;	// 교환과 드랍이 가능한가?
		bool			m_bSell;			// 팔 수 있는 아이템인가?

		bool			m_bOptionPrice;		// 옵션을 참고해서 가격 책정을 다시 할 것인가?

        unsigned long   m_dwFlags;

		DetailData();
        void Initialize();
    };
	
    struct EquipAttribute
    {           
        unsigned short m_usAttributeValue[Item::Attribute::MAX_ATTRIBUTE_NUM];        
        unsigned short m_nAttibuteNum;

        EquipAttribute();
        void Initialize();
    };

    struct UseItemInfo
    {
		Position		m_Pos;					// 이동 좌표 (순간이동서)
		unsigned char	m_cZone;				// 이동 존 (순간이동서)

		unsigned long	m_dwAmount;				// 양 (경험치쿠폰, 캐쉬북)

		unsigned short	m_usSkill_ID;			// 관련 스킬 아이디 (포션, 스킬북)
        unsigned short	m_usSkill_LockCount;	// 관련 스킬 락카운트 (포션, 스킬북)

		unsigned short	m_wQuestID;				// 시작 퀘스트 아이디 (퀘스트 시작 아이템)

		unsigned char	m_cLimitRealmPoint;		// 공헌 훈장 포인트별 사용 제한
		bool			m_bForStatueWar;		// 국가전(석상전) 시간에만 사용 가능

        UseItemInfo();
        void Initialize();
    };

    struct UseLimit
    {        
        enum DeactivatedType
		{
			STATUS_LIMIT    = ( 1 << 0 ),
			CLASS_LIMIT     = ( 1 << 1 ),
			BROKEN_ITEM     = ( 1 << 2 )
		};
                
        unsigned long			m_dwClassLimit;

		unsigned char			m_cLimitStatus;
		unsigned short			m_wLimitValue;
         
        UseLimit();
        void Initialize();
	};

    #pragma pack(8)

    struct ItemInfo
    {
        unsigned short	m_usProtoTypeID;

        DetailData      m_DetailData;
        UseLimit        m_UseLimit;
        SpriteData      m_SpriteData;
        StringData      m_StringData;

        EquipAttribute  m_EquipAttribute;
        UseItemInfo     m_UseItemInfo;

        ItemInfo();
        ItemInfo(unsigned short usPrototypeID);
        void Initialize();

        inline bool operator < (ItemInfo& rhs)
        { 
			return (m_usProtoTypeID < rhs.m_usProtoTypeID); 
		}

		const char* GetItemDescribe(void) const;
    };

    #pragma pack()


	struct ItemGarbage
	{
		CItem*			m_lpItem;
		unsigned long	m_dwRemainNum;

		ItemGarbage(CItem* lpItem, unsigned long dwRemainNum);
	};

	struct ChemicalInfo
	{
		unsigned short	m_wPickkingItemID;
		unsigned short	m_wTargetItemID;
		unsigned short	m_wResultItemID;

		unsigned char	m_cPickkingItemNum;
		unsigned char	m_cTargetItemNum;
		unsigned char	m_cResultItemNum;

		unsigned long	m_dwItemID;		// 아이템
		unsigned short	m_usAbilityID;	// 어빌리티아이디
		unsigned char	m_cAbilityLevel;
		unsigned short	m_usQuestID;	// 퀘아이디		
		unsigned char	m_cPercent;		// 성공확율


		ChemicalInfo();
		ChemicalInfo(unsigned short wPickkingItemID, unsigned char cPickkingItemNum, 
			unsigned short wTargetItemID, unsigned char cTargetItemNum, unsigned int dwItemID, 
			unsigned short usAbilityID, unsigned char cAbilityLevel, unsigned short	usQuestID, unsigned char cPercent);

		bool operator < (const ChemicalInfo& rhs)
		{ 
			if (m_wPickkingItemID == rhs.m_wPickkingItemID)
			{
				return m_wTargetItemID < rhs.m_wTargetItemID; 
			}

			return m_wPickkingItemID < rhs.m_wPickkingItemID; 
		}

		bool operator == (const ChemicalInfo& rhs)
		{
			return (m_wPickkingItemID == rhs.m_wPickkingItemID && m_wTargetItemID == rhs.m_wTargetItemID && 
				m_cPickkingItemNum == rhs.m_cPickkingItemNum && m_cTargetItemNum == rhs.m_cTargetItemNum);
		}
	};

	namespace Grade 
	{
		struct GradeInfo
		{
			EquipType::Grade	m_eItemGrade;
			unsigned char		m_cPlus;

			EquipType::Grade	m_aryAttributeGrade[Attribute::MAX_ATTRIBUTE_NUM];

			GradeInfo();
		};

		enum TableType
		{
			DROP_MIN_VALUE		= 0,	// 드랍 그레이드 최소값
			DROP_MAX_VALUE		= 1,	// 드랍 그레이드 최대값
			VALUE_GRADE			= 2,	// 가치 그레이드

			MAX_TABLE_TYPE		= 3
		};

		const float GetGradeValue(Item::CItemType::ArrayType eEquipType, Grade::TableType eTableType, 
			EquipType::Grade eGrade, Attribute::Type eType);
	};
};

#endif
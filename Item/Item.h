#ifndef _CITEM_CLASS_H_
#define _CITEM_CLASS_H_

#include <limits>
#include <vector>
#include <algorithm>
#include <Network/Packet/PacketStruct/CharItemPacketStruct.h>

#include "ItemMgr.h"

#include "ItemStructure.h"

// 전방 참조
class CCharacter;

#define DELITEMLOG(x)       (void*)0
//#define DELITEMLOG(x)     x

namespace Item
{
    class CItem
    {
    public:

        CItem();
        CItem(const ItemInfo& itemInfo);  

        // 스크립트로부터 로드한 아이템 데이터를 얻어 온다.
        const ItemInfo& GetItemInfo() const { return m_ItemInfo; }

        // Desc : 아이템 정보를 char버퍼로 기록한다.
        // In   : 버퍼, 버퍼 크기
        // Out  : (Return)성공 여부, nBufferLength_InOut - 기록한 크기
        virtual bool SerializeOut(char* lpSerializeItem_Out, size_t& nBufferLength_InOut);

        // Desc : char버퍼로부터 아이템 정보를 읽어 온다.
        // In   : 버퍼, 버퍼 크기
        // Out  : (Return)성공 여부, nBufferLength_InOut - 사용한 버퍼 크기
        virtual bool SerializeIn(const char* lpSerializeItem_In, size_t& nBufferLength_InOut);

        inline void MoveItem(ItemPos itemPos);

        inline unsigned __int64 GetUID(void) const			{ return m_ItemData.m_dwUID;						}
        inline ItemPos          GetPos(void) const			{ return m_ItemData.m_ItemPos;						}
        inline ItemPos          GetRealPos(void) const		{ return m_itemPos_Real;							}
		inline void				SwapPos(void)				{ std::swap(m_ItemData.m_ItemPos, m_itemPos_Real);	}

		inline unsigned short   GetPrototypeID(void) const							{ return m_ItemData.m_usProtoTypeID;				}
        inline unsigned char    GetNumOrDurability(void) const						{ return m_ItemData.m_cNumOrDurability;				}
        inline unsigned char    GetMaxNumOrDurability(void) const					{ return m_cMaxNumOrDurability;						}
        inline void             SetNumOrDurability(unsigned char cNumOrDurability)	{ m_ItemData.m_cNumOrDurability = cNumOrDurability; }      
        inline void             SetMaxNumOrDurability(unsigned char cNumOrDurability){ m_cMaxNumOrDurability = cNumOrDurability;		}
		inline void				SetStallPrice(unsigned long dwStallPrice)			{ m_dwStallPrice = dwStallPrice;					}

        inline unsigned long    GetStallPrice(void) const		{ return m_dwStallPrice;										}
		inline unsigned long	GetBuyBlackPrice(void) const	{ return m_ItemInfo.m_DetailData.m_dwBlackPrice;				}	
		inline unsigned long	GetBuyMedalPrice(void) const	{ return m_ItemInfo.m_DetailData.m_dwMedalPrice;				}	
		
		virtual unsigned long	GetBuyPrice(void) const;			
        virtual unsigned long	GetSellPrice(void) const;	
        virtual unsigned long	GetRepairPrice(void) const;		
		virtual unsigned long	GetUpgradePrice(void) const;
		virtual unsigned long	GetGraftPrice(void) const;

		inline const char*		GetItemTypeName(void) const		{ return m_ItemInfo.m_StringData.m_szTypeName;		}

        inline bool IsSet(unsigned long dwDetailDataFlag) const { return dwDetailDataFlag == (m_ItemInfo.m_DetailData.m_dwFlags & dwDetailDataFlag); }

		inline bool		GetEnableRepair(void);
		inline bool 	GetEnableStack(void);
		inline bool 	GetSocketItem(void);
		inline bool		GetRuneItem(void);
		inline bool 	GetTwohandItem(void);
		inline bool 	GetEnableEquip(void);
                
		inline void				SetItemPos(unsigned short wItemPos)		{ m_ItemData.m_ItemPos.m_cPos = wItemPos;	}
		inline unsigned short	GetItemPos(void)						{ return m_ItemData.m_ItemPos.m_cPos;		}

        inline void		SetPos(unsigned char cXIndex, unsigned char cYIndex, unsigned char cZIndex) { m_ItemData.m_ItemPos.SetPos(cXIndex, cYIndex, cZIndex); }

		inline void		SetXIndex(unsigned char cXIndex)		{ m_ItemData.m_ItemPos.SetXIndex(cXIndex);			}
		inline void		SetYIndex(unsigned char cYIndex)		{ m_ItemData.m_ItemPos.SetYIndex(cYIndex);			}
		inline void		SetZIndex(unsigned char cZIndex)		{ m_ItemData.m_ItemPos.SetZIndex(cZIndex);			}

		inline void		SetPos(unsigned short wIndex)			{ m_ItemData.m_ItemPos.SetPos(wIndex);				}
	
        // SetUID는 아무때나 함부로 사용하지 마시오!
        inline void     SetUID(unsigned __int64 dwItemUID) { m_ItemData.m_dwUID = dwItemUID; }

		// S 그레이드 제한값 보정(S 그레이드 이외에는 쓰지 마세요. 공식이 정확하지 않습니다. - by hackermz)
		virtual void	RevisionLimit() { }
		virtual void	RevisionLimit(short* lpattrLimit) { }
		virtual void	RevisionLimit(Item::Attribute::Type eAttributeType, short& attrLImit) { }


	protected:

        virtual ~CItem();
        
        const ItemInfo& m_ItemInfo;             // 스크립트로부터 로드한 아이템 데이터
        ItemData        m_ItemData;             // DB에 저장되는 아이템 데이터

        ItemPos         m_itemPos_Real;         // 임시 공간이 아닌 아이템 위치
        unsigned char   m_cMaxNumOrDurability;  // 최대 내구도
		unsigned long	m_dwStallPrice;			// 노점상에서의 가격
        unsigned long   m_dwPrice;              // 아이템 가격 (옵션에 의해 갱신되지만 내구도를 고려하지는 않은 값)

        friend class CItemFactory;
    };


    class CEquipment : public CItem
    {
    public:

        enum EQUIPMENT_ERROR
        {
            // 소켓 박기
            S_SUCCESS                       = 0,    // 성공
	        E_SERVER_ERROR                  = 1,    // 서버 에러
	        E_GET_EQUIP_FAILED              = 2,    // 장비 얻기 실패
	        E_GET_GEM_FAILED                = 3,    // 젬 얻기 실패
	        E_NOT_EQUIP                     = 4,    // 업그레이드 가능한 아이템 아님
	        E_NOT_GEM                       = 5,    // 젬 아이템 아님
	        E_REMOVE_GEM_FAILED             = 6,    // 젬 제거 실패
	        E_SOCKET_IS_FULL                = 7,    // 소켓 슬롯 초과
	        E_OVER_MAX_SOCKET               = 8,    // 최대 소켓 초과
			E_SOCKET_NOT_MAX				= 9,	// 소켓 개수가 7개가 아니다.
			E_SOCKET_NOT_INST				= 10,	// 소켓에 인스톨된 보석이 없다.

			// 룬 설치/제거
			R_SUCCESS						= 0,	// 룬 설치/제거 성공.
			E_GET_RUNE_FAILED				= 9,	// 룬 개수가 없을때.
			E_NOT_RUNE						= 10,	// 룬이 없을때.
			E_NOT_RUNE_SLOT					= 11,	// 룬에 빈 소켓이 없을때.
			E_NOT_RUNE_LEVEL_LIMIT			= 12,	// 룬에 레벨제한때 문제에 설치할 수 없을때.
			E_USED_RUNE_SKILL_POINT			= 13,	// 룬에 추가 스킬 포인트를 사용하고 있어서 제거 할 수 없을때.
			E_NOT_RUNE_SCRIPT_INFO			= 14,	// 룬에 스크립트 정보를 얻어올수 없을때.

            // 강화 
            E_GET_MINERAL_FAILED            = 3,    // 광물 얻기 실패
            E_NOT_MINERAL                   = 5,    // 광물이 아님
            E_NOT_MATCH_MINERAL_TYPE        = 6,    // 광물 타입 틀림
            E_NOT_ENOUGH_MINERAL            = 7,    // 광물 부족
            E_REMOVE_EQUIP_FAILED           = 8,    // 장비 제거 실패
            E_CREATE_SCRAP_FAILED           = 9,    // 고철 생성 실패
            E_SET_SCRAP_FAILED              = 10,   // 고철 놓기 실패
            E_REMOVE_MINERAL_FAILED         = 11,   // 광물 제거 실패
            E_UPGRADE_LEVEL_IS_FULL         = 12,   // 더 이상 업그레이드를 할 수 없음         
            E_NOT_ENOUGH_MONEY              = 13,   // 돈이 부족해서 업그레이드 할 수 없음
            S_BROKEN_EQUIP                  = 14,	// 성공했으나 무기 부서짐 (클라이언트로 보내지는 않는다.)

			E_NOT_UPGRADE_LEVEL				= 15,	// 옵션 이식 가능한 제련 업그레이드 레벨이 아님.
			
			E_NOT_UPGRADE_AS_OVER			= 16,	// 고대의 돌 이용 회수 초과.
			E_NOT_UPGRADE_ENDURANCE			= 17,	// 내구도 회복 아이템 사용 불가.
			E_NOT_UPGRADE_MINENDURANCE		= 18,	// 제련가능한 내구도 10이 되지 않습니다.
			E_NOT_UPGRADE_AWAKEN			= 19,	// 이미 각성되었다. (각성의 코어 사용)

			// 옵션 이식 
			E_WRONG_ATTRIBUTE				= 2,	// 이식 불가능한 옵션
			E_NOT_EQUAL_KIND				= 3,	// 다른 종류의 장비
			E_HIGH_GRADE_ORIGINAL			= 4,	// 원본의 그레이드가 높음
			E_GRAFE_FAILD					= 5,	// 옵션이식 실패

			// 보상 판매
			E_WRONG_TYPE					= 2,	// 보상 판매 대상이 아님 
			E_NEW_SEASON_EQUIP				= 3,	// 새로 드랍됬거나 이미 보상된 장비
			E_LOW_STATUS_EQUIP				= 4		// 너무 낮은 수준의 장비
        };

		enum PASSIVE_TYPE
		{
			PASSIVE_ALWAYS				=	0,		// 무기에 상관없이 항상 발생하는 패시브
			PASSIVE_THIS_WEAPON			=	1,		// 해당 무기에 발생하는 패시브
			NONE_PASSIVE_THIS_WEAPON	=	2		// 해당 무기에 발생하지 않는 패시브
		};

		enum RuneApplyType
		{
			RUNE_NONE	=	0,
			RUNE_INDEX	=	(1 << 0),				// 하나에 속성.
			RUNE_ALL	=	(1 << 1),				// 모든 속성.
			RUNE_APPLY  =	(1 << 2),				// 속성을 추가.
			RUNE_REMOVE =	(1 << 3)				// 속성을 삭제.
		};

        // 아이템을 장비로 Downcast한다.
        inline static CEquipment* DowncastToEquipment(CItem* lpItem);

        // Desc : 아이템 정보를 char버퍼로 기록한다.
        // In   : 버퍼, 버퍼 크기
        // Out  : (Return)성공 여부, nBufferLength_InOut - 기록한 크기
        virtual bool SerializeOut(char* lpSerializeItem_Out, size_t& nBufferLength_InOut);

        // Desc : char버퍼로부터 아이템 정보를 읽어 온다.
        // In   : 버퍼, 버퍼 크기
        // Out  : (Return)성공 여부, nBufferLength_InOut - 사용한 버퍼 크기
        virtual bool SerializeIn(const char* lpSerializeItem_In, size_t& nBufferLength_InOut);

        // 아이템 속성 추가 (속성 변경 후에는 반드시 가격/그레이드 계산을 다시 해줍시다.)
        inline void AddAttribute(Item::Attribute::Type eAttributeType, short nAttributeValue);
		inline void SetAttribute(Item::Attribute::Type eAttributeType, short nAttributeValue);

		// 3차 밸런스 패치(소켓, 업그레이드 값 제거)0
		inline void RemoveUpgradeAttribute(Item::Attribute::Type eAttributeType);
		inline void RemoveGemAttribute(Item::Attribute::Type eAttributeType);

        // 아이템 속성을 얻어 온다.
        inline short GetAttribute(Item::Attribute::Type eAttributeType);
        inline void GetAttribute(short* lpAttributeArray_Out, unsigned short nArraySize);

		// 옵션별 한계값에 도달했는지 체크한다.
		bool CheckAttributeLimit(Item::Attribute::Type eAttributeType, unsigned char cType = 0);

		inline bool AddSocket(unsigned char cSocket);
		inline void GetSocket(unsigned char* cSockets, unsigned char cArraySize);
		unsigned char GetSocket(unsigned char cIndex)		{ return m_cSocket[cIndex];	}

		const Grade::GradeInfo& GetItemGrade(void) const	{ return m_GradeInfo;		}

		inline void InitializeAttribute(void);         // 아이템 정보를 상점값으로 초기화
		BOOL AddRandomOption(Item::EquipType::Grade eGrade, unsigned char cBaseNum, int iMagicChance);

		// 소켓설정
		BOOL AddRandomSocket();

		// 옵션 이식
		EQUIPMENT_ERROR OptionGraft(CEquipment* lpSacrifice, bool bUpgradeLevelLimit, unsigned char cAttributeType, 
			unsigned long dwCurrentGold, unsigned long& dwUsedGold);

		bool CanOptionGraft(Item::CItemType::ArrayType eItemType, Item::Attribute::Type eAttributeType);

		// 보상 판매
		EQUIPMENT_ERROR Compensation(unsigned char cClass, unsigned char cLevel, 
			CharacterStatus status, CEquipment** lppResultEquip, long& lCompensationGold);

		unsigned char  	GetUpgradeLevel(void)     const { return m_cUpgradeLevel; 	}
		unsigned char  	GetCoreLevel(void)     const { return m_cCoreLevel; 	}
		
		unsigned char  	GetCurrentSocketNum(void) const { return m_cSocketNum;    	}
		unsigned char  	GetMaxSocketNum(void)     const { return m_cMaxSocket;    	}

		// 리밸런싱 이후 생성되는 장비는 이 함수를 호출해준다.
		void			SetNewEquip(char cStrong = 1)		
		{  
			if(m_cSeasonRecord != cStrong)
				m_cCoreLevel = 0;

			m_cSeasonRecord = cStrong;	// 1 기본, 2 행운, 3 각성
		}
		unsigned char	GetSeasonRecord(void)			{ return m_cSeasonRecord;	}

		unsigned long	GetBuyPrice(void) const;			
		unsigned long	GetSellPrice(void) const;	
		unsigned long	GetRepairPrice(void) const;		
		unsigned long	GetUpgradePrice(void) const;
		unsigned long	GetGraftPrice(void) const;

		unsigned char	GetUpgradeCnt(void)							{ return m_cUpgradeCnt;			}
		void			SetUpgradeCnt(unsigned char cUpgradeCnt)	{ m_cUpgradeCnt = cUpgradeCnt;	}

        // 업그레이드 가능 여부 리턴
        bool IsUpgradable(void) const;

		// 장착아이템에 룬 소켓 개수 체크
		inline unsigned char	GetMaxRuneSocket();

		// 룬을 아이템에 장착 가능한지 체크
		inline unsigned char	GetRuneEquipable();

		// 장착 아이템에 장착제한에 레벨을 얻어온다.		
		inline unsigned short	GetRuneEquipLimitLevel(void) const;

		// 룬 아이템을 제거(속성도 같이 제거)
		inline bool				SubRuneAttribute(void);

		// 설치 되어 있는 룬 소켓값을 넘겨준다.
		inline unsigned short	GetRuneSocket(unsigned char cIndex) 
		{ 
			if (cIndex >= EquipmentInfo::MAX_RUNE_SOCKET_NUM) 
			{
				return 0; 
			}
			
			return m_usRuneSocket[cIndex]; 
		}

		// 룬 속성을 삭제한 아이템 속성 복사본을 넘겨준다.
		inline void				GetSubRuneAttribute(short* psAttribute);
		inline void				GetSubRuneAttribute(Item::Attribute::Type eAttributeType, short& sAttribute);

		// 외부에서 사용할 룬 속성 삭제/추가
		inline void				SetRuneAttribute(RuneApplyType enRuneApplyType, unsigned char ucIndex);

		// 장착 아이템에 있는 모든 룬 아이템에 속성중 스킬포인트 리턴해준다.
		unsigned short			GetAllRuneSkillPointCheck();										

		// 룬 아이템이 장착 되어 있으면 스킬 포인트 체크
		bool					GetRuneCheckSkill(CCharacter* lpCharacter, Item::ItemPos itemPos, bool bChk = true);

		// ------------------------------------------------------------------------------------------------------------
		// ConvertTool에서만 사용되는 함수들 - RebalanceOverItem.cpp 에서 사용
		inline void				Repair();
		inline void				InitEquip();
		inline void				SetEquip();

		// ------------------------------------------------------------------------------------------------------------
		// 서버에서만 사용되는 함수들 - ItemServerFunc.cpp 에서 별도 구현

		EQUIPMENT_ERROR InstallSocket(CItem& Gem);
        EQUIPMENT_ERROR UpgradeItem(CItem& Mineral_InOut, unsigned long dwCurrentGold_In, unsigned long& dwUsedGold_Out);

		EQUIPMENT_ERROR InstallRuneSocket(CItem& Rune, bool bEquip, unsigned char cLevel);									// 장착 아이템에 룬 소켓 설치.

		EQUIPMENT_ERROR UnInstallRuneSocket(CItem& Rune, bool bEquip, unsigned char cLevel,	unsigned short usSkillPoint);	// 장착 아이템에 룬 소켓 제거.

		// ------------------------------------------------------------------------------------------------------------
		// 3차 밸런스 패치 관련 함수

		// S 그레이드 제한값 구하기
		bool			GetLimitValue(short* lpattrLimit);
		bool			GetLimitValue(Item::Attribute::Type eAttributeType, short& attrLimit);

		// S 그레이드 제한값 보정(S 그레이드 이외에는 쓰지 마세요. 공식이 정확하지 않습니다. - by hackermz)
		void			RevisionLimit();
		void			RevisionLimit(short* lpattrLimit);
		void			RevisionLimit(Item::Attribute::Type eAttributeType, short& attrLImit);

		// 그레이드 책정
		void			CalculateItemGrade(void);					
		
												
    protected:

        enum ApplyType
        {
            APPLY   = 1,    // 값 바꾸지 말 것
            REMOVE  = -1    // 값 바꾸지 말 것
        };

        CEquipment(const ItemInfo& itemInfo);        
        virtual ~CEquipment();

        inline void InitializeGemAttribute(void);      // 소켓 정보로 소켓 속성을 초기화
        inline void InitializeUpgradeAttribute(void);  // 업그레이드 단계로 업그레이드 속성을 초기화

        inline void ApplyGemAttribute(ApplyType eApplyType);        // 소켓 속성을 적용
        inline void ApplyUpgradeAttribute(ApplyType eApplyType);    // 업그레이드 속성을 적용
		inline void ApplyRuneAttribute(ApplyType eApplyType);		// 룬 아이템 속성을 적용

		void CalculateItemPrice(void);					// 가격 책정
	       
        unsigned char       m_cSocket[EquipmentInfo::MAX_SOCKET_NUM];                   // 아이템 소켓 정보
        ItemAttribute       m_SocketAttribute[EquipmentInfo::MAX_SOCKET_ATTRIBUTE_NUM]; // 아이템 소켓으로 박은 속성
        ItemAttribute       m_UpgradeAttribute[EquipmentInfo::MAX_UPGRADE_ATTRIBUTE];   // 아이템 업그레이드시 붙는 속성
		unsigned short		m_usRuneSocket[EquipmentInfo::MAX_RUNE_SOCKET_NUM];			// 룬 소켓

		short				m_wAttribute[Attribute::MAX_ATTRIBUTE_NUM];		// 최종적으로 계산된 아이템 속성 (스크립트 + DB + ...)	

		unsigned char		m_cUpgradeLevel;	// 업그레이드 단계
		unsigned char       m_cSocketNum;       // 현재 소켓 개수
        unsigned char       m_cMaxSocket;       // 최대 소켓 개수      
        unsigned char       m_cMaxAttribute;    // 최대 속성 개수

		unsigned char		m_cSeasonRecord;	// 아이템 생성 시기

		Grade::GradeInfo	m_GradeInfo;		// 그레이드 정보

		unsigned char		m_cUpgradeCnt;
		unsigned char		m_cCoreLevel;

        friend class CItemFactory;
		friend class CItemType;
    };

    class CUseItem : public CItem
    {
    public:

        inline static CUseItem* DowncastToUseItem(CItem* lpItem);
	
		// ------------------------------------------------------------------------------------------------------------
		// 서버에서만 사용되는 함수들 - ItemServerFunc.cpp 에서 별도 구현

		bool Use(CCharacter* lpSender, CCharacter* lpRecver, unsigned short& wError);

    protected:

        CUseItem(const ItemInfo& itemInfo);
        virtual ~CUseItem();
        
        bool UsePotion(CCharacter* lpSender, CCharacter* lpRecver);

        friend class CItemFactory;
    };
};


inline void Item::CItem::MoveItem(ItemPos itemPos) 
{
	if (TakeType::TS_STALL == itemPos.m_cPos)
	{
        m_itemPos_Real = itemPos;
	}
	else
	{
		m_ItemData.m_ItemPos = itemPos;
		if (TakeType::TS_TEMP != itemPos.m_cPos)
		{
			m_itemPos_Real = itemPos;
		}
	}
}

inline Item::CEquipment* Item::CEquipment::DowncastToEquipment(Item::CItem* lpItem)
{
    if (NULL == lpItem) { return NULL; }
    return lpItem->IsSet(DetailData::EQUIP) ?
        static_cast<CEquipment*>(lpItem) : NULL;
}

inline void Item::CEquipment::AddAttribute(Item::Attribute::Type eAttributeType, short nAttributeValue)
{ 
/*
	// 룬아이템에 속성 추가
	if (eAttributeType == Attribute::RUNE && 0 != nAttributeValue)
	{
		unsigned short usRuneID = nAttributeValue + EtcItemID::RUNE_START_ID;

		const ItemInfo* lpRuneInfo = CItemMgr::GetInstance().GetItemInfo(usRuneID);
		if (NULL != lpRuneInfo)
		{
			for (unsigned char cIndex = 0; cIndex < EquipmentInfo::MAX_RUNE_SOCKET_NUM; ++cIndex)
			{
				if (0 != m_usRuneSocket[cIndex])
				{
					m_usRuneSocket[cIndex] = usRuneID;

					for (int nIndex = 0; nIndex < Item::Attribute::MAX_ATTRIBUTE_NUM; ++nIndex)
					{
						if (Item::EquipType::S_GRADE != GetItemGrade().m_aryAttributeGrade[nIndex])
						{
							m_wAttribute[nIndex] += lpRuneInfo->m_EquipAttribute.m_usAttributeValue[nIndex];
						}
					}

					return;
				}
			}
		}
	}
	else
*/	{
		if (Item::EquipType::S_GRADE != GetItemGrade().m_aryAttributeGrade[eAttributeType])
		{
			m_wAttribute[eAttributeType] += nAttributeValue; 
		}

/*
		// edith 2009.03.15 min 데미지가 max보다 높아도 옵션이식 성공 추가작업된것 
		if(eAttributeType == Attribute::MIN_DAMAGE)
		{
			if(m_wAttribute[Attribute::MIN_DAMAGE] > m_wAttribute[Attribute::MAX_DAMAGE])
			{
				// 두값 교환
				short t = m_wAttribute[Attribute::MIN_DAMAGE];
				m_wAttribute[Attribute::MIN_DAMAGE] = m_wAttribute[Attribute::MAX_DAMAGE];
				m_wAttribute[Attribute::MAX_DAMAGE] = t;
			}
		}
//*/
	}
}

inline void Item::CEquipment::SetAttribute(Item::Attribute::Type eAttributeType, short nAttributeValue)
{ 
/*	// 룬아이템에 속성 추가
	if (eAttributeType == Attribute::RUNE && 0 != nAttributeValue)
	{
		unsigned short usRuneID = nAttributeValue + EtcItemID::RUNE_START_ID;

		const ItemInfo* lpRuneInfo = CItemMgr::GetInstance().GetItemInfo(usRuneID);
		if (NULL != lpRuneInfo)
		{
			for (unsigned char cIndex = 0; cIndex < EquipmentInfo::MAX_RUNE_SOCKET_NUM; ++cIndex)
			{
				if (0 != m_usRuneSocket[cIndex])
				{
					m_usRuneSocket[cIndex] = usRuneID;

					for (int nIndex = 0; nIndex < Item::Attribute::MAX_ATTRIBUTE_NUM; ++nIndex)
					{
						m_wAttribute[nIndex] = lpRuneInfo->m_EquipAttribute.m_usAttributeValue[nIndex];
					}

					return;
				}
			}
		}
	}
	else
*/	{
		m_wAttribute[eAttributeType] = nAttributeValue; 
	}
}

inline void Item::CEquipment::RemoveUpgradeAttribute(Item::Attribute::Type eAttributeType)
{
	ItemAttribute* first  = m_UpgradeAttribute;
	ItemAttribute* last   = m_UpgradeAttribute + EquipmentInfo::MAX_UPGRADE_ATTRIBUTE;

	for (;first != last; ++first) 
	{
		if(eAttributeType == static_cast<Item::Attribute::Type>(first->m_cType))
		{
			first->m_usValue = 0;
		}
	}
}

inline void Item::CEquipment::RemoveGemAttribute(Item::Attribute::Type eAttributeType)
{
	ItemAttribute* first  = m_SocketAttribute;
	ItemAttribute* last   = m_SocketAttribute + EquipmentInfo::MAX_UPGRADE_ATTRIBUTE;

	for (;first != last; ++first) 
	{
		if(eAttributeType == static_cast<Item::Attribute::Type>(first->m_cType))
		{
			first->m_usValue = 0;
		}
	}
}

inline bool Item::CEquipment::SubRuneAttribute(void)
{
	// 룬 아이템 제거
	for (unsigned char cRune = 0; cRune <= EquipmentInfo::MAX_RUNE_SOCKET_NUM; ++cRune)
	{
		if (m_usRuneSocket[cRune])
		{
			unsigned short usItemId = m_usRuneSocket[cRune];

			const ItemInfo* itemInfo = CItemMgr::GetInstance().GetItemInfo(usItemId);

			if (itemInfo)
			{
				for (unsigned char cIndex = 0; cIndex < Item::Attribute::MAX_ATTRIBUTE_NUM; ++cIndex)
				{
					m_wAttribute[cIndex] -= itemInfo->m_EquipAttribute.m_usAttributeValue[cIndex];
				}
			}

			m_usRuneSocket[cRune] = 0;	
		}		
	}

	return true;
}


inline short Item::CEquipment::GetAttribute(Item::Attribute::Type eAttributeType) 
{ 
	return m_wAttribute[eAttributeType];
}

inline void Item::CEquipment::GetAttribute(short* lpAttributeArray_Out, unsigned short nArraySize) 
{
    unsigned short nCopyNum = nArraySize < Item::Attribute::MAX_ATTRIBUTE_NUM
        ? nArraySize : Item::Attribute::MAX_ATTRIBUTE_NUM;

	for (int nIndex = 0; nIndex < nCopyNum; ++nIndex)
	{
		*(lpAttributeArray_Out + nIndex) = GetAttribute(static_cast<Item::Attribute::Type>(nIndex));
	}
}

inline bool Item::CEquipment::AddSocket(unsigned char cSocket)
{
    if (m_cSocketNum < m_cMaxSocket && 0 == m_cSocket[m_cSocketNum])
    {
        m_cSocket[m_cSocketNum] = cSocket;
        ++m_cSocketNum;

		return true;
    }

    return false;
}

inline void Item::CEquipment::GetSocket(unsigned char* cSockets, unsigned char cArraySize)
{
    unsigned short nCopyNum = cArraySize < Item::EquipmentInfo::MAX_SOCKET_NUM
        ? cArraySize : Item::EquipmentInfo::MAX_SOCKET_NUM;
    unsigned char* lpSocketPastEnd = m_cSocket + nCopyNum;
    std::copy(m_cSocket, lpSocketPastEnd , cSockets);
}

inline void Item::CEquipment::InitializeAttribute(void)
{
    for (unsigned int nIndex = 0; nIndex < Item::Attribute::MAX_ATTRIBUTE_NUM; ++nIndex)
    {
		m_wAttribute[nIndex] = m_ItemInfo.m_EquipAttribute.m_usAttributeValue[nIndex];
    }    
}

inline void Item::CEquipment::InitializeGemAttribute(void)
{
    std::fill_n(m_SocketAttribute, int(EquipmentInfo::MAX_SOCKET_ATTRIBUTE_NUM), ItemAttribute());

    // 설치된 소켓을 가지고 속성과 값을 설정
    CItemType::SetInstallGemAttribute(this);
}

inline void Item::CEquipment::InitializeUpgradeAttribute(void)
{
    std::fill_n(m_UpgradeAttribute, int(EquipmentInfo::MAX_UPGRADE_ATTRIBUTE), ItemAttribute());

    // 아이템 업그레이드 단계값을 가지고 속성과 값을 설정
	CItemType::SetUpgradeItemAttribute(this);
}

inline void Item::CEquipment::ApplyGemAttribute(ApplyType eApplyType)
{   
    ItemAttribute* first  = m_SocketAttribute;
    ItemAttribute* last   = m_SocketAttribute + EquipmentInfo::MAX_SOCKET_ATTRIBUTE_NUM;

    for (;first != last; ++first) 
	{ 
		AddAttribute(static_cast<Item::Attribute::Type>(first->m_cType), first->m_usValue * eApplyType);
	}
}

inline unsigned char Item::CEquipment::GetMaxRuneSocket()
{
	// 장착아이템에 룬 소켓 개수 체크
	return Item::CItemMgr::GetInstance().GetMaxRuneSocket(static_cast<ItemType::Type>(GetItemInfo().m_DetailData.m_cItemType));
}

inline unsigned char Item::CEquipment::GetRuneEquipable()
{
	// 장착아이템에 룬 소켓 빈공간 체크
	unsigned char cRuneSocketCnt = Item::CItemMgr::GetInstance().GetMaxRuneSocket(static_cast<ItemType::Type>(GetItemInfo().m_DetailData.m_cItemType));

	if (!cRuneSocketCnt)
	{
		return 0;
	}

	if (!m_usRuneSocket[0])
	{
		return 1;
	}

	if (!m_usRuneSocket[1] && cRuneSocketCnt == EquipmentInfo::MAX_RUNE_SOCKET_NUM)
	{
		return 2;
	}

	return 0;
}

inline void Item::CEquipment::ApplyUpgradeAttribute(ApplyType eApplyType)
{
    ItemAttribute* first  = m_UpgradeAttribute;
    ItemAttribute* last   = m_UpgradeAttribute + EquipmentInfo::MAX_UPGRADE_ATTRIBUTE;

	for (;first != last; ++first) 
	{
		AddAttribute(static_cast<Item::Attribute::Type>(first->m_cType), first->m_usValue * eApplyType);
	}
}

inline void Item::CEquipment::ApplyRuneAttribute(ApplyType eApplyType)
{
	// 룬아이템에 속성 추가/삭제
	for (unsigned char cIndex = 0; cIndex < EquipmentInfo::MAX_RUNE_SOCKET_NUM; ++cIndex)
	{
		if (m_usRuneSocket[cIndex])
		{
			unsigned short usItemId = m_usRuneSocket[cIndex];

			const ItemInfo* itemInfo = CItemMgr::GetInstance().GetItemInfo(usItemId);

			if (itemInfo)
			{
				for (unsigned char cAttribute = 0; cAttribute < Item::Attribute::MAX_ATTRIBUTE_NUM; ++cAttribute)
				{
					m_wAttribute[cAttribute] += itemInfo->m_EquipAttribute.m_usAttributeValue[cAttribute]*eApplyType;
				}
			}			
		}		
	}
}

inline void Item::CEquipment::GetSubRuneAttribute(short* psAttribute)
{
	// 룬 속성을 삭제한 아이템 속성 복사본을 넘겨준다.
	memcpy(psAttribute, m_wAttribute, sizeof(m_wAttribute));

	for (unsigned char cIndex = 0; cIndex < EquipmentInfo::MAX_RUNE_SOCKET_NUM; ++cIndex)
	{
		if (m_usRuneSocket[cIndex])
		{
			const ItemInfo* itemInfo = CItemMgr::GetInstance().GetItemInfo(m_usRuneSocket[cIndex]);

			if (itemInfo)
			{
				for (unsigned char cAttribute = 0; cAttribute < Item::Attribute::MAX_ATTRIBUTE_NUM; ++cAttribute)
				{
					psAttribute[cAttribute] -= itemInfo->m_EquipAttribute.m_usAttributeValue[cAttribute];
				}
			}			
		}		
	}
}

inline void Item::CEquipment::GetSubRuneAttribute(Item::Attribute::Type eAttributeType, short& sAttribute)
{
	sAttribute = m_wAttribute[eAttributeType];

	for (unsigned char cIndex = 0; cIndex < EquipmentInfo::MAX_RUNE_SOCKET_NUM; ++cIndex)
	{
		if (m_usRuneSocket[cIndex])
		{
			const ItemInfo* itemInfo = CItemMgr::GetInstance().GetItemInfo(m_usRuneSocket[cIndex]);

			if (itemInfo)
			{
				sAttribute -= itemInfo->m_EquipAttribute.m_usAttributeValue[eAttributeType];
			}			
		}		
	}
}

inline void Item::CEquipment::SetRuneAttribute(RuneApplyType enRuneApplyType, unsigned char ucIndex)
{
	// 외부에서 사용할 룬 속성 삭제/추가.

	if (enRuneApplyType&RuneApplyType::RUNE_INDEX && enRuneApplyType&RuneApplyType::RUNE_REMOVE)
	{		
		for (unsigned char ucRuneIndex = 0; ucRuneIndex < EquipmentInfo::MAX_RUNE_SOCKET_NUM; ucRuneIndex++)
		{
			if (m_usRuneSocket[ucRuneIndex])
			{
				const ItemInfo* itemInfo = CItemMgr::GetInstance().GetItemInfo(m_usRuneSocket[ucRuneIndex]);

				if (itemInfo)
				{				
					m_wAttribute[ucIndex] -= itemInfo->m_EquipAttribute.m_usAttributeValue[ucIndex];
				}
			}
		}		
	}
	else if (enRuneApplyType&RuneApplyType::RUNE_ALL && enRuneApplyType&RuneApplyType::RUNE_REMOVE)
	{ 
		ApplyRuneAttribute(REMOVE);
	}
	else if (enRuneApplyType&RuneApplyType::RUNE_INDEX && enRuneApplyType&RuneApplyType::RUNE_APPLY)
	{
		for (unsigned char ucRuneIndex = 0; ucRuneIndex < EquipmentInfo::MAX_RUNE_SOCKET_NUM; ucRuneIndex++)
		{
			if (m_usRuneSocket[ucRuneIndex])
			{
				const ItemInfo* itemInfo = CItemMgr::GetInstance().GetItemInfo(m_usRuneSocket[ucRuneIndex]);

				if (itemInfo)
				{				
					m_wAttribute[ucIndex] += itemInfo->m_EquipAttribute.m_usAttributeValue[ucIndex];
				}
			}
		}
	}
	else if (enRuneApplyType&RuneApplyType::RUNE_ALL && enRuneApplyType&RuneApplyType::RUNE_APPLY)
	{ 
		ApplyRuneAttribute(APPLY);
	}
}

inline unsigned short Item::CEquipment::GetRuneEquipLimitLevel(void) const
{
	// 장착 아이템에 장착제한에 레벨을 얻어온다.
	unsigned short usResult	= 0;

	for (unsigned char cIndex = 0; cIndex < EquipmentInfo::MAX_RUNE_SOCKET_NUM; ++cIndex)
	{
		if (m_usRuneSocket[cIndex])
		{
			unsigned short usItemId = m_usRuneSocket[cIndex];

			const ItemInfo* itemInfo = CItemMgr::GetInstance().GetItemInfo(usItemId);

			if (itemInfo)
			{
				if (itemInfo->m_UseLimit.m_cLimitStatus==Item::StatusLimit::LEVEL)
				{
					if (usResult<itemInfo->m_UseLimit.m_wLimitValue)
					{
						usResult = itemInfo->m_UseLimit.m_wLimitValue;
					}
				}				
			}
		}
	}

	return usResult;	
}

inline Item::CUseItem* Item::CUseItem::DowncastToUseItem(CItem* lpItem)
{
    if (NULL == lpItem) { return NULL; }
    return lpItem->IsSet(DetailData::USE_ITEM) ? 
        static_cast<CUseItem*>(lpItem) : NULL;
}

inline bool Item::CItem::GetEnableRepair(void)
{
	unsigned long dwFlags = GetItemInfo().m_DetailData.m_dwFlags;
	return ((dwFlags & Item::DetailData::EQUIP) && !(dwFlags & Item::DetailData::STACKABLE));
}

inline bool Item::CItem::GetEnableStack(void)
{
	return (GetItemInfo().m_DetailData.m_dwFlags & Item::DetailData::STACKABLE) ? true : false;
}

inline bool Item::CItem::GetEnableEquip(void)
{
	return (GetItemInfo().m_DetailData.m_dwFlags & Item::DetailData::EQUIP);
}

inline bool Item::CItem::GetSocketItem(void)
{
	return (GetItemInfo().m_DetailData.m_cItemType == Item::ItemType::GEM);
}

inline bool	Item::CItem::GetRuneItem(void)
{
	return (GetItemInfo().m_DetailData.m_cItemType == Item::ItemType::RUNE);
}

inline bool Item::CItem::GetTwohandItem(void)
{
	unsigned char cItemType = GetItemInfo().m_DetailData.m_cItemType;
	return (cItemType == Item::ItemType::BOW || 
		cItemType == Item::ItemType::CROSSBOW ||
		cItemType == Item::ItemType::STAFF || 
		cItemType == Item::ItemType::TWOHANDED_AXE ||
		cItemType == Item::ItemType::TWOHANDED_BLUNT || 
		cItemType == Item::ItemType::TWOHANDED_SWORD ||
		cItemType == Item::ItemType::OPP_SYTHE);
}

// ------------------------------------------------------------------------------------------------------------
// ConvertTool에서만 사용되는 함수들 - RebalanceOverItem.cpp 에서 사용
inline void Item::CEquipment::Repair()
{
	SetNumOrDurability(GetMaxNumOrDurability());
	CalculateItemPrice();
}

inline void Item::CEquipment::InitEquip()
{
	// 옵션 이식을 하기전에 룬 추가 옵션을 삭제한다.
	RuneApplyType	enRuneApplyType	= static_cast<RuneApplyType>(RUNE_ALL | RUNE_REMOVE);

	SetRuneAttribute(enRuneApplyType, 0);

	// 제련 단계는 초기화된다. (옵션은 남는다.)
	m_cUpgradeLevel = 0;
	m_cCoreLevel = 0;
	InitializeUpgradeAttribute();   

	// 소켓도 초기화된다. (보석은 모두 제거되지만 옵션은 남는다.)
	m_cSocketNum = 0;
	std::fill_n(m_cSocket, unsigned char(Item::EquipmentInfo::MAX_SOCKET_NUM), 0);
	InitializeGemAttribute();
	m_cMaxSocket = m_ItemInfo.m_DetailData.m_cMaxSocketNum;
}

inline void Item::CEquipment::SetEquip()
{
	// 룬 속성을 다시 추가한다.
	RuneApplyType enRuneApplyType = static_cast<RuneApplyType>(RUNE_ALL | RUNE_APPLY);

	SetRuneAttribute(enRuneApplyType, 0);
}

#endif

#include "stdafx.h"

// 클라이언트에서도 쓰이므로 include를 명시한다.
#include <algorithm>
#include <Log/ServerLog.h>
#include <Utility/Math/Math.h>
#include <Creature/Character/CharacterClass.h>

#include <Item/ItemMgr.h>
#include <Item/ItemFactory.h>

#ifdef _RYL_GAME_CLIENT_
#include "RYLNetworkData.h"
#endif

#ifndef _RYL_GAME_CLIENT_
#include <Utility/Setup/ServerSetup.h>
#endif

#include "Item.h"
#include "GMMemory.h"

using namespace Item;

static ItemInfo NullProtoType;

CItem::CItem()
:   m_ItemInfo(NullProtoType)
{
	m_ItemData.m_dwUID = 0;
	m_ItemData.m_usProtoTypeID = 0;

	m_ItemData.m_ItemPos.m_cPos = 0;
	m_ItemData.m_ItemPos.m_cIndex = 0;

	m_ItemData.m_cItemSize = sizeof(ItemData);
	m_ItemData.m_cNumOrDurability = 0;
	m_cMaxNumOrDurability = 0;
	m_dwStallPrice  = 0;
	m_dwPrice       = 0;
}

CItem::CItem(const ItemInfo& itemInfo)
:   m_ItemInfo(itemInfo)
{
	m_ItemData.m_dwUID = 0;
	m_ItemData.m_usProtoTypeID = itemInfo.m_usProtoTypeID;

	m_ItemData.m_ItemPos.m_cPos = 0;
	m_ItemData.m_ItemPos.m_cIndex = 0;

	m_ItemData.m_cItemSize = sizeof(ItemData);
	m_ItemData.m_cNumOrDurability = m_ItemInfo.m_DetailData.m_cDefaultDurabilityOrStack;
	m_cMaxNumOrDurability = m_ItemInfo.m_DetailData.m_cMaxDurabilityOrStack;
	m_dwStallPrice  = 0;
	m_dwPrice       = m_ItemInfo.m_DetailData.m_dwPrice;
}

CItem::~CItem()
{

}


bool CItem::SerializeOut(char* lpSerializeItem_Out, size_t& nBufferLength_InOut)
{
	if (nBufferLength_InOut >= sizeof(ItemData))
	{
		m_ItemData.m_cItemSize = sizeof(ItemData);
		nBufferLength_InOut = sizeof(ItemData);

		*reinterpret_cast<ItemData*>(lpSerializeItem_Out) = m_ItemData;
		return true;
	}

	nBufferLength_InOut = 0;
	return false;
}

bool CItem::SerializeIn(const char* lpSerializeItem_In, size_t& nBufferLength_InOut)
{
	const ItemData& itemData = *reinterpret_cast<const ItemData*>(lpSerializeItem_In);

	if (sizeof(ItemData) <= nBufferLength_InOut)
	{        
		nBufferLength_InOut = itemData.m_cItemSize;

		if (itemData.m_usProtoTypeID == m_ItemInfo.m_usProtoTypeID)
		{
			m_ItemData = itemData;
			m_itemPos_Real = m_ItemData.m_ItemPos;
			return true;
		}
	}

	ERRLOG4(g_Log, "SerializeIn에 실패했습니다. 아이템을 제거합니다. ItemDataSize:%d, nBufferLength_InOut:%d, "
		"itemData.m_usProtoTypeID:%d, m_ItemInfo.m_usProtoTypeID:%d", 
		sizeof(ItemData), sizeof(nBufferLength_InOut), itemData.m_usProtoTypeID, m_ItemInfo.m_usProtoTypeID);

	return false;
}

unsigned long CItem::GetBuyPrice(void) const
{ 
	// edith 판매가격
	if (0 != m_dwStallPrice) 
	{ 
		return m_dwStallPrice; 
	}

	if (true == m_ItemInfo.m_DetailData.m_bOptionPrice)
	{
		// 장비의 구매, 판매, 수리, 제련 가격의 경우 옵션과 내구도에 따라 계산된 가격을 소숫점 이하 올림합니다.
		return static_cast<unsigned long>((m_dwPrice * 
			(100 - (m_cMaxNumOrDurability - m_ItemData.m_cNumOrDurability) * 0.25f) / 100.0f) + 1);
	}

	return m_dwPrice;
}

unsigned long CItem::GetSellPrice(void) const
{	
	if (0 != m_dwStallPrice) 
	{ 
		return m_dwStallPrice; 
	}

	if (Item::ItemType::GEM_SELL == static_cast<Item::ItemType::Type>(m_ItemInfo.m_DetailData.m_cItemType))
	{
		return static_cast<unsigned long>(m_dwPrice);
	}

	unsigned long Price = static_cast<unsigned long>(m_dwPrice / 3.0f);

	if (true == m_ItemInfo.m_DetailData.m_bOptionPrice)
	{
		Price = static_cast<unsigned long>((m_dwPrice * (100 - (m_cMaxNumOrDurability - m_ItemData.m_cNumOrDurability) * 0.25f) / 100.0f) / 3);
	}

	if(Price == 0)
		Price = 1;

	return Price;
/*
	if (true == m_ItemInfo.m_DetailData.m_bOptionPrice)
	{
		return static_cast<unsigned long>(((m_dwPrice * 
			(100 - (m_cMaxNumOrDurability - m_ItemData.m_cNumOrDurability) * 0.25f) / 100.0f) / 3) + 1);
	}

	return static_cast<unsigned long>(m_dwPrice / 3.0f);
*/
}

unsigned long CItem::GetRepairPrice(void) const
{
	return static_cast<unsigned long>((m_dwPrice * 
		((m_cMaxNumOrDurability - m_ItemData.m_cNumOrDurability) * 0.25f) / 200.0f) + 1);
}

unsigned long CItem::GetUpgradePrice(void) const
{
	return static_cast<unsigned long>((m_dwPrice * 0.3f) + 1);
//	return static_cast<unsigned long>(((m_dwPrice * 
//		(100 - (m_cMaxNumOrDurability - m_ItemData.m_cNumOrDurability) * 0.25f) / 100.0f) / 2) + 1);
}

unsigned long CItem::GetGraftPrice(void) const
{
//	return static_cast<unsigned long>((m_dwPrice * 0.3f) + 1);
	return static_cast<unsigned long>((m_dwPrice * (100 * 0.25f) / 100.0f) + 1);
}


CEquipment::CEquipment(const ItemInfo& itemInfo)
:   CItem(itemInfo), m_cUpgradeLevel(0), m_cSocketNum(0), m_cSeasonRecord(0), m_cCoreLevel(0)
{
	if (1 == m_ItemInfo.m_DetailData.m_cXSize && 1 == m_ItemInfo.m_DetailData.m_cYSize)
	{
		m_cMaxSocket    = EquipmentInfo::MAX_MINSIZE_SOCKET_NUM;
		m_cMaxAttribute = EquipmentInfo::MAX_MINSIZE_ATTRIBUTE_NUM;
	}
	else
	{
		m_cMaxSocket    = EquipmentInfo::MAX_SOCKET_NUM;
		m_cMaxAttribute = EquipmentInfo::MAX_ATTRIBUTE_NUM;
	}

	m_cMaxSocket = (itemInfo.m_DetailData.m_cMaxSocketNum < m_cMaxSocket) ? 
		itemInfo.m_DetailData.m_cMaxSocketNum : m_cMaxSocket;

	m_ItemData.m_cItemSize += sizeof(EquipmentInfo);

	std::fill_n(m_cSocket, int(EquipmentInfo::MAX_SOCKET_NUM), 0);

	std::fill_n(m_usRuneSocket, int(EquipmentInfo::MAX_RUNE_SOCKET_NUM), 0);

	InitializeAttribute();    
}

CEquipment::~CEquipment()
{
}

bool CEquipment::SerializeOut(char* lpSerializeItem_Out, size_t& nBufferLength_InOut)
{
	// edith 2009.09.16 소켓 개수 수정
	const size_t nMaxItemSize = sizeof(ItemData) + sizeof(EquipmentInfo) + 
		EquipmentInfo::MAX_SOCKET_NUM * sizeof(unsigned char) + 
		Item::EquipmentInfo::MAX_ATTRIBUTE_NUM * sizeof(ItemAttribute);

	float fGradeInfo = 0;
	const CItemType::ArrayType eEquipType = CItemType::GetEquipType(m_ItemInfo.m_DetailData.m_dwFlags);

	if (nMaxItemSize <= nBufferLength_InOut)
	{
		unsigned int nIndex = 0, nSocketIndex = 0, nAttributeNum = 0;

		// 기본 정보 복사
		ItemData& itemData = *reinterpret_cast<ItemData*>(lpSerializeItem_Out);
		itemData = m_ItemData;

		EquipmentInfo&  equipmentInfo   = *reinterpret_cast<EquipmentInfo*>(lpSerializeItem_Out + sizeof(ItemData));
		char*           lpSocketInfo    = lpSerializeItem_Out + sizeof(ItemData) + sizeof(EquipmentInfo);

		// 아이템 소켓 정보 복사
		for(nIndex = 0, nSocketIndex = 0; 
			nIndex < EquipmentInfo::MAX_SOCKET_NUM && nSocketIndex < m_cMaxSocket; ++nIndex)
		{
			if (0 != m_cSocket[nIndex])
			{
				lpSocketInfo[nSocketIndex] = m_cSocket[nIndex];
				++nSocketIndex;
			}
		}

		// 속성값에서 소켓, 업그레이드에 의한 값을 제거
		ApplyGemAttribute(REMOVE);
		ApplyUpgradeAttribute(REMOVE);		
		ApplyRuneAttribute(REMOVE);

		// 아이템 속성 정보 복사
		ItemAttribute* lpAttribute = reinterpret_cast<ItemAttribute*>(
			lpSocketInfo + (nSocketIndex * sizeof(unsigned char)));

		for(nIndex = 0, nAttributeNum = 0;
			nIndex < Item::Attribute::MAX_DB_ATTRIBUTE_NUM && nAttributeNum < m_cMaxAttribute; ++nIndex)
		{
			// 현재 속성값 - 상점값(스크립트)
			short usDiffAttribute = m_wAttribute[nIndex] - m_ItemInfo.m_EquipAttribute.m_usAttributeValue[nIndex];

			// edith 2008.07.16 사용하지 않는 능력치는 제한하기
			//fGradeInfo = Grade::GetGradeValue(eEquipType, Grade::VALUE_GRADE, EquipType::S_GRADE, static_cast<Attribute::Type>(nIndex));
			//if(fGradeInfo == 0.0f)
			//	usDiffAttribute = 0;

			if (0 != usDiffAttribute)
			{
				lpAttribute->m_cType    = nIndex;
				lpAttribute->m_usValue  = usDiffAttribute;
				++nAttributeNum;
				++lpAttribute;
			}
		}
/*
		for(unsigned char cIndex = 0; cIndex < 2; cIndex++)
		{
			unsigned short usRune = GetRuneSocket(cIndex);

			if(usRune)
			{
				lpAttribute->m_cType    = Attribute::RUNE;
				lpAttribute->m_usValue  = usRune-EtcItemID::RUNE_START_ID;
				++nAttributeNum;
				++lpAttribute;
			}
		}
*/
		// 속성값에서 소켓, 업그레이드에 의한 값을 추가
		ApplyGemAttribute(APPLY);
		ApplyUpgradeAttribute(APPLY);
		ApplyRuneAttribute(APPLY);

		// 개수 및 크기 정보 복사
		m_ItemData.m_cItemSize = itemData.m_cItemSize = sizeof(ItemData) + sizeof(EquipmentInfo) + 
			nSocketIndex * sizeof(unsigned char) + nAttributeNum * sizeof(ItemAttribute);

		equipmentInfo.m_cReserved1			= 0;
		equipmentInfo.m_cUpgradeLevel		= m_cUpgradeLevel;
		equipmentInfo.m_cDiffMaxSocket      = m_cMaxSocket - m_ItemInfo.m_DetailData.m_cMaxSocketNum;
		equipmentInfo.m_cSocketNum          = nSocketIndex;      
		equipmentInfo.m_cAttributeNum       = nAttributeNum;
		equipmentInfo.m_cDiffMaxDurability  = m_cMaxNumOrDurability - m_ItemInfo.m_DetailData.m_cMaxDurabilityOrStack;
		equipmentInfo.m_cSeasonRecord		= m_cSeasonRecord;
		equipmentInfo.m_cReserved2			= 0;
		equipmentInfo.m_cUpgradeCnt			= m_cUpgradeCnt;
		equipmentInfo.m_cCoreLevel			= m_cCoreLevel;

		nBufferLength_InOut = m_ItemData.m_cItemSize;
		return true;
	}

	nBufferLength_InOut = 0;
	return false;
}

bool CEquipment::SerializeIn(const char* lpSerializeItem_In, size_t& nBufferLength_InOut)
{    
	if (true == CItem::SerializeIn(lpSerializeItem_In, nBufferLength_InOut))
	{
		const EquipmentInfo& equipmentInfo = 
			*reinterpret_cast<const EquipmentInfo*>(lpSerializeItem_In + sizeof(ItemData));

		m_cUpgradeLevel         = equipmentInfo.m_cUpgradeLevel;
		m_cMaxSocket            = m_ItemInfo.m_DetailData.m_cMaxSocketNum + equipmentInfo.m_cDiffMaxSocket;         
		m_cSocketNum            = equipmentInfo.m_cSocketNum;
		m_cMaxNumOrDurability   = m_ItemInfo.m_DetailData.m_cMaxDurabilityOrStack + equipmentInfo.m_cDiffMaxDurability;
		m_cSeasonRecord			= equipmentInfo.m_cSeasonRecord;
		m_cUpgradeCnt			= equipmentInfo.m_cUpgradeCnt;
		m_cCoreLevel			= equipmentInfo.m_cCoreLevel;

		const unsigned char* lpSocketIndex   = reinterpret_cast<const unsigned char*>(&equipmentInfo + 1);
		const unsigned char* lpSocketPastEnd = lpSocketIndex + m_cSocketNum;
		std::copy(lpSocketIndex, lpSocketPastEnd, m_cSocket);

		InitializeAttribute();

		float fGradeInfo = 0;
		const CItemType::ArrayType eEquipType = CItemType::GetEquipType(m_ItemInfo.m_DetailData.m_dwFlags);

		const ItemAttribute* first = reinterpret_cast<const ItemAttribute*>(lpSocketPastEnd);
		const ItemAttribute* last  = first + equipmentInfo.m_cAttributeNum;

		for (; first != last; ++first) 
		{ 
			// edith 2008.07.16 사용하지 않는 능력치는 제한하기
			//fGradeInfo = Grade::GetGradeValue(eEquipType, Grade::VALUE_GRADE, EquipType::S_GRADE, static_cast<Attribute::Type>(first->m_cType));

			//if(fGradeInfo > 0.0f)
				AddAttribute(static_cast<Attribute::Type>(first->m_cType), first->m_usValue);
		}

		InitializeGemAttribute();
		InitializeUpgradeAttribute();

		ApplyGemAttribute(APPLY);
		ApplyUpgradeAttribute(APPLY);

		CalculateItemGrade();
		CalculateItemPrice();

		// 3차 밸런스 패치 (S그레이드 아이템 제한선 보정)
		RevisionLimit();

		return true;
	}

	return false;
}

BOOL CEquipment::AddRandomSocket()
{
	int iRandom = Math::Random::ComplexRandom(1000);

	int iRnd[3] = { 395, 600, 5 };

	for(int i = 0; i < 3; ++i)
	{
		if(iRandom <= iRnd[i])
		{
			m_cMaxSocket = i+1;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CEquipment::AddRandomOption(EquipType::Grade eGrade, unsigned char cBaseNum, int iMagicChance)
{
	if (0 == m_ItemInfo.m_DetailData.m_wDropOption)	{ return FALSE; }

	// 매직찬스
	if(iMagicChance > 0)
	{
		if(eGrade == EquipType::F_GRADE)
		{
			if((int)Math::Random::ComplexRandom(10000) < (int)iMagicChance*100)
			{
				eGrade = EquipType::D_GRADE;
				iMagicChance = 1;
			}
		}
		else
		{            
			int iPerNum = cBaseNum;

			int iNum = iMagicChance/100;
			
			if(iNum > 0)
				cBaseNum += iNum;

			iNum = iMagicChance-(iNum*100);
			
			if((int)Math::Random::ComplexRandom(10000) < (int)iNum*100)
				++cBaseNum;

			if(iPerNum == cBaseNum)
				iMagicChance = 0;
			else 
				iMagicChance = 1;

			// 베이스는 +5까지 있기 때문에 +5 이상 올라가지 않게 5개로 제한한다.
			if(cBaseNum >= 5)
				cBaseNum = 5;
		}
	}
	else
		iMagicChance = 0;

	unsigned char cRequiredNum = cBaseNum;

	unsigned char cIndex = 0;

	// 베이스로 적용될 속성을 랜덤으로 정하기 위해 속성 인덱스를 섞어둔다.
	unsigned char aryAttributeIndex[Attribute::MAX_ATTRIBUTE_NUM] = { 0, };
	for (; cIndex < Attribute::MAX_ATTRIBUTE_NUM; ++cIndex)
	{
		aryAttributeIndex[cIndex] = cIndex;
	}
	std::random_shuffle(aryAttributeIndex, aryAttributeIndex + Attribute::MAX_ATTRIBUTE_NUM);

	const Item::CItemType::ArrayType eEquipType = CItemType::GetEquipType(m_ItemInfo.m_DetailData.m_dwFlags);

	// 각종 속성에 붙을 옵션을 계산
	for (cIndex = 0; cIndex < Attribute::MAX_ATTRIBUTE_NUM; ++cIndex)
	{
		EquipType::Grade eUseGrade = eGrade;
		if (eEquipType == CItemType::DAGGER_TYPE || 
			eEquipType == CItemType::ONEHANDED_TYPE || 	
			eEquipType == CItemType::LONGRANGE_TYPE || 		
			eEquipType == CItemType::TWOHANDED_TYPE || 	
			eEquipType == CItemType::STAFF_TYPE)
		{
			if (aryAttributeIndex[cIndex] == Attribute::BLOCK)
			{
				// 무기에 붙는 블록 옵션은 C 그레이드를 능가할 수 없다.
				eUseGrade = max(eGrade, EquipType::C_GRADE);
			}
		}

		const short wMaxValue = static_cast<short>(m_ItemInfo.m_DetailData.m_wDropOption * 
			Grade::GetGradeValue(eEquipType, Grade::DROP_MAX_VALUE, eUseGrade, static_cast<Attribute::Type>(aryAttributeIndex[cIndex])));

		if (0 != wMaxValue)
		{
			short wMinValue = 0;

			if (cBaseNum > 0)
			{
				// 베이스로 결정된 속성은 하한값을 가치그레이드에서 가져옴으로서 무조건 원하는 그레이드가 되게끔 한다.
				wMinValue = static_cast<short>(ceil(m_ItemInfo.m_DetailData.m_wDropOption * 
					Grade::GetGradeValue(eEquipType, Grade::VALUE_GRADE, eUseGrade, static_cast<Attribute::Type>(aryAttributeIndex[cIndex]))));
				--cBaseNum;
			}
			else
			{
				wMinValue = static_cast<short>(ceil(m_ItemInfo.m_DetailData.m_wDropOption * 
					Grade::GetGradeValue(eEquipType, Grade::DROP_MIN_VALUE, eUseGrade, static_cast<Attribute::Type>(aryAttributeIndex[cIndex]))));
			}

			const unsigned short wRange = wMaxValue - wMinValue + 1;

			short wPlusOption = static_cast<short>(Math::Random::ComplexRandom(wRange)) + wMinValue;
			wPlusOption = max(short(0), wPlusOption);

			AddAttribute(static_cast<Attribute::Type>(aryAttributeIndex[cIndex]), wPlusOption);
		}
	}

	// (최대 데미지의 기본 옵션 + 추가 옵션)은 (최소 데미지의 기본 옵션 + 추가 옵션)보다 작을 수 없다.
	m_wAttribute[Attribute::MAX_DAMAGE] = max(m_wAttribute[Attribute::MIN_DAMAGE], m_wAttribute[Attribute::MAX_DAMAGE]);

	// 최대 내구도 계산
	const short wAddMaxDurability = static_cast<short>(Math::Random::ComplexRandom(101) - 50);
	if (wAddMaxDurability < 0 && m_cMaxNumOrDurability < abs(wAddMaxDurability))
	{
		m_cMaxNumOrDurability = 1;
	}
	else
	{
		m_cMaxNumOrDurability = m_ItemInfo.m_DetailData.m_cMaxDurabilityOrStack + wAddMaxDurability;
	}

	// 현재 내구도 계산 (1 ~ 최대 내구도)
	m_ItemData.m_cNumOrDurability = static_cast<unsigned char>(Math::Random::ComplexRandom(m_cMaxNumOrDurability) + 1);

	// 그레이드/가격 재계산
	CalculateItemGrade();
	CalculateItemPrice();

	// 3차 밸런스 패치 (S그레이드 아이템 제한선 보정)
	RevisionLimit();

	if (m_GradeInfo.m_eItemGrade != eGrade)
	{
		ERRLOG4(g_Log, "아이템에 옵션을 붙이는데 문제가 있습니다. 요청그레이드:%d(%d), 생성그레이드:%d(%d)", 
			eGrade, cRequiredNum, m_GradeInfo.m_eItemGrade, m_GradeInfo.m_cPlus);
	}


	if(iMagicChance == 0)
		return TRUE;

	return FALSE;
}

CEquipment::EQUIPMENT_ERROR CEquipment::OptionGraft(CEquipment* lpSacrifice, bool bUpgradeLevelLimit, unsigned char cAttributeType, 
													unsigned long dwCurrentGold, unsigned long& dwUsedGold)
{
	// 옵션 이식을 하기전에 룬 추가 옵션을 삭제한다.
	Item::CEquipment::RuneApplyType	enRuneApplyType	= static_cast<Item::CEquipment::RuneApplyType>(Item::CEquipment::RUNE_ALL | Item::CEquipment::RUNE_REMOVE);

	SetRuneAttribute(enRuneApplyType, 0);
	lpSacrifice->SetRuneAttribute(enRuneApplyType, 0);

	if (false == CanOptionGraft(
		static_cast<CItemType::ArrayType>(CItemType::GetEquipType(m_ItemInfo.m_DetailData.m_dwFlags)), 
		static_cast<Attribute::Type>(cAttributeType)))
	{
		ERRLOG0(g_Log, "옵션 이식 에러 : 옵션 이식이 불가능한 옵션입니다.");
		return EQUIPMENT_ERROR::E_WRONG_ATTRIBUTE;
	}

	if (m_ItemInfo.m_usProtoTypeID != lpSacrifice->GetPrototypeID())
	{
		ERRLOG0(g_Log, "옵션 이식 에러 : 같은 종류의 아이템이 아닙니다.");
		return EQUIPMENT_ERROR::E_NOT_EQUAL_KIND;
	}

	if (m_GradeInfo.m_eItemGrade < lpSacrifice->GetItemGrade().m_eItemGrade)
	{
		ERRLOG0(g_Log, "옵션 이식 에러 : 원본 아이템의 그레이드가 더 높습니다.");
		return EQUIPMENT_ERROR::E_HIGH_GRADE_ORIGINAL;
	}

	// edith 옵션이식에 의한 가격 구하기
	dwUsedGold = GetGraftPrice();
	if (dwCurrentGold < dwUsedGold)
	{
		ERRLOG0(g_Log, "옵션 이식 에러 : 돈이 부족합니다.");
		return EQUIPMENT_ERROR::E_NOT_ENOUGH_MONEY;
	}
/*
	// edith 옵션이식 실패 로직 추가. 서버에서만 체크해야한다.
	if( 30 < Math::Random::ComplexRandom(100)) 
	{
		// 옵션이식 실패 
		// 딱히 에러메시지를 출력하지 않는다.
		return EQUIPMENT_ERROR::E_GRAFE_FAILD;
	}
*/
	if (true == bUpgradeLevelLimit)
	{
		if (m_cUpgradeLevel != 0 && m_cUpgradeLevel != 15)
		{
			ERRLOG0(g_Log, "옵션 이식 에러 : 제련 업그레이드 레벨이 0 이거나 10 인것만 가능(원본)");
			return EQUIPMENT_ERROR::E_NOT_UPGRADE_LEVEL;
		}

		if (lpSacrifice->m_cUpgradeLevel != 0 && lpSacrifice->m_cUpgradeLevel != 15)
		{
			ERRLOG0(g_Log, "옵션 이식 에러 : 제련 업그레이드 레벨이 0 이거나 10 인것만 가능(제뮬)");
			return EQUIPMENT_ERROR::E_NOT_UPGRADE_LEVEL;
		}
	}

	// 아이템 옵션 이식
	const Item::Attribute::Type eAttributeType = static_cast<Item::Attribute::Type>(cAttributeType);
	const short nAttributeValue = 
		((m_wAttribute[eAttributeType] + lpSacrifice->GetAttribute(eAttributeType) + 1) / 2) - m_wAttribute[eAttributeType];

	// edith 2009.03.15 min 데미지가 max보다 높아도 옵션이식 성공
	// 최소 데미지일때 맥스 데미지보다 값이 커지게 되면 
	// 옵션이식에 실패해야한다.
	if(eAttributeType == Attribute::MIN_DAMAGE)
	{
		// edith 2008.11.14 옵션이식 MIN_DEMAGE 버그수정.
		// 최소 데미지가 맥스 데미지보다 크면 
		// 이식 불가능한 옵션이라고 메시지를 띄운다. 후에 메시지를 세분화하자.
		if(m_wAttribute[Attribute::MIN_DAMAGE]+nAttributeValue > m_wAttribute[Item::Attribute::MAX_DAMAGE])
		{
			ERRLOG0(g_Log, "옵션 이식 실패 : MinDemage 가 MaxDemage 보다 높습니다. 옵션이식에 실패하였습니다.");
			return EQUIPMENT_ERROR::E_GRAFE_FAILD;
		}
	}

	AddAttribute(eAttributeType, nAttributeValue);

	// edith 옵션이식시 제련단계 초기화를 하지 않는다.
	// 제련 단계는 초기화된다. (옵션은 남는다.)
//	m_cUpgradeLevel = 0;
//	InitializeUpgradeAttribute();   


	// edith 옵션이식시 소켓을 초기화 하면 안된다. 
	// 제련 레벨이 남아있으니 소켓을 초기화하면 안됨.
	// 소켓도 초기화된다. (보석은 모두 제거되지만 옵션은 남는다.)
//	m_cSocketNum = 0;
//	std::fill_n(m_cSocket, unsigned char(Item::EquipmentInfo::MAX_SOCKET_NUM), 0);
//	m_cMaxSocket = m_ItemInfo.m_DetailData.m_cMaxSocketNum;

	InitializeGemAttribute();

	// 룬 속성을 다시 추가한다.
	enRuneApplyType	= static_cast<Item::CEquipment::RuneApplyType>(Item::CEquipment::RUNE_ALL | Item::CEquipment::RUNE_APPLY);

	SetRuneAttribute(enRuneApplyType, 0);
	lpSacrifice->SetRuneAttribute(enRuneApplyType, 0);

	// 그레이드/가격 재계산
	CalculateItemGrade();
	CalculateItemPrice();

	// 3차 밸런스 패치 (S그레이드 아이템 제한선 보정)
	RevisionLimit();

	// edith 새로운 장비로 설정.
	SetNewEquip();

	return EQUIPMENT_ERROR::S_SUCCESS;
}

//bool Item::CEquipment::CheckAttributeLimit(Item::Attribute::Type eAttributeType, short nAttributeValue, unsigned char cType)
//{
//	const CItemType::ArrayType eEquipType = CItemType::GetEquipType(m_ItemInfo.m_DetailData.m_dwFlags);
//
//	const float fGradeGap = 
//		Grade::GetGradeValue(eEquipType, Grade::VALUE_GRADE, EquipType::AAA_GRADE, eAttributeType) - 
//		Grade::GetGradeValue(eEquipType, Grade::VALUE_GRADE, EquipType::AA_GRADE, eAttributeType);
//
//	unsigned short wLimitMagnification = 4;	
//
//	if(!cType)
//	{
//		switch (eAttributeType)
//		{
//		case Attribute::MIN_DAMAGE:		wLimitMagnification = 35;	break;
//		case Attribute::MAX_DAMAGE:		wLimitMagnification = 35;	break;
//		case Attribute::ARMOR:			wLimitMagnification = 10;	break;
//		case Attribute::HIT_RATE:		wLimitMagnification = 10;	break;
//		case Attribute::EVADE:			wLimitMagnification = 10;	break;
//		case Attribute::MAX_HP:			wLimitMagnification = 20;	break;
//		case Attribute::HP_REGEN:		wLimitMagnification = 10;	break;
//		case Attribute::MAX_MP:			wLimitMagnification = 10;	break;
//		case Attribute::MP_REGEN:		wLimitMagnification = 10;	break;
//		case Attribute::CRITICAL:		wLimitMagnification = 15;	break;
//		case Attribute::BLOCK:			wLimitMagnification = 10;	break;
//		case Attribute::MAGIC_POWER:	wLimitMagnification = 10;	break;
//		case Attribute::MAGIC_RESIST:	wLimitMagnification = 10;	break;
//
//		default:						wLimitMagnification = 10;	break;
//		}
//
//		// 버클러의 경우 예외 처리 필요
//		if (Item::EtcItemID::BUCKLER == GetPrototypeID())
//
//
//		{
//			switch (eAttributeType)
//			{
//			case Attribute::MAX_HP:			wLimitMagnification = 330;	break;
//			case Attribute::HP_REGEN:		wLimitMagnification = 33;	break;
//			case Attribute::MAX_MP:			wLimitMagnification = 33;	break;
//			case Attribute::MP_REGEN:		wLimitMagnification = 33;	break;
//			case Attribute::BLOCK:			wLimitMagnification = 50;	break;
//			}
//		}
//	}
//
//	// 옵션별 제한 가치 그레이드 값
//	const float fLimitGradeValue = 
//		Grade::GetGradeValue(eEquipType, Grade::VALUE_GRADE, EquipType::AAA_GRADE, eAttributeType) + (fGradeGap * wLimitMagnification);
//
//	// 옵션별 제한 값
//	const float fLimitAttributeValue = m_ItemInfo.m_DetailData.m_wDropOption * fLimitGradeValue;
//
//	// 변경 후 옵션 값
//	const unsigned short wCurrnetAttributeValue = 
//		m_wAttribute[eAttributeType] + nAttributeValue - m_ItemInfo.m_EquipAttribute.m_usAttributeValue[eAttributeType];
//
//	if (0 != wCurrnetAttributeValue && wCurrnetAttributeValue >= fLimitAttributeValue)
//	{
//		// 제한에 걸리면 제련 성공률이 반으로 줄어든다.
//		return true;
//	}
//
//	return false;
//}

bool Item::CEquipment::CheckAttributeLimit(Item::Attribute::Type eAttributeType, unsigned char cType)
{
	short sattrLimit = 0, sattrFactor = 0;
	GetLimitValue(eAttributeType, sattrLimit);
	GetSubRuneAttribute(eAttributeType, sattrFactor);

	return (0 != sattrFactor && sattrFactor >= sattrLimit);		
}

bool CEquipment::CanOptionGraft(CItemType::ArrayType eItemType, Attribute::Type eAttributeType)
{
	switch (eItemType)
	{
	case CItemType::ARMOUR_TYPE:		

		// edith 2008.01.14 옵션이식 제한 제거.
	case CItemType::HELM_TYPE:		
		if (Attribute::ARMOR == eAttributeType || 
			Attribute::EVADE == eAttributeType || 
			Attribute::MAX_HP == eAttributeType || 
			Attribute::HP_REGEN == eAttributeType ||
//			Attribute::MP_REGEN == eAttributeType ||
//			Attribute::MAX_MP == eAttributeType || 
			Attribute::MAGIC_RESIST == eAttributeType)
		{
			return true;
		}
		break;

	case CItemType::DAGGER_TYPE:		
		if (Attribute::MIN_DAMAGE == eAttributeType || 
			Attribute::MAX_DAMAGE == eAttributeType || 
			Attribute::HIT_RATE == eAttributeType ||
			Attribute::MAX_MP == eAttributeType ||
			Attribute::MP_REGEN == eAttributeType || 
			Attribute::BLOCK == eAttributeType || 
			Attribute::CRITICAL == eAttributeType)
		{
			return true;
		}
		break;

	case CItemType::ONEHANDED_TYPE:	
		if (Attribute::MIN_DAMAGE == eAttributeType || 
			Attribute::MAX_DAMAGE == eAttributeType || 
			Attribute::HIT_RATE == eAttributeType ||
			Attribute::MAX_MP == eAttributeType ||
			Attribute::MP_REGEN == eAttributeType || 
			Attribute::BLOCK == eAttributeType || 
			Attribute::CRITICAL == eAttributeType)
		{
			return true;
		}
		break;

	case CItemType::LONGRANGE_TYPE:
	case CItemType::TWOHANDED_TYPE:	
		if (Attribute::MIN_DAMAGE == eAttributeType || 
			Attribute::MAX_DAMAGE == eAttributeType || 
			Attribute::HIT_RATE == eAttributeType ||
			Attribute::MAX_MP == eAttributeType ||
			Attribute::MP_REGEN == eAttributeType || 
			Attribute::BLOCK == eAttributeType || 
			Attribute::CRITICAL == eAttributeType)
		{
			return true;
		}
		break;

	case CItemType::STAFF_TYPE:		
		if (Attribute::MAGIC_POWER == eAttributeType ||
			Attribute::MIN_DAMAGE == eAttributeType ||
			Attribute::MAX_DAMAGE == eAttributeType || 
			Attribute::HIT_RATE == eAttributeType || 
			Attribute::MAX_MP == eAttributeType || 
			Attribute::MP_REGEN == eAttributeType || 
			Attribute::CRITICAL == eAttributeType)
		{
			return true;
		}
		break;

	case CItemType::SHIELD_TYPE:		
		if (Attribute::MAX_HP == eAttributeType || 
			Attribute::HP_REGEN == eAttributeType || 
			Attribute::MAX_MP == eAttributeType || 
			Attribute::MP_REGEN == eAttributeType || 
			Attribute::BLOCK == eAttributeType)
		{
			return true;
		}
		break;
	}

	return false;
}

CEquipment::EQUIPMENT_ERROR CEquipment::Compensation(unsigned char cClass, unsigned char cLevel, CharacterStatus status, 
													 CEquipment** lppResultEquip, long& lCompensationGold)
{
	if (0 != m_cSeasonRecord)
	{
		// 새롭게 드랍된 아이템이거나 이미 보상 판매를 받은 아이템입니다. 
		return CEquipment::E_NEW_SEASON_EQUIP;
	}

	if (0 == m_ItemInfo.m_DetailData.m_wDropOption)
	{
		// 보상 판매 대상 종류의 아이템이 아닙니다.
		return CEquipment::E_WRONG_TYPE;
	}

	unsigned char cCompensationType = 0;
	short wNeedStatusValue = 0;
	StatusLimit::Type eNeedStatusType = StatusLimit::NONE;

	// 특수 케이스
	switch (m_ItemInfo.m_DetailData.m_cItemType)
	{
	case ItemType::SHIELD:
		{
			if (CClass::Defender != cClass && CClass::Cleric != cClass)
			{
				cCompensationType = Compensation::CASE1;
				eNeedStatusType = StatusLimit::CON;
			}
			break;
		}

	case ItemType::CON_ARMOUR:
		{
			if (CClass::Defender == cClass || CClass::Warrior == cClass || CClass::Fighter == cClass)
			{
				cCompensationType = Compensation::CASE2;
				eNeedStatusType = StatusLimit::CON;
				break;
			}

			if (CClass::Priest == cClass || CClass::Cleric == cClass || CClass::Acolyte == cClass)
			{
				cCompensationType = Compensation::CASE3;
				eNeedStatusType = StatusLimit::CON;
				break;
			}
			break;
		}

	case ItemType::DEX_ARMOUR:
		{
			if (CClass::Sorcerer == cClass || CClass::Enchanter == cClass || CClass::Mage == cClass)
			{
				cCompensationType = Compensation::CASE4;
				eNeedStatusType = StatusLimit::DEX;
				break;
			}

			if (CClass::Assassin == cClass || CClass::Archer == cClass || CClass::Rogue == cClass)
			{
				cCompensationType = Compensation::CASE6;
				eNeedStatusType = StatusLimit::DEX;
				break;
			}
			break;
		}

	case ItemType::DEX_HELM:
		{
			if (CClass::Sorcerer == cClass || CClass::Enchanter == cClass || CClass::Mage == cClass)
			{
				cCompensationType = Compensation::CASE5;
				eNeedStatusType = StatusLimit::DEX;
				break;
			}

			if (CClass::Assassin == cClass || CClass::Archer == cClass || CClass::Rogue == cClass)
			{
				cCompensationType = Compensation::CASE7;
				eNeedStatusType = StatusLimit::DEX;
				break;
			}
			break;
		}

	case ItemType::DEX_HEAD:
		{
			if (CClass::RuneOff == cClass || CClass::LifeOff == cClass)
			{
				cCompensationType = Compensation::CASE8;
				eNeedStatusType = StatusLimit::DEX;
				break;
			}

			if (CClass::Officiator == cClass || CClass::ShadowOff == cClass)
			{
				cCompensationType = Compensation::CASE9;
				eNeedStatusType = StatusLimit::DEX;
				break;
			}
			break;
		}

	case ItemType::ONEHANDED_SWORD:
	case ItemType::ONEHANDED_AXE:
		{
			if (status.m_nSTR >= status.m_nCON)
			{
				cCompensationType = ItemType::ONEHANDED_SWORD;
				eNeedStatusType = StatusLimit::STR;
			}
			else
			{
				cCompensationType = ItemType::ONEHANDED_AXE;
				eNeedStatusType = StatusLimit::CON;
			}
			break;
		}

	case ItemType::TWOHANDED_SWORD:
	case ItemType::TWOHANDED_AXE:
		{
			if (status.m_nSTR >= status.m_nCON)
			{
				cCompensationType = ItemType::TWOHANDED_SWORD;
				eNeedStatusType = StatusLimit::STR;
			}
			else
			{
				cCompensationType = ItemType::TWOHANDED_AXE;
				eNeedStatusType = StatusLimit::CON;
			}
			break;
		}

	case ItemType::ONEHANDED_BLUNT:
		{
			if (status.m_nCON >= status.m_nWIS)
			{
				cCompensationType = ItemType::ONEHANDED_AXE;
				eNeedStatusType = StatusLimit::CON;
			}
			else
			{
				cCompensationType = ItemType::ONEHANDED_BLUNT;
				eNeedStatusType = StatusLimit::WIS;
			}
			break;
		}

	case ItemType::TWOHANDED_BLUNT:
		{
			if (status.m_nCON >= status.m_nWIS)
			{
				cCompensationType = ItemType::TWOHANDED_AXE;
				eNeedStatusType = StatusLimit::CON;
			}
			else
			{
				cCompensationType = ItemType::TWOHANDED_BLUNT;
				eNeedStatusType = StatusLimit::WIS;
			}
			break;
		}

	case ItemType::COM_SWORD:
	case ItemType::COM_BLUNT:
		{
			if (status.m_nSTR >= status.m_nCON)
			{
				cCompensationType = ItemType::COM_SWORD;
				eNeedStatusType = StatusLimit::STR;
			}
			else
			{
				cCompensationType = ItemType::COM_BLUNT;
				eNeedStatusType = StatusLimit::CON;
			}
			break;
		}

	case ItemType::OPP_SLUSHER:
	case ItemType::OPP_AXE:
		{
			if (status.m_nSTR >= status.m_nCON)
			{
				cCompensationType = ItemType::OPP_SLUSHER;
				eNeedStatusType = StatusLimit::STR;
			}
			else
			{
				cCompensationType = ItemType::OPP_AXE;
				eNeedStatusType = StatusLimit::CON;
			}
			break;
		}
	}

	if (0 == cCompensationType)
	{
		cCompensationType = m_ItemInfo.m_DetailData.m_cItemType;
		eNeedStatusType = static_cast<StatusLimit::Type>(m_ItemInfo.m_UseLimit.m_cLimitStatus);
	}

	if (m_ItemInfo.m_UseLimit.m_wLimitValue <= 
		ClassTable[cClass].GetMinState(static_cast<CClass::StatusType>(eNeedStatusType), cLevel) / 2)
	{
		// 캐릭터의 스테이터스에 비해 낮은 제한의 장비입니다.
		return CEquipment::E_LOW_STATUS_EQUIP;
	}

	switch (eNeedStatusType)
	{
	case StatusLimit::STR:		wNeedStatusValue = status.m_nSTR;	break;
	case StatusLimit::DEX:		wNeedStatusValue = status.m_nDEX;	break;
	case StatusLimit::CON:		wNeedStatusValue = status.m_nCON;	break;
	case StatusLimit::INT:		wNeedStatusValue = status.m_nINT;	break;
	case StatusLimit::WIS:		wNeedStatusValue = status.m_nWIS;	break;
	case StatusLimit::LEVEL:	wNeedStatusValue = cLevel;			break;	//--//
	}

	unsigned short wCompensationItem = 
		CItemMgr::GetInstance().GetCompensationItem(cCompensationType, wNeedStatusValue);

	if (0 == wCompensationItem)
	{
		// 보상 판매 대상 종류의 아이템이 아닙니다.
		return CEquipment::E_WRONG_TYPE;
	}

	Item::CEquipment* lpEquip = reinterpret_cast<CEquipment *>(CItemFactory::GetInstance().CreateItem(wCompensationItem));
	if (NULL == lpEquip)
	{
		return CEquipment::E_SERVER_ERROR;
	}

	lpEquip->m_ItemData.m_cNumOrDurability = m_ItemData.m_cNumOrDurability;
	lpEquip->m_cMaxNumOrDurability = m_cMaxNumOrDurability;

	for (unsigned char cAttributeIndex = 0; cAttributeIndex < Attribute::MAX_ATTRIBUTE_NUM; ++cAttributeIndex)
	{
		unsigned short wAttributeValue = 
			(GetAttribute(static_cast<Attribute::Type>(cAttributeIndex)) - m_ItemInfo.m_EquipAttribute.m_usAttributeValue[cAttributeIndex]) * 
			lpEquip->GetItemInfo().m_DetailData.m_wDropOption / m_ItemInfo.m_DetailData.m_wDropOption;

		lpEquip->AddAttribute(static_cast<Attribute::Type>(cAttributeIndex), wAttributeValue);
	}

	lpEquip->CalculateItemGrade();

	const Grade::GradeInfo originalGrade = GetItemGrade();
	const Grade::GradeInfo resultGrade = lpEquip->GetItemGrade();

	for (unsigned char cAttributeIndex = 0; cAttributeIndex < Attribute::MAX_ATTRIBUTE_NUM; ++cAttributeIndex)
	{
		if (resultGrade.m_aryAttributeGrade[cAttributeIndex] < originalGrade.m_eItemGrade)
		{
			const Item::CItemType::ArrayType eEquipType = CItemType::GetEquipType(lpEquip->GetItemInfo().m_DetailData.m_dwFlags);

			const short wMaxValue = static_cast<short>(m_ItemInfo.m_DetailData.m_wDropOption * 
				Grade::GetGradeValue(eEquipType, Grade::DROP_MAX_VALUE, originalGrade.m_eItemGrade, static_cast<Attribute::Type>(cAttributeIndex)));

			lpEquip->AddAttribute(static_cast<Attribute::Type>(cAttributeIndex), 
				wMaxValue - lpEquip->GetAttribute(static_cast<Attribute::Type>(cAttributeIndex)));
		}
	}

	lpEquip->CalculateItemGrade();
	lpEquip->CalculateItemPrice();

	// Rodin : 당분간은 보상 판매를 무제한으로 이용할 수 있다.
	//	SetNewEquip();

	const unsigned long dwOriginalGold = GetBuyPrice();
	const unsigned long dwResultGold = lpEquip->GetBuyPrice();

	lCompensationGold = static_cast<long>(dwOriginalGold - dwResultGold);

	*lppResultEquip = lpEquip;
	return CEquipment::S_SUCCESS;
}

unsigned long CEquipment::GetBuyPrice(void) const
{
	if (0 != m_dwStallPrice) 
	{ 
		return m_dwStallPrice; 
	}

	unsigned long dwPrice = CItem::GetBuyPrice();
	if (0 == m_cSeasonRecord)
	{
		if (m_GradeInfo.m_eItemGrade != EquipType::D_GRADE && 
			m_GradeInfo.m_eItemGrade != EquipType::F_GRADE &&
			m_GradeInfo.m_eItemGrade != EquipType::X_GRADE)	//--//
		{
			dwPrice *= PRICE_AGGRAVATION;
		}
	}

	return dwPrice;
}

unsigned long CEquipment::GetSellPrice(void) const
{
	if (0 != m_dwStallPrice) 
	{ 
		return m_dwStallPrice; 
	}

	unsigned long dwPrice = CItem::GetSellPrice();

	if (0 == m_cSeasonRecord)
	{
		if (m_GradeInfo.m_eItemGrade != EquipType::D_GRADE && 
			m_GradeInfo.m_eItemGrade != EquipType::F_GRADE &&
			m_GradeInfo.m_eItemGrade != EquipType::X_GRADE)	//--//
		{
			dwPrice *= PRICE_AGGRAVATION;
		}
	}

	return dwPrice;
}

unsigned long CEquipment::GetRepairPrice(void) const
{
	unsigned long dwPrice = CItem::GetRepairPrice();
	if (0 == m_cSeasonRecord)
	{
		if (m_GradeInfo.m_eItemGrade != EquipType::D_GRADE && 
			m_GradeInfo.m_eItemGrade != EquipType::F_GRADE &&
			m_GradeInfo.m_eItemGrade != EquipType::X_GRADE)	//--//
		{
			dwPrice *= PRICE_AGGRAVATION;
		}
	}

	return dwPrice;
}

unsigned long CEquipment::GetUpgradePrice(void) const
{
	unsigned long dwPrice = CItem::GetUpgradePrice();
	if (0 == m_cSeasonRecord)
	{
		if (m_GradeInfo.m_eItemGrade != EquipType::D_GRADE && 
			m_GradeInfo.m_eItemGrade != EquipType::F_GRADE &&
			m_GradeInfo.m_eItemGrade != EquipType::X_GRADE)	//--//
		{
			dwPrice *= PRICE_AGGRAVATION;
		}
	}

	return dwPrice;
}

unsigned long CEquipment::GetGraftPrice(void) const
{
	unsigned long dwPrice = CItem::GetGraftPrice();
	if (0 == m_cSeasonRecord)
	{
		if (m_GradeInfo.m_eItemGrade != EquipType::D_GRADE && 
			m_GradeInfo.m_eItemGrade != EquipType::F_GRADE &&
			m_GradeInfo.m_eItemGrade != EquipType::X_GRADE)	//--//
		{
			dwPrice *= PRICE_AGGRAVATION;
		}
	}

	return dwPrice;
}

void CEquipment::CalculateItemGrade(void)
{
	if (0 == m_ItemInfo.m_DetailData.m_wDropOption)	{ return; }

	// 아이템의 그레이드 정보를 초기화
	m_GradeInfo = Grade::GradeInfo();	

	ApplyRuneAttribute(REMOVE);

	for (unsigned char cAttributeIndex = 0; cAttributeIndex < Attribute::MAX_ATTRIBUTE_NUM; ++cAttributeIndex)
	{
		const short sAttrFactor = GetAttribute(static_cast<Attribute::Type>(cAttributeIndex));

		const float fGradeFactor = 
			(sAttrFactor - m_ItemInfo.m_EquipAttribute.m_usAttributeValue[cAttributeIndex]) / 
			static_cast<float>(m_ItemInfo.m_DetailData.m_wDropOption);

		// D 그레이드를 초과하면 F 그레이드가 된다.
		for (unsigned char cGradeIndex = 0; cGradeIndex <= EquipType::D_GRADE; ++cGradeIndex)
		{
			const Item::CItemType::ArrayType eEquipType = CItemType::GetEquipType(m_ItemInfo.m_DetailData.m_dwFlags);
			short sAttrValue = 0;

			// 3차 밸런스 패치
			// S 그레이드 제한값 계산공식은 예외적으로 적용되어 해당 부분에서도 S 그레이드 제한값 산출을 예외처리한다.
			if(EquipType::S_GRADE == static_cast<EquipType::Grade>(cGradeIndex))
			{
				GetLimitValue(static_cast<Attribute::Type>(cAttributeIndex), sAttrValue);
				if(0 < sAttrValue)
				{
					--sAttrValue;
				}
			}

			const float fItemFactor = (EquipType::S_GRADE == static_cast<EquipType::Grade>(cGradeIndex)) ?
				static_cast<const float>(sAttrFactor) : fGradeFactor;				

			const float fItemValue = (EquipType::S_GRADE == static_cast<EquipType::Grade>(cGradeIndex)) ?
				static_cast<const float>(sAttrValue) :
			Grade::GetGradeValue(eEquipType, Grade::VALUE_GRADE, 
				static_cast<EquipType::Grade>(cGradeIndex), static_cast<Attribute::Type>(cAttributeIndex));

			if (0 != fItemValue && fItemFactor > fItemValue)
			{
				m_GradeInfo.m_aryAttributeGrade[cAttributeIndex] = static_cast<EquipType::Grade>(cGradeIndex);

				if (m_GradeInfo.m_eItemGrade > m_GradeInfo.m_aryAttributeGrade[cAttributeIndex])
				{
					m_GradeInfo.m_eItemGrade = m_GradeInfo.m_aryAttributeGrade[cAttributeIndex];
					m_GradeInfo.m_cPlus = 0;
				}
				else
				{
					if (m_GradeInfo.m_eItemGrade == m_GradeInfo.m_aryAttributeGrade[cAttributeIndex])
					{
						++m_GradeInfo.m_cPlus;
					}
				}
				break;
			}
		}
	}

	ApplyRuneAttribute(APPLY);
}

void CEquipment::CalculateItemPrice(void)
{
	// edith 아이템 가격 변경

	// -------------------------------------------------------------------------------------------------
	// 가격 책정 테이블
	//--//	start..
	static const float aryPriceRateTable[Attribute::MAX_ATTRIBUTE_NUM] = {
//				MinDam.	MaxDam.	Armor	HitRate	Evade	MaxHP	HPRegen	MaxMP	MPRegen	Cri.	Block	Speed	M.Power	M.Res.	Rune	CoolDw.	SkillPt	Frost	Fire	Electro	Dark
		0,		2,		2,		2,		2,		2,		1,		1,		1,		1,		1,		2,		0,		1,		1,		0,		0,		0,		0,		0,		0,		0,
	};
	//--//	end..

	static const float aryGradeRateTable[EquipType::MAX_GRADE] = {
	//	S		AAA		AA		A		B		C		D		F		X
		3.0f,	2.4f,	1.8f,	1.4f,	1.2f,	1.15f,	1.1f,	1,		1
	};

	// -------------------------------------------------------------------------------------------------

	if (true == m_ItemInfo.m_DetailData.m_bOptionPrice)
	{
/*
		// 데미지의 경우 평균값을 이용한다.
		float fPriceFactor = 1;//((GetAttribute(Attribute::MIN_DAMAGE) + GetAttribute(Attribute::MAX_DAMAGE)) / 2.0f - 10) * aryPriceRateTable[Attribute::MIN_DAMAGE];
		// fPriceFactor = max(fPriceFactor, 0.0f);

		for (unsigned char cAttributeIndex = 0; cAttributeIndex < Attribute::MAX_ATTRIBUTE_NUM; ++cAttributeIndex)
		{
			if (Attribute::MIN_DAMAGE != cAttributeIndex && Attribute::MAX_DAMAGE != cAttributeIndex)
			{
				fPriceFactor += GetAttribute(static_cast<Attribute::Type>(cAttributeIndex)) * aryPriceRateTable[cAttributeIndex];
			}
		}

		m_dwPrice = static_cast<unsigned long>(((fPriceFactor * fPriceFactor) + 10) * aryGradeRateTable[m_GradeInfo.m_eItemGrade])/2 + m_ItemInfo.m_DetailData.m_dwPrice;
*/
		float fPriceFactor = 
			((GetAttribute(Attribute::MIN_DAMAGE) + GetAttribute(Attribute::MAX_DAMAGE)) / 2.0f - 10) * 
			aryPriceRateTable[Attribute::MIN_DAMAGE];

		fPriceFactor = max(fPriceFactor, 0.0f);

		for (unsigned char cAttributeIndex = 0; cAttributeIndex < Attribute::MAX_ATTRIBUTE_NUM; ++cAttributeIndex)
		{
			if (Attribute::MIN_DAMAGE != cAttributeIndex && Attribute::MAX_DAMAGE != cAttributeIndex)
			{
				fPriceFactor += GetAttribute(static_cast<Attribute::Type>(cAttributeIndex)) * aryPriceRateTable[cAttributeIndex];
			}
		}

		m_dwPrice = static_cast<unsigned long>(((fPriceFactor * fPriceFactor) + 10) * aryGradeRateTable[m_GradeInfo.m_eItemGrade]);
		// 장비의 기본가격을 더해준다.
		m_dwPrice += m_ItemInfo.m_DetailData.m_dwPrice;
	}
}

// 업그레이드 가능 여부 리턴
bool CEquipment::IsUpgradable(void) const
{
	switch(m_ItemInfo.m_DetailData.m_cItemType)
	{
		// 인간 방어구
	case Item::ItemType::CON_ARMOUR:
	case Item::ItemType::DEX_ARMOUR:

		// 인간 무기
	case Item::ItemType::ONEHANDED_SWORD:
	case Item::ItemType::TWOHANDED_SWORD:
	case Item::ItemType::ONEHANDED_AXE:
	case Item::ItemType::TWOHANDED_AXE:
	case Item::ItemType::ONEHANDED_BLUNT:
	case Item::ItemType::TWOHANDED_BLUNT:
	case Item::ItemType::BOW:
	case Item::ItemType::CROSSBOW:
	case Item::ItemType::STAFF:
	case Item::ItemType::DAGGER:
	case Item::ItemType::SHIELD:

		// 아칸 방어구 
	case Item::ItemType::CON_BODY:
	case Item::ItemType::DEX_BODY:

		// 아칸 무기
	case Item::ItemType::COM_BLUNT:
	case Item::ItemType::COM_SWORD:
	case Item::ItemType::OPP_HAMMER:
	case Item::ItemType::OPP_AXE:
	case Item::ItemType::OPP_SLUSHER:
	case Item::ItemType::OPP_TALON:
	case Item::ItemType::OPP_SYTHE:

		// 스킬암
	case Item::ItemType::SKILL_A_GUARD:
	case Item::ItemType::SKILL_A_ATTACK:
	case Item::ItemType::SKILL_A_GUN:
	case Item::ItemType::SKILL_A_KNIFE:

	// edith 2008.01.14 업그레이드 가능한지 추가

/*	case Item::ItemType::CON_HELM:
	case Item::ItemType::DEX_HELM:
	case Item::ItemType::CON_HEAD:
	case Item::ItemType::DEX_HEAD:

	case Item::ItemType::CON_GLOVE:
	case Item::ItemType::DEX_GLOVE:
	case Item::ItemType::CON_BOOTS:
	case Item::ItemType::DEX_BOOTS:
	case Item::ItemType::CON_PELVIS:
	case Item::ItemType::DEX_PELVIS:
	case Item::ItemType::CON_PROTECT_A:
	case Item::ItemType::DEX_PROTECT_A:*/
		return true;
	}

	return false;
}

// ------------------------------------------------------------------------------------------------------------
// 3차 밸런스 패치 관련 함수

// S 그레이드 제한값 구하기
bool CEquipment::GetLimitValue(short* lpattrLimit)
{
	short attrLimit[Item::Attribute::MAX_ATTRIBUTE_NUM] = {0};

	for(unsigned char cAttributeIndex = 0; 
		cAttributeIndex < Item::Attribute::MAX_ATTRIBUTE_NUM;
		++cAttributeIndex)
	{
		GetLimitValue(static_cast<Item::Attribute::Type>(cAttributeIndex), attrLimit[cAttributeIndex]);
	}

	memcpy(lpattrLimit, attrLimit, sizeof(short) * Item::Attribute::MAX_ATTRIBUTE_NUM);

	return true;
}

// S 그레이드 제한값 구하기
bool CEquipment::GetLimitValue(Item::Attribute::Type eAttributeType, short& attrLimit)
{
	const CItemType::ArrayType eEquipType = CItemType::GetEquipType(m_ItemInfo.m_DetailData.m_dwFlags);
	const Item::ItemType::Type eItemType = static_cast<Item::ItemType::Type>(GetItemInfo().m_DetailData.m_cItemType);

	const float fMaxGrade = Grade::GetGradeValue(eEquipType, Grade::VALUE_GRADE, EquipType::AAA_GRADE, eAttributeType);
	const float fGradeGap = fMaxGrade - Grade::GetGradeValue(eEquipType, Grade::VALUE_GRADE, EquipType::AA_GRADE, eAttributeType);

	// 제한 배율 조정
	unsigned short wLimitMagnification = 1;

	switch (eItemType)
	{
	case Item::ItemType::NECKLACE:			// 악세사리(목걸이)
	case Item::ItemType::RING:				// 악세사리(반지)
	case Item::ItemType::RUNE:				// 악세사리(룬)
	// 악세사리 아이템은 스크립트의 값으로 무조건 제한된다.
		attrLimit = static_cast<short>(m_ItemInfo.m_EquipAttribute.m_usAttributeValue[eAttributeType]);
		return true;
	case Item::ItemType::CON_ARMOUR:		// 인간 방어구(CON 갑옷)
	case Item::ItemType::CON_BODY:			// 아칸 방어구(CON 갑옷)
	case Item::ItemType::DEX_BODY:			// 아칸 방어구(DEX 갑옷)
		switch (eAttributeType)
		{
		case Attribute::ARMOR:			wLimitMagnification =  5;	break;
		case Attribute::EVADE:			wLimitMagnification =  5;	break;
		case Attribute::MAX_HP:			wLimitMagnification = 10;	break;
		case Attribute::HP_REGEN:		wLimitMagnification =  5;	break;
		case Attribute::MAX_MP:			wLimitMagnification = 10;	break;
		case Attribute::MAGIC_RESIST:	wLimitMagnification =  5;	break;
		}
		break;
	case Item::ItemType::ONEHANDED_SWORD:	// 인간 무기(한손검)
	case Item::ItemType::TWOHANDED_SWORD:	// 인간 무기(양손검)
	case Item::ItemType::ONEHANDED_BLUNT:	// 인간 무기(한손둔기)
	case Item::ItemType::TWOHANDED_BLUNT:	// 인간 무기(양손둔기)
	case Item::ItemType::ONEHANDED_AXE:		// 인간 무기(한손도끼)
	case Item::ItemType::TWOHANDED_AXE:		// 인간 무기(양손도끼)
	case Item::ItemType::DAGGER:			// 인간 무기(단검)
	case Item::ItemType::COM_BLUNT:			// 아칸 무기(컴배턴트 크러쉬웨폰)
	case Item::ItemType::COM_SWORD:			// 아칸 무기(컴배턴트 블레이드)
	case Item::ItemType::OPP_HAMMER:		// 아칸 무기(오피세이터 해머)
	case Item::ItemType::OPP_AXE:			// 아칸 무기(오피세이터 블레이드)
	case Item::ItemType::OPP_SLUSHER:		// 아칸 무기(오피세이트 크러쉬웨폰)
		switch (eAttributeType)
		{
		case Attribute::MIN_DAMAGE:		wLimitMagnification = 15;	break;
		case Attribute::MAX_DAMAGE:		wLimitMagnification = 15;	break;
		case Attribute::HIT_RATE:		wLimitMagnification =  5;	break;
		case Attribute::MAX_MP:			wLimitMagnification =  2;	break;
		case Attribute::MP_REGEN:		wLimitMagnification =  2;	break;
		case Attribute::CRITICAL:		wLimitMagnification =  5;	break;
		case Attribute::BLOCK:			wLimitMagnification =  5;	break;
		}
		break;	
	case Item::ItemType::SHIELD:			// 인간 방어구(방패)
	case Item::ItemType::SKILL_A_GUARD:		// 스킬암(가드암)
		switch (eAttributeType)
		{
		case Attribute::MAX_HP:			wLimitMagnification = 10;	break;
		case Attribute::HP_REGEN:		wLimitMagnification = 10;	break;
		case Attribute::MAX_MP:			wLimitMagnification = 10;	break;
		case Attribute::MP_REGEN:		wLimitMagnification = 10;	break;
		case Attribute::BLOCK:			wLimitMagnification	= 10;	break;
		}
		break;	
	case Item::ItemType::SKILL_A_ATTACK:	// 스킬암(어택암)
		switch (eAttributeType)
		{
		case Attribute::MIN_DAMAGE:		wLimitMagnification = 16;	break;
		case Attribute::MAX_DAMAGE:		wLimitMagnification = 16;	break;
		case Attribute::HIT_RATE:		wLimitMagnification =  5;	break;
		case Attribute::MAX_MP:			wLimitMagnification =  2;	break;
		case Attribute::MP_REGEN:		wLimitMagnification =  2;	break;
		case Attribute::CRITICAL:		wLimitMagnification =  5;	break;
		case Attribute::BLOCK:			wLimitMagnification =  5;	break;
		}
		break;	
	case Item::ItemType::BOW:				// 인간 무기(활)
	case Item::ItemType::CROSSBOW:			// 인간 무기(석궁)
	case Item::ItemType::SKILL_A_GUN:		// 스킬암(건암)
		switch (eAttributeType)
		{
		case Attribute::MIN_DAMAGE:		wLimitMagnification = 25;	break;
		case Attribute::MAX_DAMAGE:		wLimitMagnification = 25;	break;
		case Attribute::HIT_RATE:		wLimitMagnification = 10;	break;
		case Attribute::MAX_MP:			wLimitMagnification =  2;	break;
		case Attribute::MP_REGEN:		wLimitMagnification =  2;	break;
		case Attribute::CRITICAL:		wLimitMagnification = 10;	break;
		case Attribute::BLOCK:			wLimitMagnification =  5;	break;
		}
		break;
	case Item::ItemType::OPP_TALON:			// 아칸 무기(클로우)
	case Item::ItemType::SKILL_A_KNIFE:		// 스킬암(나이프암)
		switch (eAttributeType)
		{
		case Attribute::MIN_DAMAGE:		wLimitMagnification = 21;	break;
		case Attribute::MAX_DAMAGE:		wLimitMagnification = 21;	break;
		case Attribute::HIT_RATE:		wLimitMagnification =  5;	break;
		case Attribute::MAX_MP:			wLimitMagnification =  2;	break;
		case Attribute::MP_REGEN:		wLimitMagnification =  2;	break;
		case Attribute::CRITICAL:		wLimitMagnification =  5;	break;
		case Attribute::BLOCK:			wLimitMagnification =  5;	break;
		}
		break;

		// 예외처리 조항
	case Item::ItemType::DEX_ARMOUR:		// 인간 방어구(DEX 갑옷)
		switch (eAttributeType)
		{
		case Attribute::ARMOR:			wLimitMagnification =  3;	break;
		case Attribute::EVADE:			wLimitMagnification =  3;	break;
		case Attribute::MAX_HP:			wLimitMagnification =  7;	break;
		case Attribute::HP_REGEN:		wLimitMagnification =  3;	break;
//		case Attribute::MAX_MP:			wLimitMagnification =  7;	break;
		case Attribute::MAGIC_RESIST:	wLimitMagnification =  3;	break;
		}
		break;
	case Item::ItemType::STAFF:				// 인간 무기(스태프)
	case Item::ItemType::OPP_SYTHE:			// 아칸 무기(사이드)
		switch (eAttributeType)
		{
		case Attribute::MIN_DAMAGE:		wLimitMagnification = 10;	break;		// Dummy Factor. 스태프, 사이드는 최소데미지 고정
		case Attribute::MAX_DAMAGE:		wLimitMagnification =  7;	break;
		case Attribute::HIT_RATE:		wLimitMagnification =  2;	break;
		case Attribute::MAX_MP:			wLimitMagnification =  2;	break;
		case Attribute::MP_REGEN:		wLimitMagnification =  2;	break;
		case Attribute::CRITICAL:		wLimitMagnification =  2;	break;
		case Attribute::MAGIC_POWER:	wLimitMagnification =  7;	break;
		}
		break;
	}


	// 제한값 계산
	float fLimitFactor = fMaxGrade + (fGradeGap * wLimitMagnification);
	unsigned short attrDefault = m_ItemInfo.m_EquipAttribute.m_usAttributeValue[eAttributeType];

	#ifdef _RYL_GAME_CLIENT_
	if(CRYLNetworkData::Instance()->UseContents(GameRYL::REBALANCE_OVERITEM))
	#endif

	#ifndef _RYL_GAME_CLIENT_
	if(CServerSetup::GetInstance().UseContents(GameRYL::REBALANCE_OVERITEM))
	#endif
	{
		switch(eAttributeType)
		{
		case Attribute::MIN_DAMAGE:
			// 스태프, 사이드는 최소데미지 고정. Dummy 결과값으로 대체
			if(Item::ItemType::STAFF == eItemType ||
				Item::ItemType::OPP_SYTHE == eItemType)
			{
				attrLimit = static_cast<short>(ceil(static_cast<float>(attrDefault) + (wLimitMagnification * m_ItemInfo.m_DetailData.m_wDropOption)));
				break;
			}
		case Attribute::MAX_DAMAGE:
		case Attribute::ARMOR:
		case Attribute::BLOCK:
			attrLimit = static_cast<short>(ceil(static_cast<float>(attrDefault) + (fLimitFactor * m_ItemInfo.m_DetailData.m_wDropOption)));
			break;
		default:
			attrLimit = static_cast<short>(ceil(fLimitFactor * m_ItemInfo.m_DetailData.m_wDropOption));
			break;
		}
	}
	else
	{
		attrLimit = SHRT_MAX;
		return false;
	}

	return true;
}

// S 그레이드 제한값 보정(S 그레이드 이외에는 쓰지 마세요. 공식이 정확하지 않습니다. - by hackermz)
void CEquipment::RevisionLimit()
{
	short attrLimit[Item::Attribute::MAX_ATTRIBUTE_NUM] = {0};

	GetLimitValue(attrLimit);

	RevisionLimit(attrLimit);
}

void CEquipment::RevisionLimit(short* lpattrLimit)
{
	for(unsigned char cAttributeIndex = 0; 
		cAttributeIndex < Item::Attribute::MAX_ATTRIBUTE_NUM;
		++cAttributeIndex)
	{
		RevisionLimit(static_cast<Item::Attribute::Type>(cAttributeIndex), lpattrLimit[cAttributeIndex]);
	}
}

void CEquipment::RevisionLimit(Item::Attribute::Type eAttributeType, short& attrLimit)
{
	short attrFactor = 0;
	GetSubRuneAttribute(eAttributeType, attrFactor);

	if (attrFactor >= attrLimit)
	{
		// std::min이 클라이언트에서 먹지 않는 관계로..-_-;; (by hackermz)
		//		short attrRevision = (attrFactor >= attrLimit) ? attrLimit : attrFactor;

		RemoveUpgradeAttribute(eAttributeType);
		RemoveGemAttribute(eAttributeType);

		SetAttribute(eAttributeType, attrLimit);
	}
}

CUseItem::CUseItem(const ItemInfo& itemInfo)
:   CItem(itemInfo)
{
}

CUseItem::~CUseItem()
{
}
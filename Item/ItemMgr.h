#ifndef _CITEM_MGR_H_
#define _CITEM_MGR_H_

#define g_ItemMgr   Item::CItemMgr::GetInstance()

#include <vector>
#include <map>

#include "ItemStructure.h"


// ----------------------------------------------------------------------------
// class : CItemMgr ( must use in single thread )
// 아이템 스크립트를 로딩하고, 저장하고, 새 아이템을 생성하는 등의 역할을 한다.

namespace Item
{
	// 전방 참조
	class CItem;

	class CItemMgr
	{
	public:

		static CItemMgr& GetInstance();

		typedef std::map<ItemType::Type, unsigned char> RuneItemCnt;

		// ----------------------------------------------------------------------------
		// 아이템 스크립트 (ItemScript.txt)

		bool LoadItemProtoType(const char* szFileName = NULL);
		bool LoadItemProtoTypeBinary(const char* szFileNameBinary = NULL);
		bool SaveItemProtoTypeBinary(const char* szFileNameBinary = NULL, const char* szTrashFile = NULL);

		const ItemInfo* GetItemInfo(unsigned short usProtoTypeID);
		const ItemInfo* GetItemInfoFromItemName(const char* szItemName); 
		const ItemInfo* GetItemInfoFromEquipName(const char* szEquipName);

		const char*	GetEquipName(const unsigned short usItemProtoTypeID);
		const unsigned short GetItemIDFromSkillID(const unsigned short usSkill_ID, const unsigned short unSkill_LockCount);        	

		const unsigned short GetDropItem(const Item::ItemType::Type eItemType, const unsigned char cLevel);
		const unsigned short GetRingDropItem(const unsigned char cLevel, const unsigned char cGrade);	//--//
		const unsigned short GetNecklaceDropItem(const unsigned char cLevel, const unsigned char cGrade);	//--//
		const unsigned short GetRuneDropItem(const unsigned char cLevel, const unsigned char cGrade);	//--//
		void PrintDropListByLevel(void);
		void PrintRingDropListByLevel(void);	//--//
		void PrintNecklaceDropListByLevel(void);	//--//
		void PrintRuneDropListByLevel(void);	//--//


		// ----------------------------------------------------------------------------
		// 아이템 합성 스크립트 (ChemicalInfo.txt)

		bool LoadItemChemical(const char* szFileName = NULL);
		bool LoadItemChemicalBinary(const char* szFileNameBinary = NULL);
		bool SaveItemChemicalBinary(const char* szFileNameBinary = NULL, const char* szTrashFile = NULL);

		unsigned char GetChemicalResult(ChemicalInfo& chemical);

		// ----------------------------------------------------------------------------
		// 아이템 보상 스크립트 (SpeacialCompensation.txt)

		bool LoadItemSpeacialCompensation(const char* szFileName = NULL);
		bool LoadItemSpeacialCompensationBinary(const char* szFileNameBinary = NULL);
		bool SaveItemSpeacialCompensationBinary(const char* szFileNameBinary = NULL, const char* szTrashFile = NULL);

		unsigned short GetCompensationItem(unsigned char cItemType, short wLimitValue);

		// 아이템 타입에 따른 룬 소켓 쵀대값.
		unsigned char GetMaxRuneSocket(ItemType::Type type);

	private:

		CItemMgr();
		~CItemMgr();

		void DestoryItemInfo(void);
		void DestoryChemical(void);
		void DestoryCompensation(void);

		static const char*	ms_szChemicalScriptFileName;
		static const char*	ms_szSpeacialCompensationFileName;

		static CItemMgr     ms_this;

		size_t              m_nItemNum;
		ItemInfo*			m_ItemInfoArray;

		typedef std::vector<unsigned short> ItemIDVector;

		ItemIDVector	m_vecDropListByLevel[Creature::MONSTER_MAX_LEVEL];
		ItemIDVector	m_vecRingDropListByLevel[Creature::MONSTER_MAX_LEVEL][EquipType::MAX_GRADE];	//--//
		ItemIDVector	m_vecNecklaceDropListByLevel[Creature::MONSTER_MAX_LEVEL][EquipType::MAX_GRADE];	//--//
		ItemIDVector	m_vecRuneDropListByLevel[Creature::MONSTER_MAX_LEVEL][EquipType::MAX_GRADE];	//--//
		ItemIDVector	m_vecCompensation[Compensation::MAX_CASE];

		size_t				m_nChemicalNum;
		ChemicalInfo*		m_ChemicalArray;

		RuneItemCnt			m_mapRuneItemCnt;				// 룬 아이템 최대 개수.
	};
}


#endif
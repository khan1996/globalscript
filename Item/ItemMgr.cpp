
#include "stdafx.h"

#include <memory>
#include <algorithm>
#include <functional>
#include <set>
#include <string>

// 클라이언트에서도 쓰이므로 include를 명시한다.
#include <Log/ServerLog.h>

#include <Network/XORCrypt/XORCrypt.h>
#include <Network/Packet/PacketStruct/ServerInfo.h>

#include <Utility/Math/Math.h>
#include <Utility/DelimitedFile.h>
#include <Utility/TokenlizedFile.h>
#include <Utility/Resource/EnsureCleanup.h>
#include <Utility/Compress/MiniLZO/MiniLZOWrapper.h>

#include "ItemDataParser.h"
#include "ItemMgr.h"
#include "GMMemory.h"


#define DECODE_HEADER(Start_In, Length_In, PageVer_In, PageNum_In)  CXORCrypt::GetInstance().DecodeHeader((Start_In),(Length_In),(PageVer_In),(PageNum_In))
#define ENCODE_HEADER(Start_In, Length_In, PageVer_In, PageNum_In)  CXORCrypt::GetInstance().EncodeHeader((Start_In),(Length_In),(PageVer_In),(PageNum_In))
#define COMPRESS(In, In_len, Out, Out_len)      CMiniLZO::Compress((In), (In_len), (Out), (Out_len))
#define DECOMPRESS(In, In_len, Out, Out_len)    CMiniLZO::Decompress((In), (In_len), (Out), (Out_len))


using namespace Item;


Item::CItemMgr& Item::CItemMgr::GetInstance()
{
	static CItemMgr itemMgr;
	return itemMgr;
}

const char* CItemMgr::ms_szChemicalScriptFileName = "./Script/Game/Chemical.txt";
const char* CItemMgr::ms_szSpeacialCompensationFileName = "./Script/Game/SpeacialCompensation.txt";

CItemMgr CItemMgr::ms_this;

class CParseDelimitedData
{
public:

	CParseDelimitedData(CTokenlizedFile& TokenlizedFile) : m_TokenlizedFile(TokenlizedFile) { }

	bool operator() (ItemDataParser::ParseDataArray& ParserArray, ItemInfo& itemInfo)
	{
		for (ItemDataParser::ParseDataArray::iterator itr = ParserArray.begin();
			itr != ParserArray.end(); ++itr)
		{
			const char* szValue = m_TokenlizedFile.GetStringValue(itr->m_szColumnName);

			if (NULL == szValue)
			{
				ERRLOG2(g_Log, "데이터를 읽어올 수 없습니다. Line:%d, ColumnName:%s",
					m_TokenlizedFile.GetCurrentLine(), itr->m_szColumnName);
				return false;
			}

			if (false == itr->m_fnParseFunc(itemInfo, szValue))
			{
				ERRLOG2(g_Log, "알맞은 데이터가 아닙니다. Line:%d, ColumnName:%s",
					m_TokenlizedFile.GetCurrentLine(), itr->m_szColumnName);
				return false;
			}
		}

		return true;
	}

private:

	CTokenlizedFile& m_TokenlizedFile;
};


CItemMgr::CItemMgr()
:	m_ItemInfoArray(NULL), m_nItemNum(0), m_ChemicalArray(NULL), m_nChemicalNum(0)
{
	if(!m_mapRuneItemCnt.empty())
	{
		m_mapRuneItemCnt.clear();
	}

	m_mapRuneItemCnt.insert(std::make_pair(Item::ItemType::CON_ARMOUR, 1));			m_mapRuneItemCnt.insert(std::make_pair(Item::ItemType::DEX_ARMOUR, 1));
	m_mapRuneItemCnt.insert(std::make_pair(Item::ItemType::CON_BODY, 1));			m_mapRuneItemCnt.insert(std::make_pair(Item::ItemType::DEX_BODY, 1));
	m_mapRuneItemCnt.insert(std::make_pair(Item::ItemType::SHIELD, 1));				m_mapRuneItemCnt.insert(std::make_pair(Item::ItemType::SKILL_A_GUARD, 1));

	m_mapRuneItemCnt.insert(std::make_pair(Item::ItemType::ONEHANDED_SWORD, 1));	m_mapRuneItemCnt.insert(std::make_pair(Item::ItemType::ONEHANDED_AXE, 1));
	m_mapRuneItemCnt.insert(std::make_pair(Item::ItemType::ONEHANDED_BLUNT, 1));	m_mapRuneItemCnt.insert(std::make_pair(Item::ItemType::DAGGER, 1));
	m_mapRuneItemCnt.insert(std::make_pair(Item::ItemType::COM_BLUNT, 1));			m_mapRuneItemCnt.insert(std::make_pair(Item::ItemType::OPP_SLUSHER, 1));
	m_mapRuneItemCnt.insert(std::make_pair(Item::ItemType::OPP_TALON, 1));			m_mapRuneItemCnt.insert(std::make_pair(Item::ItemType::SKILL_A_ATTACK, 1));
	m_mapRuneItemCnt.insert(std::make_pair(Item::ItemType::SKILL_A_GUN, 1));		m_mapRuneItemCnt.insert(std::make_pair(Item::ItemType::SKILL_A_KNIFE, 1));
	m_mapRuneItemCnt.insert(std::make_pair(Item::ItemType::COM_SWORD, 1));

	m_mapRuneItemCnt.insert(std::make_pair(Item::ItemType::TWOHANDED_SWORD, 2));	m_mapRuneItemCnt.insert(std::make_pair(Item::ItemType::TWOHANDED_AXE, 2));
	m_mapRuneItemCnt.insert(std::make_pair(Item::ItemType::TWOHANDED_BLUNT, 2));	m_mapRuneItemCnt.insert(std::make_pair(Item::ItemType::BOW, 2));
	m_mapRuneItemCnt.insert(std::make_pair(Item::ItemType::CROSSBOW, 2));			m_mapRuneItemCnt.insert(std::make_pair(Item::ItemType::STAFF, 2));
	m_mapRuneItemCnt.insert(std::make_pair(Item::ItemType::OPP_HAMMER, 2));			m_mapRuneItemCnt.insert(std::make_pair(Item::ItemType::OPP_SYTHE, 2));
	m_mapRuneItemCnt.insert(std::make_pair(Item::ItemType::OPP_AXE, 2));
}

CItemMgr::~CItemMgr() 
{ 	
	DestoryItemInfo();
	DestoryChemical();
}

void CItemMgr::DestoryItemInfo(void)
{
	if (NULL != m_ItemInfoArray)
	{
		delete [] m_ItemInfoArray;
		m_ItemInfoArray = NULL;
	}

	m_nItemNum = 0;
}

void CItemMgr::DestoryChemical(void)
{
	if (NULL != m_ChemicalArray)
	{
		delete [] m_ChemicalArray;
		m_ChemicalArray = NULL;
	}

	m_nChemicalNum = 0;
}

void CItemMgr::DestoryCompensation(void)
{
	for (unsigned char cIndex = 0; cIndex < Compensation::MAX_CASE; ++cIndex)
	{
		m_vecCompensation[cIndex].clear();
	}
}

bool CItemMgr::LoadItemProtoType(const char* szFileName)
{
	if (NULL == szFileName)
	{
		ERRLOG0(g_Log, "아이템 스크립트 파일 로딩에 실패했습니다."
			"(인자로 받은 파일명이 NULL입니다.)");
		return false;
	}

	DestoryItemInfo();

	ItemInfo  itemInfo;
	CTokenlizedFile TokenlizedFile;	// 객체 소멸시, 자동 Close.

	if (false == TokenlizedFile.Open(szFileName))
	{
		ERRLOG0(g_Log, "아이템 스크립트 파일 로딩에 실패했습니다.");
		return false;
	}

	if (false == TokenlizedFile.ReadColumn())
	{
		ERRLOG0(g_Log, "아이템 스크립트 컬럼을 읽어오는 데 실패했습니다.");
		return false;
	}

	ItemDataParser::ParseDataArray  DefaultItemData;
	ItemDataParser::ParseDataArray  EquipItemData;
	ItemDataParser::ParseDataArray	AccessoryData;	//--//
	ItemDataParser::ParseDataArray	AvataData;	//--//
	ItemDataParser::ParseDataArray  UseItemData;
	ItemDataParser::ParseDataArray  EtcItemData;

	ItemDataParser::SetDefaultData(DefaultItemData);
	ItemDataParser::SetEquipData(EquipItemData);
	ItemDataParser::SetAccessoryData(AccessoryData);	//--//
	ItemDataParser::SetAvataData(AvataData);	//--//
	ItemDataParser::SetUseItemData(UseItemData);
	ItemDataParser::SetEtcItemData(EtcItemData);

	CItemType& ItemType = CItemType::GetInstance();

	std::vector<ItemInfo>     ItemInfoVector;

	CParseDelimitedData ParseData(TokenlizedFile);

	while (TokenlizedFile.ReadLine())
	{
		itemInfo.Initialize();

		if (false == ParseData(DefaultItemData, itemInfo)) 
		{ 
			return false; 
		}

		if (ItemType.IsCorrectItemType(CItemType::EQUIP_TYPE, itemInfo.m_DetailData.m_cItemType))
		{
			// 장비 아이템
			if (false == ParseData(EquipItemData, itemInfo)) 
			{ 
				return false; 
			}

			ItemType.SetEquipTypeFlags(itemInfo);

			// 아이템 보상 판매 대상 리스트
			if (0 != itemInfo.m_DetailData.m_wDropOption)
			{
				m_vecCompensation[itemInfo.m_DetailData.m_cItemType].push_back(itemInfo.m_usProtoTypeID);
			}

			// 드랍 리스트를 작성한다.
			if (0 != itemInfo.m_DetailData.m_cMinDropLevel && 
				0 != itemInfo.m_DetailData.m_cMaxDropLevel)
			{
				for (unsigned char cLevelIndex = itemInfo.m_DetailData.m_cMinDropLevel - 1; 
					cLevelIndex <= itemInfo.m_DetailData.m_cMaxDropLevel - 1; ++cLevelIndex)
				{
					if (cLevelIndex > Creature::MONSTER_MAX_LEVEL)
					{
						ERRLOG2(g_Log, "MaxDropLevel이 몬스터 최대 레벨(%d)을 초과하는 아이템이 존재합니다. ItemID:%d", 
							Creature::MONSTER_MAX_LEVEL, itemInfo.m_usProtoTypeID);
						continue;
					}

					m_vecDropListByLevel[cLevelIndex].push_back(itemInfo.m_usProtoTypeID);
				}
			}
		}
		//--//	start..
		else if (ItemType.IsCorrectItemType(CItemType::ACCESSORY_TYPE, itemInfo.m_DetailData.m_cItemType))
		{
			// 악세사리 아이템
			if (false == ParseData(AccessoryData, itemInfo))
			{
				return false;
			}

			ItemType.SetAccessoryTypeFlags(itemInfo);

			// 드랍 리스트를 작성한다.
			if (0 != itemInfo.m_DetailData.m_cMinDropLevel && 
				0 != itemInfo.m_DetailData.m_cMaxDropLevel)
			{
				for (unsigned char cLevelIndex = itemInfo.m_DetailData.m_cMinDropLevel - 1; 
					cLevelIndex <= itemInfo.m_DetailData.m_cMaxDropLevel - 1; ++cLevelIndex)
				{
					if (cLevelIndex > Creature::MONSTER_MAX_LEVEL)
					{
						ERRLOG2(g_Log, "MaxDropLevel이 몬스터 최대 레벨(%d)을 초과하는 아이템이 존재합니다. ItemID:%d", 
							Creature::MONSTER_MAX_LEVEL, itemInfo.m_usProtoTypeID);
						continue;
					}

					// 속성 아이템은 현재 드랍리스트에 포함시키지 않는다. 이후 속성 기획 추가시에 드랍시키도록 한다.
					if (0 < itemInfo.m_EquipAttribute.m_usAttributeValue[Attribute::FROST] ||
						0 < itemInfo.m_EquipAttribute.m_usAttributeValue[Attribute::FIRE] ||
						0 < itemInfo.m_EquipAttribute.m_usAttributeValue[Attribute::ELECTRO] ||
						0 < itemInfo.m_EquipAttribute.m_usAttributeValue[Attribute::DARKNESS])
					{
						continue;
					}

					switch (itemInfo.m_DetailData.m_cItemType)
					{
						case ItemType::NECKLACE:
							m_vecNecklaceDropListByLevel[cLevelIndex][itemInfo.m_DetailData.m_cGrade].push_back(itemInfo.m_usProtoTypeID);
							break;

						case ItemType::RING:
							m_vecRingDropListByLevel[cLevelIndex][itemInfo.m_DetailData.m_cGrade].push_back(itemInfo.m_usProtoTypeID);
							break;

						case ItemType::RUNE:
							m_vecRuneDropListByLevel[cLevelIndex][itemInfo.m_DetailData.m_cGrade].push_back(itemInfo.m_usProtoTypeID);
							break;
					}
				}
			}
		}
		else if (ItemType.IsCorrectItemType(CItemType::AVATA_TYPE, itemInfo.m_DetailData.m_cItemType))
		{
			// 사용 아이템
			if (false == ParseData(AvataData, itemInfo)) 
			{ 
				return false; 
			}

			ItemType.SetAvataTypeFlags(itemInfo);
		}
		//--//	end..
		else if (ItemType.IsCorrectItemType(CItemType::USE_ITEM_TYPE, itemInfo.m_DetailData.m_cItemType))
		{
			// 사용 아이템
			if (false == ParseData(UseItemData, itemInfo)) 
			{ 
				return false; 
			}

			ItemType.SetUseItemTypeFlags(itemInfo);
		}
		else
		{
			// 기타 아이템
			if (false == ParseData(EtcItemData, itemInfo))  
			{ 
				return false; 
			}

			ItemType.SetEtcItemTypeFlags(itemInfo);
		}

		ItemInfoVector.push_back(itemInfo);
	}

	if (false == TokenlizedFile.IsEOF())
	{
		ERRLOG0(g_Log, "아이템 스크립트 파일을 다 읽지 못하고 종료합니다.");
		return false;
	}

	std::sort(ItemInfoVector.begin(), ItemInfoVector.end());

	for (std::vector<ItemInfo>::iterator itr = ItemInfoVector.begin();
		itr != ItemInfoVector.end() - 1; ++itr)
	{
		if (itr->m_usProtoTypeID == (itr+1)->m_usProtoTypeID)
		{
			ERRLOG1(g_Log, "겹치는 아이템 종류 ID가 있습니다. 종류ID:%d", itr->m_usProtoTypeID);
			return false;
		}
	};

	m_nItemNum = ItemInfoVector.size();
	m_ItemInfoArray = new ItemInfo[m_nItemNum];
	if (NULL == m_ItemInfoArray)
	{
		ERRLOG0(g_Log, "아이템 스크립트 내용을 복사할 수 없습니다. 메모리가 부족합니다.");
		return false;
	}

	std::copy(ItemInfoVector.begin(), ItemInfoVector.end(), m_ItemInfoArray);

	return true;
}

unsigned char CItemMgr::GetMaxRuneSocket(ItemType::Type type)
{
	RuneItemCnt::iterator find = m_mapRuneItemCnt.find(type);

	if(find==m_mapRuneItemCnt.end())
		return 0;

	return find->second;
}

const ItemInfo* CItemMgr::GetItemInfo(unsigned short usProtoTypeID)
{
	ItemInfo* lpFirst = m_ItemInfoArray;
	ItemInfo* lpLast  = m_ItemInfoArray + m_nItemNum;
	ItemInfo* lpMid   = NULL;

	size_t nCount   = m_nItemNum;
	size_t nCount2  = 0;

	for (; 0 < nCount; )
	{
		nCount2 = nCount / 2;
		lpMid = lpFirst + nCount2;

		if (lpMid->m_usProtoTypeID < usProtoTypeID)
		{
			lpFirst = ++lpMid, nCount -= nCount2 + 1;
		}
		else
		{
			nCount = nCount2;
		}
	}

	return (lpFirst != lpLast && !(usProtoTypeID < lpFirst->m_usProtoTypeID)) ? lpFirst : NULL;
}

const ItemInfo* CItemMgr::GetItemInfoFromItemName(const char* szItemName)
{
	ItemInfo* lpFirst = m_ItemInfoArray;
	ItemInfo* lpLast  = m_ItemInfoArray + m_nItemNum;

	for (; lpFirst != lpLast; ++lpFirst)
	{
		if (0 == strcmp(lpFirst->m_SpriteData.m_szName, szItemName))
		{
			return lpFirst;
		}
	}

	return NULL;
}

const ItemInfo* CItemMgr::GetItemInfoFromEquipName(const char* szEquipName)
{
	ItemInfo* lpFirst = m_ItemInfoArray;
	ItemInfo* lpLast  = m_ItemInfoArray + m_nItemNum;

	for (; lpFirst != lpLast; ++lpFirst)
	{
		if (0 == strcmp(lpFirst->m_StringData.m_szEquipModelName, szEquipName))
		{
			return lpFirst;
		}
	}

	return NULL;
}

const char* CItemMgr::GetEquipName(const unsigned short usItemProtoTypeID)
{
	const ItemInfo* pItemInfo = GetItemInfo(usItemProtoTypeID);

	if(pItemInfo)
	{
		return pItemInfo->m_StringData.m_szEquipModelName;
	}

	return NULL;
}

const unsigned short CItemMgr::GetItemIDFromSkillID(const unsigned short usSkill_ID, const unsigned short unSkill_LockCount)
{
	ItemInfo* lpFirst = m_ItemInfoArray;
	ItemInfo* lpLast  = m_ItemInfoArray + m_nItemNum;

	for (; lpFirst != lpLast; ++lpFirst)
	{
		if (lpFirst->m_UseItemInfo.m_usSkill_ID == usSkill_ID && 
			lpFirst->m_UseItemInfo.m_usSkill_LockCount == unSkill_LockCount)
		{
			return lpFirst->m_usProtoTypeID;
		}
	}

	return 0;
}

const unsigned short CItemMgr::GetDropItem(const Item::ItemType::Type eItemType, const unsigned char cLevel)
{
	std::random_shuffle(m_vecDropListByLevel[cLevel].begin(), m_vecDropListByLevel[cLevel].end());

	for (std::vector<unsigned short>::iterator it = m_vecDropListByLevel[cLevel].begin(); 
		it != m_vecDropListByLevel[cLevel].end(); ++it)
	{
		unsigned short wItemID = *it;
		if (eItemType == GetItemInfo(wItemID)->m_DetailData.m_cItemType)
		{
			return wItemID;
		}
	}

	return 0;
}

//--//	start..
const unsigned short CItemMgr::GetRingDropItem(const unsigned char cLevel, const unsigned char cGrade)
{
	// AAA, AA, X Grade 는 떨어질수 없다.
	if (cGrade < EquipType::A_GRADE || cGrade > EquipType::F_GRADE || m_vecRingDropListByLevel[cLevel][cGrade].empty())
	{
		return 0;
	}

	std::random_shuffle(m_vecRingDropListByLevel[cLevel][cGrade].begin(), m_vecRingDropListByLevel[cLevel][cGrade].end());
	return (*m_vecRingDropListByLevel[cLevel][cGrade].begin());
}

const unsigned short CItemMgr::GetNecklaceDropItem(const unsigned char cLevel, const unsigned char cGrade)
{
	// AAA, AA, X Grade 는 떨어질수 없다.
	if (cGrade < EquipType::A_GRADE || cGrade > EquipType::F_GRADE || m_vecNecklaceDropListByLevel[cLevel][cGrade].empty())
	{
		return 0;
	}

	std::random_shuffle(m_vecNecklaceDropListByLevel[cLevel][cGrade].begin(), m_vecNecklaceDropListByLevel[cLevel][cGrade].end());
	return (*m_vecNecklaceDropListByLevel[cLevel][cGrade].begin());
}

const unsigned short CItemMgr::GetRuneDropItem(const unsigned char cLevel, const unsigned char cGrade)
{
	// AAA, AA, X Grade 는 떨어질수 없다.
	if (cGrade < EquipType::A_GRADE || cGrade > EquipType::F_GRADE || m_vecRuneDropListByLevel[cLevel][cGrade].empty())
	{
		return 0;
	}

	std::random_shuffle(m_vecRuneDropListByLevel[cLevel][cGrade].begin(), m_vecRuneDropListByLevel[cLevel][cGrade].end());
	return (*m_vecRuneDropListByLevel[cLevel][cGrade].begin());
}
//--//	end..

void CItemMgr::PrintDropListByLevel(void)
{
	FILE* fpItemID = fopen("DropListByLevel(ItemID).txt", "wt");
	FILE* fpTypeName = fopen("DropListByLevel(TypeName).txt", "wt");

	std::set<std::string> setTypeName[Creature::MONSTER_MAX_LEVEL];

	for (unsigned char cIndex = 0; cIndex < Creature::MONSTER_MAX_LEVEL; ++cIndex)
	{
		fprintf(fpItemID, "Level %d\t", cIndex + 1);

		for (std::vector<unsigned short>::iterator it = m_vecDropListByLevel[cIndex].begin(); 
			it != m_vecDropListByLevel[cIndex].end(); ++it)
		{
			unsigned short wItemID = *it;
			fprintf(fpItemID, "%d\t", wItemID);

			setTypeName[cIndex].insert(GetItemInfo(wItemID)->m_StringData.m_szTypeName);
		}

		fprintf(fpItemID, "\n");
	}

	for (unsigned char cIndex = 0; cIndex < Creature::MONSTER_MAX_LEVEL; ++cIndex)
	{
		fprintf(fpTypeName, "Level %d\t", cIndex + 1);

		for (std::set<std::string>::iterator it = setTypeName[cIndex].begin(); 
			it != setTypeName[cIndex].end(); ++it)
		{
			std::string strTypeName = *it;
			fprintf(fpTypeName, "%s\t", strTypeName.c_str());
		}

		fprintf(fpTypeName, "\n");
	}

	fclose(fpTypeName);
	fclose(fpItemID);
}

//--//	start..
void CItemMgr::PrintRingDropListByLevel(void)
{
	FILE* fpItemID = fopen("RingDropListByLevel(ItemID).txt", "wt");
	FILE* fpTypeName = fopen("RingDropListByLevel(TypeName).txt", "wt");

	std::set<std::string> setTypeName[Creature::MONSTER_MAX_LEVEL][EquipType::MAX_GRADE];
	CTypeName* lpTypeArray = CItemType::GetInstance().GetItemGradeName();

	for (unsigned char cIndex = 0; cIndex < Creature::MONSTER_MAX_LEVEL; ++cIndex)
	{
		fprintf(fpItemID, "Level %d\n", cIndex + 1);
		
		for (unsigned char cGradeIdx = 0; cGradeIdx < EquipType::MAX_GRADE; ++cGradeIdx)
		{
			fprintf(fpItemID,"\t%s Grade\t", lpTypeArray[cGradeIdx].GetTypeName());

			for (std::vector<unsigned short>::iterator it = m_vecRingDropListByLevel[cIndex][cGradeIdx].begin(); 
				it != m_vecRingDropListByLevel[cIndex][cGradeIdx].end(); ++it)
			{
				unsigned short wItemID = *it;
				fprintf(fpItemID, "%d\t", wItemID);

				setTypeName[cIndex][cGradeIdx].insert(GetItemInfo(wItemID)->m_StringData.m_szTypeName);
			}

			fprintf(fpItemID, "\n");
		}
	}

	for (unsigned char cIndex = 0; cIndex < Creature::MONSTER_MAX_LEVEL; ++cIndex)
	{
		fprintf(fpTypeName, "Level %d\n", cIndex + 1);

		for (unsigned char cGradeIdx = 0; cGradeIdx < EquipType::MAX_GRADE; ++cGradeIdx)
		{
			fprintf(fpItemID,"\t%s Grade\t", lpTypeArray[cGradeIdx].GetTypeName());

			for (std::set<std::string>::iterator it = setTypeName[cIndex][cGradeIdx].begin(); 
				it != setTypeName[cIndex][cGradeIdx].end(); ++it)
			{
				std::string strTypeName = *it;
				fprintf(fpTypeName, "%s\t", strTypeName.c_str());
			}

			fprintf(fpTypeName, "\n");
		}
	}

	fclose(fpTypeName);
	fclose(fpItemID);
}

void CItemMgr::PrintNecklaceDropListByLevel(void)
{
	FILE* fpItemID = fopen("NecklaceDropListByLevel(ItemID).txt", "wt");
	FILE* fpTypeName = fopen("NecklaceDropListByLevel(TypeName).txt", "wt");

	std::set<std::string> setTypeName[Creature::MONSTER_MAX_LEVEL][EquipType::MAX_GRADE];
	CTypeName* lpTypeArray = CItemType::GetInstance().GetItemGradeName();

	for (unsigned char cIndex = 0; cIndex < Creature::MONSTER_MAX_LEVEL; ++cIndex)
	{
		fprintf(fpItemID, "Level %d\n", cIndex + 1);

		for (unsigned char cGradeIdx = 0; cGradeIdx < EquipType::MAX_GRADE; ++cGradeIdx)
		{
			fprintf(fpItemID,"\t%s Grade\t", lpTypeArray[cGradeIdx].GetTypeName());

			for (std::vector<unsigned short>::iterator it = m_vecNecklaceDropListByLevel[cIndex][cGradeIdx].begin(); 
				it != m_vecNecklaceDropListByLevel[cIndex][cGradeIdx].end(); ++it)
			{
				unsigned short wItemID = *it;
				fprintf(fpItemID, "%d\t", wItemID);

				setTypeName[cIndex][cGradeIdx].insert(GetItemInfo(wItemID)->m_StringData.m_szTypeName);
			}

			fprintf(fpItemID, "\n");
		}
	}

	for (unsigned char cIndex = 0; cIndex < Creature::MONSTER_MAX_LEVEL; ++cIndex)
	{
		fprintf(fpTypeName, "Level %d\n", cIndex + 1);

		for (unsigned char cGradeIdx = 0; cGradeIdx < EquipType::MAX_GRADE; ++cGradeIdx)
		{
			fprintf(fpItemID,"\t%s Grade\t", lpTypeArray[cGradeIdx].GetTypeName());

			for (std::set<std::string>::iterator it = setTypeName[cIndex][cGradeIdx].begin(); 
				it != setTypeName[cIndex][cGradeIdx].end(); ++it)
			{
				std::string strTypeName = *it;
				fprintf(fpTypeName, "%s\t", strTypeName.c_str());
			}

			fprintf(fpTypeName, "\n");
		}
	}

	fclose(fpTypeName);
	fclose(fpItemID);
}

void CItemMgr::PrintRuneDropListByLevel(void)
{
	FILE* fpItemID = fopen("RuneDropListByLevel(ItemID).txt", "wt");
	FILE* fpTypeName = fopen("RuneDropListByLevel(TypeName).txt", "wt");

	std::set<std::string> setTypeName[Creature::MONSTER_MAX_LEVEL][EquipType::MAX_GRADE];
	CTypeName* lpTypeArray = CItemType::GetInstance().GetItemGradeName();

	for (unsigned char cIndex = 0; cIndex < Creature::MONSTER_MAX_LEVEL; ++cIndex)
	{
		fprintf(fpItemID, "Level %d\n", cIndex + 1);

		for (unsigned char cGradeIdx = 0; cGradeIdx < EquipType::MAX_GRADE; ++cGradeIdx)
		{
			fprintf(fpItemID,"\t%s Grade\t", lpTypeArray[cGradeIdx].GetTypeName());

			for (std::vector<unsigned short>::iterator it = m_vecRuneDropListByLevel[cIndex][cGradeIdx].begin(); 
				it != m_vecRuneDropListByLevel[cIndex][cGradeIdx].end(); ++it)
			{
				unsigned short wItemID = *it;
				fprintf(fpItemID, "%d\t", wItemID);

				setTypeName[cIndex][cGradeIdx].insert(GetItemInfo(wItemID)->m_StringData.m_szTypeName);
			}

			fprintf(fpItemID, "\n");
		}
	}

	for (unsigned char cIndex = 0; cIndex < Creature::MONSTER_MAX_LEVEL; ++cIndex)
	{
		fprintf(fpTypeName, "Level %d\n", cIndex + 1);

		for (unsigned char cGradeIdx = 0; cGradeIdx < EquipType::MAX_GRADE; ++cGradeIdx)
		{
			fprintf(fpItemID,"\t%s Grade\t", lpTypeArray[cGradeIdx].GetTypeName());

			for (std::set<std::string>::iterator it = setTypeName[cIndex][cGradeIdx].begin(); 
				it != setTypeName[cIndex][cGradeIdx].end(); ++it)
			{
				std::string strTypeName = *it;
				fprintf(fpTypeName, "%s\t", strTypeName.c_str());
			}

			fprintf(fpTypeName, "\n");
		}
	}

	fclose(fpTypeName);
	fclose(fpItemID);
}
//--//	end..

bool CItemMgr::LoadItemProtoTypeBinary(const char* szFileNameBinary)
{
	if (NULL == szFileNameBinary) { return false; }

	HANDLE hFile = CreateFile(szFileNameBinary, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE) { return false; } 

	CEnsureCloseHandle EnsureCloseHandle(hFile);

	unsigned long dwRead            = 0;
	unsigned long dwFileHighSize    = 0;
	unsigned long dwFileSize        = GetFileSize(hFile, &dwFileHighSize);

	char* lpAllocated = new char[dwFileSize];
	CEnsureDeleteArray<char> EnsureFileInfo(lpAllocated);

	if (false == ReadFile(hFile, lpAllocated, dwFileSize, &dwRead, NULL)) 
	{        
		return false;
	}

	unsigned long dwHeaderSize          = sizeof(unsigned long) + *reinterpret_cast<unsigned long *>(lpAllocated) + sizeof(unsigned long);
	unsigned long dwDecompressedSize    = *reinterpret_cast<unsigned long *>(lpAllocated + dwHeaderSize - sizeof(unsigned long));

	DestoryItemInfo();

	m_nItemNum = dwDecompressedSize / sizeof(ItemInfo);
	m_ItemInfoArray = new ItemInfo[m_nItemNum];
	if (NULL == m_ItemInfoArray)
	{
		ERRLOG0(g_Log, "아이템 스크립트 내용을 불러올 수 없습니다. 메모리가 부족합니다.");
		return false;
	}

	DECOMPRESS(lpAllocated + dwHeaderSize, dwFileSize - dwHeaderSize, 
		reinterpret_cast<char *>(m_ItemInfoArray), &dwDecompressedSize);
	DECODE_HEADER(reinterpret_cast<char *>(m_ItemInfoArray), dwDecompressedSize, 3, 4);

	////////////////////////////////////////////////////////////////////////////////////////
	// edith 2008.01.14 gsf 수동 수정작업.
	CItemType& ItemType = CItemType::GetInstance();

	for(int i = 0; i < (int)m_nItemNum; ++i)
	{
		if (ItemType.IsCorrectItemType(CItemType::EQUIP_TYPE, m_ItemInfoArray[i].m_DetailData.m_cItemType))
		{
			ItemType.SetEquipTypeFlags(m_ItemInfoArray[i]);
		}
	}

	return true;
}

bool CItemMgr::SaveItemProtoTypeBinary(const char* szFileNameBinary, const char* szTrashFile)
{
	if (NULL == szFileNameBinary || NULL == szTrashFile) { return false; }

	HANDLE hFile = CreateFile(szFileNameBinary, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE) { return false; }

	CEnsureCloseHandle EnsureCloseSaveHandle(hFile);

	unsigned long   nItemInfoSize       = static_cast<unsigned long>(sizeof(ItemInfo) * m_nItemNum);
	unsigned long   nCompressedInfoSize = nItemInfoSize;

	char*   lpItemInfo          = new char[nItemInfoSize];
	char*   lpCompressedInfo    = new char[nItemInfoSize];

	CEnsureDeleteArray<char> EnsureDeleteItemInfo(lpItemInfo);
	CEnsureDeleteArray<char> EnsureDeleteCompressedInfo(lpCompressedInfo);

	if (NULL == lpItemInfo || NULL == lpCompressedInfo)
	{
		return false;
	}

	memcpy(lpItemInfo, m_ItemInfoArray, nItemInfoSize);
	ENCODE_HEADER(lpItemInfo, nItemInfoSize, 3, 4);
	COMPRESS(lpItemInfo, nItemInfoSize, lpCompressedInfo, &nCompressedInfoSize);

	// 쓰레기(더미) 자료
	HANDLE hTrashFile = CreateFile(szTrashFile, GENERIC_READ, FILE_SHARE_READ, 
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hTrashFile == INVALID_HANDLE_VALUE) 
	{ 
		ERRLOG1(g_Log, "%s 파일을 열 수 없습니다.", szTrashFile);
		return false; 
	} 

	CEnsureCloseHandle EnsureCloseTrashHandle(hTrashFile);

	unsigned long dwRead = 0;
	unsigned long dwWritten = 0;
	unsigned long dwFileHighSize = 0;
	unsigned long dwTrashFileSize = GetFileSize(hTrashFile, &dwFileHighSize);

	char* lpTrashFileData = new char[dwTrashFileSize];
	CEnsureDeleteArray<char> EnsureDeleteTrashFile(lpTrashFileData);

	if (false == ReadFile(hTrashFile, lpTrashFileData, dwTrashFileSize, &dwRead, NULL))
	{
		ERRLOG0(g_Log, "쓰레기 파일을 읽을 수 없습니다.");
		return false;
	}

	// Trash File데이터
	WriteFile(hFile, &dwTrashFileSize, sizeof(unsigned long), &dwWritten, 0);	// Trash 파일의 크기
	WriteFile(hFile, lpTrashFileData, dwTrashFileSize, &dwWritten, 0);			// Trash 파일 내용

	// 올바른 자료
	WriteFile(hFile, &nItemInfoSize, sizeof(unsigned long), &dwWritten, 0);     // 원본 데이터 크기
	WriteFile(hFile, lpCompressedInfo, nCompressedInfoSize, &dwWritten, 0);		// 압축된 데이터

	return true;
}


bool CItemMgr::LoadItemChemical(const char* szFileName)
{
	const int	        ITEM_NAME_LEN = 32;
	char		        szBuffer[ITEM_NAME_LEN];

	int                 nLineCount  = 0;
	CDelimitedFile      DelimitedFile;      // 객체 소멸시, 자동 Close.

	// 매크로에 로그 코드 삽입을 잊지 말 것.
	// 매크로에서 \뒤에 공백이나 문자 삽입되지 않도록 주의할 것.
	// ( '이스케이프 시퀀스가 잘못되었습니다' 에러 발생 )
#define READ_DATA(ColumnName, Argument) \
	if (false == DelimitedFile.ReadData(Argument)) \
	{ \
	ERRLOG2(g_Log, "아이템 합성 스크립트 읽기 실패 : %d행 %s컬럼에서 에러 발생!", nLineCount, #ColumnName); \
	return false; \
	}

#define READ_STRING(ColumnName, Buffer, BufferSize) \
	if (false == DelimitedFile.ReadString(Buffer, BufferSize)) \
	{\
	ERRLOG2(g_Log, "아이템 합성 스크립트 읽기 실패 : %d행 %s컬럼에서 에러 발생!", nLineCount, #ColumnName);\
	return false;\
	}


	if (false == DelimitedFile.Open(szFileName ? szFileName : ms_szChemicalScriptFileName, 1)) 
	{
		ERRLOG1(g_Log, "%s 파일을 열 수 없습니다.", szFileName ? szFileName : ms_szChemicalScriptFileName);
		return false;
	}

	std::vector<ChemicalInfo>		ChemicalVector;
	ChemicalInfo					tempChemical;

	while (DelimitedFile.ReadLine()) 
	{
		++nLineCount;

		READ_STRING("PickkingItem Name", szBuffer, ITEM_NAME_LEN);
		READ_DATA("PickkingItem ID", tempChemical.m_wPickkingItemID);
		READ_DATA("PickkingItem Num", tempChemical.m_cPickkingItemNum);

		READ_STRING("TargetItem Name", szBuffer, ITEM_NAME_LEN);
		READ_DATA("TargetItem ID", tempChemical.m_wTargetItemID);
		READ_DATA("TargetItem Num", tempChemical.m_cTargetItemNum);

		READ_STRING("ResultItem Name", szBuffer, ITEM_NAME_LEN);
		READ_DATA("ResultItem ID", tempChemical.m_wResultItemID);
		READ_DATA("ResultItem Num", tempChemical.m_cResultItemNum);

		READ_DATA("Item ID", tempChemical.m_dwItemID);

        READ_STRING("Ability ID", szBuffer, ITEM_NAME_LEN);
        tempChemical.m_usAbilityID = Math::Convert::Atos(szBuffer);
		READ_DATA("Ability Level", tempChemical.m_cAbilityLevel);

        READ_STRING("Quest ID", szBuffer, ITEM_NAME_LEN);
        tempChemical.m_usQuestID = Math::Convert::Atos(szBuffer);
		READ_DATA("Success Percent", tempChemical.m_cPercent);
		
		const ItemInfo* lpPickkingItem = GetItemInfo(tempChemical.m_wPickkingItemID);
		const ItemInfo* lpTargetItem = GetItemInfo(tempChemical.m_wTargetItemID);
		const ItemInfo* lpResultItem = GetItemInfo(tempChemical.m_wResultItemID);

		if (NULL == lpPickkingItem || NULL == lpTargetItem || NULL == lpResultItem)
		{
			ERRLOG3(g_Log, "존재하지 않는 아이템이 합성법에 들어가 있습니다. "
				"들고있는 아이템 : %d, 합성대상 아이템 : %d, 합성결과물 : %d", 
				tempChemical.m_wPickkingItemID, tempChemical.m_wTargetItemID, tempChemical.m_wResultItemID);
			return false;
		}

		if (lpTargetItem->m_DetailData.m_cXSize < lpResultItem->m_DetailData.m_cXSize || 
			lpTargetItem->m_DetailData.m_cYSize < lpResultItem->m_DetailData.m_cYSize)
		{
			ERRLOG6(g_Log, "합성대상 아이템이 합성결과물보다 작습니다. "
				"합성대상 아이템 : %d(%dx%d), 합성결과문 : %d(%dx%d)", 
				tempChemical.m_wTargetItemID, lpTargetItem->m_DetailData.m_cXSize, lpTargetItem->m_DetailData.m_cYSize, 
				tempChemical.m_wResultItemID, lpResultItem->m_DetailData.m_cXSize, lpResultItem->m_DetailData.m_cYSize);
			return false;
		}

		ChemicalVector.push_back(tempChemical);
	}

	std::sort(ChemicalVector.begin(), ChemicalVector.end());

	m_nChemicalNum = ChemicalVector.size();
	m_ChemicalArray = new ChemicalInfo[m_nChemicalNum];
	if (NULL == m_ChemicalArray)
	{
		ERRLOG0(g_Log, "메모리 할당에 실패했습니다.");
		return false;
	}

	std::copy(ChemicalVector.begin(), ChemicalVector.end(), m_ChemicalArray);

	return true;
}

bool CItemMgr::LoadItemChemicalBinary(const char* szFileNameBinary)
{
	HANDLE hFile = CreateFile((0 == szFileNameBinary) ? ms_szChemicalScriptFileName : szFileNameBinary,
		GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE) { return false; } 

	CEnsureCloseHandle EnsureCloseHandle(hFile);

	unsigned long dwRead            = 0;
	unsigned long dwFileHighSize    = 0;
	unsigned long dwFileSize        = GetFileSize(hFile, &dwFileHighSize);

	char* lpAllocated = new char[dwFileSize];
	CEnsureDeleteArray<char> EnsureFileInfo(lpAllocated);

	if (false == ReadFile(hFile, lpAllocated, dwFileSize, &dwRead, NULL)) 
	{        
		return false;
	}

	unsigned long dwHeaderSize          = sizeof(unsigned long) + *reinterpret_cast<unsigned long *>(lpAllocated) + sizeof(unsigned long);
	unsigned long dwDecompressedSize    = *reinterpret_cast<unsigned long *>(lpAllocated + dwHeaderSize - sizeof(unsigned long));

	DestoryChemical();

	m_nChemicalNum = dwDecompressedSize / sizeof(ChemicalInfo);
	m_ChemicalArray = new ChemicalInfo[m_nChemicalNum];
	if (NULL == m_ChemicalArray)
	{
		ERRLOG0(g_Log, "아이템 함성 스크립트 내용을 불러올 수 없습니다. 메모리가 부족합니다.");
		return false;
	}

	DECOMPRESS(lpAllocated + dwHeaderSize, dwFileSize - dwHeaderSize, 
		reinterpret_cast<char *>(m_ChemicalArray), &dwDecompressedSize);
	DECODE_HEADER(reinterpret_cast<char *>(m_ChemicalArray), dwDecompressedSize, 3, 4);

	return true;
}

bool CItemMgr::SaveItemChemicalBinary(const char* szFileNameBinary, const char* szTrashFile)
{
	HANDLE hFile = CreateFile((0 == szFileNameBinary) ? ms_szChemicalScriptFileName : szFileNameBinary,
		GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE) { return false; }

	CEnsureCloseHandle EnsureCloseSaveHandle(hFile);

	unsigned long   nChemicalInfoSize	= static_cast<unsigned long>(sizeof(ChemicalInfo) * m_nChemicalNum);
	unsigned long   nCompressedInfoSize = nChemicalInfoSize*2;

	char*   lpChemicalInfo		= new char[nChemicalInfoSize];
	char*   lpCompressedInfo    = new char[nChemicalInfoSize*2];

	CEnsureDeleteArray<char> EnsureDeleteItemInfo(lpChemicalInfo);
	CEnsureDeleteArray<char> EnsureDeleteCompressedInfo(lpCompressedInfo);

	if (NULL == lpChemicalInfo || NULL == lpCompressedInfo)
	{
		return false;
	}

	memcpy(lpChemicalInfo, m_ChemicalArray, nChemicalInfoSize);
	ENCODE_HEADER(lpChemicalInfo, nChemicalInfoSize, 3, 4);
	COMPRESS(lpChemicalInfo, nChemicalInfoSize, lpCompressedInfo, &nCompressedInfoSize);

	// 쓰레기(더미) 자료
	HANDLE hTrashFile = CreateFile(szTrashFile, GENERIC_READ, FILE_SHARE_READ, 
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hTrashFile == INVALID_HANDLE_VALUE) 
	{ 
		ERRLOG1(g_Log, "%s 파일을 열 수 없습니다.", szTrashFile);
		return false; 
	} 

	CEnsureCloseHandle EnsureCloseTrashHandle(hTrashFile);

	unsigned long dwRead = 0;
	unsigned long dwWritten = 0;
	unsigned long dwFileHighSize = 0;
	unsigned long dwTrashFileSize = GetFileSize(hTrashFile, &dwFileHighSize);

	char* lpTrashFileData = new char[dwTrashFileSize];
	CEnsureDeleteArray<char> EnsureDeleteTrashFile(lpTrashFileData);

	if (false == ReadFile(hTrashFile, lpTrashFileData, dwTrashFileSize, &dwRead, NULL))
	{
		ERRLOG0(g_Log, "쓰레기 파일을 읽을 수 없습니다.");
		return false;
	}

	// Trash File데이터
	WriteFile(hFile, &dwTrashFileSize, sizeof(unsigned long), &dwWritten, 0);	// Trash 파일의 크기
	WriteFile(hFile, lpTrashFileData, dwTrashFileSize, &dwWritten, 0);			// Trash 파일 내용

	// 올바른 자료
	WriteFile(hFile, &nChemicalInfoSize, sizeof(unsigned long), &dwWritten, 0);	// 원본 데이터 크기
	WriteFile(hFile, lpCompressedInfo, nCompressedInfoSize, &dwWritten, 0);		// 압축된 데이터

	return true;
}

class CFindChemical : public std::unary_function<ChemicalInfo, bool>
{
public:

	explicit CFindChemical(unsigned short wPickkingItemID, unsigned short wTargetItemID) 
		:	m_wPickkingItemID(wPickkingItemID), m_wTargetItemID(wTargetItemID)
	{ }		

	bool operator() (ChemicalInfo& chemical) 
	{ 
		return (m_wPickkingItemID == chemical.m_wPickkingItemID && 
			m_wTargetItemID == chemical.m_wTargetItemID); 
	}


private:

	const unsigned short m_wPickkingItemID;
	const unsigned short m_wTargetItemID;
};

unsigned char CItemMgr::GetChemicalResult(ChemicalInfo& chemical)
{
	CFindChemical findChemical(chemical.m_wPickkingItemID, chemical.m_wTargetItemID);
	const ChemicalInfo* lpChemicalRecipe = 
		std::find_if(&m_ChemicalArray[0], &m_ChemicalArray[m_nChemicalNum], findChemical);
	if (&m_ChemicalArray[m_nChemicalNum] == lpChemicalRecipe)
	{
		return Chemical::F_WRONG_RECIPE;
	}

	if (lpChemicalRecipe->m_cPickkingItemNum > chemical.m_cPickkingItemNum || 
		lpChemicalRecipe->m_cTargetItemNum > chemical.m_cTargetItemNum)
	{
		return Chemical::F_NOT_ENOUGH_ITEM;
	}

	if (0 != chemical.m_cTargetItemNum % lpChemicalRecipe->m_cTargetItemNum || 
		0 != chemical.m_cPickkingItemNum % lpChemicalRecipe->m_cPickkingItemNum)
	{
		return Chemical::F_WRONG_RATE;
	}

	chemical.m_wResultItemID = lpChemicalRecipe->m_wResultItemID;

	unsigned char cPickkingItemUse = chemical.m_cPickkingItemNum / lpChemicalRecipe->m_cPickkingItemNum;
	unsigned char cTargetItemUse = chemical.m_cTargetItemNum / lpChemicalRecipe->m_cTargetItemNum;

	if (cPickkingItemUse >= cTargetItemUse)
	{
		chemical.m_cPickkingItemNum = chemical.m_cPickkingItemNum - cTargetItemUse * lpChemicalRecipe->m_cPickkingItemNum;
		chemical.m_cTargetItemNum = 0;

		chemical.m_cResultItemNum = cTargetItemUse * lpChemicalRecipe->m_cResultItemNum;
	}
	else
	{
		chemical.m_cPickkingItemNum = 0;
		chemical.m_cTargetItemNum = chemical.m_cTargetItemNum - cPickkingItemUse * lpChemicalRecipe->m_cTargetItemNum;

		chemical.m_cResultItemNum = cPickkingItemUse * lpChemicalRecipe->m_cResultItemNum;
	}

	chemical.m_usAbilityID = lpChemicalRecipe->m_usAbilityID;
	chemical.m_cAbilityLevel = lpChemicalRecipe->m_cAbilityLevel;
	chemical.m_usQuestID = lpChemicalRecipe->m_usQuestID;
	chemical.m_cPercent = lpChemicalRecipe->m_cPercent;

	return Chemical::S_SUCCESS;
}


bool CItemMgr::LoadItemSpeacialCompensation(const char* szFileName)
{
	int                 nLineCount  = 0;
	CDelimitedFile      DelimitedFile;      // 객체 소멸시, 자동 Close.

	if (false == DelimitedFile.Open(szFileName ? szFileName : ms_szSpeacialCompensationFileName)) 
	{
		ERRLOG1(g_Log, "%s 파일을 열 수 없습니다.", szFileName ? szFileName : ms_szSpeacialCompensationFileName);
		return false;
	}

	while (DelimitedFile.ReadLine()) 
	{
		unsigned short wItemID = 0;
		while (DelimitedFile.ReadData(wItemID))
		{
			m_vecCompensation[Compensation::CASE1 + nLineCount].push_back(wItemID);
		}

		++nLineCount;
	}

	return true;
}

bool CItemMgr::LoadItemSpeacialCompensationBinary(const char* szFileNameBinary)
{
	HANDLE hFile = CreateFile((0 == szFileNameBinary) ? ms_szSpeacialCompensationFileName : szFileNameBinary,
		GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE) { return false; } 

	CEnsureCloseHandle EnsureCloseHandle(hFile);

	unsigned long dwRead            = 0;
	unsigned long dwFileHighSize    = 0;
	unsigned long dwFileSize        = GetFileSize(hFile, &dwFileHighSize);

	char* lpAllocated = new char[dwFileSize];
	CEnsureDeleteArray<char> EnsureFileInfo(lpAllocated);

	if (false == ReadFile(hFile, lpAllocated, dwFileSize, &dwRead, NULL)) 
	{        
		return false;
	}

	unsigned long dwHeaderSize          = sizeof(unsigned long) + *reinterpret_cast<unsigned long *>(lpAllocated) + sizeof(unsigned long);
	unsigned long dwDecompressedSize    = *reinterpret_cast<unsigned long *>(lpAllocated + dwHeaderSize - sizeof(unsigned long));

	DestoryCompensation();

	unsigned long dwTotalNum = dwDecompressedSize / sizeof(unsigned short);
	unsigned short* lpCompensation = new unsigned short[dwTotalNum];
	CEnsureDeleteArray<unsigned short> EnsureCompensation(lpCompensation);

	if (NULL == lpCompensation)
	{
		ERRLOG0(g_Log, "아이템 보상 스크립트 내용을 불러올 수 없습니다. 메모리가 부족합니다.");
		return false;
	}

	DECOMPRESS(lpAllocated + dwHeaderSize, dwFileSize - dwHeaderSize, 
		reinterpret_cast<char *>(lpCompensation), &dwDecompressedSize);
	DECODE_HEADER(reinterpret_cast<char *>(lpCompensation), dwDecompressedSize, 3, 4);

	// 1차 배열의 값을 벡터 배열에 알맞게 넣어준다.
	unsigned char cLine = 0;
	for (unsigned long dwIndex = 0; dwIndex < dwTotalNum; ++dwIndex)
	{
		unsigned short wItemID = *(lpCompensation + dwIndex);
		if (0 == wItemID)
		{
			++cLine;
		}
		else
		{
			m_vecCompensation[cLine].push_back(wItemID);
		}
	}

	return true;
}

bool CItemMgr::SaveItemSpeacialCompensationBinary(const char* szFileNameBinary, const char* szTrashFile)
{
	HANDLE hFile = CreateFile((0 == szFileNameBinary) ? ms_szSpeacialCompensationFileName : szFileNameBinary,
		GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE) { return false; }

	CEnsureCloseHandle EnsureCloseSaveHandle(hFile);

	unsigned long dwTotalNum = 0;
	for (unsigned char cIndex = 0; cIndex < Compensation::MAX_CASE; ++cIndex)
	{
		dwTotalNum += static_cast<unsigned long>(m_vecCompensation[cIndex].size());
	}

	unsigned long   nCompensationSize	= static_cast<unsigned long>(sizeof(unsigned short) * (dwTotalNum + Compensation::MAX_CASE));
	unsigned long   nCompressedInfoSize = nCompensationSize;

	char*   lpCompensation		= new char[nCompensationSize];
	char*   lpCompressedInfo    = new char[nCompressedInfoSize];

	CEnsureDeleteArray<char> EnsureDeleteItemInfo(lpCompensation);
	CEnsureDeleteArray<char> EnsureDeleteCompressedInfo(lpCompressedInfo);

	if (NULL == lpCompensation || NULL == lpCompressedInfo)
	{
		return false;
	}

	// 벡터 배열을 1차 배열로 변환한다. 벡터 배열 각 인덱스의 경계에는 0을 넣어 구분한다.
	unsigned short* lpLine = reinterpret_cast<unsigned short *>(lpCompensation);
	for (unsigned char cIndex = 0; cIndex < Compensation::MAX_CASE; ++cIndex)
	{
		std::copy(m_vecCompensation[cIndex].begin(), m_vecCompensation[cIndex].end(), lpLine);
		lpLine += m_vecCompensation[cIndex].size();

		*lpLine = 0;
		++lpLine;
	}

	ENCODE_HEADER(lpCompensation, nCompensationSize, 3, 4);
	COMPRESS(lpCompensation, nCompensationSize, lpCompressedInfo, &nCompressedInfoSize);

	// 쓰레기(더미) 자료
	HANDLE hTrashFile = CreateFile(szTrashFile, GENERIC_READ, FILE_SHARE_READ, 
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hTrashFile == INVALID_HANDLE_VALUE) 
	{ 
		ERRLOG1(g_Log, "%s 파일을 열 수 없습니다.", szTrashFile);
		return false; 
	} 

	CEnsureCloseHandle EnsureCloseTrashHandle(hTrashFile);

	unsigned long dwRead = 0;
	unsigned long dwWritten = 0;
	unsigned long dwFileHighSize = 0;
	unsigned long dwTrashFileSize = GetFileSize(hTrashFile, &dwFileHighSize);

	char* lpTrashFileData = new char[dwTrashFileSize];
	CEnsureDeleteArray<char> EnsureDeleteTrashFile(lpTrashFileData);

	if (false == ReadFile(hTrashFile, lpTrashFileData, dwTrashFileSize, &dwRead, NULL))
	{
		ERRLOG0(g_Log, "쓰레기 파일을 읽을 수 없습니다.");
		return false;
	}

	// Trash File데이터
	WriteFile(hFile, &dwTrashFileSize, sizeof(unsigned long), &dwWritten, 0);	// Trash 파일의 크기
	WriteFile(hFile, lpTrashFileData, dwTrashFileSize, &dwWritten, 0);			// Trash 파일 내용

	// 올바른 자료
	WriteFile(hFile, &nCompensationSize, sizeof(unsigned long), &dwWritten, 0);	// 원본 데이터 크기
	WriteFile(hFile, lpCompressedInfo, nCompressedInfoSize, &dwWritten, 0);		// 압축된 데이터

	return true;
}

unsigned short CItemMgr::GetCompensationItem(unsigned char cItemType, short wLimitValue)
{
	unsigned short wCompensationItem = 0;
	short wCompensationValue = 0;

	for (std::vector<unsigned short>::iterator it = m_vecCompensation[cItemType].begin(); 
		it != m_vecCompensation[cItemType].end(); ++it)
	{
		const ItemInfo* itemInfo = GetItemInfo(*it);

		// 드랍되지않는 아이템은 보상 받을 수도 없다.
		if (0 != itemInfo->m_DetailData.m_cMinDropLevel && 0 != itemInfo->m_DetailData.m_cMaxDropLevel)
		{
			// 착용자의 스탯 제한에 안 걸리는 것들 중 가장 좋은 것으로 보상해준다.
			if (wCompensationValue < itemInfo->m_UseLimit.m_wLimitValue && itemInfo->m_UseLimit.m_wLimitValue <= wLimitValue)
			{
				wCompensationItem = itemInfo->m_usProtoTypeID;
				wCompensationValue = itemInfo->m_UseLimit.m_wLimitValue;
			}
		}
	}

	return wCompensationItem;
}
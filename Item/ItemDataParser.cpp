
#include "stdafx.h"

#include <Utility/Math/Math.h>

// 클라이언트에서도 쓰이므로 include를 명시한다.
#include <Log/ServerLog.h>

#include "ItemStructure.h"
#include "ItemDataParser.h"
#include "GMMemory.h"


using namespace Item;

bool ReadString(char* szBuffer_Out, int nBufferLen, const char* szValue_In)
{
    int nLength = _snprintf(szBuffer_Out, nBufferLen - 1, "%s", szValue_In);
    
    if (0 <= nLength)
    {
        szBuffer_Out[nLength] = '\0';
        return true;
    }

    szBuffer_Out[nBufferLen - 1] = '\0';
    ERRLOG1(g_Log, "버퍼 길이가 모자라서, 데이터를 전부 복사할 수 없습니다. Data:%s", szValue_In);
    return false;    
}

inline bool ReadValue(unsigned char& cValue_Out, const char* szValue_In)
{
    cValue_Out = static_cast<unsigned char>(atoi(szValue_In));
    return true;
}

inline bool ReadValue(unsigned short& usValue_Out, const char* szValue_In)
{
    usValue_Out = static_cast<unsigned short>(atoi(szValue_In));
    return true;
}

inline bool ReadValue(unsigned long& dwValue_Out, const char* szValue_In)
{
    dwValue_Out = static_cast<unsigned long>(atol(szValue_In));
    return true;
}

inline bool ReadValue(float& fValue_Out, const char* szValue_In)
{
	fValue_Out = static_cast<float>(atof(szValue_In));
	return true;
}

bool ReadStringToTypeValue(unsigned char& cValue_Out, const CTypeName* lpTypeArray, 
                           const unsigned char nMaxType, const char* szValue_In)
{   
    for (unsigned char nCount = 0; nCount < nMaxType; ++nCount)
    {
		const char* szTypeName = lpTypeArray[nCount].GetTypeName();
		if (NULL != szTypeName)
		{
			if (0 == strcmp(szValue_In, szTypeName)) 
			{
				cValue_Out = lpTypeArray[nCount].GetTypeValue();
				return true;
			}
		}
    }

    return false;
}

bool ReadStringToBool(bool& bValue_Out, const char* szValue_In)
{
	char szBuffer[2];
	_snprintf(szBuffer, 2, "%s", szValue_In);
	bValue_Out = (!strcmp(szBuffer, "O")) ? true : false;
	return true;
}


// 기본 데이터들
bool ReadID(Item::ItemInfo& itemInfo, const char* szValue)
{
    itemInfo.m_usProtoTypeID = static_cast<unsigned short>(atoi(szValue));
    if (0 == itemInfo.m_usProtoTypeID || 0xFFFF == itemInfo.m_usProtoTypeID)
    {
        ERRLOG1(g_Log, "아이템 ID가 이상합니다. 0이나 0xFFFF는 사용하실 수 없습니다. ID:0x%08x",
            itemInfo.m_usProtoTypeID);

        return false;
    }

    return true;
}

bool ReadItemName(Item::ItemInfo& itemInfo, const char* szValue)            { return ReadString(itemInfo.m_SpriteData.m_szName, SpriteData::MAX_NAME, szValue); }
bool ReadFieldModelName(Item::ItemInfo& itemInfo, const char* szValue)      { return ReadString(itemInfo.m_StringData.m_szFieldModelName, StringData::MAX_FIELD_MODEL_NAME, szValue); }
bool ReadAttachedModelName(Item::ItemInfo& itemInfo, const char* szValue)   { return ReadString(itemInfo.m_StringData.m_szEquipModelName, StringData::MAX_EQUIP_MODEL_NAME, szValue); }
bool ReadSpriteDDS(Item::ItemInfo& itemInfo, const char* szValue)           { return ReadString(itemInfo.m_SpriteData.m_szSpriteName, SpriteData::MAX_SPRITE_NAME, szValue); }
bool ReadMinX(Item::ItemInfo& itemInfo, const char* szValue)                { return ReadValue(itemInfo.m_SpriteData.m_nSpriteMinX, szValue); }
bool ReadMinY(Item::ItemInfo& itemInfo, const char* szValue)                { return ReadValue(itemInfo.m_SpriteData.m_nSpriteMinY, szValue); }
bool ReadMaxX(Item::ItemInfo& itemInfo, const char* szValue)                { return ReadValue(itemInfo.m_SpriteData.m_nSpriteMaxX, szValue); }
bool ReadMaxY(Item::ItemInfo& itemInfo, const char* szValue)                { return ReadValue(itemInfo.m_SpriteData.m_nSpriteMaxY, szValue); }
bool ReadSizeX(Item::ItemInfo& itemInfo, const char* szValue)               { return ReadValue(itemInfo.m_DetailData.m_cXSize, szValue); }
bool ReadSizeY(Item::ItemInfo& itemInfo, const char* szValue)               { return ReadValue(itemInfo.m_DetailData.m_cYSize, szValue); }
bool ReadEffectSound(Item::ItemInfo& itemInfo, const char* szValue)         { return ReadString(itemInfo.m_StringData.m_szEffectSoundName, StringData::MAX_EFFECT_SOUND_NAME, szValue); }

bool ReadItemType(Item::ItemInfo& itemInfo, const char* szValue)
{
    return ReadStringToTypeValue(itemInfo.m_DetailData.m_cItemType, 
        Item::CItemType::GetInstance().GetItemTypeName(), Item::ItemType::MAX_ITEM_TYPE, szValue);
}

bool ReadTypeName(Item::ItemInfo& itemInfo, const char* szValue)			{ return ReadString(itemInfo.m_StringData.m_szTypeName, StringData::MAX_TYPE_NAME, szValue);	}
bool ReadAbleExchangeNDrop(Item::ItemInfo& itemInfo, const char* szValue)	{ return ReadStringToBool(itemInfo.m_DetailData.m_bExchangeAndDrop, szValue);	}
bool ReadAbleSell(Item::ItemInfo& itemInfo, const char* szValue)			{ return ReadStringToBool(itemInfo.m_DetailData.m_bSell, szValue);				}
bool ReadMinDropLevel(Item::ItemInfo& itemInfo, const char* szValue)		{ return ReadValue(itemInfo.m_DetailData.m_cMinDropLevel, szValue); 			}
bool ReadMaxDropLevel(Item::ItemInfo& itemInfo, const char* szValue)		{ return ReadValue(itemInfo.m_DetailData.m_cMaxDropLevel, szValue); 			}
bool ReadPrice(Item::ItemInfo& itemInfo, const char* szValue)           	{ return ReadValue(itemInfo.m_DetailData.m_dwPrice, szValue);					}
bool ReadOptionPrice(Item::ItemInfo& itemInfo, const char* szValue)			{ return ReadStringToBool(itemInfo.m_DetailData.m_bOptionPrice, szValue);		}
bool ReadBlackPrice(Item::ItemInfo& itemInfo, const char* szValue)      	{ return ReadValue(itemInfo.m_DetailData.m_dwBlackPrice, szValue);				}
bool ReadMedalPrice(Item::ItemInfo& itemInfo, const char* szValue)      	{ return ReadValue(itemInfo.m_DetailData.m_dwMedalPrice, szValue);				}
bool ReadDurability(Item::ItemInfo& itemInfo, const char* szValue)      	{ return ReadValue(itemInfo.m_DetailData.m_cDefaultDurabilityOrStack, szValue); }
bool ReadMaxDurability(Item::ItemInfo& itemInfo, const char* szValue)   	{ return ReadValue(itemInfo.m_DetailData.m_cMaxDurabilityOrStack, szValue);		}
bool ReadClassLimit(Item::ItemInfo& itemInfo, const char* szValue)      	{ return ReadValue(itemInfo.m_UseLimit.m_dwClassLimit, szValue);				}
bool ReadLowLevel(Item::ItemInfo& itemInfo, const char* szValue)	{ return ReadValue(itemInfo.m_DetailData.m_wLowLimitLevel, szValue); }
bool ReadHighLevel(Item::ItemInfo& itemInfo, const char* szValue)	{ return ReadValue(itemInfo.m_DetailData.m_wHighLimitLevel, szValue); }

bool ReadLimitStatus(Item::ItemInfo& itemInfo, const char* szValue)
{
	return ReadStringToTypeValue(itemInfo.m_UseLimit.m_cLimitStatus, 
		Item::CItemType::GetInstance().GetItemLimitName(), Item::StatusLimit::MAX_STATUS_LIMIT, szValue);
}

bool ReadLimitValue(Item::ItemInfo& itemInfo, const char* szValue)      { return ReadValue(itemInfo.m_UseLimit.m_wLimitValue, szValue); }

bool ItemDataParser::SetDefaultData(ParseDataArray& parseDataArray)
{
    parseDataArray.reserve(40);

    parseDataArray.push_back(ParseData("ID",                    ReadID));
    parseDataArray.push_back(ParseData("ItemName",              ReadItemName));
    parseDataArray.push_back(ParseData("FieldModelName",        ReadFieldModelName));
    parseDataArray.push_back(ParseData("AttachedModelName", 	ReadAttachedModelName));
    parseDataArray.push_back(ParseData("SpriteDDS",             ReadSpriteDDS));
    parseDataArray.push_back(ParseData("MinX",                  ReadMinX));
    parseDataArray.push_back(ParseData("MinY",                  ReadMinY));
    parseDataArray.push_back(ParseData("MaxX",                  ReadMaxX));
    parseDataArray.push_back(ParseData("MaxY",                  ReadMaxY));
    parseDataArray.push_back(ParseData("SizeX",                 ReadSizeX));
    parseDataArray.push_back(ParseData("SizeY",                 ReadSizeY));
    parseDataArray.push_back(ParseData("EffectSound",           ReadEffectSound));
    parseDataArray.push_back(ParseData("ItemType",              ReadItemType));
	parseDataArray.push_back(ParseData("TypeName",              ReadTypeName));
	parseDataArray.push_back(ParseData("AbleExchangeNDrop",		ReadAbleExchangeNDrop));
	parseDataArray.push_back(ParseData("AbleSell",              ReadAbleSell));
	parseDataArray.push_back(ParseData("MinDropLevel",			ReadMinDropLevel));
	parseDataArray.push_back(ParseData("MaxDropLevel",			ReadMaxDropLevel));
    parseDataArray.push_back(ParseData("Price",                 ReadPrice));
	parseDataArray.push_back(ParseData("OptionPrice",			ReadOptionPrice));
    parseDataArray.push_back(ParseData("BlackPrice",            ReadBlackPrice));
	parseDataArray.push_back(ParseData("MedalPrice",            ReadMedalPrice));
    parseDataArray.push_back(ParseData("ClassLimit",            ReadClassLimit));
	parseDataArray.push_back(ParseData("LimitStat",				ReadLimitStatus));
	parseDataArray.push_back(ParseData("Limit",					ReadLimitValue));
	parseDataArray.push_back(ParseData("Durability",            ReadDurability));
	parseDataArray.push_back(ParseData("LowLevel",				ReadLowLevel)); 
	parseDataArray.push_back(ParseData("HighLevel",				ReadHighLevel)); 
	parseDataArray.push_back(ParseData("MaxDurability",         ReadMaxDurability));
	

	return true;
}

bool ReadMinDamage(Item::ItemInfo& itemInfo, const char* szValue)		{ ++itemInfo.m_EquipAttribute.m_nAttibuteNum; return ReadValue(itemInfo.m_EquipAttribute.m_usAttributeValue[Item::Attribute::MIN_DAMAGE], szValue);		}
bool ReadMaxDamage(Item::ItemInfo& itemInfo, const char* szValue)		{ ++itemInfo.m_EquipAttribute.m_nAttibuteNum; return ReadValue(itemInfo.m_EquipAttribute.m_usAttributeValue[Item::Attribute::MAX_DAMAGE], szValue);		}
bool ReadArmor(Item::ItemInfo& itemInfo, const char* szValue)			{ ++itemInfo.m_EquipAttribute.m_nAttibuteNum; return ReadValue(itemInfo.m_EquipAttribute.m_usAttributeValue[Item::Attribute::ARMOR], szValue);			}
bool ReadBlock(Item::ItemInfo& itemInfo, const char* szValue)			{ ++itemInfo.m_EquipAttribute.m_nAttibuteNum; return ReadValue(itemInfo.m_EquipAttribute.m_usAttributeValue[Item::Attribute::BLOCK], szValue);			}
bool ReadDropOption(Item::ItemInfo& itemInfo, const char* szValue)		{ return ReadValue(itemInfo.m_DetailData.m_wDropOption, szValue);	}
bool ReadGrade(Item::ItemInfo& itemInfo, const char* szValue)
{
	return ReadStringToTypeValue(itemInfo.m_DetailData.m_cGrade,
		Item::CItemType::GetInstance().GetItemGradeName(), Item::EquipType::MAX_GRADE, szValue);
}
bool ReadCriticalType(Item::ItemInfo& itemInfo, const char* szValue)	{ return ReadValue(itemInfo.m_DetailData.m_cCriticalType, szValue); }
bool ReadAttackRange(Item::ItemInfo& itemInfo, const char* szValue)		{ return ReadValue(itemInfo.m_DetailData.m_cAttackRange, szValue);	}
bool ReadMaxSocketNum(Item::ItemInfo& itemInfo, const char* szValue)	{ return ReadValue(itemInfo.m_DetailData.m_cMaxSocketNum, szValue); }
bool ReadLuckChance(Item::ItemInfo& itemInfo, const char* szValue)		{ ++itemInfo.m_EquipAttribute.m_nAttibuteNum; return ReadValue(itemInfo.m_EquipAttribute.m_usAttributeValue[Item::Attribute::LUCKCHANCE], szValue);			}

bool ItemDataParser::SetEquipData(ParseDataArray& parseDataArray)
{
	parseDataArray.push_back(ParseData("MinDamage",             ReadMinDamage));
	parseDataArray.push_back(ParseData("MaxDamage",             ReadMaxDamage));
	parseDataArray.push_back(ParseData("Defence",				ReadArmor));
	parseDataArray.push_back(ParseData("Block",					ReadBlock));
	parseDataArray.push_back(ParseData("DropOption",			ReadDropOption));
	parseDataArray.push_back(ParseData("Grade",					ReadGrade));	
	parseDataArray.push_back(ParseData("CriticalType",			ReadCriticalType));
	parseDataArray.push_back(ParseData("AttackRange",			ReadAttackRange));
	parseDataArray.push_back(ParseData("MaxSocketNum",			ReadMaxSocketNum));	
	parseDataArray.push_back(ParseData("MagicResist",			ReadLuckChance));	

	return true;
}

bool ReadHitRate(Item::ItemInfo& itemInfo, const char* szValue)			{ ++itemInfo.m_EquipAttribute.m_nAttibuteNum; return ReadValue(itemInfo.m_EquipAttribute.m_usAttributeValue[Item::Attribute::HIT_RATE], szValue);		}
bool ReadEvade(Item::ItemInfo& itemInfo, const char* szValue)			{ ++itemInfo.m_EquipAttribute.m_nAttibuteNum; return ReadValue(itemInfo.m_EquipAttribute.m_usAttributeValue[Item::Attribute::EVADE], szValue);			}
bool ReadCritical(Item::ItemInfo& itemInfo, const char* szValue)		{ ++itemInfo.m_EquipAttribute.m_nAttibuteNum; return ReadValue(itemInfo.m_EquipAttribute.m_usAttributeValue[Item::Attribute::CRITICAL], szValue);		}
bool ReadMagicPower(Item::ItemInfo& itemInfo, const char* szValue)		{ ++itemInfo.m_EquipAttribute.m_nAttibuteNum; return ReadValue(itemInfo.m_EquipAttribute.m_usAttributeValue[Item::Attribute::MAGIC_POWER], szValue);	}
bool ReadMagicResist(Item::ItemInfo& itemInfo, const char* szValue)		{ ++itemInfo.m_EquipAttribute.m_nAttibuteNum; return ReadValue(itemInfo.m_EquipAttribute.m_usAttributeValue[Item::Attribute::MAGIC_RESIST], szValue);	}
bool ReadCoolDown(Item::ItemInfo& itemInfo, const char* szValue)		{ ++itemInfo.m_EquipAttribute.m_nAttibuteNum; return ReadValue(itemInfo.m_EquipAttribute.m_usAttributeValue[Item::Attribute::COOLDOWN], szValue);		}
bool ReadMaxHP(Item::ItemInfo& itemInfo, const char* szValue)			{ ++itemInfo.m_EquipAttribute.m_nAttibuteNum; return ReadValue(itemInfo.m_EquipAttribute.m_usAttributeValue[Item::Attribute::MAX_HP], szValue);			}
bool ReadMaxMP(Item::ItemInfo& itemInfo, const char* szValue)			{ ++itemInfo.m_EquipAttribute.m_nAttibuteNum; return ReadValue(itemInfo.m_EquipAttribute.m_usAttributeValue[Item::Attribute::MAX_MP], szValue);			}
bool ReadHPRegen(Item::ItemInfo& itemInfo, const char* szValue)			{ ++itemInfo.m_EquipAttribute.m_nAttibuteNum; return ReadValue(itemInfo.m_EquipAttribute.m_usAttributeValue[Item::Attribute::HP_REGEN], szValue);		}
bool ReadMPRegen(Item::ItemInfo& itemInfo, const char* szValue)			{ ++itemInfo.m_EquipAttribute.m_nAttibuteNum; return ReadValue(itemInfo.m_EquipAttribute.m_usAttributeValue[Item::Attribute::MP_REGEN], szValue);		}
bool ReadSpeed(Item::ItemInfo& itemInfo, const char* szValue)			{ ++itemInfo.m_EquipAttribute.m_nAttibuteNum; return ReadValue(itemInfo.m_EquipAttribute.m_usAttributeValue[Item::Attribute::SPEED], szValue);			}
bool ReadSkillPoint(Item::ItemInfo& itemInfo, const char* szValue)		{ ++itemInfo.m_EquipAttribute.m_nAttibuteNum; return ReadValue(itemInfo.m_EquipAttribute.m_usAttributeValue[Item::Attribute::SKILL_POINT], szValue);	}
bool ReadFrost(Item::ItemInfo& itemInfo, const char* szValue)			{ ++itemInfo.m_EquipAttribute.m_nAttibuteNum; return ReadValue(itemInfo.m_EquipAttribute.m_usAttributeValue[Item::Attribute::FROST], szValue);			}
bool ReadFire(Item::ItemInfo& itemInfo, const char* szValue)			{ ++itemInfo.m_EquipAttribute.m_nAttibuteNum; return ReadValue(itemInfo.m_EquipAttribute.m_usAttributeValue[Item::Attribute::FIRE], szValue);			}
bool ReadElectro(Item::ItemInfo& itemInfo, const char* szValue)			{ ++itemInfo.m_EquipAttribute.m_nAttibuteNum; return ReadValue(itemInfo.m_EquipAttribute.m_usAttributeValue[Item::Attribute::ELECTRO], szValue);		}
bool ReadDarkness(Item::ItemInfo& itemInfo, const char* szValue)		{ ++itemInfo.m_EquipAttribute.m_nAttibuteNum; return ReadValue(itemInfo.m_EquipAttribute.m_usAttributeValue[Item::Attribute::DARKNESS], szValue);		}

bool ItemDataParser::SetAccessoryData(ParseDataArray& parseDataArray)
{
	parseDataArray.push_back(ParseData("MinDamage",             ReadMinDamage));
	parseDataArray.push_back(ParseData("MaxDamage",             ReadMaxDamage));
	parseDataArray.push_back(ParseData("Defence",				ReadArmor));
	parseDataArray.push_back(ParseData("Block",					ReadBlock));
	parseDataArray.push_back(ParseData("DropOption",			ReadDropOption));
	parseDataArray.push_back(ParseData("Grade",					ReadGrade));
	parseDataArray.push_back(ParseData("CriticalType",			ReadCritical));
	parseDataArray.push_back(ParseData("AttackRange",			ReadHitRate));
	parseDataArray.push_back(ParseData("MaxSocketNum",			ReadMagicPower));
	parseDataArray.push_back(ParseData("MagicResist",			ReadMagicResist));
	parseDataArray.push_back(ParseData("Evade",					ReadEvade));
	parseDataArray.push_back(ParseData("CoolDown",				ReadCoolDown));
    parseDataArray.push_back(ParseData("MaxHP",					ReadMaxHP));
	parseDataArray.push_back(ParseData("MaxMP",					ReadMaxMP));
	parseDataArray.push_back(ParseData("HPRegen",				ReadHPRegen));
	parseDataArray.push_back(ParseData("MPRegen",				ReadMPRegen));
	parseDataArray.push_back(ParseData("Speed",					ReadSpeed));
	parseDataArray.push_back(ParseData("SkillPoint",			ReadSkillPoint));
	parseDataArray.push_back(ParseData("Frost",					ReadFrost));
	parseDataArray.push_back(ParseData("Fire",					ReadFire));
	parseDataArray.push_back(ParseData("Electro",				ReadElectro));
	parseDataArray.push_back(ParseData("Darkness",				ReadDarkness));

	return true;
}

bool ItemDataParser::SetAvataData(ParseDataArray& parseDataArray)
{
	parseDataArray.push_back(ParseData("DropOption",			ReadDropOption));
	parseDataArray.push_back(ParseData("Grade",					ReadGrade));
	parseDataArray.push_back(ParseData("Speed",					ReadSpeed));

	return true;
}
bool ReadForStatueWar(Item::ItemInfo& itemInfo, const char* szValue)	{ return ReadStringToBool(itemInfo.m_UseItemInfo.m_bForStatueWar, szValue);				}
bool ReadLimitRealmPoint(Item::ItemInfo& itemInfo, const char* szValue)	{ return ReadValue(itemInfo.m_UseItemInfo.m_cLimitRealmPoint, szValue);					}
bool ReadSkill_ID(Item::ItemInfo& itemInfo, const char* szValue)        { itemInfo.m_UseItemInfo.m_usSkill_ID = Math::Convert::Atos(szValue); return true;		}
bool ReadSkill_Level(Item::ItemInfo& itemInfo, const char* szValue)     { return ReadValue(itemInfo.m_UseItemInfo.m_usSkill_LockCount, szValue);				}
bool ReadZone(Item::ItemInfo& itemInfo, const char* szValue)			{ return ReadValue(itemInfo.m_UseItemInfo.m_cZone, szValue);							}
bool ReadPositionX(Item::ItemInfo& itemInfo, const char* szValue)		{ return ReadValue(itemInfo.m_UseItemInfo.m_Pos.m_fPointX, szValue); 					}
bool ReadPositionY(Item::ItemInfo& itemInfo, const char* szValue)		{ return ReadValue(itemInfo.m_UseItemInfo.m_Pos.m_fPointY, szValue); 					}
bool ReadPositionZ(Item::ItemInfo& itemInfo, const char* szValue)		{ return ReadValue(itemInfo.m_UseItemInfo.m_Pos.m_fPointZ, szValue); 					}
bool ReadAmount(Item::ItemInfo& itemInfo, const char* szValue)			{ return ReadValue(itemInfo.m_UseItemInfo.m_dwAmount, szValue);							}
bool ReadQuestID(Item::ItemInfo& itemInfo, const char* szValue)			{ itemInfo.m_UseItemInfo.m_wQuestID = Math::Convert::Atos(szValue); return true;		}
bool ReadItemDescribe(Item::ItemInfo& itemInfo, const char* szValue)    { return ReadString(itemInfo.m_StringData.m_szItemDescribe, Item::StringData::MAX_ITEM_DESCRIBE, szValue);	}

bool ItemDataParser::SetUseItemData(ParseDataArray& parseDataArray)
{
	parseDataArray.push_back(ParseData("MinDamage",		ReadForStatueWar));
	parseDataArray.push_back(ParseData("MaxDamage",		ReadLimitRealmPoint));
	parseDataArray.push_back(ParseData("Defence",		ReadSkill_ID));
    parseDataArray.push_back(ParseData("Block",			ReadSkill_Level));
	parseDataArray.push_back(ParseData("DropOption",	ReadZone));
	parseDataArray.push_back(ParseData("Grade",			ReadPositionX));
	parseDataArray.push_back(ParseData("CriticalType",	ReadPositionY));
	parseDataArray.push_back(ParseData("AttackRange",	ReadPositionZ));
	parseDataArray.push_back(ParseData("MaxSocketNum",	ReadAmount));
	parseDataArray.push_back(ParseData("MagicResist",	ReadQuestID));
    parseDataArray.push_back(ParseData("Evade",			ReadItemDescribe));

	return true;
}

bool ItemDataParser::SetEtcItemData(ParseDataArray& parseDataArray)
{
    parseDataArray.push_back(ParseData("MinDamage",		ReadItemDescribe));

    return true;
}



#ifndef _ITEM_DATA_PARSER_H_
#define _ITEM_DATA_PARSER_H_

#include <vector>

// 전방 참조
namespace Item
{
    struct ItemInfo;
};

namespace ItemDataParser
{
    struct ParseData
    {
        typedef bool(*ParseFunc) (Item::ItemInfo& itemInfo, const char* szValue);

        const char* m_szColumnName;
        ParseFunc   m_fnParseFunc;

        ParseData(const char* szColumnName, ParseFunc fnParseFunc)
            :   m_szColumnName(szColumnName), m_fnParseFunc(fnParseFunc) { }
    };

    typedef std::vector<ParseData>  ParseDataArray;

    bool SetDefaultData(ParseDataArray& parseDataArray);
    bool SetEquipData(ParseDataArray& parseDataArray);
	bool SetAccessoryData(ParseDataArray& parseDataArray);	//--//
	bool SetAvataData(ParseDataArray& parseDataArray);
    bool SetUseItemData(ParseDataArray& parseDataArray);
    bool SetEtcItemData(ParseDataArray& parseDataArray);
};


#endif
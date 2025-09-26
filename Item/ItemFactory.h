#ifndef _RYL_GAME_ITEM_FACTORY_H_
#define _RYL_GAME_ITEM_FACTORY_H_

#include <map>

#define DELETE_ITEM(p)  if(p) { Item::CItemFactory::GetInstance().DeleteItem(p); (p)=0; }

namespace Item
{
    // 전방 참조
    class   CItem;
    struct  ItemInfo;

    class CItemFactory
    {
    public:

        static CItemFactory& GetInstance();
        
        unsigned __int64 GetItemUID() { return m_nCurrentUID; }
	    void SetItemUID(unsigned __int64 nCurrentUID) { m_nCurrentUID = nCurrentUID; }

        // Desc : 아이템 시리얼이 0인 임시 아이템을 생성한다. - 아이템 시리얼을 매기지 않는다.
        CItem* CreateTempItem(const ItemInfo& itemInfo);

        // Desc : 종류로 아이템 생성(기본 능력치) - 아이템 시리얼을 매긴다.
        CItem* CreateItem(const ItemInfo& itemInfo);

        // Desc : 종류 ID로 아이템 생성(기본 능력치) - 아이템 시리얼을 매긴다.
        CItem* CreateItem(unsigned short usProtoTypeID);

        // Desc : SerializeOut된 버퍼로 아이템 생성.
        // In   : 버퍼, 버퍼 크기
        // Out  : (Return)성공 여부, nParseLength_InOut - 사용한 버퍼 크기
        CItem* CreateItem(const char* lpSerializedItem_In, size_t& nParseLength_InOut);

        // 아이템을 삭제한다.
        void DeleteItem(CItem* lpDeleteItem);

    private:

        CItemFactory();
        ~CItemFactory();

        unsigned __int64    m_nCurrentUID;
    };


};

#endif
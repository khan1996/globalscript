#include "stdafx.h"
#include "Item.h"
#include "ItemMgr.h"
#include "ItemFactory.h"

#include <Log/ServerLog.h>
#include "GMMemory.h"

namespace Item
{



CItemFactory& CItemFactory::GetInstance()
{
    static CItemFactory itemFactory;
    return itemFactory;
}

CItemFactory::CItemFactory()
:   m_nCurrentUID(0LL)
{


}

CItemFactory::~CItemFactory()
{


}

CItem* CItemFactory::CreateItem(const ItemInfo& itemInfo)
{
    CItem* lpItem = CreateTempItem(itemInfo);

    if(0 != lpItem)
    {
        lpItem->SetUID(m_nCurrentUID++);
    }

    return lpItem;
}


// Desc     : 아이템 시리얼이 0인 임시 아이템을 생성한다.
CItem* CItemFactory::CreateTempItem(const ItemInfo& itemInfo)
{
    unsigned long dwFlags = itemInfo.m_DetailData.m_dwFlags;

    CItem* lpItem = 0;

    if(DetailData::EQUIP == (dwFlags & DetailData::EQUIP))
    {
        lpItem = new CEquipment(itemInfo);            

		//--//	start..
		if (0 == itemInfo.m_DetailData.m_wDropOption)
		{
			Item::CEquipment* lpEquip = Item::CEquipment::DowncastToEquipment(lpItem);
			if (NULL != lpEquip)
			{
				lpEquip->m_GradeInfo.m_eItemGrade = static_cast<EquipType::Grade>(itemInfo.m_DetailData.m_cGrade);
			}
		}
		//--//	end..
    }
    else if(DetailData::USE_ITEM == (dwFlags & DetailData::USE_ITEM))
    {
        lpItem = new CUseItem(itemInfo);            
    }
    else
    {
        lpItem = new CItem(itemInfo);            
    }

    return lpItem;
}



CItem* CItemFactory::CreateItem(unsigned short usProtoTypeID)
{
    const ItemInfo* lpItemInfo = CItemMgr::GetInstance().GetItemInfo(usProtoTypeID);
   
    if(0 != lpItemInfo)
    {
        return CreateItem(*lpItemInfo);
    }

    ERRLOG1(g_Log, "ProtoTypeID:%u / 아이템 생성 실패 : 스크립트에 없는 종류ID입니다", usProtoTypeID);
    return 0;
}


CItem* CItemFactory::CreateItem(const char* lpSerializedItem_In, size_t& nParseLength_InOut)
{
    const ItemData* lpItemData = reinterpret_cast<const ItemData*>(lpSerializedItem_In);
    const ItemInfo* lpItemInfo = 0;

    if(0 != lpItemData && 
        0 != (lpItemInfo = CItemMgr::GetInstance().GetItemInfo(lpItemData->m_usProtoTypeID)))
    {
        // serialize시 itemUID가 들어가므로, itemUID가 생성된 아이템을 만들 필요는 없다.
        CItem* lpItem = CreateTempItem(*lpItemInfo);

        if(0 != lpItem)
        {
            if(!lpItem->SerializeIn(lpSerializedItem_In, nParseLength_InOut))
            {
                DELETE_ITEM(lpItem);
            }
        }

        return lpItem;
    }

    return 0;
}


void CItemFactory::DeleteItem(Item::CItem* lpDeleteItem)
{
    delete lpDeleteItem;
}



}
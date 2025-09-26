#ifndef _GAMA_CLIENT_SEND_ITEM_H_
#define _GAMA_CLIENT_SEND_ITEM_H_

// 전방 참조
struct TakeType;
struct CastObject;

class ClientSocket;

namespace ClientNet
{
	class CClientEventHandler;
}

namespace Item
{
	class CItem;
	struct ItemPos;
}

namespace SendPacket
{
    bool CharPickUp(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID, unsigned __int64 nObjectID, Item::ItemPos Index);
    bool CharPullDown(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID, Item::ItemPos Index, unsigned char cNum);
    bool CharTakeItem(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID, TakeType TakeType);			
    bool CharSwapItem(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID, TakeType SrcType, TakeType DstType);
	bool CharTradeItem(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID, unsigned long dwNPCID, unsigned char cCmd, unsigned short wItemID, TakeType TakeType, Item::ItemPos CouponPos, Item::CItem* lpItem);
	bool CharEquipShopInfo(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID, unsigned long dwNPCID, unsigned char cRace, unsigned char cTabPage);
    bool CharRepairItem(ClientNet::CClientEventHandler* lpHandler, unsigned long dwNPCID, Item::ItemPos* lpIndex);
	bool CharRepairAllItem(ClientNet::CClientEventHandler* lpHandler, unsigned long dwNPCID);
    bool CharUseItem(ClientNet::CClientEventHandler* lpHandler, unsigned long dwSenderID, unsigned long dwRecver, Item::ItemPos* lpIndex);
    bool CharUseCashItem(ClientNet::CClientEventHandler* lpHandler, unsigned long dwSenderID, unsigned long dwRecver, Item::ItemPos* lpIndex);

    bool CharCastObject(ClientNet::CClientEventHandler* lpHandler, unsigned long dwSenderID, unsigned long dwRecverID, CastObject &CastObject);
    bool CharInstallSocket(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID, Item::ItemPos* lpEquipIndex, Item::ItemPos* lpSocket, unsigned char PickingNum);
	bool CharRuneInstallSocket(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID, unsigned char ucRunePos, Item::ItemPos* lpEquipIndex, Item::ItemPos* lpSocket);
	bool CharRuneRemoveSocket(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID, unsigned char ucRunePos, Item::ItemPos* lpEquipIndex, Item::ItemPos* lpSocket);
	bool CharItemChemical(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID, Item::ItemPos* lpPickkingPos, Item::ItemPos* lpTargetPos, unsigned char cPickkingNum);
    bool CharUpgradeItem(ClientNet::CClientEventHandler* lpHandler, unsigned long dwNPCID);
	bool CharItemOptionGraft(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID, unsigned char cAttributeType);
	bool CharItemCompensation(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID);
    bool CharSplitItem(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID, TakeType &TakeType);
    bool CharAutoRouting(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID, unsigned __int64 nObjectID, unsigned short wItemID, Item::ItemPos Index);

    bool StoreLogin(ClientNet::CClientEventHandler* lpHandler, char *Password, char PassSave); 
    bool StoreLogout(ClientNet::CClientEventHandler* lpHandler); 
    bool StoreBuyTab(ClientNet::CClientEventHandler* lpHandler, char TabNum);
    bool StoreChangePass(ClientNet::CClientEventHandler* lpHandler, char *Password, char *NewPassword);

    bool CharStallOpen(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID, char* StallName);
    bool CharStallRegisterItem(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID, unsigned long dwShopID, TakeType TakeType, unsigned long dwPrice, unsigned char cCmd);
    bool CharStallEnter(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCustomerID, unsigned long dwOwner);

    bool CharTakeItems(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID, unsigned char cTakeNum, TakeType* lpTakeTypes); 
    bool CharTakeGold(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID, unsigned char cSrcPos, unsigned char cDstPos, unsigned long dwGold);
    bool CharExchangeCmd(ClientNet::CClientEventHandler* lpHandler, unsigned long dwSenderID, unsigned long dwRecverID, unsigned char cCmd);
    bool CharQuickSlotMove(ClientNet::CClientEventHandler* lpHandler, TakeType &TakeType, unsigned short usSkillID, unsigned char cLockCount = 0, unsigned char cSkillLevel = 0);

    bool CharCastObjectInfo(ClientNet::CClientEventHandler* lpHandler, unsigned long dwSenderID, 
        unsigned long dwRecverID, CastObject &CastObject);
};


#endif
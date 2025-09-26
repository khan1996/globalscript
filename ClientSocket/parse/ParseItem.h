#ifndef _PARSE_ITEM_PACKET_H_
#define _PARSE_ITEM_PACKET_H_

#include <Network/Packet/PacketStruct/CharItemPacket.h>

namespace ParsePacket
{
    // 집기
    unsigned long HandleCharPickUp(PktBase* lpPktBase, unsigned long* dwCharID, unsigned __int64* nObjectID, Item::ItemPos* lpIndex, unsigned long* dwGold, unsigned long* dwSize, unsigned char* cNum, char* *lppBuffer);
    unsigned long HandleCharPickUpInfo(PktBase* lpPktBase, unsigned long* dwCharID, unsigned __int64* nObjectID);
    unsigned long HandleCharAutoRouting(PktBase* lpPktBase, unsigned long* dwCharID, unsigned __int64* nObjectID, unsigned short* wItemID, unsigned char* cNum, unsigned char* cCmd);
    unsigned long HandleCharDisappearItem(PktBase* lpPktBase, unsigned long* dwCharID, Item::ItemPos* lpIndex, unsigned char* cNum, unsigned char* cCmd);
    
    // 버리기
    unsigned long HandleCharPullDown(PktBase* lpPktBase, unsigned long* dwCharID, Item::ItemPos* lpIndex, FieldObject* lpFieldObject);
    unsigned long HandleCharPullDownInfo(PktBase* lpPktBase, unsigned long* dwCharID, FieldObject* lpFieldObject);
    
    // 이동/나누기
    unsigned long HandleCharTakeItem(PktBase* lpPktBase, unsigned long* dwCharID, TakeType* lpTakeType);
    unsigned long HandleCharSwapItem(PktBase* lpPktBase, unsigned long* dwCharID, TakeType* lpSrcType, TakeType* lpDstType);
    unsigned long HandleCharSplitItem(PktBase* lpPktBase, unsigned long* dwCharID, TakeType* lpTakeType, unsigned char* Size, char* *lppBuffer);

    // 사기/팔기/수리
	unsigned long HandleCharTradeItem(PktBase* lpPktBase, unsigned long* dwCharID, unsigned long* dwNPCID, unsigned long* dwGold, unsigned long* dwMileage, Item::ItemPos* CouponPos, Item::ItemPos* lpIndex, unsigned char* Num, unsigned short* Size, char* *lppBuffer);
	unsigned long HandleCharEquipShopInfo(PktBase* lpPktBase, unsigned long* dwCharID, unsigned long* dwNPCID, unsigned long* dwTime, unsigned char* cRace, unsigned char* cTabPage, unsigned char* cNum, unsigned short* aryItemID, unsigned char* aryItemGrade);
    unsigned long HandleCharRepairItem(PktBase* lpPktBase, unsigned long* dwCharID, Item::ItemPos* lpIndex, unsigned long* dwGold);
	unsigned long HandleCharRepairAllItem(PktBase* lpPktBase, unsigned long* dwCharID, unsigned long* dwGold);

    // 던지기
    unsigned long HandleCharCastObject(PktBase* lpPktBase, unsigned long* dwSenderID, unsigned long* dwRecverID, CastObject* lpCastObject);
    unsigned long HandleCharCastObjectInfo(PktBase* lpPktBase, unsigned long* dwSenderID, unsigned long* dwRecverID, CastObject* lpCastObject);

    // 사용/업그레이드
    unsigned long HandleCharUseItem(PktBase* lpPktBase, unsigned long& dwSenderID, unsigned long& dwRecver, Item::ItemPos& index, unsigned char& cRemainItemNum);
    unsigned long HandleCharInstallSocket(PktBase* lpPktBase, unsigned long* dwCharID, Item::ItemPos* lpEquipIndex, Item::ItemPos* lpSocket, unsigned char* Size, char* *lppBuffer, unsigned char *PickingNum); //dedi
	unsigned long HandleCharInstallRune(PktBase* lpPktBase, unsigned long* dwCharID, Item::ItemPos* lpEquipIndex, Item::ItemPos* lpSocket, unsigned char* runePos, unsigned char* type, unsigned char* Size, char* *lppBuffer);

	unsigned long HandleCharItemChemical(PktBase* lpPktBase, unsigned long* dwCharID, Item::ItemPos* lpPickkingPos, Item::ItemPos* lpTargetPos, unsigned short* wPickkingID, unsigned char* cPickkingNum, unsigned char* cSize, char* *lppBuffer);
    unsigned long HandleCharUpgradeItem(PktBase* lpPktBase, unsigned long* dwCharID, unsigned long* dwCurrentGold, unsigned char* cCurrentMineralNum, unsigned char *cValue, unsigned char* Size, char* *lppBuffer);
	unsigned long HandleCharItemOptionGraft(PktBase* lpPktBase, unsigned long* dwCharID, unsigned long* dwCurrentGold, unsigned char* cItemSize, char* *lppItemBuffer);
	unsigned long HandleCharItemCompensation(PktBase* lpPktBase, unsigned long* dwCharID);

    // 창고
    unsigned long HandleDeposit(PktBase* lpPktBase, unsigned char* Cmd, unsigned long* dwGold, char* PassSave);
    unsigned long HandleDepositUpdate(PktBase* lpPktBase, unsigned long* dwFlag, unsigned char* TabNum, bool* Complete, unsigned short* StoreSize, char* *lppStoreBuff);  

    // 교환
    unsigned long HandleCharExchangeCmd(PktBase* lpPktBase, unsigned long* dwSenderID, unsigned long* dwRecverID, unsigned char* Cmd);
    unsigned long HandleCharExchangeItem(PktBase* lpPktBase, unsigned long* dwCharID, unsigned long* dwSize, Item::ItemPos* lpIndex, unsigned char* Type, unsigned char* Num, char* *lppBuffer);

    // 노점상
    unsigned long HandleCharStallOpen(PktBase* lpPktBase, unsigned long* dwCharID, char* StallName);
    unsigned long HandleCharStallRegisterItem(PktBase* lpPktBase, unsigned long* dwCharID, unsigned long* dwShopID, TakeType* TakeType, unsigned long* dwPrice, unsigned char* Cmd, char* BuyUser);
    unsigned long HandleCharStallEnter(PktBase* lpPktBase, unsigned long* dwCustomerID, unsigned long* dwOwner);
    unsigned long HandleCharStallItemInfo(PktBase* lpPktBase, unsigned long* dwOwnerID, unsigned long* dwCustomerID, unsigned char* cTax, unsigned long* dwStallSize, char **lppStallBuff, unsigned char* ItemNum, unsigned long **dwStallPrice);

    // 기타
    unsigned long HandleCharTakeItems(PktBase* lpPktBase, unsigned long* dwCharID, unsigned char* TakeNum, TakeType** lppTakeTypes);
    unsigned long HandleCharTakeGold(PktBase* lpPktBase, unsigned long* dwCharID, unsigned char* SrcPos, unsigned char* DstPos, unsigned long* dwGold);
};

#endif
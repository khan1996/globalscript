#include "stdafx.h"
#include "ParseItem.h"

#include <Network/Packet/PacketStruct/CharCommunityPacket.h>
#include "GMMemory.h"

// 매개변수 설명은 패킷 주석을 보시오.

unsigned long ParsePacket::HandleCharPickUp(PktBase* lpPktBase, unsigned long *dwCharID, unsigned __int64 *nObjectID, 
                                            Item::ItemPos* lpIndex, unsigned long *dwGold, unsigned long *dwSize,
                                            unsigned char* cNum, char **lppBuffer)
{
    PktPUAck* lpPktPUAck = static_cast<PktPUAck *>(lpPktBase);

    *dwCharID	= lpPktPUAck->m_dwCharID;
    *nObjectID   = lpPktPUAck->m_nObjectID;

    *lpIndex	= lpPktPUAck->m_itemPos;
    *cNum       = lpPktPUAck->m_cNum;

    switch (lpPktPUAck->m_cType)
    {
		case PktPUAck::Item:
			*dwGold		= 0;
			*dwSize		= lpPktPUAck->m_dwSize;
			*lppBuffer	= reinterpret_cast<char *>(lpPktPUAck + 1);
			break;

		case PktPUAck::Gold:
			*dwGold		= lpPktPUAck->m_dwSize;
			*dwSize		= 0;
			*lppBuffer	= NULL;
			break;
    }

    return lpPktPUAck->GetError();
}


unsigned long ParsePacket::HandleCharPickUpInfo(PktBase* lpPktBase, unsigned long *dwCharID,
                                                 unsigned __int64 *nObjectID)
{
    PktPUInfo* lpPktPUInfo = static_cast<PktPUInfo *>(lpPktBase);

    *dwCharID		= lpPktPUInfo->m_dwCharID;
    *nObjectID		= lpPktPUInfo->m_nObjectID;

    return lpPktPUInfo->GetError();
}


unsigned long ParsePacket::HandleCharPullDown(PktBase* lpPktBase, unsigned long *dwCharID, 
                                              Item::ItemPos* lpIndex, FieldObject* lpFieldObject)
{
    PktPDAck* lpPktPDAck = static_cast<PktPDAck *>(lpPktBase);
     
    *dwCharID		= lpPktPDAck->m_dwCharID;
    *lpFieldObject	= lpPktPDAck->m_FieldObject;
    *lpIndex		= lpPktPDAck->m_itemPos;	

    return lpPktPDAck->GetError();
}


unsigned long ParsePacket::HandleCharPullDownInfo(PktBase* lpPktBase, unsigned long *dwCharID, 
                                                  FieldObject* lpFieldObject)
{
    PktPDInfo*  lpPktPDInfo = static_cast<PktPDInfo *>(lpPktBase);
    
    *dwCharID		= lpPktPDInfo->m_dwCharID;
    *lpFieldObject	= lpPktPDInfo->m_FieldObject;

    return lpPktPDInfo->GetError();
}


unsigned long ParsePacket::HandleCharTakeItem(PktBase* lpPktBase, 
                                              unsigned long *dwCharID, TakeType* lpTakeType)
{
    PktTI* lpPktTI = static_cast<PktTI *>(lpPktBase);
    
    *dwCharID		= lpPktTI->m_dwCharID;
    *lpTakeType		= lpPktTI->m_TakeType;

    return lpPktTI->GetError();
}


unsigned long ParsePacket::HandleCharSwapItem(PktBase* lpPktBase, unsigned long *dwCharID, 
                                              TakeType* lpSrcType, TakeType* lpDstType)
{
    PktSwI* lpPktSwI = static_cast<PktSwI *>(lpPktBase);

    *dwCharID		= lpPktSwI->m_dwCharID;
    *lpSrcType		= lpPktSwI->m_SwapSrc;
    *lpDstType		= lpPktSwI->m_SwapDst;

    return lpPktSwI->GetError();
}


unsigned long ParsePacket::HandleCharTradeItem(PktBase* lpPktBase, unsigned long *dwCharID, unsigned long *dwNPCID, 
                                               unsigned long *dwGold, unsigned long *dwMileage, Item::ItemPos* CouponPos, 
                                               Item::ItemPos* lpIndex, unsigned char *Num,
                                               unsigned short *Size, char **lppBuffer)
{
    PktTrAck* lpPktTrAck = static_cast<PktTrAck *>(lpPktBase);

    *dwCharID		= lpPktTrAck->m_dwCharID;
    *dwNPCID		= lpPktTrAck->m_dwNPCID;		

    *dwGold			= lpPktTrAck->m_dwGold;
    *dwMileage		= lpPktTrAck->m_dwMileage;
	*CouponPos		= lpPktTrAck->m_CouponPos;

    *lpIndex		= lpPktTrAck->m_itemPos;
    *Num			= lpPktTrAck->m_cNum;

    if (0 == lpPktTrAck->m_wSize)
    {
        *Size		= 0;
        *lppBuffer	= NULL;
    }
    else
    {
        *Size		= lpPktTrAck->m_wSize;
        *lppBuffer	= reinterpret_cast<char *>(lpPktTrAck + 1);
    }

    return lpPktTrAck->GetError();
}


unsigned long ParsePacket::HandleCharEquipShopInfo(PktBase* lpPktBase, unsigned long *dwCharID, unsigned long *dwNPCID, 
												   unsigned long *dwTime, unsigned char *cRace, unsigned char *cTabPage, 
												   unsigned char *cNum, unsigned short *aryItemID, unsigned char *aryItemGrade)
{
	PktEquipShopInfo* lpESIAck = static_cast<PktEquipShopInfo *>(lpPktBase);

	*dwCharID	= lpESIAck->m_dwCharID;
	*dwNPCID	= lpESIAck->m_dwNPCID;

	*dwTime		= lpESIAck->m_dwTime;

	*cRace		= lpESIAck->m_cRace;
	*cTabPage	= lpESIAck->m_cTabPage;

	*cNum		= lpESIAck->m_cNum;

	unsigned short* lpItemID = reinterpret_cast<unsigned short *>(lpESIAck + 1);
	for (unsigned char cIDIndex = 0; cIDIndex < *cNum; ++cIDIndex)
	{
		aryItemID[cIDIndex] = *lpItemID;
		++lpItemID;
	}

	unsigned char* lpItemGrade = reinterpret_cast<unsigned char *>(lpItemID);
	for (unsigned char cGradeIndex = 0; cGradeIndex < *cNum; ++cGradeIndex)
	{
		aryItemGrade[cGradeIndex] = *lpItemGrade;
		++lpItemGrade;
	}

	return lpESIAck->GetError();
}


unsigned long ParsePacket::HandleCharRepairItem(PktBase* lpPktBase, unsigned long *dwCharID, 
                                                Item::ItemPos* lpIndex, unsigned long *dwGold)
{
    PktRpI* lpPktRpI = static_cast<PktRpI *>(lpPktBase);
	
	*dwCharID		= lpPktRpI->m_dwCharID;
	*dwGold			= lpPktRpI->m_dwGold;
	*lpIndex		= lpPktRpI->m_itemPos;

	return lpPktRpI->GetError();
}

unsigned long ParsePacket::HandleCharRepairAllItem(PktBase* lpPktBase, unsigned long *dwCharID, unsigned long *dwGold)
{
	PktRpAI* lpPktRpAI = static_cast<PktRpAI *>(lpPktBase);

	*dwCharID		= lpPktRpAI->m_dwCharID;
	*dwGold			= lpPktRpAI->m_dwGold;

	return lpPktRpAI->GetError();
}


unsigned long ParsePacket::HandleCharUseItem(PktBase* lpPktBase, 
                                             unsigned long& dwSenderID, 
                                             unsigned long& dwRecver, Item::ItemPos& index,
                                             unsigned char& cRemainItemNum)
{
    PktUI* lpPktUI = static_cast<PktUI *>(lpPktBase);

	dwSenderID		= lpPktUI->m_dwSender;
	dwRecver		= lpPktUI->m_dwRecver;
	index		    = lpPktUI->m_itemPos;
    cRemainItemNum  = lpPktUI->m_cRemainItemNum;
	
	return lpPktUI->GetError();
}


unsigned long ParsePacket::HandleCharCastObject(PktBase* lpPktBase, unsigned long *dwSenderID, 
                                                unsigned long *dwRecverID, CastObject* lpCastObject)
{
	PktCO* lpPktCO = static_cast<PktCO *>(lpPktBase);

	*dwSenderID		= lpPktCO->m_dwSenderID;
	*dwRecverID		= lpPktCO->m_dwReceiverID;
	*lpCastObject	= lpPktCO->m_sCastObject;
	
	return lpPktCO->GetError();
}


unsigned long ParsePacket::HandleCharCastObjectInfo(PktBase* lpPktBase, unsigned long *dwSenderID, 
                                                    unsigned long *dwRecverID, CastObject* lpCastObject)
{
    PktCOInfo* lpPktCOInfo = static_cast<PktCOInfo *>(lpPktBase);

    *dwSenderID		= lpPktCOInfo->m_dwSenderID;
	*dwRecverID		= lpPktCOInfo->m_dwReceiverID;
	*lpCastObject	= lpPktCOInfo->m_sCastObject;
	
	return lpPktCOInfo->GetError();
}


unsigned long ParsePacket::HandleCharInstallSocket(PktBase* lpPktBase, unsigned long *dwCharID, 
                                                   Item::ItemPos* lpEquipIndex, 
                                                   Item::ItemPos* lpSocket, 
                                                   unsigned char *Size, char **lppBuffer , unsigned char *PickingNum)
{
    PktISAck* lpPktISAck = static_cast<PktISAck *>(lpPktBase);

	*dwCharID		= lpPktISAck->m_dwCharID;	
	*lpEquipIndex	= lpPktISAck->m_equipPos;	
	*lpSocket		= lpPktISAck->m_gemPos;
	*PickingNum		= lpPktISAck->m_PickingNum;
	
	*Size			= lpPktISAck->m_cSize;
    *lppBuffer      = (lpPktISAck->m_cSize > 0) ? reinterpret_cast<char *>(lpPktISAck + 1) : 0;

	return lpPktISAck->GetError();
}

unsigned long ParsePacket::HandleCharInstallRune(PktBase* lpPktBase, unsigned long *dwCharID, 
                                                   Item::ItemPos* lpEquipIndex, 
                                                   Item::ItemPos* lpSocket, unsigned char* runePos, unsigned char* type, 
                                                   unsigned char *Size, char **lppBuffer)
{
    PktIRSAck* lpPktIRSAck = static_cast<PktIRSAck *>(lpPktBase);

	*dwCharID		= lpPktIRSAck->m_dwCharID;	
	*lpEquipIndex	= lpPktIRSAck->m_equipPos;	
	*lpSocket		= lpPktIRSAck->m_runePos;
	
	*Size			= lpPktIRSAck->m_cSize;
	//*runePos		= lpPktIRSAck->m_cRuneSocketPos;
	*type			= lpPktIRSAck->m_cType;
    *lppBuffer      = (lpPktIRSAck->m_cSize > 0) ? reinterpret_cast<char *>(lpPktIRSAck + 1) : 0;

	return lpPktIRSAck->GetError();
}


unsigned long ParsePacket::HandleCharItemChemical(PktBase* lpPktBase, unsigned long *dwCharID, 
												  Item::ItemPos* lpPickkingPos, Item::ItemPos* lpTargetPos, 
												  unsigned short *wPickkingID, unsigned char *cPickkingNum, 
												  unsigned char *cSize, char **lppBuffer)
{
	PktItemChemicalAck* lpPktICAck = static_cast<PktItemChemicalAck *>(lpPktBase);

	*dwCharID		= lpPktICAck->m_dwCharID;	
	
	*lpPickkingPos	= lpPktICAck->m_pickkingPos;
	*lpTargetPos	= lpPktICAck->m_targetPos;

	*wPickkingID	= lpPktICAck->m_wPickkingID;
	*cPickkingNum	= lpPktICAck->m_cPickkingNum;

	*cSize			= lpPktICAck->m_cResultSize;
	*lppBuffer      = (lpPktICAck->m_cResultSize > 0) ? reinterpret_cast<char *>(lpPktICAck + 1) : 0;

	return lpPktICAck->GetError();
}


unsigned long ParsePacket::HandleCharSplitItem(PktBase* lpPktBase, unsigned long *dwCharID, 
                                               TakeType* lpTakeType, unsigned char *Size, char **lppBuffer)
{
    PktSpItAck* lpPktSpltAck = static_cast<PktSpItAck *>(lpPktBase);

    *dwCharID		= lpPktSpltAck->m_dwCharID;
    *lpTakeType		= lpPktSpltAck->m_TakeType;
    *Size			= lpPktSpltAck->m_cSize;
    *lppBuffer      = (lpPktSpltAck->m_cSize > 0) ? reinterpret_cast<char *>(lpPktSpltAck + 1) : 0;

    return lpPktSpltAck->GetError();
}


unsigned long ParsePacket::HandleDeposit(PktBase* lpPktBase, unsigned char *Cmd, 
                                         unsigned long *dwGold, char *PassSave)
{
	PktDeposit* lpPktDeposit = static_cast<PktDeposit *>(lpPktBase);

	*Cmd = lpPktDeposit->m_cCmd;	
	memcpy(dwGold, &lpPktDeposit->m_szData[0], sizeof(unsigned long));
	*PassSave = lpPktDeposit->m_szData[4];

	return lpPktDeposit->GetError();
}


unsigned long ParsePacket::HandleDepositUpdate(PktBase* lpPktBase, unsigned long *dwFlag, 
                                               unsigned char *TabNum, bool *Complete, 
                                               unsigned short *StoreSize, char **lppStoreBuff)  
{
    PktDepositUpdateDB* lpDepositUpdateDB = static_cast<PktDepositUpdateDB *>(lpPktBase);

    *dwFlag		    = lpDepositUpdateDB->m_dwTabFlag;
    *TabNum		    = lpDepositUpdateDB->m_cTabNum;
    *Complete	    = lpDepositUpdateDB->m_bUpdateComplete;

    *StoreSize		= lpDepositUpdateDB->m_usDataSize;
    *lppStoreBuff	= reinterpret_cast<char *>(lpDepositUpdateDB + 1);

    return lpDepositUpdateDB->GetError();
}


unsigned long ParsePacket::HandleCharUpgradeItem(PktBase* lpPktBase, unsigned long *dwCharID, 
                                                 unsigned long *dwCurrentGold, unsigned char *cCurrentMineralNum, unsigned char *cValue,
                                                 unsigned char *Size, char **lppBuffer)
{
    PktUgIAck* lpPktUgIAck = static_cast<PktUgIAck *>(lpPktBase);

    *dwCharID			= lpPktUgIAck->m_dwCharID;	
    *dwCurrentGold      = lpPktUgIAck->m_dwCurrentGold;
    *cCurrentMineralNum = lpPktUgIAck->m_cCurrentMineralNum;
    *cValue				= lpPktUgIAck->m_cValue;
    *Size	            = lpPktUgIAck->m_cSize;
    *lppBuffer          = (lpPktUgIAck->m_cSize > 0) ? reinterpret_cast<char *>(lpPktUgIAck + 1) : 0;

    return lpPktUgIAck->GetError();
}


unsigned long ParsePacket::HandleCharItemOptionGraft(PktBase* lpPktBase, unsigned long *dwCharID, 
													 unsigned long *dwCurrentGold, unsigned char *cItemSize, char **lppItemBuffer)
{
	PktItemOptionGraftAck* lpPktIOGAck = static_cast<PktItemOptionGraftAck *>(lpPktBase);

	*dwCharID			= lpPktIOGAck->m_dwCharID;	
	*dwCurrentGold      = lpPktIOGAck->m_dwGold;
	*cItemSize			= lpPktIOGAck->m_cSize;
	*lppItemBuffer      = (lpPktIOGAck->m_cSize > 0) ? reinterpret_cast<char *>(lpPktIOGAck + 1) : 0;

	return lpPktIOGAck->GetError();
}


unsigned long ParsePacket::HandleCharItemCompensation(PktBase* lpPktBase, unsigned long *dwCharID)
{
	PktItemCompensation* lpPktICAck = static_cast<PktItemCompensation *>(lpPktBase);

	*dwCharID			= lpPktICAck->m_dwCharID;

	return lpPktICAck->GetError();
}


unsigned long ParsePacket::HandleCharAutoRouting(PktBase* lpPktBase, unsigned long *dwCharID, 
                                                 unsigned __int64 *nObjectID, unsigned short *wItemID, 
                                                 unsigned char *cNum, unsigned char *cCmd)
{
	PktAutoRouting* lpPktAutoRouting = static_cast<PktAutoRouting *>(lpPktBase);

	*dwCharID	= lpPktAutoRouting->m_dwCharID;
	*nObjectID	= lpPktAutoRouting->m_nObjectID;

	*wItemID	= lpPktAutoRouting->m_wItemID;
	*cNum		= lpPktAutoRouting->m_cNum;

	*cCmd		= lpPktAutoRouting->m_cCmd;

	return lpPktAutoRouting->GetError();
}


unsigned long ParsePacket::HandleCharDisappearItem(PktBase* lpPktBase, unsigned long* dwCharID, 
                                                   Item::ItemPos* lpIndex, unsigned char* cNum, unsigned char* cCmd)
{
    PktDisappearItem* lpPktDisappearItem = static_cast<PktDisappearItem *>(lpPktBase);

    *dwCharID	= lpPktDisappearItem->m_dwCharID;
    *lpIndex	= lpPktDisappearItem->m_itemPos;
    *cNum		= lpPktDisappearItem->m_cNum;
	*cCmd		= lpPktDisappearItem->m_cCmd;

    return lpPktDisappearItem->GetError();
}


unsigned long ParsePacket::HandleCharExchangeCmd(PktBase* lpPktBase, unsigned long *dwSenderID, 
                                                 unsigned long *dwRecverID, unsigned char *Cmd)
{
	PktExC* lpExCPt = static_cast<PktExC *>(lpPktBase);

	*dwSenderID		= lpExCPt->m_dwSenderID;
	*dwRecverID		= lpExCPt->m_dwRecverID;
	*Cmd			= lpExCPt->m_cCmd;

	return lpExCPt->GetError();
}


unsigned long ParsePacket::HandleCharExchangeItem(PktBase* lpPktBase, unsigned long *dwCharID,
                                                  unsigned long *dwSize, Item::ItemPos* lpIndex, 
                                                  unsigned char *Type, unsigned char *Num, char **lppBuffer)
{
    PktExI* lpExIPt = static_cast<PktExI *>(lpPktBase);

    *dwCharID		= lpExIPt->m_dwCharID;
    *dwSize			= lpExIPt->m_dwSize;
    *lpIndex		= lpExIPt->m_itemPos;
    *Type			= lpExIPt->m_cType;
    *Num            = lpExIPt->m_cNum;

    *lppBuffer      = (0 < lpExIPt->m_dwSize) ? reinterpret_cast<char *>(lpExIPt + 1) : 0;

    return lpExIPt->GetError();
}


unsigned long ParsePacket::HandleCharStallOpen(PktBase* lpPktBase, unsigned long *dwCharID, char* StallName)
{
	PktStO* lpStOPt = static_cast<PktStO *>(lpPktBase);

	*dwCharID = lpStOPt->m_dwCharID;
	strncpy(StallName, lpStOPt->m_StallName, PktStO::MAX_STALL_NAME_LEN);

	return lpStOPt->GetError();
}


unsigned long ParsePacket::HandleCharStallRegisterItem(PktBase* lpPktBase, unsigned long *dwCharID, unsigned long *dwShopID, 
                                                       TakeType *TakeType, unsigned long *dwPrice, unsigned char *Cmd, char* BuyUser)
{
	PktStRI* lpStRIPt = static_cast<PktStRI *>(lpPktBase);

	*dwCharID	= lpStRIPt->m_dwCharID;
	*dwShopID	= lpStRIPt->m_dwShopID;
	*TakeType	= lpStRIPt->m_TakeType;
	*dwPrice	= lpStRIPt->m_dwPrice;
	*Cmd		= lpStRIPt->m_cCmd;
	strncpy(BuyUser, lpStRIPt->m_BuyUser, CHAR_INFOST::MAX_NAME_LEN);


	return lpStRIPt->GetError();
}


unsigned long ParsePacket::HandleCharStallEnter(PktBase* lpPktBase, unsigned long *dwCustomerID, unsigned long *dwOwner)
{
	PktStE* lpStEPt = static_cast<PktStE *>(lpPktBase);

    *dwCustomerID	= lpStEPt->m_dwCustomerID;
	*dwOwner		= lpStEPt->m_dwOwnerID;

	return lpStEPt->GetError();
}


unsigned long ParsePacket::HandleCharStallItemInfo(PktBase* lpPktBase, 
												   unsigned long *dwOwnerID, unsigned long *dwCustomerID, 
												   unsigned char *cTax, unsigned long *dwStallSize, char **lppStallBuff, 
                                                   unsigned char *ItemNum, unsigned long **dwStallPrice)
{
	PktStIInfo* lpPktStIInfo = static_cast<PktStIInfo *>(lpPktBase);
	
    ::memcpy(dwCustomerID, lpPktStIInfo->m_dwCustomerID, sizeof(unsigned long) * PktStIInfo::MAX_CUSTOMER_NUM);

	*dwOwnerID		= lpPktStIInfo->m_dwOwnerID;
	*cTax			= lpPktStIInfo->m_cTax;
	
    *dwStallSize	= lpPktStIInfo->m_dwItemSize;
	*lppStallBuff	= reinterpret_cast<char *>(lpPktStIInfo + 1);

	*ItemNum		= lpPktStIInfo->m_cItemNum;
	*dwStallPrice	= reinterpret_cast<unsigned long *>(*lppStallBuff + lpPktStIInfo->m_dwItemSize);

	return lpPktStIInfo->GetError();
}


unsigned long ParsePacket::HandleCharTakeItems(PktBase* lpPktBase, unsigned long *dwCharID, 
                                               unsigned char *TakeNum, TakeType** lppTakeTypes)
{
	PktTIs* lpTIsPt = static_cast<PktTIs *>(lpPktBase);

	*dwCharID		= lpTIsPt->m_dwCharID;
	*TakeNum		= lpTIsPt->m_TakeNum;
	*lppTakeTypes	= reinterpret_cast<TakeType *>(lpTIsPt + 1);

	return lpTIsPt->GetError();
}



unsigned long ParsePacket::HandleCharTakeGold(PktBase* lpPktBase, unsigned long *dwCharID, 
                                              unsigned char *SrcPos, unsigned char *DstPos, 
                                              unsigned long *dwGold)
{
    PktTG* lpTGPt = static_cast<PktTG *>(lpPktBase);

    *dwCharID		= lpTGPt->m_dwCharID;
    *SrcPos			= lpTGPt->m_cSrcPos;
    *DstPos			= lpTGPt->m_cDstPos;
    *dwGold			= lpTGPt->m_dwGold;

    return lpTGPt->GetError();
}

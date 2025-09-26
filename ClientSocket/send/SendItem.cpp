
#include "stdafx.h"

#include <Network/ClientNetwork/ClientEventHandler.h>
#include <Network/ClientSocket/ClientSocket.h>

#include <Network/Packet/PacketCommand.h>
#include <Network/Packet/PacketStruct/CharItemPacket.h>
#include <Network/Packet/PacketStruct/CharCommunityPacket.h>

#include <Item/Item.h>

#include "SendPacket.h"
#include "SendItem.h"
#include "GMMemory.h"


bool SendPacket::CharPickUp(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID, 
                            unsigned __int64 nObjectID, Item::ItemPos Index)
{
    CHECK_TRUE_RETURN(0 == dwCharID, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktPU), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);

    PktPU* lpPUPt = reinterpret_cast<PktPU *>(lpMsgBlock->wr_ptr());

    lpPUPt->m_dwCharID	= dwCharID;
    lpPUPt->m_nObjectID	= nObjectID;
    lpPUPt->m_itemPos   = Index;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktPU), CmdCharPickUp, 0, 0);
}


bool SendPacket::CharPullDown(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID, 
                              Item::ItemPos Index, unsigned char cNum)
{
    CHECK_TRUE_RETURN(0 == dwCharID, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktPD), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);
    
    PktPD* lpPDPt = reinterpret_cast<PktPD *>(lpMsgBlock->wr_ptr());

    lpPDPt->m_dwCharID		= dwCharID;	
    lpPDPt->m_itemPos		= Index;
    lpPDPt->m_cNum          = cNum;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktPD), CmdCharPullDown, 0, 0);
}


bool SendPacket::CharTakeItem(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID, TakeType TakeType)
{
    CHECK_TRUE_RETURN(0 == dwCharID, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock =
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktTI), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);
    
    PktTI* lpTIPt = reinterpret_cast<PktTI *>(lpMsgBlock->wr_ptr());

    lpTIPt->m_dwCharID		= dwCharID;
    lpTIPt->m_TakeType		= TakeType;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktTI), CmdCharTakeItem, 0, 0);
}


bool SendPacket::CharSwapItem(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID, 
                              TakeType SrcType, TakeType DstType)
{
    CHECK_TRUE_RETURN(0 == dwCharID, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktSwI), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);
    
    PktSwI* lpSwIPt = reinterpret_cast<PktSwI *>(lpMsgBlock->wr_ptr());

    lpSwIPt->m_dwCharID		= dwCharID;
    lpSwIPt->m_SwapSrc		= SrcType;
    lpSwIPt->m_SwapDst		= DstType;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktSwI), CmdCharSwapItem, 0, 0);
}


bool SendPacket::CharTradeItem(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID, 
                               unsigned long dwNPCID, unsigned char cCmd, 
                               unsigned short wItemID, TakeType TakeType, Item::ItemPos CouponPos, 
							   Item::CItem* lpItem)
{
    CHECK_TRUE_RETURN(0 == dwCharID, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
		ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktTr) + Item::MAX_ITEM_SIZE, lpHandler->GetPeerAddress());
    CHECK_NULL_RETURN(lpMsgBlock, false);
    
    PktTr* lpTrPt = reinterpret_cast<PktTr *>(lpMsgBlock->wr_ptr());

    lpTrPt->m_dwCustomerID	= dwCharID;
    lpTrPt->m_dwOwnerID		= dwNPCID;
    lpTrPt->m_cCmd			= cCmd;
    lpTrPt->m_wBuyItemID	= wItemID;
    lpTrPt->m_TakeType		= TakeType;
	lpTrPt->m_CouponPos		= CouponPos;

	size_t nItemSize = 0;
	if (NULL != lpItem)
	{
		nItemSize = Item::MAX_ITEM_SIZE;
		if (false == lpItem->SerializeOut(lpMsgBlock->wr_ptr() + sizeof(PktTr), nItemSize))
		{
			return false;
		}
	}

	lpTrPt->m_wSize = static_cast<unsigned short>(nItemSize);

	return SendPacket(lpHandler, lpMsgBlock, static_cast<unsigned short>(sizeof(PktTr) + nItemSize), 
		CmdCharTradeItem, 0, 0);
}


bool SendPacket::CharEquipShopInfo(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID, unsigned long dwNPCID, 
								   unsigned char cRace, unsigned char cTabPage)
{
	CHECK_TRUE_RETURN(0 == dwCharID, false);

	ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktEquipShopInfo), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

	PktEquipShopInfo* lpESIPt = reinterpret_cast<PktEquipShopInfo *>(lpMsgBlock->wr_ptr());

	lpESIPt->m_dwCharID		= dwCharID;
	lpESIPt->m_dwNPCID		= dwNPCID;
	
	lpESIPt->m_cRace		= cRace;
	lpESIPt->m_cTabPage		= cTabPage;

	lpESIPt->m_cNum			= 0;

	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktEquipShopInfo), CmdCharEquipShopInfo, 0, 0);
}


bool SendPacket::CharRepairItem(ClientNet::CClientEventHandler* lpHandler, unsigned long dwNPCID, Item::ItemPos* lpIndex)
{
    CHECK_TRUE_RETURN(0 == dwNPCID, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktRpI), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);

    PktRpI* lpRpIPt = reinterpret_cast<PktRpI *>(lpMsgBlock->wr_ptr());

    lpRpIPt->m_dwCharID		= dwNPCID;
    lpRpIPt->m_itemPos      = *lpIndex;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktRpI), CmdCharRepairItem, 0, 0);
}

bool SendPacket::CharRepairAllItem(ClientNet::CClientEventHandler* lpHandler, unsigned long dwNPCID)
{
	CHECK_TRUE_RETURN(0 == dwNPCID, false);

	ClientNet::CNetworkMsgBlock* lpMsgBlock =
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktRpAI), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

	PktRpAI* lpRpAIPt = reinterpret_cast<PktRpAI *>(lpMsgBlock->wr_ptr());

	lpRpAIPt->m_dwCharID	= dwNPCID;

	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktRpAI), CmdCharRepairAllItem, 0, 0);
}

bool SendPacket::CharUseItem(ClientNet::CClientEventHandler* lpHandler, unsigned long dwSenderID, 
                             unsigned long dwRecverID, Item::ItemPos* lpIndex)
{
    CHECK_TRUE_RETURN(0 == dwSenderID || 0 == dwRecverID, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktUI), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);

    PktUI* lpUIPt = reinterpret_cast<PktUI *>(lpMsgBlock->wr_ptr());

    lpUIPt->m_dwSender		    = dwSenderID;
    lpUIPt->m_dwRecver		    = dwRecverID;
    lpUIPt->m_itemPos		    = *lpIndex;
    lpUIPt->m_cRemainItemNum    = 0;            // 의미 없음

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktUI), CmdCharUseItem, 0, 0);
}

bool SendPacket::CharUseCashItem(ClientNet::CClientEventHandler* lpHandler, unsigned long dwSenderID, 
                             unsigned long dwRecverID, Item::ItemPos* lpIndex)
{
    CHECK_TRUE_RETURN(0 == dwSenderID || 0 == dwRecverID, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktUI), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);

    PktUI* lpUIPt = reinterpret_cast<PktUI *>(lpMsgBlock->wr_ptr());

    lpUIPt->m_dwSender		    = dwSenderID;
    lpUIPt->m_dwRecver		    = dwRecverID;
    lpUIPt->m_itemPos		    = *lpIndex;
    lpUIPt->m_cRemainItemNum    = 0;            // 의미 없음

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktUI), CmdCharUseCashItem, 0, 0);
}



bool SendPacket::CharCastObject(ClientNet::CClientEventHandler* lpHandler, unsigned long dwSenderID, 
                                unsigned long dwRecverID, CastObject &CastObject)
{
    CHECK_TRUE_RETURN(0 == dwSenderID || 0 == dwRecverID, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktCO), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);
    
    PktCO* lpCOPt = reinterpret_cast<PktCO *>(lpMsgBlock->wr_ptr());

    lpCOPt->m_dwSenderID		= dwSenderID;
    lpCOPt->m_dwReceiverID		= dwRecverID;
    lpCOPt->m_sCastObject		= CastObject;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktCO), CmdCharCastObject, 0, 0);
}


bool SendPacket::CharInstallSocket(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID, 
                                   Item::ItemPos* lpEquipIndex, Item::ItemPos* lpSocket , unsigned char PickingNum) //dedi
{
    CHECK_TRUE_RETURN(0 == dwCharID || NULL == lpEquipIndex || NULL == lpSocket, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktIS), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);

    PktIS* lpISPt = reinterpret_cast<PktIS *>(lpMsgBlock->wr_ptr());

    lpISPt->m_dwCharID		= dwCharID;
    lpISPt->m_equipPos		= *lpEquipIndex;
    lpISPt->m_gemPos		= *lpSocket;
	lpISPt->m_PickingNum	= PickingNum; //dedi add pick number

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktIS), CmdCharInstallSocket, 0, 0);
}

bool SendPacket::CharRuneInstallSocket(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID, unsigned char ucRunePos,
                                   Item::ItemPos* lpEquipIndex, Item::ItemPos* lpSocket)
{
    CHECK_TRUE_RETURN(0 == dwCharID || NULL == lpEquipIndex || NULL == lpSocket, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktIRS), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);

    PktIRS* lpIRSPt = reinterpret_cast<PktIRS *>(lpMsgBlock->wr_ptr());
	
	/*char pos = PktIRS::RUNE_POS_RUNE1;
	if( ucRunePos == 1 )
		pos = PktIRS::RUNE_POS_RUNE2;*/

    lpIRSPt->m_dwCharID		= dwCharID;
    lpIRSPt->m_equipPos		= *lpEquipIndex;
	lpIRSPt->m_runePos		= *lpSocket;
	//lpIRSPt->m_cRuneSocketPos = pos ;
	lpIRSPt->m_cType		= PktIRS::RUNE_INSTALL ;
	

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktIRS), CmdCharInstallRuneSocket, 0, 0);
}

bool SendPacket::CharRuneRemoveSocket(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID, unsigned char ucRunePos,
                                   Item::ItemPos* lpEquipIndex, Item::ItemPos* lpSocket)
{
    CHECK_TRUE_RETURN(0 == dwCharID || NULL == lpEquipIndex || NULL == lpSocket, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktIRS), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);

    PktIRS* lpIRSPt = reinterpret_cast<PktIRS *>(lpMsgBlock->wr_ptr());
	
	/*char pos = PktIRS::RUNE_POS_RUNE1;
	if( ucRunePos == 1 )
		pos = PktIRS::RUNE_POS_RUNE2;*/

    lpIRSPt->m_dwCharID		= dwCharID;
    lpIRSPt->m_equipPos		= *lpEquipIndex;
	lpIRSPt->m_runePos		= *lpSocket;
	//lpIRSPt->m_cRuneSocketPos = pos ;
	lpIRSPt->m_cType		= PktIRS::RUNE_UNINSTALL ;
	

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktIRS), CmdCharInstallRuneSocket, 0, 0);
}


bool SendPacket::CharItemChemical(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID,
								  Item::ItemPos* lpPickkingPos, Item::ItemPos* lpTargetPos, unsigned char cPickkingNum)
{
	CHECK_TRUE_RETURN(0 == dwCharID || NULL == lpPickkingPos || NULL == lpTargetPos, false);

	ClientNet::CNetworkMsgBlock* lpMsgBlock = 
		ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktItemChemical), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

	PktItemChemical* lpICPt = reinterpret_cast<PktItemChemical *>(lpMsgBlock->wr_ptr());

	lpICPt->m_dwCharID		= dwCharID;
	lpICPt->m_pickkingPos	= *lpPickkingPos;
	lpICPt->m_targetPos		= *lpTargetPos;
	lpICPt->m_cPickkingNum	= cPickkingNum;

	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktItemChemical), CmdCharItemChemical, 0, 0);
}


bool SendPacket::CharUpgradeItem(ClientNet::CClientEventHandler* lpHandler, unsigned long dwNPCID)
{
    CHECK_TRUE_RETURN(0 == dwNPCID, false);
        
    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktUgI), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);
    
    PktUgI* lpUgIPt = reinterpret_cast<PktUgI *>(lpMsgBlock->wr_ptr());
    lpUgIPt->m_dwNPCID = dwNPCID;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktUgI), CmdCharUpgradeItem, 0, 0);
}


bool SendPacket::CharItemOptionGraft(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID, unsigned char cAttributeType)
{
	CHECK_TRUE_RETURN(0 == dwCharID, false);

	ClientNet::CNetworkMsgBlock* lpMsgBlock = 
		ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktItemOptionGraft), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

	PktItemOptionGraft* lpIOGPt = reinterpret_cast<PktItemOptionGraft *>(lpMsgBlock->wr_ptr());
	
	lpIOGPt->m_dwCharID			= dwCharID;
	lpIOGPt->m_cAttributeType	= cAttributeType;

	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktItemOptionGraft), CmdCharItemOptionGraft, 0, 0);
}


bool SendPacket::CharItemCompensation(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID)
{
	CHECK_TRUE_RETURN(0 == dwCharID, false);

	ClientNet::CNetworkMsgBlock* lpMsgBlock = 
		ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktItemCompensation), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

	PktItemCompensation* lpICPt = reinterpret_cast<PktItemCompensation *>(lpMsgBlock->wr_ptr());

	lpICPt->m_dwCharID			= dwCharID;

	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktItemCompensation), CmdCharItemCompensation, 0, 0);
}


bool SendPacket::CharSplitItem(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID, TakeType &TakeType)
{
    CHECK_TRUE_RETURN(0 == dwCharID, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktSplt), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);
    
    PktSplt* lpSpltPt		= reinterpret_cast<PktSplt *>(lpMsgBlock->wr_ptr());

    lpSpltPt->m_dwCharID	= dwCharID;
    lpSpltPt->m_TakeType	= TakeType;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktSplt), CmdCharSplitItem, 0, 0);
}



bool SendPacket::CharAutoRouting(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID, 
                                 unsigned __int64 nObjectID, unsigned short wItemID, Item::ItemPos Index)
{
    CHECK_TRUE_RETURN(0 == dwCharID, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktAutoRouting), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);
	
    PktAutoRouting* lpARPt	= reinterpret_cast<PktAutoRouting *>(lpMsgBlock->wr_ptr());

    lpARPt->m_dwCharID		= dwCharID;
    lpARPt->m_nObjectID		= nObjectID;
	lpARPt->m_wItemID		= wItemID;		
    lpARPt->m_itemPos		= Index;

    lpARPt->m_cCmd = (TakeType::TS_INVEN == Index.m_cPos) ? 
        PktAutoRouting::ARC_POSSIBLE : PktAutoRouting::ARC_IMPOSSIBLE;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktAutoRouting), CmdCharAutoRouting, 0, 0);    
}



bool SendPacket::StoreLogin(ClientNet::CClientEventHandler* lpHandler, char *Password, char PassSave)
{
    CHECK_TRUE_RETURN(NULL == Password, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktDeposit), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);
	
    PktDeposit* lpDeposit = reinterpret_cast<PktDeposit*>(lpMsgBlock->wr_ptr());

    lpDeposit->m_cCmd = PktDeposit::CMD::LOGIN;
    memcpy(&lpDeposit->m_szData[0], Password, Deposit::PASSWORD_LENGTH);
    lpDeposit->m_szData[4] = PassSave;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktDeposit), CmdDeposit, 0);    
}


bool SendPacket::StoreLogout(ClientNet::CClientEventHandler* lpHandler)
{
    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktDeposit), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

    PktDeposit* lpDeposit = reinterpret_cast<PktDeposit*>(lpMsgBlock->wr_ptr());
    lpDeposit->m_cCmd = PktDeposit::CMD::LOGOUT;    

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktDeposit), CmdDeposit, 0);
}


bool SendPacket::StoreBuyTab(ClientNet::CClientEventHandler* lpHandler, char TabNum)
{
    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktDeposit), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

	PktDeposit* lpDeposit = reinterpret_cast<PktDeposit*>(lpMsgBlock->wr_ptr());

    lpDeposit->m_cCmd = PktDeposit::CMD::BUY_TAB;
    lpDeposit->m_szData[0] = TabNum;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktDeposit), CmdDeposit, 0);
}


bool SendPacket::StoreChangePass(ClientNet::CClientEventHandler* lpHandler, char *Password, char *NewPassword)
{
    CHECK_TRUE_RETURN(NULL == Password || NULL == NewPassword, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktDeposit), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);
	
    PktDeposit* lpDeposit = reinterpret_cast<PktDeposit*>(lpMsgBlock->wr_ptr());

    lpDeposit->m_cCmd = PktDeposit::CMD::CHANGE_PASS;
    memcpy(&lpDeposit->m_szData[0], Password, Deposit::PASSWORD_LENGTH);
    memcpy(&lpDeposit->m_szData[4], NewPassword, Deposit::PASSWORD_LENGTH);

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktDeposit), CmdDeposit, 0);    
}



bool SendPacket::CharStallOpen(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID, char* StallName)
{
    CHECK_TRUE_RETURN(NULL == StallName || 0 == dwCharID, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktStO), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

    PktStO* lpStOPt = reinterpret_cast<PktStO *>(lpMsgBlock->wr_ptr());

    lpStOPt->m_dwCharID = dwCharID;
    strncpy(lpStOPt->m_StallName, StallName, PktStO::MAX_STALL_NAME_LEN);

	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktStO), CmdCharStallOpen, 0, 0);    
}


bool SendPacket::CharStallRegisterItem(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID, unsigned long dwShopID, 
                                       TakeType TakeType, unsigned long dwPrice, unsigned char cCmd)
{
    CHECK_TRUE_RETURN(0 == dwCharID, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktStRI), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);
	
    PktStRI* lpStRIPt = reinterpret_cast<PktStRI *>(lpMsgBlock->wr_ptr());

    lpStRIPt->m_dwCharID	= dwCharID;
	lpStRIPt->m_dwShopID	= dwShopID;
    lpStRIPt->m_TakeType	= TakeType;
    lpStRIPt->m_dwPrice		= dwPrice;
    lpStRIPt->m_cCmd		= cCmd;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktStRI), CmdCharStallRegisterItem, 0, 0);    
}


bool SendPacket::CharStallEnter(ClientNet::CClientEventHandler* lpHandler, 
                                unsigned long dwCustomerID, unsigned long dwOwner)
{
    CHECK_TRUE_RETURN(0 == dwCustomerID, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktStE), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);
	
    PktStE* lpStEPt = reinterpret_cast<PktStE *>(lpMsgBlock->wr_ptr());

    lpStEPt->m_dwCustomerID	= dwCustomerID;
    lpStEPt->m_dwOwnerID	= dwOwner;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktStE), CmdCharStallEnter, 0, 0);    
}


bool SendPacket::CharTakeItems(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID, 
                               unsigned char cTakeNum, TakeType* lpTakeTypes)
{
    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktTIs) + sizeof(TakeType) * cTakeNum, lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);
    
    PktTIs* lpTIsPt = reinterpret_cast<PktTIs *>(lpMsgBlock->wr_ptr());

    lpTIsPt->m_dwCharID	= dwCharID;
    lpTIsPt->m_TakeNum	= cTakeNum;
    CopyMemory(lpMsgBlock->wr_ptr() + sizeof(PktTIs), lpTakeTypes, sizeof(TakeType) * cTakeNum);

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktTIs) + sizeof(TakeType) * cTakeNum, CmdCharTakeItems, 0, 0);
}


bool SendPacket::CharTakeGold(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID, 
                              unsigned char cSrcPos, unsigned char cDstPos, unsigned long dwGold)
{
    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktTG), lpHandler->GetPeerAddress());
    
    CHECK_NULL_RETURN(lpMsgBlock, false);
    
    PktTG* lpTGPt = reinterpret_cast<PktTG *>(lpMsgBlock->wr_ptr());

    lpTGPt->m_dwCharID	= dwCharID;
    lpTGPt->m_cSrcPos	= cSrcPos;
    lpTGPt->m_cDstPos	= cDstPos;
    lpTGPt->m_dwGold	= dwGold;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktTG), CmdCharTakeGold, 0, 0);
}


bool SendPacket::CharExchangeCmd(ClientNet::CClientEventHandler* lpHandler, unsigned long dwSenderID, 
                                 unsigned long dwRecverID, unsigned char cCmd)
{
    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktExC), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);
    
    PktExC* lpExCPt = reinterpret_cast<PktExC *>(lpMsgBlock->wr_ptr());

    lpExCPt->m_dwSenderID	= dwSenderID;
    lpExCPt->m_dwRecverID	= dwRecverID;
    lpExCPt->m_cCmd			= cCmd;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktExC), CmdCharExchangeCmd, 0, 0);
}



bool SendPacket::CharQuickSlotMove(ClientNet::CClientEventHandler* lpHandler, 
                                   TakeType &TakeType, unsigned short usSkillID, unsigned char cLockCount, unsigned char cSkillLevel)
{
    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktQSM), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);
    
    PktQSM* lpQSMPt = reinterpret_cast<PktQSM *>(lpMsgBlock->wr_ptr());

    lpQSMPt->m_TakeType	= TakeType;
    lpQSMPt->m_usSkillID = usSkillID;
	lpQSMPt->m_cLockCount = cLockCount;
	lpQSMPt->m_cSkillLevel = cSkillLevel;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktQSM), CmdCharQuickSlotMove, 0, 0);
}


bool SendPacket::CharCastObjectInfo(ClientNet::CClientEventHandler* lpHandler, unsigned long dwSenderID, 
                                    unsigned long dwRecverID, CastObject &CastObject)
{
    CHECK_TRUE_RETURN(0 == dwSenderID || 0 == dwRecverID, false);
    
    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktCOInfo), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);

    PktCOInfo* lpCOInfoPt = reinterpret_cast<PktCOInfo *>(lpMsgBlock->wr_ptr());

    lpCOInfoPt->m_dwSenderID	= dwSenderID;
    lpCOInfoPt->m_dwReceiverID	= dwRecverID;
    lpCOInfoPt->m_sCastObject	= CastObject;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktCOInfo), CmdCharCastObjectInfo, 0, 0);        
}

#include "stdafx.h"
#include "SendPacket.h"
#include "SendSkill.h"

#include <Network/ClientNetwork/ClientEventHandler.h>
#include <Network/Packet/PacketCommand.h>
#include <Network/Packet/PacketStruct/CharStatusPacket.h>
#include <Network/Packet/PacketStruct/CharAttackPacket.h>
#include "GMMemory.h"


bool SendPacket::CharSkillCreate(ClientNet::CClientEventHandler* lpHandler, unsigned long CharID_In, 
                                 unsigned short SkillID_In, unsigned char Index_In)
{
    CHECK_TRUE_RETURN(0 == CharID_In, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktSk), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);
    
    LPPktSk lpSkPt = reinterpret_cast<LPPktSk>(lpMsgBlock->wr_ptr());

    lpSkPt->m_dwCharID  = CharID_In;
    lpSkPt->m_wSkill	= SkillID_In;
    lpSkPt->m_cIndex	= Index_In;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktSk), CmdCharSkillCreate, 0, 0);
}


bool SendPacket::CharSkillErase(ClientNet::CClientEventHandler* lpHandler, unsigned long CharID_In, 
                                unsigned short SkillID_In, unsigned char Index_In, Item::ItemPos* lpItem_In)
{
    CHECK_TRUE_RETURN(0 == CharID_In, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktSk), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);
    
    LPPktSk lpSkPt = reinterpret_cast<LPPktSk>(lpMsgBlock->wr_ptr());

    lpSkPt->m_dwCharID		= CharID_In;
    lpSkPt->m_wSkill		= SkillID_In;
    lpSkPt->m_cIndex		= Index_In;
	
	// ¸Á°¢ÀÇµ¹
	lpSkPt->m_ItemPos		= *lpItem_In;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktSk), CmdCharSkillErase, 0, 0);
}


bool SendPacket::CharSummonCmd(ClientNet::CClientEventHandler* lpHandler, unsigned long CharID_In, 
                               unsigned char Cmd_In, unsigned long TargetID_In)
{
    CHECK_TRUE_RETURN(0 == CharID_In, false);
    
    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktSummonCmd), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);
	
    LPPktSummonCmd lpSuCPt = reinterpret_cast<LPPktSummonCmd>(lpMsgBlock->wr_ptr());

    lpSuCPt->m_dwCharID		= CharID_In;
    lpSuCPt->m_cCmd			= Cmd_In;
    lpSuCPt->m_dwTargetID	= TargetID_In;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktSummonCmd), CmdCharSummonCmd, 0, 0);    
}

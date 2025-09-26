#include "stdafx.h"
#include "SendPacket.h"
#include "SendQuest.h"

#include <Network/ClientNetwork/ClientEventHandler.h>
#include <Network/Packet/PacketCommand.h>
#include <Network/Packet/PacketStruct/CharQuestPacket.h>
#include "GMMemory.h"

bool SendPacket::CharStartQuest(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID_In, 
                                unsigned long dwNPCID_In, unsigned short wQuestID_In)
{
    CHECK_TRUE_RETURN(0 == dwCharID_In, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktStartQuest), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);
    
    PktStartQuest* lpSQPt = reinterpret_cast<PktStartQuest *>(lpMsgBlock->wr_ptr());

    lpSQPt->m_dwCharID	= dwCharID_In;
    lpSQPt->m_dwNPCID	= dwNPCID_In;
    lpSQPt->m_wQuestID	= wQuestID_In;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktStartQuest), CmdCharStartQuest, 0, 0);
}


bool SendPacket::CharCancelQuest(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID_In, unsigned short wQuestID_In)
{
    CHECK_TRUE_RETURN(0 == dwCharID_In, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktCancelQuest), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);
    
    PktCancelQuest* lpCQPt = reinterpret_cast<PktCancelQuest *>(lpMsgBlock->wr_ptr());

    lpCQPt->m_dwCharID	= dwCharID_In;
    lpCQPt->m_wQuestID	= wQuestID_In;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktCancelQuest), CmdCharCancelQuest, 0, 0);
}


bool SendPacket::CharOperateTrigger(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID_In, unsigned short wQuestID_In, 
                                    unsigned char cPhase_In, unsigned char cTriggerKind_In, unsigned char cTrigger_In, unsigned char cCount_In)
{
    CHECK_TRUE_RETURN(0 == dwCharID_In, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktOperateTrigger), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);
    
    PktOperateTrigger* lpOTPt = reinterpret_cast<PktOperateTrigger *>(lpMsgBlock->wr_ptr());

    lpOTPt->m_dwCharID		= dwCharID_In;
    lpOTPt->m_wQuestID		= wQuestID_In;
    lpOTPt->m_cPhase		= cPhase_In;
	lpOTPt->m_cTriggerKind	= cTriggerKind_In;
    lpOTPt->m_cTrigger		= cTrigger_In;
    lpOTPt->m_cCount		= cCount_In;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktOperateTrigger), CmdCharOperateTrigger, 0, 0);
}


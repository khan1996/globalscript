
#include "stdafx.h"

#include <Network/ClientNetwork/ClientEventHandler.h>
#include <Network/ClientSocket/ClientSocket.h>

#include <Network/Packet/PacketCommand.h>
#include <Network/Packet/PacketStruct/ServerPacket.h>
#include <Network/Packet/PacketStruct/CharStatusPacket.h>
#include <Network/Packet/PacketStruct/CharConfigPacket.h>
#include <Network/Packet/PacketStruct/CharAdminPacket.h>

#include "SendPacket.h"
#include "SendEtc.h"

// 이 헤더가 위로 올라가면 꼬여요.
#include <mmsystem.h>
#include "GMMemory.h"


bool SendPacket::SendSysPing(ClientNet::CClientEventHandler* lpHandler)
{
    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktSyP), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);

    PktSyP* lpSyPAckPt = reinterpret_cast<PktSyP *>(lpMsgBlock->wr_ptr());
    lpSyPAckPt->m_dwTickTime = timeGetTime();

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktSyP), CmdSysPing, 0);
}


bool SendPacket::CharSuicide(ClientNet::CClientEventHandler* lpHandler, unsigned long CharID_In)
{
    CHECK_TRUE_RETURN(0 == CharID_In, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktSC), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);
    
    PktSC* lpSCPt = reinterpret_cast<PktSC *>(lpMsgBlock->wr_ptr());
    lpSCPt->m_dwCharID = CharID_In;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktSC), CmdCharSuicide, 0, 0);
}


bool SendPacket::CharBindPos(ClientNet::CClientEventHandler* lpHandler, unsigned long NPCID_In, 
                             unsigned char Cmd_In, POS* lpPos_In, char Zone_In)
{
    CHECK_TRUE_RETURN(0 == NPCID_In, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktBP), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);
    
    PktBP* lpBPPt = reinterpret_cast<PktBP *>(lpMsgBlock->wr_ptr());

    lpBPPt->m_Pos			= *lpPos_In;
    lpBPPt->m_dwNPCID		= NPCID_In;
    lpBPPt->m_cCmd			= Cmd_In;	
    lpBPPt->m_cZone			= Zone_In;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktBP), CmdCharBindPosition, 0, 0);
}


bool SendPacket::CharIncreasePoint(ClientNet::CClientEventHandler* lpHandler, 
                                   unsigned long CharID_In, unsigned char StateType_In)
{
    CHECK_TRUE_RETURN(0 == CharID_In, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktIP), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);
    
    PktIP* lpIPPt = reinterpret_cast<PktIP *>(lpMsgBlock->wr_ptr());

    lpIPPt->m_dwCharID		= CharID_In;
    lpIPPt->m_cStateType	= StateType_In;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktIP), CmdCharIncreasePoint, 0, 0);
}


bool SendPacket::CharClassUpgrade(ClientNet::CClientEventHandler* lpHandler, unsigned long CharID_In, unsigned char ClassID_In)
{
    CHECK_TRUE_RETURN(0 == CharID_In, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktCU), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);
    
    PktCU* lpCUPt = reinterpret_cast<PktCU *>(lpMsgBlock->wr_ptr());

    lpCUPt->m_dwCharID		= CharID_In;
    lpCUPt->m_cClass		= ClassID_In;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktCU), CmdCharClassUpgrade, 0, 0);
}


bool SendPacket::CharControlOption(ClientNet::CClientEventHandler* lpHandler, unsigned long CharID_In, RejectOption &Reject_In)
{
    CHECK_TRUE_RETURN(0 == CharID_In, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktCOp), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);
    
    PktCOp* lpCOpPt = reinterpret_cast<PktCOp *>(lpMsgBlock->wr_ptr());

    lpCOpPt->m_dwCharID		= CharID_In;
    lpCOpPt->m_RejectOption	= Reject_In;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktCOp), CmdCharControlOption, 0, 0);
}


bool SendPacket::CharAdmin(ClientNet::CClientEventHandler* lpHandler, unsigned short Cmd_In, 
                           char* Name_In, char Zone_In, char Channel_In, 
                           unsigned short ProtoTypeID, POS& Pos_In, unsigned long dwAmount,unsigned long dwAmount2, unsigned long dwAmount3)
{
    CHECK_TRUE_RETURN(NULL == Name_In, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktAdmin), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);
    
    PktAdmin* lpAdmin   = reinterpret_cast<PktAdmin*>(lpMsgBlock->wr_ptr());
    lpAdmin->m_usCmd	= Cmd_In;

    memcpy(lpAdmin->m_stName, Name_In, PktAdmin::MAX_NAME_LEN);

    if (0 != ProtoTypeID)
    {
        lpAdmin->m_usProtoTypeID		= ProtoTypeID;
    }
    else
    {
        lpAdmin->m_ZoneInfo.m_cZone		= Zone_In;
        lpAdmin->m_ZoneInfo.m_cChannel	= Channel_In;
    }

    lpAdmin->m_Position	= Pos_In;
    lpAdmin->m_dwAmount = dwAmount;
	lpAdmin->m_dwAmount2 = dwAmount2;
	lpAdmin->m_dwAmount3 = dwAmount3;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktAdmin), CmdCharAdminCmd, 0);
}


bool SendPacket::CharStateRedistribution(ClientNet::CClientEventHandler* lpHandler, ChState* lpState_In)
{
    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktSR), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);
    
    PktSR* lpSRPt = reinterpret_cast<PktSR *>(lpMsgBlock->wr_ptr());
    lpSRPt->m_State = *lpState_In;
    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktSR), CmdCharStateRedistribution, 0, 0);
}


bool SendPacket::CharStatusRetrain(ClientNet::CClientEventHandler* lpHandler, ChState* lpState_In, Item::ItemPos* lpIndex_In)
{
	ClientNet::CNetworkMsgBlock* lpMsgBlock = 
		ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktSRT), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

	PktSRT* lpSRTPt = reinterpret_cast<PktSRT *>(lpMsgBlock->wr_ptr());
	lpSRTPt->m_State	= *lpState_In;
	lpSRTPt->m_ItemPos	= *lpIndex_In;
	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktSRT), CmdCharStatusRetrain, 0, 0);
}

bool SendPacket::CharNameChange(ClientNet::CClientEventHandler* lpHandler, unsigned long dwUID, 
                                unsigned long dwCID, const char* szChangeName, Item::ItemPos* lpItem_In)
{
    ClientNet::CNetworkMsgBlock* lpMsgBlock =
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktCharNameChange), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);

    PktCharNameChange* lpCharNameChange = 
        reinterpret_cast<PktCharNameChange*>(lpMsgBlock->wr_ptr());

    lpCharNameChange->m_dwUID = dwUID;
    lpCharNameChange->m_dwCID = dwCID;
    lpCharNameChange->m_cNameChangeCount = 0;

    strncpy(lpCharNameChange->m_szCharName, szChangeName, CHAR_INFOST::MAX_NAME_LEN);
    lpCharNameChange->m_szCharName[CHAR_INFOST::MAX_NAME_LEN - 1] = 0;

	lpCharNameChange->m_ItemPos		= *lpItem_In;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktCharNameChange), CmdCharNameChange, 0, 0);
}
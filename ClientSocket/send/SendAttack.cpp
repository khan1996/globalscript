#include "stdafx.h"

#include "SendAttack.h"
#include "SendPacket.h"

#include <Network/ClientSocket/ClientSocket.h>
#include <Network/Packet/PacketCommand.h>
#include <Network/Packet/PacketStruct/CharAttackPacket.h>
#include <Network/Packet/PacketStruct/CharItemPacket.h>
#include <Network/Packet/PacketStruct/CharStatusPacket.h>
#include <Network/ClientNetwork/NetworkMsgBlock.h>
#include <Network/ClientNetwork/ClientEventHandler.h>

#include <GameGuardLib/NPGameLib.h>

#include <mmsystem.h>
#include "GMMemory.h"

bool SendPacket::CharAttack(ClientNet::CClientEventHandler* lpHandler, unsigned long CharID_In, 
                            POS* lpPos_In, float Dir_In, AtType &AtType_In, AtNode &AtNode_In)
{
    CHECK_TRUE_RETURN(0 == CharID_In, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(
        sizeof(PktAt) + ClientSocket::MAX_GAME_GUARD_CRYPT_HEADER_SIZE, lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);
	
    PktAt* lpAtPt = reinterpret_cast<PktAt *>(lpMsgBlock->wr_ptr());

    lpAtPt->m_dwCharID	= CharID_In;
    lpAtPt->m_Postion	= *lpPos_In;
    lpAtPt->m_fDir		= Dir_In;
    lpAtPt->m_AtType	= AtType_In;
    lpAtPt->m_AtNode	= AtNode_In;

	// edith 2009.08.11 게임가드 2.5 업그레이드
	// 게임 가드에 의한 패킷 암호화
//	unsigned short wPacketSize = static_cast<unsigned short>(
//		EncryptPacket(lpMsgBlock->wr_ptr() + sizeof(PktBase), sizeof(PktAt) - sizeof(PktBase)));
//	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktBase) + wPacketSize, CmdCharAttack, timeGetTime());

	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktAt), CmdCharAttack, timeGetTime());
}

bool SendPacket::CharRespawn(ClientNet::CClientEventHandler*  lpHandler, unsigned long CharID_In, 
                             unsigned char cCmd_In, POS& Pos_In, unsigned long dwTownID_In)
{
    CHECK_TRUE_RETURN(0 == CharID_In, true);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(
        sizeof(PktRs), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);
	
    PktRs* lpRsPt = reinterpret_cast<PktRs *>(lpMsgBlock->wr_ptr());

    lpRsPt->m_dwCharID		= CharID_In;
    lpRsPt->m_cCmd			= cCmd_In;
    lpRsPt->m_Position		= Pos_In;
	lpRsPt->m_dwTownID		= dwTownID_In;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktRs), CmdCharRespawn, 0, 0);    
}

bool SendPacket::CharRespawnInfo(ClientNet::CClientEventHandler* lpHandler, unsigned long CharID_In)
{
	CHECK_TRUE_RETURN(0 == CharID_In, true);

	ClientNet::CNetworkMsgBlock* lpMsgBlock = 
		ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(
        sizeof(PktRsInfo), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

	PktRsInfo* lpPktRsInfo = reinterpret_cast<PktRsInfo*>(lpMsgBlock->wr_ptr());

	lpPktRsInfo->m_dwCharID	= CharID_In;

	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktRsInfo), CmdCharRespawnInfo, 0, 0);
}

bool SendPacket::CharRespawnAreaInfo(ClientNet::CClientEventHandler* lpHandler, unsigned long CharID_In, unsigned long TownID_In)
{
	CHECK_TRUE_RETURN(0 == CharID_In, true);

	ClientNet::CNetworkMsgBlock* lpMsgBlock = 
		ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(
        sizeof(PktRsAreaInfo), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

	PktRsAreaInfo* lpPktRsAreaInfo = reinterpret_cast<PktRsAreaInfo*>(lpMsgBlock->wr_ptr());

	lpPktRsAreaInfo->m_dwCharID = CharID_In;
	lpPktRsAreaInfo->m_dwTownID = TownID_In;

	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktRsAreaInfo), CmdCharRespawnAreaInfo, 0, 0);
}

bool SendPacket::CharRespawnWaitQueue(ClientNet::CClientEventHandler* lpHandler, unsigned long CharID_In)
{
    CHECK_TRUE_RETURN(0 == CharID_In, false);
    
    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(
        sizeof(PktRsWaitQueue), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);
	
    PktRsWaitQueue* lpRsWQPt = reinterpret_cast<PktRsWaitQueue *>(lpMsgBlock->wr_ptr());
    lpRsWQPt->m_dwCharID = CharID_In;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktRsWaitQueue), CmdCharRespawnWaitQueue, 0, 0);   
}

bool SendPacket::CharSwitchHand(ClientNet::CClientEventHandler* lpHandler, unsigned char SelectHand_In)
{
    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(
        sizeof(PktSwEQ), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);
    
    PktSwEQ* lpSwHPt = reinterpret_cast<PktSwEQ *>(lpMsgBlock->wr_ptr());
	lpSwHPt->m_cType = 0;
    lpSwHPt->m_cSelect	= SelectHand_In;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktSwEQ), CmdCharSwitchEQ, 0, 0);
}

bool SendPacket::CharSwitchRide(ClientNet::CClientEventHandler* lpHandler, unsigned char Ride_In)
{
    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(
        sizeof(PktSwEQ), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);
    
    PktSwEQ* lpSwHPt = reinterpret_cast<PktSwEQ *>(lpMsgBlock->wr_ptr());
	lpSwHPt->m_cType = 1;
    lpSwHPt->m_cSelect	= Ride_In;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktSwEQ), CmdCharSwitchEQ, 0, 0);
}

bool SendPacket::CharPeaceMode(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID_In, bool bPeace_In)
{
    CHECK_TRUE_RETURN(0 == dwCharID_In, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktPeace), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);
    
    PktPeace* lpPeacePt = reinterpret_cast<PktPeace *>(lpMsgBlock->wr_ptr());

    lpPeacePt->m_dwCharID = dwCharID_In;
    lpPeacePt->m_bPeace = bPeace_In;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktPeace), CmdCharPeaceMode, 0, 0);
}

bool SendPacket::CharDuelCmd(ClientNet::CClientEventHandler* lpHandler, unsigned long SenderID_In, 
                             unsigned long RecverID_In, unsigned char Cmd_In)
{
    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktDuC), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);

    PktDuC* lpDuCPt = reinterpret_cast<PktDuC *>(lpMsgBlock->wr_ptr());

    lpDuCPt->m_dwSenderID	= SenderID_In;
    lpDuCPt->m_dwRecverID	= RecverID_In;
    lpDuCPt->m_cCmd			= Cmd_In;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktDuC), CmdCharDuelCmd, 0, 0);
}

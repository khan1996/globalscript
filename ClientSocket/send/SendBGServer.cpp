
#include "stdafx.h"

#include <Network/ClientNetwork/ClientEventHandler.h>
#include <Network/ClientSocket/ClientSocket.h>

#include <Network/Packet/PacketCommand.h>
#include <Network/Packet/PacketStruct/BGServerPacket.h>

#include "SendPacket.h"
#include "SendBGServer.h"
#include "GMMemory.h"


bool SendPacket::CharBGServerMapList(ClientNet::CClientEventHandler* lpHandler)
{
	ClientNet::CNetworkMsgBlock* lpMsgBlock = 
		ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(
        sizeof(PktBGServerMapList), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

	PktBGServerMapList* lpBGSMLPt = reinterpret_cast<PktBGServerMapList *>(lpMsgBlock->wr_ptr());

	lpBGSMLPt->m_bAll = true;
	lpBGSMLPt->m_cMapInfoNodeNum = PktBGServerMapList::MAX_MAP_NUM;

	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktBGServerMapList), CmdBGServerMapList, 0, 0);
}


bool SendPacket::CharBGServerResultList(ClientNet::CClientEventHandler* lpHandler)
{
	ClientNet::CNetworkMsgBlock* lpMsgBlock = 
		ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(
        sizeof(PktBGServerResultList), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

	PktBGServerResultList* lpBGSRLPt = reinterpret_cast<PktBGServerResultList *>(lpMsgBlock->wr_ptr());

	lpBGSRLPt->m_bAll = true;
	lpBGSRLPt->m_cResultInfoNodeNum = PktBGServerResultList::MAX_MAP_NUM;

	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktBGServerResultList), CmdBGServerResultList, 0, 0);
}


bool SendPacket::CharBGServerMoveZone(ClientNet::CClientEventHandler* lpHandler, unsigned short wMapNumber, unsigned char cMoveType)
{
	ClientNet::CNetworkMsgBlock* lpMsgBlock = 
		ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(
        sizeof(PktBGServerMoveZone), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

	PktBGServerMoveZone* lpBGSMZPt = reinterpret_cast<PktBGServerMoveZone *>(lpMsgBlock->wr_ptr());

	lpBGSMZPt->m_wMapNumber	= wMapNumber;
	lpBGSMZPt->m_cMoveType	= cMoveType;
	lpBGSMZPt->m_cZone		= 0;

	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktBGServerMoveZone), CmdBGServerMoveZone, 0, 0);
}


bool SendPacket::CharBGServerCharSlot(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, unsigned char cGroup)
{
	ClientNet::CNetworkMsgBlock* lpMsgBlock = 
		ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(
        sizeof(PktBGServerCharSlot), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

	PktBGServerCharSlot* lpBGSCSPt = reinterpret_cast<PktBGServerCharSlot *>(lpMsgBlock->wr_ptr());

	lpBGSCSPt->m_dwCID		= dwCID;
	lpBGSCSPt->m_dwUID		= 0;
	lpBGSCSPt->m_cGroup		= cGroup;

	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktBGServerCharSlot), CmdBGServerCharSlot, 0, 0);
}


bool SendPacket::CharBGServerMileageChange(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, unsigned char cGroup, unsigned char cCmd, 
										   unsigned long dwMileage)
{
	ClientNet::CNetworkMsgBlock* lpMsgBlock = 
		ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(
        sizeof(PktBGServerMileageChange), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

	PktBGServerMileageChange* lpBGSMCPt = reinterpret_cast<PktBGServerMileageChange *>(lpMsgBlock->wr_ptr());

	lpBGSMCPt->m_dwCID		= dwCID;
	lpBGSMCPt->m_cGroup		= cGroup;
	lpBGSMCPt->m_cCmd		= cCmd;
	lpBGSMCPt->m_dwGold		= 0;
	lpBGSMCPt->m_dwMileage	= dwMileage;

	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktBGServerMileageChange), CmdBGServerMileageChange, 0, 0);
}

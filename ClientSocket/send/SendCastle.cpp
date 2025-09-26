#include "stdafx.h"
#include "SendPacket.h"
#include "SendCastle.h"

#include <Network/ClientNetwork/ClientEventHandler.h>
#include <Network/Packet/PacketCommand.h>
#include <Network/Packet/PacketStruct/CastlePacket.h>
#include "GMMemory.h"


bool SendPacket::CharCastleCmd(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, unsigned long dwCastleID, 
							   unsigned long dwCastleObjectID, unsigned long dwValue1, unsigned long dwValue2, unsigned char cSubCmd)
{
	CHECK_TRUE_RETURN(0 == dwCID, false);

	ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(
        sizeof(PktCastleCmd), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

	LPPktCastleCmd lpCCPt = reinterpret_cast<LPPktCastleCmd>(lpMsgBlock->wr_ptr());

	lpCCPt->m_dwCID				= dwCID;
	lpCCPt->m_dwCastleID		= dwCastleID;
	lpCCPt->m_dwCastleObjectID	= dwCastleObjectID;
	lpCCPt->m_dwValue1			= dwValue1;
	lpCCPt->m_dwValue2			= dwValue2;
	lpCCPt->m_cSubCmd			= cSubCmd;

	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktCastleCmd), CmdCastleCmd, 0, 0);
}

bool SendPacket::CharCampCmd(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, unsigned long dwCampID, 
							 unsigned long dwValue, unsigned long dwValue2, unsigned char cSubCmd)
{
	CHECK_TRUE_RETURN(0 == dwCID, false);

	ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(
        sizeof(PktCampCmd), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

	LPPktCampCmd lpCCPt = reinterpret_cast<LPPktCampCmd>(lpMsgBlock->wr_ptr());

	lpCCPt->m_dwCID			= dwCID;
	lpCCPt->m_dwCampID		= dwCampID;
	lpCCPt->m_cState		= 0;
	lpCCPt->m_dwValue1		= dwValue;
	lpCCPt->m_dwValue2		= dwValue2;
	lpCCPt->m_cSubCmd		= cSubCmd;

	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktCampCmd), CmdCampCmd, 0, 0);
}

bool SendPacket::CharSiegeArmsCmd(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, unsigned long dwArmsID, unsigned char cSubCmd)
{
	CHECK_TRUE_RETURN(0 == dwCID, false);

	ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(
        sizeof(PktSiegeArmsCmd), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

	LPPktSiegeArmsCmd lpSACPt = reinterpret_cast<LPPktSiegeArmsCmd>(lpMsgBlock->wr_ptr());

	lpSACPt->m_dwCID	= dwCID;
	lpSACPt->m_dwArmsID	= dwArmsID;
	lpSACPt->m_cSubCmd	= cSubCmd;

	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktSiegeArmsCmd), CmdSiegeArmsCmd, 0, 0);
}

bool SendPacket::CharCastleRight(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, unsigned long dwCastleID, CastleRight castleRight)
{
	ClientNet::CNetworkMsgBlock* lpMsgBlock = 
		ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(
        sizeof(PktCastleRight), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

	PktCastleRight* lpCRPt = reinterpret_cast<PktCastleRight *>(lpMsgBlock->wr_ptr());

	lpCRPt->m_dwCID			= dwCID;
	lpCRPt->m_dwCastleID	= dwCastleID;
	lpCRPt->m_CastleRight	= castleRight;

	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktCastleRight), CmdCastleRight, 0, 0);
}

bool SendPacket::CharCampRight(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, unsigned long dwCampID, CampRight campRight)
{
	ClientNet::CNetworkMsgBlock* lpMsgBlock = 
		ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(
        sizeof(PktCampRight), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

	PktCampRight* lpCRPt = reinterpret_cast<PktCampRight *>(lpMsgBlock->wr_ptr());

	lpCRPt->m_dwCID		= dwCID;
	lpCRPt->m_dwCampID	= dwCampID;
	lpCRPt->m_CampRight	= campRight;

	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktCampRight), CmdCampRight, 0, 0);
}

// 성 상징물 보석 아이템 넣기/빼기
bool SendPacket::CharTakeCastleJewel(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, unsigned long dwCastleID, unsigned char cSubCmd,
									 unsigned short wItemID, unsigned char cIndex, unsigned char cNum, Item::ItemPos pos)
{
	ClientNet::CNetworkMsgBlock* lpMsgBlock = 
		ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(
		sizeof(PktTakeCastleJewel), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

	PktTakeCastleJewel* lpPktTCJ = reinterpret_cast<PktTakeCastleJewel *>(lpMsgBlock->wr_ptr());

	lpPktTCJ->m_dwCID		= dwCID;
	lpPktTCJ->m_dwCastleID	= dwCastleID;
	lpPktTCJ->m_cSubCmd		= cSubCmd;
	lpPktTCJ->m_wItemID		= wItemID;
	lpPktTCJ->m_cIndex		= cIndex;
	lpPktTCJ->m_cNum		= cNum;
	lpPktTCJ->m_Pos			= pos;

	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktTakeCastleJewel), CmdTakeCastleJewel, 0, 0);
}

// 길드 요새 자재 넣기/빼기
bool SendPacket::CharTakeMaterial(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, unsigned long dwCampID, unsigned char cSubCmd,
								unsigned char cNum, Item::ItemPos pos)
{
	ClientNet::CNetworkMsgBlock* lpMsgBlock = 
		ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(
		sizeof(PktTakeMaterial), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

	PktTakeMaterial* lpPktTM = reinterpret_cast<PktTakeMaterial *>(lpMsgBlock->wr_ptr());

	lpPktTM->m_dwCID	= dwCID;
	lpPktTM->m_dwCampID	= dwCampID;
	lpPktTM->m_cSubCmd	= cSubCmd;
	lpPktTM->m_cNum		= cNum;
	lpPktTM->m_Pos		= pos;

	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktTakeMaterial), CmdTakeMaterial, 0, 0);
}

bool SendPacket::WarOnOff(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, unsigned char cFlag, unsigned char cType)
{
	ClientNet::CNetworkMsgBlock* lpMsgBlock = 
					ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktWarOnOff), lpHandler->GetPeerAddress());
		
	CHECK_NULL_RETURN(lpMsgBlock, false);

	PktWarOnOff* lpPktWarOnOff = reinterpret_cast<PktWarOnOff*>(lpMsgBlock->wr_ptr());
	
	lpPktWarOnOff->m_dwCID	= dwCID;
	lpPktWarOnOff->m_cType	= cType;	
	lpPktWarOnOff->m_cFlag	= cFlag;
	
	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktWarOnOff), CmdWarOnOff, 0, 0);
}
								  
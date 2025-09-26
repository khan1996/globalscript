
#include "stdafx.h"

#include <Network/ClientSocket/ClientSocket.h>
#include <Network/ClientNetwork/ClientEventHandler.h>
#include <Network/Packet/PacketCommand.h>
#include <Network/Packet/PacketStruct/CharCommunityPacket.h>
#include <Network/Packet/PacketStruct/CharLoginoutPacket.h>
#include <Network/Packet/PacketStruct/ServerPacket.h>

#include <Network/Packet/PacketStruct/GuildPacket.h>

#include "SendPacket.h"
#include "SendLoginout.h"
#include "GMMemory.h"


bool SendPacket::CharLogin(ClientSocket& clientSocket, unsigned long dwUID, 
                           unsigned long dwCID, unsigned long dwSessionID)
{
    clientSocket.ClearAddress(ClientSocket::MoveZoneAddr);
    
    CHECK_TRUE_RETURN(0 == dwUID || 0 == dwCID, false);

    ClientNet::CClientEventHandler* lpHandler = 
        &clientSocket.GetHandler(ClientSocket::GameEventHandler);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktCLi), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);
    
    PktCLi* lpCLiPt = reinterpret_cast<PktCLi *>(lpMsgBlock->wr_ptr());

    lpCLiPt->m_dwSessionID	= dwSessionID;
    lpCLiPt->m_dwUserID		= dwUID;
    lpCLiPt->m_dwCharID		= dwCID;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktCLi), CmdCharLogin, 0, 0);
}


bool SendPacket::CharLogout(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, unsigned long dwCloseReason)
{
    CHECK_TRUE_RETURN(0 == dwCID, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktCLo), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);
    
    PktCLo* lpCLoPt = reinterpret_cast<PktCLo *>(lpMsgBlock->wr_ptr());
    lpCLoPt->m_dwCharID	= dwCID;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktCLo), CmdCharLogout, 0, 0);
}


// sphawk : 채널 값이 추가되었습니다. 무작위 선택은 -1을 넣어 주세요.
bool SendPacket::CharMoveZone(ClientNet::CClientEventHandler* lpHandler, char cZone, char cChannel, POS& NewPos_In)
{
    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktSZMv), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);
    
    PktSZMv* lpSZMvPt = reinterpret_cast<PktSZMv *>(lpMsgBlock->wr_ptr());

    lpSZMvPt->m_cChannel = cChannel;
    lpSZMvPt->m_cZone	 = cZone;
    lpSZMvPt->m_NewPos	 = NewPos_In;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktSZMv), CmdCharMoveZone, 0, 0);
}


bool SendPacket::ServerZone(ClientNet::CClientEventHandler* lpHandler, char cZone,char cChannel)
{
    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktSZ), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);

    PktSZ* lpSZPt = reinterpret_cast<PktSZ *>(lpMsgBlock->wr_ptr());

    lpSZPt->m_cZone		= cZone;	
    lpSZPt->m_cChannel	= cChannel;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktSZ), CmdServerZone, 0, 0);
}


bool SendPacket::CSAuthReturnCode(ClientNet::CClientEventHandler* lpHandler, 
								  unsigned long dwCID, unsigned long dwAuthCodeType,
								  unsigned long dwReturnCode, const _GG_AUTH_DATA* lpAnswerCode)
{
	ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktCSAuth), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

	PktCSAuth* lpCSAuthPt = reinterpret_cast<PktCSAuth *>(lpMsgBlock->wr_ptr());

	lpCSAuthPt->m_dwCharID		= dwCID;	
	lpCSAuthPt->m_AuthCodeType	= dwAuthCodeType;
	lpCSAuthPt->m_dwAuthCode	= dwReturnCode;
	lpCSAuthPt->m_AuthCode2		= *lpAnswerCode;

	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktCSAuth), CmdCSAuth, 0, 0);
}

bool SendPacket::KeyInfo(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, unsigned long* pKeyInfo)
{
	ClientNet::CNetworkMsgBlock* lpMsgBlock = 
		ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktKeyInfo), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

	PktKeyInfo* lpKeyInfo = reinterpret_cast<PktKeyInfo*>(lpMsgBlock->wr_ptr());

	lpKeyInfo->m_dwCID = dwCID;
	memcpy(lpKeyInfo->m_dwKeyInfo, pKeyInfo, sizeof(unsigned long)*PktKeyInfo::MAX_KEY_INFO);

	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktKeyInfo), CmdKeyInfo, 0, 0);

}

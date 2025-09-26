#include "stdafx.h"
#include "SendMove.h"
#include "SendPacket.h"

#include <Network/ClientNetwork/ClientEventHandler.h>
#include <Network/ClientSocket/ClientSocket.h>
#include <Network/Packet/PacketCommand.h>
#include <Network/Packet/PacketStruct/AddressPacket.h>
#include <Network/Packet/PacketStruct/CharMovePacket.h>
#include <Network/Packet/PacketStruct/CharStatusPacket.h>

#include <mmsystem.h>
#include "GMMemory.h"


// TODO : 채팅서버로도 보내주게 외부에서 고쳐 줘야 한다.
// TODO : 1분마다 한번씩 서버에 UDP패킷 업데이트 하도록 고쳐 줘야 한다.
/*
bool SendPacket::CharMoveUpdate(ClientNet::CClientEventHandler* lpHandler, unsigned long CharID_In, 
                                bool SitMode_In, POS* Pos_In, float Dir_In)
{
    CHECK_TRUE_RETURN(0 == CharID_In, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktMU), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);

    PktMU* lpMUPt = reinterpret_cast<PktMU *>(lpMsgBlock->wr_ptr());

    lpMUPt->m_dwTick	= timeGetTime();    // 스핵 방지 루틴 
    lpMUPt->m_bSitMode	= SitMode_In;
    lpMUPt->m_Position	= *Pos_In;	
    lpMUPt->m_fDir		= Dir_In;

    return SendPacket(lpHandler, lpMsgBlock,  sizeof(PktMU), CmdCharMoveUpdate, 0, 0);
}

bool SendPacket::CharUpdateAddress(ClientSocket& clientSocket, unsigned long CharID_In)
{
    ClientNet::CSession& Session = clientSocket.GetSession(ClientSocket::UDPSession);
    CHECK_TRUE_RETURN(0 == CharID_In, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktUA), INET_Addr(
        reinterpret_cast<SOCKADDR&>(clientSocket.GetAddress(ClientSocket::GameServerUDPAddr)), sizeof(SOCKADDR_IN)), CmdCharUpdateAddress);

    CHECK_NULL_RETURN(lpMsgBlock, false);

    PktUA* lpUAPt = reinterpret_cast<PktUA *>(lpMsgBlock->wr_ptr());

    lpUAPt->m_dwCharID			= CharID_In;
    lpUAPt->m_PrivateAddress	= clientSocket.GetAddress(ClientSocket::PrivateAddr);

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktUA), CmdCharUpdateAddress, 0, 0);
}


bool SendPacket::CharAddressInfo(ClientSocket& clientSocket, unsigned long CharID_In, unsigned long TargetID_In)
{
    ClientNet::CSession& Session = clientSocket.GetSession(ClientSocket::UDPSession);    
    CHECK_TRUE_RETURN(0 == CharID_In, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktAI), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);
    
    PktAI* lpAIPt = reinterpret_cast<PktAI *>(lpMsgBlock->wr_ptr());

    lpAIPt->m_AddressInfo.m_dwCharID		= CharID_In;
    lpAIPt->m_AddressInfo.m_PublicAddress	= clientSocket.GetAddress(ClientSocket::PublicAddr);
    lpAIPt->m_AddressInfo.m_PrivateAddress	= clientSocket.GetAddress(ClientSocket::PrivateAddr);

    SendUDPList(Session, clientSocket.GetUDPList(ClientSocket::ListType_Client),
        TargetID_In, lpMsgBlock, sizeof(PktAI), CmdCharAddressInfo, 0, 0);

    return true;
}


bool SendPacket::CharMove(ClientSocket& clientSocket, unsigned long CharID_In, POS* Pos_In, 
                          float Dir_In, unsigned short UAct_In, unsigned short LAct_In, 
                          char Level_In, unsigned long ChantEf_In, unsigned long EnchantEf_In)
{
    ClientNet::CSession& Session = clientSocket.GetSession(ClientSocket::UDPSession);    
    CHECK_TRUE_RETURN(0 == CharID_In, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktMV), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);
    
    PktMV* lpMVPt = reinterpret_cast<PktMV *>(lpMsgBlock->wr_ptr());

    lpMVPt->m_dwCharID		= CharID_In;
    lpMVPt->m_Position		= *Pos_In;
    lpMVPt->m_fDir			= Dir_In;
    lpMVPt->m_wUAct			= UAct_In;
    lpMVPt->m_wLAct			= LAct_In;

    lpMVPt->m_wLevel		= Level_In;
    lpMVPt->m_dwChantEf		= ChantEf_In;
    lpMVPt->m_dwEnchantEf	= EnchantEf_In;

    SendUDPList(Session, clientSocket.GetUDPList(ClientSocket::ListType_Instance),
        clientSocket.IsRealIP(), lpMsgBlock, sizeof(PktMV), CmdCharMove, 0, 0);

    clientSocket.DeleteInstanceUDPList();
    return true;    
}
*/


bool SendPacket::CharMoveEx(ClientSocket& clientSocket, unsigned long CharID_In, 
                            CNetworkPos& netPos, unsigned char UAct_In, unsigned char LAct_In)
{
    ClientNet::CClientEventHandler* lpHandler = &clientSocket.GetHandler(ClientSocket::GameEventHandler);    

    CHECK_TRUE_RETURN(0 == CharID_In, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktMVEx), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);
    
    PktMVEx* lpPktMVEx = reinterpret_cast<PktMVEx*>(lpMsgBlock->wr_ptr());

    lpPktMVEx->m_NetworkPos = netPos;
    lpPktMVEx->m_cUAct = UAct_In;
    lpPktMVEx->m_cLAct = LAct_In;
	
    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktMVEx), CmdCharMove, CharID_In);
}

/* NOT USE
bool SendPacket::CharRequireInfo(ClientSocket& clientSocket, unsigned long SenderID_In, 
                                 unsigned long TargetID_In, unsigned char Cmd_In)
{
    ClientNet::CSession& Session = clientSocket.GetSession(ClientSocket::UDPSession);    
    CHECK_TRUE_RETURN(0 == SenderID_In || 0 == TargetID_In, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktRI), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);

    PktRI* lpRIPt = reinterpret_cast<PktRI *>(lpMsgBlock->wr_ptr());

    lpRIPt->m_dwCharID			= SenderID_In;
    lpRIPt->m_cCmd				= Cmd_In;
    lpRIPt->m_PublicAddress		= clientSocket.GetAddress(ClientSocket::PublicAddr);
    lpRIPt->m_PrivateAddress	= clientSocket.GetAddress(ClientSocket::PrivateAddr);

    SendUDPList(Session, clientSocket.GetUDPList(ClientSocket::ListType_Client),
        TargetID_In, lpMsgBlock, sizeof(PktRI), CmdCharRequireInfo, 0, 0);

    return true;
}
*/
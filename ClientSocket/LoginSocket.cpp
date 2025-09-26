///////////////////////////////////////////////////////////////////////////////////////////////
//
// ClientSocket Class
//
///////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "LoginSocket.h"


#include <Network/Packet/PacketBase.h>
#include <Network/Packet/PacketCommand.h>

#include <Network/Packet/PacketStruct/ServerInfo.h>
#include <Network/Packet/PacketStruct/ServerPacket.h>
#include <Network/Packet/PacketStruct/ClientToAuthServer.h>
#include <Network/Packet/PacketStruct/ClientToLoginServer.h>

#include <Network/ClientNetwork/ClientEventHandler.h>

#include <mmsystem.h>
#include "GMMemory.h"

LoginSocket::LoginSocket()
:   m_lpHandler(0), m_lpHandlerMgr(0)
{
    m_lpHandlerMgr = new ClientNet::CClientEventHandlerMgr;
}

LoginSocket::~LoginSocket(void)
{
    Destroy();
}

bool LoginSocket::ConnectToLoginServer(ClientNet::CClientEventHandler* lpEventHandler, 
                                       const char* Address_In, unsigned short port)
{
    CHECK_NULL_RETURN(lpEventHandler, false);
    CHECK_NULL_RETURN(m_lpHandlerMgr, false);

    if(!m_lpHandlerMgr->Connect(INET_Addr(Address_In, port), lpEventHandler))
    {
        fprintf(stderr, "LoginSocket::ConnectToLoginServer - Connect to LoginServer failed\n");
        return false;
    }

    return true;
}


void LoginSocket::Destroy()
{
    delete m_lpHandlerMgr;
}

void LoginSocket::HandleEvents(unsigned long dwProcessPerCall)
{
    if (0 != m_lpHandlerMgr)
    {
        m_lpHandlerMgr->HandleEvents(dwProcessPerCall);
    }
}

void LoginSocket::Disconnect(void)
{
    if(0 != m_lpHandlerMgr && 0 != m_lpHandler)
    {
        m_lpHandlerMgr->Close(m_lpHandler);
    }
}


bool LoginSocket::IsConnected()
{
    return (NULL != m_lpHandler);
}


//Interface/////////////////////////////////////////////////////////////////////////////////////
//
// GAS 서버 선택 [ public ]
// - GAS 서버 선택.
//
// Parameter :
//	1st : 유저 아이디[In]
//	2st : 서버 아이디[In]
//
// Return :
//	패킷 생성 후 보내기 성공시 true, 실패시 false
//
///////////////////////////////////////////////////////////////////////////////////////////////
bool LoginSocket::ServerList(DWORD ClientVersion_In)
{
    CHECK_NULL_RETURN(m_lpHandler, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(        
        sizeof(PktSvL), m_lpHandler->GetPeerAddress());

    if(NULL != lpMsgBlock)
    {
        PktSvL* lpPktSvL = reinterpret_cast<PktSvL*>(lpMsgBlock->wr_ptr());        
        lpPktSvL->m_dwClientVer= ClientVersion_In;

        if(lpMsgBlock->WrapCryptOld(sizeof(PktSvL), CmdLoginServerList, 0, PatcherModel))
        {
            m_lpHandlerMgr->SendPacket(m_lpHandler, lpMsgBlock);
            return true;
        }
    }    

    return false;
}

 
//Interface/////////////////////////////////////////////////////////////////////////////////////
//
// 패킷 처리 [ public ]
// - AUAckPt 처리
//
// Parameter :
//	1st : 패킷 버퍼[In]
//	2st : 로그인 캐릭터 정보 구조체[Out]
//
// Return :
//	에러 코드
//
///////////////////////////////////////////////////////////////////////////////////////////////
unsigned long LoginSocket::HandleServerList(PktBase* lpPktBase, DWORD* ClientVer_Out, 
                                            char* PatchAddress_Out, SERVER_LIST* lpServerList_Out)
{
	PktSvLAck* lpPktSvLAck = static_cast<PktSvLAck*>(lpPktBase);

    *ClientVer_Out = lpPktSvLAck->m_dwClientVer;
	strncpy(PatchAddress_Out, lpPktSvLAck->m_PatchAddress, PktSvLAck::PATCH_ADDRESS_LENGTH);
	*lpServerList_Out = lpPktSvLAck->m_ServerList;

	return lpPktSvLAck->GetError();
}


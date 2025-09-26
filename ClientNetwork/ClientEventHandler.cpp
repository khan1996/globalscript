#include "stdafx.h"
#include "ClientEventHandler.h"

#include <Network/Packet/PacketBase.h>
#include <Network/Packet/PacketCommand.h>
#include <Network/Packet/PacketStruct/ServerPacket.h>
#include <Network/XORCrypt/XORCrypt.h>
#include <Network/ClientNetwork/Session.h>
#include <Utility/Compress/MiniLZO/miniLZOWrapper.h>
#include <Thread/ThreadMgr.h>

#include <mmsystem.h>
#include "GMMemory.h"

namespace ClientNet
{

CClientEventHandler::CClientEventHandler()
:   m_lpEventHandlerMgr(0),
    m_dwHandlerID(0), m_dwSessionID(0), 
    m_bCloseCalled(false)
{

}

CClientEventHandler::~CClientEventHandler()
{

}

bool CClientEventHandler::SendPacket(CNetworkMsgBlock* lpMsgBlock)
{
    return (0 != m_lpEventHandlerMgr) ? 
        m_lpEventHandlerMgr->SendPacket(this, lpMsgBlock) : false;
}

CClientEventHandlerMgr::CClientEventHandlerMgr()
:   m_dwMaxEventHandlerID(0), m_bExit(FALSE)
{
    m_SessionMgr.SetDispatchQueue(m_LockedMsgQueue);

    CThreadMgr::Run(this);
}

CClientEventHandlerMgr::~CClientEventHandlerMgr()
{
    // 보유중인 모든 Handler에 대해서, Close를 호출한다.
    ClientEventHanderMap::iterator pos = m_ClientEventHandlerMap.begin();
    ClientEventHanderMap::iterator end = m_ClientEventHandlerMap.end();

    for(; pos != end; ++pos)
    {
        Close(pos->second);
    }
    
    // 2초동안 컨넥션이 전부 끊기기를 기다린다.
    DWORD dwLastTime = timeGetTime();

    do
    {        
        HandleEvents(0xFFFFFFFF);
        Sleep(100);
    }
    while(timeGetTime() - dwLastTime < 5000 && !m_ClientEventHandlerMap.empty());

    // 스레드 중지.
    End();
    CThreadMgr::Stop(this, 2000);

    // 데이터 전부 삭제
    pos = m_ClientEventHandlerMap.begin();
    end = m_ClientEventHandlerMap.end();

    for(; pos != end; ++pos)
    {
        delete pos->second;
    }    
}


void CClientEventHandlerMgr::HandleEvents(unsigned long dwProcessPerCall)
{
    m_LockedMsgQueue.GetMessages(m_LocalMsgQueue, ULONG_MAX);

    // 얻어온 메시지를 적당히 처리한다.

    CNetworkMsgBlock* lpMsgBlock = 0;

    ClientEventHanderMap::iterator pos;
    ClientEventHanderMap::iterator end = m_ClientEventHandlerMap.end();

    for (DWORD dwCount = 0; dwCount < dwProcessPerCall; ++dwCount)
    {
        lpMsgBlock = m_LocalMsgQueue.Dequeue();

        if(0 == lpMsgBlock)
        {
            break;
        }

        pos = m_ClientEventHandlerMap.find(lpMsgBlock->GetID());

        if(pos != end)
        {
            // 이미 생성된 Handler에 대한 연결이다.
            CClientEventHandler* lpHandler = pos->second;

            switch(lpMsgBlock->GetMsgType())
            {
            case NET_MSG_ACK_DISPATCH:

                if (-1 == lpHandler->OnDispatch(
                    reinterpret_cast<PktBase*>(lpMsgBlock->rd_ptr()), lpMsgBlock->GetAddress()))
                {
                    Close(lpHandler);
                }
                break;

            case NET_MSG_ACK_OPEN:

                {
                    // 능동 연결시엔 Handler가 이미 존재한다.
                    int     nErrorCode  = 0;
                    DWORD   dwSessionID = 0;

                    lpMsgBlock->pop(nErrorCode);
                    lpMsgBlock->pop(dwSessionID);

                    lpHandler->SetSessionID(dwSessionID);
                    if(-1 == lpHandler->OnOpen(nErrorCode))
                    {
                        // 세션 열기 실패. 세션 닫기를 호출한다.                        
                        if(0 == nErrorCode)
                        {
                            Close(lpHandler);
                        }
                    }

                    if (0 != nErrorCode)
                    {
                        lpHandler->OnClose();
                    }
                }
                break;

            case NET_MSG_ACK_CLOSE:

                lpHandler->OnClose();

                // 핸들러를 제거합니다.
                delete lpHandler;
                
                m_ClientEventHandlerMap.erase(pos);
                end = m_ClientEventHandlerMap.end();
                break;
            }
        }
        else
        {
            // TODO : Accept를 통한 연결시에는, Handler가 존재하지 않는다. 관련 처리를 이곳에서 해주고, 
            // 메시지를 한번 더 돌려줘야 한다.(CThreadEventHandler에 EventID를 설정할 수 있도록..)


        }

        lpMsgBlock->SetMsgType(NET_MSG_FREE);
        CNetworkMsgPool::GetInstance().FreeNetworkMsgBlock(lpMsgBlock);
    }
}

bool CClientEventHandlerMgr::Connect(INET_Addr& connAddr, CClientEventHandler* lpEventHandler)
{
    if (0 == lpEventHandler)
    {
        fprintf(stderr, "CClientEventHandlerMgr::Connect - EventHandler is NULL\n");
        return false;
    }

    CNetworkMsgBlock* lpMsgBlock = 
        CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(1, connAddr);

    if(0 == lpMsgBlock || !lpMsgBlock->is_valid())
    {
        fprintf(stderr, "CClientEventHandlerMgr::Connect - NetMsgBlock Create Failed\n");
        return false;
    }

    DWORD dwHandlerID = 0;

    ClientEventHanderMap::iterator pos;
    ClientEventHanderMap::iterator end = m_ClientEventHandlerMap.end();
    
    do
    {
        // 빈 세션ID를 찾는다.
        dwHandlerID = ++m_dwMaxEventHandlerID;
        pos = m_ClientEventHandlerMap.find(dwHandlerID);
    } 
    while(pos != end);

    m_ClientEventHandlerMap.insert(
        ClientEventHanderMap::value_type(dwHandlerID, lpEventHandler));

    lpEventHandler->SetHandlerID(dwHandlerID);
    lpEventHandler->SetPeerAddress(connAddr);
    lpEventHandler->SetEventHandlerMgr(this);

    lpMsgBlock->SetAddress(connAddr);
    lpMsgBlock->SetMsgType(NET_MSG_REQ_TCP_OPEN);
    lpMsgBlock->SetID(dwHandlerID);
    m_SessionMgr.PutMessage(lpMsgBlock);
    return true;
}

bool CClientEventHandlerMgr::Close(CClientEventHandler* lpEventHandler)
{
    CNetworkMsgBlock* lpMsgBlock = 
        CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(1, INET_Addr());

    if(0 == lpMsgBlock || !lpMsgBlock->is_valid())
    {
        fprintf(stderr, "CClientEventHandlerMgr::Close - NetMsgBlock Create Failed\n");
        return false;
    }

    lpEventHandler->SetCloseCalled();

    lpMsgBlock->SetMsgType(NET_MSG_REQ_CLOSE);
    lpMsgBlock->SetID(lpEventHandler->GetSessionID());
    m_SessionMgr.PutMessage(lpMsgBlock);
    return true;
}

bool CClientEventHandlerMgr::SendPacket(CClientEventHandler* lpEventHandler, CNetworkMsgBlock* lpMsgBlock)
{
    if (0 != lpEventHandler && 0 != lpMsgBlock)
    {
        lpMsgBlock->SetMsgType(NET_MSG_REQ_SEND);
        lpMsgBlock->SetID(lpEventHandler->GetSessionID());

        m_SessionMgr.PutMessage(lpMsgBlock);
        return true;
    }

    return false;
}


unsigned int CClientEventHandlerMgr::Run()
{
    while(!InterlockedCompareExchange(&m_bExit, TRUE, TRUE))
    {
        m_SessionMgr.HandleEvents(1000);
    }

    return true;
}

BOOL CClientEventHandlerMgr::End()
{
    InterlockedExchange(&m_bExit, TRUE);
    return TRUE;
}


CThreadEventHandler::CThreadEventHandler(CLockedMsgQueue& LockedMsgQueue, DWORD dwClientHandlerID)
:   m_LockedMsgQueue(LockedMsgQueue),
    m_Crypt(CXORCrypt::GetInstance()), 
    m_dwClientHandlerID(dwClientHandlerID),
    CEventHandler(60)   // 1분마다 호출한다.
{

}

// 패킷을 받았을 때 불린다. 자체적으로 메시지 큐잉을 수행한다.
int CThreadEventHandler::OnReceived(CNetworkMsgBlock& msg_block)
{
    const int MAX_PACKET_PER_RECV = 32;

    char*       rd_ptr          = 0;
    PktBase*	lpPktBase       = 0;
    DWORD		length          = static_cast<unsigned long>(msg_block.length());
    DWORD       decompress_len  = 0;

    CNetworkMsgBlock* lpNewMsgBlock = 0;
    CNetworkMsgPool& netMessagePool = CNetworkMsgPool::GetInstance();

    CNetworkMsgQueue netMsgQueue;

    while(sizeof(PktBase) <= length)
    {
        rd_ptr    = msg_block.rd_ptr();
        lpPktBase = reinterpret_cast<PktBase*>(rd_ptr);

        // 패킷 헤더가 vaild한지 확인한다. invalid한 경우는 그냥 끊어버린다.
        if(StartBit != lpPktBase->GetStartBit())
        {
            fprintf(stderr, "CClientEventHandler::OnReceived - Packet header is invalid. Not equal startbit\n");
            return -1;
        }

        // 패킷 헤더 디코딩
        m_Crypt.DecodeHeader(rd_ptr + 1, sizeof(PktBase) - 1, 0, 0);

        // 헤더 길이 얻어 오기.
        const PktBase::CMDType		nPacketCMD		= lpPktBase->GetCmd();
        const PktBase::LengthType	nPacketLength	= lpPktBase->GetLen();

        if(length < nPacketLength)
        {
            // 전체 스트림 사이즈가 파싱을 하기에는 모자람. 다시 인코딩 한 후 다음 턴을 기다림
            m_Crypt.EncodeHeader(rd_ptr + 1, sizeof(PktBase) - 1, 0, 0);
            break;
        }
        else
        {
            // 패킷 디코딩.
            if(lpPktBase->IsCrypt())
            {
                m_Crypt.DecodePacket(rd_ptr + sizeof(PktBase), 
                    nPacketLength - sizeof(PktBase), lpPktBase->GetCodePage());
            }

            // 패킷 압축 해제.
            if(lpPktBase->IsCompress())
            {
                decompress_len = PktMaxLen - sizeof(PktBase);

                lpNewMsgBlock = netMessagePool.GetNetworkMsgBlock(PktMaxLen, msg_block.GetAddress());
                if(0 == lpNewMsgBlock)
                {
                    fprintf(stderr, "CClientEventHandler::OnReceived - Create NetworkMsgBlock Failed for Decompress\n");
                    return -1;
                }

                char* szDecompressedPacket = lpNewMsgBlock->wr_ptr();
                memcpy(szDecompressedPacket, lpPktBase, sizeof(PktBase));

                // 로컬 버퍼에서 압축 해제.
                if(!CMiniLZO::Decompress(rd_ptr + sizeof(PktBase), nPacketLength - sizeof(PktBase), 
                    szDecompressedPacket + sizeof(PktBase), &decompress_len))
                {
                    netMessagePool.FreeNetworkMsgBlock(lpNewMsgBlock);
                    fprintf(stderr, "CClientEventHandler::OnReceived - Decompress Packet Failed\n");
                    return -1;
                }

                lpNewMsgBlock->wr_ptr(decompress_len + sizeof(PktBase));
                lpPktBase = reinterpret_cast<PktBase*>(lpNewMsgBlock->rd_ptr());

                lpPktBase->SetLen(static_cast<PktBase::LengthType>(decompress_len + sizeof(PktBase)));
            }
            else
            {
                lpNewMsgBlock = netMessagePool.GetNetworkMsgBlock(nPacketLength, msg_block.GetAddress());
                if(0 == lpNewMsgBlock)
                {
                    fprintf(stderr, "CClientEventHandler::OnReceived - Create NetworkMsgBlock Failed for Enqueue\n");
                    return -1;
                }

                memcpy(lpNewMsgBlock->wr_ptr(), rd_ptr, nPacketLength);
                lpNewMsgBlock->wr_ptr(nPacketLength);
            }

            lpNewMsgBlock->SetMsgType(NET_MSG_ACK_DISPATCH);
            lpNewMsgBlock->SetID(m_dwClientHandlerID);
            netMsgQueue.Enqueue(lpNewMsgBlock);

            length -= nPacketLength;
            msg_block.rd_ptr(nPacketLength);
        }
    }

    m_LockedMsgQueue.PutMessages(netMsgQueue);
    return 0;
}

int CThreadEventHandler::OnOpen(CSession& ConnSession, int iErrorCode)
{
    CNetworkMsgBlock* lpNewMsgBlock = 
        CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(
        sizeof(int) + sizeof(unsigned long), INET_Addr());

    if(0 == lpNewMsgBlock)
    {
        fprintf(stderr, "CThreadEventHandler::OnOpen - NetMsgBlock Create Failed\n");
        return -1;
    }

    lpNewMsgBlock->SetMsgType(NET_MSG_ACK_OPEN);
    lpNewMsgBlock->SetID(m_dwClientHandlerID);
    lpNewMsgBlock->push(iErrorCode);
    lpNewMsgBlock->push(ConnSession.GetSessionID());
    
    m_LockedMsgQueue.PutMessage(lpNewMsgBlock);
    return 0;
}

int CThreadEventHandler::OnClose(CSession& CloseSession)
{
    CNetworkMsgBlock* lpNewMsgBlock = 
        CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(1, INET_Addr());

    if(0 == lpNewMsgBlock)
    {
        fprintf(stderr, "CThreadEventHandler::OnClose - NetMsgBlock Create Failed\n");
        return -1;
    }

    lpNewMsgBlock->SetMsgType(NET_MSG_ACK_CLOSE);
    lpNewMsgBlock->SetID(m_dwClientHandlerID);
    m_LockedMsgQueue.PutMessage(lpNewMsgBlock);
    return 0;
}

int CThreadEventHandler::OnTimeout(CSession& Session)
{
    // Ping을 보낸다.
    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktSyP), INET_Addr());

    if(0 != lpMsgBlock)
    {
        PktSyP* lpSyPAckPt = reinterpret_cast<PktSyP*>(lpMsgBlock->wr_ptr());
        lpSyPAckPt->m_dwTickTime = timeGetTime();

        if (lpMsgBlock->WrapCrypt(sizeof(PktSyP), CmdSysPing, 0))
        {
            Session.SendPending(lpMsgBlock);
        }
		else
		{
			CNetworkMsgPool::GetInstance().FreeNetworkMsgBlock(lpMsgBlock);
		}
    }

    return 0;
}



}
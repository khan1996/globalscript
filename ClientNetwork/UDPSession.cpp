#include "stdafx.h"
#include "UDPSession.h"
#include "SessionMgr.h"
#include "EventHandler.h"
#include <cstdio>
#include "GMMemory.h"

ClientNet::CUDPSession::CUDPSession(DWORD dwSessionID, CEventHandler* lpEventHandler)
:   CSession(dwSessionID, lpEventHandler)
{
    

}


ClientNet::CUDPSession::~CUDPSession()
{

}


int ClientNet::CUDPSession::Open(INET_Addr& addr)
{
    if(INVALID_SOCKET != m_hSocket)
    {
        Close();
    }

    m_hSocket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, NULL, WSA_FLAG_OVERLAPPED);
    if(INVALID_SOCKET == m_hSocket)
    {
        fprintf(stderr, "ClientNet::CUDPSession::Open - Socket create error : %d\n", WSAGetLastError());
        return -1;
    }

    BOOL bReuseAddr = TRUE;
    int rc = setsockopt(m_hSocket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&bReuseAddr), sizeof(BOOL));
    if(SOCKET_ERROR == rc)
    {
        fprintf(stderr, "ClientNet::CUDPSession::Open - setsockopt error : %d\n", WSAGetLastError());
        return -1;
    }

    rc = bind(m_hSocket, &addr.get_addr(), addr.get_size());
    if(SOCKET_ERROR == rc)
    {
        fprintf(stderr, "ClientNet::CUDPSession::Open - bind error : %d\n", WSAGetLastError());
        return -1;
    }

    // EventSelect
    rc = WSAEventSelect(m_hSocket, m_hEvent, FD_READ | FD_WRITE);
    if(SOCKET_ERROR == rc)
    {
        fprintf(stderr, "ClientNet::CUDPSession::Open - EventSelect error : %d\n", WSAGetLastError());
        return -1;
    }

    return 0;
}



int ClientNet::CUDPSession::Recv()
{
    WSABUF  wsaBuf;

    for(unsigned long dwReadCount = 0;
        dwReadCount < m_dwMaxRecvNumPerPulse || 0 == m_Received.remain(); 
        ++dwReadCount)
    {
        wsaBuf.buf = m_Received.wr_ptr();
        wsaBuf.len = static_cast<unsigned long>(m_Received.remain());

        DWORD dwReceived = 0;
        DWORD dwFlags = 0;

        INET_Addr& addr = m_Received.GetAddress();
		int addr_size	= INET_Addr::get_addr_buffer_size();
        int rc = WSARecvFrom(m_hSocket, &wsaBuf, 1, &dwReceived, &dwFlags, 
            &addr.get_addr(), &addr_size, NULL, NULL);

        if(SOCKET_ERROR == rc)
        {
            if(WSAEWOULDBLOCK != WSAGetLastError())
            {
                fprintf(stderr, "ClientNet::CUDPSession::Recv - WSARecvFrom error : %d\n", WSAGetLastError());
            }

			return 0;
        }

        addr.set_size(addr_size);
        m_Received.wr_ptr(dwReceived);

        if(-1 == m_lpEventHandler->OnReceived(m_Received) || 0 != m_Received.length())
        {
            fprintf(stderr, "ClientNet::CUDPSession::Recv - UDP packet is invalid\n");
        }

        m_Received.pop_read_data();        
    }

    return 0;
}


int ClientNet::CUDPSession::Send(unsigned long dwMaxSendCount)
{
    CNetworkMsgBlock* lpMsgBlock = m_SendQueue.Dequeue();

    WSABUF  wsaBuf;
    DWORD   dwSendCompleted = 0;
    DWORD   dwFlags = 0;

    int rc = 0;

    for(unsigned long dwSendCount = 0; 
        NULL != lpMsgBlock && dwSendCount < dwMaxSendCount; ++dwSendCount)
    {
        wsaBuf.buf = lpMsgBlock->rd_ptr();
        wsaBuf.len = static_cast<unsigned long>(lpMsgBlock->length());

        dwSendCompleted = 0;
        INET_Addr& addr = lpMsgBlock->GetAddress();
                
        rc = WSASendTo(m_hSocket, &wsaBuf, 1, &dwSendCompleted, dwFlags, 
            &addr.get_addr(), addr.get_size(), NULL, NULL);

        if(SOCKET_ERROR == rc)
        {
            rc = 0;

            if(WSAEWOULDBLOCK == WSAGetLastError())
            {
                // WSAEWOULDBLOCK이면 나중에 보내고
                break;
            }
            else
            {
                // 소켓 에러면, 패킷을 버린다.
                fprintf(stderr, "ClientNet::CUDPSession::Send - WSASendTo error : %d\n", WSAGetLastError());
            }
        }

        CNetworkMsgPool::GetInstance().FreeNetworkMsgBlock(lpMsgBlock);
        lpMsgBlock = m_SendQueue.Dequeue();
    }

    if(NULL != lpMsgBlock)
    {
        m_SendQueue.Enqueue(lpMsgBlock, true);
    }
    
    return (SOCKET_ERROR != rc) ? 0 : -1;
}

#include "stdafx.h"
#include "TCPSession.h"
#include "SessionMgr.h"
#include "EventHandler.h"
#include <cstdio>
#include "GMMemory.h"

ClientNet::CTCPSession::CTCPSession(DWORD dwSessionID, CEventHandler* lpEventHandler)
:   CSession(dwSessionID, lpEventHandler), m_bClosing(FALSE)
{
    
}

ClientNet::CTCPSession::~CTCPSession()
{
    
}


int ClientNet::CTCPSession::Open(INET_Addr& addr)
{
    if(!IsValid())
    {
        fprintf(stderr, "ClientNet::CTCPSession::Open - Invalid Session, Can't open now.\n");
        return -1;
    }

    if(INVALID_SOCKET != m_hSocket)
    {
        Close();
    }

    m_hSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);
    if(INVALID_SOCKET == m_hSocket)
    {
        fprintf(stderr, "ClientNet::CTCPSession::Open - Socket create error : %d\n", WSAGetLastError());
        return -1;
    }

    // EventSelect
    int rc = WSAEventSelect(m_hSocket, m_hEvent, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);
    if(SOCKET_ERROR == rc)
    {
        fprintf(stderr, "ClientNet::CTCPSession::Open - EventSelect error : %d\n", WSAGetLastError());
        return -1;
    }

    // connect
    rc = connect(m_hSocket, &addr.get_addr(), addr.get_size());
    if(SOCKET_ERROR == rc)
    {
        int nError = WSAGetLastError();
        if(WSAEWOULDBLOCK != nError)
        {            
            fprintf(stderr, "ClientNet::CTCPSession::Open - Connect error : %d\n", nError);
            return -1;
        }
    }

    m_PeerAddress = addr;
    return 0;
}

int ClientNet::CTCPSession::Open(INET_Addr& addr, SOCKET hSocket)
{
    if(!IsValid())
    {
        fprintf(stderr, "ClientNet::CTCPSession::Open - Invalid Session, Can't open now.\n");
        return -1;
    }

    if(INVALID_SOCKET == hSocket)
    {
        fprintf(stderr, "ClientNet::CTCPSession::Open - Invalid Socket, Can't open now.\n");
        return -1;
    }

    Close();
    m_hSocket = hSocket;
    
    // EventSelect
    int rc = WSAEventSelect(m_hSocket, m_hEvent, FD_READ | FD_WRITE | FD_CLOSE);
    if(SOCKET_ERROR == rc)
    {
        fprintf(stderr, "ClientNet::CTCPSession::Open - EventSelect error : %d\n", WSAGetLastError());
        return -1;
    }

    m_PeerAddress = addr;
    return m_lpEventHandler->OnOpen(*this, 0);    
}



int ClientNet::CTCPSession::Recv()
{
    WSABUF wsaBuf;
    
    for(unsigned long dwReadCount = 0;
        dwReadCount < m_dwMaxRecvNumPerPulse || 0 == m_Received.remain(); 
        ++dwReadCount)
    {
        wsaBuf.buf = m_Received.wr_ptr();
        wsaBuf.len = static_cast<unsigned long>(m_Received.remain());

        DWORD dwReceived = 0;
        DWORD dwFlags = 0;
       
        int rc = WSARecv(m_hSocket, &wsaBuf, 1, &dwReceived, &dwFlags, NULL, NULL);
        if(SOCKET_ERROR == rc)
        {
            if(WSAEWOULDBLOCK != WSAGetLastError())
            {
                fprintf(stderr, "ClientNet::CTCPSession::Recv - WSARecv error : %d\n", WSAGetLastError());
                return -1;
            }
            else
            {
                return 0;
            }
        }

        m_Received.wr_ptr(dwReceived);
        
        if(-1 == m_lpEventHandler->OnReceived(m_Received))
        {
            fprintf(stderr, "ClientNet::CTCPSession::Recv - TCP Packet is Invalid\n");
        }

        m_Received.pop_read_data();

        if(0 == dwReceived)
        {
            m_bClosing = TRUE;
            if(0 == m_SendQueue.GetQueuedNum())
            {
                return -1;
            }
        }
    }
    
    return 0;
}


int ClientNet::CTCPSession::Send(unsigned long dwMaxSendCount)
{
    CNetworkMsgBlock* lpMsgBlock = m_SendQueue.Dequeue();

    WSABUF wsaBuf;
    DWORD  dwSendCompleted = 0;
    DWORD  dwFlags = 0;

    int rc = 0;

    for(unsigned long dwSendCount = 0; 
        NULL != lpMsgBlock && dwSendCount < dwMaxSendCount; ++dwSendCount)
    {
        wsaBuf.buf = lpMsgBlock->rd_ptr();
        wsaBuf.len = static_cast<unsigned long>(lpMsgBlock->length());

        dwSendCompleted = 0;

        rc = WSASend(m_hSocket, &wsaBuf, 1, &dwSendCompleted, dwFlags, NULL, NULL);
        if(SOCKET_ERROR == rc)
        {
            if(WSAEWOULDBLOCK != WSAGetLastError())
            {
                fprintf(stderr, "ClientNet::CTCPSession::Send - WSASend error : %d\n", WSAGetLastError());
            }
            else
            {
                // WSAEWOULDBLOCK is NOT error
                rc = 0;
            }

            break;
        }

        lpMsgBlock->rd_ptr(dwSendCompleted);

        if(0 == lpMsgBlock->length())
        {
            CNetworkMsgPool::GetInstance().FreeNetworkMsgBlock(lpMsgBlock);
            lpMsgBlock = m_SendQueue.Dequeue();
        }
    }

    if(NULL != lpMsgBlock)
    {
        m_SendQueue.Enqueue(lpMsgBlock, true);
    }
    
    if(m_bClosing && 0 == m_SendQueue.GetQueuedNum())
    {        
        return -1;
    }

    return (SOCKET_ERROR != rc) ? 0 : -1;
}

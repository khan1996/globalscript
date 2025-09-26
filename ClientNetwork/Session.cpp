#include "stdafx.h"
#include "Session.h"
#include "SessionMgr.h"
#include "EventHandler.h"

#include <cstdio>
#include "GMMemory.h"

const int RECEIVE_BLOCK_SIZE        = 65535;
const int MAX_RECV_NUM_PER_PULSE    = 20;
const int MAX_SEND_NUM_PER_PULSE    = 20;

namespace ClientNet
{
    enum SessionFlags
	{
		SHUTDOWN_CALLED		= (1 << 1),
		CLOSE_CALLED		= (1 << 2)
	};
}

ClientNet::CSession::CSession(DWORD dwSessionID, CEventHandler* lpEventHandler)
:   m_dwSessionID(dwSessionID), 
    m_hSocket(INVALID_SOCKET), 
    m_hEvent(WSACreateEvent()), 
    m_lpEventHandler(lpEventHandler),
    m_Received(RECEIVE_BLOCK_SIZE),
    m_dwMaxRecvNumPerPulse(MAX_RECV_NUM_PER_PULSE),
    m_dwMaxSendNumPerPulse(MAX_SEND_NUM_PER_PULSE),
	m_dwFlags(0)
{
    memset(m_szSessionName, 0, sizeof(char) * MAX_SESSION_NAME);
}


ClientNet::CSession::~CSession()
{
    Close();

    if(WSA_INVALID_EVENT != m_hEvent)
    {
        WSACloseEvent(m_hEvent);
        m_hEvent = WSA_INVALID_EVENT;
    }

    delete m_lpEventHandler;
    m_lpEventHandler = 0;
}


bool ClientNet::CSession::IsValid()
{
    if(NULL == m_lpEventHandler)
    {
        fprintf(stderr, "ClientNet::CSession::IsValid - Handler is Invalid (NULL)\n");
        return false;
    }
    
    if(WSA_INVALID_EVENT == m_hEvent)
    {
        fprintf(stderr, "ClientNet::CSession::IsValid - Event is Invalid (WSA_INVALID_EVENT)\n");
        return false;
    }

    if(!m_Received.is_valid())
    {
        fprintf(stderr, "ClientNet::CSession::IsValid - Receive MsgBlock is Invalid\n");
        return false;
    }

    return true;
}

int ClientNet::CSession::Shutdown(int how)
{
    if(INVALID_SOCKET != m_hSocket)
    {
        bool bFailed = false;

        while(!bFailed && 0 != m_SendQueue.GetQueuedNum())
        {                        
			bFailed = (0 == Send(0xFFFFFFFF)) ? false : true;
        }
        
        m_dwFlags |= SHUTDOWN_CALLED;

        if(SOCKET_ERROR == shutdown(m_hSocket, how))
        {
            return -1;
        }
    }

    return 0;
}


int ClientNet::CSession::Close()
{
    if(INVALID_SOCKET != m_hSocket)
    {
        Shutdown(SD_SEND);

        if(SOCKET_ERROR == closesocket(m_hSocket))
        {
            fprintf(stderr, "ClientNet::CSession::Close - closesocket error : %d\n", WSAGetLastError());
            return -1;
        }
		
        m_hSocket = INVALID_SOCKET;
		m_dwFlags |= CLOSE_CALLED;
    }

    return 0;
}

bool ClientNet::CSession::IsShutdownCalled() const 
{
	return SHUTDOWN_CALLED == (m_dwFlags & SHUTDOWN_CALLED); 
}

bool ClientNet::CSession::IsCloseCalled() const 
{
	return CLOSE_CALLED == (m_dwFlags & CLOSE_CALLED);
}

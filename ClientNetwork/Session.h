#ifndef _GAMA_CLIENT_SESSION_H_
#define _GAMA_CLIENT_SESSION_H_

#include <winsock2.h>
#include <windows.h>

#include <Network/Address/INET_Addr.h>
#include "NetworkMsgBlock.h"

namespace ClientNet
{
    // forward decl;
    class CSessionMgr;
    class CEventHandler;

    class CSession
    {
    public:

        enum
        {
            MAX_SESSION_NAME = 16
        };

        CSession(DWORD dwSessionID, CEventHandler* lpEventHandler);
        virtual ~CSession();
        
        DWORD       GetSessionID()  const { return m_dwSessionID;   }
        SOCKET      GetSocket()     const { return m_hSocket;       }
        WSAEVENT    GetEvent()      const { return m_hEvent;        }
        INET_Addr&  GetPeerAddress()      { return m_PeerAddress;   }

        CEventHandler* GetEventHandler() { return m_lpEventHandler; }
        void SetEventHandler(CEventHandler* lpEventHandler) { m_lpEventHandler = lpEventHandler; }

        bool IsValid();
        
        // Active Mode
        int Open(const char* addr, unsigned short port) { return Open(INET_Addr(addr, port)); }
        
        virtual int Open(INET_Addr& addr) = 0;                  // Active Socket
        virtual int Open(INET_Addr& addr, SOCKET hSocket) = 0;  // Passive Socket
        virtual int Recv() = 0;
        virtual int Send(unsigned long dwMaxSendCount) = 0;

        int Send() { return Send(m_dwMaxSendNumPerPulse); }
        void SendPending(CNetworkMsgBlock* lpMsgBlock) { m_SendQueue.Enqueue(lpMsgBlock); Send(1); } 

        int Shutdown(int how);
        int Close();

		bool IsShutdownCalled() const;
		bool IsCloseCalled() const;

        const char* GetSessionName() { return m_szSessionName; }

    protected:
       
        DWORD               m_dwSessionID;

        SOCKET              m_hSocket;
        WSAEVENT            m_hEvent;        
        CEventHandler*      m_lpEventHandler;

        unsigned long       m_dwMaxRecvNumPerPulse;
        unsigned long       m_dwMaxSendNumPerPulse;
		unsigned long		m_dwFlags;

        INET_Addr           m_PeerAddress;

        CNetworkMsgQueue    m_SendQueue;
        CNetworkMsgBlock    m_Received;

        char                m_szSessionName[MAX_SESSION_NAME];
    };
}


#endif
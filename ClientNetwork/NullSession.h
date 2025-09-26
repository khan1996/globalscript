#ifndef _GAMA_CLIENT_NULL_SESSION_H_
#define _GAMA_CLIENT_NULL_SESSION_H_

#include "Session.h"
#include "NullHandler.h"
#include <cstdio>

namespace ClientNet
{
    class NullSession : public CSession
    {
    public:

        NullSession(CSessionMgr& SessionMgr)
            :   CSession(SessionMgr, &NullHandler::GetInstance())
        {

        }        

        virtual ~NullSession() { m_lpEventHandler = NULL; }
        virtual CNetworkMsgBlock* GetNetworkMsgBlock(unsigned long dwSize, INET_Addr& addr, unsigned char cCmd)
        {
            fprintf(stderr, "SessionName:%s\tCmd:0x%02x\tReqSize:%6u\tNullSession cannot create message block!.\n", 
                GetSessionName(), cCmd, dwSize);
            return NULL; 
        }

        virtual int Open(INET_Addr& addr) { return 0; }
        virtual int Open(INET_Addr& addr, SOCKET hSocket) { return 0; }  // Passive Socket
        virtual int Recv() { return 0; }
        virtual int Send(unsigned long dwMaxSendCount) { return 0; }
    };
}


#endif
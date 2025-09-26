#ifndef _GAMA_CLIENT_UDP_SESSION_H_
#define _GAMA_CLIENT_UDP_SESSION_H_

#include "Session.h"


namespace ClientNet
{
    class CSessionMgr;

    class CUDPSession : public CSession
    {
    public:

        CUDPSession(DWORD dwSessionID, CEventHandler* lpEventHandler);

        virtual ~CUDPSession();

        virtual int Open(INET_Addr& addr);
        virtual int Open(INET_Addr& addr, SOCKET hSocket) { return -1; }
        virtual int Recv();
        virtual int Send(unsigned long dwMaxSendCount);
    };
}


#endif
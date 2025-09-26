#ifndef _GAMA_CLIENT_TCP_SESSION_H_

#include "Session.h"


namespace ClientNet
{
    class CTCPSession : public CSession
    {
    public:

        CTCPSession(DWORD dwSessionID, CEventHandler* lpEventHandler);

        virtual ~CTCPSession();
        virtual int Open(INET_Addr& addr);                  // Active Socket
        virtual int Open(INET_Addr& addr, SOCKET hSocket);  // Passive Socket

        virtual int Recv();
        virtual int Send(unsigned long dwMaxSendCount);

    protected:

        BOOL    m_bClosing;
    };
}


#endif
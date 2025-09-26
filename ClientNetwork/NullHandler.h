#ifndef _GAMA_CLIENT_NET_NULL_HANDLER_H_
#define _GAMA_CLIENT_NET_NULL_HANDLER_H_

#include "ClientEventHandler.h"

namespace ClientNet
{
    class CNullClientEventHandler : public CClientEventHandler
    {
    public:

        static CNullClientEventHandler& GetInstance() 
        { 
            static CNullClientEventHandler nullHandler; 
            return nullHandler;
        }

        virtual int OnDispatch(PktBase* lpPktBase, INET_Addr& peerAddr) { return 0; }
        virtual int OnOpen(int iErrorCode) { return 0; }
        virtual int OnClose() { return 0; }
    };
}

#endif
#ifndef _GAMA_CLIENT_PACKET_SEND_H_
#define _GAMA_CLIENT_PACKET_SEND_H_

#include <cstdio>

#define _QUOTE(x) # x
#define QUOTE(x) _QUOTE(x)


#ifdef _DEBUG

#define CHECK_NULL_RETURN(value, retval)\
    if(0 == (value)) { fprintf(stderr, QUOTE(__FUNCTION__) " - " QUOTE(value) " is NULL\n"); return (retval); }

#define CHECK_TRUE_RETURN(value, retval)\
    if((value)) { fprintf(stderr, QUOTE(__FUNCTION__) " - " QUOTE(value) " is TRUE\n"); return (retval); }

#else

#define CHECK_NULL_RETURN(value, retval) if(0 == (value)) { return (retval); }
#define CHECK_TRUE_RETURN(value, retval) if((value))      { return (retval); }

#endif

namespace ClientNet
{
    // forward decl.
    class CClientEventHandler;
    class CNetworkMsgBlock;
}


namespace SendPacket
{
    bool SendPacket(ClientNet::CClientEventHandler* lpHandler, 
        ClientNet::CNetworkMsgBlock* lpNetworkMsgBlock,
        unsigned short Len_In, unsigned char Cmd_In, 
        unsigned short State_In, unsigned short Error_In);

    bool SendPacket(ClientNet::CClientEventHandler* lpHandler, 
        ClientNet::CNetworkMsgBlock* lpNetworkMsgBlock,
        unsigned short Len_In, unsigned char Cmd_In, unsigned long Tick_In);
};

#endif
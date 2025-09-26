#ifndef _GAMA_CLIENT_SEND_MOVE_H_
#define _GAMA_CLIENT_SEND_MOVE_H_

#include <Network/Address/INET_Addr.h>

// 전방 참조
class ClientSocket;
class CNetworkPos;
struct POS;

namespace ClientNet
{
    // forward decl.
    class CClientEventHandler;
}

namespace SendPacket
{
    bool CharMoveUpdate(ClientNet::CClientEventHandler* lpHandler, unsigned long CharID_In, bool SitMode_In, POS* Pos_In, float Dir_In);

    bool CharMoveEx(ClientSocket& clientSocket, unsigned long CharID_In, 
        CNetworkPos& netPos, unsigned char UAct_In, unsigned char LAct_In);

    /* DEPLECATED
    // TODO : REMOVE UDP Packets
    bool CharUpdateAddress(ClientSocket& clientSocket, unsigned long CharID_In);    
    bool CharAddressInfo(ClientSocket& clientSocket, unsigned long CharID_In, unsigned long TargetID_In);    
    bool CharMove(ClientSocket& clientSocket, unsigned long CharID_In, POS* Pos_In, float Dir_In, unsigned short UAct_In, 
        unsigned short LAct_In, char Level_In, unsigned long ChantEf_In, unsigned long EnchantEf_In);    
    bool CharRequireInfo(ClientSocket& clientSocket, unsigned long SenderID_In, unsigned long TargetID_In, unsigned char Cmd_In);
    */
};

#endif
#ifndef _GAMA_CLIENT_PARSE_MOVE_BROADCAST_H_
#define _GAMA_CLIENT_PARSE_MOVE_BROADCAST_H_

#include <winsock2.h>
#include <windows.h>

#include <DB/DBDefine.h>
#include <Network/Packet/PacketStruct/AddressPacket.h>
#include <Network/Packet/PacketStruct/CharItemPacket.h>
#include <Network/Packet/PacketStruct/CharItemPacketStruct.h>

// forward decl
class  ClientSocket;
struct PktBase;

namespace BroadcastInfo
{
    class CObjectInfoMgr;
};


namespace ParsePacket
{
    unsigned long HandleMonMove(PktBase* lpPktBase, unsigned long *MonID_Out, POS* lpPos_Out, float *Dir_Out, float *Vec_Out, unsigned short *Act_Out, unsigned short *AniNum_Out);
    
    unsigned long HandleCharMoveEx(PktBase* lpPktBase, unsigned long *CharID_Out, 
        POS* Pos_Out, float *Dir_Out, unsigned char *UAct_Out, unsigned char *LAct_Out);

    // TODO : REMOVE UDP Packets
    // ClientSocket에 UDP public, private 주소를 업데이트한다.
    unsigned long HandleCharUpdateAddress(PktBase* lpPktBase, ClientSocket& clientSocket);
    unsigned long HandleCharAddressInfo(PktBase* lpPktBase, unsigned long *CharID_Out, SOCKADDR_IN* PubAddress_Out, SOCKADDR_IN* PriAddress_Out);
    unsigned long HandleCharMove(PktBase* lpPktBase, unsigned long *CharID_Out, POS* Pos_Out, float *Dir_Out, unsigned short *UAct_Out, unsigned short *LAct_Out, char *Level_Out, unsigned long *ChantEf_Out, unsigned long *EnchantEf_Out, unsigned long *PtCount_Out);
}

#endif
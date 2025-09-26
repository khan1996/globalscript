#ifndef _GAMA_CLIENT_SEND_ATTACK_PACKET_H_
#define _GAMA_CLIENT_SEND_ATTACK_PACKET_H_

#include <DB/DBdefine.h>
#include <Network/Packet/PacketStruct/CharAttackPacketStruct.h>

// forward decl.
struct CHAR_CREATE;
class ClientSocket;

namespace ClientNet
{
    // forward decl.
    class CClientEventHandler;
}

namespace SendPacket
{
    bool CharAttack(ClientNet::CClientEventHandler* lpHandler, unsigned long CharID_In, POS* lpPos_In, float Dir_In, AtType &AtType_In, AtNode &AtNode_In);
    bool CharRespawn(ClientNet::CClientEventHandler* lpHandler, unsigned long CharID_In, unsigned char cCmd_In, POS& Pos_In, unsigned long dwTownID_In);
	bool CharRespawnInfo(ClientNet::CClientEventHandler* lpHandler, unsigned long CharID_In);
	bool CharRespawnAreaInfo(ClientNet::CClientEventHandler* lpHandler, unsigned long CharID_In, unsigned long TownID_In);
    bool CharRespawnWaitQueue(ClientNet::CClientEventHandler* lpHandler, unsigned long CharID_In);
    bool CharPeaceMode(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID_In, bool bPeace_In);	
    bool CharSwitchHand(ClientNet::CClientEventHandler* lpHandler, unsigned char SelectHand_In);
    bool CharSwitchRide(ClientNet::CClientEventHandler* lpHandler, unsigned char Ride_In);
    bool CharDuelCmd(ClientNet::CClientEventHandler* lpHandler, unsigned long SenderID_In, unsigned long RecverID_In, unsigned char Cmd_In);
}


#endif
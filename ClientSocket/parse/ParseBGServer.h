#ifndef _GAMA_CLIENT_PARSE_BATTLEGROUND_SERVER_PACKET_H_
#define _GAMA_CLIENT_PARSE_BATTLEGROUND_SERVER_PACKET_H_

#include <DB/DBDefine.h>
#include <Network/Packet/PacketStruct/BGServerPacket.h>

// forward decl.
struct PktBase;

namespace ParsePacket
{
	// BATTLEGROUND SERVER
	unsigned long HandleCharBGServerMapList(PktBase* lpPktBase, bool* bAll, unsigned char* cMapInfoNodeNum, BGServerMapInfoNode* lpMapInfo);
	unsigned long HandleCharBGServerResultList(PktBase* lpPktBase, bool* bAll, unsigned char* cResultInfoNodeNum, BGServerResultInfoNode* lpResultInfo);
	unsigned long HandleCharBGServerMoveZone(PktBase* lpPktBase, unsigned char* cZone, unsigned char* cMoveType);
	unsigned long HandleCharBGServerMileageChange(PktBase* lpPktBase, unsigned long* dwCID, unsigned char* cGroup, unsigned char* cCmd, unsigned long* dwGold, unsigned long* dwMileage);
};


#endif
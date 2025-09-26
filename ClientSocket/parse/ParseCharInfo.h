#ifndef _GAMA_CLIENT_PARSE_CHAR_INFO_H_
#define _GAMA_CLIENT_PARSE_CHAR_INFO_H_

#include <Network/Packet/PacketStruct/CharStatusPacketStruct.h>
#include <Network/Packet/PacketStruct/CharItemPacketStruct.h>

// forward decl.
struct PktBase;

namespace ParsePacket
{    
    //캐릭터 정보 변경
    unsigned long HandleCharLevelUp(PktBase* lpPktBase, unsigned long *CharID, unsigned char *Level, LPChState lpChState);     
    unsigned long HandleCharIncreasePoint(PktBase* lpPktBase, unsigned long *CharID, ChState* lpChState);
    unsigned long HandleCharClassUpgrade(PktBase* lpPktBase, unsigned long *dwCharID, unsigned char *cClassID, unsigned char *cLevel, ChState* lpState, unsigned long *dwFame);
    unsigned long HandleCharHPRegen(PktBase* lpPktBase, unsigned long *CharID, unsigned short *HP, unsigned short *MP);	
    unsigned long HandleCharQuickSlotMove(PktBase* lpPktBase, TakeType* lpTakeType);
    unsigned long HandleCharSwitchEQ(PktBase* lpPktBase, unsigned char *Type, unsigned char *Select);
    unsigned long HandleCharEliteBonus(PktBase* lpPktBase, char *cEliteBonus);

    // TODO : REMOVE UDP Packets
    unsigned long HandleCharLevelUpInfo(PktBase* lpPktBase, unsigned long *CharID, unsigned char *Level);
};


#endif
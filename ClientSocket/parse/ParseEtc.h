#ifndef _GAMA_CLIENT_PARSE_ETC_PACKET_H_
#define _GAMA_CLIENT_PARSE_ETC_PACKET_H_

#include <DB/DBDefine.h>
#include <Item/ItemStructure.h>
#include <Network/Packet/PacketStruct/CharStatusPacketStruct.h>
#include <Network/Packet/PacketStruct/CharConfigPacketStruct.h>

// forward decl.
struct PktBase;

namespace ParsePacket
{
    // ETC
    unsigned long HandleCharBillingTimeoutInfo(PktBase* lpPktBase, unsigned long *dwCharID, unsigned char* cRemainMinute, unsigned char* cBillingType);
    unsigned long HandleCharBillingTimeCheckNotify(PktBase* lpPktBase, unsigned long *dwCharID, unsigned char* cRemainMinute, unsigned char* cBillingType);
    unsigned long HandleCharStateRedistribution(PktBase* lpPktBase, ChState* lpChState_Out, SKILL* lpSkill_Out);
	unsigned long HandleCharStatusRetrain(PktBase* lpPktBase, ChState* lpChState_Out, SKILL* lpSkill_Out, Item::ItemPos* lpIndex_Out, unsigned long *Gold_Out);
    unsigned long HandleControlOption(PktBase* lpPktBase, unsigned long *CharID_Out, RejectOption* Reject_Out);
    unsigned long HandleCharBindPos(PktBase* lpPktBase, unsigned long *NPCID_Out, unsigned char *Cmd_Out, POS* lpPos_Out, char *Zone_Out);
    unsigned long HandleCharLotteryResult(PktBase* lpPktBase, unsigned long *CharID_Out, Item::ItemPos* lpIndex_Out, unsigned long *dwSize_Out, char **lppBuffer_Out);
	unsigned long HandleCharAdmin(PktBase* lpPktBase, unsigned short *usCmd);

    unsigned long HandleCharNameChange(PktBase* lpPktBase, char*& szChangedName, unsigned char* cNameChangeCount, Item::ItemPos* itemPos_Out);

	unsigned long HandleExtraEvent(PktBase* lpPktBase, unsigned long* dwUserID, unsigned long* dwCharID, unsigned long* dwEventID, unsigned long* dwValue1, unsigned long* dwValue2);

};


#endif
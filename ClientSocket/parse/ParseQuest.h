#ifndef _GAMA_CLIENT_PARSE_CHAR_QUEST_
#define _GAMA_CLIENT_PARSE_CHAR_QUEST_

#include <Network/Packet/PacketStruct/CharQuestPacket.h>

namespace ParsePacket
{
    // Äù½ºÆ® °ü·Ã
    unsigned long HandleCharStartQuest(PktBase* lpPktBase, unsigned long *dwCharID_Out, unsigned long *dwNPCID_Out, unsigned short *wQuestID_Out);
    unsigned long HandleCharCancelQuest(PktBase* lpPktBase, unsigned long *dwCharID_Out, unsigned short *wQuestID_Out);
    unsigned long HandleCharOperateTrigger(PktBase* lpPktBase, unsigned long *dwCharID_Out, unsigned short *wQuestID_Out, unsigned char *cPhase_Out, unsigned char *cTrigger_Out, unsigned char *cCount_Out);
    unsigned long HandleCharQuestInfo(PktBase* lpPktBase, unsigned long *dwCharID_Out, unsigned short *wExecutingQuestNum_Out, PktQuestDB::ExecutingQuest **lppExecutingQuest_Out, unsigned short *wHistoryQuestNum_Out, unsigned short **lppHistoryQuest_Out);
    unsigned long HandleCharEndQuest(PktBase* lpPktBase, unsigned long *dwCharID_Out, unsigned short *wQuestID_Out, bool *bSave_Out);
}

#endif
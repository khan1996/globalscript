#ifndef _GAMA_CLIENT_SEND_QUEST_H_
#define _GAMA_CLIENT_SEND_QUEST_H_

namespace ClientNet
{
    // forward decl.
    class CClientEventHandler;
}

namespace SendPacket
{
    // Äù½ºÆ® °ü·Ã
    bool CharStartQuest(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID_In, unsigned long dwNPCID_In, unsigned short wQuestID_In);
    bool CharCancelQuest(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID_In, unsigned short wQuestID_In);
    bool CharOperateTrigger(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID_In, unsigned short wQuestID_In, unsigned char cPhase_In,
							unsigned char cTriggerKind_In, unsigned char cTrigger_In, unsigned char cCount_In);
}

#endif
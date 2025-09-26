#include "stdafx.h"
#include "ParseQuest.h"
#include "GMMemory.h"


unsigned long ParsePacket::HandleCharStartQuest(PktBase* lpPktBase, unsigned long *dwCharID_Out, 
                                                unsigned long *dwNPCID_Out, unsigned short *wQuestID_Out)
{
    PktStartQuest* lpPktStartQuest = static_cast<PktStartQuest*>(lpPktBase);

    *dwCharID_Out	= lpPktStartQuest->m_dwCharID;
    *dwNPCID_Out	= lpPktStartQuest->m_dwNPCID;
    *wQuestID_Out	= lpPktStartQuest->m_wQuestID;

    return lpPktStartQuest->GetError();
}


unsigned long ParsePacket::HandleCharCancelQuest(PktBase* lpPktBase, unsigned long *dwCharID_Out, 
                                                 unsigned short *wQuestID_Out)
{
    PktCancelQuest* lpPktCancelQuest = static_cast<PktCancelQuest*>(lpPktBase);

    *dwCharID_Out		= lpPktCancelQuest->m_dwCharID;
    *wQuestID_Out		= lpPktCancelQuest->m_wQuestID;

    return lpPktCancelQuest->GetError();
}


unsigned long ParsePacket::HandleCharOperateTrigger(PktBase* lpPktBase, unsigned long *dwCharID_Out, 
                                                    unsigned short *wQuestID_Out, unsigned char *cPhase_Out, 
													unsigned char *cTrigger_Out,unsigned char *cCount_Out)
{
    PktOperateTrigger* lpPktOT = static_cast<PktOperateTrigger*>(lpPktBase);
    
    *dwCharID_Out		= lpPktOT->m_dwCharID;
    *wQuestID_Out		= lpPktOT->m_wQuestID;
    *cPhase_Out			= lpPktOT->m_cPhase;
    *cTrigger_Out		= lpPktOT->m_cTrigger;
    *cCount_Out			= lpPktOT->m_cCount;

    return lpPktOT->GetError();
}


unsigned long ParsePacket::HandleCharQuestInfo(PktBase* lpPktBase, unsigned long *dwCharID_Out, 
                                               unsigned short *wExecutingQuestNum_Out, 
                                               PktQuestDB::ExecutingQuest **lppExecutingQuest_Out, 
                                               unsigned short *wHistoryQuestNum_Out, 
                                               unsigned short **lppHistoryQuest_Out)
{
    PktQuestDB* lpPktQuestDB = static_cast<PktQuestDB*>(lpPktBase);

    *dwCharID_Out			= lpPktQuestDB->m_dwCID;
    *wExecutingQuestNum_Out	= lpPktQuestDB->m_wExecuteQuestSize / sizeof(PktQuestDB::ExecutingQuest);
    *wHistoryQuestNum_Out	= lpPktQuestDB->m_wHistoryQuestSize / sizeof(unsigned short);

    *lppExecutingQuest_Out	= reinterpret_cast<PktQuestDB::ExecutingQuest *>(lpPktQuestDB + 1);
    *lppHistoryQuest_Out	= reinterpret_cast<unsigned short*>(
        reinterpret_cast<char*>(lpPktQuestDB + 1) + lpPktQuestDB->m_wExecuteQuestSize);

    return lpPktQuestDB->GetError();
}


unsigned long ParsePacket::HandleCharEndQuest(PktBase* lpPktBase, unsigned long *dwCharID_Out,
                                              unsigned short *wQuestID_Out, bool *bSave_Out)
{
    PktEndQuest* lpPktEQ = static_cast<PktEndQuest*>(lpPktBase);

    *dwCharID_Out	= lpPktEQ->m_dwCharID;
    *wQuestID_Out	= lpPktEQ->m_wQuestID;
    *bSave_Out		= lpPktEQ->m_bSave;

    return lpPktEQ->GetError();
}

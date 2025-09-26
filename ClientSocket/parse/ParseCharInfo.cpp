
#include "stdafx.h"

#include <Network/Packet/PacketStruct/CharStatusPacket.h>
#include <Network/Packet/PacketStruct/CharItemPacket.h>

#include "ParseCharInfo.h"
#include "GMMemory.h"


unsigned long ParsePacket::HandleCharLevelUp(PktBase* lpPktBase, unsigned long *CharID, 
											 unsigned char *Level, ChState* lpChState)
{
    PktLU* lpLUPt = static_cast<PktLU *>(lpPktBase);

    *CharID			= lpLUPt->m_dwCharID;
    *Level			= lpLUPt->m_cLevel;
    *lpChState		= lpLUPt->m_State;

    return lpLUPt->GetError();
}


unsigned long ParsePacket::HandleCharLevelUpInfo(PktBase* lpPktBase, unsigned long *CharID, unsigned char *Level)
{
    PktLUInfo* lpLUInfoPt = static_cast<PktLUInfo *>(lpPktBase);

    *CharID			= lpLUInfoPt->m_dwCharID;
    *Level			= lpLUInfoPt->m_cLevel;

    return lpLUInfoPt->GetError();
}


unsigned long ParsePacket::HandleCharIncreasePoint(PktBase* lpPktBase, unsigned long *CharID, ChState* lpChState)
{
	PktIPAck* lpIPAckPt = static_cast<PktIPAck *>(lpPktBase);

	*CharID			= lpIPAckPt->m_dwCharID;
	*lpChState		= lpIPAckPt->m_State;

	return lpIPAckPt->GetError();
}


unsigned long ParsePacket::HandleCharClassUpgrade(PktBase* lpPktBase, unsigned long *dwCharID, 
                                                  unsigned char *cClassID, unsigned char *cLevel, 
                                                  ChState* lpState, unsigned long *dwFame)
{
	PktCUAck* lpCUAckPt = static_cast<PktCUAck *>(lpPktBase);

	*dwCharID	= lpCUAckPt->m_dwCharID;
	*cClassID	= lpCUAckPt->m_cClass;
	*cLevel		= lpCUAckPt->m_cLevel;
	*lpState	= lpCUAckPt->m_State;
	*dwFame		= lpCUAckPt->m_dwFame;

	return lpCUAckPt->GetError();
}


unsigned long ParsePacket::HandleCharHPRegen(PktBase* lpPktBase, unsigned long *CharID, 
											 unsigned short *HP, unsigned short *MP)
{
    PktHP* lpHPPt = static_cast<PktHP *>(lpPktBase);

    *CharID			= lpHPPt->m_dwCharID;
    *HP				= lpHPPt->m_sHP;
    *MP				= lpHPPt->m_sMP;

    return lpHPPt->GetError();
}


unsigned long ParsePacket::HandleCharQuickSlotMove(PktBase* lpPktBase, TakeType* lpTakeType)
{
	PktQSM* lpQSMPt = static_cast<PktQSM *>(lpPktBase);

	*lpTakeType     = lpQSMPt->m_TakeType;

	return lpQSMPt->GetError();
}


unsigned long ParsePacket::HandleCharSwitchEQ(PktBase* lpPktBase, unsigned char *Type, unsigned char *Select)
{
	PktSwEQ* lpSwHPt = static_cast<PktSwEQ *>(lpPktBase);

	*Type		= lpSwHPt->m_cType;
	*Select		= lpSwHPt->m_cSelect;

	return lpSwHPt->GetError();
}


unsigned long ParsePacket::HandleCharEliteBonus(PktBase* lpPktBase, char *cEliteBonus)
{
	PktEB* lpEBPt = static_cast<PktEB *>(lpPktBase);

	*cEliteBonus	= lpEBPt->m_cEliteBonus;

	return lpEBPt->GetError();
}


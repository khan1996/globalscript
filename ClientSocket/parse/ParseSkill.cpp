#include "stdafx.h"
#include "ParseSkill.h"

#include <Network/Packet/PacketStruct/CharMovePacket.h>
#include <Network/Packet/PacketStruct/CharStatusPacket.h>
#include <Network/Packet/PacketStruct/CharAttackPacket.h>
#include "GMMemory.h"

unsigned long ParsePacket::HandleCharSkillLock(PktBase* lpPktBase, unsigned long *CharID_Out, 
                                               unsigned short *SkillID_Out, unsigned char *Index_Out)
{
    LPPktSk lpSkPt = (LPPktSk)lpPktBase;

    *CharID_Out			= lpSkPt->m_dwCharID;
    *SkillID_Out		= lpSkPt->m_wSkill;
    *Index_Out			= lpSkPt->m_cIndex;

    return lpSkPt->GetError();
}


unsigned long ParsePacket::HandleCharSkillUnLock(PktBase* lpPktBase, unsigned long *CharID_Out, 
                                                 unsigned short *SkillID_Out, unsigned char *Index_Out, unsigned char *cSkillLevel, Item::ItemPos* itemPos_Out)
{
    LPPktSkULAck lpSkULAckPt = (LPPktSkULAck)lpPktBase;

    *CharID_Out			= lpSkULAckPt->m_dwCharID;
    *SkillID_Out		= lpSkULAckPt->m_wSkill;
    *Index_Out			= lpSkULAckPt->m_cIndex;
	*cSkillLevel		= lpSkULAckPt->m_cSkillLevel;
	*itemPos_Out		= lpSkULAckPt->m_ItemPos;

    return lpSkULAckPt->GetError();
}



unsigned long ParsePacket::HandleCharSkillCreate(PktBase* lpPktBase, unsigned long *CharID_Out, 
                                                 unsigned short *SkillID_Out, unsigned char *Index_Out)
{
    LPPktSk lpSkPt = (LPPktSk)lpPktBase;

    *CharID_Out			= lpSkPt->m_dwCharID;
    *SkillID_Out		= lpSkPt->m_wSkill;
    *Index_Out			= lpSkPt->m_cIndex;

    return lpSkPt->GetError();
}


unsigned long ParsePacket::HandleCharSkillErase(PktBase* lpPktBase, unsigned long *CharID_Out, 
                                                unsigned short *SkillID_Out, unsigned char *Index_Out, Item::ItemPos* itemPos_Out)
{
    LPPktSk lpSkPt = (LPPktSk)lpPktBase;

    *CharID_Out			= lpSkPt->m_dwCharID;
    *SkillID_Out		= lpSkPt->m_wSkill;
    *Index_Out			= lpSkPt->m_cIndex;
	*itemPos_Out		= lpSkPt->m_ItemPos;

    return lpSkPt->GetError();
}



unsigned long ParsePacket::HandleCharSummonCmd(PktBase* lpPktBase, unsigned long *CharID_Out, 
                                               unsigned char *Cmd_Out, unsigned long *TargetID_Out)
{
	LPPktSummonCmd lpSuCPtAck = (LPPktSummonCmd)lpPktBase;
	
	*CharID_Out			= lpSuCPtAck->m_dwCharID;
	*Cmd_Out			= lpSuCPtAck->m_cCmd;
	*TargetID_Out		= lpSuCPtAck->m_dwTargetID;

	return lpSuCPtAck->GetError();
}


unsigned long ParsePacket::HandleCharSummon(PktBase* lpPktBase, unsigned long *CharID_Out, 
                                            unsigned long *TargetID_Out, POS* lpPos_Out)
{
	LPPktSummon lpSummonPt = (LPPktSummon)lpPktBase;

	*CharID_Out		    = lpSummonPt->m_dwCharID;
	*TargetID_Out		= lpSummonPt->m_dwTargetID;

	CNetworkPos& netPos = lpSummonPt->m_NetworkPos;

	lpPos_Out->fPointX	= netPos.GetXPos();
	lpPos_Out->fPointY  = netPos.GetYPos();
	lpPos_Out->fPointZ  = netPos.GetZPos();

	return lpSummonPt->GetError();
}


unsigned long ParsePacket::HandleCharSpellInfo(PktBase* lpPktBase, unsigned long *CharID_Out, 
                                               unsigned char *SpellType_Out, unsigned short *EnchantLevel_Out, unsigned long *EnchantTime_Out,
                                               bool *OnOff_Out)
{
	LPPktSpInfo lpSpInfoPt = (LPPktSpInfo)lpPktBase;

	*CharID_Out			= lpSpInfoPt->m_dwCharID;
	*SpellType_Out		= lpSpInfoPt->m_cSpellType;
	*EnchantLevel_Out	= lpSpInfoPt->m_nEnchantLevel;
	*EnchantTime_Out	= lpSpInfoPt->m_dwEnchantTime;
	*OnOff_Out			= lpSpInfoPt->m_bOnOff;

	return lpSpInfoPt->GetError();
}




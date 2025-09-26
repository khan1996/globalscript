#ifndef _GAMA_CLIENT_PARSE_SKILL_PACKET_H_
#define _GAMA_CLIENT_PARSE_SKILL_PACKET_H_

#include <Item/ItemStructure.h>

// 전방 참조
struct PktBase;

namespace ParsePacket
{
    // 스킬
    unsigned long HandleCharSkillLock(PktBase* lpPktBase, unsigned long *CharID_Out, unsigned short *SkillID_Out, unsigned char *Index_Out);
    unsigned long HandleCharSkillUnLock(PktBase* lpPktBase, unsigned long *CharID_Out, unsigned short *SkillID_Out, unsigned char *Index_Out, unsigned char *SkillLevel_Out, Item::ItemPos* itemPos_Out);
    unsigned long HandleCharSkillCreate(PktBase* lpPktBase, unsigned long *CharID_Out, unsigned short *SkillID_Out, unsigned char *Index_Out);
    unsigned long HandleCharSkillErase(PktBase* lpPktBase, unsigned long *CharID_Out, unsigned short *SkillID_Out, unsigned char *Index_Out, Item::ItemPos* itemPos_Out);
    unsigned long HandleCharSummonCmd(PktBase* lpPktBase, unsigned long *CharID_Out, unsigned char *Cmd_Out, unsigned long *TargetID_Out);
    unsigned long HandleCharSummon(PktBase* lpPktBase, unsigned long *CharID_Out, unsigned long *TargetID_Out, POS* lpPos_Out);
    unsigned long HandleCharSpellInfo(PktBase* lpPktBase, unsigned long *CharID_Out, unsigned char *SpellType_Out, unsigned short *EnchantLevel_Out, unsigned long *EnchantTime_Out, bool *OnOff_Out);
};


#endif
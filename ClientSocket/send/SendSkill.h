#ifndef _GAMA_CLIENT_SEND_SKILL_PACKET_H_
#define _GAMA_CLIENT_SEND_SKILL_PACKET_H_


#include <Item/ItemStructure.h>

namespace ClientNet
{
    // forward decl.
    class CClientEventHandler;
}


namespace SendPacket
{
    bool CharSkillCreate(ClientNet::CClientEventHandler* lpHandler, unsigned long CharID_In, unsigned short SkillID_In, unsigned char Index_In);
    bool CharSkillErase(ClientNet::CClientEventHandler* lpHandler, unsigned long CharID_In, unsigned short SkillID_In, unsigned char Index_In, Item::ItemPos* lpItem_In);
    bool CharSummonCmd(ClientNet::CClientEventHandler* lpHandler, unsigned long CharID_In, unsigned char Cmd_In, unsigned long TargetID_In = 0);
};

#endif
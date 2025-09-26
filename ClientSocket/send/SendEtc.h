#ifndef _GAMA_CLIENT_ETC_SEND_PACKET_H_
#define _GAMA_CLIENT_ETC_SEND_PACKET_H_

// 전방 참조
struct ChState;
struct POS;
struct RejectOption;

class ClientSocket;

namespace ClientNet
{
	class CClientEventHandler;
}

namespace Item
{
	struct ItemPos;
}

namespace SendPacket
{
    bool SendSysPing(ClientNet::CClientEventHandler* lpHandler);
    bool CharAdmin(ClientNet::CClientEventHandler* lpHandler, unsigned short Cmd_In, char* Name_In, char Zone_In, char Channel_In, unsigned short ProtoTypeID, POS& Pos_In, unsigned long dwAmount, unsigned long dwAmount2 = 0, unsigned long dwAmount3 = 0);
    bool CharSuicide(ClientNet::CClientEventHandler* lpHandler, unsigned long CharID_In);
    bool CharBindPos(ClientNet::CClientEventHandler* lpHandler, unsigned long NPCID_In, unsigned char Cmd_In, POS* lpPos_In, char Zone_In);
    bool CharStateRedistribution(ClientNet::CClientEventHandler* lpHandler, ChState* lpState_In);    
    bool CharControlOption(ClientNet::CClientEventHandler* lpHandler, unsigned long CharID_In, RejectOption &Reject_In);
    bool CharIncreasePoint(ClientNet::CClientEventHandler* lpHandler, unsigned long CharID_In, unsigned char StateType_In);
    bool CharClassUpgrade(ClientNet::CClientEventHandler* lpHandler, unsigned long CharID_In, unsigned char ClassID_In);

	bool CharStatusRetrain(ClientNet::CClientEventHandler* lpHandler, ChState* lpState_In, Item::ItemPos* lpIndex_In);

    bool CharNameChange(ClientNet::CClientEventHandler* lpHandler, unsigned long dwUID, 
        unsigned long dwCID, const char* szChangeName, Item::ItemPos* lpItem_In);
}

#endif
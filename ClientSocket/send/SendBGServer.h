#ifndef _GAMA_CLIENT_SEND_BATTLEGROUND_SERVER_H_
#define _GAMA_CLIENT_SEND_BATTLEGROUND_SERVER_H_

// 전방 참조
class ClientSocket;

namespace ClientNet
{
	class CClientEventHandler;
}

namespace SendPacket
{
	bool CharBGServerMapList(ClientNet::CClientEventHandler* lpHandler);
	bool CharBGServerResultList(ClientNet::CClientEventHandler* lpHandler);
	bool CharBGServerMoveZone(ClientNet::CClientEventHandler* lpHandler, unsigned short wMapNumber, unsigned char cMoveType);
	bool CharBGServerCharSlot(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, unsigned char cGroup);
	bool CharBGServerMileageChange(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, unsigned char cGroup, unsigned char cCmd, unsigned long dwMileage);
};


#endif
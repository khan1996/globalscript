#ifndef _GAMA_CLIENT_SEND_CASTLE_PACKET_H_
#define _GAMA_CLIENT_SEND_CASTLE_PACKET_H_

#include <Network/Packet/PacketStruct/CastlePacket.h>

namespace ClientNet
{
	// forward decl.
	class CClientEventHandler;
}


namespace SendPacket
{
	// 공성 명령 패킷
	bool CharCastleCmd(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, unsigned long dwCastleID, 
					   unsigned long dwCastleObjectID, unsigned long dwValue1, unsigned long dwValue2, unsigned char cSubCmd);

	bool CharCampCmd(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, unsigned long dwCampID,
					 unsigned long dwValue, unsigned long dwValue2, unsigned char cSubCmd);

	bool CharSiegeArmsCmd(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, unsigned long dwArmsID, unsigned char cSubCmd);


	// 공성 권한 설정 패킷
	bool CharCastleRight(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, unsigned long dwCastleID, CastleRight castleRight);

	bool CharCampRight(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, unsigned long dwCampID, CampRight campRight);

	

	// 성 상징물 보석 아이템 넣기/빼기
	bool CharTakeCastleJewel(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, unsigned long dwCastleID, unsigned char cSubCmd,
						unsigned short wItemID, unsigned char cIndex, unsigned char cNum, Item::ItemPos pos);
	
	// 길드 요새 자재 넣기/빼기
	bool CharTakeMaterial(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, unsigned long dwCampID, unsigned char cSubCmd,
						unsigned char cNum, Item::ItemPos pos);

	// 길드, 국가 전쟁 참여.
	bool WarOnOff(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, unsigned char cFlag, unsigned char cType);	
};

#endif
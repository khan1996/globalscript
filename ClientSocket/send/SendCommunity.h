#ifndef _GAMA_CLIENT_COMMUNITY_SEND_H_
#define _GAMA_CLIENT_COMMUNITY_SEND_H_

#include <Network/Packet/PacketStruct/FriendPacket.h>
#include <Network/Packet/PacketStruct/RankingPacket.h>

// forward decl.
class ClientSocket;
struct GuildSmallInfoNode;
struct GuildRight;
struct POS;

namespace ClientNet
{
    // forward decl.
    class CClientEventHandler;
}

namespace SendPacket
{
	// 길드
	bool CharCreateGuild(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, char* szGuildName);
	bool CharGuildCmd(ClientNet::CClientEventHandler* lpHandler, unsigned long dwGID, unsigned long dwSenderID, unsigned long dwReferenceID, unsigned short wCmd);
	bool CharGuildMark(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, unsigned long dwGID, char* szMark);
	bool CharGuildLevel(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, unsigned char cLevel);
	bool CharGuildRelation(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, unsigned long dwGID, unsigned long dwTargetGID, unsigned long dwValue, unsigned char cSubCmd);
	bool CharGuildInclination(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, unsigned char cInclination);
	bool CharGuildList(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, unsigned char cSortCmd, unsigned char cPage, unsigned char cNodeNum, GuildSmallInfoNode* lpNode);
	bool CharGuildRight(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, GuildRight guildRight);
	bool CharGuildMemberList(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, unsigned char cMemberType, unsigned char cSortCmd, unsigned char cPage);
	bool CharGuildSafe(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, unsigned long dwGold, unsigned char cCmd);
	bool CharGuildHostilityList(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, unsigned char cCurrentPage, unsigned char cPageState);
	bool CharGuildRelationInfo(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, unsigned long dwGID);

	// 파티
    bool CharPartyCmd(ClientNet::CClientEventHandler* lpHandler, unsigned long SenderID, unsigned long ReferenceID, unsigned long PartyID, unsigned short Cmd);
    bool CharPartyFind(ClientNet::CClientEventHandler* lpHandler, unsigned long CharID);

	// 친구
    bool CharFriendAddRequest(ClientNet::CClientEventHandler* lpHandler, char* szName, PktFriendAddReq::CMD addType);
    bool CharFriendRemoveRequest(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, PktFriendRemoveReq::CMD removeType, char* szTargetName);
    bool CharFriendEtcRequest(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, unsigned long dwData, PktFriendEtc::CMD etcType);

	// 기타 커뮤니티
	bool CharFameInfo(ClientNet::CClientEventHandler* lpHandler, unsigned long CharID);
	bool CharRankingInfo(ClientNet::CClientEventHandler* lpHandler, unsigned long CharID, unsigned char cClass, unsigned char cPage);
    bool CharAuthorizePanel(ClientNet::CClientEventHandler* lpHandler, unsigned long dwAffectedID, unsigned long dwCasterID, unsigned char cCmd);
    bool CharChat(ClientNet::CClientEventHandler* lpHandler, unsigned char Cmd, char* Message, int MessageSize, unsigned short usLang);
    bool CharTargetedChat(ClientNet::CClientEventHandler* lpHandler, unsigned char Cmd, char* TargetNames, int nTargetNamesNum, char* Message, int MessageSize);
    bool CharWhisper(ClientNet::CClientEventHandler* lpHandler, char* SenderName, char *RecverName, char* Message, int MessageSize);

	// -------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// UDP Packets

    /* NOT USE
    // TODO : REMOVE UDP Packets
    bool CharPartyMemInfo(ClientSocket& clientSocket, unsigned long CharID, 
						  unsigned long PartyID, unsigned char Class, char Level, POS Pos,
						  unsigned short MaxHP, short CurHP, unsigned short MaxMP, short CurMP,
						  unsigned long Chant, unsigned long Enchant);

	bool CharPartyMemAddress(ClientSocket& clientSocket, unsigned long CharID,
							 unsigned long PartyID, const SOCKADDR_IN& PublicAddress,
							 const SOCKADDR_IN& PrivateAddress);

    
    bool CharPartyData(ClientSocket& clientSocket, unsigned long CharID, 
        unsigned short Cmd, unsigned short Len, void *Data);
    */
}

#endif
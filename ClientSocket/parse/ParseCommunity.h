#ifndef _GAMA_CLIENT_PARSE_COMMUNITY_PACKET_H_
#define _GAMA_CLIENT_PARSE_COMMUNITY_PACKET_H_

#include <winsock2.h>
#include <windows.h>

#include <DB/DBDefine.h>
#include <Network/Packet/PacketStruct/AddressPacket.h>
#include <Network/Packet/PacketStruct/CharCommunityPacketStruct.h>
#include <Network/Packet/PacketStruct/RankingPacket.h>

struct GuildSmallInfoNode;
struct GuildLargeInfoNode;
struct GuildRight;
struct GuildMemberInfoNode;
struct PartyMemberData;
struct GuildHostilityInfoNode;
struct GuildRelationInfoNode;

namespace ParsePacket
{
	// 길드
	unsigned long HandleCharCreateGuild(PktBase* lpPktBase, unsigned long *dwCID, unsigned long *dwGID, unsigned char *cInclination, char* szGuildName);
	unsigned long HandleCharGuildCmd(PktBase* lpPktBase, unsigned long *dwGID, unsigned long *dwSenderID, unsigned long *dwReferenceID, char* szGuildName, char* szSenderName, unsigned short *wCmd);
	unsigned long HandleCharGuildMark(PktBase* lpPktBase, unsigned long* dwCID, unsigned long* dwGID, char* szMark, unsigned long* dwGold);
	unsigned long HandleCharGuildLevel(PktBase* lpPktBase, unsigned long *dwGID, unsigned char *cLevel, unsigned long *dwGold);
	unsigned long HandleCharGuildRelation(PktBase* lpPktBase, unsigned long *dwCID, unsigned long *dwGID, unsigned long *dwTargetGID, unsigned long *dwValue, unsigned char *cSubCmd, char* szGuildName, char* szTempGuildName);
	unsigned long HandleCharGuildInclination(PktBase* lpPktBase, unsigned long *dwGID, unsigned char *cInclination);
	unsigned long HandleCharGuildList(PktBase* lpPktBase, unsigned long *dwCID, unsigned char *cSortCmd, unsigned char *cPage, unsigned char *cSmallNodeNum, GuildSmallInfoNode* lpSmallNode, unsigned char *cLargeNodeNum, GuildLargeInfoNode* lpLargeNode);
	unsigned long HandleCharGuildRight(PktBase* lpPktBase, unsigned long *dwGID, GuildRight *guildRight);
	unsigned long HandleCharGuildMemberList(PktBase* lpPktBase, unsigned long *dwCID, unsigned char* cMemberType, unsigned char *cSortCmd, unsigned char *cPage, unsigned char *cTotalMemberNum, unsigned char *cNodeNum, GuildMemberInfoNode* lpMemberNode);
	unsigned long HandleCharMyGuildInfo(PktBase* lpPktBase, unsigned long *dwGold, GuildRight *guildRight, unsigned char *cTitle);
	unsigned long HandleCharGuildSafe(PktBase* lpPktBase, unsigned long *dwCID, char* szCharName, unsigned long *dwSafeGold, unsigned long *dwCharGold, unsigned char *cCmd);
	unsigned long HandleCharGuildHostilityList(PktBase* lpPktBase, unsigned long *dwCID, unsigned char *cCurrentPage, unsigned char *cPageState, unsigned char* cNodeNum, unsigned short* wTotalItemNum, char* szHostility, char* szAlert, GuildHostilityInfoNode* lpHostilityNode);
	unsigned long HandleCharGuildRelationInfo(PktBase* lpPktBase, unsigned long *dwCID, unsigned long *dwGID, unsigned short *wNodeNum, GuildRelationInfoNode* lpRelationNode ) ;

    // 파티 
    unsigned long HandleCharPartyInfo(PktBase* lpPktBase, PARTY_EX* lpParty);
    unsigned long HandleCharPartyCmd(PktBase* lpPktBase, AddressInfo* SenderAddress, char *SenderName, unsigned long *dwSenderID, unsigned long *dwReferenceID, unsigned long *dwPartyID, unsigned short *Cmd);
    unsigned long HandleCharPartyCmdInfo(PktBase* lpPktBase, unsigned long *dwMemberID, unsigned long *dwPartyID, unsigned short *Cmd);
    unsigned long HandleCharPartyFind(PktBase* lpPktBase, unsigned long *dwCharID, unsigned char *MemberFindPartyNum, unsigned char *PartyFindMemberNum, MemberFindParty** lppMemberFindParty, PartyFindMember** lppPartyFindMember);	
    
	unsigned long HandleCharPartyMemData(PktBase* lpPktBase, unsigned long& dwPartyID, unsigned long& dwSenderID, unsigned long& dwGID, unsigned long& dwServerID, unsigned short& wClass, char& cLevel, const char*& szSenderName, unsigned short& wCmd);

    unsigned long HandleCharPartyMemInfo(PktBase* lpPktBase, PartyMemberData* lpPartyMemberData, unsigned int& nMemberDataNum);

	// 친구
	unsigned long HandleCharFriendDB(PktBase* lpPktBase, unsigned char* cDataType, char **ppFriendListData, unsigned long *dwDataSize);

	unsigned long HandleCharFriendRegistered(PktBase* lpPktBase, unsigned long* dwCID, char* szName);

	unsigned long HandleCharFriendAck(PktBase* lpPktBase, unsigned char* cCmd, unsigned long *dwCID, char **ppszName, unsigned long* dwGID,
							unsigned short* wClass, char* cLevel, unsigned long* dwServerID);
	// 채팅
	unsigned long HandleCharChat(PktBase* lpPktBase, unsigned long& dwSenderCID, unsigned short *Cmd, unsigned short *Lang, char *Message);
	unsigned long HandleCharWhisper(PktBase* lpPktBase, char *SenderName, char *RecverName, char *Message);
	unsigned long HandleCharChatBan(PktBase* lpPktBase, bool& bChatBan);

	// 기타 커뮤니티
    unsigned long HandleCharAuthorizePanel(PktBase* lpPktBase, unsigned long *dwCasterID, char *szCasterName, unsigned char *cCmd);
	unsigned long HandleCharFameInfo(PktBase* lpPktBase, unsigned long *dwCharID, unsigned char *cOtherNation, unsigned char *cOtherClass, unsigned long *dwFame, unsigned long *dwMileage, unsigned char *RankingByRace, unsigned char *RankingByClass, unsigned char *cCmd, char *szWinCharName, char *szLoseCharName/*,unsigned long *KillCount, int* m_KillspreeLevel*/);
	unsigned long HandleCharRankingInfo(PktBase* lpPktBase, unsigned long *dwCharID, unsigned char *cClass, unsigned char *cPage, unsigned short *wNum, RankingNode **lppRankingNode);
};


#endif
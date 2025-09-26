
#include "stdafx.h"

#include <Utility/CheckSum/Crc32Static.h>

#include <Network/ClientNetwork/ClientEventHandler.h>
#include <Network/ClientSocket/ClientSocket.h>
#include <Network/Packet/PacketCommand.h>
#include <Network/Packet/PacketStruct/CharCommunityPacket.h>
#include <Network/Packet/PacketStruct/PartyPacket.h>
#include <Network/Packet/PacketStruct/GuildPacket.h>

#include "SendPacket.h"
#include "SendCommunity.h"
#include "GMMemory.h"


//--------------------------------------------------------------------------------------------------------------
// 길드

bool SendPacket::CharCreateGuild(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, char* szGuildName)
{
	ClientNet::CNetworkMsgBlock* lpMsgBlock = 
		ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(
        sizeof(PktCreateGuild), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

	PktCreateGuild* lpCGPt = reinterpret_cast<PktCreateGuild *>(lpMsgBlock->wr_ptr());

	lpCGPt->m_dwCID = dwCID;
	lpCGPt->m_dwGID = 0;			// 길드 아이디는 서버에서 받는다.
	lpCGPt->m_cInclination = 0;		// 성향도 서버에서 처리한다. 

	strncpy(lpCGPt->m_szGuildName, szGuildName, Guild::MAX_GUILD_NAME_LEN);

	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktCreateGuild), CmdCreateGuild, 0, 0);	
}

bool SendPacket::CharGuildCmd(ClientNet::CClientEventHandler* lpHandler, unsigned long dwGID, 
							  unsigned long dwSenderID, unsigned long dwReferenceID, unsigned short wCmd)
{
	ClientNet::CNetworkMsgBlock* lpMsgBlock = 
		ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(
        sizeof(PktGuildCmd), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

	PktGuildCmd* lpGCPt = reinterpret_cast<PktGuildCmd *>(lpMsgBlock->wr_ptr());

	lpGCPt->m_dwGID = dwGID & ~Guild::MEMBER_WAIT_BIT;

	lpGCPt->m_dwSenderID = dwSenderID;
	lpGCPt->m_dwReferenceID = dwReferenceID;

	lpGCPt->m_wCmd = wCmd;

	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktGuildCmd), CmdGuildCmd, 0, 0);	
}

bool SendPacket::CharGuildMark(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, unsigned long dwGID, char* szMark)
{
	ClientNet::CNetworkMsgBlock* lpMsgBlock = 
		ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktGuildMark), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

	PktGuildMark* lpGMPt= reinterpret_cast<PktGuildMark *>(lpMsgBlock->wr_ptr());

	lpGMPt->m_dwCID = dwCID;
	lpGMPt->m_dwGID = dwGID;

	::memcpy(lpGMPt->m_szMark, szMark, Guild::MAX_MARK_SIZE);

	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktGuildMark), CmdGuildMark, 0, 0);	
}

bool SendPacket::CharGuildLevel(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, unsigned char cLevel)
{
	ClientNet::CNetworkMsgBlock* lpMsgBlock = 
		ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktGuildLevel), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

	PktGuildLevel* lpGLPt = reinterpret_cast<PktGuildLevel *>(lpMsgBlock->wr_ptr());

	lpGLPt->m_dwUID = dwCID;
	lpGLPt->m_cLevel = cLevel;

	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktGuildLevel), CmdGuildLevel, 0, 0);	
}

bool SendPacket::CharGuildRelation(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID,
                                   unsigned long dwGID, unsigned long dwTargetGID, unsigned long dwValue, unsigned char cSubCmd)
{
	ClientNet::CNetworkMsgBlock* lpMsgBlock = 
		ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktGuildRelation), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

	PktGuildRelation* lpGRPt = reinterpret_cast<PktGuildRelation *>(lpMsgBlock->wr_ptr());

	lpGRPt->m_dwCID			= dwCID;
	lpGRPt->m_dwGID			= dwGID;
	lpGRPt->m_dwTargetGID	= dwTargetGID;
	lpGRPt->m_dwValue		= dwValue;
	lpGRPt->m_cSubCmd		= cSubCmd;

	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktGuildRelation), CmdGuildRelation, 0, 0);	
}


bool SendPacket::CharGuildInclination(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, unsigned char cInclination)
{
	ClientNet::CNetworkMsgBlock* lpMsgBlock = 
		ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktGuildInclination), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

	PktGuildInclination* lpGIPt = reinterpret_cast<PktGuildInclination *>(lpMsgBlock->wr_ptr());

	lpGIPt->m_dwUID = dwCID;
	lpGIPt->m_cInclination = cInclination;

	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktGuildInclination), CmdGuildInclination, 0, 0);	
}

bool SendPacket::CharGuildList(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, 
							   unsigned char cSortCmd, unsigned char cPage, unsigned char cNodeNum, GuildSmallInfoNode* lpNode)
{
	// 가변 길이
	unsigned short usPacketSize = sizeof(PktGuildList) + sizeof(GuildCheckSumNode) * cNodeNum;

	ClientNet::CNetworkMsgBlock* lpMsgBlock = 
		ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(usPacketSize, lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

	PktGuildList* lpGLPt = reinterpret_cast<PktGuildList *>(lpMsgBlock->wr_ptr());

	lpGLPt->m_dwCID = dwCID;
	lpGLPt->m_cSortCmd = cSortCmd;
	lpGLPt->m_cPage = cPage;
	lpGLPt->m_cSmallNodeNum = cNodeNum;

	unsigned short wBufferSize = sizeof(PktGuildList);
	for (int nIndex = 0; nIndex < cNodeNum; ++nIndex, ++lpNode)
	{
		// 체크섬을 뽑아내서 전송한다.
		GuildCheckSumNode tempNode;
		tempNode.m_dwGID = lpNode->m_dwGID & ~Guild::MEMBER_WAIT_BIT;
		CCrc32Static::BufferCrc32(reinterpret_cast<const char *>(lpNode), sizeof(GuildSmallInfoNode), 
			tempNode.m_dwCheckSum);

		CopyMemory(lpMsgBlock->wr_ptr() + wBufferSize, &tempNode, sizeof(GuildCheckSumNode));
		wBufferSize += sizeof(GuildCheckSumNode);
	}

	return SendPacket(lpHandler, lpMsgBlock, usPacketSize, CmdGuildList, 0, 0);	
}

bool SendPacket::CharGuildRight(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, GuildRight guildRight)
{
	ClientNet::CNetworkMsgBlock* lpMsgBlock = 
		ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktGuildRight), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

	PktGuildRight* lpGRPt = reinterpret_cast<PktGuildRight *>(lpMsgBlock->wr_ptr());

	lpGRPt->m_dwUID = dwCID;
	lpGRPt->m_GuildRight = guildRight;

	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktGuildRight), CmdGuildRight, 0, 0);
}

bool SendPacket::CharGuildMemberList(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, 
									 unsigned char cMemberType, unsigned char cSortCmd, unsigned char cPage)
{
	ClientNet::CNetworkMsgBlock* lpMsgBlock = 
		ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktGuildMemberList), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

	PktGuildMemberList* lpGMLPt = reinterpret_cast<PktGuildMemberList *>(lpMsgBlock->wr_ptr());

	lpGMLPt->m_dwCID = dwCID;

	lpGMLPt->m_cMemberType = cMemberType;
	lpGMLPt->m_cSortCmd = cSortCmd;
	lpGMLPt->m_cPage = cPage;

	lpGMLPt->m_cTotalMemberNum = 0;
	lpGMLPt->m_cNodeNum = 0;

	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktGuildMemberList), CmdGuildMemberList, 0, 0);
}

bool SendPacket::CharGuildSafe(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, unsigned long dwGold, unsigned char cCmd)
{
	ClientNet::CNetworkMsgBlock* lpMsgBlock = 
		ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktGuildSafe), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

	PktGuildSafe* lpGSPt = reinterpret_cast<PktGuildSafe *>(lpMsgBlock->wr_ptr());

	lpGSPt->m_dwCID = dwCID;
	lpGSPt->m_dwGold = dwGold;
	lpGSPt->m_cCmd = cCmd;

	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktGuildSafe), CmdGuildSafe, 0, 0);
}

bool SendPacket::CharGuildHostilityList(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID,
										unsigned char cCurrentPage, unsigned char cPageState)
{
	ClientNet::CNetworkMsgBlock* lpMsgBlock = 
		ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktGuildHostilityList), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

	PktGuildHostilityList* lpGHLPt = reinterpret_cast<PktGuildHostilityList *>(lpMsgBlock->wr_ptr());

	lpGHLPt->m_dwCID = dwCID;

	lpGHLPt->m_cCurrentPage = cCurrentPage;
	lpGHLPt->m_cPageState = cPageState;
	lpGHLPt->m_cNodeNum = 0;
	lpGHLPt->m_wTotalItemNum = 0;

	::memset(lpGHLPt->m_szHostility, 0, Guild::MAX_GUILD_NAME_LEN);
	::memset(lpGHLPt->m_szAlert, 0, Guild::MAX_GUILD_NAME_LEN);

	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktGuildHostilityList), CmdGuildHostilityList, 0, 0);
}

bool SendPacket::CharGuildRelationInfo(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, unsigned long dwGID)
{
	ClientNet::CNetworkMsgBlock* lpMsgBlock = 
		ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktGuildRelationInfo), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

	PktGuildRelationInfo* lpGRIPt = reinterpret_cast<PktGuildRelationInfo *>(lpMsgBlock->wr_ptr());

	lpGRIPt->m_dwCID = dwCID;
	lpGRIPt->m_dwGID = dwGID;
	lpGRIPt->m_wNodeNum = 0;

	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktGuildRelationInfo), CmdGuildRelationInfo, 0, 0);
}


//--------------------------------------------------------------------------------------------------------------
// 파티

bool SendPacket::CharPartyCmd(ClientNet::CClientEventHandler* lpHandler, unsigned long SenderID, 
                              unsigned long ReferenceID, unsigned long PartyID, unsigned short Cmd)
{
    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktPC), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);

    PktPC* lpPCPt = reinterpret_cast<PktPC *>(lpMsgBlock->wr_ptr());

    lpPCPt->m_wCmd			= Cmd;
    lpPCPt->m_dwPartyID		= PartyID;
    lpPCPt->m_dwSenderID	= SenderID;
    lpPCPt->m_dwReferenceID	= ReferenceID;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktPC), CmdCharPartyCmd, 0, 0);
}

bool SendPacket::CharPartyFind(ClientNet::CClientEventHandler* lpHandler, unsigned long CharID)
{
    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktPF), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);

    PktPF* lpPFPt = reinterpret_cast<PktPF *>(lpMsgBlock->wr_ptr());
    lpPFPt->m_dwCharID = CharID;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktPF), CmdCharPartyFind, 0, 0);
}


/*

bool SendPacket::CharPartyMemInfo(ClientSocket& clientSocket, unsigned long CharID, 
								  unsigned long PartyID, unsigned char Class,
								  char Level, POS Pos,
								  unsigned short MaxHP, short CurHP, 
								  unsigned short MaxMP, short CurMP, 
								  unsigned long Chant, unsigned long Enchant)
{
	ClientNet::CSession& Session = clientSocket.GetSession(ClientSocket::UDPSession);

	ClientNet::CNetworkMsgBlock* lpMsgBlock = 
		ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktPM), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

	PktPM* lpPMPt = reinterpret_cast<PktPM *>(lpMsgBlock->wr_ptr());

	lpPMPt->m_dwCharID			= CharID;
	lpPMPt->m_dwPartyID			= PartyID;

	lpPMPt->m_cClass			= Class;
	lpPMPt->m_cLevel			= Level;
	lpPMPt->m_Position			= Pos;

	lpPMPt->m_wMaxHP			= MaxHP;
	lpPMPt->m_sCurrHP			= CurHP;
	lpPMPt->m_wMaxMP			= MaxMP;
	lpPMPt->m_sCurrMP			= CurMP;

	lpPMPt->m_dwChant			= Chant;
	lpPMPt->m_dwEnchant			= Enchant;

	lpPMPt->m_PublicAddress		= clientSocket.GetAddress(ClientSocket::PublicAddr);
	lpPMPt->m_PrivateAddress	= clientSocket.GetAddress(ClientSocket::PrivateAddr);

	SendUDPList(Session, clientSocket.GetUDPList(ClientSocket::ListType_Party),
		clientSocket.IsRealIP(), lpMsgBlock, sizeof(PktPM), CmdCharPartyMemInfo, 0, 0);

	return true;
}

bool SendPacket::CharPartyMemAddress(ClientSocket& clientSocket, unsigned long CharID,
									 unsigned long PartyID, const SOCKADDR_IN& PublicAddress,
									 const SOCKADDR_IN& PrivateAddress)
{
	ClientNet::CSession& Session = clientSocket.GetSession(ClientSocket::UDPSession);

	ClientNet::CNetworkMsgBlock* lpMsgBlock = 
		ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktPMA), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

	PktPMA* lpPMAPt = reinterpret_cast<PktPMA *>(lpMsgBlock->wr_ptr());

	lpPMAPt->m_dwCharID			= CharID;
	lpPMAPt->m_dwPartyID		= PartyID;
	lpPMAPt->m_PublicAddress	= PublicAddress;
	lpPMAPt->m_PrivateAddress	= PrivateAddress;

	SendUDPList(Session, clientSocket.GetUDPList(ClientSocket::ListType_Party),
		clientSocket.IsRealIP(), lpMsgBlock, sizeof(PktPMA), CmdCharPartyMemAddress, 0, 0);

	return true;
}

*/

//--------------------------------------------------------------------------------------------------------------
// 친구

bool SendPacket::CharFriendAddRequest(ClientNet::CClientEventHandler* lpHandler, char* szName, PktFriendAddReq::CMD addType)
{
    CHECK_NULL_RETURN(szName, false);

    // 친구 또는 거부 리스트에 추가한다.
    switch(addType)
    {
		case PktFriendAddReq::ADD_FRIEND_REQ:
		case PktFriendAddReq::BAN_FRIEND_REQ:
			break;

		default:
			return false;
    }

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktFriendAddReq), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);
	
    PktFriendAddReq* lpPktFriendAddRequest = reinterpret_cast<PktFriendAddReq*>(lpMsgBlock->wr_ptr());

    memset(lpPktFriendAddRequest->m_szName, 0, PktFriendAddReq::MAX_NAME);
    memcpy(lpPktFriendAddRequest->m_szName, szName, PktFriendAddReq::MAX_NAME);
    lpPktFriendAddRequest->m_dwCID  = 0;
    lpPktFriendAddRequest->m_cCmd   = addType;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktFriendAddReq), CmdFriendAddRequest, 0);    
}


bool SendPacket::CharFriendRemoveRequest(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, 
                                         PktFriendRemoveReq::CMD removeType, char* szTargetName)
{
    // 친구 또는 거부 리스트에서 제거한다.
    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktFriendRemoveReq), lpHandler->GetPeerAddress());
	CHECK_NULL_RETURN(lpMsgBlock, false);

	PktFriendRemoveReq* lpPktFriendRemoveRequest = reinterpret_cast<PktFriendRemoveReq*>(lpMsgBlock->wr_ptr());

	
	memset(lpPktFriendRemoveRequest->m_szName, 0, PktFriendRemoveReq::MAX_NAME);
	
	if(szTargetName)
		memcpy(lpPktFriendRemoveRequest->m_szName, szTargetName, PktFriendRemoveReq::MAX_NAME);

    lpPktFriendRemoveRequest->m_cCmd    = removeType;
    lpPktFriendRemoveRequest->m_dwCID   = dwCID;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktFriendRemoveReq), CmdFriendRemoveRequest, 0);    
}


bool SendPacket::CharFriendEtcRequest(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCID, 
                                      unsigned long dwData, PktFriendEtc::CMD etcType)
{
    switch(etcType)
    {
		case PktFriendEtc::SETGROUP:    
			break;

		default:                        
			return false;
    }

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktFriendEtc), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);
	
    PktFriendEtc* lpPktFriendEtc = reinterpret_cast<PktFriendEtc*>(lpMsgBlock->wr_ptr());

    lpPktFriendEtc->m_dwCID     = dwCID;
    lpPktFriendEtc->m_dwData    = dwData;
    lpPktFriendEtc->m_cCmd      = etcType;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktFriendEtc), CmdFriendEtcRequest, 0);    
}


//--------------------------------------------------------------------------------------------------------------
// 기타

bool SendPacket::CharFameInfo(ClientNet::CClientEventHandler* lpHandler, unsigned long CharID)
{
	ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktFI), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

	PktFI* lpFIPt = reinterpret_cast<PktFI *>(lpMsgBlock->wr_ptr());

	lpFIPt->m_dwCharID	= CharID;

	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktFI), CmdCharFameInfo, 0, 0);
}

bool SendPacket::CharRankingInfo(ClientNet::CClientEventHandler* lpHandler, unsigned long dwCharID, unsigned char cClass, unsigned char cPage)
{
	ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktRankingInfo), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

	PktRankingInfo* lpRankingInfo = reinterpret_cast<PktRankingInfo* >(lpMsgBlock->wr_ptr());

	lpRankingInfo->m_dwCharID = dwCharID;
	lpRankingInfo->m_cClass = cClass;
	lpRankingInfo->m_cPage = cPage;

	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktRankingInfo), CmdRankingInfo, 0, 0);
}

bool SendPacket::CharAuthorizePanel(ClientNet::CClientEventHandler* lpHandler, unsigned long dwAffectedID, 
                                    unsigned long dwCasterID, unsigned char cCmd)
{
    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktAP), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);
	
    PktAP* lpAPPt = reinterpret_cast<PktAP *>(lpMsgBlock->wr_ptr());

    lpAPPt->m_dwAffectedID	= dwAffectedID;
    lpAPPt->m_dwCasterID	= dwCasterID;
    lpAPPt->m_cCmd			= cCmd;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktAP), CmdCharAuthorizePanel, 0, 0);    
}

// NewChat - 채팅서버
bool SendPacket::CharChat(ClientNet::CClientEventHandler* lpHandler, unsigned char Cmd, char* Message, int MessageSize, unsigned short usLang)
{
    unsigned short usPacketLen = static_cast<unsigned short>(sizeof(PktChat) + MessageSize + 1);

    CHECK_TRUE_RETURN(NULL == Message || MessageSize <= 0 || PktMaxLen <= usPacketLen, false);

    switch (Cmd)
    {        
		case PktChat::NORMAL:               // 보통
		case PktChat::PARTY:	            // 파티
		case PktChat::DICE:					// 주사위
		case PktChat::GUILD:	            // 길드
		case PktChat::CLIENT_LOG:		    // 로그
		case PktChat::TRADE :				// 거래
		case PktChat::SHOUT:                // 외치기
		case PktChat::ADMIN_NORMAL_CHAT:    // 운영자 보통 말
		case PktChat::ADMIN_SHOUT:          // 운영자 외치기
		case PktChat::FIND_PARTY:			// 파티 찾기
		case PktChat::GMREPORT:				// 운영자에게 신고
			break;

		case PktChat::NOTICE:               // 공지 (서버에서만 보낸다)
		default:

			return false;
    }

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(usPacketLen, lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);
    
    PktChat* lpPktChat = reinterpret_cast<PktChat *>(lpMsgBlock->wr_ptr());

    lpPktChat->m_dwSenderCID = 0;
	lpPktChat->m_usLang = usLang;
    lpPktChat->m_cCmd = Cmd;
    lpPktChat->m_cNum = 0;    

    *(Message + MessageSize) = NULL;
    strncpy(lpMsgBlock->wr_ptr() + sizeof(PktChat), Message, MessageSize + 1);
    *(lpMsgBlock->wr_ptr() + sizeof(PktChat) + MessageSize) = NULL;

    return SendPacket(lpHandler, lpMsgBlock, usPacketLen, CmdCharChat, 0, 0);
}

bool SendPacket::CharTargetedChat(ClientNet::CClientEventHandler* lpHandler, unsigned char Cmd, 
                                  char* TargetNames, int nTargetNamesNum, 
                                  char* Message, int MessageSize)
{
    const int NAME_LENGTH = 16;
    unsigned short usPacketLen = static_cast<unsigned short>(sizeof(PktChat) + 
        nTargetNamesNum * NAME_LENGTH + MessageSize + 1);

    CHECK_TRUE_RETURN(NULL == TargetNames || nTargetNamesNum <= 0 || 100 < nTargetNamesNum ||
        NULL == Message || MessageSize <= 0 || PktMaxLen <= usPacketLen, false);

    switch (Cmd) 
    {
		case PktChat::FRIEND:	    // 프랜드
		case PktChat::STALL:		// 노점상 대화방
		case PktChat::CAMP_SHOP:	// 길드 요새 상점 대화방
			break;

		default:
			return false;
    }
    
    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(usPacketLen, lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);

    PktChat* lpPktChat = reinterpret_cast<PktChat *>(lpMsgBlock->wr_ptr());

    lpPktChat->m_cCmd = Cmd;
    lpPktChat->m_cNum = nTargetNamesNum;

    *(Message + MessageSize) = NULL;

    memcpy(lpMsgBlock->wr_ptr() + sizeof(PktChat), TargetNames, nTargetNamesNum * NAME_LENGTH);
    strncpy(lpMsgBlock->wr_ptr() + sizeof(PktChat) + nTargetNamesNum * NAME_LENGTH, Message, MessageSize + 1);
    *(lpMsgBlock->wr_ptr() + sizeof(PktChat) + nTargetNamesNum * NAME_LENGTH + MessageSize) = NULL;

    return SendPacket(lpHandler, lpMsgBlock, usPacketLen, CmdCharChat, 0, 0);
}

bool SendPacket::CharWhisper(ClientNet::CClientEventHandler* lpHandler, char* SenderName, 
                             char *RecverName, char* Message, int MessageSize)
{
    unsigned short usPacketLen = static_cast<unsigned short>(sizeof(PktWh) + MessageSize + 1);

    CHECK_TRUE_RETURN(SenderName == NULL || RecverName == NULL || 
        Message == NULL || MessageSize <= 0 || usPacketLen <= MessageSize, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(usPacketLen, lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);
	
    PktWh* lpWhPt = reinterpret_cast<PktWh *>(lpMsgBlock->wr_ptr());

    strncpy(lpWhPt->m_SenderName, SenderName, CHAR_INFOST::MAX_NAME_LEN);
    strncpy(lpWhPt->m_RecverName, RecverName, CHAR_INFOST::MAX_NAME_LEN);

    *(Message + MessageSize) = NULL;
    CopyMemory(lpMsgBlock->wr_ptr() + sizeof(PktWh), Message, MessageSize);
	*(lpMsgBlock->wr_ptr() + sizeof(PktWh) + MessageSize) = NULL;
	
    return SendPacket(lpHandler, lpMsgBlock, usPacketLen, CmdCharWhisper, 0, 0);
}


/* NOT USE
bool SendPacket::CharPartyData(ClientSocket& clientSocket, unsigned long CharID, 
                               unsigned short Cmd, unsigned short Len, void *Data)
{
    ClientNet::CSession& Session = clientSocket.GetSession(ClientSocket::UDPSession);
    CHECK_TRUE_RETURN(0 == CharID, false);

    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(sizeof(PktPMD) + Len, lpHandler->GetPeerAddress());

    PktMD* lpMDPt = reinterpret_cast<PktMD *>(lpMsgBlock->wr_ptr());

	lpMDPt->m_dwCharID	= CharID;
	lpMDPt->m_wCmd		= Cmd;
	lpMDPt->m_wLen		= Len;

	CopyMemory(lpMsgBlock->wr_ptr() + sizeof(PktMD), Data, Len);

    SendUDPList(Session, clientSocket.GetUDPList(ClientSocket::ListType_Party), 
        clientSocket.IsRealIP(), lpMsgBlock, sizeof(PktPMD) + Len, CmdCharPartyMemData, 0, 0);

	return true;
}
*/
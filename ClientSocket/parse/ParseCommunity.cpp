
#include "stdafx.h"

#include <Network/Packet/PacketStruct/CharCommunityPacket.h>
#include <Network/Packet/PacketStruct/FriendPacket.h>
#include <Network/Packet/PacketStruct/GuildPacket.h>
#include <Network/Packet/PacketStruct/PartyPacket.h>

#include "ParseCommunity.h"
#include "GMMemory.h"


//--------------------------------------------------------------------------------------------------------------
// 길드

unsigned long ParsePacket::HandleCharCreateGuild(PktBase* lpPktBase, unsigned long *dwCID, unsigned long *dwGID, 
												 unsigned char *cInclination, char* szGuildName)
{
	PktCreateGuild* lpCGPt = static_cast<PktCreateGuild *>(lpPktBase);

	*dwCID = lpCGPt->m_dwCID;
	*dwGID = lpCGPt->m_dwGID;
	*cInclination = lpCGPt->m_cInclination;
	strncpy(szGuildName, lpCGPt->m_szGuildName, Guild::MAX_GUILD_NAME_LEN);

	return lpCGPt->GetError();
}

unsigned long ParsePacket::HandleCharGuildCmd(PktBase* lpPktBase, unsigned long *dwGID, unsigned long *dwSenderID, unsigned long *dwReferenceID, 
											  char* szGuildName, char* szSenderName, unsigned short *wCmd)
{
	PktGuildCmd* lpGCPt = static_cast<PktGuildCmd *>(lpPktBase);

	*dwGID = lpGCPt->m_dwGID;

	*dwSenderID = lpGCPt->m_dwSenderID;
	*dwReferenceID = lpGCPt->m_dwReferenceID;

	strncpy(szGuildName, lpGCPt->m_szGuildName, Guild::MAX_GUILD_NAME_LEN);
	strncpy(szSenderName, lpGCPt->m_szSenderName, Guild::MAX_MEMBER_NAME_LEN);

	*wCmd = lpGCPt->m_wCmd;

	return lpGCPt->GetError();
}

unsigned long ParsePacket::HandleCharGuildMark(PktBase* lpPktBase, unsigned long* dwCID, unsigned long* dwGID, 
											   char* szMark, unsigned long *dwGold)
{
	PktGuildMark* lpGMPt = static_cast<PktGuildMark *>(lpPktBase);

	*dwCID = lpGMPt->m_dwCID;
	*dwGID = lpGMPt->m_dwGID;

	::memcpy(szMark, lpGMPt->m_szMark, Guild::MAX_MARK_SIZE);
	*dwGold = lpGMPt->m_dwGold;

	return lpGMPt->GetError();
}

unsigned long ParsePacket::HandleCharGuildLevel(PktBase* lpPktBase, unsigned long *dwGID, unsigned char *cLevel, unsigned long *dwGold)
{
	PktGuildLevel* lpGLPt = static_cast<PktGuildLevel *>(lpPktBase);

	*dwGID = lpGLPt->m_dwUID;
	*cLevel = lpGLPt->m_cLevel;
	*dwGold = lpGLPt->m_dwGold;

	return lpGLPt->GetError();
}

unsigned long ParsePacket::HandleCharGuildRelation(PktBase* lpPktBase, unsigned long *dwCID, unsigned long *dwGID,
												   unsigned long *dwTargetGID, unsigned long *dwValue, unsigned char *cSubCmd,
												   char* szGuildName, char* szTempGuildName)
{
	PktGuildRelation* lpGRPt = static_cast<PktGuildRelation *>(lpPktBase);

	*dwCID = lpGRPt->m_dwCID;
	*dwGID = lpGRPt->m_dwGID;
	*dwTargetGID = lpGRPt->m_dwTargetGID;
	*dwValue = lpGRPt->m_dwValue;
	*cSubCmd = lpGRPt->m_cSubCmd;

	strncpy(szGuildName, lpGRPt->m_szGuildName, Guild::MAX_GUILD_NAME_LEN);
	strncpy(szTempGuildName, lpGRPt->m_szTempGuildName, Guild::MAX_GUILD_NAME_LEN);

	return lpGRPt->GetError();
}

unsigned long ParsePacket::HandleCharGuildInclination(PktBase* lpPktBase, unsigned long *dwGID, unsigned char *cInclination)
{
	PktGuildInclination* lpGIPt = static_cast<PktGuildInclination *>(lpPktBase);

	*dwGID = lpGIPt->m_dwUID;
	*cInclination = lpGIPt->m_cInclination;

	return lpGIPt->GetError();
}

unsigned long ParsePacket::HandleCharGuildList(PktBase* lpPktBase, unsigned long *dwCID, 
											   unsigned char *cSortCmd, unsigned char *cPage, 
											   unsigned char *cSmallNodeNum, GuildSmallInfoNode* lpSmallNode, 
											   unsigned char *cLargeNodeNum, GuildLargeInfoNode* lpLargeNode)
{
	PktGuildList* lpGLPt = static_cast<PktGuildList *>(lpPktBase);

	*dwCID = lpGLPt->m_dwCID;
	*cSortCmd = lpGLPt->m_cSortCmd;
	*cPage = lpGLPt->m_cPage;
	*cSmallNodeNum = lpGLPt->m_cSmallNodeNum;
	*cLargeNodeNum = lpGLPt->m_cLargeNodeNum;

	GuildSmallInfoNode* lpRecvSmallNode = reinterpret_cast<GuildSmallInfoNode *>(lpGLPt + 1);
	GuildLargeInfoNode* lpRecvLargeNode = reinterpret_cast<GuildLargeInfoNode *>(lpRecvSmallNode + lpGLPt->m_cSmallNodeNum);
	
	CopyMemory(lpSmallNode, lpRecvSmallNode, sizeof(GuildSmallInfoNode) * lpGLPt->m_cSmallNodeNum);
	CopyMemory(lpLargeNode, lpRecvLargeNode, sizeof(GuildLargeInfoNode) * lpGLPt->m_cLargeNodeNum);
	
	return lpGLPt->GetError();
}

unsigned long ParsePacket::HandleCharGuildRight(PktBase* lpPktBase, unsigned long *dwGID, GuildRight *guildRight)
{
	PktGuildRight* lpGRPt = static_cast<PktGuildRight *>(lpPktBase);

	*dwGID = lpGRPt->m_dwUID;
	*guildRight = lpGRPt->m_GuildRight;

	return lpGRPt->GetError();
}

unsigned long ParsePacket::HandleCharGuildMemberList(PktBase* lpPktBase, unsigned long *dwCID, unsigned char* cMemberType,
													 unsigned char *cSortCmd, unsigned char *cPage, unsigned char *cTotalMemberNum, 
													 unsigned char *cNodeNum, GuildMemberInfoNode* lpMemberNode)
{
	PktGuildMemberList* lpGMLPt = static_cast<PktGuildMemberList *>(lpPktBase);

	*dwCID = lpGMLPt->m_dwCID;

	*cMemberType = lpGMLPt->m_cMemberType;
	*cSortCmd = lpGMLPt->m_cSortCmd;
	*cPage = lpGMLPt->m_cPage;

	*cTotalMemberNum = lpGMLPt->m_cTotalMemberNum;
	*cNodeNum = lpGMLPt->m_cNodeNum;

	GuildMemberInfoNode* lpRecvNode = reinterpret_cast<GuildMemberInfoNode *>(lpGMLPt + 1);
	CopyMemory(lpMemberNode, lpRecvNode, sizeof(GuildMemberInfoNode) * lpGMLPt->m_cNodeNum);

	return lpGMLPt->GetError();
}

unsigned long ParsePacket::HandleCharMyGuildInfo(PktBase* lpPktBase, unsigned long *dwGold, 
												 GuildRight *guildRight, unsigned char *cTitle)
{
	PktMyGuildInfo* lpMGIPt = static_cast<PktMyGuildInfo *>(lpPktBase);

	*dwGold = lpMGIPt->m_dwGold;
	*guildRight = lpMGIPt->m_GuildRight;
	*cTitle = lpMGIPt->m_cTitle;

	return lpMGIPt->GetError();
}

unsigned long ParsePacket::HandleCharGuildSafe(PktBase* lpPktBase, unsigned long *dwCID, char* szCharName, 
											   unsigned long *dwSafeGold, unsigned long *dwCharGold, unsigned char *cCmd)
{
	PktGuildSafeAck* lpGSAckPt = static_cast<PktGuildSafeAck *>(lpPktBase);

	*dwCID = lpGSAckPt->m_dwCID;
	*dwSafeGold = lpGSAckPt->m_dwSafeGold;
	*dwCharGold = lpGSAckPt->m_dwCharGold;
	*cCmd = lpGSAckPt->m_cCmd;

	strncpy(szCharName, lpGSAckPt->m_szCharName, Guild::MAX_MEMBER_NAME_LEN);

	return lpGSAckPt->GetError();
}

unsigned long ParsePacket::HandleCharGuildHostilityList(PktBase* lpPktBase, unsigned long *dwCID, unsigned char *cCurrentPage,
														unsigned char *cPageState, unsigned char* cNodeNum, unsigned short* wTotalItemNum,
														char* szHostility, char* szAlert, GuildHostilityInfoNode* lpHostilityNode)
{
	PktGuildHostilityList* lpGHLPt = static_cast<PktGuildHostilityList *>(lpPktBase);

	*dwCID = lpGHLPt->m_dwCID;
	*cCurrentPage = lpGHLPt->m_cCurrentPage;
	*cPageState = lpGHLPt->m_cPageState;
	*cNodeNum = lpGHLPt->m_cNodeNum;
	*wTotalItemNum = lpGHLPt->m_wTotalItemNum;

	strncpy(szHostility, lpGHLPt->m_szHostility, Guild::MAX_GUILD_NAME_LEN);
	strncpy(szAlert, lpGHLPt->m_szAlert, Guild::MAX_GUILD_NAME_LEN);

	GuildHostilityInfoNode* lpRecvNode = reinterpret_cast<GuildHostilityInfoNode *>(lpGHLPt + 1);
	CopyMemory(lpHostilityNode, lpRecvNode, sizeof(GuildHostilityInfoNode) * lpGHLPt->m_cNodeNum);

	return lpGHLPt->GetError();
}

unsigned long ParsePacket::HandleCharGuildRelationInfo(PktBase* lpPktBase, unsigned long *dwCID,
													   unsigned long *dwGID, unsigned short *wNodeNum,
													   GuildRelationInfoNode* lpRelationNode )
{
	PktGuildRelationInfo* lpGRIPt = static_cast<PktGuildRelationInfo *>(lpPktBase);

	*dwCID = lpGRIPt->m_dwCID;
	*dwGID = lpGRIPt->m_dwGID;
	*wNodeNum = lpGRIPt->m_wNodeNum;

	GuildRelationInfoNode* lpRecvNode = reinterpret_cast<GuildRelationInfoNode* >( lpGRIPt + 1 );
	CopyMemory(lpRelationNode, lpRecvNode, sizeof(GuildRelationInfoNode) * lpGRIPt->m_wNodeNum);

	return lpGRIPt->GetError();
}



//--------------------------------------------------------------------------------------------------------------
// 파티

unsigned long ParsePacket::HandleCharPartyInfo(PktBase* lpPktBase, PARTY_EX* lpParty)
{
    PktPI* lpPIPt  = static_cast<PktPI *>(lpPktBase);
    *lpParty    = lpPIPt->m_Party;

    return lpPIPt->GetError();
}



unsigned long ParsePacket::HandleCharPartyCmd(PktBase* lpPktBase, AddressInfo* SenderAddress, 
                                              char *SenderName, unsigned long *dwSenderID, unsigned long *dwReferenceID, 
											  unsigned long *dwPartyID, unsigned short *Cmd)
{
    PktPCAck* lpPCAckPt = static_cast<PktPCAck *>(lpPktBase);

    strncpy(SenderName, lpPCAckPt->m_strSenderName, 16);    

    *SenderAddress	= lpPCAckPt->m_SenderAddressInfo;    
    *dwSenderID		= lpPCAckPt->m_dwSenderID;
    *dwReferenceID	= lpPCAckPt->m_dwReferenceID;
    *dwPartyID		= lpPCAckPt->m_dwPartyID;	
    *Cmd			= lpPCAckPt->m_cCmd;

    return lpPCAckPt->GetError();
}

unsigned long ParsePacket::HandleCharPartyCmdInfo(PktBase* lpPktBase, unsigned long *dwMemberID, 
                                                  unsigned long *dwPartyID, unsigned short *Cmd)
{
    PktPCInfo* lpPCInfoPt = static_cast<PktPCInfo *>(lpPktBase);

    *dwMemberID		= lpPCInfoPt->m_dwMemberID;
    *dwPartyID		= lpPCInfoPt->m_dwPartyID;
    *Cmd			= lpPCInfoPt->m_cCmd;

    return lpPCInfoPt->GetError();
}



unsigned long ParsePacket::HandleCharPartyMemData(PktBase* lpPktBase, unsigned long& dwPartyID, unsigned long& dwSenderID, unsigned long& dwGID, 
                                                  unsigned long& dwServerID, unsigned short& wClass, char& cLevel,
												  const char*& szSenderName, unsigned short& wCmd)
{
    if (sizeof(PktPMD) != lpPktBase->GetLen())
    {
        return 1;
    }

    PktPMD* lpPktPMD = reinterpret_cast<PktPMD*>(lpPktBase);

    dwPartyID       = lpPktPMD->m_dwPartyID;
    dwSenderID      = lpPktPMD->m_dwSenderID;
    dwGID			= lpPktPMD->m_dwGID;
	cLevel			= lpPktPMD->m_cLevel;
	wClass			= lpPktPMD->m_wClass;
	dwServerID		= lpPktPMD->m_dwServerID;	
    szSenderName    = lpPktPMD->m_strSenderName;
    wCmd            = lpPktPMD->m_wCmd;

    return lpPktBase->GetError();
}


inline void COPY_AND_ADVANCE_SRC(void* dst, const char*& src, size_t size)
{
    memcpy(dst, src, size); src += size;
}

unsigned long ParsePacket::HandleCharPartyMemInfo(PktBase* lpPktBase, PartyMemberData* lpPartyMemberData, 
                                                  unsigned int& nMemberDataNum)
{    
    memset(lpPartyMemberData, 0, sizeof(PartyMemberData) * nMemberDataNum);

    const char* szDataPos = reinterpret_cast<char*>(lpPktBase + 1);
    const char* szDataEnd = szDataPos + ((sizeof(PktBase) < lpPktBase->GetLen()) ? lpPktBase->GetLen() - sizeof(PktBase) : 0);

    PartyMemberData* lpPMDPos = lpPartyMemberData;
    PartyMemberData* lpPMDEnd = lpPartyMemberData + nMemberDataNum;

    for(nMemberDataNum = 0 ; 
        szDataPos + sizeof(unsigned long) + sizeof(unsigned short) < szDataEnd && lpPMDPos < lpPMDEnd;
        ++lpPMDPos)
    {           
        COPY_AND_ADVANCE_SRC(&lpPMDPos->m_dwCID, szDataPos, sizeof(unsigned long));
        COPY_AND_ADVANCE_SRC(&lpPMDPos->m_usDataChanged, szDataPos, sizeof(unsigned short));

        unsigned short usDataChanged = lpPMDPos->m_usDataChanged;

		// ------------------------------------------------------------------------------------------------------------------------------------------------------
        // 복사 순서가 바뀌면 치명적입니다.

        if (usDataChanged & PartyMemberData::CHANGED_XPOS)       		{ COPY_AND_ADVANCE_SRC(&lpPMDPos->m_fXPos, szDataPos, sizeof(float));	}
        if (usDataChanged & PartyMemberData::CHANGED_YPOS)       		{ COPY_AND_ADVANCE_SRC(&lpPMDPos->m_fYPos, szDataPos, sizeof(float)); 	}
        if (usDataChanged & PartyMemberData::CHANGED_ZPOS)       		{ COPY_AND_ADVANCE_SRC(&lpPMDPos->m_fZPos, szDataPos, sizeof(float)); 	}
        if (usDataChanged & PartyMemberData::CHANGED_MAX_HP)     		{ COPY_AND_ADVANCE_SRC(&lpPMDPos->m_usMaxHP, szDataPos, sizeof(unsigned short)); 	}
        if (usDataChanged & PartyMemberData::CHANGED_MAX_MP)     		{ COPY_AND_ADVANCE_SRC(&lpPMDPos->m_usMaxMP, szDataPos, sizeof(unsigned short)); 	}
        if (usDataChanged & PartyMemberData::CHANGED_CUR_HP)     		{ COPY_AND_ADVANCE_SRC(&lpPMDPos->m_usCurHP, szDataPos, sizeof(unsigned short)); 	}
        if (usDataChanged & PartyMemberData::CHANGED_CUR_MP)     		{ COPY_AND_ADVANCE_SRC(&lpPMDPos->m_usCurMP, szDataPos, sizeof(unsigned short)); 	}
        if (usDataChanged & PartyMemberData::CHANGED_LEVEL)      		{ COPY_AND_ADVANCE_SRC(&lpPMDPos->m_cLevel, szDataPos, sizeof(unsigned char));		}
        if (usDataChanged & PartyMemberData::CHANGED_CLASS)      		{ COPY_AND_ADVANCE_SRC(&lpPMDPos->m_cClass, szDataPos, sizeof(unsigned char)); 		}
		if (usDataChanged & PartyMemberData::CHANGED_ENCHANT_INFO_0)	{ COPY_AND_ADVANCE_SRC(&lpPMDPos->m_dwEnchantInfo[0], szDataPos, sizeof(unsigned long));	}
		if (usDataChanged & PartyMemberData::CHANGED_ENCHANT_INFO_1)	{ COPY_AND_ADVANCE_SRC(&lpPMDPos->m_dwEnchantInfo[1], szDataPos, sizeof(unsigned long));	}
		if (usDataChanged & PartyMemberData::CHANGED_ENCHANT_INFO_2)	{ COPY_AND_ADVANCE_SRC(&lpPMDPos->m_dwEnchantInfo[2], szDataPos, sizeof(unsigned long));	}
		if (usDataChanged & PartyMemberData::CHANGED_ENCHANT_INFO_3)	{ COPY_AND_ADVANCE_SRC(&lpPMDPos->m_dwEnchantInfo[3], szDataPos, sizeof(unsigned long));	}

		// ------------------------------------------------------------------------------------------------------------------------------------------------------
    }

    nMemberDataNum = static_cast<unsigned int>(lpPMDPos - lpPartyMemberData);
    return lpPktBase->GetError();
}

unsigned long ParsePacket::HandleCharPartyFind(PktBase* lpPktBase, unsigned long *dwCharID, 
                                               unsigned char *MemberFindPartyNum, 
                                               unsigned char *PartyFindMemberNum, 
                                               MemberFindParty **lppMemberFindParty, 
                                               PartyFindMember **lppPartyFindMember)
{
    PktPFAck* lpPFAckPt = static_cast<PktPFAck *>(lpPktBase);

    *dwCharID				= lpPFAckPt->m_dwCharID;
    *MemberFindPartyNum		= lpPFAckPt->m_cMemberFindPartyNum;
    *PartyFindMemberNum		= lpPFAckPt->m_cPartyFindMemberNum;

    *lppMemberFindParty		= (MemberFindParty*)(reinterpret_cast<char*>(lpPFAckPt + 1));
    *lppPartyFindMember		= (PartyFindMember*)(reinterpret_cast<char*>(lpPFAckPt + 1) + 
        (*MemberFindPartyNum * sizeof(MemberFindParty)));

    return lpPFAckPt->GetError();
}

//--------------------------------------------------------------------------------------------------------------
// 친구

unsigned long ParsePacket::HandleCharFriendDB(PktBase* lpPktBase, unsigned char* cDataType, 
											  char **ppFriendListData, unsigned long *dwDataSize)
{
	PktFriendDB* lpPktFriendDB = static_cast<PktFriendDB *>(lpPktBase);

	switch(lpPktFriendDB->m_cCmd)
	{
		case PktFriendDB::FRIEND_LIST:
		case PktFriendDB::BAN_LIST:

			*cDataType        = lpPktFriendDB->m_cCmd;
			*dwDataSize       = lpPktFriendDB->m_dwData;
			*ppFriendListData = reinterpret_cast<char*>(lpPktFriendDB + 1);
			break;

		default:
			return 1;
	}

	return lpPktFriendDB->GetError();
}


unsigned long ParsePacket::HandleCharFriendAck(PktBase* lpPktBase, unsigned char* cCmd, unsigned long *dwCID, char **ppszName, 
											   unsigned long* dwGID, unsigned short* wClass, char* cLevel, unsigned long* dwServerID)
{
	PktFriendAck* lpPktFriendAck = static_cast<PktFriendAck *>(lpPktBase);

	*cCmd		= lpPktFriendAck->m_cCmd;
	*dwCID		= lpPktFriendAck->m_dwCID;
	*dwGID		= lpPktFriendAck->m_dwGID;
	*wClass		= lpPktFriendAck->m_wClass;
	*cLevel		= lpPktFriendAck->m_cLevel;
	*ppszName   = lpPktFriendAck->m_szName;
	*dwServerID	= lpPktFriendAck->m_dwServerID;

	return lpPktFriendAck->GetError();
}


unsigned long ParsePacket::HandleCharFriendRegistered(PktBase* lpPktBase, unsigned long* dwCID, char* szName)
{
	PktFriendAddReq* lpPktFriendAddRequest = static_cast<PktFriendAddReq *>(lpPktBase);    

	memcpy(szName, lpPktFriendAddRequest->m_szName, PktFriendAddReq::MAX_NAME);
	*dwCID = lpPktFriendAddRequest->m_dwCID;   

	return lpPktFriendAddRequest->GetError();
}


//--------------------------------------------------------------------------------------------------------------
// 기타

unsigned long ParsePacket::HandleCharChat(PktBase* lpPktBase, unsigned long& dwSenderCID, unsigned short *Cmd, unsigned short *Lang, char *Message)
{
	PktChat* lpPktChat = static_cast<PktChat*>(lpPktBase);
		
	int nLen	= lpPktChat->GetLen();
	*Cmd		= lpPktChat->m_cCmd;
	*Lang		= lpPktChat->m_usLang;
    dwSenderCID = lpPktChat->m_dwSenderCID;

    char* szMessagePos = reinterpret_cast<char*>(lpPktChat + 1);
    char* szMessageEnd = reinterpret_cast<char*>(lpPktChat) + nLen;

    if (0 != lpPktChat->m_cNum)
    {
        if (szMessagePos + sizeof(char) * CHAR_INFOST::MAX_NAME_LEN * lpPktChat->m_cNum < szMessageEnd)
        {
            szMessagePos += sizeof(char) * CHAR_INFOST::MAX_NAME_LEN * lpPktChat->m_cNum;
        }        
    }

    std::copy(szMessagePos, szMessageEnd, Message);
	Message[szMessageEnd - szMessagePos - 1] = 0;

	return lpPktChat->GetError();
}

unsigned long ParsePacket::HandleCharWhisper(PktBase* lpPktBase, char *SenderName, 
                                             char *RecverName, char *Message)
{
	PktWh* lpWhPt = static_cast<PktWh *>(lpPktBase);
		
	strncpy(SenderName, lpWhPt->m_SenderName, 16);
	strncpy(RecverName, lpWhPt->m_RecverName, 16);

    int nLen = lpWhPt->GetLen();
	if (sizeof(PktWh) < nLen && nLen <= PktWh::PktWhMaxSize + sizeof(PktWh))
    {
        memcpy(Message, reinterpret_cast<char*>(lpWhPt + 1), nLen - sizeof(PktWh));
        Message[nLen - sizeof(PktWh) - 1] = NULL; 
    }

	return lpWhPt->GetError();
}

unsigned long ParsePacket::HandleCharChatBan(PktBase* lpPktBase, bool& bChatBan)
{
	PktChatBan* lpCBPtAck = static_cast<PktChatBan *>(lpPktBase);

	bChatBan = (0 == lpCBPtAck->m_dwMinutes) ? false : true;

	return lpCBPtAck->GetError();
}

unsigned long ParsePacket::HandleCharAuthorizePanel(PktBase* lpPktBase, unsigned long *dwCasterID, 
                                                    char *szCasterName, unsigned char *cCmd)
{
    PktAPAck* lpAPPtAck = static_cast<PktAPAck *>(lpPktBase);

    *dwCasterID	= lpAPPtAck->m_dwCasterID;
    *cCmd		= lpAPPtAck->m_cCmd;

    strncpy(szCasterName, lpAPPtAck->m_szCasterName, CHAR_INFOST::MAX_NAME_LEN);

    return lpAPPtAck->GetError();
}

unsigned long ParsePacket::HandleCharFameInfo(PktBase* lpPktBase, unsigned long *dwCharID,
											  unsigned char *cOtherNation, unsigned char *cOtherClass,
                                              unsigned long *dwFame, unsigned long *dwMileage, 
											  unsigned char *RankingByRace, unsigned char *RankingByClass, 
											  unsigned char *cCmd, char *szWinCharName, char *szLoseCharName/*,unsigned long *KillCount, int* m_KillspreeLevel*/)
{
    PktFIAck* lpFIAckPt = static_cast<PktFIAck *>(lpPktBase);

    *dwCharID			= lpFIAckPt->m_dwCharID;

	*cOtherNation		= (unsigned char)lpFIAckPt->m_cOtherNation;
	*cOtherClass		= lpFIAckPt->m_cOtherClass;

	*dwFame				= lpFIAckPt->m_dwFame;
	*dwMileage			= lpFIAckPt->m_dwMileage;

    *RankingByRace		= lpFIAckPt->m_cRankingByRace;
    *RankingByClass		= lpFIAckPt->m_cRankingByClass;

	*cCmd				= lpFIAckPt->m_cCmd;
//	*KillCount			= lpFIAckPt->m_KillCount;
//	*m_KillspreeLevel	= lpFIAckPt->m_KillspreeLevel;
	strncpy(szWinCharName, lpFIAckPt->m_szWinCharName, PktFIAck::MAX_NAME_LEN);
	strncpy(szLoseCharName, lpFIAckPt->m_szLoseCharName, PktFIAck::MAX_NAME_LEN);

    return lpFIAckPt->GetError();
}

unsigned long ParsePacket::HandleCharRankingInfo(PktBase* lpPktBase, unsigned long *dwCharID, 
												 unsigned char *cClass, unsigned char *cPage, 
												 unsigned short *wNum, RankingNode **lppRankingNode)
{
	PktRankingInfo* lpRankingInfo = static_cast<PktRankingInfo *>(lpPktBase);

	*dwCharID = lpRankingInfo->m_dwCharID;
	*cClass = lpRankingInfo->m_cClass;
	*cPage = lpRankingInfo->m_cPage;
	*wNum = lpRankingInfo->m_wNum;

	*lppRankingNode	= reinterpret_cast<RankingNode *>(lpRankingInfo +  1);

	return lpRankingInfo->GetError();
}


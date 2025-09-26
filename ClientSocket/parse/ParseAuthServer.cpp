#include "stdafx.h"
#include "ParseAuthServer.h"

#include <Network/Packet/PacketStruct/ClientToAuthServer.h>
#include <Network/Packet/PacketStruct/UnifiedCharPacket.h>
#include "GMMemory.h"

unsigned long ParsePacket::HandleAuthAccount(PktBase* lpPktBase, unsigned long& UserID_Out, 
                                             unsigned char& cAgentServerType_Out, unsigned char& cFirstLogin_Out)
{
    PktAUAck* lpPktAUAck = static_cast<PktAUAck*>(lpPktBase);

    UserID_Out	            = lpPktAUAck->m_dwUserID;
    cAgentServerType_Out    = lpPktAUAck->m_cAgentServerType;
    cFirstLogin_Out         = lpPktAUAck->m_cFirstLogin;

    return lpPktAUAck->GetError();
}

// WORK_LIST 2.1 계정 국적 추가
unsigned long ParsePacket::HandleUserLogin(PktBase* lpPktBase, unsigned long *SessionID_Out, unsigned long *UserID_Out, 
										   CHAR_VIEW* CharRecode_Out, sGuildData* CharGuildRecode_Out, unsigned char *cAccountNation,
										   time_t* currentTime, int* nPlayTime_Out, unsigned short* usAdminLevel_Out,
										   unsigned char* cBillingType_Out, unsigned char* cBillingUser_Out, unsigned char* cLoginType_Out,
										   unsigned long* dwTabFlag)
{
    PktULiAck* lpPktULiAck = static_cast<PktULiAck*>(lpPktBase);

    *SessionID_Out	    = lpPktULiAck->m_dwSessionID;
    *UserID_Out		    = lpPktULiAck->m_dwUserID;

    *currentTime        = lpPktULiAck->m_dwCurrentTime;
    *nPlayTime_Out      = lpPktULiAck->m_nPlayTime;
    *usAdminLevel_Out	= lpPktULiAck->m_usAdminLevel;
    *cBillingType_Out   = lpPktULiAck->m_cBillingType;
    *cBillingUser_Out   = lpPktULiAck->m_cBillingUser;

	*cLoginType_Out		= lpPktULiAck->m_cLoginType;

	*dwTabFlag			= lpPktULiAck->m_dwTabFlag;

	// WORK_LIST 2.1 계정 국적 추가
	*cAccountNation		= lpPktULiAck->m_cAccountNation;

	memcpy(CharRecode_Out, lpPktULiAck->m_CharView, sizeof(CHAR_VIEW) * PktULiAck::MAX_CHAR_VIEW);
	memcpy(CharGuildRecode_Out, lpPktULiAck->m_GuildData, sizeof(sGuildData) * PktULiAck::MAX_CHAR_VIEW);

    return lpPktULiAck->GetError();
}

unsigned long ParsePacket::HandleCharCreate(PktBase* lpPktBase, unsigned long *CharID_Out, 
                                            unsigned long *SlotNum_Out, CHAR_VIEW* lpCharView_Out)
{
    PktCCAck* lpPktCCAck = static_cast<PktCCAck*>(lpPktBase);
    
    *CharID_Out			= lpPktCCAck->m_dwCharID;
    *SlotNum_Out		= lpPktCCAck->m_dwSlotNum;
    memcpy(lpCharView_Out, &(lpPktCCAck->m_CharView), sizeof(CHAR_VIEW)); 

    return lpPktCCAck->GetError();
}



unsigned long ParsePacket::HandleCharSelect(PktBase* lpPktBase, unsigned char *Zone_Out, 
                                            unsigned short *lpChannelNum_Out)
{
    PktCSAck* lpPktCSAck = static_cast<PktCSAck*>(lpPktBase);

    *Zone_Out = lpPktCSAck->m_cZone;
    memcpy(lpChannelNum_Out, lpPktCSAck->m_wChannelNum, sizeof(unsigned short) * PktCSAck::MAX_CHANNEL_NUM);

    return lpPktCSAck->GetError();
}



unsigned long ParsePacket::HandleCharDelete(PktBase* lpPktBase, 
                                            unsigned long *UserID_Out, unsigned long *SlotNum_Out)
{
    PktCDAck* lpPktCDAck = static_cast<PktCDAck*>(lpPktBase);

    *UserID_Out			= lpPktCDAck->m_dwUserID;
    *SlotNum_Out		= lpPktCDAck->m_dwSlotNum;

    return lpPktCDAck->GetError();
}

// WORK_LIST 2.1 계정 국적 추가
unsigned long ParsePacket::HandleSelectAccountNation(PktBase* lpPktBase, unsigned long *UserID_Out, 
													 unsigned char *cType, unsigned char *cAccountNation_Out)
{
	PktSelectAccountNation* lpPktSANAck = static_cast<PktSelectAccountNation*>(lpPktBase);

	*UserID_Out			= lpPktSANAck->m_dwUserID;
	*cType				= lpPktSANAck->m_cType;
	*cAccountNation_Out	= lpPktSANAck->m_cAccountNation;

	return lpPktSANAck->GetError();
}

// WORK_LIST 2.3 계정 국적 변경 기능 구현
unsigned long ParsePacket::HandleNationChangeResult(PktBase* lpPktBase, unsigned long *UserID_Out, 
													unsigned long* aryGID_Out, unsigned long* aryFame_Out)
{
	PktNationChangeResult* lpPktNCAck = static_cast<PktNationChangeResult*>(lpPktBase);

	*UserID_Out	= lpPktNCAck->m_dwUserID;
	::memcpy(aryGID_Out, lpPktNCAck->m_dwGID, sizeof(unsigned long) * PktNationChangeResult::MAX_CHAR_VIEW);
	::memcpy(aryFame_Out, lpPktNCAck->m_dwFame, sizeof(unsigned long) * PktNationChangeResult::MAX_CHAR_VIEW);

	return lpPktNCAck->GetError();
}


// 캐릭터 정보
unsigned long ParsePacket::HandleUnifiedCharInfo(PktBase* lpPktBase, 
                                                 unsigned char& cRestrictedPart1ToPart2Level, 
                                                 unsigned char& cRemainCharTransferCount,
                                                 USER_INFO& userInfo, 
                                                 UnifiedStoreInfo** lppUnifiedStoreInfo, 
                                                 unsigned long& dwStoreInfoNum_Out,
                                                 UnifiedCharData** lppUnifiedCharData, 
                                                 unsigned long& dwCharDataNum_Out)
{
    PktUnifiedCharInfo* lpPktUnifiedCharInfo = 
        static_cast<PktUnifiedCharInfo*>(lpPktBase);

    userInfo = lpPktUnifiedCharInfo->userInfo;

    cRestrictedPart1ToPart2Level = lpPktUnifiedCharInfo->cRestrictedPart1ToPart2Level;
    cRemainCharTransferCount     = lpPktUnifiedCharInfo->cRemainCharTransferCount;

    *lppUnifiedStoreInfo    = lpPktUnifiedCharInfo->unifiedStoreInfo;
    dwStoreInfoNum_Out      = lpPktUnifiedCharInfo->cStoreInfoNum;

    *lppUnifiedCharData     = lpPktUnifiedCharInfo->unifiedCharData;
    dwCharDataNum_Out       = lpPktUnifiedCharInfo->cCharDataNum;

    return lpPktBase->GetError();
}

// 성공 / 실패 코드값만 받는다.
unsigned long ParsePacket::HandleUnifiedCharSelectAck(PktBase* lpPktBase)
{
    return lpPktBase->GetError();
}

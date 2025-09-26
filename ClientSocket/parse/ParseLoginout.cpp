
#include "stdafx.h"

#include <Network/Address/INET_Addr.h>
#include <Network/ClientSocket/ClientSocket.h>
#include <Network/Packet/PacketStruct/ServerPacket.h>
#include <Network/Packet/PacketStruct/CharCommunityPacket.h>
#include <Network/Packet/PacketStruct/CharLoginOutPacket.h>
#include <Network/Packet/PacketStruct/CharLoginOutPacketStruct.h>

#include <Network/Packet/PacketStruct/GuildPacket.h>

#include "ParseLoginout.h"
#include "GMMemory.h"


unsigned long ParsePacket::HandleCharLogin(PktBase* lpPktBase, ClientSocket& clientSocket,
                                           bool *bPeace_Out, bool *bCheckRelation, 
                                           unsigned char *cNameChangeCount, unsigned char *cAdminFlag,
										   unsigned char *cGuildWarFlag, unsigned char *cRealmWarFlag,
                                           unsigned char *cRealmPoint, unsigned char *cTacticsFlag, 
										   unsigned int *dwPlayTime, long *lPremiumTime, int *iPremiumType,
										   CHAR_INFOST* lpCharInfo_Out, SKILL* lpSkill_Out, QUICK* lpQuick_Out, SPELL* lpSpell_Out,
										   CHAR_POS* lpPos_Out,                                            
                                           unsigned long *ServerTime_Out, unsigned long *dwServerID,                                           
                                           unsigned short *EquipSize_Out, char **lppEquipBuff_Out, 
                                           unsigned short *InvenSize_Out, char **lppInvenBuff_Out, 
                                           unsigned short *ExtraSize_Out, char **lppExtraBuff_Out, 
                                           unsigned short *ExchangeSize_Out, char **lppExchangeBuff_Out, 
										   unsigned short *TempInvenSize_Out, char **lppTempInvenBuff_Out)
{
    PktCLiAck* lpCLiAckPt = static_cast<PktCLiAck*>(lpPktBase);

    *ServerTime_Out		= lpCLiAckPt->m_dwServerTime;
    *dwServerID			= lpCLiAckPt->m_dwServerID;
    *bPeace_Out			= lpCLiAckPt->m_bPeaceMode;
	*bCheckRelation		= lpCLiAckPt->m_bCheckRelation;
    *cNameChangeCount	= lpCLiAckPt->m_cNameChangeCount;
	*cAdminFlag			= lpCLiAckPt->m_cAdminFlag;
	*cGuildWarFlag		= lpCLiAckPt->m_cGuildWarFlag;
	*cRealmWarFlag		= lpCLiAckPt->m_cRealmWarFlag;
	*cRealmPoint		= lpCLiAckPt->m_cRealmPoint;
	*cTacticsFlag		= lpCLiAckPt->m_cTacticsFlag;
	*dwPlayTime			= lpCLiAckPt->m_PlayTime;
	*lPremiumTime		= lpCLiAckPt->m_PremiumTime;		// 프리미엄 서비스를 위한 정보
	*iPremiumType		= lpCLiAckPt->m_PremiumType;
    
    unsigned short *usUpdateLen	= lpCLiAckPt->m_usUpdate;

    *lppEquipBuff_Out 		= NULL;
    *lppInvenBuff_Out 		= NULL;
    *lppExtraBuff_Out 		= NULL;
    *lppExchangeBuff_Out	= NULL;
	*lppTempInvenBuff_Out	= NULL;

    *EquipSize_Out 			= 0;
    *InvenSize_Out 			= 0;
    *ExtraSize_Out 			= 0;
    *ExchangeSize_Out		= 0;
	*TempInvenSize_Out		= 0;

    char* OffSet = reinterpret_cast<char*>(lpCLiAckPt + 1);
    for (int nCount = 0; nCount < DBUpdateData::MAX_UPDATE_DB; ++nCount) 
    {			
        if (0 != usUpdateLen[nCount])
        { 
            switch (nCount) 
            {
				case DBUpdateData::STATUS_UPDATE:		    *lpCharInfo_Out			= *reinterpret_cast<CHAR_INFOST *>(OffSet);				break;
				case DBUpdateData::POSITION_UPDATE:		    *lpPos_Out				= *reinterpret_cast<CHAR_POS *>(OffSet);				break;
				case DBUpdateData::SKILL_UPDATE:			*lpSkill_Out			= *reinterpret_cast<SKILL *>(OffSet);					break;
				case DBUpdateData::QUICKSLOT_UPDATE:		*lpQuick_Out			= *reinterpret_cast<QUICK *>(OffSet);					break;
				case DBUpdateData::SPELL_UPDATE:			*lpSpell_Out			= *reinterpret_cast<SPELL *>(OffSet);					break;
				case DBUpdateData::ITEM_EQUIP_UPDATE:	    *lppEquipBuff_Out		= OffSet;   *EquipSize_Out = usUpdateLen[nCount];		break;
				case DBUpdateData::ITEM_INVEN_UPDATE:	    *lppInvenBuff_Out		= OffSet;	*InvenSize_Out = usUpdateLen[nCount];		break;
				case DBUpdateData::ITEM_EXTRA_UPDATE:	    *lppExtraBuff_Out		= OffSet;	*ExtraSize_Out = usUpdateLen[nCount];		break;
				case DBUpdateData::ITEM_EXCHANGE_UPDATE:	*lppExchangeBuff_Out	= OffSet;	*ExchangeSize_Out = usUpdateLen[nCount];	break;
				case DBUpdateData::ITEM_TEMPINVEN_UPDATE:	*lppTempInvenBuff_Out	= OffSet;	*TempInvenSize_Out = usUpdateLen[nCount];	break;
            }

            OffSet += usUpdateLen[nCount];
        }
    }

    return lpCLiAckPt->GetError();
}


unsigned long ParsePacket::HandleCharLogout(PktBase* lpPktBase, unsigned long *CharID_Out)
{
    PktCLoAck* lpCLoAckPt = static_cast<PktCLoAck*>(lpPktBase);

    *CharID_Out	= lpCLoAckPt->m_dwCharID;

    return lpCLoAckPt->GetError();
}


unsigned long ParsePacket::HandleCharMoveZone(PktBase* lpPktBase, unsigned char *Zone_Out, unsigned short *lpChannelNum_Out)
{
    PktSZMvAck* lpSZMvAckPt = static_cast<PktSZMvAck*>(lpPktBase);

    *Zone_Out = lpSZMvAckPt->m_cZone;
    memcpy(lpChannelNum_Out, lpSZMvAckPt->m_wChannelNum, sizeof(unsigned short) * PktSZMvAck::MAX_CHANNEL_NUM);

    return lpSZMvAckPt->GetError();
}


unsigned long ParsePacket::HandleServerZone(PktBase* lpPktBase, ClientSocket& clientSocket, 
                                            unsigned long *ServerID_Out)
{
	PktSZAck* lpSZAckPt = static_cast<PktSZAck*>(lpPktBase);

	*ServerID_Out = lpSZAckPt->m_dwServerID;
    clientSocket.SetAddress(ClientSocket::MoveZoneAddr, lpSZAckPt->m_GameServerTCPAddr);	

	return lpSZAckPt->GetError();
}


unsigned long ParsePacket::HandleCSAuth(PktBase* lpPktBase, unsigned long* dwCID, 
										unsigned long* dwAuthCode, GG_AUTH_DATA* lpAuthCode2)
{
	PktCSAuth* lpCSAuthPt = static_cast<PktCSAuth *>(lpPktBase);

	*dwCID			= lpCSAuthPt->m_dwCharID;
	*dwAuthCode		= lpCSAuthPt->m_dwAuthCode;
	*lpAuthCode2	= lpCSAuthPt->m_AuthCode2;

	return lpCSAuthPt->GetError();
}

unsigned long ParsePacket::HandleKeyInfo(PktBase* lpPktBase, unsigned long* dwCID, unsigned long* pKeyInfo)
{
	PktKeyInfo* lpKeyInfo = static_cast<PktKeyInfo*>(lpPktBase);

	*dwCID			= lpKeyInfo->m_dwCID;
	memcpy(pKeyInfo, lpKeyInfo->m_dwKeyInfo, sizeof(unsigned long)*PktKeyInfo::MAX_KEY_INFO);

	return lpKeyInfo->GetError();
}


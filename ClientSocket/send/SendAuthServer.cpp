#include "stdafx.h"
#include <winsock2.h>
#include <windows.h>

#include "SendPacket.h"
#include "SendAuthServer.h"

#include <DB/DBDefine.h>

#include <Network/ClientSocket/ClientSocket.h>
#include <Network/Packet/PacketCommand.h>
#include <Network/Packet/PacketStruct/ClientToAuthServer.h>
#include <Network/Packet/PacketStruct/UnifiedCharPacket.h>

#include <Network/ClientNetwork/NetworkMsgBlock.h>
#include <Network/ClientNetwork/ClientEventHandler.h>
#include "GMMemory.h"

unsigned long GetDiskSerial(char Drv_In)
{
    unsigned long VolumeSerial = 0;

    char Root[10] = "";
    sprintf(Root, "%c:\\", Drv_In);
    if(!GetVolumeInformation(Root, 0, 0, &VolumeSerial, 0, 0, 0, 0))
        return 0;

    return VolumeSerial;
}


bool SendPacket::AuthAccount(ClientSocket& clientSocket, char* UserID_In, char* UserPW_In, 
                             unsigned long ClientVer_In, unsigned long CheckSum_In, unsigned short Flag_In)
{
    clientSocket.ClearAddress(ClientSocket::MoveZoneAddr);

    ClientNet::CClientEventHandler* lpHandler = &clientSocket.GetHandler(ClientSocket::AuthEventHandler);
    
    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(
        sizeof(PktAU), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);

    PktAU* lpAUPt = reinterpret_cast<PktAU *>(lpMsgBlock->wr_ptr());

	strncpy(lpAUPt->m_UserAccount, UserID_In, PktAU::ID_LEN);
	lpAUPt->m_UserAccount[PktAU::ID_LEN-1] = 0;
	strncpy(lpAUPt->m_UserPassword, UserPW_In, PktAU::PASS_LEN);
	lpAUPt->m_UserPassword[PktAU::PASS_LEN-1] = 0;

    lpAUPt->m_dwSessionID	= GetDiskSerial('C');
    lpAUPt->m_dwClientVer	= ClientVer_In;
    lpAUPt->m_dwCheckSum	= CheckSum_In;

    lpAUPt->m_usFlag		= Flag_In;

	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktAU), CmdAuthAccount, 0, PktBase::NO_SERVER_ERR);    
}


bool SendPacket::JapanAuthAccount(ClientSocket& clientSocket, char* UserID_In, 
                                  unsigned long dwUID, unsigned long ClientVer_In, 
                                  unsigned long CheckSum_In, unsigned short Flag_In)
{
    clientSocket.ClearAddress(ClientSocket::MoveZoneAddr);

    ClientNet::CClientEventHandler* lpHandler = 
        &clientSocket.GetHandler(ClientSocket::AuthEventHandler);
    
    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(
        sizeof(PktJPAU), lpHandler->GetPeerAddress());
    CHECK_NULL_RETURN(lpMsgBlock, false);

    PktJPAU* lpJPAUPt = reinterpret_cast<PktJPAU *>(lpMsgBlock->wr_ptr());

    strncpy(lpJPAUPt->m_szUserAccount, UserID_In, 16);

    lpJPAUPt->m_dwUserID    = dwUID;
    lpJPAUPt->m_dwSessionID	= GetDiskSerial('C');
    lpJPAUPt->m_dwClientVer	= ClientVer_In;
    lpJPAUPt->m_dwCheckSum	= CheckSum_In;

    lpJPAUPt->m_usFlag		= Flag_In;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktJPAU), CmdJapanAuthAccount, 0, 0);    
}



bool SendPacket::UserLogin(ClientNet::CClientEventHandler* lpHandler, unsigned long UserID_In, unsigned char cLoginType_In)
{
    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(
        sizeof(PktULi), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);
	
    PktULi* lpULiPt = reinterpret_cast<PktULi *>(lpMsgBlock->wr_ptr());
    lpULiPt->m_dwUserID		= UserID_In;
	lpULiPt->m_cLoginType	= cLoginType_In;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktULi), CmdUserLogin, 0);    
}


bool SendPacket::CharCreate(ClientNet::CClientEventHandler* lpHandler, unsigned long UserID_In, 
                              unsigned long SlotNum_In, CHAR_CREATE &Create_In)
{
    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(
        sizeof(PktCC), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);
	
    PktCC* lpCCPt = reinterpret_cast<PktCC *>(lpMsgBlock->wr_ptr());

    lpCCPt->m_dwUserID		= UserID_In;
    lpCCPt->m_dwSlotNum	    = SlotNum_In;
    lpCCPt->m_CreateChar	= Create_In;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktCC), CmdCharCreate, 0, 0);    
}


bool SendPacket::CharSelect(ClientNet::CClientEventHandler* lpHandler, unsigned long UserID_In,unsigned long CharID_In)
{
    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(
        sizeof(PktCS), lpHandler->GetPeerAddress());
	CHECK_NULL_RETURN(lpMsgBlock, false);
	
    PktCS* lpCSPt = reinterpret_cast<PktCS *>(lpMsgBlock->wr_ptr());

    lpCSPt->m_dwUserID		= UserID_In;
    lpCSPt->m_dwCharID		= CharID_In;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktCS), CmdCharSelect, 0, 0);    
}



bool SendPacket::CharDelete(ClientNet::CClientEventHandler* lpHandler, unsigned long UserID_In,
                              unsigned long CharID_In, unsigned long SlotNum_In, char* szPassword)
{
    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(
        sizeof(PktCD), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);
	
    PktCD* lpCDPt = reinterpret_cast<PktCD *>(lpMsgBlock->wr_ptr());

    lpCDPt->m_dwUserID		= UserID_In;
    lpCDPt->m_dwCharID		= CharID_In;
    lpCDPt->m_dwSlotNum		= SlotNum_In;
	
	memset(lpCDPt->m_szPassword, 0, STORE_INFO::MAX_PASS_LEN);
	memcpy(lpCDPt->m_szPassword, szPassword, STORE_INFO::MAX_PASS_LEN);

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktCD), CmdCharDelete, 0, 0);
}

// WORK_LIST 2.1 계정 국적 추가
bool SendPacket::SelectAccountNation(ClientNet::CClientEventHandler* lpHandler, unsigned long UserID_In,
									 unsigned char cType, unsigned char cAccountNation_In)
{
	ClientNet::CNetworkMsgBlock* lpMsgBlock =
		ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(
        sizeof(PktSelectAccountNation), lpHandler->GetPeerAddress());

	CHECK_NULL_RETURN(lpMsgBlock, false);

	PktSelectAccountNation* lpSANPt = reinterpret_cast<PktSelectAccountNation*>(lpMsgBlock->wr_ptr());

	lpSANPt->m_dwUserID			= UserID_In;
	lpSANPt->m_cType			= cType;
	lpSANPt->m_cAccountNation	= cAccountNation_In;

	return SendPacket(lpHandler, lpMsgBlock, sizeof(PktSelectAccountNation), CmdSelectAccountNation, 0, 0);
}


bool SendPacket::UnifiedCharSelect(ClientNet::CClientEventHandler* lpHandler, const char* szPassword,                                    
                                   unsigned long* lpdwCID, unsigned long dwCIDNum,                                    
                                   unsigned char cSelectedStoreServerGroup,
                                   unsigned char cSelectedNation)
{
    ClientNet::CNetworkMsgBlock* lpMsgBlock = 
        ClientNet::CNetworkMsgPool::GetInstance().GetNetworkMsgBlock(
        sizeof(PktUnifiedCharSelectReq), lpHandler->GetPeerAddress());

    CHECK_NULL_RETURN(lpMsgBlock, false);

    memset(lpMsgBlock->wr_ptr(), 0, sizeof(PktUnifiedCharSelectReq));

    PktUnifiedCharSelectReq* lpPktUnifiedCharSelectReq = 
        reinterpret_cast<PktUnifiedCharSelectReq*>(lpMsgBlock->wr_ptr());

    strncpy(lpPktUnifiedCharSelectReq->szPassword, 
        szPassword, PktUnifiedCharSelectReq::MAX_PASSWORD_LEN);

    lpPktUnifiedCharSelectReq->szPassword[PktUnifiedCharSelectReq::MAX_PASSWORD_LEN - 1] = 0;

    memcpy(lpPktUnifiedCharSelectReq->dwCID, lpdwCID, 
        sizeof(unsigned long) * min(USER_INFO::MAX_CHAR_NUM, dwCIDNum));

    lpPktUnifiedCharSelectReq->cSelectedServerGroupID = cSelectedStoreServerGroup;
    lpPktUnifiedCharSelectReq->cSelectedNation = cSelectedNation;

    return SendPacket(lpHandler, lpMsgBlock, sizeof(PktUnifiedCharSelectReq), CmdUnifiedCharSelect, 0, 0);
}

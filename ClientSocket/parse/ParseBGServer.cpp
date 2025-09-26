#include "stdafx.h"
#include "ParseBGServer.h"

#include <windows.h>
#include <Network/Packet/PacketStruct/BGServerPacket.h>
#include "GMMemory.h"

// 방 리스트 정보, 혹은 방정보
unsigned long ParsePacket::HandleCharBGServerMapList(PktBase* lpPktBase, bool* bAll, unsigned char* cMapInfoNodeNum,
														BGServerMapInfoNode* lpMapInfo)
{
	LPPktBGServerMapList	lpPktBGServerMapList = (LPPktBGServerMapList)lpPktBase;
	
	*bAll = lpPktBGServerMapList->m_bAll;
	*cMapInfoNodeNum = lpPktBGServerMapList->m_cMapInfoNodeNum;

	BGServerMapInfoNode* lpMapInfoNode = reinterpret_cast<BGServerMapInfoNode*>(lpPktBGServerMapList + 1);
	CopyMemory(lpMapInfo, lpMapInfoNode, sizeof(BGServerMapInfoNode) * lpPktBGServerMapList->m_cMapInfoNodeNum);

	return lpPktBGServerMapList->GetError();
}


unsigned long ParsePacket::HandleCharBGServerResultList(PktBase* lpPktBase, bool* bAll, unsigned char* cResultInfoNodeNum,
														BGServerResultInfoNode* lpResultInfo)
{
	LPPktBGServerResultList	lpPktBGServerResultList = (LPPktBGServerResultList)lpPktBase;
	
	*bAll = lpPktBGServerResultList->m_bAll;
	*cResultInfoNodeNum = lpPktBGServerResultList->m_cResultInfoNodeNum;

	BGServerResultInfoNode*	lpResultInfoNode = reinterpret_cast<BGServerResultInfoNode*>(lpPktBGServerResultList + 1);
	CopyMemory(lpResultInfo, lpResultInfoNode, sizeof(BGServerResultInfoNode) * lpPktBGServerResultList->m_cResultInfoNodeNum);

	return lpPktBGServerResultList->GetError();
}


unsigned long ParsePacket::HandleCharBGServerMoveZone(PktBase* lpPktBase, unsigned char* cZone, unsigned char* cMoveType)
{
	LPPktBGServerMoveZone lpPktBGServerMoveZone = (LPPktBGServerMoveZone)lpPktBase;

	*cZone = lpPktBGServerMoveZone->m_cZone;
	*cMoveType = lpPktBGServerMoveZone->m_cMoveType;

	return lpPktBGServerMoveZone->GetError();
}


unsigned long ParsePacket::HandleCharBGServerMileageChange(PktBase* lpPktBase, unsigned long* dwCID, 
														   unsigned char* cGroup, unsigned char* cCmd, 
														   unsigned long* dwGold, unsigned long* dwMileage)
{
	PktBGServerMileageChange* lpPktBGServerMileageChange = static_cast<PktBGServerMileageChange *>(lpPktBase);

	*dwCID = lpPktBGServerMileageChange->m_dwCID;
	*cGroup = lpPktBGServerMileageChange->m_cGroup;
	*cCmd = lpPktBGServerMileageChange->m_cCmd;
	*dwGold = lpPktBGServerMileageChange->m_dwGold;
	*dwMileage = lpPktBGServerMileageChange->m_dwMileage;

	return lpPktBGServerMileageChange->GetError();
}

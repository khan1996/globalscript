#include "stdafx.h"
#include "ParseCastle.h"

#include <Network/Packet/PacketStruct/CastlePacket.h>
#include "GMMemory.h"


unsigned long ParsePacket::HandleCharCreateCastle(PktBase* lpPktBase, unsigned long* dwCastleID, unsigned char* cNation, unsigned char* cZone,
												  unsigned char* cNameID, unsigned char* cUpgradeStep, unsigned char* cInvincibleCount,
												  unsigned short* wTotalSiegeCount, unsigned long* dwTotalTaxMoney, float* fPosX,
												  float* fPosY, float* fPosZ, unsigned short* wItemID, unsigned char* cItemNum,
												  CastleRight* castleRight, CastleTaxInfo* castleTaxInfo)
{
	LPPktCreateCastle	lpPktCreateCastle = (LPPktCreateCastle)lpPktBase;

	*dwCastleID			= lpPktCreateCastle->m_dwCastleID;
	*cNation			= lpPktCreateCastle->m_cNation;
	*cZone				= lpPktCreateCastle->m_cZone;
	*cNameID			= lpPktCreateCastle->m_cNameID;
	*cUpgradeStep		= lpPktCreateCastle->m_cUpgradeStep;
	*cInvincibleCount	= lpPktCreateCastle->m_cInvincibleCount;
	*wTotalSiegeCount	= lpPktCreateCastle->m_wTotalSiegeCount;
	*dwTotalTaxMoney	= lpPktCreateCastle->m_dwTotalTaxMoney;
	*fPosX				= lpPktCreateCastle->m_fPosX;
	*fPosY				= lpPktCreateCastle->m_fPosY;
	*fPosZ				= lpPktCreateCastle->m_fPosZ;
	*wItemID			= lpPktCreateCastle->m_wItemID;
	*castleRight		= lpPktCreateCastle->m_CastleRight;

	for (int i=0; i<Castle::EMBLEM_UPGRADE_JEWEL_POS_NUM; ++i)
	{
		cItemNum[ i ] = lpPktCreateCastle->m_cItemNum[ i ] ;
	}

	for (int i=0; i<Castle::MAX_TAX_TYPE; ++i)
	{
		castleTaxInfo[ i ] = lpPktCreateCastle->m_CastleTax[ i ];
	}

	return lpPktCreateCastle->GetError();
}

unsigned long ParsePacket::HandleCharCreateCamp(PktBase* lpPktBase, unsigned long* dwActorCID, unsigned long* dwCID, unsigned long* dwCampID,
												unsigned long* dwGID, unsigned long* dwHP, unsigned short* wObjectType, unsigned char* cState,
												unsigned char* cSubState, unsigned char* cUpgradeStep, unsigned char* cNation, CampRight* campRight,
												float* fPointX, float* fPointY, float* fPointZ, unsigned char* cMaterial)
{
	LPPktCreateCamp	lpPktCreateCamp = (LPPktCreateCamp)lpPktBase;

	*dwActorCID			= lpPktCreateCamp->m_dwActorCID;
	*dwCID				= lpPktCreateCamp->m_dwCID;
	*dwCampID			= lpPktCreateCamp->m_dwCampID;
	*dwGID				= lpPktCreateCamp->m_dwGID;
	*dwHP				= lpPktCreateCamp->m_dwHP;
	*wObjectType		= lpPktCreateCamp->m_wObjectType;
	*cState				= lpPktCreateCamp->m_cState;
	*cSubState			= lpPktCreateCamp->m_cSubState;
	*cUpgradeStep		= lpPktCreateCamp->m_cUpgradeStep;
	*cNation			= lpPktCreateCamp->m_cNation;
	*campRight			= lpPktCreateCamp->m_CampRight;
	*fPointX			= lpPktCreateCamp->m_Pos.fPointX;
	*fPointY			= lpPktCreateCamp->m_Pos.fPointY;
	*fPointZ			= lpPktCreateCamp->m_Pos.fPointZ;
	*cMaterial			= lpPktCreateCamp->m_cMaterial;
	
	return lpPktCreateCamp->GetError();
}

unsigned long ParsePacket::HandleCharCreateSiegeArms(PktBase* lpPktBase, unsigned long* dwCID, unsigned long* dwOwnerID, unsigned char* cNation,
													 unsigned long* dwHP, unsigned short* wObjectType, unsigned char* cState, unsigned char* cUpgradeStep,
													 float* fPointX, float* fPointY, float* fPointZ)
{
	LPPktCreateSiegeArms lpPktCreateSiegeArms = (LPPktCreateSiegeArms)lpPktBase;

	*dwCID			= lpPktCreateSiegeArms->m_dwCID;
	*dwOwnerID		= lpPktCreateSiegeArms->m_dwOwnerID;
	*cNation		= lpPktCreateSiegeArms->m_cNation;
	*dwHP			= lpPktCreateSiegeArms->m_dwHP;
	*wObjectType	= lpPktCreateSiegeArms->m_wObjectType;
	*cState			= lpPktCreateSiegeArms->m_cState;
	*cUpgradeStep	= lpPktCreateSiegeArms->m_cUpgradeStep;
	*fPointX		= lpPktCreateSiegeArms->m_Pos.fPointX;
	*fPointY		= lpPktCreateSiegeArms->m_Pos.fPointY;
	*fPointZ		= lpPktCreateSiegeArms->m_Pos.fPointZ;

	return lpPktCreateSiegeArms->GetError();
}

unsigned long ParsePacket::HandleCharCastleCmd(PktBase* lpPktBase, unsigned long* dwCastleID, unsigned long* dwCastleObjectID, unsigned long* dwValue1,
											   unsigned long* dwValue2, unsigned char* cState, unsigned char* cSubCmd)
{
	LPPktCastleCmd lpPktCastleCmd = (LPPktCastleCmd)lpPktBase;

	*dwCastleID			= lpPktCastleCmd->m_dwCastleID;
	*dwCastleObjectID	= lpPktCastleCmd->m_dwCastleObjectID;
	*dwValue1			= lpPktCastleCmd->m_dwValue1;
	*dwValue2			= lpPktCastleCmd->m_dwValue2;
	*cState				= lpPktCastleCmd->m_cState;
	*cSubCmd			= lpPktCastleCmd->m_cSubCmd;

	return lpPktCastleCmd->GetError();
}

unsigned long ParsePacket::HandleCharCampCmd(PktBase* lpPktBase, unsigned long* dwCID, unsigned long* dwCampID,
											 unsigned char* cState, unsigned long* dwValue1, unsigned long* dwValue2,
											 unsigned char* cSubCmd)
{
	LPPktCampCmd	lpPktCampCmd = (LPPktCampCmd)lpPktBase;

	*dwCID			= lpPktCampCmd->m_dwCID;
	*dwCampID		= lpPktCampCmd->m_dwCampID;
	*cState			= lpPktCampCmd->m_cState;
	*dwValue1		= lpPktCampCmd->m_dwValue1;
	*dwValue2		= lpPktCampCmd->m_dwValue2;
	*cSubCmd		= lpPktCampCmd->m_cSubCmd;

	return lpPktCampCmd->GetError();
}

unsigned long ParsePacket::HandleCharSiegeArmsCmd(PktBase* lpPktBase, unsigned long* dwCID, unsigned long* dwArmsID,
												  unsigned long* dwValue, unsigned char* cState, unsigned char* cSubCmd)
{
	LPPktSiegeArmsCmd	lpPktSACmd = (LPPktSiegeArmsCmd)lpPktBase;

	*dwCID			= lpPktSACmd->m_dwCID;
	*dwArmsID		= lpPktSACmd->m_dwArmsID;
	*dwValue		= lpPktSACmd->m_dwValue;
	*cState			= lpPktSACmd->m_cState;
	*cSubCmd		= lpPktSACmd->m_cSubCmd;

	return lpPktSACmd->GetError();
}

unsigned long ParsePacket::HandleCharCastleRight(PktBase* lpPktBase, unsigned long* dwCastleID, CastleRight* castleRight)
{
	LPPktCastleRight	lpPktCR = (LPPktCastleRight)lpPktBase;

	*dwCastleID	= lpPktCR->m_dwCastleID;
	::memcpy(castleRight, &lpPktCR->m_CastleRight, sizeof(CastleRight));

	return lpPktCR->GetError();
}

unsigned long ParsePacket::HandleCharCampRight(PktBase* lpPktBase, unsigned long* dwCID, unsigned long* dwCampID, CampRight* campRight)
{
	LPPktCampRight	lpPktCR = (LPPktCampRight)lpPktBase;

	*dwCampID = lpPktCR->m_dwCampID;
	*dwCID    = lpPktCR->m_dwCID ;
	::memcpy(campRight, &lpPktCR->m_CampRight, sizeof(CampRight));

	return lpPktCR->GetError();
}

unsigned long ParsePacket::HandleCharSiegeBroadCast(PktBase* lpPktBase, unsigned long* dwCID, unsigned long* dwOwnerID,
													unsigned long* dwGID, unsigned short* wObjectType, unsigned long* dwNowHP,
													unsigned long* dwMaxHP,	unsigned char* cState, unsigned char* cSubState,
													unsigned char* cUpgradeStep, unsigned char* cUpgradeType, float* fDefaultDir,
													unsigned long* ppdwRidersID, CNetworkPos* netPos)
{
// CASTLE_TODO : 사용하지 않는 옛날 공성 오브젝트 브로드 캐스트 패킷 핸들러 함수

	LPPktSiegeBroadCast	lpPktSBC = (LPPktSiegeBroadCast)lpPktBase;

	*dwCID				= lpPktSBC->m_dwCID;
	*dwOwnerID			= lpPktSBC->m_dwOwnerID;
	*dwGID				= lpPktSBC->m_dwGID;
	*wObjectType		= lpPktSBC->m_wObjectType;
	*dwNowHP			= lpPktSBC->m_dwNowHP;
	*dwMaxHP			= lpPktSBC->m_dwMaxHP;
	*cState				= lpPktSBC->m_cState;
	*cSubState			= lpPktSBC->m_cSubState;
	*cUpgradeStep		= lpPktSBC->m_cUpgradeStep;
	*cUpgradeType		= lpPktSBC->m_cUpgradeType;
	*fDefaultDir		= lpPktSBC->m_fDefaultDir;
	
	for (int i=0; i<Siege::AIRSHIP_RIDER_NUM; ++i)
	{
		ppdwRidersID[i] = lpPktSBC->m_dwRiderID[i];
	}

	*netPos				= lpPktSBC->m_NetworkPos;

	return lpPktSBC->GetError();
}

unsigned long ParsePacket::HandleCharSiegeBroadCast2nd(PktBase* lpPktBase, unsigned long* dwObjectCount,
													   PktSiegeBroadCast* siegeObjectArray)
{
// CASTLE_TODO : 사용하지 않는 옛날 공성 오브젝트 브로드 캐스트 패킷 핸들러 함수

	LPPktSiegeBroadCast2nd	lpPktSBC = (LPPktSiegeBroadCast2nd)lpPktBase;
	int nDataLen = lpPktSBC->GetLen() - sizeof(PktSiegeBroadCast2nd);
	int nParseLen = 0;
	int nIndex = 0;
	
	if(0 < nDataLen)
	{
		PktSiegeBroadCast* szData = reinterpret_cast<PktSiegeBroadCast*>(lpPktSBC + 1);
		while (nParseLen < nDataLen)
		{
			siegeObjectArray[nIndex] = *szData;
			
			nParseLen += sizeof(PktSiegeBroadCast);
			++szData;
			++nIndex;
		}
	}

	*dwObjectCount = static_cast<unsigned long>(nIndex);
	
	return lpPktSBC->GetError();
}

unsigned long ParsePacket::HandleCharNewSiegeBroadCast(PktBase* lpPktBase, unsigned long* dwCID, unsigned char* cBroadCastType)
{
	LPPktNewSiegeBroadCast	lpPktNSBC = (LPPktNewSiegeBroadCast)lpPktBase;

	*dwCID			= lpPktNSBC->m_dwCID;
	*cBroadCastType	= lpPktNSBC->m_cType;

	return lpPktNSBC->GetError();
}

unsigned long ParsePacket::HandleCharGameTimeInfo(PktBase* lpPktBase, GuildTimeInfo* lpGuildTimeInfo, RealmTimeInfo* lpRealmTimeInfo,
												  SiegeTimeInfo* lpSiegeTimeInfo, EnterTimeInfo* lpEnterTimeInfo, EventTimeInfo* lpEventTimeInfo, unsigned char* cType, unsigned char* cCastleNum,
												  CastleSiegeInfo* lpCastleSiegeInfo)
{
	LPPktGameTimeInfo	lpPktGTI = (LPPktGameTimeInfo)lpPktBase;
	CastleSiegeInfo*	lpCastleInfo = (CastleSiegeInfo*)(lpPktGTI + 1);

	::memcpy(lpGuildTimeInfo, &lpPktGTI->m_GuildTimeInfo, sizeof(GuildTimeInfo));
	::memcpy(lpRealmTimeInfo, &lpPktGTI->m_RealmTimeInfo, sizeof(RealmTimeInfo));
	::memcpy(lpSiegeTimeInfo, &lpPktGTI->m_SiegeTimeInfo, sizeof(SiegeTimeInfo));
	::memcpy(lpEnterTimeInfo, &lpPktGTI->m_EnterTimeInfo, sizeof(EnterTimeInfo));
	::memcpy(lpEventTimeInfo, &lpPktGTI->m_EventTimeInfo, sizeof(EventTimeInfo));

	*cType		= lpPktGTI->m_cType;
	*cCastleNum	= lpPktGTI->m_cCastleNum;

	for (int i=0; i<lpPktGTI->m_cCastleNum; ++i)
	{
		::memcpy(&lpCastleSiegeInfo[i], lpCastleInfo, sizeof(CastleSiegeInfo));
		++lpCastleInfo;
	}

	return lpPktGTI->GetError();
}


unsigned long ParsePacket::HandleCharStealthInfo(PktBase* lpPktBase, bool* bUseStealth)
{
	LPPktStealthInfo	lpPktSI = (LPPktStealthInfo)lpPktBase;

	*bUseStealth		= lpPktSI->m_bUseStealth;

	return lpPktSI->GetError();
}


unsigned long ParsePacket::HandleCharCastleSiegeCount(PktBase* lpPktBase, unsigned long* dwCastleID, unsigned char* cSiegeCount,
										 unsigned char* cInvincibleCount, unsigned short* wTotalSiegeCount)
{
	LPPktCastleSiegeCount	lpPktCSC = (LPPktCastleSiegeCount)lpPktBase;

	*dwCastleID			= lpPktCSC->m_dwCastleID;
	*cSiegeCount		= lpPktCSC->m_cSiegeCount;
	*cInvincibleCount	= lpPktCSC->m_cInvincibleCount;
	*wTotalSiegeCount	= lpPktCSC->m_wTotalSiegeCount;

	return lpPktCSC->GetError();
}

unsigned long ParsePacket::HandleCharCastleTaxMove(PktBase* lpPktBase, unsigned long* dwCastleID,
												   unsigned long* dwMoveTempMoney, unsigned long* dwResultTaxMoney)
{
	LPPktCastleTaxMove	lpPktCTM = (LPPktCastleTaxMove)lpPktBase;

	*dwCastleID	= lpPktCTM->m_dwCastleID;

	for (int i=0; i<Castle::MAX_TAX_TYPE; ++i)
	{
		dwMoveTempMoney[ i ] = lpPktCTM->m_dwMoveTempMoney[ i ];
		dwResultTaxMoney[ i ] = lpPktCTM->m_dwResultTaxMoney[ i ];
	}

	return lpPktCTM->GetError();
}


unsigned long ParsePacket::HandleCharCampMessage(PktBase* lpPktBase, char* szGuildName, char* szMasterName, char* szPCName, unsigned char* cMsgCmd,
												 unsigned long* dwCampID, unsigned char* cZone, unsigned char* cRemainTime, unsigned char* cNation,
												 PktCampMessage::sRight* Right, bool* bNotify, int* nValue1, int* nValue2, unsigned char* cTimeChk)
{
	LPPktCampMessage	lpPktCM = (LPPktCampMessage)lpPktBase;

	::strcpy(szGuildName, lpPktCM->m_szGuildName);
	::strcpy(szMasterName, lpPktCM->m_szMasterName);
	::strcpy(szPCName, lpPktCM->m_szPCName);
	*cMsgCmd			= lpPktCM->m_cMsgCmd;
	*dwCampID			= lpPktCM->m_dwCampID;
	*cZone				= lpPktCM->m_cZone;
	*cRemainTime		= lpPktCM->m_cRemainTime;
	*cNation			= lpPktCM->m_cNation;
	*Right				= lpPktCM->m_Right;
	*bNotify			= lpPktCM->m_bNotify;
	*nValue1			= lpPktCM->m_nValue1;
	*nValue2			= lpPktCM->m_nValue2;
	*cTimeChk			= lpPktCM->m_cTimeChk;

	return lpPktCM->GetError();
}

// 채굴기 누적 광물 리스트 요청 결과 패킷
unsigned long ParsePacket::HandleCharMiningCampMineralInfo(PktBase* lpPktBase, unsigned long* dwCampID, unsigned char* cState, unsigned long* dwRemainTime,
														   unsigned char* cMineralType, unsigned char* cNum, CampMineralInfo* lpMineralList)
{
	LPPktMiningCampMineralInfo	lpPktMI = (LPPktMiningCampMineralInfo)lpPktBase;
	CampMineralInfo*	lpInfo = reinterpret_cast<CampMineralInfo*>(lpPktMI + 1);

	*dwCampID		= lpPktMI->m_dwCampID;
	*cState			= lpPktMI->m_cState;
	*dwRemainTime	= lpPktMI->m_dwRemainTime;
	*cMineralType	= lpPktMI->m_cMineralType;
	*cNum			= lpPktMI->m_cNum;

	if ( lpMineralList )
	{
		for (int i=0; i<*cNum; ++i, ++lpMineralList, ++lpInfo)
		{
			lpMineralList->m_wMineralID	= lpInfo->m_wMineralID;
			lpMineralList->m_wAmount = lpInfo->m_wAmount;
		}
	}

	return lpPktMI->GetError();
}


// 성 상징물 보석 아이템 넣기/빼기
unsigned long ParsePacket::HandleCharTakeCastleJewel(PktBase* lpPktBase, unsigned long* dwCID, unsigned long* dwCastleID, unsigned char* cSubCmd,
													 unsigned short* wItemID, unsigned char* cIndex, unsigned char* cNum, Item::ItemPos* pos)
{
	LPPktTakeCastleJewel	lpPktTCJ = (LPPktTakeCastleJewel)lpPktBase;

	*dwCID		= lpPktTCJ->m_dwCID;
	*dwCastleID	= lpPktTCJ->m_dwCastleID;
	*cSubCmd	= lpPktTCJ->m_cSubCmd;
	*wItemID	= lpPktTCJ->m_wItemID;
	*cIndex		= lpPktTCJ->m_cIndex;
	*cNum		= lpPktTCJ->m_cNum;
	*pos		= lpPktTCJ->m_Pos;

	return lpPktTCJ->GetError();
}

// 길드 요새 자재 넣기/빼기
unsigned long ParsePacket::HandleCharTakeMaterial(PktBase* lpPktBase, unsigned long* dwCID, unsigned long* dwCampID, unsigned char* cSubCmd,
												  unsigned char* cNum, Item::ItemPos* pos)
{
	LPPktTakeMaterial	lpPktTM = (LPPktTakeMaterial)lpPktBase;

	*dwCID		= lpPktTM->m_dwCID;
	*dwCampID	= lpPktTM->m_dwCampID;
	*cSubCmd	= lpPktTM->m_cSubCmd;
	*cNum		= lpPktTM->m_cNum;
	*pos		= lpPktTM->m_Pos;

	return lpPktTM->GetError();
}

// 성 누적 광물 세금 리스트 요청 결과 패킷
unsigned long ParsePacket::HandleCharCastleMineralInfo(PktBase* lpPktBase, unsigned long* dwCastleID, unsigned char* cNum, CastleMineral* lpMineralList)
{
	LPPktCastleMineralInfo lpPktCMI = (LPPktCastleMineralInfo)lpPktBase;
	CastleMineral*	lpInfo = reinterpret_cast<CastleMineral*>(lpPktCMI + 1);

	*dwCastleID	= lpPktCMI->m_dwCastleID;
	*cNum		= lpPktCMI->m_cNum;

	if ( lpMineralList )
	{
		for (int i=0; i<*cNum; ++i, ++lpMineralList, ++lpInfo)
		{
			lpMineralList->m_wMineralID	= lpInfo->m_wMineralID;
			lpMineralList->m_wAmount = lpInfo->m_wAmount;
		}
	}

	return lpPktCMI->GetError();
}

unsigned long ParsePacket::HandleCharWarOnOff(PktBase* lpPktBase, unsigned long* dwCID, unsigned char* cFlag, unsigned char* cType)
{
	
	PktWarOnOff* lpPktWarOnOff = (PktWarOnOff*)(lpPktBase);

	*dwCID	= lpPktWarOnOff->m_dwCID;
	*cFlag	= lpPktWarOnOff->m_cFlag;
	*cType	= lpPktWarOnOff->m_cType;

	return lpPktWarOnOff->GetError();
}

unsigned long ParsePacket::HandleCharRealmPoint(PktBase* lpPktBase, unsigned long* dwCID, unsigned char* cRealmPoint, unsigned char* cType)
{
	PktRealmPoint* lpPktRealmPoint = (PktRealmPoint*)(lpPktBase);

	*dwCID			= lpPktRealmPoint->m_dwCID;
	*cRealmPoint	= lpPktRealmPoint->m_cRealmPoint;
	*cType			= lpPktRealmPoint->m_cType;

	return lpPktRealmPoint->GetError();
}

unsigned long ParsePacket::HandleCharRealmStatueDisplayInfo(PktBase* lpPktBase, unsigned short* wSubCmd, unsigned long* dwRealmStatueNum,
															StatueDisplayInfo* lpDisplayInfo)
{
	LPPktStatueCmd	lpPktStatueCmd = (LPPktStatueCmd)lpPktBase;
	StatueDisplayInfo* lpInfo = reinterpret_cast<StatueDisplayInfo*>(lpPktStatueCmd + 1);

	*wSubCmd			= lpPktStatueCmd->m_wSubCmd;
	*dwRealmStatueNum	= lpPktStatueCmd->m_dwValue;

	if ( lpDisplayInfo )
	{
		for (unsigned long i=0; i<*dwRealmStatueNum; ++i, ++lpDisplayInfo, ++lpInfo)
		{
			*lpDisplayInfo = *lpInfo;
		}
	}

	return lpPktStatueCmd->GetError();
}

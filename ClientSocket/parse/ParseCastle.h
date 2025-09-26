#ifndef _GAMA_CLIENT_PARSE_CASTLE_PACKET_H_
#define _GAMA_CLIENT_PARSE_CASTLE_PACKET_H_

#include <Network/Packet/PacketStruct/CastlePacket.h>
#include <Network/Packet/PacketStruct/CharMovePacket.h>

// 전방 참조
struct PktBase;

namespace ParsePacket
{
	// 공성 오브젝트 생성 패킷
	unsigned long HandleCharCreateCastle(PktBase* lpPktBase, unsigned long* dwCastleID, unsigned char* cNation, unsigned char* cZone,
										 unsigned char* cNameID, unsigned char* cUpgradeStep, unsigned char* cInvincibleCount,
										 unsigned short* wTotalSiegeCount, unsigned long* dwTotalTaxMoney, float* fPosX,
										 float* fPosY, float* fPosZ, unsigned short* wItemID, unsigned char* cItemNum,
										 CastleRight* castleRight, CastleTaxInfo* castleTaxInfo);

	unsigned long HandleCharCreateCamp(PktBase* lpPktBase, unsigned long* dwActorCID, unsigned long* dwCID, unsigned long* dwCampID,
									   unsigned long* dwGID, unsigned long* dwHP, unsigned short* wObjectType, unsigned char* cState,
									   unsigned char* cSubState, unsigned char* cUpgradeStep, unsigned char* cNation, CampRight* campRight,
									   float* fPointX, float* fPointY, float* fPointZ, unsigned char* cMaterial);

	unsigned long HandleCharCreateSiegeArms(PktBase* lpPktBase, unsigned long* dwCID, unsigned long* dwOwnerID, unsigned char* cNation,
											unsigned long* dwHP, unsigned short* wObjectType, unsigned char* cState, unsigned char* cUpgradeStep,
											float* fPointX, float* fPointY, float* fPointZ);


	// 공성 명령 패킷
	unsigned long HandleCharCastleCmd(PktBase* lpPktBase, unsigned long* dwCastleID, unsigned long* dwCastleObjectID, unsigned long* dwValue1,
									  unsigned long* dwValue2, unsigned char* cState, unsigned char* cSubCmd);

	unsigned long HandleCharCampCmd(PktBase* lpPktBase, unsigned long* dwCID, unsigned long* dwCampID, unsigned char* cState,
									unsigned long* dwValue1, unsigned long* dwValue2, unsigned char* cSubCmd);

	unsigned long HandleCharSiegeArmsCmd(PktBase* lpPktBase, unsigned long* dwCID, unsigned long* dwArmsID,
										 unsigned long* dwValue, unsigned char* cState, unsigned char* cSubCmd);


	// 공성 권한 설정 패킷
	unsigned long HandleCharCastleRight(PktBase* lpPktBase, unsigned long* dwCastleID, CastleRight* castleRight);

	unsigned long HandleCharCampRight(PktBase* lpPktBase, unsigned long* dwCID, unsigned long* dwCampID, CampRight* campRight);


	// 공성 브로드 캐스트 패킷
	unsigned long HandleCharSiegeBroadCast(PktBase* lpPktBase, unsigned long* dwCID, unsigned long* dwOwnerID, unsigned long* dwGID,
										   unsigned short* wObjectType, unsigned long* dwNowHP, unsigned long* dwMaxHP,
										   unsigned char* cState, unsigned char* cSubState, unsigned char* cUpgradeStep,
										   unsigned char* cUpgradeType, float* fDefaultDir, unsigned long* ppdwRidersID, CNetworkPos* netPos);

	unsigned long HandleCharSiegeBroadCast2nd(PktBase* lpPktBase, unsigned long* dwObjectCount, PktSiegeBroadCast* siegeObjectArray);

	unsigned long HandleCharNewSiegeBroadCast(PktBase* lpPktBase, unsigned long* dwCID, unsigned char* cBroadCastType);

	// 게임 시간 패킷
	unsigned long HandleCharGameTimeInfo(PktBase* lpPktBase, GuildTimeInfo* lpGuildTimeInfo, RealmTimeInfo* lpRealmTimeInfo, SiegeTimeInfo* lpSiegeTimeInfo, EnterTimeInfo* lpEnterTimeInfo,
										EventTimeInfo* lpEventTimeInfo, unsigned char* cType, unsigned char* cCastleNum, CastleSiegeInfo* lpCastleSiegeInfo);

	// 공성 스텔스 사용가능 여부 패킷
	unsigned long HandleCharStealthInfo(PktBase* lpPktBase, bool* bUseStealth);

	// 공성후 공성 횟수 정보 업데이트
	unsigned long HandleCharCastleSiegeCount(PktBase* lpPktBase, unsigned long* dwCastleID, unsigned char* cSiegeCount,
											 unsigned char* cInvincibleCount, unsigned short* wTotalSiegeCount);

	// 공성 시간후 임시 세금 -> 누적 세금 이동 정보
	unsigned long HandleCharCastleTaxMove(PktBase* lpPktBase, unsigned long* dwCastleID, unsigned long* dwMoveTempMoney, unsigned long* dwResultTaxMoney);


	// 길드 요새 관련 메세지 패킷
	unsigned long HandleCharCampMessage(PktBase* lpPktBase, char* szGuildName, char* szMasterName, char* szPCName, unsigned char* cMsgCmd,
										unsigned long* dwCampID, unsigned char* cZone, unsigned char* cRemainTime, unsigned char* cNation,
										PktCampMessage::sRight* Right, bool* bNotify, int* nValue1, int* nValue2, unsigned char* cTimeChk);

	// 채굴기 누적 광물 리스트 요청 결과 패킷
	unsigned long HandleCharMiningCampMineralInfo(PktBase* lpPktBase, unsigned long* dwCampID, unsigned char* cState, unsigned long* dwRemainTime,
												   unsigned char* cMineralType, unsigned char* cNum, CampMineralInfo* lpMineralList);

	
	// 성 상징물 보석 아이템 넣기/빼기
	unsigned long HandleCharTakeCastleJewel(PktBase* lpPktBase, unsigned long* dwCID, unsigned long* dwCastleID, unsigned char* cSubCmd,
										unsigned short* wItemID, unsigned char* cIndex, unsigned char* cNum, Item::ItemPos* pos);
	
	// 길드 요새 자재 넣기/빼기
	unsigned long HandleCharTakeMaterial(PktBase* lpPktBase, unsigned long* dwCID, unsigned long* dwCampID, unsigned char* cSubCmd,
										unsigned char* cNum, Item::ItemPos* pos);

	// 성 누적 광물 세금 리스트 요청 결과 패킷
	unsigned long HandleCharCastleMineralInfo(PktBase* lpPktBase, unsigned long* dwCastleID, unsigned char* cNum, CastleMineral* lpMineralList);

	// 길드, 국가 전쟁 참여, 탈퇴.
	unsigned long HandleCharWarOnOff(PktBase* lpPktBase, unsigned long* dwCID, unsigned char* cFlag, unsigned char* cType);

	// 국가전쟁 공헌훈장 포인트.
	unsigned long HandleCharRealmPoint(PktBase* lpPktBase, unsigned long* dwCID, unsigned char* cRealmPoint, unsigned char* cType);

	// 국가전쟁 석상 표시 정보
	unsigned long HandleCharRealmStatueDisplayInfo(PktBase* lpPktBase, unsigned short* wSubCmd, unsigned long* dwRealmStatueNum,
												   StatueDisplayInfo* lpDisplayInfo);
};


#endif
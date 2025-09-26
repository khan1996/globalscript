#ifndef _PARSE_AUTH_SERVER_PACKET_H_
#define _PARSE_AUTH_SERVER_PACKET_H_

// 전방 참조
struct PktBase;

#include <ctime>
#include <DB/DBDefine.h>

namespace ParsePacket
{
    // AuthServer
    unsigned long HandleAuthAccount(PktBase* lpPktBase, unsigned long& UserID_Out, 
        unsigned char& cAgentServerType_Out, unsigned char& cFirstLogin_Out);

	// WORK_LIST 2.1 계정 국적 추가
    unsigned long HandleUserLogin(PktBase* lpPktBase, unsigned long *SessionID_Out, unsigned long *UserID_Out, CHAR_VIEW* CharRecode_Out,
		sGuildData* CharGuildRecode_Out, unsigned char* cAccountNation, time_t* currentTime, int* nPlayTime_Out,
		unsigned short* usAdminLevel_Out, unsigned char* cBillingType_Out, unsigned char* cBillingUser_Out, unsigned char* cLoginType_Out,
		unsigned long* dwTabFlag);

	unsigned long HandleCharSelect(PktBase* lpPktBase, unsigned char *Zone_Out, unsigned short *lpChannelNum_Out);
    unsigned long HandleCharCreate(PktBase* lpPktBase, unsigned long *CharID_Out, unsigned long *SlotNum_Out, CHAR_VIEW* lpCharView_Out);
    unsigned long HandleCharDelete(PktBase* lpPktBase, unsigned long *UserID_Out, unsigned long *SlotNum_Out);

    // WORK_LIST 2.1 계정 국적 추가
    unsigned long HandleSelectAccountNation(PktBase* lpPktBase, unsigned long *UserID_Out, unsigned char *cType, unsigned char *cAccountNation_Out);

    // WORK_LIST 2.3 계정 국적 변경 기능 구현
	unsigned long HandleNationChangeResult(PktBase* lpPktBase, unsigned long *UserID_Out, unsigned long* aryGID_Out, unsigned long* aryFame_Out);

    // 서버통합 캐릭터 정보
    unsigned long HandleUnifiedCharInfo(PktBase* lpPktBase, 
        unsigned char& cRestrictedPart1ToPart2Level, unsigned char& cRemainCharTransferCount,
        USER_INFO& userInfo,
        UnifiedStoreInfo** lppUnifiedStoreInfo, unsigned long& dwStoreInfoNum_Out,
        UnifiedCharData** lppUnifiedCharData, unsigned long& dwCharDataNum_Out);

    // 서버통합 캐릭터 선택 Ack - 성공 / 실패 코드값만 받는다.
    unsigned long HandleUnifiedCharSelectAck(PktBase* lpPktBase);
};


#endif
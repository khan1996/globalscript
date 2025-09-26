#ifndef _GAMA_CLIENT_PARSE_LOGIN_OUT_H_
#define _GAMA_CLIENT_PARSE_LOGIN_OUT_H_

#include <winsock2.h>
#include <windows.h>
#include <DB/DBDefine.h>

class ClientSocket;
struct PktBase;
struct _GG_AUTH_DATA;

namespace ParsePacket
{
    // 로그인 정보를 포함한다. ClientSocket에 게임서버 UDP주소와 채팅서버 주소를 세팅한다.
    unsigned long HandleCharLogin(PktBase* lpPktBase, ClientSocket& clientSocket,        
        bool *bPeace_Out, bool *bCheckRelation, unsigned char *cNameChangeCount, unsigned char *cAdminFlag,
		unsigned char *cGuildWarFlag, unsigned char *cRealmWarFlag, unsigned char *cRealmPoint, unsigned char *cTacticsFlag, 
		unsigned int *dwPlayTime, long *lPremiumTime, int *iPremiumType,
        CHAR_INFOST* lpCharInfo_Out, SKILL* lpSkill_Out, QUICK* lpQuick_Out, SPELL* lpSpell_Out,
		CHAR_POS* lpPos_Out, 
        unsigned long *ServerTime_Out, unsigned long *dwServerID,         
        unsigned short *EquipSize_Out, char **lppEquipBuff_Out, 
        unsigned short *InvenSize_Out, char **lppInvenBuff_Out, 
        unsigned short *ExtraSize_Out, char **lppExtraBuff_Out, 
        unsigned short *ExchangeSize_Out, char **lppExchangeBuff_Out, 
		unsigned short *TempInvenSize_Out, char **lppTempInvenBuff_Out);

    unsigned long HandleCharLogout(PktBase* lpPktBase, unsigned long *CharID_Out);
    unsigned long HandleCharMoveZone(PktBase* lpPktBase, unsigned char *Zone_Out, unsigned short *lpChannelNum_Out);

	// 내부에서 MoveZone주소를 세팅한다.
    unsigned long HandleServerZone(PktBase* lpPktBase, ClientSocket& clientSocket, unsigned long *ServerID_Out);

	unsigned long HandleCSAuth(PktBase* lpPktBase, unsigned long* dwCID, unsigned long* dwAuthCode, _GG_AUTH_DATA* lpAuthCode2);

	unsigned long HandleKeyInfo(PktBase* lpPktBase, unsigned long* dwCID, unsigned long* pKeyInfo);
};

#endif
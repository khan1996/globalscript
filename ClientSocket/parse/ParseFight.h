#ifndef _GAMA_CLIENT_PARSE_FIGHT_PACKET_H_
#define _GAMA_CLIENT_PARSE_FIGHT_PACKET_H_

#include <winsock2.h>
#include <windows.h>

#include <Network/Packet/PacketStruct/CharAttackPacket.h>

namespace ParsePacket
{
	// 전투
	unsigned long HandleCharAttack(PktBase* lpPktBase, unsigned long *CharID_Out, AtType* lpAtType_Out, unsigned short *HP_Out, unsigned short *MP_Out, unsigned short* MPHeal_Out, char *Judge_Out, unsigned char *DefenserNum_Out, DefenserNode** lppNode_Out);
	unsigned long HandleCharAttacked(PktBase* lpPktBase, unsigned long *AtCharID_Out, unsigned long *MyID_Out, AtType *AtType_Out, POS* lpAtPos_Out, float *AtDir_Out, unsigned short *Judge_Out, unsigned short *MyHP_Out, unsigned short *MyMP_Out, unsigned short* MyMPHeal_Out);

	// 장비내구도 감소
	unsigned long HandleCharEquipDurability(PktBase* lpPktBase, unsigned long *dwCharID, unsigned char *cIndex, unsigned char *cValue, unsigned char *cValueMax);
	
    unsigned long HandleCharRespawn(PktBase* lpPktBase, unsigned long *CharID_Out, unsigned short *HP_Out, unsigned short *MP_Out, POS* Pos_Out, __int64 *Exp_Out, unsigned long *dwGold_Out);
	unsigned long HandleCharRespawnInfo(PktBase* lpPktBase, unsigned long* CharID_Out, unsigned char* cReAreaNum, unsigned short* wSize, RespawnArea* pRespawnArea);
	unsigned long HandleCharRespawnAreaInfo(PktBase* lpPktBase, unsigned long* CharID_Out, unsigned long* dwTownID, unsigned short* wWaitOrder, unsigned char* cHour, unsigned char* cMin, unsigned char* cSec, bool* bCount);
    unsigned long HandleCharBattleGroundRespawn(PktBase* lpPktBase, unsigned long *CharID_Out, unsigned short *wTurn_Out, unsigned short *wWaitNum_Out, unsigned short *wLeftTime_Out, unsigned short *wHumanNum_Out, unsigned short *wAkhanNum_Out);
    unsigned long HandleCharRespawnWaitQueue(PktBase* lpPktBase, unsigned long *CharID_Out, unsigned short *wWaitNum_Out);
    unsigned long HandleCharDuelCmd(PktBase* lpPktBase, unsigned long *SenderID_Out, unsigned long *RecverID_Out, unsigned char *Cmd_Out);
    unsigned long HandleCharTeamBattleInfo(PktBase* lpPktBase, unsigned long *CharID_Out, char *ChallengerName_Out, unsigned char *Cmd_Out, unsigned char *MemberNum_Out, unsigned long *MemberID_Out, unsigned char *MemberLevel_Out);
    unsigned long HandleCharPeaceMode(PktBase* lpPktBase, unsigned long *dwCharID_Out, unsigned char *cLeftTime_Out, bool *bPeace_Out);
    unsigned long HandleCharAward(PktBase* lpPktBase, unsigned long *CharID_Out, unsigned long *Exp_Out);

	// 명예의 석상 관련
	unsigned long HandleStatueInfo(PktBase* lpPktBase, unsigned char *cIndex, unsigned short *wKID, unsigned char *cState, unsigned short *wNowHP, unsigned long *dwMileage, unsigned short *wHumanNum, unsigned short *wAkhanNum);
	unsigned long HandleCameraScript(PktBase* lpPktBase, unsigned long *dwDeleteObject, unsigned long *dwNewObject, POS* NewPos);

    // TODO : REMOVE UDP Packets
    unsigned long HandleCharAttackInfo(PktBase* lpPktBase, unsigned long *CharID_Out, AtType* lpAtType_Out, unsigned char *DefenserNum_Out, DefenserNode **lppNode_Out);
};

#endif
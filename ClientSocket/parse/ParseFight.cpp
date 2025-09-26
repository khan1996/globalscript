#include "stdafx.h"
#include "ParseFight.h"
#include "GMMemory.h"


unsigned long ParsePacket::HandleCharAttack(PktBase* lpPktBase, unsigned long *CharID_Out, AtType* lpAtType_Out, unsigned short *HP_Out, unsigned short *MP_Out,
											unsigned short* MPHeal_Out,	char *Judge_Out, unsigned char *DefenserNum_Out, DefenserNode** lppNode_Out)
{
	PktAtAck* lpAtAckPt = static_cast<PktAtAck*>(lpPktBase);

	*CharID_Out			= lpAtAckPt->m_dwCharID;
	*lpAtType_Out		= lpAtAckPt->m_AtType;
	*HP_Out				= lpAtAckPt->m_wHP;
	*MP_Out				= lpAtAckPt->m_wMP;
	*MPHeal_Out			= lpAtAckPt->m_wMPHeal;
	*Judge_Out			= lpAtAckPt->m_cJudge;
	*DefenserNum_Out	= lpAtAckPt->m_cDefenserNum;

	*lppNode_Out		= reinterpret_cast<DefenserNode*>(lpAtAckPt + 1);

	return lpAtAckPt->GetError();
}


unsigned long ParsePacket::HandleCharAttacked(PktBase* lpPktBase, unsigned long *AtCharID_Out, unsigned long *MyID_Out, 
											  AtType *AtType_Out, POS* lpAtPos_Out, float *AtDir_Out, unsigned short *Judge_Out, 
											  unsigned short *MyHP_Out, unsigned short *MyMP_Out, unsigned short* MyMPHeal_Out)
{
	PktAted* lpAtedPt = static_cast<PktAted*>(lpPktBase);

	*AtCharID_Out		= lpAtedPt->m_dwCharID;
	*MyID_Out			= lpAtedPt->m_dwMyID;

	*AtType_Out			= lpAtedPt->m_AtType;
	*lpAtPos_Out		= lpAtedPt->m_Postion;
	*AtDir_Out			= lpAtedPt->m_fDir;

	*Judge_Out			= lpAtedPt->m_cJudge;

	*MyHP_Out			= lpAtedPt->m_wMyHP;
	*MyMP_Out			= lpAtedPt->m_wMyMP;
	*MyMPHeal_Out		= lpAtedPt->m_wMyMPHeal;

	return lpAtedPt->GetError();
}


unsigned long ParsePacket::HandleCharAttackInfo(PktBase* lpPktBase, unsigned long *CharID_Out, 
                                                AtType* lpAtType_Out, unsigned char *DefenserNum_Out, 
                                                DefenserNode **lppNode_Out)
{
    PktAtInfo* lpPktAtInfo = static_cast<PktAtInfo*>(lpPktBase);

    *CharID_Out			= lpPktAtInfo->m_dwCharID;
    *lpAtType_Out		= lpPktAtInfo->m_AtType;
    *DefenserNum_Out	= lpPktAtInfo->m_cDefenserNum;
    *lppNode_Out		= reinterpret_cast<DefenserNode*>(lpPktAtInfo + 1);

    return lpPktAtInfo->GetError();
}


unsigned long ParsePacket::HandleCharEquipDurability(PktBase* lpPktBase, unsigned long *dwCharID, 
													 unsigned char *cIndex, unsigned char *cValue, unsigned char *cValueMax)
{
	PktEquipDurability* lpEDPt = static_cast<PktEquipDurability*>(lpPktBase);

	*dwCharID	= lpEDPt->m_dwCharID;
	*cIndex		= lpEDPt->m_cIndex;
	*cValue		= lpEDPt->m_cValue;
	*cValueMax	= lpEDPt->m_cValueMax;


	return lpEDPt->GetError();
}


unsigned long ParsePacket::HandleCharRespawn(PktBase* lpPktBase, unsigned long *CharID_Out, 
                                             unsigned short *HP_Out, unsigned short *MP_Out, 
                                             POS* lpPos_Out, __int64 *Exp_Out, 
                                             unsigned long *dwGold_Out)
{
	PktRsAck* lpPktRsAck = static_cast<PktRsAck*>(lpPktBase);

	*CharID_Out		= lpPktRsAck->m_dwCharID;
	*HP_Out			= lpPktRsAck->m_wHP;
	*MP_Out			= lpPktRsAck->m_wMP;
	*lpPos_Out		= lpPktRsAck->m_Position;
	*Exp_Out		= lpPktRsAck->m_dlExp;
	*dwGold_Out		= lpPktRsAck->m_dwGold;

	return lpPktRsAck->GetError();
}

unsigned long ParsePacket::HandleCharRespawnInfo(PktBase* lpPktBase, unsigned long* CharID_Out, unsigned char* cRsAreaNum, unsigned short* wSize, RespawnArea* pRespawnArea)
{
	PktRsInfoAck* lpPktRsInfoAck = static_cast<PktRsInfoAck*>(lpPktBase);

	*CharID_Out		= lpPktRsInfoAck->m_dwCharID;
	*cRsAreaNum		= lpPktRsInfoAck->m_cRsAreaNum;
	*wSize			= lpPktRsInfoAck->m_wSize;

	RespawnArea* lpRespawnAreaNode = reinterpret_cast<RespawnArea *>(lpPktRsInfoAck + 1);

	for (int i=0; i<lpPktRsInfoAck->m_cRsAreaNum; ++i)
	{
		CopyMemory(&pRespawnArea[i], lpRespawnAreaNode, sizeof(RespawnArea));
		++lpRespawnAreaNode;
	}

	return lpPktRsInfoAck->GetError();
}

unsigned long ParsePacket::HandleCharRespawnAreaInfo(PktBase* lpPktBase, unsigned long* CharID_Out, unsigned long* dwTownID,
													 unsigned short* wWaitOrder, unsigned char* cHour, unsigned char* cMin,
													 unsigned char* cSec, bool* bCount)
{
	PktRsAreaInfoAck* lpPktRsAreaInfoAck = static_cast<PktRsAreaInfoAck*>(lpPktBase);

	*CharID_Out	= lpPktRsAreaInfoAck->m_dwCharID;
	*dwTownID	= lpPktRsAreaInfoAck->m_dwTownID;
	*wWaitOrder	= lpPktRsAreaInfoAck->m_nWaitOrder;		// 대기순위
	*cHour		= lpPktRsAreaInfoAck->m_cRemainHour;	// 남은 시간 (시)
	*cMin		= lpPktRsAreaInfoAck->m_cRemainMin;		// 남은 시간 (분)
	*cSec		= lpPktRsAreaInfoAck->m_cRemainSec;		// 남은 시간 (초)
	*bCount		= lpPktRsAreaInfoAck->m_bCount;			// 시간 카운팅 시작여부

	return lpPktRsAreaInfoAck->GetError();
}

unsigned long ParsePacket::HandleCharBattleGroundRespawn(PktBase* lpPktBase, unsigned long *CharID_Out, 
                                                         unsigned short *wTurn_Out, unsigned short *wWaitNum_Out, 
                                                         unsigned short *wLeftTime_Out, unsigned short *wHumanNum_Out, 
                                                         unsigned short *wAkhanNum_Out)
{
	PktBGRsAck* lpPktBGRAck = static_cast<PktBGRsAck*>(lpPktBase);
	
	*CharID_Out		= lpPktBGRAck->m_dwCharID;
	*wTurn_Out		= lpPktBGRAck->m_wTurn;
	*wWaitNum_Out	= lpPktBGRAck->m_wWaitNum;
	*wLeftTime_Out	= lpPktBGRAck->m_wLeftTime;
	*wHumanNum_Out	= lpPktBGRAck->m_wHumanNum;
	*wAkhanNum_Out	= lpPktBGRAck->m_wAkhanNum;

	return lpPktBGRAck->GetError();
}


unsigned long ParsePacket::HandleCharRespawnWaitQueue(PktBase* lpPktBase, unsigned long *CharID_Out, 
                                                      unsigned short *wWaitNum_Out)
{
	PktRsWaitQueueAck* lpRsWQAckPt = static_cast<PktRsWaitQueueAck*>(lpPktBase);
	
	*CharID_Out	= lpRsWQAckPt->m_dwCharID;
	::memcpy(wWaitNum_Out, lpRsWQAckPt->m_wWaitNum, 
        PktRsWaitQueueAck::MAX_POINT_NUM_PER_RACE);

	return lpRsWQAckPt->GetError();
}



unsigned long ParsePacket::HandleCharDuelCmd(PktBase* lpPktBase, unsigned long *SenderID_Out, 
                                             unsigned long *RecverID_Out, unsigned char *Cmd_Out)
{
    PktDuC* lpDuCPt = static_cast<PktDuC*>(lpPktBase);

    *SenderID_Out		= lpDuCPt->m_dwSenderID;
    *RecverID_Out		= lpDuCPt->m_dwRecverID;
    *Cmd_Out			= lpDuCPt->m_cCmd;

    return lpDuCPt->GetError();
}


unsigned long ParsePacket::HandleCharTeamBattleInfo(PktBase* lpPktBase, unsigned long *CharID_Out, 
                                                    char *ChallengerName_Out, unsigned char *Cmd_Out, 
                                                    unsigned char *MemberNum_Out, unsigned long *MemberID_Out, 
                                                    unsigned char *MemberLevel_Out)
{
	PktTBInfo* lpPktTBInfo = static_cast<PktTBInfo*>(lpPktBase);

	*CharID_Out = lpPktTBInfo->m_dwCharID;
    *Cmd_Out    = lpPktTBInfo->m_cCmd;
    *MemberNum_Out = lpPktTBInfo->m_cMemberNum;

	::strncpy(ChallengerName_Out, lpPktTBInfo->m_strChallengerName, CHAR_INFOST::MAX_NAME_LEN);

	for (int nIndex = 0; nIndex < *MemberNum_Out; nIndex++)
	{
		MemberID_Out[nIndex] = *(reinterpret_cast<unsigned long *>(
            reinterpret_cast<char*>(lpPktTBInfo + 1) + 
			nIndex * (sizeof(unsigned long) + sizeof(unsigned char))));

		MemberLevel_Out[nIndex] = *(reinterpret_cast<unsigned char *>(
            reinterpret_cast<char*>(lpPktTBInfo + 1) + 
			nIndex * (sizeof(unsigned long) + sizeof(unsigned char)) + sizeof(unsigned long)));
	}

	return lpPktTBInfo->GetError();
}


unsigned long ParsePacket::HandleCharPeaceMode(PktBase* lpPktBase, unsigned long *dwCharID_Out, 
                                               unsigned char *cLeftTime_Out, bool *bPeace_Out)
{
    PktPeace* lpPeacePtAck = static_cast<PktPeace*>(lpPktBase);

	*dwCharID_Out	= lpPeacePtAck->m_dwCharID;
	*cLeftTime_Out	= lpPeacePtAck->m_cLeftTime;
	*bPeace_Out		= lpPeacePtAck->m_bPeace;

	return lpPeacePtAck->GetError();
}


unsigned long ParsePacket::HandleCharAward(PktBase* lpPktBase, unsigned long *CharID_Out, unsigned long *Exp_Out)
{
	PktAw* lpAwPt = static_cast<PktAw*>(lpPktBase);

	*CharID_Out			= lpAwPt->m_dwCharID;
	*Exp_Out			= lpAwPt->m_dwExp;

	return lpAwPt->GetError();
}


unsigned long ParsePacket::HandleStatueInfo(PktBase* lpPktBase, unsigned char *cIndex, unsigned short *wKID, unsigned char *cState, unsigned short *wNowHP, 
											unsigned long *dwMileage, unsigned short *wHumanNum, unsigned short *wAkhanNum)
{
	PktStatueInfo* lpPktSI = static_cast<PktStatueInfo*>(lpPktBase);

	*dwMileage = lpPktSI->m_dwMileage;

	*wHumanNum = lpPktSI->m_wHumanNum;
	*wAkhanNum = lpPktSI->m_wAkhanNum;

	*wNowHP = lpPktSI->m_wNowHP;
	*wKID = lpPktSI->m_wKID;
	*cState = lpPktSI->m_cState;
	*cIndex = lpPktSI->m_cIndex;

	return lpPktSI->GetError();
}


unsigned long ParsePacket::HandleCameraScript(PktBase* lpPktBase, unsigned long *dwDeleteObject, unsigned long *dwNewObject, POS* NewPos)
{
	PktCameraScript* lpPktCS = static_cast<PktCameraScript*>(lpPktBase);

	*dwDeleteObject = lpPktCS->m_dwDeleteObject;
	*dwNewObject = lpPktCS->m_dwNewObject;
	*NewPos = lpPktCS->m_NewPos;

	return lpPktCS->GetError();
}
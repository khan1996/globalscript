#include "stdafx.h"
#include "ParseMove.h"

#include <Network/ClientSocket/ClientSocket.h>
#include <Network/Packet/PacketStruct/CharStatusPacket.h>
#include <Network/Packet/PacketStruct/CharBroadcastPacket.h>

#include <mmsystem.h>
#include "GMMemory.h"


unsigned long ParsePacket::HandleCharUpdateAddress(PktBase* lpPktBase, ClientSocket& clientSocket)
{
    PktUAAck* lpUAAckPt = static_cast<PktUAAck*>(lpPktBase);

    clientSocket.SetAddress(ClientSocket::PrivateAddr, lpUAAckPt->m_PrivateAddress);
    clientSocket.SetAddress(ClientSocket::PublicAddr, lpUAAckPt->m_PublicAddress);

    return lpUAAckPt->GetError();
}


unsigned long ParsePacket::HandleCharAddressInfo(PktBase* lpPktBase, unsigned long *CharID_Out, 
                                                 SOCKADDR_IN* PubAddress_Out, SOCKADDR_IN* PriAddress_Out)
{
    PktAI* lpAIPt = static_cast<PktAI*>(lpPktBase);

    *CharID_Out			= lpAIPt->m_AddressInfo.m_dwCharID;
    *PubAddress_Out		= lpAIPt->m_AddressInfo.m_PublicAddress;
    *PriAddress_Out		= lpAIPt->m_AddressInfo.m_PrivateAddress;

    return lpAIPt->GetError();
}

unsigned long ParsePacket::HandleCharMoveEx(PktBase* lpPktBase, unsigned long *CharID_Out, POS* Pos_Out, 
                                            float *Dir_Out, unsigned char *UAct_Out, unsigned char *LAct_Out)
{
    PktMVEx* lpPktMVEx = static_cast<PktMVEx*>(lpPktBase);

	CNetworkPos& netPos = lpPktMVEx->m_NetworkPos;
    
    Pos_Out->fPointX    = netPos.GetXPos();
    Pos_Out->fPointY    = netPos.GetYPos();
    Pos_Out->fPointZ    = netPos.GetZPos();

    *Dir_Out = netPos.GetDirection();

    *CharID_Out = lpPktMVEx->GetServerInfo();
    *UAct_Out = lpPktMVEx->m_cUAct;
    *LAct_Out = lpPktMVEx->m_cLAct;

    return 0;
}

unsigned long ParsePacket::HandleMonMove(PktBase* lpPktBase, unsigned long *MonID_Out, POS* lpPos_Out, 
                                         float *Dir_Out, float *Vec_Out, 
                                         unsigned short *Act_Out, unsigned short *AniNum_Out)
{
	PktMM* lpMMPt = static_cast<PktMM*>(lpPktBase);

	*MonID_Out		    = lpMMPt->m_dwMonID;
    CNetworkPos& netPos = lpMMPt->m_NetworkPos;

	lpPos_Out->fPointX	= netPos.GetXPos();
    lpPos_Out->fPointY  = netPos.GetYPos();
    lpPos_Out->fPointZ  = netPos.GetZPos();
    
	*Dir_Out		= netPos.GetDirection();    
	*Vec_Out		= netPos.GetVelocity();

	*Act_Out		= lpMMPt->m_cAct;
	*AniNum_Out		= lpMMPt->m_cAniNum;

	return lpMMPt->GetError();
}


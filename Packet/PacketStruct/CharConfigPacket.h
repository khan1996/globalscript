#ifndef _CHAR_CONFIG_PACKET_H_
#define _CHAR_CONFIG_PACKET_H_

// CharConfigPacket.h
#include <Network/Packet/PacketBase.h>
#include <Network/Packet/PacketStruct/DataPacket.h>

#include "CharConfigPacketStruct.h"

#pragma pack(1)


typedef struct PktConfigInfo* LPPktConfigInfo;
struct PktConfigInfo : public PktDD
{
	enum { MAX_BUFFER_SIZE = 50 };

	unsigned long   m_dwUserID;
	unsigned long   m_dwCharID;

	unsigned short	m_wSize;
};

// 옵션 조정
typedef struct PktCOp* LPPktCOp;
struct PktCOp : public PktBase 
{
	unsigned long	m_dwCharID;         // 캐릭터 아이디
	RejectOption	m_RejectOption;     // 거부 옵션 비트셑
};

// 정보 변화
typedef struct PktChatInfoChange* LPPktChatInfoChange;  
struct PktChatInfoChange : public PktBase
{
	unsigned long   m_dwUID;
	unsigned long   m_dwCID;

	unsigned long   m_dwPID;
	unsigned long   m_dwGID;
	RejectOption    m_RejectOption;
	unsigned char   m_bPeaceMode;
};


#pragma pack()

#endif
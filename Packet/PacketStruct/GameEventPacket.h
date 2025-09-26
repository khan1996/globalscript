
#ifndef _GAME_EVENT_PACKET_H_
#define _GAME_EVENT_PACKET_H_

#include <Network/Packet/PacketBase.h>


#pragma pack(1)

// ---------------------------------------------------------------------------
// 게임 이벤트 관련

typedef struct PktEventDropItem* LPPktEventDropItem;
struct PktEventDropItem : public PktBase
{
	unsigned short m_usItemID;
};

struct PktItemQtyControl : public PktBase
{
    enum TYPE
    { 
        ADD             = 0,
        DEL             = 1,
        STATUS_REQUEST  = 2, 
        STATUS_ACK      = 3,
        STATUS_ACK_END  = 4
    };

    unsigned long	m_dwItemTypeID;
    unsigned long	m_dwItemQty;
    unsigned long	m_dwCurrentItemQty;
    time_t			m_tStartTime;
    time_t			m_tEndTime;

    unsigned char	m_cType;
    unsigned char   m_cGroup;
};

#pragma pack()


#endif
#ifndef _CHAR_STATUS_PACKET_STRUCT_H_
#define _CHAR_STATUS_PACKET_STRUCT_H_

#pragma pack(1)

// 캐릭터 능력치
typedef struct ChState* LPChState;
struct ChState
{
	unsigned short	m_wIP;   
	unsigned short	m_wSTR;
	unsigned short	m_wDEX;
	unsigned short	m_wCON;
	unsigned short	m_wINT;
	unsigned short	m_wWIS;
};

#pragma pack()

#endif
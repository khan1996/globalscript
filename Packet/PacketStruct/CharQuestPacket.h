#ifndef _CHAR_QUEST_PACKET_H_
#define _CHAR_QUEST_PACKET_H_

// CharQuestPacket.h
#include <Network/Packet/PacketBase.h>
#include <Network/Packet/PacketStruct/DataPacket.h>

#pragma pack(1)

// 퀘스트 시작
typedef struct PktStartQuest* LPPktStartQuest;
struct PktStartQuest : public PktBase 
{
	enum PktStartQuestError 
	{
		FAIL_NOT_NPC					= 2,
		FAIL_NOT_HAVE_QUEST				= 3,
		FAIL_FULL_QUEST					= 4,
		FAIL_HAS_QUEST					= 5,
		FAIL_TOO_FAR					= 6,
		NOT_EXIST_QUEST					= 7,
		FAIL_TOO_FAR_PARTY				= 8,
		FAIL_MIN_LEVEL					= 9,
		FAIL_MAX_LEVEL					= 10,
		FAIL_WRONG_NATION				= 11,
		FAIL_WRONG_CLASS				= 12,
		FAIL_NOT_COMPLETE_PRIOR_QUEST	= 13,
		FAIL_MIN_FAME					= 14,
		FAIL_MAX_FAME					= 15
	};

	unsigned long	m_dwCharID;
	unsigned long	m_dwNPCID;
	unsigned short	m_wQuestID;
};

// 트리거 발동
typedef struct PktOperateTrigger* LPPktOperateTrigger;
struct PktOperateTrigger : public PktBase
{
	enum PktOperateTriggerError
	{
		FAIL_WRONG_QUEST		= 2,
		FAIL_WRONG_PHASE		= 3,
		FAIL_WRONG_TRIGGER		= 4,
		FAIL_EVENT_ERROR		= 5,
		FAIL_EVENT_GET_ERROR	= 6
	};

	unsigned long	m_dwCharID;
	unsigned short	m_wQuestID;
	unsigned char	m_cPhase;
	unsigned char	m_cTriggerKind;
	unsigned char	m_cTrigger;
	unsigned char	m_cCount;
};

// 퀘스트 정보 (DB에 세이브/로드, 캐릭터 로긴시)
typedef struct PktQuestDB* LPPktQuestDB;
struct PktQuestDB : public PktDD
{
	enum Const
	{ 
		MAX_HISTORY_QUEST	= 200, 
		MAX_EXECUTING_QUEST	= 20,
		MAX_TRIGGER			= 10 
	};

	struct ExecutingQuest
	{
		unsigned short	m_wQuestID;
		unsigned char 	m_cPhase;
		unsigned char 	m_cTriggerCount[MAX_TRIGGER];
	};

    unsigned long  m_dwUID;
    unsigned long  m_dwCID; 
	unsigned short m_wExecuteQuestSize;	
	unsigned short m_wHistoryQuestSize;	
};

// 퀘스트 종료
typedef struct PktEndQuest* LPPktEndQuest;
struct PktEndQuest : public PktBase
{
	unsigned long m_dwCharID;
	unsigned short m_wQuestID;
	bool m_bSave;
};

typedef struct PktCancelQuest* LPPktCancelQuest;
struct PktCancelQuest : public PktBase
{
	enum PktOperateTriggerError
	{
		FAIL_NOT_HAVE_QUEST	= 2,
		FAIL_DISABLE_CANCEL = 3
	};

	unsigned long m_dwCharID;
	unsigned short m_wQuestID;
};

#pragma pack()

#endif
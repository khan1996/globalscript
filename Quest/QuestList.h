// NPCList.h: interface for the CNPCList class.
//
//////////////////////////////////////////////////////////////////////
#ifndef __QUEST_LIST_H__
#define __QUEST_LIST_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <vector>
#include <map>

#define TRIGGER_START 0
#define TRIGGER_PUTON 1
#define TRIGGER_GETON 2
#define TRIGGER_TALK 3
#define TRIGGER_KILL 4
#define TRIGGER_PICK 5
#define TRIGGER_FAME 6
#define TRIGGER_LEVEL_TALK 7
#define TRIGGER_FAME_TALK 8
#define TRIGGER_ABILITY_TALK 9


#define EVENT_DISAPPEAR 0
#define EVENT_GET 1
#define EVENT_SPAWN 2
#define EVENT_MONSTERDROP 3
#define EVENT_END 4
#define EVENT_AWARD 5
#define EVENT_MSGBOX 6
#define EVENT_PHASE 7
#define EVENT_ADDQUEST 8
#define EVENT_AWARDITEM 9
#define EVENT_AWARDITEMID 10
#define EVENT_AWARDITEMCLASS 11
#define EVENT_MOVE 12
#define EVENT_THEATERMODE 13
#define EVENT_ALARMBOX 14
#define EVENT_AWARDABILITY 15

#define NPC_QUEST	0					// NPC 가 주는 퀘스트
#define ITEM_QUEST	1					// 아이템 사용을 통한 퀘스트
#define AREA_QUEST	2					// 위치 트리거 퀘스트

#define PARTY_QUEST_BIT	0xF000			// 파티 퀘스트 비트

using namespace std;

typedef struct EventNode
{
	unsigned long m_dwEventKind;
	unsigned long m_dwEventNumber;
	unsigned long m_dwEventAmount;
	unsigned long m_dwEventAmount2;
	float m_fPosX;
	float m_fPosY;
	float m_fPosZ;
	char *m_strWord;
} EventNode, *LPEventNode;

typedef struct TriggerNode
{
	unsigned long m_dwTriggerKind;
	unsigned long m_dwTriggerID;
	unsigned long m_dwZoneID;
	unsigned long m_dwMaxCount;
	unsigned long m_dwLevel;
	float m_fPosX;
	float m_fPosY;
	float m_fPosZ;
	float m_fDistance;
	std::string	m_strElseMsg;	// LevelTalk 에서 Level이 맞지 않을때 출력될 메세지

	vector<LPEventNode> m_lstEvent;
	vector<LPEventNode> m_lstFalseEvent;

	vector<char *> m_lstMsgBox;
	vector<char *> m_lstFalseMsgBox;
} TriggerNode, *LPTriggerNode;

typedef struct TargetPos
{
	int			m_iType;
	unsigned long m_dwPosX;
	unsigned long m_dwPosY;
} TargetPos;

typedef struct PhaseNode
{
	unsigned long m_dwZoneID;
	unsigned long m_dwPhaseNumber;
	char *m_strPhaseDesc;
	vector<LPTriggerNode> m_lstTrigger;
	vector<TargetPos> m_lstTargetPos;

	LPTriggerNode GetTrigger(unsigned long dwTriggerKind, unsigned long dwStartPos, unsigned long *dwTriggerPos)
	{
		vector<LPTriggerNode>::iterator it;

		*dwTriggerPos = 0;
		dwStartPos++;
		for(it = m_lstTrigger.begin(); it != m_lstTrigger.end(); it++, *dwTriggerPos++)
		{
			if(dwStartPos <= *dwTriggerPos)
			{
				if((*it)->m_dwTriggerKind == dwTriggerKind) return (*it);
			}
		}

		return NULL;
	}
} PhaseNode, *LPPhaseNode;

typedef struct QuestNode
{
	enum eCheckResult
	{
		RES_SUCCESS						= 0,	// 성공
		RES_MISMATCH_CLASS				= 1,	// 클래스가 맞지 않음
		RES_ALREADY_TAKEN				= 2,	// 이미 받은 퀘스트
		RES_NOT_COMPLETE_PRIOR_QUEST	= 3,	// 선결 퀘스트를 완료하지 않았음
		RES_HIGH_THAN_MAX_LEVEL			= 4,	// 수행할 수 있는 최고 레벨보다 높다.
		RES_10_LOW_THAN_MIN_LEVEL		= 5,	// 수행할 수 있는 최저 레벨보다 10 레벨 낮다.
		RES_LOW_LEVEL					= 6,	// 수행할 수 있는 레벨보다 조금 낮다.
		RES_MISMATCH_NATION				= 7,	// 수행할 수 없는 국적입니다.
		RES_LOW_FAME					= 8,	// 수행할 수 있는 명성보다 조금 낮다.
		RES_HIGH_THAN_MAX_FAME			= 9,	// 수행할 수 있는 최고 명성 보다 높다.
	};

    enum eCheckNation
    {
        NATION_ALL                      = 0,    // 모든 국가( 카르테란트 + 메르카디아 + 신의해적단 )
        NATION_KARTERANT                = 1,    // 카르테란트
        NATION_MERKADIA                 = 2,    // 메르카디아
        NATION_ALMIGHTY_PIRATE          = 3,    // 신의해적단

		// 사용 예
		// 12 : 카르테란트 + 메르카디아
		// 13 : 카르테란트 + 신의해적단
		// 23 : 메르카디아 + 신의해적단
		// 123 : 카르테란트 + 메르카디아 + 신의해적단 = 0 (모든 국가)
    };

	unsigned short	m_wQuestID;
	unsigned short	m_wQuestType;
	unsigned short	m_wMaxPhase;
	unsigned short	m_wMinLevel;
	unsigned short	m_wMaxLevel;
	unsigned long	m_dwMinFame;
	unsigned long	m_dwMaxFame;
	unsigned long	m_dwClass;
    unsigned short	m_usNation;                 // 퀘스트 수행 가능 국가
	BOOL			m_bNationDependent;			// 퀘스트가 국가 종속성이라서 국적이 바뀌면 지워지는가?
    unsigned short	m_usBonusSkillPoint;        // 퀘스트 수행 후 받는 보너스 스킬 포인트
    unsigned short	m_usBonusAbilityPoint;        // 퀘스트 수행 후 받는 보너스 스킬 포인트

	unsigned long m_dwCompletedQuest;
	char *m_strQuestTitle;
	char *m_strQuestDesc;
	char *m_strQuestShortDesc;
	char *m_strQuestLevel;
	char *m_strQuestAward;
	char m_strIconTexture[32];
	unsigned short m_wPosX1, m_wPosY1;
	unsigned short m_wPosX2, m_wPosY2;
	BOOL m_bSave;
	BOOL m_bInvisible;			// pop up 메뉴에서 안보이는지 여부
	float m_fPosX;				// 위치 퀘스트의 영역 중심 위치
	float m_fPosY;
	float m_fPosZ;
	float m_fDistance;			// 중심 위치에서의 거리
	unsigned long	m_dwZoneID;	// 위치 퀘스트를 받는 존
	bool			m_bDelLinkItem;			// 퀘스트 취소 시 관련 아이템 삭제 여부

	vector<LPPhaseNode> m_lstPhase;

	map<unsigned short, unsigned char>	m_mapEventGetItem;	// 퀘스트 시작시 TRIGGER_START 부분에 있는 EventGet item map

	LPPhaseNode GetPhase(unsigned long dwPhase)
	{
		vector<LPPhaseNode>::iterator it;
		for(it = m_lstPhase.begin(); it != m_lstPhase.end(); it++)
		{
			if((*it)->m_dwPhaseNumber == dwPhase) return (*it);
		}

		return NULL;
	}

	unsigned short	GetQuestType()	{ return (m_wQuestType & ~PARTY_QUEST_BIT); }
	bool			IsPartyQuest()	{ return (0 != (m_wQuestType & PARTY_QUEST_BIT));	}

	unsigned long	CheckQuest(unsigned long dwLevel, unsigned long dwFame, unsigned long dwClass, unsigned long dwNation, unsigned short *lstCompleted, unsigned short wNumCompleted, unsigned short *lstQuestList);
} QuestNode, *LPQuestNode;

typedef struct QuestInstanceNode
{
	unsigned short m_wQuestID;
	LPQuestNode m_lpQuest;
} QuestInstanceNode, *LPQuestInstanceNode;


class CQuestList
{
public:
	LPQuestNode m_lpQuestNode;
	LPPhaseNode m_lpPhaseNode;
	LPTriggerNode m_lpTriggerNode;
	vector<LPQuestNode> m_lstQuestNode;

	CQuestList();
	~CQuestList();

	BOOL		Load(const char *strQuestScriptFile);
	LPQuestNode	GetQuest(unsigned short wQuestID);

protected:
	VOID		DestroyQuestList();
};

extern CQuestList g_QuestList;

#endif // __QUEST_LIST_H__

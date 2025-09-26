// NPCList.cpp: implementation of the CNPCList class.
//
//////////////////////////////////////////////////////////////////////

#include <ScriptEngine/ScriptEngine.h>
#include "QuestList.h"
#include "GMMemory.h"

CQuestList g_QuestList;

static BOOL bFalseEvent;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static void ScriptErrorMessage(const char *msg)
{
	MessageBox(NULL, msg, "Script Error", MB_OK);
}


static void QuestStartLevel(int nQuestID, int nMinLevel, int nMaxLevel, int nClass, int nCompleteClass, int nNation, BOOL bNationDependent)
{
	g_QuestList.m_lpQuestNode = new QuestNode;
	g_QuestList.m_lpQuestNode->m_wQuestID = (unsigned short)nQuestID;
	g_QuestList.m_lpQuestNode->m_wMinLevel = (unsigned short)nMinLevel;
	g_QuestList.m_lpQuestNode->m_wMaxLevel = (unsigned short)nMaxLevel;
	g_QuestList.m_lpQuestNode->m_dwMinFame = (unsigned long)0;	
	g_QuestList.m_lpQuestNode->m_dwMaxFame = (unsigned long)0;
	g_QuestList.m_lpQuestNode->m_dwClass = nClass;
	g_QuestList.m_lpQuestNode->m_usNation = (unsigned short)nNation;
	g_QuestList.m_lpQuestNode->m_bNationDependent = bNationDependent;
	g_QuestList.m_lpQuestNode->m_usBonusSkillPoint = 0;
	g_QuestList.m_lpQuestNode->m_usBonusAbilityPoint = 0;
	
	if( 0xFFFF == nCompleteClass )
	{
		g_QuestList.m_lpQuestNode->m_dwCompletedQuest = 0;
		g_QuestList.m_lpQuestNode->m_bInvisible = TRUE;
	}
	else
	{
		g_QuestList.m_lpQuestNode->m_dwCompletedQuest = nCompleteClass;
		g_QuestList.m_lpQuestNode->m_bInvisible = FALSE;
	}
	g_QuestList.m_lpQuestNode->m_wQuestType = 0;
	g_QuestList.m_lpQuestNode->m_fPosX = 0.0f;
	g_QuestList.m_lpQuestNode->m_fPosY = 0.0f;
	g_QuestList.m_lpQuestNode->m_fPosZ = 0.0f;
	g_QuestList.m_lpQuestNode->m_strQuestTitle = NULL;
	g_QuestList.m_lpQuestNode->m_strQuestDesc = NULL;
	g_QuestList.m_lpQuestNode->m_strQuestLevel = NULL;
	g_QuestList.m_lpQuestNode->m_strQuestAward = NULL;
	strcpy(g_QuestList.m_lpQuestNode->m_strIconTexture, "Quest_misc01.dds");
	g_QuestList.m_lpQuestNode->m_wPosX1 = 42;	g_QuestList.m_lpQuestNode->m_wPosY1 = 114;
	g_QuestList.m_lpQuestNode->m_wPosX2 = 84;	g_QuestList.m_lpQuestNode->m_wPosY2 = 156;
	g_QuestList.m_lpQuestNode->m_bSave = FALSE;
	g_QuestList.m_lpQuestNode->m_mapEventGetItem.clear();
	g_QuestList.m_lpQuestNode->m_bDelLinkItem = TRUE;

	g_QuestList.m_lstQuestNode.push_back(g_QuestList.m_lpQuestNode);
}

static void QuestStartFame(int nQuestID, int nMinFame, int nMaxFame, int nClass, int nCompleteClass, int nNation, BOOL bNationDependent)
{
	g_QuestList.m_lpQuestNode = new QuestNode;
	g_QuestList.m_lpQuestNode->m_wQuestID = (unsigned short)nQuestID;
	g_QuestList.m_lpQuestNode->m_wMinLevel = (unsigned short)0;
	g_QuestList.m_lpQuestNode->m_wMaxLevel = (unsigned short)0;
	g_QuestList.m_lpQuestNode->m_dwMinFame = (unsigned long)nMinFame;	
	g_QuestList.m_lpQuestNode->m_dwMaxFame = (unsigned long)nMaxFame;
	g_QuestList.m_lpQuestNode->m_dwClass = nClass;
	g_QuestList.m_lpQuestNode->m_usNation = (unsigned short)nNation;
	g_QuestList.m_lpQuestNode->m_bNationDependent = bNationDependent;
	g_QuestList.m_lpQuestNode->m_usBonusSkillPoint = 0;
	g_QuestList.m_lpQuestNode->m_usBonusAbilityPoint = 0;
	
	if( 0xFFFF == nCompleteClass )
	{
		g_QuestList.m_lpQuestNode->m_dwCompletedQuest = 0;
		g_QuestList.m_lpQuestNode->m_bInvisible = TRUE;
	}
	else
	{
		g_QuestList.m_lpQuestNode->m_dwCompletedQuest = nCompleteClass;
		g_QuestList.m_lpQuestNode->m_bInvisible = FALSE;
	}
	g_QuestList.m_lpQuestNode->m_wQuestType = 0;
	g_QuestList.m_lpQuestNode->m_fPosX = 0.0f;
	g_QuestList.m_lpQuestNode->m_fPosY = 0.0f;
	g_QuestList.m_lpQuestNode->m_fPosZ = 0.0f;
	g_QuestList.m_lpQuestNode->m_strQuestTitle = NULL;
	g_QuestList.m_lpQuestNode->m_strQuestDesc = NULL;
	g_QuestList.m_lpQuestNode->m_strQuestLevel = NULL;
	g_QuestList.m_lpQuestNode->m_strQuestAward = NULL;
	strcpy(g_QuestList.m_lpQuestNode->m_strIconTexture, "Quest_misc01.dds");
	g_QuestList.m_lpQuestNode->m_wPosX1 = 42;	g_QuestList.m_lpQuestNode->m_wPosY1 = 114;
	g_QuestList.m_lpQuestNode->m_wPosX2 = 84;	g_QuestList.m_lpQuestNode->m_wPosY2 = 156;
	g_QuestList.m_lpQuestNode->m_bSave = FALSE;
	g_QuestList.m_lpQuestNode->m_mapEventGetItem.clear();
	g_QuestList.m_lpQuestNode->m_bDelLinkItem = TRUE;

	g_QuestList.m_lstQuestNode.push_back(g_QuestList.m_lpQuestNode);
}

//static void QuestStart(int nQuestID, int nMinLevel, int nMaxLevel, int nClass, int nNation, int nCompleteClass)
//{
//	g_QuestList.m_lpQuestNode = new QuestNode;
//	g_QuestList.m_lpQuestNode->m_wQuestID   = (unsigned short)nQuestID;
//	g_QuestList.m_lpQuestNode->m_wMinLevel  = (unsigned short)nMinLevel;
//	g_QuestList.m_lpQuestNode->m_wMaxLevel  = (unsigned short)nMaxLevel;
//	g_QuestList.m_lpQuestNode->m_dwClass    = nClass;
//    g_QuestList.m_lpQuestNode->m_usNation   = static_cast<unsigned short>( nNation );
//	if( 0xFFFF == nCompleteClass )
//	{
//		g_QuestList.m_lpQuestNode->m_dwCompletedQuest = 0;
//		g_QuestList.m_lpQuestNode->m_bInvisible = TRUE;
//	}
//	else
//	{
//		g_QuestList.m_lpQuestNode->m_dwCompletedQuest = nCompleteClass;
//		g_QuestList.m_lpQuestNode->m_bInvisible = FALSE;
//	}
//	g_QuestList.m_lpQuestNode->m_wQuestType = 0;
//	g_QuestList.m_lpQuestNode->m_fPosX = 0.0f;
//	g_QuestList.m_lpQuestNode->m_fPosY = 0.0f;
//	g_QuestList.m_lpQuestNode->m_fPosZ = 0.0f;
//	g_QuestList.m_lpQuestNode->m_strQuestTitle = NULL;
//	g_QuestList.m_lpQuestNode->m_strQuestDesc = NULL;
//	g_QuestList.m_lpQuestNode->m_strQuestLevel = NULL;
//	g_QuestList.m_lpQuestNode->m_strQuestAward = NULL;
//	strcpy(g_QuestList.m_lpQuestNode->m_strIconTexture, "Quest_misc01.dds");
//	g_QuestList.m_lpQuestNode->m_wPosX1 = 42;	g_QuestList.m_lpQuestNode->m_wPosY1 = 114;
//	g_QuestList.m_lpQuestNode->m_wPosX2 = 84;	g_QuestList.m_lpQuestNode->m_wPosY2 = 156;
//	g_QuestList.m_lpQuestNode->m_bSave = FALSE;
//
//	g_QuestList.m_lstQuestNode.push_back(g_QuestList.m_lpQuestNode);
//}

static void QuestEnd()
{
	g_QuestList.m_lpQuestNode->m_wMaxPhase = g_QuestList.m_lpQuestNode->m_lstPhase.size();
	g_QuestList.m_lpQuestNode = NULL;
	g_QuestList.m_lpPhaseNode = NULL;
	g_QuestList.m_lpTriggerNode = NULL;
}

static void QuestType(unsigned short wQuestType, unsigned short wPartyQuest)
{
	if (0 != wPartyQuest)
	{
		wQuestType |= PARTY_QUEST_BIT;
	}

	g_QuestList.m_lpQuestNode->m_wQuestType = wQuestType;
}

static void QuestArea(int nZone, float fX, float fY, float fZ, float fDist)
{
	g_QuestList.m_lpQuestNode->m_fPosX = fX * 100.0f;
	g_QuestList.m_lpQuestNode->m_fPosY = fY * 100.0f;
	g_QuestList.m_lpQuestNode->m_fPosZ = fZ * 100.0f;
	g_QuestList.m_lpQuestNode->m_fDistance = fDist * 100.0f;
	g_QuestList.m_lpQuestNode->m_dwZoneID = nZone;
}

static void QuestTitle(const char *strQuestTitle)
{
	if (g_QuestList.m_lpQuestNode)
	{
		char *strScript = new char[strlen(strQuestTitle) + 1];
		strcpy(strScript, strQuestTitle);
		g_QuestList.m_lpQuestNode->m_strQuestTitle = strScript;
	}
}

static void QuestDesc(const char *strQuestDesc)
{
	if (g_QuestList.m_lpQuestNode)
	{
		char *strScript = new char[strlen(strQuestDesc) + 1];
		strcpy(strScript, strQuestDesc);
		g_QuestList.m_lpQuestNode->m_strQuestDesc = strScript;
	}
}

static void QuestShortDesc(const char *strQuestDesc)
{
	if (g_QuestList.m_lpQuestNode)
	{
		char *strScript = new char[strlen(strQuestDesc) + 1];
		strcpy(strScript, strQuestDesc);
		g_QuestList.m_lpQuestNode->m_strQuestShortDesc = strScript;
	}
}

static void QuestIcon(const char *strIconTexture, int nPosX1, int nPosY1, int nPosX2, int nPosY2)
{
	if (g_QuestList.m_lpQuestNode)
	{
		strcpy(g_QuestList.m_lpQuestNode->m_strIconTexture, strIconTexture);
		g_QuestList.m_lpQuestNode->m_wPosX1 = nPosX1;
		g_QuestList.m_lpQuestNode->m_wPosY1 = nPosY1;
		g_QuestList.m_lpQuestNode->m_wPosX2 = nPosX2;
		g_QuestList.m_lpQuestNode->m_wPosY2 = nPosY2;
	}
}

static void QuestCompleteSave(BOOL bSave)
{
	if (g_QuestList.m_lpQuestNode)
	{
		if (bSave)
			g_QuestList.m_lpQuestNode->m_bSave = TRUE;
		else
			g_QuestList.m_lpQuestNode->m_bSave = FALSE;
	}
}

static void QuestLevel(const char *strQuestLevel)
{
	if (g_QuestList.m_lpQuestNode)
	{
		char *strScript = new char[strlen(strQuestLevel) + 1];
		strcpy(strScript, strQuestLevel);
		g_QuestList.m_lpQuestNode->m_strQuestLevel = strScript;
	}
}

static void QuestAward(const char *strQuestAward)
{
	if (g_QuestList.m_lpQuestNode)
	{
		char *strScript = new char[strlen(strQuestAward) + 1];
		strcpy(strScript, strQuestAward);
		g_QuestList.m_lpQuestNode->m_strQuestAward = strScript;
	}
}

static void QuestSkillPointBonus(int nBonusNum)
{
    if( g_QuestList.m_lpQuestNode )
    {   
        g_QuestList.m_lpQuestNode->m_usBonusSkillPoint  = (unsigned short)nBonusNum;
    }
}

static void QuestAbilityPoint(int nBonusNum)
{
    if( g_QuestList.m_lpQuestNode )
    {   
        g_QuestList.m_lpQuestNode->m_usBonusAbilityPoint  = (unsigned short)nBonusNum;
    }
}

static void QuestCancelItemDel(bool bDelLinkItem)
{
	if( g_QuestList.m_lpQuestNode )
	{
		g_QuestList.m_lpQuestNode->m_bDelLinkItem = bDelLinkItem;
	}
}

static void AddPhase(int nZoneID, int nPhaseNumber, const char *strQuestDesc)
{
	if (g_QuestList.m_lpQuestNode)
	{
		g_QuestList.m_lpPhaseNode = new PhaseNode;
		g_QuestList.m_lpPhaseNode->m_dwZoneID = nZoneID;
		g_QuestList.m_lpPhaseNode->m_dwPhaseNumber = nPhaseNumber;
		char *strScript = new char[strlen(strQuestDesc) + 1];
		strcpy(strScript, strQuestDesc);
		g_QuestList.m_lpPhaseNode->m_strPhaseDesc = strScript;

		g_QuestList.m_lpQuestNode->m_lstPhase.push_back(g_QuestList.m_lpPhaseNode);

		g_QuestList.m_lpTriggerNode = NULL;
	}
}

static void Phase_Target(int nType, int nPosX, int nPosY)
{
	if (g_QuestList.m_lpPhaseNode)
	{
		TargetPos tpAdd;
		tpAdd.m_iType = nType;
		tpAdd.m_dwPosX = (unsigned long)nPosX;
		tpAdd.m_dwPosY = (unsigned long)nPosY;
		g_QuestList.m_lpPhaseNode->m_lstTargetPos.push_back(tpAdd);
	}
}

static void Trigger_Start(void)
{
	if (g_QuestList.m_lpPhaseNode)
	{
		g_QuestList.m_lpTriggerNode = new TriggerNode;
		g_QuestList.m_lpTriggerNode->m_dwTriggerKind = TRIGGER_START;
		g_QuestList.m_lpTriggerNode->m_dwTriggerID = 0;
		g_QuestList.m_lpTriggerNode->m_dwZoneID = 0;
		g_QuestList.m_lpTriggerNode->m_dwMaxCount = 1;
		g_QuestList.m_lpTriggerNode->m_dwLevel = 0;
		g_QuestList.m_lpTriggerNode->m_fDistance = 0.0f;
		g_QuestList.m_lpTriggerNode->m_fPosX = 0.0f;
		g_QuestList.m_lpTriggerNode->m_fPosY = 0.0f;
		g_QuestList.m_lpTriggerNode->m_fPosZ = 0.0f;
		bFalseEvent = FALSE;

		g_QuestList.m_lpPhaseNode->m_lstTrigger.push_back(g_QuestList.m_lpTriggerNode);
	}
}

static void Trigger_Puton(int nItemID, int nZoneID, float fPosX, float fPosY, float fPosZ, float fDistance)
{
	if (g_QuestList.m_lpPhaseNode)
	{
		g_QuestList.m_lpTriggerNode = new TriggerNode;
		g_QuestList.m_lpTriggerNode->m_dwTriggerKind = TRIGGER_PUTON;
		g_QuestList.m_lpTriggerNode->m_dwTriggerID = nItemID;
		g_QuestList.m_lpTriggerNode->m_dwZoneID = nZoneID;
		g_QuestList.m_lpTriggerNode->m_dwMaxCount = 1;
		g_QuestList.m_lpTriggerNode->m_dwLevel = 0;
		g_QuestList.m_lpTriggerNode->m_fDistance = fDistance * 100.0f;
		g_QuestList.m_lpTriggerNode->m_fPosX = fPosX * 100.0f;
		g_QuestList.m_lpTriggerNode->m_fPosY = fPosY * 100.0f;
		g_QuestList.m_lpTriggerNode->m_fPosZ = fPosZ * 100.0f;
		bFalseEvent = FALSE;

		g_QuestList.m_lpPhaseNode->m_lstTrigger.push_back(g_QuestList.m_lpTriggerNode);
	}
}

static void Trigger_Geton(int nItemID, int nZoneID, float fPosX, float fPosY, float fPosZ, float fDistance)
{
	if (g_QuestList.m_lpPhaseNode)
	{
		g_QuestList.m_lpTriggerNode = new TriggerNode;
		g_QuestList.m_lpTriggerNode->m_dwTriggerKind = TRIGGER_GETON;
		g_QuestList.m_lpTriggerNode->m_dwTriggerID = nItemID;
		g_QuestList.m_lpTriggerNode->m_dwZoneID = nZoneID;
		g_QuestList.m_lpTriggerNode->m_dwMaxCount = 1;
		g_QuestList.m_lpTriggerNode->m_dwLevel = 0;
		g_QuestList.m_lpTriggerNode->m_fDistance = fDistance * 100.0f;
		g_QuestList.m_lpTriggerNode->m_fPosX = fPosX * 100.0f;
		g_QuestList.m_lpTriggerNode->m_fPosY = fPosY * 100.0f;
		g_QuestList.m_lpTriggerNode->m_fPosZ = fPosZ * 100.0f;
		bFalseEvent = FALSE;

		g_QuestList.m_lpPhaseNode->m_lstTrigger.push_back(g_QuestList.m_lpTriggerNode);
	}
}

static void Trigger_Talk(int nNpcID)
{
	if (g_QuestList.m_lpPhaseNode)
	{
		g_QuestList.m_lpTriggerNode = new TriggerNode;
		g_QuestList.m_lpTriggerNode->m_dwTriggerKind = TRIGGER_TALK;
		g_QuestList.m_lpTriggerNode->m_dwTriggerID = nNpcID;
		g_QuestList.m_lpTriggerNode->m_dwZoneID = 0;
		g_QuestList.m_lpTriggerNode->m_dwMaxCount = 1;
		g_QuestList.m_lpTriggerNode->m_dwLevel = 0;
		g_QuestList.m_lpTriggerNode->m_fDistance = 0.0f;
		g_QuestList.m_lpTriggerNode->m_fPosX = 0.0f;
		g_QuestList.m_lpTriggerNode->m_fPosY = 0.0f;
		g_QuestList.m_lpTriggerNode->m_fPosZ = 0.0f;
		bFalseEvent = FALSE;

		g_QuestList.m_lpPhaseNode->m_lstTrigger.push_back(g_QuestList.m_lpTriggerNode);
	}
}

static void Trigger_Kill(int nCount, int nMonsterID)
{
	if (g_QuestList.m_lpPhaseNode)
	{
		g_QuestList.m_lpTriggerNode = new TriggerNode;
		g_QuestList.m_lpTriggerNode->m_dwTriggerKind = TRIGGER_KILL;
		g_QuestList.m_lpTriggerNode->m_dwTriggerID = nMonsterID;
		g_QuestList.m_lpTriggerNode->m_dwZoneID = 0;
		g_QuestList.m_lpTriggerNode->m_dwMaxCount = nCount;
		g_QuestList.m_lpTriggerNode->m_dwLevel = 0;
		g_QuestList.m_lpTriggerNode->m_fDistance = 0.0f;
		g_QuestList.m_lpTriggerNode->m_fPosX = 0.0f;
		g_QuestList.m_lpTriggerNode->m_fPosY = 0.0f;
		g_QuestList.m_lpTriggerNode->m_fPosZ = 0.0f;
		bFalseEvent = FALSE;

		g_QuestList.m_lpPhaseNode->m_lstTrigger.push_back(g_QuestList.m_lpTriggerNode);
	}
}

static void Trigger_Pick(int nCount, int nItemID)
{
	if (g_QuestList.m_lpPhaseNode)
	{
		g_QuestList.m_lpTriggerNode = new TriggerNode;
		g_QuestList.m_lpTriggerNode->m_dwTriggerKind = TRIGGER_PICK;
		g_QuestList.m_lpTriggerNode->m_dwTriggerID = nItemID;
		g_QuestList.m_lpTriggerNode->m_dwZoneID = 0;
		g_QuestList.m_lpTriggerNode->m_dwMaxCount = nCount;
		g_QuestList.m_lpTriggerNode->m_dwLevel = 0;
		g_QuestList.m_lpTriggerNode->m_fDistance = 0.0f;
		g_QuestList.m_lpTriggerNode->m_fPosX = 0.0f;
		g_QuestList.m_lpTriggerNode->m_fPosY = 0.0f;
		g_QuestList.m_lpTriggerNode->m_fPosZ = 0.0f;
		bFalseEvent = FALSE;

		g_QuestList.m_lpPhaseNode->m_lstTrigger.push_back(g_QuestList.m_lpTriggerNode);
	}
}

static void Trigger_Fame(int nFame)
{
	if (g_QuestList.m_lpPhaseNode)
	{
		g_QuestList.m_lpTriggerNode = new TriggerNode;
		g_QuestList.m_lpTriggerNode->m_dwTriggerKind = TRIGGER_FAME;
		g_QuestList.m_lpTriggerNode->m_dwTriggerID = nFame;
		g_QuestList.m_lpTriggerNode->m_dwZoneID = 0;
		g_QuestList.m_lpTriggerNode->m_dwMaxCount = 1;
		g_QuestList.m_lpTriggerNode->m_dwLevel = 0;
		g_QuestList.m_lpTriggerNode->m_fDistance = 0.0f;
		g_QuestList.m_lpTriggerNode->m_fPosX = 0.0f;
		g_QuestList.m_lpTriggerNode->m_fPosY = 0.0f;
		g_QuestList.m_lpTriggerNode->m_fPosZ = 0.0f;
		bFalseEvent = FALSE;

		g_QuestList.m_lpPhaseNode->m_lstTrigger.push_back(g_QuestList.m_lpTriggerNode);
	}
}

static void Trigger_AbilityTalk(int nAbilityID, int nAbilityLV, int nNPCID, const char* strElseMsg)
{
	if (g_QuestList.m_lpPhaseNode)
	{
		g_QuestList.m_lpTriggerNode = new TriggerNode;
		g_QuestList.m_lpTriggerNode->m_dwTriggerKind = TRIGGER_ABILITY_TALK;
		g_QuestList.m_lpTriggerNode->m_dwTriggerID = nNPCID;
		g_QuestList.m_lpTriggerNode->m_dwZoneID = nAbilityID;
		g_QuestList.m_lpTriggerNode->m_dwMaxCount = 1;
		g_QuestList.m_lpTriggerNode->m_dwLevel = nAbilityLV;
		g_QuestList.m_lpTriggerNode->m_fDistance = 0.0f;
		g_QuestList.m_lpTriggerNode->m_fPosX = 0.0f;
		g_QuestList.m_lpTriggerNode->m_fPosY = 0.0f;
		g_QuestList.m_lpTriggerNode->m_fPosZ = 0.0f;
		bFalseEvent = FALSE;

		if (0 != strcmp(strElseMsg, ""))
		{
			g_QuestList.m_lpTriggerNode->m_strElseMsg = strElseMsg;
		}
		else
		{
			g_QuestList.m_lpTriggerNode->m_strElseMsg.clear();
		}
		
		g_QuestList.m_lpPhaseNode->m_lstTrigger.push_back(g_QuestList.m_lpTriggerNode);
	}
}


static void Trigger_LevelTalk(int nLevel, int nNPCID, const char* strElseMsg)
{
	if (g_QuestList.m_lpPhaseNode)
	{
		g_QuestList.m_lpTriggerNode = new TriggerNode;
		g_QuestList.m_lpTriggerNode->m_dwTriggerKind = TRIGGER_LEVEL_TALK;
		g_QuestList.m_lpTriggerNode->m_dwTriggerID = nNPCID;
		g_QuestList.m_lpTriggerNode->m_dwZoneID = 0;
		g_QuestList.m_lpTriggerNode->m_dwMaxCount = 1;
		g_QuestList.m_lpTriggerNode->m_dwLevel = nLevel;
		g_QuestList.m_lpTriggerNode->m_fDistance = 0.0f;
		g_QuestList.m_lpTriggerNode->m_fPosX = 0.0f;
		g_QuestList.m_lpTriggerNode->m_fPosY = 0.0f;
		g_QuestList.m_lpTriggerNode->m_fPosZ = 0.0f;
		bFalseEvent = FALSE;

		if (0 != strcmp(strElseMsg, ""))
		{
			g_QuestList.m_lpTriggerNode->m_strElseMsg = strElseMsg;
		}
		else
		{
			g_QuestList.m_lpTriggerNode->m_strElseMsg.clear();
		}
		
		g_QuestList.m_lpPhaseNode->m_lstTrigger.push_back(g_QuestList.m_lpTriggerNode);
	}
}

static void Trigger_FameTalk(int nFame, int nNPCID, const char* strElseMsg)
{
	if (g_QuestList.m_lpPhaseNode)
	{
		g_QuestList.m_lpTriggerNode = new TriggerNode;
		g_QuestList.m_lpTriggerNode->m_dwTriggerKind = TRIGGER_FAME_TALK;
		g_QuestList.m_lpTriggerNode->m_dwTriggerID = nNPCID;
		g_QuestList.m_lpTriggerNode->m_dwZoneID = 0;
		g_QuestList.m_lpTriggerNode->m_dwMaxCount = 1;
		g_QuestList.m_lpTriggerNode->m_dwLevel = nFame;
		g_QuestList.m_lpTriggerNode->m_fDistance = 0.0f;
		g_QuestList.m_lpTriggerNode->m_fPosX = 0.0f;
		g_QuestList.m_lpTriggerNode->m_fPosY = 0.0f;
		g_QuestList.m_lpTriggerNode->m_fPosZ = 0.0f;
		bFalseEvent = FALSE;

		if (0 != strcmp(strElseMsg, ""))
		{
			g_QuestList.m_lpTriggerNode->m_strElseMsg = strElseMsg;
		}
		else
		{
			g_QuestList.m_lpTriggerNode->m_strElseMsg.clear();
		}
		
		g_QuestList.m_lpPhaseNode->m_lstTrigger.push_back(g_QuestList.m_lpTriggerNode);
	}
}

static void Else(void)
{
	bFalseEvent = TRUE;
}

static void Event_Disappear(int nAmount, int nItemID, int nGold)
{
	if (g_QuestList.m_lpTriggerNode)
	{
		LPEventNode lpEventNode = new EventNode;
		lpEventNode->m_dwEventKind = EVENT_DISAPPEAR;
		lpEventNode->m_dwEventNumber = nItemID;
		lpEventNode->m_dwEventAmount = nAmount;
		lpEventNode->m_dwEventAmount2 = nGold;
		lpEventNode->m_fPosX = 0.0f;
		lpEventNode->m_fPosY = 0.0f;
		lpEventNode->m_fPosZ = 0.0f;
		lpEventNode->m_strWord = NULL;

		if (bFalseEvent)
			g_QuestList.m_lpTriggerNode->m_lstFalseEvent.push_back(lpEventNode);
		else
			g_QuestList.m_lpTriggerNode->m_lstEvent.push_back(lpEventNode);
	}
}

static void Event_Get(int nAmount, int nItemID)
{
	if (g_QuestList.m_lpTriggerNode)
	{
		LPEventNode lpEventNode = new EventNode;
		lpEventNode->m_dwEventKind = EVENT_GET;
		lpEventNode->m_dwEventNumber = nItemID;
		lpEventNode->m_dwEventAmount = nAmount;
		lpEventNode->m_dwEventAmount2 = 0;
		lpEventNode->m_fPosX = 0.0f;
		lpEventNode->m_fPosY = 0.0f;
		lpEventNode->m_fPosZ = 0.0f;
		lpEventNode->m_strWord = NULL;

		if (bFalseEvent)
			g_QuestList.m_lpTriggerNode->m_lstFalseEvent.push_back(lpEventNode);
		else
			g_QuestList.m_lpTriggerNode->m_lstEvent.push_back(lpEventNode);

		if (g_QuestList.m_lpTriggerNode->m_dwTriggerKind == TRIGGER_START)
		{
			std::map<unsigned short, unsigned char>::iterator itemItr;
            itemItr = g_QuestList.m_lpQuestNode->m_mapEventGetItem.find(nItemID);

			if (itemItr != g_QuestList.m_lpQuestNode->m_mapEventGetItem.end())
			{
				itemItr->second += nAmount;
			}
			else
			{
				g_QuestList.m_lpQuestNode->m_mapEventGetItem.insert(std::make_pair(nItemID, nAmount));
			}
		}
	}
}

static void Event_Spawn(int nMonsterID, float fPosX, float fPosY, float fPosZ)
{
	if (g_QuestList.m_lpTriggerNode)
	{
		LPEventNode lpEventNode = new EventNode;
		lpEventNode->m_dwEventKind = EVENT_SPAWN;
		lpEventNode->m_dwEventNumber = nMonsterID;
		lpEventNode->m_dwEventAmount = 0;
		lpEventNode->m_dwEventAmount2 = 0;
		lpEventNode->m_fPosX = fPosX * 100.0f;
		lpEventNode->m_fPosY = fPosY * 100.0f;
		lpEventNode->m_fPosZ = fPosZ * 100.0f;
		lpEventNode->m_strWord = NULL;

		if (bFalseEvent)
			g_QuestList.m_lpTriggerNode->m_lstFalseEvent.push_back(lpEventNode);
		else
			g_QuestList.m_lpTriggerNode->m_lstEvent.push_back(lpEventNode);
	}
}

static void Event_MonsterDrop(int nAmount, int nItemID)
{
	if (g_QuestList.m_lpTriggerNode)
	{
		LPEventNode lpEventNode = new EventNode;
		lpEventNode->m_dwEventKind = EVENT_MONSTERDROP;
		lpEventNode->m_dwEventNumber = nItemID;
		lpEventNode->m_dwEventAmount = nAmount;
		lpEventNode->m_dwEventAmount2 = 0;
		lpEventNode->m_fPosX = 0.0f;
		lpEventNode->m_fPosY = 0.0f;
		lpEventNode->m_fPosZ = 0.0f;
		lpEventNode->m_strWord = NULL;

		if (bFalseEvent)
			g_QuestList.m_lpTriggerNode->m_lstFalseEvent.push_back(lpEventNode);
		else
			g_QuestList.m_lpTriggerNode->m_lstEvent.push_back(lpEventNode);
	}
}
/*
static void Event_Award(int nExp, int nGold)
{
	if (g_QuestList.m_lpTriggerNode)
	{
		LPEventNode lpEventNode = new EventNode;
		lpEventNode->m_dwEventKind = EVENT_AWARD;
		lpEventNode->m_dwEventNumber = nExp;
		lpEventNode->m_dwEventAmount = nGold;
		lpEventNode->m_dwEventAmount2 = 0;
		lpEventNode->m_fPosX = 0.0f;
		lpEventNode->m_fPosY = 0.0f;
		lpEventNode->m_fPosZ = 0.0f;
		lpEventNode->m_strWord = NULL;

		if (bFalseEvent)
			g_QuestList.m_lpTriggerNode->m_lstFalseEvent.push_back(lpEventNode);
		else
			g_QuestList.m_lpTriggerNode->m_lstEvent.push_back(lpEventNode);
	}
}
*/

static void Event_Award(int nExp, int nGold, int nFame , int nMedal )
{
	if (g_QuestList.m_lpTriggerNode)
	{
		LPEventNode lpEventNode = new EventNode;
		lpEventNode->m_dwEventKind = EVENT_AWARD;
		lpEventNode->m_dwEventNumber = nExp;
		lpEventNode->m_dwEventAmount = nGold;
		lpEventNode->m_dwEventAmount2 = 0;
		lpEventNode->m_fPosX = 0.0f;
		lpEventNode->m_fPosY = 0.0f;
		lpEventNode->m_fPosZ = 0.0f;
		lpEventNode->m_strWord = NULL;

		if (bFalseEvent)
			g_QuestList.m_lpTriggerNode->m_lstFalseEvent.push_back(lpEventNode);
		else
			g_QuestList.m_lpTriggerNode->m_lstEvent.push_back(lpEventNode);
	}
}


static void Event_AddQuest( int nUID, int nQuestID )
{

}

static void Event_AwardItem( int nOptionType, int nLevel, int nGrade, int nBaseNum )
{

}

static void Event_AwardItemID( int nOptionType, int nID, int nGrade, int nBaseNum )
{

}

static void Event_AwardItemClass( int nOptionType, int nClass, int nLevel, int nGrade, int nBaseNum )
{

}

static void Event_MsgBox(const char *strMessage)
{
	if (g_QuestList.m_lpTriggerNode)
	{
		char *strScript = new char[strlen(strMessage) + 2];
		strScript[0] = 'm';
		strcpy(&strScript[1], strMessage);

		if (bFalseEvent)
			g_QuestList.m_lpTriggerNode->m_lstFalseMsgBox.push_back(strScript);
		else
			g_QuestList.m_lpTriggerNode->m_lstMsgBox.push_back(strScript);
	}
}

static void Event_Phase(int nPhaseNumber)
{
	if (g_QuestList.m_lpTriggerNode)
	{
		LPEventNode lpEventNode = new EventNode;
		lpEventNode->m_dwEventKind = EVENT_PHASE;
		lpEventNode->m_dwEventNumber = nPhaseNumber;
		lpEventNode->m_dwEventAmount = 0;
		lpEventNode->m_dwEventAmount2 = 0;
		lpEventNode->m_fPosX = 0.0f;
		lpEventNode->m_fPosY = 0.0f;
		lpEventNode->m_fPosZ = 0.0f;
		lpEventNode->m_strWord = NULL;

		if (bFalseEvent)
			g_QuestList.m_lpTriggerNode->m_lstFalseEvent.push_back(lpEventNode);
		else
			g_QuestList.m_lpTriggerNode->m_lstEvent.push_back(lpEventNode);
	}
}

static void Event_End(void)
{
	if (g_QuestList.m_lpTriggerNode)
	{
		LPEventNode lpEventNode = new EventNode;
		lpEventNode->m_dwEventKind = EVENT_END;
		lpEventNode->m_dwEventNumber = 0;
		lpEventNode->m_dwEventAmount = 0;
		lpEventNode->m_dwEventAmount2 = 0;
		lpEventNode->m_fPosX = 0.0f;
		lpEventNode->m_fPosY = 0.0f;
		lpEventNode->m_fPosZ = 0.0f;
		lpEventNode->m_strWord = NULL;

		if (bFalseEvent)
			g_QuestList.m_lpTriggerNode->m_lstFalseEvent.push_back(lpEventNode);
		else
			g_QuestList.m_lpTriggerNode->m_lstEvent.push_back(lpEventNode);
	}
}

static void Event_Move(int nZone, float fPosX, float fPosY, float fPosZ)
{
	if (g_QuestList.m_lpTriggerNode)
	{
		LPEventNode lpEventNode = new EventNode;
		lpEventNode->m_dwEventKind = EVENT_MOVE;
		lpEventNode->m_dwEventNumber = nZone;
		lpEventNode->m_dwEventAmount = 0;
		lpEventNode->m_dwEventAmount2 = 0;
		lpEventNode->m_fPosX = fPosX;
		lpEventNode->m_fPosY = fPosY;
		lpEventNode->m_fPosZ = fPosZ;
		lpEventNode->m_strWord = NULL;

		if (bFalseEvent)
			g_QuestList.m_lpTriggerNode->m_lstFalseEvent.push_back(lpEventNode);
		else
			g_QuestList.m_lpTriggerNode->m_lstEvent.push_back(lpEventNode);
	}
}

static void Event_TheaterMode(void)
{
	if (g_QuestList.m_lpTriggerNode)
	{
		LPEventNode lpEventNode = new EventNode;
		lpEventNode->m_dwEventKind = EVENT_THEATERMODE;
		lpEventNode->m_dwEventNumber = 0;
		lpEventNode->m_dwEventAmount = 0;
		lpEventNode->m_dwEventAmount2 = 0;
		lpEventNode->m_fPosX = 0.0f;
		lpEventNode->m_fPosY = 0.0f;
		lpEventNode->m_fPosZ = 0.0f;
		lpEventNode->m_strWord = NULL;

		if (bFalseEvent)
			g_QuestList.m_lpTriggerNode->m_lstFalseEvent.push_back(lpEventNode);
		else
			g_QuestList.m_lpTriggerNode->m_lstEvent.push_back(lpEventNode);
	}
}

static void Event_AlarmBox(const char *strMessage)
{
	if (g_QuestList.m_lpTriggerNode)
	{
		char *strScript = new char[strlen(strMessage) + 2];
		strScript[0] = 'a';
		strcpy(&strScript[1], strMessage);

		if (bFalseEvent)
			g_QuestList.m_lpTriggerNode->m_lstFalseMsgBox.push_back(strScript);
		else
			g_QuestList.m_lpTriggerNode->m_lstMsgBox.push_back(strScript);
	}
}

static void Event_AwardAbility( int nAbilityID, int nAbilityLV )
{

}

CQuestList::CQuestList()
{
	m_lpQuestNode = NULL;
	m_lpPhaseNode = NULL;
	m_lpTriggerNode = NULL;
}

CQuestList::~CQuestList()
{
	DestroyQuestList();	
}

VOID CQuestList::DestroyQuestList()
{
	vector<LPQuestNode>::iterator itQuest;
	vector<LPPhaseNode>::iterator itPhase;
	vector<LPTriggerNode>::iterator itTrigger;
	vector<LPEventNode>::iterator itEvent;
	vector<char *>::iterator itString;

	LPQuestNode lpQuest;
	LPPhaseNode lpPhase;
	LPTriggerNode lpTrigger;
	LPEventNode lpEvent;
	char *strDelete;

	for (itQuest = m_lstQuestNode.begin(); itQuest != m_lstQuestNode.end();)
	{
		lpQuest = (*itQuest);
		itQuest = m_lstQuestNode.erase(itQuest);
		for (itPhase = lpQuest->m_lstPhase.begin(); itPhase != lpQuest->m_lstPhase.end();)
		{
			lpPhase = (*itPhase);
			itPhase = lpQuest->m_lstPhase.erase(itPhase);
			for (itTrigger = lpPhase->m_lstTrigger.begin(); itTrigger != lpPhase->m_lstTrigger.end();)
			{
				lpTrigger = (*itTrigger);
				itTrigger = lpPhase->m_lstTrigger.erase(itTrigger);

				for (itEvent = lpTrigger->m_lstEvent.begin(); itEvent != lpTrigger->m_lstEvent.end();)
				{
					lpEvent = (*itEvent);
					itEvent = lpTrigger->m_lstEvent.erase(itEvent);
					if (lpEvent->m_strWord) delete[] lpEvent->m_strWord;
					delete lpEvent;
				}
				lpTrigger->m_lstEvent.clear();

				for (itString = lpTrigger->m_lstMsgBox.begin(); itString != lpTrigger->m_lstMsgBox.end();)
				{
					strDelete = (*itString);
					itString = lpTrigger->m_lstMsgBox.erase(itString);
					delete[] strDelete;
				}
				lpTrigger->m_lstMsgBox.clear();

				for (itEvent = lpTrigger->m_lstFalseEvent.begin(); itEvent != lpTrigger->m_lstFalseEvent.end();)
				{
					lpEvent = (*itEvent);
					itEvent = lpTrigger->m_lstFalseEvent.erase(itEvent);
					if (lpEvent->m_strWord) delete[] lpEvent->m_strWord;
					delete lpEvent;
				}
				lpTrigger->m_lstFalseEvent.clear();

				for (itString = lpTrigger->m_lstFalseMsgBox.begin(); itString != lpTrigger->m_lstFalseMsgBox.end();)
				{
					strDelete = (*itString);
					itString = lpTrigger->m_lstFalseMsgBox.erase(itString);
					delete[] strDelete;
				}
				lpTrigger->m_lstFalseMsgBox.clear();

				delete lpTrigger;
			}
			lpPhase->m_lstTrigger.clear();

			delete[] lpPhase->m_strPhaseDesc;

			delete lpPhase;
		}
		lpQuest->m_lstPhase.clear();

		if (lpQuest->m_strQuestTitle) delete[] lpQuest->m_strQuestTitle;
		if (lpQuest->m_strQuestDesc) delete[] lpQuest->m_strQuestDesc;
		if (lpQuest->m_strQuestShortDesc) delete[] lpQuest->m_strQuestShortDesc;
		if (lpQuest->m_strQuestLevel) delete[] lpQuest->m_strQuestLevel;
		if (lpQuest->m_strQuestAward) delete[] lpQuest->m_strQuestAward;

		delete lpQuest;
	}

	m_lstQuestNode.clear();
}

BOOL CQuestList::Load(const char *strQuestScriptFile)
{
	DestroyQuestList();

	SCRIPT Script = _SE_Create(strQuestScriptFile);
	if (!Script) return FALSE;

	_SE_SetMessageFunction(ScriptErrorMessage);

	//_SE_RegisterFunction(Script, QuestStart, T_VOID, "QuestStart", T_INT, T_INT, T_INT, T_INT, T_INT, 0);
    //_SE_RegisterFunction(Script, QuestStart, T_VOID, "QuestStart", T_INT, T_INT, T_INT, T_INT, T_INT, T_INT, 0);
	_SE_RegisterFunction(Script, QuestStartLevel, T_VOID, "QuestStartLevel", T_INT, T_INT, T_INT, T_INT, T_INT, T_INT, T_BOOL, 0);
	_SE_RegisterFunction(Script, QuestStartFame, T_VOID, "QuestStartFame", T_INT, T_INT, T_INT, T_INT, T_INT, T_INT, T_BOOL, 0);
	_SE_RegisterFunction(Script, QuestEnd, T_VOID, "QuestEnd", 0);
	_SE_RegisterFunction(Script, QuestType, T_VOID, "QuestType", T_INT, T_INT, 0);
	_SE_RegisterFunction(Script, QuestArea, T_VOID, "QuestArea", T_INT, T_FLOAT, T_FLOAT, T_FLOAT, T_FLOAT, 0);
	_SE_RegisterFunction(Script, QuestTitle, T_VOID, "QuestTitle", T_STRING, 0);
	_SE_RegisterFunction(Script, QuestDesc, T_VOID, "QuestDesc", T_STRING, 0);
	_SE_RegisterFunction(Script, QuestShortDesc, T_VOID, "QuestShortDesc", T_STRING, 0);
	_SE_RegisterFunction(Script, QuestIcon, T_VOID, "QuestIcon", T_STRING, T_INT, T_INT, T_INT, T_INT, 0);
	_SE_RegisterFunction(Script, QuestCompleteSave, T_VOID, "QuestCompleteSave", T_BOOL, 0);
	_SE_RegisterFunction(Script, QuestLevel, T_VOID, "QuestLevel", T_STRING, 0);
	_SE_RegisterFunction(Script, QuestAward, T_VOID, "QuestAward", T_STRING, 0);
    _SE_RegisterFunction(Script, QuestSkillPointBonus, T_VOID, "QuestSkillPointBonus", T_INT, 0);
	_SE_RegisterFunction(Script, QuestCancelItemDel, T_VOID, "QuestCancelItemDel", T_BOOL, 0);
	// 어빌리티 시스템
    _SE_RegisterFunction(Script, QuestAbilityPoint, T_VOID, "QuestAbilityPoint", T_INT, 0);

	

	_SE_RegisterFunction(Script, AddPhase, T_VOID, "AddPhase", T_INT, T_INT, T_STRING, 0);
	_SE_RegisterFunction(Script, Phase_Target, T_VOID, "Phase_Target", T_INT, T_INT, T_INT, 0);
	_SE_RegisterFunction(Script, Trigger_Start, T_VOID, "Trigger_Start", 0);
	_SE_RegisterFunction(Script, Trigger_Puton, T_VOID, "Trigger_Puton", T_INT, T_INT, T_FLOAT, T_FLOAT, T_FLOAT, T_FLOAT, 0);
	_SE_RegisterFunction(Script, Trigger_Geton, T_VOID, "Trigger_Geton", T_INT, T_INT, T_FLOAT, T_FLOAT, T_FLOAT, T_FLOAT, 0);
	_SE_RegisterFunction(Script, Trigger_Talk, T_VOID, "Trigger_Talk", T_INT, 0);
	_SE_RegisterFunction(Script, Trigger_Kill, T_VOID, "Trigger_Kill", T_INT, T_INT, 0);
	_SE_RegisterFunction(Script, Trigger_Pick, T_VOID, "Trigger_Pick", T_INT, T_INT, 0);
	_SE_RegisterFunction(Script, Trigger_Fame, T_VOID, "Trigger_Fame", T_INT, 0);
	_SE_RegisterFunction(Script, Trigger_LevelTalk, T_VOID, "Trigger_LevelTalk", T_INT, T_INT, T_STRING, 0);
	_SE_RegisterFunction(Script, Trigger_FameTalk, T_VOID, "Trigger_FameTalk", T_INT, T_INT, T_STRING, 0);
	_SE_RegisterFunction(Script, Else, T_VOID, "Else", 0);
	_SE_RegisterFunction(Script, Event_Disappear, T_VOID, "Event_Disappear", T_INT, T_INT, T_INT, 0);
	_SE_RegisterFunction(Script, Event_Get, T_VOID, "Event_Get", T_INT, T_INT, 0);
	_SE_RegisterFunction(Script, Event_Spawn, T_VOID, "Event_Spawn", T_INT, T_FLOAT, T_FLOAT, T_FLOAT, 0);
	_SE_RegisterFunction(Script, Event_MonsterDrop, T_VOID, "Event_MonsterDrop", T_INT, T_INT, 0);
	_SE_RegisterFunction(Script, Event_MsgBox, T_VOID, "Event_MsgBox", T_STRING, 0);
	_SE_RegisterFunction(Script, Event_Phase, T_VOID, "Event_Phase", T_INT, 0);
	_SE_RegisterFunction(Script, Event_End, T_VOID, "Event_End", T_VOID, 0);
	_SE_RegisterFunction(Script, Event_AddQuest, T_VOID, "Event_AddQuest", T_INT, T_INT, 0 );
	_SE_RegisterFunction(Script, Event_AwardItem, T_VOID, "Event_AwardItem", T_INT, T_INT, T_INT, T_INT, 0 );
	_SE_RegisterFunction(Script, Event_AwardItemID, T_VOID, "Event_AwardItemID", T_INT, T_INT, T_INT, T_INT, 0 );
	_SE_RegisterFunction(Script, Event_AwardItemClass, T_VOID, "Event_AwardItemClass", T_INT, T_INT, T_INT, T_INT, T_INT, 0 );
	_SE_RegisterFunction(Script, Event_Award, T_VOID, "Event_Award", T_INT, T_INT, T_INT, T_INT, 0 );
	_SE_RegisterFunction(Script, Event_Move, T_VOID, "Event_Move", T_INT, T_FLOAT, T_FLOAT, T_FLOAT, 0);
	_SE_RegisterFunction(Script, Event_TheaterMode, T_VOID, "Event_TheaterMode", 0);
	_SE_RegisterFunction(Script, Event_AlarmBox, T_VOID, "Event_AlarmBox", T_STRING, 0);

	_SE_RegisterFunction(Script, Event_AwardAbility, T_VOID, "Event_AwardAbility", T_INT, T_INT, 0 );
	_SE_RegisterFunction(Script, Trigger_AbilityTalk, T_VOID, "Trigger_AbilityTalk", T_INT, T_INT, T_INT, T_STRING, 0);
	

	_SE_Execute(Script);

	_SE_Destroy(Script);

	return TRUE;
}




unsigned long QuestNode::CheckQuest(unsigned long dwLevel, unsigned long dwFame, unsigned long dwClass, unsigned long dwNation, unsigned short *lstCompleted, unsigned short wNumCompleted, unsigned short *lstQuestList)
{
	unsigned long count = 0;
	unsigned long i = 0;

	// 수행할 수 있는 국적인지 체크
	bool bEnableQuest = false ;
	if ( 0 == m_usNation )
	{
		bEnableQuest = true ;
	}
	else
	{
		unsigned long dwQuestNation = m_usNation ;
		unsigned long dwNationRes = 0 ;

		while ( dwQuestNation != 0 )
		{
			dwNationRes = dwQuestNation % 10 ;
			dwQuestNation /= 10 ;

			if ( 0 == dwNationRes || dwNationRes == dwNation )
			{
				bEnableQuest = true ;
				break;
			}
		}
	}

	if ( !bEnableQuest )
	{
		// 국적이 맞지 않는다.
		return QuestNode::RES_MISMATCH_NATION ;
	}

	if ( !(m_dwClass & (0x00000001 << (dwClass - 1))) )
	{
		// 클래스가 맞지 않는다.
		return QuestNode::RES_MISMATCH_CLASS ;
	}

	// 이미 한 퀘스트인지 검사와 선결 퀘스트를 했는지 검사
	for (unsigned long t = 0; t < wNumCompleted ; t++)
	{
		if (lstCompleted[t] == m_wQuestID)
		{
			// 이미 한 퀘스트이다.
			return QuestNode::RES_ALREADY_TAKEN ;
		}
	}

	for (unsigned long t = 0; t < 10; t++)
	{
		if (lstQuestList[t] == m_wQuestID)
		{
			// 지금 하고 있는 퀘스트이다.
			return QuestNode::RES_ALREADY_TAKEN ;
		}
	}

	BOOL bCompletePriorQuest = FALSE;
	if (m_dwCompletedQuest)
	{
		for (unsigned long t = 0; t < wNumCompleted ; t++)
		{
			if (lstCompleted[t] == m_dwCompletedQuest)
			{
				bCompletePriorQuest = TRUE;
			}
		}

		if ( !bCompletePriorQuest )
		{
			// 선결 퀘스트를 하지 않았다.
			return QuestNode::RES_NOT_COMPLETE_PRIOR_QUEST ;
		}
	}

	if(m_wMinLevel != 0 && m_wMaxLevel != 0)
	{
		if (m_wMinLevel <= dwLevel && dwLevel <= m_wMaxLevel)
		{
			// 퀘스트를 할 수 있다.
			return QuestNode::RES_SUCCESS ;
		}

		if ( dwLevel > m_wMaxLevel )
		{
			// 수행 가능한 최고 레벨보다 높다.
			return QuestNode::RES_HIGH_THAN_MAX_LEVEL ;
		}

		if ( dwLevel + 10 <= m_wMinLevel )
		{
			// 수행 가능한 최저 레벨보다 10 레벨 이상 낮다.
			return QuestNode::RES_10_LOW_THAN_MIN_LEVEL ;
		}

		// 레벨에 좀 낮다.
		return QuestNode::RES_LOW_LEVEL ;
	}

	if(m_dwMinFame != 0 && m_dwMaxFame != 0)
	{
		// 명성체크
		if (m_dwMinFame <= dwFame && dwFame <= m_dwMaxFame)
		{
			// 퀘스트를 할 수 있다.
			return QuestNode::RES_SUCCESS ;
		}

		if ( dwFame > m_dwMaxFame )
		{
			// 수행 가능한 최고 레벨보다 높다.
			return QuestNode::RES_HIGH_THAN_MAX_FAME ;
		}

		// 명성이 낮다
		return QuestNode::RES_LOW_FAME;
	}

	return QuestNode::RES_LOW_LEVEL;
}

LPQuestNode CQuestList::GetQuest(unsigned short wQuestID)
{
	vector<LPQuestNode>::iterator it;
	for (it = m_lstQuestNode.begin(); it != m_lstQuestNode.end(); it++)
	{
		if ((*it)->m_wQuestID == wQuestID) return (*it);
	}

	return NULL;
}


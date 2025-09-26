// NPCList.cpp: implementation of the CNPCList class.
//
//////////////////////////////////////////////////////////////////////

#include <numeric>

#include <ScriptEngine/ScriptEngine.h>
#include <Creature/CreatureStructure.h>

#include <item/ItemFactory.h>
#include <item/Item.h>

#include <Castle/CastleConstants.h>

#include "NPCList.h"

CNPCList g_NPCList;

static SCRIPT m_scQuestScript;
static BOOL bFalseEvent;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static void ScriptErrorMessage(const char *msg)
{
	MessageBox(NULL, msg, "Script Error", MB_OK);
}


//-- 2004. 9. 16. Zergra From. --//
// NPC 팝업 메뉴 중 "대화" 메뉴가 여러개 뜨는 것에 관련하여 그 갯수를 세는 함수. ShowDialog()의 페이지 인자로 사용되기때문에 1로 초기화한다.
unsigned long g_unDialogCount = 1;
//-- Zergra To. --// 




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __NEWNPCSCRIPT
	static void ContainerStart(int nContainerID)
	{
		if(g_NPCList.m_lpLoadContainer == NULL)
		{
			g_NPCList.m_lpLoadContainer = new ContainerNode;

			g_NPCList.m_lstContainerNode.insert( pair< unsigned long, LPContainerNode>( nContainerID, g_NPCList.m_lpLoadContainer ) );
		} else
		{
			MessageBox(NULL, "Container already create", "Script Error", MB_OK);
		}
	}

	static void ContainerEnd()
	{
		g_NPCList.m_lpLoadPage = NULL;
		g_NPCList.m_lpLoadTab = NULL;
		g_NPCList.m_lpLoadContainer = NULL;
	}

	static void TabStart(const char* strTitle)
	{
		if(g_NPCList.m_lpLoadContainer)
		{
			if(g_NPCList.m_lpLoadTab == NULL)
			{
				g_NPCList.m_lpLoadTab = new TabNode;
				g_NPCList.m_lpLoadTab->m_strTitle = strTitle;
//				sprintf(g_NPCList.m_lpLoadTab->m_strTitle, "%s", strTitle);
//				strcpy(g_NPCList.m_lpLoadTab->m_strTitle, strTitle);

				g_NPCList.m_lpLoadContainer->m_lstTabNode.push_back(g_NPCList.m_lpLoadTab);
				g_NPCList.m_lpLoadContainer->m_dwNumTab = g_NPCList.m_lpLoadContainer->m_lstTabNode.size();
			} else
			{
				MessageBox(NULL, "Tab already create", "Script Error", MB_OK);
			}
		}
	}

	static void TabEnd()
	{
		g_NPCList.m_lpLoadPage = NULL;
		g_NPCList.m_lpLoadTab = NULL;
	}

	static void PageStart()
	{
		if(g_NPCList.m_lpLoadTab)
		{
			if(g_NPCList.m_lpLoadPage == NULL)
			{
				g_NPCList.m_lpLoadPage = new PageNode;

				g_NPCList.m_lpLoadTab->m_lstPageNode.push_back(g_NPCList.m_lpLoadPage);
				g_NPCList.m_lpLoadTab->m_dwNumPage = g_NPCList.m_lpLoadTab->m_lstPageNode.size();
			} else
			{
				MessageBox(NULL, "Page already create", "Script Error", MB_OK);
			}
		}
	}

	static void PageEnd()
	{
		g_NPCList.m_lpLoadPage = NULL;
	}

	static void SetNormalMarket(bool bNormalMarket)
	{
		if(g_NPCList.m_lpLoadContainer)
		{
			g_NPCList.m_lpLoadContainer->m_bNormalMarket = TRUE;
		}
	}

	static void SetBlackMarket(bool bBlackMarket)
	{
		if(g_NPCList.m_lpLoadContainer)
		{
			g_NPCList.m_lpLoadContainer->m_bBlackMarket = TRUE;
			g_NPCList.m_lpLoadContainer->m_bNormalMarket = FALSE;
		}
	}

	static void SetRandomOptionMarket(bool bRandomMarket)
	{
		if(g_NPCList.m_lpLoadContainer)
		{
			g_NPCList.m_lpLoadContainer->m_bRandomOptionMarket = TRUE;
			g_NPCList.m_lpLoadContainer->m_bNormalMarket = FALSE;
		}
	}

	static void SetSkillCouponMarket(bool bCouponMarket)
	{
		if(g_NPCList.m_lpLoadContainer)
		{
			g_NPCList.m_lpLoadContainer->m_bSkillCouponMarket = TRUE;
			g_NPCList.m_lpLoadContainer->m_bNormalMarket = FALSE;
		}
	}

	static void SetMedalMarket(bool bMedalMarket)
	{
		if(g_NPCList.m_lpLoadContainer)
		{
			g_NPCList.m_lpLoadContainer->m_bMedalMarket = TRUE;
			g_NPCList.m_lpLoadContainer->m_bNormalMarket = FALSE;
		}
	}

	static void SetEnableRepair(bool bEnableMarket)
	{
		if(g_NPCList.m_lpLoadContainer)
		{
			g_NPCList.m_lpLoadContainer->m_bEnableRepair = TRUE;
		}
	}
#endif

#ifdef __NEWNPCSCRIPT
	static void AddItem(int nKindItem)
	{
		if(g_NPCList.m_lpLoadPage)
		{
			LPItemNode lpItem = g_NPCList.m_lpLoadContainer->GetITEMNode(nKindItem);
			if(!lpItem)
			{
				lpItem = new ItemNode;
				lpItem->m_wKindItem = static_cast<unsigned short>(nKindItem);
				lpItem->m_wSkill = 0;
				lpItem->m_wSkillLevel = 0;

				g_NPCList.m_lpLoadPage->m_lstItemNode.push_back(lpItem);
			}
		}
	}
#else
	static void AddItem(int nUID, int nMenuIndex, int nTab, int nPage, int nKindItem)
	{
		NPCNode* lpNode = g_NPCList.GetNPCNode(nUID);

		if (lpNode)
		{
			ITEMNode* lpItem = lpNode->GetITEMNode(nKindItem);
			if (!lpItem)
			{
				lpItem = new ITEMNode;
				lpItem->m_wKindItem = (unsigned short)nKindItem;
				lpItem->m_wSkill = 0;
				lpItem->m_wSkillLevel = 0;
				lpItem->m_cMenuIndex = static_cast<unsigned char>(nMenuIndex);
				lpItem->m_wTab = static_cast<unsigned short>(nTab);
				lpItem->m_cPage = static_cast<unsigned char>(nPage);

				lpNode->m_aryPhaseMax[ nTab + nMenuIndex * 4 ] = max( lpNode->m_aryPhaseMax[ nTab + nMenuIndex * 4 ], ((unsigned char)nPage + 1) ) ;
				lpNode->m_lstItem.push_back(lpItem);

				SingleTradeWindow*  pWindow = NULL;
				SingleTradeTab*     pTab    = NULL;
				SingleTradePage*    pPage   = NULL;

				if( lpItem->m_cMenuIndex + 1 > static_cast<int>( lpNode->m_vecTradeWindow.size() ) )
				{
					while( lpItem->m_cMenuIndex + 1 > static_cast<int>( lpNode->m_vecTradeWindow.size() ) )
					{
						pWindow = new SingleTradeWindow;
						lpNode->m_vecTradeWindow.push_back( pWindow );
					}
				}

				if( lpItem->m_wTab + 1 > static_cast<int>( lpNode->m_vecTradeWindow[ lpItem->m_cMenuIndex ]->m_vecTradeTab.size() ) )
				{
					while( lpItem->m_wTab + 1 > static_cast<int>( lpNode->m_vecTradeWindow[ lpItem->m_cMenuIndex ]->m_vecTradeTab.size() ) )
					{
						pTab = new SingleTradeTab;
						lpNode->m_vecTradeWindow[ lpItem->m_cMenuIndex ]->m_vecTradeTab.push_back( pTab );
					}
				}

				if( lpItem->m_cPage + 1 > static_cast<int>( lpNode->m_vecTradeWindow[ lpItem->m_cMenuIndex ]->m_vecTradeTab[ lpItem->m_wTab ]->m_vecTradePage.size() ) )
				{
					while( lpItem->m_cPage + 1 > static_cast<int>( lpNode->m_vecTradeWindow[ lpItem->m_cMenuIndex ]->m_vecTradeTab[ lpItem->m_wTab ]->m_vecTradePage.size() ) )
					{
						pPage = new SingleTradePage;
						lpNode->m_vecTradeWindow[ lpItem->m_cMenuIndex ]->m_vecTradeTab[ lpItem->m_wTab ]->m_vecTradePage.push_back( pPage );
					}
				}
			}
		}
	}
#endif




#ifdef __NEWNPCSCRIPT
	static void PopupStart(int nPopupID)
	{
		if(g_NPCList.m_lpLoadPopup == NULL)
		{
			g_NPCList.m_lpLoadPopup = new PopupNode;

			g_NPCList.m_lstPopupNode.insert(pair<unsigned long, LPPopupNode>(nPopupID, g_NPCList.m_lpLoadPopup));
		} else
		{
			MessageBox(NULL, "Popup already create", "Script Error", MB_OK);
		}
	}

	static void PopupEnd()
	{
		g_NPCList.m_lpLoadPopup = NULL;
	}

	static void PopupAttribute(int nKindPopup, int nLimitLevel, int nLimitClass, int nLimitQuest)
	{
		if(g_NPCList.m_lpLoadPopup)
		{
			g_NPCList.m_lpLoadPopup->m_dwPopupKind = static_cast<unsigned long>(nKindPopup);
			g_NPCList.m_lpLoadPopup->m_dwLimitLevel = static_cast<unsigned long>(nLimitLevel);
			g_NPCList.m_lpLoadPopup->m_dwLimitClass = static_cast<unsigned long>(nLimitClass);
			g_NPCList.m_lpLoadPopup->m_dwLimitQuest = static_cast<unsigned long>(nLimitQuest);
		}
	}
#endif

#ifdef __NEWNPCSCRIPT
	static void AddContainer(int nContainerID)
	{
		if(g_NPCList.m_lpLoadPopup)
		{
			LPContainerNode lpContainerNode = g_NPCList.GetContainer(static_cast<unsigned long>(nContainerID));
			if(lpContainerNode)
			{
				LPTradeJobNode lpJobNode = new TradeJobNode;
				lpJobNode->m_dwContainerID = static_cast<unsigned long>(nContainerID);
				lpJobNode->m_lpContainer = lpContainerNode;
				g_NPCList.m_lpLoadPopup->m_lpJobNode = lpJobNode;
			}
		}
	}
#else
	static void AddQuest(int nUID, int nQuestID, int nMinLevel, int nMaxLevel, int nClass, int nCompleteClass, const char *strQuestFunc)
	{
		NPCNode* lpNode = g_NPCList.GetNPCNode(nUID);

		if (lpNode)
		{
			LPQuestNode lpQuestData = g_QuestList.GetQuest((unsigned short)nQuestID);
			if (lpQuestData)
			{
				LPQuestInstanceNode lpQuest = new QuestInstanceNode;
				lpQuest->m_wQuestID = (unsigned short)nQuestID;
				lpQuest->m_lpQuest = lpQuestData;
				lpNode->m_lstQuest.push_back(lpQuest);
			}
		}
	}
#endif

#ifdef __NEWNPCSCRIPT
	static void AddZoneMove(int nZoneNumber, float fPosX, float fPosY, float fPosZ)
	{
		if(g_NPCList.m_lpLoadPopup)
		{
			LPWarpJobNode lpJobNode = new WarpJobNode;
			lpJobNode->m_dwZoneNumber = static_cast<unsigned long>(nZoneNumber);
			lpJobNode->m_fPosX = fPosX;
			lpJobNode->m_fPosY = fPosY;
			lpJobNode->m_fPosZ = fPosZ;
			g_NPCList.m_lpLoadPopup->m_lpJobNode = lpJobNode;
		}
	}
#else
	static void AddZoneMove(int nUID, int nZoneNumber, float fPosX, float fPosY, float fPosZ)
	{
		NPCNode* lpNode = g_NPCList.GetNPCNode(nUID);

		if (lpNode)
		{
			WarpNode* lpZone = new WarpNode;
			lpZone->m_wNumber = (unsigned short)lpNode->m_lstWarpZone.size();
			lpZone->m_wZoneNumber = (unsigned short)nZoneNumber;
			lpZone->m_fPosX = fPosX;
			lpZone->m_fPosY = fPosY;
			lpZone->m_fPosZ = fPosZ;
			lpNode->m_lstWarpZone.push_back(lpZone);
		}
	}
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


















///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __NEWNPCSCRIPT
	static void SetNpc(int nZone, int nUID, int nTownID, bool bBelongToCastle, const char* strNpcSkin, const char* strNpcName)
	{
		if (nZone != g_NPCList.GetZone()) return;

		if (!g_NPCList.IsExistNpc(nUID))
		{
			NPCNode* lpNPCNode = new NPCNode;
			lpNPCNode->m_dwUID = (unsigned long)nUID;
			lpNPCNode->m_dwTownOrNameID = (unsigned long)nTownID;
			lpNPCNode->m_bBelongToCastle = bBelongToCastle;
			if (bBelongToCastle && (nTownID & Castle::CASTLE_BIT))
			{
				lpNPCNode->m_dwTownOrNameID = ((nTownID & ~Castle::CASTLE_BIT) >> Castle::CASTLE_NAME_BIT_SHIFT);
			}
			lpNPCNode->m_strNpcSkin = new char[strlen(strNpcSkin) + 1];
			strcpy(lpNPCNode->m_strNpcSkin, strNpcSkin);
			lpNPCNode->m_fDirection = 0.0f;
			lpNPCNode->m_fPosX = 0.0f;
			lpNPCNode->m_fPosY = 0.0f;
			lpNPCNode->m_fPosZ = 0.0f;
			lpNPCNode->m_strNpcName = new char[strlen(strNpcName) + 1];
//			lpNPCNode->m_bInitGradeOption = false;
//			memset( lpNPCNode->m_aryPhaseMax, 0, sizeof(unsigned char) * 8 );
			strcpy(lpNPCNode->m_strNpcName, strNpcName);

//			lpNPCNode->ClearEquipItem();
//			lpNPCNode->m_dwTime = 0;

			g_NPCList.AddNpc(lpNPCNode);

			g_unDialogCount = 1;
		}
	}
#else
	static void SetNpc(int nZone, int nUID, int nTownID, bool bBelongToCastle, const char* strNpcSkin, const char* strNpcName)
	{
		if (nZone != g_NPCList.GetZone()) return;

		if (!g_NPCList.IsExistNpc(nUID))
		{
			NPCNode* lpNPCNode = new NPCNode;
			lpNPCNode->m_dwUID = (unsigned long)nUID;
			lpNPCNode->m_dwTownOrNameID = (unsigned long)nTownID;
			lpNPCNode->m_bBelongToCastle = bBelongToCastle;
			if (bBelongToCastle && (nTownID & Castle::CASTLE_BIT))
			{
				lpNPCNode->m_dwTownOrNameID = ((nTownID & ~Castle::CASTLE_BIT) >> Castle::CASTLE_NAME_BIT_SHIFT);
			}
			lpNPCNode->m_strNpcSkin = new char[strlen(strNpcSkin) + 1];
			strcpy(lpNPCNode->m_strNpcSkin, strNpcSkin);
			lpNPCNode->m_fDirection = 0.0f;
			lpNPCNode->m_fPosX = 0.0f;
			lpNPCNode->m_fPosY = 0.0f;
			lpNPCNode->m_fPosZ = 0.0f;
			lpNPCNode->m_strNpcName = new char[strlen(strNpcName) + 1];
			lpNPCNode->m_bInitGradeOption = false;
			memset( lpNPCNode->m_aryPhaseMax, 0, sizeof(unsigned char) * 8 );
			strcpy(lpNPCNode->m_strNpcName, strNpcName);

			lpNPCNode->ClearEquipItem();
			lpNPCNode->m_dwTime = 0;

			g_NPCList.AddNpc(lpNPCNode);

			g_unDialogCount = 1;
		}
	}
#endif

static void SetPosition(int nUID, float fDirection, float fPosX, float fPosY, float fPosZ)
{
	NPCNode* lpNode = g_NPCList.GetNPCNode(nUID);

	if (lpNode)
	{
		lpNode->m_fDirection = fDirection + (3.1415926535f);
		lpNode->m_fPosX = fPosX;
		lpNode->m_fPosY = fPosY;
		lpNode->m_fPosZ = fPosZ;
	}
}

static void AddWords(int nUID, const char *strNpcScript)
{
	NPCNode* lpNode = g_NPCList.GetNPCNode(nUID);

	if (lpNode)
	{
		char *strScript = new char[strlen(strNpcScript) + 1];
		strcpy(strScript, strNpcScript);
		LPDialogNode lpWord = new DialogNode;
		lpWord->m_wKindWord = 0;
		lpWord->m_wDialogFlag = 0;
		lpWord->m_strWord = strScript;
		lpNode->m_lstScript.push_back(lpWord);
	}
}

static void AddDialog(int nUID, int nPage, int nFlag, const char *strNpcScript)
{
	NPCNode* lpNode = g_NPCList.GetNPCNode(nUID);

	if (lpNode)
	{
		char *strScript = new char[strlen(strNpcScript) + 1];
		strcpy(strScript, strNpcScript);
		LPDialogNode lpWord = new DialogNode;
		lpWord->m_wKindWord = (unsigned short)nPage;
		lpWord->m_wDialogFlag = (unsigned short)nFlag;
		lpWord->m_strWord = strScript;
		lpNode->m_lstScript.push_back(lpWord);
	}
}

#ifdef __NEWNPCSCRIPT
	static void AddQuestPopup( int nUID, int nQuestID, const char* strHelpMsg )
	{
		NPCNode* lpNpcNode = g_NPCList.GetNPCNode( nUID );

		if( lpNpcNode )
		{
			LPPopupNode lpPopupNode = new PopupNode;

			if( lpPopupNode )
			{
				LPQuestJobNode lpJobNode = new QuestJobNode;
				lpJobNode->m_dwQuestID = static_cast<unsigned long>(nQuestID);
				lpJobNode->m_lpQuest = g_QuestList.GetQuest(static_cast<unsigned short>(lpJobNode->m_dwQuestID));
				lpPopupNode->m_lpJobNode = lpJobNode;

				lpPopupNode->m_dwPopupKind		= POPUPMENU_QUEST;
				lpPopupNode->m_strPopupString	= lpJobNode->m_lpQuest->m_strQuestTitle;

				lpPopupNode->m_dwLimitLevel     = static_cast<unsigned long>(lpJobNode->m_lpQuest->m_wMinLevel);
				lpPopupNode->m_dwLimitClass		= lpJobNode->m_lpQuest->m_dwClass;
				lpPopupNode->m_dwLimitQuest		= lpJobNode->m_lpQuest->m_dwCompletedQuest;
			}

			lpNpcNode->m_lstPopup.push_back( lpPopupNode );

			if ( !strlen( strHelpMsg ) )
				return;
			char* tempMsg;
			tempMsg = new char[ strlen( strHelpMsg )+1 ];
			strcpy( tempMsg, strHelpMsg );
			lpNpcNode->m_mapHelpWords.insert( pair< unsigned short, char*>( nQuestID, tempMsg ) ) ;
		}
	}
#else
	static void AddQuestWords( int nUID, int nQuestID, const char* strHelpMsg )
	{
		NPCNode* lpNode = g_NPCList.GetNPCNode(nUID);

		if ( lpNode )
		{
			if ( !strlen( strHelpMsg ) )
				return;
			char* tempMsg;
			tempMsg = new char[ strlen( strHelpMsg )+1 ];
			strcpy( tempMsg, strHelpMsg );
			lpNode->m_mapHelpWords.insert( pair< unsigned short, char*>( nQuestID, tempMsg ) ) ;
		}
	}
#endif

static void SetDropGrade(int nUID, int nDropType, int nGradeF, int nGradeD, int nGradeC, int nGradeB, int nGradeA)
{
	NPCNode* lpNode = g_NPCList.GetNPCNode(nUID);

	if (lpNode)
	{
		lpNode->m_cGradeRate[nDropType][Item::EquipType::F_GRADE] = static_cast<unsigned char>(nGradeF);
		lpNode->m_cGradeRate[nDropType][Item::EquipType::D_GRADE] = static_cast<unsigned char>(nGradeD);
		lpNode->m_cGradeRate[nDropType][Item::EquipType::C_GRADE] = static_cast<unsigned char>(nGradeC);
		lpNode->m_cGradeRate[nDropType][Item::EquipType::B_GRADE] = static_cast<unsigned char>(nGradeB);
		lpNode->m_cGradeRate[nDropType][Item::EquipType::A_GRADE] = static_cast<unsigned char>(nGradeA);
	}
}

static void SetDropBase(int nUID, int nDropType, int nGradeF, int nGradeD, int nGradeC, int nGradeB, int nGradeA)
{
	NPCNode* lpNode = g_NPCList.GetNPCNode(nUID);

	if (lpNode)
	{
		lpNode->m_cBaseNum[nDropType][Item::EquipType::F_GRADE] = static_cast<unsigned char>(nGradeF);
		lpNode->m_cBaseNum[nDropType][Item::EquipType::D_GRADE] = static_cast<unsigned char>(nGradeD);
		lpNode->m_cBaseNum[nDropType][Item::EquipType::C_GRADE] = static_cast<unsigned char>(nGradeC);
		lpNode->m_cBaseNum[nDropType][Item::EquipType::B_GRADE] = static_cast<unsigned char>(nGradeB);
		lpNode->m_cBaseNum[nDropType][Item::EquipType::A_GRADE] = static_cast<unsigned char>(nGradeA);
	}
}

#ifdef __NEWNPCSCRIPT
	static void AddPopup( int nUID, const char* pstrPopupString, int nPopupID )
	{
		NPCNode* lpNpcNode = g_NPCList.GetNPCNode( nUID );

		if( lpNpcNode )
		{
			map<unsigned long, LPPopupNode>::iterator it = g_NPCList.m_lstPopupNode.find(nPopupID);
			if(it != g_NPCList.m_lstPopupNode.end())
			{
				LPPopupNode lpPopupNode = new PopupNode;
				lpPopupNode->m_dwLimitClass = it->second->m_dwLimitClass;
				lpPopupNode->m_dwLimitLevel = it->second->m_dwLimitLevel;
				lpPopupNode->m_dwLimitQuest = it->second->m_dwLimitQuest;
				lpPopupNode->m_dwPopupKind = it->second->m_dwPopupKind;
				lpPopupNode->m_lpJobNode = it->second->m_lpJobNode;
				lpPopupNode->m_strPopupString = pstrPopupString;

				lpNpcNode->m_lstPopup.push_back(lpPopupNode);
			}
		}
	}
#else
	static void AddPopup( int nUID, int nPopupKind, int nCapacity, const char* pstrPopupString, 
						int nQuestID, int nLimitLevel, int nLimitClass )
	{
		NPCNode* lpNpcNode = g_NPCList.GetNPCNode( nUID );

		if( lpNpcNode )
		{
			LPPopupNode lpPopupNode = new PopupNode;

			if( lpPopupNode )
			{
				lpPopupNode->m_dwPopupKind      = static_cast<unsigned long>( nPopupKind );
				lpPopupNode->m_usCapacity       = nCapacity;
				lpPopupNode->m_strPopupString   = pstrPopupString;

				lpPopupNode->m_ulQuestID        = nQuestID;
				lpPopupNode->m_usLimitLevel     = nLimitLevel;
				lpPopupNode->m_ulLimitClass		= nLimitClass;
			}

			lpNpcNode->m_vecPopup.push_back( lpPopupNode );
		}
	}
#endif

static void SetNPCAttribute( int nUID, int nMiniMapIconID, int nNationality )
{
    NPCNode* lpNpcNode = g_NPCList.GetNPCNode( nUID );

    if( lpNpcNode )
    {
        lpNpcNode->m_unMiniMapIconID= nMiniMapIconID;
        lpNpcNode->m_unNationality  = nNationality;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////







#ifdef __NEWNPCSCRIPT
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// struct PageNode
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	PageNode::~PageNode(void)
	{
		vector<LPItemNode>::iterator it;
		LPItemNode lpDelete;
		for(it = m_lstItemNode.begin(); it != m_lstItemNode.end();)
		{
			lpDelete = (*it);
			it = m_lstItemNode.erase(it);
			delete lpDelete;
			lpDelete = NULL;
		}
		m_lstItemNode.clear();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// struct TabNode
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	TabNode::~TabNode(void)
	{
		vector<LPPageNode>::iterator it;
		LPPageNode lpDelete;
		for(it = m_lstPageNode.begin(); it != m_lstPageNode.end();)
		{
			lpDelete = (*it);
			it = m_lstPageNode.erase(it);
			delete lpDelete;
			lpDelete = NULL;
		}
		m_lstPageNode.clear();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// struct ContainerNode
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	ContainerNode::~ContainerNode(void)
	{
		vector<LPTabNode>::iterator it;
		LPTabNode lpDelete;
		for(it = m_lstTabNode.begin(); it != m_lstTabNode.end();)
		{
			lpDelete = (*it);
			it = m_lstTabNode.erase(it);
			delete lpDelete;
			lpDelete = NULL;
		}
		m_lstTabNode.clear();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// struct NPCNode
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	NPCNode::~NPCNode(void)
	{
		vector<LPDialogNode>::iterator itScript;
		LPDialogNode lpDialog;
		for (itScript = m_lstScript.begin(); itScript != m_lstScript.end();)
		{
			lpDialog = (*itScript);
			itScript = m_lstScript.erase(itScript);
			delete[] lpDialog->m_strWord;
			delete lpDialog;
		}
		m_lstScript.clear();

		vector<LPPopupNode>::iterator itPopup;
		LPPopupNode lpPopup;
		for( itPopup = m_lstPopup.begin(); itPopup != m_lstPopup.end(); )
		{
			lpPopup = (*itPopup);
			itPopup = m_lstPopup.erase( itPopup );
			if(lpPopup->m_dwPopupKind != POPUPMENU_QUEST) lpPopup->m_lpJobNode = NULL;		// 퀘스트 이외에는 전부 지워져있다.
			delete lpPopup;
		}
		m_lstPopup.clear();

		map< unsigned short, char*>::iterator itWord = m_mapHelpWords.begin() ;
		while ( itWord != m_mapHelpWords.end() )
		{
			if ( itWord->second )
			{
				delete itWord->second ;
			}
			++ itWord ;
		}
		m_mapHelpWords.clear() ;

		delete[] m_strNpcSkin;
		delete[] m_strNpcName;
	}
#endif

//-- Zergra To. --// 

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
// class CNPCList
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
CNPCList::CNPCList()
{
}

CNPCList::~CNPCList()
{
	DestroyNPCList();
}

#ifdef __NEWNPCSCRIPT
BOOL CNPCList::Load(const char *strNPCScriptFile, const char *strComponentScriptFile, unsigned long dwZone)
#else
BOOL CNPCList::Load(const char *strScriptFile, unsigned long dwZone)
#endif
{
	DestroyNPCList();

	m_dwZone = dwZone;

#ifdef __NEWNPCSCRIPT
	SCRIPT PopupScript = _SE_Create(strComponentScriptFile);
	if (!PopupScript) return FALSE;

	_SE_SetMessageFunction(ScriptErrorMessage);

	_SE_RegisterFunction(PopupScript, ContainerStart, T_VOID, "ContainerStart", T_INT, 0);
	_SE_RegisterFunction(PopupScript, ContainerEnd, T_VOID, "ContainerEnd", 0);
	_SE_RegisterFunction(PopupScript, TabStart, T_VOID, "TabStart", T_STRING, 0);
	_SE_RegisterFunction(PopupScript, TabEnd, T_VOID, "TabEnd", 0);
	_SE_RegisterFunction(PopupScript, PageStart, T_VOID, "PageStart", 0);
	_SE_RegisterFunction(PopupScript, PageEnd, T_VOID, "PageEnd", 0);
	_SE_RegisterFunction(PopupScript, SetNormalMarket, T_VOID, "SetNormalMarket", T_BOOL, 0);
	_SE_RegisterFunction(PopupScript, SetBlackMarket, T_VOID, "SetBlackMarket", T_BOOL, 0);
	_SE_RegisterFunction(PopupScript, SetRandomOptionMarket, T_VOID, "SetRandomOptionMarket", T_BOOL, 0);
	_SE_RegisterFunction(PopupScript, SetSkillCouponMarket, T_VOID, "SetSkillCouponMarket", T_BOOL, 0);
	_SE_RegisterFunction(PopupScript, SetMedalMarket, T_VOID, "SetMedalMarket", T_BOOL, 0);
	_SE_RegisterFunction(PopupScript, SetEnableRepair, T_VOID, "SetEnableRepair", T_BOOL, 0);
	_SE_RegisterFunction(PopupScript, AddItem, T_VOID, "AddItem", T_INT, 0);

	_SE_RegisterFunction(PopupScript, PopupStart, T_VOID, "PopupStart", T_INT, 0);
	_SE_RegisterFunction(PopupScript, PopupEnd, T_VOID, "PopupEnd", 0);
	_SE_RegisterFunction(PopupScript, PopupAttribute, T_VOID, "PopupAttribute", T_INT, T_INT, T_INT, T_INT, 0);
	_SE_RegisterFunction(PopupScript, AddContainer, T_VOID, "AddContainer", T_INT, 0);
	_SE_RegisterFunction(PopupScript, AddZoneMove, T_VOID, "AddZoneMove", T_INT, T_FLOAT, T_FLOAT, T_FLOAT, 0);

	_SE_Execute(PopupScript);

	_SE_Destroy(PopupScript);



	SCRIPT Script = _SE_Create(strNPCScriptFile);
	if (!Script) return FALSE;

	_SE_SetMessageFunction(ScriptErrorMessage);

	_SE_RegisterFunction(Script, SetNpc, T_VOID, "SetNPC", T_INT, T_INT, T_INT, T_BOOL, T_STRING, T_STRING, 0);
	_SE_RegisterFunction(Script, SetPosition, T_VOID, "SetPosition", T_INT, T_FLOAT, T_FLOAT, T_FLOAT, T_FLOAT, 0);
	_SE_RegisterFunction(Script, AddWords, T_VOID, "AddWords", T_INT, T_STRING, 0);
	_SE_RegisterFunction(Script, AddDialog, T_VOID, "AddDialog", T_INT, T_INT, T_INT, T_STRING, 0);
	_SE_RegisterFunction(Script, SetDropGrade, T_VOID, "SetDropGrade", T_INT, T_INT, T_INT, T_INT, T_INT, T_INT, T_INT, 0);
	_SE_RegisterFunction(Script, SetDropBase, T_VOID, "SetDropBase", T_INT, T_INT, T_INT, T_INT, T_INT, T_INT, T_INT, 0);
	_SE_RegisterFunction(Script, AddQuestPopup, T_VOID, "AddQuestPopup", T_INT, T_INT, T_STRING, 0);
    _SE_RegisterFunction(Script, AddPopup, T_VOID, "AddPopup", T_INT, T_STRING, T_INT, 0 );
    _SE_RegisterFunction(Script, SetNPCAttribute, T_VOID, "SetNPCAttribute", T_INT, T_INT, T_INT, 0 );

	_SE_Execute(Script);

	_SE_Destroy(Script);
#else
	SCRIPT Script = _SE_Create(strScriptFile);
	if (!Script) return FALSE;

	_SE_SetMessageFunction(ScriptErrorMessage);

	_SE_RegisterFunction(Script, SetNpc, T_VOID, "SetNPC", T_INT, T_INT, T_INT, T_BOOL, T_STRING, T_STRING, 0);
	_SE_RegisterFunction(Script, SetPosition, T_VOID, "SetPosition", T_INT, T_FLOAT, T_FLOAT, T_FLOAT, T_FLOAT, 0);
	_SE_RegisterFunction(Script, AddWords, T_VOID, "AddWords", T_INT, T_STRING, 0);
	_SE_RegisterFunction(Script, AddDialog, T_VOID, "AddDialog", T_INT, T_INT, T_INT, T_STRING, 0);
	_SE_RegisterFunction(Script, AddItem, T_VOID, "AddItem", T_INT, T_INT, T_INT, T_INT, T_INT, 0);
	_SE_RegisterFunction(Script, AddZoneMove, T_VOID, "AddZoneMove", T_INT, T_INT, T_FLOAT, T_FLOAT, T_FLOAT, 0);
	_SE_RegisterFunction(Script, AddQuest, T_VOID, "AddQuest", T_INT, T_INT, 0);
	_SE_RegisterFunction(Script, SetDropGrade, T_VOID, "SetDropGrade", T_INT, T_INT, T_INT, T_INT, T_INT, T_INT, T_INT, 0);
	_SE_RegisterFunction(Script, SetDropBase, T_VOID, "SetDropBase", T_INT, T_INT, T_INT, T_INT, T_INT, T_INT, T_INT, 0);
	_SE_RegisterFunction(Script, AddQuestWords, T_VOID, "AddQuestWords", T_INT, T_INT, T_STRING, 0);
    _SE_RegisterFunction(Script, AddPopup, T_VOID, "AddPopup", T_INT, T_INT, T_INT, T_STRING, T_INT, T_INT, T_INT, 0 );
    _SE_RegisterFunction(Script, SetNPCAttribute, T_VOID, "SetNPCAttribute", T_INT, T_INT, T_INT, 0 );

	_SE_Execute(Script);

	_SE_Destroy(Script);
#endif

	return TRUE;
}

BOOL CNPCList::IsExistNpc(unsigned long dwUID)
{
	vector<NPCNode*>::iterator it;
	for (it = m_lstNpcNode.begin(); it != m_lstNpcNode.end(); it++)
	{
		if ((*it)->m_dwUID == dwUID) return TRUE;
	}

	return FALSE;
}

void CNPCList::AddNpc(NPCNode* lpNewNode)
{
	m_lstNpcNode.push_back(lpNewNode);
}

NPCNode* CNPCList::GetNPCNode(unsigned long dwUID)
{
	vector<NPCNode*>::iterator it;

	BOOL bMonster = FALSE;
	if (dwUID & Creature::MONSTER_BIT)
	{
		dwUID = dwUID & 0x0FFFFFFF;
		bMonster = TRUE;
	}

	if (TRUE == bMonster)
	{
		for (it = m_lstNpcNode.begin(); it != m_lstNpcNode.end(); it++)
		{
			if (Creature::NPC_LINK_BIT == ( (*it)->m_dwUID & Creature::NPC_LINK_BIT ))
			{
				if (((*it)->m_dwUID & 0x0FFFFFFF) == dwUID) 
				{
					return (*it);
				}
			}
		}
	} 
	else
	{
		for (it = m_lstNpcNode.begin(); it != m_lstNpcNode.end(); it++)
		{
			if ((*it)->m_dwUID == dwUID) 
			{
				return (*it);
			}
		}
	}

	return NULL;
}

#ifdef __NEWNPCSCRIPT
	LPContainerNode CNPCList::GetContainer(unsigned long dwContainerID)
	{
		map<unsigned long, LPContainerNode>::iterator it = g_NPCList.m_lstContainerNode.find(dwContainerID);
		if(it != g_NPCList.m_lstContainerNode.end())
		{
			return it->second;
		}

		return NULL;
	}
#endif

const char *CNPCList::GetGreeting(unsigned long dwUID)
{
	vector<NPCNode*>::iterator it;
	for (it = m_lstNpcNode.begin(); it != m_lstNpcNode.end(); it++)
	{
		if ((*it)->m_dwUID == dwUID)
		{
			return (*it)->GetGreeting();
		}
	}

	return NULL;
}

NPCNode* CNPCList::GetFirstNPCNode()
{
	m_NpcNodeItr = m_lstNpcNode.begin() ;

	if ( 0 != m_lstNpcNode.size() )
	{
		return ( *m_NpcNodeItr ) ;
	}

	return NULL ;
}

NPCNode* CNPCList::GetNextNPCNode()
{
	++m_NpcNodeItr ;
	if ( m_NpcNodeItr != m_lstNpcNode.end() ) return ( *m_NpcNodeItr ) ;

	return NULL;
}

#ifdef __NEWNPCSCRIPT
	void CNPCList::DestroyNPCList()
	{
		map<unsigned long, LPContainerNode>::iterator itContainer = m_lstContainerNode.begin();
		while ( itContainer != m_lstContainerNode.end() )
		{
			if ( itContainer->second )
			{
				delete itContainer->second ;
			}
			++ itContainer ;
		}
		m_lstContainerNode.clear() ;

		map<unsigned long, LPPopupNode>::iterator itPopup = m_lstPopupNode.begin();
		while ( itPopup != m_lstPopupNode.end() )
		{
			if ( itPopup->second )
			{
				delete itPopup->second ;
			}
			++ itPopup ;
		}
		m_lstPopupNode.clear() ;

		vector<NPCNode*>::iterator itNpc;
		NPCNode* lpNPC;
		for (itNpc = m_lstNpcNode.begin(); itNpc != m_lstNpcNode.end();)
		{
			lpNPC = (*itNpc);
			itNpc = m_lstNpcNode.erase(itNpc);
			delete lpNPC;
		}
		m_lstNpcNode.clear();
	}
#else
	void CNPCList::DestroyNPCList()
	{
		vector<NPCNode*>::iterator it;
		vector<LPDialogNode>::iterator itchar;
		vector<ITEMNode*>::iterator itItem;
		vector<WarpNode*>::iterator itWarp;
	    
		vector<LPQuestInstanceNode>::iterator itQuest;

		NPCNode* lpNPC;
		LPDialogNode lpWord;
		ITEMNode* lpItem;
		WarpNode* lpWarp;
		LPQuestInstanceNode lpQuest;

		//-- 2004. 9. 10. Zergra From. --//
		vector<PopupNode*>::iterator iterPopup;
		PopupNode*                   pPopup;
		//-- Zergra To. --//

		for (it = m_lstNpcNode.begin(); it != m_lstNpcNode.end();)
		{
			lpNPC = (*it);
			it = m_lstNpcNode.erase(it);
			for (itchar = lpNPC->m_lstScript.begin(); itchar != lpNPC->m_lstScript.end();)
			{
				lpWord = (*itchar);
				itchar = lpNPC->m_lstScript.erase(itchar);
				delete[] lpWord->m_strWord;
				delete lpWord;
			}
			lpNPC->m_lstScript.clear();

			for (itItem = lpNPC->m_lstItem.begin(); itItem != lpNPC->m_lstItem.end();)
			{
				lpItem = (*itItem);
				itItem = lpNPC->m_lstItem.erase(itItem);
				delete lpItem;
			}
			lpNPC->m_lstItem.clear();

			for (itWarp = lpNPC->m_lstWarpZone.begin(); itWarp != lpNPC->m_lstWarpZone.end();)
			{
				lpWarp = (*itWarp);
				itWarp = lpNPC->m_lstWarpZone.erase(itWarp);
				delete lpWarp;
			}
			lpNPC->m_lstWarpZone.clear();

			for (itQuest = lpNPC->m_lstQuest.begin(); itQuest != lpNPC->m_lstQuest.end();)
			{
				lpQuest = (*itQuest);
				itQuest = lpNPC->m_lstQuest.erase(itQuest);
				delete lpQuest;
			}
			lpNPC->m_lstQuest.clear();


			map< unsigned short, char*>::iterator		iter = lpNPC->m_mapHelpWords.begin() ;
			while ( iter != lpNPC->m_mapHelpWords.end() )
			{
				if ( iter->second )
				{
					delete iter->second ;
				}
				++ iter ;
			}
			lpNPC->m_mapHelpWords.clear() ;

			for( iterPopup = lpNPC->m_vecPopup.begin(); iterPopup != lpNPC->m_vecPopup.end(); )
			{
				pPopup    = (*iterPopup);
				iterPopup = lpNPC->m_vecPopup.erase( iterPopup );
				delete pPopup;
			}
			lpNPC->m_vecPopup.clear();

			//-- 2004. 9. 30. Zergra From. --//
			lpNPC->DeleteEquipItem();
			//-- Zergra To. --// 
	/*		
			for (itQuest = lpNPC->m_lstQuest.begin(); itQuest != lpNPC->m_lstQuest.end();)
			{
				lpQuest = (*itQuest);
				itQuest = lpNPC->m_lstQuest.erase(itQuest);
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

						for (itEvent = lpTrigger->m_lstFalseEvent.begin(); itEvent != lpTrigger->m_lstFalseEvent.end();)
						{
							lpEvent = (*itEvent);
							itEvent = lpTrigger->m_lstFalseEvent.erase(itEvent);
							if (lpEvent->m_strWord) delete[] lpEvent->m_strWord;
							delete lpEvent;
						}
						lpTrigger->m_lstFalseEvent.clear();

						delete lpTrigger;
					}
					lpPhase->m_lstTrigger.clear();

					delete[] lpPhase->m_strPhaseDesc;

					delete lpPhase;
				}
				lpQuest->m_lstPhase.clear();

				if (lpQuest->m_strQuestTitle) delete[] lpQuest->m_strQuestTitle;
				if (lpQuest->m_strQuestDesc) delete[] lpQuest->m_strQuestDesc;
				if (lpQuest->m_strQuestLevel) delete[] lpQuest->m_strQuestLevel;	// QUEST_TODO : Vins
				if (lpQuest->m_strQuestAward) delete[] lpQuest->m_strQuestAward;	// QUEST_TODO : Vins
				
				delete lpQuest;
			}
			lpNPC->m_lstQuest.clear();
	*/

			delete[] lpNPC->m_strNpcSkin;
			delete[] lpNPC->m_strNpcName;

			delete lpNPC;
		}
		m_lstNpcNode.clear();
	}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
// struct NPCNode
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------------------------------------
// NPCNode::AddEquipItemList()
#ifdef __NEWNPCSCRIPT
#else
	VOID NPCNode::AddEquipItemList( int nRace, int nTab, int nCnt, const unsigned short* aryItemID, const unsigned char* aryItemGrade )
	{
		ClearEquipItem( nRace, nTab );
	}
#endif

//--------------------------------------------------------------------------------------------------------------------------
// NPCNode::ChangeGradeOption()
#ifdef __NEWNPCSCRIPT
	VOID NPCNode::ChangeGradeOption(void)
	{
/*		Item::EquipType::Grade		cItemGrade;
		LPItemNode pItemNode;
		Item::CItem* lpItem;
		Item::CEquipment *lpEquip;
		ClearEquipItem();

		for ( vector<LPItemNode>::iterator it = m_lstItem.begin() ; it != m_lstItem.end() ; it ++ )
		{
			pItemNode = (*it);

			lpItem = Item::CItemFactory::GetInstance().CreateItem( pItemNode->m_wKindItem );

			Item::ItemType::Type eItemType = static_cast<Item::ItemType::Type>(lpItem->GetItemInfo().m_DetailData.m_cItemType);
			cItemGrade = GetRandomGrade(eItemType);

			m_mapEquipItemGrade[ pItemNode->m_cMenuIndex * 4 + pItemNode->m_wTab ][ pItemNode->m_cPage ].insert( 
				pair< unsigned short, unsigned char> ( pItemNode->m_wKindItem, cItemGrade ) );

			lpEquip = Item::CEquipment::DowncastToEquipment( lpItem );
			lpEquip->AddRandomOption( cItemGrade, GetBaseNum(eItemType, cItemGrade) );
			lpEquip->SetNewEquip();
			lpEquip->SetNumOrDurability( lpEquip->GetMaxNumOrDurability() );

			m_lstEquipItem[ pItemNode->m_cMenuIndex * 4 + pItemNode->m_wTab ][ pItemNode->m_cPage ].push_back( lpItem );
			m_arytEquipItemNum[ pItemNode->m_cMenuIndex * 4 + pItemNode->m_wTab ][ pItemNode->m_cPage ] += 1; 
		}*/
	}
#else
	VOID NPCNode::ChangeGradeOption(void)
	{
		Item::EquipType::Grade		cItemGrade;
		ITEMNode* pItemNode;
		Item::CItem* lpItem;
		Item::CEquipment *lpEquip;
		ClearEquipItem();

		for ( vector<ITEMNode*>::iterator it = m_lstItem.begin() ; it != m_lstItem.end() ; it ++ )
		{
			pItemNode = (*it);

			lpItem = Item::CItemFactory::GetInstance().CreateItem( pItemNode->m_wKindItem );

			Item::ItemType::Type eItemType = static_cast<Item::ItemType::Type>(lpItem->GetItemInfo().m_DetailData.m_cItemType);
			cItemGrade = GetRandomGrade(eItemType);

			m_mapEquipItemGrade[ pItemNode->m_cMenuIndex * 4 + pItemNode->m_wTab ][ pItemNode->m_cPage ].insert( 
				pair< unsigned short, unsigned char> ( pItemNode->m_wKindItem, cItemGrade ) );

			lpEquip = Item::CEquipment::DowncastToEquipment( lpItem );
			lpEquip->AddRandomOption( cItemGrade, GetBaseNum(eItemType, cItemGrade) );
			lpEquip->SetNewEquip();
			lpEquip->SetNumOrDurability( lpEquip->GetMaxNumOrDurability() );

			m_lstEquipItem[ pItemNode->m_cMenuIndex * 4 + pItemNode->m_wTab ][ pItemNode->m_cPage ].push_back( lpItem );
			m_arytEquipItemNum[ pItemNode->m_cMenuIndex * 4 + pItemNode->m_wTab ][ pItemNode->m_cPage ] += 1; 
		}
	}
#endif

//--------------------------------------------------------------------------------------------------------------------------
// NPCNode::GetRandomGrade()
Item::EquipType::Grade NPCNode::GetRandomGrade(Item::ItemType::Type eItemType)
{
	Item::EquipType::OptionType optionType = 
		Item::CItemType::GetInstance().GetOptionType(static_cast<Item::ItemType::Type>(eItemType));

	int sum = std::accumulate(m_cGradeRate[optionType], m_cGradeRate[optionType] + Item::EquipType::MAX_GRADE, 0);
	if (0 != sum)
	{
		int res = rand() % sum ;

		res -= m_cGradeRate[optionType][Item::EquipType::F_GRADE] ;	if ( res < 0 )	{ return Item::EquipType::F_GRADE;	}
		res -= m_cGradeRate[optionType][Item::EquipType::D_GRADE] ;	if ( res < 0 )	{ return Item::EquipType::D_GRADE;	}
		res -= m_cGradeRate[optionType][Item::EquipType::C_GRADE] ;	if ( res < 0 )	{ return Item::EquipType::C_GRADE;	}	
		res -= m_cGradeRate[optionType][Item::EquipType::B_GRADE] ;	if ( res < 0 )	{ return Item::EquipType::B_GRADE;	}	
		res -= m_cGradeRate[optionType][Item::EquipType::A_GRADE] ;	if ( res < 0 )	{ return Item::EquipType::A_GRADE;	}	
	}

	return Item::EquipType::F_GRADE;		
}

//--------------------------------------------------------------------------------------------------------------------------
// NPCNode::ClearEquipItem()
#ifdef __NEWNPCSCRIPT
#else
	VOID NPCNode::ClearEquipItem(  int nRace, int nTab )
	{
		if ( nTab < 4 && nTab >= 0 )
		{
			vector<Item::CItem*>::iterator it;
			Item::CItem* lpItem;
			for ( int i = 0; i < m_aryPhaseMax[nRace * 4 + nTab] ; i++ )
			{
				for ( it = m_lstEquipItem[nRace * 4 + nTab][i].begin() ; it != m_lstEquipItem[nRace * 4 + nTab][i].end(); ++it )
				{
					lpItem = *it;
					if ( lpItem )
					{
						DELETE_ITEM(lpItem);
						*it = NULL;
					}
	 			}

				m_lstEquipItem[nRace * 4 + nTab][i].clear();
				m_mapEquipItemGrade[nRace * 4 + nTab][i].clear();
				m_arytEquipItemNum[nRace * 4 + nTab][i] = 0;
			}

		}
	}
#endif

//--------------------------------------------------------------------------------------------------------------------------
// NPCNode::ClearTradeWindow()
#ifdef __NEWNPCSCRIPT
#else
	void NPCNode::ClearTradeWindow()
	{
		// 너무 한거 아닌가 싶기도 하지만 그래도 양이 얼마 안되니까 이렇게 돌려도 몇 번 안돈다 -_-;
		// 메모리만 지우고 공간은 남겨둔다.
		for( vector<SingleTradeWindow*>::iterator iterWindow = m_vecTradeWindow.begin() ;
			iterWindow != m_vecTradeWindow.end() ;
			++iterWindow )
		{
			SingleTradeWindow* pWindow = (*iterWindow);

			for( vector<SingleTradeTab*>::iterator iterTab = pWindow->m_vecTradeTab.begin() ;
				iterTab != pWindow->m_vecTradeTab.end() ;
				++iterTab )
			{
				SingleTradeTab* pTab = (*iterTab);

				for( vector<SingleTradePage*>::iterator iterPage = pTab->m_vecTradePage.begin() ;
					iterPage != pTab->m_vecTradePage.end() ;
					++iterPage )
				{
					SingleTradePage* pPage = (*iterPage);

					for( vector<Item::CItem*>::iterator iterItem = pPage->m_vecEquipmentItem.begin() ;
						iterItem != pPage->m_vecEquipmentItem.end() ;
						++iterItem )
					{
						Item::CItem* lpItem = (*iterItem);
						if( lpItem )
						{
							DELETE_ITEM( lpItem );
							(*iterItem) = NULL;
						}
					}

					pPage->m_vecEquipmentItem.clear();
					pPage->m_mapEquipmentItemGrade.clear();
				}
			}
		}
	}
#endif

//--------------------------------------------------------------------------------------------------------------------------
// NPCNode::ChangeEquipItemGradeOption()
#ifndef __NEWNPCSCRIPT
	void NPCNode::ChangeEquipItemGradeOption()
	{
		Item::EquipType::Grade	cItemGrade;
		Item::CItem*            lpItem;
		Item::CEquipment*       pEquip;

		for( vector<SingleTradeWindow*>::iterator iterWindow = m_vecTradeWindow.begin() ;
			iterWindow != m_vecTradeWindow.end() ;
			++iterWindow )
		{
			SingleTradeWindow* pWindow = (*iterWindow);

			for( vector<SingleTradeTab*>::iterator iterTab = pWindow->m_vecTradeTab.begin() ;
				iterTab != pWindow->m_vecTradeTab.end() ;
				++iterTab )
			{
				SingleTradeTab* pTab = (*iterTab);

				for( vector<SingleTradePage*>::iterator iterPage = pTab->m_vecTradePage.begin() ;
					iterPage != pTab->m_vecTradePage.end() ;
					++iterPage )
				{
					SingleTradePage* pPage = (*iterPage);

					for( vector<Item::CItem*>::iterator iterItem = pPage->m_vecEquipmentItem.begin() ;
						iterItem != pPage->m_vecEquipmentItem.end() ;
						++iterItem )
					{
						lpItem = (*iterItem);

						Item::ItemType::Type eItemType = static_cast<Item::ItemType::Type>(lpItem->GetItemInfo().m_DetailData.m_cItemType);
						cItemGrade = GetRandomGrade(eItemType);

						pEquip = Item::CEquipment::DowncastToEquipment( lpItem );
						pEquip->AddRandomOption( cItemGrade, GetBaseNum(eItemType, cItemGrade) );
						pEquip->SetNewEquip();
						pEquip->SetNumOrDurability( pEquip->GetMaxNumOrDurability() );
					}
				}
			}
		}
	}
#endif

//--------------------------------------------------------------------------------------------------------------------------
// NPCNode::SetItemToTradeWindow()
#ifdef __NEWNPCSCRIPT
#else
	VOID NPCNode::SetItemToTradeWindow(void)
	{
		Item::EquipType::Grade	cItemGrade;
		ITEMNode* pItemNode;
		Item::CItem*            lpItem;

		ClearTradeWindow();

		for ( vector<ITEMNode*>::iterator it = m_lstItem.begin() ; it != m_lstItem.end() ; ++it )
		{
			pItemNode = (*it);
			lpItem = Item::CItemFactory::GetInstance().CreateItem( pItemNode->m_wKindItem );

			cItemGrade = Item::EquipType::Grade( 
				GetRandomGrade(static_cast<Item::ItemType::Type>(lpItem->GetItemInfo().m_DetailData.m_cItemType)) );

			SingleTradePage* pTradePage = 
				m_vecTradeWindow[ pItemNode->m_cMenuIndex ]->m_vecTradeTab[ pItemNode->m_wTab ]->m_vecTradePage[ pItemNode->m_cPage ];

			pTradePage->m_mapEquipmentItemGrade.insert( pair< unsigned short, unsigned char>( pItemNode->m_wKindItem, cItemGrade ) );
			pTradePage->m_vecEquipmentItem.push_back( lpItem );
		}
	}
#endif

//--------------------------------------------------------------------------------------------------------------------------
// NPCNode::DeleteEquipItem()
#ifdef __NEWNPCSCRIPT
#else
	void NPCNode::DeleteEquipItem(void)
	{
		// 너무 한거 아닌가 싶기도 하지만 그래도 양이 얼마 안되니까 이렇게 돌려도 몇 번 안돈다 -_-;
		// 메모리만 지우고 공간은 남겨둔다.
		for( vector<SingleTradeWindow*>::iterator iterWindow = m_vecTradeWindow.begin() ;
			iterWindow != m_vecTradeWindow.end() ;
			++iterWindow )
		{
			SingleTradeWindow* pWindow = (*iterWindow);

			for( vector<SingleTradeTab*>::iterator iterTab = pWindow->m_vecTradeTab.begin() ;
				iterTab != pWindow->m_vecTradeTab.end() ;
				++iterTab )
			{
				SingleTradeTab* pTab = (*iterTab);

				for( vector<SingleTradePage*>::iterator iterPage = pTab->m_vecTradePage.begin() ;
					iterPage != pTab->m_vecTradePage.end() ;
					++iterPage )
				{
					SingleTradePage* pPage = (*iterPage);

					for( vector<Item::CItem*>::iterator iterItem = pPage->m_vecEquipmentItem.begin() ;
						iterItem != pPage->m_vecEquipmentItem.end() ;
						++iterItem )
					{
						Item::CItem* lpItem = (*iterItem);
						if( lpItem )
						{
							DELETE_ITEM( lpItem );
							(*iterItem) = NULL;
						}
					}

					pPage->m_vecEquipmentItem.clear();
					pPage->m_mapEquipmentItemGrade.clear();

					delete pPage;
					(*iterPage) = NULL;
				}

				pTab->m_vecTradePage.clear();

				delete pTab;
				(*iterTab) = NULL;
			}

			pWindow->m_vecTradeTab.clear();

			delete pWindow;
			(*iterWindow) = NULL;
		}

		m_vecTradeWindow.clear();
	}
#endif

//--------------------------------------------------------------------------------------------------------------------------
// NPCNode::SearchQuestList()
#ifdef __NEWNPCSCRIPT
	BOOL NPCNode::IsExistQuest(void)
	{
		vector<LPPopupNode>::iterator it;
		for(it = m_lstPopup.begin(); it != m_lstPopup.end(); ++it)
		{
			if((*it)->m_dwPopupKind == POPUPMENU_QUEST)
			{
				return TRUE;
			}
		}

		return FALSE;
	}

	void NPCNode::SearchQuestList(unsigned long dwLevel, unsigned long dwClass, unsigned long dwNation, unsigned short *lstCompleted, unsigned short wNumCompleted, unsigned short *lstQuestList)
	{
		vector<LPPopupNode>::iterator it;
		LPQuestJobNode lpQuestJobNode;
		unsigned long count = 0;
		unsigned long i = 0;

		for (it = m_lstPopup.begin(); it != m_lstPopup.end(); it++, i++)
		{
			if((*it)->m_dwPopupKind == POPUPMENU_QUEST && (*it)->m_lpJobNode)
			{
				lpQuestJobNode = static_cast<LPQuestJobNode>((*it)->m_lpJobNode);
				if(lpQuestJobNode == NULL)
				{
					continue;
				}
			} else
			{
				continue;
			}

			if (lpQuestJobNode->m_lpQuest->m_bInvisible)
			{
				continue;	// 강제 퀘스트는 검사하지 않는다.
			}

			if ( dwLevel > lpQuestJobNode->m_lpQuest->m_wMaxLevel )
			{
				continue;
			}

			if ( dwLevel + 10 <= lpQuestJobNode->m_lpQuest->m_wMinLevel )
			{
				continue;
			}

			// 클래스 검사
			if (!(lpQuestJobNode->m_lpQuest->m_dwClass & (0x00000001 << (dwClass - 1)))) continue;

			// 수행할 수 있는 국적인지 체크
			bool bEnableQuest = false ;
			if ( 0 == lpQuestJobNode->m_lpQuest->m_usNation )
			{
				bEnableQuest = true ;
			}
			else
			{
				unsigned long dwQuestNation = lpQuestJobNode->m_lpQuest->m_usNation ;
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
				continue ;
			}

			BOOL bPass = FALSE;
			// 이미 한 퀘스트인지 검사와 선결 퀘스트를 했는지 검사
			for (unsigned long t = 0; t < wNumCompleted; t++)
			{
				if (lstCompleted[t] == lpQuestJobNode->m_dwQuestID)
				{
					bPass = TRUE;
				}
			}

			for (t = 0; t < 10; t++)
			{
				if (lstQuestList[t] == lpQuestJobNode->m_dwQuestID)
				{
					bPass = TRUE;
				}
			}

			if (bPass) continue;

			if (lpQuestJobNode->m_lpQuest->m_dwCompletedQuest)
			{
				for (unsigned long t = 0; t < wNumCompleted; t++)
				{
					if (lstCompleted[t] == lpQuestJobNode->m_lpQuest->m_dwCompletedQuest)
					{
						bPass = TRUE;
					}
				}

				if (!bPass) continue;
			}

			if (lpQuestJobNode->m_lpQuest->m_wMinLevel <= dwLevel && dwLevel <= lpQuestJobNode->m_lpQuest->m_wMaxLevel)
			{
				lstQuestList[count] = static_cast<unsigned short>(lpQuestJobNode->m_dwQuestID);
				count++;
			}
		}

		lstQuestList[count] = 0xFFFF;
	}
#else
	void NPCNode::SearchQuestList(unsigned long dwLevel, unsigned long dwClass, unsigned long dwNation, unsigned short *lstCompleted, unsigned short wNumCompleted, unsigned short *lstQuestList)
	{
		vector<LPQuestInstanceNode>::iterator it;
		unsigned long count = 0;
		unsigned long i = 0;

		for (it = m_lstQuest.begin(); it != m_lstQuest.end(); it++, i++)
		{
			// 수행할 수 있는 국적인지 체크
			bool bEnableQuest = false ;
			if ( 0 == (*it)->m_lpQuest->m_usNation )
			{
				bEnableQuest = true ;
			}
			else
			{
				unsigned long dwQuestNation = (*it)->m_lpQuest->m_usNation ;
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
				continue ;
			}

			// 클래스 검사
			if (!((*it)->m_lpQuest->m_dwClass & (0x00000001 << (dwClass - 1)))) continue;

			BOOL bPass = FALSE;
			// 이미 한 퀘스트인지 검사와 선결 퀘스트를 했는지 검사
			for (unsigned long t = 0; t < wNumCompleted; t++)
			{
				if (lstCompleted[t] == (*it)->m_wQuestID)
				{
					bPass = TRUE;
				}
			}

			for (t = 0; t < 10; t++)
			{
				if (lstQuestList[t] == (*it)->m_wQuestID)
				{
					bPass = TRUE;
				}
			}

			if (bPass) continue;

			if ((*it)->m_lpQuest->m_dwCompletedQuest)
			{
				for (unsigned long t = 0; t < wNumCompleted; t++)
				{
					if (lstCompleted[t] == (*it)->m_lpQuest->m_dwCompletedQuest)
					{
						bPass = TRUE;
					}
				}

				if (!bPass) continue;
			}

			if ( dwLevel > (*it)->m_lpQuest->m_wMaxLevel )
			{
				continue;
			}

			if ( dwLevel + 10 <= (*it)->m_lpQuest->m_wMinLevel )
			{
				continue;
			}

			if ((*it)->m_lpQuest->m_wMinLevel <= dwLevel && dwLevel <= (*it)->m_lpQuest->m_wMaxLevel)
			{
				lstQuestList[count] = (*it)->m_wQuestID;
				count++;
			}
		}

		lstQuestList[count] = 0xFFFF;
	}
#endif

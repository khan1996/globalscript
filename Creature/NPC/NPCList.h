// NPCList.h: interface for the CNPCList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPCLIST_H__E20011A2_96E4_44B4_AB08_826651FF4122__INCLUDED_)
#define AFX_NPCLIST_H__E20011A2_96E4_44B4_AB08_826651FF4122__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <vector>
#include <map>
#include <string>
#include <list>

#include <Quest/QuestList.h>
#include <Item/ItemConstants.h>

using namespace std;

namespace Item
{
	class CItem;
};

#ifdef __NEWNPCSCRIPT
	#include "NPCBaseDefine.h"
#endif

#ifdef __NEWNPCSCRIPT
	typedef struct ItemNode
	{
		unsigned short 	m_wKindItem;

		unsigned short 	m_wSkill;
		unsigned short 	m_wSkillLevel;

//		unsigned char	m_cMenuIndex;
//		unsigned short	m_wTab;
//		unsigned char	m_cPage;
	} ItemNode, *LPItemNode;
#else
	typedef struct ITEMNode
	{
		unsigned short 	m_wKindItem;

		unsigned short 	m_wSkill;
		unsigned short 	m_wSkillLevel;

		unsigned char	m_cMenuIndex;
		unsigned short	m_wTab;
		unsigned char	m_cPage;
	} ITEMNode, *LPITEMNode;
#endif

typedef struct DialogNode
{
	unsigned short m_wKindWord;
	unsigned short m_wDialogFlag;
	unsigned long m_dwFlag;
	char *m_strWord;
} DialogNode, *LPDialogNode;

#ifdef __NEWNPCSCRIPT
	typedef struct PageNode					// 컨테이너 페이지 구조체
	{
		vector<LPItemNode> m_lstItemNode;

		~PageNode(void);

		inline LPItemNode GetITEMNode(unsigned short wKindItem);
	} PageNode, *LPPageNode;

	LPItemNode PageNode::GetITEMNode(unsigned short wKindItem)
	{
		vector<LPItemNode>::iterator it;
		for (it = m_lstItemNode.begin(); it != m_lstItemNode.end(); it++)
		{
			if ((*it)->m_wKindItem == wKindItem) 
			{
				return (*it);
			}
		}

		return NULL;
	}

	typedef struct TabNode					// 컨테이너 탭 구조체
	{
		string m_strTitle;

		unsigned long m_dwNumPage;
		vector<LPPageNode> m_lstPageNode;

		TabNode(void) : m_dwNumPage(0)
		{
			m_strTitle.clear();
		}
		~TabNode(void);
	} TabNode, *LPTabNode;

	typedef struct ContainerNode
	{
		BOOL m_bBlackMarket;
		BOOL m_bMedalMarket;
		BOOL m_bRandomOptionMarket;
		BOOL m_bSkillCouponMarket;
		BOOL m_bNormalMarket;
		BOOL m_bEnableRepair;

		unsigned long m_dwNumTab;

		vector<LPTabNode> m_lstTabNode;
		ContainerNode(void) : m_bMedalMarket(FALSE), m_bBlackMarket(FALSE), m_bRandomOptionMarket(FALSE),
			m_bSkillCouponMarket(FALSE), m_bNormalMarket(TRUE), m_dwNumTab(0)
		{
		}
		~ContainerNode(void);

		inline LPItemNode GetITEMNode(unsigned short wKindItem);
	} ContainerNode, *LPContainerNode;

	LPItemNode ContainerNode::GetITEMNode(unsigned short wKindItem)
	{
		vector<LPItemNode>::iterator itItem;
		vector<LPPageNode>::iterator itPage;
		vector<LPTabNode>::iterator itTab;
		for (itTab = m_lstTabNode.begin(); itTab != m_lstTabNode.end(); itTab++)
		{
			for (itPage = (*itTab)->m_lstPageNode.begin(); itPage != (*itTab)->m_lstPageNode.end(); itPage++)
			{
				for (itItem = (*itPage)->m_lstItemNode.begin(); itItem != (*itPage)->m_lstItemNode.end(); itItem++)
				{
					if ((*itItem)->m_wKindItem == wKindItem) 
					{
						return (*itItem);
					}
				}
			}
		}

		return NULL;
	}

	typedef struct JobNode
	{
	} JobNode, *LPJobNode;

	typedef struct QuestJobNode : public JobNode
	{
		unsigned long m_dwQuestID;
		LPQuestNode m_lpQuest;	// 링크용

		QuestJobNode() : m_dwQuestID(0), m_lpQuest(NULL)
		{
		}
	} QuestJobNode, *LPQuestJobNode;

	typedef struct WarpJobNode : public JobNode
	{
		unsigned long m_dwZoneNumber;
		float m_fPosX;
		float m_fPosY;
		float m_fPosZ;

		WarpJobNode() : m_dwZoneNumber(0), m_fPosX(0.0f), m_fPosY(0.0f), m_fPosZ(0.0f)
		{
		}
	} WarpJobNode, *LPWarpJobNode;

	typedef struct TradeJobNode : public JobNode
	{
		unsigned long m_dwContainerID;

		LPContainerNode m_lpContainer;	// 링크용

		TradeJobNode() : m_dwContainerID(0), m_lpContainer(NULL)
		{
		}
	} TradeJobNode, *LPTradeJobNode;

	typedef struct PopupNode
	{
//		unsigned long   m_dwPopupID;		// 팝업 메뉴의 ID
		unsigned long	m_dwPopupKind;		// 팝업 메뉴의 종류
		string          m_strPopupString;	// 팝업 메뉴의 이름

		unsigned long	m_dwLimitLevel;		// 제한 레벨
		unsigned long	m_dwLimitClass;		// 제한 클래스
		unsigned long	m_dwLimitQuest;		// 제한 선행 퀘스트

		LPJobNode m_lpJobNode;

		PopupNode(void) : m_dwPopupKind(0), m_dwLimitLevel(0), m_dwLimitClass(0), m_lpJobNode(NULL)
		{
			m_strPopupString.clear();
		}
		~PopupNode(void)
		{
			if(m_lpJobNode) { delete m_lpJobNode; m_lpJobNode = NULL; }
		}
	} PopupNode, *LPPopupNode;

	struct NPCNode
	{
		unsigned long 	m_dwUID;
		unsigned long 	m_dwTownOrNameID;		// 마을의 NPC 는 마을 ID, 성에 속한 NPC 는 성의 이름 ID
		unsigned int    m_unMiniMapIconID;
		unsigned int    m_unNationality;
		bool			m_bBelongToCastle;

		float 			m_fDirection;
		float 			m_fPosX, m_fPosY, m_fPosZ;

		char*			m_strNpcSkin;
		char*			m_strNpcName;

		unsigned char	m_cGradeRate[Item::EquipType::MAX_OPTION_TYPE][Item::EquipType::MAX_GRADE];
		unsigned char	m_cBaseNum[Item::EquipType::MAX_OPTION_TYPE][Item::EquipType::MAX_GRADE];

		vector<LPDialogNode>	m_lstScript;
		vector<LPPopupNode>		m_lstPopup;
		map< unsigned short, char*>	m_mapHelpWords;

		// 아이템( 상점 관련.. )
//		vector<SingleTradeWindow*>          m_vecTradeWindow;               // 상점창 벡터.

//		void    ClearTradeWindow();
//		void    ChangeEquipItemGradeOption();
//		void    DeleteEquipItem();

//		unsigned char						m_aryPhaseMax[8];				// 패이즈 맥스치						[종족 * 4 + 탭]
//		vector<Item::CItem*>				m_lstEquipItem[8][10];			// 랜덤 아이템 ( 현재 생성된 것 )		[종족 * 4 + 탭][ 패이즈 ]
//		map< unsigned short, unsigned char>	m_mapEquipItemGrade[8][10];		// 아이템 아이디, 그레이드				[종족 * 4 + 탭][ 패이즈 ]
//		unsigned char						m_arytEquipItemNum[8][10];		//										[종족 * 4 + 탭][ 패이즈 ]
		unsigned long						m_dwTime;						// 아이템 변환 시간. 이 시간에 아이템 리스트를 받아 왔음.
//		bool								m_bInitGradeOption;				// 로컬에서 아이템 옵션을 변경한지 여부

		NPCNode(void) : m_dwUID(0), m_dwTownOrNameID(0), m_unMiniMapIconID(6), m_unNationality(0), m_bBelongToCastle(false)
		{
			// 미니맵 아이콘 디폴트 값은 Default Icon
			// 국적 디폴트 값은 중립

			for (int i = 0; i < Item::EquipType::MAX_OPTION_TYPE; ++i)
			{
				for (int j = 0; j < Item::EquipType::MAX_GRADE; ++j)
				{
					m_cGradeRate[i][j] = 0;
					m_cBaseNum[i][j] = 0;
				}
			}
		}
		~NPCNode(void);

//		VOID	AddEquipItemList( int nRace, int nTab, int nCnt, const unsigned short* aryItemID, const unsigned char* aryItemGrade );

		Item::EquipType::Grade	GetRandomGrade(Item::ItemType::Type eItemType);
		inline unsigned char	GetBaseNum(Item::ItemType::Type eItemType, Item::EquipType::Grade eGrade); 

		VOID	ChangeGradeOption(void);
//		VOID    SetItemToTradeWindow(void);
//		VOID	ClearEquipItem( int nRace, int nTab );

//		inline void ClearEquipItem(void);
		inline bool SetTime( unsigned long dwTime );

//		inline LPItemNode				GetITEMNode(unsigned short wKindItem);
//		inline Item::EquipType::Grade	GetItemGradeforID( int nTab, unsigned short wProtoID );
		inline const char*				GetHelpWordsForQuestID( unsigned short wQuestID );
		inline DialogNode*				GetScript(unsigned short wPage);
//		inline WarpNode*				GetWarpNode(unsigned short wNumber);
		inline QuestNode*				GetQuestforID(unsigned short wQuestID);
//		inline QuestNode*				GetQuestforNumber(unsigned short wNumber);
		inline LPPopupNode				GetPopupNode(unsigned long dwPopupKind);
		inline const char*				GetGreeting(void);
//		inline bool						GetHasItem(unsigned char cRace);

		// 퀘스트 관련
		BOOL IsExistQuest(void);
		void SearchQuestList(unsigned long m_dwLevel, unsigned long m_dwClass, unsigned long dwNation, 
			unsigned short *m_lstCompleted, unsigned short m_wNumCompleted, unsigned short *m_lstQuestList);
	};

	bool NPCNode::SetTime( unsigned long dwTime )
	{
		if ( dwTime == m_dwTime )
		{
			return true;
		}

		m_dwTime = dwTime;
		return false;
	}

	const char* NPCNode::GetHelpWordsForQuestID( unsigned short wQuestID )
	{
		map< unsigned short, char*>::iterator iter = m_mapHelpWords.find( wQuestID ) ;
		if ( iter != m_mapHelpWords.end() )
		{
			return iter->second;
		}

		return NULL;
	}

	DialogNode* NPCNode::GetScript(unsigned short wPage)
	{
		vector<DialogNode*>::iterator it;
		for (it = m_lstScript.begin(); it != m_lstScript.end(); it++)
		{
			if ((*it)->m_wKindWord == wPage) return (*it);
		}

		return NULL;
	}

	LPPopupNode NPCNode::GetPopupNode(unsigned long dwPopupKind)
	{
		vector<LPPopupNode>::iterator it;
		for (it = m_lstPopup.begin(); it != m_lstPopup.end(); it++)
		{
			if ((*it)->m_dwPopupKind == dwPopupKind) return (*it);
		}

		return NULL;
	}

	unsigned char NPCNode::GetBaseNum(Item::ItemType::Type eItemType, Item::EquipType::Grade eGrade) 
	{ 
		return m_cBaseNum[Item::CItemType::GetInstance().GetOptionType(eItemType)][eGrade]; 
	}

	const char* NPCNode::GetGreeting(void)	// 인사말
	{
		vector<LPDialogNode>::iterator it;
		for (it = m_lstScript.begin(); it != m_lstScript.end(); it++)
		{
			if ((*it)->m_wKindWord == 0) return (*it)->m_strWord;
		}

		return NULL;
	}

	QuestNode* NPCNode::GetQuestforID(unsigned short wQuestID)
	{
		vector<LPPopupNode>::iterator itPopup;
		for(itPopup = m_lstPopup.begin(); itPopup != m_lstPopup.end(); ++itPopup)
		{
			if((*itPopup)->m_dwPopupKind == POPUPMENU_QUEST)
			{
				LPQuestJobNode lpQuestNode = static_cast<LPQuestJobNode>((*itPopup)->m_lpJobNode);
				if(lpQuestNode)
				{
					if(lpQuestNode->m_dwQuestID == static_cast<unsigned long>(wQuestID)) return lpQuestNode->m_lpQuest;
				}
			}
		}

		return NULL;
	}
#else
// 상점창에서 Page 하나..
	typedef struct SingleTradePage
	{
		vector<Item::CItem*>                m_vecEquipmentItem;
		map<unsigned short, unsigned char>  m_mapEquipmentItemGrade;
		bool                                m_bSpaceStatus[12][8];         // 각 상점 페이지의 슬롯( 공간? ) 정보
	} SingleTradePage, *LPSingleTradePage;

	// 상점창에서 탭 하나..
	typedef struct SingleTradeTab
	{
		vector<SingleTradePage*>            m_vecTradePage;
	} SingleTradeTab, *LPSingleTradeTab;

	// 상점창 하나..
	typedef struct SingleTradeWindow
	{
		vector<SingleTradeTab*>              m_vecTradeTab;
	} SingleTradeWindow, *LPSingleTradeWindow;

	typedef struct PopupNode
	{
		unsigned long   m_dwPopupKind;		// 팝업 메뉴의 종류
		unsigned short  m_usCapacity;		// 탭 갯수 (상점창의 경우)
		string          m_strPopupString;	// 팝업 메뉴의 이름

		unsigned long   m_ulQuestID;		// 선행해야 하는 퀘스트
		unsigned short  m_usLimitLevel;		// 제한 레벨
		unsigned long	m_ulLimitClass;		// 제한 클래스

		PopupNode()
		{
			m_dwPopupKind   = 0;
			m_usCapacity    = 0;
			m_strPopupString.clear();

			m_ulQuestID     = 0;
			m_usLimitLevel  = 0;
			m_ulLimitClass	= 0;
		}

		PopupNode( const PopupNode& refNode )
		{
			m_dwPopupKind   = refNode.m_dwPopupKind;
			m_strPopupString= refNode.m_strPopupString;
		}
	} PopupNode, *LPPopupNode;

	typedef struct WarpNode
	{
		unsigned short m_wNumber;
		unsigned short m_wZoneNumber;
		float m_fPosX;
		float m_fPosY;
		float m_fPosZ;
	} WarpNode, *LPWarpNode;

	struct NPCNode
	{
		unsigned long 	m_dwUID;
		unsigned long 	m_dwJob;
		unsigned long 	m_dwTownOrNameID;		// 마을의 NPC 는 마을 ID, 성에 속한 NPC 는 성의 이름 ID
		unsigned int    m_unMiniMapIconID;
		unsigned int    m_unNationality;
		bool			m_bBelongToCastle;

		float 			m_fDirection;
		float 			m_fPosX, m_fPosY, m_fPosZ;

		char*			m_strNpcSkin;
		char*			m_strNpcName;

		unsigned char	m_cGradeRate[Item::EquipType::MAX_OPTION_TYPE][Item::EquipType::MAX_GRADE];
		unsigned char	m_cBaseNum[Item::EquipType::MAX_OPTION_TYPE][Item::EquipType::MAX_GRADE];

		vector<ITEMNode *>			m_lstItem;
		vector<LPDialogNode>		m_lstScript;
		vector<LPWarpNode>			m_lstWarpZone;
		vector<LPQuestInstanceNode> m_lstQuest;
		vector<PopupNode*>          m_vecPopup;
		map< unsigned short, char*>	m_mapHelpWords;

		// 아이템( 상점 관련.. )
		vector<SingleTradeWindow*>          m_vecTradeWindow;               // 상점창 벡터.

		void    ClearTradeWindow();
		void    ChangeEquipItemGradeOption();
		void    DeleteEquipItem();

		unsigned char						m_aryPhaseMax[8];				// 패이즈 맥스치						[종족 * 4 + 탭]
		vector<Item::CItem*>				m_lstEquipItem[8][10];			// 랜덤 아이템 ( 현재 생성된 것 )		[종족 * 4 + 탭][ 패이즈 ]
		map< unsigned short, unsigned char>	m_mapEquipItemGrade[8][10];		// 아이템 아이디, 그레이드				[종족 * 4 + 탭][ 패이즈 ]
		unsigned char						m_arytEquipItemNum[8][10];		//										[종족 * 4 + 탭][ 패이즈 ]
		unsigned long						m_dwTime;						// 아이템 변환 시간. 이 시간에 아이템 리스트를 받아 왔음.
		bool								m_bInitGradeOption;				// 로컬에서 아이템 옵션을 변경한지 여부

		NPCNode()
		{
			m_dwUID 			= 0;
			m_dwJob 			= 0;
			m_dwTownOrNameID	= 0;

			m_unMiniMapIconID	= 6;	// 미니맵 아이콘 디폴트 값은 Default Icon
			m_unNationality		= 0;    // 국적 디폴트 값은 중립

			m_bBelongToCastle	= false;

			for (int i = 0; i < Item::EquipType::MAX_OPTION_TYPE; ++i)
			{
				for (int j = 0; j < Item::EquipType::MAX_GRADE; ++j)
				{
					m_cGradeRate[i][j] = 0;
					m_cBaseNum[i][j] = 0;
				}
			}
		}

		VOID	AddEquipItemList( int nRace, int nTab, int nCnt, const unsigned short* aryItemID, const unsigned char* aryItemGrade );

		Item::EquipType::Grade	GetRandomGrade(Item::ItemType::Type eItemType);
		inline unsigned char	GetBaseNum(Item::ItemType::Type eItemType, Item::EquipType::Grade eGrade); 

		VOID	ChangeGradeOption(void);
		VOID    SetItemToTradeWindow(void);
		VOID	ClearEquipItem( int nRace, int nTab );

		inline void ClearEquipItem(void);
		inline bool SetTime( unsigned long dwTime );

		inline ITEMNode*				GetITEMNode(unsigned short wKindItem);
		inline Item::EquipType::Grade	GetItemGradeforID( int nTab, unsigned short wProtoID );
		inline const char*				GetHelpWordsForQuestID( unsigned short wQuestID );
		inline DialogNode*				GetScript(unsigned short wPage);
		inline WarpNode*				GetWarpNode(unsigned short wNumber);
		inline QuestNode*				GetQuestforID(unsigned short wQuestID);
		inline QuestNode*				GetQuestforNumber(unsigned short wNumber);
		inline PopupNode*				GetPopupNode(unsigned long dwPopupKind);
		inline const char*				GetGreeting(void);
		inline bool						GetHasItem(unsigned char cRace);

		// 퀘스트 관련
		void SearchQuestList(unsigned long m_dwLevel, unsigned long m_dwClass, unsigned long dwNation, 
			unsigned short *m_lstCompleted, unsigned short m_wNumCompleted, unsigned short *m_lstQuestList);
	};

	unsigned char NPCNode::GetBaseNum(Item::ItemType::Type eItemType, Item::EquipType::Grade eGrade) 
	{ 
		return m_cBaseNum[Item::CItemType::GetInstance().GetOptionType(eItemType)][eGrade]; 
	}

	void NPCNode::ClearEquipItem(void)
	{
		for ( int j = 0 ; j < 2 ; j++ )
		{
			for ( int i = 0 ; i < 4; i++ )
			{
				ClearEquipItem( j, i );
			}
		}
	}   

	ITEMNode* NPCNode::GetITEMNode(unsigned short wKindItem)
	{
		vector<ITEMNode *>::iterator it;
		for (it = m_lstItem.begin(); it != m_lstItem.end(); it++)
		{
			if ((*it)->m_wKindItem == wKindItem) 

			{
				return (*it);
			}
		}

		return NULL;
	}

	bool NPCNode::SetTime( unsigned long dwTime )
	{
		if ( dwTime == m_dwTime )
		{
			return true;
		}

		m_dwTime = dwTime;
		return false;
	}

	Item::EquipType::Grade NPCNode::GetItemGradeforID( int nTab, unsigned short wProtoID )
	{
		if ( nTab < 0 || nTab >= 4 )
		{
			return Item::EquipType::F_GRADE;
		}

		for ( int i = 0 ; i < m_aryPhaseMax[nTab]; i++ )
		{
			map< unsigned short, unsigned char >::iterator iter = m_mapEquipItemGrade[nTab][i].find( wProtoID ) ;
			if ( iter != m_mapEquipItemGrade[nTab][i].end() )
			{
				return static_cast<Item::EquipType::Grade>(iter->second);
			}
		}

		return Item::EquipType::F_GRADE;
	}

	const char* NPCNode::GetHelpWordsForQuestID( unsigned short wQuestID )
	{
		map< unsigned short, char*>::iterator iter = m_mapHelpWords.find( wQuestID ) ;
		if ( iter != m_mapHelpWords.end() )
		{
			return iter->second;
		}

		return NULL;
	}

	DialogNode* NPCNode::GetScript(unsigned short wPage)
	{
		vector<DialogNode*>::iterator it;
		for (it = m_lstScript.begin(); it != m_lstScript.end(); it++)
		{
			if ((*it)->m_wKindWord == wPage) return (*it);
		}

		return NULL;
	}

	WarpNode* NPCNode::GetWarpNode(unsigned short wNumber)
	{
		vector<WarpNode*>::iterator it;
		for (it = m_lstWarpZone.begin(); it != m_lstWarpZone.end(); it++)
		{
			if ((*it)->m_wNumber == wNumber) return (*it);
		}

		return NULL;
	}

	QuestNode* NPCNode::GetQuestforID(unsigned short wQuestID)
	{
		vector<QuestInstanceNode*>::iterator it;
		for (it = m_lstQuest.begin(); it != m_lstQuest.end(); it++)
		{
			if ((*it)->m_wQuestID == wQuestID) return (*it)->m_lpQuest;
		}

		return NULL;
	}

	QuestNode* NPCNode::GetQuestforNumber(unsigned short wNumber)
	{
		if (m_lstQuest.size() > wNumber)
		{
			return m_lstQuest[wNumber]->m_lpQuest;
		}

		return NULL;
	}

	PopupNode* NPCNode::GetPopupNode(unsigned long dwPopupKind)
	{
		vector<PopupNode*>::iterator it;
		for (it = m_vecPopup.begin(); it != m_vecPopup.end(); it++)
		{
			if ((*it)->m_dwPopupKind == dwPopupKind) return (*it);
		}

		return NULL;
	}

	const char* NPCNode::GetGreeting(void)	// 인사말
	{
		vector<LPDialogNode>::iterator it;
		for (it = m_lstScript.begin(); it != m_lstScript.end(); it++)
		{
			if ((*it)->m_wKindWord == 0) return (*it)->m_strWord;
		}

		return NULL;
	}

	bool NPCNode::GetHasItem(unsigned char cRace)
	{
		vector<ITEMNode *>::iterator it;
		for (it = m_lstItem.begin(); it != m_lstItem.end(); it++)
		{
			if ((*it)->m_cMenuIndex == cRace) return true;
		}

		return false;
	}
#endif


#ifdef __NEWNPCSCRIPT
	class CNPCList  
	{
		protected:
			vector<NPCNode*> m_lstNpcNode;

			unsigned long m_dwZone;

			vector<NPCNode*>::iterator m_NpcNodeItr;

		public:
			map<unsigned long, LPPopupNode> m_lstPopupNode;
			map<unsigned long, LPContainerNode> m_lstContainerNode;

			LPContainerNode m_lpLoadContainer;
			LPTabNode m_lpLoadTab;
			LPPageNode m_lpLoadPage;
			LPPopupNode m_lpLoadPopup;

			const char *GetGreeting(unsigned long dwUID);
			void DestroyNPCList(void);
			LPItemNode GetITEMNode(unsigned short wKindItem);
			NPCNode* GetNPCNode(unsigned long dwUID);
			NPCNode* GetNPCNodeByOrder(unsigned long dwOrder) { return m_lstNpcNode[dwOrder]; }
			void AddNpc(NPCNode* lpNewNode);
			BOOL IsExistNpc(unsigned long dwUID);
			CNPCList();
			~CNPCList();

			BOOL Load(const char *strNPCScriptFile, const char *strComponentScriptFile, unsigned long dwZone = 1);
			unsigned long GetNpcNum(void) { return m_lstNpcNode.size(); }
			unsigned long GetZone(void) { return m_dwZone; }

			NPCNode* GetFirstNPCNode();
			NPCNode* GetNextNPCNode();

			LPContainerNode GetContainer(unsigned long dwContainerID);
	};
#else
	class CNPCList  
	{
		protected:
			vector<NPCNode*> m_lstNpcNode;
			unsigned long m_dwZone;

			vector<NPCNode*>::iterator m_NpcNodeItr;


		public:

			const char *GetGreeting(unsigned long dwUID);
			void DestroyNPCList(void);
			ITEMNode* GetITEMNode(unsigned short wKindItem);
			NPCNode* GetNPCNode(unsigned long dwUID);
			NPCNode* GetNPCNodeByOrder(unsigned long dwOrder) { return m_lstNpcNode[dwOrder]; }
			void AddNpc(NPCNode* lpNewNode);
			BOOL IsExistNpc(unsigned long dwUID);
			CNPCList();
			~CNPCList();

			BOOL Load(const char *strScriptFile, unsigned long dwZone = 1);
			unsigned long GetNpcNum(void) { return m_lstNpcNode.size(); }
			unsigned long GetZone(void) { return m_dwZone; }

			NPCNode* GetFirstNPCNode();
			NPCNode* GetNextNPCNode();
	};
#endif

extern CNPCList g_NPCList;

#endif // !defined(AFX_NPCLIST_H__E20011A2_96E4_44B4_AB08_826651FF4122__INCLUDED_)

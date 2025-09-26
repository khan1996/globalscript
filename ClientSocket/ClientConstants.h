#ifndef _GAMA_CLIENT_SOCKET_CONSTANTS_H_
#define _GAMA_CLIENT_SOCKET_CONSTANTS_H_


const unsigned short	NoError					= 0x0000;				// 에러 없음
const unsigned short	WrongParameter			= 0x4001;				// 잘못된 인자
const unsigned short	ExceptionError			= 0x4002;				// 예외 값 발생


namespace ClientConstants
{
    enum Hand		
    {
        Hand_Weapon1	    = 1, 
        Hand_Weapon2	    = 2 
    };	 

    enum FameInfo	
    { 
        FameInfo_Fame		= 1, 
        FameInfo_Mileage	= 2	
    };

    enum Require	
    {
        Require_HPInfo	    = 0,
        Require_UDPInfo	    = 1, 
        Require_Shape	    = 2
    };

    enum BindPos	
    {
        Bind_SavePos	    = 0,
        Bind_Warp			= 1,
        Bind_Recall	= 2	
    };

    enum ChatType		
    {
        ChatType_Normal		= 0,	
        ChatType_Party		= 1,
        ChatType_Friend		= 2,	
        ChatType_Guild	    = 3,    
        ChatType_ClientLog	= 5,	
        ChatType_Stall		= 6,    
        ChatType_Shout      = 7,
		ChatType_FindParty	= 14,
		ChatType_Dice		= 16,
		ChatType_GMReport	= 17,
        ChatType_Notify		= 255	
    };

    enum CellOper		
    {	
        CellOper_Move	    = 0,	
        CellOper_Login      = 1,
        CellOper_Logout	    = 1,
        CellOper_Respawn	= 2
    };

    enum PartyCmd		
    {
        PartyCmd_Invite		    = 0,
        PartyCmd_Accept		    = 1,
        PartyCmd_Refuse		    = 2,
        PartyCmd_Banish 	    = 3,
        PartyCmd_Secession	    = 4,
        PartyCmd_Transfer	    = 5,
        PartyCmd_Destroy	    = 6,
        PartyCmd_Login		    = 7,
        PartyCmd_Logout		    = 8,
        PartyCmd_Delete		    = 9,
        PartyCmd_ReLogin	    = 10,
        PartyCmd_FindParty	    = 11,
        PartyCmd_FindMember	    = 12,	
        PartyCmd_AutoRoutingOn	= 13,
        PartyCmd_AutoRoutingOff	= 14
    };

    enum StateType	
    {	
        StateType_STR	= 1,	
        StateType_DEX	= 2,
        StateType_CON	= 3,	
        StateType_INT	= 4,
        StateType_WIS	= 5
    };

    enum Judge		
    {
        Judge_Front				= 0,	Judge_Side			= 1,	Judge_Back			= 2,	
        Judge_Guard				= 3,	Judge_Critical		= 4,	Judge_Heal			= 5,	
        Judge_ManaHeal			= 6,	Judge_Chant			= 7,	Judge_Enchant		= 8,	
        Judge_Disenchant		= 9,	Judge_Evade			= 10,	Judge_Resist		= 11,	
        Judge_ManaShell			= 12,	Judge_Poisoned		= 13,	Judge_NonShow		= 14, 
		Judge_Miss				= 15,	Judge_Counter		= 16,	Judge_FireCracker	= 17, 
		Judge_ChinaFireCracker	= 18,	Judge_SkillItem		= 19,   Judge_Fired			= 20,
		Judge_Social			= 21,	Judge_Gather		= 22
    };

    enum ExchangeCmd	
    {
        ExchangeCmd_Propose	= 0,	ExchangeCmd_Accept	= 1,	ExchangeCmd_Refuse		= 2,
        ExchangeCmd_OK		= 3,	ExchangeCmd_Cancel	= 4,	ExchangeCmd_Exchange	= 5,
        ExchangeCmd_Quit	= 6,	ExchangeCmd_Lock	= 7,	ExchangeCmd_UnLock		= 8
    };

    enum DuelCmd	
    {
        DuelCmd_Propose		= 0,	DuelCmd_Ready	= 1,			DuelCmd_Refuse			= 2,	
        DuelCmd_Logout		= 3,	DuelCmd_Dead	= 4,			DuelCmd_PartyPropose	= 5,
        DuelCmd_PartyReady	= 6,	DuelCmd_RequestPartyInfo = 7,	DuelCmd_Cancel			= 8
    };

    enum AdminCmd 
    {	
        MoveZoneChar	= 1,
        MoveZoneParty	= 2,
        MoveChar		= 3,	
        MoveParty	    = 4,
        RespawnChar		= 5,	
        RespawnParty	= 6,	
        KillChar		= 7,	
        KillParty	    = 8,
        DuelInit		= 9,	
        CreateItem		= 10,	
        CreateMon		= 11,	
        InfoChar	    = 12,
        GetExp			= 13,	
        GetGold			= 14,	
        LevelUp			= 15,	
        PeaceMode	    = 16,
        WarMode			= 17,	
        GetFame			= 18,	
        GetMileage		= 19,
        TraceChar	    = 20,
        HideChar		= 21,	
        ShowChar		= 22,	
        InvincibleChar	= 23,	
        NormalChar	    = 24,
        LogoutChar		= 25,	
        Apocalypse		= 26,	
        CreateEquip		= 27,	
        EliteBonus	    = 28,

		StatueInit		= 29,	// 명예의 석상 초기화
		RespawnAllChar	= 30,	// 모든 캐릭터 리스폰 (배틀 그라운드)
		ChangeClass		= 31,	// 전직 하기
		LearnSkill		= 32,	// 해당 스킬 배우기
		EraseSkill		= 33,	// 해당 스킬 지우기
		LevelDown		= 34,	// 레벨 다운
		GetQuest		= 35,	// 퀘스트 받기
		AddQuest		= 36,	// 완료 퀘스트에 추가		(히스토리에 추가)
		DeleteQuest		= 37,	// 수행/완료 퀘스트를 삭제  (히스토리에서 삭제)

								// BATTLEGROUND_SERVER 운영자 명령
		FinishGame 		= 38,	// 해당 방의 경기 종료 (승패 결정)
		DrawGame		= 39,	// 해당 방의 경기 종료 (무승부)
		IgnoreGame		= 40,	// 해당 방의 경기 종료 (승패 무시)
		SetMax			= 41,	// 해당 방의 최대 인원수 설정
		SetScore		= 42,	// 해당 방의 목표 점수 설정
		SetLimitTime	= 43,	// 해당 방의 시간 제한 설정
		SetRestTime		= 44,	// 해당 방의 쉬는 시간 설정

					//	= 45,	// 서버에서 임시 인벤용으로 사용

		AddSpell		= 46,	// 해당 스펠을 부여한다.
		DeleteSpell		= 47,	// 해당 스펠을 지운다.

		Stop			= 48,	// 해당 캐릭터를 묶어둔다.
		StopOff			= 49,	// 묶인 캐릭터를 풀어준다.

		Neo				= 52,	// 네오
		Smith			= 53,	// 스미스

		Relation		= 55,

		LearnAbility	= 60,	// 해당 어빌리티 배우기
		EraseAbility	= 61,	// 해당 어빌리티 지우기
					//	= 100	// 서버에서 추적용도로 사용
    };

    enum AuthorizePanelCmd
    { 
        APCmd_Resurrection	    = 1,
        APCmd_Recall		    = 2
    };

    enum RespawnCmd
    {
		RespawnCmd_Normal			= 0,
		RespawnCmd_Select			= 1,
		RespawnCmd_FormerPlace		= 2,
		RespawnCmd_Human1			= 3,
		RespawnCmd_Human2			= 4,
		RespawnCmd_Human3			= 5,
		RespawnCmd_Akhan1			= 6,
		RespawnCmd_Akhan2			= 7,
		RespawnCmd_Akhan3			= 8,
        //-- 2004. 2. 13. Zergra From.--//
        RespawnCmd_BGServer_Random	= 9,
        //-- Zergra To. --//        
		RespawnCmd_BG_Exit			= 10
    };

    enum AutoRoutingCmd
    {
        ARCmd_Order			    = 0,
		ARCmd_Possible	    	= 1,	
		ARCmd_Impossible    	= 2	
	};

	enum SummonCmd
	{
		SummonCmd_Attack	    = 0,
		SummonCmd_Return	    = 1,
		SummonCmd_Die		    = 2,	
		SummonCmd_GuardOn   	= 3,
		SummonCmd_GuardOff	    = 4
	};
};


#endif

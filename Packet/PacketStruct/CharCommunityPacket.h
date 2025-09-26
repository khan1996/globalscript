#ifndef _CHAR_COMMUNITY_PACKET_H_
#define _CHAR_COMMUNITY_PACKET_H_

// CharCommunityPacket.h
// DBDefine필요.

#include <Network/Packet/PacketBase.h>
#include <Network/Packet/PacketStruct/DataPacket.h>
#include <Network/Packet/PacketStruct/CharItemPacketStruct.h>

#include "AddressPacket.h"
#include "CharCommunityPacketStruct.h"

#pragma pack(1)

// 파티 생성
struct PktPI : public PktBase
{
	PARTY_EX	m_Party;
};


// 파티 명령
//
//  명령 : 
//	0 = 초대,	1 = 가입,	2 = 거절,
//	3 = 퇴장,	4 = 탈퇴,	5 = 양도,
//	6 = 파괴,	7 = 로그인,	8 = 로그아웃,
//	9 = 삭제(캐릭터),	10 = 재로그인,
//	11 = 파티찾기,		12 = 멤버찾기,
//	13 = 오토루팅 켜기,	14 = 오토루팅 끄기
//	15 = 파티 멤버 존 이동
//	16 = 파티 멤버 사망
//
//  에러 : 
//	1 = 서버 에러,	2 = 이미 파티에 가입되어 있는 경우, 3 = 가입 실패
//	4 = 퇴장 실패,	5 = 탈퇴 실패,	6 = 양도 실패, 7 = 파티 인원 초과,
//	8 = 초대 실패,	9 = 거부 옵션을 켠 상태
//
struct PktPC : public PktBase
{
	enum PartyCmd 
	{ 
		PC_INVITE		= 0,	PC_ACCEPT			= 1,	PC_REFUSE			= 2, 
		PC_BANISH		= 3,	PC_SECESSION		= 4,	PC_TRANSFER			= 5,
		PC_DESTROY		= 6,	PC_LOGIN			= 7,	PC_LOGOUT			= 8,
		PC_DELETE		= 9,	PC_RELOGIN			= 10,	PC_FINDPARTY		= 11,
		PC_FINDMEMBER	= 12,	PC_AUTOROUTING_ON	= 13,	PC_AUTOROUTING_OFF	= 14,
		PC_MOVEZONE		= 15
	};

	enum PktPCError
	{	
		SERVER_ERROR	= 1,	CHAR_JOINED		= 2,	FAIL_ACCEPT		= 3,
		FAIL_BANISH		= 4,	FAIL_SECESSION	= 5,	FAIL_TRANSFER	= 6,
		OVER_MEMBER		= 7,	FAIL_INVITE		= 8,	FAIL_REJECT		= 9,

		ALREADY_PARTY	= 10,	FAIL_NOT_PARTY	= 11,	FAIL_NOT_LEADER	= 12
	};

	unsigned long	m_dwPartyID;        // 파티 아이디
	unsigned long	m_dwSenderID;	    // 캐릭터 아이디(주체)
	unsigned long	m_dwReferenceID;	// 참조 아이디(대상 캐릭터/서버 아이디)
   	unsigned short	m_wCmd;             // 명령
};


// 파티 명령 Ack
struct PktPCAck : public PktBase
{
	AddressInfo		m_SenderAddressInfo;        // Sender의 주소
	char			m_strSenderName[CHAR_INFOST::MAX_NAME_LEN];  // Sender의 이름

	unsigned long	m_dwPartyID;                // 파티 아이디
	unsigned long	m_dwSenderID;               // 캐릭터 아이디(주체)
	unsigned long	m_dwReferenceID;	        // 참조 아이디(대상 캐릭터/서버 아이디)

	unsigned long	m_dwReferenceGID;			// 파티에 임시 길드 아이디. 

    unsigned char	m_cCmd;                     // 명령
};


// 파티 정보
struct PktPCInfo : public PktBase 
{
	unsigned long	m_dwPartyID;                // 파티 ID
	unsigned long	m_dwMemberID;	            // 멤버 ID
	unsigned char   m_cCmd;                     // 명령
};

/*
// 파티 멤버 데이터 (+ 가변길이 데이터)
struct PktMD : public PktBase 
{
	unsigned long	m_dwCharID;			// 캐릭터 아이디
	unsigned short	m_wCmd;             // 서브 명령
	unsigned short	m_wLen;             // 길이
};


// 파티 멤버 정보
struct PktPM : public PktBase 
{
	SOCKADDR_IN		m_PublicAddress;
	SOCKADDR_IN		m_PrivateAddress;

	unsigned long	m_dwCharID;			// 캐릭터 아이디
	unsigned long	m_dwPartyID;	    // 파티 아이디

	unsigned long	m_dwChant;	        // 챈트 상태
	unsigned long	m_dwEnchant;        // 인챈트 상태

    unsigned short	m_wMaxHP;	        // 최대 HP
	unsigned short	m_sCurrHP;		    // 현재 HP
	unsigned short	m_wMaxMP;		    // 최대 MP
	unsigned short	m_sCurrMP;		    // 현재 MP

	unsigned char	m_cClass;           // 캐릭터 클래스
	unsigned char	m_cLevel;           // 캐릭터 레벨
	POS				m_Position;			// 현재 좌표
};

// 파티 멤버 주소 Update
struct PktPMA : public PktBase
{
	SOCKADDR_IN		m_PublicAddress;
	SOCKADDR_IN		m_PrivateAddress;

	unsigned long	m_dwCharID;
	unsigned long	m_dwPartyID;
};
*/

// 파티 찾기
struct PktPF : public PktBase 
{
	unsigned long m_dwCharID;			
};

// 파티 찾기 Ack
struct PktPFAck : public PktBase 
{
	enum { MAX_LIST = 30, MAX_DIFF_LEVEL_VIEW = 14 };

	unsigned long m_dwCharID;			    // 캐릭터 아이디
	unsigned char m_cMemberFindPartyNum;    // 파티 찾기 리스트 수
	unsigned char m_cPartyFindMemberNum;    // 파티원 찾기 리스트 수
};

// 캐릭터 명성(공헌도) 정보
struct PktFI : public PktBase 
{
	unsigned long	m_dwCharID;         // 캐릭터 아이디
};


// 캐릭터 명성(공헌도) 정보 Ack
struct PktFIAck : public PktBase 
{
	enum 
	{	
		MAX_NAME_LEN = 16 
	};

	enum FIAckCmd
	{
		FAME_INFO					= 0,	// 일반 정보 요청

		FAME_LOSE_DUEL				= 1,	// 자신이 듀얼 상대에게 살해 당한 경우
		FAME_LOSE_CONTINUE			= 2,	// 자신이 연속 살해 당한 경우
		FAME_LOSE					= 3,	// 자신이 살해 당해서 명성을 잃는 경우

		FAME_LOSE_MEMBER			= 4,	// 파티원이 살해 당한 경우

		FAME_WIN_DUEL				= 5,	// 자신이 듀얼 상대를 살해한 경우
		FAME_WIN_LARGE_GAP			= 6,	// 자신이 살해했지만 명성차가 큰 경우
		FAME_WIN_CONTINUE			= 7,	// 자신이 연속 살해한 경우
		FAME_WIN					= 8,	// 자신이 살해하여 명성과 마일리지를 얻는 경우

		FAME_WIN_MEMBER_DUEL		= 9,	// 파티원이 듀얼 상대를 살해한 경우
		FAME_WIN_MEMBER_LARGE_GAP	= 10,	// 파티원이 살해했지만 명성차가 큰 경우
		FAME_WIN_MEMBER_CONTINUE	= 11,	// 파티원이 연속 살해한 경우
		FAME_WIN_MEMBER				= 12,	// 파티원이 살해하여 명성과 마일리지를 얻는 경우

		FAME_LOSE_LEVELMIN			= 13,	// 자신이 레벨이 높은 유저에게 살해 당한 경우
		FAME_WIN_LEVELMIN			= 14,	// 자신이 레벨이 적은 유저를 죽인경우
		FAME_WIN_MEMBER_LEVELMIN	= 15	// 파티원이 레벨이 적은 유저를 죽인경우.
	};

	unsigned long	m_dwCharID;         // 캐릭터 아이디 (요청한 사람)

	unsigned long	m_cOtherNation;		// 상대방 캐릭터 국적
	unsigned char	m_cOtherClass;		// 상대방 캐릭터 클래스

	unsigned long	m_dwFame;			// 현재 명성
	unsigned long	m_dwMileage;		// 현재 마일리지

	unsigned char	m_cRankingByRace;   // 종족별 순위
	unsigned char	m_cRankingByClass;  // 클래스별 순위

	unsigned char	m_cCmd;				// 명령
//	unsigned char   m_KillCount; 
//	int				m_KillspreeLevel;

	char	m_szWinCharName[MAX_NAME_LEN];		// 살해한 캐릭터 이름
	char	m_szLoseCharName[MAX_NAME_LEN];		// 살해당한 캐릭터 이름
};

// 커맨드 / 데이터 갯수 / 데이터, 데이터, 데이터... / 채팅 메시지(최대 PktChatMaxSize)
struct PktChat : public PktBase 
{
    enum PktChatCmd
    {
        NORMAL		        = 0,    // 현재 캐릭터 좌표를 가지고 판단.
        PARTY		        = 1,    // 파티 채팅
        FRIEND		        = 2,    // 친구 채팅 (이름 char[16] * m_cNum - MAX 100개)
        GUILD	            = 3,    // 길드 채팅

        CLIENT_LOG	        = 5,
        STALL		        = 6,    // 노점상 채팅 (이름 char[16] * m_cNum - MAX 10개)	
        SHOUT               = 7,    // 외치기

        ADMIN_NORMAL_CHAT   = 8,    // 운영자 통상 채팅     (Ack로만 사용)
        ADMIN_SHOUT         = 9,    // 운영자 외치기        (Ack로만 사용)

        WHISPER             = 10,   // 이 번호는 쓰이지 않는다.Whisper는 별도의 패킷을 사용하기 때문이다. 단, 로그시에 쓰인다.
        TRADE               = 11,   // 외치기와 동일 (색 구분용으로 다른 커맨드로 만듬)

		CAMP_SHOP			= 12,	// 길드 요새 상점 채팅 (이름 char[16] * m_cNum - MAX 10개)	

		NOTIFY_CHAR_INFO	= 13,	// 운영자 명령 hp

		FIND_PARTY			= 14,	// 파티 찾기

		ENEMY_CHECK			= 15,	// 적아군 식별.

		DICE				= 16,	// 주사위 채팅 (파티채팅과 동일)

		GMREPORT			= 17,	// 지엠 채팅

        NOTICE		        = 255,

        PktChatMaxSize      = 180
    };

    enum ERR
    {
        SHOUT_TIMEOUT   = 2,		// 외침 시간 제한 걸림
		CHAT_BAN		= 3			// 채팅 금지
    };

    unsigned long   m_dwSenderCID;  // 채팅 전송자 CID
	unsigned short	m_usLang;		// 시스템 랭귀지
    unsigned char	m_cCmd;         // 커맨드(커맨드에 따라 데이터 크기가 다름)
    unsigned char   m_cNum;	        // 데이터 갯수
};


// 게임서버에서 채팅서버로 채팅 요청을 할 때 쓰이는 패킷이다.
struct PktChatRequest : public PktBase 
{
    unsigned long   m_dwUID;    // 유저 ID      (서버간 통신에서만 넣음)
    unsigned long   m_dwCID;    // 캐릭터 ID    

	unsigned short	m_usLang;		// 시스템 랭귀지

    unsigned short  m_usXPos;   // 캐릭터 좌표  (서버간 통신에서만 넣음)
    unsigned short  m_usYPos;   // 캐릭터 좌표  (서버간 통신에서만 넣음)
    unsigned short  m_usZPos;   // 캐릭터 좌표  (서버간 통신에서만 넣음)

	char			m_cZone;		// 캐릭터 존
    unsigned char	m_cCmd;         // 커맨드(커맨드에 따라 데이터 크기가 다름)
    unsigned char   m_cNum;	        // 데이터 갯수
};

struct PktChatBanInfo : public PktBase 
{	
	unsigned long   m_dwCID;		// 캐릭터 ID    	
};

// 캐릭터 속삭임 패킷
struct PktWh : public PktBase 
{
	enum
    {	        
        FAIL_CHAR_LOGOUT	= 2,	// 로그아웃한 캐릭터
        FAIL_REJECT		    = 3,	// 귓말 거부
		FAIL_ENEMY			= 4,	// 적군에게는 귓말을 보낼 수 없다.
		FAIL_CHAT_BAN		= 5,	// 채팅 금지

        PktWhMaxSize        = PktChat::PktChatMaxSize
	};    

	char    m_SenderName[CHAR_INFOST::MAX_NAME_LEN];
	char	m_RecverName[CHAR_INFOST::MAX_NAME_LEN];
};


// 게임서버 -> 채팅서버로 데이터 전달
struct PktChatData : public PktBase
{
    enum
    {
        LOGIN   = 0,
        LOGOUT  = 1,
        DELTA   = 2    
    };

    unsigned long 	m_dwUID;
    unsigned long 	m_dwCID;    
    unsigned char 	m_cType;
};


// 채팅 금지
struct PktChatBan : public PktBase
{
	unsigned long 	m_dwAdminCID;	// 채금을 건 운영자 CID
	unsigned long	m_dwTargetCID;	// 채금 대상의 CID
	unsigned long	m_dwMinutes;	// 채금 시간 ( 0이면 해제 )
};


// 캐릭터 노점상 개설 패킷
struct PktStO : public PktBase 
{
	enum { MAX_STALL_NAME_LEN = 32 };

	unsigned long	m_dwCharID;
	char			m_StallName[MAX_STALL_NAME_LEN];
};

// 캐릭터 노점상 물건 등록 패킷
struct PktStRI : public PktBase
{
	enum PktStRICmd 
	{
		SC_REGISTER			= 0,	// 개인 노점상 물품 등록
		SC_CANCEL			= 1,	// 개인 노점상 물품 등록 취소

		SC_CAMP_REGISTER	= 2,	// 길드 요새 상점 물품 등록
		SC_CAMP_CANCEL		= 3,	// 길드 요새 상점 물품 등록 취소
		SC_CAMP_SELL		= 4		// 길드 요새 상점의 물품이 판매되어 사라짐
	};

	enum PktStRIEErr
	{
		FAIL_NOT_ITEM	= 2,		// 잘못된 아이템입니다.
		FAIL_NOT_SELL	= 3			// 판매가 불가능합니다.
	};

	unsigned long	m_dwCharID;		// 주인 아이디
	unsigned long	m_dwShopID;		// 노점상 아이디 (개인 노점상의 경우 사용하지 않는다.)

	TakeType		m_TakeType;		// 아이템 이동 구조체
	unsigned long	m_dwPrice;		// 등록 가격

	unsigned char	m_cCmd;			// 명령

	char			m_BuyUser[CHAR_INFOST::MAX_NAME_LEN];
};

// 캐릭터 노점상 입장 패킷
struct PktStE : public PktBase
{
	enum PktStEErr
	{
		SUCCESS			= 0,
		SERVER_ERROR	= 1,
		FAIL_FULL_STALL	= 2,
		FAIL_ENTER		= 3,
		FAIL_LEAVE		= 4
	};

	unsigned long	m_dwCustomerID;
	unsigned long	m_dwOwnerID;
};

// 캐릭터 노점상 아이템 정보 패킷 (+ 아이템 정보 + 아이템 가격)
struct PktStIInfo : public PktBase
{
	enum 
	{ 
		MAX_CUSTOMER_NUM = 10 
	};

	unsigned long	m_dwOwnerID;						// 주인 ID
	unsigned long	m_dwCustomerID[MAX_CUSTOMER_NUM];	// 입장객 ID

	unsigned long	m_dwTempSafe;						// 임시 금고 (길드 요새 상점용)
	unsigned char	m_cTax;								// 세율 (길드 요새 상점용)

	unsigned long	m_dwItemSize;						// 등록된 아이템 크기
	unsigned char	m_cItemNum;							// 등록된 아이템 갯수
};

// 인증 판넬에 대한 승인
struct PktAP : public PktBase
{
	enum PktAPCmd
	{ 
		AP_RESURRECTION		= 1,
		AP_RECALL			= 2
	};

	unsigned long	m_dwAffectedID;
	unsigned long	m_dwCasterID;
	unsigned char	m_cCmd;
};

// 캐릭터 인증 판넬 
struct PktAPAck : public PktBase
{
	unsigned long	m_dwCasterID;
	char			m_szCasterName[CHAR_INFOST::MAX_NAME_LEN];
	unsigned char	m_cCmd;
};

// 캐릭터 듀얼시 연사 방지 패킷.
struct PktSaveEnemy : public PktBase
{
	unsigned long	m_dwCID;	
	unsigned long	m_dwAttackedCID;
	unsigned long	m_dwTick;
};

#pragma pack()

#endif
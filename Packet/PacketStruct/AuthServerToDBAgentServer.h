#ifndef _AUTHSERVER_TO_DBAGENT_SERVER_PACKET_H_
#define _AUTHSERVER_TO_DBAGENT_SERVER_PACKET_H_

#include <winsock2.h>
#include <windows.h>
#include <DB/DBDefine.h>
#include "DataPacket.h"

#pragma pack(1)

// ---------------------------------------------------------
// 유저 로그인 요청     AuthServer -> DBAgentServer
typedef struct PktULD* LPPktULD;
struct PktULD : public PktDD 
{
	unsigned long	m_dwSessionID;
	unsigned long	m_dwUserID;

	IN_ADDR			m_Address;	

	unsigned char	m_cLoginType;
};

// 유저 로그인 응답     DBAgentServer -> AuthServer
typedef struct PktULDAck* LPPktULDAck;
struct PktULDAck : public PktDD 
{
	enum Const
	{
		MAX_CHAR_VIEW = 5
	};

	unsigned long		m_dwUserID;
	sGuildData			m_GuildData[MAX_CHAR_VIEW];
	CHAR_VIEW			m_CharView[MAX_CHAR_VIEW];

	STORE_INFO			m_sStoreInfo;

	// WORK_LIST 2.1 계정 국적 추가
	unsigned char		m_cAccountNation;	// 계정 국적
	
	int					m_nPlayTime;        // BillingType이 정액인 경우는 남은 날짜. 정량인 경우는 남은 시간(분)
	unsigned short		m_usAdminLevel;
	unsigned char		m_cBillingType;     // D : 정액 T : 정량  N : 무료
	unsigned char		m_cBillingUser;     // 0 : 개인 1 : 피시방

	unsigned char	m_cLoginType;	
};


// 유저 로그아웃	AuthServer -> DBAgentServer
typedef struct PktULoD *LPPktULoD;
struct PktULoD : public PktDD 
{
	unsigned long	m_dwSessionID;
	unsigned long	m_dwUserID;
};

// 유저 이동
typedef struct PktUMvD *LPPktUMvD;
struct PktUMvD : public PktDD 
{
	unsigned long	m_dwSessionID;
	unsigned long	m_dwUserID;
};


// 세션 시작		DBAgentServer -> AuthServer
typedef struct PktSSD *LPPktSSD;
struct PktSSD : public PktDD 
{
	enum 
	{ 
		// edith 2008.03.17 ID,PASS 길이조정
		MaxName = 24,
		MaxPass = 36,
	};

	unsigned long	m_dwSessionID;
	unsigned long	m_dwUserID;
	char			m_AccountName[MaxName];
	// edith 2008.01.15 패스워드 추가작업
	char			m_Password[MaxPass];

    unsigned char   m_cAgentServerType;    
    unsigned char   m_cFirstLogin;

	IN_ADDR			m_Address;
};

/*

* cAgentServerType 멤버
CServerSetup::AgentServerType 의 값을 참고할 것.

    Part1           = 20,
    Part1Unified    = 21,
    Part2           = 22,
    Part2Unified    = 23,
    Part2Selectable = 24,

* cFirstLogin 멤버

에러값이 0 이상이면 무시한다. 

cAgentServerType값이 Part2Unified/Part2Selectable일 경우만 유의미한 값이다.

Part2Unified
    창고 가져온 적 없다 : 0
    Part1 통합에서 창고 가져온 적 있다 : 1
    Part2 통합에서 창고 가져온 적 있다 : 2

Part2Selectable

    로그인한 적이 없다 : 0
    로그인한 적이 있다 : 2        
*/


// ---------------------------------------------------------


// ---------------------------------------------------------
// 캐릭터 생성 요청     AuthServer -> DBAgentServer
typedef struct PktCCD* LPPktCCD;
struct PktCCD : public PktDD 
{
	unsigned long	m_dwUserID;

	unsigned long	m_dwSlot;
	unsigned long	m_dwGold;
	POS				m_Pos;
	CHAR_CREATE		m_CharCreate;

	unsigned short	m_wSkill;
};


// 캐릭터 생성 응답     DBAgentServer -> AuthServer
typedef struct PktCCDAck* LPPktCCDAck;
struct PktCCDAck : public PktDD 
{
	unsigned long	m_dwCharID;
	unsigned long	m_dwSlot;
	CHAR_VIEW		m_CharView;
};
// ---------------------------------------------------------



// ---------------------------------------------------------
// 캐릭터 선택 관련 (데이터)
typedef struct PktCSD* LPPktCSD;
struct PktCSD : public PktDD 
{
	unsigned long		m_dwUserID;	
	unsigned long		m_dwCharID;		
};

typedef struct PktCSDAck* LPPktCSDAck;
struct PktCSDAck : public PktDD 
{
	enum { MAX_CHANNEL_NUM = 5 };

	unsigned char		m_cZone;
	unsigned short		m_wChannelNum[MAX_CHANNEL_NUM];		
};

// ---------------------------------------------------------


// ---------------------------------------------------------
// 캐릭터 삭제 관련 (데이터)
typedef struct PktCDD* LPPktCDD;
struct PktCDD : public PktDD 
{
	unsigned long	m_dwUserID;
	unsigned long	m_dwSlotNum;
	unsigned long	m_dwCharID;
};
// ---------------------------------------------------------


// WORK_LIST 2.1 계정 국적 추가
// ---------------------------------------------------------
// 계정 국적 변경 관련 (데이터)
typedef struct PktSAND* LPPktSAND;
struct PktSAND : public PktDD 
{
	enum eType { TYPE_SET = 1, TYPE_CHANGE = 2 };

	unsigned long	m_dwUserID;
	unsigned char	m_cType;
	unsigned char	m_cAccountNation;
};

// 계정 국적 변경에 따른 결과 패킷
typedef struct PktANCAck* LPPktANCAck;
struct PktANCAck : public PktDD
{
	enum
	{
		MAX_CHAR_VIEW	= 5
	};

	unsigned long	m_dwUserID;
	unsigned long	m_dwGID[ MAX_CHAR_VIEW ];
	unsigned long	m_dwFame[ MAX_CHAR_VIEW ];
};
// ---------------------------------------------------------

// ---------------------------------------------------------
// 캐릭터 생성 아이템 관련 (데이터)
typedef struct PktCCID* LPPktCCID;
struct PktCCID : public PktDD 
{
    unsigned __int64    m_dwItemSerial;
	unsigned long	    m_dwCharID;
    unsigned long	    m_dwSize;
};
// ---------------------------------------------------------


// ---------------------------------------------------------
// 캐릭터 로그인 관련 (데이터)
typedef struct PktCLiD* LPPktCLiD;
struct PktCLiD : public PktDD 
{
	unsigned long	m_dwUserID;
	unsigned long	m_dwCharID;
	unsigned long	m_dwSerial;	
};
// ---------------------------------------------------------

#pragma pack()

#endif
#ifndef _CLIENT_TO_AUTH_SERVER_PACKET_H_
#define _CLIENT_TO_AUTH_SERVER_PACKET_H_

#include <DB/DBDefine.h>
#include <Network/Packet/PacketBase.h>


#pragma pack(1)


////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 사용자 인증 요청 (Client to AuthServer)
//	- 계정 이름
//	- 계정 패스워드
//	- 클라이언트 버젼
//
//	: 사용자 계정으로 로그인하여 유저 아이디와 로그인 가능한 서버 리스트드을 얻는다.
//
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct PktAU* LPPktAU;
struct PktAU : public PktBase 
{
    enum 
	{ 
		// edith 2008.03.17 ID,PASS 길이조정
		ID_LEN		= 24, 
		PASS_LEN	= 36
	};

	char			m_UserAccount[ID_LEN];
	char			m_UserPassword[PASS_LEN];

    unsigned long	m_dwSessionID;
	unsigned long	m_dwClientVer;
	unsigned long	m_dwCheckSum;

	unsigned short  m_usFlag;
};


////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 일본쪽 사용자 인증 요청 (Client to AuthServer)
//
//	: 사용자 계정으로 로그인하여 유저 아이디와 로그인 가능한 서버 리스트드을 얻는다.
//
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct PktJPAU* LPPktJPAU;
struct PktJPAU : public PktBase
{
	enum { NAME_LEN = 16 };

	char            m_szUserAccount[NAME_LEN];

	unsigned long   m_dwUserID;
	unsigned long   m_dwSessionID;
	unsigned long   m_dwClientVer;
	unsigned long   m_dwCheckSum;

	unsigned short  m_usFlag;
};



////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 사용자 인증 응답	(AuthServer to Client)
//	- 유저 아이디
//
//	: 사용자 계정으로 로그인하여 유저 아이디를 얻는다.
//
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct PktAUAck* LPPktAUAck;
struct PktAUAck : public PktBase
{
	unsigned long   m_dwUserID;	
    unsigned char   m_cAgentServerType;
    unsigned char   m_cFirstLogin;     
};


////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 유저 로그인 (Client to AuthServer)
//	- 유저 아이디
//	- 체크섬
//	- 명령
//
//	: 유저 로그인 후에 캐릭터 셀렉트 관련 정보를 얻어온다.
//
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct PktULi* LPPktULi;
struct PktULi : public PktBase 
{
	enum LoginType
	{
		USERLOGIN_FIRST			= 0,	// 최초 로긴
		USERLOGIN_CHAR_SELECT	= 1		// 캐릭터 선택화면으로 이동
	};

 	unsigned long   m_dwUserID;
	unsigned long   m_dwCheckSum;

	unsigned char	m_cLoginType;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 유저 로그인 Ack (AuthServer to Client)
//	- 유저 아이디
//	- 캐릭터 정보
//
//	(1 = 서버 에러, 2 = 데이터 얻기 실패)
//
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct PktULiAck* LPPktULiAck;
struct PktULiAck : public PktBase 
{
	enum Const
	{
		MAX_CHAR_VIEW = 5
	};

	enum PktULiAckErr
	{ 
		FAIL_GET_DATA = 2 
	};

	unsigned long	m_dwSessionID;
	unsigned long	m_dwUserID;
	CHAR_VIEW		m_CharView[MAX_CHAR_VIEW];
	sGuildData		m_GuildData[MAX_CHAR_VIEW];

	unsigned long	m_dwTabFlag;

	// WORK_LIST 2.1 계정 국적 추가
	unsigned char	m_cAccountNation;	// 계정 국적

    time_t          m_dwCurrentTime;    // 현재 시간
    int             m_nPlayTime;        // BillingType이 정액이나 무료인 경우는 남은 날짜. 정량인 경우는 남은 시간(분)
    unsigned short	m_usAdminLevel;     // 운영자 레벨.
    unsigned char   m_cBillingType;     // D : 정액     T : 정량    N : 무료
    unsigned char   m_cBillingUser;     // 0 : 개인     1 : 피시방

	unsigned char	m_cLoginType;
};


////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 캐릭터 선택	( Char Select )
//	- 유저 아이디
//	- 캐릭터 아이디
//
//	: 캐릭터 선택 후 해당 캐릭터의 서버의 예약 포인터와 주소를 넘겨 받는다.
//
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct PktCS* LPPktCS;
struct PktCS : public PktBase 
{
	unsigned long		m_dwUserID;	
	unsigned long		m_dwCharID;
};

// 캐릭터 선택 Ack
typedef struct PktCSAck* LPPktCSAck;
struct PktCSAck : public PktBase 
{
	enum { MAX_CHANNEL_NUM = 5 };

	unsigned char		m_cZone;							// 존
	unsigned short		m_wChannelNum[MAX_CHANNEL_NUM];		// 채널
};


////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 캐릭터 생성	( Char Create )
//	- 유저 아이디
//	- 슬롯 번호
//	- 캐릭터 생성 데이터
//	- 복장 (셔츠, 부츠, 무기)
//
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct PktCC* LPPktCC;
struct PktCC : public PktBase
{
	unsigned long		m_dwUserID;		
	unsigned long		m_dwSlotNum;		
	CHAR_CREATE			m_CreateChar;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 캐릭터 생성 Ack
//	- 캐릭터 아이디
//	- 슬롯 번호
//	- 캐릭터 데이터
//
//	(1 = 서버 에러, 2 = 데이터 생성 실패, 3 = 존재하는 캐릭터 이름, 4 = 비어 있지 않은 슬롯, 
//		5 = 적당하지 않은 이름, 6 = 잘못된 생성 데이터)
//
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct PktCCAck* LPPktCCAck;
struct PktCCAck : public PktBase
{
	unsigned long		m_dwCharID;
	unsigned long		m_dwSlotNum;		
	CHAR_VIEW			m_CharView;

	enum 
    { 
        SERVER_ERROR            = 1, 
        FAIL_INSERT_DATA        = 2, 
        EXIST_CHAR_NAME         = 3, 
        EXIST_SLOT              = 4,
        WRONG_CHAR_NAME         = 5, 
        WRONG_CHAR_DATA         = 6,
        SLANG_FILTERED          = 7,
        NULL_SESSION            = 8,
        VIEW_RELOAD_FAILED      = 9,
        SUPRESSED_CHAR_CREATE   = 10
    };
};

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 캐릭터 삭제	( Char Delete )
//	- 유저 아이디
//	- 캐릭터 아이디
//	- 슬롯 번호
//	- 파티 아이디
//	- 길드 아이디
//
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct PktCD* LPPktCD;
struct PktCD : public PktBase 
{
	unsigned long		m_dwUserID;	
	unsigned long		m_dwCharID;		
	unsigned long		m_dwSlotNum;	

	unsigned char		m_szPassword[STORE_INFO::MAX_PASS_LEN];
};


////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 캐릭터 삭제 Ack
//	- 유저 아이디
//	- 슬롯 번호
//
//	(1 = 서버 에러, 2 = 데이터 삭제 실패)
//
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct PktCDAck* LPPktCDAck;
struct PktCDAck : public PktBase 
{
	unsigned long		m_dwUserID;			
	unsigned long		m_dwSlotNum;		
	enum 
    { 
        SERVER_ERROR        = 1, 
        FAIL_DELETE_DATA    = 2,
    
        BLOCKED_DELETE_DATA = 6,

		FAIL_PASSWORD		= 7
    };
};

// WORK_LIST 2.1 계정 국적 추가
////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 계정 국적 변경 ( Select Account Nation )
//	- 유저 아이디
//	- 계정 국적
//
//	(1 = 서버 에러, 2 = 설정 실패)
//
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct PktSelectAccountNation* LPPktSelectAccountNation;
struct PktSelectAccountNation : public PktBase
{
	unsigned long		m_dwUserID;
	unsigned char		m_cType;
	unsigned char		m_cAccountNation;

	enum eType
	{
		TYPE_SET		= 1,	// 값 셋팅
		TYPE_CHANGE		= 2		// 계정 국적 변경
	};

	enum { SERVER_ERROR = 1, FAIL_SET_NATION = 2 };
};

// WORK_LIST 2.3 계정 국적 변경 기능 구현
////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 계정 국적 변경으로 인한 캐릭터들 결과 정보
//	- 유저 아이디
//	- CID, GID, Fame
//
//	(1 = 서버 에러 )
//
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct PktNationChangeResult* LPPktNationChangeResult;
struct PktNationChangeResult : public PktBase
{
	enum 
	{
		MAX_CHAR_VIEW	= 5
	};

	unsigned long		m_dwUserID;
	unsigned long		m_dwGID[ MAX_CHAR_VIEW ];
	unsigned long		m_dwFame[ MAX_CHAR_VIEW ];
};


#pragma pack()


#endif
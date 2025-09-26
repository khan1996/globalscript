#ifndef _DBAGENT_TO_LOGIN_
#define _DBAGENT_TO_LOGIN_

#include <DB/DBDefine.h>
#include <Network/Packet/PacketBase.h>
#include <Network/Packet/PacketStruct/DataPacket.h>


#pragma pack(1)


// 핑 패킷 ( 클라이언트 -> 서버로 5초마다.. 서버 - 서버간 통신도, 연결하는 쪽에서 Ping을 준다. )
typedef struct PktSyP* LPPktSyP;
struct PktSyP : public PktBase
{
    unsigned long m_dwTickTime;
};


// 서비스 로그인 요청 패킷
typedef struct PktSL* LPPktSL;
struct PktSL : public PktBase 
{
	unsigned long	m_dwServerID;
	IN_ADDR			m_Address;	
};


// 서비스 로그인 응답 패킷
typedef struct PktSLAck* LPPktSLAck;
struct PktSLAck : public PktBase 
{
	enum { PATCH_ADDRESS_LENGTH = 100 };

	unsigned __int64    m_dlItemUID;
	unsigned long	    m_dwServerID;
	
	unsigned long	    m_dwClientVer;
	unsigned long	    m_dwCheckSum;	

	char			    m_PatchAddress[PATCH_ADDRESS_LENGTH];
};


// 서비스 버전 업데이트 정보
typedef struct PktSVU* LPPktSVU;
struct PktSVU : public PktBase 
{
	enum { PATCH_ADDRESS_LENGTH = 100 };

	unsigned long	m_dwServerID;
	IN_ADDR			m_ServerAddr;	
	unsigned long	m_dwClientVer;
	unsigned long	m_dwCheckSum;	
	char			m_PatchAddress[PATCH_ADDRESS_LENGTH];
};


// 채널 업데이트 정보
typedef struct PktSCInfo* LPPktSCInfo;
struct PktSCInfo : public PktBase 
{
	enum { MAX_CHANNEL_NUM = 5 };

	unsigned long		m_dwServerID;
	unsigned char		m_cChannelNum;
	unsigned short		m_usChannelClientNum[MAX_CHANNEL_NUM];
};


// ---------------------------------------------------------------------------
// 패치 정보 관련 패킷


// 패치 정보 요청 패킷
typedef struct PktSPI* LPPktSPI;
struct PktSPI : public PktBase 
{
	unsigned long	m_dwServerID;
};

// 패치 서버 응답 패킷
typedef struct PktSPIAck* LPPktSPIAck;
struct PktSPIAck : public PktBase 
{
	enum { PATCH_ADDRESS_LENGTH = 100 };
	unsigned long	m_dwClientVer;
	char			m_PatchAddress[PATCH_ADDRESS_LENGTH];
};


// ---------------------------------------------------------------------------
// Zone 관련 패킷

// 서버존 선택
typedef struct PktSZ* LPPktSZ;
struct PktSZ : public PktBase 
{
    unsigned short		m_cChannel;
	unsigned char		m_cZone;	
};

// 서버존 선택 Ack
typedef struct PktSZAck* LPPktSZAck;
struct PktSZAck : public PktBase 
{
	unsigned long		m_dwServerID;
    sockaddr_in         m_GameServerTCPAddr;	
};

// 서버존 이동
typedef struct PktSZMv* LPPktSZMv;
struct PktSZMv : public PktBase 
{
    POS		m_NewPos;
	char	m_cZone;
    char    m_cChannel;
};

// 서버존 선택 Ack
typedef struct PktSZMvAck* LPPktSZMvAck;
struct PktSZMvAck : public PktBase 
{
	enum { MAX_CHANNEL_NUM = 5 };

    unsigned short	m_wChannelNum[MAX_CHANNEL_NUM];
	unsigned char	m_cZone;	
};

typedef struct PktSZMvD* LPPktSZMvD;
struct PktSZMvD : public PktDD 
{
	unsigned long	m_dwUserID;
	char	m_cZone;		
	char	m_cChannel;

	POS		m_NewPos;			
};

typedef struct PktSZMvDAck* LPPktSZMvDAck;
struct PktSZMvDAck : public PktDD 
{
	enum 
    { 
        MAX_CHANNEL_NUM = 5 
    };

	enum PktSZMvDAckErr
	{
		FAIL_INVALID_ZONE	= 3,
		FAIL_INVALID_CHAR	= 5,
		FAIL_PERSONNEL_OVER	= 6,
		FAIL_LIMIT_LEVEL	= 7
	};

	unsigned char		m_cZone;
	unsigned short		m_wChannelNum[MAX_CHANNEL_NUM];		
};

// 서버 주소 얻기
typedef struct PktSA* LPPktSA;
struct PktSA : public PktDD 
{
	unsigned long		m_dwUserID;     // 유저 아이디
	char				m_cZone;		// 존 번호
	char				m_cChannel;		// 채널 번호
};

// 서버 주소 얻기 Ack
typedef struct PktSAAck* LPPktSAAck;
struct PktSAAck : public PktDD 
{
    enum PktSAAckErr
    {
        NO_SERVER_ERR	= 0,
        SERVER_ERROR	= 1,
        FAIL_USER_LIMIT	= 4
    };

	unsigned long		m_dwServerID;		
	IN_ADDR				m_GameAddress;	
};

// ---------------------------------------------------------------------------
// 유저 관련

// 유저 테이블 업데이트
typedef struct PktUUT *LPPktUUT;
struct PktUUT : public PktBase 
{
	enum UpdateType
    {
		UpdateUIDTableNone			= 0, 	// 없음
		UpdateUIDTableUserLogin		= 1,	// 유저 로그인
		UpdateUIDTableUserLogout	= 2, 	// 유저 로그아웃
		UpdateUIDTableUserMove		= 3, 	// 유저 이동
		UpdateUIDTableCharLogin		= 4,	// 캐릭터 로그인
		UpdateUIDTableCharLogout	= 5, 	// 캐릭터 로그아웃
		UpdateUIDTableCharMove		= 6, 	// 캐릭터 이동

		UpdateUIDTableBillingCheck	= 7, 	// 빌링 체크

        MAX_UPDATE_TYPE             = 8
	};

    enum Err
    {
        DISCONNECT_USER     = 1
    };

	enum
    {
		// edith 2008.03.17 ID,PASS 길이조정
        MaxAccountLen       = 24,	
        MaxPasswordLen       = 36
    };

	unsigned char	m_cCmd;
	char			m_strAccount[MaxAccountLen];

	// edith 2008.01.15 패스워드 추가작업
	char			m_strPassword[MaxPasswordLen];

	IN_ADDR			m_IPAddress;	

	unsigned long	m_dwSessionID;
	unsigned long	m_dwServerID;
	unsigned long	m_dwUserID;
	unsigned long	m_dwCharID;
};

typedef struct PktUUTAck *LPPktUUTAck;
struct PktUUTAck : public PktBase
{
	unsigned char	m_cCmd;

	unsigned long	m_dwUserID;
	unsigned long	m_dwFlag;				// Success(=0)
	int             m_nPlayTime;			// 현재 남은 시간 (일, 분)
    unsigned long   m_dwCRMIndex1;          // 피시방 ID(0이면 개인유저)
	char			m_cstrBillingType;		// D, T(N: 무료계정)
};

// 유저 죽이기.
typedef struct PktUK* LPPktUK;
struct PktUK : public PktBase
{
	unsigned long	m_dwUserID;
	unsigned long	m_dwCharID;
	unsigned long	m_dwServerID;
};

// 빌링 타임아웃 공지. (Billing Timeout Notify)
typedef struct PktBTN* LPPktBTN;
struct PktBTN : public PktBase 
{
    unsigned long	m_dwUserID;
    unsigned long	m_dwCharID;
    unsigned long	m_dwServerID;
    unsigned char   m_cRemainMinute;    // 남은 분..
    unsigned char   m_cBillingType;     // 'D' 정액제    'T' 정량제
};

// 한게임 통합빌링 경고 메시지
typedef struct PktHanBTN* LPPktHanBTN;
struct PktHanBTN : public PktBase
{
    enum
    {
        MAX_HAN_BTN = 70
    };

    unsigned long   m_dwIP;
    unsigned long   m_dwUID;
    unsigned long   m_dwCID;
    char    m_szAccount[PktUUT::MaxAccountLen];
    char    m_szMsg[MAX_HAN_BTN];
};

// 한게임 통합빌링 접속 끊기 메시지(계정명 및, IP로)
typedef struct PktHanUserKill* LPPktHanUserKill;
struct PktHanUserKill : public PktBase
{
    enum
    {
        DISCONN_ALL_IP  = 0xFFFFFFFF
    };   

    unsigned long   m_dwIP;
    char    m_szAccount[PktUUT::MaxAccountLen];
};

#pragma pack()

#endif
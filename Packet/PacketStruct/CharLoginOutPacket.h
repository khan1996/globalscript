#ifndef _CHAR_LOGIN_OUT_PACKET_H_
#define _CHAR_LOGIN_OUT_PACKET_H_

// CharLoginOutPacket.h
#include <winsock2.h>
#include <windows.h>

#include <DB/DBDefine.h>

#include <GameGuardLib/ggsrv.h>
#include <Network/Packet/PacketBase.h>

#include "CharLoginOutPacketStruct.h"

// 전방 참조
class CSession;

#pragma pack(1)

typedef struct PktDBUpdate* LPPktDBUpdate;
struct PktDBUpdate : public PktBase 
{
	unsigned __int64	m_dlItemSerial;     // 가장 마지막으로 업데이트한 아이템 시리얼
	unsigned long	    m_dwSessionID;      // 세션 ID
	unsigned long	    m_dwUserID;         // 유저 ID
	unsigned long	    m_dwCharID;         // 캐릭터 ID
	unsigned long       m_dwRequestKey;     // DB요청 키

    char                m_szAccountName[CHAR_INFOST::MAX_ACCOUNT_LEN];  // 접속 계정명

	IN_ADDR			    m_Address;	        // 주소

    unsigned char       m_cAdminLevel;      // 운영자 레벨

	unsigned char		m_cAdminFlag;
	
	// WORK_LIST 2.4 계정 국적을 게임서버의 캐릭터가 가지도록 구현
	unsigned char		m_cAccountNation;	// 계정 국적 (추가하면서 패딩값을 삭제했음)

    unsigned char       m_cNameChangeCount; // 이름 바꿀 수 있는 횟수

	unsigned char		m_cGuildWarFlag;	// 길드 전쟁 참여 플래그
	unsigned char		m_cRealmWarFlag;	// 국가 전쟁 참여 플래그
	unsigned char		m_cTacticsFlag;		// 욥병 플래그.

	unsigned char		m_cRealmPoint;		// 국가 전쟁 훈장공헌 포인트.

	unsigned short	    m_TypeCode;         // 업데이트 타입

	unsigned short	    m_usUpdate[DBUpdateData::MAX_UPDATE_DB];  // 업데이트 데이터 크기

	unsigned int		m_PlayTime;			// 게임을 연속 몇분이나 했는지 (중국판을 위한 패킷)
	long				m_PremiumTime;		// 프리미엄 서비스를 위한 정보
	int					m_PremiumType;		// 프리미엄 서비스를 위한 정보
};

// 캐릭터 로그인	(Char Login)
typedef struct PktCLi* LPPktCLi;
struct PktCLi : public PktBase 
{
	unsigned long	m_dwUserID;
	unsigned long	m_dwCharID;
	unsigned long	m_dwSessionID;	
};

// 캐릭터 로그인 Ack (1 = 서버 에러, 2 = 데이터 얻기 실패)
typedef struct PktCLiAck* LPPktCLiAck;
struct PktCLiAck : public PktBase 
{
    enum { MAX_ADDRESS_LEN = 16 };

    sockaddr_in     m_GameServerUDPAddr;    // 더 이상 사용하징 않음 (UDP Removed 2004/09/24)
    sockaddr_in     m_ChatServerTCPAddr;    // 더 이상 사용하징 않음 (ChatServer Connection Removed 2004/09/24)

    unsigned long   m_dwUID;
    unsigned long   m_dwServerID;
	unsigned long	m_dwServerTime;

	bool			m_bPeaceMode;			// 반전/전투 모드
	bool			m_bCheckRelation;		// 신청/ 신청 받음 길드 관계 체크

    unsigned char   m_cNameChangeCount;     // 이름 변경 가능한 횟수

	unsigned char	m_cAdminFlag;			// GM 모델 관련.

	unsigned char	m_cGuildWarFlag;		// 길드 전쟁 참여 플래그
	unsigned char	m_cRealmWarFlag;		// 국가 전쟁 참여 플래그
	unsigned char	m_cRealmPoint;			// 국가 전쟁 공헌훈장 포인트.
	unsigned char	m_cTacticsFlag;			// 용병 플레그.

	unsigned short	m_usUpdate[DBUpdateData::MAX_UPDATE_DB];

	unsigned int	m_PlayTime;			// 게임을 연속 몇분이나 했는지 (중국판을 위한 패킷)
	long			m_PremiumTime;		// 프리미엄 서비스를 위한 정보
	int				m_PremiumType;		// 프리미엄 서비스를 위한 정보
};

typedef struct PktCSAuth* LPPktCSAuth;
struct PktCSAuth : public PktBase
{
	unsigned long	m_dwCharID;				// 캐릭터 아이디		
	unsigned long 	m_AuthCodeType;			// 인증코드 타입. 타입에 맞지 않는 값이 있으면 접속 해제. (1혹은 2)

	unsigned long	m_dwAuthCode;			// 인증 코드 (게임 가드)
	GG_AUTH_DATA	m_AuthCode2;			// 인증 코드2 (게임 가드)
};

// 캐릭터 로그아웃	(Char Logout)
typedef struct PktCLo* LPPktCLo;
struct PktCLo : public PktBase 
{
	unsigned long	m_dwCharID;				// 캐릭터 아이디			
};

// 캐릭터 로그아웃	(Char Logout)
typedef struct PktCLoAck* LPPktCLoAck;
struct PktCLoAck : public PktBase 
{
	unsigned long	m_dwCharID;				// 캐릭터 아이디			
};

#pragma pack()

#endif
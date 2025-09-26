#ifndef _CLIENT_TO_LOGIN_SERVER_PACKET_H_
#define _CLIENT_TO_LOGIN_SERVER_PACKET_H_

#include "ServerInfo.h"
#include <Network/Packet/PacketBase.h>

#pragma pack(1)

// -----------------------------------------------------------------------------
// 로그인 클라이언트 패킷.


// -----------------------------------------------------------------------------
// 서버 리스트 요청 패킷 : Launcher to LoginServer
typedef struct PktSvL* LPPktSvL;
struct PktSvL : public PktBase 
{
	unsigned long		m_dwClientVer;
};

// 서버 리스트 응답 패킷 : LoginServer to Launcher
typedef struct PktSvLAck* LPPktSvLAck;
struct PktSvLAck : public PktBase 
{
	enum { PATCH_ADDRESS_LENGTH = 100 };

	unsigned long		m_dwClientVer;
	char				m_PatchAddress[PATCH_ADDRESS_LENGTH];

	SERVER_LIST			m_ServerList;				// 서버 리스트
};
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// 서버군 선택 요청 패킷 : Launcher to LoginServer
typedef struct PktSG* LPPktSG;
struct PktSG : public PktBase 
{
    char				m_cGroup;
};

// 서버군 선택 응답 패킷 : LoginServer to Launcher
typedef struct PktSGAck* LPPktSGAck;
struct PktSGAck : public PktBase
{
	unsigned long		m_dwServerID;
	IN_ADDR				m_AuthAddress;
};
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// 사용자 인증 요청 패킷(OLD) : Launcher to LoginServer
typedef struct PktAUO* LPPktAUO;
struct PktAUO : public PktBase 
{
	enum 
	{ 
		// edith 2008.03.17 ID,PASS 길이조정
		NAME_LEN = 24,
		PASS_LEN = 36
	};

	char				m_UserAccount[NAME_LEN];	
	char				m_UserPassword[PASS_LEN];	

	unsigned long		m_ClientVerInfo;
	unsigned short		m_cFlag;
};

// 사용자 인증 응답 패킷 : LoginServer to Launcher
typedef struct PktAUOAck* LPPktAUOAck;
struct PktAUOAck : public PktBase 
{
	unsigned long		m_dwUserID;					// 유저 아이디
	SERVER_LIST			m_ServerList;				// 서버 리스트
};

// -----------------------------------------------------------------------------


#pragma pack()

#endif
#ifndef _CHAR_FRIEND_PACKET_H_
#define _CHAR_FRIEND_PACKET_H_

// CharFriendPacket.h
#include <Network/Packet/PacketBase.h>
#include <Network/Packet/PacketStruct/DataPacket.h>

#pragma pack(1)

/*
    struct PktFriendAddReq : public PktBase         CMD : 0x64
    struct PktFriendRemoveReq : public PktBase      CMD : 0x65
    struct PktFriendEtc : public PktBase            CMD : 0x66
    struct PktFriendAck : public PktBase            CMD : 0x67
    struct PktFriendDB : public PktBase             CMD : 0x68

    친구 관련 Operations 

    <Client->GameServer>

    친구 등록 : PktFriendAddReq
    거부 등록 : PktFriendAddReq
    친구 삭제 : PktFriendRemoveReq
    거부 삭제 : PktFriendRemoveReq
    그룹 설정 : PktFriendEtc(Ack없음)
    
    <GameServer->Client>

    친구 리스트 주기 : PktFriendDB
    거부 리스트 추기 : PktFriendDB

    친구 등록 Ack : PktFriendAck
    친구 삭제 Ack : PktFriendAck
    거부 등록 Ack : PktFriendAck
    거부 삭제 Ack : PktFriendAck
    
    친구 로그인   : PktFriendAck
    친구 로그아웃 : PktFriendAck
    친구 등록당함 : PktFriendAddReq
    

    <GameServer->DBAgent>

    친구 등록 : PktFriendDB
    거부 등록 : PktFriendDB
    친구 제거 : PktFriendDB
    거부 제거 : PktFriendDB
    그룹 세팅 : PktFriendDB


    <DBAgent->GameServer>

    친구 리스트 주기 : PktFriendDB
    거부 리스트 주기 : PktFriendDB
*/

struct DBFriend
{
	enum { MAX_NAME = 16 };

	unsigned long   m_dwCID;
	unsigned long   m_dwStatusFlag;
	char            m_szName[MAX_NAME];	
};

struct DBFriendInfo
{
	unsigned long	m_dwGID;				// 길드정보.
	unsigned short	m_wClass;				// 클래스정보.
	char			m_cLevel;				// 레벨정보.
};

struct FriendInfo
{
private:

    enum StatusMask
    {
        IS_LOGINED  = 0x00000001,
        GROUP       = 0x000000F0,		
    };

public:

    enum
	{ 
		MAX_NAME	= 16,
		MOVE_ZONE	= 0x00000100,
	};
    
    unsigned long   m_dwCID;
    unsigned long   m_dwStatusFlag;
    char            m_szName[MAX_NAME];

	unsigned long	m_dwGID;				// 길드정보.
	unsigned short	m_wClass;				// 클래스정보.
	char			m_cLevel;				// 레벨정보.

	unsigned long	m_dwServerID;			// 서버 정보.

    FriendInfo() : m_dwCID(0), m_dwStatusFlag(0), m_dwGID(0), m_wClass(0), m_cLevel(0), m_dwServerID(0)	{ }

    bool IsLogined() { return (0 != (m_dwStatusFlag & IS_LOGINED)); }

	void SetLoginStatus(bool bLogined)
	{ 
		unsigned long dwMoveZone = 0;

		if(!bLogined)
		{
			if(m_dwStatusFlag&MOVE_ZONE )
				dwMoveZone = MOVE_ZONE;
		}

		m_dwStatusFlag = bLogined ? (m_dwStatusFlag | IS_LOGINED) : (m_dwStatusFlag & (~IS_LOGINED));
		m_dwStatusFlag = (m_dwStatusFlag - dwMoveZone);
	}

    unsigned long GetGroup() { return ((m_dwStatusFlag & GROUP) >> 4); }

	bool IsMoveZone() { if(m_dwStatusFlag & MOVE_ZONE) return true; return false; }

	void SetMoveZone(unsigned long dwMoveZone)  { m_dwStatusFlag = (m_dwStatusFlag | MOVE_ZONE); }

    bool SetGroup(unsigned long dwGroup) 
    {
        if(dwGroup <= 0xF) { m_dwStatusFlag = (m_dwStatusFlag & (~GROUP)) | (dwGroup << 4); return true; }
        return false;
    }
};

struct DBBan
{
	enum { MAX_NAME = 16 };

	unsigned long   m_dwCID;	
	char            m_szName[MAX_NAME];	
};

struct DBBanInfo
{
	unsigned long	m_dwGID;				// 길드정보.
	unsigned short	m_wClass;				// 클래스정보.
	char			m_cLevel;				// 레벨정보.
};

struct BanInfo
{
    enum { MAX_NAME = 16 };
    unsigned long   m_dwCID;
    char            m_szName[MAX_NAME];

	unsigned long	m_dwGID;
	unsigned short	m_wClass;
	char			m_cLevel;

	unsigned long	m_dwServerID;	

    BanInfo() : m_dwCID(0) { }
};

// 추가 패킷
struct PktFriendAddReq : public PktBase
{
    enum { MAX_NAME = 16 };

    enum CMD
    {
        ADD_FRIEND_REQ      = 0, // 친구 등록      (Client->GameServer), m_szName는 등록할 사람의 이름.
        BAN_FRIEND_REQ      = 1, // 거부 등록      (Client->GameServer), m_szName은 등록할 사람의 이름.
        ADDED_INFO          = 2  // 친구가 등록됨  (GameServer->Client), m_szName은 나를 등록한 사람의 이름.
    };
   
    char            m_szName[MAX_NAME]; // 등록한 사람 이름.
    unsigned long   m_dwCID;            // ADDED_INFO일때만 CID를 넣어 준다.
    unsigned char   m_cCmd;
};


// 제거 패킷
struct PktFriendRemoveReq : public PktBase
{
	enum { MAX_NAME = 16 };

    enum CMD
    {
        REMOVE_FRIEND_REQ   = 0, // 친구 제거      (Client->GameServer), m_dwCID는 제거할 사람의 CID.
        REMOVE_BAN_REQ      = 1, // 거부 등록 제거 (Client->GameServer), m_dwCID는 제거할 사람의 CID.
		NOFRIEND_REQ		= 2, // 상대친구 리스트에서 자신을 삭제
    };
    
	char            m_szName[MAX_NAME]; // 삭제할 사람 이름.
    unsigned long   m_dwCID;       // 대상자 CID;
    unsigned char   m_cCmd;
};


// 기타 패킷
struct PktFriendEtc : public PktBase
{
    enum CMD
    {
        SETGROUP            = 0, // 그룹 등록     (Client->GameServer), 
                                 // m_dwCID는 그룹 세팅할 사람의 CID, m_dwData는 바뀔 그룹 번호
        
    };

    unsigned long   m_dwCID;
    unsigned long   m_dwData;
    unsigned char   m_cCmd;
};


// 친구 관련 패킷 ( Client <--> GameServer )
struct PktFriendAck : public PktBase
{
    enum CMD
    {
        UNKNOWN_ERROR_ACK   = 0, // 알 수 없는 커맨드 에러 (Ack)

        ADD_FRIEND_ACK      = 1, // 친구 등록 Ack (GameServer->Client)
                                 // 가능한 에러 : SERVER_ERROR (서버 에러)
                                 //               CLIENT_ERROR (클라이언트가 잘못된 커맨드를 주는 경우)
                                 //               NOT_LOGINED  (로그인되지 않은 캐릭터를 등록하려 함)
                                 //               LIST_FULL    (친구 리스트가 꽉 차 있음)
                                 //               REJECTED     (상대의 거부 리스트에 속해 있음)
                                 // m_dwCID : 등록 성공시 : 등록한 사람의 CID, 실패시 : 0

        MOVE_BAN_TO_FRIEND  = 2, // 친구 등록 Ack (GameServer->Client) 거부 리스트에서 삭제하고 친구를 추가함.
                                 // 가능한 에러 : SERVER_ERROR (서버 에러)
                                 //               CLIENT_ERROR (클라이언트가 잘못된 커맨드를 주는 경우)
                                 //               NOT_LOGINED  (로그인되지 않은 캐릭터를 등록하려 함)
                                 //               LIST_FULL    (친구 리스트가 꽉 차 있음)
                                 //               REJECTED     (상대의 거부 리스트에 속해 있음)
                                 // m_dwCID : 등록 성공시 : 등록한 사람의 CID, 실패시 : 0

        REMOVE_FRIEND_ACK   = 3, // 친구 제거 Ack (GameServer->Client)
                                 // 가능한 에러 : SERVER_ERROR (서버 에러)
                                 //               CLIENT_ERROR (클라이언트가 잘못된 커맨드를 주는 경우)
                                 //               NOT_IN_LIST  (요청하는 캐릭터가 리스트에 없어서 삭제할 수 없음)
                                 // m_dwCID : 0

        ADD_BAN_ACK         = 4, // 거부 등록 Ack (GameServer->Client)
                                 // 가능한 에러 : SERVER_ERROR (서버 에러)
                                 //               CLIENT_ERROR (클라이언트가 잘못된 커맨드를 주는 경우)
                                 //               NOT_LOGINED  (로그인되지 않은 캐릭터를 등록하려 함)
                                 //               LIST_FULL    (친구 리스트가 꽉 차 있음)
                                 // m_dwCID : 등록 성공시 : 등록한 사람의 CID, 실패시 : 0

        MOVE_FRIEND_TO_BAN  = 5, // 친구 둥록 Ack (GameServer->Client) 친구 리스트에서 삭제하고 거부를 추가함.
                                 // 가능한 에러 : SERVER_ERROR (서버 에러)
                                 //               CLIENT_ERROR (클라이언트가 잘못된 커맨드를 주는 경우)
                                 //               NOT_LOGINED  (로그인되지 않은 캐릭터를 등록하려 함)
                                 //               LIST_FULL    (친구 리스트가 꽉 차 있음)
                                 //               REJECTED     (상대의 거부 리스트에 속해 있음)
                                 // m_dwCID : 등록 성공시 : 등록한 사람의 CID, 실패시 : 0

        REMOVE_BAN_ACK      = 6, // 거부 제거 Ack (GameServer->Client)
                                 // 가능한 에러 : SERVER_ERROR (서버 에러)
                                 //               CLIENT_ERROR (클라이언트가 잘못된 커맨드를 주는 경우)
                                 //               NOT_IN_LIST  (요청하는 캐릭터가 리스트에 없어서 삭제할 수 없음)
                                 // m_dwCID : 0

        FRIEND_LOGIN        = 7, // 친구 로그인   (GameServer->Client), m_dwCID는 로그인한 친구의 CID, m_szName은 로그인한 친구의 이름
        FRIEND_LOGOUT       = 8, // 친구 로그아웃 (GameServer->Client), m_dwCID는 로그아웃한 친구의 CID, m_szName은 로그인한 친구의 이름

        FRIEND_LOGIN_TO_GAME    = 9,    // 친구 로그인      (Chat->GameServer)
                                        // m_dwCID는 로그인한 친구의 CID, m_szName은 로그인한 친구의 이름
                                        // 패킷 뒤에 캐릭터 CID개수(2byte unsigned long)
                                        // 캐릭터 CID가 CID개수만큼 따라온다. 해당 캐릭터들에게 패킷을 전송하면 된다.

        FRIEND_LOGOUT_TO_GAME   = 10,   // 친구 로그아웃    (Chat->GameServer)
                                        // m_dwCID는 로그아웃한 친구의 CID, m_szName은 로그인한 친구의 이름
                                        // 패킷 뒤에 캐릭터 CID개수(2byte unsigned long)
                                        // 캐릭터 CID가 CID개수만큼 따라온다. 해당 캐릭터들에게 패킷을 전송하면 된다.

		FRIEND_INFO_UPDATE		= 11,	// 친구 정보 업데이트.

		BAN_INFO_UPDATE			= 12,	// 거부 정보 업데이트.

		BAN_LOGIN				= 13,	// 거부 로그인.	(GameServer->Client)
		BAN_LOGOUT				= 14,	// 거부 로그아웃. (GameServer->Client)

		BAN_LOGIN_TO_GAME		= 15,	// 거부 로그인.	 (Chat->GameServer)
		BAN_LOGOUT_TO_GAME		= 16,   // 거부 로그아웃. (Chat->GameServer)

		// edith 2009.08.28 NoFriend 추가
		NOFRIEND_ACK			= 17,	// 친구 제거 Ack (GameServer->Client)
										// 가능한 에러 : SERVER_ERROR (서버 에러)
										//               CLIENT_ERROR (클라이언트가 잘못된 커맨드를 주는 경우)
										//               NOT_IN_LIST  (요청하는 캐릭터가 리스트에 없어서 삭제할 수 없음)
										// m_dwCID : 0
	};

    enum FriendERROR
    {
		SERVER_ERROR        = 1,    // 서버 에러
		CLIENT_ERROR        = 2,    // 클라이언트가 잘못된 커맨드를 주는 경우
		NOT_LOGINED         = 3,    // 로그인하지 않았음
		LIST_FULL           = 4,    // 리스트가 꽉 찼음
		REJECTED            = 5,    // 거부당했음
		NOT_IN_LIST         = 6,	// 요청하는 작업이 리스트에 없음
		FAIL_ENEMY			= 7		// 적군은 추가 불가능
    };

    enum { MAX_NAME = 16 };
        
    unsigned long   m_dwCID;
    char            m_szName[MAX_NAME];

	unsigned long	m_dwGID;		// 길드 아이디.
	unsigned short	m_wClass;		// 클래스.
	char			m_cLevel;		// 레벨.

	unsigned long	m_dwServerID;	// 서버 아이디.

    unsigned char   m_cCmd;
};


// 친구 관련 패킷
struct PktFriendDB : public PktDD
{
    enum CMD
    {
        ADD_FRIEND          = 0, // 친구 등록     (GameServer->DBAgent) Owner가 Reference를 등록.
        REMOVE_FRIEND       = 1, // 친구 제거     (GameServer->DBAgent) Owner가 Reference를 제거. 이때는 ReferenceUID = 0;
        ADD_BAN             = 2, // 거부 등록     (GameServer->DBAgent) Owner가 Reference를 등록. 
        REMOVE_BAN          = 3, // 거부 제거     (GameServer->DBAgent) Owner가 Reference를 제거. 이때는 ReferenceUID = 0;

        SETGROUP            = 4, // 그룹 세팅     (GameServer->DBAgent) 
                                 // Owner가 Reference를 m_dwData에 들어 있는 그룹으로 세팅. 이때는 ReferenceUID = 0;

        FRIEND_LIST         = 5, // 친구 리스트   (DBAgent->GameServer, GameServer->Client)
                                 // FriendList의 배열이 붙는다.
                                 // Reference는 의미 없음. m_dwData는 뒤에 붙는 데이터의 크기.

        BAN_LIST            = 6, // 거부 리스트   (DBAgent->GameServer, GameServer->Client)
                                 // BanInfo의 배열이 붙는다.
                                 // Reference는 의미 없음. m_dwData는 뒤에 붙는 데이터의 크기.
		
		FRIEND_INFO_UPDATE	= 7, // 정보 업데이트.

		FRIEND_INFO			= 8, // 정보 업데이트.
		BAN_INFO			= 9, // 정보 업데이트.
    };

    unsigned long   m_dwOwnerUID;       // 등록 하는 사람 UID
    unsigned long   m_dwOwnerCID;       // 등록 하는 사람 CID
    unsigned long   m_dwReferenceUID;   // 등록 되는 사람 UID
    unsigned long   m_dwReferenceCID;   // 등록 되는 사람 CID
    unsigned long   m_dwData;           // Data

	unsigned long	m_dwGID;			// 등록 되는 사람 길드 정보.
	unsigned short	m_wClass;			// 등록 되는 사람 클래스 정보.
	char			m_cLevel;			// 등록 되는 사람 레벨 정보.
	unsigned long	m_dwServerID;		// 등록 되는 사람 서버 정보.

    unsigned char   m_cCmd;
};

#pragma pack()

#endif
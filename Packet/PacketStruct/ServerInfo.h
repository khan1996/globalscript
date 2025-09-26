#ifndef _LOGIN_SERVER_INFO_
#define _LOGIN_SERVER_INFO_

#include <winsock2.h>
#include <windows.h>
#include <algorithm>

#pragma pack(1)


// -----------------------------------------------------------------------------
// 서버 정보 관련 구조체들 
typedef union SERVER_ID* LPSERVER_ID; 
union SERVER_ID
{
	// 서버군
	enum Group	
	{ 
		GROUP_TEST			= 0,	// 테스트
		GROUP_SOLPHISTIA	= 1,	// 솔피스티아
		GROUP_KARTERANT		= 2,	// 카르테란트
		GROUP_IRANSA		= 3,	// 이란샤
		GROUP_HIGHLAND		= 4,	// 하이랜드
		GROUP_ALDIA			= 5,	// 알디아
		GROUP_UNITE			= 6,	// 유니테
		GROUP_BATTLE_SERVER	= 7,	// 배틀 서버
		GROUP_US_TOURNAMENT	= 8,	// 토너먼트 (미국용)

        // Rodin : MAX_GROUP_NUM을 8로 하면 로긴 서버가 릴리즈에서 죽습니다. 왜?!?!
		MAX_GROUP_NUM		= 10
	}; 

	enum Zone	
	{ 
		NONE			= 0,
        ZONE1			= 1,		// 그랜드 코스트
		ZONE2			= 2,		// 레한 스팟
		ZONE3			= 3,		// X 구 배틀그라운드 (현재 사용안함)
		ZONE4			= 4,		// 사이너 아일랜드
		ZONE5			= 5,		// 아일랜드 오브 페인
		BATTLE_ZONE		= 6,		// X 배틀 존
		ADMIN_TOOL_ZONE = 7,		// X (클라이언트 사정으로 쓰이지 않음) - 운영툴이 사용하겠소.
		CAPITAL			= 8,		// 카나번
		ZONE9			= 9,		// X 구 NEWZONE5 번인데 글로벌에서 사용해야하기 때문에 ZONE9로 이전후 제거
        FRAG_ZONE		= 10,		// X 배틀 그라운드 전용 서버의 가상 존
		BATTLE_SERVER	= 11,		// X 배틀 그라운드 전용 서버
        ZONE12          = 12,       // 신의 대지
		STATUE_ZONE		= 13,		// X 배틀 그라운드 전용 서버의 가상 존
		PRISON			= 14,		// 감옥 (감금 시스템)
		CHAR_SELECT		= 15,		// X 캐릭터 선택 화면 존
		STONE_WAR1		= 16,		// 로우 석상전용
		STONE_WAR2		= 17,		// 로우 석상전용
		STONE_WAR3		= 18,		// 로우 석상전용

		MAX_ZONE_NUM	= 19
	};

	enum Channel
	{
        // sphawk : 채널 관련해서 고쳤습니다. 여기저기 복잡하게 흩어져 있으니 함부로 건들지 마세요.
        // 고친 패킷은 다음과 같습니다.
        // 1. 여기.
        // 2. PktSCInfo     (중계쪽에서도 사용하는 패킷입니다)
        
        // 3. PktCSAck
        // 4. PktSZMvAck

        // 5. PktCSDAck     (중계쪽에서도 사용하는 패킷입니다)
        // 6. PktSZMvDAck   (중계쪽에서도 사용하는 패킷입니다)

        // SERVER_GROUP쪽은 채널이라고 써 있지만 실제로는 휴먼/아칸 의 인원수입니다.(중계 소스를 보면 압니다.)
        // 착오 없으시길 바랍니다.
		MAX_CHANNEL_NUM	= 5
	};

	typedef struct SID* LPSID;
    struct SID
	{
		char Type;
		char Group;
		char Channel;
		char ID;
	};

	SID				sID;
	unsigned long	dwID;

    inline char GetType(void)       { return sID.Type;    }
	inline char GetChannel(void)	{ return sID.Channel; }
	inline char GetZone(void)		{ return sID.ID;	  }
	inline char GetGroup(void)		{ return sID.Group;   }
};


typedef struct SERVER* LPSERVER_GROUP;
struct SERVER_GROUP
{
	enum { CHANNEL_NUM = 2, SERVER_NAME = 15 };
	
	char			m_Group;					// 서버 그룹
	char			m_Name[SERVER_NAME];		// 서버 이름
 	long	        m_ClientNum[CHANNEL_NUM];	// 접속 자 수
	IN_ADDR         m_Address;					// 서버 주소
};

typedef struct SERVER* LPSERVER_LIST;
struct SERVER_LIST
{
	enum { SERVER_NUM = 10 };

	unsigned short		m_ServerNum;						// 서버 숫자
	SERVER_GROUP		m_ServerGroup[SERVER_NUM];			// 서버 그룹
};


#pragma pack()



typedef struct RylServerInfo* LPRylServerInfo;
struct RylServerInfo
{
    enum
    {
        GROUP_NAME_LENGTH       = 120,
        PATCH_ADDRESS_LENGTH    = 100
    };

	SERVER_ID		m_ServerUID;
    IN_ADDR         m_ServerAddress;
    unsigned long	m_dwClientVer;
    unsigned long   m_dwChecksum;
    
    size_t          m_nGroupNameLen;
    size_t          m_nPatchAddressLen;

    char            m_szGroupName[GROUP_NAME_LENGTH];        
    char            m_szPatchAddress[PATCH_ADDRESS_LENGTH];

    unsigned short	m_usChannelClientNum[SERVER_GROUP::CHANNEL_NUM];
    unsigned short	m_usChannelNum;

    RylServerInfo::RylServerInfo()
        :   m_dwClientVer(0), m_dwChecksum(0), 
            m_nPatchAddressLen(0), m_usChannelNum(0)
    {
        m_ServerUID.dwID = 0;
        m_ServerAddress.S_un.S_addr = 0;

        m_szPatchAddress[0] = '\0';

        strncpy(m_szGroupName, "UnKnown", GROUP_NAME_LENGTH);
        m_nGroupNameLen = strlen(m_szGroupName);

        std::fill_n(m_usChannelClientNum, int(SERVER_GROUP::CHANNEL_NUM), 0);
    }
};



#endif
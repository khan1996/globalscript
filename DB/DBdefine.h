///////////////////////////////////////////////////////////////////////////////////////////////
//
// DB Struct Define
//
//////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _DBDefine
#define _DBDefine

#include <algorithm>

#pragma pack(1)	// 1byte order로 패킹

///////////////////////////////////////////////////////////////////////////////////////////////
//
// 캐릭터 정보 DB Table
//
//////////////////////////////////////////////////////////////////////////////////////////////
struct CHAR_INFOST
{
    enum 
    { 
		// edith 2008.03.17 ID,PASS 길이조정
        MAX_ACCOUNT_LEN = 24,
        MAX_NAME_LEN    = 16 
    };

    unsigned long		CID;

    char				Name[MAX_NAME_LEN];	// 캐릭터 이름
    char				Sex;				// 캐릭터 성
    char				Hair;				// 캐릭터 머리 모양
    char 				Face;				// 캐릭터 얼굴 모양
    char				Race;				// 캐릭터 종족
    unsigned short		Class;				// 캐릭터 클래스

    unsigned long		Fame;				// 명성		
    unsigned long		Mileage;			// 마일리지			
    unsigned long		GID;				// 캐릭터 길드
    unsigned long		PID;				// 캐릭터 파티
    char				Level;				// 캐릭터 레벨

    unsigned long		Gold;				// 돈 

    unsigned short		IP;					// 증가 포인트
    unsigned short		STR;				// 캐릭터 STR
    unsigned short		DEX;				// 캐릭터 DEX
    unsigned short		CON;				// 캐릭터 CON
    unsigned short		INT;				// 캐릭터 INT
    unsigned short		WIS;				// 캐릭터 WIS

    short				HP;					// 캐릭터 HP
    short				MP;					// 캐릭터 MP	
    __int64				Exp;				// 캐릭터 경험점

    char				Chance;				// 캐릭터 찬스
};

typedef CHAR_INFOST* LPCHAR_INFOST;

///////////////////////////////////////////////////////////////////////////////////////////////
//
// 시간
//
//////////////////////////////////////////////////////////////////////////////////////////////
struct TIME
{
	unsigned short	Year;					// 연도
	unsigned short	Month;					// 달 
	unsigned short	Day;					// 날짜
	unsigned short	Hour;					// 시간
	unsigned short	Minute;					// 분
	unsigned short	Second;					// 초
	unsigned long	MSecond;				// 밀리초

};
typedef TIME* LPTIME;

///////////////////////////////////////////////////////////////////////////////////////////////
//
// 요시랜드 인증
//
//////////////////////////////////////////////////////////////////////////////////////////////
struct AUTH
{
	unsigned long			Errcode;			// 에러 코드
	unsigned long			UID;				// 유저 아이디
	union COMMENT
	{
		unsigned long		Point;				// 포인트 양
		TIME				Time;				// 시간
	} Comment;

	unsigned long			Dumy[4];			// 규격에 맞지 않은 사이즈의 데이터 대비
};

struct PAY_AUTH
{
	enum { AccountLen = 20 };

	unsigned long			Errcode;			    // 에러 코드
	char					Account[AccountLen];    // 계정 이름
	unsigned long			UID;				    // 유저 아이디
	unsigned long			Time;				    // 시간 (분)
    int                     WarningCode;            // 0 : 대만 경고보냄 / 1 : 중국 경고없음 / 2 : 중국 경고보냄
};

///////////////////////////////////////////////////////////////////////////////////////////////
//
// 한게임 인증
//
//////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _HAN_UID
{
	unsigned long			hUID;				// 유저 아이디
	unsigned long			lUID;				// 유저 아이디
}HAN_UID, *LPHAN_UID;

typedef struct _HAN_AUTH
{
	unsigned char			Num;				// 자리수
	unsigned short			Extra;				// 뭐하는 건지 모름
	unsigned long			UID;				// 유저 아이디
	unsigned char			Dump;				// 빈공간
}HAN_AUTH, *LPHAN_AUTH;

typedef union _HAN_CONVERT
{
	unsigned _int64			iValue;				// 64 Bit 값
	HAN_AUTH				sValue;				// 구조체
}HAN_CONVERT, *LPHAN_CONVERT;

///////////////////////////////////////////////////////////////////////////////////////////////
//
// 위치
//
//////////////////////////////////////////////////////////////////////////////////////////////
struct POS
{
	float			fPointX;				// Point X 좌표
	float			fPointY;				// Point Y 좌표
	float			fPointZ;				// Point Z 좌표
};

///////////////////////////////////////////////////////////////////////////////////////////////
//
// 스킬 슬롯	// modified 2002/10/3 11:20 by sparrowhawk
//
//////////////////////////////////////////////////////////////////////////////////////////////
union SKILLSLOT
{
	struct 
	{
		unsigned short	wSkill;				// 스킬 아이디
		char			cLockCount;			// 스킬 락 카운트	( 0~4 )
		char			cSkillLevel;		// 스킬 레벨		( -127 ~ 128 )
	}SKILLINFO;
	
	unsigned long		dwSkillSlot;

	SKILLSLOT() : dwSkillSlot(0) { }
	SKILLSLOT(unsigned short skill, char lockcount, char skillLevel)
		{ SKILLINFO.wSkill = skill; SKILLINFO.cLockCount = lockcount; SKILLINFO.cSkillLevel = skillLevel; }
};
typedef SKILLSLOT* LPSKILLSLOT;

///////////////////////////////////////////////////////////////////////////////////////////////
//
// 스킬			
//
//////////////////////////////////////////////////////////////////////////////////////////////
struct SKILL
{
    enum Const
    { 
        MAX_SKILL_LEVEL	= 6,
        MAX_SLOT_NUM	= 40 
    };

    unsigned short	wSkillNum;				// 스킬 수 (TODO : 사용하지 않습니다.)
    unsigned short	wSlotNum;				// 슬롯 수

    SKILLSLOT		SSlot[MAX_SLOT_NUM];	// 스킬

    SKILL() : wSkillNum(0), wSlotNum(0) 
    { 
        std::fill_n(&SSlot[0], int(MAX_SLOT_NUM), SKILLSLOT());
    }

    SKILL(unsigned short usSkillNum, unsigned short usSlotNum)
        : wSkillNum(usSkillNum), wSlotNum(usSlotNum) 
    { 
        std::fill_n(&SSlot[0], int(MAX_SLOT_NUM), SKILLSLOT());
    }

    unsigned short GetSkillNum(void)
    {
        unsigned short wCount = 0;
        for (int Index = 0; Index < wSlotNum; ++Index)
        {
			// 스킬아이디가 0x8000 보다 작으면 소설모션, 어빌리티 이다.
			if(SSlot[Index].SKILLINFO.wSkill < 0x8000)
				continue;

            wCount += (SSlot[Index].SKILLINFO.cLockCount * MAX_SKILL_LEVEL + 
                SSlot[Index].SKILLINFO.cSkillLevel);
        }

        return wCount;
    }

    unsigned short GetAbilityNum(void)
    {
        unsigned short wCount = 0;
        for (int Index = 0; Index < wSlotNum; ++Index)
        {
			if(SSlot[Index].SKILLINFO.wSkill >= 0x2000)
				continue;

            wCount += SSlot[Index].SKILLINFO.cLockCount;
        }

        return wCount;
    }
};
typedef SKILL* LPSKILL;


///////////////////////////////////////////////////////////////////////////////////////////////
//
// 타이틀
//
//////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _TITLE
{
	__int64			dlTitle;				// 타이틀 아이디
	unsigned short	wScore;					// 타이틀 점수
}TITLE, *LPTITLE;

///////////////////////////////////////////////////////////////////////////////////////////////
//
// 장비
//
//////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _EQUIP
{
	enum { MAX_EQUIP_SIZE = 1436 }; // + 4 = 1440

	unsigned long	dwSize;
	char			Data[MAX_EQUIP_SIZE];
} EQUIP, *LPEQUIP;

///////////////////////////////////////////////////////////////////////////////////////////////
//
// 인벤
//
//////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _INVEN
{
	enum { MAX_INVEN_SIZE = 5116 }; // + 4 = 5120

	unsigned long	dwSize;
	char			Data[MAX_INVEN_SIZE];
} INVEN, *LPINVEN;

///////////////////////////////////////////////////////////////////////////////////////////////
//
// Extra
//
//////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _EXTRA
{
	enum { MAX_EXTRA_SIZE = 508 }; // + 4 = 512

	unsigned long	dwSize;
	char			Data[MAX_EXTRA_SIZE];
} EXTRA, *LPEXTRA;

///////////////////////////////////////////////////////////////////////////////////////////////
//
// Exchange
//
//////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _EXCHANGE
{
	enum { MAX_EXCHANGE_SIZE = 1280 }; // + 4 = 1284

	unsigned long	dwSize;
	char			Data[MAX_EXCHANGE_SIZE];
} EXCHANGE, *LPEXCHANGE;

///////////////////////////////////////////////////////////////////////////////////////////////
//
// 임시 인벤토리
//
//////////////////////////////////////////////////////////////////////////////////////////////
struct TEMPINVEN
{
	enum { MAX_TEMPINVEN_SIZE = 1436 }; // + 4 = 1440

	unsigned long	dwSize;
	char			Data[MAX_TEMPINVEN_SIZE];
};

///////////////////////////////////////////////////////////////////////////////////////////////
//
// 창고
//
//////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _STORE
{
	enum { MAX_STORE_SIZE = 7296 }; // + 4 = 7300

	unsigned long	dwSize;
	char			Data[MAX_STORE_SIZE];
} STORE, *LPSTORE;

///////////////////////////////////////////////////////////////////////////////////////////////
//
// 퀵 슬롯
//
//////////////////////////////////////////////////////////////////////////////////////////////
struct QUICKSLOT {

	enum Type { NONE = 0, SKILL = 1, ITEM = 2 };
	char nType;
	char nSkillLockCount;
	char nSkillLevel;
	unsigned short wID;			// Skill_ID 또는 아이템 ProtoType ID
	
	QUICKSLOT() : nType(NONE), nSkillLockCount(0), nSkillLevel(0), wID(0) { }
	QUICKSLOT(char type, char lockCount, char skillLevel, unsigned short wid)
		:	nType(type), nSkillLockCount(lockCount), nSkillLevel(skillLevel), wID(wid) { }
};
typedef QUICKSLOT* LPQUICKSLOT;

struct QUICK
{
	enum { MAX_QUICK_NUM = 20 };
	QUICKSLOT Slots[MAX_QUICK_NUM];

	QUICK()
	{
		std::fill_n(&Slots[0], int(MAX_QUICK_NUM), QUICKSLOT());
	}

};
typedef QUICK *LPQUICK;

///////////////////////////////////////////////////////////////////////////////////////////////
//
// 캐릭터 생성 구조체
//
//////////////////////////////////////////////////////////////////////////////////////////////
struct CHAR_CREATE
{
	enum RaceType
	{ 
		HUMAN	= 0,
		AKHAN	= 1
	};

    char				Name[CHAR_INFOST::MAX_NAME_LEN];	// 캐릭터 이름
	char				Sex;				// 캐릭터 성
	char				Hair;				// 캐릭터 머리 모양
	char 				Face;				// 캐릭터 얼굴 모양
	char				Race;				// 캐릭터 종족
	unsigned short		Class;				// 캐릭터 클래스

	unsigned short		Equip[15];			// 장비

	unsigned short		STR;				// 캐릭터 STR
	unsigned short		DEX;				// 캐릭터 DEX
	unsigned short		CON;				// 캐릭터 CON
	unsigned short		INT;				// 캐릭터 INT
	unsigned short		WIS;				// 캐릭터 WIS
};

///////////////////////////////////////////////////////////////////////////////////////////////
//
// 캐릭터 모양 구조체
//
//////////////////////////////////////////////////////////////////////////////////////////////
struct CHAR_VIEW
{
	enum RaceType
	{ 
		HUMAN	= 0,
		AKHAN	= 1
	};

	unsigned long		CID;

	char				Name[CHAR_INFOST::MAX_NAME_LEN];	// 캐릭터 이름
	char				Sex;				// 캐릭터 성
	char				Hair;				// 캐릭터 머리 모양
	char 				Face;				// 캐릭터 얼굴 모양
	char				Race;				// 캐릭터 종족
	unsigned short		Class;				// 캐릭터 클래스

	unsigned long		Fame;				// 명성		
	unsigned long		Mileage;			// 마일리지			
	unsigned long		GID;				// 캐릭터 길드
	unsigned long		PID;				// 캐릭터 파티
	char				Level;				// 캐릭터 레벨

	unsigned short		Equip[15];			// 장비(겉보기)
};

// DB 에 저장하지는 않지만 CHAR_VIEW 에 관련된 추가 정보이다.
// 길드 및 국적 정보
struct sGuildData
{
	unsigned char	m_cNation;
	char			m_szName[CHAR_INFOST::MAX_NAME_LEN];
};

///////////////////////////////////////////////////////////////////////////////////////////////
//
// 캐릭터 정보 DB Table
//
//////////////////////////////////////////////////////////////////////////////////////////////
struct USER_INFO
{
    enum
    {
        MAX_CHAR_NUM = 5
    };
    
    unsigned long       CharID[MAX_CHAR_NUM];   // 캐릭터 

    // WORK_LIST 2.1 계정 국적 추가
    unsigned char		Nation;                 // 계정 국적

    // 서버통합 - by sphawk
    unsigned char       OldServerGroupID;       // 통합전 서버그룹ID (0이면 현재 서버군의 정보임을 나타냄)
    unsigned char       FirstLogin;             

    bool HasCharacter(unsigned long dwCID) const 
    {
        for(int nCount = 0; nCount < MAX_CHAR_NUM; ++nCount)
        {
            if(CharID[nCount] == dwCID) { return true; }
        }

        return false;
    }

    unsigned char GetCharacterNum() const
    {
        unsigned char cSelectedCharacter = 0;
        for(int nCount = 0; nCount < MAX_CHAR_NUM; ++nCount)
        {
            if(0 != CharID[nCount]) { ++cSelectedCharacter; }
        }

        return cSelectedCharacter;
    }

};
typedef USER_INFO *LPUSER_INFO;


///////////////////////////////////////////////////////////////////////////////////////////////
//
// 캐릭터 추가 정보
//
//////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _CHAR_INFOEX
{
	unsigned long		Total;				// 총 이용 시간(분)
	unsigned long		ServerID;			// 존
    unsigned char       cNameChangeCount;   // 이름 바꿀 수 있는 횟수
	unsigned char		cAdminFlag;			// Admin 모델 정보.
	unsigned char		GuildWarFlag;		// 길드 전쟁 참여 플래그
	unsigned char		RealmWarFlag;		// 국가 전쟁 참여 플래그

	unsigned char		RealmPoint;			// 국가 전쟁 공헌훈장 포인트.

	unsigned char		RealmCheckPoint;	// 국가 전쟁 중에 로그아웃해서 포인트를 얻었을 경우.

	unsigned char		RealmMinute;		// 국가 전쟁 플레이 타임 기록.
	
	TIME				LoggoutTime;		// 로그아웃된 시간.

}	CHAR_INFOEX, *LPCHAR_INFOEX;

///////////////////////////////////////////////////////////////////////////////////////////////
//
// 캐릭터 위치 정보 DB Table
//
//////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _CHAR_POS
{
	POS	LastPoint;							// 마지막 위치
	POS	SavePoint;							// 세이브 위치
}	CHAR_POS, *LPCHAR_POS;

///////////////////////////////////////////////////////////////////////////////////////////////
//
// 파티 정보 DB Table
//
//////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _PARTY_INFO
{
	enum { MAX_MEM = 10 };

	unsigned long	MemberCID[MAX_MEM];							// 멤버 CID
	char			Name[MAX_MEM][CHAR_INFOST::MAX_NAME_LEN];	// 멤버 이름 	

}	PARTY_INFO, *LPPARTY_INFO;

typedef struct _PARTY_USER_INFO
{
	enum { MAX_MEM = 10 };

	unsigned short	m_wClass[MAX_MEM];							// 멤버 클래스 
	char			m_cLevel[MAX_MEM];							// 멤버 레벨
	unsigned long	m_dwGID[MAX_MEM];							// 멥버 길드 아이드

}	PARTY_USER_INFO, *LPPARTY_USER_INFO;

typedef struct _PARTY_DB_INFO									// 디비에서 얻어올때 사용.
{
	PARTY_INFO			PartyInfo;
	PARTY_USER_INFO		PartyUserInfo;

} PARTY_DB_INFO, *LPPARTY_DB_INFO;
///////////////////////////////////////////////////////////////////////////////////////////////
//
// 파티
//
//////////////////////////////////////////////////////////////////////////////////////////////
struct PARTY
{
	enum { MAX_MEM = 10 };

	unsigned long	m_dwPartyID;
	unsigned long	m_dwLeaderID;	

	unsigned char	m_cMemberNum;

	char			Name[MAX_MEM][CHAR_INFOST::MAX_NAME_LEN];
	unsigned long	MemberCID[MAX_MEM];			
	unsigned long	ServerID[MAX_MEM];

	unsigned short	m_wClass[MAX_MEM];					// 멤버 클래스 
	char			m_cLevel[MAX_MEM];					// 멤버 레벨
	unsigned long	m_dwGID[MAX_MEM];					// 멤버 길드 아이드

	PARTY() : m_dwPartyID(0), m_dwLeaderID(0), m_cMemberNum(0)
	{
		std::fill_n(&MemberCID[0], int(MAX_MEM), 0);
		std::fill_n(&ServerID[0], int(MAX_MEM), 0);

		std::fill_n(&m_wClass[0], int(MAX_MEM), 0);
		std::fill_n(&m_cLevel[0], int(MAX_MEM), 0);	
		std::fill_n(&m_dwGID[0], int(MAX_MEM), 0);	
	}
};

struct PARTY_USERINFO
{
	unsigned long	m_dwGID[PARTY::MAX_MEM];
};

// 파티 정보(길드정보 추가) //

struct PARTY_EX : public PARTY
{		
    bool			m_bAutoRouting[PARTY::MAX_MEM];
	POS				m_Position[PARTY::MAX_MEM];

	PARTY_EX()
	{				
        InitMemberAutoRouting();
        InitMemberPos();
    }

	PARTY_EX(const PARTY& party)
	{
		m_dwPartyID		= party.m_dwPartyID;
		m_dwLeaderID	= party.m_dwLeaderID;
		m_cMemberNum	= party.m_cMemberNum;		

		for (int nIndex = 0; nIndex < PARTY::MAX_MEM; ++nIndex)
		{
			::memcpy(Name[nIndex], party.Name[nIndex], CHAR_INFOST::MAX_NAME_LEN);
			
			MemberCID[nIndex]	= party.MemberCID[nIndex];
			ServerID[nIndex]	= party.ServerID[nIndex];
			m_wClass[nIndex]	= party.m_wClass[nIndex];
			m_cLevel[nIndex]	= party.m_cLevel[nIndex];
			m_dwGID[nIndex]		= party.m_dwGID[nIndex];
		}

        InitMemberAutoRouting();
        InitMemberPos();
	}

    inline void InitMemberAutoRouting()
    {
        std::fill_n(&m_bAutoRouting[0], int(PARTY::MAX_MEM), false);
    }

    inline void InitMemberPos()
    {
		for (int i = 0; i < PARTY::MAX_MEM; ++i)
		{
			m_Position[i].fPointX = 0.0f;
			m_Position[i].fPointY = 0.0f;
			m_Position[i].fPointZ = 0.0f;
		}
    }

};

///////////////////////////////////////////////////////////////////////////////////////////////
//
// 창고
//
//////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _STORE_INFO
{
	enum { MAX_PASS_LEN = 5 };

	char			Password[MAX_PASS_LEN];			// 패스워드 
	unsigned long	Flag;							// 플래그
	unsigned long	Gold;							// 플래그

}	STORE_INFO, *LPSTORE_INFO;

///////////////////////////////////////////////////////////////////////////////////////////////
//
// 운영자
//
//////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _ADMIN
{
	enum	{	MAX_IP_LEN = 16	};

	unsigned long	UID;
	unsigned short	Level;
	char			IP_S[MAX_IP_LEN];
	char			IP_E[MAX_IP_LEN];
}	ADMIN, *LPADMIN;

///////////////////////////////////////////////////////////////////////////////////////////////
//
// 프랜드
//
//////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _FRIEND
{
	enum
	{ 
		MAX_FRIEND_SIZE		= 2400,		// + 4 = 2404
		MAX_FRIENDINFO_SIZE	= 700,		// + 4 = 704 (캐릭터 정보 추가.)
	};

	unsigned long	dwSize;
	char			Data[MAX_FRIEND_SIZE];	

	unsigned long	dwInfoSize;						//
	char			Info[MAX_FRIENDINFO_SIZE];		

} FRIEND, *LPFRIEND;

typedef struct _BAN
{
	enum
	{ 
		MAX_BAN_SIZE		= 2000,			// + 4 = 2004
		MAX_BANINFO_SIZE	= 700,			// + 4 = 704 (캐릭터 정보 추가.)
	};	

	unsigned long	dwSize;
	char			Data[MAX_BAN_SIZE];

	unsigned long	dwInfoSize;
	char			Info[MAX_BANINFO_SIZE];

} BAN, *LPBAN;

///////////////////////////////////////////////////////////////////////////////////////////////
//
// 과금 관련
//
//////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _RE_USPCheckBilling_Login
{
	unsigned long   Flag;						// Success(=0)
	char            strBillingType[2];			// D, T(N: 무료계정)
	int             PlayTime;					// 현재 남은 시간 (일, 분)
	unsigned long   intCRMIndex1;				// 피시방 고유아이디
} RE_USPCheckBilling_Login, *LPRE_USPCheckBilling_Login;

typedef struct _RE_USPGRoomCurrent_DisConn
{
	enum	{ ClientIDLen = 20 };

	unsigned char			ClientID[ClientIDLen];		// 계정
	unsigned long			UID;						// 유저 아이디
	char					ServerID;					// 서버 아이디
}RE_USPGRoomCurrent_DisConn, *LPRE_USPGRoomCurrent_DisConn;


///////////////////////////////////////////////////////////////////////////////////////////////
//
// 계정 블록 관련
//
//////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _RE_CMS_CheckUserBlock
{
	unsigned char   m_cFlags;           //  0 : 블럭 안함, 1 : 계정 블럭, 2 : 캐릭터 블럭  
    char            m_szCharName[20][USER_INFO::MAX_CHAR_NUM];  //  블럭된 캐릭터 이름

} RE_CMS_CheckUserBlock, *LPRE_CMS_CheckUserBlock;

///////////////////////////////////////////////////////////////////////////////////////////////
//
// 퀘스트
//
//////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _QUEST
{
	enum { MAX_QUEST_SIZE = 260 }; // + 4 = 264

	unsigned long	dwSize;
	char			Data[MAX_QUEST_SIZE];
} QUEST, *LPQUEST;

typedef struct _HISTORY
{
	enum { MAX_HISTORY_SIZE = 400 };	// + 4 = 404

	unsigned long	dwSize;
	char			Data[MAX_HISTORY_SIZE];
} HISTORY, *LPHISTORY;

///////////////////////////////////////////////////////////////////////////////////////////////
//
// 설정
//
//////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _CONFIG
{
	enum { MAX_CONFIG_SIZE = 46 }; // + 4 = 50

	unsigned long	dwSize;
	char			Data[MAX_CONFIG_SIZE];
} CONFIG, *LPCONFIG;


///////////////////////////////////////////////////////////////////////////////////////////////
//
// 특수 소설모션 
//
//////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _SOCIALEX
{
	enum { MAX_SOCIAL_SIZE = 20 }; // + 4 = 24

	unsigned long	dwSize;
	char			Data[MAX_SOCIAL_SIZE];
} SOCIALEX, *LPSOCIALEX;


///////////////////////////////////////////////////////////////////////////////////////////////
//
// 이벤트 아이템
//
//////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _EVENTITEM
{
	enum { MAX_ITEM_NAME = 32 };

	unsigned long	nItemIndex;
	char			strItemName[MAX_ITEM_NAME];
	unsigned long	nItemAmount;
	TIME			UpdateTime;

} EVENTITEM, *LPEVENTITEM;


// 서버통합 - 캐릭터 생성을 파라미터로 DB에 호출.
struct CHAR_CREATE_DB
{
    unsigned long   dwUID;
    unsigned long   dwSlotPos;
    char            szName[CHAR_INFOST::MAX_NAME_LEN];
    unsigned char   cSex;
    unsigned char   cHair;
    unsigned char   cFace;
    unsigned char   cRace;
    short           sClass;
    unsigned long   dwGold;
    short           sSTR;
    short           sDEX;
    short           sCON;
    short           sINT;
    short           sWIS;
    float           fPointX;
    float           fPointY;
    float           fPointZ;
    unsigned short  Equip[15];
    SKILL           skill;
    unsigned char   cServerGroupID;
    int             dwCID_Out;
};

// 서버통합 - 통합된 캐릭터 정보를 얻어온다.
struct UnifiedCharData
{
    char            szName[16];
    char            szBeforeCharName[20];
    unsigned long   dwNewCID;
    unsigned long   dwBeforeCID;
    short           sClass;
    unsigned char   cLevel;
    unsigned char   cOldServerGroupID;
};

struct UnifiedStoreInfo
{
    unsigned char   cOldServerGroupID;     // 구 서버그룹

    char			Password[STORE_INFO::MAX_PASS_LEN];	// 패스워드 
    unsigned long	Flag;					            // 플래그
    unsigned long	Gold;					            // 금액
};

struct KeyInfo
{
	enum Const
	{
		MAX_KEY_INFO	= 34
	};

	unsigned char	m_cUsed;

	unsigned long	m_dwKeyInfo[MAX_KEY_INFO];
};

///////////////////////////////////////////////////////////////////////////////////////////////
//
// 스펠
//
//////////////////////////////////////////////////////////////////////////////////////////////
struct SPELLINSTANCE {
	unsigned long	dwDurationSec;
	unsigned short	wSpellID;
	unsigned short	wEnchantLevel;
	unsigned char	cSpellType; 

	SPELLINSTANCE() : dwDurationSec(0), wSpellID(0), wEnchantLevel(0), cSpellType(0) { }
	SPELLINSTANCE(unsigned long dwDurationSec, unsigned short wSpellID, unsigned short wEnchantLevel, unsigned char cSpellType)
		:	dwDurationSec(dwDurationSec), wSpellID(wSpellID), wEnchantLevel(wEnchantLevel), cSpellType(cSpellType) { }
};
typedef SPELLINSTANCE* LPSPELLINSTANCE;

struct SPELL
{
	enum { MAX_SPELL_NUM = 30 };
	SPELLINSTANCE Spells[MAX_SPELL_NUM];

	SPELL()
	{
		std::fill_n(&Spells[0], int(MAX_SPELL_NUM), SPELLINSTANCE());
	}
};
typedef SPELL* LPSPELL;

#pragma pack()

#endif
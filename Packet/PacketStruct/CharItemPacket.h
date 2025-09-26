#ifndef _CHAR_ITEM_PACKET_H_
#define _CHAR_ITEM_PACKET_H_

// CharItemPacket.h
// ItemStructure.h을 포함해야 한다. (집기, 떨구기 등등)

#include <Item/ItemStructure.h>
#include <Network/Packet/PacketBase.h>
#include <Network/Packet/PacketStruct/DataPacket.h>

#include "CharItemPacketStruct.h"

#pragma pack(1)

// 집기 (응답 필요)
struct PktPU : public PktBase
{
    unsigned __int64    m_nObjectID;    // 오브젝트 아이디
	unsigned long	    m_dwCharID;     // 캐릭터 아이디
    Item::ItemPos       m_itemPos;      // 아이템 위치
};

// 집기 Ack
struct PktPUAck : public PktBase
{
	enum ObjectType { Item = 0, Gold = 1 };
	enum PktPUAckError 
    {
		NO_SERVER_ERR			= 0,
        SERVER_ERROR			= 1,
        FAIL_GET_CELL			= 2,
        FAIL_GET_ITEM			= 3,
        FAIL_PICKUP_INVEN		= 4,
		FAIL_NOT_OWNER_OF_ITEM	= 5,
		FAIL_NOT_ITEM			= 6,
		FAIL_CREATE_ITEM		= 7,
		FAIL_AUTO_ROUTING		= 8,
		FAIL_GOLD_OVERFLOW		= 9
    };

    unsigned __int64	m_nObjectID;	// 오브젝트 아이디
	unsigned long		m_dwCharID;	    // 캐릭터 아이디	
	unsigned long		m_dwSize;       // Object크기(혹은 돈 양)

    Item::ItemPos		m_itemPos;      // 아이템이 놓일 위치
	unsigned char		m_cType;        // Object종류(아이템 혹은 돈)
   	unsigned char		m_cNum;			// 아이템 갯수
};

// 집기 정보
struct PktPUInfo : public PktBase
{
    unsigned __int64	m_nObjectID;	// 오브젝트 아이디
	unsigned long		m_dwCharID;     // 캐릭터 아이디	
};

// 오토 루팅
struct PktAutoRouting : public PktBase
{
	enum PktARCmd
	{
		ARC_ORDER		= 0,
		ARC_POSSIBLE	= 1,
		ARC_IMPOSSIBLE	= 2
	};

    unsigned __int64	m_nObjectID;	// 오브젝트 아이디
	unsigned long		m_dwCharID;     // 캐릭터 아이디	

	unsigned short		m_wItemID;		// 아이템 아이디
	unsigned char		m_cNum;			// 아이템 갯수

	Item::ItemPos		m_itemPos;      // 아이템 위치

	unsigned char		m_cCmd;			// 명령
};

// 떨구기 (응답 필요)
struct PktPD : public PktBase 
{
	unsigned long	m_dwCharID;		// 캐릭터 아이디
    Item::ItemPos   m_itemPos;      // 아이템 위치 및 인덱스
    unsigned char   m_cNum;         // 아이템 갯수
};

// 떨구기 Ack
struct PktPDAck : public PktBase 
{
	enum PktPDAckError
    {	
		SERVER_ERROR	= 1,
        FAIL_GET_CELL	= 2,
        FAIL_DROP_INVEN	= 3,
        FAIL_DROP_FIELD	= 4,
        FAIL_ZERO_ITEM	= 5
	};

	unsigned long	m_dwCharID;     // 캐릭터 아이디
    FieldObject		m_FieldObject;  // FieldObject
    Item::ItemPos   m_itemPos;      // 아이템 위치 및 인덱스
};

// 떨구기 정보
struct PktPDInfo : public PktBase 
{
	unsigned long	m_dwCharID;
	FieldObject		m_FieldObject;
};

// 사라지기
struct PktDisappearItem : public PktBase
{
	enum PktDICmd
	{
		DIC_DEFAULT			= 0,	// 일반적인 아이템 삭제
		DIC_LINKED_QUEST	= 1		// 퀘스트 종속 아이템의 삭제 (메세지 출력 필요)
	};

	unsigned long	m_dwCharID;		// 캐릭터 아이디
	
	Item::ItemPos   m_itemPos;      // 아이템 위치 및 인덱스
	unsigned char   m_cNum;         // 아이템 갯수

	unsigned char	m_cCmd;			// 명령
};


/* TODO : REMOVE HERE 

셀 정보
struct PktCeInfo : public PktBase
{
    struct Stall_Info
    {
        enum { MAX_STALL_NAME = 32 };

        unsigned long   m_dwCID;
        char            m_szStallName[MAX_STALL_NAME];
    };

	unsigned char   m_cItemNum;     // 뒤에 따라오는 아이템 개수.
    unsigned char   m_cStallNum;    // 뒤에 따라오는 상점 이름 개수.    
};
*/

// 아이템 처리 (응답 필요)
struct PktTI : public PktBase 
{
	enum PktTIError
    {
        SERVER_ERROR	= 1,
        FAIL_MOVE	    = 2,
		FAIL_MOVE_FIFTHSKILL_LOCKITEM = 3
    };

	unsigned long	m_dwCharID;     // 캐릭터 아이디
	TakeType		m_TakeType;     // 아이템 집기 구조체
};

// 돈 처리 (응답 필요)
struct PktTG : public PktBase
{
	enum PktTGError
    {
        SERVER_ERROR	= 1,
        FAIL_MOVE       = 2,
        GOLD_OVERFLOW   = 3
    };

	unsigned long	m_dwCharID;     // 캐릭터 아이디
	unsigned long	m_dwGold;       // 돈 액수

    unsigned char	m_cSrcPos : 4;  // Src위치 - TakeType 참조
	unsigned char	m_cDstPos : 4;  // Dst위치 - TakeType 참조
};

// 아이템 처리 (응답 필요)
struct PktTIs : public PktBase 
{
    enum PktTlsError
    {
        SERVER_ERROR    = 1
    };

	unsigned long	m_dwCharID;     // 캐릭터 아이디
	unsigned char	m_TakeNum;      // 아이템 집기 숫자
};

// 아이템 스왑
struct PktSwI : public PktBase 
{
	enum PktSwlError
    {
        SERVER_ERROR	= 1,
        FAIL_SRC_MOVE	= 2,
        FAIL_DST_MOVE	= 3,
        CANT_SWAP       = 4
    };

	unsigned long	m_dwCharID;     //	- 캐릭터 아이디
	TakeType		m_SwapSrc;      //	- 스왑 소스 아이템 집기 구조체
	TakeType		m_SwapDst;      //	- 스왑 대상 아이템 집기 구조체
};

// 아이템 거래
struct PktTr : public PktBase
{
	enum PktTrError
    {
        FAIL_DROP_INVEN		= 2,		// 인벤토리에 아이템이 없어 팔기 실패
        FAIL_WRONG_NPC		= 3,		// 잘못된 NPC
        FAIL_ITEM_BUY		= 4,		// 아이템 구입 실패
        FAIL_PUT_INVEN		= 5,		// 인벤토리에 아이템 놓기 실패
		FAIL_FULL_INVEN		= 6,		// 인벤토리가 꽉 찬 상태
		FAIL_ENEMY_ITEM		= 7,		// 공성시간에 적대 길드 요새 상점에서 아이템 구매 실패.
		FAIL_GOLD_OVERFLOW	= 8			// 거래후 최대 소지금 초과
    };

	enum PktTrCmd
	{
		TRC_BUY_WITH_GOLD			= 0,	// 아이템 구입 (일반 상점)
		TRC_BUY_WITH_MILEAGE		= 1,	// 아이템 구입 (메달 상점)
		TRC_SELL					= 2,	// 아이템 판매
		TRC_BUY_EQUIP				= 3,	// 랜덤 옵션이 붙은 장비 구입
		TRC_BUY_WITH_SKILL_COUPON	= 4,		// 아이템 구입 (상위 트레이너)
	};

	unsigned long	m_dwCustomerID;	// 손님 아이디
	unsigned long	m_dwOwnerID;	// 상점 주인 아이디 (NPC 또는 캐릭터)

	unsigned char	m_cCmd;			// 명령

	unsigned short	m_wBuyItemID;   // 사려고 하는 아이템 종류 ID
	TakeType		m_TakeType;		// 아이템 집기 구조체

	Item::ItemPos   m_CouponPos;	// 스킬북 쿠폰 위치 (TRC_BUY_WITH_SKILL_COUPON 명령의 경우 돈 대신 쿠폰을 사용한다.)

	unsigned short	m_wSize;		// 아이템 크기 (TRC_BUY_EQUIP 명령의 경우 '가변길이 아이템'이 덧붙는다.)

	// Rodin : 아이템 전체가 아닌 속성 배열을 넣어 패킷 길이 체크와 더불어 해킹 위험을 줄이도록 수정합시다.
};

// 아이템 거래 Ack (+ 가변길이 아이템)
struct PktTrAck : public PktBase 
{
    unsigned long   m_dwCharID;     // 캐릭터 아이디
    unsigned long   m_dwNPCID;      // 상점 NPC 아이디

	unsigned long	m_dwGold;       // 현재 소지금
	unsigned long	m_dwMileage;	// 현재 마일리지

	Item::ItemPos   m_CouponPos;	// 스킬북 쿠폰 위치 (TRC_BUY_WITH_SKILL_COUPON 명령의 경우 돈 대신 쿠폰을 사용한다.)

	unsigned short	m_wSize;        // 아이템 크기
    Item::ItemPos   m_itemPos;      // 아이템 위치
	unsigned char	m_cNum;			// 아이템 갯수
};

// NPC 장비 상점 정보 (+ Ack시엔 가변 길이 아이템 목록)
struct PktEquipShopInfo : public PktBase
{
	unsigned long	m_dwCharID;		// 캐릭터 아이디
	unsigned long	m_dwNPCID;		// NPC 아이디

	unsigned long	m_dwTime;		// 물품 재진열 시간 (그레이드가 새로 책정된 시간)

	unsigned char	m_cRace;		// 종족
	unsigned char	m_cTabPage;		// 탭

	unsigned char	m_cNum;			// 아이템 갯수
};

// 아이템의 수리 (응답 필요)
struct PktRpI : public PktBase
{
	enum Err
	{
		NOT_ENOUGH_GOLD	= 2,				// 가지고 있는 돈이 부족
		FAIL_NOT_CREATURE = 10,				// 크리에이쳐가 존재하지 않습니다.
		FAIL_NOT_NPCZONE = 11,				// NPC의 존번호가 다릅니다.
		FAIL_NOT_REPAIR = 12,				// 리페어아이템이없다.
		FAIL_NOT_POSITEM = 13,				// 아이템이 위치에없다.
		FAIL_NOT_STACKABLE = 14,			// 소모성아이템이다.
		FAIL_NOT_EQUIP = 15,				// 장비아이템이아니다.
		FAIL_FULL_DRUA = 16,				// 내구도가 풀이다.
	};

	enum Pos
	{
		EQUIPMENT       = 1,
		INVENTORY       = 2,
		EQUIPMENT_ALL   = 3,
		INVENTORY_ALL   = 4,
	};

	unsigned long	m_dwCharID;	    // 캐릭터 아이디
	unsigned long	m_dwGold;       // 수리 비용
    Item::ItemPos   m_itemPos;      // 수리 아이템 위치
};

// 장비하고 있는 모든 아이템 수리 (응답 필요)
struct PktRpAI : public PktBase
{
	enum Err
	{
		NOT_ENOUGH_GOLD	= 2,				// 가지고 있는 돈이 부족
		FAIL_NOT_CREATURE = 10,				// 크리에이쳐가 존재하지 않습니다.
		FAIL_NOT_NPCZONE = 11,				// NPC의 존번호가 다릅니다.
		FAIL_NOT_REPAIR = 12,				// 리페어아이템이없다.
	};

	unsigned long	m_dwCharID;	    // 캐릭터 아이디
	unsigned long	m_dwGold;       // 수리 비용
};

// 아이템의 사용 (응답 필요)
struct PktUI : public PktBase
{
	enum PktUIAck
    {
        USE_FAILED					= 2,
												/* 공성 관련 아이템 사용 에러 */
		NOT_SIEGE_TIME				= 3,		// 공성 시간이 아님
		NOT_EXIST_GUILD				= 4,		// 길드가 없거나, 길드에 가입하지 않았음
		NOT_MASTER					= 5,		// 길드 마스터가 아님
		NOT_ENOUGH_FAME				= 6,		// 길드 명성 부족
		ANOTHER_DEV_CAMP			= 7,		// 구축중, 취소중, 파괴중인 다른 진지가 있음
		INNER_RADIUS				= 8,		// 다른 진지나 성의 반경 안에 있음
		OUTER_RADIUS				= 9,		// 아군 진지의 영역 밖에 있음
		NOT_ENOUGH_MATERIAL			= 10,		// 공성 병기 생성 자재 부족
		IN_SAFETYZONE				= 11,		// 진지를 세울수 없는 곳임
		FAIL_PEACE_GUILD			= 12,		// 평화 모드 길드
		FAIL_CAPITAL_ZONE			= 13,		// 카나번에만 생성 가능
//		FAIL_ALMIGHTY_GROUND		= 13,		// 신의 대지에는 생성 할수 없음
		FAIL_STONE_BATTLE			= 14,		// 다크 카나번에는 생성 할수 없음
		FAIL_EXIST_ANOTHER_WEAPON	= 15,		// 이미 월드 웨폰이 존재함
		FAIL_ALMIGHTY_PIRATE		= 16,		// 신의 해적단은 생성 불가
		FAIL_REMAIN_3_DAY			= 17,		// 3번의 공성 시간후에 가능
		FAIL_REMAIN_2_DAY			= 18,		// 2번의 공성 시간후에 가능
		FAIL_REMAIN_1_DAY			= 19,		// 1번의 공성 시간후에 가능
		FAIL_POSITION				= 20,		// 위치가 이상함

		NOT_EXIST_QUEST				= 21,		// 퀘스트가 존재하지 않음
		FAIL_HAS_QUEST				= 22,		// 이미 가지고 있는 퀘스트
		FAIL_FULL_QUEST				= 23,		// 받을 수 있는 퀘스트수가 꽉 찼음
		FAIL_QUEST_LEVEL			= 24,		// 퀘스트를 수행할수 있는 레벨 아님

		FAIL_NOT_GT_GUILD_WAR_TIME	= 25,		// 길드전 시간이 아님

		OUTER_RADIUS_SIEGE			= 26,		// 영역 밖에서 공성 병기를 생성
		FAIL_HAS_SIEGE_ARMS			= 27,		// 1개 이상에 공성 병기를 생성 하려고 할때.		
		FAIL_MATERIAL_COUNT			= 28,		// 공성용 자재가 부족할때.

		NOT_GUILD_MASTER			= 29,		// 길드 마스터가 아닐 경우.
		NOT_CASTLE_NATION			= 30,		// 공성측일 경우.

		FAIL_MAX_SIEGE_OBJECT		= 31,		// 충차가 최대값으로 생성되었을 경우.

		FAIL_NEWZONE				= 32,		// 신규존에서는 생성 할수 없음

		NOT_LIMIT_LEVEL				= 33		// 80 레벨 미만 일 경우.
    };

	unsigned long	m_dwSender;         // 사용자 아이디
	unsigned long	m_dwRecver;		    // 대상자 아이디
	Item::ItemPos   m_itemPos;          // 사용 아이템 위치
    unsigned char   m_cRemainItemNum;   // 남은 아이템 개수. 서버->클라이언트로 보내는 패킷일 경우에만 의미 있음.
};

// 오브젝트 던지기 ( 응답 필요 )
struct PktCO : public PktBase
{
    CastObject		m_sCastObject;
	unsigned long	m_dwSenderID;       // 던지는 사람
    unsigned long	m_dwReceiverID;	    // 받는 사람
};

// 오브젝트 던지기 (Cast Object)
struct PktCOInfo : public PktBase
{
    CastObject		m_sCastObject;
	unsigned long	m_dwSenderID;       // 던지는 사람
    unsigned long	m_dwReceiverID;	    // 받는 사람
};

// 소켓 설치
struct PktIS : public PktBase 
{
    unsigned long   m_dwCharID;			// 캐릭터 아이디
    Item::ItemPos   m_equipPos;         // 장비 위치
    Item::ItemPos   m_gemPos;           // 보석 위치
	unsigned char	m_PickingNum; // dedi add this
};

// 소켓 설치 Ack (+ 소켓이 설치된 장비 아이템)
struct PktISAck : public PktBase 
{
    unsigned long   m_dwCharID;			// 캐릭터 아이디
    Item::ItemPos   m_equipPos;         // 장비 위치
    Item::ItemPos   m_gemPos;           // 보석 위치
	unsigned char	m_cSize;            // 아이템 크기
	unsigned char m_PickingNum; //dedi add this
};

// 룬 설치/제거.
struct PktIRS : public PktBase
{
	enum
	{
		RUNE_INSTALL	= 0x02,
		RUNE_UNINSTALL	= 0x03,		
	};

	unsigned long	m_dwCharID;			// 캐릭터 아이디.
	Item::ItemPos	m_equipPos;			// 장비 위치.
	Item::ItemPos	m_runePos;			// 룬 위치.	
	unsigned char	m_cType;			// 패킷 타입(설치/제거).
};

// 룬 설치/제거 Ack.
struct PktIRSAck : public PktBase
{
	enum
	{
		RUNE_INSTALL	= 0x02,
		RUNE_UNINSTALL	= 0x03,		
	};

	unsigned long   m_dwCharID;			// 캐릭터 아이디
	Item::ItemPos	m_equipPos;			// 장비 위치.
	Item::ItemPos	m_runePos;			// 룬 위치.
	unsigned char	m_cSize;			// 아이템 크기.
	unsigned char	m_cType;			// 패킷 타입(설치/제거).
};

// 아이템 합성
struct PktItemChemical : public PktBase
{
	unsigned long	m_dwCharID;			// 캐릭터 아이디

	Item::ItemPos	m_pickkingPos;		// 들고 있는 아이템 위치
	Item::ItemPos	m_targetPos;		// 합성 대상 아이템 위치

	unsigned char	m_cPickkingNum;		// 들고 있는 아이템 갯수
};

// 아이템 합성 Ack
struct PktItemChemicalAck : public PktBase
{
	unsigned long	m_dwCharID;			// 캐릭터 아이디

	Item::ItemPos	m_pickkingPos;		// 들고 있는 아이템 위치
	Item::ItemPos	m_targetPos;		// 합성 대상 아이템 위치

	unsigned short	m_wPickkingID;		// 들고 있는 아이템 아이디
	unsigned char	m_cPickkingNum;		// 들고 있는 아이템의 남은 갯수

	unsigned char	m_cResultSize;		// 합성 결과물 크기
};

// 업그레이드 아이템
struct PktUgI : public PktBase
{
	unsigned long	m_dwNPCID;			// 대장장이 NPC 아이디
};

// 업그레이드 아이템 Ack (+ 아이템 구조체)
struct PktUgIAck : public PktBase
{
	unsigned long	m_dwCharID;		        // 캐릭터 아이디
    unsigned long   m_dwCurrentGold;        // 현재 인벤토리에 남은 금액, 에러시 의미 없음
    unsigned char   m_cCurrentMineralNum;   // 현재 남은 광물 갯수, 에러시 의미 없음.
    unsigned char   m_cValue;				// 업그레이드 추가 스텟
	unsigned char	m_cSize;                // 장비 아이템 크기
};

// 아이템 옵션 이식
struct PktItemOptionGraft : public PktBase
{
	unsigned long	m_dwCharID;			// 캐릭터 아이디
	unsigned char	m_cAttributeType;	// 이식할 옵션 종류
};

// 아이템 옵션 이식 Ack (+ 아이템 구조체)
struct PktItemOptionGraftAck : public PktBase
{
	unsigned long	m_dwCharID;			// 캐릭터 아이디

	unsigned long	m_dwGold;			// 남은 금액
	unsigned char	m_cSize;			// 결과 아이템 크기
};

// 아이템 보상 판매
struct PktItemCompensation : public PktBase
{
	unsigned long	m_dwCharID;			// 캐릭터 아이디
};

// 아이템 스플릿 요청 (응답 필요)
struct PktSplt : public PktBase
{
	unsigned long	m_dwCharID;     // 캐릭터 아이디
	TakeType		m_TakeType;	    // 아이템 집기 구조체 
                                    //  ( Src : 나눌 아이템, Dst : 나누어서 놓을 자리(항상 비어 있어야 함), 
                                    //    Num : 나눌 아이템 갯수)
};

// 아이템 스플릿 Ack (응답 필요) (+ 가변길이 아이템 구조체 (나누어서 생긴 아이템 정보))
struct PktSpItAck : public PktBase 
{
	enum PktSpltAckError
    {
        SERVER_ERROR	= 1,
        FAIL_SPLIT	    = 2
    };

	unsigned long	m_dwCharID;     // 캐릭터 아이디
	TakeType		m_TakeType;     // 아이템 집기 구조체
	unsigned char   m_cSize;        // 아이템 크기
};

// 퀵 슬롯 이동	(응답 필요)
struct PktQSM : public PktBase 
{
	enum PktQSMError
    {
        SERVER_ERROR	= 1,
        FAIL_MOVE	    = 2
    };

	TakeType		m_TakeType;		
	unsigned short	m_usSkillID;
	unsigned char	m_cLockCount;
	unsigned char	m_cSkillLevel;
};

// 스위치 장비
struct PktSwEQ : public PktBase 
{
	unsigned char	m_cType;		// 0 : 핸드, 1 : 말
	unsigned char	m_cSelect;      // 선택 핸드 ( 1 = 무기1 & 방패1, 2 = 무기2 & 방패2	) 
};

// 교환 명령
struct PktExC : public PktBase 
{
	enum ExchageCmd 
	{ 
		EXC_PROPOSE		= 0,	// 제안
		EXC_ACCEPT		= 1,	// 수락
		EXC_REFUSE		= 2,	// 거절
		EXC_OK			= 3,	// 확인
		EXC_CANCEL		= 4,	// 확인 취소
		EXC_EXCHANGE	= 5,	// 거래 성립
		EXC_QUIT		= 6,	// 거래 취소
		EXC_LOCK		= 7,	// 락
		EXC_UNLOCK		= 8		// 언락
	};

	enum PktExCError
	{
		FAIL_EXCHANGING_CHAR	= 2,	// 다른 캐릭터와 교환중인 캐릭터에게 교환 신청
		FAIL_NOT_ALL_OK			= 3,	// 양쪽 모두 확인하지 않은 상태에서 거래 성립
		FAIL_REJECT				= 4,	// 거부 옵션이 켜진 상태
		FAIL_EXCHANGING_ID		= 5		// 수락했을때와 거래성립때에 아이디가 틀릴경우.
	};

	unsigned long	m_dwSenderID;   // 보낸이 아이디
	unsigned long	m_dwRecverID;   // 받는이 아이디
    unsigned char   m_cCmd;         // 명령
};

// 교환 아이템 (+ 아이템 구조체)
struct PktExI : public PktBase 
{
	enum Type
    {
		Item    = 0,
        Gold    = 1,
        Remove  = 2
	};

	unsigned long	m_dwCharID;     // 캐릭터 아이디
   	unsigned long	m_dwSize;       // 아이템 데이터 크기(혹은 돈 양)
    Item::ItemPos   m_itemPos;      // 아이템 위치
	unsigned char	m_cType;        // 아이템 종류(아이템 = 0, 돈 = 1)
    unsigned char   m_cNum;         // 아이템 갯수(stack아이템인 경우, 개수만 보냄)
};


namespace Deposit
{
    enum Const { PASSWORD_LENGTH = 4 };
};

// 창고 (GameServer <--> DBAgent)
// 로그인시에, PktDBUpdate를 보낸 후, PktDepositUpdateDB를 여러개 준다.
// 마지막 패킷에 m_bUpdateComplete를 true로 Set해 준다.

// 게임 중에는 게임 서버가 수시로 Update를 한다.
struct PktDepositUpdateDB : public PktDD
{
    enum TabFlag
    {
        ITEM_TAB1       = (  1 << 0 ),
        ITEM_TAB2       = (  1 << 1 ),
        ITEM_TAB3       = (  1 << 2 ),
        ITEM_TAB4       = (  1 << 3 ),
        USED_DEPOSIT    = (  1 << 30),  // 0 : 창고 사용 X,     1 : 창고 사용함.
        SAVED_PASSWORD  = (  1 << 31)   // 0 : NOSAVE           1 : SAVE
    };

    enum TabNum
    {
        TAB_12          = 0,
        TAB_34          = 1,
    };

    unsigned long   m_dwUID;            // 캐릭터 UID
    unsigned long   m_dwCID;            // 캐릭터 CID
    unsigned long   m_dwTabFlag;        // 현재 탭의 구입 상태를 나타내는 bitset
    unsigned short  m_usDataSize;       // 탭(혹은 암호) 데이터 크기. 암호인 경우는 Deposit::PASSWORD_LENGTH와 같아야 한다.
    unsigned char   m_cTabNum;          // password인 경우는 PASSWORD_TABNUM임
    bool            m_bUpdateComplete;  // 업데이트가 완료되는 경우, true로 set.
};

// 창고 (Client <--> GameServer)
struct PktDeposit : public PktDD
{
    enum CMD
    {
        LOGIN       = 1,    // Ack있음(Ack데이터 없음) (Client<->GameServer) szData[0]~[3]는 password, 그 다음에 암호 저장 여부 1byte (0, 1)
        BUY_TAB     = 2,    // Ack있음(Ack데이터 없음) (Client<->GameServer) szData[0]에 사용할 탭 번호를 넣어 줌 
        RETURN_TAB  = 3,    // Ack있음(Ack데이터 없음) (Client<->GameServer) szData[0]에 사용할 탭 번호를 넣어 줌, 아직 구현되지 않음

        LOGOUT      = 4,    // Ack있음 (Client->GameServer)   데이터 없음. 
        CHANGE_PASS = 5,    // Ack있음 (Client->GameServer)   szData[0]~[3]는 password. 인증된 상태에서만 가능.
        SAVED_PASS  = 6,    // Ack없음 (GameServer -> Client) 게임 로그인시 줌. szData[0]~[3]는 Gold, [4]는 암호 저장 여부 1byte (0, 1)
                                
 		PASSWORD	= 7,	// AgentServer -> GameServer (CID, Password),   GameServer -> AgentServer (UID, Password)
		GOLD		= 8		// AgentServer -> GameServer (CID, Gold),       GameServer -> AgentServer (UID, Gold)
    };

    enum DepositError
    {        
        INSUFFICIENT_MONEY  = 2,
        AUTH_FAILED         = 3,
        INVALID_ARGUMENT    = 4
    };

    enum { MIN_DATA_SIZE = Deposit::PASSWORD_LENGTH + 4 };

    unsigned char   m_cCmd;
    char            m_szData[MIN_DATA_SIZE];
};

// 복권 결과
struct PktLotteryResult : public PktBase
{
	enum PktLRErr
	{
		FAIL_BLANK		= 2			// 존재하지 않는 상품
	};

	unsigned long	m_dwCharID;
	unsigned long	m_dwSize;

	Item::ItemPos	m_itemPos;
};

#pragma pack()

#endif
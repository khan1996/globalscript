#ifndef _UNIFIED_CHAR_DATA_PACKET_H_
#define _UNIFIED_CHAR_DATA_PACKET_H_

#include <DB/DBdefine.h>
#include <Network/Packet/PacketBase.h>

namespace UnifiedConst
{
    enum AgentServerType
    {
        // OldServerGroupID의 값으로도 사용한다. 0 ~ 10 은 예전 서버군 ID이다.

        Part1			= 20,
        Part1Unified	= 21,
        ROW				= 22,
        Part2Unified	= 23,
        Part2Selectable = 24,
    };

    enum StoreSelected
    {
        NONE_SELECTED   = 0,
        SELECTED_PART1  = (1 << 0),
        SELECTED_PART2  = (1 << 1)
    };
}


struct PktUnifiedCharInfo : public PktBase
{
    enum
    {
        MAX_STORE_INFO  = 10,
        MAX_CHAR_DATA   = 30
    };

    enum ErrorCode
    {
        STORE_READ_ERROR        = 1,
        CHARINFO_READ_ERROR     = 2        
    };

    unsigned long       dwUID;
    
    unsigned char       cStoreInfoNum;
    unsigned char       cCharDataNum;
    unsigned char       cRestrictedPart1ToPart2Level;   // Part1 에서 Part2 로 이전 가능한 캐릭터 레벨
    unsigned char       cRemainCharTransferCount;       // Part1 에서 Part2 로 이전 가능한 횟수

    USER_INFO           userInfo;
    UnifiedStoreInfo    unifiedStoreInfo[MAX_STORE_INFO];
    UnifiedCharData     unifiedCharData[MAX_CHAR_DATA];
};

struct PktUnifiedCharSelectReq : public PktBase
{
    enum
    {        
		// edith 2008.03.17 ID,PASS 길이조정
        MAX_PASSWORD_LEN = 36            
    };

    char            szPassword[MAX_PASSWORD_LEN];   // 계정 패스워드

    unsigned char   cSelectedServerGroupID;         // 선택한 서버군 번호
    unsigned char   cSelectedNation;                // 선택한 국적        
    unsigned long   dwRequestKey;                   // RequestKey

    unsigned long   dwUID;                          // UID
    unsigned long   dwCID[USER_INFO::MAX_CHAR_NUM]; // 선택한 슬롯 번호에 CID를 채워 준다.    
};

struct PktUnifiedCharSelectAck : public PktBase
{
    enum ErrorCode
    {
        SERVER_ERROR                = 1,
        PACKET_ERROR                = 2,
        PASSWORD_FAILED             = 3,

        WRONG_STORE_SELECTED        = 4,        // 잘못된 창고 선택(내 소유 아닌것 선택)
        WRONG_CHAR_SELECTED         = 5,        // 잘못된 캐릭터 선택(내 소유 아닌것 선택. 이미 선택한 것 선택)

        UNIFIED_STORE_READ_ERROR    = 6,        // 선택한 창고 읽기 실패
        UNIFIED_STORE_WRITE_ERROR   = 7,        // 선택한 창고 기록 실패

        UNIFIED_CHAR_READ_ERROR     = 8,        // 캐릭터 선택 정보 읽기 실패
        UNIFIED_CHAR_WRITE_ERROR    = 9,        // 캐릭터 선택 정보 기록 실패
        CHAR_VIEW_RELOAD_ERROR      = 10,       // 캐릭터 뷰 리로드 실패

        PART1_DB_LOGINED            = 11,       // Part1 DB에 로그인 해 있음    
        TRANSFER_COUNT_OVER         = 12        // TransferCount가 일정 회수 이상 초과되어 있다.
    };

    unsigned long   dwUID;
    unsigned long   dwRequestKey;
};

#endif
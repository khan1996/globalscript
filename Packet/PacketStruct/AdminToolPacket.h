#ifndef _ADMIN_TOOL_PACKET_H_
#define _ADMIN_TOOL_PACKET_H_

#include <Network/Packet/PacketBase.h>
#include <DB/DBDefine.h>

#pragma pack(1)

namespace PktAdminTool
{
    enum DefaultConstants
    {
        MAX_NAME            = 16,
        MAX_ITEM_NAME       = 32,
        MAX_PASSWORD        = 16,
        MAX_IP_LEN          = 16,        
        MAX_DATE_LEN        = 11,
        MAX_DATE_TIME_LEN   = 20,
        ADMIN_NAME_LEN      = 7,
        BLOCK_TYPE_LEN      = 9,
        MAX_RIGHTS_NUM      = 20,
        MAX_DESCRIPTION     = 31
    };

    namespace CMD
    {
        enum Type
        {
            Auth                    = 1,    // struct Auth
            AuthAck                 = 1,    // struct AuthAck

            AddAdmin                = 2,    // struct AddAdmin
            AddAdminAck             = 2,    // struct SimpleAck

            SearchAdmin             = 3,    // struct SearchAdmin
            SearchAdminAck          = 3,    // struct SearchAdminAck

            ViewAdmin               = 4,    // struct PktBase
            ViewAdminAck            = 4,    // struct SearchAdminAck

            DeleteAdmin             = 5,    // struct RemoveAdmin
            DeleteAdminAck          = 5,    // struct SimpleAck

            UpdateAdmin             = 6,    // struct UpdateAdmin
            UpdateAdminAck          = 6,    // struct SimpleAck

            FindLoginUser           = 7,    // struct FindLoginUser
            FindLoginUserAck        = 7,    // struct FindLoginUserAck

            ForceLogoutUser         = 8,    // struct ForceLogoutUser
            ForceLogoutUserAck      = 8,    // struct SimpleAck

            ViewRestraintUser       = 9,    // struct ViewRestraintUser
            ViewRestraintUserAck    = 9,    // struct ViewRestraintUserAck

            AddRestraintUser        = 10,   // struct AddRestraintUser
            AddRestraintUserAck     = 10,   // struct SimpleAck

            CheckRestraintUser      = 11,   // struct CheckRestraintUser
            CheckRestraintUserAck   = 11,   // struct CheckRestraintUserAck

            DeleteRestraintUser     = 12,   // struct DeleteRestraintUser
            DeleteRestraintUserAck  = 12,   // struct SimpleAck

            GetCharacterNames       = 13,   // struct GetCharacterNames
            GetCharacterNamesAck    = 13,   // struct GetCharacterNamesAck

            GetCharData             = 14,   // struct GetCharData
            GetCharDataAck          = 14,   // struct GetCharDataAck

            CommitData              = 15,   // struct PktBase
            CommitDataAck           = 15,   // struct SimpleAck

            UpdateCharData          = 16,   // struct UpdateCharData
            UpdateCharDataAck       = 16,   // struct SimpleAck

            UpdateCharPos           = 17,   // struct UpdateCharPos
            UpdateCharPosAck        = 17,   // struct SimpleAck

            CreateItem              = 18,   // struct CreateItem
            CreateItemAck           = 18,   // struct CreateItemAck

            UpdateItem              = 19,   // struct UpdateItem
            UpdateItemAck           = 19,   // struct UpdateItemAck

            DeleteItem              = 20,   // struct DeleteItem
            DeleteItemAck           = 20,   // struct DeleteItem

            ViewLoginoutLog         = 21,   // struct ViewLoginoutLog
            ViewLoginoutLogAck      = 21,   // struct ViewLoginoutLogAck

            GetAdminDetailRight     = 22,   // struct GetAdminDetailRight
            GetAdminDetailRightAck  = 22,   // struct AdminDetailRightAck

            SetAdminDetailRight     = 23,   // struct SetAdminDetailRight
            SetAdminDetailRightAck  = 23    // struct AdminDetailRightAck
        };
    };

    struct AdminToolItemInfo
    {
        char                m_szItemName[64];       // 아이템 이름. 수정불가
        unsigned __int64    m_ItemUID;              // 아이템 UID.  수정불가
        unsigned short      m_usItemProtoTypeID;    // 아이템 종류ID. 수정 불가
        unsigned char       m_cItemType;            // 아이템 타입.   수정 불가
        unsigned char       m_cTakePos;             // 아이템 컨테이너 위치(TakeType). 수정 불가

        unsigned char       m_cXPos;                // 아이템 X위치.    수정불가
        unsigned char       m_cYPos;                // 아이템 Y위치.    수정불가
        unsigned char       m_cZPos;                // 아이템 Z위치.    수정불가
        unsigned char       m_cNumOrDurability;     // 아이템 개수 혹은 내구도. 수정가능

        unsigned char       m_cXSize;               // 아이템 X크기.    수정불가
        unsigned char       m_cYSize;               // 아이템 Y크기.    수정불가
        bool                m_bEquip;               // 아이템 장비여부.    수정불가
        unsigned char       m_cPadding;             // 의미 없음.

        AdminToolItemInfo()
        {
            memset(m_szItemName, 0, 64);
            m_ItemUID = m_usItemProtoTypeID = m_cItemType = m_cTakePos = 0;
            m_cXPos = m_cYPos = m_cZPos = m_cNumOrDurability = m_cXSize = m_cYSize = m_cPadding = 0;
            m_bEquip = false;
        }
    };

    struct AdminToolEquipInfo : public AdminToolItemInfo
    {
        unsigned short      m_usRuneSocket;         // 아이템 룬 소켓
        unsigned char       m_cMaxDurability;       // 아이템 최대 내구도
        unsigned char       m_cCurrentSocketNum;    // 아이템 현재 소켓 수

        unsigned char       m_cMaxSocketNum;        // 아이템 최대 소켓 수
        unsigned char       m_cMaterialType;        // 아이템 업그레이드 단계

        unsigned char       m_cSocket[8];           // 아이템 소켓 내용
        short               m_usAttribute[35];      // 아이템 속성

        AdminToolEquipInfo()
        {
            m_usRuneSocket = m_cMaxDurability = m_cCurrentSocketNum = m_cMaxSocketNum = m_cMaterialType = 0;
            memset(m_cSocket, 0, 8);
            memset(m_usAttribute, 0, sizeof(unsigned short) * 35);
        }
    };

    // 단순 Ack
    struct SimpleAck : public PktBase
    {

    };

    // ----------------------------------------------------------------------------

    // 운영자툴 인증
    struct Auth : public PktBase
    {
        char            m_szName[MAX_NAME];
        char            m_szPassword[MAX_PASSWORD];        
    };


    // 운영자툴 인증 Ack
    struct AuthAck : public PktBase
    {        
        char    m_cRights;                   // 사용 권한 ( G, M, T... )
        char    m_szRights[MAX_RIGHTS_NUM];  // 세부 사용 권한
    };

    // ----------------------------------------------------------------------------

    // 관리자 추가
    struct AddAdmin : public PktBase
    {
        char            m_szName[MAX_NAME];
        char            m_szPassword[MAX_PASSWORD];
        char            m_szIP[MAX_IP_LEN];             // 접속가능 IP
        char            m_szAdminName[ADMIN_NAME_LEN];  // 실제 사용자 이름.
        unsigned char   m_cRights;                      // 사용 권한 ( 0, 1, 2, ... )
    };

    // ----------------------------------------------------------------------------

    // 관리자 검색
    struct SearchAdmin : public PktBase
    {
        char            m_szName[MAX_NAME];
    };

    struct SearchAdminAck : public PktBase
    {
        char            m_szAdminID[MAX_NAME];
        char            m_szPassword[MAX_PASSWORD];        
        char            m_szIP[MAX_IP_LEN];             // 접속가능 IP.
        char            m_szAdminName[ADMIN_NAME_LEN];  // 관리자 이름.
        char            m_cRights;                      // 사용 권한 ( 0, 1, 2, ... )        
        char            m_szDate[MAX_DATE_LEN];         // 날짜 길이
    };

    // ----------------------------------------------------------------------------

    // 관리자 삭제
    struct RemoveAdmin : public PktBase
    {
        char            m_szName[MAX_NAME];
    };

    // ----------------------------------------------------------------------------

    // 관리자 정보 업데이트
    struct UpdateAdmin : public PktBase
    {
        char            m_szAdminID[MAX_NAME];
        char            m_szPassword[MAX_PASSWORD];        
        char            m_szIP[MAX_IP_LEN];             // 접속가능 IP.
        char            m_szAdminName[MAX_NAME];        // 관리자 이름.
        char            m_cRights;                      // 사용 권한 ( 0, 1, 2, ... )                
    };

    // ----------------------------------------------------------------------------

    // 현재 로그인 사용자 검색. ( Billing DB에서만 검색함. 나중에 중계로 Query도 필요.
    struct FindLoginUser : public PktBase
    {
        char            m_szSearchID[MAX_NAME];
    };

    struct FindLoginUserAck : public PktBase
    {
        char            m_szUserID[MAX_NAME];                   // 유저 ID
        unsigned long   m_dwUID;                                // UID
        unsigned long   m_dwCRMIndex1;                          // PC방 ID
        char            m_szIP[MAX_IP_LEN];                     // 접속 IP
        unsigned long   m_dwConnectedServerID;                  // 접속된 서버 ID
        char            m_szLoginDateTime[MAX_DATE_TIME_LEN];   // 접속 시간
        unsigned char   m_cBillingType;                         // 빌링타입
    };

    // ----------------------------------------------------------------------------

    // 강제 로그아웃 하기.
    struct ForceLogoutUser : public PktBase
    {        
        unsigned long   m_dwUID;                // UID
        unsigned long   m_dwConnectedServerID;  // 접속된 서버 ID
    };

    // ----------------------------------------------------------------------------

    // 제재 조회하기
    struct ViewRestraintUser : public PktBase
    {
        unsigned char   m_cFlags;               // 플래그
        unsigned char   m_cConnectedServer;     // 접속 서버
        unsigned long   m_dwUID;                // 제재자 UID
    };

    struct ViewRestraintUserAck : public PktBase
    {
        unsigned long   m_dwUID;
        char            m_szUserID[MAX_NAME];                       // UserID
        char            m_szCharacterName[MAX_NAME];                // 캐릭터 이름
        char            m_szBlockType[BLOCK_TYPE_LEN];              // 블럭 종류
        char            m_szBlockKind[BLOCK_TYPE_LEN];              // 블럭 종류
        char            m_szHasBlock[BLOCK_TYPE_LEN];               // 블럭 유무
        char            m_szBlockFinishDateTime[MAX_DATE_TIME_LEN]; // 블럭 종료일자
        char            m_szAdminID[MAX_NAME];                      // 블럭을 한 사람 ID
        char            m_szDescription[MAX_DESCRIPTION];           // 블럭 사유
    };

    // ----------------------------------------------------------------------------

    // 제재 등록하기
    struct AddRestraintUser : public PktBase
    {
        unsigned char   m_cConnectedServer;                         // 접속 서버
        unsigned long   m_dwUID;                                    // UID
        char            m_szCharacterName[MAX_NAME];                // 캐릭터 이름
        unsigned char   m_cBlockType;                               // 블럭 타입 ( 캐릭터 또는 계정 )
        unsigned char   m_cBlockKind;                               // 블럭 종류 ( 임시블럭, 영구블럭 )
        char            m_szBlockFinishDateTime[MAX_DATE_TIME_LEN]; // 블럭 종료일자
        char            m_szDescription[MAX_DESCRIPTION];           // 블럭 사유
    };

    // ----------------------------------------------------------------------------

    // 제재 확인하기
    struct CheckRestraintUser : public PktBase
    {
        unsigned char   m_cConnectedServer;
        unsigned long   m_dwUID;
    };

    struct CheckRestraintUserAck : public PktBase
    {
        unsigned char   m_cResult;                  // 0 : 제재 없음, 1 : 계정 블럭, 2 : 캐릭터 블럭
        char            m_szCharName1[MAX_NAME];    // 제재 캐릭터 이름 1
        char            m_szCharName2[MAX_NAME];    // 제재 캐릭터 이름 2
        char            m_szCharName3[MAX_NAME];    // 제재 캐릭터 이름 3
    };

    // ----------------------------------------------------------------------------

    // 제재 삭제하기
    struct DeleteRestraintUser : public PktBase
    {
        unsigned char   m_cServerID;                // 서버ID
        unsigned long   m_dwUID;                    // UID
        char            m_szCharName[MAX_NAME];     // 제재 캐릭터 이름
    };

    // ----------------------------------------------------------------------------

    // UID로 캐릭터 이름 얻어 오기
    struct GetCharacterNames : public PktBase
    {
        unsigned long   m_dwUID;
        unsigned char   m_cConnectedServer;
    };

    struct GetCharacterNamesAck : public PktBase
    {
        char            m_szCharName[3][MAX_NAME];
    };

    // ----------------------------------------------------------------------------

    // 캐릭터 데이터 얻어오기
    struct PktAdminToolGetData : public PktBase
    {
        enum GetDataType
        {
            BasicInfo   = 0,    // 0 : SendLogin때 받은 정보
            StoreInfo   = 1,    // 1 : 창고 정보
            Store1Data  = 2,    // 2 : 창고 1데이터
            Store2Data  = 3,    // 3 : 창고 2데이터
        };

        char            m_szName[MAX_NAME];
        unsigned long   m_dwRequestKey;
        unsigned long   m_dwUID;
        unsigned long   m_dwCID;
        unsigned char   m_cType;
    };

    struct PktAdminToolGetDataAck : public PktBase
    {
        unsigned long   m_dwRequestKey;
        unsigned long   m_dwUID;
        unsigned long   m_dwCID;
        unsigned char   m_cType;
    };

    // 어드민 툴에서 데이터 업데이트하기. (뒤에 데이터를 넣음)
    struct PktAdminToolSetData : public PktBase
    {
        enum SetDataType
        {
            BasicInfo   = 0,    // 0 : SendLogin때 받은 정보
            StoreInfo   = 1,    // 1 : 창고 정보
            Store1Data  = 2,    // 2 : 창고 1데이터
            Store2Data  = 3,    // 3 : 창고 2데이터
        };

        unsigned __int64    m_dwSerial;
        unsigned long       m_dwRequestKey;
        unsigned long       m_dwUID;
        unsigned long       m_dwCID;
        unsigned char       m_cType;
    };

    struct PktAdminToolSetDataAck : public PktBase
    {
        unsigned long       m_dwRequestKey;
        unsigned char       m_cType;
    };

    // ----------------------------------------------------------------------------
    
    // 관리툴에서 데이터 얻어 오기
    struct GetCharData : public PktBase
    {
        char                m_szName[MAX_NAME];
        unsigned long       m_dwUID;
        unsigned long       m_dwCID;
        unsigned char       m_cServerGroup;
    };

    struct GetCharDataAck : public PktBase
    {
        unsigned long       m_dwUID;
        unsigned long       m_dwCID;
        unsigned char       m_cType;    // DataType
    };

    // ----------------------------------------------------------------------------

    // 캐릭터 데이터 업데이트
    struct UpdateCharData : public PktBase
    {
        CHAR_INFOST         m_CharInfo;
    };

    // 캐릭터 위치 업데이트
    struct UpdateCharPos : public PktBase
    {
        POS                 m_LastPos;
    };

    // ----------------------------------------------------------------------------

    // 아이템 생성
    struct CreateItem : public PktBase
    {
        char                m_szItemName[MAX_ITEM_NAME];
        unsigned char       m_cTakePos;
        unsigned char       m_cXPos;
        unsigned char       m_cYPos;
        unsigned char       m_cZPos;
    };

    // 아이템 생성 Ack (뒤에 생성된 아이템 정보 보냄)
    struct CreateItemAck : public PktBase
    {
        AdminToolEquipInfo  m_EquipInfo;    // 장비가 아니면 뒤의 데이터는 0으로 채워 줌
    };

    // ----------------------------------------------------------------------------

    // 아이템 업데이트
    struct UpdateItem : public PktBase
    {
        AdminToolEquipInfo  m_EquipInfo;
    };

    // 아이템 업데이트 Ack
    struct UpdateItemAck : public PktBase
    {
        AdminToolEquipInfo  m_EquipInfo;
    };

    // ----------------------------------------------------------------------------

    // 아이템 삭제
    struct DeleteItem : public PktBase
    {
        unsigned __int64    m_dwItemUID;
        unsigned short      m_usItemProtoTypeID;
        unsigned char       m_cTakePos;
        unsigned char       m_cXPos;
        unsigned char       m_cYPos;
        unsigned char       m_cZPos;
    };

    // ----------------------------------------------------------------------------

    // 로그인아웃 보기
    struct ViewLoginoutLog : public PktBase
    {
        char            m_szClientID[MAX_NAME];
    };

    // 로그인 아웃 정보
    struct ViewLoginoutLogAck : public PktBase 
    {
        char            m_szUserID[MAX_NAME];                   // 유저 ID
        char            m_szIP[MAX_IP_LEN];                     // 접속 IP
        char            m_szLoginDateTime[MAX_DATE_TIME_LEN];   // 접속 시간
        char            m_szLogoutDateTime[MAX_DATE_TIME_LEN];  // 접속 해제 시간

        unsigned long   m_dwUID;                                // UID
        unsigned long   m_dwCRMIndex1;                          // PC방 ID
        unsigned long   m_dwConnectedServerID;                  // 접속된 서버 ID
        
        unsigned char   m_cBillingType;                         // 빌링타입
    };

    // ----------------------------------------------------------------------------

    struct GetAdminDetailRight : public PktBase
    {
        char m_cRights;
    };

    struct SetAdminDetailRight : public PktBase
    {
        char m_cRights;
        char m_szRights[MAX_RIGHTS_NUM];
    };

    struct AdminDetailRightAck : public PktBase
    {        
        char m_cRights;
        char m_szRights[MAX_RIGHTS_NUM];
    };

    // ----------------------------------------------------------------------------
};

#pragma pack()

#endif
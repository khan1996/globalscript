#ifndef _GAME_LOG_PACKET_H_
#define _GAME_LOG_PACKET_H_

#include <Network/Packet/PacketBase.h>


namespace GAMELOG
{
    namespace CMD
    {
        enum Packet
        {
            DefaultInfo                 = 1,
            ItemInfo                    = 2,
            EquipInfo                   = 3,
        };

        enum LogType
        {
            CHAR_LOGIN                  = 1,    // 로그인
            CHAR_LOGOUT                 = 2,    // 로그아웃
            PICKUP                      = 3,    // 아이템 집기                      - 돈 포함
            DROP                        = 4,    // 아이템 버리기                    - 돈 포함
            BUY_ITEM                    = 5,    // 아이템 사기                      - 돈 포함
            SELL_ITEM                   = 6,    // 아이템 팔기                      - 돈 포함
            BEFORE_EXCHANGE             = 7,    // 아이템 교환 전의 교환창 아이템   - 돈 포함
            AFTER_EXCHANGE              = 8,    // 아이템 교환 전의 교환창 아이템   - 돈 포함
            USE_ITEM                    = 9,    // 아이템 사용                      - 돈 포함
            UPGRADE_ITEM                = 10,   // 아이템 업그레이드                - 돈 포함

            INSTALL_SOCKET              = 11,   // 아이템 소켓에 보석 박기          
            GET_QUEST_ITEM              = 12,   // 퀘스트 아이템 얻기
            GET_QUEST_REWARD            = 13,   // 퀘스트 보상 얻기                 - 돈 포함
            REMOVE_QUEST_ITEM           = 14,   // 퀘스트 아이템 제거
            REPAIR_ITEM                 = 15,   // 아이템 수리                      - 돈 포함
            ADMIN_DROP                  = 16,   // 운영자 명령으로 아이템 떨구기    - 돈 포함

            CHAR_LOGIN_INVENTORY_GOLD   = 17,   // 캐릭터 로그인시 인벤토리 돈      
            CHAR_LOGIN_DEPOSIT_GOLD     = 18,   // 캐릭터 로그인시 창고 돈          
            CHAR_LOGOUT_INVENTORY_GOLD  = 19,   // 캐릭터 로그아웃시 인벤토리 돈    
            CHAR_LOGOUT_DEPOSIT_GOLD    = 20,   // 캐릭터 로그아웃시 창고 돈        
            DEPOSIT_RENT_GOLD           = 21,   // 창고 대여료

			FAME_GET_BATTLE				= 22,	// 전투로 얻은 명성
			FAME_LOSE_BATTLE			= 23,	// 전투로 잃은 명성
			FAME_GET_CAMP				= 24,	// 요새로 얻은 명성
			FAME_LOSE_CAMP				= 25,	// 요새로 잃은 명성

			QUEST_GET_REWARD			= 26,	// 퀘스트 수행으로 얻은 보상

            MAX_LOG_TYPE                = 27
        };
    };

    namespace PACKET
    {
        enum Flags
        {
            ITEM_INFO   = (1 << 0),
            EQUIP_INFO  = (1 << 1)
        };

        enum
        {
            MAX_SOCKET_NUM      = 8,
            MAX_ATTRIBUTE_NUM   = 35
        };
      
        #pragma pack(1)

        struct DefaultInfo : public PktBase
        {
            time_t              m_dwTime;
            unsigned long       m_dwFirstUID;       // 행위자의 UID
            unsigned long       m_dwFirstCID;       // 행위자의 CID
            unsigned long       m_dwGold;           // 행위로 인해서 영향을 받게 된 금액
            unsigned long       m_dwSecondUID;      // 대상자의 UID
            unsigned long       m_dwSecondCID;      // 대상자의 CID
            
            unsigned short      m_usPosX;           // 사건이 일어난 장소(X)
            unsigned short      m_usPosY;           // 사건이 일어난 장소(Y)
            unsigned short      m_usPosZ;           // 사건이 일어난 장소(Z)
            unsigned char       m_cCommand;         // 사건 종류
            unsigned char       m_cFlags;           // 기타 정보(아이템 정보인지.. 장비 정보가 따라 붙는지 등..)
        };

        struct ItemInfo : public DefaultInfo
        {
            unsigned __int64    m_dwItemUID;
            unsigned short      m_usItemProtoTypeID;
            unsigned char       m_cTakeType;
            unsigned char       m_cItemAmount;

            unsigned char       m_cItemPosX;
            unsigned char       m_cItemPosY;
            unsigned char       m_cItemPosZ;
            unsigned char       m_cItemExtra;       // 추가 정보
        };

        struct EquipInfo : public ItemInfo
        {
            unsigned short      m_usRuneSocket;                     // 룬 소켓 내용
            unsigned char       m_cUpgradeStep;                     // 업그레이드 단계
            unsigned char       m_cMaxSocket;                       // 현재 뚫려 있는 소켓 수            
            unsigned char       m_cMaxDurability;                   // 최대 내구도

            unsigned char       m_cSocket[MAX_SOCKET_NUM];          // 소켓 내용
            short               m_usAttribute[MAX_ATTRIBUTE_NUM];   // 속성 내용
        };

        #pragma pack()
    };
};



#endif
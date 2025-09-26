#ifndef _CHAR_BROAD_CAST_PACKET_H_
#define _CHAR_BROAD_CAST_PACKET_H_

#include <winsock2.h>
#include <windows.h>

#include "CharMovePacket.h"

#include <vector>
#include <utility>

#include <Creature/CreatureStructure.h>

// FLAG (32bit, remain 29bit)
// 타입 코드 4bit ( 0 : Object              1 : AggresiveCreature정보
//                  2 : Monster정보,        3 : 캐릭터 Phase 1
//                  4 : 캐릭터 Phase
//
//
//

// new broadcast
namespace Broadcast2nd
{
    #pragma pack(1)

    struct PktBroadcast : PktBase
    {
        enum BroadcastType
        {
            LOGIN           = 0,
            LOGOUT          = 1,
            BROADCAST       = 2,
            BROADCAST_END   = 3
        };

        enum DataType
        {            
            CHAR_DATA   = 0,
            CHAR_CID    = 1,
            ITEM_DATA   = 2,
            NONE_DATA   = 3
        };

        unsigned long   m_dwCurrentPulse;
        unsigned char   m_cBroadcastType;
        unsigned char   m_cDataType;        
    };

    #pragma pack()
}


/*

// old broadcast
namespace BroadcastInfo
{
    // 생성해야 할 객체 타입.
    namespace ObjectType
    {
        enum Type
        {
            OBJECT                  = 0,
            AGGRESIVE_CREATURE      = 1,
            MONSTER                 = 2,
            SUMMON_MONSTER			= 4,
            MAX_OBJECT_TYPE         = 5
        };
    }

    // 현재 가지고 있는 데이터 타입.
    namespace DataType
    {
        enum Type
        {
            OBJECT                  = 0,
            AGGRESIVE_CREATURE      = 1,
            MONSTER                 = 2,
            SUMMON_MONSTER			= 4,
			MAX_OBJECT_TYPE         = 5
        };
    }

#pragma pack(1)

    struct SObject
    {
        // dwFlags    상위 비트  ------------- 하위 비트
        //            23 bit(Reserved) / DataType(3bit) / ObjectPhase(3bit) / ObjectType(3bit)

        enum TypeMask
        {
            ObjectTypeMask      = 0x00000007,       // 0000/0000/0000/0000/0000/0000/0000/0111
            ObjectPhaseMask     = 0x00000038,       // 0000/0000/0000/0000/0000/0000/0011/1000
            DataTypeMask        = 0x000001C0        // 0000/0000/0000/0000/0000/0001/1100/0000
        };
    
        SObject() : m_dwCID(0), m_dwFlags(0) { }
		SObject(unsigned long dwCID) : m_dwCID(dwCID), m_dwFlags(0) { }

        unsigned long   m_dwCID;            // CID.
        unsigned long   m_dwFlags;          // 각종 Flag들. 맨 뒤 3bit는 ObjectType임.
        CNetworkPos     m_NetworkPos;       // 크리쳐 위치.

        ObjectType::Type GetObjectType() const { return static_cast<ObjectType::Type>(ObjectTypeMask & m_dwFlags); }
        void SetObjectType(ObjectType::Type eObjectType) { m_dwFlags = ((m_dwFlags & (~ObjectTypeMask)) | eObjectType); }

        DataType::Type GetDataType() const { return static_cast<DataType::Type>((DataTypeMask & m_dwFlags) >> 6); }
        void SetDataType(DataType::Type eDataType) { m_dwFlags = ((m_dwFlags & (~DataTypeMask)) | (eDataType << 6)); }
    };


    struct SAggresiveCreature : public SObject
    {
		EnchantInfo		m_EnchantInfo;
		unsigned short	m_nNowHP;
    };

    namespace Monster
    {
		struct SMonster : public SAggresiveCreature
		{
		};

		struct SSummonMonster : public SMonster
		{
			unsigned long   m_dwMasterID;
		};
    };

#pragma pack()

    inline unsigned long GetObjectSize(const DataType::Type dataType)
    {
        switch(dataType)
        {
        case DataType::OBJECT:                  return sizeof(SObject);
        case DataType::AGGRESIVE_CREATURE:      return sizeof(SAggresiveCreature);
		case DataType::MONSTER:                 return sizeof(Monster::SMonster);
		case DataType::SUMMON_MONSTER:          return sizeof(Monster::SSummonMonster);
        }

        return 0;
    }

	inline unsigned long GetMaxObjectSize()
	{
        size_t max_size = max(sizeof(Monster::SMonster), sizeof(Monster::SSummonMonster));
		return static_cast<unsigned long>(max_size);
	}

    inline unsigned long GetObjectSize(const SObject& object) 
    { return GetObjectSize(object.GetDataType()); }

    inline bool IsAggresiveCreatureInfo(const ObjectType::Type eObjectType)
    {
        switch(eObjectType)
        {
			case ObjectType::AGGRESIVE_CREATURE:
			case ObjectType::MONSTER:
			case ObjectType::SUMMON_MONSTER:
				return true;
        }

        return false;
    }

    inline bool IsAggresiveCreatureInfo(const DataType::Type eDataType)
    {
        switch(eDataType)
        {
			case DataType::AGGRESIVE_CREATURE:
			case DataType::MONSTER:
			case DataType::SUMMON_MONSTER:
				return true;
        }

        return false;
    }


    inline bool IsMonsterInfo(const ObjectType::Type eObjectType) 
	{ 
        switch(eObjectType)
		{
			case ObjectType::MONSTER:
			case ObjectType::SUMMON_MONSTER:
				return true;
		}

		return false;
	}

	inline bool IsMonsterInfo(const DataType::Type eDataType) 
	{ 
        switch(eDataType)
		{
			case DataType::MONSTER:
			case DataType::SUMMON_MONSTER:
				return true;
		}

		return false;
	}


	inline bool IsSummonMonsterInfo(const ObjectType::Type eObjectType) { return (eObjectType == ObjectType::SUMMON_MONSTER); }
	inline bool IsSummonMonsterInfo(const DataType::Type eDataType) { return (eDataType == DataType::SUMMON_MONSTER); }
};

*/


#endif
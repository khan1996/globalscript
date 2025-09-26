#ifndef _CHAR_ITEM_PACKET_STRUCT_H_
#define _CHAR_ITEM_PACKET_STRUCT_H_

#include <DB/DBDefine.h>
#include <Item/ItemStructure.h>

#pragma pack(1)

// 필드 오브젝트 구조체
struct FieldObject
{
	enum CheckBit	{ Gold = 0x80000000 };

	unsigned __int64	m_nOID;			//  오브젝트 아이디 (몬스터 혹은 Object)
	unsigned long		m_dwTypeID;     //  타입 아이디 (상위 1비트 셋이면 돈)

    unsigned short		m_usXPos;       //  아이템 X위치
    unsigned short		m_usYPos;       //  아이템 Y위치
    unsigned short		m_usZPos;       //  아이템 Z위치
    unsigned char		m_cNum;         //  아이템 수    
};

struct TakeType 
{
	enum TakeSource 
	{ 
        TS_NONE			= 0,	TS_EQUIP		= 1, 
        TS_INVEN		= 2,	TS_QSLOT		= 3,
        TS_SSLOT		= 4,	TS_TEMP			= 6, 
        TS_EXTRA		= 7,	TS_EXCHANGE		= 8,
        TS_DEPOSIT		= 9,	TS_STALL		= 10,
		TS_TEMPINVEN	= 11,	TS_CAMPSHOP		= 12, 

		TS_ADMIN		= 13,	TS_MAXTYPE		= 14
	};

    Item::ItemPos   m_srcPos;
    Item::ItemPos   m_dstPos;
    unsigned char   m_cNum;

	TakeType() : m_cNum(0) { }
	TakeType(Item::ItemPos srcPos, Item::ItemPos dstPos, unsigned char cNum)
        : m_srcPos(srcPos), m_dstPos(dstPos), m_cNum(cNum) { }
};

// 캐스트 오브젝트 구조체 (Cast Object)
struct CastObject
{
	POS				m_DstPos;           // 목적 위치
	unsigned long	m_dwTargetID;       // 타겟 아이디

	unsigned short	m_wTypeID;          // 아이디	( 돈일경우 금액 )
	unsigned char	m_cObjectType;      // 종류	( 0 = 캐스트, 어텍 류, 2 = 돈 )
	unsigned char	m_cObjectLevel;     // 레벨
};

#pragma pack()

#endif
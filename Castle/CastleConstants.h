#ifndef _CASTLE_CONSTANTS_H_
#define _CASTLE_CONSTANTS_H_

namespace Castle
{
	enum Type
	{
		NONE			= 0,
		CASTLE			= 1,
		CAMP			= 2
	};

	enum IndentifyBit
	{
		STATUE_BIT		= 0x80000000,	// 다크 카나번 석상 (몬스터 비트와 같다.)
		CASTLE_BIT		= 0x40000000,	// 성
		CAMP_BIT		= 0x20000000,	// 길드 요새
		TOWN_BIT		= 0x10000000,	// 마을

		SIEGE_BIT		= 0x00FF0000,	// 공성시간에 공성측 국가가 사용하는 임시 리스폰 포인트 비트
										// CASTLE_BIT | SIEGE_BIT 로 사용!!
										// ex) 0x40FF1001 : CastleNameID 가 1 이고 임시 리스폰 포인트 2번

		CASTLE_NAME_BIT_SHIFT	= 12	// 다른 채널에 성이 존재할수 있기 때문에 스크립트에 들어가는 성 ID 대신
										// CASTLE_BIT | (CastleNameID << CASTLE_NAME_BIT_SHIFT) 의 값을 사용한다.
										// ex) 0x40001000 : CastleNameID 가 1 이다.
	};

	enum TaxType
	{
		TRADE_TAX			= 0,	// 상점 구매/판매
		REPAIR_TAX			= 1,	// 상점 수리
		RESMELT_TAX			= 2,	// 대장장이 재련
		CAMP_GOLD_TAX		= 3,	// 요새 Gold
		CAMP_MINERAL_TAX	= 4,	// 요새 광물

		MAX_TAX_TYPE		= 5
	};

	enum TaxChangable
	{
		TAX_DISABLE		= 0,		// 세율 변경 불가능
		TAX_ENABLE		= 1			// 세율 변경 가능
	};

	enum MineralType
	{
		ACCUMULATED_MINERAL					= 1,		// 누적된 광물 아이템
		TEMPORARY_MINERAL					= 2,		// 임시 보관된 광물 아이템
	};

	enum CastleName
	{
		TEMP_CASTLE_NAME_0					= 0,
		TEMP_CASTLE_NAME_1					= 1,
		TEMP_CASTLE_NAME_2					= 2,
		TEMP_CASTLE_NAME_3					= 3,
		TEMP_CASTLE_NAME_4					= 4,
		
		MAX_CASTLE_NAME_NUM					= 5,		
		MAX_CASTLE_NAME_LEN					= 32
	};

	enum CastleTitle
	{
		TITLE_NEW							= 0,	// 칭호 '새로운'
		TITLE_SETTLED						= 1,	// 칭호 '정착한'
		TITLE_FLOURISHED					= 2,	// 칭호 '번창한'
		TITLE_HONORABLE						= 3,	// 칭호 '명예로운'
		TITLE_GLORIOUS						= 4,	// 칭호 '영광의'

		MAX_TITLE_NUM						= 5
	};

	enum Const
	{
		CASTLE_EXP_BONUS_RADIUS				= 300,		// 공성 경험치 보상 반경

		MAX_CASTLE_NUM						= 10,
		MAX_CASTLE_MINERAL_NUM				= 200,
		MAX_CASTLE_OBJECT_NUM				= 250,

		MAX_TEMP_TAX_UPDATE_COUNT			= 10,
		MAX_TEMP_TAX_AMOUNT					= 50000,

		EMBLEM_UPGRADE_JEWEL_NUM			= 90,
		EMBLEM_UPGRADE_JEWEL_NUM_PER_POS	= 10,
		EMBLEM_UPGRADE_JEWEL_POS_NUM		= 9,

		FAME_FOR_CAPTURE_CASTLE				= 10000,	// 공성에서 성을 차지한 길드원들이 얻는 명성치
		FAME_FOR_FRIENDLY_GUILD				= 5000,		// 공성에서 성을 차지한 우호길드의 길드원들이 얻는 명성치

		INVINCIBLE_COUNT					= 0,		// 성주 변경시 주어지는 무적 공성 횟수 (0회)
		TEMP_TAX_GAIN_COUNT					= 1,		// 임시 세금을 누적 세금으로 옮길시 필요한 공성 횟수 (1회)

		DAY_END_TIME						= 0			// 하루가 끝나는 시간	(밤 12시)
	};
}

#endif	// _CASTLE_CONSTANTS_H_

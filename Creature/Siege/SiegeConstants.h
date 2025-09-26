#ifndef _SIEGE_CONSTANTS_H_
#define _SIEGE_CONSTANTS_H_

namespace Siege
{
	enum SiegeObjectType
	{
		// 몬스터 Kind ID 를 사용한다.
		EMBLEM					= 5000,		// 성 상징물					( DESTORYING, CHANGING, DEVELOPING, CANCELING, RESTORING, DESTROYED 1개만 존재 )
		GATE					= 5324,		// 성문							( DESTORYING, CHANGING, DEVELOPING, CANCELING, RESTORING, DESTROYED 1개만 존재 )
		BACKDOOR				= 5378,		// 뒷문							( Only COMPLETE 상태만 존재 )
		CAMP					= 5379,		// 길드 요새 (캠프)				( DEVELOPING, CANCELING, RESTORING, DESTROYED 1개만 존재 )
		CASTLE_ARMS_NPC			= 5433,		// 병기 관리 NPC				( Only COMPLETE 상태만 존재 )
		GUARD					= 5434,		// 가드							( DESTORYING, CHANGING, DEVELOPING, CANCELING, RESTORING, DESTROYED 1개만 존재 )
		SHORT_RANGE_CASTLE_ARMS	= 5488,		// 근거리 수성 병기				( DESTORYING, CHANGING, DEVELOPING, CANCELING, RESTORING, DESTROYED 1개만 존재 )
		LONG_RANGE_CASTLE_ARMS	= 5542,		// 원거리 수성 병기				( DESTORYING, CHANGING, DEVELOPING, CANCELING, RESTORING, DESTROYED 1개만 존재 )
		SHORT_RANGE_SIEGE_ARMS	= 5596,		// 근거릭 공성 병기				( CHANGING, CANCELING, RESTORING, DESTROYED 1개만 존재 )
		LONG_RANGE_SIEGE_ARMS	= 5650,		// 원거리 공성 병기				( CHANGING, CANCELING, RESTORING, DESTROYED 1개만 존재 )
		AIRSHIP					= 5704,		// 비공정						( CHANGING, CANCELING, RESTORING, DESTROYED 1개만 존재 )
		MINING_CAMP				= 5758,		// 채굴기 (캠프)				( DEVELOPING, CANCELING, RESTORING, DESTROYED 1개만 존재 )
		CAMP_SHOP				= 5812,		// 상점타입요새 (캠프)			( DEVELOPING, CANCELING, RESTORING, DESTROYED 1개만 존재 )
		KARTERANT_WEAPON		= 5866,		// 카르테란트 월드 웨폰 (캠프)	( DEVELOPING, CANCELING, RESTORING, DESTROYED 1개만 존재 )
		MERKADIA_WEAPON			= 5920,		// 메르카디아 월드 웨폰 (캠프)	( DEVELOPING, CANCELING, RESTORING, DESTROYED 1개만 존재 )
		
		DEFAULT_TYPE			= 0			// 디폴트 전달인자용
	};

	enum State
	{
		// KID             => SiegeObjectType
		//					  + (State * UPGRADE_KIND_NUM)
		//					  + UpgradeStep

		// KID (성 상징물) => SiegeObjectType
		//					  + (State * JEWEL_TYPE_NUM * UPGRADE_KIND_NUM)
		//					  +	(JewelType * UPGRADE_KIND_NUM)
		//					  + UpgradeStep

		COMPLETE				= 0,		// 성 상징물 소환 완료, 길드 요새 구축 완료, 가드, 경비병, 근거리, 원거리, 비공정 개발 완료
		UPGRADING				= 1,		// 성 상징물, 길드 요새, 성문, 가드, 근거리/원거리 수성병기 업그레이드 중인 상태
		REPARING				= 2,		// 길드 요새, 성문, 근거리/원거리, 비공정 수리 상태
		DESTROYING				= 3,		// 길드 요새, 근거리/원거리, 비공정 파괴해서 스타트 킷으로 돌리는 상태
		CHANGING				= 4,		// 길드 요새 변형중
		DEVELOPING				= 5,		// 성 상징물 소환중, 길드 요새 구축중, 가드, 경비병, 근거리, 원거리, 비공정 개발중
		CANCELING				= 6,		// 길드 요새, 근거리/원거리, 비공정 개발을 취소중인 상태 (스타트킷 아이템으로 돌아간다.)
		RESTORING				= 7,		// 성문 복구 상태
		DESTROYED				= 8,		// 성문 파괴 상태

		// SubState
		NONE					= 0,

        MINE					= 1,		// 아군 성 상징물
		ENEMY					= 2,		// 적군 성 상징물

		OPENED					= 1,		// 성문 열린 상태
		CLOSED					= 2,		// 성문 닫힌 상태

		MINING_OFF				= 0,		// 채굴기 정지 상태
		MINING_ON				= 1,		// 채굴기 가동 상태
		MINING_READY			= 2,		// 채굴기 가동 준비 상태

		WEAPON_EMPTY			= 0,		// 월드웨폰 무기 비장전 상태
		WEAPON_CHARGE			= 1,		// 무기 장전중
		WEAPON_READY			= 2,		// 무기 장전된 상태
		WEAPON_FIRE				= 3,		// 무기 발사중
	};

	enum JewelType
	{
		// 보석 순서가 바뀌면 안됨!!
		// 아이템 스크립트의 순서대로이다.
		NO_JEWEL				= 0,		// 보석 없음
		RUBY					= 1,		// 루비
		EMERALD					= 2,		// 에메랄드
		SAPPHIRE				= 3,		// 사파이어
		DIAMOND					= 4,		// 다이아몬드
		BLACKMOON				= 5,		// 블랙문
				
		JEWEL_PROTOTYPE_ID		= 1900,
		JEWEL_KIND				= 5,
		
		JEWEL_TYPE_NUM			= 6,		// 상징물 업그레이트 보석 타입 종류수 (없음 포함)
	};


	enum CampConst
	{
		CAMP_BUILDING_RADIUS				= 315,		// 캠프 구축 반경 (m)

		CAMP_REPAIR_GOLD_PER_HP				= 10,		// HP 단위당 10 골드

		CAMP_UPGRADE_MATERIAL_NUM			= 30,		// 길드 요새 한단계 업그레이드시 필요한 자재수
		MINING_CAMP_UPGRADE_MATERIAL_NUM	= 50,		// 채굴기 한단계 업그레이드시 필요한 자재수
		CAMP_SHOP_UPGRADE_MATERIAL_NUM		= 40,		// 길드 요새 상점 한단계 업그레이드시 필요한 자재수
		WEAPON_CHARGE_MATERIAL_NUM			= 50,		// 월드 웨폰 무기 장전에 필요한 자재수
		CAMP_REPAIR_HP_PER_MATERIAL			= 5000,		// 자재 하나당 수리되는 HP

		FAME_FOR_CAMP_BUILDING_TEST			= 0,//1000,		// 길드 요새 구축시 길드의 필요 명성치 (테섭용)
		FAME_FOR_CAMP_BUILDING				= 0,//2000,		// 길드 요새 구축시 길드의 필요 명성치 (정섭용)
		FAME_FOR_WEAPON_BUILDING_TEST		= 1000,		// 월드 웨폰 구축시 길드의 필요 명성치 (테섭용)
		FAME_FOR_WEAPON_BUILDING			= 2000,		// 월드 웨폰 구축시 길드의 필요 명성치 (정섭용)
		FAME_FOR_SELF_DESTROY_CAMP_FOR_USA	= -50,		// 길드 요새 파괴시 깍이는 명성치 (자신이 파괴(미국))
		FAME_FOR_SELF_DESTROY_CAMP			= -50,		// 길드 요새 파괴시 깍이는 명성치 (자신이 파괴)
		FAME_FOR_DESTROYED_CAMP				= -100,		// 길드 요새 파괴시 깍이는 명성치 (다른 사람이 파괴)

		GOLD_FOR_SELF_DESTROY_CAMP			= 500000,	// 길드 요새 파괴시 돌려주는 돈 (자신이 파괴)
		GOLD_FOR_CANCEL_CAMP				= 1000000,	// 길드 요새 구축 해제시 돌려주는 돈

		CAMP_ATTACK_RANGE					= 45,		// 요새 공격 범위 (반경 45m)

		// 시간 업데이트 타입
		TYPE_REMAIN_TIME					= 0,		// 시간 변수 업데이트
		TYPE_LAST_USE_TIME					= 1,		// 마지막 사용 시간 업데이트

		// 채굴 아이템 타입 종류
		ACCUMULATED_MINERAL					= 1,		// 누적된 광물 아이템
		TEMPORARY_MINERAL					= 2,		// 임시 보관된 광물 아이템

		MAX_MINERAL_NUM						= 1000,		// 광물 최고 스택 갯수

		// 상점 타입 관련
		MAX_TAX								= 100,		// 최대 세율

		// 월드 웨폰 관련
		WEAPON_FIRE_WARNING_COUNT			= 10,		// 월드 웨폰 발사 경고 표시 횟수 (10번 : 10초)
		WEAPON_REBUILD_SIEGE_TIME_COUNT		= 3,		// 공성 시간이 3번 지나야 다시 지을수 있음 (부서진 국가일 경우)
		WEAPON_DAMAGE_RANGE					= 256,		// 월드 웨폰 데미지 반경 (256m)
		WEAPON_DAMAGE						= 10000		// 월드 웨폰 절대 데미지
	};

	enum EmblemConst
	{
		MAX_EMBLEM_UPGRADE					= 3,		// 성 상징물 업그레이드 최대치
														// TODO : 상위 보석 2종류가 추가되면 Const::MAX_UPGRADE_NUM 를 사용한다.

		EMBLEM_ATTACK_HEIGHT_ERROR			= 5,		// 높이차가 최소 5m 안에 있는 적만 공격이 가능
	};

	enum GateConst
	{
		GATE_REPAIR_MIN_PER_HP				= 1,		// HP 1당 1분의 수리 시간
	};

	enum CastleArmsConst
	{
		CASTLE_ARMS_REPAIR_TIME				= 1,		// 수성 병기 수리 시간 (1분)
		CASTLE_ARMS_REPAIR_GOLD_PER_UNIT	= 10000,	// 수리 비용 단위당 10000 골드 ( 단위는 10%당 )

		FAME_FOR_DESTROY_CASTLE_ARMS		= -500,		// 수성 병기를 NPC 로 변형할때 깍이는 명성치
	};

	enum SiegeArmsConst
	{
		SIEGE_ARMS_REPAIR_TIME				= 1,		// 공성 병기 수리 시간 (1분).

		SIEGE_ARMS_UPGRADE_MATERIAL_NUM		= 10,		// 공성 병기 한단계 업그레이드시 필요한 자재수
		SIEGE_ARMS_REPAIR_HP_PER_MATERIAL	= 10,		// 자재 하나당 10% 의 HP 수리

		AIRSHIP_RIDER_NUM					= 10,		// 수송선 최대 탑승 인원
		AIRSHIP_PASSENGER_NUM				= 9,		// 수송선 승객 최대 인원
	};

	enum Const
	{
		INSIDE								= 0,		// 뒷문 사용( 안으로 )
		OUTSIDE								= 1,		// 뒷문 사용( 밖으로 )

		MAX_UPGRADE_NUM						= 5,		// 공성 오브젝트 업그레이드 최대치
		UPGRADE_KIND_NUM					= 6,		// 업그레이트 종류 수 (0 ~ 5)

		VIRTUAL_CID_START_NUM				= 1000,		// 길드 요새, 공성 병기의 가상 CID 인덱스 시작 번호

		BROADCAST_CELL_SIZE					= 5,		// 브로드캐스트 반경 (5셀)
		BROADCAST_RADIUS					= 150,		// 브로드캐스트 반경 (150m)
		BROADCAST_SQUARED_RADIUS			= 22500,	// 브로드캐스트 반경 제곱값 (150 x 150)

		MAX_HP_UPDATE_COUNT					= 5,		// 5 번에 한번은 DB 중계서버로 HP 정보 갱신

		MAX_REPAIR_GOLD						= 99999999,	// 수리를 할수 없는 객체를 수리할때 드는 디폴트 비용 (에러나게 하기위해)

		MAX_SIEGE_OBJECT					= 5,

		CREATE_LEVEL_LIMIT					= 80
	};

	enum RiderValue
	{
		NOT_RIDER							= 0,		// 탑승하지 않았음
		RIDER_FOR_OWNER						= 1,		// 주인으로 탑승한 상태
		RIDER_FOR_PASSENGER					= 2			// 승객으로 탑승한 상태
	};

	enum TimeValue
	{
/*
		// Test : 길드 요새 테스트용 시간
		DEFAULT_TIME_VALUE			= 1,		//     1 분
		DEFAULT_REPAIR_TIME			= 30,		//    30 초

		CAMP_ENCAMPING_TIME			= 1,		//     1 분
		CAMP_CANCELING_TIME			= 1,		//     1 분
		CAMP_REPAIR_TIME			= 30,		//    30 초
		CAMP_TO_STARTKIT_TIME		= 1,		//     1 분
		CAMP_CHANGING_TIME			= 1,		//     1 분
		CAMP_LEAST_USE_TIME			= 7,		//     7 일
		CAMP_ENCAMPING_INTERVAL		= 10,		//    10 분 간격으로 구축중 메세지 보냄
		CAMP_ATTACKED_INTERVAL		= 60000,	// 60000 ms (1분)
		MINING_CAMP_GAIN_COUNT		= 1,		//	   1 회
		CAMP_SHOP_TRANSFER_COUNT	= 1,		//     1 회
		WEAPON_CHARGE_TIME			= 1,		//	   1 분
*/

		DEFAULT_TIME_VALUE			= 10,		//    10 분
		DEFAULT_REPAIR_TIME			= 30,		//    30 초

		CAMP_ENCAMPING_TIME			= 30,		//    30 분
		CAMP_CANCELING_TIME			= 20,		//    20 분
		CAMP_REPAIR_TIME			= 30,		//    30 초
		CAMP_TO_STARTKIT_TIME		= 10,		//    10 분
		CAMP_CHANGING_TIME			= 10,		//    10 분
		CAMP_LEAST_USE_TIME			= 7,		//     7 일
		CAMP_ENCAMPING_INTERVAL		= 10,		//    10 분 간격으로 구축중 메세지 보냄
		CAMP_ATTACKED_INTERVAL		= 60000,	// 60000 ms (1분)
		MINING_CAMP_GAIN_COUNT		= 1,		//     1 회
		CAMP_SHOP_TRANSFER_COUNT	= 1,		//     1 회
		WEAPON_CHARGE_TIME			= 10,		//	  10 분

		EMBLEM_SUMMON_TIME			= 1,		//	   1 분
		EMBLEM_ATTACKED_INTERVAL	= 60000,	// 60000 ms (1분)

		SIEGE_ARMS_BUILDING_TIME	= 3,		//     3 분
		SIEGE_ARMS_TO_STARTKIT_TIME	= 3,		//     1 분
		SIEGE_ARMS_LEAST_USE_TIME	= 10,		//    10 분		

		CASTLE_ARMS_LEAST_USE_TIME	= 10,		//	  10 분
	};

	enum eReturn
	{
		RET_OK					= 0,
		RET_DESTROY_CAMP		= 1,			// 길드 요새 객체 삭제
		RET_DESTROY_SIEGE_ARMS	= 2,			// 공성 병기 객체 삭제
		RET_CHANGE_TYPE			= 3,			// 길드 요새 타입 변형		
	};

	enum eUpdateHPType
	{
		NOW_HP					= 0,			// 현재 HP 그대로
		UPGRADE_HP				= 1,			// 업그레이드된 HP 로
		REPAIR_HP				= 2,			// 수리된 HP 로
		FULL_HP					= 3,			// Full HP 로
	};


	static int GetKID(unsigned short wObjectType, unsigned char cState, unsigned char cUpgradeType, unsigned char cUpgradeStep)
	{
		int nKID = 0;

		if (Siege::EMBLEM == wObjectType)
		{
			nKID = wObjectType
				   + (cState * Siege::JEWEL_TYPE_NUM * Siege::UPGRADE_KIND_NUM)
				   + (cUpgradeType * Siege::UPGRADE_KIND_NUM)
				   + cUpgradeStep;
		}
		else
		{
			nKID = wObjectType
				+ (cState * Siege::UPGRADE_KIND_NUM)
				+ cUpgradeStep;
		}

		return nKID;
	}

	static int GetUpgradeMaterialNum(unsigned short wObjectType)
	{
		int nNum = 0;

		switch (wObjectType)
		{
		case Siege::MINING_CAMP:
			nNum = Siege::MINING_CAMP_UPGRADE_MATERIAL_NUM;
			break;

		case Siege::CAMP_SHOP:
			nNum = Siege::CAMP_SHOP_UPGRADE_MATERIAL_NUM;
			break;

		case Siege::CAMP:
		case Siege::KARTERANT_WEAPON:
		case Siege::MERKADIA_WEAPON:
			nNum = Siege::CAMP_UPGRADE_MATERIAL_NUM;
			break;
		}

		return nNum;
	}

	static int GetChangingTypeMaterialNum(unsigned short wBeforeType, unsigned short wAfterType, unsigned char cUpgradeStep)
	{
		int nBeforeNum = GetUpgradeMaterialNum(wBeforeType);
		int nAfterNum = GetUpgradeMaterialNum(wAfterType);

		if (nBeforeNum < nAfterNum)
		{
			return ( (nAfterNum - nBeforeNum) * cUpgradeStep );
		}

		return 0;
	}
}

#endif	// _SIEGE_CONSTANTS_H_
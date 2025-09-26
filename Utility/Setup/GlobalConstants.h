#ifndef _GLOBAL_CONSTANTS_H_
#define _GLOBAL_CONSTANTS_H_

namespace GameRYL
{
	enum GameContents
	{
		LEVEL_LIMIT_80		= (1 << 0),											// 80레벨 제한
		LEVEL_LIMIT_90		= (1 << 1) | LEVEL_LIMIT_80,						// 90레벨 제한
		LEVEL_LIMIT_100		= (1 << 2) | LEVEL_LIMIT_90 | LEVEL_LIMIT_80,		// 100레벨 제한

		CAMP				= (1 << 3),											// 요새
		CAMP_UPGRADE		= (1 << 4) | CAMP,									// 요새 강화
		SIEGE				= (1 << 5) | CAMP_UPGRADE | CAMP,					// 공성전

		JUDGE_PROTOTYPE		= (1 << 6),											// 적아군 식별 (초기 버전)

		ACCESSORY			= (1 << 7),											// 악세사리
		RUNE				= (1 << 8),											// 룬
		BATTLE_LOHAN		= (1 << 9),											// 배틀 로한
		STONE_BATTLE		= (1 << 10),										// 석상전
		LOTTO				= (1 << 11),										// 대만 로또.
		SHOW_ENEMYNAME		= (1 << 12),										// 적국 캐릭터명 표시
		NEWZONE_ZONE9		= (1 << 13),										// 신규존
		REBALANCE_OVERITEM	= (1 << 14),										// 오버아이템 조정
		FIFTHSKILL_LOCKITEM	= (1 << 15)											// 5단계 스킬 락 아이템
	};

	enum ServiceNation
	{
		KOREA				= 0,		// 한국 (디폴트)
		GLOBAL				= 1,		// 글로벌
		CHINA				= 2,		// 중국
		THAILAND			= 3,		// 태국 IME때문에 저장
		JAPAN				= 4,		// 일본 출력용때문에 저장
	
		MAX_NATION_TYPE		= 5
	};

	// 클라이언트용 (각 서버군에 알맞게 클라이언트 루틴 분리를 위해...)
	enum ServerType
	{
		SERVER_TEST				= 0,	// 테스트 서버
		SERVER_REGULAR			= 1,	// 정식 서버
		SERVER_BATTLE_LOHAN		= 2,	// 배틀 로한 서버

		MAX_SERVER_TYPE			= 3
	};
};

#endif
#ifndef _GAME_TIME_CONSTANTS_H_
#define _GAME_TIME_CONSTANTS_H_

namespace GameTime
{
	// 게임 시간 타입
	enum TimeType
	{
		TYPE_NONE	= 0,
		GUILD		= 1,			// 길드전 시간
		REALM		= 2,			// 렐름전 시간
		SIEGE		= 3,			// 공성 시간
		MINE		= 4,			// 매일 주기적으로 임시 채굴
		NEWZONE		= 5,			// 테섭용 신규존 입장제한

		MAX_GAME_TIME_TYPE	= 6
	};


	// 시간 주기
	enum Cycle
	{
		CYCLE_NONE		= 0,
		WEEKLY			= 1,		// 매주 특저 요일 특정 시간대
		DAILY			= 2,		// 매일 특정 시간대
		MONTHLY			= 3,		// 매월 특정 날짜 특정 시간대
		DATE			= 4,		// 특정 날짜 특정 시간대

		MAX_TIME_CYCLE_TYPE	= 5
	};


	// 요일
	enum Week
	{
		WEEK_NONE	= 0,
		SUNDAY		= 1,
		MONDAY		= 2,
		TUESDAY		= 3,
		WEDNESDAY	= 4,
		THURSDAY	= 5,
		FRIDAY		= 6,
		SATURDAY	= 7,
	};


	// 길드전 시간 종류
	enum GuildTime
	{
		GT_NORMAL				= 0,	// 일반
		GT_PREPARE_START_30		= 1,	// 길드전 시작 30분전
		GT_PREPARE_START_5		= 2,	// 길드전 시작 5분전
		GT_GUILD_WAR			= 3,	// 길드전 시간
		GT_PREPARE_END_30		= 4,	// 길드전 종료 30분전
	};


	// 렐름전 시간 종류
	enum RealmTime
	{
		RT_NORMAL				= 0,	// 일반
		RT_PREPARE_START_30		= 1,	// 렐름전 시작 30분전
		RT_PREPARE_START_15		= 2,	// 렐름전 시작 15분전
		RT_PREPARE_START_10		= 3,	// 렐름전 시작 10분전
		RT_PREPARE_START_5		= 4,	// 렐름전 시작 5분전
		RT_REALM_WAR			= 5,	// 렐름전 시간
		RT_REALM_WAR_END_5		= 6,	// 렐름전 종료 5분전
	};


	// 공성전 시간 종류
	enum SiegeTime
	{
		ST_NORMAL				= 0,	// 일반
		ST_PREPARE_START_5		= 1,	// 공성전 시작 5분전
		ST_SIEGE_WAR			= 2,	// 공성 시간
		ST_PREPARE_END_5		= 3,	// 공성전 종료 5분전
	};

	// 신규존 시간 종류
	enum NewZoneTime
	{
		ET_LIMIT				= 0,	// 입장 불가
		ET_PREPARE_START_5		= 1,	// 입장 5분전
		ET_ALLOWENTER			= 2,	// 입장 가능
		ET_PREPARE_END_5		= 3,	// 종료 5분전
	};

	// 임새 채굴 시간 종류
	enum MiningTime
	{
		MT_NORMAL				= 0,	// 일반
		MT_MINING				= 1,	// 채굴중인 시간 (MT_MINING -> MT_NORMAL 로 바뀔때 임시 채굴됨)
	};

	// 게임 이벤트 종류
	enum EventTime
	{
		EVENT_NORMAL				= 0,	// 이벤트 시간이 아니다.
		EVENT_All20					= 1,	// Value1 에 의해 결정되는 값으로 Value1 값이 이벤트 값이된다.
		EVENT_All50					= 2,	// Value1 에 의해 결정되는 값으로 Value1 값이 이벤트 값이된다.
	};

	// GameTimeInfo 패킷 종류
	enum GameTimeInfoType
	{
		TYPE_GAMESERVER_LOGIN		= 0,		// 게임서버가 중계서버에 연결될때 게임 시간 정보 전송
		TYPE_CHARACTER_LOGIN		= 1,		// 캐릭터가 게임서버에 로그인시 게임 시간 정보 전송
		TYPE_CHANGE_GAME_TIME		= 2,		// 게임 시간이 변경되어서 업데이트시
		TYPE_ENABLE_CASTLE_SIEGE	= 3,		// 특정 성의 공성 가능 정보 변경시
		TYPE_UPDATE_TIME			= 4,		// 클라이언트에게 주기적으로 시간 정보 업데이트시
	};
}

#endif	//_GAME_TIME_CONSTANTS_H_
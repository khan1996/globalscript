
#ifndef _GUILD_CONSTANTS_H_
#define _GUILD_CONSTANTS_H_

namespace Guild
{
	enum Const
	{
		MAX_GUILD_NAME_LEN			= 11,				// 길드 이름의 최대 길이 (실제 길이)
		MAX_GUILD_NAME_FOR_DB		= 20,				// 길드 이름의 최대 길이 (DB 저장용)
		MAX_MEMBER_NAME_LEN			= 16,				// 길드원 이름의 최대 길이 (일반적인 캐릭터 이름의 최대 길이와 동일)

		MAX_MARK_SIZE				= 18 * 12 * 2 + 1,	// 길드 마크 크기 (18 * 12 크기의 16비트 칼라의 BMP 파일, 첫 1바이트는 마크 존재 여부)
		MAX_RIGHT_SIZE				= 50,				// 길드 권한 구조체 크기 

		MAX_MEMBER_NUM				= 100,				// 멤버 수가 가질 수 있는 최대 크기 (가입대기자 때문에 여유분을 더 둔다.)
		MAX_RELATION_NUM			= 255,				// 우호/적대 관계 리스트의 최대 크기 (카운터 적대 신청때문에 여유분을 더 둔다.)
		MAX_HOSTILITY_APPLY_NUM		= 49,				// 대상으로 받을수 있는 최대 적대 관계 수
		MAX_HOSTILITY_NUM			= 1,				// 자신이 적대 선언할 수 있는 최대 수
		MAX_ALERT_NUM				= 1,				// 자신이 경계 적대 선언할 수 있는 최대 수

		MAX_LEVEL					= 5,				// 길드의 최대 레벨
		CREATE_LEVEL				= 30,				// 길드 생성시 요구되는 최소 캐릭터 레벨
		CREATE_LEVEL_FOR_CHINA		= 15,				// (중국용) 길드 생성시 요구되는 최소 캐릭터 레벨
		SET_MARK_GOLD				= 5000000,			// 마크 변경시 필요한 금액
		FAME_LEAVE_PENALTY			= 1000,				// 길드마스터의 승인 없이 탈퇴시 깎이는 명성

		MAX_SMALL_NODE_NUM			= 500,				// 한번에 전송 가능한 길드 간략 정보의 최대 숫자
		MEMBER_INFO_UPDATE_COUNT	= 10,				// 길드원 정보를 다른 서버에 업데이트하는 타이밍
		DELETE_GUILD_MEMBER_NUM		= 4,				// 길드가 유지될 수 있는 최소 인원

		MINUTES_PER_HOUR			= 60,				

		// 해외의 요청을 수용하여 탈퇴 대기 시간이 없어졌습니다. (2005-06-13 by 로딘)
		LEAVE_WAIT_TIME				= 0,				// 탈퇴 대기 기간 (단위 : 분)
		LEAVE_WAIT_TIME_FOR_CHINA	= 10080,			// (중국용) 탈퇴 대기 기간 (단위 : 분)
		DELETE_GUILD_LOGOUT_TIME	= 43200,			// 길드원이 로그인하지 않아 길드가 해체될 때까지의 기간 (단위 : 분)
		MEMBER_NUM_DELETE_TIME		= 1440,				// 길드원이 DELETE_GUILD_MEMBER_NUM 미만이 된 후 길드 해체까지의 기간 (단위 : 분)
		TRANFER_MASTER_TIME			= 20160,			// 오랫동안 접속하지 않은 길드마스터의 권한 양도 기간 (단위 : 분)
		CHANGE_INCLINATION_TIME		= 1440,				// 길드 성향 변경 후 다시 변경할 수 있을 때까지의 기간 (단위 : 분)
		RELATION_WAIT_TIME			= 1440,				// 관계 변경 대기 시간 (단위 : 분)

/*
		// Test : 길드 테스트를 위한 시간
		LEAVE_WAIT_TIME_FOR_CHINA	= 10,				// (중국용) 탈퇴 대기 기간 (단위 : 분)
		DELETE_GUILD_LOGOUT_TIME	= 30,				// 길드원이 로그인하지 않아 길드가 해체될 때까지의 기간 (단위 : 분)
		MEMBER_NUM_DELETE_TIME		= 15,				// 길드원이 DELETE_GUILD_MEMBER_NUM 미만이 된 후 길드 해체까지의 기간 (단위 : 분)
		TRANFER_MASTER_TIME			= 20,				// 오랫동안 접속하지 않은 길드마스터의 권한 양도 기간 (단위 : 분)
		CHANGE_INCLINATION_TIME		= 10,				// 길드 성향 변경 후 다시 변경할 수 있을 때까지의 기간 (단위 : 분)
		RELATION_WAIT_TIME			= 1,				// 관계 변경 대기 시간 (단위 : 분)
*/

		MEMBER_WAIT_BIT				= 0x80000000,		// 가입 대기자에게 마크를 달지못하게 하는 플래그

		TIME_GUILD_POINT			= 30,				// 길드전 재정비 시간(분단위).

		TACTICE_JON_LEVEL			= 1,				// 용병 가입 레벨.

		TACTICS						= 1,			// 용병.
		TACTICS_WAIT				= 2,			// 용병 가입 대기자.
	};

	enum Title
	{
		NONE			= 0, 
		MASTER			= 1,			// 길드 마스터
		MIDDLE_ADMIN	= 2,			// 중간 관리자
		COMMON			= 3,			// 일반 길드원
		LEAVE_WAIT		= 4,			// 탈퇴 대기자
		JOIN_WAIT		= 5,			// 가입 대기자		

		MAX_TITLE		= 6
	};

	enum MemberType
	{
		TYPE_MEMBER		= 0,			// 멤버
		TYPE_TACTICS	= 1				// 용병
	};

	enum GuildSort
	{
		GUILD_FAME			= 0,		// 순위(명성)
		GUILD_NAME			= 1,		// 길드명
		GUILD_HOSTILITY		= 2,		// 적대길드
		GUILD_NEUTRALITY	= 3,		// 비적대길드
		GUILD_MINE			= 4,		// 자신의 길드

		GUILD_REQUEST		= 5			// 길드 리스트 요청 (셀 로긴 등에 의한 무작위 정보 요청)
	};

	enum MemberSort
	{
		MEMBER_TITLE		= 0,		// 직위순
		MEMBER_CLASS		= 1,		// 클래스별 소트
		MEMBER_LEVEL		= 2,		// 레벨별 소트
		MEMBER_FAME			= 3,		// 명성별 소트
		MEMBER_NAME			= 4,		// 이름별 소트
		MEMBER_GOLD			= 5,		// 소지금액별 소트
		MEMBER_POSITION		= 6,		// 위치별 소트
		MEMBER_JOIN_WAIT	= 7,		// 신청중인 플레이어
		MEMBER_WAR_ON		= 8,		// 길드 전쟁 참여중인 멤버 (이름순으로 소트)
		MEMBER_WAR_OFF		= 9,		// 길드 전쟁 불참중인 멤버 (이름순으로 소트)
		TACTICS_JOIN_WAIT	= 10,		// 신청중인 용병 (높은 레벨 순)
		TACTICS_ACTIVE		= 11		// 활동중인 용병 (높은 레벨 순)
	};

	enum Relation
	{
        NEUTRALITY			= 0,			// 중립 관계
		HOSTILITY			= 1,			// 적대
		COUNTER_HOSTILITY	= 2,			// 카운터 적대
		ALERT_HOSTILITY		= 3,			// 경계 적대
		MINE				= 4,			// 자신의 길드
		TARGET_NEUTRALITY	= 5,			// A->B적대를 하면 DB값이 1로 저장되고 이상태에서 피스를 선언하면
											// 해당값이 5로 바뀐다. 5로 바뀐뒤에는 B길드가 적대선언삭제를 해주면
											// 이때 DB에서 사라져 완전히 평화관계가 된다.

		ALL_HOSTILITY		= 6				// 모든 적대 관계
	};

	enum RelationActor
	{
		BY_MINE				= 0,			// 자신이 맺은 길드 관계
		BY_TARGET			= 1,			// 대상으로부터 받은 길드 관계

		MAX_RELATION_ACTOR	= 2
	};

	// 중계 서버 전용
	enum TimeUpdateType
	{
		// 수정시엔 CDBObject::UpdateGuildTime 함수를 주의

		LAST_LOGOUT		= 0,			// 길드원이 마지막으로 로그아웃한 시각
		MEMBER_NUM_DOWN	= 1,			// 길드원 수가 3인 이하가 된 시각	
		MASTER_LOGOUT	= 2				// 마스터가 마지막으로 로그아웃한 시각
	};

	enum ErrType
	{
		S_SUCCESS			= 0,

		// CGuild::SetTitle 함수용
		E_NOT_MEMBER		= 1,		// 길드원이 아님
		E_DISSOLVE_GUILD	= 2,		// 길드 해체
		E_NOT_CHANGE		= 3,		// 변경된 바 없음
		E_DB_ERROR			= 4,		// DB 에러
		E_NOT_TEMP_MASTER	= 5			// 임시 마스터가 될 길드원이 없음
	};

	enum AddFameType
	{
		// CGuildDB::AddAllMemberFame, AddFriendGuildMemberFame, AddMemberFame 함수용

		TYPE_VALUE			= 0,		// Fame 값을 그대로 더해준다.
		TYPE_HALF			= 1,		// 명성치를 1/2로 만든다.
		TYPE_QUARTER		= 2			// 명성치를 1/4로 만든다.
	};

	// 게임 서버 전용
	enum MemberInfoUpdateType
	{
		// CGuild::UpdateMemberInfo 함수 참조 

		TYPE_LEVEL		= 0,
		TYPE_CLASS		= 1,
		TYPE_FAME		= 2,
		TYPE_GOLD		= 3,
		TYPE_WARFLAG	= 4
	};

	const unsigned char ms_aryMaxMemberNum[MAX_LEVEL] 			= { 10,			20,			30,			40,			50			};
	const unsigned long ms_arySetLevelFame[MAX_LEVEL] 			= { 0,			1000,		3000,		5000,		10000		};
	const unsigned long ms_arySetLevelFameForUSA[MAX_LEVEL] 	= { 0,			400,		2000,		5000,		10000		};
	const unsigned long ms_arySetLevelGold[MAX_LEVEL] 			= { 1000000,	5000000,	10000000,	50000000,	100000000	};
	const unsigned long ms_arySetLevelGoldForChina[MAX_LEVEL]	= { 100000,		5000000,	10000000,	50000000,	100000000	};
};

#endif
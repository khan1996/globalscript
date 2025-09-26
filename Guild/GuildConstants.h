
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

		MAX_MEMBER_NUM				= 50,				// 멤버 수가 가질 수 있는 최대 크기(ms_aryMaxMemberNum[MAX_LEVEL - 1] 값과 동일해야 한다.)
		MAX_RELATION_NUM			= 20,				// 우호/적대 관계 리스트의 최대 크기

		MAX_LEVEL					= 5,				// 길드의 최대 레벨
		CREATE_LEVEL				= 30,				// 길드 생성시 요구되는 최소 캐릭터 레벨
		SET_MARK_GOLD				= 5000000,			// 마크 변경시 필요한 금액

		MAX_SMALL_NODE_NUM			= 500,				// 한번에 전송 가능한 길드 간략 정보의 최대 숫자
		MEMBER_INFO_UPDATE_COUNT	= 10,				// 길드원 정보를 다른 서버에 업데이트하는 타이밍
		DELETE_GUILD_MEMBER_NUM		= 4,				// 길드가 유지될 수 있는 최소 인원

		DELETE_GUILD_LOGOUT_TIME	= 30,				// 길드원이 로그인하지 않아 길드가 해체될 때까지의 기간 (단위 : 일)
		MEMBER_NUM_DELETE_TIME		= 1,				// 길드원이 DELETE_GUILD_MEMBER_NUM 미만이 된 후 길드 해체까지의 기간 (단위 : 일)
		TRANFER_MASTER_TIME			= 14,				// 오랫동안 접속하지 않은 길드마스터의 권한 양도 기간 (단위 : 일)
		LEAVE_WAIT_TIME				= 24,				// 탈퇴 대기 기간 (단위 : 시간)
		
		MEMBER_WAIT_BIT				= 0x80000000		// 가입/탈퇴 대기자에게 마크를 달지못하게 하는 플래그
	};

	enum Title
	{
		NONE			= 0, 
		MASTER			= 1,					// 길드 마스터
		MIDDLE_ADMIN	= 2,					// 중간 관리자
		COMMON			= 3,					// 일반 길드원
		LEAVE_WAIT		= 4,					// 탈퇴 대기자
		JOIN_WAIT		= 5,					// 가입 대기자

		MAX_TITLE		= 6
	};

	enum Relation
	{
		NEUTRALITY		= 0,					// 중립 관계
		FRIENDLY		= 1,					// 우호 관계
		HOSTILITY		= 2,					// 적대 관계
		MINE			= 3						// 자신의 길드
	};

	// 중계 서버 전용
	enum TimeUpdateType
	{
		// 수정시엔 CDBObject::UpdateGuildTime 함수를 주의

		LAST_LOGOUT		= 0,					// 길드원이 마지막으로 로그아웃한 시각
		MEMBER_NUM_DOWN	= 1,					// 길드원 수가 3인 이하가 된 시각	
		MEMBER_NUM_UP	= 2,					// 길드원 수가 4인 이상이 된 시각	
		MASTER_LOGOUT	= 3						// 마스터가 마지막으로 로그아웃한 시각
	};

	// 게임 서버 전용
	enum MemberInfoUpdateType
	{
		// CGuild::UpdateMemberInfo 함수 참조 

		TYPE_LEVEL		= 0,
		TYPE_CLASS		= 1,
		TYPE_FAME		= 2,
		TYPE_GOLD		= 3
	};

	enum Inclination
	{
		// 종족
		RACE_HUMAN			= 0x20,		// 인간
		RACE_AKHAN			= 0x40,		// 아칸
		RACE_BOTH			= 0x60,		// 양쪽 모두

		// 국적
		NATION_KARTERANT	= 0x04,		// 카르테란트
		NATION_MERKADIA		= 0x08,		// 메르카디아
		NATION_THIRD		= 0x10,		// 제3국적
		NATION_STATELESS	= 0x00,		// 무국적

		// 전쟁/평화 모드
		MODE_WAR			= 0x01,		// 전쟁 모드
		MODE_PEACE			= 0x02,		// 평화 모드

		// 논리 연산을 위한 비트셋
		RACE_BIT			= 0x60,		// 종족
		NATION_BIT			= 0x1C,		// 국적
		MODE_BIT			= 0x03		// 전쟁/평화 모드
	};
	
	inline unsigned char	Inclination2Race( unsigned char inclination ){ return (inclination&RACE_BIT); };
	inline unsigned char	Inclination2Nation( unsigned char inclination ){ return (inclination&NATION_BIT); };
	inline unsigned char	Inclination2PeaceMode( unsigned char inclination ){ return (inclination&MODE_BIT); };


	const unsigned char ms_aryMaxMemberNum[MAX_LEVEL] = { 10, 20, 30, 40, 50 };
	const unsigned long ms_arySetLevelFame[MAX_LEVEL] = { 0, 2000, 10000, 25000, 50000 };
	const unsigned long ms_arySetLevelGold[MAX_LEVEL] = { 1000000, 5000000, 10000000, 50000000, 100000000 };
};

#endif
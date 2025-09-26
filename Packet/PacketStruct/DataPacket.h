#ifndef _DATA_PACKET_H_
#define _DATA_PACKET_H_

#include <Network/Packet/PacketBase.h>

#pragma pack(1)

typedef struct PktDD* LPPktDD;
struct PktDD : public PktBase 
{
	unsigned long   m_dwRequestKey;
	unsigned short	m_wCmd;

    enum SubCommands
    {
        SCmdUserLogin			= 0x0001,	// 유저 로그인
        SCmdUserLogout			= 0x0002,	// 유저 로그인
        SCmdUserMove			= 0x0003,	// 유저 존 이동
		// WORK_LIST 2.1 계정 국적 추가
		SCmdSelectNation		= 0x0004,	// 유저 계정 국적
		// WORK_LIST 2.3 계정 국적 변경 기능 구현
		SCmdNationChangeResult	= 0x0005,

        SCmdCharCreate			= 0x0021,	// 캐릭터 생성
        SCmdCharDelete			= 0x0022,	// 캐릭터 삭제
        SCmdCharCreateItem		= 0x0023,	// 캐릭터 생성 아이템
        SCmdCharSelect			= 0x0024,	// 캐릭터 선택
        
        SCmdCreateParty			= 0x0031,	// 파티 생성
        SCmdDeleteParty			= 0x0032,	// 파티 삭제
        SCmdGetPartyInfo		= 0x0033,	// 파티 정보 얻기
        SCmdInsertPartyMem		= 0x0034,	// 파티 멤버 추가
        SCmdDeletePartyMem		= 0x0035,	// 파티 멤버 삭제
        SCmdLoginPartyMem		= 0x0036,	// 파티 멤버 로그인
        SCmdLogoutPartyMem		= 0x0037,	// 파티 멤버 로그아웃
        SCmdTransferLeader		= 0x0038,	// 파티 리더 양도
		SCmdAutoRoutingOn		= 0x0039,	// 파티 오토루팅 켜기
		SCmdAutoRoutingOff		= 0x003A,	// 파티 오토루팅 끄기
		//SCmdPartyAddress		= 0x003B,	// 파티원 UDP 주소 업데이트
		SCmdDeadPartyMem		= 0x003C,	// 파티원 사망
        SCmdMoveZonePartyMem    = 0x003D,   // 파티원 존이동        : RefID = 이동할 존 ID
        SCmdLevelUpPartyMem     = 0x003E,   // 파티원 레벨업        : RefID = 레벨업한 레벨
        SCmdChangeClassPartyMem = 0x003F,   // 파티원 클래스변경    : RefID = 바뀐 클래스
		SCmdChangeGuildPartyMem	= 0x0040,	// 파티원 길드 변경시	: RefID = 바뀐 길드 ID
		
        SCmdStartSession		= 0x0050,	// 세션 시작
        SCmdServerZone			= 0x0051,	// 서버 존
        SCmdCharMoveZone		= 0x0052,	// 캐릭터 존이동
        SCmdServerZoneEnd       = 0x0053	// 서버 존 끝
    };

	inline void InitPtSubCmd(unsigned long dwRequestKey, unsigned short Cmd_In);
};

inline void PktDD::InitPtSubCmd(unsigned long dwRequestKey, unsigned short Cmd_In)
{
	m_dwRequestKey  = dwRequestKey;
	m_wCmd          = Cmd_In;
}

#pragma pack()

#endif
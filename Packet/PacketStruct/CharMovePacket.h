#ifndef _CHAR_MOVE_PACKET_H_
#define _CHAR_MOVE_PACKET_H_

// CharMovePacket.h
#include <winsock2.h>
#include <windows.h>
#include <Network/Packet/PacketBase.h>
#include <DB/DBdefine.h>
#include <cmath>

#pragma pack(1)

class CNetworkPos
{
private:

    static const float  ms_fToRadian;
    static const float  ms_fToAngle;
    static const float  ms_fPositionDetail;
    static const float  ms_fVelocityDetail;

    unsigned short  m_usXPos;
    unsigned short  m_usYPos;
    unsigned short  m_usZPos;
    unsigned char   m_cDirection;   //  정밀도 - 0~180 (1에 2도)
    unsigned char   m_cVelocity;    //  속도/프레임(1프레임에 움직일 수 있는 최대거리.)
                                    //  속도 = 초당 이동거리 = 1~5미터.
                                    //  프레임 = 최대 60프레임.
                                    //  최대 가능한 속도/프레임 * 1000 = 0 ~ 255
                                    //  속도/프레임 = 0 ~ 0.255미터
    // TODO : maximum 10meter/sec    

public:

    CNetworkPos() : m_usXPos(0), m_usYPos(0), m_usZPos(0), m_cDirection(0), m_cVelocity(0) { }
    CNetworkPos(float fXPos, float fYPos, float fZPos, float fDir = 0.0f, float fVel = 0.0f) { Initialize(fXPos, fYPos, fZPos, fDir, fVel); }
    CNetworkPos(const POS& pos, float fDir = 0.0f, float fVel = 0.0f) { Initialize(pos.fPointX, pos.fPointY, pos.fPointZ, fDir, fVel); }

    void Initialize(float fXPos, float fYPos, float fZPos, float fDir, float fVel);

    float GetXPos() const { return static_cast<float>(m_usXPos) / ms_fPositionDetail; }
    float GetYPos() const { return static_cast<float>(m_usYPos) / ms_fPositionDetail; }
    float GetZPos() const { return static_cast<float>(m_usZPos) / ms_fPositionDetail; }

    float GetDirection() const { return static_cast<float>(m_cDirection) * 2 * ms_fToRadian; }
    float GetVelocity()  const { return static_cast<float>(m_cVelocity) / ms_fVelocityDetail; }

    float GetSquaredRange(const CNetworkPos& lhs) const
    {
        const float xDiff = GetXPos() - lhs.GetXPos();
        const float yDiff = GetYPos() - lhs.GetYPos();
        const float zDiff = GetZPos() - lhs.GetZPos();
        
        return (xDiff * xDiff) + (yDiff * yDiff) + (zDiff * zDiff);
    }

    float GetRange(const CNetworkPos& lhs) const { return std::sqrtf(GetSquaredRange(lhs)); }

    inline bool operator == (const CNetworkPos& lhs)
    {
        return (m_usXPos == lhs.m_usXPos && m_usYPos == lhs.m_usYPos && m_usZPos == lhs.m_usZPos
            && m_cDirection == lhs.m_cDirection && m_cVelocity == lhs.m_cVelocity);
    }

    inline bool operator != (const CNetworkPos& lhs) { return !(*this == lhs); }
};

inline void CNetworkPos::Initialize(float fXPos, float fYPos, float fZPos, float fDir, float fVel)
{
	float fbuf;
	fbuf = fXPos * ms_fPositionDetail;
	m_usXPos = static_cast<unsigned short>(fbuf);
	fbuf = fYPos * ms_fPositionDetail;
	m_usYPos = static_cast<unsigned short>(fbuf);
	fbuf = fZPos * ms_fPositionDetail;
	m_usZPos = static_cast<unsigned short>(fbuf);

	/*
	// float의 소숫점 연산에 오차가 있어 반올림합니다.
    m_usXPos        = static_cast<unsigned short>(fXPos * ms_fPositionDetail + 0.5f);
    m_usYPos        = static_cast<unsigned short>(fYPos * ms_fPositionDetail + 0.5f);
    m_usZPos        = static_cast<unsigned short>(fZPos * ms_fPositionDetail + 0.5f);
	*/

    float fAngle = fDir * ms_fToAngle;
    while (360 <= fAngle) { fAngle -= 360; }

    m_cDirection    = static_cast<unsigned char>(fAngle/2);

    unsigned int nVelocity = static_cast<unsigned int>(fVel * ms_fVelocityDetail);
    if(255 < nVelocity) { nVelocity = 255; }

    m_cVelocity     = static_cast<unsigned char>(nVelocity);
}


// 캐릭터 이동 패킷 Ex (CharMove, TCP)
typedef struct PktMVEx* LPPktMVEx;
struct PktMVEx : public PktBase
{
    // CID는 PktBase의 dwServerInfo에 담는다.
    CNetworkPos     m_NetworkPos;    
    unsigned char   m_cUAct;
    unsigned char   m_cLAct;
};

/*
    Deplicated

// 캐릭터 이동 패킷	(CharMove, UDP)
typedef struct PktMV* LPPktMV;
struct PktMV : public PktBase
{
	POS				m_Position;     // 캐릭터 위치
	float			m_fDir;         // 캐릭터 방향
	unsigned long	m_dwCharID;     // 캐릭터 ID
    unsigned long	m_dwChantEf;	// Chant Effect
	unsigned long	m_dwEnchantEf;  // Enchant Effect
	unsigned short	m_wUAct;        // 액션 번호
	unsigned short	m_wLAct;        // 액션 번호
	unsigned char	m_wLevel;       // 캐릭터 레벨
};

// 캐릭터 이동 업데이트 패킷	( Char Move Update )
typedef struct PktMU* LPPktMU;
struct PktMU : public PktBase 
{
	POS				m_Position;     // 위치
   	float			m_fDir;         // 방향
	unsigned long	m_dwTick;       // 클라이언트 틱
	bool			m_bSitMode;     // 앉기 (true = 앉아 있음.)
};

// 캐릭터 이동 업데이트 패킷 Ack	( Char Move Update )
typedef struct PktMUAck* LPPktMUAck;
struct PktMUAck : public PktBase
{
	unsigned long	m_dwTick;       // 클라이언트 틱 (처리 시간 퍼포 측정)
	unsigned char	m_cFlag;        //	- 플래그 (0 = 이동, 1 = 셀이동)
};
*/


// 소환
typedef struct PktSummon* LPPktSummon;
struct PktSummon : public PktBase
{
	unsigned long	m_dwCharID;		// 캐릭터 ID
	unsigned long	m_dwTargetID;	// 대상 ID
	CNetworkPos		m_NetworkPos;	// 네트워크 위치
};

// 몬스터 움직임	( Monster Move )
typedef struct PktMM* LPPktMM;
struct PktMM : public PktBase
{
    unsigned long	m_dwMonID;      // 몬스터 아이디
    CNetworkPos     m_NetworkPos;   // 네트워크 위치
    unsigned char	m_cAct;         // 몬스터 행동
	unsigned char	m_cAniNum;      // 몬스터 이동 횟수
};


namespace CellCommand
{
    enum Type
    {
        CELL_MOVE   = 0,
        CELL_LOGIN  = 1,
        RESPAWN     = 2
    };
};

// 캐릭터 셀 로그인 패킷	(Char Cell Login)
typedef struct PktCCLi* LPPktCCLi;
struct PktCCLi : public PktBase
{
	unsigned char	m_cCmd;             // see namespace CellCommand
    // 패킷 뒤에 BroadcastInfo::CharInfo::SLoginPhase가 붙어 온다.
};

// 캐릭터 셀 로그아웃 패킷	( Char Cell Logout )
typedef struct PktCCLo* LPPktCCLo;
struct PktCCLo : public PktBase
{
	unsigned long	m_dwCharID;
	unsigned char	m_cCmd;             // see namespace CellCommand
};

// 셀 브로드 캐스팅 + Address구조체
typedef struct PktCB PktCB, *LPPktCB;
struct PktCB : public PktBase 
{
    unsigned char   m_cCharacterPhase;  // 셀 브로드캐스팅 Phase(캐릭터).
	unsigned short	m_usBufferSize;     // 셀 버퍼 크기
};				

#pragma pack()

#endif
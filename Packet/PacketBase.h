#ifndef _PACKET_BASE
#define _PACKET_BASE


////////////////////////////////////////////////////////////////////////////////////////////////////
//															
//	서버의 버퍼링 상태	
//
//	SS_SMOOTH			: 원활한 소통 중임.			
//	SS_LOADED			: 약간의 부하가 있슴.	(50 % of bufferring)			
//	SS_BUSY				: 과중한 부하가 있슴.	(70 % of bufferring)			
//	SS_VERYBUSY			: 극심한 과부하 상태임	(패킷 손실을 초래할 수 있슴.)			
//
////////////////////////////////////////////////////////////////////////////////////////////////////
union ServerInfo 
{
    enum ServerState 
    { 
        SS_SMOOTH, SS_LOADED, SS_BUSY, SS_VERYBUSY 
    };

	struct sSrvState
	{		
		unsigned short	wError;			// 에러 코드
		unsigned short	wSrvState;		// 서버 상태
	};
    
    sSrvState       SrvState;	
	unsigned long	dwServerInfo;
};




////////////////////////////////////////////////////////////////////////////////////////////////////
// 기본 패킷									
#pragma pack(1)

struct PktBase 
{
	enum PktBaseErr
	{
		NO_SERVER_ERR	= 0,
		SERVER_ERROR	= 1
	};

	typedef unsigned char	StartType;
	typedef unsigned char	CMDType;
	typedef unsigned short	LengthType;
	
protected:

	StartType		m_StartBit;
	CMDType			m_Cmd;
	LengthType		m_Len;				// 헤더 길이를 포함한 패킷 길이

	unsigned long	m_CodePage;
	ServerInfo		m_SrvInfo;

public:

	inline void InitPtHead(unsigned short Len_In, unsigned char Cmd_In,
		unsigned short State_In, unsigned short Error_In);

	inline void InitPtHead(unsigned short Len_In, unsigned char Cmd_In, unsigned long Tick_In);
	
	inline void SetError(unsigned short Error)		{ m_SrvInfo.SrvState.wError = Error;	}
	inline unsigned short GetError(void)			{ return m_SrvInfo.SrvState.wError;	}
    inline unsigned short GetState(void)            { return m_SrvInfo.SrvState.wSrvState; }

	inline StartType GetStartBit(void)				{ return m_StartBit; }

	inline void SetCodePage(unsigned long Code)		{ m_CodePage = Code; }
	inline unsigned long GetCodePage(void)			{ return m_CodePage; }

	inline void	SetServerInfo(unsigned long Info)	{ m_SrvInfo.dwServerInfo = Info; }
	inline unsigned long GetServerInfo(void)		{ return m_SrvInfo.dwServerInfo; }

	inline CMDType GetCmd(void) const               { return m_Cmd;	}
	
	inline bool IsCrypt(void);
	inline void SetCrypt(void);
	inline bool IsCompress(void);
	inline void SetCompress(void);
	inline LengthType GetUncompressLen(void);

	inline void SetLen(LengthType Len);
	inline LengthType GetLen(void);
};
typedef PktBase* LPPktBase;


// 여러개를 압축해서 보낼 때 사용하는 패킷 헤더.
struct PktBlockCompressedBase
{
public:

    unsigned short  GetLength() const { return m_usLength; }
    unsigned char   GetCmd()    const { return m_cCommand; }
    unsigned char   GetExtra()  const { return m_cExtra;   }

    void InitPtHead(unsigned short usLength, unsigned char cCmd, unsigned char cExtra)
    {
        m_usLength = usLength;
        m_cCommand = cCmd;
        m_cExtra = cExtra;
    }

protected:

    unsigned short  m_usLength;
    unsigned char   m_cCommand;
    unsigned char   m_cExtra;
};

#pragma pack()


const unsigned int	PktBaseSize = sizeof(PktBase);
const PktBase::StartType	StartBit	= 0xFF;		// 시작 비트
const PktBase::LengthType	Crypt		= 0x8000;	// 암호화 여부 - 1000B
const PktBase::LengthType	Compress	= 0x4000;	// 압축 여부 - 0100B
const PktBase::LengthType	LengthHdr	= 0xC000;	// 1100B
const PktBase::LengthType	LengthMask	= 0x3FFF;	// 실제 길이 얻어내느 마스크 값. 전부 14비트. 최대 16383byte.
const PktBase::LengthType	PktMinLen	= sizeof(PktBase);
const PktBase::LengthType	PktMaxLen	= 16383;

const PktBase::LengthType	MIN_PACKET_LEN	= sizeof(PktBase);
const PktBase::LengthType	MAX_PACKET_LEN	= 16383;

inline bool PktBase::IsCrypt(void)		{ return ((m_Len & Crypt) == Crypt) ? true : false; }
inline void PktBase::SetCrypt(void)		{ m_Len |= Crypt; }

inline bool PktBase::IsCompress(void)	{ return ((m_Len & Compress) == Compress) ? true : false; }
inline void PktBase::SetCompress(void)	{ m_Len |= Compress; }
inline PktBase::LengthType PktBase::GetUncompressLen(void) { return PktMaxLen; }

inline void PktBase::SetLen(PktBase::LengthType Len) { m_Len = (m_Len & LengthHdr) | Len; }
inline PktBase::LengthType PktBase::GetLen(void)	 { return m_Len & LengthMask; }



//Interface/////////////////////////////////////////////////////////////////////////////////////
//
// 패킷 해더 초기화
//
// Parameter :
//	1st : 길이[In]
//	2st : 커맨드[In]
//	3st : 서버 상태[In]
//	4st : 에러[In]
//
// Return :
//
///////////////////////////////////////////////////////////////////////////////////////////////
inline void PktBase::InitPtHead(unsigned short Len_In, unsigned char Cmd_In, 
                                unsigned short State_In, unsigned short Error_In)
{
	m_StartBit						= StartBit;
	m_Cmd							= Cmd_In;
	m_Len							= Len_In;	
	m_CodePage						= 0;
	m_SrvInfo.SrvState.wSrvState	= State_In;
	m_SrvInfo.SrvState.wError		= Error_In;	
}

inline void PktBase::InitPtHead(unsigned short Len_In, unsigned char Cmd_In, unsigned long Tick_In)
{
	m_StartBit						= StartBit;
	m_Cmd							= Cmd_In;
	m_Len							= Len_In;	
	m_CodePage						= 0;
	m_SrvInfo.dwServerInfo			= Tick_In;
}


#endif
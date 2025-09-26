///////////////////////////////////////////////////////////////////////////////////////////////
//
// ClientSocket Class
//
///////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "ClientSocket.h"

#include <mmsystem.h>
#include <Network/ClientNetwork/SessionMgr.h>
#include <Network/ClientNetwork/TCPSession.h>
#include <Network/ClientNetwork/UDPSession.h>
#include <Network/ClientNetwork/NullHandler.h>
#include <Network/ClientNetwork/ClientEventHandler.h>

#include <ctime>
#include "GMMemory.h"

// forward decl
bool GetHostIP(IN_ADDR &Addr, bool AllowVirtual);
bool IsWrongIP(IN_ADDR &Addr);
bool GetNATAddress(SOCKET Socket_In, SOCKADDR_IN *Address_Out, bool AllowVirtual);

ClientSocket* ClientSocket::s_pInstance = 0;

ClientSocket* ClientSocket::CreateInstance()
{ 
	if( s_pInstance )
		delete s_pInstance;

	s_pInstance = new ClientSocket;
	return s_pInstance; 
};

ClientSocket::ClientSocket()
:   m_dwStatusFlag(0)
{
    memset(m_Address, 0, sizeof(SOCKADDR_IN) * (MAX_ADDRESS + 1));
    memset(m_lpEventHandler, 0, sizeof(ClientNet::CClientEventHandler*) * MAX_HANDLER + 1);

    m_bStatusFlag.reset();

	m_lpNullEventHandler = &ClientNet::CNullClientEventHandler::GetInstance();
#include "GMUndefNew.h"
    m_lpEventHandlerMgr = new (std::nothrow) ClientNet::CClientEventHandlerMgr;
#include "GMRedefNew.h"

	InitNSCheckList();
}


ClientSocket::~ClientSocket(void)
{
    delete m_lpEventHandlerMgr;
}


bool ClientSocket::Open(const char* address, unsigned short usport,
                        ClientNet::CClientEventHandler* lpEventHandler)
{
    return (0 != lpEventHandler && 0 != m_lpEventHandlerMgr && 
        m_lpEventHandlerMgr->Connect(INET_Addr(address, usport), lpEventHandler));
}

void ClientSocket::SetHandler(ClientNet::CClientEventHandler* lpHandler, HandlerType eHandlerType)
{
    m_lpEventHandler[eHandlerType] = lpHandler;
}

void ClientSocket::ResetHandler(ClientNet::CClientEventHandler* lpHandler, HandlerType eHandlerType)
{
    if (lpHandler == m_lpEventHandler[eHandlerType])
    {
        m_lpEventHandler[eHandlerType] = 0;
    }
}

bool ClientSocket::IsConnected(HandlerType eHandlerType) 
{ 
    return (0 != m_lpEventHandler[eHandlerType]);
}

void ClientSocket::Disconnect(HandlerType eHandlerType)
{
    if (0 != m_lpEventHandlerMgr)
    {
        if (eHandlerType == MAX_HANDLER)
        {
            for(int idx = 0; idx < MAX_HANDLER; ++idx)
            {
                if(0 != m_lpEventHandler[idx])
                {
                    m_lpEventHandlerMgr->Close(m_lpEventHandler[idx]);
                }
            }
        }
        else if(0 != m_lpEventHandler[eHandlerType])
        {
            m_lpEventHandlerMgr->Close(m_lpEventHandler[eHandlerType]);
        }
    }
}

void ClientSocket::SetAddress(AddressType eAddrType, const char* szAddress, unsigned port)
{
    m_Address[eAddrType] = INET_Addr(szAddress, port).get_addr_in();
}

void ClientSocket::HandleEvents(DWORD dwProcessPerCall)
{
    if(0 != m_lpEventHandlerMgr)
    {
        m_lpEventHandlerMgr->HandleEvents(dwProcessPerCall);
    }
}

bool GetHostIP(IN_ADDR &Addr, bool AllowVirtual)
{
    char HostName[256];
    if(!gethostname(HostName, 255))
    {
        PHOSTENT pHost = gethostbyname(HostName);
        if(NULL == pHost)
        {
            return false;
        }

		int Count = 0;
        for(;pHost->h_addr_list[Count]; ++Count)
        {
            memcpy(&(Addr.S_un.S_addr), pHost->h_addr_list[Count], sizeof(IN_ADDR));

            if(!AllowVirtual)
            {
                // 가상 아이피 확인
                // 10.0.0.0		~ 10.255.255.255
                // 172.16.0.0	~ 172.31.255.255
                // 192.168.0.0	~ 192.168.255.255
                if((unsigned char)10 == Addr.S_un.S_un_b.s_b1)
                {
                    continue;
                }
                else if((unsigned char)172 == Addr.S_un.S_un_b.s_b1)
                {
                    if(Addr.S_un.S_un_b.s_b2 >= (unsigned char)16 && Addr.S_un.S_un_b.s_b2 <= (unsigned char)31)
                    {
                        continue;
                    }
                }
                else if((unsigned char)192 == Addr.S_un.S_un_b.s_b1)
                {
                    if((unsigned char)168 == Addr.S_un.S_un_b.s_b2)
                    {
                        continue;
                    }
                }
            }

            // 잘못된 아이피
            // 169.X.X.X 자동 아이피 할당 ( SyGate )
            // 0.X.X.X
            if((unsigned char)169 == Addr.S_un.S_un_b.s_b1)
            {
                continue;
            }
            else if((unsigned char)0 == Addr.S_un.S_un_b.s_b1)
            {
                continue;
            }

            return true;
        }

        if(0 != Count)
            return true;
    }

    return false;
}


bool IsWrongIP(IN_ADDR &Addr)
{
    if((unsigned char)169 == Addr.S_un.S_un_b.s_b1)
    {
        if((unsigned char)254 == Addr.S_un.S_un_b.s_b2)
        {
            return true;
        }
    }

    if((unsigned char)0 == Addr.S_un.S_un_b.s_b1)
        return true;

    return false;
}



bool GetNATAddress(SOCKET Socket_In, SOCKADDR_IN *Address_Out, bool AllowVirtual)
{
	int AddressSize = sizeof(SOCKADDR_IN);
	if(getsockname(Socket_In, (struct sockaddr *)Address_Out, &AddressSize) != SOCKET_ERROR)
	{
		char HostName[256];
 		if(!gethostname(HostName, 255))
		{
			PHOSTENT pHost = gethostbyname(HostName);
			if(NULL == pHost)
			{
				return false;
			}

			IN_ADDR& Addr = Address_Out->sin_addr;
			int Count = 0;
			for(;pHost->h_addr_list[Count]; ++Count)
			{
				memcpy(&(Addr.S_un.S_addr), pHost->h_addr_list[Count], sizeof(IN_ADDR));

				if(!AllowVirtual)
				{
					// 가상 아이피 확인
					// 10.0.0.0		~ 10.255.255.255
					// 172.16.0.0	~ 172.31.255.255
					// 192.168.0.0	~ 192.168.255.255
					if((unsigned char)10 == Addr.S_un.S_un_b.s_b1)
					{
						continue;
					}
					else if((unsigned char)172 == Addr.S_un.S_un_b.s_b1)
					{
						if(Addr.S_un.S_un_b.s_b2 >= (unsigned char)16 && Addr.S_un.S_un_b.s_b2 <= (unsigned char)31)
						{
							continue;
						}
					}
					else if((unsigned char)192 == Addr.S_un.S_un_b.s_b1)
					{
						if((unsigned char)168 == Addr.S_un.S_un_b.s_b2)
						{
							continue;
						}
					}
				}

				// 잘못된 아이피
				// 169.X.X.X 자동 아이피 할당 ( SyGate )
				// 0.X.X.X
				if((unsigned char)169 == Addr.S_un.S_un_b.s_b1)
				{
					continue;
				}
				else if((unsigned char)0 == Addr.S_un.S_un_b.s_b1)
				{
					continue;
				}

				return true;
			}

			if(0 != Count)
				return true;
		}

		return false;
	}

	return false;
}


//by Hades Kang

void
ClientSocket::SetStatusFlag(unsigned long dwStatusFlag)
{
	m_dwStatusFlag = dwStatusFlag;
}
	
unsigned long 
ClientSocket::GetStatusFlag()
{
	return m_dwStatusFlag;
}



	
bool 
//해당 플래그가 체크되었는지 여부
ClientSocket::IsNSFlagChecked( unsigned char ucCmdFlag )
{
	if( s_pInstance )
		return s_pInstance->m_bStatusFlag[ ucCmdFlag ];
	return false;
}


bool 
//해당 플래그 체인이 체크되었는지 여부
ClientSocket::IsNSFlagChainChecked( unsigned char ucCmdFlag )
{
	if( s_pInstance )
	{
		std::vector<unsigned char>*	pCheckList;
		std::vector<unsigned char>::iterator iter;
		pCheckList = &(s_pInstance->m_SendCheckList[ucCmdFlag]);
		for( iter = pCheckList->begin() ; iter != pCheckList->end(); iter++ )
		{
			if( s_pInstance->m_bStatusFlag[ (*iter) ] )						//해당 플레그가 체크 되어 있다면...
				return true;
		}
	}
	return false;
}


void 
//해당 플래그를 활성화 시킴 - Send 시에 쓰임
ClientSocket::SetNSFlagOn( unsigned char ucCmdFlag )
{
	if( ucCmdFlag == CmdCharMove ||							//체크할 필요 없는 부분들.
		ucCmdFlag == CmdCharUpdateAddress ||
		ucCmdFlag == CmdSysPing ||
		ucCmdFlag == CmdCharAdminCmd )
		return ;


	if( s_pInstance )
		s_pInstance->m_bStatusFlag[ ucCmdFlag ] = true;
}

void 
//해당 플래그를 비활성화 시킴 - Receive 시에 쓰임
ClientSocket::SetNSFlagOff( unsigned char ucCmdFlag )
{
	if( s_pInstance )
	{
		std::vector<unsigned char>*	pCheckList;
		std::vector<unsigned char>::iterator iter;
		pCheckList = &(s_pInstance->m_ReceiveCheckList[ucCmdFlag]);
		for( iter = pCheckList->begin() ; iter != pCheckList->end(); iter++ )
		{
			s_pInstance->m_bStatusFlag[ (*iter) ] = false;
		}	
	}
}


void	
// Network Status 관련 Check List 초기화 ( Send, Receive )
ClientSocket::InitNSCheckList()
{
	InitSendCheckList();
	InitReceiveCheckList();
}

void	
ClientSocket::InitSendCheckList()
{
/*
	m_SendCheckList[ CmdAuthAccount				].push_back( CmdAuthAccount				) ;			// 계정 인증
															
	m_SendCheckList[ CmdServerGroup				].push_back( CmdServerGroup				) ;			// 서버군 선택
															
	m_SendCheckList[ CmdUserLogin				].push_back( CmdUserLogin				) ;			// 유저 로그인
	m_SendCheckList[ CmdUserKill				].push_back( CmdUserKill				) ;			// 유저 제거
															
	m_SendCheckList[ CmdCharSelect				].push_back( CmdCharSelect				) ;			// 캐릭터 선택
	m_SendCheckList[ CmdCharCreate				].push_back( CmdCharCreate				) ;			// 캐릭터 생성
	m_SendCheckList[ CmdCharDelete				].push_back( CmdCharDelete				) ;			// 캐릭터 삭제
															
	m_SendCheckList[ CmdCharLogin				].push_back( CmdCharLogin				) ;			// 캐릭터 로그인
	m_SendCheckList[ CmdCharSuicide				].push_back( CmdCharSuicide				) ;			// 캐릭터 자살
	m_SendCheckList[ CmdCharRespawn				].push_back( CmdCharRespawn				) ;			// 캐릭터 부활
	//m_SendCheckList[ CmdCharMove				].push_back( CmdCharMove				) ;			// 캐릭터 이동 (P2P)
	m_SendCheckList[ CmdCharMoveUpdate			].push_back( CmdCharMoveUpdate			) ;			// 캐릭터 이동 업데이트
	m_SendCheckList[ CmdCharChat				].push_back( CmdCharChat				) ;			// 캐릭터 채팅
	m_SendCheckList[ CmdCharAttack				].push_back( CmdCharAttack				) ;			// 캐릭터 공격
	m_SendCheckList[ CmdCharCastObject			].push_back( CmdCharCastObject			) ;			// 캐릭터 오브젝트 던지기
	m_SendCheckList[ CmdCharUseSkill			].push_back( CmdCharUseSkill			) ;			// 캐릭터 스킬 사용
*/
	m_SendCheckList[ CmdCharTakeItem			].push_back( CmdCharTakeItem			) ;			// 캐릭터 아이템 집기
	m_SendCheckList[ CmdCharTakeItem			].push_back( CmdCharSwapItem			) ;			// 캐릭터 아이템 스왑
	m_SendCheckList[ CmdCharTakeItem			].push_back( CmdCharRepairItem			) ;			// 캐릭터 아이템 수리
	m_SendCheckList[ CmdCharTakeItem			].push_back( CmdCharUseItem				) ;			// 캐릭터 아이템 사용
	m_SendCheckList[ CmdCharTakeItem			].push_back( CmdCharTradeItem			) ;			// 캐릭터 아이템 거래
	m_SendCheckList[ CmdCharTakeItem			].push_back( CmdCharItemOptionGraft		) ;			// 아이템 옵션 이식
	m_SendCheckList[ CmdCharTakeItem			].push_back( CmdCharItemCompensation	) ;			// 아이템 보상 판매
	m_SendCheckList[ CmdCharTakeItem			].push_back( CmdCharSplitItem			) ;			// 캐릭터 스플릿 아이템 (개수 있는 아이템을 두개로 쪼갤 때 쓰임)

	m_SendCheckList[ CmdCharSwapItem			].push_back( CmdCharSwapItem			) ;			// 캐릭터 아이템 스왑
	m_SendCheckList[ CmdCharSwapItem			].push_back( CmdCharTakeItem			) ;			// 캐릭터 아이템 집기
	m_SendCheckList[ CmdCharSwapItem			].push_back( CmdCharTradeItem			) ;			// 캐릭터 아이템 거래
	m_SendCheckList[ CmdCharSwapItem			].push_back( CmdCharRepairItem			) ;			// 캐릭터 아이템 수리
	m_SendCheckList[ CmdCharSwapItem			].push_back( CmdCharUseItem				) ;			// 캐릭터 아이템 사용
	m_SendCheckList[ CmdCharSwapItem			].push_back( CmdCharItemOptionGraft		) ;			// 아이템 옵션 이식
	m_SendCheckList[ CmdCharSwapItem			].push_back( CmdCharItemCompensation	) ;			// 아이템 보상 판매
	m_SendCheckList[ CmdCharSwapItem			].push_back( CmdCharSplitItem			) ;			// 캐릭터 스플릿 아이템 (개수 있는 아이템을 두개로 쪼갤 때 쓰임)

	m_SendCheckList[ CmdCharRepairItem			].push_back( CmdCharRepairItem			) ;			// 캐릭터 아이템 수리
	m_SendCheckList[ CmdCharRepairItem			].push_back( CmdCharTradeItem			) ;			// 캐릭터 아이템 거래
	m_SendCheckList[ CmdCharRepairItem			].push_back( CmdCharTakeItem			) ;			// 캐릭터 아이템 집기
	m_SendCheckList[ CmdCharRepairItem			].push_back( CmdCharSwapItem			) ;			// 캐릭터 아이템 스왑
	m_SendCheckList[ CmdCharRepairItem			].push_back( CmdCharUseItem				) ;			// 캐릭터 아이템 사용
	m_SendCheckList[ CmdCharRepairItem			].push_back( CmdCharItemOptionGraft		) ;			// 아이템 옵션 이식
	m_SendCheckList[ CmdCharRepairItem			].push_back( CmdCharItemCompensation	) ;			// 아이템 보상 판매
	m_SendCheckList[ CmdCharRepairItem			].push_back( CmdCharSplitItem			) ;			// 캐릭터 스플릿 아이템 (개수 있는 아이템을 두개로 쪼갤 때 쓰임)

	m_SendCheckList[ CmdCharUseItem				].push_back( CmdCharUseItem				) ;			// 캐릭터 아이템 사용
	m_SendCheckList[ CmdCharUseItem				].push_back( CmdCharTradeItem			) ;			// 캐릭터 아이템 거래
	m_SendCheckList[ CmdCharUseItem				].push_back( CmdCharTakeItem			) ;			// 캐릭터 아이템 집기
	m_SendCheckList[ CmdCharUseItem				].push_back( CmdCharSwapItem			) ;			// 캐릭터 아이템 스왑
	m_SendCheckList[ CmdCharUseItem				].push_back( CmdCharRepairItem			) ;			// 캐릭터 아이템 수리
	m_SendCheckList[ CmdCharUseItem				].push_back( CmdCharItemOptionGraft		) ;			// 아이템 옵션 이식
	m_SendCheckList[ CmdCharUseItem				].push_back( CmdCharItemCompensation	) ;			// 아이템 보상 판매
	m_SendCheckList[ CmdCharUseItem				].push_back( CmdCharSplitItem			) ;			// 캐릭터 스플릿 아이템 (개수 있는 아이템을 두개로 쪼갤 때 쓰임)


	m_SendCheckList[ CmdCharTradeItem			].push_back( CmdCharTradeItem			) ;			// 캐릭터 아이템 거래
	m_SendCheckList[ CmdCharTradeItem			].push_back( CmdCharTakeItem			) ;			// 캐릭터 아이템 집기
	m_SendCheckList[ CmdCharTradeItem			].push_back( CmdCharSwapItem			) ;			// 캐릭터 아이템 스왑
	m_SendCheckList[ CmdCharTradeItem			].push_back( CmdCharRepairItem			) ;			// 캐릭터 아이템 수리
	m_SendCheckList[ CmdCharTradeItem			].push_back( CmdCharUseItem				) ;			// 캐릭터 아이템 사용
	m_SendCheckList[ CmdCharTradeItem			].push_back( CmdCharItemOptionGraft		) ;			// 아이템 옵션 이식
	m_SendCheckList[ CmdCharTradeItem			].push_back( CmdCharItemCompensation	) ;			// 아이템 보상 판매
	m_SendCheckList[ CmdCharTradeItem			].push_back( CmdCharSplitItem			) ;			// 캐릭터 스플릿 아이템 (개수 있는 아이템을 두개로 쪼갤 때 쓰임)

	m_SendCheckList[ CmdCharStartQuest			].push_back( CmdCharStartQuest			) ;			// 퀘스트 시작

	m_SendCheckList[ CmdCharEquipShopInfo		].push_back( CmdCharEquipShopInfo		) ;			// NPC 장비 상점 정보

	m_SendCheckList[ CmdCharItemOptionGraft		].push_back( CmdCharItemOptionGraft		) ;			// 아이템 옵션 이식
	m_SendCheckList[ CmdCharItemOptionGraft		].push_back( CmdCharTradeItem			) ;			// 캐릭터 아이템 거래
	m_SendCheckList[ CmdCharItemOptionGraft		].push_back( CmdCharTakeItem			) ;			// 캐릭터 아이템 집기
	m_SendCheckList[ CmdCharItemOptionGraft		].push_back( CmdCharSwapItem			) ;			// 캐릭터 아이템 스왑
	m_SendCheckList[ CmdCharItemOptionGraft		].push_back( CmdCharRepairItem			) ;			// 캐릭터 아이템 수리
	m_SendCheckList[ CmdCharItemOptionGraft		].push_back( CmdCharUseItem				) ;			// 캐릭터 아이템 사용
	m_SendCheckList[ CmdCharItemOptionGraft		].push_back( CmdCharItemCompensation	) ;			// 아이템 보상 판매
	m_SendCheckList[ CmdCharItemOptionGraft		].push_back( CmdCharSplitItem			) ;			// 캐릭터 스플릿 아이템 (개수 있는 아이템을 두개로 쪼갤 때 쓰임)
	
	m_SendCheckList[ CmdCharItemCompensation	].push_back( CmdCharItemCompensation	) ;			// 아이템 보상 판매
	m_SendCheckList[ CmdCharItemCompensation	].push_back( CmdCharTradeItem			) ;			// 캐릭터 아이템 거래
	m_SendCheckList[ CmdCharItemCompensation	].push_back( CmdCharTakeItem			) ;			// 캐릭터 아이템 집기
	m_SendCheckList[ CmdCharItemCompensation	].push_back( CmdCharSwapItem			) ;			// 캐릭터 아이템 스왑
	m_SendCheckList[ CmdCharItemCompensation	].push_back( CmdCharRepairItem			) ;			// 캐릭터 아이템 수리
	m_SendCheckList[ CmdCharItemCompensation	].push_back( CmdCharUseItem				) ;			// 캐릭터 아이템 사용
	m_SendCheckList[ CmdCharItemCompensation	].push_back( CmdCharItemOptionGraft		) ;			// 아이템 옵션 이식
	m_SendCheckList[ CmdCharItemCompensation	].push_back( CmdCharSplitItem			) ;			// 캐릭터 스플릿 아이템 (개수 있는 아이템을 두개로 쪼갤 때 쓰임)

	m_SendCheckList[ CmdCharSplitItem			].push_back( CmdCharSplitItem			) ;			// 캐릭터 스플릿 아이템 (개수 있는 아이템을 두개로 쪼갤 때 쓰임)
	m_SendCheckList[ CmdCharSplitItem			].push_back( CmdCharTradeItem			) ;			// 캐릭터 아이템 거래
	m_SendCheckList[ CmdCharSplitItem			].push_back( CmdCharTakeItem			) ;			// 캐릭터 아이템 집기
	m_SendCheckList[ CmdCharSplitItem			].push_back( CmdCharSwapItem			) ;			// 캐릭터 아이템 스왑
	m_SendCheckList[ CmdCharSplitItem			].push_back( CmdCharRepairItem			) ;			// 캐릭터 아이템 수리
	m_SendCheckList[ CmdCharSplitItem			].push_back( CmdCharUseItem				) ;			// 캐릭터 아이템 사용
	m_SendCheckList[ CmdCharSplitItem			].push_back( CmdCharItemOptionGraft		) ;			// 아이템 옵션 이식
	m_SendCheckList[ CmdCharSplitItem			].push_back( CmdCharItemCompensation	) ;			// 아이템 보상 판매

	m_SendCheckList[ CmdCharPullDown			].push_back( CmdCharPullDown			) ;				
	m_SendCheckList[ CmdCharPullDown			].push_back( CmdCharPickUp				) ;				
	m_SendCheckList[ CmdCharPullDown			].push_back( CmdCharSplitItem			) ;			// 캐릭터 스플릿 아이템 (개수 있는 아이템을 두개로 쪼갤 때 쓰임)
	m_SendCheckList[ CmdCharPullDown			].push_back( CmdCharTradeItem			) ;			// 캐릭터 아이템 거래
	m_SendCheckList[ CmdCharPullDown			].push_back( CmdCharTakeItem			) ;			// 캐릭터 아이템 집기
	m_SendCheckList[ CmdCharPullDown			].push_back( CmdCharSwapItem			) ;			// 캐릭터 아이템 스왑
	m_SendCheckList[ CmdCharPullDown			].push_back( CmdCharRepairItem			) ;			// 캐릭터 아이템 수리
	m_SendCheckList[ CmdCharPullDown			].push_back( CmdCharUseItem				) ;			// 캐릭터 아이템 사용
	m_SendCheckList[ CmdCharPullDown			].push_back( CmdCharItemOptionGraft		) ;			// 아이템 옵션 이식
	m_SendCheckList[ CmdCharPullDown			].push_back( CmdCharItemCompensation	) ;			// 아이템 보상 판매
	
	
	m_SendCheckList[ CmdCharPickUp				].push_back( CmdCharPickUp				) ;				
	m_SendCheckList[ CmdCharPickUp				].push_back( CmdCharSplitItem			) ;			// 캐릭터 스플릿 아이템 (개수 있는 아이템을 두개로 쪼갤 때 쓰임)
	m_SendCheckList[ CmdCharPickUp				].push_back( CmdCharTradeItem			) ;			// 캐릭터 아이템 거래
	m_SendCheckList[ CmdCharPickUp				].push_back( CmdCharTakeItem			) ;			// 캐릭터 아이템 집기
	m_SendCheckList[ CmdCharPickUp				].push_back( CmdCharSwapItem			) ;			// 캐릭터 아이템 스왑
	m_SendCheckList[ CmdCharPickUp				].push_back( CmdCharRepairItem			) ;			// 캐릭터 아이템 수리
	m_SendCheckList[ CmdCharPickUp				].push_back( CmdCharUseItem				) ;			// 캐릭터 아이템 사용
	m_SendCheckList[ CmdCharPickUp				].push_back( CmdCharItemOptionGraft		) ;			// 아이템 옵션 이식
	m_SendCheckList[ CmdCharPickUp				].push_back( CmdCharItemCompensation	) ;			// 아이템 보상 판매

/*
	m_SendCheckList[ CmdCharSkillLock			].push_back( CmdCharSkillLock			) ;			// 캐릭터 스킬 락
	m_SendCheckList[ CmdCharSkillCreate			].push_back( CmdCharSkillCreate			) ;			// 캐릭터 스킬 생성
	m_SendCheckList[ CmdCharSkillErase			].push_back( CmdCharSkillErase			) ;			// 캐릭터 스킬 지우기
	m_SendCheckList[ CmdCharClassUpgrade		].push_back( CmdCharClassUpgrade		) ;			// 캐릭터 클래스 업그레이드
	m_SendCheckList[ CmdCharShapeInfo			].push_back( CmdCharShapeInfo			) ;			// 캐릭터 정보 (P2P)
	m_SendCheckList[ CmdCharIncreasePoint		].push_back( CmdCharIncreasePoint		) ;			// 캐릭터 능력 포인트 증가
	m_SendCheckList[ CmdCharBindPosition		].push_back( CmdCharBindPosition		) ;			// 캐릭터 바인드 포지션
	m_SendCheckList[ CmdCharRequireInfo			].push_back( CmdCharRequireInfo			) ;			// 캐릭터 해당 정보 요청
//	m_SendCheckList[ CmdCharUpdateAddress		].push_back( CmdCharUpdateAddress		) ;			// 캐릭터 UDP 주소 업데이트
	m_SendCheckList[ CmdCharPartyCmd			].push_back( CmdCharPartyCmd			) ;			// 캐릭터 파티 명령
	m_SendCheckList[ CmdCharPartyMemInfo		].push_back( CmdCharPartyMemInfo		) ;			// 캐릭터 파티 맴버 정보 전달 (P2P)
	m_SendCheckList[ CmdCharExchangeCmd			].push_back( CmdCharExchangeCmd			) ;			// 캐릭터 아이템 교환 명령
	m_SendCheckList[ CmdSysConnectAgent			].push_back( CmdSysConnectAgent			) ;			// 시스템 중계 접속 (UDP 중계)		# 사용 안함
	m_SendCheckList[ CmdSysPacketTransmit		].push_back( CmdSysPacketTransmit		) ;			// 시스템 패킷 전달 (UDP 중계)		# 사용 안함
	m_SendCheckList[ CmdCharLogout				].push_back( CmdCharLogout				) ;			// 캐릭터 로그 아웃
	m_SendCheckList[ CmdDBGetData				].push_back( CmdDBGetData				) ;			// DB 데이터 얻기
	m_SendCheckList[ CmdDBUpdateData			].push_back( CmdDBUpdateData			) ;			// 업데이트 DB 데이터
	m_SendCheckList[ CmdAgentParty				].push_back( CmdAgentParty				) ;			// DB 에이전트 파티
	m_SendCheckList[ CmdSysServerLogin			].push_back( CmdSysServerLogin			) ;			// 시스템 서버 로그인
															
	m_SendCheckList[ CmdServerZone				].push_back( CmdServerZone				) ;			// 서버존 선택
															
	m_SendCheckList[ CmdGameCellInfo			].push_back( CmdGameCellInfo			) ;			// 게임 셀 정보 (오브젝트 정보)	
	m_SendCheckList[ CmdCharInfo				].push_back( CmdCharInfo				) ;			// 캐릭터 정보						# 사용 안함
	m_SendCheckList[ CmdCharAddressInfo			].push_back( CmdCharAddressInfo			) ;			// 캐릭터 UDP 주소 정보
	m_SendCheckList[ CmdCharCellLogin			].push_back( CmdCharCellLogin			) ;			// 캐릭터 셀 로그인
	m_SendCheckList[ CmdCharCellLogout			].push_back( CmdCharCellLogout			) ;			// 캐릭터 셀 로그아웃
	m_SendCheckList[ CmdMonMove					].push_back( CmdMonMove					) ;			// 몬스터 이동
	m_SendCheckList[ CmdCharAttackInfo			].push_back( CmdCharAttackInfo			) ;			// 캐릭터 공격 정보 (P2P)
	m_SendCheckList[ CmdCharAttacked			].push_back( CmdCharAttacked			) ;			// 캐릭터 공격 얻음
	m_SendCheckList[ CmdCharAward				].push_back( CmdCharAward				) ;			// 캐릭터 어워드 
	m_SendCheckList[ CmdCharItemInfo			].push_back( CmdCharItemInfo			) ;			// 캐릭터 아이템 정보
	m_SendCheckList[ CmdCharPickUp				].push_back( CmdCharPickUp				) ;			// 캐릭터 집기
	m_SendCheckList[ CmdCharPullDown			].push_back( CmdCharPullDown			) ;			// 캐릭터 떨구기
	m_SendCheckList[ CmdCharPickUpInfo			].push_back( CmdCharPickUpInfo			) ;			// 캐릭터 집기 정보
	m_SendCheckList[ CmdCharPullDownInfo		].push_back( CmdCharPullDownInfo		) ;			// 캐릭터 떨구기 정보
	m_SendCheckList[ CmdCharCastObjectInfo		].push_back( CmdCharCastObjectInfo		) ;			// 캐릭터 던지기 오브젝트 정보
	m_SendCheckList[ CmdCharInstallSocket		].push_back( CmdCharInstallSocket		) ;			// 캐릭터 아이템 소켓 설치
	m_SendCheckList[ CmdCharLevelUp				].push_back( CmdCharLevelUp				) ;			// 캐릭터 레벨 업
	m_SendCheckList[ CmdCharPartyInfo			].push_back( CmdCharPartyInfo			) ;			// 캐릭터 파티 정보
	m_SendCheckList[ CmdCharUpgradeItem			].push_back( CmdCharUpgradeItem			) ;			// 캐릭터 아이템 업그레이드
	m_SendCheckList[ CmdCharHPRegen				].push_back( CmdCharHPRegen				) ;			// 캐릭터 HP 리젠
	m_SendCheckList[ CmdCharLevelUpInfo			].push_back( CmdCharLevelUpInfo			) ;			// 캐릭터 레벨업 정보
	m_SendCheckList[ CmdCharSplitItem			].push_back( CmdCharSplitItem			) ;			// 캐릭터 스플릿 아이템 (개수 있는 아이템을 두개로 쪼갤 때 쓰임)
	m_SendCheckList[ CmdUpdateUIDTable			].push_back( CmdUpdateUIDTable			) ;			// UID 테이블 업데이트
	m_SendCheckList[ CmdCharQuickSlotMove		].push_back( CmdCharQuickSlotMove		) ;			// 캐릭터 퀵슬롯 이동
	m_SendCheckList[ CmdCharSwitchEQ			].push_back( CmdCharSwitchHand			) ;			// 손 바꾸기
	m_SendCheckList[ CmdSysMngerRegistry		].push_back( CmdSysMngerRegistry		) ;			// 서버 관리자 등록
	m_SendCheckList[ CmdSysMngerRequest			].push_back( CmdSysMngerRequest			) ;			// 서버 관리자의 요청
	m_SendCheckList[ CmdSysMngerResponse		].push_back( CmdSysMngerResponse		) ;			// 서버 관리자로 응답
	m_SendCheckList[ CmdCharTakeItems			].push_back( CmdCharTakeItems			) ;			// 캐릭터 아이템 집기 (복수)
	m_SendCheckList[ CmdCharTakeGold			].push_back( CmdCharTakeGold			) ;			// 캐릭터 돈 집기
	m_SendCheckList[ CmdCharExchangeItem		].push_back( CmdCharExchangeItem		) ;			// 캐릭터 아이템 교환
	m_SendCheckList[ CmdCellBroadCasting		].push_back( CmdCellBroadCasting		) ;			// 셀 브로드 캐스팅
	m_SendCheckList[ CmdSysPatchAddress			].push_back( CmdSysPatchAddress			) ;			// 패치 주소 
	m_SendCheckList[ CmdCharPartyCmdInfo		].push_back( CmdCharPartyCmdInfo		) ;			// 파티 명령 정보
	m_SendCheckList[ CmdServerLog				].push_back( CmdServerLog				) ;			// 로그 정보 (???)
	m_SendCheckList[ CmdCharWhisper				].push_back( CmdCharWhisper				) ;			// 캐릭터 귓속말
	m_SendCheckList[ CmdSysServerVerUpdate		].push_back( CmdSysServerVerUpdate		) ;			// 서버 버젼 업데이트
	m_SendCheckList[ CmdSysMng					].push_back( CmdSysMng					) ;			// 서버 관리 서버, 클라이언트가 사용하는 패킷 (임시) (???)
	m_SendCheckList[ CmdSysChannelUpdate		].push_back( CmdSysChannelUpdate		) ;			// 서버 채널 업데이트
	m_SendCheckList[ CmdCharPartyFind			].push_back( CmdCharPartyFind			) ;			// 파티 찾기
	m_SendCheckList[ CmdCharPartyMemData		].push_back( CmdCharPartyMemData		) ;			// 파티 멤버 데이터
	m_SendCheckList[ CmdCharControlOption		].push_back( CmdCharControlOption		) ;			// 캐릭터 옵션 조정
	m_SendCheckList[ CmdCharDuelCmd				].push_back( CmdCharDuelCmd				) ;			// 캐릭터 듀얼 명령
	m_SendCheckList[ CmdCharFameInfo			].push_back( CmdCharFameInfo			) ;			// 캐릭터 명성 정보
															
	m_SendCheckList[ CmdLoginServerList			].push_back( CmdLoginServerList			) ;			// 서버 리스트 #!! 번호 수정 불가 !!#
															
	m_SendCheckList[ CmdCharSpellInfo			].push_back( CmdCharSpellInfo			) ;			// 캐릭터 챈트&인챈트 정보
	m_SendCheckList[ CmdCharSkillUnLock			].push_back( CmdCharSkillUnLock			) ;			// 캐릭터 스킬 락 해제
//	m_SendCheckList[ CmdSysPing					].push_back( CmdSysPing					) ;			// 서버 핑 패킷
	m_SendCheckList[ CmdCharMoveZone			].push_back( CmdCharMoveZone			) ;			// 존 이동
															
	m_SendCheckList[ CmdAgentZone				].push_back( CmdAgentZone				) ;			// 존 관리
															
	m_SendCheckList[ CmdDeposit					].push_back( CmdDeposit					) ; 		// 창고 처리 패킷 (Client <--> GameServer)
	m_SendCheckList[ CmdDepositUpdate			].push_back( CmdDepositUpdate			) ; 		// 창고 업데이트 패킷 (GameServer <--> DBAgent)
															
	m_SendCheckList[ CmdCharStallOpen			].push_back( CmdCharStallOpen			) ;			// 캐릭터 노점상 개설
	m_SendCheckList[ CmdCharStallRegisterItem	].push_back( CmdCharStallRegisterItem	) ;			// 캐릭터 노점상 아이템 등록
	m_SendCheckList[ CmdCharStallEnter			].push_back( CmdCharStallEnter			) ;			// 캐릭터 노점상 입장
	m_SendCheckList[ CmdCharStallItemInfo		].push_back( CmdCharStallItemInfo		) ;			// 캐릭터 노점상 아이템 정보
															
	//m_SendCheckList[ CmdCharAdminCmd			].push_back( CmdCharAdminCmd			) ;			// 캐릭터 어드민
															
	m_SendCheckList[ CmdCharTeamBattleInfo		].push_back( CmdCharTeamBattleInfo		) ;			// 팀배틀 정보
															
	m_SendCheckList[ CmdFriendAddRequest		].push_back( CmdFriendAddRequest		) ; 		// 친구 추가
	m_SendCheckList[ CmdFriendRemoveRequest		].push_back( CmdFriendRemoveRequest		) ; 		// 친구 삭제
	m_SendCheckList[ CmdFriendEtcRequest		].push_back( CmdFriendEtcRequest		) ; 		// 친구 기타
	m_SendCheckList[ CmdFriendAck				].push_back( CmdFriendAck				) ; 		// 친구 Ack
	m_SendCheckList[ CmdFriendDB				].push_back( CmdFriendDB				) ; 		// 친구 데이터 (및 친구 리스트 정보)
															
	m_SendCheckList[ CmdEliteBonus				].push_back( CmdEliteBonus				) ; 		// 엘리트 보너스

	m_SendCheckList[ CmdCharOperateTrigger		].push_back( CmdCharOperateTrigger		) ;			// 트리거 발동
	m_SendCheckList[ CmdQuestDB					].push_back( CmdQuestDB					) ;			// 퀘스트 정보 (DB에 세이브/로드, 캐릭터 로긴시)
	m_SendCheckList[ CmdCharEndQuest			].push_back( CmdCharEndQuest			) ;			// 퀘스트 종료
															
	m_SendCheckList[ CmdCharDisappearItem		].push_back( CmdCharDisappearItem		) ;			// 캐릭터 아이템 사라지기
															
	m_SendCheckList[ CmdCharAuthorizePanel		].push_back( CmdCharAuthorizePanel		) ;			// 캐릭터 인증 판넬
	m_SendCheckList[ CmdCharPeaceMode			].push_back( CmdCharPeaceMode			) ;			// 캐릭터 반전 모드
	m_SendCheckList[ CmdConfigInfoDB			].push_back( CmdConfigInfoDB			) ;			// 설정 정보
	m_SendCheckList[ CmdCharAutoRouting			].push_back( CmdCharAutoRouting			) ;			// 캐릭터 오토 루팅
	m_SendCheckList[ CmdRankingInfo				].push_back( CmdRankingInfo				) ;			// 랭킹 정보
	m_SendCheckList[ CmdCharStateRedistribution	].push_back( CmdCharStateRedistribution	) ;			// 스탯 재분배 (클래스 처음 상태로)
	m_SendCheckList[ CmdBillingTimeoutNotify	].push_back( CmdBillingTimeoutNotify	) ; 		// 빌링 타임아웃 Notify정보
															
	m_SendCheckList[ CmdAdminToolGetData		].push_back( CmdAdminToolGetData		) ; 		// 운영툴에서 DBAgent의 데이터 얻어 오기.
	m_SendCheckList[ CmdAdminToolSetData		].push_back( CmdAdminToolSetData		) ;			// 운영툴에서 DBAgent로 데이터 쓰기.
															
	m_SendCheckList[ CmdEventDropItem			].push_back( CmdEventDropItem			) ;			// 아이템 떨구기 이벤트
	m_SendCheckList[ CmdCharCancelQuest			].push_back( CmdCharCancelQuest			) ;			// 퀘스트 취소
	m_SendCheckList[ CmdBillingTimeCheckNotify	].push_back( CmdBillingTimeCheckNotify	) ;			// 빌링 타임 체크
	m_SendCheckList[ CmdCharLotteryResult		].push_back( CmdCharLotteryResult		) ;			// 복권 결과
	m_SendCheckList[ CmdCharSummonCmd			].push_back( CmdCharSummonCmd			) ;			// 소환수 명령
															
	m_SendCheckList[ CmdChatClientLogin			].push_back( CmdChatClientLogin			) ; 		// 클라이언트에서 채팅서버에 붙을 때 정보 줌
	m_SendCheckList[ CmdChatLogin				].push_back( CmdChatLogin				) ; 		// 중계서버가 채팅서버로 클라이언트 로그인 줌.
	m_SendCheckList[ CmdChatLogout				].push_back( CmdChatLogout				) ; 		// 중계서버가 채팅서버로 클라이언트 로그아웃 줌.
	m_SendCheckList[ CmdChatInfoChanged			].push_back( CmdChatInfoChanged			) ; 		// 중계서버가 채팅서버로 클라이언트 정보 변경 줌.
															
	m_SendCheckList[ CmdCharSummon				].push_back( CmdCharSummon				) ;			// 소환
															
	m_SendCheckList[ CmdJapanAuthAccount		].push_back( CmdJapanAuthAccount		) ; 		// 일본쪽 계정 인증 (ID, Password대신, 인증된 코드가 온다.)
															
	m_SendCheckList[ CmdCharBattleGroundRespawn	].push_back( CmdCharBattleGroundRespawn	) ;			// 리스폰 (배틀그라운드 대기중)
	m_SendCheckList[ CmdCharRespawnWaitQueue	].push_back( CmdCharRespawnWaitQueue	) ;			// 리스폰 큐 대기인 정보 (배틀그라운드용)
	m_SendCheckList[ CmdStatueInfo				].push_back( CmdStatueInfo				) ;			// 석상 정보
	m_SendCheckList[ CmdCameraScript			].push_back( CmdCameraScript			) ;			// 카메라 스크립트
	m_SendCheckList[ CmdCharEquipDurability		].push_back( CmdCharEquipDurability		) ;			// 캐릭터 장비 내구도
															
	m_SendCheckList[ CmdCreateGuild				].push_back( CmdCreateGuild				) ;			// 길드 생성
	m_SendCheckList[ CmdGuildCmd				].push_back( CmdGuildCmd				) ;			// 길드 멤버 관련 명령
	m_SendCheckList[ CmdGuildMark				].push_back( CmdGuildMark				) ;			// 길드 마크 변경
	m_SendCheckList[ CmdGuildLevel				].push_back( CmdGuildLevel				) ;			// 길드 레벨 변경
	m_SendCheckList[ CmdGuildRelation			].push_back( CmdGuildRelation			) ;			// 길드 관계 변경
	m_SendCheckList[ CmdGuildList				].push_back( CmdGuildList				) ;			// 길드 리스트
	m_SendCheckList[ CmdGuildDB					].push_back( CmdGuildDB					) ;			// 길드 데이터
	m_SendCheckList[ CmdGuildRight				].push_back( CmdGuildRight				) ;			// 길드 권한 설정
	m_SendCheckList[ CmdGuildMemberList			].push_back( CmdGuildMemberList			) ;			// 길드 멤버 리스트
	m_SendCheckList[ CmdMyGuildInfo				].push_back( CmdMyGuildInfo				) ;			// 자기 길드 정보
	m_SendCheckList[ CmdGuildSafe				].push_back( CmdGuildSafe				) ;			// 길드 금고
	m_SendCheckList[ CmdGuildMemberInfoUpdate	].push_back( CmdGuildMemberInfoUpdate	) ;			// 길드 멤버 정보 업데이트
															
	m_SendCheckList[ CmdCharStatusRetrain		].push_back( CmdCharStatusRetrain		) ;			// 스테이터스 재훈련 (일정량 다시 돌려받음)
	m_SendCheckList[ CmdSysServerLogout			].push_back( CmdSysServerLogout			) ;			// 서버 로그아웃

//	m_SendCheckList[ CmdCharPartyAddress		].push_back( CmdCharPartyAddress		) ;			// 파티원 UDP 주소 정보
//	m_SendCheckList[ CmdCharPartyMemAddress		].push_back( CmdCharPartyMemAddress		) ;			// 파티원 UDP 주소 정보 (P2P)

	m_SendCheckList[ CmdBGServerMapList			].push_back( CmdBGServerMapList			) ;			// 배틀 그라운드 서버 맵(방) 정보 리스트 요청
	m_SendCheckList[ CmdBGServerResultList		].push_back( CmdBGServerResultList		) ;			// 배틀 그라운드 서버 맵(방) 결과 리스트 요청
	m_SendCheckList[ CmdBGServerMoveZone		].push_back( CmdBGServerMoveZone		) ;			// 배틀 그라운드 서버 맵(방) 이동 (존 이동)
	m_SendCheckList[ CmdBGServerMileageChange	].push_back( CmdBGServerMileageChange	) ;			// 배틀 그라운드 서버 환전소 명령
	m_SendCheckList[ CmdBGServerCharSlot		].push_back( CmdBGServerCharSlot		) ;			// 배틀 그라운드 서버 정섭 캐릭터 정보 요청
															
	m_SendCheckList[ CmdHanBTNWarning			].push_back( CmdHanBTNWarning			) ; 		// 한게임 통합빌링 접속 끊기전 경고 메시지
	m_SendCheckList[ CmdHanBTNUserKill			].push_back( CmdHanBTNUserKill			) ; 		// 한게임 통합빌링 접속 끊기 메시지
															
	m_SendCheckList[ CmdCharRepairAllItem		].push_back( CmdCharRepairAllItem		) ;			// 장비 아이템 모두 수리
	m_SendCheckList[ CmdCSAuth					].push_back( CmdCSAuth					) ;			// 인증 코드 (게임 가드)
	m_SendCheckList[ CmdCharItemChemical		].push_back( CmdCharItemChemical		) ;			// 아이템 합성
	m_SendCheckList[ CmdItemQtyCheck			].push_back( CmdItemQtyCheck			) ;			// 아이템 수량 제어
	m_SendCheckList[ CmdGuildInclination		].push_back( CmdGuildInclination		) ;			// 길드 성향
	m_SendCheckList[ CmdGuildMemberFameUpdate	].push_back( CmdGuildMemberFameUpdate	) ;			// 길드 멤버 명성 업데이트
															
	m_SendCheckList[ CmdCastleInfo				].push_back( CmdCastleInfo				) ;			// 성 정보 보내기			(DBAgentServer -> GameServer)
	m_SendCheckList[ CmdCampInfo				].push_back( CmdCampInfo				) ;			// 길드 요새 정보 보내기	(DBAgentServer -> GameServer)
	m_SendCheckList[ CmdCreateCastle			].push_back( CmdCreateCastle			) ;			// 성 생성					(GameServer -> Client)
	m_SendCheckList[ CmdCreateCamp				].push_back( CmdCreateCamp				) ;			// 길드 요새 생성			(Client <- GameServer -> DBAgentServer)
	m_SendCheckList[ CmdCreateSiegeArms			].push_back( CmdCreateSiegeArms			) ;			// 공성 병기 생성			(Client <- GameServer -> DBAgentServer)
	m_SendCheckList[ CmdCastleCmd				].push_back( CmdCastleCmd				) ;			// 성 관련 명령 패킷		(Client <-> GameServer <-> DBAgentServer)
	m_SendCheckList[ CmdCampCmd					].push_back( CmdCampCmd					) ;			// 길드 요새 관련 명령 패킷	(Client <-> GameServer <-> DBAgentServer)
	m_SendCheckList[ CmdSiegeArmsCmd			].push_back( CmdSiegeArmsCmd			) ;			// 공성 병기 관련 명령 패킷	(Client <-> GameServer <-> DBAgentServer)
	m_SendCheckList[ CmdCastleRight				].push_back( CmdCastleRight				) ;			// 성 관리 권한 패킷		(Client <-> GameServer <-> DBAgentServer)
	m_SendCheckList[ CmdCampRight				].push_back( CmdCampRight				) ;			// 길드 요새 관리 권한 패킷
	m_SendCheckList[ CmdSiegeBroadCast			].push_back( CmdSiegeBroadCast			) ;			// 공성 오브젝트 브로드 캐스트
	m_SendCheckList[ CmdSiegeTimeInfo			].push_back( CmdSiegeTimeInfo			) ;			// 공성 시간 정보
	m_SendCheckList[ CmdStealthInfo				].push_back( CmdStealthInfo				) ;			// 스텔스 사용가능 정보
															
	m_SendCheckList[ CmdCellBroadCast2nd		].push_back( CmdCellBroadCast2nd		) ;			// 셀 브로드캐스트 수정된 패킷
															
	m_SendCheckList[ CmdCharRespawnInfo			].push_back( CmdCharRespawnInfo			) ;			// 리스폰 정보 (리스폰 지역 리스트 정보)
	m_SendCheckList[ CmdCharRespawnAreaInfo		].push_back( CmdCharRespawnAreaInfo		) ;			// 선택한 리스폰 지역의 세부 정보
															
	m_SendCheckList[ CmdCharEquipShopInfo		].push_back( CmdCharEquipShopInfo		) ;			// NPC 장비 상점 정보
	m_SendCheckList[ CmdSiegeBroadCast2nd		].push_back( CmdSiegeBroadCast2nd		) ;			// 공성 오브젝트 브로드 캐스트 다른 버젼
	m_SendCheckList[ CmdCharItemOptionGraft		].push_back( CmdCharItemOptionGraft		) ;			// 아이템 옵션 이식
	m_SendCheckList[ CmdCharItemCompensation	].push_back( CmdCharItemCompensation	) ;			// 아이템 보상 판매
*/															
}

void	
ClientSocket::InitReceiveCheckList()
{
	for (int nIndex = 0; nIndex < CmdFinalPacketNum; ++nIndex)
	{
		m_ReceiveCheckList[nIndex].push_back(nIndex);
	}
}

int	
ClientSocket::CheckFlag( int* arrFlag )			//디버깅용
{
	int res = 0;
	for( int i = 0; i < CmdFinalPacketNum ; i++ )
	{
		if( s_pInstance->m_bStatusFlag[i] )
			arrFlag[res++] = i;
	}
	return res;
}
#ifndef _GAMA_CLIENT_EVENT_HANDLER_H_
#define _GAMA_CLIENT_EVENT_HANDLER_H_

namespace ClientNet
{
    // forward decl.
    class CSession;
    class CNetworkMsgBlock;
    
    class CEventHandler
    {
    public:

        CEventHandler(DWORD dwTimeoutSec = 1) 
			:	m_dwTimeoutSec(dwTimeoutSec), m_dwLastCalled(0) 
		{ 
		
		}

        virtual ~CEventHandler() { }
        
        virtual int OnReceived(CNetworkMsgBlock& msg_block) = 0;        // 패킷을 받았을 때 불린다.

        virtual int OnOpen(CSession& ConnSession, int iErrorCode) = 0;  // 연결이 완료되었을 때 불린다. (성공/실패. UDP는 해당 없음)
        virtual int OnClose(CSession& CloseSession) = 0;                // 연결이 끝났을 때 불린다.  (UDP는 해당 없음)
        virtual int OnDispatch(CSession& ReceivedSession) = 0;          // 메시지를 처리할 때 불린다.

        virtual int OnTimeout(CSession& Session) = 0;                   // 지정 시간이 지나면 호출된다. 기본은 매초마다 불린다.

        bool IsTimeout(DWORD dwCurrentTime) const 
		{
			return (0 == m_dwLastCalled) || 
				(m_dwLastCalled + (m_dwTimeoutSec * 1000) < dwCurrentTime);
		}

        void UpdateTimeoutCalled(DWORD dwCurrentTime) 
		{
			m_dwLastCalled = dwCurrentTime; 		
			if (0 == m_dwLastCalled) { ++m_dwLastCalled; }
		}

    protected:

        DWORD   m_dwTimeoutSec;
        DWORD   m_dwLastCalled;
    };
};

#endif
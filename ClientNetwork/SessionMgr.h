#ifndef _GAMA_CLIENT_SESSION_MGR_H_
#define _GAMA_CLIENT_SESSION_MGR_H_

#include "NetworkMsgBlock.h"
#include <map>

#pragma comment(lib, "ws2_32")

namespace ClientNet
{
    class CSession;

    class CSessionMgr : public CUnboundedWaitableMsgQueue
    {
    public:

        CSessionMgr();
        ~CSessionMgr();

        void SetDispatchQueue(CLockedMsgQueue& LockedMsgQueue) { m_lpLockedMsgQueue = &LockedMsgQueue; }
        CLockedMsgQueue* GetDispatchQueue() { return m_lpLockedMsgQueue; }

        int HandleEvents(DWORD dwMilliseconds = INFINITE);

    private:

        void RearrangeHandleSet();

        // 해당 세션에 문제가 있으면 -1, 없으면 0을 리턴한다.
        int Connect(INET_Addr& address, NetMsgType eNetMsgType, DWORD dwHandlerID);
        int HandleIO(CSession* lpSession);

        // 메시지 큐의 패킷을 처리한다. HandleSet에 변화가 있으면 -1을 리턴한다.
        int HandleMsgQueue();

        typedef std::map<DWORD, CSession*>  SessionMap;

        DWORD               m_dwMaxSessionID;
        DWORD               m_dwLastTimeoutCall;
        CLockedMsgQueue*    m_lpLockedMsgQueue;

        SessionMap          m_SessionMap;
        HANDLE              m_hHandleSet[MAXIMUM_WAIT_OBJECTS];
    };
}


#endif
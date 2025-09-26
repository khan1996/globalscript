#ifndef _GAMA_CLIENT_EVENT_HANDLER_DERIVED_H_
#define _GAMA_CLIENT_EVENT_HANDLER_DERIVED_H_

#include <Thread/Lock.h>
#include <Thread/Thread.h>
#include <Network/ClientNetwork/EventHandler.h>
#include <Network/ClientNetwork/NetworkMsgBlock.h>
#include <Network/ClientNetwork/SessionMgr.h>


#include <map>

// forward decl.
struct PktBase;
class  CXORCrypt;

namespace ClientNet
{
    class CSessionMgr;
    class CClientEventHandlerMgr;

    // 클라이언트측 이벤트 핸들러
    class CClientEventHandler
    {
    public:

        CClientEventHandler();
        virtual ~CClientEventHandler();

        virtual int OnDispatch(PktBase* lpPktBase, INET_Addr& peerAddr) = 0;
        virtual int OnOpen(int iErrorCode) = 0;
        virtual int OnClose() = 0;

        DWORD GetHandlerID() const { return m_dwHandlerID; }
        DWORD GetSessionID() const { return m_dwSessionID; }

        INET_Addr& GetPeerAddress() { return m_peerAddress; }

        bool IsCloseCalled() { return m_bCloseCalled; }
        bool SendPacket(CNetworkMsgBlock* lpMsgBlock);

    private:

        void SetHandlerID(DWORD dwHandlerID) { m_dwHandlerID = dwHandlerID; }
        void SetSessionID(DWORD dwSessionID) { m_dwSessionID = dwSessionID; }
        void SetCloseCalled() { m_bCloseCalled = true; }
        void SetPeerAddress(INET_Addr& peerAddress) { m_peerAddress = peerAddress; }
        void SetEventHandlerMgr(CClientEventHandlerMgr* lpEventHandlerMgr) { m_lpEventHandlerMgr = lpEventHandlerMgr; }

        CClientEventHandlerMgr* m_lpEventHandlerMgr;

        DWORD       m_dwHandlerID;
        DWORD       m_dwSessionID;

        INET_Addr   m_peerAddress;

        bool        m_bCloseCalled;
        
        // 세션 ID세팅을 위해서 필요함.
        friend class CClientEventHandlerMgr;
    };

    // 클라이언트 핸들러 관리자.
    class CClientEventHandlerMgr : public CThread
    {
    public:

        CClientEventHandlerMgr();
        virtual ~CClientEventHandlerMgr();

        bool Connect(INET_Addr& connAddr, CClientEventHandler* lpEventHandler);
        bool Close(CClientEventHandler* lpEventHandler);
        bool SendPacket(CClientEventHandler* lpEventHandler, CNetworkMsgBlock* lpMsgBlock);
        
        void HandleEvents(unsigned long dwProcessPerCall);
        
    private:

        virtual unsigned int Run();		// 실제 실행 되는 루프를 넣는다.
	    virtual BOOL End();				// 루프가 끝날 수 있는 루틴을 넣는다.

        typedef std::map<DWORD, CClientEventHandler*>   ClientEventHanderMap;

        CLockedMsgQueue         m_LockedMsgQueue;
        CNetworkMsgQueue        m_LocalMsgQueue;
        ClientEventHanderMap    m_ClientEventHandlerMap;

        DWORD                   m_dwMaxEventHandlerID;
        LONG                    m_bExit;
        
        // 스레드 내부에서 사용.
        CSessionMgr             m_SessionMgr;
    };

    // 스레드 내부 이벤트 핸들러
    class CThreadEventHandler : public CEventHandler
    {
    public:

        CThreadEventHandler(CLockedMsgQueue& LockedMsgQueue, DWORD dwClientHandlerID);
        
        virtual int OnReceived(CNetworkMsgBlock& msg_block);
        virtual int OnDispatch(CSession& ReceivedSession) { return 0; }
        
        virtual int OnOpen(CSession& ConnSession, int iErrorCode);
        virtual int OnClose(CSession& CloseSession);
        virtual int OnTimeout(CSession& Session);

    private:

        CLockedMsgQueue&    m_LockedMsgQueue;
        CXORCrypt&          m_Crypt;
        DWORD               m_dwClientHandlerID;
    };
};

#endif
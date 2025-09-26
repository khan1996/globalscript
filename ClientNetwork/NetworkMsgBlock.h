#ifndef _GAMA_CLIENT_NETWORK_BLOCK_MSG_
#define _GAMA_CLIENT_NETWORK_BLOCK_MSG_

#include <Thread/Lock.h>
#include <Network/Address/INET_Addr.h>
#include "MessageBlock.h"

#include <list>

namespace ClientNet
{
    enum NetMsgType
    {
        NET_MSG_FREE,
        NET_MSG_ACK_OPEN,
        NET_MSG_ACK_CLOSE,
        NET_MSG_ACK_DISPATCH,
        
        NET_MSG_REQ_TCP_OPEN,
        NET_MSG_REQ_UDP_OPEN,
        NET_MSG_REQ_CLOSE,
        NET_MSG_REQ_SEND
    };

    // Network Message Block
    class CNetworkMsgBlock : public CMessageBlock
    {
    public:
        
        void SetMsgType(NetMsgType eMsgType) { m_eMsgType = eMsgType; }
        NetMsgType GetMsgType() const { return m_eMsgType; }

        void SetID(DWORD dwID) { m_dwID = dwID; }
        DWORD GetID() const { return m_dwID; }

        INET_Addr& GetAddress() { return m_dstaddr; }
        void SetAddress(INET_Addr& addr) { m_dstaddr = addr; }

        bool WrapHeader(unsigned short usUsed_In, unsigned char cCMD_In, unsigned short usState_In, unsigned short usError_In);
        bool WrapCryptOld(unsigned short usUsed_In, unsigned char cCMD_In, unsigned short usState_In, unsigned short usError_In);
        bool WrapCryptOld(unsigned short usUsed_In, unsigned char cCMD_In, unsigned long Tick_In);
        bool WrapCrypt(unsigned short usUsed_In, unsigned char cCMD_In, unsigned short usState_In, unsigned short usError_In);
        bool WrapCrypt(unsigned short usUsed_In, unsigned char cCMD_In, unsigned long Tick_In);
        bool WrapCompress(char* lpSourceData, unsigned short usSourceLength, unsigned char cCMD_In, unsigned short usState_In, unsigned short usError_In);

        static unsigned long GetMinCompressSize(unsigned short usSourceLength);
        CNetworkMsgBlock* next() { return m_next; }

    private:

        void next(CNetworkMsgBlock* next) { m_next = next; }

        explicit CNetworkMsgBlock(size_t n);
        explicit CNetworkMsgBlock(size_t n, INET_Addr& addr);
		virtual ~CNetworkMsgBlock();

        INET_Addr           m_dstaddr;
        NetMsgType          m_eMsgType;
        DWORD               m_dwID;
        CNetworkMsgBlock*   m_next;
        
        friend class CSession;
        friend class CNetworkMsgQueue;
        friend class CNetworkMsgPool;
    };

    // Network Message Queue
    class CNetworkMsgQueue
    {
    public:

        CNetworkMsgQueue();
        ~CNetworkMsgQueue();

        void clear();

        CNetworkMsgBlock* GetHead() { return m_lpListHead; }
        CNetworkMsgBlock* GetTail() { return m_lpListTail; }
        long GetQueuedNum() { return m_nListNum; }

        void Enqueue(CNetworkMsgBlock* lpMsgBlock, bool bPendHead = false);
        CNetworkMsgBlock* Dequeue();

    private:

        CNetworkMsgBlock*   m_lpListHead;
        CNetworkMsgBlock*   m_lpListTail;
        long                m_nListNum;
    };

    class CNetworkMsgPool
    {
    public:

        static CNetworkMsgPool& GetInstance();

        CNetworkMsgBlock* GetNetworkMsgBlock(unsigned long dwSize, INET_Addr& addr);
        void FreeNetworkMsgBlock(CNetworkMsgBlock* lpMsgBlock);
        void FreeNetworkMsgBlock(CNetworkMsgQueue& msgQueue);

    private:

        CNetworkMsgPool();
        ~CNetworkMsgPool();

        typedef CCSLock                      MsgBlockLock;
        typedef std::list<CNetworkMsgBlock*> MsgBlockList;

        MsgBlockLock    m_MsgBlockLock;
        MsgBlockList    m_MsgBlockList;

        DWORD           m_dwMaxBlockNum;
    };

    class CLockedMsgQueue
    {
    public:

        typedef CCSLock MsgQueueLock;

        virtual ~CLockedMsgQueue();

        CNetworkMsgBlock* GetMessage();
        void GetMessages(CNetworkMsgQueue& msgQueue, DWORD dwItemNum);

        void PutMessage(CNetworkMsgBlock* lpMsg, bool bPendHead = false);
        void PutMessages(CNetworkMsgQueue& msgQueue);

    private:
        
        MsgQueueLock     m_MsgQueueLock;
        CNetworkMsgQueue m_MessageQueue;
    };

    class CUnboundedWaitableMsgQueue
    {
    public:

        typedef CCSLock MsgQueueLock;

        CUnboundedWaitableMsgQueue();
        virtual ~CUnboundedWaitableMsgQueue();

        const HANDLE GetQueueHandle() const { return m_hNotEmpty; }
        DWORD WaitForNotEmptyQueue(DWORD dwMilliseconds = INFINITE);

        CNetworkMsgBlock* GetMessage();
        BOOL GetMessages(CNetworkMsgQueue& msgQueue, DWORD dwItemNum);
       
        void PutMessage(CNetworkMsgBlock* lpMsg, bool bPendHead = false);
        void PutMessages(CNetworkMsgQueue& msgQueue);

    private:

        void InternalSetNotEmpty();
        void InternalResetNotEmpty();
        
        HANDLE  m_hNotEmpty;    // Auto-reset Event
        bool    m_bNotEmpty;

        MsgQueueLock     m_MsgQueueLock;
        CNetworkMsgQueue m_MessageQueue;
    };

}

#endif
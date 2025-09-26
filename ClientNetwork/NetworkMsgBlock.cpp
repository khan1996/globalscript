#include "stdafx.h"
#include "NetworkMsgBlock.h"
#include <Network/Packet/PacketBase.h>
#include <Network/XORCrypt/XORCrypt.h>
#include <Utility/Compress/MiniLZO/miniLZOWrapper.h>

#include <cstdio>
#include "GMMemory.h"


namespace ClientNet
{

CNetworkMsgBlock::CNetworkMsgBlock(size_t n) 
:   CMessageBlock(n), m_next(0), 
    m_eMsgType(NET_MSG_FREE), m_dwID(0)
{

}

CNetworkMsgBlock::CNetworkMsgBlock(size_t n, INET_Addr& addr) 
:   CMessageBlock(n), m_dstaddr(addr), m_next(0), 
    m_eMsgType(NET_MSG_FREE), m_dwID(0)
{


}

CNetworkMsgBlock::~CNetworkMsgBlock()
{


}

bool CNetworkMsgBlock::WrapHeader(unsigned short usUsed_In, unsigned char cCMD_In, 
                                             unsigned short usState_In, unsigned short usError_In)
{    
    char* lpBuffer = rd_ptr();

    if(NULL != lpBuffer && sizeof(PktBase) <= usUsed_In && usUsed_In <= remain())
    {
        reinterpret_cast<PktBase*>(lpBuffer)->InitPtHead(usUsed_In, cCMD_In, usState_In, usError_In);
        CXORCrypt::GetInstance().EncodeHeader(lpBuffer + 1, sizeof(PktBase) - 1, 0, 0);

        wr_ptr(usUsed_In);
        return true;
    }

    fprintf(stderr, "CNetworkMsgBlock::WrapHeader - Packet wrapping error. CMD:0x%02x, usUsed:%d, capacity:%d\n",
        cCMD_In, usUsed_In, capacity());
    return false;
}


bool CNetworkMsgBlock::WrapCryptOld(unsigned short usUsed_In, unsigned char cCMD_In, 
                                            unsigned long Tick_In)
{
    char* lpBuffer = rd_ptr();

    if(NULL != lpBuffer && sizeof(PktBase) <= usUsed_In && usUsed_In <= remain())
    {
        CXORCrypt& Crypt = CXORCrypt::GetInstance();
        DWORD dwCodePage = Crypt.GetCodePage(1);

        if(Crypt.EncodePacket(lpBuffer + sizeof(PktBase), usUsed_In - sizeof(PktBase), dwCodePage))
        {
            PktBase* lpPktBase = reinterpret_cast<PktBase*>(lpBuffer);

            lpPktBase->InitPtHead(usUsed_In, cCMD_In, Tick_In);
            lpPktBase->SetCodePage(dwCodePage);
            lpPktBase->SetCrypt();

            // 헤더 인코딩
            Crypt.EncodeHeader(lpBuffer + 1, sizeof(PktBase) - 1, 0, 0);

            wr_ptr(usUsed_In);
            return true;
        }
    }

    fprintf(stderr, "CNetworkMsgBlock::WrapCrypt - Packet wrapping error. CMD:0x%02x, usUsed:%d, capacity:%d\n",
        cCMD_In, usUsed_In, capacity());
    return false;
}


bool CNetworkMsgBlock::WrapCryptOld(unsigned short usUsed_In, unsigned char cCMD_In, 
                                            unsigned short usState_In, unsigned short usError_In)
{   
    char* lpBuffer = rd_ptr();

    if(NULL != lpBuffer && sizeof(PktBase) <= usUsed_In && usUsed_In <= remain())
    {
        CXORCrypt& Crypt = CXORCrypt::GetInstance();
        DWORD dwCodePage = Crypt.GetCodePage(1);

        if(Crypt.EncodePacket(lpBuffer + sizeof(PktBase), usUsed_In - sizeof(PktBase), dwCodePage))
        {
            PktBase* lpPktBase = reinterpret_cast<PktBase*>(lpBuffer);

            lpPktBase->InitPtHead(usUsed_In, cCMD_In, usState_In, usError_In);
            lpPktBase->SetCodePage(dwCodePage);
            lpPktBase->SetCrypt();

            // 헤더 인코딩
            Crypt.EncodeHeader(lpBuffer + 1, sizeof(PktBase) - 1, 0, 0);

            wr_ptr(usUsed_In);
            return true;
        }
    }

    fprintf(stderr, "CNetworkMsgBlock::WrapCrypt - Packet wrapping error. CMD:0x%02x, usUsed:%d, capacity:%d\n",
        cCMD_In, usUsed_In, capacity());
    return false;
}

bool CNetworkMsgBlock::WrapCrypt(unsigned short usUsed_In, unsigned char cCMD_In, 
                                            unsigned long Tick_In)
{
    char* lpBuffer = rd_ptr();

    if(NULL != lpBuffer && sizeof(PktBase) <= usUsed_In && usUsed_In <= remain())
    {
        CXORCrypt& Crypt = CXORCrypt::GetInstance();
        DWORD dwCodePage = Crypt.GetCodePage();

        if(Crypt.EncodePacket(lpBuffer + sizeof(PktBase), usUsed_In - sizeof(PktBase), dwCodePage))
        {
            PktBase* lpPktBase = reinterpret_cast<PktBase*>(lpBuffer);

            lpPktBase->InitPtHead(usUsed_In, cCMD_In, Tick_In);
            lpPktBase->SetCodePage(dwCodePage);
            lpPktBase->SetCrypt();

            // 헤더 인코딩
            Crypt.EncodeHeader(lpBuffer + 1, sizeof(PktBase) - 1, 0, 0);

            wr_ptr(usUsed_In);
            return true;
        }
    }

    fprintf(stderr, "CNetworkMsgBlock::WrapCrypt - Packet wrapping error. CMD:0x%02x, usUsed:%d, capacity:%d\n",
        cCMD_In, usUsed_In, capacity());
    return false;
}


bool CNetworkMsgBlock::WrapCrypt(unsigned short usUsed_In, unsigned char cCMD_In, 
                                            unsigned short usState_In, unsigned short usError_In)
{   
    char* lpBuffer = rd_ptr();

    if(NULL != lpBuffer && sizeof(PktBase) <= usUsed_In && usUsed_In <= remain())
    {
        CXORCrypt& Crypt = CXORCrypt::GetInstance();
        DWORD dwCodePage = Crypt.GetCodePage();

        if(Crypt.EncodePacket(lpBuffer + sizeof(PktBase), usUsed_In - sizeof(PktBase), dwCodePage))
        {
            PktBase* lpPktBase = reinterpret_cast<PktBase*>(lpBuffer);

            lpPktBase->InitPtHead(usUsed_In, cCMD_In, usState_In, usError_In);
            lpPktBase->SetCodePage(dwCodePage);
            lpPktBase->SetCrypt();

            // 헤더 인코딩
            Crypt.EncodeHeader(lpBuffer + 1, sizeof(PktBase) - 1, 0, 0);

            wr_ptr(usUsed_In);
            return true;
        }
    }

    fprintf(stderr, "CNetworkMsgBlock::WrapCrypt - Packet wrapping error. CMD:0x%02x, usUsed:%d, capacity:%d\n",
        cCMD_In, usUsed_In, capacity());
    return false;
}

unsigned long CNetworkMsgBlock::GetMinCompressSize(unsigned short usSourceLength)
{
    return usSourceLength + (usSourceLength / 64) + 16 + 3;
}

bool CNetworkMsgBlock::WrapCompress(char* lpSourceData, unsigned short usSourceLength, 
                                               unsigned char cCMD_In, unsigned short usState_In, 
                                               unsigned short usError_In)
{
    unsigned long dwDstBufferSize = static_cast<unsigned long>(remain());
    
    if(usSourceLength < sizeof(PktBase) || PktMaxLen < usSourceLength)
    {        
        fprintf(stderr, "CNetworkMsgBlock::WrapCompress - Packet wrapping error. CMD:0x%02x, usSourceLength:%d, dwDstBufferSize:%d\n",
            cCMD_In, usSourceLength, dwDstBufferSize);
    }
    else
    {
        if(dwDstBufferSize < sizeof(PktBase) || dwDstBufferSize < GetMinCompressSize(usSourceLength))
        {
            fprintf(stderr, "CNetworkMsgBlock::WrapCompress - Too small buffer size. CMD:0x%02x, usSourceLength:%d, dwDstBufferSize:%d\n",
                cCMD_In, usSourceLength, dwDstBufferSize);
        }
        else
        {   
            char*           lpDstBuffer      = rd_ptr();
            unsigned long   dwCompressedSize = dwDstBufferSize - sizeof(PktBase);
            
            if(!CMiniLZO::Compress(lpSourceData + sizeof(PktBase), usSourceLength - sizeof(PktBase),
                lpDstBuffer + sizeof(PktBase), &dwCompressedSize))
            {
                fprintf(stderr, "CNetworkMsgBlock::WrapCompress - Packet Compress failed. CMD:0x%02x, usSourceLength:%d, "
                    "dwDstBufferSize:%d\n", cCMD_In, usSourceLength, dwDstBufferSize);
            }
            else
            {
                if(dwDstBufferSize <= dwCompressedSize)
                {
                    fprintf(stderr, "CNetworkMsgBlock::WrapCompress - Compressed size is over than buffer size."
                        "CMD:0x%02x, dwDstBufferSize:%d, dwCompressedSize:%d\n", cCMD_In, dwDstBufferSize, dwCompressedSize);
                }

                dwCompressedSize += sizeof(PktBase);

                PktBase* lpPktDst = reinterpret_cast<PktBase*>(lpDstBuffer);
                lpPktDst->InitPtHead(static_cast<PktBase::LengthType>(dwCompressedSize), cCMD_In, usState_In, usError_In);
                lpPktDst->SetCompress();

                // 인코딩 헤더
                CXORCrypt::GetInstance().EncodeHeader(lpDstBuffer + 1, sizeof(PktBase) - 1, 0, 0);

				wr_ptr(dwCompressedSize);
                return true;
            }
        }
    }

    return false;
}








CNetworkMsgQueue::CNetworkMsgQueue()
:   m_lpListHead(NULL), m_lpListTail(NULL), m_nListNum(0)
{

}

CNetworkMsgQueue::~CNetworkMsgQueue()
{
	clear();
}

void CNetworkMsgQueue::clear()
{
    CNetworkMsgPool::GetInstance().FreeNetworkMsgBlock(*this);
}

void CNetworkMsgQueue::Enqueue(CNetworkMsgBlock* lpMsgBlock, bool bPendHead)
{
    if(NULL != lpMsgBlock)
    {
        if(m_lpListHead == NULL)
        {
            m_lpListHead = m_lpListTail = lpMsgBlock;
            lpMsgBlock->next(NULL);
        }
        else if(!bPendHead)
        {
            m_lpListTail->next(lpMsgBlock);
            m_lpListTail = lpMsgBlock;
            lpMsgBlock->next(NULL);
        }
        else
        {
            lpMsgBlock->next(m_lpListHead);
            m_lpListHead = lpMsgBlock;
        }

        ++m_nListNum;
    }
}

CNetworkMsgBlock* CNetworkMsgQueue::Dequeue()
{
    CNetworkMsgBlock* lpMsgBlock = NULL;
    
    if(m_lpListTail != NULL)
    {
        lpMsgBlock      = m_lpListHead;
        m_lpListHead    = m_lpListHead->next();

        if(m_lpListTail == lpMsgBlock)
        {
            m_lpListTail = NULL;
        }

		lpMsgBlock->next(NULL);
        --m_nListNum;
    }

    return lpMsgBlock;
}


CNetworkMsgBlock* CNetworkMsgPool::GetNetworkMsgBlock(unsigned long dwSize, INET_Addr& addr)
{
    MsgBlockLock::Syncronize sync(m_MsgBlockLock);

    CNetworkMsgBlock* lpMsgBlock = 0;

    if (m_MsgBlockList.empty())
    {
        lpMsgBlock = new CNetworkMsgBlock(dwSize);
    }
    else
    {
        lpMsgBlock = m_MsgBlockList.front();
        m_MsgBlockList.pop_front();

        if (0 != lpMsgBlock)
        {
            lpMsgBlock->init(dwSize);
        }
    }

    if (0 != lpMsgBlock && lpMsgBlock->is_valid() 
        && dwSize <= lpMsgBlock->capacity())
    {
        return lpMsgBlock;
    }

    delete lpMsgBlock;
    return 0;
}

void CNetworkMsgPool::FreeNetworkMsgBlock(CNetworkMsgBlock* lpMsgBlock)
{
    MsgBlockLock::Syncronize sync(m_MsgBlockLock);

	if (0 != lpMsgBlock)
	{
		if (m_MsgBlockList.size() < m_dwMaxBlockNum)
		{        
			m_MsgBlockList.push_back(lpMsgBlock);
		}
		else
		{		
			delete lpMsgBlock;
		}
	}
}

void CNetworkMsgPool::FreeNetworkMsgBlock(CNetworkMsgQueue& msgQueue)
{
    MsgBlockLock::Syncronize sync(m_MsgBlockLock);

    CNetworkMsgBlock* lpMsgBlock = 0;

    while (0 != (lpMsgBlock = msgQueue.Dequeue()))
    {
        if (m_MsgBlockList.size() < m_dwMaxBlockNum)
        {        
            m_MsgBlockList.push_back(lpMsgBlock);
        }
        else
        {
            delete lpMsgBlock;
        }
    }
}

CNetworkMsgPool& CNetworkMsgPool::GetInstance()
{
    static CNetworkMsgPool msgPool;
    return msgPool;
}

CNetworkMsgPool::CNetworkMsgPool()
:   m_dwMaxBlockNum(50)
{
    
}

CNetworkMsgPool::~CNetworkMsgPool()
{
    MsgBlockLock::Syncronize sync(m_MsgBlockLock);
        
    MsgBlockList::iterator pos = m_MsgBlockList.begin();
    MsgBlockList::iterator end = m_MsgBlockList.end();

    for(; pos != end; ++pos)
    {
        delete *pos;
    }

    m_MsgBlockList.clear();
}



CLockedMsgQueue::~CLockedMsgQueue()
{
    MsgQueueLock::Syncronize sync(m_MsgQueueLock); 
    m_MessageQueue.clear();
}

CNetworkMsgBlock* CLockedMsgQueue::GetMessage() 
{ 
    MsgQueueLock::Syncronize sync(m_MsgQueueLock); 
    return m_MessageQueue.Dequeue();
}


void CLockedMsgQueue::GetMessages(CNetworkMsgQueue& msgQueue, DWORD dwItemNum)
{
    CNetworkMsgBlock* lpMsg = 0;

    MsgQueueLock::Syncronize sync(m_MsgQueueLock); 

    for(DWORD dwCount = 0; dwCount < dwItemNum; ++dwCount)
    {
        if(0 != (lpMsg = m_MessageQueue.Dequeue()))
        {
            msgQueue.Enqueue(lpMsg);
        }
        else
        {
            break;
        }
    }
}

void CLockedMsgQueue::PutMessage(CNetworkMsgBlock* lpMsg, bool bPendHead)
{
    MsgQueueLock::Syncronize sync(m_MsgQueueLock);
    m_MessageQueue.Enqueue(lpMsg, bPendHead);
}

void CLockedMsgQueue::PutMessages(CNetworkMsgQueue& msgQueue)
{
    CNetworkMsgBlock* lpMsg = 0;

    MsgQueueLock::Syncronize sync(m_MsgQueueLock); 
    while(0 != (lpMsg = msgQueue.Dequeue()))
    {
        m_MessageQueue.Enqueue(lpMsg);
    }
}


CUnboundedWaitableMsgQueue::CUnboundedWaitableMsgQueue()
{
    // Create Auto-Reset Event
    m_hNotEmpty = CreateEvent(0, FALSE, FALSE, 0);
}

CUnboundedWaitableMsgQueue::~CUnboundedWaitableMsgQueue()
{
    CloseHandle(m_hNotEmpty);
}

DWORD CUnboundedWaitableMsgQueue::WaitForNotEmptyQueue(DWORD dwMilliseconds)
{
    return WaitForSingleObject(m_hNotEmpty, dwMilliseconds);
}

CNetworkMsgBlock* CUnboundedWaitableMsgQueue::GetMessage()
{    
    MsgQueueLock::Syncronize sync(m_MsgQueueLock);
    CNetworkMsgBlock* lpMsg = m_MessageQueue.Dequeue();

    InternalResetNotEmpty();

    return lpMsg;
}

BOOL CUnboundedWaitableMsgQueue::GetMessages(CNetworkMsgQueue& msgQueue, DWORD dwItemNum)
{
    CNetworkMsgBlock* lpMsg = 0;
	int iMsgCount = 0;

    MsgQueueLock::Syncronize sync(m_MsgQueueLock); 

    for(DWORD dwCount = 0; dwCount < dwItemNum; ++dwCount)
    {
        if(0 != (lpMsg = m_MessageQueue.Dequeue()))
        {
			++iMsgCount;
            msgQueue.Enqueue(lpMsg);
        }
        else
        {
            break;
        }
    }

    InternalResetNotEmpty();

	if(iMsgCount == 0)
		return FALSE;

	return TRUE;
}
       
void CUnboundedWaitableMsgQueue::PutMessage(CNetworkMsgBlock* lpMsg, bool bPendHead)
{
    MsgQueueLock::Syncronize sync(m_MsgQueueLock);
    m_MessageQueue.Enqueue(lpMsg, bPendHead);

    InternalSetNotEmpty();
}

void CUnboundedWaitableMsgQueue::PutMessages(CNetworkMsgQueue& msgQueue)
{
    CNetworkMsgBlock* lpMsg = 0;

    MsgQueueLock::Syncronize sync(m_MsgQueueLock); 
    while(0 != (lpMsg = msgQueue.Dequeue()))
    {
        m_MessageQueue.Enqueue(lpMsg);
    }

    InternalSetNotEmpty();
}

void CUnboundedWaitableMsgQueue::InternalSetNotEmpty()
{ 
    if(!m_bNotEmpty && 0 != m_MessageQueue.GetQueuedNum()) 
    {
        SetEvent(m_hNotEmpty); m_bNotEmpty = true;
    }
}

void CUnboundedWaitableMsgQueue::InternalResetNotEmpty()
{
    if(m_bNotEmpty && 0 == m_MessageQueue.GetQueuedNum())
    {
        ResetEvent(m_hNotEmpty);
        m_bNotEmpty = false;
    }
}


}
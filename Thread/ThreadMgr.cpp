#include "stdafx.h"
#include "Thread.h"
#include "ThreadMgr.h"

#include <algorithm>
#include <functional>
#include "GMMemory.h"

// constructor and destructor
CThreadMgr::CThreadMgr() 
: m_nThreadNum(0), m_bJoinStarted(FALSE)
{
    std::fill_n(m_lpThreads, size_t(MAX_THREAD_NUM), reinterpret_cast<CThread*>(0));
    std::fill_n(m_hThreads, size_t(MAX_THREAD_NUM), INVALID_HANDLE_VALUE);
}

CThreadMgr::~CThreadMgr()
{
	JoinThread();
}


bool CThreadMgr::RegisterAndRun(CThread* llpThread)
{		
	unsigned int nThreadID = 0;
	unsigned int nThreadIndex = 0;

	if(0 == llpThread)	
	{		
		return false;
	}

	//	Lock
	{
		ThreadLock::Syncronize sync(m_ThreadLock);

		if(63 <= m_nThreadNum || TRUE == m_bJoinStarted)
		{
			return false;	
		}

		nThreadIndex = m_nThreadNum;
		++m_nThreadNum;
	}

	m_lpThreads[nThreadIndex] = llpThread;
	m_hThreads[nThreadIndex] = reinterpret_cast<HANDLE>(_beginthreadex(0, 0, 
		CThread::ThreadFunc, llpThread, 0, &nThreadID));

	return (0 != m_hThreads[nThreadIndex]);
}

bool CThreadMgr::JoinThread()
{
	
	{			
		ThreadLock::Syncronize sync(m_ThreadLock);
		if(0 == m_nThreadNum)
		{
			return true;
		}

		// 조인이 시작되면, 더이상의 스레드 생성 및 등록을 금지함.
		m_bJoinStarted = TRUE;
	}

	// 스레드 전부 종료 & 대기.
	std::for_each(&m_lpThreads[0], &m_lpThreads[m_nThreadNum], std::mem_fun(&CThread::End));	
	WaitForMultipleObjects(m_nThreadNum, m_hThreads, TRUE, INFINITE);
	
	CThread** lppPastEndThread = m_lpThreads + MAX_THREAD_NUM;
	HANDLE*	  lppPastEndHandle = m_hThreads + MAX_THREAD_NUM;

	// 스레드 소멸.
	for(CThread** lplpThread = m_lpThreads;
		lplpThread != lppPastEndThread; ++lplpThread)
	{
		delete *lplpThread;
	}

	// 스레드 핸들 소멸.
	for(HANDLE*	lppHandle = m_hThreads; 
		lppHandle != lppPastEndHandle; ++lppHandle)
	{
		CloseHandle(*lppHandle);
	}
    
	{
		// 스레드 개수 및 조인 여부 리셋.
		ThreadLock::Syncronize sync(m_ThreadLock);
		m_nThreadNum = 0;
		m_bJoinStarted = FALSE;
	}

	return true;
}


HANDLE CThreadMgr::Run(CThread* lpThread) 
{
	unsigned int nThreadID = 0;
	HANDLE hThread = reinterpret_cast<HANDLE>(_beginthreadex(0, 
		0, CThread::ThreadFunc, lpThread, 0, &nThreadID));

	lpThread->SetHandle(hThread);
	return hThread;
}

bool CThreadMgr::Stop(CThread* lpThread, unsigned long dwTimeout)
{
	if(0 == lpThread)
	{
		return false;
	}

	HANDLE hThread = lpThread->GetHandle();
	if(INVALID_HANDLE_VALUE == hThread)
	{
		return false;
	}

	lpThread->SetHandle(INVALID_HANDLE_VALUE);
	lpThread->End();
	WaitForSingleObject(hThread, dwTimeout);
	return (TRUE == CloseHandle(hThread));
}

#ifndef _CTHREADMGR_H_
#define _CTHREADMGR_H_

#ifndef WINDOWS_LEAN_AND_MEAN
#define WINDOWS_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <windows.h>
#include <process.h>

#include "Lock.h"

// forward decl
class CThread;

class CThreadMgr
{
public:

	enum { MAX_THREAD_NUM = 63 };

	CThreadMgr();
	virtual ~CThreadMgr();
	
	bool RegisterAndRun(CThread* lpThread);	// 스레드를 등록한다. 최대 63개까지 등록할 수 있다.
	bool JoinThread();						// 모든 스레드를 종료시킨다.
	
	// 매니저에 등록하지 않고, 그냥 실행 시킨다.
	static HANDLE	Run(CThread* lpThread);
	static bool		Stop(CThread* lpThread, unsigned long dwTimeout = INFINITE);

private:

	typedef CCSLock	ThreadLock;	

	CThread*		m_lpThreads[MAX_THREAD_NUM];
	HANDLE			m_hThreads[MAX_THREAD_NUM];
	
	ThreadLock		m_ThreadLock;

	unsigned int	m_nThreadNum;
	unsigned int	m_bJoinStarted;
};

#endif
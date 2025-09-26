#ifndef _CTHREAD_H_
#define _CTHREAD_H_

#include <winsock2.h>
#include <windows.h>

class CThread
{
public:

	CThread() : m_hThreadHandle(INVALID_HANDLE_VALUE) { }
	virtual ~CThread() { }
	
protected:

	virtual unsigned int Run() = 0;		// 실제 실행 되는 루프를 넣는다.
	virtual BOOL End() = 0;				// 루프가 끝날 수 있는 루틴을 넣는다.

	typedef unsigned int(__stdcall *LPThreadFunc)(void*);
	static inline unsigned int __stdcall ThreadFunc(void* pArg);

	inline void SetHandle(HANDLE hHandle) { m_hThreadHandle = hHandle; }
	inline HANDLE GetHandle() { return m_hThreadHandle; }
	
	HANDLE	m_hThreadHandle;

	friend class CThreadMgr;
};

inline unsigned int __stdcall CThread::ThreadFunc(void* pArg)
{
	return static_cast<CThread*>(pArg)->Run();
}

#endif
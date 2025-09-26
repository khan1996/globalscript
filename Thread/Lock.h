#ifndef _CLOCK_H_
#define _CLOCK_H_

#include <winsock2.h>
#include <windows.h>


// 다중 CPU환경에서는 Cache-Line에 대한 고려를 해야만 할 것.
#define CACHE_ALIGN		32
#define CACHE_PAD(Name, BytesSoFar) \
		BYTE Name[CACHE_ALIGN - ((BytesSoFar) % CACHE_ALIGN)]


template<class DerivedLockClass>
class CLock	
{
public:
	class Syncronize
	{
	public:
		inline Syncronize(DerivedLockClass* pLockClass) : Lock(*pLockClass) { Lock.Lock(); }
		inline Syncronize(DerivedLockClass& LockClass)  : Lock(LockClass)   { Lock.Lock(); }
		inline ~Syncronize() { Lock.Unlock();}
	protected:
		DerivedLockClass& Lock;
	};
};


// 이 Lock클래스들은 더이상 상속을 받아서는 안 된다.

// ----------------------------------------------------------------------------
// DummyLock ------------------------------------------------------------------

class CDummyLock : public CLock<CDummyLock>
{
public:
	inline void Lock()   { }
	inline void Unlock() { }
	inline bool IsVaildLock() { return true; }
};


// ----------------------------------------------------------------------------
// SpinLock -------------------------------------------------------------------

class CSpinLock : public CLock<CSpinLock>
{
public:
	inline CSpinLock() : m_bLocked(FALSE) { }
	inline ~CSpinLock() { }
	inline void Lock()   { while(TRUE == InterlockedExchange(&m_bLocked, TRUE)) { Sleep(0); } }
	inline void Unlock() { InterlockedExchange(&m_bLocked, FALSE); }
	inline bool IsVaildLock() { return true; }
private:
	volatile LONG m_bLocked;
};



// ----------------------------------------------------------------------------
// Critical Section -----------------------------------------------------------

class CCSLock : public CLock<CCSLock>
{
private:
	CRITICAL_SECTION	m_CSLock;
public:
	inline CCSLock()     { InitializeCriticalSection(&m_CSLock); }
	inline ~CCSLock()    { DeleteCriticalSection(&m_CSLock);     }
	inline void Lock()   { EnterCriticalSection(&m_CSLock);      }
	inline void Unlock() { LeaveCriticalSection(&m_CSLock);      }
	inline bool IsVaildLock() { return true; }
};


// ----------------------------------------------------------------------------
// Mutex Lock -----------------------------------------------------------------

class CMutex : public CLock<CMutex>
{
private:
	HANDLE	m_hMutex;
public:
	inline CMutex() : m_hMutex(CreateMutex(NULL, FALSE, NULL)) { }
	inline ~CMutex()      { if(0 != m_hMutex) { CloseHandle(m_hMutex); }    }
	inline DWORD Lock()   { return WaitForSingleObject(m_hMutex, INFINITE); }
	inline DWORD Unlock() { return ReleaseMutex(m_hMutex); }
	inline bool IsVaildLock() { return (NULL != m_hMutex); }
};



class CNamedMutex : public CLock<CNamedMutex>
{
private:
	HANDLE m_hMutex;
public:
	inline CNamedMutex(const char* szMutexName, BOOL bInitialOwner)
		:	m_hMutex(CreateMutex(NULL, bInitialOwner, szMutexName)) { }
	inline ~CNamedMutex() { if(0 != m_hMutex) { CloseHandle(m_hMutex); } }
	inline DWORD Lock()   { return WaitForSingleObject(m_hMutex, INFINITE); }
	inline DWORD Unlock() { return ReleaseMutex(m_hMutex); }
	inline bool IsVaildLock() { return (NULL != m_hMutex); }
};


/*
class CFairRWLock : private CCSLock
{
private:
  Semaphore     access_lock;     // used as a one-at-a-time release valve
  CCSLock       read_barrier;    // used to block/wakeup readers
  unsigned int  is_write_lock;   // nonzero if locked for writing
  unsigned int  writer_count;    // # of writers waiting for or holding the lock
  unsigned int  reader_count;    // # of readers holding the lock
  unsigned int  readers_waiting; // # of readers waiting for the lock

public:
  ReadLock()
      : access_lock(1), is_write_lock(0), 
        writers_waiting(0), reader_count(0), readers_waiting(0)
  {
    
  }

  void ReadLock() 
  {
    Mutex::Lock();

    // if there is at least one writer using the lock or waiting for it,
    // we need to wait for access
    if (writer_count > 0)) 
    {
      if (readers_waiting++ == 0) // if we're the first reader in line
        Mutex::Unlock();
        access_lock.Wait();       // get the access lock
        Mutex::Lock();
        if (readers_waiting > 1)  // then if there are other readers
          read_barrier.Open();    // let them go
      }
      else 
      {
        Mutex::Unlock();
        read_barrier.Wait();      // otherwise wait until someone lets us go
        Mutex::Lock();
      }
      readers_waiting--;
    }
    reader_count++;
    Mutex::Unlock();
  }

  void WriteLock() 
  {
    Mutex::Lock();
    writer_count++;       // one more writer
    Mutex::Unlock();
    access_lock.Wait();   // wait until the access lock is available
    Mutex::Lock();
    is_write_lock = 1;    // lock is a write lock
    read_barrier.Close(); // give readers something to wait for
    Mutex::Unlock();
  }

  void Unlock() 
  {
    Mutex::Lock();
    if (is_write_lock)
    {          // if this is a write lock
      is_write_lock = 0;          // let it go
      writer_count--;             // one less writer
      access_lock.Post();         // now let someone else have a chance
    }
    else if (--reader_count == 0)
    {
        // if we're the last reader
        access_lock.Post();         // release the access lock
    }
    Mutex::Unlock();
  }
};
*/



#endif
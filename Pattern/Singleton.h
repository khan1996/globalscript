#ifndef _CSINGLETON_H_
#define _CSINGLETON_H_

#include <cassert>

template<typename Derived>
class CSingleton
{
private:

	static Derived* ms_pSingleton;

protected:

	CSingleton();
	~CSingleton();

public:

	inline static Derived& GetInstance();
	inline static Derived* GetInstancePtr();
};


template<typename Derived>
CSingleton<Derived>::CSingleton()
{
	assert(!ms_pSingleton && "Singleton 클래스가 이미 생성되어 있습니다.");
	size_t nOffset = (size_t)(Derived*) 1 - (size_t)(CSingleton<Derived>*)(Derived*) 1;
	ms_pSingleton = (Derived*)((size_t)this + nOffset);
}


template<typename Derived>
CSingleton<Derived>::~CSingleton() 
{
	assert(ms_pSingleton && "Singleton 클래스가 생성되지 않았습니다");
    ms_pSingleton = 0;
}

template<typename Derived>
inline Derived& CSingleton<Derived>::GetInstance() 
{
	assert(ms_pSingleton && "Singleton 클래스가 생성되지 않았습니다");
    return (*ms_pSingleton);
}

template<typename Derived>
inline Derived* CSingleton<Derived>::GetInstancePtr()
{
	assert(ms_pSingleton && "Singleton 클래스가 생성되지 않았습니다");
    return (ms_pSingleton);
}

template<typename Derived>
Derived* CSingleton<Derived>::ms_pSingleton = 0;



// Scotte Meyer's Implementation
template<typename Derived>
class CStaticSingleton
{
public:

	inline static Derived& GetInstance() 
    {
        static Derived Instance;
        return Instance;
    }
};

template<typename Derived>
class CLeakStaticSingleton
{
public:

    inline static Derived& GetInstance()
    {
        static Derived* lpInstance = new Derived;
        return *lpInstance;
    }
};



// C++ FAQ's Implementation
template<typename T>
class CNiftyCounterSingleton
{
public:

    CNiftyCounterSingleton() { ++nifty_count_; }
    ~CNiftyCounterSingleton() { if ( 0 == --nifty_count_ && pInstance_ ) { delete pInstance_; } }

    static T& GetInstance()
    {
        if (NULL == pInstance_) { pInstance_ = new T; }
        return	*pInstance_;
    }

private:

    static int  nifty_count_;
    static T*   pInstance_;
};

template<typename T>
int CNiftyCounterSingleton<T>::nifty_count_	= 0;

template<typename T>
T*  CNiftyCounterSingleton<T>::pInstance_	= 0;

/*
    클래스 헤더에 다음을 포함할 것.
    클래스 생성, 소멸자는 private이고, CNiftyCounterSingleton을 멤버로 가질 것.

    namespace
    {
        CNiftyCounterSingleton<T>   TNiftyCounter;
    }
*/

#endif
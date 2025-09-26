#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_


/*
// 자체 함수 Exception 사용
Nave::Exception::EnableException(true);
// Dump 저장
Nave::Exception::EnableSaveDump(true);

*/ 

#include <string>
#include <ostream>

void MiniDump(const char* filename, PEXCEPTION_POINTERS ep);

class Exception
{
private:
	enum {
		BufferSize=1024
	};
	char buffer[BufferSize];
	void set(const char* message);

public:
	Exception(const char* message);
	Exception(const std::string& message);

	virtual ~Exception(void);

	virtual const char* What() const;

public:
	enum MiniDumpTypes 
	{ 
		NormalDump=0, DataSegs, HeapDump 
	};

	static char DefaultDumpFileName[512];
	static MiniDumpTypes MiniDumpType;
	static bool ExceptionSaveDump;
	static bool ExecptionAutoThrow;

	static LPTOP_LEVEL_EXCEPTION_FILTER lpPreviousFilter;

	static void SetDumpType(MiniDumpTypes type);
	static void EnableSaveDump(bool enable = true);
	static void SetDumpName(char* strDumpName) { strcpy(DefaultDumpFileName, strDumpName); }

	// EnableUnhandledExceptioinFilter 이걸 사용할경우 
	// try~catch로 묶이지 않은 곳에서 예외가 발생할때 프로그램이 특정행동을
	// 하고 종료될수 있다록 정의할수있다.
	// EnableSaveDump 이 정보에 의해 조용히 죽을주, dmp를 남기고 죽을지 
	// 정의한다.
	// 현재는 덤프를 남기는 역활만 한다.
	// 나중에 죽으면서 특정 어플리케이션을 샐행 하도록 처리할수 있다.
	static void EnableUnhandledExceptioinFilterEx(LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter, bool enable = true);
	static void EnableUnhandledExceptioinFilter(bool enable = true);

	// 강제로 Dump파일을 남기기 위한 방법
	static void MiniDump(const char* filename);
};

#endif
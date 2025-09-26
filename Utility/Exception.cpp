#include "stdafx.h"
#include <windows.h>
#include <crtdbg.h>
#include <malloc.h> 
#include <time.h>
#include <tlhelp32.h>
#include <excpt.h>

#pragma comment (lib, "dbghelp")

#include "DbgHelp.h"
#include "Exception.h"
#include <sstream>

#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)
#pragma warning(disable:4127)
#pragma warning(disable:4535) // must be compiled with: /EHa

/*
// 기본 에러메시지 박스창을 띄우지 않게 하기 위한창.
SetErrorMode(UINT uMode)

SEM_FAILCRITICALERRORS
“응용프로그램 오류”라고 하는 회색창(-_-)을 보여주지 않는다.

SEM_NOGPFAULTERRORBOX
NOGPFAULT 란다. NO General Protection FAULT 를 줄여놓은것이다. 일반 보호오류가 발생해도 회색창을 보여주지 않는다.

SEM_NOOPENFILEERRORBOX
혹시 본적이 있는지 모르겠지만, 파일을 오픈할 때 없는 파일을 오픈하면 오류가 발생한다. 회색이지만(-_-) “응용프로그램 오류” 같은 창이 아닌 메시지 박스가 뜬다(‘빈 파일에 엑세스할 수 없습니다.’라던가? 그 비슷한 메시지를 보여줬던 것 같다). 그걸 안보이게 해준다.
*/


/*
	Release 모드에서는 예외가 작동하지 않는다.
	예외를 작동하기 위해선 옵션의 최적화 모드를 /Od 로 만들어야 한다.
	/Od가 아닌 디폴트값 /O2 에서도 예외가 가능한 방법을 찾자.
*/

Exception::MiniDumpTypes Exception::MiniDumpType = Exception::NormalDump;
bool Exception::ExceptionSaveDump = false;
char Exception::DefaultDumpFileName[512]="";
LPTOP_LEVEL_EXCEPTION_FILTER Exception::lpPreviousFilter = NULL;


bool IsNT()
{	
	OSVERSIONINFO vi = { sizeof(vi)};	
	::GetVersionEx(&vi);
	return vi.dwPlatformId == VER_PLATFORM_WIN32_NT;
}

HANDLE SymGetProcessHandle()
{
	if (IsNT())
		return GetCurrentProcess();
	else
		return (HANDLE)GetCurrentProcessId();
}

void MiniDump(const char* filename, PEXCEPTION_POINTERS ep)
{
	HANDLE process=SymGetProcessHandle();
	DWORD processId=GetCurrentProcessId();

	MINIDUMP_EXCEPTION_INFORMATION ei;
	ei.ClientPointers=FALSE;
	ei.ThreadId=GetCurrentThreadId();
	ei.ExceptionPointers=ep;

	HANDLE file=CreateFile(filename, GENERIC_WRITE, FILE_SHARE_READ
		, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file==INVALID_HANDLE_VALUE) return;

	_MINIDUMP_TYPE type=MiniDumpNormal;
	switch(Exception::MiniDumpType)
	{
		case Exception::HeapDump: type=MiniDumpWithFullMemory; break;
	};

	MiniDumpWriteDump(process, processId, file, type, &ei, NULL, NULL);

	CloseHandle(file);

}

char* GetExceptionString(DWORD Code)
{
	switch(Code)
	{
	case EXCEPTION_ACCESS_VIOLATION:		 return "EXCEPTION_ACCESS_VIOLATION";
	case EXCEPTION_BREAKPOINT:				 return "EXCEPTION_BREAKPOINT";
	case EXCEPTION_DATATYPE_MISALIGNMENT:	 return "EXCEPTION_DATATYPE_MISALIGNMENT";
	case EXCEPTION_SINGLE_STEP:				 return "EXCEPTION_SINGLE_STEP";
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:	 return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
	case EXCEPTION_FLT_DENORMAL_OPERAND:	 return "EXCEPTION_FLT_DENORMAL_OPERAND";
	case EXCEPTION_FLT_DIVIDE_BY_ZERO:		 return "EXCEPTION_FLT_DIVIDE_BY_ZERO";
	case EXCEPTION_FLT_INEXACT_RESULT:		 return "EXCEPTION_FLT_INEXACT_RESULT";
	case EXCEPTION_FLT_INVALID_OPERATION:	 return "EXCEPTION_FLT_INVALID_OPERATION";
	case EXCEPTION_FLT_OVERFLOW:			 return "EXCEPTION_FLT_OVERFLOW";
	case EXCEPTION_FLT_STACK_CHECK:			 return "EXCEPTION_FLT_STACK_CHECK";
	case EXCEPTION_FLT_UNDERFLOW:			 return "EXCEPTION_FLT_UNDERFLOW";
	case EXCEPTION_INT_DIVIDE_BY_ZERO:		 return "EXCEPTION_INT_DIVIDE_BY_ZERO";
	case EXCEPTION_INT_OVERFLOW:			 return "EXCEPTION_INT_OVERFLOW";
	case EXCEPTION_PRIV_INSTRUCTION:		 return "EXCEPTION_PRIV_INSTRUCTION";
	case EXCEPTION_NONCONTINUABLE_EXCEPTION: return "EXCEPTION_NONCONTINUABLE_EXCEPTION";
	case EXCEPTION_STACK_OVERFLOW:			 return "EXCEPTION_STACK_OVERFLOW";
	}
	return "INVALID";
}

LONG __stdcall UserUnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo) 
{
	if(Exception::ExceptionSaveDump)
	{
		// 예외를 호출하기 전에 미니 덤프파일을 생성한다.
		char dumpfile[512];

		if(strlen(Exception::DefaultDumpFileName) == 0)
		{
			SYSTEMTIME	st;
			GetLocalTime(&st);

			sprintf(dumpfile, ".\\Errors\\%04d%02d%02d-%02d%02d%02d.dmp",
				st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		}
		else 
			strcpy(dumpfile, Exception::DefaultDumpFileName);

		// 이 덤프 생성루틴을 Execption 에서 옵션으로 변경할수 있도록 변경.
		// 미니덤프 생성.
		MiniDump(dumpfile, pExceptionInfo);
	}

	MessageBox(GetActiveWindow(), "ROW Client Error.", "Error", MB_OK);

	return EXCEPTION_EXECUTE_HANDLER;
}

void Exception::SetDumpType(MiniDumpTypes type)
{
	Exception::MiniDumpType=type;
}

void Exception::EnableSaveDump(bool enable)
{
	Exception::ExceptionSaveDump = enable;
}

void Exception::EnableUnhandledExceptioinFilterEx(LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter, bool enable)
{
	if(lpPreviousFilter)
	{
		::SetUnhandledExceptionFilter(lpPreviousFilter);
		lpPreviousFilter = NULL;
	}

	// try~catch 에 정의 되지 않은 예외오류를 검사하기 위한 
	// 오류처리루틴이다.
	if(enable)
		lpPreviousFilter = ::SetUnhandledExceptionFilter(lpTopLevelExceptionFilter);
	else
	{
		::SetUnhandledExceptionFilter(lpPreviousFilter);
		lpPreviousFilter = NULL;
	}
}

void Exception::EnableUnhandledExceptioinFilter(bool enable)
{
	if(lpPreviousFilter)
	{
		::SetUnhandledExceptionFilter(lpPreviousFilter);
		lpPreviousFilter = NULL;
	}

	// try~catch 에 정의 되지 않은 예외오류를 검사하기 위한 
	// 오류처리루틴이다.
	if(enable)
		lpPreviousFilter = ::SetUnhandledExceptionFilter(UserUnhandledExceptionFilter);
	else
	{
		::SetUnhandledExceptionFilter(lpPreviousFilter);
		lpPreviousFilter = NULL;
	}
}

// 이방법은 강제로 덤프파일을 남길수 있다.
void Exception::MiniDump(const char* filename)
{
	HANDLE process=SymGetProcessHandle();
	DWORD processId=GetCurrentProcessId();
/*		MINIDUMP_EXCEPTION_INFORMATION ei;
	PEXCEPTION_POINTERS ep = GetExceptionInformation();
	ei.ClientPointers=false;
	ei.ThreadId=GetCurrentThreadId();
	ei.ExceptionPointers=ep;
*/
	HANDLE file=CreateFile(filename, GENERIC_WRITE, FILE_SHARE_READ
		, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file==INVALID_HANDLE_VALUE) return;

	_MINIDUMP_TYPE type=MiniDumpNormal;
	switch(Exception::MiniDumpType)
	{
//		case Exception::DataSegs: type=MiniDumpWithDataSegs; break;
		case Exception::HeapDump: type=MiniDumpWithFullMemory; break;
	};

	MiniDumpWriteDump(process, processId, file, type, NULL, NULL, NULL);
	CloseHandle(file);
}

void Exception::set(const char* message) 
{
	strcpy(buffer, message);
}

Exception::Exception(const char* message) {
	set(message);
}

Exception::Exception(const std::string& message) {
	set(message.c_str());
}

const char* Exception::What() const {
	return buffer;
}

Exception::~Exception(void)
{
}

#pragma warning(pop)

#ifndef _SERVER_LOG_DEFINE_H_
#define _SERVER_LOG_DEFINE_H_

#pragma once

// ---------------------------------------------------------------------------------------
// 소스 레벨

#define _SOURCE_LEVEL_MINIMUM	0	// Only output errors
#define _SOURCE_LEVEL_RELEASE	1	// Normal release mode
#define _SOURCE_LEVEL_BETA		2	// Normal beta season mode
#define _SOURCE_LEVEL_DEBUG		3	// Output mostly
#define _SOURCE_LEVEL_FULL		4	// Output fully
#define _SOURCE_LEVEL_NONE      5   // DO NOT OUTPUT

#define _SRCFILE_LEVEL  _SOURCE_LEVEL_NONE

/*
#ifndef _SRCFILE_LEVEL		// 로그 레벨이 지정되어 있는지 확인

	#ifdef _DEBUG
		#define _SRCFILE_LEVEL		_SOURCE_LEVEL_FULL
	#else
		#define _SRCFILE_LEVEL		_SOURCE_LEVEL_FULL
	#endif

#endif
*/

// Function, File, Line을 인자로 넘기기 위한 매크로.
#define LOG_FFL __FUNCTION__, __FILE__, __LINE__


#if (_SRCFILE_LEVEL == _SOURCE_LEVEL_MINIMUM)
	#define USE_LOG_SYSERR
	#define USE_LOG_ERROR
	#define USE_LOG_RULE
	#define USE_LOG_INFO
#elif (_SRCFILE_LEVEL == _SOURCE_LEVEL_RELEASE)
	#define USE_LOG_SYSERR
	#define USE_LOG_ERROR
	#define USE_LOG_WARN
	#define USE_LOG_INFO
	#define USE_LOG_RULE
#elif (_SRCFILE_LEVEL == _SOURCE_LEVEL_BETA)
	#define USE_LOG_SYSERR
	#define USE_LOG_ERROR
	#define USE_LOG_DEBUG
	#define USE_LOG_WARN
	#define USE_LOG_DUMP
	#define USE_LOG_INFO
	#define USE_LOG_RULE
	#define USE_LOG_STAT
#elif (_SRCFILE_LEVEL == _SOURCE_LEVEL_DEBUG)
	#define USE_LOG_SYSERR
	#define USE_LOG_ERROR
	#define USE_LOG_DEBUG
	#define USE_LOG_WARN
	#define USE_LOG_DUMP
	#define USE_LOG_INFO
	#define USE_LOG_RULE
	#define USE_LOG_STAT
#elif (_SRCFILE_LEVEL == _SOURCE_LEVEL_FULL)
	#define USE_LOG_NORMAL
	#define USE_LOG_DETAIL
	#define USE_LOG_SYSERR
	#define USE_LOG_ERROR
	#define USE_LOG_DEBUG
	#define USE_LOG_WARN
	#define USE_LOG_DUMP
	#define USE_LOG_INFO
	#define USE_LOG_RULE
	#define USE_LOG_STAT
#elif (_SRCFILE_LEVEL == _SOURCE_LEVEL_NONE)

	#undef USE_LOG_NORMAL
	#undef USE_LOG_DETAIL
	#undef USE_LOG_SYSERR
	#undef USE_LOG_ERROR
	#undef USE_LOG_DEBUG
	#undef USE_LOG_WARN
	#undef USE_LOG_DUMP
	#undef USE_LOG_INFO
	#undef USE_LOG_RULE
	#undef USE_LOG_STAT

#endif


enum LOG_TYPE
{
	LOG_NORMAL,		// 일반 로그 (클래스 생성, 종료)
	LOG_RULE,		// 운영 관련 로그		
	LOG_INFO,		// 정보 관련 로그

	LOG_DETAIL,		// 하이 디테일 관련 로그
	LOG_SYSERR,		// 시스템 함수 에러 관련 로그
	LOG_ERROR,		// 에러 관련 로그
	LOG_DEBUG,		// 디버그 관련 로그
	LOG_WARN,		// 경고 관련 로그

	MAX_LOG_TYPE
};

// -------------------------------------------------------------------------------------
// Simple Log Type

#ifdef	USE_LOG_NORMAL
	#define NORLOG(x)	x
	#define NORLOG0(LogInstance, szString)															LogInstance.SimpleLog(LOG_NORMAL, szString)
	#define NORLOG1(LogInstance, szFormat, arg1)													LogInstance.SimpleLog(LOG_NORMAL, szFormat, arg1)
	#define NORLOG2(LogInstance, szFormat, arg1, arg2)												LogInstance.SimpleLog(LOG_NORMAL, szFormat, arg1, arg2)
	#define NORLOG3(LogInstance, szFormat, arg1, arg2, arg3)										LogInstance.SimpleLog(LOG_NORMAL, szFormat, arg1, arg2, arg3)
	#define NORLOG4(LogInstance, szFormat, arg1, arg2, arg3, arg4)									LogInstance.SimpleLog(LOG_NORMAL, szFormat, arg1, arg2, arg3, arg4)
	#define NORLOG5(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5)							LogInstance.SimpleLog(LOG_NORMAL, szFormat, arg1, arg2, arg3, arg4, arg5)
	#define NORLOG6(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6)						LogInstance.SimpleLog(LOG_NORMAL, szFormat, arg1, arg2, arg3, arg4, arg5, arg6)
	#define NORLOG7(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7)				LogInstance.SimpleLog(LOG_NORMAL, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
	#define NORLOG8(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)			LogInstance.SimpleLog(LOG_NORMAL, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)
	#define NORLOG9(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)	LogInstance.SimpleLog(LOG_NORMAL, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)
#else
	#define NORLOG(x)	(void*)0
	#define NORLOG0(LogInstance, szString)															(void*)0
	#define NORLOG1(LogInstance, szFormat, arg1)													(void*)0
	#define NORLOG2(LogInstance, szFormat, arg1, arg2)												(void*)0
	#define NORLOG3(LogInstance, szFormat, arg1, arg2, arg3)										(void*)0
	#define NORLOG4(LogInstance, szFormat, arg1, arg2, arg3, arg4)									(void*)0
	#define NORLOG5(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5)							(void*)0
	#define NORLOG6(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6)						(void*)0	
	#define NORLOG7(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7)				(void*)0
	#define NORLOG8(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)			(void*)0
	#define NORLOG9(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)	(void*)0
#endif


#ifdef	USE_LOG_INFO
	#define INFLOG(x)	x
	#define INFLOG0(LogInstance, szString)															LogInstance.SimpleLog(LOG_INFO, szString)
	#define INFLOG1(LogInstance, szFormat, arg1)													LogInstance.SimpleLog(LOG_INFO, szFormat, arg1)
	#define INFLOG2(LogInstance, szFormat, arg1, arg2)												LogInstance.SimpleLog(LOG_INFO, szFormat, arg1, arg2)
	#define INFLOG3(LogInstance, szFormat, arg1, arg2, arg3)										LogInstance.SimpleLog(LOG_INFO, szFormat, arg1, arg2, arg3)
	#define INFLOG4(LogInstance, szFormat, arg1, arg2, arg3, arg4)									LogInstance.SimpleLog(LOG_INFO, szFormat, arg1, arg2, arg3, arg4)
	#define INFLOG5(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5)							LogInstance.SimpleLog(LOG_INFO, szFormat, arg1, arg2, arg3, arg4, arg5)
	#define INFLOG6(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6)						LogInstance.SimpleLog(LOG_INFO, szFormat, arg1, arg2, arg3, arg4, arg5, arg6)
	#define INFLOG7(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7)				LogInstance.SimpleLog(LOG_INFO, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
	#define INFLOG8(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)			LogInstance.SimpleLog(LOG_INFO, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)
	#define INFLOG9(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)	LogInstance.SimpleLog(LOG_INFO, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)
#else
	#define INFLOG(x)	(void*)0
	#define INFLOG0(LogInstance, szString)															(void*)0
	#define INFLOG1(LogInstance, szFormat, arg1)													(void*)0
	#define INFLOG2(LogInstance, szFormat, arg1, arg2)												(void*)0
	#define INFLOG3(LogInstance, szFormat, arg1, arg2, arg3)										(void*)0
	#define INFLOG4(LogInstance, szFormat, arg1, arg2, arg3, arg4)									(void*)0
	#define INFLOG5(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5)							(void*)0
	#define INFLOG6(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6)						(void*)0
	#define INFLOG7(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7)				(void*)0
	#define INFLOG8(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)			(void*)0
	#define INFLOG9(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)	(void*)0
#endif


#ifdef	USE_LOG_RULE
	#define RULLOG(x)	x
	#define RULLOG0(LogInstance, szString)															LogInstance.SimpleLog(LOG_RULE, szString)
	#define RULLOG1(LogInstance, szFormat, arg1)													LogInstance.SimpleLog(LOG_RULE, szFormat, arg1)
	#define RULLOG2(LogInstance, szFormat, arg1, arg2)												LogInstance.SimpleLog(LOG_RULE, szFormat, arg1, arg2)
	#define RULLOG3(LogInstance, szFormat, arg1, arg2, arg3)										LogInstance.SimpleLog(LOG_RULE, szFormat, arg1, arg2, arg3)
	#define RULLOG4(LogInstance, szFormat, arg1, arg2, arg3, arg4)									LogInstance.SimpleLog(LOG_RULE, szFormat, arg1, arg2, arg3, arg4)
	#define RULLOG5(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5)							LogInstance.SimpleLog(LOG_RULE, szFormat, arg1, arg2, arg3, arg4, arg5)
	#define RULLOG6(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6)						LogInstance.SimpleLog(LOG_RULE, szFormat, arg1, arg2, arg3, arg4, arg5, arg6)
	#define RULLOG7(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7)				LogInstance.SimpleLog(LOG_RULE, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
	#define RULLOG8(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)			LogInstance.SimpleLog(LOG_RULE, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)
	#define RULLOG9(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)	LogInstance.SimpleLog(LOG_RULE, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)
#else
	#define RULLOG(x)	(void*)0
	#define RULLOG0(LogInstance, szString)															(void*)0
	#define RULLOG1(LogInstance, szFormat, arg1)													(void*)0
	#define RULLOG2(LogInstance, szFormat, arg1, arg2)												(void*)0
	#define RULLOG3(LogInstance, szFormat, arg1, arg2, arg3)										(void*)0
	#define RULLOG4(LogInstance, szFormat, arg1, arg2, arg3, arg4)									(void*)0
	#define RULLOG5(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5)							(void*)0
	#define RULLOG6(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6)						(void*)0
	#define RULLOG7(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7)				(void*)0
	#define RULLOG8(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)			(void*)0
	#define RULLOG9(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)	(void*)0
#endif


// -------------------------------------------------------------------------------------
// Detail Log Type


#ifdef	USE_LOG_DETAIL
	#define DETLOG(x)	x
	#define DETLOG0(LogInstance, szString)															LogInstance.DetailLog(LOG_DETAIL, LOG_FFL, szString)
	#define DETLOG1(LogInstance, szFormat, arg1)													LogInstance.DetailLog(LOG_DETAIL, LOG_FFL, szFormat, arg1)
	#define DETLOG2(LogInstance, szFormat, arg1, arg2)												LogInstance.DetailLog(LOG_DETAIL, LOG_FFL, szFormat, arg1, arg2)
	#define DETLOG3(LogInstance, szFormat, arg1, arg2, arg3)										LogInstance.DetailLog(LOG_DETAIL, LOG_FFL, szFormat, arg1, arg2, arg3)
	#define DETLOG4(LogInstance, szFormat, arg1, arg2, arg3, arg4)									LogInstance.DetailLog(LOG_DETAIL, LOG_FFL, szFormat, arg1, arg2, arg3, arg4)
	#define DETLOG5(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5)							LogInstance.DetailLog(LOG_DETAIL, LOG_FFL, szFormat, arg1, arg2, arg3, arg4, arg5)
	#define DETLOG6(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6)						LogInstance.DetailLog(LOG_DETAIL, LOG_FFL, szFormat, arg1, arg2, arg3, arg4, arg5, arg6)
	#define DETLOG7(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7)				LogInstance.DetailLog(LOG_DETAIL, LOG_FFL, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
	#define DETLOG8(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)			LogInstance.DetailLog(LOG_DETAIL, LOG_FFL, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)
	#define DETLOG9(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)	LogInstance.DetailLog(LOG_DETAIL, LOG_FFL, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)
	#define DETLOG10(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10)	LogInstance.DetailLog(LOG_DETAIL, LOG_FFL, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10)
#else
	#define DETLOG(x)	(void*)0
	#define DETLOG0(LogInstance, szString)															(void*)0
	#define DETLOG1(LogInstance, szFormat, arg1)													(void*)0
	#define DETLOG2(LogInstance, szFormat, arg1, arg2)												(void*)0
	#define DETLOG3(LogInstance, szFormat, arg1, arg2, arg3)										(void*)0
	#define DETLOG4(LogInstance, szFormat, arg1, arg2, arg3, arg4)									(void*)0
	#define DETLOG5(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5)							(void*)0
	#define DETLOG6(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6)						(void*)0
	#define DETLOG7(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7)				(void*)0
	#define DETLOG8(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)			(void*)0
	#define DETLOG9(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)	(void*)0
	#define DETLOG10(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10)	(void*)0
#endif


#ifdef	USE_LOG_SYSERR
	#define SERLOG(x)	x
	#define SERLOG0(LogInstance, szString)															LogInstance.DetailLog(LOG_SYSERR, LOG_FFL, szString)
	#define SERLOG1(LogInstance, szFormat, arg1)													LogInstance.DetailLog(LOG_SYSERR, LOG_FFL, szFormat, arg1)
	#define SERLOG2(LogInstance, szFormat, arg1, arg2)												LogInstance.DetailLog(LOG_SYSERR, LOG_FFL, szFormat, arg1, arg2)
	#define SERLOG3(LogInstance, szFormat, arg1, arg2, arg3)										LogInstance.DetailLog(LOG_SYSERR, LOG_FFL, szFormat, arg1, arg2, arg3)
	#define SERLOG4(LogInstance, szFormat, arg1, arg2, arg3, arg4)									LogInstance.DetailLog(LOG_SYSERR, LOG_FFL, szFormat, arg1, arg2, arg3, arg4)
	#define SERLOG5(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5)							LogInstance.DetailLog(LOG_SYSERR, LOG_FFL, szFormat, arg1, arg2, arg3, arg4, arg5)
	#define SERLOG6(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6)						LogInstance.DetailLog(LOG_SYSERR, LOG_FFL, szFormat, arg1, arg2, arg3, arg4, arg5, arg6)
	#define SERLOG7(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7)				LogInstance.DetailLog(LOG_SYSERR, LOG_FFL, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
	#define SERLOG8(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)			LogInstance.DetailLog(LOG_SYSERR, LOG_FFL, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)
	#define SERLOG9(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)	LogInstance.DetailLog(LOG_SYSERR, LOG_FFL, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)
#else
	#define SERLOG(x)	(void*)0
	#define SERLOG0(LogInstance, szString)															(void*)0
	#define SERLOG1(LogInstance, szFormat, arg1)													(void*)0
	#define SERLOG2(LogInstance, szFormat, arg1, arg2)												(void*)0
	#define SERLOG3(LogInstance, szFormat, arg1, arg2, arg3)										(void*)0
	#define SERLOG4(LogInstance, szFormat, arg1, arg2, arg3, arg4)									(void*)0
	#define SERLOG5(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5)							(void*)0
	#define SERLOG6(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6)						(void*)0
	#define SERLOG7(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7)				(void*)0
	#define SERLOG8(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)			(void*)0
	#define SERLOG9(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)	(void*)0
#endif


#ifdef	USE_LOG_ERROR
	#define ERRLOG(x)	x
	#define ERRLOG0(LogInstance, szString)															LogInstance.DetailLog(LOG_ERROR, LOG_FFL, szString)
	#define ERRLOG1(LogInstance, szFormat, arg1)													LogInstance.DetailLog(LOG_ERROR, LOG_FFL, szFormat, arg1)
	#define ERRLOG2(LogInstance, szFormat, arg1, arg2)												LogInstance.DetailLog(LOG_ERROR, LOG_FFL, szFormat, arg1, arg2)
	#define ERRLOG3(LogInstance, szFormat, arg1, arg2, arg3)										LogInstance.DetailLog(LOG_ERROR, LOG_FFL, szFormat, arg1, arg2, arg3)
	#define ERRLOG4(LogInstance, szFormat, arg1, arg2, arg3, arg4)									LogInstance.DetailLog(LOG_ERROR, LOG_FFL, szFormat, arg1, arg2, arg3, arg4)
	#define ERRLOG5(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5)							LogInstance.DetailLog(LOG_ERROR, LOG_FFL, szFormat, arg1, arg2, arg3, arg4, arg5)
	#define ERRLOG6(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6)						LogInstance.DetailLog(LOG_ERROR, LOG_FFL, szFormat, arg1, arg2, arg3, arg4, arg5, arg6)
	#define ERRLOG7(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7)				LogInstance.DetailLog(LOG_ERROR, LOG_FFL, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
	#define ERRLOG8(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)			LogInstance.DetailLog(LOG_ERROR, LOG_FFL, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)
	#define ERRLOG9(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)	LogInstance.DetailLog(LOG_ERROR, LOG_FFL, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)
#else
	#define ERRLOG(x)	(void*)0
	#define ERRLOG0(LogInstance, szString)															(void*)0
	#define ERRLOG1(LogInstance, szFormat, arg1)													(void*)0
	#define ERRLOG2(LogInstance, szFormat, arg1, arg2)												(void*)0
	#define ERRLOG3(LogInstance, szFormat, arg1, arg2, arg3)										(void*)0
	#define ERRLOG4(LogInstance, szFormat, arg1, arg2, arg3, arg4)									(void*)0
	#define ERRLOG5(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5)							(void*)0
	#define ERRLOG6(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6)						(void*)0
	#define ERRLOG7(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7)				(void*)0
	#define ERRLOG8(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)			(void*)0
	#define ERRLOG9(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)	(void*)0
#endif


#ifdef	USE_LOG_DEBUG
	#define DBGLOG(x)	x
	#define DBGLOG0(LogInstance, szString)															LogInstance.DetailLog(LOG_DEBUG, LOG_FFL, szString)
	#define DBGLOG1(LogInstance, szFormat, arg1)													LogInstance.DetailLog(LOG_DEBUG, LOG_FFL, szFormat, arg1)
	#define DBGLOG2(LogInstance, szFormat, arg1, arg2)												LogInstance.DetailLog(LOG_DEBUG, LOG_FFL, szFormat, arg1, arg2)
	#define DBGLOG3(LogInstance, szFormat, arg1, arg2, arg3)										LogInstance.DetailLog(LOG_DEBUG, LOG_FFL, szFormat, arg1, arg2, arg3)
	#define DBGLOG4(LogInstance, szFormat, arg1, arg2, arg3, arg4)									LogInstance.DetailLog(LOG_DEBUG, LOG_FFL, szFormat, arg1, arg2, arg3, arg4)
	#define DBGLOG5(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5)							LogInstance.DetailLog(LOG_DEBUG, LOG_FFL, szFormat, arg1, arg2, arg3, arg4, arg5)
	#define DBGLOG6(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6)						LogInstance.DetailLog(LOG_DEBUG, LOG_FFL, szFormat, arg1, arg2, arg3, arg4, arg5, arg6)
	#define DBGLOG7(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7)				LogInstance.DetailLog(LOG_DEBUG, LOG_FFL, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
	#define DBGLOG8(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)			LogInstance.DetailLog(LOG_DEBUG, LOG_FFL, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)
	#define DBGLOG9(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)	LogInstance.DetailLog(LOG_DEBUG, LOG_FFL, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)
#else
	#define DBGLOG(x)	(void*)0
	#define DBGLOG0(LogInstance, szString)															(void*)0
	#define DBGLOG1(LogInstance, szFormat, arg1)													(void*)0
	#define DBGLOG2(LogInstance, szFormat, arg1, arg2)												(void*)0
	#define DBGLOG3(LogInstance, szFormat, arg1, arg2, arg3)										(void*)0
	#define DBGLOG4(LogInstance, szFormat, arg1, arg2, arg3, arg4)									(void*)0
	#define DBGLOG5(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5)							(void*)0
	#define DBGLOG6(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6)						(void*)0
	#define DBGLOG7(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7)				(void*)0
	#define DBGLOG8(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)			(void*)0
	#define DBGLOG9(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)	(void*)0
#endif


#ifdef	USE_LOG_WARN
	#define WRNLOG(x)	x
	#define WRNLOG0(LogInstance, szString)															LogInstance.DetailLog(LOG_WARN, LOG_FFL, szString)
	#define WRNLOG1(LogInstance, szFormat, arg1)													LogInstance.DetailLog(LOG_WARN, LOG_FFL, szFormat, arg1)
	#define WRNLOG2(LogInstance, szFormat, arg1, arg2)												LogInstance.DetailLog(LOG_WARN, LOG_FFL, szFormat, arg1, arg2)
	#define WRNLOG3(LogInstance, szFormat, arg1, arg2, arg3)										LogInstance.DetailLog(LOG_WARN, LOG_FFL, szFormat, arg1, arg2, arg3)
	#define WRNLOG4(LogInstance, szFormat, arg1, arg2, arg3, arg4)									LogInstance.DetailLog(LOG_WARN, LOG_FFL, szFormat, arg1, arg2, arg3, arg4)
	#define WRNLOG5(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5)							LogInstance.DetailLog(LOG_WARN, LOG_FFL, szFormat, arg1, arg2, arg3, arg4, arg5)
	#define WRNLOG6(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6)						LogInstance.DetailLog(LOG_WARN, LOG_FFL, szFormat, arg1, arg2, arg3, arg4, arg5, arg6)
	#define WRNLOG7(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7)				LogInstance.DetailLog(LOG_WARN, LOG_FFL, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
	#define WRNLOG8(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)			LogInstance.DetailLog(LOG_WARN, LOG_FFL, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)
	#define WRNLOG9(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)	LogInstance.DetailLog(LOG_WARN, LOG_FFL, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)
#else
	#define WRNLOG(x)	(void*)0
	#define WRNLOG0(LogInstance, szString)															(void*)0
	#define WRNLOG1(LogInstance, szFormat, arg1)													(void*)0
	#define WRNLOG2(LogInstance, szFormat, arg1, arg2)												(void*)0
	#define WRNLOG3(LogInstance, szFormat, arg1, arg2, arg3)										(void*)0
	#define WRNLOG4(LogInstance, szFormat, arg1, arg2, arg3, arg4)									(void*)0
	#define WRNLOG5(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5)							(void*)0
	#define WRNLOG6(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6)						(void*)0
	#define WRNLOG7(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7)				(void*)0
	#define WRNLOG8(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)			(void*)0
	#define WRNLOG9(LogInstance, szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)	(void*)0
#endif


#if defined(__linux__) || defined(i386) || defined(_WIN32)
	#define VA_START(Argument, pFormat) va_start(Argument, pFormat)
#else
	#define VA_START(Argument, pFormat) va_start(Argument)
#endif


#endif

#ifndef _ScriptEngine_H_
#define _ScriptEngine_H_

#include "BaseDef.h"

#define DllExport   __declspec( dllexport )

class CVirtualMachine;
typedef CVirtualMachine *	SCRIPT;
typedef void (*MESSAGE_FUNCTION)( const char * ErrMsg );
typedef void (*MESSAGE_FUNCTION2)( const char *, ... );
typedef char *  va_list;

////////////////////////////////////////////////////////////////////////////////
//

DllExport SCRIPT	_SE_Create( const char * codeFile );
DllExport void		_SE_Destroy( SCRIPT );
DllExport bool		_SE_Save( SCRIPT, const char * );
DllExport void		_SE_Execute( SCRIPT );

DllExport void		_SE_RegisterFunction( SCRIPT, ANY_FUNCTION, eDataType, const char * FuncName, ... );
DllExport SE_FUNC	_SE_GetScriptFunction( SCRIPT, eDataType, const char * FuncName, ... );
DllExport void		_SE_SetMessageFunction( MESSAGE_FUNCTION );

DllExport AnyData	_SE_CallScriptFunction( SCRIPT, SE_FUNC );
DllExport AnyData	_SE_CallScriptFunction( SCRIPT, SE_FUNC, AnyData );
DllExport AnyData	_SE_CallScriptFunction( SCRIPT, SE_FUNC, AnyData, AnyData );
DllExport AnyData	_SE_CallScriptFunction( SCRIPT, SE_FUNC, AnyData, AnyData, AnyData );
DllExport AnyData	_SE_CallScriptFunction( SCRIPT, SE_FUNC, AnyData, AnyData, AnyData, AnyData );
DllExport AnyData	_SE_CallScriptFunction( SCRIPT, SE_FUNC, AnyData args[], int nArgs );

////////////////////////////////////////////////////////////////////////////////

#endif
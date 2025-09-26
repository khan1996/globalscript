#ifndef DATATYPES_H
#define DATATYPES_H
#pragma once
/***************************************************************\

    ^^^^^^^^  ^^^^^^^^  ^^^^^^^^  ^^^^^^^^
    ^^    ^^  ^^    ^^     ^^     ^^    ^^
    ^^        ^^^^^^^^     ^^     ^^^^^^^^
    ^^   ^^^  ^^    ^^     ^^     ^^    ^^
    ^^    ^^  ^^    ^^     ^^     ^^    ^^
    ^^^^^^^^  ^^    ^^  ^^^^^^^^  ^^    ^^

    sample code from the book...
    Real Time 3D Terrain Engines Using C++ and DirectX

    by Greg Snook
    greg@mightystudios.com

\***************************************************************/


//	Name Space declaration
namespace gaia
{
#pragma warning(disable : 4786)

/*	DataTypes
-----------------------------------------------------------------

	The basic data types and ranges used by Gaia

-----------------------------------------------------------------
*/

typedef signed long     int32;
typedef signed short    int16;
typedef signed char     int8;
typedef unsigned long   uint32;
typedef unsigned short  uint16;
typedef unsigned char   uint8;

typedef __int64   int64;
typedef unsigned __int64  uint64;

extern const int32	MIN_INT32;
extern const int32	MAX_INT32;
extern const int16	MIN_INT16;
extern const int16	MAX_INT16;
extern const int8	MIN_INT8;
extern const int8	MAX_INT8;
extern const uint32	MIN_UINT32;
extern const uint32	MAX_UINT32;
extern const uint16	MIN_UINT16;
extern const uint16	MAX_UINT16;
extern const uint8	MIN_UINT8;
extern const uint8	MAX_UINT8;

extern const float	MIN_REAL32;
extern const float	MAX_REAL32;
extern const float	TINY_REAL32;

// unicode vs. mbcs character types
#ifdef _UNICODE
	// define tchar as a wide-character
	typedef wchar_t tchar;
	// create a macro to build string literals
	#define _text(a) L ## a
#else
	// define tchar as a plain char
	typedef char tchar;
	// stub the macro to build string literals
	#define _text
#endif


} //End NameSpace: gaia

#endif  // end of file      ( DataTypes.h )

//----------------------------------------------------------
//$Log: $




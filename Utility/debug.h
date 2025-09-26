#ifndef DEBUG_H
#define DEBUG_H
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
#ifndef DATATYPES_H
#include "data_types.h"
#endif

#ifndef BITFLAGS_H
#include "bit_flags.h"
#endif

#ifndef STRING_H
#include "string.h"
#endif

//	Name Space declaration
namespace gaia
{

//
// Undefine the common macros we wish to take over
//
#ifdef ASSERT
#undef ASSERT
#endif

#ifdef assert
#undef assert
#endif

#ifdef VERIFY
#undef VERIFY
#endif

#ifdef verify
#undef verify
#endif

#ifdef TRACE
#undef TRACE
#endif

// access to the cDebugMessageHandler singleton
#define Debug g_Debug

/*	Debug Output Macros
------------------------------------------------------------------------------------------
	
	
------------------------------------------------------------------------------------------
*/
#ifdef _DEBUG_OUTPUT
//
// DEBUG OUTPUT ENABLED
//
	#define debug_print Debug.output

	#define debug_assert(x, comment) {\
			static bool _ignoreAssert = false;\
			if (!_ignoreAssert && !(x)) \
			{\
				ERROR_RESULT _err_result = notifyAssertion(_text(#x), _text(comment), __FILE__, __LINE__);\
				if (_err_result == VR_IGNORE)\
				{\
					_ignoreAssert = true;\
				}\
				else if (_err_result == VR_BREAKPOINT)\
				{\
					_asm{int 3};\
				}\
			}}
		
	#define debug_error(x) {\
			static bool _ignoreError = false;\
			if (!_ignoreError) \
			{\
				ERROR_RESULT _err_result = notifyError((x), \
				__FILE__, __LINE__);\
				if (_err_result == VR_IGNORE)\
				{\
					_ignoreError = true;\
				}\
				else if (_err_result == VR_BREAKPOINT)\
				{\
					_asm{int 3};\
				}\
			}}	


#else

	// depending on the compiler version, 
	// we either set unused function definitions
	// to (0) or __noop
	#ifndef NOP_FUNCTION
	#if (_MSC_VER >= 1210)
	#define NOP_FUNCTION __noop
	#else
	#define NOP_FUNCTION (void)(0)
	#endif
	#endif

	//
	// DEBUG OUTPUT DISABLED
	//
	#define debug_print (void)NOP_FUNCTION 
	#define debug_assert(x, comment)
	#define debug_error(x)

#endif


#define compiler_assert(x) {\
    const int _value = (x) ? 1:0;\
    switch (x)\
    {\
        case 0: \
        case _value: \
        default: break;\
    };}


//
// Map our macros to the commonly used names
//
#define ASSERT(x) debug_assert(x,"")
#define assert(x) debug_assert(x,"")
#define error(x) debug_error(x)
#define TRACE debug_print

// Assert function return values
enum ERROR_RESULT
{
	VR_IGNORE = 0,
	VR_CONTINUE,
	VR_BREAKPOINT,
	VR_ABORT
};

extern ERROR_RESULT notifyError(uint32 errorCode, const tchar* fileName, int lineNumber);
extern ERROR_RESULT notifyAssertion(const tchar* condition, const tchar* description, const tchar* fileName, int lineNumber);

// Private Functions...
extern ERROR_RESULT displayError(	const tchar* errorTitle,
							const tchar* errorText,
							const tchar* errorDescription,
							const tchar* fileName, 
							int lineNumber);


/*	cDebugMessageHandler
-----------------------------------------------------------------
    
    cDebugMessageHandler is a singleton interface to handle the output of 
	text messages for debugging purposes. A Total of 32 channels
	are available for output. Channel Zero is reserved for standard
	debug output. All other channels can be routed to files, etc.
    
-----------------------------------------------------------------
*/
class cDebugMessageChannel;

class cDebugMessageHandler
{
public:

	// Data Types & Constants...
	enum
	{
		nMaxOutputStringSize = 2048
	};

	enum DEBUG_CHANNELS
	{
		nSystemChannelBit = 0,
		nMaxChannels = 32,
	};

	enum MESSAGE_FLAGS
	{
		k_defaultMessageFlag	 = FLAG(0),
	};


	// Public Data...

	// Creators...

    cDebugMessageHandler();
    ~cDebugMessageHandler();

	// Mutators...
	bool setChannel(int index, cDebugMessageChannel* pChannel);
	void clearChannel(int index);
	bool openChannel(int index);
	void closeChannel(int index);
	void setChannelMessageFilter(int index, u32Flags flags);
	void addChannelMessageFilter(int index, u32Flags flags);
	void removeChannelMessageFilter(int index, u32Flags flags);

	void output(const tchar* text, ...);
	void output(uint32 messageFlags, const tchar* text, ...);

	// Accessors...
	bool isChannelSet(int index)const;
	bool isChannelOpen(int index)const;
	u32Flags channelFilter(int index)const;

private:

	// Private Data...
	cDebugMessageChannel*		m_channel[nMaxChannels];
	u32Flags					m_openChannels;
};
extern cDebugMessageHandler g_Debug;

//- Inline Functions --------------------------------------------

//. Accessors ...................................................
inline bool cDebugMessageHandler::isChannelSet(int index)const
{
	debug_assert(index >= 0 && index<nMaxChannels, "invalid channel index");
	return m_channel[index] != 0;
}

inline bool cDebugMessageHandler::isChannelOpen(int index)const
{
	debug_assert(index >= 0 && index<nMaxChannels, "invalid channel index");
	return TEST_BIT(m_openChannels,index);
}

//- End of cDebugMessageHandler ------------------------------------------------


//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cDebugMessageHandler.h )

//----------------------------------------------------------
//$Log: $
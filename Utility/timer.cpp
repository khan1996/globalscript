#define TIMER_CPP
#define CORE_DLL
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

#include "timer.h"
#include "windows.h"
#include "GMMemory.h"

using namespace gaia;

uint32 cTimer::s_secondsFrequency = 0;
uint32 cTimer::s_millisecondsFrequency = 0;
float cTimer::s_invSecFrequency = 0.0f;

void cTimer::setupTimerFrequency()
{
	if (!s_secondsFrequency)
	{
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);

		s_secondsFrequency = frequency.LowPart;
		s_millisecondsFrequency = frequency.LowPart/1000;

		s_invSecFrequency = 1.0f/(float)s_secondsFrequency;
	}
}

uint32 cTimer::samplePerformanceCounter()
{
	LARGE_INTEGER sample;
	QueryPerformanceCounter(&sample);
	return sample.LowPart;
}

//***************************************************************
// end of file      ( Timer.cpp )

//----------------------------------------------------------
//$Log: $
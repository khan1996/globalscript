#ifndef TIMER_H
#define TIMER_H
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

#ifndef NUMERICTOOLS_H
#include "numeric_tools.h"
#endif

//	Name Space declaration
namespace gaia
{

/*	cTimer
-----------------------------------------------------------------
    
    A basic timer
    
-----------------------------------------------------------------
*/

class cTimer
{
public:

	// Data Types & Constants...
	enum TIMER_STATE
	{
		OFF = 0,
		ON,
		HOLD,
	};

	// Public Data...

	// Creators...

    cTimer();
    ~cTimer();

	// Operators...


	void start();	// start the timer
	void stop();	// stop the timer

	void suspend();	// pause the timer
	void resume();	// resume the timer from when is was suspended

	// Accessors...
	float elapsedTime();
	uint32 elapsedSeconds();
	uint32 elapsedMilliseconds();
	uint32 elapsedCount();

private:

	// Private Data...
	static uint32 s_secondsFrequency;
	static uint32 s_millisecondsFrequency;
	static float s_invSecFrequency;

	uint32			m_startTime;
	uint32			m_stopTime;
	uint32			m_timeDelta;
	uint32			m_elapsedCount;
	TIMER_STATE		m_state;

	// Private Functions...
	static void setupTimerFrequency();
	uint32 samplePerformanceCounter();

	// non-existant functions
    cTimer(const cTimer& Src);
    cTimer& operator=(const cTimer& Src);

};

//- Inline Functions --------------------------------------------

//. Creators ....................................................

/*	cTimer
-----------------------------------------------------------------
    
    Default Constructor
    
-----------------------------------------------------------------
*/
inline cTimer::cTimer()
:m_state(OFF)
,m_elapsedCount(0)
{
	memset(&m_startTime, 0, sizeof(m_startTime));
	memset(&m_stopTime, 0, sizeof(m_stopTime));
	memset(&m_timeDelta, 0, sizeof(m_timeDelta));
	setupTimerFrequency();
}

/*	~cTimer
-----------------------------------------------------------------
    
    Default Destructor
    
-----------------------------------------------------------------
*/
inline cTimer::~cTimer()
{
}


inline void cTimer::start()	// start the timer
{
	m_startTime = samplePerformanceCounter();
	m_elapsedCount = 0;
	m_state = ON;
}


inline void cTimer::stop()	// stop the timer
{
	m_elapsedCount = elapsedCount();
	m_state = OFF;
}


inline void cTimer::suspend()	// resume the timer from when is was stopped
{
	if (m_state == ON)
	{
		m_elapsedCount = elapsedCount();
		m_state = HOLD;
	}
}


inline void cTimer::resume()	// resume the timer from when is was stopped
{
	if (m_state == HOLD)
	{
		// get the current time
		m_startTime = samplePerformanceCounter();

		// roll the start time back by our previous delta
		m_startTime -= m_timeDelta;

		m_elapsedCount = 0;
		m_state = ON;
	}
}

//. Accessors ............................................................................

inline float cTimer::elapsedTime()
{
	if (m_state != ON)
	{
		return (float)m_elapsedCount*s_invSecFrequency;
	}
	else
	{
		m_stopTime = samplePerformanceCounter();
		m_timeDelta = m_stopTime - m_startTime;

		float ReportedTime = (float)m_timeDelta*s_invSecFrequency;

		return(ReportedTime);
	}
}

inline uint32 cTimer::elapsedSeconds()
{
	if (m_state != ON)
	{
		return m_elapsedCount/s_secondsFrequency;
	}
	else
	{
		m_stopTime = samplePerformanceCounter();
		m_timeDelta = m_stopTime - m_startTime;

		uint32 ReportedTime = m_timeDelta/s_secondsFrequency;

		return(ReportedTime);
	}
}

inline uint32 cTimer::elapsedMilliseconds()
{
	if (m_state != ON)
	{
		return m_elapsedCount/s_millisecondsFrequency;
	}
	else
	{
		m_stopTime = samplePerformanceCounter();
		m_timeDelta = m_stopTime - m_startTime;

		uint32 ReportedTime = m_timeDelta/s_millisecondsFrequency;

		return(ReportedTime);
	}
}

inline uint32 cTimer::elapsedCount()
{
	if (m_state != ON)
	{
		return m_elapsedCount;
	}
	else
	{
		m_stopTime = samplePerformanceCounter();
		m_timeDelta = m_stopTime - m_startTime;

		uint32 ReportedTime = m_timeDelta;

		return(ReportedTime);
	}
}

//- End of cTimer ------------------------------------------------

//***************************************************************

} //End NameSpace: gaia

#endif  // end of file      ( cTimer.h )

//----------------------------------------------------------
//$Log: $
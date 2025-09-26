#include "stdafx.h"
#include "Random.h"

namespace Math
{

	RandomInt::RandomInt() 
	{
		SetSeed(GetTickCount());
	}

	RandomInt::RandomInt(unsigned long seed) : random(seed) 
	{
	}

	void RandomInt::SetSeed(unsigned long seed)
	{
		random.SetSeed(seed);
	}

	unsigned int RandomInt::Next() 
	{ 
		return random.Next(); 
	}

	unsigned int RandomInt::Next(unsigned int excludedMax) 
	{ 
		if(excludedMax == 0)
			return excludedMax;

		return Next(0, excludedMax); 
	}

	unsigned int RandomInt::Next(unsigned int includedMin, unsigned int excludedMax) 
	{ 
		if(includedMin == excludedMax)
			return includedMin;

		return random.Next(includedMin, excludedMax); 
	}


	RandomDouble::RandomDouble() 
	{
		SetSeed(GetTickCount());
	}

	RandomDouble::RandomDouble(unsigned long seed) : random(seed) 
	{
	}

	void RandomDouble::SetSeed(unsigned long seed)
	{
		random.SetSeed(seed);
	}

	double RandomDouble::Next() 
	{ 
		return random.Next(); 
	}


} 
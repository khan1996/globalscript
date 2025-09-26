#pragma once

#include "PseudoRandom.h"

namespace Math
{
	class RandomInt
	{
	private:
		PseudoRandomInt random;
		
		// unable to copy
		RandomInt(const RandomInt& );
		void operator =(const RandomInt&);

	public:
		RandomInt();
		RandomInt(unsigned long seed);

		void SetSeed(unsigned long seed);

		virtual unsigned int Next();
		virtual unsigned int Next(unsigned int excludedMax);
		virtual unsigned int Next(unsigned int includedMin, unsigned int excludedMax);
	};

	class RandomDouble
	{
	private:
		PseudoRandomFloatClosed random;

		// unable to copy
		RandomDouble(const RandomDouble& );
		void operator =(const RandomDouble&);

	public:
		RandomDouble();
		RandomDouble(unsigned long seed);

		void SetSeed(unsigned long seed);

		virtual double Next(); // generates a float number [0, 1]
	};

} 
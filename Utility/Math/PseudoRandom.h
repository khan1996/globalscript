#pragma once

namespace Math
{
	/**
	The Mersenne Twister random number generator

	usage:

	PseudoRandomInt foo;  //initializes random sequence by the current time
	PseudoRandomInt foo(false); // the default seed 5489UL is used

	foo.SetSeed(230194019);

	foo.Next()
	foo.Next(10,4300); //returns unsigned long between 10 and 4300

	PseudoRandomFloat foo;
	foo.Next(); // return double between [0..1)
	foo.Next(245); // return double between [0..245)

	PseudoRandomFloatClosed foo;
	foo.Next(); // return double between [0..1]

	PseudoRandomFloatOpen foo;
	foo.Next(); // return double between (0..1)

	
	details:

	The Mersenne twister is a pseudorandom number generator that was developed in 1997 by Makoto Matsumoto 
	(áæÜâ òØ) and Takuji Nishimura (à¤õ½ ô¬ÞÍ). It provides for fast generation of very high quality random 
	numbers - having been designed specifically to rectify many of the flaws found in older algorithms.

    It has the following desirable properties:

	It was designed to have a colossal period of 219937-1 (the creators of the algorithm proved this property). 
	It has a very high order of dimensional equidistribution (see linear congruential generator). Note that this means, by default, that there is negligible serial correlation between successive values in the output sequence. 
	It is faster than all but the most statistically unsound generators. 
	It is statistically random in all the bits of its output. 

	This implementation is based C++ code by Takuji Nishimura and Makoto Matsumoto, Jasper Bedaux, Isaku Wada

	*/
	
	class MersenneTwister { 
	public:
		// default constructor: uses default seed only if this is the first instance
		MersenneTwister(BOOL useCurrentTime=TRUE);
		
		// constructor with 32 bit int as seed
		MersenneTwister(unsigned long s) { SetSeed(s); }
		// constructor with array of size 32 bit ints as seed
		MersenneTwister(const unsigned long* array, int size) { SetSeed(array, size); }
		// the two seed functions
		void SetSeed(unsigned long); // seed with 32 bit integer
		void SetSeed(const unsigned long*, int size); // seed with array
		// overload operator() to make this a generator (functor)
		//unsigned long operator()() { return GenerateRandom(); }

		//unsigned long Next() { return (*this)(); }
		~MersenneTwister() {} // destructor

	protected: // used by derived classes, otherwise not accessible; use the ()-operator
			unsigned long GenerateRandom() { // generate 32 bit random integer
			if (position == Constant1) GenerateState(); // new state vector needed
			// GenerateState() is split off to be non-inline, because it is only called once
			// in every 624 calls and otherwise irand() would become too big to get inlined
			unsigned long x = state[position++];
			x ^= (x >> 11);
			x ^= (x << 7) & 0x9D2C5680UL;
			x ^= (x << 15) & 0xEFC60000UL;
			return x ^ (x >> 18);
		}

	private:
		enum { Constant1 = 624, Constant2 = 397 };
		
		unsigned long state[Constant1]; // state vector array
		int position; // position in state array
		
		// private functions used to generate the pseudo random numbers
			unsigned long Twiddle(unsigned long u, unsigned long v) {
			return (((u & 0x80000000UL) | (v & 0x7FFFFFFFUL)) >> 1)	^ ((v & 1UL) ? 0x9908B0DFUL : 0x0UL);
		} // used by GenerateState()
		void GenerateState(); // generate new state
		// make copy constructor and assignment operator unavailable, they don't make sense
		MersenneTwister(const MersenneTwister&); // copy constructor not defined
		void operator=(const MersenneTwister&); // assignment operator not defined
	};

	class PseudoRandomInt : public MersenneTwister {
	public:
		PseudoRandomInt(BOOL useCurrentTime=TRUE): MersenneTwister(useCurrentTime) {}
		PseudoRandomInt(unsigned long seed) : MersenneTwister(seed) {}
		PseudoRandomInt(const unsigned long* seed, int size) : MersenneTwister(seed, size) {}

			unsigned long operator()() { return GenerateRandom(); }

			unsigned long Next() { return (*this)(); }
			unsigned long Next(unsigned long minval, unsigned long maxval) {
			return minval+(Next()%(maxval-minval));
		}
	
	};
	// generates double floating point numbers in the half-open interval [0, 1)
	class PseudoRandomFloat : public MersenneTwister {
	public:
		PseudoRandomFloat(BOOL useCurrentTime=TRUE) : MersenneTwister(useCurrentTime) {}
		PseudoRandomFloat(unsigned long seed) : MersenneTwister(seed) {}
		PseudoRandomFloat(const unsigned long* seed, int size) : MersenneTwister(seed, size) {}
		~PseudoRandomFloat() {}
		double operator()() {
			return static_cast<double>(GenerateRandom()) * (1. / 4294967296.); } // divided by 2^32
		double Next() { return (*this)(); }
		double Next(double mul) { return Next()*mul; }
	private:
		PseudoRandomFloat(const PseudoRandomFloat&); // copy constructor not defined
		void operator=(const PseudoRandomFloat&); // assignment operator not defined
	};

	// generates double floating point numbers in the closed interval [0, 1]
	class PseudoRandomFloatClosed : public MersenneTwister {
	public:
		PseudoRandomFloatClosed(BOOL useCurrentTime=TRUE) : MersenneTwister(useCurrentTime) {}
		PseudoRandomFloatClosed(unsigned long seed) : MersenneTwister(seed) {}
		PseudoRandomFloatClosed(const unsigned long* seed, int size) : MersenneTwister(seed, size) {}
		~PseudoRandomFloatClosed() {}
		double operator()() {
			return static_cast<double>(GenerateRandom()) * (1. / 4294967295.); } // divided by 2^32 - 1
		double Next() { return (*this)(); }
		double Next(double mul) { return Next()*mul; }
	private:
		PseudoRandomFloatClosed(const PseudoRandomFloatClosed&); // copy constructor not defined
		void operator=(const PseudoRandomFloatClosed&); // assignment operator not defined
	};

	// generates double floating point numbers in the open interval (0, 1)
	class PseudoRandomFloatOpen : public MersenneTwister {
	public:
		PseudoRandomFloatOpen(BOOL useCurrentTime=TRUE) : MersenneTwister(useCurrentTime) {}
		PseudoRandomFloatOpen(unsigned long seed) : MersenneTwister(seed) {}
		PseudoRandomFloatOpen(const unsigned long* seed, int size) : MersenneTwister(seed, size) {}
		~PseudoRandomFloatOpen() {}
		double operator()() {
			return (static_cast<double>(GenerateRandom()) + .5) * (1. / 4294967296.); } // divided by 2^32
		double Next() { return (*this)(); }
		double Next(double mul) { return Next()*mul; }
	private:
		PseudoRandomFloatOpen(const PseudoRandomFloatOpen&); // copy constructor not defined
		void operator=(const PseudoRandomFloatOpen&); // assignment operator not defined
	};

	// generates 53 bit resolution doubles in the half-open interval [0, 1)
	class PseudoRandomFloat53Bit : public MersenneTwister {
	public:
		PseudoRandomFloat53Bit(BOOL useCurrentTime=TRUE) : MersenneTwister(useCurrentTime) {}
		PseudoRandomFloat53Bit(unsigned long seed) : MersenneTwister(seed) {}
		PseudoRandomFloat53Bit(const unsigned long* seed, int size) : MersenneTwister(seed, size) {}
		~PseudoRandomFloat53Bit() {}
		double operator()() {
			return (static_cast<double>(GenerateRandom() >> 5) * 67108864. + 
				static_cast<double>(GenerateRandom() >> 6)) * (1. / 9007199254740992.); }
		double Next() { return (*this)(); }
		double Next(double mul) { return Next()*mul; }
	private:
		PseudoRandomFloat53Bit(const PseudoRandomFloat53Bit&); // copy constructor not defined
		void operator=(const PseudoRandomFloat53Bit&); // assignment operator not defined
	};

//		typedef MersenneTwister PseudoRandom;


//	Pseudorandom series(21301391);


	
	

	// mtrand.h
	// C++ include file for MT19937, with initialization improved 2002/1/26.
	// Coded by Takuji Nishimura and Makoto Matsumoto.
	// Ported to C++ by Jasper Bedaux 2003/1/1 (see http://www.bedaux.net/mtrand/).
	// The generators returning floating point numbers are based on
	// a version by Isaku Wada, 2002/01/09
	//
	// Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
	// All rights reserved.
	//
	// Redistribution and use in source and binary forms, with or without
	// modification, are permitted provided that the following conditions
	// are met:
	//
	// 1. Redistributions of source code must retain the above copyright
	//    notice, this list of conditions and the following disclaimer.
	//
	// 2. Redistributions in binary form must reproduce the above copyright
	//    notice, this list of conditions and the following disclaimer in the
	//    documentation and/or other materials provided with the distribution.
	//
	// 3. The names of its contributors may not be used to endorse or promote
	//    products derived from this software without specific prior written
	//    permission.
	//
	// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
	// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
	// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
	// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
	// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
	// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
	// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
	// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	//
	// Any feedback is very welcome.
	// http://www.math.keio.ac.jp/matumoto/emt.html
	// email: matumoto@math.keio.ac.jp
	//
	// Feedback about the C++ port should be sent to Jasper Bedaux,
	// see http://www.bedaux.net/mtrand/ for e-mail address and info.

} 
//-----------------------------------------------------------------------------
// MIT License
// 
// Copyright (c) 2017 Jeff Hutchinson
// Copyright (c) 2017 Tim Barnes
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//-----------------------------------------------------------------------------

#ifndef _CORE_NOISE_HPP_
#define _CORE_NOISE_HPP_

#include <string>
#include <cstdio>
#include <open-simplex-noise.h>
#include "core/singleton.hpp"

/// @class Noise
/// A singleton that represents the noise generation engine.
/// The noise generator is using the *Open Simplex Noise* algorithm.
/// Any function that is marked threadsafe can be used by any thread,
/// but a word of caution: Any function that isn't thread safe should
/// only be called once NO threads other than the main thread are
/// touching the Noise singleton (either at the beginning of world generation
/// or in deallocation of the program), or all hell can break loose.
/// @notice You have been warned.
class Noise : public Singleton<Noise>
{
	friend class Singleton<Noise>;
protected:
	Noise();
	~Noise();
	Noise(const Noise&) = delete;
	Noise(const Noise&&) = delete;

public:
	/// Sets the seed for the noise generator. If a seed is already
	/// set it kills the current seed. Only call this before generating
	/// a world. Once the noise library can be used for threading, resetting this
	/// can literally cause all hell to break loose. I'd much rather have it this
	/// way instead of having to put mutex's around the noise library when it
	/// is already mathematically intensive.
	/// @param seed A 64bit integer representing the seed of the generator.
	///  Note that this seed will be used for any noise dimention.
	/// @threadsafety Not thread safe, call from main thread.
	/// @notice You have been warned.
	void setSeed(const int64_t seed);

	/// Noise2 permuatation based upon the seed set and the x/y provided.
	/// @param x The x coordinate for the permutation.
	/// @param y The y coordinate for the permutation.
	/// @return double The height of the permutation calculation.
	/// @threadsafety Thread safe, can call from any thread.
	double noise2(double x, double y);

private:
	/// The 64bit integer representing the seed of the generator.
	int64_t mSeed;

	/// The context of the noise algorithm based on the seed.
	osn_context *mContext;
};

#endif
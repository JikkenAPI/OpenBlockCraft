//-----------------------------------------------------------------------------
// OpenBlockCraft -- A 3D block based game
// Copyright(C) 2017 Jeff Hutchinson
//
// This program is free software : you can redistribute it and / or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>
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
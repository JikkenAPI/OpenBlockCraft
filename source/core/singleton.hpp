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

#ifndef _CORE_SINGLETON_HPP_
#define _CORE_SINGLETON_HPP_

#include <cassert>

/// A base class that wraps a Singleton for a class that is to only be 
/// defined once. A singleton is more useful than a static class because it
/// can have virtual inheritence. However, if no virtual inheritence is needed,
/// it is probably better to use a static class in terms of performance.
///
/// Usage for singleton pattern:
///
/// class A : public Singleton<A>
/// {
///    friend class Singleton<A>;
/// protected:
///    A();
///    ~A();
///    A(const A&) = delete;
///    A(const A&&) = delete;
/// }
template<class T>
class Singleton
{
public:
	/// Gets the singleton object. If it does not exist, it creates it.
	/// @return The singleton of type T.
	static T* get()
	{
		if (sSingleton == nullptr)
			sSingleton = new T();
		return sSingleton;
	}

	/// Destroys the singleton object, followed by setting the singleton to
	/// nullptr in case it wants to be created at another time again.
	static void destroy()
	{
		assert(sSingleton);
		delete sSingleton;
		sSingleton = nullptr;
	}

protected:
	/// The pointer to the singleton object.
	static T* sSingleton;
};

/// Initializes the singleton object pointer to nullptr so it can be checked
/// when it comes time to create the singleton.
/// @note: Must initialize out of the class definition.
/// #see: http://stackoverflow.com/a/3531494
template<class T> T* Singleton<T>::sSingleton = nullptr;

#endif
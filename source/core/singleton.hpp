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
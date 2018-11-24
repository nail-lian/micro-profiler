//	Copyright (c) 2011-2018 by Artem A. Gevorkyan (gevorkyan.org)
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.

#pragma once

#include "primitives.h"

namespace micro_profiler
{
	timestamp_t ticks_per_second();
	unsigned int current_thread_id();

	class mutex
	{
	public:
		mutex();
		~mutex();

		void lock();
		void unlock();

	private:
		mutex(const mutex &);
		mutex &operator =(const mutex &);

	private:
		char _mtx_buffer[6 * sizeof(void*)];
	};

	template <typename MutexT>
	class lock_guard
	{
	public:
		explicit lock_guard(MutexT &mtx) throw();
		~lock_guard() throw();

	private:
		lock_guard(const lock_guard &other);
		const lock_guard &operator =(const lock_guard &rhs);

	private:
		MutexT &_mutex;
	};



	template <typename MutexT>
	inline lock_guard<MutexT>::lock_guard(MutexT &mtx) throw()
		: _mutex(mtx)
	{	_mutex.lock();	}

	template <typename MutexT>
	inline lock_guard<MutexT>::~lock_guard() throw()
	{	_mutex.unlock();	}


	template <typename T, size_t ObjectSize>
	struct atomic
	{
	};

	template <typename T>
	struct atomic<T, 4>
	{
		static T compare_exchange(volatile T &destination, T new_value, T comparand);
	};

	template <typename T>
	struct atomic<T, 8>
	{
		static T compare_exchange(volatile T &destination, T new_value, T comparand);
	};


	template <typename T>
	inline T atomic_compare_exchange(volatile T &destination, T new_value, T comparand)
	{  return atomic<T, sizeof(T)>::compare_exchange(destination, new_value, comparand);   }

	template <typename T>
	inline void atomic_store(volatile T& destination, T value)
	{  destination = value; }


   long interlocked_compare_exchange(long volatile *destination, long exchange, long comperand);
   long long interlocked_compare_exchange64(long long volatile *destination, long long exchange, long long comperand);

	template <typename T>
	inline T atomic<T, 4>::compare_exchange(volatile T &destination, T new_value, T comparand)
	{
		return reinterpret_cast<T>(interlocked_compare_exchange(reinterpret_cast<volatile long *>(&destination),
			reinterpret_cast<long>(new_value), reinterpret_cast<long>(comparand)));
	}

	template <typename T>
	inline T atomic<T, 8>::compare_exchange(volatile T &destination, T new_value, T comparand)
	{
		return reinterpret_cast<T>(interlocked_compare_exchange64(reinterpret_cast<volatile long long *>(&destination),
			reinterpret_cast<long long>(new_value), reinterpret_cast<long long>(comparand)));
	}
}

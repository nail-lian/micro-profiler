#pragma once

#include "basics.h"

#include <functional>

namespace mt
{
	struct waitable : destructible
	{
		static const unsigned int infinite = -1;

		virtual void wait(unsigned int timeout) = 0;
	};

	class thread : noncopyable
	{
		void *_thread;

	public:
		thread(const std::function<void()> &job);
		virtual ~thread() throw();
	};
}

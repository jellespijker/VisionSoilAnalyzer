#pragma once

#include <exception>
#include <string>

using namespace std;

namespace Hardware
{
	namespace Exception
	{
		class FailedToCreateGPIOPollingThreadException : public std::exception
		{
		public:
			FailedToCreateGPIOPollingThreadException(string m = "Failed to create GPIO polling thread!") : msg(m) {};
			~FailedToCreateGPIOPollingThreadException()  _GLIBCXX_USE_NOEXCEPT{};
			const char* what() const _GLIBCXX_USE_NOEXCEPT{ return msg.c_str(); };
		private:
			string msg;
		};
	}
}
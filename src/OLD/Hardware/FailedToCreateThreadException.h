#pragma once

#include <exception>
#include <string>

using namespace std;

namespace Hardware
{
	namespace Exception
	{
		class FailedToCreateThreadException : public std::exception
		{
		public:
			FailedToCreateThreadException(string m = "Couldn't create the thread!") : msg(m) {};
			~FailedToCreateThreadException()  _GLIBCXX_USE_NOEXCEPT{};
			const char* what() const _GLIBCXX_USE_NOEXCEPT{ return msg.c_str(); };
		private:
			string msg;
		};
	}
}
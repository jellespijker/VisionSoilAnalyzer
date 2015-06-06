#pragma once

#include <exception>
#include <string>

using namespace std;

namespace Hardware
{
	namespace Exception
	{
		class GPIOReadException : public std::exception
		{
		public:
			GPIOReadException(string m = "Can't read GPIO data!") : msg(m) {};
			~GPIOReadException()  _GLIBCXX_USE_NOEXCEPT{};
			const char* what() const _GLIBCXX_USE_NOEXCEPT{ return msg.c_str(); };
		private:
			string msg;
		};
	}
}
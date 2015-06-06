#pragma once

#include <exception>
#include <string>

using namespace std;

namespace Hardware
{
	namespace Exception
	{
		class ValueOutOfBoundsException : public std::exception
		{
		public:
			ValueOutOfBoundsException(string m = "Value out of bounds!") : msg(m) {};
			~ValueOutOfBoundsException()  _GLIBCXX_USE_NOEXCEPT{};
			const char* what() const _GLIBCXX_USE_NOEXCEPT{ return msg.c_str(); };
		private:
			string msg;
		};
	}
}
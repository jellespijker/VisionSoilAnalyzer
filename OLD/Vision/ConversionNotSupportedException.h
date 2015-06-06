/*! \class ConversionNotSupportedException
Exception class which is thrown when an illegal conversion is requested.
*/
#pragma once

#include <exception>
#include <string>

using namespace std;

namespace Vision
{
	namespace Exception
	{
		class ConversionNotSupportedException : public std::exception
		{
		public:
			ConversionNotSupportedException(string m = "Requested conversion is not supported!") : msg(m) {};
			~ConversionNotSupportedException()  _GLIBCXX_USE_NOEXCEPT{};
			const char* what() const _GLIBCXX_USE_NOEXCEPT{ return msg.c_str(); };
		private:
			string msg;
		};
	}
}
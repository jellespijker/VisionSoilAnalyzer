/*! \class WrongKernelSizeException
Exception class which is thrown when a wrong kernelsize is requested
*/
#pragma once

#include <exception>
#include <string>

using namespace std;

namespace Vision
{
	namespace Exception
	{
		class WrongKernelSizeException : public std::exception
		{
		public:
			WrongKernelSizeException(string m = "Wrong kernel dimensions!") : msg(m) {};
			~WrongKernelSizeException()  _GLIBCXX_USE_NOEXCEPT{};
			const char* what() const _GLIBCXX_USE_NOEXCEPT{ return msg.c_str(); };
		private:
			string msg;
		};
	}
}
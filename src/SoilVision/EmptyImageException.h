/*! \class EmtpyImageException
Exception class which is thrown when operations are about to start on a empty image.
*/

#pragma once

#include <exception>
#include <string>

using namespace std;

namespace Vision
{
	namespace Exception
	{
		class EmtpyImageException : public std::exception
		{
		public:
			EmtpyImageException(string m = "Empty Image!") : msg(m) {};
			~EmtpyImageException()  _GLIBCXX_USE_NOEXCEPT{};
			const char* what() const _GLIBCXX_USE_NOEXCEPT{ return msg.c_str(); };
		private:
			string msg;
		};
	}
}
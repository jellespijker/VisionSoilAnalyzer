#pragma once
#include <exception>
#include <string>

using namespace std;

namespace Hardware
{
	namespace Exception
	{
		class CouldNotGrabImageException : public std::exception
		{
		public:
			CouldNotGrabImageException(string m = "Unable to grab the next image!") : msg(m) {};
			~CouldNotGrabImageException()  _GLIBCXX_USE_NOEXCEPT{};
			const char* what() const _GLIBCXX_USE_NOEXCEPT{ return msg.c_str(); };
		private:
			string msg;
		};
	}
}
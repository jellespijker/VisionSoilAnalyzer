#pragma once
#include <exception>
#include <string>

using namespace std;

namespace Hardware
{
	namespace Exception
	{
		class MicroscopeNotFoundException : public std::exception
		{
		public:
			MicroscopeNotFoundException(string m = "Microscope not found exception!") : msg(m) {};
			~MicroscopeNotFoundException()  _GLIBCXX_USE_NOEXCEPT {};
			const char* what() const _GLIBCXX_USE_NOEXCEPT{ return msg.c_str(); };
		private:
			string msg;
		};
	}
}


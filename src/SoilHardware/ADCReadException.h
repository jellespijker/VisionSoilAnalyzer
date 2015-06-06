#pragma once
#include <exception>
#include <string>

using namespace std;

namespace Hardware
{
	namespace Exception
	{
		class ADCReadException : public std::exception
		{
		public:
			ADCReadException(string m = "Can't read ADC data!") : msg(m) {};
			~ADCReadException()  _GLIBCXX_USE_NOEXCEPT{};
			const char* what() const _GLIBCXX_USE_NOEXCEPT{ return msg.c_str(); };
		private:
			string msg;
		};
	}
}
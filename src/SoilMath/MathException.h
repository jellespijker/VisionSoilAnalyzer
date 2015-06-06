#pragma once
#include <exception>
#include <string>

using namespace std;

namespace SoilMath {
namespace Exception {
class MathException : public std::exception {
public:
  MathException(string m = "Math Exception!") : msg(m){};
  ~MathException() _GLIBCXX_USE_NOEXCEPT{};
  const char *what() const _GLIBCXX_USE_NOEXCEPT { return msg.c_str(); };

private:
  string msg;
};
}
}

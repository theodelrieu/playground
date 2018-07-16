#pragma once

#include <stdexcept>

namespace mgs
{
inline namespace v1
{
class exception : public std::runtime_error
{
public:
  using std::runtime_error::runtime_error;
};
}
}

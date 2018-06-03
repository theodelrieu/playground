#pragma once

#include <stdexcept>

namespace mgs
{
class exception : std::runtime_error
{
public:
  using std::runtime_error::runtime_error;
};
}

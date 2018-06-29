#pragma once

#include <stdexcept>

namespace mgs
{
class exception : public std::runtime_error
{
public:
  using std::runtime_error::runtime_error;
};
}

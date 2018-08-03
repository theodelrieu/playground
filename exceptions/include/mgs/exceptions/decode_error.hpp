#pragma once

#include <string>

#include <mgs/exceptions/exception.hpp>

namespace mgs
{
inline namespace v1
{
namespace exceptions
{
class decode_error : public exception
{
public:
  decode_error(char const* encoding_name, std::string const& msg)
    : exception(encoding_name + std::string{": "} + msg)
  {
  }
};
}
}
}
